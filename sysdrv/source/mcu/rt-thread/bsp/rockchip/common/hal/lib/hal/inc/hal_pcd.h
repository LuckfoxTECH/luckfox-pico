/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_PCD_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup PCD
 *  @{
 */

#ifndef _HAL_PCD_H_
#define _HAL_PCD_H_

#include "hal_def.h"

/***************************** MACRO Definition ******************************/
/** @defgroup PCD_Exported_Definition_Group1 Basic Definition
 *  @{
 */

/** PCD Speed */
#define PCD_SPEED_HIGH         0U
#define PCD_SPEED_HIGH_IN_FULL 1U
#define PCD_SPEED_LOW          2U
#define PCD_SPEED_FULL         3U

/** PCD Interrupts Handle */
#define __HAL_PCD_ENABLE(__HANDLE__)  USB_EnableGlobalInt((__HANDLE__)->pReg)
#define __HAL_PCD_DISABLE(__HANDLE__) USB_DisableGlobalInt((__HANDLE__)->pReg)

#define __HAL_PCD_GET_FLAG(__HANDLE__, __INTERRUPT__)   ((USB_ReadInterrupts((__HANDLE__)->pReg) & (__INTERRUPT__)) == (__INTERRUPT__))
#define __HAL_PCD_CLEAR_FLAG(__HANDLE__, __INTERRUPT__) (((__HANDLE__)->pReg->GINTSTS) = (__INTERRUPT__))
#define __HAL_PCD_IS_INVALID_INTERRUPT(__HANDLE__)      (USB_ReadInterrupts((__HANDLE__)->pReg) == 0)

/***************************** Structure Definition **************************/
/** PCD State Structure definition */
typedef enum {
    HAL_PCD_STATE_RESET   = 0x00U,
    HAL_PCD_STATE_READY   = 0x01U,
    HAL_PCD_STATE_ERROR   = 0x02U,
    HAL_PCD_STATE_BUSY    = 0x03U,
    HAL_PCD_STATE_TIMEOUT = 0x04U
} ePCD_state;

/** LPM suspend state Structure definition */
typedef enum {
    LPM_L0 = 0x00U, /**< on */
    LPM_L1 = 0x01U, /**< LPM L1 sleep */
    LPM_L2 = 0x02U, /**< suspend */
    LPM_L3 = 0x03U, /**< off */
} ePCD_lpmState;

/** PCD BCD State Structure definition */
typedef enum {
    PCD_BCD_ERROR                    = 0xFF,
    PCD_BCD_CONTACT_DETECTION        = 0xFE,
    PCD_BCD_STD_DOWNSTREAM_PORT      = 0xFD,
    PCD_BCD_CHARGING_DOWNSTREAM_PORT = 0xFC,
    PCD_BCD_DEDICATED_CHARGING_PORT  = 0xFB,
    PCD_BCD_FLOATING_CHARGING_PORT   = 0xFA,
    PCD_BCD_DEFAULT_STATE            = 0x00,
} ePCD_bcdMsg;

/** PCD Handle Structure definition */
struct PCD_HANDLE {
    struct USB_GLOBAL_REG *pReg;        /*!< Register base address   */
    struct USB_OTG_CFG cfg;             /*!< PCD required parameters */
    struct USB_OTG_EP inEp[16];         /*!< IN endpoint parameters  */
    struct USB_OTG_EP outEp[16];        /*!< OUT endpoint parameters */
#ifdef CACHE_LINE_SIZE
    HAL_CACHELINE_ALIGNED
    uint8_t setupBuf[CACHE_LINE_SIZE];  /*!< Setup packet buffer     */
#else
    uint8_t setupBuf[32];               /*!< Setup packet buffer     */
#endif
    uint8_t enumSpeed;                  /*!< Enumerated Speed        */
    void *pData;                        /*!< Pointer to upper stack Handler */
    ePCD_state pcdState;                /*!< PCD communication state */
    ePCD_lpmState lpmState;             /*!< LPM State               */
    ePCD_bcdMsg bcdState;               /*!< BCD State               */
};

#include "hal_pcd_ex.h"

/** @} */

/***************************** Function Declare ******************************/
/** @defgroup PCD_Public_Function_Declare Public Function Declare
 *  @{
 */
HAL_Status HAL_PCD_Init(struct PCD_HANDLE *pPCD);
HAL_Status HAL_PCD_DeInit(struct PCD_HANDLE *pPCD);
HAL_Status HAL_PCD_Start(struct PCD_HANDLE *pPCD);
HAL_Status HAL_PCD_Stop(struct PCD_HANDLE *pPCD);
void HAL_PCD_IRQHandler(struct PCD_HANDLE *pPCD);
void HAL_PCD_DataOutStageCallback(struct PCD_HANDLE *pPCD, uint8_t epNum);
void HAL_PCD_DataInStageCallback(struct PCD_HANDLE *pPCD, uint8_t epNum);
void HAL_PCD_SetupStageCallback(struct PCD_HANDLE *pPCD);
void HAL_PCD_SOFCallback(struct PCD_HANDLE *pPCD);
void HAL_PCD_ResetCallback(struct PCD_HANDLE *pPCD);
void HAL_PCD_SuspendCallback(struct PCD_HANDLE *pPCD);
void HAL_PCD_ResumeCallback(struct PCD_HANDLE *pPCD);
void HAL_PCD_ISOOUTIncompleteCallback(struct PCD_HANDLE *pPCD, uint8_t epNum);
void HAL_PCD_ISOINIncompleteCallback(struct PCD_HANDLE *pPCD, uint8_t epNum);
void HAL_PCD_ConnectCallback(struct PCD_HANDLE *pPCD);
void HAL_PCD_DisconnectCallback(struct PCD_HANDLE *pPCD);
HAL_Status HAL_PCD_DevConnect(struct PCD_HANDLE *pPCD);
HAL_Status HAL_PCD_DevDisconnect(struct PCD_HANDLE *pPCD);
HAL_Status HAL_PCD_SetAddress(struct PCD_HANDLE *pPCD, uint8_t address);
HAL_Status HAL_PCD_EPOpen(struct PCD_HANDLE *pPCD, uint8_t epAddr, uint16_t ep_mps, uint8_t epType);
HAL_Status HAL_PCD_EPClose(struct PCD_HANDLE *pPCD, uint8_t epAddr);
HAL_Status HAL_PCD_EPReceive(struct PCD_HANDLE *pPCD, uint8_t epAddr, uint8_t *pBuf, uint32_t len);
HAL_Status HAL_PCD_EPTransmit(struct PCD_HANDLE *pPCD, uint8_t epAddr, uint8_t *pBuf, uint32_t len);
uint16_t HAL_PCD_EPGetRxCount(struct PCD_HANDLE *pPCD, uint8_t epAddr);
HAL_Status HAL_PCD_EPSetStall(struct PCD_HANDLE *pPCD, uint8_t epAddr);
HAL_Status HAL_PCD_EPClrStall(struct PCD_HANDLE *pPCD, uint8_t epAddr);
HAL_Status HAL_PCD_EPFlush(struct PCD_HANDLE *pPCD, uint8_t epAddr);
HAL_Status HAL_PCD_ActivateRemoteWakeup(struct PCD_HANDLE *pPCD);
HAL_Status HAL_PCD_DeActivateRemoteWakeup(struct PCD_HANDLE *pPCD);
ePCD_state HAL_PCD_GetState(struct PCD_HANDLE *pPCD);

/** @} */

#endif /* _HAL_PCD_H_ */

/** @} */

/** @} */

#endif /* HAL_PCD_MODULE_ENABLED */
