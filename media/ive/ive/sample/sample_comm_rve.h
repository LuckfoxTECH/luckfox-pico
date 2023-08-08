/*
 * Copyright 2020 Rockchip Electronics Co. LTD
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
 *
 */
#ifndef __SAMPLE_COMM_RVE_H__
#define __SAMPLE_COMM_RVE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

//#include "sample_comm.h"
#include <rk_comm_ive.h>
#include <rk_mpi_ive.h>
#include "sample_comm_mem.h"

#define VIDEO_WIDTH 352
#define VIDEO_HEIGHT 288
#define IVE_ALIGN_VAL 16
#define IVE_CHAR_CALW 8
#define IVE_CHAR_CALH 8
#define IVE_CHAR_NUM (IVE_CHAR_CALW *IVE_CHAR_CALH)
#define IVE_FILE_NAME_LEN 256
#define IVE_RECT_NUM   20
#define VPSS_CHN_NUM 2

#define SAMPLE_ALIGN_BACK(x, a)     ((a) * (((x) / (a))))

typedef struct rkPOINT_S
{
    RK_S32 s32X;
    RK_S32 s32Y;
} POINT_S;

typedef struct rkSAMPLE_IVE_SWITCH_S
{
   bool bVenc;
   bool bVo;
}SAMPLE_IVE_SWITCH_S;

typedef struct rkSAMPLE_IVE_RECT_S
{
    POINT_S astPoint[4];
} SAMPLE_IVE_RECT_S;

typedef struct rkSAMPLE_RECT_ARRAY_S
{
    RK_U16 u16Num;
    SAMPLE_IVE_RECT_S astRect[IVE_RECT_NUM];
} SAMPLE_RECT_ARRAY_S;

typedef struct rkIVE_LINEAR_DATA_S
{
    RK_S32 s32LinearNum;
    RK_S32 s32ThreshNum;
    POINT_S* pstLinearPoint;
} IVE_LINEAR_DATA_S;

typedef struct rkVIDEO_FRAME_S
{
    RK_U32              u32Width;
    RK_U32              u32Height;
    RK_U64 u64PhyAddr[3];
    RK_U64 u64VirAddr[3];
    RK_U32 u32Stride[3];
} VIDEO_FRAME_S;

typedef struct rkVIDEO_FRAME_INFO_S
{
    VIDEO_FRAME_S stVFrame;
    RK_U32           u32PoolId;
    RK_U8            enModId;
} VIDEO_FRAME_INFO_S;

typedef struct rkSAMPLE_IVE_DRAW_RECT_MSG_S
{
    VIDEO_FRAME_INFO_S stFrameInfo;
    SAMPLE_RECT_ARRAY_S stRegion;
} SAMPLE_IVE_DRAW_RECT_MSG_S;

#ifndef SAMPLE_PRT
#define SAMPLE_PRT(fmt...)                           \
    do {                                             \
        printf("[%s]-%d: ", __FUNCTION__, __LINE__); \
        printf(fmt);                                 \
    } while (0)
#endif

#ifndef PAUSE
#define PAUSE()                                                             \
    do {                                                                    \
        printf("---------------press Enter key to exit!---------------\n"); \
        getchar();                                                          \
    } while (0)
#endif

//free mmz
#define IVE_MMZ_FREE(phy,vir)\
    do{\
        if ((0 != (vir)))\
        {\
            MmzFree((void *)(vir));\
            (vir) = 0;\
        }\
    }while(0)

#define IVE_CLOSE_FILE(fp)\
    do{\
        if (NULL != (fp))\
        {\
            fclose((fp));\
            (fp) = NULL;\
        }\
    }while(0)

#define SAMPLE_PAUSE()\
    do {\
        printf("---------------press Enter key to exit!---------------\n");\
        (void)getchar();\
    } while (0)
#define SAMPLE_CHECK_EXPR_RET(expr, ret, fmt...)\
do\
{\
    if(expr)\
    {\
        SAMPLE_PRT(fmt);\
        return (ret);\
    }\
}while(0)
#define SAMPLE_CHECK_EXPR_GOTO(expr, label, fmt...)\
do\
{\
    if(expr)\
    {\
        SAMPLE_PRT(fmt);\
        goto label;\
    }\
}while(0)

#define SAMPLE_COMM_IVE_CONVERT_64BIT_ADDR(Type,Addr) (Type*)(Addr)

/******************************************************************************
* function : Mpi init
******************************************************************************/
void SAMPLE_COMM_IVE_CheckIveMpiInit(void);
/******************************************************************************
* function : Mpi exit
******************************************************************************/
RK_S32 SAMPLE_COMM_IVE_IveMpiExit(void);
/******************************************************************************
* function :Read file
******************************************************************************/
RK_S32 SAMPLE_COMM_IVE_ReadFile(RK_CHAR *filename, IVE_IMAGE_S* pstImg, FILE **fp_out);
/******************************************************************************
* function :Write file
******************************************************************************/
RK_S32 SAMPLE_COMM_IVE_WriteFile(RK_CHAR *filename, IVE_IMAGE_S* pstImg, FILE **fp_out);
/******************************************************************************
* function :Calc stride
******************************************************************************/
RK_U16 SAMPLE_COMM_IVE_CalcStride(RK_U32 u32Width, RK_U8 u8Align);
/******************************************************************************
* function : Copy blob to rect
******************************************************************************/
void SAMPLE_COMM_IVE_BlobToRect(IVE_CCBLOB_S *pstBlob, SAMPLE_RECT_ARRAY_S *pstRect,
                                            RK_U16 u16RectMaxNum,RK_U16 u16AreaThrStep,
                                            RK_U32 u32SrcWidth, RK_U32 u32SrcHeight,
                                            RK_U32 u32DstWidth,RK_U32 u32DstHeight);
/******************************************************************************
* function : Create ive image
******************************************************************************/
RK_S32 SAMPLE_COMM_IVE_CreateImage(IVE_IMAGE_S* pstImg, IVE_IMAGE_TYPE_E enType,
                                   RK_U32 u32Width, RK_U32 u32Height);
/******************************************************************************
* function : Create memory info
******************************************************************************/
RK_S32 SAMPLE_COMM_IVE_CreateMemInfo(IVE_MEM_INFO_S* pstMemInfo, RK_U32 u32Size);
/******************************************************************************
* function : Create ive image by cached
******************************************************************************/
RK_S32 SAMPLE_COMM_IVE_CreateImageByCached(IVE_IMAGE_S* pstImg,
        IVE_IMAGE_TYPE_E enType, RK_U32 u32Width, RK_U32 u32Height);
/******************************************************************************
* function : Create IVE_DATA_S
******************************************************************************/
RK_S32 SAMPLE_COMM_IVE_CreateData(IVE_DATA_S* pstData,RK_U32 u32Width, RK_U32 u32Height);

/******************************************************************************
* function : Dma frame info to  ive image
******************************************************************************/
RK_S32 SAMPLE_COMM_IVE_DmaImage(VIDEO_FRAME_INFO_S *pstFrameInfo,IVE_DST_IMAGE_S *pstDst,bool bInstant);

/******************************************************************************
* function : histogram sample
******************************************************************************/
void SAMPLE_IVE_TestHist(void);
void SAMPLE_IVE_TestHist_HandleSig(void);
/******************************************************************************
* function : object tracking sample
******************************************************************************/
void SAMPLE_IVE_Object_Tracking(void);
void SAMPLE_IVE_Object_Tracking_HandleSig(void);
#endif


