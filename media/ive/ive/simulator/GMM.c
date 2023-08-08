#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rk_mpi_ive.h"
#include "rve_tests.h"
#include "sample_assist.h"
#include "sample_define.h"
#include "sample_file.h"
#include "sample_gfx.h"
#include "sample_mem.h"

#ifndef MAX
#define MAX(a, b) ((a) < (b) ? (b) : (a))
#endif

RK_VOID GMM_Sample_U8C1(const RK_CHAR *moduleName, RK_CHAR *fileName, RK_CHAR *outDir,
                        RK_U32 u32Width, RK_U32 u32Height) {
    RK_S32 s32Ret;
    IVE_HANDLE hIveHandle;
    RK_BOOL bInstant = RK_TRUE;
    RK_DOUBLE dTime;
    RK_CHAR outFile[RK_MAX_FILE_LEN];

    IVE_SRC_IMAGE_S stIveImg;
    IVE_DST_IMAGE_S stIveFg;
    IVE_DST_IMAGE_S stIveBg;
    IVE_MEM_INFO_S stModel;
    IVE_DST_IMAGE_S stMatchModelInfo;
    IVE_GMM_CTRL_S stGmmCtrl = {0};

    FILE *fp = NULL, *fo_fg = NULL, *fo_bg = NULL;

    RK_U32 u32FrmNum = 0;
    RK_U32 u32TotalFrmNum = 5;
#if BUILD_SIMULATOR
    RK_U32 u32ModelSize = 6;
#else
    RK_U32 u32ModelSize = 4;
#endif

    // Initialization--------------------------------------------------------------------
    // set default parameters
    stGmmCtrl.u8PicFormat = 0;
    stGmmCtrl.u8FirstFrameFlag = 0;
    stGmmCtrl.u8EnBgOut = 1;
    stGmmCtrl.u8MaxModelNum = 5;
    stGmmCtrl.u8UpdateVar = 1;
    stGmmCtrl.u8q2WeightInitVal = 16;
    stGmmCtrl.u8q2WeightAddFactor = 4;
    stGmmCtrl.u8q2WeightReduFactor = 1016;
    stGmmCtrl.u8q2WeightThr = 4;
    stGmmCtrl.u8VarThreshGen = 9;
    stGmmCtrl.u8q2BgRatio = 712;
    stGmmCtrl.u10q0InitVar = 225;
    stGmmCtrl.u10q0MinVar = 200;
    stGmmCtrl.u10q0MaxVar = 512;
    stGmmCtrl.u8VarThr = 16;

    // for input image
    memset(&stIveImg, 0, sizeof(IVE_SRC_IMAGE_S));
    memset(&stIveFg, 0, sizeof(IVE_DST_IMAGE_S));
    memset(&stIveBg, 0, sizeof(IVE_DST_IMAGE_S));
    memset(&stMatchModelInfo, 0, sizeof(IVE_DST_IMAGE_S));
    memset(&stModel, 0, sizeof(IVE_MEM_INFO_S));

    fp = NULL;

    // for src
    s32Ret =
        RK_CreateIveImage(&stIveImg, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    RK_CHECK_NET_GOTO_PRT(s32Ret, RK_SUCCESS, FAILURE,
                          "RK_CreateIveImage fail\n");

    // for foreground
    s32Ret =
        RK_CreateIveImage(&stIveFg, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    RK_CHECK_NET_GOTO_PRT(s32Ret, RK_SUCCESS, FAILURE,
                          "RK_CreateIveImage fail\n");

    // for background
    s32Ret =
        RK_CreateIveImage(&stIveBg, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    RK_CHECK_NET_GOTO_PRT(s32Ret, RK_SUCCESS, FAILURE,
                          "RK_CreateIveImage fail\n");

    // for matched model information
    s32Ret = RK_CreateIveImage(&stMatchModelInfo, IVE_IMAGE_TYPE_U8C1, u32Width,
                               u32Height);
    RK_CHECK_NET_GOTO_PRT(s32Ret, RK_SUCCESS, FAILURE,
                          "RK_CreateIveImage fail\n");

    // for models
    stModel.u32Size = stGmmCtrl.u8MaxModelNum * u32ModelSize * u32Width *u32Height + sizeof(IVE_GMM_NUM_INFO_S);
    s32Ret = RK_CreateIveMem(&stModel, stModel.u32Size);
    RK_CHECK_ET_GOTO(s32Ret, RK_FAILURE, FAILURE);
    memset((RK_U8 *)stModel.u64VirAddr, 0, stModel.u32Size);

    // Process
    for (u32FrmNum = 1; u32FrmNum <= u32TotalFrmNum; u32FrmNum++) {
        RK_ReadFile(fileName, &stIveImg, &fp);

        if (u32FrmNum == 1) {
            stGmmCtrl.u8FirstFrameFlag = 1;
        } else {
            stGmmCtrl.u8FirstFrameFlag = 0;
        }

        // To building a stable background model quickly at the begin, some
        // parameters are set specially.
        if (1 == stGmmCtrl.u8MaxModelNum) {
            // If the parameter u8ModelNum is set to 1, the parameter
            // u16FreqReduFactor is usually set to a small value at the first N
            // frames. Here, N = 300.
            stGmmCtrl.u8q2WeightReduFactor = (u32FrmNum >= 300) ? 0x3F8 : 0x3E0;
        } else {
            // If the parameter u8ModelNum is more than 1, the global life mode
            // should be used at the first N frames, and the parameter
            // u16GlbLifeUpdateFactor is usually set to a big value. Here, N =
            // 300.
            //stGmmCtrl.u8q2WeightReduFactor = (u32FrmNum >= 300) ? 4 : 0x3FF / u32FrmNum;
        }

        dTime = (RK_DOUBLE)RK_GetTickCount();
        s32Ret = RK_MPI_IVE_GMM(&hIveHandle, &stIveImg, &stIveFg,
                                &stIveBg, &stMatchModelInfo, &stModel,
                                &stGmmCtrl, bInstant);
        RK_CHECK_NET_GOTO_PRT(s32Ret, RK_SUCCESS, FAILURE,
                              "RK_MPI_IVE_GMM fail,Error(%#x)\n", s32Ret);
        dTime = (RK_DOUBLE)RK_GetTickCount() - dTime;

        printf("GMM,time = %g ms\n",
               dTime / ((RK_DOUBLE)RK_GetTickFrequency() * 1000.));
    }

    if (outDir == NULL) {
        RK_WriteFile(NULL, (void *)stIveFg.au64VirAddr[0],
                     stIveFg.u32Width * stIveFg.u32Height, &fo_fg);
    } else {
        RK_SPRINTF(outFile, RK_MAX_FILE_LEN, "%s/rve_%s_fg_out.yuv", outDir, moduleName);
        RK_WriteFile(outFile, (void *)stIveFg.au64VirAddr[0],
                     stIveFg.u32Width * stIveFg.u32Height, &fo_fg);
    }

    if (outDir == NULL) {
        RK_WriteFile(NULL, (void *)stIveBg.au64VirAddr[0],
                     stIveBg.u32Width * stIveBg.u32Height, &fo_bg);
    } else {
        RK_SPRINTF(outFile, RK_MAX_FILE_LEN, "%s/rve_%s_bg_out.yuv", outDir, moduleName);
        RK_WriteFile(outFile, (void *)stIveBg.au64VirAddr[0],
                     stIveBg.u32Width * stIveBg.u32Height, &fo_bg);
    }

FAILURE:
    RK_FCLOSE(fp);
    RK_FCLOSE(fo_fg);
    RK_FCLOSE(fo_bg);
    RK_DestroyIveImage(&stIveImg);
    RK_DestroyIveImage(&stIveFg);
    RK_DestroyIveImage(&stIveBg);
    RK_DestroyIveImage(&stMatchModelInfo);
    RK_DestroyIveMem(&stModel);

    return;
}

static RK_S32 GMM2_Sample_GenFgMask(IVE_SRC_IMAGE_S *pstFg,
                                    IVE_DST_IMAGE_S *pstFgMask) {
    RK_S32 s32Ret;
    IVE_HANDLE IveHandle;
    IVE_THRESH_U8_CTRL_S stCtrl;

    stCtrl.enMode = IVE_THRESH_MODE_BINARY;
    stCtrl.u8MinVal = 0;
    stCtrl.u8MaxVal = 1;
    stCtrl.u8LowThr = 5;
    s32Ret = RK_MPI_IVE_Thresh(&IveHandle, pstFg, pstFgMask, &stCtrl, RK_TRUE);
    RK_CHECK_NET_RET_PRT(s32Ret, RK_SUCCESS, s32Ret,
                         "RK_MPI_IVE_Thresh fail,Error(%#x)\n", s32Ret);

    return s32Ret;
}
/*
 *Reduce the factor gradually to the default value
 */
static RK_VOID GMM2_Sample_ReduFactor(IVE_IMAGE_S *pstFactor) {
    RK_U16 i, j;
    RK_U8 *pu8Tmp;

    pu8Tmp = (RK_U8 *)pstFactor->au64VirAddr[0];

    MmzFlushStart(pu8Tmp);

    for (i = 0; i < pstFactor->u32Height; i++) {
        for (j = 0; j < pstFactor->u32Width; j++) {
            pu8Tmp[j << 1] = MAX(8, pu8Tmp[i << 1] - 1);
            pu8Tmp[(i << 1) + 1] = MAX(8, pu8Tmp[(i << 1) + 1] - 2);
        }
        pu8Tmp += pstFactor->au32Stride[0] * sizeof(RK_U16);
    }

    MmzFlushEnd(pu8Tmp);
}
/*
 *Change factor by difference frame
 */
static RK_BOOL GMM2_Sample_ChgFactorByDiffFrm(IVE_SRC_IMAGE_S *pstSrc,
                                              IVE_SRC_IMAGE_S *pstLastImg,
                                              IVE_DST_IMAGE_S *pstDiffImg,
                                              IVE_DST_IMAGE_S *pstFactor) {
    RK_S32 s32Ret;
    RK_BOOL bInstant = RK_TRUE;
    IVE_HANDLE IveHandle;
    RK_U16 i, j;
    RK_S32 s32AbsTh = 10;
    RK_U32 u32PointSum = 0;
    IVE_SUB_CTRL_S stSubCtrl;
    RK_U8 *pu8Tmp, *pu8Factor;

    stSubCtrl.enMode = IVE_SUB_MODE_ABS;
    s32Ret = RK_MPI_IVE_Sub(&IveHandle, pstSrc, pstLastImg, pstDiffImg,
                            &stSubCtrl, bInstant);
    RK_CHECK_NET_RET_PRT(s32Ret, RK_SUCCESS, RK_FALSE,
                         "RK_MPI_IVE_Sub fail,Error(%#x)\n", s32Ret);

    pu8Tmp = (RK_U8 *)pstDiffImg->au64VirAddr[0];

    MmzFlushStart((RK_VOID *)pu8Tmp);

    for (i = 0; i < pstSrc->u32Height; i++) {
        for (j = 0; j < pstSrc->u32Width; j++) {
            u32PointSum += pu8Tmp[j] > s32AbsTh;
        }
        pu8Tmp += pstDiffImg->au32Stride[0];
    }

    MmzFlushEnd((RK_VOID *)pu8Tmp);

    if ((u32PointSum * 3) >
        (RK_U16)(pstSrc->u32Width * pstSrc->u32Height * 2)) {
        pu8Tmp = (RK_U8 *)pstDiffImg->au64VirAddr[0];
        pu8Factor = (RK_U8 *)pstFactor->au64VirAddr[0];

        MmzFlushStart((RK_VOID *)pu8Tmp);
        MmzFlushStart((RK_VOID *)pu8Factor);

        for (i = 0; i < pstSrc->u32Height; i++) {
            for (j = 0; j < pstSrc->u32Width; j++) {
                if (pu8Tmp[j] > s32AbsTh) {
                    pu8Factor[j << 1] = 20;
                    pu8Factor[(j << 1) + 1] = 200;
                }
            }
            pu8Tmp += pstDiffImg->au32Stride[0];
            pu8Factor += pstFactor->au32Stride[0] * sizeof(RK_U16);
        }

        MmzFlushEnd((RK_VOID *)pu8Tmp);
        MmzFlushEnd((RK_VOID *)pu8Factor);

        return RK_TRUE;
    }
    return RK_FALSE;
}

/*
 *Change factor by gradient
 */
static RK_VOID GMM2_Sample_ChgFactorByGrad(
    IVE_SRC_IMAGE_S *pstFgMask, IVE_SRC_IMAGE_S *pstSrc,
    IVE_SRC_IMAGE_S *pstLastImg, IVE_SRC_IMAGE_S *pstLastNormMag,
    IVE_DST_IMAGE_S *pstMagImg, IVE_DST_IMAGE_S *pstAngImg, IVE_DST_IMAGE_S *pstCurNormMag,
    IVE_DST_MEM_INFO_S *pstBlob, IVE_DST_IMAGE_S *pstDiffImg,
    IVE_DST_IMAGE_S *pstFactor, IVE_MEM_INFO_S *pstMem) {

    RK_S32 s32Ret;
    RK_BOOL bInstant = RK_FALSE;
    IVE_HANDLE IveHandle;
    RK_U16 i, j, k;
    RK_U16 u16Top;
    RK_U16 u16Left;
    RK_U16 u16Right;
    RK_U16 u16Bottom;
    RK_U8 *pu8FgCur;
    RK_U8 *pu8GradDiff;
    RK_U8 *pu8Factor;
    RK_S32 s32FgSum = 0;
    RK_S32 s32GradSum = 0;

    RK_S8 au8Mask[25] = {0, 0, 0, 0,  0, 0, -1, 0, 1, 0, 0, -2, 0,
                         2, 0, 0, -1, 0, 1, 0,  0, 0, 0, 0, 0};
    IVE_CCL_CTRL_S stCclCtrl;
    IVE_CCBLOB_S *pstCclBlob;
    IVE_MAG_AND_ANG_CTRL_S stMagAndAngCtrl;
    IVE_16BIT_TO_8BIT_CTRL_S st16To8Ctrl;
    IVE_SUB_CTRL_S stSubCtrl;

    stCclCtrl.enMode = IVE_CCL_MODE_8C;
    stCclCtrl.u16Step = 2;
    stCclCtrl.u16InitAreaThr = 4;
    stCclCtrl.stMem = *pstMem;
    s32Ret =
        RK_MPI_IVE_CCL(&IveHandle, pstFgMask, pstBlob, &stCclCtrl, RK_TRUE);
    RK_CHECK_NET_RET_VOID_PRT(s32Ret, RK_SUCCESS,
                              "RK_MPI_IVE_CCL fail,Error(%#x)\n", s32Ret);

    pstCclBlob = (IVE_CCBLOB_S *)pstBlob->u64VirAddr;

    MmzFlushStart((RK_U8 *)pstCclBlob);

    if (pstCclBlob->u8RegionNum > 0) {
        // Calc the gradient difference of the current image and the last image
        memcpy(stMagAndAngCtrl.as8Mask, au8Mask, 25);
        stMagAndAngCtrl.u16Thr = 0;
        stMagAndAngCtrl.stMem = *pstMem;
        s32Ret = RK_MPI_IVE_MagAndAng(&IveHandle, pstSrc, pstMagImg, pstAngImg,
                                      &stMagAndAngCtrl, bInstant);
        RK_CHECK_NET_RET_VOID_PRT(s32Ret, RK_SUCCESS,
                                  "RK_MPI_IVE_MagAndAng fail,Error(%#x)\n",
                                  s32Ret);

        st16To8Ctrl.s8Bias = 0;
        st16To8Ctrl.enMode = IVE_16BIT_TO_8BIT_MODE_U16_TO_U8;
        st16To8Ctrl.u8Numerator = 255;
        st16To8Ctrl.u16Denominator = 255 * 4;
        s32Ret = RK_MPI_IVE_16BitTo8Bit(&IveHandle, pstMagImg, pstCurNormMag,
                                        &st16To8Ctrl, bInstant);
        RK_CHECK_NET_RET_VOID_PRT(s32Ret, RK_SUCCESS,
                                  "RK_MPI_IVE_16BitTo8Bit fail,Error(%#x)\n",
                                  s32Ret);

        s32Ret = RK_MPI_IVE_MagAndAng(&IveHandle, pstLastImg, pstMagImg,
                                      pstAngImg, &stMagAndAngCtrl, bInstant);
        RK_CHECK_NET_RET_VOID_PRT(s32Ret, RK_SUCCESS,
                                  "RK_MPI_IVE_MagAndAng fail,Error(%#x)\n",
                                  s32Ret);

        s32Ret = RK_MPI_IVE_16BitTo8Bit(&IveHandle, pstMagImg, pstLastNormMag,
                                        &st16To8Ctrl, bInstant);
        RK_CHECK_NET_RET_VOID_PRT(s32Ret, RK_SUCCESS,
                                  "RK_MPI_IVE_16BitTo8Bit fail,Error(%#x)\n",
                                  s32Ret);

        stSubCtrl.enMode = IVE_SUB_MODE_ABS;
        s32Ret = RK_MPI_IVE_Sub(&IveHandle, pstCurNormMag, pstLastNormMag,
                                pstDiffImg, &stSubCtrl, RK_TRUE);
        RK_CHECK_NET_RET_VOID_PRT(s32Ret, RK_SUCCESS,
                                  "RK_MPI_IVE_Sub fail,Error(%#x)\n", s32Ret);
    }

    // for each blob, analyze the gradient change
    for (k = 0; k < IVE_MAX_REGION_NUM; k++) {
        if (0 == pstCclBlob->astRegion[k].u32Area) {
            continue;
        }

        u16Top = pstCclBlob->astRegion[k].u16Top;
        u16Left = pstCclBlob->astRegion[k].u16Left;
        u16Right = pstCclBlob->astRegion[k].u16Right;
        u16Bottom = pstCclBlob->astRegion[k].u16Bottom;
        if ((u16Right - u16Left) * (u16Bottom - u16Top) < 60) {
            continue;
        }

        s32FgSum = 0;
        s32GradSum = 0;
        pu8FgCur = (RK_U8 *)(pstFgMask->au64VirAddr[0] +
                             u16Top * pstFgMask->au32Stride[0]);
        pu8GradDiff = (RK_U8 *)(pstDiffImg->au64VirAddr[0] +
                                u16Top * pstDiffImg->au32Stride[0]);
        for (i = u16Top; i < u16Bottom; i++) {
            for (j = u16Left; j < u16Right; j++) {
                if (pu8FgCur[j]) {
                    s32FgSum++;
                    if (pu8GradDiff[j]) {
                        s32GradSum++;
                    }
                }
            }

            pu8FgCur += pstFgMask->au32Stride[0];
            pu8GradDiff += pstDiffImg->au32Stride[0];
        }

        if ((s32GradSum < 10) || (s32GradSum * 100 < s32FgSum)) {
            pu8Factor = (RK_U8 *)(pstFactor->au64VirAddr[0] +
                                  2 * u16Top * pstFactor->au32Stride[0]);
            pu8FgCur = (RK_U8 *)(pstFgMask->au64VirAddr[0] +
                                 u16Top * pstFgMask->au32Stride[0]);
            for (i = u16Top; i < u16Bottom; i++) {
                for (j = u16Left; j < u16Right; j++) {
                    if (pu8FgCur[j]) {
                        pu8Factor[j << 1] = 16;
                        pu8Factor[(j << 1) + 1] = 200;
                    }
                }
                pu8FgCur += pstFgMask->au32Stride[0];
                pu8Factor += pstFactor->au32Stride[0] * sizeof(RK_U16);
            }
        }
    }

    MmzFlushEnd((RK_U8 *)pstCclBlob);
}
/*
 *Adjustment factor
 */
static RK_VOID GMM2_Sample_AdjustmentFactor(
    IVE_SRC_IMAGE_S *pstFactor, IVE_SRC_IMAGE_S *pstFgMask,
    IVE_SRC_IMAGE_S *pstSrc, IVE_SRC_IMAGE_S *pstLastImg,
    IVE_DST_IMAGE_S *pstDiffImg, IVE_DST_IMAGE_S *pstMagImg, IVE_DST_IMAGE_S *pstAngImg,
    IVE_DST_IMAGE_S *pstCurNormMag, IVE_DST_IMAGE_S *pstLastNormMag,
    IVE_DST_MEM_INFO_S *pstBlob, IVE_MEM_INFO_S *pstMem) {
    RK_BOOL bChg;
    // First, reduce the factor gradually to the default
    // value------------------------------
    GMM2_Sample_ReduFactor(pstFactor);

    // Second, analyze the frame
    // difference------------------------------------------------- When the
    // number of changed pixels is more than the threshold, there maybe a light
    // switch. And then, we should set a big factor to adapt to it quickly.
    bChg = GMM2_Sample_ChgFactorByDiffFrm(pstSrc, pstLastImg, pstDiffImg,
                                       pstFactor);
    if (RK_TRUE == bChg) {
        return;
    }

    // Third, analyze the gradient for foreground
    // blobs------------------------------------ When gradient change of a
    // foreground blob is very small, it maybe a local illumination change, a
    // ghost, or a static object. Here we try to reduce the influence by a local
    // illumination change or a ghost only.
    GMM2_Sample_ChgFactorByGrad(pstFgMask, pstSrc, pstLastImg, pstLastNormMag,
                                pstMagImg, pstAngImg, pstCurNormMag, pstBlob, pstDiffImg,
                                pstFactor, pstMem);
}

RK_VOID GMM2_Sample_U8C1_PixelCtrl(const RK_CHAR *moduleName, RK_CHAR *fileName, RK_CHAR *outDir,
                        RK_U32 u32Width, RK_U32 u32Height) {
    RK_S32 i, s32Ret;
    RK_U32 u32totalFrame;
    IVE_HANDLE hIveHandle;
    RK_BOOL bInstant = RK_TRUE;
    FILE *fp = NULL, *fo_fg = NULL;
    RK_CHAR outFile[RK_MAX_FILE_LEN];

    IVE_SRC_IMAGE_S stIveImg;
    IVE_DST_IMAGE_S stIveFg;
    IVE_DST_IMAGE_S stIveBg;
    IVE_MEM_INFO_S stModel;
    IVE_GMM2_CTRL_S stGmm2Ctrl;
    IVE_SRC_IMAGE_S stFactor;
    IVE_DST_IMAGE_S stMatchModelInfo;
    IVE_SRC_IMAGE_S stFgMask;
    IVE_SRC_IMAGE_S stLastImg;
    IVE_DST_IMAGE_S stDiffImg;
    IVE_DST_IMAGE_S stMagImg;
    IVE_DST_IMAGE_S stAngImg;
    IVE_DST_IMAGE_S stCurNormMag;
    IVE_DST_IMAGE_S stLastNormMag;
    IVE_DST_MEM_INFO_S stBlob;
    IVE_MEM_INFO_S stMem;

#if BUILD_SIMULATOR
    RK_U32 u32ModelSize = 6;
#else
    RK_U32 u32ModelSize = 4;
#endif

    RK_U32 s32FrmNum = 0;
    RK_S32 s32OneFrmProcess = 0;
    RK_S32 s32Key;

    // Initialization--------------------------------------------------------------------
    // set default parameters
    stGmm2Ctrl.u8PicFormat = 0;
    stGmm2Ctrl.u8FirstFrameFlag = 0;
    stGmm2Ctrl.u8EnBgOut = 1;
    stGmm2Ctrl.u8MaxModelNum = 3;
    stGmm2Ctrl.u8UseVarFactor = 1;
    stGmm2Ctrl.u8GlobalLearningRateMode = 0;
    stGmm2Ctrl.u8UpdateVar = 1;
    stGmm2Ctrl.u8q2WeightInitVal = 16;
    stGmm2Ctrl.u8q2WeightAddFactor = 4;
    stGmm2Ctrl.u8q2WeightReduFactor = 1016;
    stGmm2Ctrl.u8q2WeightThr = 4;
    stGmm2Ctrl.u8VarThreshGen = 9;
    stGmm2Ctrl.u8q2BgRatio = 712;
    stGmm2Ctrl.u10q0InitVar = 225;
    stGmm2Ctrl.u10q0MinVar = 200;
    stGmm2Ctrl.u10q0MaxVar = 512;
    stGmm2Ctrl.u8VarThr = 16;

    memset(&stIveImg, 0, sizeof(IVE_SRC_IMAGE_S));
    memset(&stIveFg, 0, sizeof(IVE_DST_IMAGE_S));
    memset(&stIveBg, 0, sizeof(IVE_DST_IMAGE_S));
    memset(&stFactor, 0, sizeof(IVE_SRC_IMAGE_S));
    memset(&stMatchModelInfo, 0, sizeof(IVE_DST_IMAGE_S));
    memset(&stModel, 0, sizeof(IVE_MEM_INFO_S));
    memset(&stFgMask, 0, sizeof(IVE_DST_IMAGE_S));
    memset(&stLastImg, 0, sizeof(IVE_DST_IMAGE_S));
    memset(&stDiffImg, 0, sizeof(IVE_DST_IMAGE_S));
    memset(&stMagImg, 0, sizeof(IVE_DST_IMAGE_S));
    memset(&stAngImg, 0, sizeof(IVE_DST_IMAGE_S));
    memset(&stCurNormMag, 0, sizeof(IVE_DST_IMAGE_S));
    memset(&stLastNormMag, 0, sizeof(IVE_DST_IMAGE_S));
    memset(&stBlob, 0, sizeof(IVE_MEM_INFO_S));
    memset(&stMem, 0, sizeof(IVE_MEM_INFO_S));

    RK_MPI_IVE_Init();

#if BUILD_SIMULATOR
    gfx_query_frame(&u32Width, &u32Height, &u32totalFrame, fileName);
#else
    u32totalFrame = RK_ReadFileSize(fileName, &fp) / (u32Width * u32Height);
#endif

    // for input image
    s32Ret = RK_CreateIveImage(&stIveImg, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    RK_CHECK_NET_GOTO_PRT(s32Ret, RK_SUCCESS, FAILURE, "RK_CreateIveImage fail\n");
    RK_Memset((RK_U8 *)stIveImg.au64VirAddr[0], 0, sizeof(RK_U8) * u32Width * u32Height);

    // for foreground
    s32Ret = RK_CreateIveImage(&stIveFg, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    RK_CHECK_NET_GOTO_PRT(s32Ret, RK_SUCCESS, FAILURE, "RK_CreateIveImage fail\n");
    RK_Memset((RK_U8 *)stIveFg.au64VirAddr[0], 0, sizeof(RK_U8) * u32Width * u32Height);

    // for background
    s32Ret = RK_CreateIveImage(&stIveBg, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    RK_CHECK_NET_GOTO_PRT(s32Ret, RK_SUCCESS, FAILURE, "RK_CreateIveImage fail\n");
    RK_Memset((RK_U8 *)stIveBg.au64VirAddr[0], 0, sizeof(RK_U8) * u32Width * u32Height);

    // for sensitivity factor
    s32Ret = RK_CreateIveImage(&stFactor, IVE_IMAGE_TYPE_U16C1, u32Width, u32Height);
    RK_CHECK_NET_GOTO_PRT(s32Ret, RK_SUCCESS, FAILURE, "RK_CreateIveImage fail\n");
    MmzFlushStart((RK_U8 *)stFactor.au64VirAddr[0]);
    for (i = 0; i < u32Width * u32Height; i++) {
        ((RK_U8 *)stFactor.au64VirAddr[0])[i << 1] = 8;
        ((RK_U8 *)stFactor.au64VirAddr[0])[(i << 1) + 1] = 4;
    }
    MmzFlushEnd((RK_U8 *)stFactor.au64VirAddr[0]);

    // for matched model information
    s32Ret = RK_CreateIveImage(&stMatchModelInfo, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    RK_CHECK_NET_GOTO_PRT(s32Ret, RK_SUCCESS, FAILURE,  "RK_CreateIveImage fail\n");
    RK_Memset((RK_U8 *)stMatchModelInfo.au64VirAddr[0], 0, sizeof(RK_U8) * u32Width * u32Height);

    // for models
    stModel.u32Size =
        stGmm2Ctrl.u8MaxModelNum * u32ModelSize * u32Width * u32Height + sizeof(IVE_GMM_NUM_INFO_S);
    s32Ret = RK_CreateIveMem(&stModel, stModel.u32Size);
    RK_CHECK_NET_GOTO_PRT(s32Ret, RK_SUCCESS, FAILURE, "RK_CreateIveMem fail\n");
    RK_Memset((RK_U8 *)stModel.u64VirAddr, 0, stModel.u32Size);

    // for pixel factor adjustment
    s32Ret = RK_CreateIveImage(&stFgMask, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    RK_CHECK_NET_GOTO_PRT(s32Ret, RK_SUCCESS, FAILURE, "RK_CreateIveImage fail\n");
    RK_Memset((RK_U8 *)stFgMask.au64VirAddr[0], 0, sizeof(RK_U8) * u32Width * u32Height);

    s32Ret = RK_CreateIveImage(&stLastImg, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    RK_CHECK_NET_GOTO_PRT(s32Ret, RK_SUCCESS, FAILURE, "RK_CreateIveImage fail\n");
    RK_Memset((RK_U8 *)stLastImg.au64VirAddr[0], 0, sizeof(RK_U8) * u32Width * u32Height);

    s32Ret = RK_CreateIveImage(&stDiffImg, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    RK_CHECK_NET_GOTO_PRT(s32Ret, RK_SUCCESS, FAILURE, "RK_CreateIveImage fail\n");
    RK_Memset((RK_U8 *)stDiffImg.au64VirAddr[0], 0, sizeof(RK_U8) * u32Width * u32Height);

    s32Ret = RK_CreateIveImage(&stMagImg, IVE_IMAGE_TYPE_U16C1, u32Width, u32Height);
    RK_CHECK_NET_GOTO_PRT(s32Ret, RK_SUCCESS, FAILURE, "RK_CreateIveImage fail\n");
    RK_Memset((RK_U8 *)stMagImg.au64VirAddr[0], 0, sizeof(RK_U16) * u32Width * u32Height);

    s32Ret = RK_CreateIveImage(&stAngImg, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    RK_CHECK_NET_GOTO_PRT(s32Ret, RK_SUCCESS, FAILURE, "RK_CreateIveImage fail\n");
    RK_Memset((RK_U8 *)stAngImg.au64VirAddr[0], 0, sizeof(RK_U8) * u32Width * u32Height);

    s32Ret = RK_CreateIveImage(&stCurNormMag, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    RK_CHECK_NET_GOTO_PRT(s32Ret, RK_SUCCESS, FAILURE, "RK_CreateIveImage fail\n");
    RK_Memset((RK_U8 *)stCurNormMag.au64VirAddr[0], 0, sizeof(RK_U8) * u32Width * u32Height);

    s32Ret = RK_CreateIveImage(&stLastNormMag, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    RK_CHECK_NET_GOTO_PRT(s32Ret, RK_SUCCESS, FAILURE, "RK_CreateIveImage fail\n");
    RK_Memset((RK_U8 *)stLastNormMag.au64VirAddr[0], 0, sizeof(RK_U8) * u32Width * u32Height);

    // for CC BLOB
    stBlob.u32Size = sizeof(IVE_CCBLOB_S);
    s32Ret = RK_CreateIveMem(&stBlob, stBlob.u32Size);
    RK_CHECK_NET_GOTO_PRT(s32Ret, RK_SUCCESS, FAILURE, "RK_CreateIveMem fail\n");

    // staging mem for magandang
    stMem.u32Size = u32Width * u32Height * 4;
    s32Ret = RK_CreateIveMem(&stMem, stMem.u32Size);
    RK_CHECK_NET_GOTO_PRT(s32Ret, RK_SUCCESS, FAILURE, "RK_CreateIveMem fail\n");

    // Process
    for (s32FrmNum = 1; s32FrmNum < u32totalFrame; s32FrmNum++) {
#if BUILD_SIMULATOR
        gfx_read_frame((RK_U8 *)stIveImg.au64VirAddr[0], u32Width * u32Height, fileName);
#else
        RK_ReadFile(fileName, &stIveImg, &fp);
#endif
        if (s32FrmNum == 1) {
            stGmm2Ctrl.u8FirstFrameFlag = 1;
        } else {
            stGmm2Ctrl.u8FirstFrameFlag = 0;
        }
    
        // To building a stable background model quickly at the begin, some
        // parameters are set specially.
        if (1 == stGmm2Ctrl.u8MaxModelNum) {
            stGmm2Ctrl.u8q2WeightAddFactor = (s32FrmNum >= 300) ? 16 : 200;
        } else {
            if (s32FrmNum >= 100) {
                stGmm2Ctrl.u8q2WeightAddFactor = 4;
            } else {
                stGmm2Ctrl.u8q2WeightAddFactor = 0x3FF / s32FrmNum;
            }
        }

        // GMM2
        s32Ret = RK_MPI_IVE_GMM2(&hIveHandle, &stIveImg, &stFactor, &stIveFg,
                                 &stIveBg, &stMatchModelInfo, &stModel,
                                 &stGmm2Ctrl, bInstant);
        RK_CHECK_NET_GOTO_PRT(s32Ret, RK_SUCCESS, FAILURE,
                              "RK_MPI_IVE_GMM2 fail,Error(%#x)\n", s32Ret);

        // factor adjustment
        if (s32FrmNum > 1) {
            // gen fg mask
            GMM2_Sample_GenFgMask(&stIveFg, &stFgMask);
            // adjustment  factor
            GMM2_Sample_AdjustmentFactor(
                &stFactor, &stFgMask, &stIveImg, &stLastImg, &stDiffImg,
                &stMagImg, &stAngImg, &stCurNormMag, &stLastNormMag, &stBlob, &stMem);
        }
        RK_Memcpy((RK_U8 *)stLastImg.au64VirAddr[0], (RK_U8 *)stIveImg.au64VirAddr[0], u32Width * u32Height);

        if (NULL != outDir) {
#if BUILD_SIMULATOR
            RK_SPRINTF(outFile, RK_MAX_FILE_LEN, "%s/rve_%s_fg_out.avi", outDir, moduleName);
            gfx_write_frame((RK_U8 *)stIveFg.au64VirAddr[0], u32Width,
                            u32Height, IVE_IMAGE_TYPE_U8C1,
                            (s32FrmNum == u32totalFrame - 1) ? 1 : 0, outFile);
            gfx_show_img("ori", stIveImg.au64VirAddr[0], u32Width, u32Height, IVE_IMAGE_TYPE_U8C1, 1);
            gfx_show_img("fg", (void*)stIveFg.au64VirAddr[0], u32Width, u32Height, IVE_IMAGE_TYPE_U8C1, 1);
            gfx_show_img("bg", stIveBg.au64VirAddr[0], u32Width, u32Height, IVE_IMAGE_TYPE_U8C1, 1);
#else
            RK_SPRINTF(outFile, RK_MAX_FILE_LEN, "%s/rve_%s_fg_out.yuv", outDir, moduleName);
            RK_WriteFile(outFile, (void *)stIveFg.au64VirAddr[0],
                     stIveFg.u32Width * stIveFg.u32Height, &fo_fg);
#endif
        }
    }

FAILURE:
    RK_FCLOSE(fp);
    RK_FCLOSE(fo_fg);
    RK_DestroyIveImage(&stIveImg);
    RK_DestroyIveImage(&stIveFg);
    RK_DestroyIveImage(&stIveBg);
    RK_DestroyIveImage(&stFactor);
    RK_DestroyIveImage(&stMatchModelInfo);
    RK_DestroyIveImage(&stFgMask);
    RK_DestroyIveImage(&stLastImg);
    RK_DestroyIveImage(&stDiffImg);
    RK_DestroyIveImage(&stMagImg);
    RK_DestroyIveImage(&stCurNormMag);
    RK_DestroyIveImage(&stLastNormMag);
    RK_DestroyIveMem(&stModel);
    RK_DestroyIveMem(&stBlob);
    RK_DestroyIveMem(&stMem);

    RK_MPI_IVE_Deinit();

    return;
}