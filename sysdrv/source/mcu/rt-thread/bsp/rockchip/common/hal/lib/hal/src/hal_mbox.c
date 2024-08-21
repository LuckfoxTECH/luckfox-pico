/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_MBOX_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup MBOX
 *  @{
 */

/** @defgroup MBOX_How_To_Use How To Use
 *  @{

 The Mbox driver can be used as follows:

 - Invoke HAL_MBOX_Init() in driver wrapper file to initialize mailbox.
 - Invoke HAL_MBOX_RegisterClient() in protocol layer to register mailbox
     client (user). The consumer should ensure client memory must not be
     released until HAL_MBOX_UnregisterClient() API is invoked.
 - Protocol layer call HAL_MBOX_SendMsg() to send mailbox message and receive
     message via interrupt handler.
 - Using destroy related function to finish resource release work.
 - Mailbox driver is just responsible for passing simple command data, and
     the detail data is the user's responsibility.
 - More details refer to APIs' descriptions as below.

 @} */

/** @defgroup MBOX_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/

#define IS_VALID_CHAN(n) (((uint32_t)(n)) < MBOX_CHAN_CNT)

/********************* Private Structure Definition **************************/

struct MBOX_CHAN {
    const struct MBOX_CLIENT *client;
};

struct MBOX_DEV {
    struct MBOX_REG *pReg;
    struct MBOX_CHAN chans[MBOX_CHAN_CNT];
    uint8_t A2B;
};

/********************* Private Variable Definition ***************************/

static struct MBOX_DEV g_MBoxDevs[MBOX_CNT];

/********************* Private Function Definition ***************************/

static struct MBOX_DEV *MBOX_FindEntry(const struct MBOX_REG *pReg)
{
    int i;

    for (i = 0; i < MBOX_CNT; i++) {
        if (pReg == g_MBoxDevs[i].pReg) {
            return &g_MBoxDevs[i];
        }
    }

    return NULL;
}

static uint32_t MBOX_A2BIntStGet(struct MBOX_REG *pReg)
{
    return pReg->A2B_STATUS & 0x0f;
}

static uint32_t MBOX_B2AIntStGet(struct MBOX_REG *pReg)
{
    return pReg->B2A_STATUS & 0x0f;
}

static void MBOX_ChanEnable(struct MBOX_REG *pReg, eMBOX_CH chan, uint8_t isA2B)
{
    if (isA2B) {
        pReg->B2A_STATUS = 1UL << chan;
        pReg->B2A_INTEN |= 1UL << chan;
    } else {
        pReg->A2B_STATUS = 1UL << chan;
        pReg->A2B_INTEN |= 1UL << chan;
    }
}

static void MBOX_ChanDisable(struct MBOX_REG *pReg, eMBOX_CH chan,
                             uint8_t isA2B)
{
    if (isA2B) {
        pReg->B2A_INTEN &= ~(1UL << chan);
        pReg->B2A_STATUS = 1UL << chan;
    } else {
        pReg->A2B_INTEN &= ~(1UL << chan);
        pReg->A2B_STATUS = 1UL << chan;
    }
}

static uint32_t MBOX_ChanIntStGet(struct MBOX_REG *pReg, eMBOX_CH chan,
                                  uint8_t isA2B)
{
    if (isA2B) {
        return pReg->B2A_STATUS & (1UL << chan);
    }

    return pReg->A2B_STATUS & (1UL << chan);
}

static void MBOX_ChanIntStClear(struct MBOX_REG *pReg, eMBOX_CH chan,
                                uint8_t isA2B)
{
    if (isA2B) {
        pReg->B2A_STATUS = 1UL << chan;
    } else {
        pReg->A2B_STATUS = 1UL << chan;
    }
}

static void MBOX_ChanSendMsg(struct MBOX_REG *pReg, eMBOX_CH chan,
                             uint8_t isA2B, const struct MBOX_CMD_DAT *msg)
{
    if (isA2B) {
        pReg->A2B[chan].CMD = msg->CMD;
        pReg->A2B[chan].DATA = msg->DATA;
    } else {
        pReg->B2A[chan].CMD = msg->CMD;
        pReg->B2A[chan].DATA = msg->DATA;
    }
}

static void MBOX_ChanRecvMsg(struct MBOX_CMD_DAT *msg, struct MBOX_REG *pReg,
                             eMBOX_CH chan, uint8_t isA2B)
{
    if (isA2B) {
        msg->CMD = pReg->B2A[chan].CMD;
        msg->DATA = pReg->B2A[chan].DATA;
    } else {
        msg->CMD = pReg->A2B[chan].CMD;
        msg->DATA = pReg->A2B[chan].DATA;
    }
}

static HAL_Status MBOX_RecvMsg(struct MBOX_DEV *mbox, eMBOX_CH chan)
{
    const struct MBOX_CLIENT *cl;
    struct MBOX_CMD_DAT msg = { 0, 0 };

    MBOX_ChanRecvMsg(&msg, mbox->pReg, chan, mbox->A2B);

    cl = mbox->chans[chan].client;
    if (cl && cl->RXCallback) {
        cl->RXCallback(&msg, cl->callbackData);
    }

    return HAL_OK;
}

/** @} */
/********************* Public Function Definition ****************************/

/** @defgroup MBOX_Exported_Functions_Group1 Suspend and Resume Functions

This section provides functions allowing to suspend and resume the module:

*  @{
*/

HAL_Status HAL_MBOX_Suspend(struct MBOX_REG *pReg)
{
    HAL_ASSERT(IS_MBOX_INSTANCE(pReg));

    return HAL_OK;
}

HAL_Status HAL_MBOX_Resume(struct MBOX_REG *pReg)
{
    struct MBOX_DEV *pMBox;
    uint32_t chan;

    HAL_ASSERT(IS_MBOX_INSTANCE(pReg));

    pMBox = MBOX_FindEntry(pReg);
    if (!pMBox) {
        return HAL_NODEV;
    }

    for (chan = 0; chan < MBOX_CH_MAX; chan++) {
        if (pMBox->chans[chan].client) {
            MBOX_ChanEnable(pReg, (eMBOX_CH)chan, pMBox->A2B);
        }
    }

    return HAL_OK;
}

/** @} */
/********************* Public Function Definition ****************************/

/** @defgroup MBOX_Exported_Functions_Group2 Send and Receive Functions

This section provides functions allowing to send and receive mailbox message:

*  @{
*/

/**
 * @brief  Mailbox send message with specified direction
 * @param  pReg: mailbox base addr
 * @param  chan: mailbox channel id
 * @param  msg: the message to send
 * @param  isA2B: the direction that message to send
 * @return HAL_Status
 */
HAL_Status HAL_MBOX_SendMsg2(struct MBOX_REG *pReg, eMBOX_CH chan,
                             const struct MBOX_CMD_DAT *msg, uint8_t isA2B)
{
    uint32_t status;

    HAL_ASSERT(IS_MBOX_INSTANCE(pReg) && IS_VALID_CHAN(chan) && msg);

    if (isA2B) {
        status = MBOX_A2BIntStGet(pReg);
    } else {
        status = MBOX_B2AIntStGet(pReg);
    }

    /* Previous message has not been consumed. */
    if (status & (1UL << chan)) {
        return HAL_BUSY;
    }

    MBOX_ChanSendMsg(pReg, chan, isA2B, msg);

    return HAL_OK;
}

/**
 * @brief  Mailbox send message API
 * @param  pReg: mailbox base addr
 * @param  chan: mailbox channel id
 * @param  msg: the message to send
 * @return HAL_Status
 */
HAL_Status HAL_MBOX_SendMsg(struct MBOX_REG *pReg, eMBOX_CH chan,
                            const struct MBOX_CMD_DAT *msg)
{
    struct MBOX_DEV *pMBox;

    HAL_ASSERT(IS_MBOX_INSTANCE(pReg) && IS_VALID_CHAN(chan) && msg);

    pMBox = MBOX_FindEntry(pReg);
    if (!pMBox) {
        return HAL_NODEV;
    }

    return HAL_MBOX_SendMsg2(pReg, chan, msg, pMBox->A2B);
}

/**
 * @brief  Mailbox receive message API
 * @param  pReg: mailbox base addr
 * @param  chan: mailbox channel id
 * @return HAL_Status
 */
HAL_Status HAL_MBOX_RecvMsg(struct MBOX_REG *pReg, eMBOX_CH chan)
{
    struct MBOX_DEV *pMBox;

    HAL_ASSERT(IS_MBOX_INSTANCE(pReg) && IS_VALID_CHAN(chan));

    pMBox = MBOX_FindEntry(pReg);
    if (!pMBox) {
        return HAL_NODEV;
    }

    return MBOX_RecvMsg(pMBox, chan);
}

/** @} */

/** @defgroup MBOX_Exported_Functions_Group3 Interrupt Handle Functions

 This section provides functions allowing to handle interrupt request:

 *  @{
 */

/**
 * @brief  Common mailbox interrupt handler
 * @param  irq: mailbox irq number
 * @param  pReg: mailbox base addr
 * @return result of the interrupt handling
 * Common interrupt handler is always wrapped by the driver.
 */
HAL_Status HAL_MBOX_IrqHandler(int irq, struct MBOX_REG *pReg)
{
    struct MBOX_DEV *pMBox;
    uint32_t chan;
    HAL_Status ret = HAL_OK;

    if (irq < 0 || !IS_MBOX_INSTANCE(pReg)) {
        return HAL_INVAL;
    }

    pMBox = MBOX_FindEntry(pReg);
    if (!pMBox) {
        return HAL_NODEV;
    }

    for (chan = 0; chan < MBOX_CHAN_CNT; chan++) {
        if (!MBOX_ChanIntStGet(pReg, (eMBOX_CH)chan, pMBox->A2B)) {
            continue;
        }

        if (irq != pMBox->chans[chan].client->irq) {
            continue;
        }

        ret = MBOX_RecvMsg(pMBox, (eMBOX_CH)chan);
        MBOX_ChanIntStClear(pReg, (eMBOX_CH)chan, pMBox->A2B);

        if (ret) {
            break;
        }
    }

    return ret;
}

/** @} */

/** @defgroup MBOX_Exported_Functions_Group4 Init and DeInit Functions

 This section provides functions allowing to init and deinit the module:

 *  @{
 */

/**
 * @brief  Mailbox init
 * @param  pReg: mailbox base addr
 * @param  isA2B: A2B mode
 * @return HAL_Status
 */
HAL_Status HAL_MBOX_Init(struct MBOX_REG *pReg, uint8_t isA2B)
{
    struct MBOX_DEV *pMBox = NULL;
    uint32_t chan;
    int i;

    HAL_ASSERT(IS_MBOX_INSTANCE(pReg));

    for (i = 0; i < MBOX_CNT; i++) {
        if (!g_MBoxDevs[i].pReg) {
            pMBox = &g_MBoxDevs[i];
            break;
        }
    }

    if (!pMBox) {
        return HAL_INVAL;
    }

    pMBox->pReg = pReg;
    pMBox->A2B = isA2B;

    for (chan = 0; chan < MBOX_CH_MAX; chan++) {
        pMBox->chans[chan].client = NULL;
    }

    return HAL_OK;
}

/**
 * @brief  Mailbox deinit
 * @param  pReg: mailbox base addr
 * @return HAL_Status
 */
HAL_Status HAL_MBOX_DeInit(struct MBOX_REG *pReg)
{
    struct MBOX_DEV *pMBox;
    uint32_t chan;

    HAL_ASSERT(IS_MBOX_INSTANCE(pReg));
    pMBox = MBOX_FindEntry(pReg);
    if (!pMBox) {
        return HAL_NODEV;
    }

    pMBox->pReg = NULL;

    for (chan = 0; chan < MBOX_CHAN_CNT; chan++) {
        pMBox->chans[chan].client = NULL;
    }

    return HAL_OK;
}

/** @} */

/** @defgroup MBOX_Exported_Functions_Group5 Register and Unregister Functions

 This section provides functions allowing to register/unregister mbox client:

 *  @{
 */

/**
 * @brief  Register mailbox client to specified mailbox and channel
 * @param  pReg: mailbox base addr
 * @param  chan: chan id
 * @param  cl: mailbox client wanna register, its memory must not be released
               until HAL_MBOX_UnregisterClient() API is invoked.
 * @return HAL_Status
 */
HAL_Status HAL_MBOX_RegisterClient(struct MBOX_REG *pReg, eMBOX_CH chan,
                                   const struct MBOX_CLIENT *cl)
{
    struct MBOX_DEV *pMBox;

    HAL_ASSERT(IS_MBOX_INSTANCE(pReg) && IS_VALID_CHAN(chan) && cl);

    pMBox = MBOX_FindEntry(pReg);
    if (!pMBox) {
        return HAL_NODEV;
    }

    if (pMBox->chans[chan].client) {
        return HAL_BUSY;
    }

    pMBox->chans[chan].client = cl;
    MBOX_ChanEnable(pReg, chan, pMBox->A2B);

    return HAL_OK;
}

/**
 * @brief  Unregister mailbox client from specified mailbox and channel
 * @param  pReg: mailbox base addr
 * @param  chan: chan id
 * @param  cl: mailbox client wanna register
 * @return HAL_Status
 */
HAL_Status HAL_MBOX_UnregisterClient(struct MBOX_REG *pReg, eMBOX_CH chan,
                                     const struct MBOX_CLIENT *cl)
{
    struct MBOX_DEV *pMBox;

    HAL_ASSERT(IS_MBOX_INSTANCE(pReg) && IS_VALID_CHAN(chan) && cl);

    pMBox = MBOX_FindEntry(pReg);
    if (!pMBox) {
        return HAL_NODEV;
    }

    if (cl != pMBox->chans[chan].client) {
        return HAL_ERROR;
    }

    MBOX_ChanDisable(pReg, chan, pMBox->A2B);
    pMBox->chans[chan].client = NULL;

    return HAL_OK;
}

/** @} */

/** @} */

/** @} */

#endif /* HAL_MBOX_MODULE_ENABLED */
