/* Copyright 2020 Rockchip Electronics Co. LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include <vector>

#include "rk_debug.h"
#include "rk_comm_mb.h"
#include "rk_mpi_mmz.h"

#include "test_comm_argparse.h"

#define MB_POOL_COUNT           10
#define MB_POOL_MB_COUNT        10
#define MB_POOL_MB_SIZE         1280 * 720 * 2

typedef struct _rkTestMMZCtx {
    RK_S32      s32MmzCount;
    RK_S32      s32MmzSize;
    RK_S32      s32PoolCount;
    RK_S32      s32LoopCount;
    RK_BOOL     bExternal;
    RK_BOOL     bPreAlloc;
    RK_S32      s32RemapMode;
    RK_S32      s32AllocType;
} TEST_MMZ_CTX_S;

#define vaddr_to_fd_offset(vaddr, fd, offset) do {\
    MB_BLK __blk = RK_MPI_MMZ_VirAddr2Handle(vaddr); \
    if (__blk != (MB_BLK)RK_NULL) { \
        RK_VOID* __vaddr = RK_MPI_MMZ_Handle2VirAddr(__blk); \
        offset = vaddr - reinterpret_cast<RK_U8 *>(__vaddr); \
        fd = RK_MPI_MMZ_Handle2Fd(__blk); \
    } else { \
        offset = -1; \
        fd = -1; \
    } \
} while (0);

#define paddr_to_fd_offset(paddr, fd, offset) do {\
    MB_BLK __blk = RK_MPI_MMZ_PhyAddr2Handle(paddr); \
    if (__blk != (MB_BLK)RK_NULL) { \
        RK_U64 __paddr = RK_MPI_MMZ_Handle2PhysAddr(__blk); \
        offset = (RK_U8)paddr - (RK_U8)__paddr; \
        fd = RK_MPI_MMZ_Handle2Fd(__blk); \
    } else { \
        offset = -1; \
        fd = -1; \
    } \
} while (0);

RK_S32 unit_test_mpi_mmz(const TEST_MMZ_CTX_S *pCtx) {
    MB_BLK mb;
    RK_S32 len = 1048576;
    RK_S32 ret = RK_SUCCESS;
    RK_S32 flags = RK_MMZ_ALLOC_CACHEABLE | RK_MMZ_ALLOC_TYPE_CMA;

    ret = RK_MPI_MMZ_Alloc(&mb, len, flags);
    if (ret < 0) {
        RK_LOGI("alloc mmz fail");
        return ret;
    }

    RK_U8 *vaddr = reinterpret_cast<RK_U8 *>(RK_MPI_MMZ_Handle2VirAddr(mb));
    RK_U64 paddr = RK_MPI_MMZ_Handle2PhysAddr(mb);
    RK_S32 fd = RK_MPI_MMZ_Handle2Fd(mb);
    len = RK_MPI_MMZ_GetSize(mb);
    RK_S32 is_cacheable = RK_MPI_MMZ_IsCacheable(mb);

    RK_LOGI("alloc buffer: fd=%d, len=%d, paddr=0x%x, vaddr=%p, cacheable=%d", fd, len, paddr, vaddr, is_cacheable);
    MB_BLK mb_by_fd = RK_MPI_MMZ_Fd2Handle(fd);
    MB_BLK mb_by_vaddr = RK_MPI_MMZ_VirAddr2Handle(reinterpret_cast<RK_VOID *>(vaddr));
    RK_LOGI("MB: %p %p %p", mb, mb_by_fd, mb_by_vaddr);

    RK_U8 *vaddr_temp = vaddr + 1;
    RK_LOGI("vaddr+1: %p, mb: %p", vaddr_temp, RK_MPI_MMZ_VirAddr2Handle(reinterpret_cast<RK_VOID *>(vaddr_temp)));

    vaddr_temp = vaddr-1;
    RK_LOGI("vaddr-1: %p, mb: %p", vaddr_temp, RK_MPI_MMZ_VirAddr2Handle(reinterpret_cast<RK_VOID *>(vaddr_temp)));

    vaddr_temp = vaddr+len;
    RK_LOGI("vaddr+len: %p, mb: %p", vaddr_temp, RK_MPI_MMZ_VirAddr2Handle(reinterpret_cast<RK_VOID *>(vaddr_temp)));

    vaddr_temp = vaddr+len-1;
    RK_LOGI("vaddr+len-1: %p, mb: %p", vaddr_temp, RK_MPI_MMZ_VirAddr2Handle(reinterpret_cast<RK_VOID *>(vaddr_temp)));

    RK_U64 paddr_temp = paddr+1;
    RK_LOGI("paddr+1: 0x%x, mb: %p", paddr_temp, RK_MPI_MMZ_PhyAddr2Handle(paddr_temp));

    paddr_temp = paddr-1;
    RK_LOGI("paddr-1: 0x%x, mb: %p", paddr_temp, RK_MPI_MMZ_PhyAddr2Handle(paddr_temp));

    paddr_temp = paddr+len;
    RK_LOGI("paddr+len: 0x%x, mb: %p", paddr_temp, RK_MPI_MMZ_PhyAddr2Handle(paddr_temp));

    paddr_temp = paddr+len-1;
    RK_LOGI("paddr+len-1: 0x%x, mb: %p", paddr_temp, RK_MPI_MMZ_PhyAddr2Handle(paddr_temp));

    RK_U32 offset;
    vaddr_temp = vaddr+len-10;
    vaddr_to_fd_offset(vaddr_temp, fd, offset);
    RK_LOGI("vaddr+len-10: %p, fd: %d, offset: %d", vaddr_temp, fd, offset);

    paddr_temp = paddr+len-10;
    paddr_to_fd_offset(paddr_temp, fd, offset);
    RK_LOGI("paddr+len-10: 0x%x, fd: %d, offset: %d", paddr_temp, fd, offset);

    ret = RK_MPI_MMZ_FlushCacheStart(mb, 0, 0, RK_MMZ_SYNC_WRITEONLY);
    memset(vaddr, 0x5A, len);
    ret = RK_MPI_MMZ_FlushCacheEnd(mb, 0, 0, RK_MMZ_SYNC_WRITEONLY);

    ret = RK_MPI_MMZ_FlushCacheStart(mb, 4096, 4096, RK_MMZ_SYNC_RW);
    memset(vaddr, 0x5A, len);
    ret = RK_MPI_MMZ_FlushCacheEnd(mb, 4096, 4096, RK_MMZ_SYNC_RW);

    vaddr_temp = vaddr+len-10;
    ret = RK_MPI_MMZ_FlushCacheVaddrStart(reinterpret_cast<RK_VOID *>(vaddr_temp), 4096, RK_MMZ_SYNC_WRITEONLY);
    memset(vaddr_temp, 0x5A, 10);
    ret = RK_MPI_MMZ_FlushCacheVaddrEnd(reinterpret_cast<RK_VOID *>(vaddr_temp), 4096, RK_MMZ_SYNC_WRITEONLY);

    paddr_temp = paddr_temp+len-10;
    ret = RK_MPI_MMZ_FlushCachePaddrStart(paddr_temp, 4096, RK_MMZ_SYNC_WRITEONLY);
    vaddr_temp = vaddr+len-10;
    memset(vaddr_temp, 0x5A, 10);
    ret = RK_MPI_MMZ_FlushCachePaddrEnd(paddr_temp, 4096, RK_MMZ_SYNC_WRITEONLY);

    usleep(100000);
    RK_MPI_MMZ_Free(mb);
    return RK_SUCCESS;
}


static const char *const usages[] = {
    "./rk_mpi_mmz_test [-c MB_COUNT] [-s MB_SIZE]...",
    NULL,
};

RK_S32 main(RK_S32 argc, const char **argv) {
    RK_S32 s32Ret = RK_SUCCESS;
    TEST_MMZ_CTX_S stMmzCtx;

    memset(&stMmzCtx, 0, sizeof(TEST_MMZ_CTX_S));

    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("basic options:"),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "\nselect a test case to run.",
                                 "\nuse --help for details.");

    argc = argparse_parse(&argparse, argc, argv);

    s32Ret = unit_test_mpi_mmz(&stMmzCtx);
    if (s32Ret != RK_SUCCESS) {
        goto __FAILED;
    }

    RK_LOGI("test running ok.");
    return RK_SUCCESS;
__FAILED:
    RK_LOGE("test running failed!");
    return RK_ERR_MB_BUSY;
}
