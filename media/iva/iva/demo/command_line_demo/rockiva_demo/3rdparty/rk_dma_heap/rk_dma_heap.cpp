#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "rk_dma_heap.h"

typedef unsigned long long __u64;
typedef  unsigned int __u32;

struct dma_heap_allocation_data {
	__u64 len;
	__u32 fd;
	__u32 fd_flags;
	__u64 heap_flags;
};

#define DMA_HEAP_IOC_MAGIC		'R'
#define DMA_HEAP_IOCTL_ALLOC	_IOWR(DMA_HEAP_IOC_MAGIC, 0x0,\
				      struct dma_heap_allocation_data)

#define DMA_BUF_SYNC_READ      (1 << 0)
#define DMA_BUF_SYNC_WRITE     (2 << 0)
#define DMA_BUF_SYNC_RW        (DMA_BUF_SYNC_READ | DMA_BUF_SYNC_WRITE)
#define DMA_BUF_SYNC_START     (0 << 2)
#define DMA_BUF_SYNC_END       (1 << 2)

struct dma_buf_sync {
	__u64 flags;
};

#define DMA_BUF_BASE		'b'
#define DMA_BUF_IOCTL_SYNC	_IOW(DMA_BUF_BASE, 0, struct dma_buf_sync)

#define CMA_HEAP_PATH	"/dev/rk_dma_heap/rk-dma-heap-cma"

static int get_rk_dma_heap() {
    static int fd_ = -1;
    if (fd_ < 0) {
        printf("open %s\n", CMA_HEAP_PATH);
        fd_ = open(CMA_HEAP_PATH, O_RDWR);
        if (fd_ < 0) {
            printf("error, open %s fail!\n", CMA_HEAP_PATH);
            return fd_;
        }
        printf("fd_=%d\n", fd_);
    }
    return fd_;
}

int rk_dma_heap_alloc(int size, int *buf_fd, void **buf_virt_addr) {
    int ret;
    int fd = get_rk_dma_heap();
    if (fd < 0) {
        printf("get rk_dma_heap fd error fd=%d\n", fd);
        return -1;
    }

    struct dma_heap_allocation_data value;
    memset(&value, 0, sizeof(dma_heap_allocation_data));
    value.len = size;
    value.fd_flags = O_CLOEXEC | O_RDWR;
    int prot = PROT_READ;
    void *ptr;

    /* allc contiguors buffer */
    ret = ioctl(fd, DMA_HEAP_IOCTL_ALLOC, &value);
    if (ret < 0) {
        printf("DMA_HEAP_ALLOC_BUFFER failed ret=%d %s\n", ret, strerror(errno));
        return ret;
    }

    printf("dmabuf fd = %d\n", value.fd);

    if (fcntl(value.fd, F_GETFL) & O_RDWR)
        prot |= PROT_WRITE;

    /* mmap contiguors buffer to user */
    ptr = mmap(NULL, size, prot, MAP_SHARED, value.fd, 0);
    if (ptr == MAP_FAILED) {
        printf("mmap failed: %s\n", strerror(errno));
        return -errno;
    }

    *buf_fd = value.fd;
    *buf_virt_addr = ptr;

    return 0;
}

int rk_dma_heap_release(int buf_fd, void *buf_virt_addr, size_t size) {
    int fd = get_rk_dma_heap();
    if (buf_fd <= 0 || buf_virt_addr == nullptr) {
        printf("param error (buf_fd=%d buf_virt_addr=%p)\n", buf_fd, buf_virt_addr);
        return -1;
    }
    munmap(buf_virt_addr, size);
    close(buf_fd);

    return 0;
}

int rk_dma_heap_sync_start(int buf_fd, void *sync) {
    int ret;
    sync = malloc(sizeof(struct dma_buf_sync));
    struct dma_buf_sync* _sync = (struct dma_buf_sync*)sync;
    _sync->flags = DMA_BUF_SYNC_START | DMA_BUF_SYNC_RW;
    ret = ioctl(buf_fd, DMA_BUF_IOCTL_SYNC, _sync);
    if (ret) {
        printf("sync start failed %d\n", errno);
        return ret;
    }
    return 0;
}

int rk_dma_heap_sync_end(int buf_fd, void *sync) {
    int ret;
    struct dma_buf_sync* _sync = (struct dma_buf_sync*)sync;
    _sync->flags = DMA_BUF_SYNC_END | DMA_BUF_SYNC_RW;
    ret = ioctl(buf_fd, DMA_BUF_IOCTL_SYNC, _sync);
    if (ret) {
        printf("sync end failed %d\n", errno);
        return ret;
    }
    free(_sync);
    return 0;
}