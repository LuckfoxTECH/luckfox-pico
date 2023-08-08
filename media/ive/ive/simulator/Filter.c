#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rk_mpi_ive.h"
#include "rve_tests.h"
#include "sample_assist.h"
#include "sample_define.h"
#include "sample_file.h"

RK_VOID FilterSample(IVE_IMAGE_TYPE_E enType, RK_S8 *ps8Mask, RK_U8 u8Norm,
                     const RK_CHAR *moduleName, RK_CHAR *fileName, RK_CHAR *outDir,
                     RK_U32 u32Width, RK_U32 u32Height) {
    IVE_SRC_IMAGE_S stSrc;
    IVE_DST_IMAGE_S stDst;
    IVE_FILTER_CTRL_S stCtrl;
    IVE_DATA_S dmaSrc;
    IVE_DST_DATA_S dmaDst;
    IVE_DMA_CTRL_S dmaCtrl;
    RK_S32 s32Result;
    FILE *fp = NULL, *fo = NULL;
    IVE_HANDLE handle;
    RK_DOUBLE dTime;
    RK_CHAR outFile[RK_MAX_FILE_LEN];

    memset(&stSrc, 0, sizeof(IVE_SRC_IMAGE_S));
    memset(&stDst, 0, sizeof(IVE_DST_IMAGE_S));
    memset(&stCtrl, 0, sizeof(IVE_FILTER_CTRL_S));

    s32Result = RK_CreateIveImage(&stSrc, enType, u32Width, u32Height);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);
    s32Result = RK_CreateIveImage(&stDst, enType, u32Width, u32Height);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    s32Result = RK_ReadFile(fileName, &stSrc, &fp);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    stCtrl.u8OutMode = 0;
    memcpy(stCtrl.as8Mask, ps8Mask, sizeof(RK_S8) * 25);
    stCtrl.u8Norm = u8Norm;

    dTime = (RK_DOUBLE)RK_GetTickCount();

    s32Result = RK_MPI_IVE_Filter(&handle, &stSrc, &stDst, &stCtrl, RK_TRUE);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    dTime = (RK_DOUBLE)RK_GetTickCount() - dTime;

    printf("test: rve process image %d,time = %g ms\n", enType,
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
    gfx_show_img("filter", (void *)stDst.au64VirAddr[0], stDst.u32Width,
                 stDst.u32Height, IVE_IMAGE_TYPE_U8C1, 0);
#endif
FAILURE:
    RK_FCLOSE(fp);
    RK_FCLOSE(fo);
    RK_DestroyIveImage(&stSrc);
    RK_DestroyIveImage(&stDst);
}

RK_VOID FilterSampleY5x5(const RK_CHAR *moduleName, RK_CHAR *fileName,
                         RK_CHAR *outDir, RK_U32 u32Width, RK_U32 u32Height) {
    RK_S8 mask[25] = {1, 2, 3, 2, 1, 2, 5, 6, 5, 2, 3, 6, 8,
                      6, 3, 2, 5, 6, 5, 2, 1, 2, 3, 2, 1};

    FilterSample(IVE_IMAGE_TYPE_U8C1, mask, 7, moduleName, fileName, outDir,
                 u32Width, u32Height);
}
