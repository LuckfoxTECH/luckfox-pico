#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#include<stdbool.h>

#include "rk_common.h"

#include "sample_comm_mem.h"
#include "sample_comm_rve.h"

static bool bMpiInit = false;

RK_S32 RK_MPI_SYS_Init() {
    return RK_SUCCESS;
}

RK_S32 RK_MPI_SYS_Exit() {
    return RK_SUCCESS;
}

RK_U16 SAMPLE_COMM_IVE_CalcStride(RK_U32 u32Width, RK_U8 u8Align)
{
    return (u32Width + (u8Align - u32Width % u8Align) % u8Align);
}

static RK_S32 SAMPLE_IVE_MPI_Init(void)
{
    RK_S32 s32Ret;

    RK_MPI_SYS_Exit();

    s32Ret = RK_MPI_SYS_Init();
    if (RK_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("RK_MPI_SYS_Init fail,Error(%#x)\n", s32Ret);
        return s32Ret;
    }

    return RK_SUCCESS;
}


void SAMPLE_COMM_IVE_CheckIveMpiInit(void)
{
    if (false == bMpiInit)
    {
        if (SAMPLE_IVE_MPI_Init())
        {
            SAMPLE_PRT("Ive mpi init failed!\n");
            exit(-1);
        }
        bMpiInit = true;
    }
}
RK_S32 SAMPLE_COMM_IVE_IveMpiExit(void)
{
    bMpiInit = false;
    if (RK_MPI_SYS_Exit())
    {
        SAMPLE_PRT("Sys exit failed!\n");
        return RK_FAILURE;
    }

    return RK_SUCCESS;
}

RK_S32 SAMPLE_COMM_IVE_ReadFile(RK_CHAR *filename, IVE_IMAGE_S* pstImg, FILE **fp_out)
{
    RK_U16 y;
    RK_U8* pU8;
    RK_U16 height;
    RK_U16 width;
    RK_U16 loop;
    FILE *pFp = *fp_out;
    RK_S32 s32Ret;

    if (pFp == NULL) {
        if (filename == NULL) {
            printf("filename is null\n");
            return -1;
        }

        pFp = fopen(filename, "rb");

        if (pFp == NULL) {
            printf("failed to open(%s)\n", filename);
            return -1;
        }
    }

    if (feof(pFp)) {
        fseek(pFp, 0, SEEK_SET);
    }

    height = pstImg->u32Height;
    width = pstImg->u32Width;

    SYS_MmzFlush_Start((void *)pstImg->au64VirAddr[0]);

    switch (pstImg->enType)
    {
        case  IVE_IMAGE_TYPE_U8C1:
        {
            pU8 = (RK_U8 *)pstImg->au64VirAddr[0];
            for (y = 0; y < height; y++)
            {
                if ( 1 != fread(pU8, width, 1, pFp))
                {
                    SAMPLE_PRT("Read file fail\n");
                    return RK_FAILURE;
                }

                pU8 += pstImg->au32Stride[0];
            }
        }
        break;
        case  IVE_IMAGE_TYPE_YUV420SP:
        {
            pU8 = (RK_U8 *)pstImg->au64VirAddr[0];
            for (y = 0; y < height; y++)
            {
                if ( 1 != fread(pU8, width, 1, pFp))
                {
                    SAMPLE_PRT("Read file fail\n");
                    return RK_FAILURE;
                }

                pU8 += pstImg->au32Stride[0];
            }

            pU8 = (RK_U8 *)pstImg->au64VirAddr[1];
            for (y = 0; y < height / 2; y++)
            {
                if ( 1 != fread(pU8, width, 1, pFp))
                {
                    SAMPLE_PRT("Read file fail\n");
                    return RK_FAILURE;
                }

                pU8 += pstImg->au32Stride[1];
            }
        }
        break;
        case IVE_IMAGE_TYPE_YUV422SP: {
            pU8 = (RK_U8 *)pstImg->au64VirAddr[0];
            for (y = 0; y < height; y++)
            {
                if ( 1 != fread(pU8, width, 1, pFp))
                {
                    SAMPLE_PRT("Read file fail\n");
                    return RK_FAILURE;
                }

                pU8 += pstImg->au32Stride[0];
            }

            pU8 = (RK_U8 *)pstImg->au64VirAddr[1];
            for (y = 0; y < height; y++)
            {
                if ( 1 != fread(pU8, width, 1, pFp))
                {
                    SAMPLE_PRT("Read file fail\n");
                    return RK_FAILURE;
                }

                pU8 += pstImg->au32Stride[1];
            }
        } break;
        case IVE_IMAGE_TYPE_U8C3_PACKAGE: {
            pU8 = (RK_U8 *)pstImg->au64VirAddr[0];
            for (y = 0; y < height; y++)
            {
                if ( 1 != fread(pU8, width * 3, 1, pFp))
                {
                    SAMPLE_PRT("Read file fail\n");
                    return RK_FAILURE;
                }

                pU8 += pstImg->au32Stride[0] * 3;
            }

        } break;
        case IVE_IMAGE_TYPE_U8C3_PLANAR: {
            for (loop = 0; loop < 3; loop++)
            {
                pU8 = (RK_U8 *)pstImg->au64VirAddr[loop];
                for (y = 0; y < height; y++)
                {
                    if ( 1 != fread(pU8, width, 1, pFp))
                    {
                        SAMPLE_PRT("Read file fail\n");
                        return RK_FAILURE;
                    }

                    pU8 += pstImg->au32Stride[loop];
                }
            }

        } break;
        case IVE_IMAGE_TYPE_S16C1:
        case IVE_IMAGE_TYPE_U16C1: {
            pU8 = (RK_U8 *)pstImg->au64VirAddr[0];
            for ( y = 0; y < height; y++ )
            {
                if ( sizeof(RK_U16) != fread(pU8, width, sizeof(RK_U16), pFp) )
                {
                    SAMPLE_PRT("Read file fail\n");
                    return RK_FAILURE;
                }

                pU8 += pstImg->au32Stride[0] * 2;
            }
        } break;
        default:
            break;
    }

    *fp_out = pFp;

end:
    SYS_MmzFlush_End((void *)pstImg->au64VirAddr[0]);

    return RK_SUCCESS;
}

RK_S32 SAMPLE_COMM_IVE_WriteFile(RK_CHAR *filename, IVE_IMAGE_S* pstImg, FILE **fp_out)
{
    RK_U16 y;
    RK_U8* pU8;
    RK_U16 height;
    RK_U16 width;
    FILE *pFp = *fp_out;

    if (pFp == NULL) {
        if (filename == NULL) {
            printf("filename is null\n");
            return -1;
        }

        pFp = fopen(filename, "wb");
        if (pFp == NULL) {
            printf("failed to open(%s)\n", filename);
            return -1;
        }
    }

    height = pstImg->u32Height;
    width = pstImg->u32Width;

    SYS_MmzFlush_Start((void *)pstImg->au64VirAddr[0]);

    switch (pstImg->enType)
    {
        case  IVE_IMAGE_TYPE_U8C1:
        case  IVE_IMAGE_TYPE_S8C1:
        {
            pU8 = (RK_U8 *)pstImg->au64VirAddr[0];
            for (y = 0; y < height; y++)
            {
                if ( 1 != fwrite(pU8, width, 1, pFp))
                {
                    SAMPLE_PRT("Write file fail\n");
                    return RK_FAILURE;
                }

                pU8 += pstImg->au32Stride[0];
            }
        }
        break;
        case  IVE_IMAGE_TYPE_YUV420SP:
        {
            pU8 = (RK_U8 *)pstImg->au64VirAddr[0];
            for (y = 0; y < height; y++)
            {
                if ( width != fwrite(pU8, 1, width, pFp))
                {
                    SAMPLE_PRT("Write file fail\n");
                    return RK_FAILURE;
                }

                pU8 += pstImg->au32Stride[0];
            }

            pU8 = (RK_U8 *)pstImg->au64VirAddr[1];
            for (y = 0; y < height / 2; y++)
            {
                if ( width != fwrite(pU8, 1, width, pFp))
                {
                    SAMPLE_PRT("Write file fail\n");
                    return RK_FAILURE;
                }

                pU8 += pstImg->au32Stride[1];
            }
        }
        break;
        case IVE_IMAGE_TYPE_YUV422SP:
        {
            pU8 = (RK_U8 *)pstImg->au64VirAddr[0];
            for (y = 0; y < height; y++)
            {
                if ( width != fwrite(pU8, 1, width, pFp))
                {
                    SAMPLE_PRT("Write file fail\n");
                    return RK_FAILURE;
                }

                pU8 += pstImg->au32Stride[0];
            }

            pU8 = (RK_U8 *)pstImg->au64VirAddr[1];
            for (y = 0; y < height; y++)
            {
                if ( width != fwrite(pU8, 1, width, pFp))
                {
                    SAMPLE_PRT("Write file fail\n");
                    return RK_FAILURE;
                }

                pU8 += pstImg->au32Stride[1];
            }
        }
        break;
        case IVE_IMAGE_TYPE_S16C1:
        case  IVE_IMAGE_TYPE_U16C1:
        {
            pU8 = (RK_U8 *)pstImg->au64VirAddr[0];
            for ( y = 0; y < height; y++ )
            {
                if ( sizeof(RK_U16) != fwrite(pU8, width, sizeof(RK_U16), pFp) )
                {
                    SAMPLE_PRT("Write file fail\n");
                    return RK_FAILURE;
                }

                pU8 += pstImg->au32Stride[0] * 2;
            }
        }
        break;
        case IVE_IMAGE_TYPE_U32C1:
        {

            pU8 = (RK_U8 *)pstImg->au64VirAddr[0];
            for ( y = 0; y < height; y++ )
            {
                if ( width != fwrite(pU8, sizeof(RK_U32), width, pFp) )
                {
                    SAMPLE_PRT("Write file fail\n");
                    return RK_FAILURE;
                }

                pU8 += pstImg->au32Stride[0] * 4;
            }
            break;
        }

        default:
            break;
    }

    *fp_out = pFp;

end:
    SYS_MmzFlush_End((void *)pstImg->au64VirAddr[0]);

    return RK_SUCCESS;
}

void SAMPLE_COMM_IVE_BlobToRect(IVE_CCBLOB_S *pstBlob, SAMPLE_RECT_ARRAY_S *pstRect,
                                            RK_U16 u16RectMaxNum,RK_U16 u16AreaThrStep,
                                            RK_U32 u32SrcWidth, RK_U32 u32SrcHeight,
                                            RK_U32 u32DstWidth,RK_U32 u32DstHeight)
{
    RK_U16 u16Num;
    RK_U16 i,j,k;
    RK_U32 u32Thr= 0;
    bool bValid;

    if(pstBlob->u8RegionNum > u16RectMaxNum)
    {
        u32Thr = pstBlob->u32CurAreaThr;                                                                                                               
        do
        {
            u16Num = 0;
            u32Thr += u16AreaThrStep;
            for(i = 0;i < 254;i++)
            {
                if(pstBlob->astRegion[i].u32Area > u32Thr)
                {
                    u16Num++;
                }
            }
        }while(u16Num > u16RectMaxNum);
    }

   u16Num = 0;

   for(i = 0;i < 254;i++)
    {
        if(pstBlob->astRegion[i].u32Area > u32Thr)
        {
            pstRect->astRect[u16Num].astPoint[0].s32X = (RK_U32)((float)pstBlob->astRegion[i].u16Left / (float)u32SrcWidth * (float)u32DstWidth) & (~1) ;
            pstRect->astRect[u16Num].astPoint[0].s32Y = (RK_U32)((float)pstBlob->astRegion[i].u16Top / (float)u32SrcHeight * (float)u32DstHeight) & (~1);

            pstRect->astRect[u16Num].astPoint[1].s32X = (RK_U32)((float)pstBlob->astRegion[i].u16Right/ (float)u32SrcWidth * (float)u32DstWidth) & (~1);
            pstRect->astRect[u16Num].astPoint[1].s32Y = (RK_U32)((float)pstBlob->astRegion[i].u16Top / (float)u32SrcHeight * (float)u32DstHeight) & (~1);

            pstRect->astRect[u16Num].astPoint[2].s32X = (RK_U32)((float)pstBlob->astRegion[i].u16Right / (float)u32SrcWidth * (float)u32DstWidth) & (~1);
            pstRect->astRect[u16Num].astPoint[2].s32Y = (RK_U32)((float)pstBlob->astRegion[i].u16Bottom / (float)u32SrcHeight * (float)u32DstHeight) & (~1);

            pstRect->astRect[u16Num].astPoint[3].s32X = (RK_U32)((float)pstBlob->astRegion[i].u16Left / (float)u32SrcWidth * (float)u32DstWidth) & (~1);
            pstRect->astRect[u16Num].astPoint[3].s32Y = (RK_U32)((float)pstBlob->astRegion[i].u16Bottom / (float)u32SrcHeight * (float)u32DstHeight) & (~1);

            bValid = true;
            for(j = 0; j < 3;j++)
            {
                for (k = j + 1; k < 4;k++)
                {
                    if ((pstRect->astRect[u16Num].astPoint[j].s32X == pstRect->astRect[u16Num].astPoint[k].s32X)
                         &&(pstRect->astRect[u16Num].astPoint[j].s32Y == pstRect->astRect[u16Num].astPoint[k].s32Y))
                    {
                    bValid = false;
                    break;
                    }
                }
            }
            if (true == bValid)
            {
                u16Num++;
            }
        }
    }

    pstRect->u16Num = u16Num;
}

/******************************************************************************
* function : Create ive image
******************************************************************************/
RK_S32 SAMPLE_COMM_IVE_CreateImage(IVE_IMAGE_S* pstImg, IVE_IMAGE_TYPE_E enType, RK_U32 u32Width, RK_U32 u32Height)
{
    RK_U32 u32Size = 0;
    RK_S32 s32Ret;
    if (NULL == pstImg)
    {
        SAMPLE_PRT("pstImg is null\n");
        return RK_FAILURE;
    }

    pstImg->enType = enType;
    pstImg->u32Width = u32Width;
    pstImg->u32Height = u32Height;
    pstImg->au32Stride[0] = SAMPLE_COMM_IVE_CalcStride(pstImg->u32Width, IVE_ALIGN_VAL);

    switch (enType)
    {
        case IVE_IMAGE_TYPE_U8C1:
        case IVE_IMAGE_TYPE_S8C1:
        {
            u32Size = pstImg->au32Stride[0] * pstImg->u32Height;
            s32Ret = MmzAlloc(&pstImg->au64PhyAddr[0], (void**)&pstImg->au64VirAddr[0], u32Size);
            if (s32Ret != RK_SUCCESS)
            {
                SAMPLE_PRT("Mmz Alloc fail,Error(%#x)\n", s32Ret);
                return s32Ret;
            }
        }
        break;
        case IVE_IMAGE_TYPE_YUV420SP:
        {
            u32Size = pstImg->au32Stride[0] * pstImg->u32Height * 3 / 2;
            s32Ret = MmzAlloc(&pstImg->au64PhyAddr[0], (void**)&pstImg->au64VirAddr[0], u32Size);
            if (s32Ret != RK_SUCCESS)
            {
                SAMPLE_PRT("Mmz Alloc fail,Error(%#x)\n", s32Ret);
                return s32Ret;
            }
            pstImg->au32Stride[1] = pstImg->au32Stride[0];
            pstImg->au64PhyAddr[1] = pstImg->au64PhyAddr[0] + pstImg->au32Stride[0] * pstImg->u32Height;
            pstImg->au64VirAddr[1] = pstImg->au64VirAddr[0] + pstImg->au32Stride[0] * pstImg->u32Height;

        }
        break;
        case IVE_IMAGE_TYPE_YUV422SP:
        {
            u32Size = pstImg->au32Stride[0] * pstImg->u32Height * 2;
            s32Ret = MmzAlloc(&pstImg->au64PhyAddr[0], (void**)&pstImg->au64VirAddr[0], u32Size);
            if (s32Ret != RK_SUCCESS)
            {
                SAMPLE_PRT("Mmz Alloc fail,Error(%#x)\n", s32Ret);
                return s32Ret;
            }
            pstImg->au32Stride[1] = pstImg->au32Stride[0];
            pstImg->au64PhyAddr[1] = pstImg->au64PhyAddr[0] + pstImg->au32Stride[0] * pstImg->u32Height;
            pstImg->au64VirAddr[1] = pstImg->au64VirAddr[0] + pstImg->au32Stride[0] * pstImg->u32Height;

        }
        break;
        case IVE_IMAGE_TYPE_YUV420P:
            break;
        case IVE_IMAGE_TYPE_YUV422P:
            break;
        case IVE_IMAGE_TYPE_S8C2_PACKAGE:
            break;
        case IVE_IMAGE_TYPE_S8C2_PLANAR:
            break;
        case IVE_IMAGE_TYPE_S16C1:
        case IVE_IMAGE_TYPE_U16C1:
        {

            u32Size = pstImg->au32Stride[0] * pstImg->u32Height * sizeof(RK_U16);
            s32Ret = MmzAlloc(&pstImg->au64PhyAddr[0], (void**)&pstImg->au64VirAddr[0], u32Size);
            if (s32Ret != RK_SUCCESS)
            {
                SAMPLE_PRT("Mmz Alloc fail,Error(%#x)\n", s32Ret);
                return s32Ret;
            }
        }
        break;
        case IVE_IMAGE_TYPE_U8C3_PACKAGE:
        {
            u32Size = pstImg->au32Stride[0] * pstImg->u32Height * 3;
            s32Ret = MmzAlloc(&pstImg->au64PhyAddr[0], (void**)&pstImg->au64VirAddr[0], u32Size);
            if (s32Ret != RK_SUCCESS)
            {
                SAMPLE_PRT("Mmz Alloc fail,Error(%#x)\n", s32Ret);
                return s32Ret;
            }
            pstImg->au64VirAddr[1] = pstImg->au64VirAddr[0] + 1;
            pstImg->au64VirAddr[2] = pstImg->au64VirAddr[1] + 1;
            pstImg->au64PhyAddr[1] = pstImg->au64PhyAddr[0] + 1;
            pstImg->au64PhyAddr[2] = pstImg->au64PhyAddr[1] + 1;
            pstImg->au32Stride[1] = pstImg->au32Stride[0];
            pstImg->au32Stride[2] = pstImg->au32Stride[0];
        }
        break;
        case IVE_IMAGE_TYPE_U8C3_PLANAR:
            break;
        case IVE_IMAGE_TYPE_S32C1:
        case IVE_IMAGE_TYPE_U32C1:
        {
            u32Size = pstImg->au32Stride[0] * pstImg->u32Height * sizeof(RK_U32);
            s32Ret = MmzAlloc(&pstImg->au64PhyAddr[0], (void**)&pstImg->au64VirAddr[0], u32Size);
            if (s32Ret != RK_SUCCESS)
            {
                SAMPLE_PRT("Mmz Alloc fail,Error(%#x)\n", s32Ret);
                return s32Ret;
            }
        }
        break;
        case IVE_IMAGE_TYPE_S64C1:
        case IVE_IMAGE_TYPE_U64C1:
        {

            u32Size = pstImg->au32Stride[0] * pstImg->u32Height * sizeof(RK_U64);
            s32Ret = MmzAlloc(&pstImg->au64PhyAddr[0], (void**)&pstImg->au64VirAddr[0], u32Size);
            if (s32Ret != RK_SUCCESS)
            {
                SAMPLE_PRT("Mmz Alloc fail,Error(%#x)\n", s32Ret);
                return s32Ret;
            }
        }
        break;
        default:
            break;

    }

    return RK_SUCCESS;
}
/******************************************************************************
* function : Create memory info
******************************************************************************/
RK_S32 SAMPLE_COMM_IVE_CreateMemInfo(IVE_MEM_INFO_S* pstMemInfo, RK_U32 u32Size)
{
    RK_S32 s32Ret;

    if (NULL == pstMemInfo)
    {
        SAMPLE_PRT("pstMemInfo is null\n");
        return RK_FAILURE;
    }
    pstMemInfo->u32Size = u32Size;
    s32Ret = MmzAlloc(&pstMemInfo->u64PhyAddr, (void**)&pstMemInfo->u64VirAddr, u32Size);
    if (s32Ret != RK_SUCCESS)
    {
        SAMPLE_PRT("Mmz Alloc fail,Error(%#x)\n", s32Ret);
        return RK_FAILURE;
    }

    return RK_SUCCESS;
}
/******************************************************************************
* function : Create ive image by cached
******************************************************************************/
RK_S32 SAMPLE_COMM_IVE_CreateImageByCached(IVE_IMAGE_S* pstImg,
        IVE_IMAGE_TYPE_E enType, RK_U32 u32Width, RK_U32 u32Height)
{
    RK_U32 u32Size = 0;
    RK_S32 s32Ret;
    if (NULL == pstImg)
    {
        SAMPLE_PRT("pstImg is null\n");
        return RK_FAILURE;
    }

    pstImg->enType = enType;
    pstImg->u32Width = u32Width;
    pstImg->u32Height = u32Height;
    pstImg->au32Stride[0] = SAMPLE_COMM_IVE_CalcStride(pstImg->u32Width, IVE_ALIGN_VAL);

    switch (enType)
    {
        case IVE_IMAGE_TYPE_U8C1:
        case IVE_IMAGE_TYPE_S8C1:
        {
            u32Size = pstImg->au32Stride[0] * pstImg->u32Height;
            s32Ret = MmzAlloc_Cached(&pstImg->au64PhyAddr[0], (void**)&pstImg->au64VirAddr[0], u32Size);
            if (s32Ret != RK_SUCCESS)
            {
                SAMPLE_PRT("Mmz Alloc fail,Error(%#x)\n", s32Ret);
                return s32Ret;
            }
        }
        break;
        case IVE_IMAGE_TYPE_YUV420SP:
            break;
        case IVE_IMAGE_TYPE_YUV422SP:
            break;
        case IVE_IMAGE_TYPE_YUV420P:
            break;
        case IVE_IMAGE_TYPE_YUV422P:
            break;
        case IVE_IMAGE_TYPE_S8C2_PACKAGE:
            break;
        case IVE_IMAGE_TYPE_S8C2_PLANAR:
            break;
        case IVE_IMAGE_TYPE_S16C1:
        case IVE_IMAGE_TYPE_U16C1:
        {

            u32Size = pstImg->au32Stride[0] * pstImg->u32Height * sizeof(RK_U16);
            s32Ret = MmzAlloc_Cached(&pstImg->au64PhyAddr[0], (void**)&pstImg->au64VirAddr[0], u32Size);
            if (s32Ret != RK_SUCCESS)
            {
                SAMPLE_PRT("Mmz Alloc fail,Error(%#x)\n", s32Ret);
                return s32Ret;
            }
        }
        break;
        case IVE_IMAGE_TYPE_U8C3_PACKAGE:
            break;
        case IVE_IMAGE_TYPE_U8C3_PLANAR:
            break;
        case IVE_IMAGE_TYPE_S32C1:
        case IVE_IMAGE_TYPE_U32C1:
        {
            u32Size = pstImg->au32Stride[0] * pstImg->u32Height * sizeof(RK_U32);
            s32Ret = MmzAlloc_Cached(&pstImg->au64PhyAddr[0], (void**)&pstImg->au64VirAddr[0], u32Size);
            if (s32Ret != RK_SUCCESS)
            {
                SAMPLE_PRT("Mmz Alloc fail,Error(%#x)\n", s32Ret);
                return s32Ret;
            }
        }
        break;
        case IVE_IMAGE_TYPE_S64C1:
        case IVE_IMAGE_TYPE_U64C1:
        {

            u32Size = pstImg->au32Stride[0] * pstImg->u32Height * sizeof(RK_U64);
            s32Ret = MmzAlloc_Cached(&pstImg->au64PhyAddr[0], (void**)&pstImg->au64VirAddr[0], u32Size);
            if (s32Ret != RK_SUCCESS)
            {
                SAMPLE_PRT("Mmz Alloc fail,Error(%#x)\n", s32Ret);
                return s32Ret;
            }
        }
        break;
        default:
            break;
    }

    return RK_SUCCESS;
}

RK_S32 SAMPLE_COMM_IVE_CreateData(IVE_DATA_S* pstData,RK_U32 u32Width, RK_U32 u32Height)
{
    RK_S32 s32Ret;
    RK_U32 u32Size;

    if (NULL == pstData)
    {
        SAMPLE_PRT("pstData is null\n");
        return RK_FAILURE;
    }
    pstData->u32Width = u32Width;
    pstData->u32Height = u32Height;
    pstData->u32Stride = SAMPLE_COMM_IVE_CalcStride(pstData->u32Width, IVE_ALIGN_VAL);
    u32Size = pstData->u32Stride * pstData->u32Height;
    s32Ret = MmzAlloc(&pstData->u64PhyAddr, (void**)&pstData->u64VirAddr, u32Size);
    if (s32Ret != RK_SUCCESS)
    {
        SAMPLE_PRT("Mmz Alloc fail,Error(%#x)\n", s32Ret);
        return RK_FAILURE;
    }

    return RK_SUCCESS;
}

/******************************************************************************
* function : Dma frame info to  ive image
******************************************************************************/
RK_S32 SAMPLE_COMM_IVE_DmaImage(VIDEO_FRAME_INFO_S *pstFrameInfo,IVE_DST_IMAGE_S *pstDst,bool bInstant)
{
    RK_S32 s32Ret;
    IVE_HANDLE hIveHandle;
    IVE_SRC_DATA_S stSrcData;
    IVE_DST_DATA_S stDstData;
    IVE_DMA_CTRL_S stCtrl = {IVE_DMA_MODE_DIRECT_COPY,0};
    bool bFinish = false;
    bool bBlock = true;

    //fill src
    //stSrcData.u64VirAddr = pstFrameInfo->stVFrame.u64VirAddr[0];
    stSrcData.u64PhyAddr = pstFrameInfo->stVFrame.u64PhyAddr[0];
    stSrcData.u32Width   = pstFrameInfo->stVFrame.u32Width;
    stSrcData.u32Height  = pstFrameInfo->stVFrame.u32Height;
    stSrcData.u32Stride  = pstFrameInfo->stVFrame.u32Stride[0];

    //fill dst
    //stDstData.u64VirAddr = pstDst->au64VirAddr[0];
    stDstData.u64PhyAddr = pstDst->au64PhyAddr[0];
    stDstData.u32Width   = pstDst->u32Width;
    stDstData.u32Height  = pstDst->u32Height;
    stDstData.u32Stride  = pstDst->au32Stride[0];

    s32Ret = RK_MPI_IVE_DMA(&hIveHandle,&stSrcData,&stDstData,&stCtrl,bInstant);
    SAMPLE_CHECK_EXPR_RET(RK_SUCCESS != s32Ret,s32Ret,"Error(%#x),IVE_DMA_ID failed!\n",s32Ret);

    if (true == bInstant)
    {
        s32Ret = RK_MPI_IVE_Query(hIveHandle,&bFinish,bBlock);
        while(ERR_IVE_QUERY_TIMEOUT == s32Ret)
        {
            usleep(100);
            s32Ret = RK_MPI_IVE_Query(hIveHandle, &bFinish, bBlock);
        }
        SAMPLE_CHECK_EXPR_RET(RK_SUCCESS != s32Ret,s32Ret,"Error(%#x),IVE_Query failed!\n",s32Ret);
    }

    return RK_SUCCESS;
}
