#include "efvi_receive_base.h"

bool udp_quote_base::init() {
	m_res = (resources*)calloc(1, sizeof(struct resources));
	if (!m_res)
		exit(1);

	/* Open driver and allocate a VI. */
	TRY(ef_driver_open(&m_res->dh));

	if(m_udp_param_list.empty()) {
		std::cout << "IP/Port is not selected!" << std::endl;
		return false;
	}

	TRY(ef_pd_alloc_by_name(&m_res->pd, m_res->dh, m_udp_param_list[0].m_eth_name.c_str(), EF_PD_DEFAULT));

	// TODO: 2048 -> 4096 ?
	TRY(ef_vi_alloc_from_pd(&m_res->vi, m_res->dh, &m_res->pd, m_res->dh
							, -1			// The capacity of the event queue
							, 2048			// The number of slots in the RX descriptor ring
							, 0				// 发送队列，不用管，txq_capacity, TODO: -1 ？
							, NULL			// event queue 备胎，不用管，event queue to use if evq_capacity=0.
							, -1			// 和 event queue 备胎相关的 ef_driver_handle ，不用管
							, EF_VI_FLAGS_DEFAULT));

	/* Allocate memory for DMA transfers. Try mmap() with MAP_HUGETLB to get huge
	* pages. If that fails, fall back to posix_memalign() and hope that we do
	* get them. */
	// TODO: ef_vi_alloc_from_pd 中的 2048 应该要一样吧？
	m_res->pkt_bufs_n = ef_vi_receive_capacity(&m_res->vi);
	std::cout << "rx slots size: " << m_res->pkt_bufs_n << std::endl;
	// PKT_BUF_SIZE 是一个 buf 的大小
	size_t alloc_size = m_res->pkt_bufs_n * PKT_BUF_SIZE;	// TODO: 是不是还要乘以 1024？
	alloc_size = ROUND_UP(alloc_size, huge_page_size);
	std::cout << "alloc_size: " << alloc_size << ", before is: " << m_res->pkt_bufs_n * PKT_BUF_SIZE << std::endl;
	m_res->pkt_bufs = mmap(NULL, alloc_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE | MAP_HUGETLB, -1, 0);
	if (m_res->pkt_bufs == MAP_FAILED) {
		/* Allocate huge-page-aligned memory to give best chance of allocating
		* transparent huge-pages.
		*/
		TEST(posix_memalign(&m_res->pkt_bufs, huge_page_size, alloc_size) == 0);
	}

	/* Register the memory so that the adapter can access it. */
	TRY(ef_memreg_alloc(&m_res->memreg, m_res->dh, &m_res->pd, m_res->dh, m_res->pkt_bufs, alloc_size));


	// 上面已经把内存分配好了，可以用了

	m_res->rx_prefix_len = ef_vi_receive_prefix_len(&m_res->vi);
	int i = 0;
	for (i = 0; i < m_res->pkt_bufs_n; ++i) {
		// pkt_buf 是 slot 的地址，RX_DMA_OFF 是人为的定义，为了存放用户自定义的数据，在 ef_vi_receive_init 的参数加 RX_DMA_OFF
		// rx_prefix_len 这个偏移是为了存放 NIC 的元数据，api 设计定义的
		struct pkt_buf* pkt_buf = pkt_buf_from_id(m_res, i);
		pkt_buf->rx_ptr = (char*)pkt_buf + RX_DMA_OFF + m_res->rx_prefix_len;
		pkt_buf->id = i;
		pkt_buf_free(m_res, pkt_buf);
	}

	for (i = 0; i < m_res->pkt_bufs_n; ++i) {
		struct pkt_buf* pkt_buf = pkt_buf_from_id(m_res, i);
		pkt_buf->m_ef_addr = ef_memreg_dma_addr(&m_res->memreg, i * PKT_BUF_SIZE);
	}

	/* Fill the RX ring. */
	while (ef_vi_receive_space(&m_res->vi) > REFILL_BATCH_SIZE)
		refill_rx_ring(m_res);

	/* Add filters so that adapter will send packets to this VI. */
	for(auto& udp_param : m_udp_param_list) {
		ef_filter_spec guava_80;
		ef_filter_spec_init(&guava_80, EF_FILTER_FLAG_NONE);
		if (sl_parse(&guava_80, udp_param.m_multicast_ip.c_str(), udp_param.m_multicast_port) != 0) {
			std::cout << "Add EFVI Filter Error!" << std::endl;
			exit(1);
		}
		TRY(ef_vi_filter_add(&m_res->vi, m_res->dh, &guava_80, NULL));
	}
	
	// 开始接收线程
	m_recv_thread = (new std::thread(&udp_quote_base::work_thread, this));
	return true;
}

void udp_quote_base::close() {
	m_receive_quit_flag = true;
	if (m_recv_thread != nullptr && m_recv_thread->joinable()) {
		m_recv_thread->join();
		m_recv_thread = nullptr;
	}
}

int udp_quote_base::sl_parse(ef_filter_spec* fs, const char* ip, unsigned short port) {
	sockaddr_in host_addr;
	bzero(&host_addr, sizeof(host_addr));
	host_addr.sin_family = AF_INET;
	inet_aton(ip, &(host_addr.sin_addr));
	host_addr.sin_port = htons(port);

	TRY(ef_filter_spec_set_ip4_local(fs, IPPROTO_UDP, host_addr.sin_addr.s_addr, host_addr.sin_port));

	return 0;
}

// 返回下一个 slot 的内存地址，每个 slot 的大小是 PKT_BUF_SIZE
struct pkt_buf* udp_quote_base::pkt_buf_from_id(struct resources* res, int pkt_buf_i) {
	assert((unsigned)pkt_buf_i < (unsigned)res->pkt_bufs_n);
	return (pkt_buf*)((char*)res->pkt_bufs + (size_t)pkt_buf_i * PKT_BUF_SIZE);
}

// 将 pkt_buf 挂到空闲 slot 数组
void udp_quote_base::pkt_buf_free(struct resources* res, struct pkt_buf* pkt_buf) {
	pkt_buf->next = res->free_pkt_bufs;
	res->free_pkt_bufs = pkt_buf;
	++(res->free_pkt_bufs_n);
}

void udp_quote_base::handle_rx(struct resources* res, int pkt_buf_i, int len) {
	struct pkt_buf* pkt_buf = pkt_buf_from_id(res, pkt_buf_i);
	const char* ptr = (const char*)(pkt_buf->rx_ptr);

	/***************网络处理***************/
	// 网络包头
	ip_head* p_ip = (ip_head*)(ptr + sizeof(mac_head));
	// ip头长度
	int ip_head_total_len = p_ip->m_version.m_head_len * 4;
	udp_head* p_udp = (udp_head*)(ptr + sizeof(mac_head) + ip_head_total_len);
	// udp数据长度
	int udp_len = ntohs(p_udp->m_udp_len) - sizeof(udp_head);
	int net_header_len = ip_head_total_len + (int)sizeof(mac_head) + (int)sizeof(udp_head);

	// 数据处理部分
	const char* ptr_udp = ptr + net_header_len;

	process_packet(ptr_udp, udp_len);
	// 用完又重新挂回空闲数组上。注意，这只是用户程序自己的记录，ef_vi 层面的话还需要 refill_ring 才行
	pkt_buf_free(res, pkt_buf);
}

void udp_quote_base::handle_rx_discard(struct resources* res, int pkt_buf_i, int len, int discard_type) {
	struct pkt_buf* pkt_buf;
	LOGE("ERROR: discard type=%d\n", discard_type);
	if ( /* accept_discard_pkts */ 1) {
		handle_rx(res, pkt_buf_i, len);
	}
	else {
		pkt_buf = pkt_buf_from_id(res, pkt_buf_i);
		pkt_buf_free(res, pkt_buf);
	}
}

// 每次从 free_pkt_bufs 取 REFILL_BATCH_SIZE(16) 个出来，初始化 slots ，为收数据做准备
void udp_quote_base::refill_rx_ring(struct resources* res) {
	struct pkt_buf* pkt_buf;
	int i;

	// 如果空闲 slot 数不够就返回
	if (ef_vi_receive_space(&res->vi) < REFILL_BATCH_SIZE || res->free_pkt_bufs_n < REFILL_BATCH_SIZE)
		return;

	for (i = 0; i < REFILL_BATCH_SIZE; ++i) {
		pkt_buf = res->free_pkt_bufs;
		res->free_pkt_bufs = res->free_pkt_bufs->next;
		--(res->free_pkt_bufs_n);
		ef_vi_receive_init(&res->vi, pkt_buf->m_ef_addr + RX_DMA_OFF, pkt_buf->id);
	}
	ef_vi_receive_push(&res->vi);
}

void udp_quote_base::bind_thread(std::thread& thread, int core_id) {
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	CPU_SET(core_id, &cpuset);
	pthread_t native_handle = thread.native_handle();
	if (pthread_setaffinity_np(native_handle, sizeof(cpu_set_t), &cpuset)) {
		LOGE("Failed to bind thread to core %d.", core_id);
	}
}

void udp_quote_base::work_thread() {
	bind_thread(*m_recv_thread, m_udp_param_list[0].m_cpu_id);

	ef_event evs[32];
	int i = 0, n_ev = 0;
	while (!m_receive_quit_flag) {
		/* Poll event queue for events */
		n_ev = ef_eventq_poll(&m_res->vi, evs, sizeof(evs) / sizeof(evs[0]));
		if (n_ev > 0) {
			for (i = 0; i < n_ev; ++i) {
				switch (EF_EVENT_TYPE(evs[i])) {
				case EF_EVENT_TYPE_RX:
					/* This code does not handle jumbos. */
					assert(EF_EVENT_RX_SOP(evs[i]) != 0);
					assert(EF_EVENT_RX_CONT(evs[i]) == 0);
					// Get the dma_id used for a received packet
					// 我是从哪个 slot 里面拿到了多少的数据
					handle_rx(m_res, EF_EVENT_RX_RQ_ID(evs[i]),
						EF_EVENT_RX_BYTES(evs[i]) - m_res->rx_prefix_len);
					break;
				case EF_EVENT_TYPE_RX_DISCARD:
					handle_rx_discard(m_res, EF_EVENT_RX_DISCARD_RQ_ID(evs[i]),
						EF_EVENT_RX_DISCARD_BYTES(evs[i]) - m_res->rx_prefix_len,
						EF_EVENT_RX_DISCARD_TYPE(evs[i]));
					break;
				default:
					LOGE("ERROR: unexpected event type=%d\n", (int)EF_EVENT_TYPE(evs[i]));
					break;
				}
			}
			refill_rx_ring(m_res);
		}
	}
}