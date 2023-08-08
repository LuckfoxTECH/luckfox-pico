#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#include "rk_mpi_ive.h"
#include "rve_tests.h"
#include "sample_assist.h"
#include "sample_define.h"
#include "sample_file.h"

RK_VOID ShitomasiSample(const RK_CHAR *moduleName, RK_CHAR *fileName, RK_CHAR *outDir,
                        RK_U32 u32Width, RK_U32 u32Height) {
    int i;
    IVE_IMAGE_S stSrcImg;
    IVE_MEM_INFO_S stCorner, stCandiCorner;
    IVE_ST_CANDI_CORNER_CTRL_S stStCandiCornerCtrl;
    IVE_ST_CORNER_CTRL_S stStCornerCtrl;
    IVE_ST_CORNER_INFO_S *pstCornerInfo;
    RK_U32 *pstCandiCornerInfo;

    IVE_HANDLE hIveHandle;
    RK_BOOL bInstant = RK_TRUE;
    RK_S32 s32Result;
    RK_DOUBLE dTime;

    FILE *fp = NULL, *fo = NULL;
    RK_CHAR outFile[RK_MAX_FILE_LEN];

    // memset(&pSrcImage,0,sizeof(IVE_SRC_IMAGE_S));
    s32Result =
        RK_CreateIveImage(&stSrcImg, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    // read file from res
    s32Result = RK_ReadFile(fileName, &stSrcImg, &fp);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    // step1: calc candidate corners
    stCandiCorner.u32Size = u32Width * u32Height * sizeof(RK_U16) +
                            sizeof(IVE_ST_CANDI_STACK_SIZE_S);
    s32Result = RK_CreateIveMem(&stCandiCorner, stCandiCorner.u32Size);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    stStCandiCornerCtrl.u0q8QualityLevel = 25;
    dTime = (RK_DOUBLE)RK_GetTickCount();
    s32Result = RK_MPI_IVE_STCandiCorner(&hIveHandle, &stSrcImg, &stCandiCorner,
                                         &stStCandiCornerCtrl, bInstant);
    dTime = (RK_DOUBLE)RK_GetTickCount() - dTime;

    printf("STCandiCorner,time = %g ms\n",
           dTime / ((RK_DOUBLE)RK_GetTickFrequency() * 1000.));
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    pstCandiCornerInfo = (RK_U32 *)stCandiCorner.u64VirAddr;

    // step2: filter candiate corners
    stStCornerCtrl.u16MaxCornerNum = 200;
    stStCornerCtrl.u16MinDist = 10;

    stStCornerCtrl.stMem.u32Size = stSrcImg.u32Height * stSrcImg.au32Stride[0] +
                                   sizeof(IVE_ST_CORNER_MEM_S) * 2;
    s32Result =
        RK_CreateIveMem(&stStCornerCtrl.stMem, stStCornerCtrl.stMem.u32Size);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    stCorner.u32Size = sizeof(IVE_ST_CORNER_INFO_S);
    s32Result = RK_CreateIveMem(&stCorner, stCorner.u32Size);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    dTime = (RK_DOUBLE)RK_GetTickCount();
    s32Result = RK_MPI_IVE_STCorner(&hIveHandle, &stSrcImg, &stCandiCorner,
                                    &stCorner, &stStCornerCtrl, bInstant);
    dTime = (RK_DOUBLE)RK_GetTickCount() - dTime;
    printf("STCorner,time = %g ms\n",
           dTime / ((RK_DOUBLE)RK_GetTickFrequency() * 1000.));
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    pstCornerInfo = (IVE_ST_CORNER_INFO_S *)stCorner.u64VirAddr;

    // draw corners on input image
    unsigned char *p_pixel_in = (unsigned char *)stSrcImg.au64VirAddr[0];
    for (i = 0; i < pstCornerInfo->u16CornerNum; i++) {
        int postion = (int)(pstCornerInfo->astCorner[i].u16Y * u32Width +
                            pstCornerInfo->astCorner[i].u16X);
        p_pixel_in[postion] = 255;
        if (postion % u32Width != 0 && postion % u32Width != u32Width - 1) {
            p_pixel_in[postion - 1] = 255;
            p_pixel_in[postion + 1] = 255;
        }
        if (postion > u32Width - 1 && postion < u32Width * (u32Height - 1)) {
            p_pixel_in[postion - u32Width] = 255;
            p_pixel_in[postion + u32Width] = 255;
        }
    }

    // write marked image to disk
    if (outDir == NULL) {
        RK_WriteFile(NULL, p_pixel_in, u32Width * u32Height, &fo);
    } else {
        RK_SPRINTF(outFile, RK_MAX_FILE_LEN, "%s/rve_%s_out.yuv", outDir, moduleName);
        RK_WriteFile(outFile, p_pixel_in, u32Width * u32Height, &fo);
    }

 #if BUILD_SIMULATOR
    gfx_show_img("stcorner", p_pixel_in, u32Width, u32Height,
                 IVE_IMAGE_TYPE_U8C1, 0);
#endif

FAILURE:
    RK_FCLOSE(fp);
    RK_FCLOSE(fo);
    RK_DestroyIveImage(&stSrcImg);
    RK_DestroyIveMem(&stCandiCorner);
    RK_DestroyIveMem(&stStCornerCtrl.stMem);
    RK_DestroyIveMem(&stCorner);
}