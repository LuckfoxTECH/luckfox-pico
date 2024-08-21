/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_PWM_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
  * @{
  */

/** @addtogroup PWM
  * @{
  */

#ifndef __HAL_PWM_H
#define __HAL_PWM_H

#include "hal_def.h"

/***************************** MACRO Definition ******************************/
/** @defgroup PWM_Exported_Definition_Group1 Basic Definition
 *  @{
 */

#define HAL_PWM_NUM_CHANNELS   (HAL_ARRAY_SIZE(((struct PWM_REG *)0)->CHANNELS))
#define PWM_PWRMATCH_MAX_COUNT (10)

/***************************** Structure Definition **************************/

/**
  * @brief  PWM operation mode definition
  */
typedef enum {
    HAL_PWM_ONE_SHOT = 0, /**< one-shot mode generates configured periods */
    HAL_PWM_CONTINUOUS,   /**< continuous mode generates periods in series */
    HAL_PWM_CAPTURE,      /**< capture mode supports to measure input waveform */
} ePWM_Mode;

/**
  * @brief  PWM capture counter mode definition
  */
typedef enum {
    HAL_PWM_POS_CAPTURE = 1, /**< count the number of rising edges */
    HAL_PWM_NEG_CAPTURE,     /**< count the number of falling edges */
    HAL_PWM_POS_NEG_CAPTURE, /**< count the number of both rising and falling edges */
} ePWM_captureCntMode;

/**
  * @brief  PWM aligned mode definition
  */
typedef enum {
    HAL_PWM_LEFT_ALIGNED = 1, /**< set waveform to left-aligned mode */
    HAL_PWM_CENTER_ALIGNED,   /**< set waveform to center-aligned mode */
    HAL_PWM_UNALIGNED,        /**< set waveform to unaligned mode */
} ePWM_alignedMode;

/**
  * @brief  PWM HW information definition
  */
struct HAL_PWM_DEV {
    struct PWM_REG *pReg;
    eCLOCK_Name clkID;
    uint32_t clkGateID;
    uint32_t pclkGateID;
    IRQn_Type irqNum;
};

/**
  * @brief  PWM handle Structure definition
  */
struct HAL_PWM_CONFIG {
    uint8_t channel;
    uint32_t periodNS;
    uint32_t dutyNS;
    bool polarity;
    ePWM_alignedMode alignedMode;
};

/**
  * @brief  PWM capture data
  */
struct PWM_CAPTURE {
    uint32_t period;
    bool pol;
    bool active;
};

/**
  * @brief  PWM match data
  */
struct PWM_MATCH {
    uint32_t match[PWM_PWRMATCH_MAX_COUNT];
    uint8_t matchCount;
    uint16_t lpreMin;
    uint16_t lpreMax;
    uint16_t hpreMin;
    uint16_t hpreMax;
    uint16_t ldMin;
    uint16_t ldMax;
    uint16_t hdZeroMin;
    uint16_t hdZeroMax;
    uint16_t hdOneMin;
    uint16_t hdOneMax;
};

/**
  * @brief  PWM Handle Structure definition
  */

struct PWM_HANDLE {
    struct PWM_REG *pReg;
    uint32_t freq;
    ePWM_Mode mode[HAL_PWM_NUM_CHANNELS];
    struct PWM_CAPTURE result[HAL_PWM_NUM_CHANNELS];
};

/**
  * @}
  */

/***************************** Function Declare ******************************/
/** @defgroup PWM_Public_Function_Declare Public Function Declare
 *  @{
 */

HAL_Status HAL_PWM_IRQHandler(struct PWM_HANDLE *pPWM);
HAL_Status HAL_PWM_SetConfig(struct PWM_HANDLE *pPWM, uint8_t channel,
                             const struct HAL_PWM_CONFIG *config);
HAL_Status HAL_PWM_SetOneshot(struct PWM_HANDLE *pPWM, uint8_t channel, uint32_t count);
HAL_Status HAL_PWM_SetCapturedFreq(struct PWM_HANDLE *pPWM, uint8_t channel, uint32_t freq);
HAL_Status HAL_PWM_SetMatch(struct PWM_HANDLE *pPWM, uint8_t channel, const struct PWM_MATCH *data);
#ifdef PWM_PWM0_OFFSET_OFFSET
HAL_Status HAL_PWM_SetOutputOffset(struct PWM_HANDLE *pPWM, uint8_t channel, uint32_t offsetNS);
#endif
#ifdef PWM_FILTER_CTRL_PWM0_GLOBAL_LOCK_SHIFT
HAL_Status HAL_PWM_GlobalLock(struct PWM_HANDLE *pPWM, uint8_t channelMask);
HAL_Status HAL_PWM_GlobalUnlock(struct PWM_HANDLE *pPWM, uint8_t channelMask);
#endif
#ifdef PWM_PWM0_CAPTURE_CNT_EN_OFFSET
HAL_Status HAL_PWM_EnableCaptureCnt(struct PWM_HANDLE *pPWM, uint8_t channel,
                                    ePWM_captureCntMode mode);
HAL_Status HAL_PWM_DisableCaptureCnt(struct PWM_HANDLE *pPWM, uint8_t channel,
                                     ePWM_captureCntMode mode);
uint32_t HAL_PWM_GetPosCaptureCnt(struct PWM_HANDLE *pPWM, uint8_t channel);
uint32_t HAL_PWM_GetNegCaptureCnt(struct PWM_HANDLE *pPWM, uint8_t channel);
#endif
ePWM_Mode HAL_PWM_GetMode(struct PWM_HANDLE *pPWM, uint8_t channel);
HAL_Status HAL_PWM_Enable(struct PWM_HANDLE *pPWM, uint8_t channel, ePWM_Mode mode);
HAL_Status HAL_PWM_Disable(struct PWM_HANDLE *pPWM, uint8_t channel);
HAL_Status HAL_PWM_Init(struct PWM_HANDLE *pPWM, struct PWM_REG *pReg, uint32_t freq);
HAL_Status HAL_PWM_DeInit(struct PWM_HANDLE *pPWM);

/** @} */

#endif

/**
  * @}
  */

/**
  * @}
  */

#endif /* HAL_PWM_MODULE_ENABLED */
