#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rk_mpi_ive.h"
#include "rve_tests.h"
#include "sample_assist.h"
#include "sample_define.h"
#include "sample_file.h"

RK_VOID NCCSample(const RK_CHAR *moduleName, RK_CHAR *fileName1, RK_CHAR *fileName2,
                  RK_CHAR *outDir, RK_U32 u32Width, RK_U32 u32Height) {
    IVE_SRC_IMAGE_S stSrc1;
    IVE_SRC_IMAGE_S stSrc2;
    IVE_DST_MEM_INFO_S stDst;
    RK_S32 s32Result;
    FILE *fp1 = NULL;
    FILE *fp2 = NULL;
    IVE_HANDLE handle;
    RK_DOUBLE dTime;
    RK_CHAR outFile[RK_MAX_FILE_LEN];
    RK_U64 *numerator;
    RK_U64 *quadSum1;
    RK_U64 *quadSum2;
    RK_FLOAT fR;

    // memset(&stSrc1,0,sizeof(IVE_SRC_IMAGE_S));
    // memset(&stSrc2,0,sizeof(IVE_SRC_IMAGE_S));
    // memset(&stDst,0,sizeof(IVE_DST_MEM_INFO_S));

    s32Result =
        RK_CreateIveImage(&stSrc1, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);
    s32Result =
        RK_CreateIveImage(&stSrc2, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    stDst.u32Size = sizeof(IVE_NCC_DST_MEM_S);
    s32Result = RK_CreateIveMem(&stDst, stDst.u32Size);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);
    memset(stDst.u64VirAddr, 0, stDst.u32Size);

    s32Result = RK_ReadFile(fileName1, &stSrc1, &fp1);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);
    s32Result = RK_ReadFile(fileName2, &stSrc2, &fp2);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    dTime = (RK_DOUBLE)RK_GetTickCount();
    s32Result = RK_MPI_IVE_NCC(&handle, &stSrc1, &stSrc2, &stDst, RK_TRUE);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);
    dTime = (RK_DOUBLE)RK_GetTickCount() - dTime;

    numerator = (RK_U64 *)stDst.u64VirAddr;
    quadSum1 = (RK_U64 *)(stDst.u64VirAddr + sizeof(RK_U64));
    quadSum2 = quadSum1 + 1;

    fR = (RK_FLOAT)((RK_DOUBLE)*numerator /
                    (sqrt((RK_DOUBLE)*quadSum1) * sqrt((RK_DOUBLE)*quadSum2)));

    printf("NCC,r = %f,time = %g ms\n", fR,
           dTime / ((RK_DOUBLE)RK_GetTickFrequency() * 1000.));

FAILURE:

    RK_FCLOSE(fp1);
    RK_FCLOSE(fp2);
    RK_DestroyIveImage(&stSrc1);
    RK_DestroyIveImage(&stSrc2);
    RK_DestroyIveMem(&stDst);
}