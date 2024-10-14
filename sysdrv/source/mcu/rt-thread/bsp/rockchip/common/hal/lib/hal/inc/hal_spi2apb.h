/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_SPI2APB_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup SPI2APB
 *  @{
 */

#ifndef _HAL_SPI2APB_H
#define _HAL_SPI2APB_H

#include "hal_def.h"

/***************************** MACRO Definition ******************************/
/** @defgroup SPI2APB_Exported_Definition_Group1 Basic Definition
 *  @{
 */

/* Config */
#define SPI2APB_LSB (0 << SPI2APB_CTRL0_FBM_SHIFT)          /**< LSB */
#define SPI2APB_MSB (1 << SPI2APB_CTRL0_FBM_SHIFT)          /**< MSB */

#define SPI2APB_BIG_ENDIAN    (0 << SPI2APB_CTRL0_EM_SHIFT)   /**< Big endian */
#define SPI2APB_LITTLE_ENDIAN (1 << SPI2APB_CTRL0_EM_SHIFT)   /**< Little endian */

#define SPI2APB_RXCP        (0 << SPI2APB_CTRL0_RXCP_SHIFT)   /**< RX clock is not inverted */
#define SPI2APB_RXCP_INVERT (1 << SPI2APB_CTRL0_RXCP_SHIFT)   /**< RX clock is inverted */

#define SPI2APB_TXCP        (0 << SPI2APB_CTRL0_TXCP_SHIFT)   /**< TX clock is not inverted */
#define SPI2APB_TXCP_INVERT (1 << SPI2APB_CTRL0_TXCP_SHIFT)   /**< TX clock is inverted */

/* Status */
#define SPI2APB_BUSY     (1 << SPI2APB_SR_BSF_SHIFT)   /**< Indicate busy state */
#define SPI2APB_TX_FULL  (1 << SPI2APB_SR_TFF_SHIFT)   /**< Transmit FIFO is full */
#define SPI2APB_TX_EMPTY (1 << SPI2APB_SR_TFE_SHIFT)   /**< Transmit FIFO is empty */
#define SPI2APB_RX_FULL  (1 << SPI2APB_SR_RFF_SHIFT)   /**< Receive FIFO is full */
#define SPI2APB_RX_EMPTY (1 << SPI2APB_SR_RFE_SHIFT)   /**< Receive FIFO is empty */

/***************************** Structure Definition **************************/

/** @} */

/***************************** Function Declare ******************************/
/** @defgroup SPI2APB_Public_Function_Declare Public Function Declare
 *  @{
 */

HAL_Status HAL_SPI2APB_Configurate(struct SPI2APB_REG *pReg, uint32_t ctrl);
HAL_Status HAL_SPI2APB_UnmaskIrq(struct SPI2APB_REG *pReg, bool unmask);
HAL_Status HAL_SPI2APB_CleanIrq(struct SPI2APB_REG *pReg);
HAL_Status HAL_SPI2APB_WriteReg2(struct SPI2APB_REG *pReg, uint32_t value);
uint32_t HAL_SPI2APB_ReadStatus(struct SPI2APB_REG *pReg);
uint32_t HAL_SPI2APB_ReadReg0(struct SPI2APB_REG *pReg);
uint32_t HAL_SPI2APB_ReadReg1(struct SPI2APB_REG *pReg);
HAL_Status HAL_SPI2APB_Suspend(struct SPI2APB_REG *pReg, uint32_t *regTbl);
HAL_Status HAL_SPI2APB_Resume(struct SPI2APB_REG *pReg, uint32_t *regTbl);

/** @} */

#endif

/** @} */

/** @} */

#endif /* HAL_SPI2APB_MODULE_ENABLED */
