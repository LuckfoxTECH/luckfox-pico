/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_SFC_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup SFC
 *  @{
 */

/** @defgroup SFC_How_To_Use How To Use
 *  @{

 The SFC driver can be used as follows:

 This host driver need to be used in conjunction with device flash driver like
 hal_snor.c

 - DMA mode: Register handler (HAL_SFC_IRQHelper()).
 - DMA mode: Unmask TRANSM interrupt (HAL_SFC_UnmaskDMAInterrupt()).
 - Initialize SFC controller (HAL_SFC_Init());
 - Send SFC request:
    - SNOR support api: (HAL_SFC_SpiXfer());
 - DMA mode: Handling interrupt return in DMA mode SFC request.

 @} */

/** @defgroup SFC_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/

/* SFC_CTRL */
#define SFC_CTRL_SHIFTPHASE_NEGEDGE 1

/* SFC_RCVR */
#define SFC_RCVR_RCVR_RESET (1 << SFC_RCVR_RCVR_SHIFT) /* Recover The SFC State Machine */

/* SFC_ISR */
#define SFC_ISR_DMAS_ACTIVE   (1 << SFC_ISR_DMAS_SHIFT) /* DMA Finish Interrupt Active */
#define SFC_ISR_NSPIS_ACTIVE  (1 << SFC_ISR_NSPIS_SHIFT) /* SPI Error Interrupt Active */
#define SFC_ISR_AHBS_ACTIVE   (1 << SFC_ISR_AHBS_SHIFT) /* AHB Error Interrupt Active */
#define SFC_ISR_TRANSS_ACTIVE (1 << SFC_ISR_TRANSS_SHIFT) /* Transfer finish Interrupt Active */
#define SFC_ISR_TXES_ACTIVE   (1 << SFC_ISR_TXES_SHIFT) /* Transmit FIFO Empty Interrupt Active */
#define SFC_ISR_TXOS_ACTIVE   (1 << SFC_ISR_TXOS_SHIFT) /* Transmit FIFO Overflow Interrupt Active */
#define SFC_ISR_RXUS_ACTIVE   (1 << SFC_ISR_RXUS_SHIFT) /* Receive FIFO Underflow Interrupt Active */
#define SFC_ISR_RXFS_ACTIVE   (1 << SFC_ISR_RXFS_SHIFT) /* Receive FIFO Full Interrupt Active */

/* SFC_FSR */
#define SFC_FSR_RXFS_EMPTY (1 << SFC_FSR_RXFS_SHIFT) /* Receive FIFO Full */
#define SFC_FSR_RXES_EMPTY (1 << SFC_FSR_RXES_SHIFT) /* Receive FIFO Empty */
#define SFC_FSR_TXFS_FULL  (1 << SFC_FSR_TXFS_SHIFT) /* Transmit FIFO Full */
#define SFC_FSR_TXES_EMPTY (1 << SFC_FSR_TXES_SHIFT) /* Transmit FIFO Empty */

/* SFC_SR */
#define SFC_SR_SR_BUSY (1 << SFC_SR_SR_SHIFT) /* When busy, do not set the control register. */

/* SFC_DMATR */
#define SFC_DMATR_DMATR_START (1 << SFC_DMATR_DMATR_SHIFT) /* Write 1 to start the dma transfer. */

/* SFC_RISR */
#define SFC_RISR_TRANSS_ACTIVE (1 << SFC_RISR_TRANSS_SHIFT)

/********************* Private Structure Definition **************************/

typedef union {
    uint32_t d32;
    struct {
        unsigned txempty : 1; /* tx fifo empty */
        unsigned txfull : 1; /* tx fifo full */
        unsigned rxempty : 1; /* rx fifo empty */
        unsigned rxfull : 1; /* tx fifo empty interrupt mask */
        unsigned reserved7_4 : 4;
        unsigned txlevel : 5; /* tx fifo: 0: full; 1: left 1 entry; ... */
        unsigned reserved15_13 : 3;
        unsigned rxlevel : 5; /* rx fifo: 0: full; 1: left 1 entry; ... */
        unsigned reserved31_21 : 11;
    } b;
} SFCFSR_DATA;
/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/
static void SFC_Reset(struct SFC_REG *pReg)
{
    int32_t timeout = 10000;

    pReg->RCVR = SFC_RCVR_RCVR_MASK;
    while ((pReg->RCVR == SFC_RCVR_RCVR_MASK) && (timeout > 0)) {
        HAL_DelayUs(1);
        timeout--;
    }
    pReg->ICLR = 0xFFFFFFFF;
}

/**
 * @brief  Clear internal data transmission finish interrupt.
 * @param  host: SFC host.
 * @return HAL_Status.
 */
static HAL_Status SFC_ClearIsr(struct HAL_SFC_HOST *host)
{
    host->instance->ICLR = 0xFFFFFFFF;

    return HAL_OK;
}

/**
 * @brief  Check internal data transmission finish interrupt.
 * @param  host: SFC host.
 * @return HAL_Check.
 */
HAL_UNUSED static HAL_Check SFC_IsDMAInterrupt(struct HAL_SFC_HOST *host)
{
    return (HAL_Check)HAL_IS_BIT_SET(host->instance->ISR, SFC_ISR_DMAS_ACTIVE);
}

#ifdef HAL_SNOR_MODULE_ENABLED
/**
 * @brief  Configuration register with flash operation protocol.
 * @param  host: SFC host.
 * @param  op: flash operation protocol.
 * @return HAL_Status.
 */
static HAL_Status SFC_XferStart(struct HAL_SFC_HOST *host, struct HAL_SPI_MEM_OP *op)
{
    struct SFC_REG *pReg = host->instance;
    SFCCMD_DATA sfcCmd;
    SFCCTRL_DATA sfcCtrl;

    sfcCmd.d32 = 0;
    sfcCtrl.d32 = 0;

    /* set CMD */
    sfcCmd.b.cmd = op->cmd.opcode;

    /* set ADDR */
    if (op->addr.nbytes) {
        sfcCmd.b.addrbits = op->addr.nbytes == 4 ? SFC_ADDR_32BITS : SFC_ADDR_24BITS;
        sfcCtrl.b.addrlines = op->addr.buswidth == 4 ? SFC_LINES_X4 : SFC_LINES_X1;
    }
    /* set DUMMY*/
    if (op->dummy.nbytes) {
        sfcCmd.b.dummybits = (op->dummy.nbytes * 8) / (op->dummy.buswidth);
    }

    /* set DATA */
    if (op->data.nbytes) {
        sfcCmd.b.datasize = op->data.nbytes;
        if (op->data.dir == HAL_SPI_MEM_DATA_OUT) {
            sfcCmd.b.rw = SFC_WRITE;
        }
        sfcCtrl.b.datalines = op->data.buswidth == 4 ? SFC_LINES_X4 : SFC_LINES_X1;
    }

    /* spitial setting */
    sfcCtrl.b.sps = SFC_CTRL_SHIFTPHASE_NEGEDGE;

    if (!(pReg->FSR & SFC_FSR_TXES_EMPTY) || !(pReg->FSR & SFC_FSR_RXES_EMPTY) || (pReg->SR & SFC_SR_SR_BUSY)) {
        SFC_Reset(pReg);
    }

    /* HAL_DBG("%s 1 %lx %lx %lx\n", __func__, op->addr.nbytes, op->dummy.nbytes, op->data.nbytes); */
    /* HAL_DBG("%s 2 %lx %lx %lx\n", __func__, sfcCtrl.d32, sfcCmd.d32, op->addr.val); */

    /* config SFC */
    pReg->CTRL = sfcCtrl.d32;
    pReg->CMD = sfcCmd.d32;
    if (op->addr.nbytes) {
        pReg->ADDR = op->addr.val;
    }

    return HAL_OK;
}

/**
 * @brief  IO transfer.
 * @param  host: SFC host.
 * @param  len: data n bytes.
 * @param  data: transfer buffer.
 * @param  dir: transfer direction.
 * @return HAL_Status.
 */
static HAL_Status SFC_XferData(struct HAL_SFC_HOST *host, uint32_t len, void *data, uint32_t dir)
{
    int32_t ret = HAL_OK;
    __IO SFCFSR_DATA fifostat;
    int32_t timeout = 0;
    uint32_t i, words;
    uint32_t *pData = (uint32_t *)data;
    struct SFC_REG *pReg = host->instance;

    /* HAL_DBG("%s %p %lx %x %lx %lx\n", __func__, (uint32_t *)host->instance, SFCCmd, cmd.b.datasize, SFCCtrl, addr); */
    if (dir == SFC_WRITE) {
        words = (len + 3) >> 2;
        while (words) {
            fifostat.d32 = pReg->FSR;
            if (fifostat.b.txlevel > 0) {
                uint32_t count = HAL_MIN(words, fifostat.b.txlevel);

                for (i = 0; i < count; i++) {
                    pReg->DATA = *pData++;
                    words--;
                }
                if (words == 0) {
                    break;
                }
                timeout = 0;
            } else {
                HAL_DelayUs(1);
                if (timeout++ > 10000) {
                    ret = HAL_TIMEOUT;
                    break;
                }
            }
        }
    } else {
        uint32_t bytes = len & 0x3;

        words = len >> 2;
        while (words) {
            fifostat.d32 = pReg->FSR;
            if (fifostat.b.rxlevel > 0) {
                uint32_t count = HAL_MIN(words, fifostat.b.rxlevel);

                for (i = 0; i < count; i++) {
                    *pData++ = pReg->DATA;
                    words--;
                }
                if (0 == words) {
                    break;
                }
                timeout = 0;
            } else {
                HAL_DelayUs(1);
                if (timeout++ > 10000) {
                    ret = HAL_TIMEOUT;
                    break;
                }
            }
        }

        timeout = 0;
        while (bytes) {
            fifostat.d32 = pReg->FSR;
            if (fifostat.b.rxlevel > 0) {
                uint8_t *pData1 = (uint8_t *)pData;
                words = pReg->DATA;
                for (i = 0; i < bytes; i++) {
                    pData1[i] = (uint8_t)((words >> (i * 8)) & 0xFF);
                }
                break;
            } else {
                HAL_DelayUs(1);
                if (timeout++ > 10000) {
                    ret = HAL_TIMEOUT;
                    break;
                }
            }
        }
    }

    return ret;
}

/**
 * @brief  Wait for SFC host transfer finished.
 * @return HAL_Status.
 */
static HAL_Status SFC_XferDone(struct HAL_SFC_HOST *host)
{
    int32_t ret = HAL_OK;
    int32_t timeout = 0;
    struct SFC_REG *pReg = host->instance;

    while (pReg->SR & SFC_SR_SR_BUSY) {
        HAL_DelayUs(1);
        if (timeout++ > 100000) { /*wait 100ms*/
            ret = HAL_TIMEOUT;
            break;
        }
    }
    HAL_DelayUs(1); //CS# High Time (read/write) >100ns

    return ret;
}
#endif

/** @} */
/********************* Public Function Definition ****************************/

/** @defgroup SFC_Exported_Functions_Group3 IO Functions

 This section provides functions allowing to IO controlling:

 - Operate in blocking mode (DMA IT) using HAL_SFC_Request_DMA();
 - Operate in blocking mode (Normal) using HAL_SFC_Request();

 *  @{
 */

#ifdef HAL_SNOR_MODULE_ENABLED
/**
 * @brief  SPI Nor flash data transmission interface supporting open source specifications.
 * @param  spi: host abstract.
 * @param  op: flash operation protocol.
 * @return HAL_Status.
 */
HAL_Status HAL_SFC_SpiXfer(struct SNOR_HOST *spi, struct HAL_SPI_MEM_OP *op)
{
    struct HAL_SFC_HOST *host = (struct HAL_SFC_HOST *)spi->userdata;
    uint32_t ret = HAL_OK;
    uint32_t dir = op->data.dir;
    void *pData = NULL;

    if (op->data.buf.in) {
        pData = (void *)op->data.buf.in;
    } else if (op->data.buf.out) {
        pData = (void *)op->data.buf.out;
    }

    SFC_XferStart(host, op);
    if (pData) {
        ret = SFC_XferData(host, op->data.nbytes, pData, dir);
        if (ret) {
            HAL_DBG("%s xfer data failed ret %ld\n", __func__, ret);

            return ret;
        }
    }

    return SFC_XferDone(host);
}
#endif

/** @} */

/** @defgroup SFC_Exported_Functions_Group4 Init and DeInit Functions

 This section provides functions allowing to init and deinit the module:

 *  @{
 */

/**
 * @brief  Init SFC.
 * @param  host: SFC host.
 * @return HAL_Status.
 */
HAL_Status HAL_SFC_Init(struct HAL_SFC_HOST *host)
{
    HAL_ASSERT(IS_SFC_INSTANCE(host->instance));

    SFC_Reset(host->instance);
    host->status = HAL_UNLOCKED;
    host->instance->CTRL = 0;

    return HAL_OK;
}

/**
 * @brief  DeInit SFC.
 * @param  host: SFC host.
 * @return HAL_Status.
 */
HAL_Status HAL_SFC_DeInit(struct HAL_SFC_HOST *host)
{
    SFC_Reset(host->instance);

    return HAL_OK;
}

/** @} */

/** @defgroup SFC_Exported_Functions_Group5 Other Functions
 *  @{
 */

/**
 * @brief  Mask internal data transmission finish interrupt.
 * @param  host: SFC host.
 * @return HAL_Status.
 */
HAL_Status HAL_SFC_MaskDMAInterrupt(struct HAL_SFC_HOST *host)
{
    SET_BIT(host->instance->IMR, SFC_IMR_DMAM_MASK);

    return HAL_OK;
}

/**
 * @brief  Unmask internal data transmission finish interrupt.
 * @param  host: SFC host.
 * @return HAL_Status.
 */
HAL_Status HAL_SFC_UnmaskDMAInterrupt(struct HAL_SFC_HOST *host)
{
    CLEAR_BIT(host->instance->IMR, SFC_IMR_DMAM_MASK);

    return HAL_OK;
}

/**
 * @brief  SFC interrupt handler.
 * @param  host: SFC host.
 * @return HAL_Status.
 */
HAL_Status HAL_SFC_IRQHelper(struct HAL_SFC_HOST *host)
{
    HAL_ASSERT(SFC_IsDMAInterrupt(host)); /* Only support TRANSM IT */
    SFC_ClearIsr(host);
    host->status = HAL_UNLOCKED;

    return HAL_OK;
}

/** @} */

/** @} */

/** @} */

#endif /* HAL_SFC_MODULE_ENABLED */
