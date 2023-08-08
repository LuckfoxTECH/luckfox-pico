#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rk_mpi_ive.h"
#include "rve_tests.h"
#include "sample_assist.h"
#include "sample_define.h"
#include "sample_file.h"

RK_VOID ThreshSample(IVE_THRESH_MODE_E enMode, const RK_CHAR *moduleName,
                     RK_CHAR *fileName, RK_CHAR *outDir, RK_U32 u32Width,
                     RK_U32 u32Height) {
    IVE_SRC_IMAGE_S stSrc;
    IVE_DST_IMAGE_S stDst;
    IVE_THRESH_U8_CTRL_S stCtrl;
    RK_S32 s32Result;
    FILE *fp = NULL, *fo = NULL;
    IVE_HANDLE handle;
    RK_DOUBLE dTime;
    RK_CHAR outFile[RK_MAX_FILE_LEN];

    memset(&stSrc, 0, sizeof(IVE_SRC_IMAGE_S));
    memset(&stDst, 0, sizeof(IVE_DST_IMAGE_S));
    memset(&stCtrl, 0, sizeof(IVE_THRESH_U8_CTRL_S));

    s32Result =
        RK_CreateIveImage(&stSrc, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);
    s32Result =
        RK_CreateIveImage(&stDst, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    s32Result = RK_ReadFile(fileName, &stSrc, &fp);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    stCtrl.enMode = enMode;
    stCtrl.u8LowThr = 50;//RK_GenRand(RK_MAX_U8 - 1, RK_MIN_U8);
    stCtrl.u8HighThr = 150;//RK_GenRand(RK_MAX_U8, stCtrl.u8LowThr + 1);
    stCtrl.u8MinVal = 0;//RK_GenRand(RK_MAX_U8, RK_MIN_U8);
    stCtrl.u8MidVal = 128;//RK_GenRand(RK_MAX_U8, RK_MIN_U8);
    stCtrl.u8MaxVal = 255;//RK_GenRand(RK_MAX_U8, RK_MIN_U8);

    dTime = (RK_DOUBLE)RK_GetTickCount();
    s32Result = RK_MPI_IVE_Thresh(&handle, &stSrc, &stDst, &stCtrl, RK_TRUE);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);
    dTime = (RK_DOUBLE)RK_GetTickCount() - dTime;

    printf("Thresh mode %d,time = %g ms\n", enMode,
           dTime / ((RK_DOUBLE)RK_GetTickFrequency() * 1000.));

    if (outDir == NULL) {
        RK_WriteFile(NULL, (void *)stDst.au64VirAddr[0],
                     stDst.u32Width * stDst.u32Height, &fo);
    } else {
        RK_SPRINTF(outFile, RK_MAX_FILE_LEN, "%s/%02d_rve_%s_out.yuv", outDir, enMode, moduleName);
        RK_WriteFile(outFile, (void *)stDst.au64VirAddr[0],
                     stDst.u32Width * stDst.u32Height, &fo);
    }

 #if BUILD_SIMULATOR
    gfx_show_img("thresh", (void *)stDst.au64VirAddr[0], stDst.u32Width,
                 stDst.u32Height, IVE_IMAGE_TYPE_U8C1, 0);
#endif
FAILURE:
    RK_FCLOSE(fp);
    RK_FCLOSE(fo);
    RK_DestroyIveImage(&stSrc);
    RK_DestroyIveImage(&stDst);
}

RK_VOID ThreshSampleBinary(const RK_CHAR *moduleName, RK_CHAR *fileName,
                           RK_CHAR *outDir, RK_U32 u32Width, RK_U32 u32Height) {
    IVE_THRESH_MODE_E enMode = IVE_THRESH_MODE_BINARY;

    ThreshSample(enMode, moduleName, fileName, outDir, u32Width, u32Height);
}
RK_VOID ThreshSampleTrunc(const RK_CHAR *moduleName, RK_CHAR *fileName,
                          RK_CHAR *outDir, RK_U32 u32Width, RK_U32 u32Height) {
    IVE_THRESH_MODE_E enMode = IVE_THRESH_MODE_TRUNC;

    ThreshSample(enMode, moduleName, fileName, outDir, u32Width, u32Height);
}
RK_VOID ThreshSampleToMinval(const RK_CHAR *moduleName, RK_CHAR *fileName,
                             RK_CHAR *outDir, RK_U32 u32Width,
                             RK_U32 u32Height) {
    IVE_THRESH_MODE_E enMode = IVE_THRESH_MODE_TO_MINVAL;

    ThreshSample(enMode, moduleName, fileName, outDir, u32Width, u32Height);
}
RK_VOID ThreshSampleMinMidMax(const RK_CHAR *moduleName, RK_CHAR *fileName,
                              RK_CHAR *outDir, RK_U32 u32Width,
                              RK_U32 u32Height) {
    IVE_THRESH_MODE_E enMode = IVE_THRESH_MODE_MIN_MID_MAX;

    ThreshSample(enMode, moduleName, fileName, outDir, u32Width, u32Height);
}
RK_VOID ThreshSampleOriMidMax(const RK_CHAR *moduleName, RK_CHAR *fileName,
                              RK_CHAR *outDir, RK_U32 u32Width,
                              RK_U32 u32Height) {
    IVE_THRESH_MODE_E enMode = IVE_THRESH_MODE_ORI_MID_MAX;

    ThreshSample(enMode, moduleName, fileName, outDir, u32Width, u32Height);
}
RK_VOID ThreshSampleMinMidOri(const RK_CHAR *moduleName, RK_CHAR *fileName,
                              RK_CHAR *outDir, RK_U32 u32Width,
                              RK_U32 u32Height) {
    IVE_THRESH_MODE_E enMode = IVE_THRESH_MODE_MIN_MID_ORI;

    ThreshSample(enMode, moduleName, fileName, outDir, u32Width, u32Height);
}
RK_VOID ThreshSampleMinOriMax(const RK_CHAR *moduleName, RK_CHAR *fileName,
                              RK_CHAR *outDir, RK_U32 u32Width,
                              RK_U32 u32Height) {
    IVE_THRESH_MODE_E enMode = IVE_THRESH_MODE_MIN_ORI_MAX;

    ThreshSample(enMode, moduleName, fileName, outDir, u32Width, u32Height);
}
RK_VOID ThreshSampleOriMinOri(const RK_CHAR *moduleName, RK_CHAR *fileName,
                              RK_CHAR *outDir, RK_U32 u32Width,
                              RK_U32 u32Height) {
    IVE_THRESH_MODE_E enMode = IVE_THRESH_MODE_ORI_MID_ORI;

    ThreshSample(enMode, moduleName, fileName, outDir, u32Width, u32Height);
}