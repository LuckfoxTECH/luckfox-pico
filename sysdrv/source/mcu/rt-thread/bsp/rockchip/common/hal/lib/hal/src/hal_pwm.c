/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_PWM_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup PWM
 *  @{
 */

/** @defgroup PWM_How_To_Use How To Use
 *  @{

 The PWM HAL driver can be used as follows:

 - Declare a PWM_HANDLE handle structure, for example:
   ```
   struct PWM_HANDLE instance;
   ```
 - Invoke HAL_PWM_Init() API to initialize base address and clock frequency:
     - Base register address;
     - Input clock frequency.

 - (optionally)Invoke HAL_PWM_GlobalLock() API to make configuration of channels unchangeable:
     - Use HAL_PWM_GlobalLock() to lock specified channels.

 - Invoke HAL_PWM_SetConfig() API and HAL_PWM_SetEnable() to start/stop:
     - Use HAL_PWM_SetConfig() to configurate the request mode;
     - (optionally)Use HAL_PWM_SetOutputOffset() to configurate the output offset;
     - Use HAL_PWM_Enable() to start PWM;
     - Use HAL_PWM_Disable() to stop PWM.

 - (optionally)Invoke HAL_PWM_GlobalUnlock() API to make sure channels update configuration simultaneously:
     - Use HAL_PWM_GlobalUnlock() to unlock specified channels.

 - (optionally)Invoke HAL_PWM_EnableCaptureCnt() API to enable postive/negtive capture counter:
     - Use HAL_PWM_GetPosCaptureCnt() to read the number of rising edges in capture mode;
     - Use HAL_PWM_GetNegCaptureCnt() to read the number of falling edges in capture mode.

 - (optionally)Invoke HAL_PWM_DisableCaptureCnt() API to disable postive/negtive capture counter.

 - Invoke HAL_PWM_DeInit() if necessary.

 @} */

/** @defgroup PWM_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/

#define PWM_CNT_REG(pPWM, ch)    (pPWM->pReg->CHANNELS[ch].CNT)
#define PWM_PERIOD_REG(pPWM, ch) (pPWM->pReg->CHANNELS[ch].PERIOD_HPR)
#define PWM_DUTY_REG(pPWM, ch)   (pPWM->pReg->CHANNELS[ch].DUTY_LPR)
#define PWM_CTRL_REG(pPWM, ch)   (pPWM->pReg->CHANNELS[ch].CTRL)

#define PWM_INT_EN(ch)     (1 << (ch))
#define PWM_PWR_INT_EN(ch) (1 << ((ch) + 4 ))

#define PWM_DISABLE (0 << PWM_PWM0_CTRL_PWM_EN_SHIFT)
#define PWM_ENABLE  (1 << PWM_PWM0_CTRL_PWM_EN_SHIFT)

#define PWM_MODE_SHIFT (1)
#define PWM_MODE_MASK  (0x3U << PWM_MODE_SHIFT)

#define PWM_DUTY_POSTIVE  (1 << PWM_PWM0_CTRL_DUTY_POL_SHIFT)
#define PWM_DUTY_NEGATIVE (0 << PWM_PWM0_CTRL_DUTY_POL_SHIFT)
#define PWM_DUTY_MASK     (1 << 3)

#define PWM_INACTIVE_POSTIVE  (1 << PWM_PWM0_CTRL_INACTIVE_POL_SHIFT)
#define PWM_INACTIVE_NEGATIVE (0 << PWM_PWM0_CTRL_INACTIVE_POL_SHIFT)
#define PWM_INACTIVE_MASK     (1 << 4)

#define PWM_OUTPUT_LEFT       (0 << PWM_PWM0_CTRL_OUTPUT_MODE_SHIFT)
#define PWM_OUTPUT_CENTER     (1 << PWM_PWM0_CTRL_OUTPUT_MODE_SHIFT)
#define PWM_ALIGNED_MODE_MASK (1 << PWM_PWM0_CTRL_OUTPUT_MODE_SHIFT)

#define PWM_UNLOCK (0 << PWM_PWM0_CTRL_CONLOCK_SHIFT)
#define PWM_LOCK   (1 << PWM_PWM0_CTRL_CONLOCK_SHIFT)

#define PWM_LP_DISABLE (0 << PWM_PWM0_CTRL_FORCE_CLK_EN_SHIFT)
#define PWM_LP_ENABLE  (1 << PWM_PWM0_CTRL_FORCE_CLK_EN_SHIFT)

#define PWM_SEL_SRC_CLK   (0 << PWM_PWM0_CTRL_CLK_SEL_SHIFT)
#define PWM_SEL_SCALE_CLK (1 << PWM_PWM0_CTRL_CLK_SEL_SHIFT)

#ifdef PWM_PWM0_CTRL_ALIGNED_VLD_N_SHIFT
#define PWM_ALIGNED_VALID      (0 << PWM_PWM0_CTRL_ALIGNED_VLD_N_SHIFT)
#define PWM_ALIGNED_INVALID    (1 << PWM_PWM0_CTRL_ALIGNED_VLD_N_SHIFT)
#define PWM_ALIGNED_VALID_MASK (1 << PWM_PWM0_CTRL_ALIGNED_VLD_N_SHIFT)
#endif

#define PWM_CTRL_SCALE_SHIFT (PWM_PWM0_CTRL_SCALE_SHIFT)
#define PWM_CTRL_SCALE_MASK  (PWM_PWM0_CTRL_SCALE_MASK)

#define PWM_PWRMATCH_MAX_SHIFT (PWM_PWRMATCH_LPRE_CNT_MIN_SHIFT)

#ifdef PWM_FILTER_CTRL_PWM0_GLOBAL_LOCK_SHIFT
#define PWM_GLOBAL_LOCK_SHIFT (PWM_FILTER_CTRL_PWM0_GLOBAL_LOCK_SHIFT)
#define PWM_GLOBAL_LOCK_MASK  (PWM_FILTER_CTRL_PWM0_GLOBAL_LOCK_MASK | \
                               PWM_FILTER_CTRL_PWM1_GLOBAL_LOCK_MASK | \
                               PWM_FILTER_CTRL_PWM2_GLOBAL_LOCK_MASK | \
                               PWM_FILTER_CTRL_PWM3_GLOBAL_LOCK_MASK)
#endif

#ifdef PWM_PWM0_CAPTURE_CNT_EN_OFFSET
#define PWM_CAPTURE_EN_SHIFT (PWM_PWM0_CAPTURE_CNT_EN_POS_CAPTURE_EN_SHIFT)
#define PWM_CAPTURE_EN_MASK  (PWM_PWM0_CAPTURE_CNT_EN_POS_CAPTURE_EN_MASK | \
                              PWM_PWM0_CAPTURE_CNT_EN_NEG_CAPTURE_EN_MASK)
#endif

/********************* Private Structure Definition **************************/

/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/

/** @} */
/********************* Public Function Definition ****************************/

/** @defgroup PWM_Exported_Functions_Group3 IO Functions

 This section provides functions allowing to IO controlling:

 *  @{
 */

/**
  * @brief  Handle PWM interrupt for capture/oneshot mode.
  * @param  pPWM: pointer to a PWM_HANDLE structure that contains
  *               the information for PWM module.
  * @retval HAL status
  */
HAL_Status HAL_PWM_IRQHandler(struct PWM_HANDLE *pPWM)
{
    uint32_t status = READ_REG(pPWM->pReg->INTSTS);
    uint32_t i;

    /* clean ipd */
    WRITE_REG(pPWM->pReg->INTSTS, status & 0xf);

    if (status & 0xf) {
        for (i = 0; i < HAL_PWM_NUM_CHANNELS; i++) {
            if ((status & (1 << i)) &&
                (pPWM->mode[i] == HAL_PWM_CAPTURE)) {
                pPWM->result[i].active = true;
                pPWM->result[i].pol = status & (1 << (i + 8));
                if (pPWM->result[i].pol) {
                    pPWM->result[i].period = READ_REG(PWM_PERIOD_REG(pPWM, i));
                } else {
                    pPWM->result[i].period = READ_REG(PWM_DUTY_REG(pPWM, i));
                }
            }
        }
    }

    return HAL_OK;
}

/**
 * @brief  Configurate PWM mode.
 * @param  pPWM: pointer to a PWM_HANDLE structure that contains
 *               the information for PWM module.
 * @param  channel: PWM channle(0~3).
 * @param  config: Configuration for PWM.
 * @retval HAL status
 */
HAL_Status HAL_PWM_SetConfig(struct PWM_HANDLE *pPWM, uint8_t channel,
                             const struct HAL_PWM_CONFIG *config)
{
    unsigned long period, duty;
    uint32_t ctrl;

    HAL_ASSERT(pPWM != NULL);
    HAL_ASSERT(channel < HAL_PWM_NUM_CHANNELS);
    HAL_ASSERT(config != NULL);
    HAL_DBG("channel=%d, period_ns=%ld, duty_ns=%ld\n",
            channel, config->periodNS, config->dutyNS);

    period = HAL_DivU64((uint64_t)pPWM->freq * config->periodNS, 1000000000);
    duty = HAL_DivU64((uint64_t)pPWM->freq * config->dutyNS, 1000000000);

    ctrl = READ_REG(PWM_CTRL_REG(pPWM, channel));
    ctrl |= PWM_LOCK;
    WRITE_REG(PWM_CTRL_REG(pPWM, channel), ctrl);

    WRITE_REG(PWM_PERIOD_REG(pPWM, channel), period);
    WRITE_REG(PWM_DUTY_REG(pPWM, channel), duty);

    ctrl &= ~(PWM_DUTY_MASK | PWM_INACTIVE_MASK);
    if (config->polarity) {
        ctrl |= PWM_DUTY_NEGATIVE | PWM_INACTIVE_POSTIVE;
    } else {
        ctrl |= PWM_DUTY_POSTIVE | PWM_INACTIVE_NEGATIVE;
    }

    ctrl &= ~(PWM_ALIGNED_MODE_MASK);
    switch (config->alignedMode) {
    case HAL_PWM_LEFT_ALIGNED:
        ctrl |= PWM_OUTPUT_LEFT;
        break;
    case HAL_PWM_CENTER_ALIGNED:
        ctrl |= PWM_OUTPUT_CENTER;
        break;
    case HAL_PWM_UNALIGNED:
    default:
        break;
    }

#ifdef PWM_PWM0_CTRL_ALIGNED_VLD_N_SHIFT
    ctrl &= ~(PWM_ALIGNED_VALID_MASK);
    switch (config->alignedMode) {
    case HAL_PWM_LEFT_ALIGNED:
    case HAL_PWM_CENTER_ALIGNED:
        ctrl |= PWM_ALIGNED_VALID;
        break;
    case HAL_PWM_UNALIGNED:
        ctrl |= PWM_ALIGNED_INVALID;
        break;
    default:
        break;
    }
#endif

    ctrl &= ~PWM_LOCK;
    WRITE_REG(PWM_CTRL_REG(pPWM, channel), ctrl);

    HAL_DBG("channel=%d, period=%lu, duty=%lu, polarity=%d\n",
            channel, period, duty, config->polarity);

    return HAL_OK;
}

/**
 * @brief  Configurate PWM oneshot count.
 * @param  pPWM: pointer to a PWM_HANDLE structure that contains
 *               the information for PWM module.
 * @param  channel: PWM channle(0~3).
 * @param  count: (count + 1)repeated effective periods of output waveform
 * @retval HAL status
 */
HAL_Status HAL_PWM_SetOneshot(struct PWM_HANDLE *pPWM, uint8_t channel, uint32_t count)
{
    uint32_t ctrl;

    HAL_ASSERT(pPWM != NULL);
    HAL_ASSERT(channel < HAL_PWM_NUM_CHANNELS);
    HAL_DBG("Oneshot count=%ld\n", count);

    ctrl = READ_REG(PWM_CTRL_REG(pPWM, channel));
    ctrl &= ~PWM_PWM0_CTRL_RPT_MASK;
    ctrl |= (count << PWM_PWM0_CTRL_RPT_SHIFT) & PWM_PWM0_CTRL_RPT_MASK;
    WRITE_REG(PWM_CTRL_REG(pPWM, channel), ctrl);

    return HAL_OK;
}

/**
 * @brief  Configurate PWM captured frequency.
 * @param  pPWM: pointer to a PWM_HANDLE structure that contains
 *               the information for PWM module.
 * @param  channel: PWM channle(0~3).
 * @param  freq: PWM use the frequency to capture data
 * @retval HAL status
 */
HAL_Status HAL_PWM_SetCapturedFreq(struct PWM_HANDLE *pPWM, uint8_t channel, uint32_t freq)
{
    uint32_t ctrl;

    HAL_ASSERT(pPWM != NULL);
    HAL_ASSERT(channel < HAL_PWM_NUM_CHANNELS);
    HAL_ASSERT(freq != 0);
    HAL_DBG("Captured freq=%ld\n", freq);

    ctrl = READ_REG(PWM_CTRL_REG(pPWM, channel));
    ctrl &= ~PWM_CTRL_SCALE_MASK;
    ctrl |= PWM_LP_ENABLE | PWM_SEL_SCALE_CLK;
    ctrl |= ((pPWM->freq / (2 * freq)) << PWM_CTRL_SCALE_SHIFT) & PWM_CTRL_SCALE_MASK;
    WRITE_REG(PWM_CTRL_REG(pPWM, channel), ctrl);

    return HAL_OK;
}

/**
 * @brief  Configurate PWM matched setting.
 * @param  pPWM: pointer to a PWM_HANDLE structure that contains
 *               the information for PWM module.
 * @param  channel: PWM channle(0~3).
 * @param  data: matching configuration.
 * @retval HAL status
 */
HAL_Status HAL_PWM_SetMatch(struct PWM_HANDLE *pPWM, uint8_t channel, const struct PWM_MATCH *data)
{
    uint8_t i;

    HAL_ASSERT(pPWM != NULL);
    HAL_ASSERT(channel < HAL_PWM_NUM_CHANNELS);
    HAL_ASSERT(data != NULL);
    HAL_ASSERT(data->matchCount <= PWM_PWRMATCH_MAX_COUNT);

    /* preloader low */
    WRITE_REG(pPWM->pReg->PWRMATCH_LPRE, data->lpreMin | (data->lpreMax << PWM_PWRMATCH_MAX_SHIFT));
    /* preloader high */
    WRITE_REG(pPWM->pReg->PWRMATCH_HPRE, data->hpreMin | (data->hpreMax << PWM_PWRMATCH_MAX_SHIFT));
    /* logic 0/1 low */
    WRITE_REG(pPWM->pReg->PWRMATCH_LD, data->ldMin | (data->ldMax << PWM_PWRMATCH_MAX_SHIFT));
    /* logic 0 high */
    WRITE_REG(pPWM->pReg->PWRMATCH_HD_ZERO, data->hdZeroMin | (data->hdZeroMax << PWM_PWRMATCH_MAX_SHIFT));
    /* logic 1 high */
    WRITE_REG(pPWM->pReg->PWRMATCH_HD_ONE, data->hdOneMin | (data->hdOneMax << PWM_PWRMATCH_MAX_SHIFT));

    for (i = 0; i < data->matchCount; i++) {
        WRITE_REG(pPWM->pReg->PWRMATCH_VALUE[i], data->match[i]);
    }

    /* Enable pwr irq */
    SET_BIT(pPWM->pReg->INT_EN, PWM_PWR_INT_EN(channel));
    /* Enable pwr */
    SET_BIT(pPWM->pReg->PWRMATCH_CTRL, channel);

    return HAL_OK;
}

#ifdef PWM_PWM0_OFFSET_OFFSET
/**
 * @brief  Configurate PWM channel output offset.
 * @param  pPWM: pointer to a PWM_HANDLE structure that contains
 *               the information for PWM module.
 * @param  channel: PWM channel(0~3).
 * @param  offsetNS: PWM channel output offset configuration.
 * @retval HAL status
 */
HAL_Status HAL_PWM_SetOutputOffset(struct PWM_HANDLE *pPWM, uint8_t channel, uint32_t offsetNS)
{
    uint32_t period, duty;
    uint32_t offset, offsetMax;

    HAL_ASSERT(pPWM != NULL);
    HAL_ASSERT(channel < HAL_PWM_NUM_CHANNELS);

    period = READ_REG(PWM_PERIOD_REG(pPWM, channel));
    duty = READ_REG(PWM_DUTY_REG(pPWM, channel));

    offset = HAL_DivU64((uint64_t)pPWM->freq * offsetNS, 1000000000);
    offsetMax = period - duty;
    if (offset < 0 || offset > offsetMax) {
        return HAL_INVAL;
    }

    HAL_DBG("channel=%d, offsetNS=%ld\n", channel, offsetNS);

    WRITE_REG(pPWM->pReg->OFFSET[channel], offset);

    return HAL_OK;
}
#endif

#ifdef PWM_FILTER_CTRL_PWM0_GLOBAL_LOCK_SHIFT
/**
 * @brief  Enable PWM global lock.
 * @param  pPWM: pointer to a PWM_HANDLE structure that contains
 *               the information for PWM module.
 * @param  channelMask: PWM channel mask, such as 0x5 indicates
 *                      channel0 and channel2.
 * @retval HAL status
 */
HAL_Status HAL_PWM_GlobalLock(struct PWM_HANDLE *pPWM, uint8_t channelMask)
{
    uint32_t filterCtrl;

    HAL_ASSERT(pPWM != NULL);
    HAL_ASSERT(channelMask <= (PWM_GLOBAL_LOCK_MASK >> PWM_GLOBAL_LOCK_SHIFT));
    HAL_DBG("channelMask=0x%04x, global lock\n", channelMask);

    filterCtrl = READ_REG(pPWM->pReg->FILTER_CTRL);
    filterCtrl |= (channelMask << PWM_GLOBAL_LOCK_SHIFT) & PWM_GLOBAL_LOCK_MASK;
    WRITE_REG(pPWM->pReg->FILTER_CTRL, filterCtrl);

    return HAL_OK;
}

/**
 * @brief  Disable PWM global lock.
 * @param  pPWM: pointer to a PWM_HANDLE structure that contains
 *               the information for PWM module.
 * @param  channelMask: PWM channel mask, such as 0x5 indicates
 *                      channel0 and channel2.
 * @retval HAL status
 */
HAL_Status HAL_PWM_GlobalUnlock(struct PWM_HANDLE *pPWM, uint8_t channelMask)
{
    uint32_t filterCtrl;

    HAL_ASSERT(pPWM != NULL);
    HAL_ASSERT(channelMask <= (PWM_GLOBAL_LOCK_MASK >> PWM_GLOBAL_LOCK_SHIFT));
    HAL_DBG("channelMask=0x%04x, global unlock\n", channelMask);

    filterCtrl = READ_REG(pPWM->pReg->FILTER_CTRL);
    filterCtrl &= ~((channelMask << PWM_GLOBAL_LOCK_SHIFT) & PWM_GLOBAL_LOCK_MASK);
    WRITE_REG(pPWM->pReg->FILTER_CTRL, filterCtrl);

    return HAL_OK;
}
#endif

#ifdef PWM_PWM0_CAPTURE_CNT_EN_OFFSET
/**
 * @brief  Enable PWM capture counter.
 * @param  pPWM: pointer to a PWM_HANDLE structure that contains
 *               the information for PWM module.
 * @param  channel: PWM channel(0~3).
 * @param  mode: PWM capture counter mode(postive/negtive).
 * @retval HAL status
 */
HAL_Status HAL_PWM_EnableCaptureCnt(struct PWM_HANDLE *pPWM, uint8_t channel,
                                    ePWM_captureCntMode mode)
{
    uint32_t modeCtrl = 0;

    HAL_ASSERT(pPWM != NULL);
    HAL_ASSERT(channel < HAL_PWM_NUM_CHANNELS);
    HAL_ASSERT(mode >= HAL_PWM_POS_CAPTURE && mode <= HAL_PWM_POS_NEG_CAPTURE);
    HAL_DBG("channel=%d, capture counter mode=%d enable\n", channel, mode);

    modeCtrl = READ_REG(pPWM->pReg->CAPTURE_CNT_EN[channel]);
    modeCtrl |= (mode << PWM_CAPTURE_EN_SHIFT) & PWM_CAPTURE_EN_MASK;
    WRITE_REG(pPWM->pReg->CAPTURE_CNT_EN[channel], modeCtrl);

    return HAL_OK;
}

/**
 * @brief  Disable PWM capture counter.
 * @param  pPWM: pointer to a PWM_HANDLE structure that contains
 *               the information for PWM module.
 * @param  channel: PWM channel(0~3).
 * @param  mode: PWM capture counter mode(postive/negtive).
 * @retval HAL status
 */
HAL_Status HAL_PWM_DisableCaptureCnt(struct PWM_HANDLE *pPWM, uint8_t channel,
                                     ePWM_captureCntMode mode)
{
    uint32_t modeCtrl;

    HAL_ASSERT(pPWM != NULL);
    HAL_ASSERT(channel < HAL_PWM_NUM_CHANNELS);
    HAL_ASSERT(mode >= HAL_PWM_POS_CAPTURE && mode <= HAL_PWM_POS_NEG_CAPTURE);
    HAL_DBG("channel=%d, capture counter mode=%d disable\n", channel, mode);

    modeCtrl = READ_REG(pPWM->pReg->CAPTURE_CNT_EN[channel]);
    modeCtrl &= ~((mode << PWM_CAPTURE_EN_SHIFT) & PWM_CAPTURE_EN_MASK);
    WRITE_REG(pPWM->pReg->CAPTURE_CNT_EN[channel], modeCtrl);

    return HAL_OK;
}

/**
 * @brief  Get PWM postive capture counter result.
 * @param  pPWM: pointer to a PWM_HANDLE structure that contains
 *               the information for PWM module.
 * @param  channel: PWM channel(0~3).
 * @retval counter value.
 */
uint32_t HAL_PWM_GetPosCaptureCnt(struct PWM_HANDLE *pPWM, uint8_t channel)
{
    HAL_ASSERT(pPWM != NULL);
    HAL_ASSERT(channel < HAL_PWM_NUM_CHANNELS);

    return READ_REG(pPWM->pReg->CAPTURE_POS_CNT[channel]);
}

/**
 * @brief  Get PWM negtive capture counter result.
 * @param  pPWM: pointer to a PWM_HANDLE structure that contains
 *               the information for PWM module.
 * @param  channel: PWM channel(0~3).
 * @retval counter value.
 */
uint32_t HAL_PWM_GetNegCaptureCnt(struct PWM_HANDLE *pPWM, uint8_t channel)
{
    HAL_ASSERT(pPWM != NULL);
    HAL_ASSERT(channel < HAL_PWM_NUM_CHANNELS);

    return READ_REG(pPWM->pReg->CAPTURE_NEG_CNT[channel]);
}
#endif

/**
 * @brief  Get PWM mode.
 * @param  pPWM: pointer to a PWM_HANDLE structure that contains
 *               the information for PWM module.
 * @param  channel: PWM channle(0~3).
 * @retval ePWM_Mode
 */
ePWM_Mode HAL_PWM_GetMode(struct PWM_HANDLE *pPWM, uint8_t channel)
{
    uint32_t ctrl;

    HAL_ASSERT(pPWM != NULL);
    HAL_ASSERT(channel < HAL_PWM_NUM_CHANNELS);
    HAL_DBG("channel=%d\n", channel);

    ctrl = READ_REG(PWM_CTRL_REG(pPWM, channel));

    return (ePWM_Mode)((ctrl >> PWM_MODE_SHIFT) & PWM_MODE_MASK);
}

/**
 * @brief  Enable PWM.
 * @param  pPWM: pointer to a PWM_HANDLE structure that contains
 *               the information for PWM module.
 * @param  channel: PWM channle(0~3).
 * @param  mode: Current mode on for PWM.
 * @retval HAL status
 */
HAL_Status HAL_PWM_Enable(struct PWM_HANDLE *pPWM, uint8_t channel, ePWM_Mode mode)
{
    uint32_t enableConf, intEnable;

    HAL_ASSERT(pPWM != NULL);
    HAL_ASSERT(channel < HAL_PWM_NUM_CHANNELS);
    HAL_DBG("Enable channel=%d\n", channel);

    pPWM->mode[channel] = mode;
    if (pPWM->mode[channel] != HAL_PWM_CONTINUOUS) {
        intEnable = READ_REG(pPWM->pReg->INT_EN);
        /* Enable irq */
        intEnable |= PWM_INT_EN(channel);
        WRITE_REG(pPWM->pReg->INT_EN, intEnable);
    }

    enableConf = READ_REG(PWM_CTRL_REG(pPWM, channel));
    /* clean mode */
    enableConf &= ~PWM_MODE_MASK;
    enableConf |= (mode << PWM_MODE_SHIFT) | PWM_OUTPUT_LEFT | PWM_LP_DISABLE | PWM_ENABLE;
    WRITE_REG(PWM_CTRL_REG(pPWM, channel), enableConf);

    return HAL_OK;
}

/**
 * @brief  Disable PWM.
 * @param  pPWM: pointer to a PWM_HANDLE structure that contains
 *               the information for PWM module.
 * @param  channel: PWM channle(0~3).
 * @retval HAL status
 */
HAL_Status HAL_PWM_Disable(struct PWM_HANDLE *pPWM, uint8_t channel)
{
    uint32_t ctrl, intEnable;

    HAL_ASSERT(pPWM != NULL);
    HAL_ASSERT(channel < HAL_PWM_NUM_CHANNELS);
    HAL_DBG("Disable channel=%d\n", channel);

    if (pPWM->mode[channel] != HAL_PWM_CONTINUOUS) {
        intEnable = READ_REG(pPWM->pReg->INT_EN);
        /* Disable irq */
        intEnable &= ~PWM_INT_EN(channel);
        WRITE_REG(pPWM->pReg->INT_EN, intEnable);
    }

    ctrl = READ_REG(PWM_CTRL_REG(pPWM, channel));
    ctrl &= ~PWM_ENABLE;
    WRITE_REG(PWM_CTRL_REG(pPWM, channel), ctrl);

    return HAL_OK;
}

/** @} */

/** @defgroup PWM_Exported_Functions_Group4 Init and DeInit Functions

 This section provides functions allowing to init and deinit the module:

 *  @{
 */

/**
 * @brief  Initialize the PWM according to the specified parameters.
 * @param  pPWM: pointer to a PWM_HANDLE structure that contains
 *               the information for PWM module.
 * @param  pReg: PWM controller register base address.
 * @param  freq: PWM bus input clock frequency.
 * @return HAL_Status
 */
HAL_Status HAL_PWM_Init(struct PWM_HANDLE *pPWM, struct PWM_REG *pReg, uint32_t freq)
{
    HAL_ASSERT(pPWM != NULL);

    pPWM->pReg = pReg;
    HAL_ASSERT(IS_PWM_INSTANCE(pPWM->pReg));

    pPWM->freq = freq;

    return HAL_OK;
}

/**
  * @brief  De Initialize the PWM peripheral.
  * @param  pPWM: pointer to a PWM_HANDLE structure that contains
  *               the information for PWM module.
  * @return HAL status
  */
HAL_Status HAL_PWM_DeInit(struct PWM_HANDLE *pPWM)
{
    /* ...to do */
    return HAL_OK;
}

/** @} */

/** @} */

/** @} */

#endif /* HAL_PWM_MODULE_ENABLED */
