#ifndef __RK_ALGO_DIS_DEF_H__
#define __RK_ALGO_DIS_DEF_H__

#include "rk_algo_dis_comm.h"


#ifdef __cplusplus
extern "C" {
#endif

#define RKALGO_DIS_ORB_MAX_POINTS 40000
#define RKALGO_DIS_ORB_DESCRIPTOR_SIZE 15
#define RKALGO_DIS_ORB_FEATURE_DESCRIPTOR_BITS 120

    /*
    * @struct  RKALGO_DIS_ORB_POINT_S
    * @brief   definition of DIS ORB points struct
    * u32X          : coordinate of x-axis(horizontal)
    * u32Y          : coordinate of y-axis(vertical)
    * u8Brief       : brief descriptor
    */
    typedef struct rkAlgoDIS_ORB_POINT_S
    {
        uint32_t u32X;
        uint32_t u32Y;
        uint8_t u8Brief[RKALGO_DIS_ORB_DESCRIPTOR_SIZE];
    }RKALGO_DIS_ORB_POINT_S;

    /*
    * @struct  RKALGO_DIS_ORB_STAT_S
    * @brief   definition of DIS ORB stats data struct
    * u32OrbNum     : ORB points number
    * stOrbPoints   : ORB points data
    */
    typedef struct rkAlgoDIS_ORB_STAT_S
    {
        uint32_t u32OrbNum;
        RKALGO_DIS_ORB_POINT_S stOrbPoints[RKALGO_DIS_ORB_MAX_POINTS];
    }RKALGO_DIS_ORB_STAT_S;

    /*
    * @struct  RKALGO_DIS_CTX_S
    * @brief   definition of DIS context struct
    * pCtxData          : DIS context data
    */
    typedef struct rkAlgoDIS_CTX_S
    {
        void *pCtxData;
    }RKALGO_DIS_CTX_S;

    /*
    * @struct  RKALGO_DIS_INIT_CONFIG_S
    * @brief   definition of DIS init config params, which can only be set in DIS initialization
    * enPdtType         : product type: IPC; DV; DRONE
    * bCameraSteady     : bool value, whether camera is steady. Only work in IPC product type. In DV or Drone, is FALSE.
    * enMotionLevel     : motion level: LOW; NORMAL; HIGH
    * enDisAlgorithm    : DIS algorithm: GME_4DOF; GME_6DOF; GYRO
    * enCamStatus       : camera status: static; mobile
    * u32CropRatio      : crop ratio, range: [50, 98], unit: 0.01
    * enImgFormat       : src image format
    * u32SrcWid         : src image width
    * u32SrcHgt         : src image height
    * u32SrcStride      : src image stride
    * u32DstWid         : dst image width
    * u32DstHgt         : dst image height
    * bLevelZeroIsScale : if FALSE: u32ImgPyrWid[0]=srcW, u32ImgPyrHgt[0]=srcH; if TRUE: u32ImgPyrWid[0]=srcW/scale, u32ImgPyrHgt[0]=srcH/scale, scale>1
    * u32ImgPyrNum      : src image pyramid num, range in [1, RKALGO_DIS_MAX_PYR_NUM]
    * u32ImgPyrWid      : src image pyramid width in every level
    * u32ImgPyrHgt      : src image pyramid height in every level
    * enMeshType        : mesh type
    * u32MeshStepX      : mesh step in x direction(horizontal)
    * u32MeshStepY      : mesh step in y direction(vertical)
    * u32MeshNumX       : mesh sample num in x direction(horizontal)
    * u32MeshNumY       : mesh sample num in y direction(vertical)
    */
    typedef struct rkAlgoDIS_INIT_CONFIG_S
    {
        RKALGO_DIS_PDT_TYPE_E enPdtType;
        RKALGO_DIS_BOOL bCameraSteady;
        RKALGO_DIS_MOTION_LEVEL_E enMotionLevel;
        RKALGO_DIS_ALGORITHM enDisAlgorithm;
        uint32_t u32CropRatio;
        RKALGO_DIS_IMG_FORMAT_E enImgFormat;
        uint32_t u32SrcWid;
        uint32_t u32SrcHgt;
        uint32_t u32SrcStride;
        uint32_t u32DstWid;
        uint32_t u32DstHgt;
        RKALGO_DIS_BOOL bLevelZeroIsScale;
        uint32_t u32ImgPyrNum;
        uint32_t u32ImgPyrWid[RKALGO_DIS_PYR_NUM_MAX];
        uint32_t u32ImgPyrHgt[RKALGO_DIS_PYR_NUM_MAX];
        RKALGO_DIS_MESH_TYPE enMeshType;
        uint32_t u32MeshStepX;
        uint32_t u32MeshStepY;
        uint32_t u32MeshNumX;
        uint32_t u32MeshNumY;
    }RKALGO_DIS_INIT_CONFIG_S;

    /*
    * @struct  RKALGO_DIS_PROC_CONFIG_S
    * @brief   definition of DIS process config params, which can be dynamically changed in every frame when DIS is processing
    * enDisMode                 : DIS mode: bypass; stillcrop; DIS
    * u32CropRatio              : crop ratio, range: [50, 98], unit: 0.01
    * u32MovingObjectLevel      : moving object's motion level, range: [0, 6]
    * u32RollingShutterCoef     : rolling shutter coef, range: [0, 1000]
    * u32HorizontalLimit        : horizontal limit, range: [0, 1000], only work when bCameraSteady=true
    * u32VerticalLimit          : vertical limit, range: [0, 1000], only work when bCameraSteady=true
    */
    typedef struct rkAlgoDIS_PROC_CONFIG_S
    {
        RKALGO_DIS_MODE enDisMode;
        uint32_t u32CropRatio;
        uint32_t u32MovingObjectLevel;
        uint32_t u32RollingShutterCoef;
        uint32_t u32HorizontalLimit;
        uint32_t u32VerticalLimit;
    }RKALGO_DIS_PROC_CONFIG_S;

    /*
    * @struct  RKALGO_DIS_IMG_BUF_S
    * @brief   definition the input image buffer of DIS
    * u64ImgIdx         : image index
    * s32ImgFd          : file descriptor
    * u32ImgStrideWid   : stride of src image buffer in width
    * u32ImgStrideHgt   : stride of src image buffer in height
    * pu8ImgData        : src image data addr
    * pstOrbStat        : ORB stats data of src image
    */
    typedef struct rkAlgoDIS_IMG_BUF_S
    {
        uint64_t u64ImgIdx;
        int32_t s32ImgFd;
        uint32_t u32ImgStrideWid;
        uint32_t u32ImgStrideHgt;
        uint8_t *pu8ImgData[RKALGO_DIS_PYR_NUM_MAX];
        RKALGO_DIS_ORB_STAT_S *pstOrbStat;
    }RKALGO_DIS_IMG_BUF_S;

    /*
    * @struct  RKALGO_DIS_MESH_BUF_S
    * @brief   definition the output mesh params and buffer of DIS
    * u64MeshIdx        : mesh index
    * pfMeshX           : GPU/DEBUG mesh: x data
    * pfMeshY           : GPU/DEBUG mesh: y data
    * enFecVersion      : FEC mesh: the version of FEC
    * u32FecMeshNumXI   : FEC mesh: sample num of x integer
    * u32FecMeshNumYI   : FEC mesh: sample num of y integer
    * u32FecMeshNumXF   : FEC mesh: sample num of x fraction
    * u32FecMeshNumYF   : FEC mesh: sample num of y fraction
    * pu16FecMeshXI     : FEC mesh: x integer part
    * pu16FecMeshYI     : FEC mesh: y integer part
    * pu8FecMeshXF      : FEC mesh: x fraction part
    * pu8FecMeshYF      : FEC mesh: y fraction part
    */
    typedef struct rkAlgoDIS_MESH_BUF_S
    {
        uint64_t u64MeshIdx;
        /* for GPU or DEBUG or FEC float mesh */
        float *pfMeshX;
        float *pfMeshY;
        /* for FEC mesh */
        RKALGO_DIS_FEC_VER_E enFecVersion;
        uint32_t u32FecMeshNumXI;
        uint32_t u32FecMeshNumYI;
        uint32_t u32FecMeshNumXF;
        uint32_t u32FecMeshNumYF;
        uint16_t *pu16FecMeshXI;
        uint16_t *pu16FecMeshYI;
        uint8_t  *pu8FecMeshXF;
        uint8_t  *pu8FecMeshYF;
    }RKALGO_DIS_MESH_BUF_S;

    /*
    * @struct  RKALGO_DIS_FEC_SIZE_IN_S
    * @brief   definition the config of computing FEC mesh buffer size
    * u32DstImgWid      : dst image width
    * u32DstImgHgt      : dst image height
    * enFecVersion      : FEC version
    */
    typedef struct rkAlgoDIS_FEC_SIZE_IN_S
    {
        uint32_t u32DstImgWid;
        uint32_t u32DstImgHgt;
        RKALGO_DIS_FEC_VER_E enFecVersion;
    }RKALGO_DIS_FEC_SIZE_IN_S;

    /*
    * @struct  RKALGO_DIS_FEC_SIZE_OUT_S
    * @brief   definition the output of computing FEC mesh buffer size
    * u32MeshStepX      : mesh step in x direction(horizontal)
    * u32MeshStepY      : mesh step in y direction(vertical)
    * u32MeshNumX       : mesh sample num in x direction(horizontal)
    * u32MeshNumY       : mesh sample num in y direction(vertical)
    * u64FecMeshSize    : the mesh size of every FEC mesh buffer(meshxi, meshxf, meshyi, meshyf)
    */
    typedef struct rkAlgoDIS_FEC_SIZE_OUT_S
    {
        uint32_t u32MeshStepX;
        uint32_t u32MeshStepY;
        uint32_t u32MeshNumX;
        uint32_t u32MeshNumY;
        uint64_t u64FecMeshSize;
    }RKALGO_DIS_FEC_SIZE_OUT_S;

    /*
    * @struct  RKALGO_DIS_FEC_BASE_MESH_IN_S
    * @brief   definition the config of generating FEC base mesh, with cropRatio, zero DIS effect
    * u32CropRatio      : crop ratio, range: [50, 98], unit: 0.01
    * u32DstImgWid      : dst image width
    * u32DstImgHgt      : dst image height
    * enFecVersion      : FEC version
    */
    typedef struct rkAlgoDIS_FEC_BASE_MESH_IN_S
    {
        uint32_t u32CropRatio;
        uint32_t u32DstImgWid;
        uint32_t u32DstImgHgt;
        RKALGO_DIS_FEC_VER_E enFecVersion;
    }RKALGO_DIS_FEC_BASE_MESH_IN_S;

    /*
    * @struct  RKALGO_DIS_FEC_BASE_MESH_OUT_S
    * @brief   definition the output of generating FEC base mesh, with cropRatio, zero DIS effect
    * pu16MeshXI        : FEC mesh: x integer part
    * pu16MeshYI        : FEC mesh: y integer part
    * pu8MeshXF         : FEC mesh: x fraction part
    * pu8MeshYF         : FEC mesh: y fraction part
    */
    typedef struct rkAlgoDIS_FEC_BASE_MESH_OUT_S
    {
        uint16_t *pu16FecMeshXI;
        uint16_t *pu16FecMeshYI;
        uint8_t  *pu8FecMeshXF;
        uint8_t  *pu8FecMeshYF;
    }RKALGO_DIS_FEC_BASE_MESH_OUT_S;


    /* ==============for async interface==============*/

    typedef int32_t(*RKALGO_DIS_CALL_BACK) (RKALGO_DIS_CTX_S *pstDisCtx, RKALGO_DIS_MESH_BUF_S *pstMeshBuf);

    /*
    * @struct  RKALGO_DIS_ASYNC_CALL_BACK_S
    * @brief   definition the call back functions for the input of async interface
    * funcCallBackMesh        : call back function
    */
    typedef struct rkAlgoDIS_ASYNC_CALL_BACK_S
    {
        RKALGO_DIS_CALL_BACK funcCallBackImgMeshBuf;
    }RKALGO_DIS_ASYNC_CALL_BACK_S;

#ifdef __cplusplus
} /* extern "C" { */
#endif


#endif // !__RK_ALGO_DIS_DEF_H__