#ifndef __RK_ALGO_DIS_API_H__
#define __RK_ALGO_DIS_API_H__

#include "rk_algo_dis_def.h"


#ifdef __cplusplus
extern "C" {
#endif

#if defined _WIN32 || defined __CYGWIN__
#ifdef BUILDING_DLL
#ifdef __GNUC__
#define DLL_PUBLIC __attribute__ ((dllexport))
#else
    // Note: actually gcc seems to also supports this syntax.
#define DLL_PUBLIC __declspec(dllexport)
#endif
#else
#ifdef __GNUC__
#define DLL_PUBLIC __attribute__ ((dllimport))
#else
    // Note: actually gcc seems to also supports this syntax.
#define DLL_PUBLIC
#endif
#define DLL_LOCAL
#endif
#else
#if __GNUC__ >= 4
#define DLL_PUBLIC __attribute__ ((visibility ("default")))
#define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define DLL_PUBLIC
#define DLL_LOCAL
#endif
#endif

    /* ==============for sync interface==============*/
    DLL_PUBLIC int32_t RKALGO_DIS_Init(RKALGO_DIS_CTX_S *pstDisCtx, RKALGO_DIS_INIT_CONFIG_S *pstInitConfig);
    DLL_PUBLIC int32_t RKALGO_DIS_Process(RKALGO_DIS_CTX_S *pstDisCtx, RKALGO_DIS_PROC_CONFIG_S *pstProcConfig, RKALGO_DIS_IMG_BUF_S *pstImgBuf, RKALGO_DIS_MESH_BUF_S *pstMeshBuf);
    DLL_PUBLIC int32_t RKALGO_DIS_DeInit(RKALGO_DIS_CTX_S *pstDisCtx);
    DLL_PUBLIC int32_t RKALGO_DIS_GetFecMeshBufferSize(RKALGO_DIS_FEC_SIZE_IN_S *pstFecSizeIn, RKALGO_DIS_FEC_SIZE_OUT_S *pstFecSizeOut);                   /* compute the FEC mesh buffer size */
    DLL_PUBLIC int32_t RKALGO_DIS_GetFecBaseMesh(RKALGO_DIS_FEC_BASE_MESH_IN_S *pstFecBaseMeshIn, RKALGO_DIS_FEC_BASE_MESH_OUT_S *pstFecBaseMeshOut);       /* generate the FEC base mesh, with cropRatio, zero DIS effect */



    /* ==============for async interface==============*/
    DLL_PUBLIC int32_t RKALGO_DIS_ASYNC_Init(RKALGO_DIS_CTX_S *pstDisCtx, RKALGO_DIS_INIT_CONFIG_S *pstInitConfig, RKALGO_DIS_ASYNC_CALL_BACK_S *pstCallBack);
    DLL_PUBLIC int32_t RKALGO_DIS_ASYNC_Start(RKALGO_DIS_CTX_S *pstDisCtx);
    DLL_PUBLIC int32_t RKALGO_DIS_ASYNC_PushImgData(RKALGO_DIS_CTX_S *pstDisCtx, RKALGO_DIS_PROC_CONFIG_S *pstProcConfig, RKALGO_DIS_IMG_BUF_S *pstImgBuf);
    DLL_PUBLIC int32_t RKALGO_DIS_ASYNC_PushMeshBuf(RKALGO_DIS_CTX_S *pstDisCtx, RKALGO_DIS_MESH_BUF_S *pstMeshBuf);
    DLL_PUBLIC int32_t RKALGO_DIS_ASYNC_Stop(RKALGO_DIS_CTX_S *pstDisCtx);
    DLL_PUBLIC int32_t RKALGO_DIS_ASYNC_DeInit(RKALGO_DIS_CTX_S *pstDisCtx);

#ifdef __cplusplus
} /* extern "C" { */
#endif


#endif // !__RK_ALGO_DIS_API_H__