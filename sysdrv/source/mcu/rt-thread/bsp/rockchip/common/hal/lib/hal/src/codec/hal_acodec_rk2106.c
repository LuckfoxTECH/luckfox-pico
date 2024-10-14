/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#if defined(HAL_ACODEC_MODULE_ENABLED) && defined(RKMCU_RK2106)

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup ACODEC
 *  @{
 */

/** @defgroup ACODEC_How_To_Use How To Use
 *  @{

 The ACODEC driver can be used as follows:

 - Invoke HAL_ACODEC_Init() to initialize acodec.
 - Invoke HAL_ACODEC_Config() to config samplerate, width.
 - Invoke HAL_ACODEC_Enable() to enable acodec.
 - Invoke HAL_ACODEC_Disable() to disable acodec.
 - Invoke HAL_ACODEC_DeInit() to deinitialize acodec.
 - More details refer to APIs' descriptions as below.

 @} */

/** @defgroup ACODEC_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/

/* VCTL */
#define ACODEC_ADCBYPS_DISABLE     (0x0U << ACODEC_ADCBYPS_SHIFT)
#define ACODEC_ADCBYPS_ENABLE      (0x1U << ACODEC_ADCBYPS_SHIFT)
#define ACODEC_DACBYPS_DISABLE     (0x0U << ACODEC_DACBYPS_SHIFT)
#define ACODEC_DACBYPS_ENABLE      (0x1U << ACODEC_DACBYPS_SHIFT)
#define ACODEC_ADCFADE_IMMEDIATELY (0x0U << ACODEC_ADCFADE_SHIFT)
#define ACODEC_ADCFADE_AS_ADCCZDT  (0x1U << ACODEC_ADCFADE_SHIFT)
#define ACODEC_DACFADE_IMMEDIATELY (0x0U << ACODEC_DACFADE_SHIFT)
#define ACODEC_DACFADE_AS_DACCZDT  (0x1U << ACODEC_DACFADE_SHIFT)
#define ACODEC_ADCCZDT_0           (0x0U << ACODEC_ADCCZDT_SHIFT)
#define ACODEC_ADCCZDT_1           (0x1U << ACODEC_ADCCZDT_SHIFT)
#define ACODEC_DACCZDT_0           (0x0U << ACODEC_DACCZDT_SHIFT)
#define ACODEC_DACCZDT_1           (0x1U << ACODEC_DACCZDT_SHIFT)
/* LPST */
#define ACODEC_LPDET_NO_VAILD (0x0U << ACODEC_LPDET_SHIFT)
#define ACODEC_LPDET_VAILD    (0x1U << ACODEC_LPDET_SHIFT)
/* SRST */
#define ACODEC_SOFT_RESET (0x1U << ACODEC_SOFT_SHIFT)
/* DIGEN */
#define ACODEC_ADC_EN_DISABLE   (0x0U << ACODEC_ADC_EN_SHIFT)
#define ACODEC_ADC_EN_ENABLE    (0x1U << ACODEC_ADC_EN_SHIFT)
#define ACODEC_I2STX_EN_DISABLE (0x0U << ACODEC_I2STX_EN_SHIFT)
#define ACODEC_I2STX_EN_ENABLE  (0x1U << ACODEC_I2STX_EN_SHIFT)
#define ACODEC_DAC_EN_DISABLE   (0x0U << ACODEC_DAC_EN_SHIFT)
#define ACODEC_DAC_EN_ENABLE    (0x1U << ACODEC_DAC_EN_SHIFT)
#define ACODEC_I2SRX_EN_DISABLE (0x0U << ACODEC_I2SRX_EN_SHIFT)
#define ACODEC_I2SRX_EN_ENABLE  (0x1U << ACODEC_I2SRX_EN_SHIFT)
/* CLKE */
#define ACODEC_ADC_CKE_DISABLE   (0x0U << ACODEC_ADC_CKE_SHIFT)
#define ACODEC_ADC_CKE_ENABLE    (0x1U << ACODEC_ADC_CKE_SHIFT)
#define ACODEC_I2STX_CKE_DISABLE (0x0U << ACODEC_I2STX_CKE_SHIFT)
#define ACODEC_I2STX_CKE_ENABLE  (0x1U << ACODEC_I2STX_CKE_SHIFT)
#define ACODEC_DAC_CKE_DISABLE   (0x0U << ACODEC_DAC_CKE_SHIFT)
#define ACODEC_DAC_CKE_ENABLE    (0x1U << ACODEC_DAC_CKE_SHIFT)
#define ACODEC_I2SRX_CKE_DISABLE (0x0U << ACODEC_I2SRX_CKE_SHIFT)
#define ACODEC_I2SRX_CKE_ENABLE  (0x1U << ACODEC_I2SRX_CKE_SHIFT)
/* RTCFG0 */
#define ACODEC_HALF_ADC_BUF_DISABLE (0x0U << ACODEC_HALF_ADC_BUF_SHIFT)
#define ACODEC_HALF_ADC_BUF_ENABLE  (0x1U << ACODEC_HALF_ADC_BUF_SHIFT)
#define ACODEC_HALF_VAG_BUF_DISABLE (0x0U << ACODEC_HALF_VAG_BUF_SHIFT)
#define ACODEC_HALF_VAG_BUF_ENABLE  (0x1U << ACODEC_HALF_VAG_BUF_SHIFT)
#define ACODEC_IBIAS_ANA_SEL_100    (0x0U << ACODEC_IBIAS_ANA_SEL_SHIFT)
#define ACODEC_IBIAS_ANA_SEL_80     (0x1U << ACODEC_IBIAS_ANA_SEL_SHIFT)
#define ACODEC_IBIAS_ANA_SEL_120    (0x2U << ACODEC_IBIAS_ANA_SEL_SHIFT)
#define ACODEC_IBIAS_ANA_SEL_140    (0x3U << ACODEC_IBIAS_ANA_SEL_SHIFT)
#define ACODEC_IBIAS_BIT(x)         ((x) << ACODEC_IBIAS_BIT_SHIFT)
/* RTCFG1 */
#define ACODEC_BOOST_ADC_BUF_DISABLE (0x0U << ACODEC_BOOST_ADC_BUF_SHIFT)
#define ACODEC_BOOST_ADC_BUF_ENABLE  (0x1U << ACODEC_BOOST_ADC_BUF_SHIFT)
#define ACODEC_BOOST_VAG_BUF_DISABLE (0x0U << ACODEC_BOOST_VAG_BUF_SHIFT)
#define ACODEC_BOOST_VAG_BUF_ENABLE  (0x1U << ACODEC_BOOST_VAG_BUF_SHIFT)
#define ACODEC_REF_ADC_SEL_12V       (0x0U << ACODEC_REF_ADC_SEL_SHIFT)
#define ACODEC_REF_ADC_SEL_15V       (0x1U << ACODEC_REF_ADC_SEL_SHIFT)
#define ACODEC_VBG_TRIM(x)           ((x) << ACODEC_VBG_TRIM_SHIFT)
/* RTCFG2 */
#define ACODEC_IBIAS_PWD_ON     (0x0U << ACODEC_IBIAS_PWD_SHIFT)
#define ACODEC_IBIAS_PWD_DOWN   (0x1U << ACODEC_IBIAS_PWD_SHIFT)
#define ACODEC_VAG_BUF_PWD_ON   (0x0U << ACODEC_VAG_BUF_PWD_SHIFT)
#define ACODEC_VAG_BUF_PWD_DOWN (0x1U << ACODEC_VAG_BUF_PWD_SHIFT)
#define ACODEC_REF_PWD_ON       (0x0U << ACODEC_REF_PWD_SHIFT)
#define ACODEC_REF_PWD_DOWN     (0x1U << ACODEC_REF_PWD_SHIFT)
/* ADCCFG0 */
#define ACODEC_ADC_L_PWD_ON         (0x0U << ACODEC_ADC_L_PWD_SHIFT)
#define ACODEC_ADC_L_PWD_DOWN       (0x1U << ACODEC_ADC_L_PWD_SHIFT)
#define ACODEC_ADC_R_PWD_ON         (0x0U << ACODEC_ADC_R_PWD_SHIFT)
#define ACODEC_ADC_R_PWD_DOWN       (0x1U << ACODEC_ADC_R_PWD_SHIFT)
#define ACODEC_ADC_DEM_EN_DISABLE   (0x0U << ACODEC_ADC_DEM_EN_SHIFT)
#define ACODEC_ADC_DEM_EN_ENABLE    (0x1U << ACODEC_ADC_DEM_EN_SHIFT)
#define ACODEC_ADC_DITH_OFF_DISABLE (0x0U << ACODEC_ADC_DITH_OFF_SHIFT)
#define ACODEC_ADC_DITH_OFF_ENABLE  (0x1U << ACODEC_ADC_DITH_OFF_SHIFT)
/* ADCCFG1 */
#define ACODEC_ADC_MUTE_L_DISABLE        (0x0U << ACODEC_ADC_MUTE_L_SHIFT)
#define ACODEC_ADC_MUTE_L_ENABLE         (0x1U << ACODEC_ADC_MUTE_L_SHIFT)
#define ACODEC_ADC_MUTE_R_DISABLE        (0x0U << ACODEC_ADC_MUTE_R_SHIFT)
#define ACODEC_ADC_MUTE_R_ENABLE         (0x1U << ACODEC_ADC_MUTE_R_SHIFT)
#define ACODEC_ADC_ATTN_ALLIBIAS_DISABLE (0x0U << ACODEC_ADC_ATTN_ALLIBIAS_SHIFT)
#define ACODEC_ADC_ATTN_ALLIBIAS_ENABLE  (0x1U << ACODEC_ADC_ATTN_ALLIBIAS_SHIFT)
#define ACODEC_ADC_ATTN_OPBIAS_DISABLE   (0x0U << ACODEC_ADC_ATTN_OPBIAS_SHIFT)
#define ACODEC_ADC_ATTN_OPBIAS_ENABLE    (0x1U << ACODEC_ADC_ATTN_OPBIAS_SHIFT)
#define ACODEC_ADC_DLY_INC_DISABLE       (0x0U << ACODEC_ADC_DLY_INC_SHIFT)
#define ACODEC_ADC_DLY_INC_ENABLE        (0x1U << ACODEC_ADC_DLY_INC_SHIFT)
#define ACODEC_ADC_OVERLAP_INC_DISABLE   (0x0U << ACODEC_ADC_OVERLAP_INC_SHIFT)
#define ACODEC_ADC_OVERLAP_INC_ENABLE    (0x1U << ACODEC_ADC_OVERLAP_INC_SHIFT)
#define ACODEC_ADC_BOOST_OPAMP_DISABLE   (0x0U << ACODEC_ADC_BOOST_OPAMP_SHIFT)
#define ACODEC_ADC_BOOST_OPAMP_ENABLE    (0x1U << ACODEC_ADC_BOOST_OPAMP_SHIFT)
#define ACODEC_ADC_BOOST_VAGOP_DISABLE   (0x0U << ACODEC_ADC_BOOST_VAGOP_SHIFT)
#define ACODEC_ADC_BOOST_VAGOP_ENABLE    (0x1U << ACODEC_ADC_BOOST_VAGOP_SHIFT)
/* ALC0 */
#define ACODEC_ALCL_DISABLE (0x0U << ACODEC_ALCL_SHIFT)
#define ACODEC_ALCL_ENABLE  (0x1U << ACODEC_ALCL_SHIFT)
#define ACODEC_ALCR_DISABLE (0x0U << ACODEC_ALCR_SHIFT)
#define ACODEC_ALCR_ENABLE  (0x1U << ACODEC_ALCR_SHIFT)
/* ALC2 */
#define ACODEC_ALCMAX_0dB   (0x0U << ACODEC_ALCMAX_SHIFT)
#define ACODEC_ALCMAX_F3dB  (0x1U << ACODEC_ALCMAX_SHIFT)
#define ACODEC_ALCMAX_F6dB  (0x2U << ACODEC_ALCMAX_SHIFT)
#define ACODEC_ALCMAX_F9dB  (0x3U << ACODEC_ALCMAX_SHIFT)
#define ACODEC_ALCMAX_F12dB (0x4U << ACODEC_ALCMAX_SHIFT)
#define ACODEC_ALCMAX_F18dB (0x5U << ACODEC_ALCMAX_SHIFT)
#define ACODEC_ALCMAX_F24dB (0x6U << ACODEC_ALCMAX_SHIFT)
#define ACODEC_ALCMAX_F30dB (0x7U << ACODEC_ALCMAX_SHIFT)
#define ACODEC_ALCMIN_0dB   (0x0U << ACODEC_ALCMIN_SHIFT)
#define ACODEC_ALCMIN_F3dB  (0x1U << ACODEC_ALCMIN_SHIFT)
#define ACODEC_ALCMIN_F6dB  (0x2U << ACODEC_ALCMIN_SHIFT)
#define ACODEC_ALCMIN_F9dB  (0x3U << ACODEC_ALCMIN_SHIFT)
#define ACODEC_ALCMIN_F12dB (0x4U << ACODEC_ALCMIN_SHIFT)
#define ACODEC_ALCMIN_F18dB (0x5U << ACODEC_ALCMIN_SHIFT)
#define ACODEC_ALCMIN_F24dB (0x6U << ACODEC_ALCMIN_SHIFT)
#define ACODEC_ALCMIN_F30dB (0x7U << ACODEC_ALCMIN_SHIFT)
/* ADCNG */
#define ACODEC_NGCHL_EITHER_CH (0x0U << ACODEC_NGCHL_EITHER_SHIFT)
#define ACODEC_NGCHL_BOTH_CH   (0x1U << ACODEC_NGCHL_EITHER_SHIFT)
#define ACODEC_NGEN_DISABLE    (0x0U << ACODEC_NGEN_SHIFT)
#define ACODEC_NGEN_ENABLE     (0x1U << ACODEC_NGEN_SHIFT)
#define ACODEC_NGBOOST_NORMAL  (0x0U << ACODEC_NGBOOST_SHIFT)
#define ACODEC_NGBOOST_BOOST   (0x1U << ACODEC_NGBOOST_SHIFT)
#define ACODEC_NGDLY_2048      (0x0U << ACODEC_NGDLY_SHIFT)
#define ACODEC_NGDLY_4096      (0x1U << ACODEC_NGDLY_SHIFT)
#define ACODEC_NGDLY_8192      (0x2U << ACODEC_NGDLY_SHIFT)
#define ACODEC_NGDLY_16384     (0x3U << ACODEC_NGDLY_SHIFT)
/* ADCVSTL */
#define ACODEC_HPFLE_DISABLE (0x0U << ACODEC_HPFLE_SHIFT)
#define ACODEC_HPFLE_ENABLE  (0x1U << ACODEC_HPFLE_SHIFT)
#define ACODEC_HPFRE_DISABLE (0x0U << ACODEC_HPFRE_SHIFT)
#define ACODEC_HPFRE_ENABLE  (0x1U << ACODEC_HPFRE_SHIFT)
#define ACODEC_HPF_CF_3_79Hz (0x0U << ACODEC_HPF_CF_SHIFT)
#define ACODEC_HPF_CF_60Hz   (0x1U << ACODEC_HPF_CF_SHIFT)
#define ACODEC_HPF_CF_243Hz  (0x2U << ACODEC_HPF_CF_SHIFT)
#define ACODEC_HPF_CF_493Hz  (0x3U << ACODEC_HPF_CF_SHIFT)
/* DACACFG0 */
#define ACODEC_DAC_AMP_INC2DB_DISABLE (0x0U << ACODEC_DAC_AMP_INC2DB_SHIFT)
#define ACODEC_DAC_AMP_INC2DB_ENABLE  (0x1U << ACODEC_DAC_AMP_INC2DB_SHIFT)
#define ACODEC_DAC_MAX_OUT_DISABLE    (0x0U << ACODEC_DAC_MAX_OUT_SHIFT)
#define ACODEC_DAC_MAX_OUT_ENABLE     (0x1U << ACODEC_DAC_MAX_OUT_SHIFT)
#define ACODEC_INC_DAC_RSTB_DISABLE   (0x0U << ACODEC_INC_DAC_RSTB_SHIFT)
#define ACODEC_INC_DAC_RSTB_ENABLE    (0x1U << ACODEC_INC_DAC_RSTB_SHIFT)
/* DACACFG1 */
#define ACODEC_INC_DAC_SWITCH_DISABLE (0x0U << ACODEC_INC_DAC_SWITCH_SHIFT)
#define ACODEC_INC_DAC_SWITCH_ENABLE  (0x1U << ACODEC_INC_DAC_SWITCH_SHIFT)
#define ACODEC_STOP_DAC_RSTB_DISABLE  (0x0U << ACODEC_STOP_DAC_RSTB_SHIFT)
#define ACODEC_STOP_DAC_RSTB_ENABLE   (0x1U << ACODEC_STOP_DAC_RSTB_SHIFT)
#define ACODEC_STOP_DAC_SW_DISABLE    (0x0U << ACODEC_STOP_DAC_SW_SHIFT)
#define ACODEC_STOP_DAC_SW_ENABLE     (0x1U << ACODEC_STOP_DAC_SW_SHIFT)
/* DACACFG2 */
#define ACODEC_PWD_DACBIAS_DOWN (0x1U << ACODEC_PWD_DACBIAS_SHIFT)
#define ACODEC_PWD_DACBIAS_ON   (0x0U << ACODEC_PWD_DACBIAS_SHIFT)
#define ACODEC_PWD_DACL_DOWN    (0x1U << ACODEC_PWD_DACL_SHIFT)
#define ACODEC_PWD_DACL_ON      (0x0U << ACODEC_PWD_DACL_SHIFT)
#define ACODEC_PWD_DACR_DOWN    (0x1U << ACODEC_PWD_DACR_SHIFT)
#define ACODEC_PWD_DACR_ON      (0x0U << ACODEC_PWD_DACR_SHIFT)
/* DACPOPD */
#define ACODEC_ATPCE_DISABLE   (0x0U << ACODEC_ATPCE_SHIFT)
#define ACODEC_ATPCE_ENABLE    (0x1U << ACODEC_ATPCE_SHIFT)
#define ACODEC_HPSEL_LINE2DAC  (0x0U << ACODEC_HPSEL_SHIFT)
#define ACODEC_HPSEL_HP2DAC    (0x1U << ACODEC_HPSEL_SHIFT)
#define ACODEC_SMTPO_DOWN      (0x0U << ACODEC_SMTPO_SHIFT)
#define ACODEC_SMTPO_ON        (0x1U << ACODEC_SMTPO_SHIFT)
#define ACODEC_ANTIPOP_DISABLE (0x0U << ACODEC_ANTIPOP_SHIFT)
#define ACODEC_ANTIPOP_ENABLE  (0x1U << ACODEC_ANTIPOP_SHIFT)
#define ACODEC_DACATPO_DOWN    (0x0U << ACODEC_DACATPO_SHIFT)
#define ACODEC_DACATPO_ON      (0x1U << ACODEC_DACATPO_SHIFT)
/* LMT0 */
#define ACODEC_LMTEN_DISABLE        (0x0U << ACODEC_LMTEN_SHIFT)
#define ACODEC_LMTEN_ENABLE         (0x1U << ACODEC_LMTEN_SHIFT)
#define ACODEC_LMTCHL_COMBINATIONAL (0x0U << ACODEC_LMTCHL_SHIFT)
#define ACODEC_LMTCHL_INDEPENTLY    (0x1U << ACODEC_LMTCHL_SHIFT)
/* DACMUTE */
#define ACODEC_DACMTE_DISABLE (0x0U << ACODEC_DACMTE_SHIFT)
#define ACODEC_DACMTE_ENABLE  (0x1U << ACODEC_DACMTE_SHIFT)
/* MIXCTRL */
#define ACODEC_MIXE_DISABLE (0x0U << ACODEC_MIXE_SHIFT)
#define ACODEC_MIXE_ENABLE  (0x1U << ACODEC_MIXE_SHIFT)
/* LICFG0 */
#define ACODEC_MIC_L_BOOST_0DB  (0x0U << ACODEC_MIC_L_BOOST_SHIFT)
#define ACODEC_MIC_L_BOOST_10DB (0x1U << ACODEC_MIC_L_BOOST_SHIFT)
#define ACODEC_MIC_L_BOOST_20DB (0x2U << ACODEC_MIC_L_BOOST_SHIFT)
#define ACODEC_MIC_L_BOOST_30DB (0x3U << ACODEC_MIC_L_BOOST_SHIFT)
#define ACODEC_MIC_L_BOOST(x)   ((x) << ACODEC_MIC_L_BOOST_SHIFT)
#define ACODEC_MIC_R_BOOST_0DB  (0x0U << ACODEC_MIC_R_BOOST_SHIFT)
#define ACODEC_MIC_R_BOOST_10DB (0x1U << ACODEC_MIC_R_BOOST_SHIFT)
#define ACODEC_MIC_R_BOOST_20DB (0x2U << ACODEC_MIC_R_BOOST_SHIFT)
#define ACODEC_MIC_R_BOOST_30DB (0x3U << ACODEC_MIC_R_BOOST_SHIFT)
#define ACODEC_MIC_R_BOOST(x)   ((x) << ACODEC_MIC_R_BOOST_SHIFT)
/* LICFG1 */
#define ACODEC_MIC_L_DIFF_EN_DISABLE (0x0U << ACODEC_MIC_L_DIFF_EN_SHIFT)
#define ACODEC_MIC_L_DIFF_EN_ENABLE  (0x1U << ACODEC_MIC_L_DIFF_EN_SHIFT)
#define ACODEC_MIC_R_DIFF_EN_DISABLE (0x0U << ACODEC_MIC_R_DIFF_EN_SHIFT)
#define ACODEC_MIC_R_DIFF_EN_ENABLE  (0x1U << ACODEC_MIC_R_DIFF_EN_SHIFT)
#define ACODEC_MUX_L_IN_SEL_LINE_1   (0x0U << ACODEC_MUX_L_IN_SEL_SHIFT)
#define ACODEC_MUX_L_IN_SEL_LINE_2   (0x1U << ACODEC_MUX_L_IN_SEL_SHIFT)
#define ACODEC_MUX_L_IN_SEL_MIC      (0x2U << ACODEC_MUX_L_IN_SEL_SHIFT)
#define ACODEC_MUX_R_IN_SEL_LINE_1   (0x0U << ACODEC_MUX_R_IN_SEL_SHIFT)
#define ACODEC_MUX_R_IN_SEL_LINE_2   (0x1U << ACODEC_MUX_R_IN_SEL_SHIFT)
#define ACODEC_MUX_R_IN_SEL_MIC      (0x2U << ACODEC_MUX_R_IN_SEL_SHIFT)
/* LICFG2 */
#define ACODEC_MUX_L_GAIN(x) ((x) << ACODEC_MUX_L_GAIN_SHIFT)
#define ACODEC_MUX_R_GAIN(x) ((x) << ACODEC_MUX_R_GAIN_SHIFT)
/* LICFG3 */
#define ACODEC_CHOP_SEL_200K       (0x0U << ACODEC_CHOP_SEL_SHIFT)
#define ACODEC_CHOP_SEL_400K       (0x1U << ACODEC_CHOP_SEL_SHIFT)
#define ACODEC_CHOP_SEL_800K       (0x2U << ACODEC_CHOP_SEL_SHIFT)
#define ACODEC_MIC_CHOP_EN_DISABLE (0x0U << ACODEC_MIC_CHOP_EN_SHIFT)
#define ACODEC_MIC_CHOP_EN_ENABLE  (0x1U << ACODEC_MIC_CHOP_EN_SHIFT)
#define ACODEC_MUX_CHOP_EN_DISABLE (0x0U << ACODEC_MUX_CHOP_EN_SHIFT)
#define ACODEC_MUX_CHOP_EN_ENABLE  (0x1U << ACODEC_MUX_CHOP_EN_SHIFT)
/* LICFG4 */
#define ACODEC_MIC_L_PD_DOWN (0x1U << ACODEC_MIC_L_PD_SHIFT)
#define ACODEC_MIC_L_PD_ON   (0x0U << ACODEC_MIC_L_PD_SHIFT)
#define ACODEC_MIC_R_PD_DOWN (0x1U << ACODEC_MIC_R_PD_SHIFT)
#define ACODEC_MIC_R_PD_ON   (0x0U << ACODEC_MIC_R_PD_SHIFT)
#define ACODEC_MUX_L_PD_DOWN (0x1U << ACODEC_MUX_L_PD_SHIFT)
#define ACODEC_MUX_L_PD_ON   (0x0U << ACODEC_MUX_L_PD_SHIFT)
#define ACODEC_MUX_R_PD_DOWN (0x1U << ACODEC_MUX_R_PD_SHIFT)
#define ACODEC_MUX_R_PD_ON   (0x0U << ACODEC_MUX_R_PD_SHIFT)
/* LILMT0 */
#define ACODEC_LMT_EN_DISABLE (0x0U << ACODEC_LMT_EN_SHIFT)
#define ACODEC_LMT_EN_ENABLE  (0x1U << ACODEC_LMT_EN_SHIFT)
/* ADCNGLMTCFG */
#define ACODEC_NGCHL_LI_INDIVIDUAL (0x0U << ACODEC_NGCHL_LI_SHIFT)
#define ACODEC_NGCHL_LI_BOTH       (0x1U << ACODEC_NGCHL_LI_SHIFT)
#define ACODEC_NGEN_LI_DISABLE     (0x0U << ACODEC_NGEN_LI_SHIFT)
#define ACODEC_NGEN_LI_ENABLE      (0x1U << ACODEC_NGEN_LI_SHIFT)
#define ACODEC_NGBOOST_LI_NORMAL   (0x0U << ACODEC_NGBOOST_LI_SHIFT)
#define ACODEC_NGBOOST_LI_BOOST    (0x1U << ACODEC_NGBOOST_LI_SHIFT)
#define ACODEC_NGDLY_LI_2048       (0x0U << ACODEC_NGDLY_LI_SHIFT)
#define ACODEC_NGDLY_LI_4096       (0x1U << ACODEC_NGDLY_LI_SHIFT)
#define ACODEC_NGDLY_LI_8192       (0x2U << ACODEC_NGDLY_LI_SHIFT)
#define ACODEC_NGDLY_LI_16384      (0x3U << ACODEC_NGDLY_LI_SHIFT)
/* ADCNGLMTST */
#define ACODEC_NGACT_LI_ACTIVE    (0x1U << ACODEC_NGACT_LI_SHIFT)
#define ACODEC_NGACT_LI_NO_ACTIVE (0x0U << ACODEC_NGACT_LI_SHIFT)
/* HPLOCFG0 */
#define ACODEC_INC_LO_AMP_0DB           (0x0U << ACODEC_INC_LO_AMP_SHIFT)
#define ACODEC_INC_LO_AMP_3DB           (0x1U << ACODEC_INC_LO_AMP_SHIFT)
#define ACODEC_INC_LO_AMP_6DB           (0x2U << ACODEC_INC_LO_AMP_SHIFT)
#define ACODEC_INC_LO_AMP_9DB           (0x3U << ACODEC_INC_LO_AMP_SHIFT)
#define ACODEC_INC_LO_AMP(x)            (((x) / 3) << ACODEC_INC_LO_AMP_SHIFT)
#define ACODEC_LO_VAG_RISE_SLOW_ENABLE  (0x1U << ACODEC_LO_VAG_RISE_SLOW_SHIFT)
#define ACODEC_LO_VAG_RISE_SLOW_DISABLE (0x0U << ACODEC_LO_VAG_RISE_SLOW_SHIFT)
#define ACODEC_LO_OUT_VAG_ENABLE        (0x1U << ACODEC_LO_OUT_VAG_SHIFT)
#define ACODEC_LO_OUT_VAG_DISABLE       (0x0U << ACODEC_LO_OUT_VAG_SHIFT)
/* HPLOCFG1 */
#define ACODEC_INC_HP_AMP_0DB             (0x0U << ACODEC_INC_HP_AMP_SHIFT)
#define ACODEC_INC_HP_AMP_3DB             (0x1U << ACODEC_INC_HP_AMP_SHIFT)
#define ACODEC_INC_HP_AMP_6DB             (0x2U << ACODEC_INC_HP_AMP_SHIFT)
#define ACODEC_INC_HP_AMP_9DB             (0x3U << ACODEC_INC_HP_AMP_SHIFT)
#define ACODEC_INC_HP_AMP(x)              (((x) / 3) << ACODEC_INC_HP_AMP_SHIFT)
#define ACODEC_HP_TWO_STAGE_ENABLE        (0x1U << ACODEC_HP_TWO_STAGE_SHIFT)
#define ACODEC_HP_TWO_STAGE_DISABLE       (0x0U << ACODEC_HP_TWO_STAGE_SHIFT)
#define ACODEC_INC_OC_RANGE_ENABLE        (0x1U << ACODEC_INC_OC_RANGE_SHIFT)
#define ACODEC_INC_OC_RANGE_DISABLE       (0x0U << ACODEC_INC_OC_RANGE_SHIFT)
#define ACODEC_HP_OPAMP_HALF_BIAS_ENABLE  (0x1U << ACODEC_HP_OPAMP_HALF_BIAS_SHIFT)
#define ACODEC_HP_OPAMP_HALF_BIAS_DISABLE (0x0U << ACODEC_HP_OPAMP_HALF_BIAS_SHIFT)
/* HPLOCFG2 */
#define ACODEC_HP_LN_OC_ENABLE      (0x1U << ACODEC_HP_LN_OC_SHIFT)
#define ACODEC_HP_LN_OC_DISABLE     (0x0U << ACODEC_HP_LN_OC_SHIFT)
#define ACODEC_HP_LP_OC_ENABLE      (0x1U << ACODEC_HP_LP_OC_SHIFT)
#define ACODEC_HP_LP_OC_DISABLE     (0x0U << ACODEC_HP_LP_OC_SHIFT)
#define ACODEC_HP_RN_OC_ENABLE      (0x1U << ACODEC_HP_RN_OC_SHIFT)
#define ACODEC_HP_RN_OC_DISABLE     (0x0U << ACODEC_HP_RN_OC_SHIFT)
#define ACODEC_HP_RP_OC_ENABLE      (0x1U << ACODEC_HP_RP_OC_SHIFT)
#define ACODEC_HP_RP_OC_DISABLE     (0x0U << ACODEC_HP_RP_OC_SHIFT)
#define ACODEC_HP_VGND_N_OC_ENABLE  (0x1U << ACODEC_HP_VGND_N_OC_SHIFT)
#define ACODEC_HP_VGND_N_OC_DISABLE (0x0U << ACODEC_HP_VGND_N_OC_SHIFT)
#define ACODEC_HP_VGND_P_OC_ENABLE  (0x1U << ACODEC_HP_VGND_P_OC_SHIFT)
#define ACODEC_HP_VGND_P_OC_DISABLE (0x0U << ACODEC_HP_VGND_P_OC_SHIFT)
/* HPLOCFG3 */
#define ACODEC_PWD_HP_OSTG_DOWN     (0x1U << ACODEC_PWD_HP_OSTG_SHIFT)
#define ACODEC_PWD_HP_OSTG_ON       (0x0U << ACODEC_PWD_HP_OSTG_SHIFT)
#define ACODEC_PWD_HP_VGND_DOWN     (0x1U << ACODEC_PWD_HP_VGND_SHIFT)
#define ACODEC_PWD_HP_VGND_ON       (0x0U << ACODEC_PWD_HP_VGND_SHIFT)
#define ACODEC_PWD_HP_BUF_DOWN      (0x1U << ACODEC_PWD_HP_BUF_SHIFT)
#define ACODEC_PWD_HP_BUF_ON        (0x0U << ACODEC_PWD_HP_BUF_SHIFT)
#define ACODEC_HP_SHORT_OUT_ENABLE  (0x1U << ACODEC_HP_SHORT_OUT_SHIFT)
#define ACODEC_HP_SHORT_OUT_DISABLE (0x0U << ACODEC_HP_SHORT_OUT_SHIFT)
/* HPLOCFG4 */
#define ACODEC_HP_ANTIPOP_ENABLE  (0x1U << ACODEC_HP_ANTIPOP_SHIFT)
#define ACODEC_HP_ANTIPOP_DISABLE (0x0U << ACODEC_HP_ANTIPOP_SHIFT)
#define ACODEC_HP_ANTIPOP_BIT(x)  (((x) - 1) << ACODEC_HP_ANTIPOP_BIT_SHIFT)
/* HPLOCFG5 */
#define ACODEC_PWD_LO_OSTG_ON     (0x0U << ACODEC_PWD_LO_OSTG_SHIFT)
#define ACODEC_PWD_LO_OSTG_DOWN   (0x1U << ACODEC_PWD_LO_OSTG_SHIFT)
#define ACODEC_PWD_LO_BUF_ON      (0x0U << ACODEC_PWD_LO_BUF_SHIFT)
#define ACODEC_PWD_LO_BUF_DOWN    (0x1U << ACODEC_PWD_LO_BUF_SHIFT)
#define ACODEC_LO_ANTIPOP_ENABLE  (0x1U << ACODEC_LO_ANTIPOP_SHIFT)
#define ACODEC_LO_ANTIPOP_DISABLE (0x0U << ACODEC_LO_ANTIPOP_SHIFT)
#define ACODEC_LO_ANTIPOP_BIT(x)  (((x) - 1) << ACODEC_LO_ANTIPOP_BIT_SHIFT)
/* PLLCFG0 */
#define ACODEC_PLL_CLKIN_SEL_M_CLOCK   (0x0U << ACODEC_PLL_CLKIN_SEL_SHIFT)
#define ACODEC_PLL_CLKIN_SEL_M2_CLOCK  (0x1U << ACODEC_PLL_CLKIN_SEL_SHIFT)
#define ACODEC_PLL_CLKIN_SEL_S_CLOCK   (0x2U << ACODEC_PLL_CLKIN_SEL_SHIFT)
#define ACODEC_PLL_CLKIN_SEL_I2S_CLOCK (0x3U << ACODEC_PLL_CLKIN_SEL_SHIFT)
#define ACODEC_PLL_OUTDIV_ENABLE       (0x1U << ACODEC_PLL_OUTDIV_EN_SHIFT)
#define ACODEC_PLL_OUTDIV_DISABLE      (0x0U << ACODEC_PLL_OUTDIV_EN_SHIFT)
/* PLLCFG4 */
#define ACODEC_PLL_OUTDIV(x) ((x) << ACODEC_PLL_OUTDIV_SHIFT)
/* PLLCFG5 */
#define ACODEC_PLL_RESET         (0x1U << ACODEC_PLL_RESET_SHIFT)
#define ACODEC_PLL_RELEASE_RESET (0x0U << ACODEC_PLL_RESET_SHIFT)
#define ACODEC_PLL_PW_DOWN       (0x1U << ACODEC_PLL_PW_SHIFT)
#define ACODEC_PLL_PW_UP         (0x0U << ACODEC_PLL_PW_SHIFT)
/* I2SCKM */
#define ACODEC_I2S_MCK_DIV(x) (((x) - 1) << ACODEC_I2S_MCK_DIV_SHIFT)
#define ACODEC_I2S_MST_MASTER (0x1U << ACODEC_I2S_MST_SHIFT)
#define ACODEC_I2S_MST_SLAVE  (0x0U << ACODEC_I2S_MST_SHIFT)
#define ACODEC_SCK_P_ENABLE   (0x1U << ACODEC_SCK_P_SHIFT)
#define ACODEC_SCK_P_DISABLE  (0x0U << ACODEC_SCK_P_SHIFT)
#define ACODEC_SCK_EN_ENABLE  (0x1U << ACODEC_SCK_EN_SHIFT)
#define ACODEC_SCK_EN_DISABLE (0x0U << ACODEC_SCK_EN_SHIFT)
/* I2SRXCR0 */
#define ACODEC_SCKD_RX_64_DIV  (0x0U << ACODEC_SCKD_RX_DIV_SHIFT)
#define ACODEC_SCKD_RX_128_DIV (0x1U << ACODEC_SCKD_RX_DIV_SHIFT)
#define ACODEC_SCKD_RX_256_DIV (0x2U << ACODEC_SCKD_RX_DIV_SHIFT)
#define ACODEC_RXRL_P_NORMAL   (0x0U << ACODEC_RXRL_P_SHIFT)
#define ACODEC_RXRL_P_INVERTED (0x1U << ACODEC_RXRL_P_SHIFT)
/* I2SRXCR1 */
#define ACODEC_TFS_RX_I2S_MODE        (0x0U << ACODEC_TFS_RX_SHIFT)
#define ACODEC_TFS_RX_PCM_MODE        (0x1U << ACODEC_TFS_RX_SHIFT)
#define ACODEC_PBM_RX_BUS_MODE_DELAY0 (0x0U << ACODEC_PBM_RX_BUS_MODE_SHIFT)
#define ACODEC_PBM_RX_BUS_MODE_DELAY1 (0x1U << ACODEC_PBM_RX_BUS_MODE_SHIFT)
#define ACODEC_PBM_RX_BUS_MODE_DELAY2 (0x2U << ACODEC_PBM_RX_BUS_MODE_SHIFT)
#define ACODEC_PBM_RX_BUS_MODE_DELAY3 (0x3U << ACODEC_PBM_RX_BUS_MODE_SHIFT)
#define ACODEC_IBM_RX_BUS_MODE_NORMAL (0x0U << ACODEC_IBM_RX_BUS_MODE_SHIFT)
#define ACODEC_IBM_RX_BUS_MODE_LEFT   (0x1U << ACODEC_IBM_RX_BUS_MODE_SHIFT)
#define ACODEC_IBM_RX_BUS_MODE_RIGHT  (0x2U << ACODEC_IBM_RX_BUS_MODE_SHIFT)
#define ACODEC_EXRL_RX_NORMAL         (0x0U << ACODEC_EXRL_RX_SHIFT)
#define ACODEC_EXRL_RX_EXCHANGE       (0x1U << ACODEC_EXRL_RX_SHIFT)
#define ACODEC_LSB_RX_LSB             (0x1U << ACODEC_LSB_RX_SHIFT)
#define ACODEC_LSB_RX_MSB             (0x0U << ACODEC_LSB_RX_SHIFT)
/* I2SRXCR2 */
#define ACODEC_VDW_RX_WIDTH_24BIT (0x17U << ACODEC_VDW_RX_WIDTH_SHIFT)
#define ACODEC_VDW_RX_WIDTH_16BIT (0xFU << ACODEC_VDW_RX_WIDTH_SHIFT)
#define ACODEC_VDW_RX_WIDTH(x)    (((x) - 1) << ACODEC_VDW_RX_WIDTH_SHIFT)
/* I2SRXCMD */
#define ACODEC_RX_CLEAR (0x1U << ACODEC_RXC_SHIFT)
#define ACODEC_RX_START (0x1U << ACODEC_RXS_SHIFT)
#define ACODEC_RX_STOP  (0x0U << ACODEC_RXS_SHIFT)
/* I2STXCR0 */
#define ACODEC_SCKD_TX_64_DIV  (0x0U << ACODEC_SCKD_TX_DIV_SHIFT)
#define ACODEC_SCKD_TX_128_DIV (0x1U << ACODEC_SCKD_TX_DIV_SHIFT)
#define ACODEC_SCKD_TX_256_DIV (0x2U << ACODEC_SCKD_TX_DIV_SHIFT)
#define ACODEC_TXRL_P_NORMAL   (0x0U << ACODEC_TXRL_P_SHIFT)
#define ACODEC_TXRL_P_INVERTED (0x1U << ACODEC_TXRL_P_SHIFT)
/* I2STXCR1 */
#define ACODEC_TFS_TX_I2S_MODE        (0x0U << ACODEC_TFS_TX_MODE_SHIFT)
#define ACODEC_TFS_TX_PCM_MODE        (0x1U << ACODEC_TFS_TX_MODE_SHIFT)
#define ACODEC_PBM_TX_BUS_MODE_DELAY0 (0x0U << ACODEC_PBM_TX_BUS_MODE_SHIFT)
#define ACODEC_PBM_TX_BUS_MODE_DELAY1 (0x1U << ACODEC_PBM_TX_BUS_MODE_SHIFT)
#define ACODEC_PBM_TX_BUS_MODE_DELAY2 (0x2U << ACODEC_PBM_TX_BUS_MODE_SHIFT)
#define ACODEC_IBM_TX_BUS_MODE_NORMAL (0x0U << ACODEC_IBM_TX_BUS_MODE_SHIFT)
#define ACODEC_IBM_TX_BUS_MODE_LEFT   (0x1U << ACODEC_IBM_TX_BUS_MODE_SHIFT)
#define ACODEC_IBM_TX_BUS_MODE_RIGHT  (0x2U << ACODEC_IBM_TX_BUS_MODE_SHIFT)
#define ACODEC_EXRL_TX_NORMAL         (0x0U << ACODEC_EXRL_TX_SHIFT)
#define ACODEC_EXRL_TX_EXCHANGE       (0x1U << ACODEC_EXRL_TX_SHIFT)
#define ACODEC_LSB_TX_LSB             (0x1U << ACODEC_LSB_TX_SHIFT)
#define ACODEC_LSB_TX_MSB             (0x0U << ACODEC_LSB_TX_SHIFT)
/* I2STXCR2 */
#define ACODEC_VDW_TX_WIDTH_24BIT (0x17U << ACODEC_VDW_TX_WIDTH_SHIFT)
#define ACODEC_VDW_TX_WIDTH_16BIT (0xFU << ACODEC_VDW_TX_WIDTH_SHIFT)
#define ACODEC_VDW_TX_WIDTH(x)    (((x) - 1) << ACODEC_VDW_TX_WIDTH_SHIFT)
/* I2STXCMD */
#define ACODEC_TX_CLEAR (0x1U << ACODEC_TXC_SHIFT)
#define ACODEC_TX_START (0x1U << ACODEC_TXS_SHIFT)
#define ACODEC_TX_STOP  (0x0U << ACODEC_TXS_SHIFT)
/* TMCFG0 */
#define ACODEC_BURN_IN_MODE_ENBALE   (0x1U << ACODEC_BURN_IN_MODE_SHIFT)
#define ACODEC_BURN_IN_MODE_DISBALE  (0x0U << ACODEC_BURN_IN_MODE_SHIFT)
#define ACODEC_ATE_TEST_MODE_ENBALE  (0x1U << ACODEC_ATE_TEST_MODE_SHIFT)
#define ACODEC_ATE_TEST_MODE_DISBALE (0x0U << ACODEC_ATE_TEST_MODE_SHIFT)

#define ACODEC_MCLK_24MHZ (24000000)
#define PLL_FREQ_40960    (40960000)
#define PLL_FREQ_56448    (56448000)
#define PLL_FREQ_61440    (61440000)

/********************* Private Structure Definition **************************/
struct ACODEC_PLL {
    uint32_t srcRate;
    uint32_t outRate;
    uint32_t postDivL3;
    uint32_t postDivH8;
    uint32_t preDivBit;
    uint32_t outDiv;
};

typedef enum {
    ACODEC_HP,
    ACODEC_LINE,
} eACODEC_outSel;

typedef enum {
    ACODEC_MIC1_MONO,
    ACODEC_MIC2_MONO,
    ACODEC_MIC_STEREO,
    ACODEC_LINE_IN1,
    ACODEC_LINE_IN2,
} eACODEC_inSel;

typedef enum {
    HEADPHONE,
    LINEOUT,
    LINEIN1,
    LINEIN1_TO_DAC,
    LINEIN2,
    LINEIN2_TO_DAC,
    MIC_STEREO,
    MIC1_MONO,
    MIC2_MONO,
} eACODEC_route;

/********************* Private Variable Definition ***************************/
static const struct ACODEC_PLL s_pllTable61440[] =
{
    { 2048000, 61440000, 0, 45, 2, 6 },
    { 3072000, 61440000, 0, 45, 3, 6 },
    { 4096000, 61440000, 0, 45, 4, 6 },
    { 6000000, 61440000, 0, 64, 10, 5 },
    { 6144000, 61440000, 0, 45, 6, 6 },
    { 12000000, 61440000, 0, 96, 25, 6 },
    { 12288000, 61440000, 0, 45, 12, 6 },
    { 24000000, 61440000, 0, 48, 25, 10 },
    { /* sentinel */ }
};

static const struct ACODEC_PLL s_pllTable56448[] =
{
    { 2048000, 56448000, 3, 41, 2, 6 },
    { 3072000, 56448000, 1, 55, 4, 6 },
    { 4096000, 56448000, 3, 41, 4, 6 },
    { 6000000, 56448000, 0, 147, 25, 5 },
    { 6144000, 56448000, 1, 55, 8, 6 },
    { 12000000, 56448000, 4, 73, 25, 5 },
    { 12288000, 56448000, 1, 55, 16, 6 },
    { 24000000, 56448000, 6, 36, 25, 3 },
    { /* sentinel */ }
};

static const struct ACODEC_PLL s_pllTable40960[] =
{
    { 2048000, 40960000, 4, 22, 1, 9 },
    { 3072000, 40960000, 0, 45, 3, 9 },
    { 4096000, 40960000, 4, 22, 2, 9 },
    { 6000000, 40960000, 0, 192, 25, 9 },
    { 6144000, 40960000, 0, 40, 6, 9 },
    { 12000000, 40960000, 0, 96, 25, 9 },
    { 12288000, 40960000, 0, 45, 12, 9 },
    { 24000000, 40960000, 0, 96, 50, 8 },
    { /* sentinel */ }
};

static uint32_t s_pllOutRate;

/********************* Private Function Definition ***************************/

/**
 * @brief  enable dac lineout
 * @param  acodec: the handle of acodec.
 * @return none
 */
HAL_UNUSED static void ACODEC_EnableLineOut(struct HAL_ACODEC_DEV *acodec)
{
    struct ACODEC_REG *reg = acodec->pReg;
    int i = 0;

    WRITE_REG(reg->DACMUTE, ACODEC_DACMTE_ENABLE);
    HAL_DelayMs(1);
    WRITE_REG(reg->DACACFG2,
              ACODEC_PWD_DACBIAS_ON | ACODEC_PWD_DACL_ON | ACODEC_PWD_DACR_ON);

    for (i = 16; i >= 1; i--) {
        MODIFY_REG(reg->HPLOCFG5,
                   ACODEC_LO_ANTIPOP_BIT_MASK | ACODEC_LO_ANTIPOP_MASK,
                   ACODEC_LO_ANTIPOP_BIT(i) | ACODEC_LO_ANTIPOP_ENABLE);
        HAL_DelayMs(1);
    }

    MODIFY_REG(reg->HPLOCFG5,
               ACODEC_LO_ANTIPOP_MASK, ACODEC_LO_ANTIPOP_DISABLE);
    WRITE_REG(reg->DACMUTE, ACODEC_DACMTE_DISABLE);
}

/**
 * @brief  disable dac lineout
 * @param  acodec: the handle of acodec.
 * @return none
 */
HAL_UNUSED static void ACODEC_DisableLineOut(struct HAL_ACODEC_DEV *acodec)
{
    struct ACODEC_REG *reg = acodec->pReg;
    uint32_t i = 0;

    WRITE_REG(reg->DACMUTE, ACODEC_DACMTE_ENABLE);

    for (i = 1; i <= 16; i++) {
        MODIFY_REG(reg->HPLOCFG5,
                   ACODEC_LO_ANTIPOP_BIT_MASK | ACODEC_LO_ANTIPOP_MASK,
                   ACODEC_LO_ANTIPOP_BIT(i) | ACODEC_LO_ANTIPOP_ENABLE);
        HAL_DelayMs(1);
    }

    WRITE_REG(reg->DACACFG2,
              ACODEC_PWD_DACBIAS_DOWN | ACODEC_PWD_DACL_ON | ACODEC_PWD_DACR_ON);
}

/**
 * @brief  enable dac hp
 * @param  acodec: the handle of acodec.
 * @return none
 */
HAL_UNUSED static void ACODEC_EnableHP(struct HAL_ACODEC_DEV *acodec)
{
    struct ACODEC_REG *reg = acodec->pReg;
    int i = 0;

    WRITE_REG(reg->DACACFG2,
              ACODEC_PWD_DACBIAS_ON | ACODEC_PWD_DACL_ON | ACODEC_PWD_DACR_ON);

    for (i = 16; i >= 1; i--) {
        MODIFY_REG(reg->HPLOCFG4,
                   ACODEC_HP_ANTIPOP_BIT_MASK | ACODEC_HP_ANTIPOP_MASK,
                   ACODEC_HP_ANTIPOP_BIT(i) | ACODEC_HP_ANTIPOP_ENABLE);
        HAL_DelayMs(5);
    }

    MODIFY_REG(reg->HPLOCFG4, ACODEC_HP_ANTIPOP_MASK, ACODEC_HP_ANTIPOP_DISABLE);
    WRITE_REG(reg->DACMUTE, ACODEC_DACMTE_DISABLE);
}

/**
 * @brief  disable dac hp
 * @param  acodec: the handle of acodec.
 * @return none
 */
HAL_UNUSED static void ACODEC_DisableHP(struct HAL_ACODEC_DEV *acodec)
{
    struct ACODEC_REG *reg = acodec->pReg;
    uint32_t i = 0;

    WRITE_REG(reg->DACMUTE, ACODEC_DACMTE_ENABLE);

    for (i = 1; i <= 16; i++) {
        MODIFY_REG(reg->HPLOCFG4,
                   ACODEC_HP_ANTIPOP_BIT_MASK | ACODEC_HP_ANTIPOP_MASK,
                   ACODEC_HP_ANTIPOP_BIT(i) | ACODEC_HP_ANTIPOP_ENABLE);
        HAL_DelayMs(1);
    }

    WRITE_REG(reg->DACACFG2,
              ACODEC_PWD_DACBIAS_DOWN | ACODEC_PWD_DACL_ON | ACODEC_PWD_DACR_ON);
}

/**
 * @brief  set dac mode
 * @param  acodec: the handle of acodec.
 * @param  mode: dac out mode.
 * @return none
 */
static void ACODEC_SetDacMode(struct HAL_ACODEC_DEV *acodec, eACODEC_outSel mode)
{
    struct ACODEC_REG *reg = acodec->pReg;

    WRITE_REG(reg->DACMUTE, ACODEC_DACMTE_ENABLE);
    MODIFY_REG(reg->VCTL,
               ACODEC_DACBYPS_MASK | ACODEC_DACFADE_MASK,
               ACODEC_DACBYPS_DISABLE | ACODEC_DACFADE_AS_DACCZDT);
    MODIFY_REG(reg->CLKE,
               ACODEC_DAC_CKE_MASK | ACODEC_I2SRX_CKE_MASK,
               ACODEC_DAC_CKE_ENABLE | ACODEC_I2SRX_CKE_ENABLE);

    MODIFY_REG(reg->DIGEN, ACODEC_I2SRX_EN_MASK, ACODEC_I2SRX_EN_ENABLE);
    HAL_DelayMs(1);
    MODIFY_REG(reg->DIGEN, ACODEC_DAC_EN_MASK, ACODEC_DAC_EN_ENABLE);
    HAL_DelayMs(1);

    MODIFY_REG(reg->HPLOCFG1,
               ACODEC_HP_OPAMP_HALF_BIAS_MASK,
               ACODEC_HP_OPAMP_HALF_BIAS_ENABLE);
    MODIFY_REG(reg->RTCFG0, ACODEC_IBIAS_BIT_MASK, ACODEC_IBIAS_BIT(0x1));
    HAL_DelayMs(1);
    MODIFY_REG(reg->RTCFG1, ACODEC_VBG_TRIM_MASK, ACODEC_VBG_TRIM(0x1));
    HAL_DelayMs(1);

    switch (mode) {
    case ACODEC_HP:
        ACODEC_EnableHP(acodec);
        break;
    case ACODEC_LINE:
        ACODEC_EnableLineOut(acodec);
        break;
    default:
        break;
    }
}

/**
 * @brief  set mic mode
 * @param  acodec: the handle of acodec.
 * @param  iMode: which mic.
 * @return none
 */
static void ACODEC_SetMicMode(struct HAL_ACODEC_DEV *acodec, eACODEC_inSel iMode)
{
    struct ACODEC_REG *reg = acodec->pReg;

    if (acodec->micDifferential) {
        MODIFY_REG(reg->LICFG1,
                   ACODEC_MIC_L_DIFF_EN_MASK | ACODEC_MIC_R_DIFF_EN_MASK,
                   ACODEC_MIC_L_DIFF_EN_ENABLE | ACODEC_MIC_R_DIFF_EN_ENABLE);
    }

    switch (iMode) {
    case ACODEC_MIC1_MONO:
        MODIFY_REG(reg->LICFG1,
                   ACODEC_MUX_L_IN_SEL_MASK, ACODEC_MUX_L_IN_SEL_MIC);
        WRITE_REG(reg->LICFG4, ACODEC_MIC_L_PD_ON | ACODEC_MUX_L_PD_ON);
        break;
    case ACODEC_MIC2_MONO:
        MODIFY_REG(reg->LICFG1,
                   ACODEC_MUX_R_IN_SEL_MASK, ACODEC_MUX_R_IN_SEL_MIC);
        WRITE_REG(reg->LICFG4, ACODEC_MIC_R_PD_ON | ACODEC_MUX_R_PD_ON);
        break;
    case ACODEC_MIC_STEREO:
        MODIFY_REG(reg->LICFG1,
                   ACODEC_MUX_L_IN_SEL_MASK | ACODEC_MUX_R_IN_SEL_MASK,
                   ACODEC_MUX_L_IN_SEL_MIC | ACODEC_MUX_R_IN_SEL_MIC);
        WRITE_REG(reg->LICFG4,
                  ACODEC_MIC_L_PD_ON | ACODEC_MIC_R_PD_ON |
                  ACODEC_MUX_L_PD_ON | ACODEC_MUX_R_PD_ON);
        break;
    default:
        break;
    }
}

/**
 * @brief  set mic bias
 * @param  acodec: the handle of acodec.
 * @param  bias: bias voltage.
 * @return none
 */
static void ACODEC_SetMicBias(struct HAL_ACODEC_DEV *acodec,
                              uint32_t bias)
{
    struct ACODEC_REG *reg = acodec->pReg;

    MODIFY_REG(reg->LICFG0, ACODEC_MICBIAS_MASK, bias);
}

/**
 * @brief  set linein
 * @param  acodec: the handle of acodec.
 * @return none
 */
static void ACODEC_SetLineIn(struct HAL_ACODEC_DEV *acodec,
                             eACODEC_inSel mode)
{
    struct ACODEC_REG *reg = acodec->pReg;

    switch (mode) {
    case ACODEC_LINE_IN1:
        WRITE_REG(reg->LICFG1,
                  ACODEC_MUX_L_IN_SEL_LINE_1 | ACODEC_MUX_R_IN_SEL_LINE_1);
        WRITE_REG(reg->LICFG4,
                  ACODEC_MUX_L_PD_ON | ACODEC_MUX_R_PD_ON |
                  ACODEC_MIC_L_PD_DOWN | ACODEC_MIC_R_PD_DOWN);
        break;
    case ACODEC_LINE_IN2:
        WRITE_REG(reg->LICFG1,
                  ACODEC_MUX_L_IN_SEL_LINE_2 | ACODEC_MUX_R_IN_SEL_LINE_2);
        WRITE_REG(reg->LICFG4,
                  ACODEC_MUX_L_PD_ON | ACODEC_MUX_R_PD_ON |
                  ACODEC_MIC_L_PD_DOWN | ACODEC_MIC_R_PD_DOWN);
        break;
    default:
        break;
    }
}

/**
 * @brief  set adc mode
 * @param  acodec: the handle of acodec.
 * @param  mode: adc mode.
 * @return none
 */
static void ACODEC_SetAdcMode(struct HAL_ACODEC_DEV *acodec,
                              eACODEC_inSel mode)
{
    struct ACODEC_REG *reg = acodec->pReg;

    MODIFY_REG(reg->ADCCFG1,
               ACODEC_ADC_MUTE_L_MASK | ACODEC_ADC_MUTE_R_MASK,
               ACODEC_ADC_MUTE_L_ENABLE | ACODEC_ADC_MUTE_R_ENABLE);

    switch (mode) {
    case ACODEC_MIC1_MONO:
    /* fallthrough */
    case ACODEC_MIC2_MONO:
    /* fallthrough */
    case ACODEC_MIC_STEREO:
        ACODEC_SetMicMode(acodec, mode);
        ACODEC_SetMicBias(acodec, acodec->micBias);
        break;
    case ACODEC_LINE_IN1:
    /* fallthrough */
    case ACODEC_LINE_IN2:
        ACODEC_SetLineIn(acodec, mode);
        break;
    default:
        break;
    }

    MODIFY_REG(reg->VCTL,
               ACODEC_ADCBYPS_MASK | ACODEC_ADCFADE_MASK,
               ACODEC_ADCBYPS_DISABLE | ACODEC_ADCFADE_AS_ADCCZDT);

    switch (mode) {
    case ACODEC_MIC1_MONO:
        MODIFY_REG(reg->ADCCFG0,
                   ACODEC_ADC_L_PWD_MASK | ACODEC_ADC_R_PWD_MASK,
                   ACODEC_ADC_L_PWD_ON | ACODEC_ADC_R_PWD_DOWN);
        break;
    case ACODEC_MIC2_MONO:
        MODIFY_REG(reg->ADCCFG0,
                   ACODEC_ADC_L_PWD_MASK | ACODEC_ADC_R_PWD_MASK,
                   ACODEC_ADC_L_PWD_DOWN | ACODEC_ADC_R_PWD_ON);
        break;
    default:
        MODIFY_REG(reg->ADCCFG0,
                   ACODEC_ADC_L_PWD_MASK | ACODEC_ADC_R_PWD_MASK,
                   ACODEC_ADC_L_PWD_ON | ACODEC_ADC_R_PWD_ON);
        break;
    }

    MODIFY_REG(reg->CLKE,
               ACODEC_ADC_CKE_MASK | ACODEC_I2STX_CKE_MASK,
               ACODEC_ADC_CKE_ENABLE | ACODEC_I2STX_CKE_ENABLE);
    MODIFY_REG(reg->DIGEN, ACODEC_I2STX_EN_MASK, ACODEC_I2STX_EN_MASK);
    MODIFY_REG(reg->DIGEN, ACODEC_ADC_EN_MASK, ACODEC_ADC_EN_ENABLE);

    HAL_DelayMs(1);

    switch (mode) {
    case ACODEC_MIC1_MONO:
        MODIFY_REG(reg->ADCCFG1,
                   ACODEC_ADC_MUTE_L_MASK | ACODEC_ADC_MUTE_R_MASK,
                   ACODEC_ADC_MUTE_L_DISABLE | ACODEC_ADC_MUTE_R_ENABLE);
        break;
    case ACODEC_MIC2_MONO:
        MODIFY_REG(reg->ADCCFG1,
                   ACODEC_ADC_MUTE_L_MASK | ACODEC_ADC_MUTE_R_MASK,
                   ACODEC_ADC_MUTE_L_ENABLE | ACODEC_ADC_MUTE_R_DISABLE);
        break;
    default:
        MODIFY_REG(reg->ADCCFG1,
                   ACODEC_ADC_MUTE_L_MASK | ACODEC_ADC_MUTE_R_MASK,
                   ACODEC_ADC_MUTE_L_DISABLE | ACODEC_ADC_MUTE_R_DISABLE);
        break;
    }
}

/**
 * @brief  set adc hpf
 * @param  acodec: the handle of acodec.
 * @return none
 */
static void ACODEC_SetAdcHpf(struct HAL_ACODEC_DEV *acodec)
{
    struct ACODEC_REG *reg = acodec->pReg;

    MODIFY_REG(reg->ADCHPF,
               ACODEC_HPFLE_MASK | ACODEC_HPFRE_MASK,
               ACODEC_HPFLE_ENABLE | ACODEC_HPFRE_ENABLE);
}

/**
 * @brief  set hp amplitude
 * @param  acodec: the handle of acodec.
 * @param  vol: volume.
 * @return none
 */
HAL_UNUSED static void ACODEC_SetHpAmplitude(struct HAL_ACODEC_DEV *acodec,
                                             uint8_t vol)
{
    struct ACODEC_REG *reg = acodec->pReg;

    HAL_ASSERT(vol <= 9);

    if (vol) {
        MODIFY_REG(reg->HPLOCFG4,
                   ACODEC_HP_ANTIPOP_MASK, ACODEC_HP_ANTIPOP_ENABLE);
    }

    MODIFY_REG(reg->HPLOCFG1,
               ACODEC_INC_HP_AMP_MASK, ACODEC_INC_HP_AMP(vol));
}

/**
 * @brief  set lineout amplitude
 * @param  acodec: the handle of acodec.
 * @param  vol: volume.
 * @return none
 */
HAL_UNUSED static void ACODEC_SetLineOutAmplitude(struct HAL_ACODEC_DEV *acodec,
                                                  uint8_t vol)
{
    struct ACODEC_REG *reg = acodec->pReg;

    HAL_ASSERT(vol <= 9);

    if (vol) {
        MODIFY_REG(reg->HPLOCFG4,
                   ACODEC_LO_ANTIPOP_MASK, ACODEC_LO_ANTIPOP_ENABLE);
    }

    MODIFY_REG(reg->HPLOCFG0,
               ACODEC_INC_LO_AMP_MASK, ACODEC_INC_LO_AMP(vol));
}

/**
 * @brief  set mic volume
 * @param  acodec: the handle of acodec.
 * @param  vol: volume.
 * @return none
 */
static void ACODEC_SetMicAnaVol(struct HAL_ACODEC_DEV *acodec,
                                uint8_t vol)
{
    struct ACODEC_REG *reg = acodec->pReg;

    HAL_ASSERT(vol <= 3);

    MODIFY_REG(reg->LICFG0,
               ACODEC_MIC_L_BOOST_MASK | ACODEC_MIC_R_BOOST_MASK,
               ACODEC_MIC_L_BOOST(vol) | ACODEC_MIC_R_BOOST(vol));
}

/**
 * @brief  set adcmux volume
 * @param  acodec: the handle of acodec.
 * @param  vol: volume.
 * @return none
 */
static void ACODEC_SetAdcMuxVol(struct HAL_ACODEC_DEV *acodec,
                                uint8_t vol)
{
    struct ACODEC_REG *reg = acodec->pReg;

    HAL_ASSERT(vol <= 15);

    MODIFY_REG(reg->LICFG2,
               ACODEC_MUX_L_GAIN_MASK | ACODEC_MUX_R_GAIN_MASK,
               ACODEC_MUX_L_GAIN(vol) | ACODEC_MUX_R_GAIN(vol));
}

/**
 * @brief  set dac volume
 * @param  acodec: the handle of acodec.
 * @param  vol: volume.
 * @return none
 */
HAL_UNUSED static void ACODEC_SetDacDigVol(struct HAL_ACODEC_DEV *acodec,
                                           uint8_t vol)
{
    struct ACODEC_REG *reg = acodec->pReg;

    WRITE_REG(reg->DACVCTLL, vol);
    WRITE_REG(reg->DACVCTLR, vol);
}

/**
 * @brief  set adc digital volume
 * @param  acodec: the handle of acodec.
 * @param  vol: volume.
 * @return none
 */
static void ACODEC_SetAdcDigVol(struct HAL_ACODEC_DEV *acodec,
                                uint8_t vol)
{
    struct ACODEC_REG *reg = acodec->pReg;

    WRITE_REG(reg->ADCVCTLL, vol);
    WRITE_REG(reg->ADCVCTLR, vol);
}

/**
 * @brief  unmute dac
 * @param  acodec: the handle of acodec.
 * @return none
 */
HAL_UNUSED static void ACODEC_DacUnmute(struct HAL_ACODEC_DEV *acodec)
{
    struct ACODEC_REG *reg = acodec->pReg;

    WRITE_REG(reg->DACMUTE, ACODEC_DACMTE_DISABLE);
}

/**
 * @brief  mute dac
 * @param  acodec: the handle of acodec.
 * @return none
 */
HAL_UNUSED static void ACODEC_DacMute(struct HAL_ACODEC_DEV *acodec)
{
    struct ACODEC_REG *reg = acodec->pReg;

    WRITE_REG(reg->DACMUTE, ACODEC_DACMTE_ENABLE);
}

/**
 * @brief  mix adc to dac
 * @param  acodec: the handle of acodec.
 * @return none
 */
static void ACODEC_MixAdcToDac(struct HAL_ACODEC_DEV *acodec, bool en)
{
    struct ACODEC_REG *reg = acodec->pReg;

    if (en) {
        WRITE_REG(reg->MIXCTRL, ACODEC_MIXE_ENABLE);
    } else {
        WRITE_REG(reg->MIXCTRL, ACODEC_MIXE_DISABLE);
    }
}

/**
 * @brief  Set pll rate for samplerate.
 * @param  acodec: the handle of acodec.
 * @param  outRate: pll out rate.
 * @return none
 */
static void ACODEC_SetPll(struct HAL_ACODEC_DEV *acodec,
                          uint32_t outRate)
{
    struct ACODEC_REG *reg = acodec->pReg;
    const struct ACODEC_PLL *ap;

    if (s_pllOutRate == outRate) {
        return;
    }

    switch (acodec->mclk) {
    case 24000000:
        MODIFY_REG(reg->PLLCFG0,
                   ACODEC_PLL_CLKIN_SEL_MASK,
                   ACODEC_PLL_CLKIN_SEL_S_CLOCK);
        break;
    case 12288000:
    /* fallthrough */
    case 6144000:
    /* fallthrough */
    case 4096000:
    /* fallthrough */
    case 3072000:
    /* fallthrough */
    case 2048000:
        MODIFY_REG(reg->PLLCFG0,
                   ACODEC_PLL_CLKIN_SEL_MASK,
                   ACODEC_PLL_CLKIN_SEL_I2S_CLOCK);
        break;
    default:

        return;
    }

    switch (outRate) {
    case PLL_FREQ_40960:
        ap = s_pllTable40960;
        break;
    case PLL_FREQ_56448:
        ap = s_pllTable56448;
        break;
    case PLL_FREQ_61440:
        ap = s_pllTable61440;
        break;
    default:

        return;
    }

    for (; ap->srcRate && ap->srcRate != acodec->mclk; ap++) {
        ;
    }

    if (ap->srcRate != acodec->mclk) {
        return;
    }

    /* disable out vco */
    MODIFY_REG(reg->PLLCFG0,
               ACODEC_PLL_OUTDIV_EN_MASK, ACODEC_PLL_OUTDIV_DISABLE);
    MODIFY_REG(reg->PLLCFG5, ACODEC_PLL_RESET_MASK, ACODEC_PLL_RESET);

    WRITE_REG(reg->PLLCFG1, ap->postDivL3);
    WRITE_REG(reg->PLLCFG2, ap->postDivH8);
    WRITE_REG(reg->PLLCFG3, ap->preDivBit);
    MODIFY_REG(reg->PLLCFG4,
               ACODEC_PLL_OUTDIV_MASK, ACODEC_PLL_OUTDIV(ap->outDiv));

    /* PLL power on */
    MODIFY_REG(reg->PLLCFG5, ACODEC_PLL_PW_MASK, ACODEC_PLL_PW_UP);
    MODIFY_REG(reg->PLLCFG5, ACODEC_PLL_RESET_MASK, ACODEC_PLL_RELEASE_RESET);
    HAL_DelayMs(5);

    /* enable out vco */
    MODIFY_REG(reg->PLLCFG0,
               ACODEC_PLL_OUTDIV_EN_MASK, ACODEC_PLL_OUTDIV_ENABLE);

    s_pllOutRate = outRate;
}

/**
 * @brief  get srt.
 * @param  sr: samplerate.
 * @return srt
 */
static uint32_t ACODEC_GetSrtTime(uint32_t sr)
{
    uint32_t srt = 0;

    switch (sr) {
    case 8000:
    case 11025:
    case 12000:
        srt = 0;
        break;

    case 16000:
    case 22050:
    case 24000:
        srt = 1;
        break;

    case 32000:
    case 44100:
    case 48000:
        srt = 2;
        break;
    case 64000:
    case 88200:
    case 96000:
        srt = 3;
        break;

    case 128000:
    case 176400:
    case 192000:
        srt = 4;
        break;
    default:
        break;
    }

    return srt;
}

/**
 * @brief  Set i2s samplerate.
 * @param  acodec: the handle of acodec.
 * @param  sclkDiv: sclk div.
 * @param  fs: samplerate.
 * @return none
 */
static void ACODEC_I2s_SetSampleRate(struct HAL_ACODEC_DEV *acodec,
                                     uint32_t sclkDiv, uint32_t fs)
{
    struct ACODEC_REG *reg = acodec->pReg;
    uint32_t srt = ACODEC_GetSrtTime(fs);

    MODIFY_REG(reg->I2SCKM,
               ACODEC_I2S_MCK_DIV_MASK, ACODEC_I2S_MCK_DIV(sclkDiv));
    WRITE_REG(reg->DACSR, srt);
    WRITE_REG(reg->ADCSR, srt);
}

/**
 * @brief  Set samplerate.
 * @param  acodec: the handle of acodec.
 * @param  fs: samplerate.
 * @return none
 */
static void ACODEC_SetSampleRate(struct HAL_ACODEC_DEV *acodec,
                                 uint32_t fs)
{
    struct ACODEC_REG *reg = acodec->pReg;

    if ((fs == 8000) || (fs == 12000) || (fs == 11025)) {
        MODIFY_REG(reg->DACACFG2,
                   ACODEC_PWD_DACBIAS_MASK, ACODEC_PWD_DACBIAS_DOWN);
        MODIFY_REG(reg->DIGEN, ACODEC_DAC_EN_MASK, ACODEC_DAC_EN_DISABLE);
    }

    switch (fs) {
    case 8000:
    case 16000:
    case 32000:
    case 64000:
    case 128000:
        ACODEC_SetPll(acodec, PLL_FREQ_40960);
        ACODEC_I2s_SetSampleRate(acodec, 128000 / fs, fs);
        break;
    case 12000:
    case 24000:
    case 48000:
    case 96000:
    case 192000:
        ACODEC_SetPll(acodec, PLL_FREQ_61440);
        ACODEC_I2s_SetSampleRate(acodec, 192000 / fs, fs);
        break;
    case 11025:
    case 22050:
    case 44100:
    case 88200:
    case 176400:
        ACODEC_SetPll(acodec, PLL_FREQ_56448);
        ACODEC_I2s_SetSampleRate(acodec, 176400 / fs, fs);
        break;
    default:
        break;
    }

    if ((fs == 8000) || (fs == 12000) || (fs == 11025)) {
        MODIFY_REG(reg->DIGEN, ACODEC_DAC_EN_MASK, ACODEC_DAC_EN_ENABLE);
        MODIFY_REG(reg->DACACFG2,
                   ACODEC_PWD_DACBIAS_MASK, ACODEC_PWD_DACBIAS_ON);
    }
}

/**
 * @brief  Set route.
 * @param  acodec: the handle of acodec.
 * @param  route: audio route.
 * @return none
 */
static void ACODEC_SetRoute(struct HAL_ACODEC_DEV *acodec,
                            eACODEC_route route)
{
    switch (route) {
    case HEADPHONE:
        ACODEC_SetDacMode(acodec, ACODEC_HP);
        break;
    case LINEOUT:
        ACODEC_SetDacMode(acodec, ACODEC_LINE);
        break;
    case LINEIN1:
        ACODEC_SetAdcMode(acodec, ACODEC_LINE_IN1);
        ACODEC_MixAdcToDac(acodec, false);
        ACODEC_SetAdcMuxVol(acodec, 5);
        ACODEC_SetAdcDigVol(acodec, 0);
        ACODEC_SetAdcHpf(acodec);
        break;
    case LINEIN1_TO_DAC:
        ACODEC_SetAdcMode(acodec, ACODEC_LINE_IN1);
        ACODEC_MixAdcToDac(acodec, true);
        ACODEC_SetAdcHpf(acodec);
        break;
    case LINEIN2:
        ACODEC_SetAdcMode(acodec, ACODEC_LINE_IN2);
        ACODEC_MixAdcToDac(acodec, false);
        ACODEC_SetAdcMuxVol(acodec, 5);
        ACODEC_SetAdcDigVol(acodec, 0);
        ACODEC_SetAdcHpf(acodec);
        break;
    case LINEIN2_TO_DAC:
        ACODEC_SetAdcMode(acodec, ACODEC_LINE_IN2);
        ACODEC_MixAdcToDac(acodec, true);
        ACODEC_SetAdcHpf(acodec);
        break;
    case MIC_STEREO:
        ACODEC_SetAdcMode(acodec, ACODEC_MIC_STEREO);
        ACODEC_MixAdcToDac(acodec, false);
        ACODEC_SetMicAnaVol(acodec, 2);
        ACODEC_SetAdcMuxVol(acodec, 5);
        ACODEC_SetAdcDigVol(acodec, 0);
        ACODEC_SetAdcHpf(acodec);
        break;
    case MIC1_MONO:
        ACODEC_SetAdcMode(acodec, ACODEC_MIC1_MONO);
        ACODEC_MixAdcToDac(acodec, false);
        ACODEC_SetMicAnaVol(acodec, 2);
        ACODEC_SetAdcMuxVol(acodec, 5);
        ACODEC_SetAdcDigVol(acodec, 0);
        ACODEC_SetAdcHpf(acodec);
        break;
    case MIC2_MONO:
        ACODEC_SetAdcMode(acodec, ACODEC_MIC2_MONO);
        ACODEC_MixAdcToDac(acodec, false);
        ACODEC_SetMicAnaVol(acodec, 2);
        ACODEC_SetAdcMuxVol(acodec, 5);
        ACODEC_SetAdcDigVol(acodec, 0);
        ACODEC_SetAdcHpf(acodec);
        break;
    default:
        break;
    }
}

/**
 * @brief  start i2s rx.
 * @param  acodec: the handle of acodec.
 * @return none
 */
static void ACODEC_StartI2sRx(struct HAL_ACODEC_DEV *acodec)
{
    struct ACODEC_REG *reg = acodec->pReg;

    WRITE_REG(reg->I2SRXCMD, ACODEC_RX_START);
}

/**
 * @brief  stop i2s rx.
 * @param  acodec: the handle of acodec.
 * @return none
 */
static void ACODEC_StopI2sRx(struct HAL_ACODEC_DEV *acodec)
{
    struct ACODEC_REG *reg = acodec->pReg;

    WRITE_REG(reg->I2SRXCMD, ACODEC_RX_STOP);
}

/**
 * @brief  start i2s tx.
 * @param  acodec: the handle of acodec.
 * @return none
 */
static void ACODEC_StartI2sTx(struct HAL_ACODEC_DEV *acodec)
{
    struct ACODEC_REG *reg = acodec->pReg;

    WRITE_REG(reg->I2STXCMD, ACODEC_TX_START);
}

/**
 * @brief  stop i2s tx.
 * @param  acodec: the handle of acodec.
 * @return none
 */
static void ACODEC_StopI2sTx(struct HAL_ACODEC_DEV *acodec)
{
    struct ACODEC_REG *reg = acodec->pReg;

    WRITE_REG(reg->I2STXCMD, ACODEC_TX_STOP);
}

/**
 * @brief  soft reset.
 * @param  acodec: the handle of acodec.
 * @return none
 */
static void ACODEC_Reset(struct HAL_ACODEC_DEV *acodec)
{
    struct ACODEC_REG *reg = acodec->pReg;
    uint32_t timeout = 200000;

    WRITE_REG(reg->SRST, ACODEC_SOFT_RESET);

    while (READ_REG(reg->SRST)) {
        if (--timeout == 0) {
            HAL_DBG_ERR("reset fail.");
            break;
        }
    }
}

/**
 * @brief  power on.
 * @param  acodec: the handle of acodec.
 * @return none
 */
static void ACODEC_PowerOn(struct HAL_ACODEC_DEV *acodec)
{
    struct ACODEC_REG *reg = acodec->pReg;

    s_pllOutRate = 0;

    ACODEC_Reset(acodec);

    MODIFY_REG(reg->PLLCFG5, ACODEC_PLL_PW_MASK, ACODEC_PLL_PW_UP);
    MODIFY_REG(reg->RTCFG1,
               ACODEC_VBG_TRIM_MASK, ACODEC_VBG_TRIM(0x7));
    HAL_DelayMs(5);
    MODIFY_REG(reg->RTCFG2, ACODEC_REF_PWD_MASK, ACODEC_REF_PWD_ON);
    HAL_DelayMs(200);
    MODIFY_REG(reg->RTCFG2, ACODEC_VAG_BUF_PWD_MASK, ACODEC_VAG_BUF_PWD_ON);
    MODIFY_REG(reg->RTCFG2, ACODEC_IBIAS_PWD_MASK, ACODEC_IBIAS_PWD_ON);
    MODIFY_REG(reg->RTCFG1,
               ACODEC_VBG_TRIM_MASK, ACODEC_VBG_TRIM(0x1));
    WRITE_REG(reg->DACPOPD,
              ACODEC_ATPCE_DISABLE | ACODEC_SMTPO_DOWN | ACODEC_ANTIPOP_DISABLE);
    WRITE_REG(reg->HPLOCFG3,
              ACODEC_PWD_HP_OSTG_ON | ACODEC_PWD_HP_VGND_ON |
              ACODEC_PWD_HP_BUF_ON | ACODEC_HP_SHORT_OUT_DISABLE);
    MODIFY_REG(reg->I2SCKM, ACODEC_SCK_EN_MASK, ACODEC_SCK_EN_ENABLE);
}

/** @} */
/********************* Public Function Definition ****************************/
/** @defgroup ACODEC_Exported_Functions_Group1 Suspend and Resume Functions

 This section provides functions allowing to suspend and resume the module:

 ...to do or delete this row

 *  @{
 */

/**
 * @brief  acodec suspend.
 * @param  acodec: the handle of acodec.
 * @return HAL_Status
 */
HAL_Status HAL_ACODEC_Supsend(struct HAL_ACODEC_DEV *acodec)
{
    return HAL_OK;
}

/**
 * @brief  acodec resume.
 * @param  acodec: the handle of acodec.
 * @return HAL_Status
 */
HAL_Status HAL_ACODEC_Resume(struct HAL_ACODEC_DEV *acodec)
{
    return HAL_OK;
}

/** @} */

/** @defgroup ACODEC_Exported_Functions_Group2 State and Errors Functions

 This section provides functions allowing to get the status of the module:

 *  @{
 */

/** @} */

/** @defgroup ACODEC_Exported_Functions_Group3 IO Functions

 This section provides functions allowing to IO controlling:

 *  @{
 */

/** @} */

/** @defgroup ACODEC_Exported_Functions_Group4 Init and DeInit Functions

 This section provides functions allowing to init and deinit the module:

 ...to do or delete this row

 *  @{
 */

/**
 * @brief  Init acodec.
 * @param  acodec: the handle of acodec.
 * @param  config: init config for acodec init.
 * @return HAL_Status
 */
HAL_Status HAL_ACODEC_Init(struct HAL_ACODEC_DEV *acodec, struct AUDIO_INIT_CONFIG *config)
{
    struct ACODEC_REG *reg = acodec->pReg;

    HAL_ASSERT(config->master);

    ACODEC_PowerOn(acodec);

    MODIFY_REG(reg->I2SCKM, ACODEC_I2S_MST_MASK, ACODEC_I2S_MST_MASTER);

    MODIFY_REG(reg->I2SRXCR0, ACODEC_SCKD_TX_DIV_MASK, ACODEC_SCKD_TX_64_DIV);
    MODIFY_REG(reg->I2STXCR0, ACODEC_SCKD_TX_DIV_MASK, ACODEC_SCKD_TX_64_DIV);

    MODIFY_REG(reg->I2SRXCR1, ACODEC_TFS_TX_MODE_MASK, ACODEC_TFS_TX_I2S_MODE);
    MODIFY_REG(reg->I2STXCR1, ACODEC_TFS_TX_MODE_MASK, ACODEC_TFS_TX_I2S_MODE);

    MODIFY_REG(reg->I2SRXCR2, ACODEC_VDW_TX_WIDTH_MASK, ACODEC_VDW_TX_WIDTH_16BIT);
    MODIFY_REG(reg->I2STXCR2, ACODEC_VDW_TX_WIDTH_MASK, ACODEC_VDW_TX_WIDTH_16BIT);

    /* TBD fixup */
    ACODEC_StartI2sRx(acodec);
    ACODEC_StartI2sTx(acodec);

    ACODEC_SetSampleRate(acodec, 48000);

    ACODEC_SetRoute(acodec, HEADPHONE);
    ACODEC_SetRoute(acodec, MIC_STEREO);

    return HAL_OK;
}

/**
 * @brief  DeInit acodec.
 * @param  acodec: the handle of acodec.
 * @return HAL_Status
 */
HAL_Status HAL_ACODEC_DeInit(struct HAL_ACODEC_DEV *acodec)
{
    struct ACODEC_REG *reg = acodec->pReg;

    MODIFY_REG(reg->RTCFG2, ACODEC_IBIAS_PWD_MASK, ACODEC_IBIAS_PWD_DOWN);
    HAL_DelayMs(50);
    MODIFY_REG(reg->RTCFG2, ACODEC_VAG_BUF_PWD_MASK, ACODEC_VAG_BUF_PWD_DOWN);
    HAL_DelayMs(50);
    MODIFY_REG(reg->RTCFG2, ACODEC_REF_PWD_MASK, ACODEC_REF_PWD_DOWN);
    HAL_DelayMs(50);

    MODIFY_REG(reg->PLLCFG0,
               ACODEC_PLL_OUTDIV_EN_MASK, ACODEC_PLL_OUTDIV_DISABLE);
    MODIFY_REG(reg->PLLCFG5, ACODEC_PLL_RESET_MASK, ACODEC_PLL_RESET);
    MODIFY_REG(reg->PLLCFG5, ACODEC_PLL_PW_MASK, ACODEC_PLL_PW_DOWN);

    return HAL_OK;
}

/** @} */

/** @defgroup ACODEC_Exported_Functions_Group5 Other Functions
 *  @{
 */

/**
 * @brief  Enable acodec.
 * @param  acodec: the handle of acodec.
 * @param  stream: AUDIO_STREAM_PLAYBACK or AUDIO_STREAM_CAPTURE.
 * @return HAL_Status
 */
HAL_Status HAL_ACODEC_Enable(struct HAL_ACODEC_DEV *acodec,
                             eAUDIO_streamType stream)
{
    if (stream == AUDIO_STREAM_PLAYBACK) {
        ACODEC_StartI2sRx(acodec);
    } else {
        ACODEC_StartI2sTx(acodec);
    }

    return HAL_OK;
}

/**
 * @brief  Disable acodec.
 * @param  acodec: the handle of acodec.
 * @param  stream: AUDIO_STREAM_PLAYBACK or AUDIO_STREAM_CAPTURE.
 * @return HAL_Status
 */
HAL_Status HAL_ACODEC_Disable(struct HAL_ACODEC_DEV *acodec,
                              eAUDIO_streamType stream)
{
    if (stream == AUDIO_STREAM_PLAYBACK) {
        ACODEC_StopI2sRx(acodec);
    } else {
        ACODEC_StopI2sTx(acodec);
    }

    return HAL_OK;
}

/**
 * @brief  Config acodec.
 * @param  acodec: the handle of acodec.
 * @param  stream: AUDIO_STREAM_PLAYBACK or AUDIO_STREAM_CAPTURE.
 * @param  params: audio params.
 * @return HAL_Status
 */
HAL_Status HAL_ACODEC_Config(struct HAL_ACODEC_DEV *acodec,
                             eAUDIO_streamType stream,
                             struct AUDIO_PARAMS *params)
{
    struct ACODEC_REG *reg = acodec->pReg;
    uint32_t val = 0;

    ACODEC_SetSampleRate(acodec, params->sampleRate);

    if (stream == AUDIO_STREAM_CAPTURE) {
        val |= ACODEC_VDW_RX_WIDTH(params->sampleBits);
        MODIFY_REG(reg->I2SRXCR2, ACODEC_VDW_RX_WIDTH_MASK, val);
    } else {
        val |= ACODEC_VDW_TX_WIDTH(params->sampleBits);
        MODIFY_REG(reg->I2STXCR2, ACODEC_VDW_TX_WIDTH_MASK, val);
    }

    return HAL_OK;
}

/** @} */

/** @} */

/** @} */

#endif
