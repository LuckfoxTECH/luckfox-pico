#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rk_mpi_ive.h"
#include "rve_tests.h"
#include "sample_assist.h"
#include "sample_define.h"
#include "sample_file.h"

RK_VOID DMASample(IVE_DMA_MODE_E enMode, const RK_CHAR *moduleName, RK_CHAR *fileName,
                  RK_CHAR *outDir, RK_U32 u32Width, RK_U32 u32Height) {
    IVE_DATA_S stSrc1;
    IVE_SRC_IMAGE_S stSrc;
    IVE_DATA_S stDst1;
    IVE_SRC_IMAGE_S stDst;
    IVE_DMA_CTRL_S stCtrl = {0};
    RK_S32 s32Result;

    FILE *fp = NULL, *fo = NULL;
    IVE_HANDLE handle;
    RK_DOUBLE dTime;
    RK_CHAR outFile[RK_MAX_FILE_LEN];
    RK_BOOL bUse = RK_FALSE;

    memset(&stSrc1, 0, sizeof(IVE_DATA_S));
    memset(&stSrc, 0, sizeof(IVE_SRC_IMAGE_S));
    memset(&stDst1, 0, sizeof(IVE_DATA_S));
    memset(&stDst, 0, sizeof(IVE_SRC_IMAGE_S));
    memset(&stCtrl, 0, sizeof(IVE_DMA_CTRL_S));

    s32Result =
        RK_CreateIveImage(&stSrc, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    s32Result = RK_ReadFile(fileName, &stSrc, &fp);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    stSrc1.u64VirAddr = stSrc.au64VirAddr[0];
    stSrc1.u64PhyAddr = stSrc.au64PhyAddr[0];
    stSrc1.u32Stride = stSrc.au32Stride[0];
    stSrc1.u32Width = stSrc.u32Width;
    stSrc1.u32Height = stSrc.u32Height;

    stCtrl.enMode = enMode;

    switch (enMode) {
        case IVE_DMA_MODE_DIRECT_COPY: {
            s32Result = RK_CreateIveImage(&stDst, IVE_IMAGE_TYPE_U8C1, u32Width,
                                          u32Height);
            RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

            stDst1.u64VirAddr = stDst.au64VirAddr[0];
            stDst1.u64PhyAddr = stDst.au64PhyAddr[0];
            stDst1.u32Stride = stDst.au32Stride[0];
            stDst1.u32Width = stDst.u32Width;
            stDst1.u32Height = stDst.u32Height;

            bUse = RK_TRUE;
        } break;
        case IVE_DMA_MODE_INTERVAL_COPY: {
            RK_U16 dstWidth;
            RK_U16 dstHeight;
            RK_U16 dstStride;

            stCtrl.u8ElemSize = 1;
            stCtrl.u8HorSegSize = 4;
            stCtrl.u8VerSegRows = 1;

            dstWidth =
                (stSrc1.u32Width / stCtrl.u8HorSegSize) * stCtrl.u8ElemSize;
            dstHeight = stSrc1.u32Height / stCtrl.u8VerSegRows;

            s32Result = RK_CreateIveImage(&stDst, IVE_IMAGE_TYPE_U8C1, dstWidth,
                                          dstHeight);
            RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);
            stDst.au32Stride[0] = dstWidth;

            stDst1.u64VirAddr = stDst.au64VirAddr[0];
            stDst1.u64PhyAddr = stDst.au64PhyAddr[0];
            stDst1.u32Stride = stDst.au32Stride[0];
            stDst1.u32Width = stDst.u32Width;
            stDst1.u32Height = stDst.u32Height;

            bUse = RK_TRUE;
        } break;
        case IVE_DMA_MODE_SET_3BYTE:
        case IVE_DMA_MODE_SET_8BYTE: {
            stCtrl.u64Val = 1;
        } break;
        default:
            break;
    }

    dTime = (RK_DOUBLE)RK_GetTickCount();
    s32Result =
        RK_MPI_IVE_DMA(&handle, &stSrc1, RK_TRUE == bUse ? &stDst1 : RK_NULL,
                       &stCtrl, RK_TRUE);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);
    dTime = (RK_DOUBLE)RK_GetTickCount() - dTime;

    printf("DMA mode %d,time = %g ms\n", enMode,
           dTime / ((RK_DOUBLE)RK_GetTickFrequency() * 1000.));

    RK_SPRINTF(outFile, RK_MAX_FILE_LEN, "%s/%02d_rve_%s_out.yuv", outDir, enMode,
            moduleName);

    IVE_DATA_S outData =
        (RK_TRUE == bUse) ? stDst1 : stSrc1;
    void *outPtr = (void *)(outData.u64VirAddr);
    RK_U32 outSize = outData.u32Width * outData.u32Height;

    if (outDir == NULL) {
        RK_WriteFile(NULL, outPtr, outSize, &fo);
    } else {
        RK_WriteFile(outFile, outPtr, outSize, &fo);
    }

 #if BUILD_SIMULATOR
    gfx_show_img("dma", outPtr, outData.u32Width,
                 outData.u32Height, IVE_IMAGE_TYPE_U8C1, 0);
#endif
FAILURE:
    RK_FCLOSE(fp);
    RK_FCLOSE(fo);
    if (RK_TRUE == bUse) {
        RK_DestroyIveImage(&stDst);
    }
    RK_DestroyIveImage(&stSrc);
}

RK_VOID DMASampleDirectCopy(const RK_CHAR *moduleName, RK_CHAR *fileName,
                            RK_CHAR *outDir, RK_U32 u32Width,
                            RK_U32 u32Height) {
    IVE_DMA_MODE_E enMode = IVE_DMA_MODE_DIRECT_COPY;

    DMASample(enMode, moduleName, fileName, outDir, u32Width, u32Height);
}
RK_VOID DMASampleIntervalCopy(const RK_CHAR *moduleName, RK_CHAR *fileName,
                              RK_CHAR *outDir, RK_U32 u32Width,
                              RK_U32 u32Height) {
    IVE_DMA_MODE_E enMode = IVE_DMA_MODE_INTERVAL_COPY;

    DMASample(enMode, moduleName, fileName, outDir, u32Width, u32Height);
}
RK_VOID DMASampleSet3Byte(const RK_CHAR *moduleName, RK_CHAR *fileName,
                          RK_CHAR *outDir, RK_U32 u32Width, RK_U32 u32Height) {
    IVE_DMA_MODE_E enMode = IVE_DMA_MODE_SET_3BYTE;

    DMASample(enMode, moduleName, fileName, outDir, u32Width, u32Height);
}
RK_VOID DMASampleSet8Byte(const RK_CHAR *moduleName, RK_CHAR *fileName,
                          RK_CHAR *outDir, RK_U32 u32Width, RK_U32 u32Height) {
    IVE_DMA_MODE_E enMode = IVE_DMA_MODE_SET_8BYTE;

    DMASample(enMode, moduleName, fileName, outDir, u32Width, u32Height);
}
