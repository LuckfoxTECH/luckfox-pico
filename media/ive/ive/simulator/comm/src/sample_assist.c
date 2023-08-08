
#include "sample_assist.h"
#include "sample_define.h"
#include "sample_mem.h"
#include <stdlib.h>
#if defined WIN32 || defined _WIN32 || defined WINCE
#include <Windows.h>
#else
#include <time.h>
#endif

RK_U32 RK_CalcStride(RK_U32 u32Width, RK_U32 u32Align)
{
	RK_U32 u32stride = u32Width + (u32Align - u32Width%u32Align)%u32Align;
	return u32stride;
}

RK_S32 RK_CreateIveImage(IVE_IMAGE_S *pstImage,IVE_IMAGE_TYPE_E enType, RK_U32 u32Width, RK_U32 u32Height)
{
	RK_U32 u32Stride;	
	RK_S32 s32Succ;

	RK_CHECK_ET_NULL_RET(pstImage,RK_FAILURE);

	pstImage->enType = enType;
	pstImage->u32Width = u32Width;
	pstImage->u32Height = u32Height;

	u32Stride = RK_CalcStride(u32Width, RK_IVE2_STRIDE_ALIGN);
	s32Succ = RK_SUCCESS;

	switch(enType)
	{
	case IVE_IMAGE_TYPE_U8C1:
	case IVE_IMAGE_TYPE_S8C1:
		{
            MmzAlloc(&pstImage->au64PhyAddr[0], &pstImage->au64VirAddr[0], u32Stride * u32Height);
            RK_CHECK_ET_RET(pstImage->au64VirAddr[0], 0, RK_FAILURE);
            pstImage->au32Stride[0] = u32Stride;
		}
		break;
	case IVE_IMAGE_TYPE_U16C1:
	case IVE_IMAGE_TYPE_S16C1:
		{
            MmzAlloc(&pstImage->au64PhyAddr[0], &pstImage->au64VirAddr[0], u32Stride * u32Height * 2);
            RK_CHECK_ET_RET(pstImage->au64VirAddr[0], 0, RK_FAILURE);
            pstImage->au32Stride[0] = u32Stride;
		}
		break;
	case IVE_IMAGE_TYPE_U32C1:
		{
            MmzAlloc(&pstImage->au64PhyAddr[0], &pstImage->au64VirAddr[0], u32Stride * u32Height * 4);
            RK_CHECK_ET_RET(pstImage->au64VirAddr[0], 0, RK_FAILURE);
            pstImage->au32Stride[0] = u32Stride;
		}
		break;
	case IVE_IMAGE_TYPE_U64C1:
		{
            MmzAlloc(&pstImage->au64PhyAddr[0], &pstImage->au64VirAddr[0], u32Stride * u32Height * 8);
            RK_CHECK_ET_RET(pstImage->au64VirAddr[0], 0, RK_FAILURE);
            pstImage->au32Stride[0] = u32Stride;
		}
		break;
	case IVE_IMAGE_TYPE_YUV420SP:
		{
            MmzAlloc(&pstImage->au64PhyAddr[0], &pstImage->au64VirAddr[0], u32Stride * u32Height * 3 / 2);
            RK_CHECK_ET_RET(pstImage->au64VirAddr[0], 0, RK_FAILURE);
            pstImage->au32Stride[0] = u32Stride;
            pstImage->au32Stride[1] = u32Stride;
            pstImage->au64PhyAddr[1] =
                pstImage->au64PhyAddr[0] + u32Stride * u32Height;
            pstImage->au64VirAddr[1] =
                pstImage->au64VirAddr[0] + u32Stride * u32Height;
        }
		break;
	case IVE_IMAGE_TYPE_U8C3_PACKAGE:
		{
            MmzAlloc(&pstImage->au64PhyAddr[0], &pstImage->au64VirAddr[0], u32Stride * u32Height * 3);
            RK_CHECK_ET_RET(pstImage->au64VirAddr[0], 0, RK_FAILURE);
            pstImage->au32Stride[0] = u32Stride;
		}
		break;
    case IVE_IMAGE_TYPE_U8C3_PLANAR:
        {
            MmzAlloc(&pstImage->au64PhyAddr[0],
                        &pstImage->au64VirAddr[0],
                        u32Stride * u32Height * 3);
            RK_CHECK_ET_RET(pstImage->au64VirAddr[0], 0, RK_FAILURE);
            pstImage->au32Stride[0] = u32Stride;

            pstImage->au32Stride[1] = u32Stride;
            pstImage->au64PhyAddr[1] =
                pstImage->au64PhyAddr[0] + u32Stride * u32Height;
            pstImage->au64VirAddr[1] =
                pstImage->au64VirAddr[0] + u32Stride * u32Height;

            pstImage->au32Stride[2] = u32Stride;
            pstImage->au64PhyAddr[2] =
                pstImage->au64PhyAddr[0] + u32Stride * u32Height * 2;
            pstImage->au64VirAddr[2] =
                pstImage->au64VirAddr[0] + u32Stride * u32Height * 2;
        } break;
        default:
		{
			s32Succ = RK_FAILURE;
		}
		break;
	}

	return s32Succ;
}

RK_S32 RK_CreateIveMem(IVE_MEM_INFO_S *pstImage, RK_U32 u32Size) {
    RK_U32 u32Stride;
    RK_S32 s32Succ = 0;

    RK_CHECK_ET_NULL_RET(pstImage, RK_FAILURE);

    MmzAlloc(&pstImage->u64PhyAddr, &pstImage->u64VirAddr,
                u32Size);
    RK_CHECK_ET_RET(pstImage->u64VirAddr, 0, RK_FAILURE);

    pstImage->u32Size = u32Size;

    return s32Succ;
}

RK_S32 RK_CreateIveData(IVE_DATA_S *pstImage, RK_U32 u32Width,
                        RK_U32 u32Height) {
    RK_U32 u32Stride;
    RK_S32 s32Succ = 0;

    RK_CHECK_ET_NULL_RET(pstImage, RK_FAILURE);

    pstImage->u32Width = u32Width;
    pstImage->u32Height = u32Height;

    u32Stride = RK_CalcStride(u32Width, RK_IVE2_STRIDE_ALIGN);

    MmzAlloc(&pstImage->u64PhyAddr, &pstImage->u64VirAddr,
             u32Stride * u32Height);
    RK_CHECK_ET_RET(pstImage->u64VirAddr, 0, RK_FAILURE);
    pstImage->u32Stride = u32Stride;

    return s32Succ;
}

RK_S32 RK_DestroyIveImage(IVE_IMAGE_S *pstImage)
{

	RK_CHECK_ET_NULL_RET(pstImage,RK_FAILURE);
	if (0 != pstImage->au64VirAddr[0])
	{
            MmzFree((RK_VOID *)pstImage->au64VirAddr[0]);
            pstImage->au64VirAddr[0] = 0;
	}

	return RK_SUCCESS;
}

RK_S32 RK_DestroyIveMem(IVE_MEM_INFO_S *pstImage) {
    RK_CHECK_ET_NULL_RET(pstImage, RK_FAILURE);
    if (0 != pstImage->u64VirAddr) {
        MmzFree((RK_VOID *)pstImage->u64VirAddr);
        pstImage->u64VirAddr = 0;
    }

    return RK_SUCCESS;
}

RK_S32 RK_DestroyIveData(IVE_DATA_S *pstImage) {
    RK_CHECK_ET_NULL_RET(pstImage, RK_FAILURE);
    if (0 != pstImage->u64VirAddr) {
        MmzFree((RK_VOID *)pstImage->u64VirAddr);
        pstImage->u64VirAddr = 0;
    }

    return RK_SUCCESS;
}

RK_S32 comp(const RK_VOID *a,const RK_VOID *b)
{
	return  *(RK_U8 *)(*(RK_U32 *)b) - *(RK_U8 *)(*(RK_U32 *)a);
}

RK_S64 RK_GetTickCount(RK_VOID)
{
	RK_S64 s32Tick = 0;

#if defined WIN32 || defined _WIN32 || defined WINCE
	LARGE_INTEGER counter;
	QueryPerformanceCounter( &counter );
	s32Tick = (RK_S64)counter.QuadPart;
#elif defined __linux || defined __linux__ || defined __DARWIN__ || defined __APPLE__
	struct timespec tp;
	clock_gettime(CLOCK_MONOTONIC, &tp);
	s32Tick =  (RK_S64)tp.tv_sec*1000000000 + tp.tv_nsec;
#else
	struct timeval tv;
	struct timezone tz;
	gettimeofday( &tv, &tz );
	s32Tick =  (RK_S64)tv.tv_sec*1000000 + tv.tv_usec;
#endif

	return s32Tick;
}

RK_DOUBLE RK_GetTickFrequency(RK_VOID)
{
	RK_DOUBLE dFre = 1.0;
#if defined WIN32 || defined _WIN32 || defined WINCE
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	dFre =  (RK_DOUBLE)freq.QuadPart;
#elif defined __linux || defined __linux__ || defined __DARWIN__ || defined __APPLE__
	dFre =  1e9;
#else
	dFre =  1e6;
#endif

	return (dFre * 1e-6);

}

RK_S32 RK_GenRand(RK_S32 s32Max,RK_S32 s32Min)
{
	RK_S32 s32Result = 0;

	if (s32Min >= 0)
	{
		s32Result = s32Min + rand()%(s32Max - s32Min + 1);
	}
	else 
	{		
		s32Result = rand() % (s32Max - s32Min + 1);
		s32Result = s32Result > s32Max ?   s32Max - s32Result: s32Result;
	}

	return s32Result;
}
