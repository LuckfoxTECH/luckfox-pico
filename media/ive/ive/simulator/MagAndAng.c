#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rk_mpi_ive.h"
#include "rve_tests.h"
#include "sample_assist.h"
#include "sample_define.h"
#include "sample_file.h"

RK_VOID MagAndAngSample(RK_S8 *ps8Mask,
                        RK_BOOL bUseThresh, const RK_CHAR *moduleName,
                        RK_CHAR *fileName, RK_CHAR *outDir, RK_U32 u32Width,
                        RK_U32 u32Height) {
    IVE_SRC_IMAGE_S stSrc;
    IVE_DST_IMAGE_S stDstMag;
    IVE_DST_IMAGE_S stDstAng;
    IVE_MAG_AND_ANG_CTRL_S stCtrl;
    RK_S32 s32Result;
    FILE *fp = NULL, *fo0 = NULL, *fo1 = NULL;
    IVE_HANDLE handle;
    RK_DOUBLE dTime;
    RK_CHAR outFile[RK_MAX_FILE_LEN];

    memset(&stSrc, 0, sizeof(IVE_SRC_IMAGE_S));
    memset(&stDstMag, 0, sizeof(IVE_DST_IMAGE_S));
    memset(&stDstAng, 0, sizeof(IVE_DST_IMAGE_S));
    memset(&stCtrl, 0, sizeof(IVE_MAG_AND_ANG_CTRL_S));

    s32Result =
        RK_CreateIveImage(&stSrc, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);
    s32Result =
        RK_CreateIveImage(&stDstMag, IVE_IMAGE_TYPE_U16C1, u32Width, u32Height);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);
    s32Result = 
        RK_CreateIveImage(&stDstAng, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    stCtrl.stMem.u32Size = u32Width * u32Height * sizeof(RK_S16) * 2;
    s32Result = RK_CreateIveMem(&stCtrl.stMem, stCtrl.stMem.u32Size);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    s32Result = RK_ReadFile(fileName, &stSrc, &fp);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    stCtrl.u16Thr =
        RK_TRUE == bUseThresh ? RK_GenRand(RK_MAX_U16, RK_MIN_U16 + 1) : 0;
    memcpy(stCtrl.as8Mask, ps8Mask, sizeof(RK_S8) * 25);

    dTime = (RK_DOUBLE)RK_GetTickCount();
    s32Result = RK_MPI_IVE_MagAndAng(
        &handle, &stSrc, &stDstMag,
        &stDstAng,
        &stCtrl, RK_TRUE);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);
    dTime = (RK_DOUBLE)RK_GetTickCount() - dTime;

    printf("MagAndAng time = %g ms\n",
           dTime / ((RK_DOUBLE)RK_GetTickFrequency() * 1000.));

    if (outDir == NULL) {
        RK_WriteFile(NULL, (void *)stDstMag.au64VirAddr[0],
                     stDstMag.u32Width * stDstMag.u32Height * 2, &fo0);
    } else {
        RK_SPRINTF(outFile, RK_MAX_FILE_LEN, "%s/rve_%s_mag_out.yuv", outDir, moduleName);
        RK_WriteFile(outFile, (void *)stDstMag.au64VirAddr[0],
                     stDstMag.u32Width * stDstMag.u32Height * 2, &fo0);
    }

    if (outDir == NULL) {
        RK_WriteFile(NULL, (void *)stDstAng.au64VirAddr[0],
                        stDstAng.u32Width * stDstAng.u32Height, &fo1);
    } else {
        RK_SPRINTF(outFile, RK_MAX_FILE_LEN, "%s/rve_%s_ang_out.yuv", outDir, moduleName);
        RK_WriteFile(outFile, (void *)stDstAng.au64VirAddr[0],
                        stDstAng.u32Width * stDstAng.u32Height, &fo1);
    }

FAILURE:
    RK_FCLOSE(fp);
    RK_FCLOSE(fo0);
    RK_FCLOSE(fo1);
    RK_DestroyIveImage(&stSrc);
    RK_DestroyIveImage(&stDstMag);
    RK_DestroyIveImage(&stDstAng);
    RK_DestroyIveMem(&stCtrl.stMem);
}

RK_VOID MagAndAngSampleMagAndAng3x3(const RK_CHAR *moduleName, RK_CHAR *fileName,
                                    RK_CHAR *outDir, RK_U32 u32Width,
                                    RK_U32 u32Height) {
    RK_S8 mask[25] = {0, 0, 0, 0,  0, 0, -1, 0, 1, 0, 0, -2, 0,
                      2, 0, 0, -1, 0, 1, 0,  0, 0, 0, 0, 0};

    MagAndAngSample(mask, RK_FALSE, moduleName, fileName, outDir,
                    u32Width, u32Height);
}
RK_VOID MagAndAngSampleMagAndAng5x5(const RK_CHAR *moduleName, RK_CHAR *fileName,
                                    RK_CHAR *outDir, RK_U32 u32Width,
                                    RK_U32 u32Height) {
    RK_S8 mask[25] = {-1, -2, 0,  2,  1, -4, -8, 0,  8,  4, -6, -12, 0,
                      12, 6,  -4, -8, 0, 8,  4,  -1, -2, 0, 2,  1};
    MagAndAngSample(mask, RK_FALSE, moduleName, fileName, outDir,
                    u32Width, u32Height);
}