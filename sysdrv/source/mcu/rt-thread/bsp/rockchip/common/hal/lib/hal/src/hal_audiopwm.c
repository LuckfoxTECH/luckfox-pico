/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_AUDIOPWM_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup AUDIOPWM
 *  @{
 */

/** @defgroup AUDIOPWM_How_To_Use How To Use
 *  @{

 The AUDIOPWM driver can be used as follows:

 @} */

/** @defgroup AUDIOPWM_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/

/* XFER */
#define AUDIOPWM_XFER_LSTOP (0x1U << AUDIOPWM_XFER_LSTOP_SHIFT)
#define AUDIOPWM_XFER_START (0x1U << AUDIOPWM_XFER_START_SHIFT)
#define AUDIOPWM_XFER_STOP  (0x0U << AUDIOPWM_XFER_START_SHIFT)
/* SRC_CFG */
#define AUDIOPWM_SRC_CFG_WIDTH(x) ((x - 1) << AUDIOPWM_SRC_CFG_WIDTH_SHIFT)
#define AUDIOPWM_SRC_CFG_ALIGN_R  (0x0U << AUDIOPWM_SRC_CFG_ALIGN_SHIFT)
#define AUDIOPWM_SRC_CFG_ALIGN_L  (0x1U << AUDIOPWM_SRC_CFG_ALIGN_SHIFT)
#define AUDIOPWM_SRC_CFG_HALF_EN  (0x1U << AUDIOPWM_SRC_CFG_HALF_EN_SHIFT)
/* PWM_CFG */
#define AUDIOPWM_PWM_CFG_INTERP_RATE(x)   ((x) << AUDIOPWM_PWM_CFG_INTERP_RATE_SHIFT)
#define AUDIOPWM_PWM_CFG_LINEAR_INTERP_EN (0x1U << AUDIOPWM_PWM_CFG_LINEAR_INTERP_EN_SHIFT)
#define AUDIOPWM_PWM_CFG_OUT_SWAP         (0x1U << AUDIOPWM_PWM_CFG_OUT_SWAP_SHIFT)
#define AUDIOPWM_PWM_CFG_SAMPLE_WIDTH(x)  ((x - 8) << AUDIOPWM_PWM_CFG_SAMPLE_WIDTH_SHIFT)
/* FIFO_CFG */
#define AUDIOPWM_FIFO_CFG_DMA_WATERMARK(x) ((x - 1) << AUDIOPWM_FIFO_CFG_DMA_WATERMARK_SHIFT)
#define AUDIOPWM_FIFO_CFG_DMA_EN           (0x1U << AUDIOPWM_FIFO_CFG_DMA_EN_SHIFT)
#define AUDIOPWM_FIFO_CFG_DMA_DIS          (0x0U << AUDIOPWM_FIFO_CFG_DMA_EN_SHIFT)

#define AUDIOPWM_DMA_BURST_SIZE (8) /* size * width: 8*4 = 32 bytes */

/********************* Private Structure Definition **************************/

/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/

/** @} */
/********************* Public Function Definition ****************************/
/** @defgroup AUDIOPWM_Exported_Functions_Group1 Suspend and Resume Functions

 This section provides functions allowing to suspend and resume the module:

 ...to do or delete this row

 *  @{
 */

/**
 * @brief  audioPwm suspend.
 * @param  audioPwm: the handle of audioPwm.
 * @return HAL_Status
 */
HAL_Status HAL_AUDIOPWM_Supsend(struct HAL_AUDIOPWM_DEV *audioPwm)
{
    return HAL_OK;
}

/**
 * @brief  audioPwm resume.
 * @param  audioPwm: the handle of audioPwm.
 * @return HAL_Status
 */
HAL_Status HAL_AUDIOPWM_Resume(struct HAL_AUDIOPWM_DEV *audioPwm)
{
    return HAL_OK;
}

/** @} */

/** @defgroup AUDIOPWM_Exported_Functions_Group2 State and Errors Functions

 This section provides functions allowing to get the status of the module:

 *  @{
 */

/** @} */

/** @defgroup AUDIOPWM_Exported_Functions_Group3 IO Functions

 This section provides functions allowing to IO controlling:

 *  @{
 */

/** @} */

/** @defgroup AUDIOPWM_Exported_Functions_Group4 Init and DeInit Functions

 This section provides functions allowing to init and deinit the module:

 ...to do or delete this row

 *  @{
 */

/**
 * @brief  Init audioPwm controller.
 * @param  audioPwm: the handle of audioPwm.
 * @param  config: init config for audioPwm init.
 * @return HAL_Status
 */
HAL_Status HAL_AUDIOPWM_Init(struct HAL_AUDIOPWM_DEV *audioPwm, struct AUDIO_INIT_CONFIG *config)
{
    HAL_CRU_ClkEnable(audioPwm->hclk);

    return HAL_OK;
}

/**
 * @brief  DeInit audioPwm controller.
 * @param  audioPwm: the handle of audioPwm.
 * @return HAL_Status
 */
HAL_Status HAL_AUDIOPWM_DeInit(struct HAL_AUDIOPWM_DEV *audioPwm)
{
    HAL_CRU_ClkDisable(audioPwm->hclk);

    return HAL_OK;
}

/** @} */

/** @defgroup AUDIOPWM_Exported_Functions_Group5 Other Functions
 *  @{
 */

/**
 * @brief  Enable audioPwm controller.
 * @param  audioPwm: the handle of audioPwm.
 * @return HAL_Status
 */
HAL_Status HAL_AUDIOPWM_Enable(struct HAL_AUDIOPWM_DEV *audioPwm)
{
    struct AUDIOPWM_REG *reg = audioPwm->pReg;

    WRITE_REG_MASK_WE(reg->FIFO_CFG,
                      AUDIOPWM_FIFO_CFG_DMA_EN_MASK,
                      AUDIOPWM_FIFO_CFG_DMA_EN);
    WRITE_REG_MASK_WE(reg->XFER,
                      AUDIOPWM_XFER_START_MASK,
                      AUDIOPWM_XFER_START);

    return HAL_OK;
}

/**
 * @brief  Disable audioPwm controller.
 * @param  audioPwm: the handle of audioPwm.
 * @return HAL_Status
 */
HAL_Status HAL_AUDIOPWM_Disable(struct HAL_AUDIOPWM_DEV *audioPwm)
{
    struct AUDIOPWM_REG *reg = audioPwm->pReg;

    WRITE_REG_MASK_WE(reg->FIFO_CFG,
                      AUDIOPWM_FIFO_CFG_DMA_EN_MASK,
                      AUDIOPWM_FIFO_CFG_DMA_DIS);
    WRITE_REG_MASK_WE(reg->XFER,
                      AUDIOPWM_XFER_START_MASK,
                      AUDIOPWM_XFER_STOP);

    return HAL_OK;
}

/**
 * @brief  Config audioPwm controller.
 * @param  audioPwm: the handle of audioPwm.
 * @param  params: audio params.
 * @return HAL_Status
 */
HAL_Status HAL_AUDIOPWM_Config(struct HAL_AUDIOPWM_DEV *audioPwm,
                               struct AUDIO_PARAMS *params)
{
    struct AUDIOPWM_REG *reg = audioPwm->pReg;

    WRITE_REG_MASK_WE(reg->SRC_CFG,
                      AUDIOPWM_SRC_CFG_WIDTH_MASK,
                      AUDIOPWM_SRC_CFG_WIDTH(params->sampleBits));

    WRITE_REG_MASK_WE(reg->PWM_CFG,
                      AUDIOPWM_PWM_CFG_SAMPLE_WIDTH_MASK |
                      AUDIOPWM_PWM_CFG_INTERP_RATE_MASK |
                      AUDIOPWM_PWM_CFG_LINEAR_INTERP_EN_MASK,
                      AUDIOPWM_PWM_CFG_SAMPLE_WIDTH(11) |
                      AUDIOPWM_PWM_CFG_INTERP_RATE(1) |
                      AUDIOPWM_PWM_CFG_LINEAR_INTERP_EN);

    WRITE_REG_MASK_WE(reg->FIFO_CFG,
                      AUDIOPWM_FIFO_CFG_DMA_WATERMARK_MASK,
                      AUDIOPWM_FIFO_CFG_DMA_WATERMARK(16));

    return HAL_OK;
}

/** @} */

/** @} */

/** @} */

#endif /* HAL_AUDIOPWM_MODULE_ENABLED */
