/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_SDIO_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup SDIO
 *  @{
 */

/** @defgroup SDIO_How_To_Use How To Use
 *  @{

 The SDIO driver can be used as follows:

 @} */

/** @defgroup SDIO_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/
#define MMC_FIFOTH_DMA_MULTIPLE_TRANSACTION_SIZE_8 (0x2U)

/********************* Private Function Definition ***************************/
static inline uint32_t MMC_GetWaterlevel(struct HAL_MMC_HOST *host)
{
    struct MMC_REG *pReg = host->pReg;

    return (READ_REG(pReg->STATUS) >> MMC_STATUS_FIFO_COUNT_SHIFT) &
           MMC_STATUS_FIFO_COUNT_MASK;
}

HAL_UNUSED static inline uint32_t MMC_IsFifoEmpty(struct HAL_MMC_HOST *host)
{
    struct MMC_REG *pReg = host->pReg;

    return (READ_REG(pReg->STATUS) >> MMC_STATUS_FIFO_EMPTY_SHIFT) &
           MMC_STATUS_FIFO_EMPTY_MASK;
}

/** @} */
/********************* Public Function Definition ****************************/

/** @defgroup SDIO_Exported_Functions_Group2 State and Errors Functions

 This section provides functions allowing to get the status of the module:

 *  @{
 */

/**
 * @brief  Check if card is present.
 * @param  host: private hal host data.
 * @return zero means cd isn't present and vice versa.
 */
uint32_t HAL_MMC_GetCardStatus(struct HAL_MMC_HOST *host)
{
    uint32_t card_status;
    struct MMC_REG *pReg = host->pReg;

    card_status = READ_REG(pReg->CDETECT);

    return card_status & MMC_CARD_DETECT_N;
}

/**
 * @brief  Get cmd resp.
 * @param  host: private hal host data.
 * @param  respNum: which resp the caller want.
 * @return the resp value respectivey.
 */
uint32_t HAL_MMC_GetResponse(struct HAL_MMC_HOST *host, int32_t respNum)
{
    struct MMC_REG *pReg = host->pReg;

    return READ_REG(pReg->RESP[respNum]);
}

/**
 * @brief  Check if the card is in busy state.
 * @param  host: private hal host data.
 * @return HAL_Check status code.
 */
HAL_Check HAL_MMC_IsDataStateBusy(struct HAL_MMC_HOST *host)
{
    struct MMC_REG *pReg = host->pReg;
    uint32_t ret;

    ret = READ_BIT(pReg->STATUS, MMC_STATUS_DATA_BUSY_MASK) ||
          READ_BIT(pReg->STATUS, MMC_STATUS_DATA_STATE_MC_BUSY_MASK);

    return ret ? HAL_TRUE : HAL_FALSE;
}

/** @} */

/** @defgroup SDIO_Exported_Functions_Group3 IO Functions

 This section provides functions allowing to IO controlling:

 *  @{
 */

/**
 * @brief  It is for mask an interrupt
 * @param  host: private hal host data.
 * @param  mask: the interrupt mask map
 * @return HAL_Status.
 */
HAL_Status HAL_MMC_SetInterruptMask(struct HAL_MMC_HOST *host, uint32_t mask)
{
    struct MMC_REG *pReg = host->pReg;

    WRITE_REG(pReg->INTMASK, mask);

    return HAL_OK;
}

/**
 * @brief  It is for clear an interrupt
 * @param  host: private hal host data.
 * @param  interrupts: the interrupt map to be cleared
 * @return HAL_Status.
 */
HAL_Status HAL_MMC_ClearRawInterrupt(struct HAL_MMC_HOST *host,
                                     uint32_t interrupts)
{
    struct MMC_REG *pReg = host->pReg;

    WRITE_REG(pReg->RINTSTS, interrupts);

    return HAL_OK;
}

/**
 * @brief  It is for getting the raw interrupt
 * @param  host: private hal host data.
 * @return the raw interrupt status.
 */
uint32_t HAL_MMC_GetRawInterrupt(struct HAL_MMC_HOST *host)
{
    struct MMC_REG *pReg = host->pReg;

    return READ_REG(pReg->RINTSTS);
}

/**
 * @brief  It is for getting the interrupt mask
 * @param  host: private hal host data.
 * @return the interrupt mask.
 */
uint32_t HAL_MMC_GetInterruptMask(struct HAL_MMC_HOST *host)
{
    struct MMC_REG *pReg = host->pReg;

    return READ_REG(pReg->INTMASK);
}

/**
 * @brief  It is for getting the unmasked interrupt
 * @param  host: private hal host data.
 * @return the unmasked interrupt.
 */
uint32_t HAL_MMC_GetUnmaskedInterrupt(struct HAL_MMC_HOST *host)
{
    struct MMC_REG *pReg = host->pReg;

    return READ_REG(pReg->MINTSTS);
}

/**
 * @brief  It is for info how many bytes need to perform.
 * @param  host: private hal host data.
 * @param  bytes: total bytes need to read or write.
 * @return HAL_Status.
 */
HAL_Status HAL_MMC_SetByteCount(struct HAL_MMC_HOST *host, uint32_t bytes)
{
    struct MMC_REG *pReg = host->pReg;

    WRITE_REG(pReg->BYTCNT, bytes);

    return HAL_OK;
}

/**
 * @brief  It is for info the size of each block need to perform
 * @param  host: private hal host data.
 * @param  size: size of one block in bytes.
 * @return HAL_Status.
 */
HAL_Status HAL_MMC_SetBlockSize(struct HAL_MMC_HOST *host, uint32_t size)
{
    struct MMC_REG *pReg = host->pReg;

    WRITE_REG(pReg->BLKSIZ, size);

    return HAL_OK;
}

/**
 * @brief  It is for info the controller's FSM.
 * @param  host: private hal host data.
 * @return the whole register value of STATUS
 */
uint32_t HAL_MMC_GetStatus(struct HAL_MMC_HOST *host)
{
    struct MMC_REG *pReg = host->pReg;

    return READ_REG(pReg->STATUS);
}

/**
 * @brief  It is for data write.
 * @param  host: private hal host data.
 * @param  buf: data buffer.
 * @param  size: size of data to be written.
 * @return HAL_Status.
 */
HAL_Status HAL_MMC_WriteData(struct HAL_MMC_HOST *host, uint32_t *buf,
                             uint32_t size)
{
    uint32_t fifo_available, i;
    struct MMC_REG *pReg = host->pReg;
    uint32_t start, timeoutMs = 500;

    if (size % 4) {
        size = size + size % 4;
    }

    for (i = 0; i < size / 4; i++) {
        start = HAL_GetTick();
        do {
            fifo_available = MMC_FIFO_DEPTH - MMC_GetWaterlevel(host);
            if ((HAL_GetTick() - start) > timeoutMs) {
                HAL_DBG_ERR("%s: get water level timeout\n", __func__);

                return HAL_TIMEOUT;
            }
        } while (!fifo_available);
        WRITE_REG(pReg->FIFO_BASE, *buf++);
    }

    start = HAL_GetTick();
    while (HAL_MMC_IsDataStateBusy(host)) {
        if ((HAL_GetTick() - start) > timeoutMs * 2) {
            HAL_DBG_ERR("%s: timeout for data line keep being busy\n",
                        __func__);

            return HAL_TIMEOUT;
        }
    }

    return HAL_OK;
}

/**
 * @brief  It is for data read.
 * @param  host: private hal host data.
 * @param  buf: data buffer.
 * @param  size: size of data to be read.
 * @return HAL_Status.
 */
HAL_Status HAL_MMC_ReadData(struct HAL_MMC_HOST *host, uint32_t *buf,
                            uint32_t size)
{
#if MMC_WAIT_FOR_BR
    uint32_t fifo_available, retries;
#endif
    uint32_t i;
    struct MMC_REG *pReg = host->pReg;

    for (i = 0; i < size / 4; i++) {
#if MMC_WAIT_FOR_BR
        retries = 0;
        do {
            fifo_available = MMC_GetWaterlevel(host);
            if (retries++ > 10000) {
                HAL_DBG_ERR("%s: get water level timeout\n", __func__);

                return HAL_TIMEOUT;
            }
        } while (!fifo_available);
#endif
        *buf++ = READ_REG(pReg->FIFO_BASE);
    }

#if MMC_WAIT_FOR_BR
    retries = 0;
    while (HAL_MMC_IsDataStateBusy(host)) {
        if (retries++ > 10000) {
            HAL_DBG_ERR("%s: timeout for data line keep being busy\n",
                        __func__);

            return HAL_TIMEOUT;
        }
    }
#endif

    return HAL_OK;
}

/**
 * @brief  Submit DMA descriptors.
 * @param  host: private hal host data.
 * @param  buf: data buffer.
 * @param  size: size of data to be conducted.
 * @return HAL_Status.
 */
HAL_Status HAL_MMC_InitDescriptors(struct HAL_MMC_HOST *host, uint32_t *buf,
                                   uint32_t size)
{
    MMC_DMA_DESCRIPTORS *desc;
    uint32_t len = 0;
    int32_t desc_cnt = 0;

    desc = host->descriptors;

    while (size > 0) {
        desc[desc_cnt].desc0.bit.own = 1;
        desc[desc_cnt].desc0.bit.sencondAddressChained = 1;
        desc[desc_cnt].desc1.bit.buffer1Size =
            (size >= MMC_DMA_DESC_BUFF_SIZE) ? MMC_DMA_DESC_BUFF_SIZE : size;
        desc[desc_cnt].desc2.bit.bufferAddr0 = (uint32_t)buf + len;
        desc[desc_cnt].desc3.bit.bufferAddr1 =
            (uint32_t)host->descriptors +
            (desc_cnt + 1) * sizeof(MMC_DMA_DESCRIPTORS);

        size -= desc[desc_cnt].desc1.bit.buffer1Size;
        len += desc[desc_cnt].desc1.bit.buffer1Size;
        desc_cnt++;
    }

    desc[0].desc0.bit.firstDescriptor = 1;
    desc[desc_cnt - 1].desc0.bit.lastDescriptor = 1;
    desc[desc_cnt - 1].desc3.bit.bufferAddr1 = 0;

    return HAL_OK;
}

/**
 * @brief  Update clock.
 * @param  host: private hal host data.
 * @param div: div depends on (src / desired).
 * @return HAL_Status
 */
HAL_Status HAL_MMC_UpdateClockRegister(struct HAL_MMC_HOST *host, int32_t div)
{
    uint32_t timeoutMs = 5000, startTick;
    struct MMC_REG *pReg = host->pReg;

    /* Disable clock */
    WRITE_REG(pReg->CLKENA, 0);
    WRITE_REG(pReg->CLKSRC, 0);

    /* Inform CIU */
    WRITE_REG(pReg->CMD, HAL_BIT(31) | HAL_BIT(21));
    startTick = HAL_GetTick();
    while (READ_REG(pReg->CMD) & HAL_BIT(31)) {
        if ((HAL_GetTick() - startTick) > timeoutMs) {
            HAL_DBG_ERR("%s: update clock timeout\n", __func__);

            return HAL_TIMEOUT;
        }
    }

    /* Set clock to desired speed */
    WRITE_REG(pReg->CLKDIV, div);

    /* Inform CIU */
    WRITE_REG(pReg->CMD, HAL_BIT(31) | HAL_BIT(21));

    startTick = HAL_GetTick();
    while (READ_REG(pReg->CMD) & HAL_BIT(31)) {
        if ((HAL_GetTick() - startTick) > timeoutMs) {
            HAL_DBG_ERR("%s: update clock timeout\n", __func__);

            return HAL_TIMEOUT;
        }
    }

    /* Enable clock */
    WRITE_REG(pReg->CLKENA, 1);

    /* Inform CIU */
    WRITE_REG(pReg->CMD, HAL_BIT(31) | HAL_BIT(21));

    startTick = HAL_GetTick();
    while (READ_REG(pReg->CMD) & HAL_BIT(31)) {
        if ((HAL_GetTick() - startTick) > timeoutMs) {
            HAL_DBG_ERR("%s: update clock timeout\n", __func__);

            return HAL_TIMEOUT;
        }
    }

    return HAL_OK;
}

/**
 * @brief  Set card width.
 * @param  host: private hal host data.
 * @param  width: data width need to be set.
 * @return HAL_Status.
 */
HAL_Status HAL_MMC_SetCardWidth(struct HAL_MMC_HOST *host, int32_t width)
{
    struct MMC_REG *pReg = host->pReg;

    switch (width) {
    case MMC_CARD_WIDTH_1BIT:
        WRITE_REG(pReg->CTYPE, 0);
        break;
    case MMC_CARD_WIDTH_4BIT:
        WRITE_REG(pReg->CTYPE, 1);
        break;
    case MMC_CARD_WIDTH_8BIT:
        WRITE_REG(pReg->CTYPE, 0x10000);
        break;
    default:
        HAL_DBG_ERR("%s: card width %ld is not supported\n", __func__, width);

        return HAL_INVAL;
        break;
    }

    return HAL_OK;
}

/**
 * @brief  Send command.
 * @param  host: private hal host data.
 * @param  cmd: cmd index.
 * @param  arg: argument for cmd.
 * @param  flags: cmd flag for different cases.
 * @return HAL_Status.
 */
HAL_Status HAL_MMC_SendCommand(struct HAL_MMC_HOST *host, uint32_t cmd,
                               uint32_t arg, uint32_t flags)
{
    struct MMC_REG *pReg = host->pReg;
    uint32_t timeoutMs = 5000, startTick;

    WRITE_REG(pReg->CMDARG, arg);
    flags |= MMC_CMD_USE_HOLD_REG | MMC_CMD_START_CMD | cmd;

    WRITE_REG(pReg->CMD, flags);

    startTick = HAL_GetTick();
    while (READ_REG(pReg->CMD) & MMC_CMD_START_CMD) {
        if ((HAL_GetTick() - startTick) > timeoutMs) {
            HAL_DBG_ERR("%s: send cmd timeout\n", __func__);

            return HAL_TIMEOUT;
        }
    }

    /* TODO: need check HLE_INT_STATUS */
    return HAL_OK;
}

/**
 * @brief  Reset FIFO.
 * @param  host: private hal host data.
 * @return HAL_Status.
 */
HAL_Status HAL_MMC_ResetFifo(struct HAL_MMC_HOST *host)
{
    struct MMC_REG *pReg = host->pReg;
    uint32_t timeoutMs = 5000, reg, startTick;

    reg = READ_REG(pReg->CTRL);
    reg |= MMC_CTRL_FIFO_RESET;
    WRITE_REG(pReg->CTRL, reg);

    /* Wait until fifo reset finish */
    startTick = HAL_GetTick();
    while (READ_REG(pReg->CTRL) & MMC_CTRL_FIFO_RESET) {
        if ((HAL_GetTick() - startTick) > timeoutMs) {
            HAL_DBG_ERR("%s: FIFO reset timeout\n", __func__);

            return HAL_TIMEOUT;
        }
    }

    return HAL_OK;
}

/**
 * @brief  Reset controller.
 * @param  host: private hal host data.
 * @return HAL_Status.
 */
static HAL_Status HAL_MMC_Reset(struct HAL_MMC_HOST *host)
{
    uint32_t reg, timeoutMs = 5000, startTick;;
    struct MMC_REG *pReg = host->pReg;

    reg = READ_REG(pReg->BMOD);
    reg |= MMC_BMOD_RESET;
    WRITE_REG(pReg->BMOD, reg);

    startTick = HAL_GetTick();
    while (READ_REG(pReg->BMOD) & MMC_BMOD_RESET) {
        if ((HAL_GetTick() - startTick) > timeoutMs) {
            HAL_DBG_ERR("%s: BMOD Software reset timeout\n", __func__);

            return HAL_TIMEOUT;
        }
    }

    reg = READ_REG(pReg->CTRL);
    reg |= MMC_CTRL_CONTROLLER_RESET | MMC_CTRL_FIFO_RESET | MMC_CTRL_DMA_RESET;
    WRITE_REG(pReg->CTRL, reg);

    startTick = HAL_GetTick();
    while (READ_REG(pReg->CTRL) & (MMC_CTRL_CONTROLLER_RESET |
                                   MMC_CTRL_FIFO_RESET | MMC_CTRL_DMA_RESET)) {
        if ((HAL_GetTick() - startTick) > timeoutMs) {
            HAL_DBG_ERR("%s: CTRL dma|fifo|ctrl reset timeout\n", __func__);

            return HAL_TIMEOUT;
        }
    }

    return HAL_OK;
}

/**
 * @brief  Start Dma.
 * @param  host: private hal host data.
 * @return HAL_Status.
 */
inline HAL_Status HAL_MMC_StartDma(struct HAL_MMC_HOST *host)
{
    uint32_t reg;
    struct MMC_REG *pReg = host->pReg;

    WRITE_REG(pReg->DBADDR, (uint32_t)host->descriptors);

    reg = READ_REG(pReg->BMOD);
    reg |= HAL_BIT(7);
    WRITE_REG(pReg->BMOD, reg);

    return HAL_OK;
}

/**
 * @brief  Stop Dma.
 * @param  host: private hal host data.
 * @return HAL_Status.
 */
inline HAL_Status HAL_MMC_StopDma(struct HAL_MMC_HOST *host)
{
    uint32_t reg;
    struct MMC_REG *pReg = host->pReg;

    reg = READ_REG(pReg->BMOD);
    reg &= ~HAL_BIT(7);
    WRITE_REG(pReg->BMOD, reg);

    return HAL_OK;
}

/**
 * @brief  Power control.
 * @param  host: private hal host data.
 * @param  on: TRUE stands for power on and FALSE stands for power off.
 * @return HAL_Status.
 */
inline HAL_Status HAL_MMC_PowerCtrl(struct HAL_MMC_HOST *host, bool on)
{
    uint32_t reg;
    struct MMC_REG *pReg = host->pReg;

    reg = READ_REG(pReg->PWREN);
    if (on) {
        reg |= HAL_BIT(0);
    } else {
        reg &= ~HAL_BIT(0);
    }

    WRITE_REG(pReg->PWREN, reg);

    return HAL_OK;
}

/** @} */

/** @defgroup SDIO_Exported_Functions_Group4 Init and DeInit Functions

 This section provides functions allowing to init and deinit the module:

 *  @{
 */

/**
 * @brief  Init controller.
 * @param  host: private hal host data.
 * @return HAL_Status.
 */
HAL_Status HAL_MMC_Init(struct HAL_MMC_HOST *host)
{
    uint32_t reg;
    struct MMC_REG *pReg;

    if (host->MMC_Reset) {
        host->MMC_Reset(host);
    }

    HAL_ASSERT(host->pReg != NULL);

    pReg = host->pReg;

    HAL_MMC_Reset(host);

    /* Fixed burst for DMA master burst */
    reg = READ_REG(pReg->BMOD);
    reg |= HAL_BIT(1);
    WRITE_REG(pReg->BMOD, reg);

    /* TODO: pinmux -> power-ctrl  */
    HAL_MMC_ClearRawInterrupt(host, MMC_INT_STATUS_ALL);
    HAL_MMC_SetInterruptMask(host, 0x0);

    /* Use IDMAC */
    reg = READ_REG(pReg->CTRL);
    reg |= MMC_CTRL_INT_ENABLE;
#ifdef MMC_USE_DMA
    reg |= MMC_CTRL_USE_DMA;
#endif
    WRITE_REG(pReg->CTRL, reg);

    /* Set max timeiout value for all transfer */
    WRITE_REG(pReg->TMOUT, 0xffffffff);

    /* Set FIFO */
    reg = READ_REG(pReg->FIFOTH);
#ifndef MMC_USE_DMA
    reg = (reg >> MMC_FIFOTH_RX_WMARK_SHIFT) & MMC_FIFOTH_RX_WMARK_MASK;
    reg = ((MMC_FIFOTH_DMA_MULTIPLE_TRANSACTION_SIZE_8
            << MMC_FIFOTH_DMA_MUTIPLE_TRANSACTION_SIZE_SHIFT) |
           ((reg / 2) << MMC_FIFOTH_RX_WMARK_SHIFT) |
           ((reg / 2 + 1) << MMC_FIFOTH_TX_WMARK_SHIFT));

#else
    reg = ((reg >> MMC_FIFOTH_RX_WMARK_SHIFT) & 0xfff) + 1;
    reg = ((MMC_FIFOTH_DMA_MULTIPLE_TRANSACTION_SIZE_8
            << MMC_FIFOTH_DMA_MUTIPLE_TRANSACTION_SIZE_SHIFT) |
           ((reg / 2 - 1) << MMC_FIFOTH_RX_WMARK_SHIFT) |
           ((reg / 2) << MMC_FIFOTH_TX_WMARK_SHIFT));
#endif

    WRITE_REG(pReg->FIFOTH, reg);

    WRITE_REG(pReg->TMOUT, MMC_TMOUT_RESPONSE_TIMEOUT_MASK | MMC_TMOUT_DATA_TIMEOUT_MASK);

    return HAL_OK;
}

/**
 * @brief  DeInit controller.
 * @param  host: private hal host data.
 * @return HAL_Status.
 */
HAL_Status HAL_MMC_DeInit(struct HAL_MMC_HOST *host)
{
    /* TBD */
    return HAL_OK;
}

/** @} */

/** @} */

/** @} */

#endif /* HAL_SDIO_MODULE_ENABLED */
