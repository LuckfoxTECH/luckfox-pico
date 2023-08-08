#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rk_mpi_ive.h"
#include "rve_tests.h"
#include "sample_assist.h"
#include "sample_define.h"
#include "sample_file.h"

RK_VOID Integ(IVE_INTEG_OUT_CTRL_E enOutCtrl, const RK_CHAR *moduleName,
              RK_CHAR *fileName, RK_CHAR *outDir, RK_U32 u32Width,
              RK_U32 u32Height) {
    IVE_SRC_IMAGE_S stSrc;
    IVE_DST_IMAGE_S stDst;
    IVE_INTEG_CTRL_S stCtrl;
    RK_S32 s32Result;
    RK_U32 u32Size;
    FILE *fp = NULL, *fo = NULL;
    IVE_HANDLE handle;
    RK_DOUBLE dTime;
    RK_CHAR info[RK_INFO_LEN];
    memset(&stSrc, 0, sizeof(IVE_SRC_IMAGE_S));
    memset(&stDst, 0, sizeof(IVE_DST_IMAGE_S));
    memset(&stCtrl, 0, sizeof(IVE_INTEG_CTRL_S));
    RK_CHAR outFile[RK_MAX_FILE_LEN];

    stCtrl.stMem.u32Size = u32Width * u32Height * 6;
    s32Result = RK_CreateIveMem(&stCtrl.stMem, stCtrl.stMem.u32Size);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    s32Result =
        RK_CreateIveImage(&stSrc, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    switch (enOutCtrl) {
        case IVE_INTEG_OUT_CTRL_COMBINE:
            RK_STRCPY(info, RK_INFO_LEN, "Integral combine time =");
            s32Result = RK_CreateIveImage(&stDst, IVE_IMAGE_TYPE_U64C1,
                                          u32Width, u32Height);
            u32Size = u32Width * u32Height * 8;
            break;
        case IVE_INTEG_OUT_CTRL_SUM:
            RK_STRCPY(info, RK_INFO_LEN, "Integral sum time =");
            s32Result = RK_CreateIveImage(&stDst, IVE_IMAGE_TYPE_U32C1,
                                          u32Width, u32Height);
            u32Size = u32Width * u32Height * 4;
            break;
        case IVE_INTEG_OUT_CTRL_SQSUM:
            RK_STRCPY(info, RK_INFO_LEN, "Integral sqsum time =");
            s32Result = RK_CreateIveImage(&stDst, IVE_IMAGE_TYPE_U64C1,
                                          u32Width, u32Height);
            u32Size = u32Width * u32Height * 8;
            break;
        default:
            s32Result = RK_FAILURE;
            break;
    }

    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);
    stCtrl.enOutCtrl = enOutCtrl;

    s32Result = RK_ReadFile(fileName, &stSrc, &fp);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    dTime = (RK_DOUBLE)RK_GetTickCount();
    s32Result = RK_MPI_IVE_Integ(&handle, &stSrc, &stDst, &stCtrl, RK_TRUE);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);
    dTime = (RK_DOUBLE)RK_GetTickCount() - dTime;

    printf("%s %g ms\n", info,
           dTime / ((RK_DOUBLE)RK_GetTickFrequency() * 1000.));

    if (outDir == NULL) {
        RK_WriteFile(NULL, (void *)stDst.au64VirAddr[0], u32Size, &fo);
    } else {
        RK_SPRINTF(outFile, RK_MAX_FILE_LEN, "%s/%02d_rve_%s_out.yuv", outDir, enOutCtrl,
                moduleName);
        RK_WriteFile(outFile, (void *)stDst.au64VirAddr[0], u32Size, &fo);
    }

FAILURE:
    RK_FCLOSE(fp);
    RK_FCLOSE(fo);
    RK_DestroyIveImage(&stSrc);
    RK_DestroyIveImage(&stDst);
    RK_DestroyIveMem(&stCtrl.stMem);
}

RK_VOID IntegSampleCombine(const RK_CHAR *moduleName, RK_CHAR *fileName,
                           RK_CHAR *outDir, RK_U32 u32Width, RK_U32 u32Height) {
    Integ(IVE_INTEG_OUT_CTRL_COMBINE, moduleName, fileName, outDir, u32Width,
          u32Height);
}
RK_VOID IntegSampleSum(const RK_CHAR *moduleName, RK_CHAR *fileName, RK_CHAR *outDir,
                       RK_U32 u32Width, RK_U32 u32Height) {
    Integ(IVE_INTEG_OUT_CTRL_SUM, moduleName, fileName, outDir, u32Width,
          u32Height);
}
RK_VOID IntegSampleSqsum(const RK_CHAR *moduleName, RK_CHAR *fileName,
                         RK_CHAR *outDir, RK_U32 u32Width, RK_U32 u32Height) {
    Integ(IVE_INTEG_OUT_CTRL_SQSUM, moduleName, fileName, outDir, u32Width,
          u32Height);
}