#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rk_mpi_ive.h"
#include "rve_tests.h"
#include "sample_assist.h"
#include "sample_define.h"
#include "sample_file.h"

RK_VOID SobelSample(IVE_SOBEL_OUT_CTRL_E enOutCtrl, RK_S8 *ps8Mask,
                    const RK_CHAR *moduleName, RK_CHAR *fileName, RK_CHAR *outDir,
                    RK_U32 u32Width, RK_U32 u32Height) {
    IVE_SRC_IMAGE_S stSrc;
    IVE_DST_IMAGE_S stDstH;
    IVE_DST_IMAGE_S stDstV;
    IVE_SOBEL_CTRL_S stCtrl;
    RK_S32 s32Result;
    FILE *fp = NULL, *fo0 = NULL, *fo1 = NULL;
    IVE_HANDLE handle;
    RK_DOUBLE dTime;
    RK_CHAR outFile[RK_MAX_FILE_LEN];

    memset(&stSrc, 0, sizeof(IVE_SRC_IMAGE_S));
    memset(&stDstH, 0, sizeof(IVE_DST_IMAGE_S));
    memset(&stDstV, 0, sizeof(IVE_DST_IMAGE_S));
    memset(&stCtrl, 0, sizeof(IVE_MAG_AND_ANG_CTRL_S));

    s32Result =
        RK_CreateIveImage(&stSrc, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    if (IVE_SOBEL_OUT_CTRL_VER != enOutCtrl) {
        s32Result = RK_CreateIveImage(&stDstH, IVE_IMAGE_TYPE_S16C1, u32Width,
                                      u32Height);
        RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);
    }

    if (IVE_SOBEL_OUT_CTRL_HOR != enOutCtrl) {
        s32Result = RK_CreateIveImage(&stDstV, IVE_IMAGE_TYPE_S16C1, u32Width,
                                      u32Height);
        RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);
    }

    s32Result = RK_ReadFile(fileName, &stSrc, &fp);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    stCtrl.u8Norm = 0;
    stCtrl.u8OutCtrl = enOutCtrl;
    stCtrl.u8OutMode = 3;
    memcpy(stCtrl.as8Mask, ps8Mask, sizeof(RK_S8) * 25);

    dTime = (RK_DOUBLE)RK_GetTickCount();
    s32Result = RK_MPI_IVE_Sobel(
        &handle, &stSrc,
        IVE_SOBEL_OUT_CTRL_VER != enOutCtrl ? &stDstH : RK_NULL,
        IVE_SOBEL_OUT_CTRL_HOR != enOutCtrl ? &stDstV : RK_NULL, &stCtrl,
        RK_TRUE);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);
    dTime = (RK_DOUBLE)RK_GetTickCount() - dTime;

    printf("Sobel outCtrl %d,time = %g ms\n", enOutCtrl,
           dTime / ((RK_DOUBLE)RK_GetTickFrequency() * 1000.));

    if (IVE_SOBEL_OUT_CTRL_VER != enOutCtrl) {
        if (outDir == NULL) {
            RK_WriteFile(NULL, (void *)stDstH.au64VirAddr[0],
                         u32Width * u32Height * 2, &fo0);
        } else {
            RK_SPRINTF(outFile, RK_MAX_FILE_LEN, "%s/rve_%s_h_out.yuv", outDir, moduleName);
            RK_WriteFile(outFile, (void *)stDstH.au64VirAddr[0],
                         u32Width * u32Height * 2, &fo0);
        }
    }
    if (IVE_SOBEL_OUT_CTRL_HOR != enOutCtrl) {
        if (outDir == NULL) {
            RK_WriteFile(NULL, (void *)stDstV.au64VirAddr[0],
                         u32Width * u32Height * 2, &fo1);
        } else {
            RK_SPRINTF(outFile, RK_MAX_FILE_LEN, "%s/rve_%s_v_out.yuv", outDir, moduleName);
            RK_WriteFile(outFile, (void *)stDstV.au64VirAddr[0],
                         u32Width * u32Height * 2, &fo1);
        }
    }

FAILURE:
    RK_FCLOSE(fp);
    RK_FCLOSE(fo0);
    RK_FCLOSE(fo1);
    RK_DestroyIveImage(&stSrc);
    RK_DestroyIveImage(&stDstH);
    RK_DestroyIveImage(&stDstV);
}

RK_VOID SobelSampleBoth3x3(const RK_CHAR *moduleName, RK_CHAR *fileName,
                           RK_CHAR *outDir, RK_U32 u32Width, RK_U32 u32Height) {
    IVE_SOBEL_OUT_CTRL_E enOutCtrl = IVE_SOBEL_OUT_CTRL_BOTH;
    RK_S8 mask[25] = {0, 0, 0, 0,  0, 0, -1, 0, 1, 0, 0, -2, 0,
                      2, 0, 0, -1, 0, 1, 0,  0, 0, 0, 0, 0};

    SobelSample(enOutCtrl, mask, moduleName, fileName, outDir, u32Width,
                u32Height);
}

RK_VOID SobelSampleBoth5x5(const RK_CHAR *moduleName, RK_CHAR *fileName,
                           RK_CHAR *outDir, RK_U32 u32Width, RK_U32 u32Height) {
    IVE_SOBEL_OUT_CTRL_E enOutCtrl = IVE_SOBEL_OUT_CTRL_BOTH;
    RK_S8 mask[25] = {-1, -2, 0,  2,  1, -4, -8, 0,  8,  4, -6, -12, 0,
                      12, 6,  -4, -8, 0, 8,  4,  -1, -2, 0, 2,  1};

    SobelSample(enOutCtrl, mask, moduleName, fileName, outDir, u32Width,
                u32Height);
}
RK_VOID SobelSampleHor3x3(const RK_CHAR *moduleName, RK_CHAR *fileName,
                          RK_CHAR *outDir, RK_U32 u32Width, RK_U32 u32Height) {
    IVE_SOBEL_OUT_CTRL_E enOutCtrl = IVE_SOBEL_OUT_CTRL_HOR;
    RK_S8 mask[25] = {0, 0, 0, 0,  0, 0, -1, 0, 1, 0, 0, -2, 0,
                      2, 0, 0, -1, 0, 1, 0,  0, 0, 0, 0, 0};

    SobelSample(enOutCtrl, mask, moduleName, fileName, outDir, u32Width,
                u32Height);
}
RK_VOID SobelSampleHor5x5(const RK_CHAR *moduleName, RK_CHAR *fileName,
                          RK_CHAR *outDir, RK_U32 u32Width, RK_U32 u32Height) {
    IVE_SOBEL_OUT_CTRL_E enOutCtrl = IVE_SOBEL_OUT_CTRL_HOR;
    RK_S8 mask[25] = {-1, -2, 0,  2,  1, -4, -8, 0,  8,  4, -6, -12, 0,
                      12, 6,  -4, -8, 0, 8,  4,  -1, -2, 0, 2,  1};

    SobelSample(enOutCtrl, mask, moduleName, fileName, outDir, u32Width,
                u32Height);
}
RK_VOID SobelSampleVer3x3(const RK_CHAR *moduleName, RK_CHAR *fileName,
                          RK_CHAR *outDir, RK_U32 u32Width, RK_U32 u32Height) {
    IVE_SOBEL_OUT_CTRL_E enOutCtrl = IVE_SOBEL_OUT_CTRL_VER;
    RK_S8 mask[25] = {0, 0, 0, 0,  0, 0, -1, 0, 1, 0, 0, -2, 0,
                      2, 0, 0, -1, 0, 1, 0,  0, 0, 0, 0, 0};

    SobelSample(enOutCtrl, mask, moduleName, fileName, outDir, u32Width,
                u32Height);
}
RK_VOID SobelSampleVer5x5(const RK_CHAR *moduleName, RK_CHAR *fileName,
                          RK_CHAR *outDir, RK_U32 u32Width, RK_U32 u32Height) {
    IVE_SOBEL_OUT_CTRL_E enOutCtrl = IVE_SOBEL_OUT_CTRL_VER;
    RK_S8 mask[25] = {-1, -2, 0,  2,  1, -4, -8, 0,  8,  4, -6, -12, 0,
                      12, 6,  -4, -8, 0, 8,  4,  -1, -2, 0, 2,  1};

    SobelSample(enOutCtrl, mask, moduleName, fileName, outDir, u32Width,
                u32Height);
}