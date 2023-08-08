/* GPL-2.0 WITH Linux-syscall-note OR Apache 2.0 */
/* Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd */

#ifndef INCLUDE_RT_MPI_RK_COMM_MB_H_
#define INCLUDE_RT_MPI_RK_COMM_MB_H_

#include "rk_errno.h"
#include "rk_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef enum rkMB_UID_E {
  MB_UID_VI = 0,
  MB_UID_VO = 1,
  MB_UID_VGS = 2,
  MB_UID_VENC = 3,
  MB_UID_VDEC = 4,
  MB_UID_VPSS = 5,
  MB_UID_AI = 6,
  MB_UID_AENC = 7,
  MB_UID_ADEC = 8,
  MB_UID_AVS = 9,
  MB_UID_BUTT = 10
} MB_UID_E;

#define MB_INVALID_POOLID (-1U)
#define MB_INVALID_HANDLE NULL

#define MB_MAX_COMM_POOLS 16
#define MB_MAX_MOD_COMM_POOLS 16

#define MB_MAX_COUNT 10240

/* Generall common pool use this owner id, module common pool use VB_UID as
 * owner id */
#define POOL_OWNER_COMMON -1

/* Private pool use this owner id */
#define POOL_OWNER_PRIVATE -2

typedef RK_U32 MB_POOL;
typedef void *MB_BLK;

typedef enum rkMB_BUF_SYNC_TYPE_E {
  MB_BUF_SYNC_TYPE_READ = 0,
  MB_BUF_SYNC_TYPE_WRITE,
  MB_BUF_SYNC_TYPE_RW,
} MB_BUF_SYNC_TYPE_E;

typedef enum rkMB_ALLOC_TYPE {
  MB_ALLOC_TYPE_UNUSED = -1,
  MB_ALLOC_TYPE_DMA = 0,
  MB_ALLOC_TYPE_MALLOC,
  MB_ALLOC_TYPE_MAX,
} MB_ALLOC_TYPE_E;

typedef enum rkMB_REMAP_MODE_E {
  MB_REMAP_MODE_NONE = 0,         /* no remap */
  MB_REMAP_MODE_NOCACHE = 1 << 8, /* no cache remap */
  MB_REMAP_MODE_CACHED = 1 << 9,  /* cache remap, if you use this mode, you
                                     should flush cache by yourself */
  MB_REMAP_MODE_BUTT
} MB_REMAP_MODE_E;

typedef enum rkMB_DMA_TYPE_E {
  MB_DMA_TYPE_NONE = 0,      /* Physically Non-Continuous memory default */
  MB_DMA_TYPE_CMA = 1 << 12, /* Physically Continuous memory */
  MB_DMA_TYPE_BUTT
} MB_DMA_TYPE_E;

typedef struct rkMB_POOL_CONFIG_S {
  RK_U64 u64MBSize;
  RK_U32 u32MBCnt;
  MB_REMAP_MODE_E enRemapMode;
  MB_ALLOC_TYPE_E enAllocType;
  MB_DMA_TYPE_E enDmaType;
  RK_BOOL bPreAlloc;
} MB_POOL_CONFIG_S;

typedef struct rkMB_CONFIG_S {
  RK_U32 u32MaxPoolCnt;
  MB_POOL_CONFIG_S astCommPool[MB_MAX_COMM_POOLS];
} MB_CONFIG_S;

typedef RK_S32 (*RK_MPI_MB_FREE_CB)(void *);

typedef struct _rkMB_EXT_CONFIG_S {
  RK_U8 *pu8VirAddr;
  RK_U64 u64PhyAddr;
  RK_S32 s32Fd;
  RK_U64 u64Size;
  RK_MPI_MB_FREE_CB pFreeCB;
  RK_VOID *pOpaque;
} MB_EXT_CONFIG_S;

#define RK_ERR_MB_NULL_PTR                                                     \
  RK_DEF_ERR(RK_ID_MB, RK_ERR_LEVEL_ERROR, RK_ERR_NULL_PTR)
#define RK_ERR_MB_NOMEM RK_DEF_ERR(RK_ID_MB, RK_ERR_LEVEL_ERROR, RK_ERR_NOMEM)
#define RK_ERR_MB_NOBUF RK_DEF_ERR(RK_ID_MB, RK_ERR_LEVEL_ERROR, RK_ERR_NOBUF)
#define RK_ERR_MB_UNEXIST                                                      \
  RK_DEF_ERR(RK_ID_MB, RK_ERR_LEVEL_ERROR, RK_ERR_UNEXIST)
#define RK_ERR_MB_ILLEGAL_PARAM                                                \
  RK_DEF_ERR(RK_ID_MB, RK_ERR_LEVEL_ERROR, RK_ERR_ILLEGAL_PARAM)
#define RK_ERR_MB_NOTREADY                                                     \
  RK_DEF_ERR(RK_ID_MB, RK_ERR_LEVEL_ERROR, RK_ERR_NOTREADY)
#define RK_ERR_MB_BUSY RK_DEF_ERR(RK_ID_MB, RK_ERR_LEVEL_ERROR, RK_ERR_BUSY)
#define RK_ERR_MB_NOT_PERM                                                     \
  RK_DEF_ERR(RK_ID_MB, RK_ERR_LEVEL_ERROR, RK_ERR_NOT_PERM)
#define RK_ERR_MB_SIZE_NOT_ENOUGH                                              \
  RK_DEF_ERR(RK_ID_MB, RK_ERR_LEVEL_ERROR, RK_ERR_SIZE_NOT_ENOUGH)

#define RK_ERR_MB_2MPOOLS                                                      \
  RK_DEF_ERR(RK_ID_MB, RK_ERR_LEVEL_ERROR, RK_ERR_BUTT + 1)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* INCLUDE_RT_MPI_RK_COMM_MB_H_ */
