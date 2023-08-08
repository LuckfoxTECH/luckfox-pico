#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rk_mpi_ive.h"
#include "rve_tests.h"
#include "sample_assist.h"
#include "sample_define.h"
#include "sample_file.h"

RK_VOID CCL_Sample01(const RK_CHAR *moduleName, RK_CHAR *fileName, RK_CHAR *outDir,
                     RK_U32 u32Width, RK_U32 u32Height) {
    int i, k;
    IVE_HANDLE hIveHandle;
    RK_BOOL bInstant = RK_TRUE;
    RK_S32 s32Result;
    RK_DOUBLE dTime;

    IVE_IMAGE_S stIveImg;
    IVE_CCL_CTRL_S stCCLCtrl;
    IVE_DST_MEM_INFO_S stDst;
    IVE_CCBLOB_S *pstCCBlob = RK_NULL;
    RK_U8 *showImg = RK_NULL;
    RK_CHAR outFile[RK_MAX_FILE_LEN];
    FILE *fp = NULL, *fo = NULL;

    stDst.u32Size = sizeof(IVE_CCBLOB_S);
    s32Result = RK_CreateIveMem(&stDst, stDst.u32Size);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    stCCLCtrl.stMem.u32Size = u32Width * u32Height;
    s32Result = RK_CreateIveMem(&stCCLCtrl.stMem, stCCLCtrl.stMem.u32Size);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    pstCCBlob = (IVE_CCBLOB_S *)stDst.u64VirAddr;

    {
        s32Result = RK_CreateIveImage(&stIveImg, IVE_IMAGE_TYPE_U8C1, u32Width,
                                      u32Height);
        RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

        s32Result = RK_ReadFile(fileName, &stIveImg, &fp);
        RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

        stCCLCtrl.enMode = IVE_CCL_MODE_8C;
        stCCLCtrl.u16InitAreaThr = 4;
        stCCLCtrl.u16Step = 2;

        dTime = (RK_DOUBLE)RK_GetTickCount();
        s32Result = RK_MPI_IVE_CCL(&hIveHandle, &stIveImg, &stDst, &stCCLCtrl,
                                   bInstant);
        RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);
        dTime = (RK_DOUBLE)RK_GetTickCount() - dTime;

        printf("CCL,time = %g ms\n",
               dTime / ((RK_DOUBLE)RK_GetTickFrequency() * 1000.));

        showImg = (RK_U8 *)malloc(u32Width * u32Height);
        RK_Memcpy(showImg, (void *)stIveImg.au64VirAddr[0],
                  u32Width * u32Height);

        for (k = 0; k < IVE_MAX_REGION_NUM; k++) {
            int left, right, top, bottom, i, j;
            if (pstCCBlob->astRegion[k].u32Area == 0) continue;
            left = pstCCBlob->astRegion[k].u16Left;
            right = pstCCBlob->astRegion[k].u16Right;
            top = pstCCBlob->astRegion[k].u16Top;
            bottom = pstCCBlob->astRegion[k].u16Bottom;
            for (j = top; j <= bottom; j++) {
                for (i = left; i <= right; i++) {
                    if (i == left || i == right || j == top || j == bottom) {
                        showImg[j * u32Width + i] = 255;
                    }
                }
            }
        }

        if (outDir == NULL) {
            RK_WriteFile(NULL, showImg, u32Width * u32Height, &fo);
        } else {
            RK_SPRINTF(outFile, RK_MAX_FILE_LEN, "%s/rve_%s_out.yuv", outDir, moduleName);
            RK_WriteFile(outFile, showImg, u32Width * u32Height, &fo);
        }

 #if BUILD_SIMULATOR
        gfx_show_img("ccl", showImg, u32Width, u32Height, IVE_IMAGE_TYPE_U8C1, 0);
#endif
    FAILURE:
        RK_FCLOSE(fp);
        RK_FCLOSE(fo);
        RK_DestroyIveImage(&stIveImg);
        RK_DestroyIveMem(&stDst);
        RK_DestroyIveMem(&stCCLCtrl.stMem);
        free(showImg);
    }
}