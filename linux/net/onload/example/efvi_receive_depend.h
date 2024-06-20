/*!****************************************************************************
	@file   	efvi_receive_depend.h

	@brief		提供接收处理行情类提供的头文件和结构体
******************************************************************************/
#pragma once
#include <stdlib.h>
#include <etherfabric/vi.h>
#include <etherfabric/pd.h>
#include <etherfabric/memreg.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stddef.h>
#include <string>
#include <iostream>

using std::string;

#ifndef MAP_HUGETLB
/* Not always defined in glibc headers.  If the running kernel does not
 * understand this flag it will ignore it and you may not get huge pages.
 * (In that case ef_memreg_alloc() may fail when using packed-stream mode).
 */
# define MAP_HUGETLB		0x40000
#endif

#ifdef __PPC__
# define huge_page_size    (16ll * 1024 * 1024)
#elif defined(__x86_64__) || defined(__i386__)
// huge_page_size, 自定义页的大小，主要用来给内存地址对齐，2M
# define huge_page_size    (2ll * 1024 * 1024)
#else
# error "Please define huge_page_size"
#endif

#define TRY(x)                                                  \
  do {                                                          \
    int __rc = (x);                                             \
    if( __rc < 0 ) {                                            \
      fprintf(stderr, "ERROR: TRY(%s) failed\n", #x);           \
      fprintf(stderr, "ERROR: at %s:%d\n", __FILE__, __LINE__); \
      fprintf(stderr, "ERROR: rc=%d errno=%d (%s)\n",           \
              __rc, errno, strerror(errno));                    \
      abort();                                                  \
    }                                                           \
  } while( 0 )

#define TEST(x)                                                 \
  do {                                                          \
    if( ! (x) ) {                                               \
      fprintf(stderr, "ERROR: TEST(%s) failed\n", #x);          \
      fprintf(stderr, "ERROR: at %s:%d\n", __FILE__, __LINE__); \
      abort();                                                  \
    }                                                           \
  } while( 0 )

#define LOGE(...)   do{ fprintf(stderr, __VA_ARGS__); }while(0)
#define LOGW(...)   do{ fprintf(stderr, __VA_ARGS__); }while(0)
#define LOGI(...)   do{ fprintf(stderr, __VA_ARGS__); }while(0)

// 将 p 扩大到 align 的倍数
#define ROUND_UP(p, align)   (((p)+(align)-1u) & ~((align)-1u))

 /* Hardware delivers at most ef_vi_receive_buffer_len() bytes to each
  * buffer (default 1792), and for best performance buffers should be
  * aligned on a 64-byte boundary.  Also, RX DMA will not cross a 4K
  * boundary.  The I/O address space may be discontiguous at 4K boundaries.
  * So easiest thing to do is to make buffers always be 2K in size.
  */
#define PKT_BUF_SIZE			2048

  /* Align address where data is delivered onto EF_VI_DMA_ALIGN boundary,
   * because that gives best performance.
   */
#define RX_DMA_OFF				ROUND_UP(sizeof(struct pkt_buf), EF_VI_DMA_ALIGN)

// 涉及到 Receive descriptors, 8 16 32 都行
#define REFILL_BATCH_SIZE		16

// optional application metadata
// 每个 buf 的前面字节用来填充 pkt_buf, 限定大小不超过 EF_VI_DMA_ALIGN
struct pkt_buf {
	/* I/O address corresponding to the start of this pkt_buf struct */
	ef_addr            m_ef_addr;		// 每个 slot 对应的 DMA address

	/* pointer to where received packets start */
	void* rx_ptr;

	int                id;
	// 查找空闲 slot 的时候，用来表示下一个空闲 slot 的位置
	struct pkt_buf* next;
};

struct resources {
	/* handle for accessing the driver */
	ef_driver_handle   dh;

	/* protection domain */
	struct ef_pd       pd;

	/* virtual interface (rxq + txq) */
	struct ef_vi       vi;
	int                rx_prefix_len;
	int                pktlen_offset;

	/* registered memory for DMA */
	void* pkt_bufs;		// 分配内存的首地址，内存又分为 pkt_buf_n 份，每一份用 struct pkt_buf 来管理
	int                pkt_bufs_n;
	struct ef_memreg   memreg;

	/* pool of free packet buffers (LIFO to minimise working set) */
	struct pkt_buf* free_pkt_bufs = nullptr;		// 空闲的 slot 数组首地址，后进先出
	int                free_pkt_bufs_n;	// 空闲的 slot 数
};

#pragma pack(push, 1)
/// MAC头
struct mac_head {
public:
	unsigned char		m_dst_addr[6];
	unsigned char		m_src_addr[6];
	unsigned short		m_type;
};

/// IP头
union ip_version {
	unsigned char		m_ver_and_len;
	struct {
		unsigned int	m_head_len : 4;
		unsigned int	m_version : 4;
	};
};

union ip_offset {
	unsigned short		m_slice_and_offset;
	struct {
		unsigned int	m_slice : 3;
		unsigned int	m_offset : 13;
	};
};

union ip_addr {
	unsigned int		m_ip;
	struct {
		unsigned char	m_b[4];
	};
};

struct ip_head {
	ip_version			m_version;			///<版本与首部长度
	unsigned char		m_tos;				///<8位服务类型
	unsigned short		m_total_len;		///<16位总长度
	unsigned short		m_packet_id;		///<16位标识
	ip_offset			m_offset;			///<3位标志信息及以13位偏移
	unsigned char		m_ttl;				///<8位生成时间(TTL)
	unsigned char		m_protocol;			///<8位协议
	unsigned short		m_check_sum;		///<16位首部校验和
	ip_addr				m_src_ip;			///<32位源地址
	ip_addr				m_dst_ip;			///<32位目标地址
};

struct udp_head {
	unsigned short		m_src_port;			///< 源端口
	unsigned short		m_dst_port;			///< 目标端口
	unsigned short		m_udp_len;			///< UDP长度
	unsigned short		m_crc;				///< 检验和
};

#pragma pack(pop)

/// 组播配置参数
class sock_udp_param {
public:
	sock_udp_param();
	~sock_udp_param();
	sock_udp_param(const sock_udp_param& other);
	sock_udp_param& operator= (const sock_udp_param& other);

private:
	void assign(const sock_udp_param& other);
public:
	bool			m_open = false;
	int				m_cpu_id;		/// 用于接收的cpu序号
	string			m_multicast_ip;
	unsigned short	m_multicast_port;
	string			m_local_ip;
	unsigned short	m_local_port;
	string			m_eth_name;
};

