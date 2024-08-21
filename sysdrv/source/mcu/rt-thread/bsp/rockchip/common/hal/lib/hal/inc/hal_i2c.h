/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_I2C_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
  * @{
  */

/** @addtogroup I2C
  * @{
  */

#ifndef __HAL_I2C_H
#define __HAL_I2C_H

#include "hal_def.h"
#include "hal_base.h"

/***************************** MACRO Definition ******************************/
/** @defgroup I2C_Exported_Definition_Group1 Basic Definition
 *  @{
 */

/**
  * @brief  Hal I2C message flags
  */
#define HAL_I2C_M_WR         (0x0000) /**< write data, from master to slave */
#define HAL_I2C_M_RD         (0x1 << 0) /**< read data, from slave to master */
#define HAL_I2C_M_TEN        (0x1 << 1) /**< this is a ten bit chip address */
#define HAL_I2C_M_IGNORE_NAK (0x1 << 2) /**< if I2C_FUNC_PROTOCOL_MANGLING */

/**
  * @brief I2C MRXADDR valid bits
  */
#define HAL_I2C_REG_MRXADDR_VALID(x) (0x1 << (24 + (x))) /**< [x*8+7:x*8] of MRX[R]ADDR valid */

/***************************** Structure Definition **************************/

/**
  * @brief  I2C mode definition
  */
typedef enum {
    REG_CON_MOD_TX = 0,      /**< transmit data */
    REG_CON_MOD_REGISTER_TX, /**< select register and restart */
    REG_CON_MOD_RX,          /**< receive data */
    REG_CON_MOD_REGISTER_RX /**< broken: transmits read addr AND writes*/
} eI2C_Mode;

/**
  * @brief  I2C transfer mode definition
  */
typedef enum {
    I2C_100K = 0,
    I2C_400K,
    I2C_1000K
} eI2C_BusSpeed;

/**
  * @brief  I2C transfer type definition
  */
typedef enum {
    I2C_POLL = 0,
    I2C_IT
} eI2C_TransferType;

/**
  * @brief  I2C transfer state definition
  */
typedef enum {
    STATE_IDLE = 0,
    STATE_START,
    STATE_READ,
    STATE_WRITE,
    STATE_STOP
} eI2C_State;

/**
  * @brief  I2C HW information definition
  */
struct HAL_I2C_DEV {
    struct I2C_REG *pReg;
    eCLOCK_Name clkID;
    uint32_t clkGateID;
    uint32_t pclkGateID;
    IRQn_Type irqNum;
    ePM_RUNTIME_ID runtimeID;
};

/**
  * @brief  I2C message definition
  */
struct I2C_MSG {
    uint16_t addr; /**< slave address */
    uint16_t flags; /**< flags */
    uint16_t len; /**< msg length */
    uint8_t *buf; /**< pointer to msg data */
};

/**
  * @brief  I2C handle Structure definition
  */

struct I2C_HANDLE {
    struct I2C_REG *pReg;
    eI2C_Mode mode;
    eI2C_TransferType type;
    eI2C_BusSpeed speed;
    struct I2C_MSG msg;
    eI2C_State state;
    uint32_t cfg;
    uint32_t processed;
    HAL_Status error;
    bool isLastMSG;
};

/** @} */

/***************************** Function Declare ******************************/
/** @defgroup I2C_Public_Function_Declare Public Function Declare
 *  @{
 */

HAL_Status HAL_I2C_AdaptDIV(struct I2C_HANDLE *pI2C, uint32_t rate);
HAL_Status HAL_I2C_IRQHandler(struct I2C_HANDLE *pI2C);
HAL_Status HAL_I2C_ConfigureMode(struct I2C_HANDLE *pI2C, eI2C_Mode mode,
                                 uint32_t addr, uint32_t regAddr);
HAL_Status HAL_I2C_SetupMsg(struct I2C_HANDLE *pI2C, uint16_t addr, uint8_t *buf,
                            uint16_t len, uint16_t flags);
HAL_Status HAL_I2C_Transfer(struct I2C_HANDLE *pI2C, eI2C_TransferType type, bool last);
HAL_Status HAL_I2C_ForceStop(struct I2C_HANDLE *pI2C);
HAL_Status HAL_I2C_Close(struct I2C_HANDLE *pI2C);
HAL_Status HAL_I2C_WriteFinish(struct I2C_HANDLE *pI2C);
HAL_Status HAL_I2C_StopFinish(struct I2C_HANDLE *pI2C);
HAL_Status HAL_I2C_StartTX(struct I2C_HANDLE *pI2C, uint16_t addr,
                           uint8_t *buf, uint16_t len);
HAL_Status HAL_I2C_StartTXU32(struct I2C_HANDLE *pI2C, uint32_t *buf,
                              uint16_t len32, uint16_t len8);
HAL_Status HAL_I2C_StopTX(struct I2C_HANDLE *pI2C);
HAL_Status HAL_I2C_CloseTX(struct I2C_HANDLE *pI2C);
HAL_Status HAL_I2C_Init(struct I2C_HANDLE *pI2C, struct I2C_REG *pReg, uint32_t rate,
                        eI2C_BusSpeed speed);
HAL_Status HAL_I2C_DeInit(struct I2C_HANDLE *pI2C);

/** @} */

#endif

/** @} */

/** @} */

#endif /* HAL_I2C_MODULE_ENABLED */
