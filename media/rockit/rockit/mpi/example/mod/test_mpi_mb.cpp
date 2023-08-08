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

#ifdef DBG_MOD_ID
#undef DBG_MOD_ID
#endif
#define DBG_MOD_ID DBG_MOD_COMB1(RK_ID_MB)

#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include <vector>

#include "rk_debug.h"
#include "rk_mpi_mb.h"
#include "rk_mpi_sys.h"

#include "test_comm_argparse.h"

#define MB_POOL_COUNT           10
#define MB_POOL_MB_COUNT        10
#define MB_POOL_MB_SIZE         1280 * 720 * 2

typedef struct _rkTestMbCtx {
    RK_S32      s32MbCount;
    RK_S32      s32MbSize;
    RK_S32      s32PoolCount;
    RK_S32      s32LoopCount;
    RK_BOOL     bExternal;
    RK_BOOL     bPreAlloc;
    RK_S32      s32RemapMode;
    RK_S32      s32AllocType;
    RK_S32      ss2DmaType;
} TEST_MB_CTX_S;

RK_S32 unit_test_mpi_mb(const TEST_MB_CTX_S *pstCtx) {
    MB_POOL_CONFIG_S pstMbPoolCfg;
    std::vector<MB_BLK> vector;
    std::vector<MB_BLK>::iterator iter;
    RK_S32 s32Ret = RK_SUCCESS;
    RK_S32 loopCount = pstCtx->s32LoopCount;
    RK_S32 s32UniqueId = -1;
    RK_S32 s32DupFd = -1;

    memset(&pstMbPoolCfg, 0, sizeof(MB_POOL_CONFIG_S));
    pstMbPoolCfg.u64MBSize   = pstCtx->s32MbSize;
    pstMbPoolCfg.u32MBCnt    = pstCtx->s32MbCount;
    pstMbPoolCfg.bPreAlloc   = pstCtx->bPreAlloc;
    pstMbPoolCfg.enRemapMode = (MB_REMAP_MODE_E)pstCtx->s32RemapMode;
    pstMbPoolCfg.enAllocType = (MB_ALLOC_TYPE_E)pstCtx->s32AllocType;
    pstMbPoolCfg.enDmaType = (MB_DMA_TYPE_E)(pstCtx->ss2DmaType << 12);

    do {
        for (RK_S32 i = 0; i < pstCtx->s32PoolCount; i++) {
            MB_POOL pool = RK_MPI_MB_CreatePool(&pstMbPoolCfg);
            if (pool == MB_INVALID_POOLID) {
                s32Ret = RK_ERR_MB_2MPOOLS;
                goto __FAILED;
            }
            RK_LOGI("pool id %d", pool);
        }

        for (RK_S32 i = 0; i < pstCtx->s32PoolCount; i++) {
            vector.clear();
            for (RK_S32 j = 0; j < pstCtx->s32MbCount; j++) {
                MB_BLK mb = RK_MPI_MB_GetMB(i, pstMbPoolCfg.u64MBSize, RK_FALSE);
                if (mb == MB_INVALID_HANDLE) {
                    s32Ret = RK_ERR_MB_NOBUF;
                    goto __FAILED;
                }
                RK_U64 phyAddr = RK_MPI_MB_Handle2PhysAddr(mb);
                if (pstMbPoolCfg.enDmaType == MB_DMA_TYPE_CMA
                      && pstMbPoolCfg.enAllocType == MB_ALLOC_TYPE_DMA
                      && phyAddr == 0) {
                    s32Ret = RK_ERR_MB_BUSY;
                    RK_MPI_MB_ReleaseMB(mb);
                    goto __FAILED;
                }
                MB_POOL poolId = RK_MPI_MB_Handle2PoolId(mb);
                if (poolId != i) {
                    s32Ret = RK_ERR_MB_BUSY;
                    RK_MPI_MB_ReleaseMB(mb);
                    goto __FAILED;
                }
                RK_S32 refsCount = RK_MPI_MB_InquireUserCnt(mb);
                if (refsCount != 1) {
                    s32Ret = RK_ERR_MB_BUSY;
                    RK_MPI_MB_ReleaseMB(mb);
                    goto __FAILED;
                }
                RK_VOID *virAddr = RK_MPI_MB_Handle2VirAddr(mb);
                if (virAddr == RK_NULL) {
                    s32Ret = RK_ERR_MB_BUSY;
                    RK_MPI_MB_ReleaseMB(mb);
                    goto __FAILED;
                }
                RK_S32 fd = RK_MPI_MB_Handle2Fd(mb);
                if (pstMbPoolCfg.enAllocType == MB_ALLOC_TYPE_DMA
                      && fd < 0) {
                    s32Ret = RK_ERR_MB_BUSY;
                    RK_MPI_MB_ReleaseMB(mb);
                    goto __FAILED;
                }
                if (pstMbPoolCfg.enAllocType == MB_ALLOC_TYPE_DMA) {
                    if (RK_MPI_MB_VirAddr2Handle(virAddr) != mb) {
                        s32Ret = RK_ERR_MB_BUSY;
                        RK_MPI_MB_ReleaseMB(mb);
                        goto __FAILED;
                    }
                    s32UniqueId = RK_MPI_MB_Handle2UniqueId(mb);
                    if (s32UniqueId < 0) {
                        s32Ret = RK_ERR_MB_BUSY;
                        RK_MPI_MB_ReleaseMB(mb);
                        goto __FAILED;
                    }
                    s32DupFd = RK_MPI_MB_UniqueId2Fd(s32UniqueId);
                }

                RK_LOGI("get poolId(%d) from mb(%p) refsCount(%d) phyAddr(0x%llx) virAddr(%p) "
                         "fd(%d) uniqueId(%d) dupFd(%d)",
                         poolId, mb, refsCount, phyAddr, virAddr, fd, s32UniqueId, s32DupFd);
                if (s32DupFd >= 0) {
                    close(s32DupFd);
                    s32DupFd = -1;
                }
                vector.emplace_back(mb);
            }
            iter = vector.begin();
            for (; iter != vector.end(); ++iter) {
                s32Ret = RK_MPI_MB_ReleaseMB(*iter);
                if (s32Ret != RK_SUCCESS) {
                    goto __FAILED;
                }
            }
        }

        for (RK_S32 i = 0; i < pstCtx->s32PoolCount; i++) {
            s32Ret = RK_MPI_MB_DestroyPool(i);
            if (s32Ret != RK_SUCCESS) {
                goto __FAILED;
            }
        }
        loopCount--;
        RK_LOGI("looping times %d", pstCtx->s32LoopCount - loopCount);
    } while (loopCount > 0);

    return RK_SUCCESS;
__FAILED:
    iter = vector.begin();
    for (; iter != vector.end(); ++iter) {
        RK_MPI_MB_ReleaseMB(*iter);
    }
    for (RK_S32 i = 0; i < pstCtx->s32PoolCount; i++) {
        RK_MPI_MB_DestroyPool(i);
    }
    return s32Ret;
}

static const char *const usages[] = {
    "./rk_mpi_mb_test [-c MB_COUNT] [-s MB_SIZE]...",
    NULL,
};

RK_S32 main(RK_S32 argc, const char **argv) {
    RK_S32 s32Ret = RK_SUCCESS;
    TEST_MB_CTX_S stMbCtx;

    memset(&stMbCtx, 0, sizeof(TEST_MB_CTX_S));
    stMbCtx.s32MbCount   = 1;
    stMbCtx.s32MbSize    = 4 * 1024 * 1024;
    stMbCtx.s32LoopCount = 1;
    stMbCtx.s32PoolCount = 1;
    stMbCtx.bPreAlloc    = RK_FALSE;
    stMbCtx.s32RemapMode = MB_REMAP_MODE_CACHED;
    stMbCtx.s32AllocType = MB_ALLOC_TYPE_DMA;
    stMbCtx.ss2DmaType = MB_DMA_TYPE_NONE;

    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("basic options:"),
        OPT_INTEGER('n', "loop", &(stMbCtx.s32LoopCount), "the count of looping. default(1).", NULL, 0, 0),
        OPT_INTEGER('c', "mb_count", &(stMbCtx.s32MbCount), "the count of mb. default(1).", NULL, 0, 0),
        OPT_INTEGER('s', "mb_size", &(stMbCtx.s32MbSize), "mb size. default(4MB).", NULL, 0, 0),
        OPT_INTEGER('p', "pool_count", &(stMbCtx.s32PoolCount), "the count of pool. default(1).", NULL, 0, 0),
        OPT_INTEGER('a', "pre_alloc", &(stMbCtx.bPreAlloc), "pre alloc or not. default(0). 0: no 1: yes.", NULL, 0, 0),
        OPT_INTEGER('r', "remap_mode", &(stMbCtx.s32RemapMode),
                    "remapping mode. default(2). 0: none, 256: no cache, 512: cached", NULL, 0, 0),
        OPT_INTEGER('t', "alloc_type", &(stMbCtx.s32AllocType),
                    "alloc type. default(0). 0: DMA, 1: malloc", NULL, 0, 0),
        OPT_INTEGER('d', "dma_type", &(stMbCtx.ss2DmaType),
                    "dma type. default(0). 0: IOMMU, 1: CMA", NULL, 0, 0),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "\nselect a test case to run.",
                                 "\nuse --help for details.");

    argc = argparse_parse(&argparse, argc, argv);

    if (RK_MPI_SYS_Init() != RK_SUCCESS) {
        RK_LOGE("rk mpi sys init fail!");
        goto __FAILED;
    }

    s32Ret = unit_test_mpi_mb(&stMbCtx);
    if (s32Ret != RK_SUCCESS) {
        goto __FAILED;
    }

    RK_MPI_SYS_Exit();

    RK_LOGI("test running ok.");
    return RK_SUCCESS;
__FAILED:
    RK_LOGE("test running failed!");
    return RK_ERR_MB_BUSY;
}
