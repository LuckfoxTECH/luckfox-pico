/* GPL-2.0 WITH Linux-syscall-note OR Apache 2.0 */
/* Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd */

#ifndef INCLUDE_RT_MPI_RK_MPI_SYS_H__
#define INCLUDE_RT_MPI_RK_MPI_SYS_H__

#include "rk_comm_mb.h"
#include "rk_comm_sys.h"
#include "rk_common.h"
#include "rk_debug.h"
#include "rk_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

RK_S32 RK_MPI_SYS_Init(RK_VOID);
RK_S32 RK_MPI_SYS_Exit(RK_VOID);

RK_S32 RK_MPI_SYS_Bind(const MPP_CHN_S *pstSrcChn, const MPP_CHN_S *pstDestChn);
RK_S32 RK_MPI_SYS_UnBind(const MPP_CHN_S *pstSrcChn,
                         const MPP_CHN_S *pstDestChn);

RK_S32 RK_MPI_SYS_GetBindbyDest(const MPP_CHN_S *pstDestChn,
                                MPP_CHN_S *pstSrcChn);
RK_S32 RK_MPI_SYS_GetBindbySrc(const MPP_CHN_S *pstSrcChn,
                               MPP_BIND_DEST_S *pstBindDest);

/* alloc mmz memory in user context                                         */
RK_S32 RK_MPI_SYS_MmzAlloc(MB_BLK *pBlk, const RK_CHAR *pstrMmb,
                           const RK_CHAR *pstrZone, RK_U32 u32Len);

/* alloc mmz memory with cache */
RK_S32 RK_MPI_SYS_MmzAlloc_Cached(MB_BLK *pBlk, const RK_CHAR *pstrMmb,
                                  const RK_CHAR *pstrZone, RK_U32 u32Len);

/* alloc mmz memory with flags */
RK_S32 RK_MPI_SYS_MmzAllocEx(MB_BLK *pBlk, const RK_CHAR *pstrMmb,
                             const RK_CHAR *pstrZone, RK_U32 u32Len,
                             RK_U32 u32HeapFlags);

/* free mmz memory in user context                                          */
RK_S32 RK_MPI_SYS_MmzFree(MB_BLK blk);

/* flush cache */
RK_S32 RK_MPI_SYS_MmzFlushCache(MB_BLK blk, RK_BOOL bReadOnly);

RK_S32 RK_MPI_SYS_Malloc(MB_BLK *pBlk, RK_U32 u32Len);
RK_S32 RK_MPI_SYS_Free(MB_BLK blk);

RK_S32 RK_MPI_SYS_CreateMB(MB_BLK *pBlk, MB_EXT_CONFIG_S *pstMbExtConfig);

/*
** u64Base is the global PTS of the system.
** ADVICE:
** 1. Better to call RK_MPI_SYS_GetCurPTS on the host board to get u64Base.
** 2. When os start up, call RK_MPI_SYS_InitPTSBase to set the init PTS.
** 3. When media bussines is running, synchronize the PTS one time per minute
**    by calling RK_MPI_SYS_SyncPTS.
*/
RK_S32 RK_MPI_SYS_GetCurPTS(RK_U64 *pu64CurPTS);
RK_S32 RK_MPI_SYS_InitPTSBase(RK_U64 u64PTSBase);
RK_S32 RK_MPI_SYS_SyncPTS(RK_U64 u64PTSBase);

/* set mpp channel input stream mode */
RK_S32 RK_MPI_SYS_SetChnInputMode(const MPP_CHN_S *pstChn,
                                  CHN_INPUT_MODE_E mode);

/* log level configuration */
RK_S32 RK_MPI_LOG_SetLevelConf(LOG_LEVEL_CONF_S *pstConf);
RK_S32 RK_MPI_LOG_GetLevelConf(LOG_LEVEL_CONF_S *pstConf);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* INCLUDE_RT_MPI_RK_MPI_SYS_H__ */
