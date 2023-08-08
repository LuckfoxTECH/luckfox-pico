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
#ifndef _HI_MD_H_
#define _HI_MD_H_

#include "rk_ive.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
/*
* Definition md algorithm mode
*/
typedef enum rkMD_ALG_MODE_E {
    MD_ALG_MODE_BG = 0x0,  /* Base on background image */
    MD_ALG_MODE_REF = 0x1, /* Base on reference image */

    MD_ALG_MODE_BUTT
} MD_ALG_MODE_E;
/*
* Definition of md attribute
*/
typedef struct rkMD_ATTR_S {
    MD_ALG_MODE_E enAlgMode;         /* Md algorithm mode */
    IVE_SAD_MODE_E enSadMode;        /* Sad mode */
    IVE_SAD_OUT_CTRL_E enSadOutCtrl; /* Sad output ctrl */
    RK_U32 u32Width;                 /* Image width */
    RK_U32 u32Height;                /* Image height */
    RK_U16 u16SadThr;                /* Sad thresh */
    IVE_CCL_CTRL_S stCclCtrl;        /* Ccl ctrl */
    IVE_ADD_CTRL_S stAddCtrl;        /* Add ctrl */
} MD_ATTR_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* _HI_MD_H_ */