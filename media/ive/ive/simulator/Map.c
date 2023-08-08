#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rk_mpi_ive.h"
#include "rve_tests.h"
#include "sample_assist.h"
#include "sample_define.h"
#include "sample_file.h"

RK_VOID GetMap(RK_U32 *pu32Hist, RK_U8 *pu8Table, RK_U16 u16Width,
               RK_U16 u16Height) {
    RK_S32 s32Sum;
    RK_FLOAT fScale;
    RK_U32 u32Total = u16Width * u16Height;
    RK_S32 s32Index = 0;
    RK_S32 s32Tmp;

    while (0 == pu32Hist[s32Index]) ++s32Index;

    if (pu32Hist[s32Index] == u32Total) {
        memset(pu8Table, s32Index, 256 * sizeof(RK_U8));
    } else {
        fScale = (256 - 1.f) / (u16Width * u16Height - pu32Hist[s32Index]);

        for (pu8Table[s32Index++] = 0, s32Sum = 0; s32Index < 256; ++s32Index) {
            s32Sum += pu32Hist[s32Index];
            s32Tmp = (RK_S32)(s32Sum * fScale + 0.5);
            pu8Table[s32Index] = RK_CLIP(s32Tmp, RK_MAX_U8, RK_MIN_U8);
        }
    }
}

RK_VOID MapSample(const RK_CHAR *moduleName, RK_CHAR *fileName, RK_CHAR *outDir,
                  RK_U32 u32Width, RK_U32 u32Height) {
    IVE_SRC_IMAGE_S stSrc1;
    IVE_SRC_MEM_INFO_S stSrc2;
    IVE_DST_MEM_INFO_S stDst1;
    IVE_DST_IMAGE_S stDst2;
    RK_S32 s32Result;
    FILE *fp = NULL, *fo = NULL;
    IVE_HANDLE handle;
    RK_DOUBLE dTime;
    RK_CHAR outFile[RK_MAX_FILE_LEN];
    IVE_MAP_CTRL_S stMapCtrl = {IVE_MAP_MODE_U8};

    memset(&stSrc1, 0, sizeof(IVE_SRC_IMAGE_S));
    memset(&stSrc2, 0, sizeof(IVE_SRC_MEM_INFO_S));
    memset(&stDst1, 0, sizeof(IVE_DST_MEM_INFO_S));
    memset(&stDst2, 0, sizeof(IVE_DST_IMAGE_S));

    s32Result =
        RK_CreateIveImage(&stSrc1, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);
    s32Result =
        RK_CreateIveImage(&stDst2, IVE_IMAGE_TYPE_U8C1, u32Width, u32Height);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    switch (stDst2.enType) {
        case IVE_IMAGE_TYPE_U8C1: {
            stMapCtrl.enMode = IVE_MAP_MODE_U8;
            stSrc2.u32Size = 256 * sizeof(RK_U8);
        } break;
        case IVE_IMAGE_TYPE_S16C1: {
            stMapCtrl.enMode = IVE_MAP_MODE_S16;
            stSrc2.u32Size = 256 * sizeof(RK_S16);
        } break;
        case IVE_IMAGE_TYPE_U16C1: {
            stMapCtrl.enMode = IVE_MAP_MODE_U16;
            stSrc2.u32Size = 256 * sizeof(RK_U16);
        } break;
        default: {
            printf("stMapCtrl.enMode(%d) must be in [0, %d)!\n",
                   stMapCtrl.enMode, IVE_MAP_MODE_BUTT);
            return;
        }
    }

    s32Result = RK_CreateIveMem(&stSrc2, stSrc2.u32Size);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    stDst1.u32Size = 256 * sizeof(RK_U32) + 100;
    s32Result = RK_CreateIveMem(&stDst1, stDst1.u32Size);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    s32Result = RK_ReadFile(fileName, &stSrc1, &fp);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);


#if 0
    s32Result = RK_MPI_IVE_Hist(&handle, &stSrc1, &stDst1, RK_TRUE);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);

    GetMap((RK_U32 *)stDst1.u64VirAddr, (RK_U8 *)stSrc2.u64VirAddr, u16Width,
           u16Height);
#else
    RK_U8 TEST_MAP[256]={
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 
        16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 
        32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 
        48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 
        64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 
        80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 
        96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 
        112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 
        128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 
        144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 
        160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 
        176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 
        192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 
        208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 
        224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 
        240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 
    };
    RK_Memcpy((RK_U8 *)stSrc2.u64VirAddr, TEST_MAP, stSrc2.u32Size);
#endif

    dTime = (RK_DOUBLE)RK_GetTickCount();
    s32Result =
        RK_MPI_IVE_Map(&handle, &stSrc1, &stSrc2, &stDst2, &stMapCtrl, RK_TRUE);
    RK_CHECK_ET_GOTO(s32Result, RK_FAILURE, FAILURE);
    dTime = (RK_DOUBLE)RK_GetTickCount() - dTime;

    printf("Map time = %g ms\n",
           dTime / ((RK_DOUBLE)RK_GetTickFrequency() * 1000.));

    if (outDir == NULL) {
        RK_WriteFile(NULL, (void *)stDst2.au64VirAddr[0],
                     stDst2.u32Width * stDst2.u32Height, &fo);
    } else {
        RK_SPRINTF(outFile, RK_MAX_FILE_LEN, "%s/rve_%s_out.yuv", outDir, moduleName);
        RK_WriteFile(outFile, (void *)stDst2.au64VirAddr[0],
                     stDst2.u32Width * stDst2.u32Height, &fo);
    }

 #if BUILD_SIMULATOR
    gfx_show_img("map", (void *)stDst2.au64VirAddr[0], stDst2.u32Width,
                 stDst2.u32Height, IVE_IMAGE_TYPE_U8C1, 0);
#endif
FAILURE:
    RK_FCLOSE(fp);
    RK_FCLOSE(fo);
    RK_DestroyIveImage(&stSrc1);
    RK_DestroyIveImage(&stDst2);
    RK_DestroyIveMem(&stSrc2);
    RK_DestroyIveMem(&stDst1);
}
