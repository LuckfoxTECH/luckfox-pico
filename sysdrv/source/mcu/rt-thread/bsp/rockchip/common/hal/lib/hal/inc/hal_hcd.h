/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_HCD_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup HCD
 *  @{
 */

#ifndef _HAL_HCD_H_
#define _HAL_HCD_H_

#include "hal_def.h"

/***************************** MACRO Definition ******************************/
/** @defgroup HCD_Exported_Definition_Group1 Basic Definition
 *  @{
 */

/** HCD Speed */
#define HCD_SPEED_HIGH 0U
#define HCD_SPEED_FULL 1U
#define HCD_SPEED_LOW  2U

/** HCD Interrupts Handle */
#define __HAL_HCD_ENABLE(__HANDLE__)  USB_EnableGlobalInt((__HANDLE__)->pReg)
#define __HAL_HCD_DISABLE(__HANDLE__) USB_DisableGlobalInt((__HANDLE__)->pReg)

#define __HAL_HCD_GET_FLAG(__HANDLE__, __INTERRUPT__)   ((USB_ReadInterrupts((__HANDLE__)->pReg) & (__INTERRUPT__)) == (__INTERRUPT__))
#define __HAL_HCD_CLEAR_FLAG(__HANDLE__, __INTERRUPT__) (((__HANDLE__)->pReg->GINTSTS) = (__INTERRUPT__))
#define __HAL_HCD_IS_INVALID_INTERRUPT(__HANDLE__)      (USB_ReadInterrupts((__HANDLE__)->pReg) == 0U)

#define __HAL_HCD_CLEAR_HC_INT(chNum, __INTERRUPT__) (USB_HC(chNum)->HCINT = (__INTERRUPT__))
#define __HAL_HCD_MASK_HALT_HC_INT(chNum)            (USB_HC(chNum)->HCINTMSK &= ~USB_OTG_HCINTMSK_CHHM)
#define __HAL_HCD_UNMASK_HALT_HC_INT(chNum)          (USB_HC(chNum)->HCINTMSK |= USB_OTG_HCINTMSK_CHHM)
#define __HAL_HCD_MASK_ACK_HC_INT(chNum)             (USB_HC(chNum)->HCINTMSK &= ~USB_OTG_HCINTMSK_ACKM)
#define __HAL_HCD_UNMASK_ACK_HC_INT(chNum)           (USB_HC(chNum)->HCINTMSK |= USB_OTG_HCINTMSK_ACKM)

/***************************** Structure Definition **************************/
/** HCD State Structure definition */
typedef enum {
    HAL_HCD_STATE_RESET   = 0x00U,
    HAL_HCD_STATE_READY   = 0x01U,
    HAL_HCD_STATE_ERROR   = 0x02U,
    HAL_HCD_STATE_BUSY    = 0x03U,
    HAL_HCD_STATE_TIMEOUT = 0x04U
} eHCD_state;

/** Group2 HCD Handle Structure definition */
struct HCD_HANDLE {
    struct USB_GLOBAL_REG *pReg;        /*!< Register base address    */
    struct USB_OTG_CFG cfg;             /*!< HCD required parameters  */
    struct USB_OTG_HC hc[15];           /*!< Host channels parameters */
    eHCD_state hcdState;                /*!< HCD communication state  */
    void *pData;                        /*!< Pointer Stack Handler    */
};

/** @} */

/***************************** Function Declare ******************************/
/** @defgroup HCD_Public_Function_Declare Public Function Declare
 *  @{
 */
HAL_Status HAL_HCD_Init(struct HCD_HANDLE *pHCD);
HAL_Status HAL_HCD_DeInit(struct HCD_HANDLE *pHCD);
HAL_Status HAL_HCD_HCInit(struct HCD_HANDLE *pHCD,
                          uint8_t chNum,
                          uint8_t epNum,
                          uint8_t devAddress,
                          uint8_t speed,
                          uint8_t epType,
                          uint16_t mps);
HAL_Status HAL_HCD_HCHalt(struct HCD_HANDLE *pHCD, uint8_t chNum);
HAL_Status HAL_HCD_HCSubmitRequest(struct HCD_HANDLE *pHCD,
                                   uint8_t pipe,
                                   uint8_t direction,
                                   uint8_t epType,
                                   uint8_t token,
                                   uint8_t *pbuff,
                                   uint16_t length,
                                   uint8_t doPing);
void HAL_HCD_IRQHandler(struct HCD_HANDLE *pHCD);
void HAL_HCD_SOF_Callback(struct HCD_HANDLE *pHCD);
void HAL_HCD_Connect_Callback(struct HCD_HANDLE *pHCD);
void HAL_HCD_Disconnect_Callback(struct HCD_HANDLE *pHCD);
void HAL_HCD_PortEnabled_Callback(struct HCD_HANDLE *pHCD);
void HAL_HCD_PortDisabled_Callback(struct HCD_HANDLE *pHCD);
void HAL_HCD_HCNotifyURBChange_Callback(struct HCD_HANDLE *pHCD,
                                        uint8_t chNum,
                                        eUSB_OTG_urbState urbState);
HAL_Status HAL_HCD_ResetPort(struct HCD_HANDLE *pHCD);
HAL_Status HAL_HCD_Start(struct HCD_HANDLE *pHCD);
HAL_Status HAL_HCD_Stop(struct HCD_HANDLE *pHCD);
eHCD_state HAL_HCD_GetState(struct HCD_HANDLE *pHCD);
eUSB_OTG_urbState HAL_HCD_HCGetURBState(struct HCD_HANDLE *pHCD, uint8_t chNum);
eUSB_OTG_hcState HAL_HCD_HCGetState(struct HCD_HANDLE *pHCD, uint8_t chNum);
uint32_t HAL_HCD_HCGetXferCount(struct HCD_HANDLE *pHCD, uint8_t chNum);
uint32_t HAL_HCD_GetCurrentFrame(struct HCD_HANDLE *pHCD);
uint32_t HAL_HCD_GetCurrentSpeed(struct HCD_HANDLE *pHCD);

/** @} */

#endif /* _HAL_HCD_H_ */

/** @} */

/** @} */

#endif /* HAL_HCD_MODULE_ENABLED */
