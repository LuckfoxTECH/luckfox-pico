/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_SPI_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
  * @{
  */
/** @defgroup SPI SPI
  * @brief SPI HAL module driver
  * @{
  */

/** @defgroup How_To_Use How To Use
 *  @{

 The SPI HAL driver can be used as follows:

 - Declare a SPI_Handle handle structure, for example:
   ```
   SPI_Handle instance;
   ```

 - Invoke HAL_SPI_Init() API to configure default config:
     - opMode: slave or master
     - apbTransform
     - endianMode
     - ssd
     - Clock rate

 - Invoke HAL_SPI_Configure() API to program other mode:
     - Data size
     - Clock polarity and phase
     - FirstBit
     - Clock div
     - Number of data frames received at RX only mode
     - IT FIFO Level and DMA FIFO Level
     - Transfer Mode

 - There are two modes of transfer:
     - Blocking mode: The communication is performed in polling mode by calling HAL_SPI_PioTransfer().
     - No-Blocking mode: The communication is performed using Interrupts or DMA.
         - The HAL_SPI_ItTransfer(), HAL_SPI_IrqHandler() is used for Interrupt mode.
         - The HAL_SPI_DmaTransfer() is used for DMA mode, and Hal driver dones not
         - provide more DMA functions.

 - Invoke HAL_SPI_DeInit() if necessary.

 - More details refer to APIs' descriptions as below.

 Using the HAL it is not possible to reach all supported SPI frequency with the differents SPI Modes.

 @} */

/** @defgroup Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/

#define SPI_VER1_TYPE1 0x0B050001

#if (SPI_VERSION == SPI_VER1_TYPE1)
#define HAL_SPI_FIFO_LENGTH 32
#else
#define HAL_SPI_FIFO_LENGTH 64
#endif
/* Bit fields in SR */
#define HAL_SPI_SR_BUSY (0x1 << SPI_SR_BSF_SHIFT)
#ifdef SPI_SR_STB_SHIFT
#define HAL_SPI_SR_STB_BUSY (0x1 << SPI_SR_STB_SHIFT)
#else
#define HAL_SPI_SR_STB_BUSY (0x1 << SPI_SR_BSF_SHIFT)
#endif

/* Bit fields in ISR, IMR, RISR, 7 bits */
#define SPI_INT_TXEI (1 << SPI_IMR_TFEIM_SHIFT)
#define SPI_INT_TXOI (1 << SPI_IMR_TFOIM_SHIFT)
#define SPI_INT_RXUI (1 << SPI_IMR_RFUIM_SHIFT)
#define SPI_INT_RXOI (1 << SPI_IMR_RFOIM_SHIFT)
#define SPI_INT_RXFI (1 << SPI_IMR_RFFIM_SHIFT)
#define SPI_INT_TOI  (1 << SPI_IMR_TOIM_SHIFT)
#define SPI_INT_SSPI (1 << SPI_IMR_SSPIM_SHIFT)
#if (SPI_VERSION != SPI_VER1_TYPE1)
#define SPI_INT_TXFIM (1 << SPI_IMR_TXFIM_SHIFT)
#endif

/* Bit fields in ICR */
#define SPI_CLEAR_INT_ALL  (1 << SPI_ICR_CCI_SHIFT)
#define SPI_CLEAR_INT_RXUI (1 << SPI_ICR_CRFUI_SHIFT)
#define SPI_CLEAR_INT_RXOI (1 << SPI_ICR_CRFOI_SHIFT)
#define SPI_CLEAR_INT_TXOI (1 << SPI_ICR_CTFOI_SHIFT)
#define SPI_CLEAR_INT_TOI  (1 << SPI_ICR_CTOI_SHIFT)
#define SPI_ICR_SSPI_SHIFT (1 << SPI_ICR_CSSPI_SHIFT)
#ifdef SPI_ICR_CTXFI_SHIFT
#define SPI_CLEAR_INT_TXFI (1 << SPI_ICR_CTXFI_SHIFT)
#else
#define SPI_CLEAR_INT_TXFI (1 << SPI_ICR_CTFOI_SHIFT)
#endif

/* Bit fields in DMACR */
#define SPI_DMACR_TX_ENABLE (1 << SPI_DMACR_TDE_SHIFT)
#define SPI_DMACR_RX_ENABLE (1 << SPI_DMACR_RDE_SHIFT)

/* Bit fields in SPI TIMEOUT */
#define SPI_TIMEOUT_ENABLE  (1 << SPI_TIMEOUT_TOE_SHIFT)
#define SPI_TIMEOUT_DISABLE 0

#define IS_SPI_MODE(__MODE__) (((__MODE__) == CR0_OPM_SLAVE) || \
                               ((__MODE__) == CR0_OPM_MASTER))

#define IS_SPI_DIRECTION(__MODE__) (((__MODE__) == CR0_XFM_TR)        || \
                                    ((__MODE__) == CR0_XFM_TO) ||        \
                                    ((__MODE__) == CR0_XFM_RO))

#define IS_SPI_DATASIZE(__DATASIZE__) (((__DATASIZE__) == CR0_DATA_FRAME_SIZE_4BIT) || \
                                       ((__DATASIZE__) == CR0_DATA_FRAME_SIZE_8BIT) || \
                                       ((__DATASIZE__) == CR0_DATA_FRAME_SIZE_16BIT))

#define IS_SPI_CPOL(__CPOL__) (((__CPOL__) == CR0_POLARITY_LOW) || \
                               ((__CPOL__) == CR0_POLARITY_HIGH))

#define IS_SPI_CPHA(__CPHA__) (((__CPHA__) == CR0_PHASE_1EDGE) || \
                               ((__CPHA__) == CR0_PHASE_2EDGE))

#define IS_SPI_FIRST_BIT(__BIT__) (((__BIT__) == CR0_FIRSTBIT_MSB) || \
                                   ((__BIT__) == CR0_FIRSTBIT_LSB))

#define IS_SPI_APBTRANSFORM(__MODE__) (((__MODE__) == CR0_BHT_16BIT) || \
                                      ((__MODE__) == CR0_BHT_8BIT))

#define IS_SPI_ENDIAN_MODE(__MODE__) (((__MODE__) == CR0_EM_BIG) || \
                                      ((__MODE__) == CR0_EM_LITTLE))

#define IS_SPI_SSD_BIT(__MODE__) (((__MODE__) == CR0_SSD_HALF) || \
                                  ((__MODE__) == CR0_SSD_ONE))

#define IS_SPI_CSM(__NCYCLES__) (((__NCYCLES__) == CR0_CSM_0CYCLE) ||  \
                                 ((__NCYCLES__) == CR0_CSM_1CYCLE) ||  \
                                 ((__NCYCLES__) == CR0_CSM_2CYCLES) || \
                                 ((__NCYCLES__) == CR0_CSM_3CYCLES))

/*
 * About 200us cost for calling DMA function in each SPI DMA xfer in whtch it
 * can transfer 10Kbps in 50MHz IO rate. Unless DMA large data, or it's CPU waste.
 */
#define HAL_SPI_DMA_SIZE_MIN 512

/** @} */
/********************* Public Function Definition ****************************/

/** @defgroup SPI_Exported_Functions_Group4 Init and DeInit Functions

 This section provides functions allowing to init and deinit module as follows:

 *  @{
 */

/**
  * @brief  Initialize the SPI according to the specified parameters.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for SPI module.
  * @param  base: SPI controller register base address.
  * @param  slave: working at slave or master.
  * @return HAL status
  */
HAL_Status HAL_SPI_Init(struct SPI_HANDLE *pSPI, uint32_t base, bool slave)
{
    /* Check the SPI handle allocation */
    HAL_ASSERT(pSPI != NULL);

    pSPI->pReg = (struct SPI_REG *)base;
    HAL_ASSERT(IS_SPI_INSTANCE(pSPI->pReg));

    if (slave) {
        pSPI->config.opMode = CR0_OPM_SLAVE;
    } else {
        pSPI->config.opMode = CR0_OPM_MASTER;
    }

    /* Default config */
    pSPI->config.apbTransform = CR0_BHT_8BIT;
    pSPI->config.endianMode = CR0_EM_BIG;
    pSPI->config.ssd = CR0_SSD_ONE;
    pSPI->config.csm = CR0_CSM_0CYCLE;
    pSPI->dmaBurstSize = 1;

    return HAL_OK;
}

/**
  * @brief  DeInitialize the SPI peripheral.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for SPI module.
  * @return HAL status
  */
HAL_Status HAL_SPI_DeInit(struct SPI_HANDLE *pSPI)
{
    HAL_ASSERT(pSPI != NULL);

    /* TO-DO */
    return HAL_OK;
}

/** @} */

/** @defgroup SPI_Exported_Functions_Group3 IO Functions

 This section provides functions allowing to IO controlling:

 *  @{
 */

/**
  * @brief  Start or stop the spi module.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for SPI module.
  * @param  enable: start or stop the spi module.
  * @return HAL status
  */
static inline HAL_Status HAL_SPI_EnableChip(struct SPI_HANDLE *pSPI, int enable)
{
    WRITE_REG(pSPI->pReg->ENR, (enable ? 1 : 0));

    return HAL_OK;
}

/**
  * @brief  Configure the spi clock division.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for SPI module.
  * @param  div: clock division.
  * @return HAL status
  */
static inline HAL_Status HAL_SPI_SetClock(struct SPI_HANDLE *pSPI, uint16_t div)
{
    WRITE_REG(pSPI->pReg->BAUDR, div);

    return HAL_OK;
}

/**
  * @brief  Configure the cs signal.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for SPI module.
  * @param select: cs number select.
  * @param  enable: active or inactive the cs signal.
  * @return HAL status
  */
HAL_Status HAL_SPI_SetCS(struct SPI_HANDLE *pSPI, char select, bool enable)
{
    uint32_t ser;

    HAL_ASSERT(pSPI != NULL);

    ser = READ_REG(pSPI->pReg->SER) & SPI_SER_SER_MASK;

    if (enable) {
        ser |= 1 << select;
    } else {
        ser &= ~(1 << select);
    }

    WRITE_REG(pSPI->pReg->SER, ser);

    return HAL_OK;
}

/**
  * @brief  Wait for the transfer finished.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for SPI module.
  * @return HAL status
  */
HAL_Status HAL_SPI_FlushFifo(struct SPI_HANDLE *pSPI)
{
    HAL_ASSERT(pSPI != NULL);

    while (READ_REG(pSPI->pReg->RXFLR)) {
        READ_REG(pSPI->pReg->RXDR);
    }

    return HAL_OK;
}

/**
  * @brief  Calculate the timeout for transfer finished.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for SPI module.
  * @return Time
  */
uint32_t HAL_SPI_CalculateTimeout(struct SPI_HANDLE *pSPI)
{
    uint32_t timeout;
    uint32_t div, speed;

    HAL_ASSERT(pSPI != NULL);

    div = HAL_DIV_ROUND_UP(pSPI->maxFreq, pSPI->config.speed);
    div = (div + 1) & 0xfffe;

    speed = pSPI->maxFreq / div;

    timeout = pSPI->len * 8 * 1000 / speed;
    timeout += timeout + 100; /* some tolerance */

    return timeout;
}

/**
  * @brief  Query the SPI bus state is idle or busy.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for SPI module.
  * @return HAL status
  */
HAL_Status HAL_SPI_QueryBusState(struct SPI_HANDLE *pSPI)
{
    HAL_ASSERT(pSPI != NULL);

    if (pSPI->config.opMode == CR0_OPM_SLAVE) {
#if (SPI_VERSION == SPI_VER1_TYPE1)
        if (READ_REG(pSPI->pReg->SR) & SPI_SR_TFE_MASK) {
            uint32_t div, speed, us;

            div = HAL_DIV_ROUND_UP(pSPI->maxFreq, pSPI->config.speed);
            div = (div + 1) & 0xfffe;
            speed = pSPI->maxFreq / div;
            us = 8 * 1000000 / speed + 1;
            HAL_DelayUs(us);

            return HAL_OK;
        }
#else
        if (!(READ_REG(pSPI->pReg->SR) & HAL_SPI_SR_STB_BUSY)) {
            return HAL_OK;
        }
#endif
    } else {
        if (!(READ_REG(pSPI->pReg->SR) & HAL_SPI_SR_BUSY)) {
            return HAL_OK;
        }
    }

    return HAL_BUSY;
}

/**
  * @brief  The max amount of data can be written in blocking mode.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for SPI module.
  * @return Max bytes can xfer.
  */
static inline uint32_t HAL_SPI_TxMax(struct SPI_HANDLE *pSPI)
{
    uint32_t txLeft, txRoom;

    txLeft = (pSPI->pTxBufferEnd - pSPI->pTxBuffer) / pSPI->config.nBytes;
    txRoom = HAL_SPI_FIFO_LENGTH - READ_REG(pSPI->pReg->TXFLR);

    return HAL_MIN(txLeft, txRoom);
}

/**
  * @brief  Send an amount of data in blocking mode.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for SPI module.
  * @return HAL status
  */
static HAL_Status HAL_SPI_PioWrite(struct SPI_HANDLE *pSPI)
{
    uint32_t max = HAL_SPI_TxMax(pSPI);
    uint32_t txw = 0;

    while (max--) {
        if (pSPI->config.nBytes == 1) {
            txw = *(const uint8_t *)(pSPI->pTxBuffer);
        } else {
            txw = *(const uint16_t *)(pSPI->pTxBuffer);
        }

        WRITE_REG(pSPI->pReg->TXDR, txw);
        pSPI->pTxBuffer += pSPI->config.nBytes;
    }

    return HAL_OK;
}

#if (SPI_VERSION == SPI_VER1_TYPE1)
/**
  * @brief  Read an amount of data(byte) in blocking mode.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for SPI module.
  * @return HAL status
  */
static HAL_Status HAL_SPI_PioReadByte(struct SPI_HANDLE *pSPI)
{
    uint32_t rxRoom = READ_REG(pSPI->pReg->RXFLR);
    uint32_t rxLen = pSPI->pRxBufferEnd - pSPI->pRxBuffer;
    uint32_t ftl, rxLeft;

    rxLeft = rxLen > rxRoom ? rxLen - rxRoom : 0;
    /*
     * the low version hardware doesn't allow us to change fifo
     * threshold level while spi is enabled, so instead make sure to
     * leave enough words in the rx fifo to get the last interrupt
     * exactly when all words have been received
     */
    if (rxLeft) {
        ftl = READ_REG(pSPI->pReg->RXFTLR) + 1;

        if (rxLeft < ftl) {
            rxLeft = ftl;
            rxRoom = rxLen - rxLeft;
        }
    }

    while (rxRoom > 7) {
        *(pSPI->pRxBuffer + 0) = (uint8_t)READ_REG(pSPI->pReg->RXDR);
        *(pSPI->pRxBuffer + 1) = (uint8_t)READ_REG(pSPI->pReg->RXDR);
        *(pSPI->pRxBuffer + 2) = (uint8_t)READ_REG(pSPI->pReg->RXDR);
        *(pSPI->pRxBuffer + 3) = (uint8_t)READ_REG(pSPI->pReg->RXDR);
        *(pSPI->pRxBuffer + 4) = (uint8_t)READ_REG(pSPI->pReg->RXDR);
        *(pSPI->pRxBuffer + 5) = (uint8_t)READ_REG(pSPI->pReg->RXDR);
        *(pSPI->pRxBuffer + 6) = (uint8_t)READ_REG(pSPI->pReg->RXDR);
        *(pSPI->pRxBuffer + 7) = (uint8_t)READ_REG(pSPI->pReg->RXDR);
        pSPI->pRxBuffer += 8;
        rxRoom -= 8;
    }

    while (rxRoom--) {
        *pSPI->pRxBuffer = (uint8_t)READ_REG(pSPI->pReg->RXDR);
        pSPI->pRxBuffer++;
    }

    return HAL_OK;
}

/**
  * @brief  Read an amount of data(short) in blocking mode.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for SPI module.
  * @return HAL status
  */
static HAL_Status HAL_SPI_PioReadShort(struct SPI_HANDLE *pSPI)
{
    uint32_t rxRoom = READ_REG(pSPI->pReg->RXFLR);
    uint32_t rxLen = (pSPI->pRxBufferEnd - pSPI->pRxBuffer) >> 1;
    uint32_t ftl, rxLeft;

    rxLeft = rxLen > rxRoom ? rxLen - rxRoom : 0;
    /*
     * the low version hardware doesn't allow us to change fifo
     * threshold level while spi is enabled, so instead make sure to
     * leave enough words in the rx fifo to get the last interrupt
     * exactly when all words have been received
     */
    if (rxLeft) {
        ftl = READ_REG(pSPI->pReg->RXFTLR) + 1;

        if (rxLeft < ftl) {
            rxLeft = ftl;
            rxRoom = rxLen - rxLeft;
        }
    }

    while (rxRoom > 7) {
        *((uint16_t *)pSPI->pRxBuffer + 0) = (uint16_t)READ_REG(pSPI->pReg->RXDR);
        *((uint16_t *)pSPI->pRxBuffer + 1) = (uint16_t)READ_REG(pSPI->pReg->RXDR);
        *((uint16_t *)pSPI->pRxBuffer + 2) = (uint16_t)READ_REG(pSPI->pReg->RXDR);
        *((uint16_t *)pSPI->pRxBuffer + 3) = (uint16_t)READ_REG(pSPI->pReg->RXDR);
        *((uint16_t *)pSPI->pRxBuffer + 4) = (uint16_t)READ_REG(pSPI->pReg->RXDR);
        *((uint16_t *)pSPI->pRxBuffer + 5) = (uint16_t)READ_REG(pSPI->pReg->RXDR);
        *((uint16_t *)pSPI->pRxBuffer + 6) = (uint16_t)READ_REG(pSPI->pReg->RXDR);
        *((uint16_t *)pSPI->pRxBuffer + 7) = (uint16_t)READ_REG(pSPI->pReg->RXDR);
        pSPI->pRxBuffer += 16;
        rxRoom -= 8;
    }

    while (rxRoom--) {
        *((uint16_t *)pSPI->pRxBuffer) = (uint16_t)READ_REG(pSPI->pReg->RXDR);
        pSPI->pRxBuffer += 2;
    }

    return HAL_OK;
}

#else

/**
  * @brief  Read an amount of data(byte) in blocking mode.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for SPI module.
  * @return HAL status
  */
static HAL_Status HAL_SPI_PioReadByte(struct SPI_HANDLE *pSPI)
{
    uint32_t rxLeft = pSPI->pRxBufferEnd - pSPI->pRxBuffer;
    uint32_t rxRoom = READ_REG(pSPI->pReg->RXFLR);
    uint32_t max = HAL_MIN(rxLeft, rxRoom);

    while (max > 7) {
        *(pSPI->pRxBuffer + 0) = (uint8_t)READ_REG(pSPI->pReg->RXDR);
        *(pSPI->pRxBuffer + 1) = (uint8_t)READ_REG(pSPI->pReg->RXDR);
        *(pSPI->pRxBuffer + 2) = (uint8_t)READ_REG(pSPI->pReg->RXDR);
        *(pSPI->pRxBuffer + 3) = (uint8_t)READ_REG(pSPI->pReg->RXDR);
        *(pSPI->pRxBuffer + 4) = (uint8_t)READ_REG(pSPI->pReg->RXDR);
        *(pSPI->pRxBuffer + 5) = (uint8_t)READ_REG(pSPI->pReg->RXDR);
        *(pSPI->pRxBuffer + 6) = (uint8_t)READ_REG(pSPI->pReg->RXDR);
        *(pSPI->pRxBuffer + 7) = (uint8_t)READ_REG(pSPI->pReg->RXDR);
        pSPI->pRxBuffer += 8;
        max -= 8;
    }

    while (max--) {
        *pSPI->pRxBuffer = (uint8_t)READ_REG(pSPI->pReg->RXDR);
        pSPI->pRxBuffer++;
    }

    return HAL_OK;
}

/**
  * @brief  Read an amount of data(short) in blocking mode.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for SPI module.
  * @return HAL status
  */
static HAL_Status HAL_SPI_PioReadShort(struct SPI_HANDLE *pSPI)
{
    uint32_t rxLeft = (pSPI->pRxBufferEnd - pSPI->pRxBuffer) >> 1;
    uint32_t rxRoom = READ_REG(pSPI->pReg->RXFLR);
    uint32_t max = HAL_MIN(rxLeft, rxRoom);

    while (max > 7) {
        *((uint16_t *)pSPI->pRxBuffer + 0) = (uint16_t)READ_REG(pSPI->pReg->RXDR);
        *((uint16_t *)pSPI->pRxBuffer + 1) = (uint16_t)READ_REG(pSPI->pReg->RXDR);
        *((uint16_t *)pSPI->pRxBuffer + 2) = (uint16_t)READ_REG(pSPI->pReg->RXDR);
        *((uint16_t *)pSPI->pRxBuffer + 3) = (uint16_t)READ_REG(pSPI->pReg->RXDR);
        *((uint16_t *)pSPI->pRxBuffer + 4) = (uint16_t)READ_REG(pSPI->pReg->RXDR);
        *((uint16_t *)pSPI->pRxBuffer + 5) = (uint16_t)READ_REG(pSPI->pReg->RXDR);
        *((uint16_t *)pSPI->pRxBuffer + 6) = (uint16_t)READ_REG(pSPI->pReg->RXDR);
        *((uint16_t *)pSPI->pRxBuffer + 7) = (uint16_t)READ_REG(pSPI->pReg->RXDR);
        pSPI->pRxBuffer += 16;
        max -= 8;
    }

    while (max--) {
        *((uint16_t *)pSPI->pRxBuffer) = (uint16_t)READ_REG(pSPI->pReg->RXDR);
        pSPI->pRxBuffer += 2;
    }

    return HAL_OK;
}
#endif

/**
  * @brief  Transmit an amount of data in blocking mode.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for SPI module.
  * @return HAL status
  */
HAL_Status HAL_SPI_PioTransfer(struct SPI_HANDLE *pSPI)
{
    uint32_t remain = 0;

    HAL_ASSERT(pSPI != NULL);

    pSPI->type = SPI_POLL;
    HAL_SPI_EnableChip(pSPI, 1);

    do {
        if (pSPI->pTxBuffer) {
            remain = pSPI->pTxBufferEnd - pSPI->pTxBuffer;
            HAL_SPI_PioWrite(pSPI);
        }

        if (pSPI->pRxBuffer) {
            remain = pSPI->pRxBufferEnd - pSPI->pRxBuffer;
            if (pSPI->config.nBytes == 1) {
                HAL_SPI_PioReadByte(pSPI);
            } else {
                HAL_SPI_PioReadShort(pSPI);
            }
        }
    } while (remain);

    return HAL_OK;
}

/**
  * @brief  Disable IRQ bits.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for the specified SPI module.
  *         mask: The bit will be mask.
  * @return HAL status
  */
static inline HAL_Status HAL_SPI_MaskIntr(struct SPI_HANDLE *pSPI, uint32_t mask)
{
    uint32_t newMask;

    newMask = READ_REG(pSPI->pReg->IMR) & ~mask;
    WRITE_REG(pSPI->pReg->IMR, newMask);

    return HAL_OK;
}

/**
  * @brief  Enable IRQ bits.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for the specified SPI module.
  *         mask: The bit will be unmask.
  * @return HAL status
  */
static inline HAL_Status HAL_SPI_UnmaskIntr(struct SPI_HANDLE *pSPI, uint32_t mask)
{
    uint32_t newMask;

    newMask = READ_REG(pSPI->pReg->IMR) | mask;
    WRITE_REG(pSPI->pReg->IMR, newMask);

    return HAL_OK;
}

/**
  * @brief  Handle SPI interrupt request.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for the specified SPI module.
  * @return HAL status
  */
HAL_Status HAL_SPI_IrqHandler(struct SPI_HANDLE *pSPI)
{
    uint32_t irqStatus = READ_REG(pSPI->pReg->ISR);
    uint32_t int_level = HAL_SPI_FIFO_LENGTH / 2;
    uint32_t left;
    HAL_Status result;

    HAL_ASSERT(pSPI != NULL);

    if (!irqStatus) {
        result = HAL_NODEV;
        goto out;
    }

    if (!(pSPI->pTxBuffer || pSPI->pRxBuffer)) {
        result = HAL_INVAL;
        goto out;
    }

    /* Error handling */
    if (irqStatus & (SPI_INT_TXOI | SPI_INT_RXOI | SPI_INT_RXUI)) {
        WRITE_REG(pSPI->pReg->ICR, SPI_CLEAR_INT_TXOI | SPI_CLEAR_INT_RXOI | SPI_CLEAR_INT_RXUI);
        result = HAL_ERROR;
        goto out;
    }

    if (irqStatus & SPI_INT_RXFI) {
        HAL_SPI_MaskIntr(pSPI, SPI_INT_RXFI);
        if (pSPI->config.nBytes == 1) {
            HAL_SPI_PioReadByte(pSPI);
        } else {
            HAL_SPI_PioReadShort(pSPI);
        }
        HAL_SPI_UnmaskIntr(pSPI, SPI_INT_RXFI);

        if (pSPI->pRxBufferEnd > pSPI->pRxBuffer) {
            left = ((pSPI->pRxBufferEnd - pSPI->pRxBuffer) / pSPI->config.nBytes) - 1;
            left = (left > int_level) ? int_level : left;
            WRITE_REG(pSPI->pReg->RXFTLR, left);

            if (pSPI->config.xfmMode == CR0_XFM_TR) {
                HAL_SPI_PioWrite(pSPI);
            }
        } else {
            result = HAL_OK;
            goto out;
        }
    }

    if ((irqStatus & SPI_INT_TXEI) && (pSPI->pTxBufferEnd > pSPI->pTxBuffer)) {
        HAL_SPI_MaskIntr(pSPI, SPI_INT_TXEI);
        HAL_SPI_PioWrite(pSPI);
        HAL_SPI_UnmaskIntr(pSPI, SPI_INT_TXEI);
    }

#if (SPI_VERSION == SPI_VER1_TYPE1)
    if ((irqStatus & SPI_INT_TXEI) && pSPI->pTxBuffer && pSPI->pTxBufferEnd == pSPI->pTxBuffer) {
        uint32_t div, speed, us;

        div = HAL_DIV_ROUND_UP(pSPI->maxFreq, pSPI->config.speed);
        div = (div + 1) & 0xfffe;
        speed = pSPI->maxFreq / div;
        us = 8 * 1000000 / speed + 1;
        HAL_DelayUs(us);

        result = HAL_OK;
        goto out;
    }
#else
    if ((irqStatus & SPI_INT_TXFIM) && (pSPI->pTxBufferEnd == pSPI->pTxBuffer)) {
        WRITE_REG(pSPI->pReg->ICR, SPI_CLEAR_INT_TXFI);
        result = HAL_OK;
        goto out;
    }
#endif

    return HAL_BUSY;

out:
    WRITE_REG(pSPI->pReg->IMR, 0);
    WRITE_REG(pSPI->pReg->ICR, SPI_CLEAR_INT_ALL);

    return result;
}

/**
  * @brief  Transmit an amount of data with interrupt in noblocking mode.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for SPI module.
  * @return HAL_status
  */
HAL_Status HAL_SPI_ItTransfer(struct SPI_HANDLE *pSPI)
{
    uint32_t txLevel, rxLevel, tempLevel;
    uint32_t newMask;

    HAL_ASSERT(pSPI != NULL);

    pSPI->type = SPI_IT;
    if (pSPI->config.xfmMode == CR0_XFM_RO || pSPI->config.xfmMode == CR0_XFM_TR) {
        tempLevel = pSPI->len / pSPI->config.nBytes - 1;
        rxLevel = HAL_SPI_FIFO_LENGTH / 2;
        rxLevel = (tempLevel > rxLevel) ? rxLevel : tempLevel;

        if (rxLevel && rxLevel != READ_REG(pSPI->pReg->RXFTLR)) {
            WRITE_REG(pSPI->pReg->RXFTLR, rxLevel - 1);
        }

        HAL_SPI_EnableChip(pSPI, 1);

        if (pSPI->config.xfmMode == CR0_XFM_TR) {
            HAL_SPI_PioWrite(pSPI);
        }

        newMask = SPI_INT_RXFI | SPI_INT_RXOI | SPI_INT_RXUI;
    } else {
        tempLevel = pSPI->len / (uint16_t)pSPI->config.nBytes;
        txLevel = HAL_SPI_FIFO_LENGTH / 2;
        txLevel = (tempLevel > txLevel) ? txLevel : tempLevel;

        if (txLevel != READ_REG(pSPI->pReg->TXFTLR)) {
            WRITE_REG(pSPI->pReg->TXFTLR, txLevel);
        }

        HAL_SPI_EnableChip(pSPI, 1);
        HAL_SPI_PioWrite(pSPI);
        newMask = SPI_INT_TXEI | SPI_INT_TXOI;
#if (SPI_VERSION != SPI_VER1_TYPE1)
        newMask |= SPI_INT_TXFIM;
#endif
    }

    WRITE_REG(pSPI->pReg->IMR, newMask);

    return HAL_OK;
}

/**
  * @brief  Is the SPI slave mode or not.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for SPI module.
  * @return Bool
  */
bool HAL_SPI_IsSlave(struct SPI_HANDLE *pSPI)
{
    HAL_ASSERT(pSPI != NULL);

    return (pSPI->config.opMode == CR0_OPM_SLAVE);
}

/**
  * @brief  Can the Transmit use dma in noblocking mode or not.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for SPI module.
  * @return Bool
  */
bool HAL_SPI_IsDmaXfer(struct SPI_HANDLE *pSPI)
{
    HAL_ASSERT(pSPI != NULL);

    return (pSPI->type == SPI_DMA);
}

/**
  * @brief  Can the Transmit use dma in noblocking mode or not.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for SPI module.
  * @return Bool
  */
bool HAL_SPI_CanDma(struct SPI_HANDLE *pSPI)
{
    HAL_ASSERT(pSPI != NULL);

#ifdef HAL_DCACHE_MODULE_ENABLED
    if (!HAL_IS_CACHELINE_ALIGNED(pSPI->pRxBuffer) ||
        !HAL_IS_CACHELINE_ALIGNED(pSPI->pTxBuffer) ||
        !HAL_IS_CACHELINE_ALIGNED(pSPI->len)) {
        return false;
    }
#endif

    return (pSPI->len > HAL_SPI_DMA_SIZE_MIN);
}

/**
  * @brief  Transmit an amount of data with dma in noblocking mode.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for SPI module.
  * @return HAL status
  */
HAL_Status HAL_SPI_DmaTransfer(struct SPI_HANDLE *pSPI)
{
    uint32_t dmacr = 0;

    HAL_ASSERT(pSPI != NULL);

    pSPI->type = SPI_DMA;
    if (HAL_SPI_CanDma(pSPI)) {
        if (pSPI->pTxBuffer) {
            dmacr |= SPI_DMACR_TX_ENABLE;
        }
        if (pSPI->pRxBuffer) {
            dmacr |= SPI_DMACR_RX_ENABLE;
        }
    }

    WRITE_REG(pSPI->pReg->DMACR, dmacr);

    HAL_SPI_EnableChip(pSPI, 1);

    return HAL_OK;
}

/**
  * @brief  Stop the transmit.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for SPI module.
  * @return HAL status
  */
HAL_Status HAL_SPI_Stop(struct SPI_HANDLE *pSPI)
{
    HAL_ASSERT(pSPI != NULL);

    /* IRQ disabled moved to handler is better than here */
    if (pSPI->type == SPI_DMA) {
        WRITE_REG(pSPI->pReg->DMACR, 0);
    }

    HAL_SPI_EnableChip(pSPI, 0);

    return HAL_OK;
}

/**
  * @brief  Configure the SPI transfer mode depend on the tx/rx buffer.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for SPI module.
  * @return HAL status
  */
static HAL_Status HAL_SPI_ConfigureTransferMode(struct SPI_HANDLE *pSPI)
{
    uint32_t cr0;

    if (pSPI->pTxBuffer && pSPI->pRxBuffer) {
        pSPI->config.xfmMode = CR0_XFM_TR;
    } else if (pSPI->pTxBuffer) {
        pSPI->config.xfmMode = CR0_XFM_TO;
    } else if (pSPI->pRxBuffer) {
        pSPI->config.xfmMode = CR0_XFM_RO;
    }

    cr0 = READ_REG(pSPI->pReg->CTRLR[0]);
    cr0 &= ~SPI_CTRLR0_XFM_MASK;
    cr0 |= pSPI->config.xfmMode;

    WRITE_REG(pSPI->pReg->DMARDLR, pSPI->dmaBurstSize - 1);

    WRITE_REG(pSPI->pReg->CTRLR[0], cr0);

    return HAL_OK;
}

/**
  * @brief  Program the SPI config via this api.
  * @param  pSPI: pointer to a SPI_Handle structure that contains
  *               the configuration information for SPI module.
  * @param  pTxData: pointer to TX buffer.
  * @param  pRxData: pointer to RX buffer.
  * @param  size: amount of data to be sent.
  * @return HAL status
  */
HAL_Status HAL_SPI_Configure(struct SPI_HANDLE *pSPI, const uint8_t *pTxData, uint8_t *pRxData, uint32_t size)
{
    uint32_t cr0 = 0;
    uint32_t div = 0;

    HAL_ASSERT(pSPI != NULL);
    HAL_ASSERT((pTxData != NULL) || (pRxData != NULL));
    HAL_ASSERT(IS_SPI_MODE(pSPI->config.opMode));
    HAL_ASSERT(IS_SPI_DIRECTION(pSPI->config.xfmMode));
    HAL_ASSERT(IS_SPI_DATASIZE(pSPI->config.nBytes));
    HAL_ASSERT(IS_SPI_CPOL(pSPI->config.clkPolarity));
    HAL_ASSERT(IS_SPI_CPHA(pSPI->config.clkPhase));
    HAL_ASSERT(IS_SPI_FIRST_BIT(pSPI->config.firstBit));
    HAL_ASSERT(IS_SPI_ENDIAN_MODE(pSPI->config.endianMode));
    HAL_ASSERT(IS_SPI_APBTRANSFORM(pSPI->config.apbTransform));
    HAL_ASSERT(IS_SPI_SSD_BIT(pSPI->config.ssd));
    HAL_ASSERT(IS_SPI_CSM(pSPI->config.csm));

    cr0 |= pSPI->config.opMode;

    cr0 |= pSPI->config.apbTransform | pSPI->config.endianMode | pSPI->config.ssd;
    /* Data width */
    cr0 |= pSPI->config.nBytes;

    /* Mode for polarity, phase, first bit and endian */
    cr0 |= pSPI->config.clkPolarity | pSPI->config.clkPhase | pSPI->config.firstBit;

    /* Config CSM cycles */
    cr0 |= pSPI->config.csm;

    /* div doesn't support odd number */
    div = HAL_DIV_ROUND_UP(pSPI->maxFreq, pSPI->config.speed);
    div = (div + 1) & 0xfffe;

    WRITE_REG(pSPI->pReg->CTRLR[0], cr0);

    WRITE_REG(pSPI->pReg->TXFTLR, HAL_SPI_FIFO_LENGTH / 2 - 1);
    WRITE_REG(pSPI->pReg->RXFTLR, HAL_SPI_FIFO_LENGTH / 2 - 1);

    WRITE_REG(pSPI->pReg->DMATDLR, HAL_SPI_FIFO_LENGTH / 2 - 1);
    WRITE_REG(pSPI->pReg->DMARDLR, 0);

    HAL_SPI_SetClock(pSPI, div);

    pSPI->pTxBuffer = pTxData;
    pSPI->pTxBufferEnd = pTxData + size;
    pSPI->pRxBuffer = pRxData;
    pSPI->pRxBufferEnd = pRxData + size;
    pSPI->len = size;

    HAL_SPI_ConfigureTransferMode(pSPI);

    if (pSPI->config.xfmMode == CR0_XFM_RO) {
        if (pSPI->config.nBytes == 1) {
            WRITE_REG(pSPI->pReg->CTRLR[1], pSPI->len - 1);
        } else if (pSPI->config.nBytes == 2) {
            WRITE_REG(pSPI->pReg->CTRLR[1], (pSPI->len / 2) - 1);
        } else {
            WRITE_REG(pSPI->pReg->CTRLR[1], (pSPI->len * 2) - 1);
        }
    }

    return HAL_OK;
}

/** @} */

/** @} */

/** @} */

#endif /* HAL_SPI_MODULE_ENABLED */
