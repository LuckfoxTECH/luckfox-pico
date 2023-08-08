/*
 *  Copyright (c) 2021 Rockchip Corporation
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

#ifndef __ATMO_UAPI_HEAD_H__
#define __ATMO_UAPI_HEAD_H__

#include "atmo_head.h"

typedef struct uapiTmoCurrCtlData_s
{
  // M4_ENUM_DESC("GlobalLumaMode", "GlobalLumaMode_t", "GLOBALLUMAMODE_ENVLV")
  GlobalLumaMode_t GlobalLumaMode;
  // M4_ENUM_DESC("Mode", "DetailsHighLightMode_t", "DETAILSHIGHLIGHTMODE_ENVLV")
  DetailsHighLightMode_t DetailsHighLightMode;
  // M4_ENUM_DESC("DetailsLowLightMode", "DetailsLowLightMode_t", "DETAILSLOWLIGHTMODE_ISO")
  DetailsLowLightMode_t DetailsLowLightMode;
  // M4_ENUM_DESC("GlobalTmoMode", "TmoTypeMode_t", "TMOTYPEMODE_DYNAMICRANGE")
  TmoTypeMode_t GlobalTmoMode;
  // M4_ENUM_DESC("LocalTMOMode", "TmoTypeMode_t", "TMOTYPEMODE_DYNAMICRANGE")
  TmoTypeMode_t LocalTMOMode;
  float Envlv;
  float ISO;
  float OEPdf;
  float FocusLuma;
  float DarkPdf;
  float DynamicRange;
} uapiTmoCurrCtlData_t;

#endif  /*__ATMO_UAPI_HEAD_H__*/
