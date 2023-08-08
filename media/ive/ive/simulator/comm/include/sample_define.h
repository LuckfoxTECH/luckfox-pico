
/*
 * Copyright 2020 Rockchip Electronics Co. LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <stdio.h>

#ifndef SAMPLE_DEFINE_H_
#define SAMPLE_DEFINE_H_

#define IVE_STRIDE_ALIGN 16
#define SAMPLE_PRT(...)\
do{\
	printf("[Func]:%s [Line]:%d [Info]:", __FUNCTION__, __LINE__);\
	printf(__VA_ARGS__);\
}while(0)

#define RK_CHECK_ET_RET(srcData,value,ret)\
do{\
	if((value) == (srcData))\
	{\
		return (ret);\
	}\
}while(0)

#define RK_CHECK_ET_RET_VOID(srcData,value)\
do{\
	if((value) == (srcData))\
	{\
		return ;\
	}\
}while(0)

#define RK_CHECK_NET_RET(srcData,value,ret)\
do{\
	if((value) != (srcData))\
	{\
		return (ret);\
	}\
}while(0)

#define RK_CHECK_NET_RET_VOID(srcData,value)\
do{\
	if((value) != (srcData))\
	{\
		return ;\
	}\
}while(0)

#define RK_CHECK_GT_RET(srcData,value,ret)\
do{\
	if((srcData) > (value))\
	{\
		return (ret);\
	}\
}while(0)


#define RK_CHECK_GT_AND_ET_RET(srcData,value,ret)\
do{\
	if((srcData) >= (value))\
	{\
		return (ret);\
	}\
}while(0)

#define RK_CHECK_LT_RET(srcData,value,ret)\
do{\
	if((srcData) < (value))\
	{\
		return (ret);\
	}\
}while(0)

#define RK_CHECK_LT_AND_ET_RET(srcData,value,ret)\
do{\
	if((srcData) <= (value))\
	{\
		return (ret);\
	}\
}while(0)

#define RK_CHECK_NET_GOTO(srcData,value,label)\
do{\
	if((value) != (srcData))\
	{\
		goto label;\
	}\
}while(0)

#define RK_CHECK_ET_GOTO(srcData,value,label)\
do{\
	if((value) == (srcData))\
	{\
		goto label;\
	}\
}while(0)

#define RK_CHECK_ET_NULL_RET_VOID(ptr) RK_CHECK_ET_RET_VOID(ptr,NULL)
#define RK_CHECK_ET_NULL_RET(ptr,ret) RK_CHECK_ET_RET(ptr,NULL,ret)

#define RK_CHECK_ET_RET_PRT(srcData,value,ret, ...)\
do{\
	if((value) == (srcData))\
	{\
		SAMPLE_PRT(__VA_ARGS__);\
		return (ret);\
	}\
}while(0)

#define RK_CHECK_ET_RET_VOID_PRT(srcData,value, ...)\
do{\
	if((value) == (srcData))\
	{\
		SAMPLE_PRT(__VA_ARGS__);\
		return ;\
	}\
}while(0)

#define RK_CHECK_NET_RET_PRT(srcData,value,ret, ...)\
do{\
	if((value) != (srcData))\
	{\
		SAMPLE_PRT(__VA_ARGS__);\
		return (ret);\
	}\
}while(0)

#define RK_CHECK_NET_RET_VOID_PRT(srcData,value, ...)\
do{\
	if((value) != (srcData))\
	{\
		SAMPLE_PRT(__VA_ARGS__);\
		return ;\
	}\
}while(0)

#define RK_CHECK_GT_RET_PRT(srcData,value,ret, ...)\
do{\
	if((srcData) > (value))\
	{\
		SAMPLE_PRT(__VA_ARGS__);\
		return (ret);\
	}\
}while(0)


#define RK_CHECK_GT_AND_ET_RET_PRT(srcData,value,ret, ...)\
do{\
	if((srcData) >= (value))\
	{\
		SAMPLE_PRT(__VA_ARGS__);\
		return (ret);\
	}\
}while(0)

#define RK_CHECK_LT_RET_PRT(srcData,value,ret, ...)\
do{\
	if((srcData) < (value))\
	{\
		SAMPLE_PRT(__VA_ARGS__);\
		return (ret);\
	}\
}while(0)

#define RK_CHECK_LT_AND_ET_RET_PRT(srcData,value,ret, ...)\
do{\
	if((srcData) <= (value))\
	{\
		SAMPLE_PRT(__VA_ARGS__);\
		return (ret);\
	}\
}while(0)

#define RK_CHECK_NET_GOTO_PRT(srcData,value,label, ...)\
do{\
	if((value) != (srcData))\
	{\
		SAMPLE_PRT(__VA_ARGS__);\
		goto label;\
	}\
}while(0)

#define RK_CHECK_ET_GOTO_PRT(srcData,value,label, ...)\
do{\
	if((value) == (srcData))\
	{\
		SAMPLE_PRT(__VA_ARGS__);\
		goto label;\
	}\
}while(0)

#define RK_CHECK_ET_NULL_RET_VOID_PRT(ptr, ...) RK_CHECK_ET_RET_VOID_PRT(ptr,NULL,__VA_ARGS__)
#define RK_CHECK_ET_NULL_RET_PRT(ptr,ret, ...) RK_CHECK_ET_RET_PRT(ptr,NULL,ret,__VA_ARGS__)

#define RK_FREE(p)\
do{\
	if (NULL != (p))\
	{\
		free((p));\
		(p) = NULL;\
	}\
} while (0)

#define RK_FCLOSE(fp)\
do{\
	if (NULL != (fp))\
	{\
		fclose((fp));\
		(fp) = NULL;\
	}\
} while (0)

#define RK_FREE(p)\
do{\
	if (NULL != (p))\
	{\
		free((p));\
		(p) = NULL;\
	}\
} while (0)

#define RK_FREE_64(p)\
do{\
	if (0 != (p))\
	{\
		free(((RK_VOID*)p));\
		(p) = 0;\
	}\
} while (0)

#define RK_ERROR(err)                           \
do                                              \
{                                               \
    printf("\n%s, at:>File name: %s"            \
    " >Function : %s >Line No. : %d\n",         \
    err, __FILE__,__FUNCTION__,                 \
    __LINE__);                                  \
    _exit(-1);                                  \
}while(0)


#define RK_CHECK_CONDITION_RET(expr, name, ret) \
do                                              \
{                                               \
    if (expr)                                   \
    {                                           \
        printf("%s, at %s: LINE: %d !\n",       \
        name, __FUNCTION__, __LINE__);          \
        return (ret);                           \
    }                                           \
}while(0)

#define RK_CHECK_CONDITION_RET_VOID(expr, name) \
do                                              \
{                                               \
    if (expr)                                   \
    {                                           \
        printf("%s, at %s: LINE: %d !\n",       \
        name, __FUNCTION__, __LINE__);          \
        return;                                 \
    }                                           \
}while(0)

#define RK_CLIP(a, maxv, minv)		 (((a)>(maxv)) ? (maxv) : (((a) < (minv)) ? (minv) : (a))) 

//stride align byte
#define RK_IVE2_STRIDE_ALIGN 16
//max file length
#define RK_MAX_FILE_LEN  256
//image width 704
#define RK_IMAGE_WIDTH_704  704
//image height 576
#define RK_IMAGE_HEIGHT_576 576
//image width 352
#define RK_IMAGE_WIDTH_352 352
//image height 288
#define RK_IMAGE_HEIGHT_288 288
//print info len
#define RK_INFO_LEN   100

//U8/S8/U16/S16 max and  min value define
#define RK_MAX_U8 255
#define RK_MIN_U8 0
#define RK_MAX_S8 127
#define RK_MIN_S8 -128
#define RK_MAX_U16 65535
#define RK_MIN_U16 0
#define RK_MAX_S16 32767
#define RK_MIN_S16 -32768

#endif