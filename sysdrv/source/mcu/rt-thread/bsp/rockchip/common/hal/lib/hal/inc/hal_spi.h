/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_SPI_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 * @{
 */

/** @addtogroup SPI
 * @{
 */

#ifndef __HAL_SPI_H
#define __HAL_SPI_H

#include "hal_def.h"

/***************************** MACRO Definition ******************************/
/** @defgroup SPI_Exported_Definition_Group1 Basic Definition
 *  @{
 */

#define HAL_SPI_MASTER_MAX_SCLK_OUT 50000000 /**< Max io clock in master mode */
#define HAL_SPI_SLAVE_MAX_SCLK_OUT  20000000 /**< Max io in clock in slave mode */

#define CR0_DATA_FRAME_SIZE_4BIT  (0x00 << SPI_CTRLR0_DFS_SHIFT)
#define CR0_DATA_FRAME_SIZE_8BIT  (0x01 << SPI_CTRLR0_DFS_SHIFT)
#define CR0_DATA_FRAME_SIZE_16BIT (0x02 << SPI_CTRLR0_DFS_SHIFT)

/* serial clock toggles in middle of first data bit */
#define CR0_PHASE_1EDGE (0x00 << SPI_CTRLR0_SCPH_SHIFT)
/* serial clock toggles at start of first data bit */
#define CR0_PHASE_2EDGE (0x01 << SPI_CTRLR0_SCPH_SHIFT)

#define CR0_POLARITY_LOW  (0x00 << SPI_CTRLR0_SCPOL_SHIFT)
#define CR0_POLARITY_HIGH (0x01 << SPI_CTRLR0_SCPOL_SHIFT)

/*
 * The period between ss_n active and
 * sclk_out active is half sclk_out cycles
 */
#define CR0_SSD_HALF (0x00 << SPI_CTRLR0_SSD_SHIFT)
/*
 * The period between ss_n active and
 * sclk_out active is one sclk_out cycle
 */
#define CR0_SSD_ONE (0x01 << SPI_CTRLR0_SSD_SHIFT)

#define CR0_EM_LITTLE (0x0 << SPI_CTRLR0_EM_SHIFT)
#define CR0_EM_BIG    (0x1 << SPI_CTRLR0_EM_SHIFT)

#define CR0_FIRSTBIT_MSB (0x0 << SPI_CTRLR0_FBM_SHIFT)
#define CR0_FIRSTBIT_LSB (0x1 << SPI_CTRLR0_FBM_SHIFT)

#define CR0_BHT_16BIT (0x0 << SPI_CTRLR0_BHT_SHIFT)
#define CR0_BHT_8BIT  (0x1 << SPI_CTRLR0_BHT_SHIFT)

#define CR0_XFM_TR (0x00 << SPI_CTRLR0_XFM_SHIFT)
#define CR0_XFM_TO (0x01 << SPI_CTRLR0_XFM_SHIFT)
#define CR0_XFM_RO (0x02 << SPI_CTRLR0_XFM_SHIFT)

#define CR0_OPM_MASTER (0x00 << SPI_CTRLR0_OPM_SHIFT)
#define CR0_OPM_SLAVE  (0x01 << SPI_CTRLR0_OPM_SHIFT)

#define CR0_CSM(nCycles) (((nCycles) << SPI_CTRLR0_CSM_SHIFT) & SPI_CTRLR0_CSM_MASK)
#define CR0_CSM_0CYCLE   CR0_CSM(0)
#define CR0_CSM_1CYCLE   CR0_CSM(1)
#define CR0_CSM_2CYCLES  CR0_CSM(2)
#define CR0_CSM_3CYCLES  CR0_CSM(3)

/***************************** Structure Definition **************************/

/** @brief  SPI Type definition */
typedef enum {
    SSI_MOTO_SPI = 0,
    SSI_TI_SSP,
    SSI_NS_MICROWIRE
} eSPI_SSIType;

/** @brief  SPI Transfer type definition */
typedef enum {
    SPI_POLL = 0,
    SPI_IT,
    SPI_DMA
} eSPI_TransferType;

/** @brief  SPI Configuration Structure definition */
struct SPI_CONFIG {
    uint32_t opMode;           /**< Specifies the SPI operating mode, master or slave. */
    uint32_t xfmMode;          /**< Specifies the SPI bidirectional mode state, tx only, rx only or trx mode. */
    uint32_t nBytes;           /**< Specifies the SPI data size. */
    uint32_t clkPolarity;      /**< Specifies the serial clock steady state. */
    uint32_t clkPhase;         /**< Specifies the clock active edge for the bit capture. */
    uint32_t firstBit;         /**< Specifies whether data transfers start from MSB or LSB bit. */
    uint32_t endianMode;       /**< Specifies whether data transfers start from little or big endian. */
    uint32_t apbTransform;     /**< Specifies apb transform type. */
    uint32_t ssd;              /**< Specifies period between ss_n active and sclk_out. */
    uint32_t speed;            /**< Specifies the Baud Rate prescaler value which will be
                                  used to configure the transmit and receive SCK clock. */
    uint32_t ssiType;          /**< Specifies if the TI mode is enabled or not.*/
    uint32_t csm;              /**< Specifies Motorola SPI Master SS_N high cycles for each frame data is transfer. */
};

/* We have 2 DMA channels per SPI, one for RX and one for TX */
struct HAL_SPI_DMA_INFO {
    uint8_t channel;
    uint8_t direction;
    uint32_t addr;
    struct DMA_REG *dmac; /**< dmac reg base ptr */
};

struct HAL_SPI_DEV {
    const uint32_t base;
    const eCLOCK_Name clkId;
    const uint32_t clkGateID;
    const uint32_t pclkGateID;
    const uint8_t irqNum;
    const uint8_t isSlave;
    const struct HAL_SPI_DMA_INFO txDma;
    const struct HAL_SPI_DMA_INFO rxDma;
};

/** @brief  SPI handle Structure definition */
struct SPI_HANDLE {
    struct SPI_REG *pReg;          /**< Specifies SPI registers base address. */
    uint32_t maxFreq;              /**< Specifies SPI clock frequency. */
    struct SPI_CONFIG config;      /**< Specifies SPI communication parameters. */
    const uint8_t *pTxBuffer;      /**< Specifies pointer to SPI Tx transfer Buffer. */
    const uint8_t *pTxBufferEnd;   /**< Specifies pointer to SPI Tx End transfer Buffer. */
    uint8_t *pRxBuffer;            /**< Specifies pointer to SPI Rx transfer Buffer. */
    uint8_t *pRxBufferEnd;         /**< Specifies pointer to SPI Rx End transfer Buffer. */
    uint32_t len;                  /**< Specifies the transfer length . */
    eSPI_TransferType type;        /**< Specifies the transfer type: POLL/IT/DMA. */
    uint32_t dmaBurstSize;         /**< Specifies Dma Burst size */
};

/** @} */

/***************************** Function Declare ******************************/
/** @defgroup SPI_Public_Function_Declare Public Function Declare
 *  @{
 */

HAL_Status HAL_SPI_Init(struct SPI_HANDLE *pSPI, uint32_t base, bool slave);
HAL_Status HAL_SPI_DeInit(struct SPI_HANDLE *pSPI);
HAL_Status HAL_SPI_FlushFifo(struct SPI_HANDLE *pSPI);
HAL_Status HAL_SPI_SetCS(struct SPI_HANDLE *pSPI, char select, bool enable);
HAL_Status HAL_SPI_QueryBusState(struct SPI_HANDLE *pSPI);
HAL_Status HAL_SPI_PioTransfer(struct SPI_HANDLE *pSPI);
HAL_Status HAL_SPI_IrqHandler(struct SPI_HANDLE *pSPI);
HAL_Status HAL_SPI_ItTransfer(struct SPI_HANDLE *pSPI);
HAL_Status HAL_SPI_DmaTransfer(struct SPI_HANDLE *pSPI);
HAL_Status HAL_SPI_Stop(struct SPI_HANDLE *pSPI);
HAL_Status HAL_SPI_Configure(struct SPI_HANDLE *pSPI, const uint8_t *pTxData,
                             uint8_t *pRxData, uint32_t Size);
uint32_t HAL_SPI_CalculateTimeout(struct SPI_HANDLE *pSPI);
bool HAL_SPI_CanDma(struct SPI_HANDLE *pSPI);
bool HAL_SPI_IsSlave(struct SPI_HANDLE *pSPI);
bool HAL_SPI_IsDmaXfer(struct SPI_HANDLE *pSPI);

/** @} */

#endif

/** @} */

/** @} */

#endif /* HAL_SPI_MODULE_ENABLED */
