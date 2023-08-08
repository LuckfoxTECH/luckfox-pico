#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//#include <unistd.h>

#include "rve_tests.h"
#include "sample_assist.h"
#include "sample_define.h"
#include "sample_file.h"
#include "sample_gfx.h"
#include "sample_mem.h"

#define LK_PYR 1

RK_VOID InitSTCorner(RK_U16 u16Width, RK_U16 u16Height,
                     IVE_ST_CANDI_CORNER_CTRL_S *pstStCandiCornerCtrl,
                     IVE_ST_CORNER_CTRL_S *pstStCornerCtrl,
                     IVE_MEM_INFO_S *pstCandiCorner, IVE_MEM_INFO_S *pstCorner,
                     IVE_MEM_INFO_S *pstMem) {
    RK_S32 s32Ret = RK_SUCCESS;

    pstCandiCorner->u32Size = u16Width * u16Height * sizeof(RK_U16) +
                              sizeof(IVE_ST_CANDI_STACK_SIZE_S);
    s32Ret = RK_CreateIveMem(pstCandiCorner, pstCandiCorner->u32Size);
    RK_CHECK_ET_RET_VOID(s32Ret, RK_FAILURE);

    pstStCandiCornerCtrl->u0q8QualityLevel = 25;

    pstMem->u32Size = u16Width * u16Height + sizeof(IVE_ST_CORNER_MEM_S) * 2;
    s32Ret = RK_CreateIveMem(pstMem, pstMem->u32Size);
    RK_CHECK_ET_RET_VOID(s32Ret, RK_FAILURE);

    pstCorner->u32Size = sizeof(IVE_ST_CORNER_INFO_S);
    s32Ret = RK_CreateIveMem(pstCorner, pstCorner->u32Size);
    RK_CHECK_ET_RET_VOID(s32Ret, RK_FAILURE);

    pstStCornerCtrl->u16MaxCornerNum = 200;
    pstStCornerCtrl->u16MinDist = 10;
}

RK_VOID InitLKOpticalFlowPyr(RK_U32 u32Width, RK_U32 u32Height,
                             RK_U8 u8MaxLevel, IVE_IMAGE_S *pstGray,
                             IVE_IMAGE_S *pstPrevGray, IVE_MEM_INFO_S *pstMem,
                             IVE_IMAGE_S astPrevPyr[], IVE_IMAGE_S astNextPyr[],
                             IVE_MEM_INFO_S *pstPrevPts,
                             IVE_MEM_INFO_S *pstNextPts,
                             IVE_MEM_INFO_S *pstStatus,
                             IVE_MEM_INFO_S *pstErr,
                             IVE_MEM_INFO_S *pstMv) {
    RK_S32 s32Ret = RK_SUCCESS;
    RK_U32 u32Size, i;
    RK_U32 u32ImgSize[4];
    RK_U8 *pu8Mem;
    RK_U8 *pu8Mem_phy;

    u32Size = 0;
    for (i = 0; i <= u8MaxLevel; i++) {
        astPrevPyr[i].enType = IVE_IMAGE_TYPE_U8C1;
        astPrevPyr[i].u32Width = u32Width >> i;
        astPrevPyr[i].u32Height = u32Height >> i;
        astPrevPyr[i].au32Stride[0] =
            RK_CalcStride(astPrevPyr[i].u32Width, IVE_STRIDE_ALIGN);

        astNextPyr[i].enType = IVE_IMAGE_TYPE_U8C1;
        astNextPyr[i].u32Width = astPrevPyr[i].u32Width;
        astNextPyr[i].u32Height = astPrevPyr[i].u32Height;
        astNextPyr[i].au32Stride[0] = astPrevPyr[i].au32Stride[0];

        u32ImgSize[i] = astPrevPyr[i].au32Stride[0] * astPrevPyr[i].u32Height;
        u32Size += 2 * u32ImgSize[i];
    }

    pstPrevPts->u32Size =
        RK_CalcStride(sizeof(IVE_POINT_S25Q7_S) * LK_OPTICAL_FLOW_MAX_POINT_NUM,
                      IVE_STRIDE_ALIGN);
    pstNextPts->u32Size = pstPrevPts->u32Size;
    u32Size += 2 * pstPrevPts->u32Size;

    pstStatus->u32Size = RK_CalcStride(
        sizeof(RK_U8) * LK_OPTICAL_FLOW_MAX_POINT_NUM, IVE_STRIDE_ALIGN);
    u32Size += pstStatus->u32Size;

    pstErr->u32Size = RK_CalcStride(
        sizeof(IVE_U9Q7) * LK_OPTICAL_FLOW_MAX_POINT_NUM, IVE_STRIDE_ALIGN);
    u32Size += pstErr->u32Size;

    pstMv->u32Size =
        RK_CalcStride(sizeof(IVE_MV_S9Q7_S) * LK_OPTICAL_FLOW_MAX_POINT_NUM,
                      IVE_STRIDE_ALIGN);
    u32Size += pstMv->u32Size;

    pstMem->u32Size = u32Size;
    s32Ret = RK_CreateIveMem(pstMem, pstMem->u32Size);
    RK_CHECK_ET_RET_VOID(s32Ret, RK_FAILURE);

    pu8Mem = (RK_U8 *)pstMem->u64VirAddr;
    pu8Mem_phy = (RK_U8 *)pstMem->u64PhyAddr;

    MmzFlushStart(pu8Mem);
    memset(pu8Mem, 0, pstMem->u32Size);
    MmzFlushEnd(pu8Mem);

    for (i = 0; i <= u8MaxLevel; i++) {
        astPrevPyr[i].au64VirAddr[0] = (RK_U64)pu8Mem;
        astPrevPyr[i].au64PhyAddr[0] = (RK_U64)pu8Mem_phy;
        pu8Mem += u32ImgSize[i];
        pu8Mem_phy += u32ImgSize[i];

        astNextPyr[i].au64VirAddr[0] = (RK_U64)pu8Mem;
        astNextPyr[i].au64PhyAddr[0] = (RK_U64)pu8Mem_phy;
        pu8Mem += u32ImgSize[i];
        pu8Mem_phy += u32ImgSize[i];
    }

    pstPrevPts->u64VirAddr = (RK_U64)pu8Mem;
    pstPrevPts->u64PhyAddr = (RK_U64)pu8Mem_phy;
    pu8Mem += pstPrevPts->u32Size;
    pu8Mem_phy += pstPrevPts->u32Size;

    pstNextPts->u64VirAddr = (RK_U64)pu8Mem;
    pstNextPts->u64PhyAddr = (RK_U64)pu8Mem_phy;
    pu8Mem += pstNextPts->u32Size;
    pu8Mem_phy += pstNextPts->u32Size;

    pstStatus->u64VirAddr = (RK_U64)pu8Mem;
    pstStatus->u64PhyAddr = (RK_U64)pu8Mem_phy;
    pu8Mem += pstStatus->u32Size;
    pu8Mem_phy += pstStatus->u32Size;

    pstErr->u64VirAddr = (RK_U64)pu8Mem;
    pstErr->u64PhyAddr = (RK_U64)pu8Mem_phy;
    pu8Mem += pstErr->u32Size;
    pu8Mem_phy += pstErr->u32Size;

    pstMv->u64VirAddr = (RK_U64)pu8Mem;
    pstMv->u64PhyAddr = (RK_U64)pu8Mem_phy;

    // allocate memory for src
    s32Ret =
        RK_CreateIveImage(pstGray, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    RK_CHECK_ET_RET_VOID(s32Ret, RK_FAILURE);
    s32Ret = RK_CreateIveImage(pstPrevGray, IVE_IMAGE_TYPE_U8C1, u32Width,
                               u32Height);
    RK_CHECK_ET_RET_VOID(s32Ret, RK_FAILURE);
}

RK_VOID CvtImageToData(IVE_IMAGE_S *image, IVE_DATA_S *data) {
    data->u64PhyAddr = image->au64PhyAddr[0];
    data->u64VirAddr = image->au64VirAddr[0];
    data->u32Stride = image->au32Stride[0];
    data->u32Width = image->u32Width;
    data->u32Height = image->u32Height;
}

RK_S32 BuildLKOpticalFlowPyr(RK_U8 u8MaxLevel, IVE_IMAGE_S astPrevPyr[],
                             IVE_IMAGE_S astNextPyr[]) {
    RK_S32 s32Ret = RK_SUCCESS;
    int i, k;
    IVE_HANDLE hIveHandle;
    IVE_DMA_CTRL_S dma_ctrl = {0};
    IVE_FILTER_CTRL_S filter_ctrl = {0};
    RK_BOOL bInstant = RK_TRUE;
    FILE *fo_pyr = NULL;
    RK_CHAR *savePyramid = "/root/output/rve_pyr_out.yuv";

    IVE_DATA_S inData, outData;
    IVE_IMAGE_S stImage = {0};
    s32Ret = RK_CreateIveImage(&stImage, IVE_IMAGE_TYPE_U8C1,
                               astPrevPyr[0].u32Width, astPrevPyr[0].u32Height);
    RK_CHECK_ET_RET(s32Ret, RK_FAILURE, s32Ret);

    //dma controller
    dma_ctrl.enMode = IVE_DMA_MODE_INTERVAL_COPY;
    dma_ctrl.u8HorSegSize = 2;
    dma_ctrl.u8ElemSize = 1;
    dma_ctrl.u8VerSegRows = 2;

    //filter controller
    filter_ctrl.u8Norm = 8;
    filter_ctrl.u8OutMode = 0;
    char ps8Mask[25] = {1,  4,  6,  4, 1,
                        4, 16, 24, 16, 4,
                        6, 24, 36, 24, 6,
                        4, 16, 24, 16, 4,
                        1,  4,  6,  4, 1};

    memcpy(filter_ctrl.as8Mask, ps8Mask, 25 * sizeof(unsigned char));

    // filter prev pyramid, dma interval copy prev pyramid
    for (k = 0; k < 3; k++) {
        RK_MPI_IVE_Filter(&hIveHandle, &astPrevPyr[k], &stImage, &filter_ctrl,
                          bInstant);

        stImage.u32Width = astPrevPyr[k].u32Width;
        stImage.u32Height = astPrevPyr[k].u32Height;
        stImage.au32Stride[0] = astPrevPyr[k].au32Stride[0];
        CvtImageToData(&stImage, &inData);
        CvtImageToData(&astPrevPyr[k + 1], &outData);

        RK_MPI_IVE_DMA(&hIveHandle, &inData, &outData, &dma_ctrl, bInstant);

        stImage.u32Width = astPrevPyr[k + 1].u32Width;
        stImage.u32Height = astPrevPyr[k + 1].u32Height;
        stImage.au32Stride[0] = astPrevPyr[k + 1].au32Stride[0];

        //memcpy((void *)astPrevPyr[k + 1].au64VirAddr[0], stImage.au64VirAddr[0],
        //       astPrevPyr[k + 1].u32Width * astPrevPyr[k + 1].u32Height);
    }

    // filter next pyramid, dma interval copy next pyramid
    stImage.u32Width = astNextPyr[0].u32Width;
    stImage.u32Height = astNextPyr[0].u32Height;
    stImage.au32Stride[0] = astNextPyr[0].au32Stride[0];
    for (k = 0; k < 3; k++) {
        RK_MPI_IVE_Filter(&hIveHandle, &astNextPyr[k], &stImage, &filter_ctrl,
                          bInstant);

        stImage.u32Width = astNextPyr[k].u32Width;
        stImage.u32Height = astNextPyr[k].u32Height;
        stImage.au32Stride[0] = astNextPyr[k].au32Stride[0];
        CvtImageToData(&stImage, &inData);
        CvtImageToData(&astNextPyr[k + 1], &outData);
        RK_MPI_IVE_DMA(&hIveHandle, &inData, &outData, &dma_ctrl, bInstant);
        stImage.u32Width = astNextPyr[k + 1].u32Width;
        stImage.u32Height = astNextPyr[k + 1].u32Height;
        stImage.au32Stride[0] = astNextPyr[k + 1].au32Stride[0];

        //memcpy((void *)astNextPyr[k + 1].au64VirAddr[0], stImage.au64VirAddr[0],
        //       astNextPyr[k + 1].u32Width * astNextPyr[k + 1].u32Height);
    }



    RK_DestroyIveImage(&stImage);

    // print pyramid
#if 0
    {
        fo_pyr = fopen(savePyramid, "wb");
        if (fo_pyr == NULL) {
            printf("failed to open(%s)", savePyramid);
            return -1;
        }

        for (k = 0; k < 4; k++) {
            for (i = 0; i < astPrevPyr[k].u32Height; i++) {
                RK_U8 *cur_addr = ((RK_U8 *)astPrevPyr[k].au64VirAddr[0]) +
                                i * astPrevPyr[k].u32Width;
                fwrite(cur_addr, astPrevPyr[k].u32Width, 1, fo_pyr);
                fseek(fo_pyr, (astPrevPyr[0].u32Width - astPrevPyr[k].u32Width),
                    SEEK_CUR);
            }
        }

        for (k = 0; k < 4; k++) {
            for (i = 0; i < astNextPyr[k].u32Height; i++) {
                RK_U8 *cur_addr = ((RK_U8 *)astNextPyr[k].au64VirAddr[0]) +
                                i * astNextPyr[k].u32Width;
                fwrite(cur_addr, astNextPyr[k].u32Width, 1, fo_pyr);
                fseek(fo_pyr, (astNextPyr[0].u32Width - astNextPyr[k].u32Width),
                    SEEK_CUR);
            }
        }
        int last_row_blank_size = (astNextPyr[0].u32Width - astNextPyr[3].u32Width);
        fseek(fo_pyr, -last_row_blank_size, SEEK_CUR);
        while (last_row_blank_size > 0) {
            fputc(0, fo_pyr);
            last_row_blank_size--;
        }
        fflush(fo_pyr);
        RK_FCLOSE(fo_pyr);
    }
#endif
    return RK_SUCCESS;
}

RK_VOID DeinitLKOpticalFlowPyr(RK_U8 u8MaxLevel, IVE_IMAGE_S *pstGray,
                               IVE_IMAGE_S *pstPrevGray,
                               IVE_MEM_INFO_S *pstMem,
                               IVE_IMAGE_S astPrevPyr[],
                               IVE_IMAGE_S astNextPyr[],
                               IVE_MEM_INFO_S *pstPrevPts,
                               IVE_MEM_INFO_S *pstNextPts,
                               IVE_MEM_INFO_S *pstStatus,
                               IVE_MEM_INFO_S *pstErr) {
    RK_S32 i;

    RK_DestroyIveImage(pstGray);
    RK_DestroyIveImage(pstPrevGray);
    RK_DestroyIveMem(pstMem);
}

RK_VOID LKOpticalFlowPyrSample(const RK_CHAR *moduleName, RK_CHAR *fileName,
                               RK_CHAR *outDir, RK_U32 u32Width,
                               RK_U32 u32Height) {
    int i, j, k;
    RK_CHAR outFile[RK_MAX_FILE_LEN];

    // for LK
    RK_DOUBLE dTime;
    RK_S32 s32Ret = RK_SUCCESS;
    IVE_HANDLE iveHandle;
    IVE_IMAGE_S astPrevPyr[4], astNextPyr[4];
    IVE_IMAGE_S gray, prevGray;
    IVE_MEM_INFO_S stPrevPts, stNextPts, stStatus, stErr, stMv;
    IVE_MEM_INFO_S stCorner, stCandiCorner;
    IVE_ST_CANDI_CORNER_CTRL_S stStCandiCornerCtrl = {0};
    IVE_ST_CORNER_CTRL_S stStCornerCtrl = {0};
    IVE_LK_OPTICAL_FLOW_PYR_CTRL_S stLkPyrCtrl = {0};
    IVE_LK_OPTICAL_FLOW_CTRL_S stLkCtrl = {0};
    RK_BOOL bInstant = RK_TRUE;

    IVE_MEM_INFO_S stMem;
    IVE_POINT_S25Q7_S *prevPts, *nextPts;
    IVE_MV_S9Q7_S *vector;
    RK_U8 *statusTan;
    IVE_U9Q7 *errTan;
    FILE *fp = NULL;
    FILE *fo = NULL;
    int frmCnt = 10;
    bool bBlock = true;
    bool bFinish = false;

    // ctrl of LK-tan
    stLkPyrCtrl.enOutMode = IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_BOTH;
    stLkPyrCtrl.bUseInitFlow = RK_FALSE;
    stLkPyrCtrl.u16PtsNum = LK_OPTICAL_FLOW_MAX_POINT_NUM;
    stLkPyrCtrl.u8IterCnt = 20;
    stLkPyrCtrl.u8MaxLevel = 3;
    stLkPyrCtrl.u0q8MinEigThr = 100;  // opencv: 0.001
    stLkPyrCtrl.u0q11Eps = 32;         // opencv: (RK_U0Q8)(0.03 * 256);

    stLkCtrl.u16PtsNum = LK_OPTICAL_FLOW_MAX_POINT_NUM;
    stLkCtrl.u8IterCnt = 20;
    stLkCtrl.u0q8MinEigThr = 100;
    stLkCtrl.u0q11Eps = 32;

    RK_MPI_IVE_Init();

#if BUILD_SIMULATOR
    gfx_query_frame(&u32Width, &u32Height, &frmCnt, fileName);
#else
    if(fileName != NULL)
       frmCnt = RK_ReadFileSize(fileName, &fp) / (u32Width * u32Height);
#endif

    InitSTCorner(u32Width, u32Height, &stStCandiCornerCtrl, &stStCornerCtrl,
                 &stCandiCorner, &stCorner, &stStCornerCtrl.stMem);

    InitLKOpticalFlowPyr((RK_U16)u32Width, (RK_U16)u32Height,
                         stLkPyrCtrl.u8MaxLevel, &gray, &prevGray, &stMem,
                         astPrevPyr, astNextPyr, &stPrevPts, &stNextPts,
                         &stStatus, &stErr, &stMv);

    prevPts = (IVE_POINT_S25Q7_S *)stPrevPts.u64VirAddr;
    nextPts = (IVE_POINT_S25Q7_S *)stNextPts.u64VirAddr;
    vector = (IVE_MV_S9Q7_S *)stMv.u64VirAddr;
    statusTan = (RK_U8 *)stStatus.u64VirAddr;
    errTan = (IVE_U9Q7 *)stErr.u64VirAddr;

    for (j = 0; j < frmCnt; j++) {
        RK_ReadFile(fileName, &gray, &fp);
#if BUILD_SIMULATOR
        gfx_read_frame((RK_U8 *)gray.au64VirAddr[0], u32Width * u32Height,
                       fileName);
#else
        RK_ReadFile(fileName, &gray, &fp);
#endif

        if (0 == j) {
            s32Ret = RK_MPI_IVE_STCandiCorner(&iveHandle, &gray, &stCandiCorner,
                                              &stStCandiCornerCtrl, true);
            RK_CHECK_ET_RET_VOID(s32Ret, RK_FAILURE);
            /*
                        s32Ret = RK_MPI_IVE_Query(iveHandle, &bFinish, bBlock);
                        while (ERR_IVE_QUERY_TIMEOUT == s32Ret) {
                            usleep(100);
                            s32Ret = RK_MPI_IVE_Query(iveHandle, &bFinish,
               bBlock);
                        }
                        RK_CHECK_ET_RET_VOID(s32Ret, RK_FAILURE);
            */
            s32Ret =
                RK_MPI_IVE_STCorner(&iveHandle, &astNextPyr[0], &stCandiCorner,
                                    &stCorner, &stStCornerCtrl, true);
            RK_CHECK_ET_RET_VOID(s32Ret, RK_FAILURE);

            IVE_ST_CORNER_INFO_S *pstCornerInfo =
                (IVE_ST_CORNER_INFO_S *)stCorner.u64VirAddr;
            for (k = 0; k < pstCornerInfo->u16CornerNum; k++) {
                prevPts[k].s25q7X =
                    (IVE_S25Q7)(pstCornerInfo->astCorner[k].u16X) * 128;
                prevPts[k].s25q7Y =
                    (IVE_S25Q7)(pstCornerInfo->astCorner[k].u16Y) * 128;
            }
#if 0
            /* begin of test */
            pstCornerInfo->u16CornerNum = 3;
            prevPts[0].s25q7X= 300 * 128;
            prevPts[0].s25q7Y = 300 * 128;
            prevPts[1].s25q7X = 136 * 128;
            prevPts[1].s25q7Y = 266 * 128;
            prevPts[2].s25q7X = 206 * 128;
            prevPts[2].s25q7Y = 210 * 128;
            /* end of test */
#endif
            stLkPyrCtrl.u16PtsNum = pstCornerInfo->u16CornerNum;
            stLkCtrl.u16PtsNum = pstCornerInfo->u16CornerNum;

            RK_Memcpy((void *)prevGray.au64VirAddr[0],
                      (void *)gray.au64VirAddr[0], u32Width * u32Height);
        }

        {
            RK_Memcpy((void *)astPrevPyr[0].au64VirAddr[0],
                      (void *)prevGray.au64VirAddr[0], u32Width * u32Height);
            RK_Memcpy((void *)astNextPyr[0].au64VirAddr[0],
                      (void *)gray.au64VirAddr[0], u32Width * u32Height);
            RK_Memcpy((void *)prevGray.au64VirAddr[0],
                      (void *)gray.au64VirAddr[0], u32Width * u32Height);
#if LK_PYR
            printf("lk test: build pyramid outside\n");
            BuildLKOpticalFlowPyr(stLkPyrCtrl.u8MaxLevel, astPrevPyr,
                                  astNextPyr);

            dTime = (RK_DOUBLE)RK_GetTickCount();
            s32Ret = RK_MPI_IVE_LKOpticalFlowPyr(
                &iveHandle, astPrevPyr, astNextPyr, &stPrevPts, &stNextPts,
                &stStatus, &stErr, &stLkPyrCtrl, bInstant);
            dTime = (RK_DOUBLE)RK_GetTickCount() - dTime;

            printf("LKOpticalFlowPyr,time = %g ms\n",
                   dTime / ((RK_DOUBLE)RK_GetTickFrequency() * 1000.));

            unsigned char *p_pixel_in = (unsigned char *)gray.au64VirAddr[0];

            MmzFlushStart(p_pixel_in);

            for (i = k = 0; i < stLkPyrCtrl.u16PtsNum; i++) {
                if (!statusTan[i]) continue;
                nextPts[k].s25q7X = nextPts[i].s25q7X;
                nextPts[k].s25q7Y = nextPts[i].s25q7Y;

                IVE_POINT_FLOAT point;
                point.fpX = nextPts[k].s25q7X / 128;
                point.fpY = nextPts[k].s25q7Y / 128;

                int postion = (int)(point.fpY * u32Width + point.fpX);
                if (postion < 0) continue;
                p_pixel_in[postion] = 255;
                if (postion % u32Width != 0 &&
                    postion % u32Width != u32Width - 1) {
                    p_pixel_in[postion - 1] = 255;
                    p_pixel_in[postion + 1] = 255;
                }
                if (postion > u32Width - 1 &&
                    postion < u32Width * (u32Height - 1)) {
                    p_pixel_in[postion - u32Width] = 255;
                    p_pixel_in[postion + u32Width] = 255;
                }
                k++;
            }

            MmzFlushEnd(p_pixel_in);

            stLkPyrCtrl.u16PtsNum = k;

            if (stLkPyrCtrl.u16PtsNum == 0) {
                printf("lk test: failed to track points, ptsnum is zero\n");
                break;
            }

            memcpy((RK_U8 *)stPrevPts.u64VirAddr, (RK_U8 *)stNextPts.u64VirAddr,
                   stLkPyrCtrl.u16PtsNum * sizeof(IVE_POINT_S25Q7_S));

            printf("lk test, ptsnum: %d\n", stLkPyrCtrl.u16PtsNum);

#else
            printf("lk test: build pyramid inside\n");
            dTime = (RK_DOUBLE)RK_GetTickCount();
            RK_MPI_IVE_LKOpticalFlow(&iveHandle, &astPrevPyr[0], &astNextPyr[0],
                                     &stPrevPts, &stMv, &stLkCtrl, bInstant);
            dTime = (RK_DOUBLE)RK_GetTickCount() - dTime;

            printf("LKOpticalFlow,time = %g ms\n",
                   dTime / ((RK_DOUBLE)RK_GetTickFrequency() * 1000.));

            unsigned char *p_pixel_in = (unsigned char *)gray.au64VirAddr[0];

            MmzFlushStart(p_pixel_in);
            for (i = k = 0; i < stLkCtrl.u16PtsNum; i++) {
                if (vector[i].s32Statys == 0) continue;
                nextPts[k].s25q7X = vector[i].s9q7X + prevPts[i].s25q7X;
                nextPts[k].s25q7Y = vector[i].s9q7Y + prevPts[i].s25q7Y;

                IVE_POINT_FLOAT point;
                point.fpX = nextPts[k].s25q7X / 128;
                point.fpY = nextPts[k].s25q7Y / 128;
                if (point.fpX >= u32Width || point.fpY >= u32Height || point.fpX < 0 || point.fpY < 0) {
                    printf("point%3d[%f-%f]\n", k, point.fpX, point.fpY);
                    continue;
                }

                int postion = (int)(point.fpY * u32Width + point.fpX);
                p_pixel_in[postion] = 255;
                if (postion % u32Width != 0 && postion % u32Width != u32Width - 1) {
                    p_pixel_in[postion - 1] = 255;
                    p_pixel_in[postion + 1] = 255;
                }
                if (postion > u32Width - 1 && postion < u32Width * (u32Height - 1)) {
                    p_pixel_in[postion - u32Width] = 255;
                    p_pixel_in[postion + u32Width] = 255;
                }
                k++;
            }

            MmzFlushEnd(p_pixel_in);

            stLkCtrl.u16PtsNum = k;

            if (stLkCtrl.u16PtsNum == 0) {
                printf("lk test: failed to track points, ptsnum is zero\n");
                break;
            }

            memcpy((RK_U8 *)stPrevPts.u64VirAddr, (RK_U8 *)stNextPts.u64VirAddr,
                   stLkCtrl.u16PtsNum * sizeof(IVE_POINT_S25Q7_S));

            printf("lk test, ptsnum: %d\n", stLkCtrl.u16PtsNum);

#endif
        }
#if BUILD_SIMULATOR
        RK_SPRINTF(outFile, RK_MAX_FILE_LEN, "%s/rve_%s_out.avi", outDir,
                   moduleName);
        gfx_write_frame((RK_U8 *)gray.au64VirAddr[0], u32Width, u32Height,
                        IVE_IMAGE_TYPE_U8C1, (j == frmCnt - 1) ? 1 : 0,
                        outFile);
        gfx_show_img("lk", gray.au64VirAddr[0], u32Width, u32Height, IVE_IMAGE_TYPE_U8C1, 1);
#endif

    // fwrite((void *)astPrevPyr[3].au64VirAddr[0], 80 * 60, 1, &fo);
    if (outDir == NULL) {
        RK_WriteFile(NULL, (void *)gray.au64VirAddr[0], u32Width * u32Height,
                     &fo);
    } else {
        RK_SPRINTF(outFile, RK_MAX_FILE_LEN, "%s/rve_%s_out.yuv", outDir,
                   moduleName);
        RK_WriteFile(outFile, (void *)gray.au64VirAddr[0], u32Width * u32Height,
                     &fo);
    }
    // fflush(fo);

    }

    DeinitLKOpticalFlowPyr(stLkPyrCtrl.u8MaxLevel, &gray, &prevGray, &stMem, astPrevPyr,
                           astNextPyr, &stPrevPts, &stNextPts, &stStatus,
                           &stErr);

    RK_FCLOSE(fp);
    RK_FCLOSE(fo);

    RK_MPI_IVE_Deinit();
}
