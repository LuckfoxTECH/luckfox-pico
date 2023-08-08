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
#ifndef _RK_IVS_MD_H_
#define _RK_IVS_MD_H_

#include "rk_md.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/*****************************************************************************
*   Prototype    : RK_IVS_MD_Init
*   Description  : Motion Detection(MD) initialization.
*   Parameters   : RK_VOID.
*
*   Return Value : RK_SUCCESS: Success;Error codes: Failure.
*****************************************************************************/
RK_S32 RK_IVS_MD_Init(RK_VOID);

/*****************************************************************************
*   Prototype    : RK_IVS_MD_Exit
*   Description  : Motion Detection(MD) exit.
*   Parameters   : RK_VOID.
*
*   Return Value : RK_SUCCESS: Success;Error codes: Failure.
*****************************************************************************/
RK_S32 RK_IVS_MD_Exit(RK_VOID);

/*****************************************************************************
*   Prototype    : RK_IVS_MD_CreateChn
*   Description  : Create Motion Detection(MD) Chn.
*   Parameters   : MD_CHN			MdChn		Md chn.
*                  MD_ATTR_S		*pstMdAttr	Md attribute parameters
*   Return Value : RK_SUCCESS: Success;Error codes: Failure.
*****************************************************************************/
RK_S32 RK_IVS_MD_CreateChn(MD_CHN MdChn, MD_ATTR_S *pstMdAttr);

/*****************************************************************************
*   Prototype    : RK_IVS_MD_DestroyChn
*   Description  : Destroy Motion Detection(MD) chn.
*   Parameters   : MD_CHN		MdChn   Md chn that would be destroy.
*
*   Return Value : RK_SUCCESS: Success;Error codes: Failure.
*****************************************************************************/
RK_S32 RK_IVS_MD_DestroyChn(MD_CHN MdChn);

/*****************************************************************************
*   Prototype    : RK_IVS_MD_SetChnAttr
*   Description  : Set Motion Detection(MD) chn attribute.
*   Parameters   : MD_CHN			MdChn		Md chn.
*                  MD_ATTR_S		*pstMdAttr	Md attribute parameters
*   Return Value : RK_SUCCESS: Success;Error codes: Failure.
*****************************************************************************/
RK_S32 RK_IVS_MD_SetChnAttr(MD_CHN MdChn, MD_ATTR_S *pstMdAttr);

/*****************************************************************************
*   Prototype    : RK_IVS_MD_GetChnAttr
*   Description  : Get Motion Detection(MD) chn attribute.
*   Parameters   : MD_CHN			MdChn		Md chn.
*                  MD_ATTR_S		*pstMdAttr	Md attribute parameters
*   Return Value : RK_SUCCESS: Success;Error codes: Failure.
*****************************************************************************/
RK_S32 RK_IVS_MD_GetChnAttr(MD_CHN MdChn, MD_ATTR_S *pstMdAttr);

/*****************************************************************************
*   Prototype    : RK_IVS_MD_GetBg
*   Description  : Get Motion Detection(MD) background image.
*   Parameters   : MD_CHN				MdChn		Md chn.
*                  IVE_DST_IMAGE_S	*pstBg		Output background image
*   Return Value : RK_SUCCESS: Success;Error codes: Failure.
*****************************************************************************/
RK_S32 RK_IVS_MD_GetBg(MD_CHN MdChn, IVE_DST_IMAGE_S *pstBg);

/*****************************************************************************
*   Prototype    : RK_IVS_MD_Process
*   Description  : Motion Detection(MD) process.
*   Parameters   : MD_CHN					MdChn	    Md chn.
*                  IVE_SRC_IMAGE_S		*pstCur	    Current image
* 				    IVE_SRC_IMAGE_S		*pstRef    Reference image
* 				    IVE_DST_IMAGE_S      *pstSad    Output result of sad value
* 				    IVE_DST_MEM_INFO_S	*pstBlob   Output blob
*   Return Value : RK_SUCCESS: Success;Error codes: Failure.
*****************************************************************************/
RK_S32 RK_IVS_MD_Process(MD_CHN MdChn, IVE_SRC_IMAGE_S *pstCur, IVE_SRC_IMAGE_S *pstRef,
                         IVE_DST_IMAGE_S *pstSad, IVE_DST_MEM_INFO_S *pstBlob);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif /* _RK_IVS_MD_H_ */