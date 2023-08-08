#include <ctype.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "rk_mpi_ive.h"
#include "sample_comm_rve.h"

#define min(a, b)  (((a) < (b)) ? (a) : (b))
#define max(a, b)  (((a) > (b)) ? (a) : (b)) 

static bool mSignalExit = false;

typedef struct rkTRACKING_OBJECT_S {
    int x;
    int y;
    int width;
    int height;
    IVE_IMAGE_S stImg;
} TRACKING_OBJECT_S;

typedef struct rkSAMPLE_IVE_OBJECT_TRACKING_S {
    IVE_IMAGE_S stSrcImg;
    IVE_IMAGE_S stDstImg;
    IVE_IMAGE_S stRefTileImg;
    IVE_IMAGE_S stDstTileImg;
    RK_U32 tileWidth;
    RK_U32 tileHeight;
    IVE_MEM_INFO_S stRefCorner;
    IVE_MEM_INFO_S stDstCorner;
    IVE_MEM_INFO_S stCandiCorner;
    IVE_MEM_INFO_S stNCCDst;
    TRACKING_OBJECT_S stTrackingObject;
    IVE_ST_CANDI_CORNER_CTRL_S stStCandiCornerCtrl;
    IVE_ST_CORNER_CTRL_S stStCornerCtrl;
}SAMPLE_IVE_OBJECT_TRACKING_S;

static SAMPLE_IVE_OBJECT_TRACKING_S mOTAlgoCtrl = {0};

RK_S32 SAMPLE_IVE_Object_Tracking_Uninit(SAMPLE_IVE_OBJECT_TRACKING_S *pAlgoCtrl) {
    RK_S32 s32Result = RK_SUCCESS;

    IVE_MMZ_FREE(pAlgoCtrl->stTrackingObject.stImg.au64PhyAddr[0], pAlgoCtrl->stTrackingObject.stImg.au64VirAddr[0]);
    IVE_MMZ_FREE(pAlgoCtrl->stSrcImg.au64PhyAddr[0], pAlgoCtrl->stSrcImg.au64VirAddr[0]);
    IVE_MMZ_FREE(pAlgoCtrl->stDstImg.au64PhyAddr[0], pAlgoCtrl->stDstImg.au64VirAddr[0]);

    IVE_MMZ_FREE(pAlgoCtrl->stRefCorner.u64PhyAddr, pAlgoCtrl->stRefCorner.u64VirAddr);
    IVE_MMZ_FREE(pAlgoCtrl->stDstCorner.u64PhyAddr, pAlgoCtrl->stDstCorner.u64VirAddr);
    IVE_MMZ_FREE(pAlgoCtrl->stCandiCorner.u64PhyAddr, pAlgoCtrl->stCandiCorner.u64VirAddr);
    IVE_MMZ_FREE(pAlgoCtrl->stNCCDst.u64PhyAddr, pAlgoCtrl->stNCCDst.u64VirAddr);

    return s32Result;
}

RK_S32 SAMPLE_IVE_Object_Tracking_Init(SAMPLE_IVE_OBJECT_TRACKING_S *pAlgoCtrl, RK_U32 u32Width, RK_U32 u32Height) {
    RK_S32 s32Result = RK_SUCCESS;

    s32Result = SAMPLE_COMM_IVE_CreateImage(&pAlgoCtrl->stTrackingObject.stImg, IVE_IMAGE_TYPE_U8C1, pAlgoCtrl->stTrackingObject.width, pAlgoCtrl->stTrackingObject.height);
    SAMPLE_CHECK_EXPR_GOTO(s32Result == RK_FAILURE, INIT_FAILURE, "failed to create image");

    s32Result = SAMPLE_COMM_IVE_CreateImage(&pAlgoCtrl->stSrcImg, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(s32Result == RK_FAILURE, INIT_FAILURE, "failed to create image");

    s32Result = SAMPLE_COMM_IVE_CreateImage(&pAlgoCtrl->stDstImg, IVE_IMAGE_TYPE_U8C1, pAlgoCtrl->stTrackingObject.width, pAlgoCtrl->stTrackingObject.height);
    SAMPLE_CHECK_EXPR_GOTO(s32Result == RK_FAILURE, INIT_FAILURE, "failed to create image");

    pAlgoCtrl->stRefTileImg.u32Width = pAlgoCtrl->tileWidth;
    pAlgoCtrl->stRefTileImg.u32Height = pAlgoCtrl->tileHeight;
    pAlgoCtrl->stRefTileImg.au32Stride[0] = pAlgoCtrl->tileWidth;

    pAlgoCtrl->stDstTileImg.u32Width = pAlgoCtrl->tileWidth;
    pAlgoCtrl->stDstTileImg.u32Height = pAlgoCtrl->tileHeight;
    pAlgoCtrl->stDstTileImg.au32Stride[0] = pAlgoCtrl->tileWidth;

    /* shitomasi corner */
    pAlgoCtrl->stCandiCorner.u32Size = pAlgoCtrl->stTrackingObject.width * pAlgoCtrl->stTrackingObject.height * sizeof(RK_U16) + sizeof(IVE_ST_CANDI_STACK_SIZE_S);
    s32Result = SAMPLE_COMM_IVE_CreateMemInfo(&pAlgoCtrl->stCandiCorner, pAlgoCtrl->stCandiCorner.u32Size);
    SAMPLE_CHECK_EXPR_GOTO(s32Result == RK_FAILURE, INIT_FAILURE, "failed to create image");

    pAlgoCtrl->stStCornerCtrl.stMem.u32Size = pAlgoCtrl->stTrackingObject.width * pAlgoCtrl->stTrackingObject.height + sizeof(IVE_ST_CORNER_MEM_S) * 2;
    s32Result = SAMPLE_COMM_IVE_CreateMemInfo(&pAlgoCtrl->stStCornerCtrl.stMem, pAlgoCtrl->stStCornerCtrl.stMem.u32Size);
    SAMPLE_CHECK_EXPR_GOTO(s32Result == RK_FAILURE, INIT_FAILURE, "failed to create image");

    pAlgoCtrl->stRefCorner.u32Size = sizeof(IVE_ST_CORNER_INFO_S);
    s32Result = SAMPLE_COMM_IVE_CreateMemInfo(&pAlgoCtrl->stRefCorner, pAlgoCtrl->stRefCorner.u32Size);
    SAMPLE_CHECK_EXPR_GOTO(s32Result == RK_FAILURE, INIT_FAILURE, "failed to create image");

    pAlgoCtrl->stDstCorner.u32Size = sizeof(IVE_ST_CORNER_INFO_S);
    s32Result = SAMPLE_COMM_IVE_CreateMemInfo(&pAlgoCtrl->stDstCorner, pAlgoCtrl->stDstCorner.u32Size);
    SAMPLE_CHECK_EXPR_GOTO(s32Result == RK_FAILURE, INIT_FAILURE, "failed to create image");

    /* NCC dest mem */
    pAlgoCtrl->stNCCDst.u32Size = sizeof(IVE_NCC_DST_MEM_S);
    s32Result = SAMPLE_COMM_IVE_CreateMemInfo(&pAlgoCtrl->stNCCDst, pAlgoCtrl->stNCCDst.u32Size);
    SAMPLE_CHECK_EXPR_GOTO(s32Result == RK_FAILURE, INIT_FAILURE, "failed to create image");

    pAlgoCtrl->stStCandiCornerCtrl.u0q8QualityLevel = 25;
    pAlgoCtrl->stStCornerCtrl.u16MaxCornerNum = 20;
    pAlgoCtrl->stStCornerCtrl.u16MinDist = 10;

INIT_FAILURE:
    if (RK_SUCCESS != s32Result)
    {
        SAMPLE_IVE_Object_Tracking_Uninit(pAlgoCtrl);
    }
    return s32Result;
}

RK_S32 gfx_draw_rect(const char* name, void* data, int width, int height, int orix, int oriy, int rectw, int recth, int type, int show_result) {
    int i, j;
    int startpos = oriy * width + orix;
    unsigned char* imgdata = (unsigned char*)data;

    for (j = 0; j < recth; j++) {
        for (i = 0; i < rectw; i++) {
            int index = j * width + i;
            if (j == 0 || j == recth - 1) {
                imgdata[startpos + index] = 255;
            } else {
                if (i == 0 || i == rectw - 1) {
                    imgdata[startpos + index] = 255;
                }
            }
        }
    }

    return 0;
}

RK_S32 GET_TILE_IMAGE_FROM_START_POS(IVE_IMAGE_S *tileImg, IVE_IMAGE_S *srcImg, RK_S32 sx, RK_S32 sy) {
    int j;
    int offset = sy * srcImg->au32Stride[0] + sx;
    int region_height = min(tileImg->u32Height, srcImg->u32Height - 1 - sy);
    int region_width = min(tileImg->u32Width, srcImg->u32Width - 1 - sx);

    for (j = 0; j < region_height; j++) {
        int step_offset_tile = tileImg->u32Width * j;
        int step_offset_src = srcImg->u32Width * j + offset;
        memcpy((RK_U8*)tileImg->au64VirAddr[0] + step_offset_tile,
                (RK_U8*)srcImg->au64VirAddr[0] + step_offset_src,
                region_width);
    }

    return 0;
}

RK_S32 GET_TILE_IMAGE_FROM_CENTER_POS(IVE_IMAGE_S *tileImg, IVE_IMAGE_S *srcImg, RK_S32 cx, RK_S32 cy) {
    int j;
    int region_x = max(0, cx - tileImg->u32Width / 2);
    int region_y = max(0, cy - tileImg->u32Height / 2);
    int offset = region_y * srcImg->au32Stride[0] + region_x;

    tileImg->au64VirAddr[0] = (RK_U64)((RK_U8*)srcImg->au64VirAddr[0] + offset);
    tileImg->au64PhyAddr[0] = (RK_U64)((RK_U8*)srcImg->au64PhyAddr[0] + offset);
    tileImg->au32Stride[0] = srcImg->au32Stride[0];

    return 0;
}

RK_S32 SAMPLE_IVE_Object_Tracking_Corner(SAMPLE_IVE_OBJECT_TRACKING_S *pAlgoCtrl, IVE_IMAGE_S *stImg, IVE_MEM_INFO_S *stCorner, RK_U32 u32Width, RK_U32 u32Height) {
    int i;
    RK_U32 *pstCandiCornerInfo;

    IVE_HANDLE hIveHandle;
    RK_BOOL bInstant = RK_TRUE;
    RK_S32 s32Result;

    // step1: calc candidate corners
    s32Result = RK_MPI_IVE_STCandiCorner(&hIveHandle, stImg, &pAlgoCtrl->stCandiCorner,
                                         &pAlgoCtrl->stStCandiCornerCtrl, bInstant);

    SAMPLE_CHECK_EXPR_GOTO(s32Result == RK_FAILURE, FAILURE, "failed to run RK_MPI_IVE_STCandiCorner");

    pstCandiCornerInfo = (RK_U32 *)pAlgoCtrl->stCandiCorner.u64VirAddr;

    // step2: filter candiate corners
    s32Result = RK_MPI_IVE_STCorner(&hIveHandle, stImg, &pAlgoCtrl->stCandiCorner,
                                    stCorner, &pAlgoCtrl->stStCornerCtrl, bInstant);
    SAMPLE_CHECK_EXPR_GOTO(s32Result == RK_FAILURE, FAILURE, "failed to run RK_MPI_IVE_STCorner");

FAILURE:
    if (RK_SUCCESS != s32Result)
    {
        SAMPLE_IVE_Object_Tracking_Uninit(pAlgoCtrl);
    }
    return s32Result;
}

RK_S32 SAMPLE_IVE_Object_Tracking_Proc(SAMPLE_IVE_OBJECT_TRACKING_S *pAlgoCtrl, RK_U32 u32Width, RK_U32 u32Height) {
    RK_S32 s32Result = RK_SUCCESS;
    RK_S32 i, j, m, n, k;
    IVE_HANDLE hIveHandle;
    RK_U64 *numerator;
    RK_U64 *quadSum1;
    RK_U64 *quadSum2;
    RK_FLOAT fR, fRMax = 0, fRStep = 0.02, fRThresh = 0.985, fRPassThresh = 0.996;
    IVE_POINT_U16_S match_point = {0};
    IVE_POINT_U16_S match_pattern[200][2] = {0};
    RK_S16 match_pattern_num = 0;
    IVE_MV_S16_S mv = {0};
    RK_S32 objw, objh;
    RK_S32 ssx, ssy;
    RK_S32 ss_boarder = 6;
    RK_S32 search_times = 0;
    double similarity, similarity_extreme = 1;
    bool findMatch = false;

    objw = pAlgoCtrl->stTrackingObject.stImg.u32Width;
    objh = pAlgoCtrl->stTrackingObject.stImg.u32Height;

    SAMPLE_IVE_Object_Tracking_Corner(pAlgoCtrl, &pAlgoCtrl->stTrackingObject.stImg, &pAlgoCtrl->stRefCorner, pAlgoCtrl->stTrackingObject.width, pAlgoCtrl->stTrackingObject.height);
    IVE_ST_CORNER_INFO_S *pstRefCornerInfo = (IVE_ST_CORNER_INFO_S *)pAlgoCtrl->stRefCorner.u64VirAddr;

    SAMPLE_IVE_Object_Tracking_Corner(pAlgoCtrl, &pAlgoCtrl->stDstImg, &pAlgoCtrl->stDstCorner, pAlgoCtrl->stTrackingObject.width, pAlgoCtrl->stTrackingObject.height);
    IVE_ST_CORNER_INFO_S *pstDstCornerInfo = (IVE_ST_CORNER_INFO_S *)pAlgoCtrl->stDstCorner.u64VirAddr;

    printf("tracking obj corner num: %d\n", pstRefCornerInfo->u16CornerNum);
    printf("current frame corner num: %d\n", pstDstCornerInfo->u16CornerNum);

search_again:
    for (i = 0; i < pstRefCornerInfo->u16CornerNum; i++) {
        int x0 = pstRefCornerInfo->astCorner[i].u16X;
        int y0 = pstRefCornerInfo->astCorner[i].u16Y;
        s32Result = GET_TILE_IMAGE_FROM_CENTER_POS(&pAlgoCtrl->stRefTileImg, &pAlgoCtrl->stTrackingObject.stImg, x0, y0);
        if (s32Result < 0) {
            continue;
        }

        fRMax = 0;
        findMatch = false;
        for (j = 0; j < pstDstCornerInfo->u16CornerNum; j++) {
            int x1 = pstDstCornerInfo->astCorner[j].u16X;
            int y1 = pstDstCornerInfo->astCorner[j].u16Y;
            s32Result = GET_TILE_IMAGE_FROM_CENTER_POS(&pAlgoCtrl->stDstTileImg, &pAlgoCtrl->stDstImg, x1, y1);
            if (s32Result < 0) {
                continue;
            }

            /* calc image ncc value*/
            s32Result = RK_MPI_IVE_NCC(&hIveHandle, &pAlgoCtrl->stRefTileImg, &pAlgoCtrl->stDstTileImg, &pAlgoCtrl->stNCCDst, RK_TRUE);

            numerator = (RK_U64 *)pAlgoCtrl->stNCCDst.u64VirAddr;
            quadSum1 = (RK_U64 *)(pAlgoCtrl->stNCCDst.u64VirAddr + sizeof(RK_U64));
            quadSum2 = quadSum1 + 1;

            fR = (RK_FLOAT)((RK_DOUBLE)*numerator /
                            (sqrt((RK_DOUBLE)*quadSum1) * sqrt((RK_DOUBLE)*quadSum2)));

            if (fR > fRMax) {
                if (fR > fRThresh)
                {
                    findMatch = true;
                    match_point.u16X = x1;
                    match_point.u16Y = y1;
                    if (fR > fRPassThresh) {
                        fRMax = fR;
                        break;
                    }
                }
                fRMax = fR;
            }
        }

        if (findMatch) {
            match_pattern[match_pattern_num][0].u16X = x0;
            match_pattern[match_pattern_num][0].u16Y = y0;

            match_pattern[match_pattern_num][1].u16X = match_point.u16X;
            match_pattern[match_pattern_num][1].u16Y = match_point.u16Y;

            match_pattern_num++;
        }
    }

    if (match_pattern_num < 2) {
        if (search_times < 3) {
            fRThresh -= fRStep;
            match_pattern_num = 0;
            search_times++;
            goto search_again;
        }
    } else {
        // calc centroid of refer and current image
        int avg_refx = 0;
        int avg_refy = 0;
        int avg_dstx = 0;
        int avg_dsty = 0;

        for (k = 0; k < match_pattern_num; k++) {
            avg_refx += (RK_S16)match_pattern[k][0].u16X;
            avg_refy += (RK_S16)match_pattern[k][0].u16Y;
            avg_dstx += (RK_S16)match_pattern[k][1].u16X;
            avg_dsty += (RK_S16)match_pattern[k][1].u16Y;
        }

        avg_refx = avg_refx / match_pattern_num;
        avg_refy = avg_refy / match_pattern_num;
        avg_dstx = avg_dstx / match_pattern_num;
        avg_dsty = avg_dsty / match_pattern_num;

        // calc motion vector
        mv.s16X = avg_dstx - avg_refx;
        mv.s16Y = avg_dsty - avg_refy;

        // update position of tracked object
        pAlgoCtrl->stTrackingObject.x += mv.s16X;
        pAlgoCtrl->stTrackingObject.y += mv.s16Y;
    }

    printf("search times: %d, match_pattern_num: %d\n", search_times, match_pattern_num);
    printf("motion vector: %d, %d\n", mv.s16X, mv.s16Y);

    //return (search_times > 2 || match_pattern_num < 1) ? -1 : s32Result;
    return s32Result;
}

void SAMPLE_IVE_Object_Tracking(void) {
    RK_S32 s32Result = RK_SUCCESS;
    RK_S32 i, j;
    FILE *fp = NULL;
    FILE *fo = NULL;
    RK_U32 u32Width = 640;
    RK_U32 u32Height = 360;
    RK_U32 u32TotalFrames = 250;

    SAMPLE_IVE_OBJECT_TRACKING_S *mAlgoCtrl = &mOTAlgoCtrl;
    mAlgoCtrl->tileWidth = 16;
    mAlgoCtrl->tileHeight = 32;

    const char *videoFileName = "/root/data/stream/rocker_640x360_f300.yuv";
    const char *outVideoFileName = "/root/output/ot_out_640x360_f300.yuv";
    mAlgoCtrl->stTrackingObject.x = 128;
    mAlgoCtrl->stTrackingObject.y = 112;
    mAlgoCtrl->stTrackingObject.width = 96;
    mAlgoCtrl->stTrackingObject.height = 160;


    SAMPLE_COMM_IVE_CheckIveMpiInit();

    // initialize object tracking
    SAMPLE_IVE_Object_Tracking_Init(mAlgoCtrl, u32Width, u32Height);

    // read first frame
    SAMPLE_COMM_IVE_ReadFile(videoFileName, &mAlgoCtrl->stSrcImg, &fp);

    // get reference tile image from incoming frame
    GET_TILE_IMAGE_FROM_START_POS(
        &mAlgoCtrl->stTrackingObject.stImg, &mAlgoCtrl->stSrcImg,
        mAlgoCtrl->stTrackingObject.x, mAlgoCtrl->stTrackingObject.y);

    gfx_draw_rect(
        "frame",
        (RK_VOID *)mAlgoCtrl->stSrcImg.au64VirAddr[0],
        u32Width, u32Height,
        mAlgoCtrl->stTrackingObject.x, mAlgoCtrl->stTrackingObject.y,
        mAlgoCtrl->stTrackingObject.width, mAlgoCtrl->stTrackingObject.height,
        IVE_IMAGE_TYPE_U8C1, 1);

    // loop run
    for (j = 1; j < u32TotalFrames; j++) {
        if (mSignalExit) {
            printf("exit loop\n");
            break;
        }
        SAMPLE_COMM_IVE_ReadFile(videoFileName, &mAlgoCtrl->stSrcImg, &fp);

        // get current tile image from incoming frame
        GET_TILE_IMAGE_FROM_START_POS(
            &mAlgoCtrl->stDstImg, &mAlgoCtrl->stSrcImg,
            mAlgoCtrl->stTrackingObject.x, mAlgoCtrl->stTrackingObject.y);

        // tracking object
        s32Result = SAMPLE_IVE_Object_Tracking_Proc(mAlgoCtrl, u32Width, u32Height);

        gfx_draw_rect(
            "frame",
            (RK_VOID *)mAlgoCtrl->stSrcImg.au64VirAddr[0],
            u32Width, u32Height,
            mAlgoCtrl->stTrackingObject.x, mAlgoCtrl->stTrackingObject.y,
            mAlgoCtrl->stTrackingObject.width, mAlgoCtrl->stTrackingObject.height,
            IVE_IMAGE_TYPE_U8C1, 1);

        // save object tracking result video
        SAMPLE_COMM_IVE_WriteFile(outVideoFileName, &mAlgoCtrl->stSrcImg, &fo);

        // update reference image
        if (s32Result == 0) 
        {
            GET_TILE_IMAGE_FROM_START_POS(
                &mAlgoCtrl->stTrackingObject.stImg, &mAlgoCtrl->stSrcImg,
                mAlgoCtrl->stTrackingObject.x, mAlgoCtrl->stTrackingObject.y);
        }
    }

    IVE_CLOSE_FILE(fp);
    IVE_CLOSE_FILE(fo);

    // exit
    SAMPLE_IVE_Object_Tracking_Uninit(mAlgoCtrl);

    SAMPLE_COMM_IVE_IveMpiExit();
}

void SAMPLE_IVE_Object_Tracking_HandleSig(void)
{
    mSignalExit = true;
}
