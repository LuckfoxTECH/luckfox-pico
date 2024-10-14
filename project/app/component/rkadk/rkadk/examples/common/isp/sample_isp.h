/*
 * Copyright (c) 2021 Rockchip, Inc. All Rights Reserved.
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

#ifndef __SAMPLE_ISP_H__
#define __SAMPLE_ISP_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef RKAIQ
#include "rkadk_common.h"

#ifdef RKAIQ_UAPI2
#include <rk_aiq_user_api2_imgproc.h>
#include <rk_aiq_user_api2_sysctl.h>
#else
#include <rk_aiq_user_api_imgproc.h>
#include <rk_aiq_user_api_sysctl.h>
#endif

typedef struct {
  const char *iqFileDir;
  rk_aiq_working_mode_t WDRMode;
  bool bMultiCam;
  int fps;

#ifdef ENABLE_EIS
  rk_aiq_mems_sensor_intf_t *pEisApi;
#endif
} SAMPLE_ISP_PARAM;

int SAMPLE_ISP_Start(RKADK_U32 u32CamId, SAMPLE_ISP_PARAM stIspParam);
int SAMPLE_ISP_Stop(RKADK_U32 u32CamId);
int SAMPLE_ISP_UpdateIq(RKADK_U32 u32CamId, char *iqfile);
int SAMPLE_ISP_SET_FecEn(RKADK_U32 u32CamId, bool bFECEnable);
int SAMPLE_ISP_GET_FecAttrib(RKADK_U32 u32CamId, rk_aiq_fec_attrib_t *attr);
int SAMPLE_ISP_GET_AeExpResInfo(RKADK_U32 u32CamId,
                                  Uapi_ExpQueryInfo_t *pstExpInfo);
int SAMPLE_ISP_GET_CCT(RKADK_U32 u32CamId, rk_aiq_wb_cct_t *pstCCT);
int SAMPLE_ISP_QueryWBInfo(RKADK_U32 u32CamId,
                             rk_aiq_wb_querry_info_t *pstWbInfo);
int SAMPLE_ISP_DumpExpInfo(RKADK_U32 u32CamId, rk_aiq_working_mode_t WDRMode);
int SAMPLE_ISP_SET_FrameRate(RKADK_U32 u32CamId, unsigned int uFps);
int SAMPLE_ISP_EnableLdch(RKADK_U32 u32CamId, bool on, unsigned int level);
int SAMPLE_ISP_GET_LdchAttrib(RKADK_U32 u32CamId, rk_aiq_ldch_attrib_t *attr);
int SAMPLE_ISP_SET_WDRModeDyn(RKADK_U32 u32CamId,
                                rk_aiq_working_mode_t WDRMode);
int SAMPLE_ISP_SET_Brightness(RKADK_U32 u32CamId, unsigned int value);
int SAMPLE_ISP_SET_Contrast(RKADK_U32 u32CamId, unsigned int value);
int SAMPLE_ISP_SET_Saturation(RKADK_U32 u32CamId, unsigned int value);
int SAMPLE_ISP_SET_Sharpness(RKADK_U32 u32CamId, unsigned int value);
int SAMPLE_ISP_SET_ManualExposureAutoGain(RKADK_U32 u32CamId,
                                            unsigned int u32Shutter);
int SAMPLE_ISP_SET_ManualExposureManualGain(RKADK_U32 u32CamId,
                                              unsigned int u32Shutter,
                                              unsigned int u32Gain);
int SAMPLE_ISP_SET_AutoExposure(RKADK_U32 u32CamId);
int SAMPLE_ISP_SET_Exposure(RKADK_U32 u32CamId, bool bIsAutoExposure,
                              unsigned int bIsAGC,
                              unsigned int u32ElectronicShutter,
                              unsigned int u32Agc);
int SAMPLE_ISP_SET_BackLight(RKADK_U32 u32CamId, bool bEnable,
                               unsigned int u32Strength);
int SAMPLE_ISP_SET_DarkAreaBoostStrth(RKADK_U32 u32CamId, unsigned int level);
int SAMPLE_ISP_GET_DarkAreaBoostStrth(RKADK_U32 u32CamId,
                                        unsigned int *level);
int SAMPLE_ISP_SET_LightInhibition(RKADK_U32 u32CamId, bool bEnable,
                                     unsigned char u8Strength,
                                     unsigned char u8Level);
int SAMPLE_ISP_SET_CpslCfg(RKADK_U32 u32CamId, rk_aiq_cpsl_cfg_t *cpsl);
int SAMPLE_ISP_SET_OpenColorCloseLed(RKADK_U32 u32CamId);
int SAMPLE_ISP_SET_GrayOpenLed(RKADK_U32 u32CamId, unsigned char u8Strength);
int SAMPLE_ISP_SET_AutoWhiteBalance(RKADK_U32 u32CamId);
int SAMPLE_ISP_SET_ManualWhiteBalance(RKADK_U32 u32CamId,
                                        unsigned int u32RGain,
                                        unsigned int u32GGain,
                                        unsigned int u32BGain);
int SAMPLE_ISP_GET_WhiteBalanceGain(RKADK_U32 u32CamId,
                                      rk_aiq_wb_gain_t *gain);
int SAMPLE_ISP_SET_DNRStrength(RKADK_U32 u32CamId, unsigned int u32Mode,
                                 unsigned int u322DValue,
                                 unsigned int u323Dvalue);
int SAMPLE_ISP_GET_DNRStrength(RKADK_U32 u32CamId, unsigned int *u322DValue,
                                 unsigned int *u323Dvalue);

int SAMPLE_ISP_SET_Flicker(RKADK_U32 u32CamId, unsigned char u32Flicker);
int SAMPLE_ISP_EnableDefog(RKADK_U32 u32CamId, bool on, opMode_t mode,
                             unsigned int level);
int SAMPLE_ISP_GET_MDhzStrth(RKADK_U32 u32CamId, bool *on,
                               unsigned int *level);
int SAMPLE_ISP_SET_Correction(RKADK_U32 u32CamId, RKADK_S32 u32Mode,
                                unsigned int u32Value);
int SAMPLE_ISP_SET_MirrorFlip(RKADK_U32 u32CamId, bool mirror, bool flip);
int SAMPLE_ISP_GET_MirrorFlip(RKADK_U32 u32CamId, bool *mirror, bool *flip);

int SAMPLE_ISP_SET_BypassStreamRotation(RKADK_U32 u32CamId, int S32Rotation);
int SAMPLE_ISP_SET_Crop(RKADK_U32 u32CamId, rk_aiq_rect_t rect);

int SAMPLE_ISP_Get_AiqHandle(RKADK_U32 u32CamId, RKADK_MW_PTR *ppAiqCtx);
void SAMPLE_ISP_SingleFrame(RKADK_U32 u32CamId);
void SAMPLE_ISP_MultiFrame(RKADK_U32 u32CamId);
int SAMPLE_ISP_GetAINrParams(RKADK_U32 u32CamId, rk_ainr_param *param);
int SAMPLE_ISP_RegMemsSensorIntf(RKADK_U32 u32CamId, rk_aiq_mems_sensor_intf_t *api);

#endif

#ifdef __cplusplus
}
#endif
#endif
