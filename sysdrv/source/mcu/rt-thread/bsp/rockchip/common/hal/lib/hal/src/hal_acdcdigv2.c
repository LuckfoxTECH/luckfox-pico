/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_ACDCDIG_MODULE_ENABLED
#if (ACDCDIG_VERSION == 0x2U) /* ACDCDIGv2 */

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup ACDCDIG
 *  @{
 */

/** @defgroup ACDCDIG_How_To_Use How To Use
 *  @{

 The ACDCDIG driver can be used as follows:

 - Init ACDCDIG Codec during open audio device.
 - Configure ACDCDIG with sample rate.
 - Enable ACDCDIG and start playback or capture.
 - Disable ACDCDIG and stop playback or capture.
 - Deinit ACDCDIG Codec during close audio device.

 @} */

/** @defgroup ACDCDIG_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/
/* REG_CON bits */
#define REG_CON_EN         (0x1 << ACDCDIG_I2C_CON0_I2C_EN_SHIFT)
#define REG_CON_MOD_TXONLY (0x0 << I2C_CON_I2C_MODE_SHIFT)
#define REG_CON_START      (0x1 << ACDCDIG_I2C_CON1_START_SETUP_SHIFT)
#define REG_CON_STOP       (0x1 << ACDCDIG_I2C_CON1_STOP_SETUP_SHIFT)
#define REG_CON_ACTACK     (0x1 << ACDCDIG_I2C_CON0_ACT2NAK_SHIFT)/* send NACK after last received byte */

#define REG_CON_TUNING_MASK  (ACDCDIG_I2C_CON1_DATA_UPD_ST_MASK)
#define REG_CON_SDA_CFG(cfg) ((cfg) << ACDCDIG_I2C_CON1_DATA_UPD_ST_SHIFT)
#define REG_CON_STA_CFG(cfg) ((cfg) << ACDCDIG_I2C_CON1_START_SETUP_SHIFT)
#define REG_CON_STO_CFG(cfg) ((cfg) << ACDCDIG_I2C_CON1_STOP_SETUP_SHIFT)

/* SYSCTRL0 */
#define ACDCDIG_SYSCTRL0_GLB_CKE_EN   (0x1U << ACDCDIG_SYSCTRL0_GLB_CKE_SHIFT)
#define ACDCDIG_SYSCTRL0_GLB_CKE_DIS  (0x0U << ACDCDIG_SYSCTRL0_GLB_CKE_SHIFT)
#define ACDCDIG_SYSCTRL0_CLK_SEL_DAC  (0x1U << ACDCDIG_SYSCTRL0_CLK_SEL_SHIFT)
#define ACDCDIG_SYSCTRL0_CLK_SEL_ADC  (0x0U << ACDCDIG_SYSCTRL0_CLK_SEL_SHIFT)
#define ACDCDIG_SYSCTRL0_SYNC_SEL_DAC (0x1U << ACDCDIG_SYSCTRL0_SYNC_SEL_SHIFT)
#define ACDCDIG_SYSCTRL0_SYNC_SEL_ADC (0x0U << ACDCDIG_SYSCTRL0_SYNC_SEL_SHIFT)
/* ADCVUCTL */
#define ACDCDIG_ADCVUCTL_ADC_ZDT_EN  (0x1U << ACDCDIG_ADCVUCTL_ADC_ZDT_SHIFT)
#define ACDCDIG_ADCVUCTL_ADC_ZDT_DIS (0x0U << ACDCDIG_ADCVUCTL_ADC_ZDT_SHIFT)
/* ADCDIGEN */
#define ACDCDIG_ADCDIGEN_ADC_L0R1_EN  (0x1U << ACDCDIG_ADCDIGEN_ADCEN_L0R1_SHIFT)
#define ACDCDIG_ADCDIGEN_ADC_L0R1_DIS (0x0U << ACDCDIG_ADCDIGEN_ADCEN_L0R1_SHIFT)
#define ACDCDIG_ADCDIGEN_ADC_L2_EN    (0x1U << ACDCDIG_ADCDIGEN_ADCEN_L2_SHIFT)
#define ACDCDIG_ADCDIGEN_ADC_L2_DIS   (0x0U << ACDCDIG_ADCDIGEN_ADCEN_L2_SHIFT)
#define ACDCDIG_ADCDIGEN_ADC_GLB_EN   (0x1U << ACDCDIG_ADCDIGEN_ADC_GLBEN_SHIFT)
#define ACDCDIG_ADCDIGEN_ADC_GLB_DIS  (0x0U << ACDCDIG_ADCDIGEN_ADC_GLBEN_SHIFT)
/* ADCCLKCTRL */
#define ACDCDIG_ADCCLKCTRL_ADC_SYNC_EN     (0x1U << ACDCDIG_ADCCLKCTRL_ADC_SYNC_ENA_SHIFT)
#define ACDCDIG_ADCCLKCTRL_ADC_SYNC_DIS    (0x0U << ACDCDIG_ADCCLKCTRL_ADC_SYNC_ENA_SHIFT)
#define ACDCDIG_ADCCLKCTRL_CIC_DS_RATIO_4  (0x2U << ACDCDIG_ADCCLKCTRL_CIC_DS_RATIO_SHIFT)
#define ACDCDIG_ADCCLKCTRL_CIC_DS_RATIO_8  (0x1U << ACDCDIG_ADCCLKCTRL_CIC_DS_RATIO_SHIFT)
#define ACDCDIG_ADCCLKCTRL_CIC_DS_RATIO_16 (0x0U << ACDCDIG_ADCCLKCTRL_CIC_DS_RATIO_SHIFT)
#define ACDCDIG_ADCCLKCTRL_ADC_CKE_EN      (0x1U << ACDCDIG_ADCCLKCTRL_ADC_CKE_SHIFT)
#define ACDCDIG_ADCCLKCTRL_ADC_CKE_DIS     (0x0U << ACDCDIG_ADCCLKCTRL_ADC_CKE_SHIFT)
#define ACDCDIG_ADCCLKCTRL_I2STX_CKE_EN    (0x1U << ACDCDIG_ADCCLKCTRL_I2STX_CKE_SHIFT)
#define ACDCDIG_ADCCLKCTRL_I2STX_CKE_DIS   (0x0U << ACDCDIG_ADCCLKCTRL_I2STX_CKE_SHIFT)
#define ACDCDIG_ADCCLKCTRL_CKE_BCLKTX_EN   (0x1U << ACDCDIG_ADCCLKCTRL_CKE_BCLKTX_SHIFT)
#define ACDCDIG_ADCCLKCTRL_CKE_BCLKTX_DIS  (0x0U << ACDCDIG_ADCCLKCTRL_CKE_BCLKTX_SHIFT)
#define ACDCDIG_ADCCLKCTRL_FILTER_DIS      (0x1U << ACDCDIG_ADCCLKCTRL_FILTER_GATE_EN_SHIFT)
#define ACDCDIG_ADCCLKCTRL_FILTER_EN       (0x0U << ACDCDIG_ADCCLKCTRL_FILTER_GATE_EN_SHIFT)
/* ADCINT_DIV */
#define ACDCDIG_ADCINT_DIV_INT_DIV(x) (((x) - 1) << ACDCDIG_ADCINT_DIV_INT_DIV_CON_SHIFT)
/* ADCSCLKTXINT_DIV */
#define ACDCDIG_ADCSCLKTXINT_DIV_SCKTXDIV(x) (((x) - 1) << ACDCDIG_ADCSCLKTXINT_DIV_SCKTXDIV_SHIFT)
/* ADCCFG1 */
#define ACDCDIG_ADCCFG1_ADCSRT(x) ((x) << ACDCDIG_ADCCFG1_ADCSRT_SHIFT)
/* ADCVOGP */
#define ACDCDIG_ADCVOGP_VOLGPL0_POS_GAIN (0x1U << ACDCDIG_ADCVOGP_VOLGPL0_SHIFT)
#define ACDCDIG_ADCVOGP_VOLGPL0_NEG_GAIN (0x0U << ACDCDIG_ADCVOGP_VOLGPL0_SHIFT)
#define ACDCDIG_ADCVOGP_VOLGPR1_POS_GAIN (0x1U << ACDCDIG_ADCVOGP_VOLGPR1_SHIFT)
#define ACDCDIG_ADCVOGP_VOLGPR1_NEG_GAIN (0x0U << ACDCDIG_ADCVOGP_VOLGPR1_SHIFT)
#define ACDCDIG_ADCVOGP_VOLGPL2_POS_GAIN (0x1U << ACDCDIG_ADCVOGP_VOLGPL2_SHIFT)
#define ACDCDIG_ADCVOGP_VOLGPL2_NEG_GAIN (0x0U << ACDCDIG_ADCVOGP_VOLGPL2_SHIFT)
/* ADCVOLL0 / ADCVOLL1 / ADCVOLR0 */
#define ACDCDIG_ADCVOL(x) ((x) << ACDCDIG_ADCVOGP_VOLGPL0_SHIFT)
/* ADCHPFEN */
#define ACDCDIG_ADCHPFEN_HPFEN_L0  (0x1U << ACDCDIG_ADCHPFEN_HPFEN_L0_SHIFT)
#define ACDCDIG_ADCHPFEN_HPFDIS_L0 (0x0U << ACDCDIG_ADCHPFEN_HPFEN_L0_SHIFT)
#define ACDCDIG_ADCHPFEN_HPFEN_R1  (0x1U << ACDCDIG_ADCHPFEN_HPFEN_R1_SHIFT)
#define ACDCDIG_ADCHPFEN_HPFDIS_R1 (0x0U << ACDCDIG_ADCHPFEN_HPFEN_R1_SHIFT)
#define ACDCDIG_ADCHPFEN_HPFEN_L2  (0x1U << ACDCDIG_ADCHPFEN_HPFEN_L2_SHIFT)
#define ACDCDIG_ADCHPFEN_HPFDIS_L2 (0x0U << ACDCDIG_ADCHPFEN_HPFEN_L2_SHIFT)
/* ADCHPFCF */
#define ACDCDIG_ADCHPFCF_HPFCF_3_79HZ (0x0U << ACDCDIG_ADCHPFCF_HPFCF_SHIFT)
#define ACDCDIG_ADCHPFCF_HPFCF_60HZ   (0x1U << ACDCDIG_ADCHPFCF_HPFCF_SHIFT)
#define ACDCDIG_ADCHPFCF_HPFCF_243HZ  (0x2U << ACDCDIG_ADCHPFCF_HPFCF_SHIFT)
#define ACDCDIG_ADCHPFCF_HPFCF_493HZ  (0x3U << ACDCDIG_ADCHPFCF_HPFCF_SHIFT)
/* DACVUCTL */
#define ACDCDIG_DACVUCTL_DAC_ZDT_EN  (0x1U << ACDCDIG_DACVUCTL_DAC_ZDT_SHIFT)
#define ACDCDIG_DACVUCTL_DAC_ZDT_DIS (0x0U << ACDCDIG_DACVUCTL_DAC_ZDT_SHIFT)
/* DACDIGEN */
#define ACDCDIG_DACDIGEN_DAC_GLB_EN  (0x1U << ACDCDIG_DACDIGEN_DAC_GLBEN_SHIFT)
#define ACDCDIG_DACDIGEN_DAC_GLB_DIS (0x0U << ACDCDIG_DACDIGEN_DAC_GLBEN_SHIFT)
#define ACDCDIG_DACDIGEN_DAC_L0_EN   (0x1U << ACDCDIG_DACDIGEN_DACEN_L0_SHIFT)
#define ACDCDIG_DACDIGEN_DAC_L0_DIS  (0x0U << ACDCDIG_DACDIGEN_DACEN_L0_SHIFT)
/* DACCLKCTRL */
#define ACDCDIG_DACCLKCTRL_DAC_SYNC_EN    (0x1U << ACDCDIG_DACCLKCTRL_DAC_SYNC_ENA_SHIFT)
#define ACDCDIG_DACCLKCTRL_DAC_SYNC_DIS   (0x0U << ACDCDIG_DACCLKCTRL_DAC_SYNC_ENA_SHIFT)
#define ACDCDIG_DACCLKCTRL_DAC_CKE_EN     (0x1U << ACDCDIG_DACCLKCTRL_DAC_CKE_SHIFT)
#define ACDCDIG_DACCLKCTRL_DAC_CKE_DIS    (0x0U << ACDCDIG_DACCLKCTRL_DAC_CKE_SHIFT)
#define ACDCDIG_DACCLKCTRL_I2SRX_CKE_EN   (0x1U << ACDCDIG_DACCLKCTRL_I2SRX_CKE_SHIFT)
#define ACDCDIG_DACCLKCTRL_I2SRX_CKE_DIS  (0x0U << ACDCDIG_DACCLKCTRL_I2SRX_CKE_SHIFT)
#define ACDCDIG_DACCLKCTRL_CKE_BCLKRX_EN  (0x1U << ACDCDIG_DACCLKCTRL_CKE_BCLKRX_SHIFT)
#define ACDCDIG_DACCLKCTRL_CKE_BCLKRX_DIS (0x0U << ACDCDIG_DACCLKCTRL_CKE_BCLKRX_SHIFT)
/* DACINT_DIV */
#define ACDCDIG_DACINT_DIV_INT_DIV(x) (((x) - 1) << ACDCDIG_DACINT_DIV_INT_DIV_CON_SHIFT)
/* DACSCLKRXINT_DIV */
#define ACDCDIG_DACSCLKRXINT_DIV_SCKRXDIV(x) (((x) - 1) << ACDCDIG_DACSCLKRXINT_DIV_SCKRXDIV_SHIFT)
/* DACCFG1 */
#define ACDCDIG_DACCFG1_DACSRT(x) ((x) << ACDCDIG_DACCFG1_DACSRT_SHIFT)
/* DACVOLL0 */
#define ACDCDIG_DACVOL(x) ((x) << ACDCDIG_DACVOLL0_DACLV0_SHIFT)
/* DACVOGP */
#define ACDCDIG_DACVOGP_VOLGPL0_POS_GAIN (0x1U << ACDCDIG_DACVOGP_VOLGPL0_SHIFT)
#define ACDCDIG_DACVOGP_VOLGPL0_NEG_GAIN (0x0U << ACDCDIG_DACVOGP_VOLGPL0_SHIFT)
/* I2S_TXCR0 */
#define ACDCDIG_I2S_TXCR0_VDW(x)          (((x) - 1) << ACDCDIG_I2S_TXCR0_VDW_SHIFT)
#define ACDCDIG_I2S_TXCR0_TFS_I2S         (0x0U << ACDCDIG_I2S_TXCR0_TFS_SHIFT)
#define ACDCDIG_I2S_TXCR0_TFS_PCM         (0x1U << ACDCDIG_I2S_TXCR0_TFS_SHIFT)
#define ACDCDIG_I2S_TXCR0_PBM_PCM_MODE(x) ((x) << ACDCDIG_I2S_TXCR0_PBM_SHIFT)

/* I2S_TXCR1 */
#define ACDCDIG_I2S_TXCR1_TCSR(x)         ((x) << ACDCDIG_I2S_TXCR1_TCSR_SHIFT)
#define ACDCDIG_I2S_TXCR1_IBM_I2S_NORMAL  (0x0U << ACDCDIG_I2S_TXCR1_IBM_SHIFT)
#define ACDCDIG_I2S_TXCR1_IBM_I2S_L_AJUST (0x1U << ACDCDIG_I2S_TXCR1_IBM_SHIFT)
#define ACDCDIG_I2S_TXCR1_IBM_I2S_R_AJUST (0x2U << ACDCDIG_I2S_TXCR1_IBM_SHIFT)

/* I2S_RXCR0 */
#define ACDCDIG_I2S_RXCR0_VDW(x)          (((x) - 1) << ACDCDIG_I2S_RXCR0_VDW_SHIFT)
#define ACDCDIG_I2S_RXCR0_TFS_I2S         (0x0U << ACDCDIG_I2S_RXCR0_TFS_SHIFT)
#define ACDCDIG_I2S_RXCR0_TFS_PCM         (0x1U << ACDCDIG_I2S_RXCR0_TFS_SHIFT)
#define ACDCDIG_I2S_RXCR0_PBM_PCM_MODE(x) ((x) << ACDCDIG_I2S_RXCR0_PBM_SHIFT)

/* I2S_CKR0 */
#define ACDCDIG_I2S_CKR0_RSD(x) ((((x) / 64) - 1) << ACDCDIG_I2S_CKR0_RSD_SHIFT)
#define ACDCDIG_I2S_CKR0_TSD(x) ((((x) / 64) - 1) << ACDCDIG_I2S_CKR0_TSD_SHIFT)
/* I2S_CKR1 */
#define ACDCDIG_I2S_CKR1_TLP_NORMAL  (0x0U << ACDCDIG_I2S_CKR1_TLP_SHIFT)
#define ACDCDIG_I2S_CKR1_TLP_OPPSITE (0x1U << ACDCDIG_I2S_CKR1_TLP_SHIFT)
#define ACDCDIG_I2S_CKR1_RLP_NORMAL  (0x0U << ACDCDIG_I2S_CKR1_RLP_SHIFT)
#define ACDCDIG_I2S_CKR1_RLP_OPPSITE (0x1U << ACDCDIG_I2S_CKR1_RLP_SHIFT)
#define ACDCDIG_I2S_CKR1_CKP_POS     (0x0U << ACDCDIG_I2S_CKR1_CKP_SHIFT)
#define ACDCDIG_I2S_CKR1_CKP_NEG     (0x1U << ACDCDIG_I2S_CKR1_CKP_SHIFT)
#define ACDCDIG_I2S_CKR1_MSS_MASTER  (0x0U << ACDCDIG_I2S_CKR1_MSS_SHIFT)
#define ACDCDIG_I2S_CKR1_MSS_SLAVE   (0x1U << ACDCDIG_I2S_CKR1_MSS_SHIFT)

/* I2S_XFER */
#define ACDCDIG_I2S_XFER_TXS_START (0x1U << ACDCDIG_I2S_XFER_TXS_SHIFT)
#define ACDCDIG_I2S_XFER_TXS_STOP  (0x0U << ACDCDIG_I2S_XFER_TXS_SHIFT)
#define ACDCDIG_I2S_XFER_RXS_START (0x1U << ACDCDIG_I2S_XFER_RXS_SHIFT)
#define ACDCDIG_I2S_XFER_RXS_STOP  (0x0U << ACDCDIG_I2S_XFER_RXS_SHIFT)
/* I2S_CLR */
#define ACDCDIG_I2S_CLR_RXC (0x1U << ACDCDIG_I2S_CLR_RXC_SHIFT)
#define ACDCDIG_I2S_CLR_TXC (0x1U << ACDCDIG_I2S_CLR_TXC_SHIFT)

/* The groups of mclk */
#define ACDCDIG_GROUP0_MCLK 49152000
#define ACDCDIG_GROUP1_MCLK 45158400
#define ACDCDIG_GROUP2_MCLK 32768000

/* The groups of sync clock */
#define ACDCDIG_GROUP0_SYNCCLK 6144000
#define ACDCDIG_GROUP1_SYNCCLK 5644800
#define ACDCDIG_GROUP2_SYNCCLK 4096000

/* Using ACDCDIG mclk group0. */
#define HAL_ACDCDIG_MCLK_GROUP_ID 0

/* Match ACDCDIG_MCLK. */
#ifdef HAL_ACDCDIG_MCLK_GROUP_ID
#if (HAL_ACDCDIG_MCLK_GROUP_ID == 1)
#define ACDCDIG_MCLK ACDCDIG_GROUP1_MCLK
#elif (HAL_ACDCDIG_MCLK_GROUP_ID == 2)
#define ACDCDIG_MCLK ACDCDIG_GROUP2_MCLK
#else /* HAL_ACDCDIG_MCLK_GROUP_ID == 0 */
#define ACDCDIG_MCLK ACDCDIG_GROUP0_MCLK
#endif
#else /* HAL_ACDCDIG_MCLK_GROUP_ID is not defined, select group0 mclk. */
#define ACDCDIG_MCLK ACDCDIG_GROUP0_MCLK
#endif

/**
 * The relationship between mclk and sampling rate is shown in the following
 * table:
 *
 * ```
 *         ACDC_CLK  D2A_CLK   D2A_SYNC Sample rates supported
 * Group0: 49.152MHz 49.152MHz 6.144MHz 12/24/48/96/192kHz
 * Group1: 45.154MHz 45.154MHz 5.644MHz 11.024/22.05/44.1/88.2/176.4kHz
 * Gruop2: 32.768MHz 32.768MHz 4.096MHz 8/16/32/64/128kHz
 * ```
 * The Group0 is the highest. Usually, if you need 48kHz to playback which
 * belongs in Group0, it's better to use the unified group0 mclk as a base
 * clock for playback and capture.
 */
#define ACDCDIG_MCLK_RATE ACDCDIG_GROUP0_MCLK

/* GRF registers */
#define GRF_CODEC_I2C_TRANS ((GRF_SOC_CON16_GRF_I2C_TRANS_REQ_MASK << 16) | (0 << GRF_SOC_CON16_GRF_I2C_TRANS_REQ_SHIFT))
#define GRF_MCU_I2C_TRANS   ((GRF_SOC_CON16_GRF_I2C_TRANS_REQ_MASK << 16) | (1 << GRF_SOC_CON16_GRF_I2C_TRANS_REQ_SHIFT))

/**
 * The range of gains.
 * To avoid using decimals, the scaled 1000 times dB is like Linux ALSA TLV.
 * min: -95.625dB, max: 95.625dB, step: 0.375dB.
 */
#define ACDCDIG_DB_MIN  (-95625)
#define ACDCDIG_DB_MAX  (95625)
#define ACDCDIG_DB_STEP (375)

/* The monotonous max at 8-bit width */
#define ACDCDIG_ADCVOL_MONO_MAX 0xffU
#define ACDCDIG_DACVOL_MONO_MAX 0xffU

/* The number of ADC and DAC. */
#define ACDCDIG_ADC_NUM 3
#define ACDCDIG_DAC_NUM 1

/* Requested i2s mclk and mux. */
#define CLK_CODEC_SEL_MCLK     MCLK_I2S8CH_1_TX
#define CLK_CODEC_SEL_MCLK_MUX CLK_CODEC_SEL_MCLK_I2S8CH_1_TX_MUX

/**
 * For compatibility, the sample rate modes are suitable for different
 * scenarios.
 */
#ifndef HAL_ACDCDIG_SAMPLERATE_MODE
#define HAL_ACDCDIG_FLEXIBLE_SAMPLERATE 0
#define HAL_ACDCDIG_STRICT_SAMPLERATE   1
#define HAL_ACDCDIG_SAMPLERATE_MODE     HAL_ACDCDIG_FLEXIBLE_SAMPLERATE
#endif

#if (HAL_ACDCDIG_SAMPLERATE_MODE == HAL_ACDCDIG_STRICT_SAMPLERATE)
#define STRICT_SAMPLERATE_CAPTURE  AUDIO_SAMPLERATE_16000
#define STRICT_SAMPLERATE_PLAYBACK AUDIO_SAMPLERATE_48000
#endif

/* This is checking sync status count for wait a while. */
#define SYNC_STATUS_COUNT 50
/********************* Private Structure Definition **************************/

#ifdef HAL_I2C_MODULE_ENABLED
/**
 * enum ACDCDIG I2C transfer mode definition - These definitions are
 * independent of the generic I2C driven.
 */
typedef enum {
    ACDCDIG_I2C_100K = 0,
    ACDCDIG_I2C_400K,
    ACDCDIG_I2C_1000K
} eACDCDIG_I2C_BusSpeed;

/**
 * enum Clock Sync Type - indicate which clock sync is.
 */
typedef enum {
    CLOCK_SYNC_ADC = 0,
    CLOCK_SYNC_DAC,
} eClockSyncType;

struct I2C_SPEC_VALUES {
    uint32_t minLowNS;
    uint32_t minHighNS;
    uint32_t maxRiseNS;
    uint32_t maxFallNS;
};

/********************* Private Variable Definition ***************************/

static const struct I2C_SPEC_VALUES standardModeSpec = {
    .minLowNS = 4700,
    .minHighNS = 4000,
    .maxRiseNS = 1000,
    .maxFallNS = 300,
};

static const struct I2C_SPEC_VALUES fastModeSpec = {
    .minLowNS = 1300,
    .minHighNS = 600,
    .maxRiseNS = 300,
    .maxFallNS = 300,
};

static const struct I2C_SPEC_VALUES fastModePlusSpec = {
    .minLowNS = 500,
    .minHighNS = 260,
    .maxRiseNS = 120,
    .maxFallNS = 120,
};

/********************* Private Function Definition ***************************/

/**
 * @brief  Get the I2C timing specification.
 * @param  speed: desired I2C bus speed.
 * @return matched i2c spec values
 */
static const struct I2C_SPEC_VALUES *I2C_GetSpec(eACDCDIG_I2C_BusSpeed speed)
{
    if (speed == ACDCDIG_I2C_1000K) {
        return &fastModePlusSpec;
    } else if (speed == ACDCDIG_I2C_400K) {
        return &fastModeSpec;
    } else {
        return &standardModeSpec;
    }
}

/**
 * @brief  Auto adapte the clock div base on input clock rate and desired speed.
 * @param  acdcDig: pointer to a HAL_ACDCDIG_DEV structure that contains
 *               the information for I2C module.
 * @param  rate: desired input clock rate for I2C.
 * @param  speed: desired I2C bus speed.
 * @return HAL status
 */
static HAL_Status ACDCDIG_I2C_AdaptDIV(struct HAL_ACDCDIG_DEV *acdcDig,
                                       uint32_t rate,
                                       eACDCDIG_I2C_BusSpeed speed)
{
    const struct I2C_SPEC_VALUES *spec;
    uint32_t rateKHZ, speedKHZ;
    uint32_t minTotalDIV, minLowDIV, minHighDIV, minHoldDIV;
    uint32_t lowDIV, highDIV, extraDIV, extraLowDIV;
    uint32_t minLowNS, minHighNS;
    uint32_t startSetup = 0;

    switch (speed) {
    case ACDCDIG_I2C_1000K:
        speedKHZ = 1000;
        break;
    case ACDCDIG_I2C_400K:
        speedKHZ = 400;
        break;
    default:
        /* default start setup time may not enough for 100K */
        startSetup = 1;
        speedKHZ = 100;
        break;
    }

    rateKHZ = HAL_DIV_ROUND_UP(rate, 1000);
    spec = I2C_GetSpec(speed);

    minTotalDIV = HAL_DIV_ROUND_UP(rateKHZ, speedKHZ * 8);

    minHighNS = spec->maxRiseNS + spec->minHighNS;
    minHighDIV = HAL_DIV_ROUND_UP(rateKHZ * minHighNS, 8 * 1000000);

    minLowNS = spec->maxFallNS + spec->minLowNS;
    minLowDIV = HAL_DIV_ROUND_UP(rateKHZ * minLowNS, 8 * 1000000);

    minHighDIV = (minHighDIV < 1) ? 2 : minHighDIV;
    minLowDIV = (minLowDIV < 1) ? 2 : minLowDIV;

    minHoldDIV = minHighDIV + minLowDIV;

    if (minHoldDIV >= minTotalDIV) {
        highDIV = minHighDIV;
        lowDIV = minLowDIV;
    } else {
        extraDIV = minTotalDIV - minHoldDIV;
        extraLowDIV = HAL_DIV_ROUND_UP(minLowDIV * extraDIV, minHoldDIV);

        lowDIV = minLowDIV + extraLowDIV;
        highDIV = minHighDIV + (extraDIV - extraLowDIV);
    }

    highDIV--;
    lowDIV--;

    if (highDIV > 0xffff || lowDIV > 0xffff) {
        return HAL_INVAL;
    }

    WRITE_REG(acdcDig->pReg->I2C_CLKDIVL[0], (lowDIV & 0xff));
    WRITE_REG(acdcDig->pReg->I2C_CLKDIVL[1], ((lowDIV >> 8) & 0xff));
    WRITE_REG(acdcDig->pReg->I2C_CLKDIVH[0], (highDIV & 0xff));
    WRITE_REG(acdcDig->pReg->I2C_CLKDIVH[1], ((highDIV >> 8) & 0xff));
    /* 1 for data hold/setup time is enough */
    WRITE_REG(acdcDig->pReg->I2C_CON[1], REG_CON_SDA_CFG(1) | REG_CON_STA_CFG(startSetup));

    return HAL_OK;
}

/**
 * @brief  Disable the I2C controller.
 * @param  acdcDig: pointer to a HAL_ACDCDIG_DEV structure that contains
 *               the information for I2C module.
 * @return HAL status
 */
static HAL_Status ACDCDIG_I2C_Disable(struct HAL_ACDCDIG_DEV *acdcDig)
{
    uint32_t val = READ_REG(acdcDig->pReg->I2C_CON[1]) & REG_CON_TUNING_MASK;

    WRITE_REG(acdcDig->pReg->I2C_CON[1], val);

    return HAL_OK;
}

/**
 * @brief  Send I2C start signal.
 * @param  acdcDig: pointer to a HAL_ACDCDIG_DEV structure that contains
 *               the information for I2C module.
 * @return HAL status
 */
static HAL_Status ACDCDIG_I2C_Start(struct HAL_ACDCDIG_DEV *acdcDig)
{
    uint32_t val, ret;

    return HAL_OK; /* FIXME: the i2c is used by MCU */

    ret = HAL_ACDCDIG_RequestI2C(acdcDig, CODEC_USE_I2C);
    if (ret != HAL_OK) {
        return ret;
    }

    val = READ_REG(acdcDig->pReg->I2C_CON[0]) & REG_CON_TUNING_MASK;

    /* enable adapter with correct mode, send START condition */
    val |= REG_CON_EN | REG_CON_MOD_TXONLY | REG_CON_START;

    /* if we want to react to NACK, set ACTACK bit */
    if (!(acdcDig->i2cFlags & HAL_I2C_M_IGNORE_NAK)) {
        val |= REG_CON_ACTACK;
    }

    WRITE_REG(acdcDig->pReg->I2C_CON[0], val);

    return HAL_OK;
}

/**
 * @brief  Send I2C stop signal.
 * @param  acdcDig: pointer to a HAL_ACDCDIG_DEV structure that contains
 *               the information for I2C module.
 * @return HAL status
 */
static HAL_Status ACDCDIG_I2C_Stop(struct HAL_ACDCDIG_DEV *acdcDig)
{
    uint32_t ctrl, ret;

    return HAL_OK; /* FIXME: the i2c is used by MCU */

    ret = HAL_ACDCDIG_RequestI2C(acdcDig, CODEC_USE_I2C);
    if (ret != HAL_OK) {
        return ret;
    }

    /*
     * To start the next message. The HW is actually not capable of REPEATED
     * START. But we can get the intended effect by resetting its internal
     * state and issuing an ordinary START.
     */
    ctrl = READ_REG(acdcDig->pReg->I2C_CON[0]) & REG_CON_TUNING_MASK;
    WRITE_REG(acdcDig->pReg->I2C_CON[0], ctrl);

    return HAL_OK;
}

/**
 * @brief  Initialize the I2C according to the specified parameters.
 * @param  acdcDig: pointer to a HAL_ACDCDIG_DEV structure that contains
 *               the information for I2C module.
 * @return HAL status
 */
static HAL_Status ACDCDIG_I2C_Init(struct HAL_ACDCDIG_DEV *acdcDig)
{
    uint32_t i2cCodecRate = HAL_CRU_ClkGetFreq(acdcDig->clkI2cCodec);

    /* Init speed */
    return ACDCDIG_I2C_AdaptDIV(acdcDig, i2cCodecRate, 100);
}

/**
 * @brief  De Initialize the I2C peripheral.
 * @param  acdcDig: pointer to a HAL_ACDCDIG_DEV structure that contains
 *               the information for I2C module.
 * @return HAL status
 */
static HAL_Status ACDCDIG_I2C_DeInit(struct HAL_ACDCDIG_DEV *acdcDig)
{
    /* TO-DO */
    return HAL_OK;
}
#endif /* HAL_I2C_MODULE_ENABLED */

/**
 * @brief  Enable ACDCDIG DACCLKCTRL.
 * @param  acdcDig: the handle of acdcDig.
 * @return HAL_Status
 */
static HAL_Status ACDCDIG_ADCCLKCTRL_Enable(struct HAL_ACDCDIG_DEV *acdcDig)
{
    struct ACDCDIG_REG *reg = acdcDig->pReg;

    MODIFY_REG(reg->ADCCLKCTRL,
               ACDCDIG_ADCCLKCTRL_CIC_DS_RATIO_MASK |
               ACDCDIG_ADCCLKCTRL_ADC_CKE_MASK |
               ACDCDIG_ADCCLKCTRL_I2STX_CKE_MASK |
               ACDCDIG_ADCCLKCTRL_CKE_BCLKTX_MASK |
               ACDCDIG_ADCCLKCTRL_FILTER_GATE_EN_MASK |
               ACDCDIG_ADCCLKCTRL_ADC_SYNC_ENA_MASK,
               ACDCDIG_ADCCLKCTRL_CIC_DS_RATIO_16 |
               ACDCDIG_ADCCLKCTRL_ADC_CKE_EN |
               ACDCDIG_ADCCLKCTRL_I2STX_CKE_EN |
               ACDCDIG_ADCCLKCTRL_CKE_BCLKTX_EN |
               ACDCDIG_ADCCLKCTRL_FILTER_DIS |
               ACDCDIG_ADCCLKCTRL_ADC_SYNC_EN);

    return HAL_OK;
}

/**
 * @brief  Disable ACDCDIG DACCLKCTRL.
 * @param  acdcDig: the handle of acdcDig.
 * @return HAL_Status
 */
static HAL_Status ACDCDIG_ADCCLKCTRL_Disable(struct HAL_ACDCDIG_DEV *acdcDig)
{
    struct ACDCDIG_REG *reg = acdcDig->pReg;

    MODIFY_REG(reg->ADCCLKCTRL,
               ACDCDIG_ADCCLKCTRL_ADC_CKE_MASK |
               ACDCDIG_ADCCLKCTRL_I2STX_CKE_MASK |
               ACDCDIG_ADCCLKCTRL_CKE_BCLKTX_MASK |
               ACDCDIG_ADCCLKCTRL_FILTER_GATE_EN_MASK,
               ACDCDIG_ADCCLKCTRL_ADC_CKE_DIS |
               ACDCDIG_ADCCLKCTRL_I2STX_CKE_DIS |
               ACDCDIG_ADCCLKCTRL_CKE_BCLKTX_DIS |
               ACDCDIG_ADCCLKCTRL_FILTER_DIS);

    return HAL_OK;
}

/**
 * @brief  Enable ACDCDIG DACCLKCTRL.
 * @param  acdcDig: the handle of acdcDig.
 * @return HAL_Status
 */
static HAL_Status ACDCDIG_DACCLKCTRL_Enable(struct HAL_ACDCDIG_DEV *acdcDig)
{
    struct ACDCDIG_REG *reg = acdcDig->pReg;

    MODIFY_REG(reg->DACCLKCTRL,
               ACDCDIG_DACCLKCTRL_DAC_CKE_MASK |
               ACDCDIG_DACCLKCTRL_I2SRX_CKE_MASK |
               ACDCDIG_DACCLKCTRL_CKE_BCLKRX_MASK |
               ACDCDIG_DACCLKCTRL_DAC_SYNC_ENA_MASK,
               ACDCDIG_DACCLKCTRL_DAC_CKE_EN |
               ACDCDIG_DACCLKCTRL_I2SRX_CKE_EN |
               ACDCDIG_DACCLKCTRL_CKE_BCLKRX_EN |
               ACDCDIG_DACCLKCTRL_DAC_SYNC_EN);

    return HAL_OK;
}

/**
 * @brief  Disable ACDCDIG DACCLKCTRL.
 * @param  acdcDig: the handle of acdcDig.
 * @return HAL_Status
 */
static HAL_Status ACDCDIG_DACCLKCTRL_Disable(struct HAL_ACDCDIG_DEV *acdcDig)
{
    struct ACDCDIG_REG *reg = acdcDig->pReg;

    MODIFY_REG(reg->DACCLKCTRL,
               ACDCDIG_DACCLKCTRL_DAC_CKE_MASK |
               ACDCDIG_DACCLKCTRL_I2SRX_CKE_MASK |
               ACDCDIG_DACCLKCTRL_CKE_BCLKRX_MASK,
               ACDCDIG_DACCLKCTRL_DAC_CKE_DIS |
               ACDCDIG_DACCLKCTRL_I2SRX_CKE_DIS |
               ACDCDIG_DACCLKCTRL_CKE_BCLKRX_DIS);

    return HAL_OK;
}

/**
 * @brief  Select the type of clock sync from ADC or DAC.
 * @param  acdcDig: the handle of acdcDig.
 * @param  stream: audio stream type.
 * @param  sampleRate: sample rate.
 * @return HAL_Status
 */
static HAL_Status ACDCDIG_ClockSyncSelect(struct HAL_ACDCDIG_DEV *acdcDig,
                                          eAUDIO_streamType stream,
                                          eAUDIO_sampleRate sampleRate)
{
    struct ACDCDIG_REG *reg = acdcDig->pReg;
    uint32_t mclkRate = ACDCDIG_MCLK_RATE;
    uint32_t bclkRate, divBclk;
    uint32_t syncClkRate, divSyncClk;
    bool codecIsMaster;

#if (HAL_ACDCDIG_SAMPLERATE_MODE == HAL_ACDCDIG_STRICT_SAMPLERATE)
    if (stream == AUDIO_STREAM_PLAYBACK) {
        if (sampleRate != STRICT_SAMPLERATE_PLAYBACK) {
            return HAL_INVAL;
        }
    } else {
        if (sampleRate != STRICT_SAMPLERATE_CAPTURE) {
            return HAL_INVAL;
        }
    }

    if (acdcDig->enabled == 0) {
        /* Prepare DACINT_DIV just once. */
        syncClkRate = HAL_ACDCDIG_GetSyncClk(STRICT_SAMPLERATE_PLAYBACK);
        HAL_ASSERT(syncClkRate != 0);
        divSyncClk = HAL_DivRoundClosest(mclkRate, syncClkRate);
        MODIFY_REG(reg->DACINT_DIV,
                   ACDCDIG_DACINT_DIV_INT_DIV_CON_MASK,
                   ACDCDIG_DACINT_DIV_INT_DIV(divSyncClk));

        /* Prepare ADCINT_DIV just once. */
        syncClkRate = HAL_ACDCDIG_GetSyncClk(STRICT_SAMPLERATE_CAPTURE);
        HAL_ASSERT(syncClkRate != 0);
        divSyncClk = HAL_DivRoundClosest(mclkRate, syncClkRate);
        MODIFY_REG(reg->ADCINT_DIV,
                   ACDCDIG_ADCINT_DIV_INT_DIV_CON_MASK,
                   ACDCDIG_ADCINT_DIV_INT_DIV(divSyncClk));
    }
#else /* FLEXIBLE_SAMPLERATE */
    syncClkRate = HAL_ACDCDIG_GetSyncClk(sampleRate);
    HAL_ASSERT(syncClkRate != 0);
    divSyncClk = HAL_DivRoundClosest(mclkRate, syncClkRate);

    if (acdcDig->enabled == 0) {
        /**
         * The ADC sync clk is from DACCLK, so we need to prepare both DAC and
         * ADC INT DIV here during capture only.
         */
        MODIFY_REG(reg->DACINT_DIV,
                   ACDCDIG_DACINT_DIV_INT_DIV_CON_MASK,
                   ACDCDIG_DACINT_DIV_INT_DIV(divSyncClk));
        MODIFY_REG(reg->ADCINT_DIV,
                   ACDCDIG_ADCINT_DIV_INT_DIV_CON_MASK,
                   ACDCDIG_ADCINT_DIV_INT_DIV(divSyncClk));
    } else {
        /**
         * Prepare ADCINT_DIV or DACINT_DIV after the other stream are enabled.
         */
        if (stream == AUDIO_STREAM_PLAYBACK) {
            MODIFY_REG(reg->DACINT_DIV,
                       ACDCDIG_DACINT_DIV_INT_DIV_CON_MASK,
                       ACDCDIG_DACINT_DIV_INT_DIV(divSyncClk));
        } else {
            MODIFY_REG(reg->ADCINT_DIV,
                       ACDCDIG_ADCINT_DIV_INT_DIV_CON_MASK,
                       ACDCDIG_ADCINT_DIV_INT_DIV(divSyncClk));
        }
    }

    if (acdcDig->enabled) {
        uint32_t adcIntDiv = READ_REG(reg->ADCINT_DIV) & ACDCDIG_ADCINT_DIV_INT_DIV_CON_MASK;
        uint32_t dacIntDiv = READ_REG(reg->DACINT_DIV) & ACDCDIG_DACINT_DIV_INT_DIV_CON_MASK;
        int syncCount = SYNC_STATUS_COUNT;

        if (adcIntDiv != dacIntDiv) {
            /**
             * If the INT_DIVs are different, means that the sample rates
             * (e.g. 16kHz and 48kHz) are in different mclk groups. Therefore,
             * we need to reset GLB_CKE quickly here.
             */
            MODIFY_REG(reg->SYSCTRL0,
                       ACDCDIG_SYSCTRL0_GLB_CKE_MASK,
                       ACDCDIG_SYSCTRL0_GLB_CKE_DIS);

            while (READ_BIT(reg->ADCCLKCTRL, ACDCDIG_ADCCLKCTRL_ADC_SYNC_STATUS_MASK) ||
                   READ_BIT(reg->DACCLKCTRL, ACDCDIG_DACCLKCTRL_DAC_SYNC_STATUS_MASK)) {
                /**
                 * Ensure both ADC nad DAC sync status are 0, and waiting time
                 * is about 50us here.
                 *
                 * It will return invalid if count is timeout.
                 */
                if (syncCount-- <= 0) {
                    return HAL_INVAL;
                }

                HAL_DelayUs(1);
            }

            MODIFY_REG(reg->SYSCTRL0,
                       ACDCDIG_SYSCTRL0_GLB_CKE_MASK,
                       ACDCDIG_SYSCTRL0_GLB_CKE_EN);
        }
    }
#endif

    if (acdcDig->enabled == 0) {
        ACDCDIG_ADCCLKCTRL_Enable(acdcDig);
        ACDCDIG_DACCLKCTRL_Enable(acdcDig);

        /* Select a remote i2s mclk source and set the mclk freq. */
        HAL_CRU_ClkSetMux(CLK_GET_MUX(CLK_CODEC), CLK_CODEC_SEL_MCLK_MUX);
        HAL_CRU_ClkSetFreq(CLK_CODEC_SEL_MCLK, mclkRate);

        /* Select clock sync is from DAC. */
        MODIFY_REG(reg->SYSCTRL0,
                   ACDCDIG_SYSCTRL0_SYNC_SEL_MASK | ACDCDIG_SYSCTRL0_CLK_SEL_MASK,
                   ACDCDIG_SYSCTRL0_SYNC_SEL_DAC | ACDCDIG_SYSCTRL0_CLK_SEL_DAC);
    }
    acdcDig->enabled++;

    bclkRate = acdcDig->bclkFs * sampleRate;
    HAL_ASSERT(bclkRate);
    divBclk = HAL_DivRoundClosest(mclkRate, bclkRate);

    /* It is ignored when the codec is slave mode. */
    codecIsMaster = (READ_BIT(reg->I2S_CKR[1], ACDCDIG_I2S_CKR1_MSS_MASK) == ACDCDIG_I2S_CKR1_MSS_MASTER);
    if (codecIsMaster) {
        if (stream == AUDIO_STREAM_PLAYBACK) {
            MODIFY_REG(reg->DACSCLKRXINT_DIV, ACDCDIG_DACSCLKRXINT_DIV_SCKRXDIV_MASK,
                       ACDCDIG_DACSCLKRXINT_DIV_SCKRXDIV(divBclk));
            MODIFY_REG(reg->I2S_CKR[0], ACDCDIG_I2S_CKR0_RSD_MASK,
                       ACDCDIG_I2S_CKR0_RSD(acdcDig->bclkFs));
        } else {
            MODIFY_REG(reg->ADCSCLKTXINT_DIV, ACDCDIG_ADCSCLKTXINT_DIV_SCKTXDIV_MASK,
                       ACDCDIG_ADCSCLKTXINT_DIV_SCKTXDIV(divBclk));
            MODIFY_REG(reg->I2S_CKR[0], ACDCDIG_I2S_CKR0_TSD_MASK,
                       ACDCDIG_I2S_CKR0_TSD(acdcDig->bclkFs));
        }
    }

    return HAL_OK;
}

/** @} */
/********************* Public Function Definition ****************************/

/** @defgroup ACDCDIG_Exported_Functions_Group1 Suspend and Resume Functions

 This section provides functions allowing to suspend and resume the module:

 *  @{
 */

/**
 * @brief  acdcDig suspend.
 * @param  acdcDig: the handle of acdcDig.
 * @return HAL_Status
 */
HAL_Status HAL_ACDCDIG_Supsend(struct HAL_ACDCDIG_DEV *acdcDig)
{
    return HAL_OK;
}

/**
 * @brief  acdcDig resume.
 * @param  acdcDig: the handle of acdcDig.
 * @return HAL_Status
 */
HAL_Status HAL_ACDCDIG_Resume(struct HAL_ACDCDIG_DEV *acdcDig)
{
    return HAL_OK;
}

/** @} */

/** @defgroup ACDCDIG_Exported_Functions_Group2 State and Errors Functions

 This section provides functions allowing to get the status of the module:

 *  @{
 */

/** @} */

/** @defgroup ACDCDIG_Exported_Functions_Group3 IO Functions

 This section provides functions allowing to IO controlling:

 *  @{
 */

/** @} */

/** @defgroup ACDCDIG_Exported_Functions_Group4 Init and DeInit Functions

 This section provides functions allowing to init and deinit the module:

 *  @{
 */

/**
 * @brief  Init acdcDig controller.
 * @param  acdcDig: the handle of acdcDig.
 * @param  config: init config for i2s init.
 * @return HAL_Status
 */
HAL_Status HAL_ACDCDIG_Init(struct HAL_ACDCDIG_DEV *acdcDig, struct AUDIO_INIT_CONFIG *config)
{
    struct ACDCDIG_REG *reg = acdcDig->pReg;

    if (acdcDig->enabled == 0) {
        HAL_CRU_ClkEnable(acdcDig->hclk);
    }

#ifdef HAL_I2C_MODULE_ENABLED
    ACDCDIG_I2C_Init(acdcDig);
#endif

    /* Disable volume cross zero detect. */
    MODIFY_REG(reg->ADCVUCTL,
               ACDCDIG_ADCVUCTL_ADC_ZDT_MASK, ACDCDIG_ADCVUCTL_ADC_ZDT_DIS);
    MODIFY_REG(reg->DACVUCTL,
               ACDCDIG_DACVUCTL_DAC_ZDT_MASK, ACDCDIG_DACVUCTL_DAC_ZDT_DIS);

    MODIFY_REG(reg->I2S_CKR[1],
               ACDCDIG_I2S_CKR1_MSS_MASK, config->master ?
               ACDCDIG_I2S_CKR1_MSS_MASTER : ACDCDIG_I2S_CKR1_MSS_SLAVE);

    MODIFY_REG(reg->I2S_CKR[1],
               ACDCDIG_I2S_CKR1_CKP_MASK, config->clkInvert ?
               ACDCDIG_I2S_CKR1_CKP_NEG : ACDCDIG_I2S_CKR1_CKP_POS);

    switch (config->format) {
    case AUDIO_FMT_I2S:
        MODIFY_REG(reg->I2S_TXCR[0],
                   ACDCDIG_I2S_TXCR0_TFS_MASK,
                   ACDCDIG_I2S_TXCR0_TFS_I2S);
        MODIFY_REG(reg->I2S_RXCR[0],
                   ACDCDIG_I2S_RXCR0_TFS_MASK,
                   ACDCDIG_I2S_RXCR0_TFS_I2S);
        break;
    case AUDIO_FMT_PCM:
    case AUDIO_FMT_PCM_DELAY1:
    case AUDIO_FMT_PCM_DELAY2:
    case AUDIO_FMT_PCM_DELAY3:
        MODIFY_REG(reg->I2S_TXCR[0],
                   ACDCDIG_I2S_TXCR0_TFS_MASK |
                   ACDCDIG_I2S_TXCR0_PBM_MASK,
                   ACDCDIG_I2S_TXCR0_TFS_PCM |
                   ACDCDIG_I2S_TXCR0_PBM_PCM_MODE(config->format - AUDIO_FMT_PCM));
        MODIFY_REG(reg->I2S_RXCR[0],
                   ACDCDIG_I2S_RXCR0_TFS_MASK |
                   ACDCDIG_I2S_RXCR0_PBM_MASK,
                   ACDCDIG_I2S_RXCR0_TFS_PCM |
                   ACDCDIG_I2S_RXCR0_PBM_PCM_MODE(config->format - AUDIO_FMT_PCM));
        break;
    default:
        break;
    }
    acdcDig->format = config->format;

    return HAL_OK;
}

/**
 * @brief  DeInit acdcDig controller.
 * @param  acdcDig: the handle of acdcDig.
 * @return HAL_Status
 */
HAL_Status HAL_ACDCDIG_DeInit(struct HAL_ACDCDIG_DEV *acdcDig)
{
#ifdef HAL_I2C_MODULE_ENABLED
    ACDCDIG_I2C_Disable(acdcDig);
    ACDCDIG_I2C_DeInit(acdcDig);
#endif /* HAL_I2C_MODULE_ENABLED */

    if (acdcDig->enabled == 0) {
        HAL_CRU_ClkDisable(acdcDig->hclk);
    }

    return HAL_OK;
}

/** @} */

/** @defgroup ACDCDIG_Exported_Functions_Group5 Other Functions
 *  @{
 */

/**
 * @brief  Enable acdcDig controller.
 * @param  acdcDig: the handle of acdcDig.
 * @param  stream: audio stream type.
 * @return HAL_Status
 */
HAL_Status HAL_ACDCDIG_Enable(struct HAL_ACDCDIG_DEV *acdcDig,
                              eAUDIO_streamType stream)
{
    struct ACDCDIG_REG *reg = acdcDig->pReg;
    uint32_t val = READ_BIT(reg->SYSCTRL0, ACDCDIG_SYSCTRL0_GLB_CKE_EN);
    HAL_Status ret = HAL_OK;

    /* The format is i2s by default */
    if (stream == AUDIO_STREAM_PLAYBACK) {
        MODIFY_REG(reg->I2S_XFER,
                   ACDCDIG_I2S_XFER_RXS_MASK,
                   ACDCDIG_I2S_XFER_RXS_START);

#ifdef HAL_I2C_MODULE_ENABLED
        /* Just used for playback */
        ACDCDIG_I2C_Start(acdcDig);
#endif

        /* Check GLB_CKE whether is enabled. */
        if (!val) {
            MODIFY_REG(reg->SYSCTRL0,
                       ACDCDIG_SYSCTRL0_GLB_CKE_MASK,
                       ACDCDIG_SYSCTRL0_GLB_CKE_EN);
        }

        /* There is MONO channel for DAC */
        MODIFY_REG(reg->DACDIGEN,
                   ACDCDIG_DACDIGEN_DAC_GLBEN_MASK |
                   ACDCDIG_DACDIGEN_DACEN_L0_MASK,
                   ACDCDIG_DACDIGEN_DAC_GLB_EN |
                   ACDCDIG_DACDIGEN_DAC_L0_EN);
    } else {
        MODIFY_REG(reg->I2S_XFER,
                   ACDCDIG_I2S_XFER_TXS_MASK,
                   ACDCDIG_I2S_XFER_TXS_START);

        /* Check GLB_CKE whether is enabled. */
        if (!val) {
            MODIFY_REG(reg->SYSCTRL0,
                       ACDCDIG_SYSCTRL0_GLB_CKE_MASK,
                       ACDCDIG_SYSCTRL0_GLB_CKE_EN);
        }

        val = READ_REG(reg->I2S_TXCR[1]) & ACDCDIG_I2S_TXCR1_TCSR_MASK;
        switch (val >> ACDCDIG_I2S_TXCR1_TCSR_SHIFT) {
        case 1: /* 4 channels */
            val = ACDCDIG_ADCDIGEN_ADC_GLB_EN |
                  ACDCDIG_ADCDIGEN_ADC_L2_EN |
                  ACDCDIG_ADCDIGEN_ADC_L0R1_EN;
            break;
        case 0: /* 2 channels */
            val = ACDCDIG_ADCDIGEN_ADC_GLB_EN |
                  ACDCDIG_ADCDIGEN_ADC_L2_DIS |
                  ACDCDIG_ADCDIGEN_ADC_L0R1_EN;
            break;
        default:

            return HAL_INVAL;
        }

        /* There is max 3 channel for ADC */
        MODIFY_REG(reg->ADCDIGEN,
                   ACDCDIG_ADCDIGEN_ADC_GLBEN_MASK |
                   ACDCDIG_ADCDIGEN_ADCEN_L2_MASK |
                   ACDCDIG_ADCDIGEN_ADCEN_L0R1_MASK,
                   val);
    }

    return ret;
}

/**
 * @brief  Disable acdcDig controller.
 * @param  acdcDig: the handle of acdcDig.
 * @param  stream: audio stream type.
 * @return HAL_Status
 */
HAL_Status HAL_ACDCDIG_Disable(struct HAL_ACDCDIG_DEV *acdcDig,
                               eAUDIO_streamType stream)
{
    struct ACDCDIG_REG *reg = acdcDig->pReg;
    HAL_Status ret = HAL_OK;

    if (stream == AUDIO_STREAM_PLAYBACK) {
#ifdef HAL_I2C_MODULE_ENABLED
        /* Just used for playback */
        ACDCDIG_I2C_Stop(acdcDig);
#endif /* HAL_I2C_MODULE_ENABLED */

        MODIFY_REG(reg->I2S_XFER,
                   ACDCDIG_I2S_XFER_RXS_MASK,
                   ACDCDIG_I2S_XFER_RXS_STOP);
        MODIFY_REG(reg->I2S_CLR,
                   ACDCDIG_I2S_CLR_RXC_MASK,
                   ACDCDIG_I2S_CLR_RXC);

        /* There is MONO channel for DAC */
        MODIFY_REG(reg->DACDIGEN,
                   ACDCDIG_DACDIGEN_DAC_GLBEN_MASK |
                   ACDCDIG_DACDIGEN_DACEN_L0_MASK,
                   ACDCDIG_DACDIGEN_DAC_GLB_DIS |
                   ACDCDIG_DACDIGEN_DAC_L0_DIS);
    } else {
        MODIFY_REG(reg->I2S_XFER,
                   ACDCDIG_I2S_XFER_TXS_MASK,
                   ACDCDIG_I2S_XFER_TXS_STOP);
        MODIFY_REG(reg->I2S_CLR,
                   ACDCDIG_I2S_CLR_TXC_MASK,
                   ACDCDIG_I2S_CLR_TXC);

        /* Disable HPF for ADC */
        MODIFY_REG(reg->ADCHPFEN,
                   ACDCDIG_ADCHPFEN_HPFEN_L0_MASK |
                   ACDCDIG_ADCHPFEN_HPFEN_R1_MASK |
                   ACDCDIG_ADCHPFEN_HPFEN_L2_MASK,
                   ACDCDIG_ADCHPFEN_HPFDIS_L0 |
                   ACDCDIG_ADCHPFEN_HPFDIS_R1 |
                   ACDCDIG_ADCHPFEN_HPFDIS_L2);

        /* There is max 3 channel for ADC */
        MODIFY_REG(reg->ADCDIGEN,
                   ACDCDIG_ADCDIGEN_ADC_GLBEN_MASK |
                   ACDCDIG_ADCDIGEN_ADCEN_L2_MASK |
                   ACDCDIG_ADCDIGEN_ADCEN_L0R1_MASK,
                   ACDCDIG_ADCDIGEN_ADC_GLB_DIS |
                   ACDCDIG_ADCDIGEN_ADC_L2_DIS |
                   ACDCDIG_ADCDIGEN_ADC_L0R1_DIS);
    }

    acdcDig->enabled--;
    if (acdcDig->enabled == 0) {
        MODIFY_REG(reg->SYSCTRL0,
                   ACDCDIG_SYSCTRL0_GLB_CKE_MASK,
                   ACDCDIG_SYSCTRL0_GLB_CKE_DIS);

        ACDCDIG_DACCLKCTRL_Disable(acdcDig);
        ACDCDIG_ADCCLKCTRL_Disable(acdcDig);
    }

    return ret;
}

/**
 * @brief  Set acdcDig clock.
 * @param  acdcDig: the handle of acdcDig.
 * @param  stream: audio stream type.
 * @param  freq: audio clock freq.
 * @return HAL_Status
 */
HAL_Status HAL_ACDCDIG_SetClock(struct HAL_ACDCDIG_DEV *acdcDig,
                                eAUDIO_streamType stream,
                                uint32_t freq)
{
    return HAL_OK;
}

/**
 * @brief  Config acdcDig controller.
 * @param  acdcDig: the handle of acdcDig.
 * @param  stream: audio stream type.
 * @param  params: audio params.
 * @return HAL_Status
 */
HAL_Status HAL_ACDCDIG_Config(struct HAL_ACDCDIG_DEV *acdcDig,
                              eAUDIO_streamType stream,
                              struct AUDIO_PARAMS *params)
{
    struct ACDCDIG_REG *reg = acdcDig->pReg;
    HAL_Status ret = HAL_OK;
    uint32_t srt = 0, hpf = 0;

    ACDCDIG_ClockSyncSelect(acdcDig, stream, params->sampleRate);

    if (stream == AUDIO_STREAM_PLAYBACK) {
        switch (params->sampleRate) {
        case AUDIO_SAMPLERATE_8000:
        case AUDIO_SAMPLERATE_11025:
        case AUDIO_SAMPLERATE_12000:
            srt = 0;
            break;
        case AUDIO_SAMPLERATE_16000:
        case AUDIO_SAMPLERATE_22050:
        case AUDIO_SAMPLERATE_24000:
            srt = 1;
            break;
        case AUDIO_SAMPLERATE_32000:
        case AUDIO_SAMPLERATE_44100:
        case AUDIO_SAMPLERATE_48000:
            srt = 2;
            break;
        case AUDIO_SAMPLERATE_64000:
        case AUDIO_SAMPLERATE_88200:
        case AUDIO_SAMPLERATE_96000:
            srt = 3;
            break;
        case AUDIO_SAMPLERATE_128000:
        case AUDIO_SAMPLERATE_176400:
        case AUDIO_SAMPLERATE_192000:
            srt = 4;
            break;
        default:
            ret = HAL_INVAL;
            break;
        }

        MODIFY_REG(reg->DACCFG1, ACDCDIG_DACCFG1_DACSRT_MASK,
                   ACDCDIG_DACCFG1_DACSRT(srt));
        switch (params->sampleBits) {
        case AUDIO_SAMPLEBITS_16:
            MODIFY_REG(reg->I2S_RXCR[0], ACDCDIG_I2S_RXCR0_VDW_MASK,
                       ACDCDIG_I2S_RXCR0_VDW(16));
            break;
        case AUDIO_SAMPLEBITS_24:
        case AUDIO_SAMPLEBITS_32:
            MODIFY_REG(reg->I2S_RXCR[0], ACDCDIG_I2S_RXCR0_VDW_MASK,
                       ACDCDIG_I2S_RXCR0_VDW(24));
            break;
        default:
            ret = HAL_INVAL;
            break;
        }
    } else {
        switch (params->sampleRate) {
        case AUDIO_SAMPLERATE_8000:
        case AUDIO_SAMPLERATE_11025:
        case AUDIO_SAMPLERATE_12000:
            srt = 0;
            break;
        case AUDIO_SAMPLERATE_16000:
        case AUDIO_SAMPLERATE_22050:
        case AUDIO_SAMPLERATE_24000:
            srt = 1;
            break;
        case AUDIO_SAMPLERATE_32000:
            srt = 2;
            break;
        case AUDIO_SAMPLERATE_44100:
        case AUDIO_SAMPLERATE_48000:
            srt = 3;
            break;
        case AUDIO_SAMPLERATE_64000:
        case AUDIO_SAMPLERATE_88200:
        case AUDIO_SAMPLERATE_96000:
            srt = 4;
            break;
        case AUDIO_SAMPLERATE_128000:
        case AUDIO_SAMPLERATE_176400:
        case AUDIO_SAMPLERATE_192000:
            srt = 5;
            break;
        default:
            ret = HAL_INVAL;
            break;
        }

        MODIFY_REG(reg->ADCCFG1, ACDCDIG_ADCCFG1_ADCSRT_MASK,
                   ACDCDIG_ADCCFG1_ADCSRT(srt));
        switch (params->sampleBits) {
        case AUDIO_SAMPLEBITS_16:
            MODIFY_REG(reg->I2S_TXCR[0], ACDCDIG_I2S_TXCR0_VDW_MASK,
                       ACDCDIG_I2S_TXCR0_VDW(16));
            break;
        case AUDIO_SAMPLEBITS_24:
        case AUDIO_SAMPLEBITS_32:
            MODIFY_REG(reg->I2S_TXCR[0], ACDCDIG_I2S_TXCR0_VDW_MASK,
                       ACDCDIG_I2S_TXCR0_VDW(24));
            break;
        default:
            ret = HAL_INVAL;
            break;
        }

        switch (params->channels) {
        case 4:
            srt = 1;
            hpf = ACDCDIG_ADCHPFEN_HPFEN_L0 |
                  ACDCDIG_ADCHPFEN_HPFEN_R1 |
                  ACDCDIG_ADCHPFEN_HPFEN_L2;
            break;
        case 2:
            srt = 0;
            hpf = ACDCDIG_ADCHPFEN_HPFEN_L0 |
                  ACDCDIG_ADCHPFEN_HPFEN_R1 |
                  ACDCDIG_ADCHPFEN_HPFDIS_L2;
            break;
        default:

            return HAL_INVAL;
        }

        /* Enable HPF for ADC */
        MODIFY_REG(reg->ADCHPFCF,
                   ACDCDIG_ADCHPFCF_HPFCF_MASK,
                   ACDCDIG_ADCHPFCF_HPFCF_60HZ);
        MODIFY_REG(reg->ADCHPFEN,
                   ACDCDIG_ADCHPFEN_HPFEN_L0_MASK |
                   ACDCDIG_ADCHPFEN_HPFEN_R1_MASK |
                   ACDCDIG_ADCHPFEN_HPFEN_L2_MASK,
                   hpf);

        MODIFY_REG(reg->I2S_TXCR[1], ACDCDIG_I2S_TXCR1_TCSR_MASK,
                   ACDCDIG_I2S_TXCR1_TCSR(srt));
    }

    return ret;
}

/**
 * @brief  Set Gain for acdcDig.
 * @param  acdcDig: the handle of acdcDig.
 * @param  stream: audio stream type.
 * @param  dBConfig: The gains config of codec, it includes ADC/DAC channel,
 *                   and the dB range is: -95625(-95.625dB) ~ 95625(95.625dB).
 *                   Similar to Linux ALSA TLV dB scale, they are scaled 1000
 *                   times to avoid representation of fractional parts.
 * @return HAL_Status
 */
HAL_Status HAL_ACDCDIG_SetGain(struct HAL_ACDCDIG_DEV *acdcDig,
                               eAUDIO_streamType stream,
                               struct AUDIO_DB_CONFIG *dBConfig)
{
    struct ACDCDIG_REG *reg = acdcDig->pReg;
    uint32_t mask, val;
    int dB;

    HAL_ASSERT(dBConfig != NULL);

    dB = dBConfig->dB;

    switch (stream) {
    case AUDIO_STREAM_PLAYBACK:
        HAL_ASSERT(dBConfig->ch <= ACDCDIG_DAC_NUM);

        if (dB > 0) {
            if (dB > ACDCDIG_DB_MAX) {
                dB = ACDCDIG_DB_MAX;
            }

            dB = dB / ACDCDIG_DB_STEP;
            MODIFY_REG(reg->DACVOGP,
                       ACDCDIG_DACVOGP_VOLGPL0_MASK,
                       ACDCDIG_DACVOGP_VOLGPL0_POS_GAIN);
        } else {
            if (dB < ACDCDIG_DB_MIN) {
                dB = ACDCDIG_DB_MIN;
            }

            dB = -(dB / ACDCDIG_DB_STEP);
            MODIFY_REG(reg->DACVOGP,
                       ACDCDIG_DACVOGP_VOLGPL0_MASK,
                       ACDCDIG_DACVOGP_VOLGPL0_NEG_GAIN);
        }

        WRITE_REG(reg->DACVOLL0, ACDCDIG_DACVOL(dB));
        break;
    case AUDIO_STREAM_CAPTURE:
        HAL_ASSERT(dBConfig->ch <= ACDCDIG_ADC_NUM);

        if (dB > 0) {
            if (dB > ACDCDIG_DB_MAX) {
                dB = ACDCDIG_DB_MAX;
            }

            dB = dB / ACDCDIG_DB_STEP;

            if (dBConfig->ch == 0) {
                mask = ACDCDIG_ADCVOGP_VOLGPL0_MASK;
                val = ACDCDIG_ADCVOGP_VOLGPL0_POS_GAIN;
                WRITE_REG(reg->ADCVOLL[0], ACDCDIG_ADCVOL(dB));
            } else if (dBConfig->ch == 1) {
                mask = ACDCDIG_ADCVOGP_VOLGPR1_MASK;
                val = ACDCDIG_ADCVOGP_VOLGPR1_POS_GAIN;
                WRITE_REG(reg->ADCVOLR0, ACDCDIG_ADCVOL(dB));
            } else if (dBConfig->ch == 2) {
                mask = ACDCDIG_ADCVOGP_VOLGPL2_MASK;
                val = ACDCDIG_ADCVOGP_VOLGPL2_POS_GAIN;
                WRITE_REG(reg->ADCVOLL[1], ACDCDIG_ADCVOL(dB));
            } else {
                /* The dBConfig->ch == 3 and set all ADC gains. */
                mask = ACDCDIG_ADCVOGP_VOLGPL0_MASK |
                       ACDCDIG_ADCVOGP_VOLGPR1_MASK |
                       ACDCDIG_ADCVOGP_VOLGPL2_MASK;
                val = ACDCDIG_ADCVOGP_VOLGPL0_POS_GAIN |
                      ACDCDIG_ADCVOGP_VOLGPR1_POS_GAIN |
                      ACDCDIG_ADCVOGP_VOLGPL2_POS_GAIN;
                WRITE_REG(reg->ADCVOLL[0], ACDCDIG_ADCVOL(dB));
                WRITE_REG(reg->ADCVOLR0, ACDCDIG_ADCVOL(dB));
                WRITE_REG(reg->ADCVOLL[1], ACDCDIG_ADCVOL(dB));
            }
        } else {
            if (dB < ACDCDIG_DB_MIN) {
                dB = ACDCDIG_DB_MIN;
            }

            dB = -(dB / ACDCDIG_DB_STEP);

            if (dBConfig->ch == 0) {
                mask = ACDCDIG_ADCVOGP_VOLGPL0_MASK;
                val = ACDCDIG_ADCVOGP_VOLGPL0_NEG_GAIN;
                WRITE_REG(reg->ADCVOLL[0], ACDCDIG_ADCVOL(dB));
            } else if (dBConfig->ch == 1) {
                mask = ACDCDIG_ADCVOGP_VOLGPR1_MASK;
                val = ACDCDIG_ADCVOGP_VOLGPR1_NEG_GAIN;
                WRITE_REG(reg->ADCVOLR0, ACDCDIG_ADCVOL(dB));
            } else if (dBConfig->ch == 2) {
                mask = ACDCDIG_ADCVOGP_VOLGPL2_MASK;
                val = ACDCDIG_ADCVOGP_VOLGPL2_NEG_GAIN;
                WRITE_REG(reg->ADCVOLL[1], ACDCDIG_ADCVOL(dB));
            } else {
                /* The dBConfig->ch == 3 and set all ADC gains. */
                mask = ACDCDIG_ADCVOGP_VOLGPL0_MASK |
                       ACDCDIG_ADCVOGP_VOLGPR1_MASK |
                       ACDCDIG_ADCVOGP_VOLGPL2_MASK;
                val = ACDCDIG_ADCVOGP_VOLGPL0_NEG_GAIN |
                      ACDCDIG_ADCVOGP_VOLGPR1_NEG_GAIN |
                      ACDCDIG_ADCVOGP_VOLGPL2_NEG_GAIN;

                WRITE_REG(reg->ADCVOLL[0], ACDCDIG_ADCVOL(dB));
                WRITE_REG(reg->ADCVOLR0, ACDCDIG_ADCVOL(dB));
                WRITE_REG(reg->ADCVOLL[1], ACDCDIG_ADCVOL(dB));
            }
        }

        MODIFY_REG(reg->ADCVOGP, mask, val);
        break;
    default:

        return HAL_ERROR;
    }

    return HAL_OK;
}

/**
 * @brief  Get Gain for acdcDig.
 * @param  acdcDig: the handle of acdcDig.
 * @param  stream: audio stream type.
 * @param  dBConfig: The gains config of codec, it includes ADC/DAC channel,
 *                   and the dB range is: -95625(-95.625dB) ~ 95625(95.625dB).
 *                   Similar to Linux ALSA TLV dB scale, they are scaled 1000
 *                   times to avoid representation of fractional parts.
 * @return HAL_Status
 */
HAL_Status HAL_ACDCDIG_GetGain(struct HAL_ACDCDIG_DEV *acdcDig,
                               eAUDIO_streamType stream,
                               struct AUDIO_DB_CONFIG *dBConfig)
{
    struct ACDCDIG_REG *reg = acdcDig->pReg;
    uint8_t vol;

    HAL_ASSERT(dBConfig != NULL);

    switch (stream) {
    case AUDIO_STREAM_PLAYBACK:
        HAL_ASSERT(dBConfig->ch <= ACDCDIG_DAC_NUM);

        vol = READ_REG(reg->DACVOLL0);
        if (READ_REG(reg->DACVOGP)) {
            dBConfig->dB = vol * ACDCDIG_DB_STEP; /* Positive gain */
        } else {
            dBConfig->dB = -(vol * ACDCDIG_DB_STEP); /* Negative gain */
        }
        break;
    case AUDIO_STREAM_CAPTURE:
        /* Need to specify ADC channel here. */
        HAL_ASSERT(dBConfig->ch < ACDCDIG_ADC_NUM);

        /* There are the same volumes for 3ADCs */
        if (dBConfig->ch == 0) {
            vol = READ_REG(reg->ADCVOLL[0]);
        } else if (dBConfig->ch == 1) {
            vol = READ_REG(reg->ADCVOLR0);
        } else { /* The array ADCVOLL[1] is left channel 2 */
            vol = READ_REG(reg->ADCVOLL[1]);
        }

        if (READ_BIT(reg->ADCVOGP, 1 << dBConfig->ch)) {
            dBConfig->dB = vol * ACDCDIG_DB_STEP;   /* Positive gain */
        } else {
            dBConfig->dB = -(vol * ACDCDIG_DB_STEP);   /* Negative gain */
        }
        break;
    default:

        return HAL_ERROR;
    }

    return HAL_OK;
}

/**
 * @brief  Get Gain informations from acdcDig.
 * @param  acdcDig: the handle of acdcDig.
 * @param  info: the handle of gain info.
 * @return HAL_Status
 */
HAL_Status HAL_ACDCDIG_GetGainInfo(struct HAL_ACDCDIG_DEV *acdcDig,
                                   struct AUDIO_GAIN_INFO *info)
{
    if (!info) {
        return HAL_ERROR;
    }

    info->mindB = ACDCDIG_DB_MIN;
    info->maxdB = ACDCDIG_DB_MAX;
    info->step = ACDCDIG_DB_STEP;

    return HAL_OK;
}

/**
 * @brief  Request ACDCDIG to use i2c by codec or MCU.
 * @param  acdcDig: the handle of acdcDig.
 * @param  i2cUsed: codec or MCU to use i2c.
 * @return HAL_Status
 */
HAL_Status HAL_ACDCDIG_RequestI2C(struct HAL_ACDCDIG_DEV *acdcDig,
                                  eACDCDIG_i2cUsed i2cUsed)
{
#ifdef HAL_I2C_MODULE_ENABLED
    struct GRF_REG *pGRF = (struct GRF_REG *)(GRF_BASE);
    uint32_t val = GRF_CODEC_I2C_TRANS;

    HAL_ASSERT(acdcDig);
    HAL_ASSERT((i2cUsed == CODEC_USE_I2C) ||
               ((i2cUsed == MCU_USE_I2C)));

    /* Checking the I2C whether is busy */
    if (READ_BIT((acdcDig->pReg->I2C_CON[0]), I2C_CON_START_MASK)) {
        return HAL_BUSY;
    }

    if (i2cUsed == MCU_USE_I2C) {
        val = GRF_MCU_I2C_TRANS;
    }

    WRITE_REG(pGRF->SOC_CON16, val);
#endif /* HAL_I2C_MODULE_ENABLED */

    return HAL_OK;
}

/**
 * @brief  Check I2C used by which module.
 * @return eACDCDIG_i2cUsed
 */
eACDCDIG_i2cUsed HAL_ACDCDIG_CheckI2C(struct HAL_ACDCDIG_DEV *acdcDig)
{
#ifdef HAL_I2C_MODULE_ENABLED
    struct GRF_REG *pGRF = (struct GRF_REG *)(GRF_BASE);
    eACDCDIG_i2cUsed i2cUsed = CODEC_USE_I2C;
    uint32_t status;

    HAL_ASSERT(acdcDig);

    status = READ_BIT(pGRF->SOC_STATUS,
                      GRF_SOC_STATUS_GRF_CODEC_I2C_TRANS_ACK_MASK);
    if (status) {
        i2cUsed = MCU_USE_I2C;
    }

    return i2cUsed;
#else

    /* Here return MCU_USE_I2C directly if codec doesn't reference I2C. */
    return MCU_USE_I2C;
#endif
}

/**
 * @brief  Get a suitable sync clock for codec during different sample rates.
 * @param  sampleRate: sample rate.
 * @return syncClkRate
 */
uint32_t HAL_ACDCDIG_GetSyncClk(eAUDIO_sampleRate sampleRate)
{
    uint32_t syncClkRate;

    switch (sampleRate) {
    case AUDIO_SAMPLERATE_12000:
    case AUDIO_SAMPLERATE_24000:
    case AUDIO_SAMPLERATE_48000:
    case AUDIO_SAMPLERATE_96000:
    case AUDIO_SAMPLERATE_192000:
        syncClkRate = ACDCDIG_GROUP0_SYNCCLK;
        break;
    case AUDIO_SAMPLERATE_11025:
    case AUDIO_SAMPLERATE_22050:
    case AUDIO_SAMPLERATE_44100:
    case AUDIO_SAMPLERATE_88200:
    case AUDIO_SAMPLERATE_176400:
        syncClkRate = ACDCDIG_GROUP1_SYNCCLK;
        break;
    case AUDIO_SAMPLERATE_8000:
    case AUDIO_SAMPLERATE_16000:
    case AUDIO_SAMPLERATE_32000:
    case AUDIO_SAMPLERATE_64000:
    case AUDIO_SAMPLERATE_128000:
        syncClkRate = ACDCDIG_GROUP2_SYNCCLK;
        break;
    default:
        syncClkRate = 0;
        break;
    }

    return syncClkRate;
}

/** @} */

/** @} */

/** @} */

#endif /* (ACDCDIG_VERSION == 0x2U) */
#endif /* HAL_ACDCDIG_MODULE_ENABLED */
