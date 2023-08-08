#include <string.h>
#include <rk_mpi_ive.h>
#include <sample_assist.h>
#include <sample_define.h>
#include "sample_file.h"

#include "rve_tests.h"

RK_S32 InitMatchBgModel(IVE_SRC_IMAGE_S *pstCurImg, IVE_DATA_S *pstBgModel,
                        IVE_MATCH_BG_MODEL_CTRL_S *pstMatchBgModelCtrl,
                        RK_U32 u32Width, RK_U32 u32Height) {
    RK_S32 s32Result = RK_SUCCESS;

    // malloc pstCurImg
    s32Result =
        RK_CreateIveImage(pstCurImg, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    RK_CHECK_ET_RET(s32Result, RK_FAILURE, s32Result);

    // malloc pstBgModel
    pstBgModel->u32Height = u32Height;
    pstBgModel->u32Width = u32Width * sizeof(IVE_BG_MODEL_PIX_S);
    s32Result = RK_CreateIveData(pstBgModel, pstBgModel->u32Width,
                                pstBgModel->u32Height);
    RK_CHECK_ET_RET(s32Result, RK_FAILURE, s32Result);

#if 0
    // malloc pstMatchFg
    s32Result = RK_CreateIveImage(pstBgDiffFg, IVE_IMAGE_TYPE_S8C1, u32Width,
                                  u32Height);
    RK_CHECK_ET_RET(s32Result, RK_FAILURE, s32Result);

    // malloc pstDiffFg
    s32Result = RK_CreateIveImage(pstFrmDiffFg, IVE_IMAGE_TYPE_S8C1, u32Width,
                                  u32Height);
    RK_CHECK_ET_RET(s32Result, RK_FAILURE, s32Result);

    // malloc pstStatData
    pstStatData->u32Size = sizeof(IVE_BG_STAT_DATA_S);
    s32Result = RK_CreateIveMem(pstStatData, pstStatData->u32Size);
    RK_CHECK_ET_RET(s32Result, RK_FAILURE, s32Result);
#endif

    // init ctrl info
    pstMatchBgModelCtrl->u8CodeWordNum = 3;
    pstMatchBgModelCtrl->u32CurFrmNum = 0;
    pstMatchBgModelCtrl->u8TrainTimeThr = 20;
    pstMatchBgModelCtrl->u8DiffMaxThr = 10;
    pstMatchBgModelCtrl->u8DiffMinThr = 10;

    return s32Result;
}

RK_S32 InitUpdateBgModel(IVE_DST_IMAGE_S *pstFgFlag, IVE_DST_IMAGE_S *pstBgImg,
                         IVE_UPDATE_BG_MODEL_CTRL_S *pstUpdateBgModelCtrl,
                         RK_U16 u16Width, RK_U16 u16Height) {
    RK_S32 s32Result = RK_SUCCESS;

    // malloc pstFgFlag
    s32Result =
        RK_CreateIveImage(pstFgFlag, IVE_IMAGE_TYPE_U8C1, u16Width, u16Height);
    RK_CHECK_ET_RET(s32Result, RK_FAILURE, s32Result);

    // malloc pstBgImg
    s32Result =
        RK_CreateIveImage(pstBgImg, IVE_IMAGE_TYPE_U8C1, u16Width, u16Height);
    RK_CHECK_ET_RET(s32Result, RK_FAILURE, s32Result);

#if 0
    // malloc pstChgStaImg
    s32Result = RK_CreateIveImage(pstChgStaImg, IVE_IMAGE_TYPE_U8C1, u16Width,
                                  u16Height);
    RK_CHECK_ET_RET(s32Result, RK_FAILURE, s32Result);

    // malloc pstChgStaFg
    s32Result = RK_CreateIveImage(pstChgStaFg, IVE_IMAGE_TYPE_S8C1, u16Width,
                                  u16Height);
    RK_CHECK_ET_RET(s32Result, RK_FAILURE, s32Result);

    // malloc pstChgStaLife
    s32Result = RK_CreateIveImage(pstChgStaLife, IVE_IMAGE_TYPE_U16C1, u16Width,
                                  u16Height);
    RK_CHECK_ET_RET(s32Result, RK_FAILURE, s32Result);
#endif

    // init ctrl info
    pstUpdateBgModelCtrl->u8CodeWordNum = 3;
    pstUpdateBgModelCtrl->u32CurFrmNum = 0;
    pstUpdateBgModelCtrl->u8TimeThr = 50;
    pstUpdateBgModelCtrl->u8DiffMaxThr = 10;
    pstUpdateBgModelCtrl->u8DiffMinThr = 10;
    pstUpdateBgModelCtrl->u8FastLearnRate = 4;
    pstUpdateBgModelCtrl->u8Alpha = 4;

    return s32Result;
}

RK_VOID UninitMatchBgModel(IVE_SRC_IMAGE_S *pstCurImg, IVE_DATA_S *pstBgModel) {
    RK_DestroyIveImage(pstCurImg);
    RK_DestroyIveData(pstBgModel);
}

RK_VOID UninitUpdateBgModel(IVE_DST_IMAGE_S *pstFgFlag,
                            IVE_DST_IMAGE_S *pstBgImg) {
    RK_DestroyIveImage(pstFgFlag);
    RK_DestroyIveImage(pstBgImg);
}

RK_VOID BgModelSample1(const RK_CHAR *moduleName, RK_CHAR *fileName, RK_CHAR *outDir,
                       RK_U32 u32Width, RK_U32 u32Height) {
    IVE_HANDLE hIveHandle;
    RK_BOOL bInstant = RK_TRUE;
    RK_S32 s32Result;
    RK_DOUBLE dTime;

    IVE_SRC_IMAGE_S stCurImg = {0};
    IVE_DATA_S stBgModel = {0};
    IVE_IMAGE_S stFgFlag = {0};
    IVE_DST_IMAGE_S stBgImg = {0};

    IVE_MATCH_BG_MODEL_CTRL_S stMatchBgModelCtrl = {0};
    IVE_UPDATE_BG_MODEL_CTRL_S stUpdateBgModelCtrl = {0};

    IVE_DST_IMAGE_S stFgImg = {0};
    IVE_SUB_CTRL_S stSubCtrl;

    RK_CHAR outFile[RK_MAX_FILE_LEN];
    RK_U32 u32FrameNum = 0;
    RK_S32 frmCnt = 10;

    FILE *fp = NULL, *fo_fg = NULL, *fo_bg = NULL;

    // init match bg model info
    s32Result = InitMatchBgModel(&stCurImg, &stBgModel, &stMatchBgModelCtrl,
                                 u32Width, u32Height);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    // init update bg model info
    s32Result = InitUpdateBgModel(&stFgFlag, &stBgImg, &stUpdateBgModelCtrl,
                                  u32Width, u32Height);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

#if BUILD_SIMULATOR
    gfx_query_frame(&u32Width, &u32Height, &frmCnt, fileName);
#else
    frmCnt = RK_ReadFileSize(fileName, &fp) / (u32Width * u32Height);
#endif

    for (u32FrameNum = 0; u32FrameNum < 60; u32FrameNum++) {
#if BUILD_SIMULATOR
        gfx_read_frame((RK_U8 *)stCurImg.au64VirAddr[0], u32Width * u32Height,
                       fileName);
#else
        s32Result = RK_ReadFile(fileName, &stCurImg, &fp);
#endif
        // call match bg model
        stMatchBgModelCtrl.u32CurFrmNum = u32FrameNum;

        dTime = (RK_DOUBLE)RK_GetTickCount();
        s32Result = RK_MPI_IVE_MatchBgModel(
            &hIveHandle, &stCurImg, &stBgModel, &stMatchBgModelCtrl, bInstant);
        RK_CHECK_NET_GOTO(s32Result, RK_SUCCESS, FAILURE);
        dTime = (RK_DOUBLE)RK_GetTickCount() - dTime;

        printf("BGM MatchBgModel,time = %g ms\n",
               dTime / ((RK_DOUBLE)RK_GetTickFrequency() * 1000.));
    }

    for (u32FrameNum = 60; u32FrameNum < frmCnt; u32FrameNum++) {
        stUpdateBgModelCtrl.u32CurFrmNum = u32FrameNum;

        if (stUpdateBgModelCtrl.u32CurFrmNum % 3 == 0) {
#if BUILD_SIMULATOR
            gfx_read_frame((RK_U8 *)stCurImg.au64VirAddr[0],
                           u32Width * u32Height, fileName);
#else
            s32Result = RK_ReadFile(fileName, &stCurImg, &fp);
#endif

            dTime = (RK_DOUBLE)RK_GetTickCount();
            // call update bg model
            s32Result = RK_MPI_IVE_UpdateBgModel(
                &hIveHandle, &stCurImg, &stBgModel, &stFgFlag, &stBgImg,
                &stUpdateBgModelCtrl, bInstant);
            RK_CHECK_NET_GOTO(s32Result, RK_SUCCESS, FAILURE);
            dTime = (RK_DOUBLE)RK_GetTickCount() - dTime;

            printf("BGM UpdateBgModel,time = %g ms\n",
                   dTime / ((RK_DOUBLE)RK_GetTickFrequency() * 1000.));
        }
#if BUILD_SIMULATOR
        gfx_show_img("fg", (void *)stFgFlag.au64VirAddr[0], stFgFlag.u32Width,
                     stFgFlag.u32Height, IVE_IMAGE_TYPE_U8C1, 1);
#endif
    }

    if (outDir == NULL) {
        RK_WriteFile(NULL, (void *)stFgFlag.au64VirAddr[0],
                     stFgFlag.u32Width * stFgFlag.u32Height, &fo_fg);
    } else {
        RK_SPRINTF(outFile, RK_MAX_FILE_LEN, "%s/rve_%s_fg_out.yuv", outDir, moduleName);
        RK_WriteFile(outFile, (void *)stFgFlag.au64VirAddr[0],
                     stFgFlag.u32Width * stFgFlag.u32Height, &fo_fg);
    }

    if (outDir == NULL) {
        RK_WriteFile(NULL, (void *)stBgImg.au64VirAddr[0],
                     stBgImg.u32Width * stBgImg.u32Height, &fo_bg);
    } else {
        RK_SPRINTF(outFile, RK_MAX_FILE_LEN, "%s/rve_%s_bg_out.yuv", outDir, moduleName);
        RK_WriteFile(outFile, (void *)stBgImg.au64VirAddr[0],
                     stBgImg.u32Width * stBgImg.u32Height, &fo_bg);
    }

    // release source
FAILURE:
    RK_FCLOSE(fp);
    RK_FCLOSE(fo_fg);
    RK_FCLOSE(fo_bg);

    UninitMatchBgModel(&stCurImg, &stBgModel);
    UninitUpdateBgModel(&stFgFlag, &stBgImg);
}

#if 0
RK_VOID BgModelSample2() {
    IVE_HANDLE hIveHandle = 0;
    RK_BOOL bInstant = RK_TRUE;
    RK_BOOL bBlock = RK_TRUE;
    RK_BOOL bFinish = RK_FALSE;

    IVE_DATA_S stBgModel = {0};
    IVE_SRC_IMAGE_S stCurImg;
    IVE_IMAGE_S stFgFlag;
    IVE_DST_IMAGE_S stBgDiffFg;
    IVE_DST_IMAGE_S stFrmDiffFg;
    IVE_DST_MEM_INFO_S stStatData;
    IVE_BGM_CTRL_S stMatchBgModelCtrl;

    IVE_DST_IMAGE_S stBgImg;
    IVE_DST_IMAGE_S stChgStaImg;
    IVE_DST_IMAGE_S stChgStaFg;
    IVE_DST_IMAGE_S stChgStaLife;
    IVE_UPDATE_BG_MODEL_CTRL_S stUpdateBgModelCtrl;

    IVE_IMAGE_S stFg;
    IVE_THRESH_CTRL_S stThrCtrl = {IVE_THRESH_MODE_BINARY, 0, 0, 0, 0, 255};

    RK_U32 u32UpdCnt = 0;         // �������´���
    RK_U32 u32PreUpdTime = 0;     // ��һ�α�������ʱ��
    RK_U32 u32FrmUpdPeriod = 15;  // ������������
    RK_U32 u32PreChkTime = 0;     // ��һ�α������ʱ��
    RK_U32 u32FrmChkPeriod = 50;  // �����������

    // malloc pstStatData
    stStatData.u32Size = sizeof(IVE_BG_STAT_DATA_S);
    stStatData.u64VirAddr = (RK_U64)malloc(stStatData.u32Size);
    RK_CHECK_ET_RET_VOID(stStatData.u64VirAddr, 0);
    stStatData.u64PhyAddr = (RK_U32)stStatData.u64VirAddr;

    stMatchBgModelCtrl.u32CurFrmNum = 0;
    stMatchBgModelCtrl.u32PreFrmNum = 0;
    stMatchBgModelCtrl.u16TimeThr = 20;
    stMatchBgModelCtrl.u8DiffThrCrlCoef = 0;
    stMatchBgModelCtrl.u8DiffMaxThr = 10;
    stMatchBgModelCtrl.u8DiffMinThr = 10;
    stMatchBgModelCtrl.u8DiffThrInc = 0;
    stMatchBgModelCtrl.u8FastLearnRate = 4;
    stMatchBgModelCtrl.u8DetChgRegion = 0;

    stUpdateBgModelCtrl.u32CurFrmNum = 0;
    stUpdateBgModelCtrl.u32PreChkTime = 0;
    stUpdateBgModelCtrl.u32FrmChkPeriod = 50;
    stUpdateBgModelCtrl.u32InitMinTime = 25;
    stUpdateBgModelCtrl.u32StyBgMinBlendTime = 100;
    stUpdateBgModelCtrl.u32StyBgMaxBlendTime = 1500;
    stUpdateBgModelCtrl.u32DynBgMinBlendTime = 0;
    stUpdateBgModelCtrl.u32StaticDetMinTime = 80;
    stUpdateBgModelCtrl.u16FgMaxFadeTime = 15;
    stUpdateBgModelCtrl.u16BgMaxFadeTime = 60;
    stUpdateBgModelCtrl.u8StyBgAccTimeRateThr = 80;
    stUpdateBgModelCtrl.u8ChgBgAccTimeRateThr = 60;
    stUpdateBgModelCtrl.u8DynBgAccTimeThr = 0;
    stUpdateBgModelCtrl.u8DynBgDepth = 3;
    stUpdateBgModelCtrl.u8BgEffStaRateThr = 90;
    stUpdateBgModelCtrl.u8AcceBgLearn = 0;
    stUpdateBgModelCtrl.u8DetChgRegion = 0;

    CvCapture *pstCvCap = NULL;
    IplImage *pstCvImg = NULL;
    IplImage *pstCvImgGray = NULL;
    IplImage *pstCvFg = NULL;
    IplImage *pstCvFgRGB = NULL;
    IplImage *pstCvBgGray = NULL;
    RK_S32 s32Result;
    RK_U32 u32FrameNum = 0;
    CvVideoWriter *pstCvFGAvi = NULL;
    CvSize stSize;
    RK_S32 s32OneFrameProcess = 0;
    RK_S32 s32Key;

    pstCvCap = cvCaptureFromFile("/root/data/avi/hall.avi");
    if (NULL == pstCvCap) {
        printf("the avi is not exist");
        return;
    }

    for (u32FrameNum = 1;
         (s32Key = cvWaitKey(s32OneFrameProcess ? 0 : 1)) != 27;
         u32FrameNum++) {
        if (s32Key != -1) {
            s32OneFrameProcess = 1;
            if (s32Key == 'r') s32OneFrameProcess = 0;
        }
        pstCvImg = cvQueryFrame(pstCvCap);
        if (pstCvImg == NULL) {
            break;
        }
        if (NULL == pstCvImgGray) {
            stSize = cvSize(pstCvImg->width, pstCvImg->height);
            pstCvImgGray = cvCreateImage(stSize, 8, 1);
            if (RK_NULL == pstCvImgGray) {
                goto FAILURE;
            }
        }
        cvCvtColor(pstCvImg, pstCvImgGray, CV_BGR2GRAY);

        // first frame, init all image needed
        if (1 == u32FrameNum) {
            stBgModel.u32Width = sizeof(IVE_BG_MODEL_PIX_S) * pstCvImg->width;
            stBgModel.u32Height = pstCvImg->height;
            stBgModel.u32Stride =
                RK_CalcStride(stBgModel.u32Width, RK_IVE2_STRIDE_ALIGN);
            stBgModel.u64VirAddr =
                (RK_U64)malloc(stBgModel.u32Stride * pstCvImg->height);
            memset((RK_VOID *)stBgModel.u64VirAddr, 0,
                   stBgModel.u32Stride * pstCvImg->height);
            stBgModel.u64PhyAddr = (RK_U32)stBgModel.u64VirAddr;

            s32Result = RK_CreateIveImage(&stCurImg, IVE_IMAGE_TYPE_U8C1,
                                          pstCvImg->width, pstCvImg->height);
            RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

            s32Result = RK_CreateIveImage(&stFgFlag, IVE_IMAGE_TYPE_U8C1,
                                          pstCvImg->width, pstCvImg->height);
            RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);
            memset((RK_VOID *)stFgFlag.au64VirAddr[0], 0,
                   pstCvImg->width * pstCvImg->height);

            s32Result = RK_CreateIveImage(&stBgDiffFg, IVE_IMAGE_TYPE_S8C1,
                                          pstCvImg->width, pstCvImg->height);
            RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);
            memset((RK_VOID *)stBgDiffFg.au64VirAddr[0], 0,
                   pstCvImg->width * pstCvImg->height);

            s32Result = RK_CreateIveImage(&stFrmDiffFg, IVE_IMAGE_TYPE_S8C1,
                                          pstCvImg->width, pstCvImg->height);
            RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);
            memset((RK_VOID *)stFrmDiffFg.au64VirAddr[0], 0,
                   pstCvImg->width * pstCvImg->height);

            s32Result = RK_CreateIveImage(&stBgImg, IVE_IMAGE_TYPE_U8C1,
                                          pstCvImg->width, pstCvImg->height);
            RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);
            memset((RK_VOID *)stBgImg.au64VirAddr[0], 0,
                   pstCvImg->width * pstCvImg->height);

            s32Result = RK_CreateIveImage(&stChgStaImg, IVE_IMAGE_TYPE_U8C1,
                                          pstCvImg->width, pstCvImg->height);
            RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);
            memset((RK_VOID *)stChgStaImg.au64VirAddr[0], 0,
                   pstCvImg->width * pstCvImg->height);

            s32Result = RK_CreateIveImage(&stChgStaFg, IVE_IMAGE_TYPE_S8C1,
                                          pstCvImg->width, pstCvImg->height);
            RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);
            memset((RK_VOID *)stChgStaFg.au64VirAddr[0], 0,
                   pstCvImg->width * pstCvImg->height);

            s32Result = RK_CreateIveImage(&stChgStaLife, IVE_IMAGE_TYPE_U16C1,
                                          pstCvImg->width, pstCvImg->height);
            RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);
            memset((RK_VOID *)stChgStaLife.au64VirAddr[0], 0,
                   pstCvImg->width * pstCvImg->height);
        }
        CopyCvToIve(&stCurImg, pstCvImgGray);

        stMatchBgModelCtrl.u32PreFrmNum = stMatchBgModelCtrl.u32CurFrmNum;
        stMatchBgModelCtrl.u32CurFrmNum = u32FrameNum;
        s32Result = RK_MPI_IVE_MatchBgModel(
            &hIveHandle, &stCurImg, &stBgModel, &stFgFlag, &stBgDiffFg,
            &stFrmDiffFg, &stStatData, &stMatchBgModelCtrl, bInstant);
        if (RK_SUCCESS != s32Result) {
            goto FAILURE;
        }

        if (u32UpdCnt == 0 || u32FrameNum >= u32PreUpdTime + u32FrmUpdPeriod) {
            u32UpdCnt++;
            u32PreUpdTime = u32FrameNum;
            stUpdateBgModelCtrl.u32CurFrmNum = u32FrameNum;
            stUpdateBgModelCtrl.u32PreChkTime = u32PreChkTime;
            stUpdateBgModelCtrl.u32FrmChkPeriod = 0;
            if (u32FrameNum >= u32PreChkTime + u32FrmChkPeriod) {
                stUpdateBgModelCtrl.u32FrmChkPeriod = u32FrmChkPeriod;
                u32PreChkTime = u32FrameNum;
            }

            s32Result = RK_MPI_IVE_UpdateBgModel(
                &hIveHandle, &stBgModel, &stFgFlag, &stBgImg, &stChgStaImg,
                &stChgStaFg, &stChgStaLife, &stStatData, &stUpdateBgModelCtrl,
                bInstant);
            if (s32Result != RK_SUCCESS) {
                goto FAILURE;
            }
        }

        // for show fg
        if (pstCvFg == RK_NULL) {
            stSize = cvSize(pstCvImg->width, pstCvImg->height);
            pstCvFg = cvCreateImage(stSize, 8, 1);
            if (RK_NULL == pstCvFg) {
                goto FAILURE;
            }
        }
        // extract fg
        stFg.enType = IVE_IMAGE_TYPE_U8C1;
        stFg.au64VirAddr[0] = stBgDiffFg.au64VirAddr[0];
        stFg.au64PhyAddr[0] = stBgDiffFg.au64PhyAddr[0];
        stFg.u32Width = stBgDiffFg.u32Width;
        stFg.u32Height = stBgDiffFg.u32Height;
        stFg.au32Stride[0] = stBgDiffFg.au32Stride[0];
        RK_MPI_IVE_Thresh(&hIveHandle, &stFg, &stFg, &stThrCtrl, bInstant);
        CopyIveToCv(pstCvFg, &stFg);

        // for show bg
        if (RK_NULL == pstCvBgGray) {
            stSize = cvSize(pstCvImg->width, pstCvImg->height);
            pstCvBgGray = cvCreateImage(stSize, 8, 1);
            if (RK_NULL == pstCvBgGray) {
                goto FAILURE;
            }
        }
        CopyIveToCv(pstCvBgGray, &stBgImg);

#if 0  // write fg avi
        if (RK_NULL == pstCvFgRGB) {
            stSize = cvSize(pstCvImg->width, pstCvImg->height);
            pstCvFgRGB = cvCreateImage(stSize, 8, 3);
            if (RK_NULL == pstCvFgRGB) {
                goto FAILURE;
            }
        }
        cvCvtColor(pstCvFg, pstCvFgRGB, CV_GRAY2BGR);

        if (pstCvFGAvi == NULL) {
            pstCvFGAvi =
                cvCreateVideoWriter("/root/data/avi/BgModelResult2.avi",
                                    CV_FOURCC('x', 'v', 'i', 'd'), 25, stSize);
            if (RK_NULL == pstCvFGAvi) {
                goto FAILURE;
            }
        }
        cvWriteFrame(pstCvFGAvi, pstCvFgRGB);
#endif

        cvNamedWindow("src", 0);
        cvShowImage("src", pstCvImg);
        cvNamedWindow("Fg", 0);
        cvShowImage("Fg", pstCvFg);
        cvNamedWindow("Bg", 0);
        cvShowImage("Bg", pstCvBgGray);
    }

FAILURE:
    RK_DestroyIveImage(&stCurImg);
    RK_DestroyIveImage(&stFgFlag);
    RK_DestroyIveImage(&stBgDiffFg);
    RK_DestroyIveImage(&stFrmDiffFg);
    RK_DestroyIveImage(&stBgImg);
    RK_DestroyIveImage(&stChgStaImg);
    RK_DestroyIveImage(&stChgStaFg);
    RK_DestroyIveImage(&stChgStaLife);
    RK_FREE_64(stBgModel.u64VirAddr);
    RK_FREE_64(stStatData.u64VirAddr);

    if (RK_NULL != pstCvFgRGB) cvReleaseImage(&pstCvFgRGB);
    if (RK_NULL != pstCvFg) cvReleaseImage(&pstCvFg);
    if (RK_NULL != pstCvBgGray) cvReleaseImage(&pstCvBgGray);
    if (RK_NULL != pstCvFGAvi) cvReleaseVideoWriter(&pstCvFGAvi);
    if (pstCvCap) cvReleaseCapture(&pstCvCap);
    cvDestroyAllWindows();

    return;
}
#endif