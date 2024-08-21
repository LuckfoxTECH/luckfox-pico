/* GPL-2.0 WITH Linux-syscall-note OR Apache 2.0 */
/* Copyright (c) 2023 Fuzhou Rockchip Electronics Co., Ltd */

#ifndef INCLUDE_RT_MPI_RK_COMMON_AIISP_H_
#define INCLUDE_RT_MPI_RK_COMMON_AIISP_H_

#include "rk_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/** AIISP update AINR params callback function */
typedef RK_S32 (*AIISP_CALLBACK)(RK_VOID *pAinrParam, RK_VOID *pPrivateData);

typedef struct AIISP_CALLBACK_FUNC_S {
    AIISP_CALLBACK              pfUpdateCallback;
    RK_VOID                    *pPrivateData;   /* Not yet used */
} AIISP_CALLBACK_FUNC_S;

typedef struct rkAIISP_ATTR_S {
    RK_BOOL               bEnable;              /* Whether AIISP is enable */
    AIISP_CALLBACK_FUNC_S stAiIspCallback;      /* AIISP callback function */
    const RK_CHAR        *pModelFilePath;       /* AIISP model file path   */
    RK_U32                u32FrameBufCnt;       /* RW; frame buffer cnt    */
} AIISP_ATTR_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* INCLUDE_RT_MPI_RK_COMMON_AIISP_H_ */