//
// Created by alfred on 16-10-16.
//

#ifndef THUNDER_THUNDERRINGBUFFERT_H
#define THUNDER_THUNDERRINGBUFFERT_H

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define SHM_RB_SIZE_T (65536)

namespace thunder {

/*
 * 创建一块共享内存，里面存放的数据类型为 T, 大小会调整为 2 的幂
 *
 * publisher 是 MDName-Data, consumer 是 MDName-Ptr
 * 用法：
 *      写： getCurrentNode 获取 node 指针，写入数据, 调用 moveNext 移动到下一个节点
 *      读： 调用 getCount() 获取当前的个数与上一次，然后 getOneNode(n) 获取 node 指针
*/
template <typename T>
class thunderRingBufferT {
public:
    typedef unsigned long ShmMDPtr;

#pragma pack(push, 1)
    struct node
    {
        T data;
    };
#pragma pack(pop)

    thunderRingBufferT(const char *MDName) : MDName(MDName)
    {
        struct stat buf;
        std::string dataId = "/" + std::string(MDName) + "-Data";
        int fd = shm_open(dataId.c_str(), O_RDONLY, ACCESSPERMS);
        fstat(fd, &buf);
        _size = buf.st_size / sizeof(T);
        assert((_size != 0) && ((_size & (~_size + 1)) == _size));
        if (!((_size != 0) && ((_size & (~_size + 1)) == _size))) {
            throw std::runtime_error("Error buffer size " + std::to_string(_size));
        }
        printf("Thunder deduced size for %s is %lu\n", MDName, (unsigned long)_size);
        isValid &= initConsumer(MDName);
        isValid &= initPtr(MDName, false);
        close(fd);
    }

    thunderRingBufferT(const char *MDName, bool isPublisher, size_t size = SHM_RB_SIZE_T, bool prefault = false)
        : _size(size), MDName(MDName)
    {

        assert((_size != 0) && ((_size & (~_size + 1)) == _size));
        if (!((_size != 0) && ((_size & (~_size + 1)) == _size))) {
            throw std::runtime_error("Error buffer size " + std::to_string(_size));
        }
        
        if (isPublisher)
        {
            isValid &= initPublisher(MDName, prefault);
        }
        else
        {
            isValid &= initConsumer(MDName);
        }

        isValid &= initPtr(MDName, isPublisher);
    }

    // init by core
    thunderRingBufferT(const int core_id, bool isPublisher, size_t size = SHM_RB_SIZE_T, bool prefault = false)
        : thunderRingBufferT((std::string("thunder-") + std::to_string(core_id)).c_str(), isPublisher, size, prefault) {}

    thunderRingBufferT(const int core_id)
        : thunderRingBufferT((std::string("thunder-") + std::to_string(core_id)).c_str())
    {
    }

    ~thunderRingBufferT()
    {
    }

    ShmMDPtr getCount()
    {
        return (*ptr);
    }

    ShmMDPtr getSize()
    {
        return _size;
    }

    void moveNext()
    {
        (*ptr)++;
    }

    node *getCurrentNode()
    {
        return (buffer + getRBPtr(*ptr));
    }

    inline ShmMDPtr getRBPtr(ShmMDPtr p)
    {
        return (p & (_size - 1));
    }

    // 获取 buffer 往前 p 个位置的地址
    node *getOneNode(ShmMDPtr p)
    {
        return (buffer + getRBPtr(p));
    }

    bool isIsValid() const
    {
        return isValid;
    }

    void resetCount()
    {
        (*ptr) = 0;
    }

    static bool isPubliserExist(const std::string &MDName)
    {
        std::string dataId = "/" + MDName + "-Data";
        auto fd = shm_open(dataId.c_str(), O_RDONLY, ACCESSPERMS);
        if (fd < 0)
        {
            return false;
        }
        else
        {
            close(fd);
            return true;
        }
    }

    // 返回总个数，也就是共享内存能存储 _size 个 T 类型 Data
    size_t get_thunder_ring_buffer_size()
    {
        return _size;
    }

private:
    bool isFileExist(std::string filename)
    {
        std::ifstream ifile(filename.c_str());
        bool toRet = ifile.good();
        ifile.close();
        return toRet;
    }

    /*
     * publisher 创建 MDName-Data, 设置 mmap
    */
    bool initPublisher(const char *MDName, bool prefault)
    {
        std::string dataId = "/" + std::string(MDName) + "-Data";

        int fd = shm_open(dataId.c_str(), O_CREAT | O_RDWR, ACCESSPERMS);

        if (fd < 0)
        {
            perror("Producer Error Initializing");
            return false;
        }

        if (ftruncate(fd, sizeof(node) * _size) < 0)
        {
            perror("ftruncate");
            return false;
        }

        auto buf_size = sizeof(node) * _size;
        void *mmap_ptr = prefault ? mmap(NULL, buf_size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_POPULATE, fd, 0)
                                  : mmap(NULL, buf_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        // void *mmap_ptr = prefault ? mmap(NULL, buf_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)
        //                           : mmap(NULL, buf_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (mmap_ptr == (caddr_t)-1)
        {
            std::cerr << "mmap error" << std::endl;
            shm_unlink(dataId.c_str());
            close(fd);
            return false;
        }


        // if (prefault && madvise(mmap_ptr, buf_size, MADV_RANDOM) != 0 && mlock(mmap_ptr, buf_size) != 0)
        // {
        //     munmap(mmap_ptr, buf_size);
        //     close(fd);
        //     std::cerr << "failed to lock memory for page " << dataId << std::endl;
        //     return false;
        // }

        buffer = (node *)mmap_ptr;

        return true;
    }

    /*
     * 打开已经创建好的 MDName-Data, 获取 mmap 句柄，以供后续使用
    */
    bool initConsumer(const char *MDName)
    {
        std::string dataId = "/" + std::string(MDName) + "-Data";

        int fd = shm_open(dataId.c_str(), O_RDONLY, ACCESSPERMS);

        if (fd < 0)
        {
            fprintf(stderr, "Consumer Error Initializing: %s failed: %s\n",
                    dataId.c_str(), strerror(errno));
            return false;
        }

        void *mmap_ptr = mmap(NULL, sizeof(node) * _size, PROT_READ, MAP_SHARED, fd, 0);

        if (mmap_ptr == (caddr_t)-1)
        {
            std::cerr << "mmap error" << std::endl;
            shm_unlink(dataId.c_str());
            return false;
        }

        buffer = (node *)mmap_ptr;

        return true;
    }

    /*
     * 打开或者创建 MDName-Ptr, 如果之前已经存在，会获取当前的 Ptr 值
    */
    bool initPtr(const char *MDName, bool isPublisher)
    {
        std::string dataId = "/" + std::string(MDName) + "-Ptr";

        bool newFile = !isFileExist("/dev/shm" + dataId);

        int fd = 0;
        if (isPublisher)
        {
            fd = shm_open(dataId.c_str(), O_CREAT | O_RDWR, ACCESSPERMS);
        }
        else
        {
            fd = shm_open(dataId.c_str(), O_RDONLY, ACCESSPERMS);
        }

        if (fd < 0)
        {
            std::cerr << "Shm Ptr Init Error..." << std::endl;
            return false;
        }

        void *mmap_ptr;
        if (isPublisher)
        {
            if (ftruncate(fd, sizeof(ShmMDPtr)) < 0)
            {
                perror("ftruncate");
                exit(1);
            }
            mmap_ptr = mmap(NULL, sizeof(ShmMDPtr), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        }
        else
        {
            mmap_ptr =
                mmap(NULL, sizeof(ShmMDPtr), PROT_READ, MAP_SHARED, fd, 0);
        }

        if (mmap_ptr == (caddr_t)-1)
        {
            std::cerr << "mmap error" << std::endl;
            shm_unlink(dataId.c_str());
            return false;
        }

        ShmMDPtr *toRet = (ShmMDPtr *)mmap_ptr;

        if (newFile)
        {
            (*toRet) = 0;
        }

        ptr = toRet;

        return true;
    }

    ShmMDPtr *ptr;
    node *buffer;

    size_t _size;

    std::string MDName;

    bool isValid = true;
};
} // namespace thunder

#endif //SHMRINGBUFFER_SHMRINGBUFFERT_H
