/*
 * Copyright (c) 2022 Rockchip, Inc. All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef __RKADK_PLATFORM_H__
#define __RKADK_PLATFORM_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef RV1106_1103
// rkispp_m_bypass rkispp_scale0 rkispp_scale1 rkispp_scale2
#define RKADK_ISPP_VI_NODE_CNT 4

/* sensor default parameters */
#define SENSOR_MAX_WIDTH 2688
#define SENSOR_MAX_HEIGHT 1520

//default vi device name
#define DEF_DEVICE_NEME_VI_0 "rkispp_m_bypass"
#define DEF_DEVICE_NEME_VI_1 "rkispp_scale0"
#define DEF_DEVICE_NEME_VI_2 "rkispp_scale1"
#define DEF_DEVICE_NEME_VI_3 "rkispp_scale2"

// default vi module
#define DEF_MODULE_VI_0 "RECORD_MAIN|PHOTO"
#define DEF_MODULE_VI_1 "RECORD_MAIN|PHOTO"
#define DEF_MODULE_VI_2 "THUMB"
#define DEF_MODULE_VI_3 "RECORD_SUB|PREVIEW|LIVE|DISP"

//default vi resolution
#define DEF_WIDTH_VI_1 1920
#define DEF_HEIGHT_VI_1 1080

#define DEF_WIDTH_VI_2 480
#define DEF_HEIGHT_VI_2 272

#else
// rkisp_mainpath rkisp_selfpath rkisp_bypasspath rkisp_mainpath_4x4sampling
#define RKADK_ISPP_VI_NODE_CNT 3

/* sensor default parameters */
#define SENSOR_MAX_WIDTH 2880
#define SENSOR_MAX_HEIGHT 1620

//default vi device name
#define DEF_DEVICE_NEME_VI_0 "rkisp_mainpath"
#define DEF_DEVICE_NEME_VI_1 "rkisp_selfpath"
#define DEF_DEVICE_NEME_VI_2 "rkisp_bypasspath"
#define DEF_DEVICE_NEME_VI_3 "rkisp_mainpath_4x4sampling"

// default vi module
#define DEF_MODULE_VI_0 "RECORD_MAIN|PHOTO"
#define DEF_MODULE_VI_1 "THUMB"
#define DEF_MODULE_VI_2 "RECORD_SUB|PREVIEW|LIVE|DISP"
#define DEF_MODULE_VI_3 "NONE"

//default vi resolution
#define DEF_WIDTH_VI_1 320
#define DEF_HEIGHT_VI_1 180

#define DEF_WIDTH_VI_2 1280
#define DEF_HEIGHT_VI_2 720

#endif

#ifdef __cplusplus
}
#endif
#endif
