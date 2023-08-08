#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#define __USE_GNU
#include <pthread.h>
#include <sched.h>

#include <rk_mpi_sys.h>
#include <rk_mpi_mmz.h>

#define DMA_BUF_SYNC_READ (1 << 0)
#define DMA_BUF_SYNC_WRITE (2 << 0)
#define DMA_BUF_SYNC_RW (DMA_BUF_SYNC_READ | DMA_BUF_SYNC_WRITE)
#define DMA_BUF_SYNC_START (0 << 2)
#define DMA_BUF_SYNC_END (1 << 2)

#define DMA_BUF_BASE 'b'
#define DMA_BUF_IOCTL_SYNC _IOW(DMA_BUF_BASE, 0, struct dma_buf_sync)

typedef unsigned long long __u64;
typedef unsigned int __u32;

struct dma_buf_sync {
    __u64 flags;
};

int SYS_MmzAlloc(__u64* phy_addr, void** vir_addr, __u32 size) {
    int s32Ret;
    MB_BLK mb = NULL;
    s32Ret = RK_MPI_MMZ_Alloc(&mb, size,
                              RK_MMZ_ALLOC_TYPE_CMA | RK_MMZ_ALLOC_UNCACHEABLE);
    if (s32Ret == 0) {
        *phy_addr = RK_MPI_MB_Handle2PhysAddr(mb);
        *vir_addr = RK_MPI_MB_Handle2VirAddr(mb);
    }

    return 0;
}

int SYS_MmzAlloc_Cached(__u64* phy_addr, void** vir_addr, __u32 size) {
    int s32Ret;
    MB_BLK mb_blk = NULL;
    s32Ret = RK_MPI_SYS_MmzAlloc_Cached(phy_addr, RK_NULL, RK_NULL, size);
    if (s32Ret == 0) {
        *phy_addr = RK_MPI_MB_Handle2PhysAddr(mb_blk);
        *vir_addr = RK_MPI_MB_Handle2VirAddr(mb_blk);
        RK_MPI_SYS_MmzFlushCache(mb_blk, RK_FALSE);
    }

    return 0;
}

int SYS_MmzFlush_Start(void* vir_addr) {
    int s32Ret;
    MB_BLK mb_blk = RK_MPI_MB_VirAddr2Handle(vir_addr);
    int fd = RK_MPI_MMZ_Handle2Fd(mb_blk);
    __u32 size = RK_MPI_MMZ_GetSize(mb_blk);

    struct dma_buf_sync sync = {0};
    sync.flags = DMA_BUF_SYNC_START | DMA_BUF_SYNC_RW;
    s32Ret = ioctl(fd, DMA_BUF_IOCTL_SYNC, &sync);
    if (s32Ret) {
        printf("sync start failed %d\n", errno);
        return s32Ret;
    }

    return 0;
}

int SYS_MmzFlush_End(void* vir_addr) {
    int s32Ret;
    MB_BLK mb_blk = RK_MPI_MB_VirAddr2Handle(vir_addr);
    int fd = RK_MPI_MMZ_Handle2Fd(mb_blk);
    __u32 size = RK_MPI_MMZ_GetSize(mb_blk);

    struct dma_buf_sync sync = {0};
    sync.flags = DMA_BUF_SYNC_END | DMA_BUF_SYNC_RW;
    s32Ret = ioctl(fd, DMA_BUF_IOCTL_SYNC, &sync);
    if (s32Ret) {
        printf("sync end failed %d\n", errno);
        return s32Ret;
    }

    return 0;
}

int SYS_MmzFree(void* vir_addr) {
    MB_BLK mb_blk = RK_MPI_MB_VirAddr2Handle(vir_addr);
    RK_MPI_MMZ_Free(mb_blk);
    return 0;
}
