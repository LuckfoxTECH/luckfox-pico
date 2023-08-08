#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "sample_comm_rve.h"

typedef struct rkTEST_HIST_S {
    IVE_SRC_IMAGE_S stSrc;
    IVE_DST_MEM_INFO_S stHist;
} TEST_HIST_S;

static TEST_HIST_S s_stTestMem;
/******************************************************************************
 * function : test memory uninit
 ******************************************************************************/
static void SAMPLE_IVE_TestHist_Uninit(TEST_HIST_S* pstTestMem) {
    IVE_MMZ_FREE(pstTestMem->stSrc.au64PhyAddr[0],
                 pstTestMem->stSrc.au64VirAddr[0]);
    IVE_MMZ_FREE(pstTestMem->stHist.u64PhyAddr, pstTestMem->stHist.u64VirAddr);
}
/******************************************************************************
 * function : test memory init
 ******************************************************************************/
static RK_S32 SAMPLE_IVE_TestHist_Init(TEST_HIST_S* pstTestMem,
                                       RK_U32 u32Width,
                                       RK_U32 u32Height) {
    RK_S32 s32Ret = RK_FAILURE;
    RK_U32 u32Size;

    memset(pstTestMem, 0, sizeof(TEST_HIST_S));

    s32Ret = SAMPLE_COMM_IVE_CreateImage(
        &(pstTestMem->stSrc), IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(RK_SUCCESS != s32Ret, TEST_HIST_INIT_FAIL,
                           "Error(%#x),Create src1 image failed!\n", s32Ret);

    printf("stSrc, vir: %llx, phy: %llx\n", pstTestMem->stSrc.au64VirAddr[0], pstTestMem->stSrc.au64PhyAddr[0]);
    memset((void*)(pstTestMem->stSrc.au64VirAddr[0]), 0, u32Width * u32Height);

    u32Size = IVE_HIST_NUM * sizeof(RK_U32);
    s32Ret = SAMPLE_COMM_IVE_CreateMemInfo(&(pstTestMem->stHist), u32Size);
    SAMPLE_CHECK_EXPR_GOTO(RK_SUCCESS != s32Ret, TEST_HIST_INIT_FAIL,
                           "Error(%#x),Create hist mem info failed!\n", s32Ret);

    s32Ret = RK_SUCCESS;

TEST_HIST_INIT_FAIL:

    if (RK_SUCCESS != s32Ret) {
        SAMPLE_IVE_TestHist_Uninit(pstTestMem);
    }
    return s32Ret;
}
/******************************************************************************
 * function : test memory
 ******************************************************************************/
static RK_S32 SAMPLE_IVE_TestHistProc(TEST_HIST_S* pstTestMem) {
    RK_S32 s32Ret;
    RK_U32* pu32Hist;
    RK_U32 i;
    IVE_HANDLE IveHandle;
    bool bInstant = true;
    bool bBlock = true;
    bool bFinish = false;

    bInstant = true;
    s32Ret = RK_MPI_IVE_Hist(&IveHandle, &pstTestMem->stSrc,
                             &pstTestMem->stHist, bInstant);

    SAMPLE_CHECK_EXPR_RET(RK_SUCCESS != s32Ret, s32Ret,
                          "Error(%#x),IVE_Hist failed!\n", s32Ret);

    s32Ret = RK_MPI_IVE_Query(IveHandle, &bFinish, bBlock);
    while (ERR_IVE_QUERY_TIMEOUT == s32Ret) {
        usleep(100);
        s32Ret = RK_MPI_IVE_Query(IveHandle, &bFinish, bBlock);
    }
    SAMPLE_CHECK_EXPR_RET(RK_SUCCESS != s32Ret, s32Ret,
                          "Error(%#x),IVE_Query failed!\n", s32Ret);

    pu32Hist = SAMPLE_COMM_IVE_CONVERT_64BIT_ADDR(
        RK_U32, pstTestMem->stHist.u64VirAddr);

    if (pu32Hist[0] !=
        pstTestMem->stSrc.u32Width * pstTestMem->stSrc.u32Height) {
        s32Ret = RK_FAILURE;
        SAMPLE_PRT("Test hist error,pu32Hist[0] = %d\n", pu32Hist[0]);
        for (i = 1; i < IVE_HIST_NUM; i++) {
            if (pu32Hist[i] != 0) {
                SAMPLE_PRT("Test mem error, pu32Hist[%d] = %d\n", i,
                           pu32Hist[i]);
            }
        }
    } else {
        SAMPLE_PRT("Test HIST, pu32Hist[0] = %d\n", pu32Hist[0]);
    }

    return s32Ret;
}
/******************************************************************************
 * function : Show test memory sample
 ******************************************************************************/
void SAMPLE_IVE_TestHist(void) {

    RK_S32 s32Ret;
    RK_U32 u32Width = 16;
    RK_U32 u32Height = 16;

    memset(&s_stTestMem, 0, sizeof(s_stTestMem));
    SAMPLE_COMM_IVE_CheckIveMpiInit();
    s32Ret = SAMPLE_IVE_TestHist_Init(&s_stTestMem, u32Width, u32Height);
    SAMPLE_CHECK_EXPR_GOTO(RK_SUCCESS != s32Ret, TEST_HIST_FAIL,
                           "Error(%#x),SAMPLE_IVE_TestHist_Init failed!\n",
                           s32Ret);

    s32Ret = SAMPLE_IVE_TestHistProc(&s_stTestMem);
    if (RK_SUCCESS == s32Ret) {
        SAMPLE_PRT("Process success!\n");
    }

    SAMPLE_IVE_TestHist_Uninit(&s_stTestMem);
    memset(&s_stTestMem, 0, sizeof(s_stTestMem));

TEST_HIST_FAIL:
    SAMPLE_COMM_IVE_IveMpiExit();
    return s32Ret;
}

void SAMPLE_IVE_TestHist_HandleSig(void)
{
    SAMPLE_COMM_IVE_IveMpiExit();
}
