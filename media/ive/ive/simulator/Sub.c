#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rk_mpi_ive.h"
#include "rve_tests.h"
#include "sample_assist.h"
#include "sample_define.h"
#include "sample_file.h"

RK_VOID SubSample(IVE_SUB_MODE_E enMode, const RK_CHAR *moduleName,
                  RK_CHAR *fileName1, RK_CHAR *fileName2, RK_CHAR *outDir,
                  RK_U32 u32Width, RK_U32 u32Height) {
    IVE_SRC_IMAGE_S stSrc1;
    IVE_SRC_IMAGE_S stSrc2;
    IVE_DST_IMAGE_S stDst;
    IVE_SUB_CTRL_S stCtrl;
    RK_S32 s32Result;
    FILE *fp1 = NULL;
    FILE *fp2 = NULL;
    FILE *fo = NULL;
    IVE_HANDLE handle;
    RK_DOUBLE dTime;
    RK_CHAR outFile[RK_MAX_FILE_LEN];

    memset(&stSrc1, 0, sizeof(IVE_SRC_IMAGE_S));
    memset(&stSrc2, 0, sizeof(IVE_SRC_IMAGE_S));
    memset(&stDst, 0, sizeof(IVE_DST_IMAGE_S));
    memset(&stCtrl, 0, sizeof(IVE_SUB_CTRL_S));

    s32Result =
        RK_CreateIveImage(&stSrc1, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);
    s32Result =
        RK_CreateIveImage(&stSrc2, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    if (IVE_SUB_MODE_SHIFT == enMode) {
        s32Result =
            RK_CreateIveImage(&stDst, IVE_IMAGE_TYPE_S8C1, u32Width, u32Height);
        RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);
    } else {
        s32Result =
            RK_CreateIveImage(&stDst, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
        RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);
    }

    s32Result = RK_ReadFile(fileName1, &stSrc1, &fp1);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);
    s32Result = RK_ReadFile(fileName2, &stSrc2, &fp2);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    stCtrl.enMode = enMode;

    dTime = (RK_DOUBLE)RK_GetTickCount();
    s32Result =
        RK_MPI_IVE_Sub(&handle, &stSrc1, &stSrc2, &stDst, &stCtrl, RK_TRUE);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);
    dTime = (RK_DOUBLE)RK_GetTickCount() - dTime;

    printf("Sub,mode %d,time = %g ms\n", enMode,
           dTime / ((RK_DOUBLE)RK_GetTickFrequency() * 1000.));

    if (outDir == NULL) {
        RK_WriteFile(NULL, (void *)stDst.au64VirAddr[0],
                     stDst.u32Width * stDst.u32Height, &fo);
    } else {
        RK_SPRINTF(outFile, RK_MAX_FILE_LEN, "%s/%02d_rve_%s_sub_out.yuv", outDir, enMode, moduleName);
        RK_WriteFile(outFile, (void *)stDst.au64VirAddr[0],
                     stDst.u32Width * stDst.u32Height, &fo);
    }
#if BUILD_SIMULATOR
    gfx_show_img("sub", (void *)stDst.au64VirAddr[0], stDst.u32Width,
                 stDst.u32Height, IVE_IMAGE_TYPE_U8C1, 0);
#endif
FAILURE:

    RK_FCLOSE(fp1);
    RK_FCLOSE(fp2);
    RK_FCLOSE(fo);
    RK_DestroyIveImage(&stSrc1);
    RK_DestroyIveImage(&stSrc2);
    RK_DestroyIveImage(&stDst);
}

RK_VOID SubSampleABS(const RK_CHAR *moduleName, RK_CHAR *fileName1,
                     RK_CHAR *fileName2, RK_CHAR *outDir, RK_U32 u32Width,
                     RK_U32 u32Height) {
    IVE_SUB_MODE_E enMode = IVE_SUB_MODE_ABS;

    SubSample(enMode, moduleName, fileName1, fileName2, outDir, u32Width,
              u32Height);
}
RK_VOID SubSampleSHIFT(const RK_CHAR *moduleName, RK_CHAR *fileName1,
                       RK_CHAR *fileName2, RK_CHAR *outDir, RK_U32 u32Width,
                       RK_U32 u32Height) {
    IVE_SUB_MODE_E enMode = IVE_SUB_MODE_SHIFT;

    SubSample(enMode, moduleName, fileName1, fileName2, outDir, u32Width,
              u32Height);
}