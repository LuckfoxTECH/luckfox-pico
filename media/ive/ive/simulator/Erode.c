#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rk_mpi_ive.h"
#include "rve_tests.h"
#include "sample_assist.h"
#include "sample_define.h"
#include "sample_file.h"

RK_VOID ErodeSample(RK_U8 *pu8Mask, const RK_CHAR *moduleName, RK_CHAR *fileName,
                    RK_CHAR *outDir, RK_U32 u32Width, RK_U32 u32Height) {
    IVE_SRC_IMAGE_S stSrc;
    IVE_DST_IMAGE_S stDst;
    IVE_ERODE_CTRL_S stCtrlErode;

    RK_S32 s32Result;
    FILE *fp = NULL, *fo = NULL;
    IVE_HANDLE handle;
    RK_DOUBLE dTime;
    RK_CHAR outFile[RK_MAX_FILE_LEN];

    memset(&stSrc, 0, sizeof(IVE_SRC_IMAGE_S));
    memset(&stDst, 0, sizeof(IVE_DST_IMAGE_S));
    memset(&stCtrlErode, 0, sizeof(IVE_ERODE_CTRL_S));

    s32Result =
        RK_CreateIveImage(&stSrc, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);
    s32Result =
        RK_CreateIveImage(&stDst, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    s32Result = RK_ReadFile(fileName, &stSrc, &fp);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    memcpy(stCtrlErode.au8Mask, pu8Mask, sizeof(RK_U8) * 25);

    dTime = (RK_DOUBLE)RK_GetTickCount();
    s32Result =
        RK_MPI_IVE_Erode(&handle, &stSrc, &stDst, &stCtrlErode, RK_TRUE);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);
    dTime = (RK_DOUBLE)RK_GetTickCount() - dTime;

    printf("Erode time = %g ms\n",
           dTime / ((RK_DOUBLE)RK_GetTickFrequency() * 1000.));

    if (outDir == NULL) {
        RK_WriteFile(NULL, (void *)stDst.au64VirAddr[0],
                     stDst.u32Width * stDst.u32Height, &fo);
    } else {
        RK_SPRINTF(outFile, RK_MAX_FILE_LEN, "%s/rve_%s_out.yuv", outDir, moduleName);
        RK_WriteFile(outFile, (void *)stDst.au64VirAddr[0],
                     stDst.u32Width * stDst.u32Height, &fo);
    }

#if BUILD_SIMULATOR
    gfx_show_img("erode", (void *)stDst.au64VirAddr[0], stDst.u32Width,
                 stDst.u32Height, IVE_IMAGE_TYPE_U8C1, 0);
#endif
FAILURE:
    RK_FCLOSE(fp);
    RK_FCLOSE(fo);
}

RK_VOID ErodeSample3x3(const RK_CHAR *moduleName, RK_CHAR *fileName, RK_CHAR *outDir,
                       RK_U32 u32Width, RK_U32 u32Height) {
    RK_U8 mask[25] = {0,   0, 0, 0, 0,   0, 0, 255, 0, 0, 0, 255, 255,
                      255, 0, 0, 0, 255, 0, 0, 0,   0, 0, 0, 0};
    ErodeSample(mask, moduleName, fileName, outDir, u32Width, u32Height);
}
RK_VOID ErodeSample5x5(const RK_CHAR *moduleName, RK_CHAR *fileName, RK_CHAR *outDir,
                       RK_U32 u32Width, RK_U32 u32Height) {
    RK_U8 mask[25] = {0,   0,   255, 0, 0,   0, 0, 255, 0, 0,   255, 255, 255,
                      255, 255, 0,   0, 255, 0, 0, 0,   0, 255, 0,   0};

    ErodeSample(mask, moduleName, fileName, outDir, u32Width, u32Height);
}
