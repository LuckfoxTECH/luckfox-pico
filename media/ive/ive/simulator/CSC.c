#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rk_mpi_ive.h"
#include "rve_tests.h"
#include "sample_assist.h"
#include "sample_define.h"
#include "sample_file.h"

RK_VOID CSCSample(IVE_IMAGE_TYPE_E enSrcType, IVE_CSC_MODE_E enMode,
                  IVE_IMAGE_TYPE_E enDstType, const RK_CHAR *moduleName,
                  RK_CHAR *fileName, RK_CHAR *outDir, RK_U32 u32Width,
                  RK_U32 u32Height) {
    IVE_SRC_IMAGE_S stSrc;
    IVE_DST_IMAGE_S stDst;
    IVE_CSC_CTRL_S stCtrl;
    RK_S32 s32Result;
    FILE *fp = NULL, *fo = NULL;
    IVE_HANDLE handle;
    RK_DOUBLE dTime;
    RK_CHAR outFile[RK_MAX_FILE_LEN];

    memset(&stSrc, 0, sizeof(IVE_SRC_IMAGE_S));
    memset(&stDst, 0, sizeof(IVE_DST_IMAGE_S));
    memset(&stCtrl, 0, sizeof(IVE_CSC_CTRL_S));

    s32Result = RK_CreateIveImage(&stSrc, enSrcType, u32Width, u32Height);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);
    s32Result = RK_CreateIveImage(&stDst, enDstType, u32Width, u32Height);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    s32Result = RK_ReadFile(fileName, &stSrc, &fp);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    stCtrl.enMode = enMode;

    dTime = (RK_DOUBLE)RK_GetTickCount();
    s32Result = RK_MPI_IVE_CSC(&handle, &stSrc, &stDst, &stCtrl, RK_TRUE);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);
    dTime = (RK_DOUBLE)RK_GetTickCount() - dTime;

    printf("CSC mode %d,time = %g ms\n", enMode,
           dTime / ((RK_DOUBLE)RK_GetTickFrequency() * 1000.));

    if (outDir == NULL) {
        RK_WriteFile(NULL, (void *)stDst.au64VirAddr[0],
                     stDst.u32Width * stDst.u32Height, &fo);
    } else {
        if (stDst.enType == IVE_IMAGE_TYPE_U8C3_PACKAGE ||
                stDst.enType == IVE_IMAGE_TYPE_U8C3_PLANAR) {
            RK_SPRINTF(outFile, RK_MAX_FILE_LEN, "%s/rve_%s_out.rgb", outDir, moduleName);
            RK_WriteFile(outFile, (void *)stDst.au64VirAddr[0],
                        stDst.u32Width * stDst.u32Height * 3, &fo);
#if BUILD_SIMULATOR
            gfx_show_img("csc", (void *)stDst.au64VirAddr[0], stDst.u32Width,
                         stDst.u32Height, IVE_IMAGE_TYPE_U8C3_PACKAGE, 0);
#endif
        } else if (stDst.enType == IVE_IMAGE_TYPE_YUV420SP) {
            RK_SPRINTF(outFile, RK_MAX_FILE_LEN, "%s/rve_%s_420sp_out.yuv", outDir, moduleName);
            RK_WriteFile(outFile, (void *)stDst.au64VirAddr[0],
                        stDst.u32Width * stDst.u32Height * 3 / 2, &fo);
#if BUILD_SIMULATOR
            gfx_show_img("csc", (void *)stDst.au64VirAddr[0], stDst.u32Width,
                         stDst.u32Height, IVE_IMAGE_TYPE_U8C1, 0);
#endif
        }
    }

FAILURE:
    RK_FCLOSE(fp);
    RK_FCLOSE(fo);
    RK_DestroyIveImage(&stSrc);
    RK_DestroyIveImage(&stDst);
}

RK_VOID CSCSampleVideoBT601YUV2RGB(const RK_CHAR *moduleName, RK_CHAR *fileName,
                                   RK_CHAR *outDir, RK_U32 u32Width,
                                   RK_U32 u32Height) {
    IVE_IMAGE_TYPE_E enSrcType = IVE_IMAGE_TYPE_YUV420SP;
    IVE_IMAGE_TYPE_E enDstType = IVE_IMAGE_TYPE_U8C3_PACKAGE;
    IVE_CSC_MODE_E enMode = IVE_CSC_MODE_FULL_BT601_YUV2RGB;

    CSCSample(enSrcType, enMode, enDstType, moduleName, fileName, outDir,
              u32Width, u32Height);
}

RK_VOID CSCSampleVideoBT601RGB2YUV(const RK_CHAR *moduleName, RK_CHAR *fileName,
                                   RK_CHAR *outDir, RK_U32 u32Width,
                                   RK_U32 u32Height) {
    IVE_IMAGE_TYPE_E enSrcType = IVE_IMAGE_TYPE_U8C3_PACKAGE;
    IVE_IMAGE_TYPE_E enDstType = IVE_IMAGE_TYPE_YUV420SP;
    IVE_CSC_MODE_E enMode = IVE_CSC_MODE_FULL_BT601_RGB2YUV;

    CSCSample(enSrcType, enMode, enDstType, moduleName, fileName, outDir,
              u32Width, u32Height);
}