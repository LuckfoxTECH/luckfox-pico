/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#if defined(HAL_ACODEC_MODULE_ENABLED) && defined(SOC_RK3308)

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
#define ACODEC_VERSION_A 0xa
#define ACODEC_VERSION_B 0xb
#define ACODEC_VERSION_C 0xc

/* RK3308_GLB_CON - REG: 0x0000 */
#define RK3308_ADC_BIST_WORK  (1 << 7)
#define RK3308_ADC_BIST_RESET (0 << 7)
#define RK3308_DAC_BIST_WORK  (1 << 6)
#define RK3308_DAC_BIST_RESET (0 << 6)
#define RK3308_ADC_MCLK_MSK   (1 << 5)
#define RK3308_ADC_MCLK_DIS   (1 << 5)
#define RK3308_ADC_MCLK_EN    (0 << 5)
#define RK3308_DAC_MCLK_MSK   (1 << 4)
#define RK3308_DAC_MCLK_DIS   (1 << 4)
#define RK3308_DAC_MCLK_EN    (0 << 4)
#define RK3308_CODEC_RST_MSK  (0x7 << 0)
#define RK3308_ADC_DIG_WORK   (1 << 2)
#define RK3308_ADC_DIG_RESET  (0 << 2)
#define RK3308_DAC_DIG_WORK   (1 << 1)
#define RK3308_DAC_DIG_RESET  (0 << 1)
#define RK3308_SYS_WORK       (1 << 0)
#define RK3308_SYS_RESET      (0 << 0)

#define ADC_LR_GROUP_MAX 4

#define ADC_GRP(base, ch)                                                          \
    ({                                                                             \
        volatile uint32_t *addr = &base;                                           \
        (volatile uint32_t *)((((ch) & 0x3) * 0x40) + (volatile uint8_t *)(addr)); \
    })

/********************* Private Structure Definition **************************/
typedef enum {
    DAC_LINEOUT = 0,
    DAC_HPOUT,
    DAC_LINEOUT_HPOUT,
} eACODEC_route;

struct ACODEC_PRIV {
    uint32_t codecVer;
    eACODEC_route dacRoute;
    bool inited;
};

static struct ACODEC_PRIV g_acodecPriv;

/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/

/**
 * @brief  soft reset.
 * @param  acodec: the handle of acodec.
 * @return none
 */
static void ACODEC_Reset(struct HAL_ACODEC_DEV *acodec)
{
    struct ACODEC_REG *reg = acodec->pReg;

    HAL_CRU_ClkResetAssert(SRST_P_ACODEC);
    HAL_DelayMs(10);
    HAL_CRU_ClkResetDeassert(SRST_P_ACODEC);

    WRITE_REG(reg->GLB_CON, 0x00);
    HAL_DelayMs(10);
    WRITE_REG(reg->GLB_CON,
              RK3308_SYS_WORK |
              RK3308_DAC_DIG_WORK |
              RK3308_ADC_DIG_WORK);
}

static int ACODEC_ADCDIG_Reset(struct HAL_ACODEC_DEV *acodec)
{
    struct ACODEC_REG *reg = acodec->pReg;

    MODIFY_REG(reg->GLB_CON,
               RK3308_ADC_DIG_WORK,
               RK3308_ADC_DIG_RESET);
    HAL_DelayUs(50);
    MODIFY_REG(reg->GLB_CON,
               RK3308_ADC_DIG_WORK,
               RK3308_ADC_DIG_WORK);

    return 0;
}

static int ACODEC_DACDIG_Reset(struct HAL_ACODEC_DEV *acodec)
{
    struct ACODEC_REG *reg = acodec->pReg;

    MODIFY_REG(reg->GLB_CON,
               RK3308_DAC_DIG_WORK,
               RK3308_DAC_DIG_RESET);
    HAL_DelayMs(10);
    // HAL_DelayUs(50);
    MODIFY_REG(reg->GLB_CON,
               RK3308_DAC_DIG_WORK,
               RK3308_DAC_DIG_WORK);

    return 0;
}

/**
 * @brief  power on.
 * @param  acodec: the handle of acodec.
 * @return none
 */
static void ACODEC_PowerOn(struct HAL_ACODEC_DEV *acodec)
{
    struct ACODEC_REG *reg = acodec->pReg;
    struct ACODEC_PRIV *acodecPriv = acodec->privateData;
    uint32_t v;

    HAL_ASSERT(acodecPriv);

    /* 0. Supply the power of digital part and reset the Audio Codec */
    /* Do nothing */

    /*
     * 1. Configure ACODEC_DAC_ANA_CON1[1:0] and ACODEC_DAC_ANA_CON1[5:4]
     *    to 0x1, to setup dc voltage of the DAC channel output.
     */
    MODIFY_REG(reg->DAC_ANA_CON1,
               RK3308_DAC_HPOUT_POP_SOUND_L_MSK,
               RK3308_DAC_HPOUT_POP_SOUND_L_INIT);
    MODIFY_REG(reg->DAC_ANA_CON1,
               RK3308_DAC_HPOUT_POP_SOUND_R_MSK,
               RK3308_DAC_HPOUT_POP_SOUND_R_INIT);

    if (acodecPriv->codecVer >= ACODEC_VERSION_B) {
        /*
         * 2. Configure ACODEC_DAC_ANA_CON15[1:0] and
         *    ACODEC_DAC_ANA_CON15[5:4] to 0x1, to setup dc voltage of
         *    the DAC channel output.
         */
        MODIFY_REG(reg->DAC_ANA_CON15,
                   RK3308_DAC_LINEOUT_POP_SOUND_L_MSK,
                   RK3308_DAC_L_SEL_DC_FROM_VCM);
        MODIFY_REG(reg->DAC_ANA_CON15,
                   RK3308_DAC_LINEOUT_POP_SOUND_R_MSK,
                   RK3308_DAC_R_SEL_DC_FROM_VCM);
    }

    /*
     * 3. Configure the register ACODEC_ADC_ANA_CON10[3:0] to 7?b000_0001.
     */
    MODIFY_REG((*ADC_GRP(reg->ADC_ANA_CON10, 0)),
               RK3308_ADC_CURRENT_CHARGE_MSK,
               RK3308_ADC_SEL_I(0x1));

    if (acodecPriv->codecVer >= ACODEC_VERSION_B) {
        /*
         * 4. Configure the register ACODEC_ADC_ANA_CON14[3:0] to
         *    4?b0001.
         */
        MODIFY_REG(reg->DAC_ANA_CON14,
                   RK3308_DAC_CURRENT_CHARGE_MSK,
                   RK3308_DAC_SEL_I(0x1));
    }

    /* 5. Supply the power of the analog part(AVDD,AVDDRV) */

    /*
     * 6. Configure the register ACODEC_ADC_ANA_CON10[7] to 0x1 to setup
     *    reference voltage
     */

    MODIFY_REG((*ADC_GRP(reg->ADC_ANA_CON10, 0)),
               RK3308_ADC_REF_EN, RK3308_ADC_REF_EN);

    if (acodecPriv->codecVer >= ACODEC_VERSION_B) {
        /*
         * 7. Configure the register ACODEC_ADC_ANA_CON14[4] to 0x1 to
         *    setup reference voltage
         */
        MODIFY_REG(reg->DAC_ANA_CON14,
                   RK3308_DAC_VCM_LINEOUT_EN,
                   RK3308_DAC_VCM_LINEOUT_EN);
    }

    /*
     * 8. Change the register ACODEC_ADC_ANA_CON10[6:0] from the 0x1 to
     *    0x7f step by step or configure the ACODEC_ADC_ANA_CON10[6:0] to
     *    0x7f directly. Here the slot time of the step is 200us.
     */
    for (v = 0x1; v <= 0x7f; v++) {
        MODIFY_REG((*ADC_GRP(reg->ADC_ANA_CON10, 0)),
                   RK3308_ADC_CURRENT_CHARGE_MSK,
                   v);
        HAL_DelayUs(200);
    }

    if (acodecPriv->codecVer >= ACODEC_VERSION_B) {
        /*
         * 9. Change the register ACODEC_ADC_ANA_CON14[3:0] from the 0x1
         *    to 0xf step by step or configure the
         *    ACODEC_ADC_ANA_CON14[3:0] to 0xf directly. Here the slot
         *    time of the step is 200us.
         */
        for (v = 0x1; v <= 0xf; v++) {
            MODIFY_REG(reg->DAC_ANA_CON14,
                       RK3308_DAC_CURRENT_CHARGE_MSK,
                       v);
            HAL_DelayUs(200);
        }
    }

    /* 10. Wait until the voltage of VCM keeps stable at the AVDD/2 */
    HAL_DelayMs(20); /* estimated value */

    /*
     * 11. Configure the register ACODEC_ADC_ANA_CON10[6:0] to the
     *     appropriate value(expect 0x0) for reducing power.
     */
    MODIFY_REG((*ADC_GRP(reg->ADC_ANA_CON10, 0)),
               RK3308_ADC_CURRENT_CHARGE_MSK, 0x7c);

    if (acodecPriv->codecVer >= ACODEC_VERSION_B) {
        /*
         * 12. Configure the register ACODEC_DAC_ANA_CON14[6:0] to the
         *     appropriate value(expect 0x0) for reducing power.
         */
        MODIFY_REG(reg->DAC_ANA_CON14,
                   RK3308_DAC_CURRENT_CHARGE_MSK, 0xf);
    }

    if (acodecPriv->codecVer == ACODEC_VERSION_C) {
        /* Using large driver strength for HPOUT and LINEOUT */
        WRITE_REG(reg->DAC_ANA_CON7, 0x11);
        WRITE_REG(reg->DAC_ANA_CON8, 0x11);
    }
}

static int ACODEC_SetDefaultGains(struct HAL_ACODEC_DEV *acodec)
{
    struct ACODEC_REG *reg = acodec->pReg;
    struct ACODEC_PRIV *acodecPriv = acodec->privateData;
    int grp;

    /* Prepare ADC gains */
    /* vendor step 12, set MIC PGA default gains */
    for (grp = 0; grp < ADC_LR_GROUP_MAX; grp++) {
        MODIFY_REG((*ADC_GRP(reg->ADC_ANA_CON1, grp)),
                   RK3308_ADC_CH1_MIC_GAIN_MSK |
                   RK3308_ADC_CH2_MIC_GAIN_MSK,
                   RK3308_ADC_CH1_MIC_GAIN_0DB |
                   RK3308_ADC_CH2_MIC_GAIN_0DB);
    }

    /* vendor step 13, set ALC default gains */
    for (grp = 0; grp < ADC_LR_GROUP_MAX; grp++) {
        MODIFY_REG((*ADC_GRP(reg->ADC_ANA_CON3, grp)),
                   RK3308_ADC_CH1_ALC_GAIN_MSK,
                   RK3308_ADC_CH1_ALC_GAIN_0DB);
        MODIFY_REG((*ADC_GRP(reg->ADC_ANA_CON4, grp)),
                   RK3308_ADC_CH2_ALC_GAIN_MSK,
                   RK3308_ADC_CH2_ALC_GAIN_0DB);
    }

#if 0 // TODO for rk3308bs new rigsters
    if (acodecPriv->codecVer == ACODEC_VERSION_C) {
        /* recover ADC digtial volume to 0dB */
        for (grp = 0; grp < ADC_LR_GROUP_MAX; grp++) {
            /* DIG_VOL: -97dB ~ +32dB  */
            regAddr = ADC_GRP(reg->ADC_DIG_CON5, grp);
            WRITE_REG(regAddr,
                      RK3308_ADC_DIG_VOL_CON_L(RK3308_ADC_DIG_VOL_0DB));
            regAddr = ADC_GRP(reg->ADC_DIG_CON6, grp);
            WRITE_REG(regAddr,
                      RK3308_ADC_DIG_VOL_CON_R(RK3308_ADC_DIG_VOL_0DB));
        }
    }
#endif

    /* Prepare DAC gains */
    /* Step 15, set HPMIX default gains */
    MODIFY_REG(reg->DAC_ANA_CON12,
               RK3308_DAC_L_HPMIX_GAIN_MSK |
               RK3308_DAC_R_HPMIX_GAIN_MSK,
               RK3308_DAC_L_HPMIX_GAIN_0DB |
               RK3308_DAC_R_HPMIX_GAIN_0DB);

    /* Step 18, set HPOUT default gains */
    MODIFY_REG(reg->DAC_ANA_CON5,
               RK3308_DAC_L_HPOUT_GAIN_MSK,
               RK3308_DAC_L_HPOUT_GAIN_0DB);
    MODIFY_REG(reg->DAC_ANA_CON6,
               RK3308_DAC_R_HPOUT_GAIN_MSK,
               RK3308_DAC_R_HPOUT_GAIN_0DB);

    /* Step 19, set LINEOUT default gains */
    MODIFY_REG(reg->DAC_ANA_CON4,
               RK3308_DAC_L_LINEOUT_GAIN_MSK |
               RK3308_DAC_R_LINEOUT_GAIN_MSK,
               RK3308_DAC_L_LINEOUT_GAIN_0DB |
               RK3308_DAC_R_LINEOUT_GAIN_0DB);

    if (acodecPriv->codecVer == ACODEC_VERSION_C) {
        /* recover DAC digtial gain to 0dB */
        WRITE_REG(reg->DAC_DIG_CON4,
                  RK3308BS_DAC_DIG_GAIN(RK3308BS_DAC_DIG_0DB));
    }

    return 0;
}

/**
 * @brief  get version.
 * @param  acodec: the handle of acodec.
 * @return none
 */
static int ACODEC_GetVersion(struct HAL_ACODEC_DEV *acodec)
{
    struct ACODEC_PRIV *acodecPriv = acodec->privateData;
    struct GRF_REG *pGRF = (struct GRF_REG *)(GRF_BASE);
    uint32_t chipID = READ_REG(pGRF->CHIP_ID);

    acodecPriv->codecVer = chipID;

    switch (chipID) {
    case 3306:
        acodecPriv->codecVer = ACODEC_VERSION_A;
        break;
    case 0x3308:
        acodecPriv->codecVer = ACODEC_VERSION_B;
        break;
    case 0x3308c:
        acodecPriv->codecVer = ACODEC_VERSION_C;
        break;
    default:
        HAL_DBG_ERR("Unknown chipID: 0x%lx\n", chipID);

        return HAL_INVAL;
    }

    HAL_DBG("The acodec version is: 0x%lx\n", acodecPriv->codecVer);

    return HAL_OK;
}

static int ACODEC_ADCMCLK_Disable(struct HAL_ACODEC_DEV *acodec)
{
    struct ACODEC_REG *reg = acodec->pReg;

    MODIFY_REG(reg->GLB_CON,
               RK3308_ADC_MCLK_MSK,
               RK3308_ADC_MCLK_DIS);

    return HAL_OK;
}

static int ACODEC_ADCMCLK_Enable(struct HAL_ACODEC_DEV *acodec)
{
    struct ACODEC_REG *reg = acodec->pReg;

    MODIFY_REG(reg->GLB_CON,
               RK3308_ADC_MCLK_MSK,
               RK3308_ADC_MCLK_EN);
    HAL_DelayUs(20);

    return HAL_OK;
}

static int ACODEC_DACMCLK_Disable(struct HAL_ACODEC_DEV *acodec)
{
    struct ACODEC_REG *reg = acodec->pReg;

    // if (!rk3308->no_hp_det && acodecPriv->codecVer == ACODEC_VERSION_C)
    //     return;

    MODIFY_REG(reg->GLB_CON,
               RK3308_DAC_MCLK_MSK,
               RK3308_DAC_MCLK_DIS);

    return HAL_OK;
}

static int ACODEC_DACMCLK_Enable(struct HAL_ACODEC_DEV *acodec)
{
    struct ACODEC_REG *reg = acodec->pReg;

    MODIFY_REG(reg->GLB_CON,
               RK3308_DAC_MCLK_MSK,
               RK3308_DAC_MCLK_EN);
    HAL_DelayUs(20);

    return HAL_OK;
}

static int ACODEC_ADCMicBias_Disable(struct HAL_ACODEC_DEV *acodec)
{
    return HAL_OK;
}

static HAL_Status check_micbias(int volt)
{
    switch (volt) {
    case RK3308_ADC_MICBIAS_VOLT_0_85:
    case RK3308_ADC_MICBIAS_VOLT_0_8:
    case RK3308_ADC_MICBIAS_VOLT_0_75:
    case RK3308_ADC_MICBIAS_VOLT_0_7:
    case RK3308_ADC_MICBIAS_VOLT_0_65:
    case RK3308_ADC_MICBIAS_VOLT_0_6:
    case RK3308_ADC_MICBIAS_VOLT_0_55:
    case RK3308_ADC_MICBIAS_VOLT_0_5:
        return HAL_OK;
    }

    return HAL_INVAL;
}

static int ACODEC_ADCMicBias_Enable(struct HAL_ACODEC_DEV *acodec, int volt)
{
    struct ACODEC_REG *reg = acodec->pReg;
    int ret;

    /* 0. Power up the ACODEC and keep the AVDDH stable */

    /* Step 1. Configure ACODEC_ADC_ANA_CON7[2:0] to the certain value */
    ret = check_micbias(volt);
    if (ret < 0) {
        HAL_DBG_ERR("This is an invalid volt: %d\n", volt);

        return ret;
    }

    /*
     * Note: Only the reg (ADC_ANA_CON7+0x0)[2:0] represent the level range
     * control signal of MICBIAS voltage
     */
    MODIFY_REG((*ADC_GRP(reg->ADC_ANA_CON7, 0)),
               RK3308_ADC_LEVEL_RANGE_MICBIAS_MSK,
               volt);

    /* Step 2. Wait until the VCMH keep stable */
    HAL_DelayMs(20); /* estimated value */

    /*
     * Step 3. Configure ACODEC_ADC_ANA_CON8[4] to 0x1
     *
     * Note: Only the reg (ADC_ANA_CON8+0x0)[4] represent the enable
     * signal of current source for MICBIAS
     */
    MODIFY_REG((*ADC_GRP(reg->ADC_ANA_CON8, 0)),
               RK3308_ADC_MICBIAS_CURRENT_MSK,
               RK3308_ADC_MICBIAS_CURRENT_EN);

    /*
     * Step 4. Configure the (ADC_ANA_CON7+0x40)[3] or
     * (ADC_ANA_CON7+0x80)[3] to 0x1.
     *
     * (ADC_ANA_CON7+0x40)[3] used to control the MICBIAS1, and
     * (ADC_ANA_CON7+0x80)[3] used to control the MICBIAS2
     */
    /* micbias1 */
    MODIFY_REG((*ADC_GRP(reg->ADC_ANA_CON7, 1)),
               RK3308_ADC_MIC_BIAS_BUF_EN,
               RK3308_ADC_MIC_BIAS_BUF_EN);

    /* micbias2 */
    MODIFY_REG((*ADC_GRP(reg->ADC_ANA_CON7, 2)),
               RK3308_ADC_MIC_BIAS_BUF_EN,
               RK3308_ADC_MIC_BIAS_BUF_EN);

    /* waiting micbias stabled*/
    HAL_DelayMs(50);

    return HAL_OK;
}

static int ACODEC_ADC_Disable(struct HAL_ACODEC_DEV *acodec)
{
    struct ACODEC_REG *reg = acodec->pReg;
    int grp;

    for (grp = 0; grp < ADC_LR_GROUP_MAX; grp++) {
        /* vendor step 1 */
        MODIFY_REG((*ADC_GRP(reg->ADC_ANA_CON2, grp)),
                   RK3308_ADC_CH1_ZEROCROSS_DET_EN |
                   RK3308_ADC_CH2_ZEROCROSS_DET_EN,
                   RK3308_ADC_CH1_ZEROCROSS_DET_DIS |
                   RK3308_ADC_CH2_ZEROCROSS_DET_DIS);
    }

    for (grp = 0; grp < ADC_LR_GROUP_MAX; grp++) {
        /* vendor step 2 */
        MODIFY_REG((*ADC_GRP(reg->ADC_ANA_CON5, grp)),
                   RK3308_ADC_CH1_ADC_EN |
                   RK3308_ADC_CH2_ADC_EN,
                   RK3308_ADC_CH1_ADC_DIS |
                   RK3308_ADC_CH2_ADC_DIS);
    }

    for (grp = 0; grp < ADC_LR_GROUP_MAX; grp++) {
        /* vendor step 3 */
        MODIFY_REG((*ADC_GRP(reg->ADC_ANA_CON5, grp)),
                   RK3308_ADC_CH1_CLK_EN |
                   RK3308_ADC_CH2_CLK_EN,
                   RK3308_ADC_CH1_CLK_DIS |
                   RK3308_ADC_CH2_CLK_DIS);
    }

    for (grp = 0; grp < ADC_LR_GROUP_MAX; grp++) {
        /* vendor step 4 */
        MODIFY_REG((*ADC_GRP(reg->ADC_ANA_CON2, grp)),
                   RK3308_ADC_CH1_ALC_EN |
                   RK3308_ADC_CH2_ALC_EN,
                   RK3308_ADC_CH1_ALC_DIS |
                   RK3308_ADC_CH2_ALC_DIS);
    }

    for (grp = 0; grp < ADC_LR_GROUP_MAX; grp++) {
        /* vendor step 5 */
        MODIFY_REG((*ADC_GRP(reg->ADC_ANA_CON0, grp)),
                   RK3308_ADC_CH1_MIC_EN |
                   RK3308_ADC_CH2_MIC_EN,
                   RK3308_ADC_CH1_MIC_DIS |
                   RK3308_ADC_CH2_MIC_DIS);
    }

    for (grp = 0; grp < ADC_LR_GROUP_MAX; grp++) {
        /* vendor step 6 */
        MODIFY_REG((*ADC_GRP(reg->ADC_ANA_CON0, grp)),
                   RK3308_ADC_CH1_BUF_REF_EN |
                   RK3308_ADC_CH2_BUF_REF_EN,
                   RK3308_ADC_CH1_BUF_REF_DIS |
                   RK3308_ADC_CH2_BUF_REF_DIS);
    }

    for (grp = 0; grp < ADC_LR_GROUP_MAX; grp++) {
        /* vendor step 7 */
        MODIFY_REG((*ADC_GRP(reg->ADC_ANA_CON6, grp)),
                   RK3308_ADC_CURRENT_MSK,
                   RK3308_ADC_CURRENT_DIS);
    }

    for (grp = 0; grp < ADC_LR_GROUP_MAX; grp++) {
        /* vendor step 8 */
        MODIFY_REG((*ADC_GRP(reg->ADC_ANA_CON5, grp)),
                   RK3308_ADC_CH1_ADC_WORK |
                   RK3308_ADC_CH2_ADC_WORK,
                   RK3308_ADC_CH1_ADC_INIT |
                   RK3308_ADC_CH2_ADC_INIT);
    }

    for (grp = 0; grp < ADC_LR_GROUP_MAX; grp++) {
        /* vendor step 9 */
        MODIFY_REG((*ADC_GRP(reg->ADC_ANA_CON2, grp)),
                   RK3308_ADC_CH1_ALC_WORK |
                   RK3308_ADC_CH2_ALC_WORK,
                   RK3308_ADC_CH1_ALC_INIT |
                   RK3308_ADC_CH2_ALC_INIT);
    }

    for (grp = 0; grp < ADC_LR_GROUP_MAX; grp++) {
        /* vendor step 10 */
        MODIFY_REG((*ADC_GRP(reg->ADC_ANA_CON0, grp)),
                   RK3308_ADC_CH1_MIC_WORK |
                   RK3308_ADC_CH2_MIC_WORK,
                   RK3308_ADC_CH1_MIC_INIT |
                   RK3308_ADC_CH2_MIC_INIT);
    }

    return HAL_OK;
}

static int ACODEC_ADC_Enable(struct HAL_ACODEC_DEV *acodec)
{
    struct ACODEC_REG *reg = acodec->pReg;
    int grp;

    /*
     * 1. Set the ACODEC_ADC_ANA_CON7[7:6] and ACODEC_ADC_ANA_CON7[5:4],
     * to select the line-in or microphone as input of ADC
     *
     * Note1. Please ignore the step1 for enabling ADC3, ADC4, ADC5,
     * ADC6, ADC7, and ADC8
     */
    for (grp = 0; grp < ADC_LR_GROUP_MAX; grp++) {
        MODIFY_REG((*ADC_GRP(reg->ADC_ANA_CON7, grp)),
                   RK3308_ADC_CH1_IN_SEL_MSK |
                   RK3308_ADC_CH2_IN_SEL_MSK,
                   RK3308_ADC_CH1_IN_MIC |
                   RK3308_ADC_CH2_IN_MIC);
    }

    /*
     * 2. Set ACODEC_ADC_ANA_CON0[7] and [3] to 0x1, to end the mute station
     * of ADC, to enable the MIC module, to enable the reference voltage
     * buffer, and to end the initialization of MIC
     */
    for (grp = 0; grp < ADC_LR_GROUP_MAX; grp++) {
        MODIFY_REG((*ADC_GRP(reg->ADC_ANA_CON0, grp)),
                   RK3308_ADC_CH1_MIC_UNMUTE |
                   RK3308_ADC_CH2_MIC_UNMUTE,
                   RK3308_ADC_CH1_MIC_UNMUTE |
                   RK3308_ADC_CH2_MIC_UNMUTE);
    }

    /*
     * 3. Set ACODEC_ADC_ANA_CON6[0] to 0x1, to enable the current source
     * of audio
     */
    for (grp = 0; grp < ADC_LR_GROUP_MAX; grp++) {
        MODIFY_REG((*ADC_GRP(reg->ADC_ANA_CON6, grp)),
                   RK3308_ADC_CURRENT_MSK,
                   RK3308_ADC_CURRENT_EN);
    }

    /*
     * This is mainly used for BIST mode that wait ADCs are stable.
     *
     * By tested results, the type delay is >40us, but we need to leave
     * enough delay margin.
     */
    HAL_CPUDelayUs(450);

    /* vendor step 4*/
    for (grp = 0; grp < ADC_LR_GROUP_MAX; grp++) {
        MODIFY_REG((*ADC_GRP(reg->ADC_ANA_CON0, grp)),
                   RK3308_ADC_CH1_BUF_REF_EN |
                   RK3308_ADC_CH2_BUF_REF_EN,
                   RK3308_ADC_CH1_BUF_REF_EN |
                   RK3308_ADC_CH2_BUF_REF_EN);
    }

    /* vendor step 5 */
    for (grp = 0; grp < ADC_LR_GROUP_MAX; grp++) {
        MODIFY_REG((*ADC_GRP(reg->ADC_ANA_CON0, grp)),
                   RK3308_ADC_CH1_MIC_EN |
                   RK3308_ADC_CH2_MIC_EN,
                   RK3308_ADC_CH1_MIC_EN |
                   RK3308_ADC_CH2_MIC_EN);
    }

    /* vendor step 6 */
    for (grp = 0; grp < ADC_LR_GROUP_MAX; grp++) {
        MODIFY_REG((*ADC_GRP(reg->ADC_ANA_CON2, grp)),
                   RK3308_ADC_CH1_ALC_EN |
                   RK3308_ADC_CH2_ALC_EN,
                   RK3308_ADC_CH1_ALC_EN |
                   RK3308_ADC_CH2_ALC_EN);
    }

    /* vendor step 7 */
    for (grp = 0; grp < ADC_LR_GROUP_MAX; grp++) {
        MODIFY_REG((*ADC_GRP(reg->ADC_ANA_CON5, grp)),
                   RK3308_ADC_CH1_CLK_EN |
                   RK3308_ADC_CH2_CLK_EN,
                   RK3308_ADC_CH1_CLK_EN |
                   RK3308_ADC_CH2_CLK_EN);
    }

    /* vendor step 8 */
    for (grp = 0; grp < ADC_LR_GROUP_MAX; grp++) {
        MODIFY_REG((*ADC_GRP(reg->ADC_ANA_CON5, grp)),
                   RK3308_ADC_CH1_ADC_EN |
                   RK3308_ADC_CH2_ADC_EN,
                   RK3308_ADC_CH1_ADC_EN |
                   RK3308_ADC_CH2_ADC_EN);
    }

    /* vendor step 9 */
    for (grp = 0; grp < ADC_LR_GROUP_MAX; grp++) {
        MODIFY_REG((*ADC_GRP(reg->ADC_ANA_CON5, grp)),
                   RK3308_ADC_CH1_ADC_WORK |
                   RK3308_ADC_CH2_ADC_WORK,
                   RK3308_ADC_CH1_ADC_WORK |
                   RK3308_ADC_CH2_ADC_WORK);
    }

    /* vendor step 10 */
    for (grp = 0; grp < ADC_LR_GROUP_MAX; grp++) {
        MODIFY_REG((*ADC_GRP(reg->ADC_ANA_CON2, grp)),
                   RK3308_ADC_CH1_ALC_WORK |
                   RK3308_ADC_CH2_ALC_WORK,
                   RK3308_ADC_CH1_ALC_WORK |
                   RK3308_ADC_CH2_ALC_WORK);
    }

    /* vendor step 11 */
    for (grp = 0; grp < ADC_LR_GROUP_MAX; grp++) {
        MODIFY_REG((*ADC_GRP(reg->ADC_ANA_CON0, grp)),
                   RK3308_ADC_CH1_MIC_WORK |
                   RK3308_ADC_CH2_MIC_WORK,
                   RK3308_ADC_CH1_MIC_WORK |
                   RK3308_ADC_CH2_MIC_WORK);
    }

    return HAL_OK;
}

static int ACODEC_DAC_Disable(struct HAL_ACODEC_DEV *acodec)
{
    struct ACODEC_REG *reg = acodec->pReg;
    struct ACODEC_PRIV *acodecPriv = acodec->privateData;

    /*
     * Step 00 skipped. Keep the DAC channel work and input the mute signal.
     */

    /* Step 01 skipped. May set the min gain for LINEOUT. */

    /* Step 02 skipped. May set the min gain for HPOUT. */

    /* Step 03 */
    MODIFY_REG(reg->DAC_ANA_CON13,
               RK3308_DAC_L_HPMIX_UNMUTE |
               RK3308_DAC_R_HPMIX_UNMUTE,
               RK3308_DAC_L_HPMIX_UNMUTE |
               RK3308_DAC_R_HPMIX_UNMUTE);

    /* Step 04 */
    MODIFY_REG(reg->DAC_ANA_CON12,
               RK3308_DAC_L_HPMIX_SEL_MSK |
               RK3308_DAC_R_HPMIX_SEL_MSK,
               RK3308_DAC_L_HPMIX_NONE |
               RK3308_DAC_R_HPMIX_NONE);
    /* Step 05 */
    MODIFY_REG(reg->DAC_ANA_CON3,
               RK3308_DAC_L_HPOUT_UNMUTE |
               RK3308_DAC_R_HPOUT_UNMUTE,
               RK3308_DAC_L_HPOUT_MUTE |
               RK3308_DAC_R_HPOUT_MUTE);

    /* Step 06 */
    MODIFY_REG(reg->DAC_ANA_CON2,
               RK3308_DAC_L_DAC_WORK |
               RK3308_DAC_R_DAC_WORK,
               RK3308_DAC_L_DAC_INIT |
               RK3308_DAC_R_DAC_INIT);

    /* Step 07 */
    MODIFY_REG(reg->DAC_ANA_CON3,
               RK3308_DAC_L_HPOUT_EN |
               RK3308_DAC_R_HPOUT_EN,
               RK3308_DAC_L_HPOUT_DIS |
               RK3308_DAC_R_HPOUT_DIS);

    /* Step 08 */
    MODIFY_REG(reg->DAC_ANA_CON4,
               RK3308_DAC_L_LINEOUT_UNMUTE |
               RK3308_DAC_R_LINEOUT_UNMUTE,
               RK3308_DAC_L_LINEOUT_MUTE |
               RK3308_DAC_R_LINEOUT_MUTE);

    /* Step 09 */
    MODIFY_REG(reg->DAC_ANA_CON4,
               RK3308_DAC_L_LINEOUT_EN |
               RK3308_DAC_R_LINEOUT_EN,
               RK3308_DAC_L_LINEOUT_DIS |
               RK3308_DAC_R_LINEOUT_DIS);

    /* Step 10 */
    MODIFY_REG(reg->DAC_ANA_CON13,
               RK3308_DAC_L_HPMIX_EN |
               RK3308_DAC_R_HPMIX_EN,
               RK3308_DAC_L_HPMIX_DIS |
               RK3308_DAC_R_HPMIX_DIS);

    /* Step 11 */
    MODIFY_REG(reg->DAC_ANA_CON2,
               RK3308_DAC_L_DAC_EN |
               RK3308_DAC_R_DAC_EN,
               RK3308_DAC_L_DAC_DIS |
               RK3308_DAC_R_DAC_DIS);

    /* Step 12 */
    MODIFY_REG(reg->DAC_ANA_CON2,
               RK3308_DAC_L_CLK_EN |
               RK3308_DAC_R_CLK_EN,
               RK3308_DAC_L_CLK_DIS |
               RK3308_DAC_R_CLK_DIS);

    /* Step 13 */
    MODIFY_REG(reg->DAC_ANA_CON2,
               RK3308_DAC_L_REF_EN |
               RK3308_DAC_R_REF_EN,
               RK3308_DAC_L_REF_DIS |
               RK3308_DAC_R_REF_DIS);

    /* Step 14 */
    MODIFY_REG(reg->DAC_ANA_CON1,
               RK3308_DAC_HPOUT_POP_SOUND_L_MSK |
               RK3308_DAC_HPOUT_POP_SOUND_R_MSK,
               RK3308_DAC_HPOUT_POP_SOUND_L_INIT |
               RK3308_DAC_HPOUT_POP_SOUND_R_INIT);

    /* Step 15 */
    if (acodecPriv->codecVer >= ACODEC_VERSION_B &&
        (acodecPriv->dacRoute == DAC_LINEOUT ||
         acodecPriv->dacRoute == DAC_LINEOUT_HPOUT)) {
        MODIFY_REG(reg->DAC_ANA_CON15,
                   RK3308_DAC_LINEOUT_POP_SOUND_L_MSK |
                   RK3308_DAC_LINEOUT_POP_SOUND_R_MSK,
                   RK3308_DAC_L_SEL_DC_FROM_VCM |
                   RK3308_DAC_R_SEL_DC_FROM_VCM);
    }

    /* Step 16 */
    MODIFY_REG(reg->DAC_ANA_CON1,
               RK3308_DAC_BUF_REF_L_EN |
               RK3308_DAC_BUF_REF_R_EN,
               RK3308_DAC_BUF_REF_L_DIS |
               RK3308_DAC_BUF_REF_R_DIS);

    /* Step 17 */
    MODIFY_REG(reg->DAC_ANA_CON0,
               RK3308_DAC_CURRENT_EN,
               RK3308_DAC_CURRENT_DIS);

    /* Step 18 */
    MODIFY_REG(reg->DAC_ANA_CON3,
               RK3308_DAC_L_HPOUT_WORK |
               RK3308_DAC_R_HPOUT_WORK,
               RK3308_DAC_L_HPOUT_INIT |
               RK3308_DAC_R_HPOUT_INIT);

    /* Step 19 */
    MODIFY_REG(reg->DAC_ANA_CON13,
               RK3308_DAC_L_HPMIX_WORK |
               RK3308_DAC_R_HPMIX_WORK,
               RK3308_DAC_L_HPMIX_WORK |
               RK3308_DAC_R_HPMIX_WORK);

    /* Step 20 skipped, may set the min gain for HPOUT. */

    /*
     * Note2. If the ACODEC_DAC_ANA_CON12[7] or ACODEC_DAC_ANA_CON12[3]
     * is set to 0x1, add the steps from the section Disable ADC
     * Configuration Standard Usage Flow after complete the step 19
     *
     * IF USING LINE-IN
     * rk3308_codec_adc_ana_disable(rk3308, type);
     */

    return HAL_OK;
}

static int ACODEC_DAC_Enable(struct HAL_ACODEC_DEV *acodec)
{
    struct ACODEC_REG *reg = acodec->pReg;
    struct ACODEC_PRIV *acodecPriv = acodec->privateData;

    /*
     * Note1. If the ACODEC_DAC_ANA_CON12[6] or ACODEC_DAC_ANA_CON12[2]
     * is set to 0x1, ignoring the step9~12.
     */

    /*
     * Note2. If the ACODEC_ DAC_ANA_CON12[7] or ACODEC_DAC_ANA_CON12[3]
     * is set to 0x1, the ADC0 or ADC1 should be enabled firstly, and
     * please refer to Enable ADC Configuration Standard Usage Flow(expect
     * step7~step9,step14).
     */

    /*
     * Note3. If no opening the line out, ignoring the step6, step17 and
     * step19.
     */

    /*
     * Note4. If no opening the headphone out, ignoring the step3,step7~8,
     * step16 and step18.
     */

    /*
     * Note5. In the step18, adjust the register step by step to the
     * appropriate value and taking 10ms as one time step
     */

    /*
     * 1. Set the ACODEC_DAC_ANA_CON0[0] to 0x1, to enable the current
     * source of DAC
     */
    MODIFY_REG(reg->DAC_ANA_CON0,
               RK3308_DAC_CURRENT_MSK,
               RK3308_DAC_CURRENT_EN);

    HAL_DelayUs(20);

    /*
     * 2. Set the ACODEC_DAC_ANA_CON1[6] and ACODEC_DAC_ANA_CON1[2] to 0x1,
     * to enable the reference voltage buffer
     */
    MODIFY_REG(reg->DAC_ANA_CON1,
               RK3308_DAC_BUF_REF_L_MSK |
               RK3308_DAC_BUF_REF_R_MSK,
               RK3308_DAC_BUF_REF_L_EN |
               RK3308_DAC_BUF_REF_R_EN);

    /* Waiting the stable reference voltage */
    HAL_DelayMs(1);

    /* Step 03 */
    MODIFY_REG(reg->DAC_ANA_CON1,
               RK3308_DAC_HPOUT_POP_SOUND_L_MSK |
               RK3308_DAC_HPOUT_POP_SOUND_R_MSK,
               RK3308_DAC_HPOUT_POP_SOUND_L_WORK |
               RK3308_DAC_HPOUT_POP_SOUND_R_WORK);

    HAL_DelayUs(20);

    if (acodecPriv->codecVer >= ACODEC_VERSION_B &&
        (acodecPriv->dacRoute == DAC_LINEOUT ||
         acodecPriv->dacRoute == DAC_LINEOUT_HPOUT)) {
        /* Step 04 */
        MODIFY_REG(reg->DAC_ANA_CON15,
                   RK3308_DAC_LINEOUT_POP_SOUND_L_MSK |
                   RK3308_DAC_LINEOUT_POP_SOUND_R_MSK,
                   RK3308_DAC_L_SEL_DC_FROM_INTERNAL |
                   RK3308_DAC_R_SEL_DC_FROM_INTERNAL);

        HAL_DelayUs(20);
    }

    /* Step 05 */
    MODIFY_REG(reg->DAC_ANA_CON13,
               RK3308_DAC_L_HPMIX_EN |
               RK3308_DAC_R_HPMIX_EN,
               RK3308_DAC_L_HPMIX_EN |
               RK3308_DAC_R_HPMIX_EN);

    /* Waiting the stable HPMIX */
    HAL_DelayMs(1);

    /* Step 06. Reset HPMIX and recover HPMIX gains */
    MODIFY_REG(reg->DAC_ANA_CON13,
               RK3308_DAC_L_HPMIX_WORK |
               RK3308_DAC_R_HPMIX_WORK,
               RK3308_DAC_L_HPMIX_INIT |
               RK3308_DAC_R_HPMIX_INIT);
    HAL_DelayUs(50);
    MODIFY_REG(reg->DAC_ANA_CON13,
               RK3308_DAC_L_HPMIX_WORK |
               RK3308_DAC_R_HPMIX_WORK,
               RK3308_DAC_L_HPMIX_WORK |
               RK3308_DAC_R_HPMIX_WORK);

    HAL_DelayUs(20);

    if (acodecPriv->dacRoute == DAC_LINEOUT ||
        acodecPriv->dacRoute == DAC_LINEOUT_HPOUT) {
        /* Step 07 */
        MODIFY_REG(reg->DAC_ANA_CON4,
                   RK3308_DAC_L_LINEOUT_EN |
                   RK3308_DAC_R_LINEOUT_EN,
                   RK3308_DAC_L_LINEOUT_EN |
                   RK3308_DAC_R_LINEOUT_EN);

        HAL_DelayUs(20);
    }

    if (acodecPriv->dacRoute == DAC_HPOUT ||
        acodecPriv->dacRoute == DAC_LINEOUT_HPOUT) {
        /* Step 08 */
        MODIFY_REG(reg->DAC_ANA_CON3,
                   RK3308_DAC_L_HPOUT_EN |
                   RK3308_DAC_R_HPOUT_EN,
                   RK3308_DAC_L_HPOUT_EN |
                   RK3308_DAC_R_HPOUT_EN);

        HAL_DelayUs(20);

        /* Step 09 */
        MODIFY_REG(reg->DAC_ANA_CON3,
                   RK3308_DAC_L_HPOUT_WORK |
                   RK3308_DAC_R_HPOUT_WORK,
                   RK3308_DAC_L_HPOUT_WORK |
                   RK3308_DAC_R_HPOUT_WORK);

        HAL_DelayUs(20);
    }

    if (acodecPriv->codecVer >= ACODEC_VERSION_B) {
        /* Step 10 */
        if (acodecPriv->dacRoute == DAC_HPOUT) {
            MODIFY_REG(reg->DAC_ANA_CON15,
                       RK3308_DAC_LINEOUT_POP_SOUND_L_MSK |
                       RK3308_DAC_LINEOUT_POP_SOUND_R_MSK,
                       RK3308_DAC_L_SEL_DC_FROM_INTERNAL |
                       RK3308_DAC_R_SEL_DC_FROM_INTERNAL);
        } else {
            /* LINEOUT and LINEOUT + HPOUT */
            MODIFY_REG(reg->DAC_ANA_CON15,
                       RK3308_DAC_LINEOUT_POP_SOUND_L_MSK |
                       RK3308_DAC_LINEOUT_POP_SOUND_R_MSK,
                       RK3308_DAC_L_SEL_LINEOUT_FROM_INTERNAL |
                       RK3308_DAC_R_SEL_LINEOUT_FROM_INTERNAL);
        }

        HAL_DelayUs(20);
    }

    /* Step 11 */
    MODIFY_REG(reg->DAC_ANA_CON2,
               RK3308_DAC_L_REF_EN |
               RK3308_DAC_R_REF_EN,
               RK3308_DAC_L_REF_EN |
               RK3308_DAC_R_REF_EN);

    HAL_DelayUs(20);

    /* Step 12 */
    MODIFY_REG(reg->DAC_ANA_CON2,
               RK3308_DAC_L_CLK_EN |
               RK3308_DAC_R_CLK_EN,
               RK3308_DAC_L_CLK_EN |
               RK3308_DAC_R_CLK_EN);

    HAL_DelayUs(20);

    /* Step 13 */
    MODIFY_REG(reg->DAC_ANA_CON2,
               RK3308_DAC_L_DAC_EN |
               RK3308_DAC_R_DAC_EN,
               RK3308_DAC_L_DAC_EN |
               RK3308_DAC_R_DAC_EN);

    HAL_DelayUs(20);

    /* Step 14 */
    MODIFY_REG(reg->DAC_ANA_CON2,
               RK3308_DAC_L_DAC_WORK |
               RK3308_DAC_R_DAC_WORK,
               RK3308_DAC_L_DAC_WORK |
               RK3308_DAC_R_DAC_WORK);

    HAL_DelayUs(20);

    /* Step 15 */
    MODIFY_REG(reg->DAC_ANA_CON12,
               RK3308_DAC_L_HPMIX_SEL_MSK |
               RK3308_DAC_R_HPMIX_SEL_MSK,
               RK3308_DAC_L_HPMIX_I2S |
               RK3308_DAC_R_HPMIX_I2S);

    HAL_DelayUs(20);

    /* Step 16 */
    MODIFY_REG(reg->DAC_ANA_CON13,
               RK3308_DAC_L_HPMIX_UNMUTE |
               RK3308_DAC_R_HPMIX_UNMUTE,
               RK3308_DAC_L_HPMIX_UNMUTE |
               RK3308_DAC_R_HPMIX_UNMUTE);

    HAL_DelayUs(20);

    /* Step 17: Put configuration HPMIX Gain to DAPM */

    if (acodecPriv->dacRoute == DAC_HPOUT ||
        acodecPriv->dacRoute == DAC_LINEOUT_HPOUT) {
        /* Step 18 */
        MODIFY_REG(reg->DAC_ANA_CON3,
                   RK3308_DAC_L_HPOUT_UNMUTE |
                   RK3308_DAC_R_HPOUT_UNMUTE,
                   RK3308_DAC_L_HPOUT_UNMUTE |
                   RK3308_DAC_R_HPOUT_UNMUTE);

        HAL_DelayUs(20);
    }

    if (acodecPriv->dacRoute == DAC_LINEOUT ||
        acodecPriv->dacRoute == DAC_LINEOUT_HPOUT) {
        /* Step 19 */
        MODIFY_REG(reg->DAC_ANA_CON4,
                   RK3308_DAC_L_LINEOUT_UNMUTE |
                   RK3308_DAC_R_LINEOUT_UNMUTE,
                   RK3308_DAC_L_LINEOUT_UNMUTE |
                   RK3308_DAC_R_LINEOUT_UNMUTE);
        HAL_DelayUs(20);
    }

    /* TODO: TRY TO TEST DRIVE STRENGTH */

    return 0;
}

static int ACODEC_ADCDIG_Config(struct HAL_ACODEC_DEV *acodec,
                                struct AUDIO_PARAMS *params)
{
    struct ACODEC_REG *reg = acodec->pReg;
    unsigned int adc_aif1 = 0, adc_aif2 = 0;
    int grp;

    /* Clear the status of ADC DIG Digital reigisters */
    ACODEC_ADCDIG_Reset(acodec);

    switch (params->sampleBits) {
    case AUDIO_SAMPLEBITS_16:
        adc_aif1 |= RK3308_ADC_I2S_VALID_LEN_16BITS;
        break;
    case AUDIO_SAMPLEBITS_20:
        adc_aif1 |= RK3308_ADC_I2S_VALID_LEN_20BITS;
        break;
    case AUDIO_SAMPLEBITS_24:
        adc_aif1 |= RK3308_ADC_I2S_VALID_LEN_24BITS;
        break;
    case AUDIO_SAMPLEBITS_32:
        adc_aif1 |= RK3308_ADC_I2S_VALID_LEN_32BITS;
        break;
    default:
        return HAL_INVAL;
    }

    switch ((uint16_t)params->channels) {
    case 1:
        adc_aif1 |= RK3308_ADC_I2S_MONO;
        break;
    case 2:
    case 4:
    case 6:
    case 8:
        adc_aif1 |= RK3308_ADC_I2S_STEREO;
        break;
    default:
        return HAL_INVAL;
    }

    adc_aif1 |= RK3308_ADC_I2S_LR_NORMAL;
    adc_aif2 |= RK3308_ADC_I2S_WORK;

    for (grp = 0; grp < ADC_LR_GROUP_MAX; grp++) {
        MODIFY_REG((*ADC_GRP(reg->ADC_DIG_CON1, grp)),
                   RK3308_ADC_I2S_VALID_LEN_MSK |
                   RK3308_ADC_I2S_LR_MSK |
                   RK3308_ADC_I2S_TYPE_MSK,
                   adc_aif1);
        MODIFY_REG((*ADC_GRP(reg->ADC_DIG_CON2, grp)),
                   RK3308_ADC_I2S_MSK,
                   adc_aif2);
    }

    return HAL_OK;
}

static int ACODEC_DACDIG_Config(struct HAL_ACODEC_DEV *acodec,
                                struct AUDIO_PARAMS *params)
{
    struct ACODEC_REG *reg = acodec->pReg;
    unsigned int dac_aif1 = 0, dac_aif2 = 0;

    /* Clear the status of DAC DIG Digital reigisters */
    ACODEC_DACDIG_Reset(acodec);

    switch (params->sampleBits) {
    case AUDIO_SAMPLEBITS_16:
        dac_aif1 |= RK3308_DAC_I2S_VALID_LEN_16BITS;
        break;
    case AUDIO_SAMPLEBITS_20:
        dac_aif1 |= RK3308_DAC_I2S_VALID_LEN_20BITS;
        break;
    case AUDIO_SAMPLEBITS_24:
        dac_aif1 |= RK3308_DAC_I2S_VALID_LEN_24BITS;
        break;
    case AUDIO_SAMPLEBITS_32:
        dac_aif1 |= RK3308_DAC_I2S_VALID_LEN_32BITS;
        break;
    default:
        return HAL_INVAL;
    }

    dac_aif1 |= RK3308_DAC_I2S_LR_NORMAL;
    dac_aif2 |= RK3308_DAC_I2S_WORK;

    MODIFY_REG(reg->DAC_DIG_CON1,
               RK3308_DAC_I2S_VALID_LEN_MSK |
               RK3308_DAC_I2S_LR_MSK,
               dac_aif1);
    MODIFY_REG(reg->DAC_DIG_CON2,
               RK3308_DAC_I2S_MSK,
               dac_aif2);

    return HAL_OK;
}

static int ACODEC_SetDAIFormat(struct HAL_ACODEC_DEV *acodec, struct AUDIO_INIT_CONFIG *config)
{
    struct ACODEC_REG *reg = acodec->pReg;
    struct ACODEC_PRIV *acodecPriv = acodec->privateData;
    unsigned int adc_aif1 = 0, adc_aif2 = 0, dac_aif1 = 0, dac_aif2 = 0;
    uint32_t grp;
    bool codecIsMaster = config->master;
    bool clkInvert = config->clkInvert;

    if (codecIsMaster) {
        adc_aif2 |= RK3308_ADC_IO_MODE_MASTER;
        adc_aif2 |= RK3308_ADC_MODE_MASTER;
        if (acodecPriv->codecVer == ACODEC_VERSION_C) {
            dac_aif2 |= RK3308BS_DAC_IO_MODE_MASTER;
            dac_aif2 |= RK3308BS_DAC_MODE_MASTER;
        } else {
            dac_aif2 |= RK3308_DAC_IO_MODE_MASTER;
            dac_aif2 |= RK3308_DAC_MODE_MASTER;
        }
    } else {
        adc_aif2 |= RK3308_ADC_IO_MODE_SLAVE;
        adc_aif2 |= RK3308_ADC_MODE_SLAVE;
        if (acodecPriv->codecVer == ACODEC_VERSION_C) {
            dac_aif2 |= RK3308BS_DAC_IO_MODE_SLAVE;
            dac_aif2 |= RK3308BS_DAC_MODE_SLAVE;
        } else {
            dac_aif2 |= RK3308_DAC_IO_MODE_SLAVE;
            dac_aif2 |= RK3308_DAC_MODE_SLAVE;
        }
    }

    switch (config->format) {
    case AUDIO_FMT_I2S:
        adc_aif1 |= RK3308_ADC_I2S_MODE_I2S;
        dac_aif1 |= RK3308_DAC_I2S_MODE_I2S;
        break;
    case AUDIO_FMT_PCM:
    /* fallthrough */
    case AUDIO_FMT_PCM_DELAY1:
    /* fallthrough */
    case AUDIO_FMT_PCM_DELAY2:
    /* fallthrough */
    case AUDIO_FMT_PCM_DELAY3:
        adc_aif1 |= RK3308_ADC_I2S_MODE_PCM;
        dac_aif1 |= RK3308_DAC_I2S_MODE_PCM;
        break;

    case AUDIO_FMT_RIGHT_J:
        adc_aif1 |= RK3308_ADC_I2S_MODE_RJ;
        dac_aif1 |= RK3308_DAC_I2S_MODE_RJ;
        break;
    case AUDIO_FMT_LEFT_J:
        adc_aif1 |= RK3308_ADC_I2S_MODE_LJ;
        dac_aif1 |= RK3308_DAC_I2S_MODE_LJ;
        break;
    default:
        return HAL_INVAL;
    }

    if (clkInvert) {
        adc_aif1 |= RK3308_ADC_I2S_LRC_POL_NORMAL;
        adc_aif2 |= RK3308_ADC_I2S_BIT_CLK_POL_REVERSAL;
        dac_aif1 |= RK3308_DAC_I2S_LRC_POL_NORMAL;
        dac_aif2 |= RK3308_DAC_I2S_BIT_CLK_POL_REVERSAL;
    } else {
        adc_aif1 |= RK3308_ADC_I2S_LRC_POL_NORMAL;
        adc_aif2 |= RK3308_ADC_I2S_BIT_CLK_POL_NORMAL;
        dac_aif1 |= RK3308_DAC_I2S_LRC_POL_NORMAL;
        dac_aif2 |= RK3308_DAC_I2S_BIT_CLK_POL_NORMAL;
    }

    /*
     * Hold ADC Digital registers start at master mode
     *
     * There are 8 ADCs and use the same SCLK and LRCK internal for master
     * mode, We need to make sure that they are in effect at the same time,
     * otherwise they will cause the abnormal clocks.
     */
    if (codecIsMaster) {
        MODIFY_REG(reg->GLB_CON,
                   RK3308_ADC_DIG_WORK,
                   RK3308_ADC_DIG_RESET);
    }

    for (grp = 0; grp < ADC_LR_GROUP_MAX; grp++) {
        // if (grp < 0 || grp > ADC_LR_GROUP_MAX - 1)
        //     continue;
        MODIFY_REG((*ADC_GRP(reg->ADC_DIG_CON1, grp)),
                   RK3308_ADC_I2S_LRC_POL_MSK |
                   RK3308_ADC_I2S_MODE_MSK,
                   adc_aif1);
        MODIFY_REG((*ADC_GRP(reg->ADC_DIG_CON2, grp)),
                   RK3308_ADC_IO_MODE_MSK |
                   RK3308_ADC_MODE_MSK |
                   RK3308_ADC_I2S_BIT_CLK_POL_MSK,
                   adc_aif2);
    }

    /* Hold ADC Digital registers end at master mode */
    if (codecIsMaster) {
        MODIFY_REG(reg->GLB_CON,
                   RK3308_ADC_DIG_WORK,
                   RK3308_ADC_DIG_WORK);
    }

    MODIFY_REG(reg->DAC_DIG_CON1,
               RK3308_DAC_I2S_LRC_POL_MSK |
               RK3308_DAC_I2S_MODE_MSK,
               dac_aif1);
    if (acodecPriv->codecVer == ACODEC_VERSION_C) {
        MODIFY_REG(reg->DAC_DIG_CON2,
                   RK3308BS_DAC_IO_MODE_MSK |
                   RK3308BS_DAC_MODE_MSK |
                   RK3308_DAC_I2S_BIT_CLK_POL_MSK,
                   dac_aif2);
    } else {
        MODIFY_REG(reg->DAC_DIG_CON2,
                   RK3308_DAC_IO_MODE_MSK |
                   RK3308_DAC_MODE_MSK |
                   RK3308_DAC_I2S_BIT_CLK_POL_MSK,
                   dac_aif2);
    }

    return HAL_OK;
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
    struct ACODEC_PRIV *acodecPriv = &g_acodecPriv;

    if (acodecPriv->inited) {
        return HAL_OK;
    }

    acodec->privateData = &g_acodecPriv;
    acodecPriv->dacRoute = DAC_LINEOUT;

    ACODEC_GetVersion(acodec);
    ACODEC_Reset(acodec);
    ACODEC_PowerOn(acodec);
    ACODEC_SetDefaultGains(acodec);
    ACODEC_SetDAIFormat(acodec, config);

    acodecPriv->inited = true;

    return HAL_OK;
}

/**
 * @brief  DeInit acodec.
 * @param  acodec: the handle of acodec.
 * @return HAL_Status
 */
HAL_Status HAL_ACODEC_DeInit(struct HAL_ACODEC_DEV *acodec)
{
    struct ACODEC_PRIV *acodecPriv = &g_acodecPriv;

    acodecPriv->inited = false;

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
        /* DAC only supports 2 channels */
        ACODEC_DACMCLK_Enable(acodec);
        ACODEC_DAC_Enable(acodec);
    } else {
        ACODEC_ADCMicBias_Enable(acodec, RK3308_ADC_MICBIAS_VOLT_0_85);
        ACODEC_ADCMCLK_Enable(acodec);
        ACODEC_ADC_Enable(acodec);
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
        ACODEC_DAC_Disable(acodec);
        ACODEC_DACMCLK_Disable(acodec);
    } else {
        ACODEC_ADC_Disable(acodec);
        ACODEC_ADCMCLK_Disable(acodec);
        ACODEC_ADCMicBias_Disable(acodec);
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
    if (stream == AUDIO_STREAM_PLAYBACK) {
        ACODEC_DACDIG_Config(acodec, params);
    } else {
        ACODEC_ADCDIG_Config(acodec, params);
    }

    return HAL_OK;
}

/** @} */

/** @} */

/** @} */

#endif
