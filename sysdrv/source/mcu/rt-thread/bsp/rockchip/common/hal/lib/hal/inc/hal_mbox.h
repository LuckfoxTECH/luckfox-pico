/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_MBOX_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup MBOX
 *  @{
 */

#ifndef _HAL_MBOX_H_
#define _HAL_MBOX_H_

#include "hal_def.h"

/** @defgroup MBOX_Exported_Definition_Group1 Basic Definition
 *  @{
 */

/***************************** MACRO Definition ******************************/

typedef enum {
    MBOX_CH_0 = 0,
    MBOX_CH_1,
    MBOX_CH_2,
    MBOX_CH_3,
    MBOX_CH_MAX = MBOX_CHAN_CNT
} eMBOX_CH;

/***************************** Structure Definition **************************/

struct MBOX_CLIENT {
    char name[16];
    int irq;
    void (*RXCallback)(struct MBOX_CMD_DAT *msg, void *cbData);
    void *callbackData;
};

/** @} */

/***************************** Function Declare ******************************/
/** @defgroup MBOX_Public_Function_Declare Public Function Declare
 *  @{
 */

HAL_Status HAL_MBOX_IrqHandler(int irq, struct MBOX_REG *pReg);
HAL_Status HAL_MBOX_Init(struct MBOX_REG *pReg, uint8_t isA2B);
HAL_Status HAL_MBOX_DeInit(struct MBOX_REG *pReg);
HAL_Status HAL_MBOX_RecvMsg(struct MBOX_REG *pReg, eMBOX_CH chan);
HAL_Status HAL_MBOX_SendMsg(struct MBOX_REG *pReg, eMBOX_CH chan,
                            const struct MBOX_CMD_DAT *msg);
HAL_Status HAL_MBOX_SendMsg2(struct MBOX_REG *pReg, eMBOX_CH chan,
                             const struct MBOX_CMD_DAT *msg, uint8_t isA2B);
HAL_Status HAL_MBOX_RegisterClient(struct MBOX_REG *pReg, eMBOX_CH chan,
                                   const struct MBOX_CLIENT *cl);
HAL_Status HAL_MBOX_UnregisterClient(struct MBOX_REG *pReg, eMBOX_CH chan,
                                     const struct MBOX_CLIENT *cl);
HAL_Status HAL_MBOX_Suspend(struct MBOX_REG *pReg);
HAL_Status HAL_MBOX_Resume(struct MBOX_REG *pReg);

/** @} */

#endif

/** @} */

/** @} */

#endif /* HAL_MBOX_MODULE_ENABLED */
