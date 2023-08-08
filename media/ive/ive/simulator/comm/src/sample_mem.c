#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if BUILD_FPGA_ENV
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/poll.h>
#include <sys/socket.h>
#endif
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#if defined __linux || defined __linux__ || defined __DARWIN__ || defined __APPLE__
#include <unistd.h>
#define __USE_GNU
#include <pthread.h>
#include <sched.h>
#include <getopt.h>
#endif

typedef unsigned long long __u64;
typedef unsigned int __u32;

int RVE_MmzAlloc(__u64* phy_addr, void** vir_addr, __u32 size) {
    int s32Ret = 0;
    *vir_addr = malloc(size);
    *phy_addr = (__u64)*vir_addr;

    return s32Ret;
}

int RVE_MmzFree(void* addr) {
    free(addr);

    return 0;
}

int RVE_MmzAlloc_Cached(__u64* phy_addr, void** vir_addr, __u32 size) {
    *vir_addr = malloc(size);
    *phy_addr = (__u64)*vir_addr;

    return 0;
}

// CMA memory interface
#if BUILD_FPGA_ENV
struct dma_heap_allocation_data {
    __u64 len;
    __u32 fd;
    __u32 fd_flags;
    __u64 heap_flags;
    __u64 phys;
};
struct dma_heap_import_data {
    __u64 len;
    __u32 fd;
    __u32 fd_flags;
    __u64 heap_flags;
};
#define DMA_HEAP_IOC_MAGIC 'H'
#define DMA_HEAP_IOCTL_ALLOC \
    _IOWR(DMA_HEAP_IOC_MAGIC, 0x0, struct dma_heap_allocation_data)
#define DMA_HEAP_IOCTL_IMPORT \
    _IOWR(DMA_HEAP_IOC_MAGIC, 0x1, struct dma_heap_import_data)
#define DMA_BUF_SYNC_READ (1 << 0)
#define DMA_BUF_SYNC_WRITE (2 << 0)
#define DMA_BUF_SYNC_RW (DMA_BUF_SYNC_READ | DMA_BUF_SYNC_WRITE)
#define DMA_BUF_SYNC_START (0 << 2)
#define DMA_BUF_SYNC_END (1 << 2)
struct dma_buf_sync {
    __u64 flags;
};
#define DMA_BUF_BASE 'b'
#define DMA_BUF_IOCTL_SYNC _IOW(DMA_BUF_BASE, 0, struct dma_buf_sync)
#define CMA_HEAP_PATH "/dev/dma_heap/cma-uncached"
#define SYSTEM_HEAP_PATH "/dev/dma_heap/system"
#define CMA_HEAP_SIZE 1024 * 1024 * 3
struct dma_heap_allocation_data value = {
    .len = CMA_HEAP_SIZE,
    .fd_flags = O_CLOEXEC | O_RDWR,
};

struct cma_buffer {
    __u64 vir_addr;
    __u64 phy_addr;
    int fd;
    int size;
};

#define MAX_CMA_POOL_SIZE 20
struct cma_pool  {
    struct cma_buffer pool[MAX_CMA_POOL_SIZE];
    int pool_size;
};

struct cma_pool cma_pool_user = {0};

int CMA_pool_find(void* ptr, struct cma_buffer *buf) {
    int i;
    __u64 vir_addr = (__u64)ptr;

    for (i = 0; i < MAX_CMA_POOL_SIZE; i++) {
        if (vir_addr == cma_pool_user.pool[i].vir_addr) {
            buf = &cma_pool_user.pool[i];
            printf("CMA_pool_find mem id: %d, fd: %d\n", i,
                   cma_pool_user.pool[i].fd);
            return 0;
        }
    }

    return -1;
}

int CMA_pool_del(void* ptr) {
    int i;
    int id = -1;
    __u64 vir_addr = (__u64)ptr;

    for (i = 0; i < MAX_CMA_POOL_SIZE; i++) {
        if (vir_addr == cma_pool_user.pool[i].vir_addr) {
            id = i;
            break;
        }
    }

    printf("CMA_pool_del mem id: %d\n", id);
    if (id == -1) {
        return -1;
    }

    for (i = id; i < cma_pool_user.pool_size; i++) {
        cma_pool_user.pool[i] = cma_pool_user.pool[i + 1];
    }

    cma_pool_user.pool_size--;

    return 0;
}

int CMA_alloc(__u64* phy_addr, __u64* vir_addr, __u32 size) {
    int fd;
    int ret;
    unsigned char* ptr;
    int prot = PROT_READ;
    struct dma_buf_sync sync = {0};

    fd = open(CMA_HEAP_PATH, O_RDWR);
    if (fd < 0) {
        printf("open %s failed!\n", CMA_HEAP_PATH);
        return ret;
    }

    value.fd = 0;
    ret = ioctl(fd, DMA_HEAP_IOCTL_ALLOC, &value);
    if (ret < 0) {
        printf("DMA_HEAP_ALLOC_BUFFER failed\n");
        return ret;
    }

    *phy_addr = value.phys;

    printf("[func]%s(%d): phy=0x%016llx, fd = %d, flags = 0x%x\n", __func__, __LINE__, value.phys, value.fd,
           value.fd_flags);

    if (fcntl(value.fd, F_GETFL) & O_RDWR) prot |= PROT_WRITE;

    /* mmap contiguors buffer to user */
    ptr = mmap(NULL, size, prot, MAP_SHARED, value.fd, 0);
    if (ptr == MAP_FAILED) {
        printf("mmap failed: %s\n", strerror(errno));
        return -errno;
    } else {
        printf("[func]%s(%d): mmap vir=0x%016llx\n", __func__, __LINE__, (__u64)ptr);
    }

    *vir_addr = (__u64)ptr;

    struct cma_buffer *buf = &cma_pool_user.pool[cma_pool_user.pool_size];
    buf->fd = value.fd;
    buf->vir_addr = (__u64)ptr;
    buf->phy_addr = (__u64)value.phys;
    buf->size = size;
    cma_pool_user.pool_size++;

    close(fd);

    return ret;
}

void CMA_sync(void* ptr) {
    int ret;
    struct dma_buf_sync sync = {0};
    struct cma_buffer buf = {0};

    ret = CMA_pool_find(ptr, &buf);
    if (ret < 0) {
        printf("[func]%s(%d):unknown buffer\n", __func__, __LINE__);
        return;
    }

    /* sync cache */
    sync.flags = DMA_BUF_SYNC_END | DMA_BUF_SYNC_RW;
    ret = ioctl(buf.fd, DMA_BUF_IOCTL_SYNC, &sync);
    if (ret) {
        printf("sync end failed %d\n", errno);
    }
}

void CMA_free(void* ptr) {
    int ret;
    struct cma_buffer buf = {0};

    ret = CMA_pool_find(ptr, &buf);
    if (ret < 0) {
        printf("[func]%s(%d):unknown buffer\n", __func__, __LINE__);
        return;
    }

    munmap(ptr, buf.size);
    close(buf.fd);

    CMA_pool_del(ptr);
}
#endif

// System memory interface
#if BUILD_SDK_ENV
#include <rk_mpi_mmz.h>
#include <sys/ioctl.h>
struct dma_buf_sync {
    __u64 flags;
};

#define DMA_BUF_SYNC_READ (1 << 0)
#define DMA_BUF_SYNC_WRITE (2 << 0)
#define DMA_BUF_SYNC_RW (DMA_BUF_SYNC_READ | DMA_BUF_SYNC_WRITE)
#define DMA_BUF_SYNC_START (0 << 2)
#define DMA_BUF_SYNC_END (1 << 2)

#define DMA_BUF_BASE 'b'
#define DMA_BUF_IOCTL_SYNC _IOW(DMA_BUF_BASE, 0, struct dma_buf_sync)

int SYS_MmzAlloc(__u64* phy_addr, void** vir_addr, __u32 size) {
    int s32Ret;
    MB_BLK mb = NULL;
    s32Ret = RK_MPI_MMZ_Alloc(&mb, size, RK_MMZ_ALLOC_TYPE_CMA | RK_MMZ_ALLOC_UNCACHEABLE);
    if (s32Ret == 0) {
        *phy_addr = RK_MPI_MB_Handle2PhysAddr(mb);
        *vir_addr = RK_MPI_MB_Handle2VirAddr(mb);
    }

    return 0;
}

int SYS_MmzAlloc_Cached(__u64* phy_addr, void** vir_addr, __u32 size) {
    int s32Ret;
    MB_BLK mb_blk = NULL;
    s32Ret = RK_MPI_MMZ_Alloc(&mb_blk, size, RK_MMZ_ALLOC_TYPE_CMA | RK_MMZ_ALLOC_CACHEABLE);
    if (s32Ret == 0) {
        *phy_addr = RK_MPI_MB_Handle2PhysAddr(mb_blk);
        *vir_addr = RK_MPI_MB_Handle2VirAddr(mb_blk);
    }

    return 0;
}

int SYS_MmzFlush_Start(void* vir_addr) {
    int s32Ret;
    MB_BLK mb_blk = RK_MPI_MB_VirAddr2Handle(vir_addr);
    int fd = RK_MPI_MMZ_Handle2Fd(mb_blk);
    __u32 size = RK_MPI_MMZ_GetSize(mb_blk);

#if 0
    RK_MPI_MMZ_FlushCacheVaddrStart(
        vir_addr, size, RK_MMZ_ALLOC_TYPE_CMA | RK_MMZ_ALLOC_CACHEABLE);
#else
    struct dma_buf_sync sync = {0};
    sync.flags = DMA_BUF_SYNC_START | DMA_BUF_SYNC_RW;
    s32Ret = ioctl(fd, DMA_BUF_IOCTL_SYNC, &sync);
    if (s32Ret) {
        //printf("sync start failed %d\n", errno);
        return s32Ret;
    }
#endif
    return 0;
}

int SYS_MmzFlush_End(void* vir_addr) {
    int s32Ret;
    MB_BLK mb_blk = RK_MPI_MB_VirAddr2Handle(vir_addr);
    int fd = RK_MPI_MMZ_Handle2Fd(mb_blk);
    __u32 size = RK_MPI_MMZ_GetSize(mb_blk);

#if 0
    RK_MPI_MMZ_FlushCacheVaddrEnd(
        vir_addr, size, RK_MMZ_ALLOC_TYPE_CMA | RK_MMZ_ALLOC_CACHEABLE);
#else
    struct dma_buf_sync sync = {0};
    sync.flags = DMA_BUF_SYNC_END | DMA_BUF_SYNC_RW;
    s32Ret = ioctl(fd, DMA_BUF_IOCTL_SYNC, &sync);
    if (s32Ret) {
        //printf("sync end failed %d\n", errno);
        return s32Ret;
    }
#endif
    return 0;
}

int SYS_MmzFree(void* vir_addr) {
    MB_BLK mb_blk = RK_MPI_MB_VirAddr2Handle(vir_addr);
    RK_MPI_MMZ_Free(mb_blk);
    return 0;
}
#endif
