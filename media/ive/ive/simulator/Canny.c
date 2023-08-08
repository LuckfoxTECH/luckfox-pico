#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rk_mpi_ive.h"
#include "rve_tests.h"
#include "sample_assist.h"
#include "sample_define.h"
#include "sample_file.h"

RK_VOID CannySample(RK_S8 *ps8Mask, const RK_CHAR *moduleName, RK_CHAR *fileName,
                    RK_CHAR *outDir, RK_U32 u32Width, RK_U32 u32Height) {
    IVE_SRC_IMAGE_S stSrc;
    IVE_DST_IMAGE_S stEdge;
    IVE_DST_MEM_INFO_S stStack;
    IVE_CANNY_EDGE_CTRL_S stCannyEdgeCtrl = {0};

    RK_S32 s32Result;
    FILE *fp = NULL, *fo = NULL;
    IVE_HANDLE handle;
    RK_DOUBLE dTime;
    RK_CHAR outFile[RK_MAX_FILE_LEN];
    
    memset(&stSrc,0,sizeof(IVE_SRC_IMAGE_S));
    memset(&stEdge,0,sizeof(IVE_DST_IMAGE_S));
    memset(&stStack,0,sizeof(IVE_DST_MEM_INFO_S));
    memset(&stCannyEdgeCtrl,0,sizeof(IVE_CANNY_EDGE_CTRL_S));

    s32Result =
        RK_CreateIveImage(&stSrc, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);
    s32Result =
        RK_CreateIveImage(&stEdge, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    stStack.u32Size =
        stSrc.u32Width * stSrc.u32Height * (sizeof(IVE_POINT_U16_S)) +
        sizeof(IVE_CANNY_STACK_SIZE_S);
    s32Result = RK_CreateIveMem(&stStack, stStack.u32Size);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    stCannyEdgeCtrl.stMem.u32Size = u32Width * u32Height;
    s32Result = RK_CreateIveMem(&stCannyEdgeCtrl.stMem,
                                stCannyEdgeCtrl.stMem.u32Size);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    stCannyEdgeCtrl.u16LowThr = 50;
    stCannyEdgeCtrl.u16HighThr = 150;
    memcpy(stCannyEdgeCtrl.as8Mask, ps8Mask, 25 * sizeof(RK_S8));

    s32Result = RK_ReadFile(fileName, &stSrc, &fp);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    dTime = (RK_DOUBLE)RK_GetTickCount();

    s32Result = RK_MPI_IVE_CannyEdge(&handle, &stSrc, &stEdge, NULL,//&stStack,
                                        &stCannyEdgeCtrl, RK_TRUE);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    dTime = (RK_DOUBLE)RK_GetTickCount() - dTime;

    printf("Canny time = %g ms\n",
           dTime / ((RK_DOUBLE)RK_GetTickFrequency() * 1000.));


    if (outDir == NULL) {
        RK_WriteFile(NULL, (void *)stEdge.au64VirAddr[0],
                     stEdge.u32Width * stEdge.u32Height, &fo);
    } else {
        RK_SPRINTF(outFile, RK_MAX_FILE_LEN, "%s/rve_%s_out.yuv", outDir, moduleName);
        RK_WriteFile(outFile, (void *)stEdge.au64VirAddr[0],
                     stEdge.u32Width * stEdge.u32Height, &fo);
    }

 #if BUILD_SIMULATOR
    gfx_show_img("canny", (void *)stEdge.au64VirAddr[0], stEdge.u32Width,
                 stEdge.u32Height, IVE_IMAGE_TYPE_U8C1, 0);
#endif
FAILURE:
    RK_FCLOSE(fp);
    RK_FCLOSE(fo);
    RK_DestroyIveImage(&stSrc);
    RK_DestroyIveImage(&stEdge);
    RK_DestroyIveMem(&stStack);
    RK_DestroyIveMem(&stCannyEdgeCtrl.stMem);
}

RK_VOID CannySample3x3(const RK_CHAR *moduleName, RK_CHAR *fileName, RK_CHAR *outDir,
                       RK_U32 u32Width, RK_U32 u32Height) {
    RK_S8 mask[25] = {0, 0, 0, 0,  0, 0, -1, 0, 1, 0, 0, -2, 0,
                      2, 0, 0, -1, 0, 1, 0,  0, 0, 0, 0, 0};

    CannySample(mask, moduleName, fileName, outDir, u32Width, u32Height);
}

RK_VOID CannySample5x5(const RK_CHAR *moduleName, RK_CHAR *fileName, RK_CHAR *outDir,
                       RK_U32 u32Width, RK_U32 u32Height) {
    RK_S8 mask[25] = {-1, -2, 0,  2,  1, -4, -8, 0,  8,  4, -6, -12, 0,
                      12, 6,  -4, -8, 0, 8,  4,  -1, -2, 0, 2,  1};

    CannySample(mask, moduleName, fileName, outDir, u32Width, u32Height);
}
