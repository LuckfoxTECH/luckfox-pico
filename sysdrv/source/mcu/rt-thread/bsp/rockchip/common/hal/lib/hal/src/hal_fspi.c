/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_FSPI_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup FSPI
 *  @{
 */

/** @defgroup FSPI_How_To_Use How To Use
 *  @{

 The FSPI driver can be used as follows:

 For Nor flash This host driver need to be used in conjunction with device flash driver like
 hal_snor.c:

 - DMA mode: Register handler by calling HAL_FSPI_IRQHelper().
 - DMA mode: Unmask TRANSM interrupt by calling HAL_FSPI_UnmaskDMAInterrupt().
 - Initialize FSPI controller by calling HAL_FSPI_Init();
 - Send FSPI request:
    - SNOR support api: HAL_FSPI_SpiXfer() which can analyze the flash
        read and write protocol of the SNOR framework;
 - DMA mode: Handling interrupt return in DMA mode.
 - Configure XIP mode if needed by calling:
    - SNOR support api: HAL_FSPI_SpiXipConfig() which can analyze the flash
        read and write protocol of the SNOR framework;

 For psram:
 - Initialize FSPI controller by calling HAL_FSPI_Init();
 - Configure XIP mode if needed by calling HAL_FSPI_XmmcRequest() which support for FSPI config FSPI directly.

 Note.
 - If Nor flash and psram place in one FSPI host, Nor flash for cs0 and psram for cs1.
 - If psram is initial by preloader and work all the timer, set g_fspidev->xmmcDev[cs].type = DEV_PSRAM in hal_bsp.c.

 @} */

/** @defgroup FSPI_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/
// #define FSPI_DEBUG
#ifdef FSPI_DEBUG
#define FSPI_DBG(...) HAL_DBG(__VA_ARGS__)
#else
#define FSPI_DBG(...)
#endif

/* FSPI_CTRL */
#define FSPI_CTRL_SHIFTPHASE_NEGEDGE 1

/* FSPI_RCVR */
#define FSPI_RCVR_RCVR_RESET (1 << FSPI_RCVR_RCVR_SHIFT) /* Recover The FSPI State Machine */

/* FSPI_ISR */
#define FSPI_ISR_DMAS_ACTIVE   (1 << FSPI_ISR_DMAS_SHIFT) /* DMA Finish Interrupt Active */
#define FSPI_ISR_NSPIS_ACTIVE  (1 << FSPI_ISR_NSPIS_SHIFT) /* SPI Error Interrupt Active */
#define FSPI_ISR_AHBS_ACTIVE   (1 << FSPI_ISR_AHBS_SHIFT) /* AHB Error Interrupt Active */
#define FSPI_ISR_TRANSS_ACTIVE (1 << FSPI_ISR_TRANSS_SHIFT) /* Transfer finish Interrupt Active */
#define FSPI_ISR_TXES_ACTIVE   (1 << FSPI_ISR_TXES_SHIFT) /* Transmit FIFO Empty Interrupt Active */
#define FSPI_ISR_TXOS_ACTIVE   (1 << FSPI_ISR_TXOS_SHIFT) /* Transmit FIFO Overflow Interrupt Active */
#define FSPI_ISR_RXUS_ACTIVE   (1 << FSPI_ISR_RXUS_SHIFT) /* Receive FIFO Underflow Interrupt Active */
#define FSPI_ISR_RXFS_ACTIVE   (1 << FSPI_ISR_RXFS_SHIFT) /* Receive FIFO Full Interrupt Active */

/* FSPI_FSR */
#define FSPI_FSR_RXFS_EMPTY (1 << FSPI_FSR_RXFS_SHIFT) /* Receive FIFO Full */
#define FSPI_FSR_RXES_EMPTY (1 << FSPI_FSR_RXES_SHIFT) /* Receive FIFO Empty */
#define FSPI_FSR_TXFS_FULL  (1 << FSPI_FSR_TXFS_SHIFT) /* Transmit FIFO Full */
#define FSPI_FSR_TXES_EMPTY (1 << FSPI_FSR_TXES_SHIFT) /* Transmit FIFO Empty */

/* FSPI_SR */
#define FSPI_SR_SR_BUSY (1 << FSPI_SR_SR_SHIFT) /* When busy, do not set the control register. */

/* FSPI_DMATR */
#define FSPI_DMATR_DMATR_START (1 << FSPI_DMATR_DMATR_SHIFT) /* Write 1 to start the dma transfer. */

/* FSPI_RISR */
#define FSPI_RISR_TRANSS_ACTIVE (1 << FSPI_RISR_TRANSS_SHIFT)

/* FSPI attributes */
#define FSPI_VER_VER_1 1
#define FSPI_VER_VER_3 3
#define FSPI_VER_VER_4 4
#define FSPI_VER_VER_5 5
#define FSPI_VER_VER_6 6

#define FSPI_NOR_FLASH_PAGE_SIZE 0x100

#define FSPI_MAX_IOSIZE_VER3 (1024U * 8)
#define FSPI_MAX_IOSIZE_VER4 (0xFFFFFFFFU)

#define FSPI_DLL_CTRL0_DLL_MAX_VER3 0xFFU
#define FSPI_DLL_CTRL0_DLL_MAX_VER5 0x1FFU

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
} FSPIFSR_DATA;

/** FSPI_XMMC bit union */
typedef union {
    uint32_t d32;
    struct {
        unsigned reserverd1 : 5;
        unsigned devHwEn : 1; /* device Hwrite Enable */
        unsigned prefetch : 1; /* prefetch enable */
        unsigned uincrPrefetchEn : 1; /* undefine INCR Burst Prefetch Enable */
        unsigned uincrLen : 4; /* undefine INCR length */
        unsigned devWrapEn : 1; /* device Wrap Enable */
        unsigned devIncrEn : 1; /* device INCR2/4/8/16 Enable */
        unsigned devUdfincrEn : 1; /* device Undefine INCR Enable */
        unsigned reserved2 : 17;
    } b;
} FSPIXMMCCTRL_DATA;
/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/
static void FSPI_Reset(struct HAL_FSPI_HOST *host)
{
    int32_t timeout = 10000;

    host->instance->RCVR = FSPI_RCVR_RCVR_RESET;
    while ((host->instance->RCVR == FSPI_RCVR_RCVR_RESET) && (timeout > 0)) {
        HAL_CPUDelayUs(1);
        timeout--;
    }
    host->instance->ICLR = 0xFFFFFFFF;
}

/**
 * @brief  Clear internal data transmission finish interrupt.
 * @param  host: FSPI host.
 * @return HAL_Status.
 */
static HAL_Status FSPI_ClearIsr(struct HAL_FSPI_HOST *host)
{
    host->instance->ICLR = 0xFFFFFFFF;

    return HAL_OK;
}

/*
 * VER 3: FSPI0 1/2 map space avail , FSPI1 1/4 map space avail:
 *   FSPI0: CS divide in average, each CS up to 16MB
 *   FSPI1-n: all map space in CS0
 * VER 4-n: It's configurable, 1/2/4 CS, TBD
 */
static void FSPI_XmmcDevRegionInit(struct HAL_FSPI_HOST *host)
{
#if (FSPI_VER == FSPI_VER_VER_3)
    if (host->instance == FSPI0) {
        host->instance->DEVRGN = 24;
        host->instance->DEVSIZE0 = 24;
        host->instance->DEVSIZE1 = 24;
    } else {
#ifdef FSPI1
        host->instance->DEVRGN = 23;
        host->instance->DEVSIZE0 = 23;
        host->instance->DEVSIZE1 = 23;
#endif
    }
#endif /* FSPI_VER == FSPI_VER_VER_3 */
}

HAL_UNUSED static void FSPI_TimeOutInit(struct HAL_FSPI_HOST *host)
{
    WRITE_REG(host->instance->SCLK_INATM_CNT, 0x20);
    SET_BIT(host->instance->TME0, FSPI_TME0_SCLK_INATM_EN_MASK);
    SET_BIT(host->instance->TME1, FSPI_TME0_SCLK_INATM_EN_MASK);
}

static void FSPI_ContModeInit(struct HAL_FSPI_HOST *host)
{
    /* Setup when FSPI->AX = 0xa5, Cancel when FSPI->AX = 0x0 */
    WRITE_REG(host->instance->EXT_AX, 0xa5 << FSPI_EXT_AX_AX_SETUP_PAT_SHIFT |
              0x5a << FSPI_EXT_AX_AX_CANCEL_PAT_SHIFT);
    /* Skip Continuous mode in default */
    switch (host->cs) {
    case 0:
        WRITE_REG(host->instance->AX0, 0 << FSPI_AX0_AX_SHIFT);
        break;
    case 1:
#ifdef RKMCU_RK2108
        WRITE_REG(host->instance->AX0, 0 << FSPI_AX0_AX_SHIFT);
#else
        WRITE_REG(host->instance->AX1, 0 << FSPI_AX1_AX_SHIFT);
#endif
        break;
    default:
        break;
    }
}

static void FSPI_ContModeDeInit(struct HAL_FSPI_HOST *host)
{
    /* FSPI avoid setup pattern match with FSPI->AX */
    WRITE_REG(host->instance->EXT_AX, 0x77 << FSPI_EXT_AX_AX_SETUP_PAT_SHIFT |
              0x88 << FSPI_EXT_AX_AX_CANCEL_PAT_SHIFT);
}

/** @} */
/********************* Public Function Definition ****************************/

/** @defgroup FSPI_Exported_Functions_Group3 IO Functions

 This section provides functions allowing to IO controlling:

 - Operate in blocking mode (Normal) using HAL_FSPI_Request();

 *  @{
 */

/**
 * @brief  Configuration register with flash operation protocol.
 * @param  host: FSPI host.
 * @param  op: flash operation protocol.
 * @return HAL_Status.
 * @attention Set host->cs to select chip.
 */
HAL_Status HAL_FSPI_XferStart(struct HAL_FSPI_HOST *host, struct HAL_SPI_MEM_OP *op)
{
    struct FSPI_REG *pReg = host->instance;
    FSPICMD_DATA FSPICmd;
    FSPICTRL_DATA FSPICtrl;

    FSPICmd.d32 = 0;
    FSPICtrl.d32 = 0;

    /* set CMD */
    FSPICmd.b.cmd = op->cmd.opcode;
    if (op->cmd.buswidth == 4) {
        FSPICtrl.b.cmdlines = FSPI_LINES_X4;
        FSPICtrl.b.datalines = FSPI_LINES_X4; /* cmdlines work with datalines */
    }

    /* set ADDR */
    if (op->addr.nbytes) {
        if (op->addr.nbytes == 4) {
            FSPICmd.b.addrbits = FSPI_ADDR_32BITS;
        } else if (op->addr.nbytes == 3) {
            FSPICmd.b.addrbits = FSPI_ADDR_24BITS;
        } else {
            FSPICmd.b.addrbits = FSPI_ADDR_XBITS;
            pReg->ABIT0 = op->addr.nbytes * 8 - 1;
        }

        FSPICtrl.b.addrlines = op->addr.buswidth == 4 ? FSPI_LINES_X4 : FSPI_LINES_X1;
    }

    /* set DUMMY*/
    if (op->dummy.nbytes) {
        switch (op->dummy.buswidth) {
        case 4:
            FSPICmd.b.dummybits = op->dummy.nbytes * 2;
            break;
        case 2:
            FSPICmd.b.dummybits = op->dummy.nbytes * 4;
            break;
        default:
            FSPICmd.b.dummybits = op->dummy.nbytes * 8;
            break;
        }
    }

    /* set DATA */
#if (FSPI_VER > FSPI_VER_VER_3)
    WRITE_REG(pReg->LEN_EXT, op->data.nbytes);
#else
    FSPICmd.b.datasize = op->data.nbytes;
#endif
    if (op->data.nbytes) {
        if (op->data.dir == HAL_SPI_MEM_DATA_OUT) {
            FSPICmd.b.rw = FSPI_WRITE;
        }
        if (op->data.buswidth == 4) {
            FSPICtrl.b.datalines = FSPI_LINES_X4;
        } else if (op->data.buswidth == 2) {
            FSPICtrl.b.datalines = FSPI_LINES_X2;
        } else {
            FSPICtrl.b.datalines = FSPI_LINES_X1;
        }
    }

    /* spitial setting */
    FSPICtrl.b.sps = host->mode & HAL_SPI_CPHA;
#ifdef RKMCU_RK2108
    if (host->cs >= 1) {
        FSPICmd.b.cs = 2;
    }
#else
    FSPICmd.b.cs = host->cs;
#endif
    if (op->data.nbytes == 0 && op->addr.nbytes) {
        FSPICmd.b.rw = FSPI_WRITE;
    }

    if (!(pReg->FSR & FSPI_FSR_TXES_EMPTY) || !(pReg->FSR & FSPI_FSR_RXES_EMPTY) || (pReg->SR & FSPI_SR_SR_BUSY)) {
        FSPI_Reset(host);
    }

    // FSPI_DBG("%s 1 %x %x %x\n", __func__, op->addr.nbytes, op->dummy.nbytes, op->data.nbytes);
    // FSPI_DBG("%s 2 %lx %lx %lx %x\n", __func__, FSPICtrl.d32, FSPICmd.d32, op->addr.val, host->cs);

    /* config FSPI */
    switch (host->cs) {
    case 0:
        pReg->CTRL0 = FSPICtrl.d32;
        break;
    case 1:
#ifdef RKMCU_RK2108
        pReg->CTRL0 = FSPICtrl.d32;
#else
        pReg->CTRL1 = FSPICtrl.d32;
#endif
        break;
    default:
        break;
    }
    pReg->CMD = FSPICmd.d32;
    if (op->addr.nbytes) {
        pReg->ADDR = op->addr.val;
    }

    return HAL_OK;
}

/**
 * @brief  IO transfer.
 * @param  host: FSPI host.
 * @param  len: data n bytes.
 * @param  data: transfer buffer.
 * @param  dir: transfer direction.
 * @return HAL_Status.
 */
HAL_Status HAL_FSPI_XferData(struct HAL_FSPI_HOST *host, uint32_t len, void *data, uint32_t dir)
{
    HAL_Status ret = HAL_OK;
    __IO FSPIFSR_DATA fifostat;
    int32_t timeout = 0;
    uint32_t i, words;
    uint32_t *pData = (uint32_t *)data;
    struct FSPI_REG *pReg = host->instance;
    uint32_t temp = 0;

    HAL_ASSERT(data && len);

    if (len && len < 4 && dir == FSPI_WRITE) {
        if (len == 1) {
            temp = *((uint8_t *)data);
        } else if (len == 2) {
            temp = *((uint16_t *)data);
        } else {
            temp = ((uint8_t *)data)[0] | ((uint8_t *)data)[1] << 8 | ((uint8_t *)data)[2] << 16;
        }
        pData = &temp;
    } else if (len >= 4 && !HAL_IS_ALIGNED((uint32_t)data, 4)) {
        FSPI_DBG("%s data unaligned access\n", __func__);
#if !defined(__CORTEX_M) && !defined(HAL_FSPI_ALIGNED_CHECK_BYPASS)

        return HAL_INVAL;
#endif
    }

    /* FSPI_DBG("%s %p len %lx word0 %lx dir %lx\n", __func__, pData, len, pData[0], dir); */
    if (dir == FSPI_WRITE) {
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
                HAL_CPUDelayUs(1);
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
                if (count == 15) { /* Reduce CPU cost */
                    *pData++ = pReg->DATA;
                    *pData++ = pReg->DATA;
                    *pData++ = pReg->DATA;
                    *pData++ = pReg->DATA;
                    *pData++ = pReg->DATA;
                    *pData++ = pReg->DATA;
                    *pData++ = pReg->DATA;
                    *pData++ = pReg->DATA;

                    *pData++ = pReg->DATA;
                    *pData++ = pReg->DATA;
                    *pData++ = pReg->DATA;
                    *pData++ = pReg->DATA;
                    *pData++ = pReg->DATA;
                    *pData++ = pReg->DATA;
                    *pData++ = pReg->DATA;
                    words -= 15;
                } else {
                    for (i = 0; i < count; i++) {
                        *pData++ = pReg->DATA;
                    }
                    words -= count;
                }

                if (0 == words) {
                    break;
                }
                timeout = 0;
            } else {
                HAL_CPUDelayUs(1);
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
                HAL_CPUDelayUs(1);
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
 * @brief  IO transfer by internal DMA.
 * @param  host: FSPI host.
 * @param  len: data n bytes.
 * @param  data: transfer buffer.
 * @param  dir: transfer direction.
 * @return HAL_Status.
 */
HAL_Status HAL_FSPI_XferData_DMA(struct HAL_FSPI_HOST *host, uint32_t len, void *data, uint32_t dir)
{
    HAL_Status ret = HAL_OK;
    int32_t timeout = 0;
    struct FSPI_REG *pReg = host->instance;

    HAL_ASSERT(data && len);

    pReg->ICLR = 0xFFFFFFFF;
    pReg->DMAADDR = (uint32_t)data;
    pReg->DMATR = FSPI_DMATR_DMATR_START;

    timeout = len * 10;
    while (timeout--) {
        if (READ_BIT(pReg->RISR, FSPI_ISR_DMAS_MASK)) {
            break;
        }
        HAL_DelayUs(1);
    }

    pReg->ICLR = 0xFFFFFFFF;
    if (timeout <= 0) {
        ret = HAL_TIMEOUT;
    }

    return ret;
}

/**
 * @brief  Wait for FSPI host transfer finished.
 * @return HAL_Status.
 */
HAL_Status HAL_FSPI_XferDone(struct HAL_FSPI_HOST *host)
{
    HAL_Status ret = HAL_OK;
    int32_t timeout = 0;
    struct FSPI_REG *pReg = host->instance;

    while (pReg->SR & FSPI_SR_SR_BUSY) {
        HAL_CPUDelayUs(1);
        if (timeout++ > 100000) { /*wait 100ms*/
            ret = HAL_TIMEOUT;
            break;
        }
    }
    HAL_CPUDelayUs(1); //CS# High Time (read/write) >100ns

    return ret;
}

/**
 * @brief  SPI Nor flash data transmission interface to support open source specifications SNOR.
 * @param  host: FSPI host.
 * @param  op: flash operation protocol.
 * @return HAL_Status.
 * @attention Set host->cs to select chip.
 */
HAL_Status HAL_FSPI_SpiXfer(struct HAL_FSPI_HOST *host, struct HAL_SPI_MEM_OP *op)
{
    HAL_Status ret = HAL_OK;
    uint32_t dir = op->data.dir;
    void *pData = NULL;

    HAL_ASSERT(IS_FSPI_INSTANCE(host->instance));

    if (op->data.buf.in) {
        pData = (void *)op->data.buf.in;
    } else if (op->data.buf.out) {
        pData = (void *)op->data.buf.out;
    }

    HAL_FSPI_XferStart(host, op);
    if (pData) {
#if defined(HAL_FSPI_DMA_ENABLED)
        if ((op->data.nbytes >= FSPI_NOR_FLASH_PAGE_SIZE) &&
            (dir == HAL_SPI_MEM_DATA_IN)) {
            ret = HAL_FSPI_XferData_DMA(host, op->data.nbytes, pData, dir);
        } else
#endif
        ret = HAL_FSPI_XferData(host, op->data.nbytes, pData, dir);
        if (ret) {
            FSPI_DBG("%s xfer data failed ret %d\n", __func__, ret);

            return ret;
        }
    }

    return HAL_FSPI_XferDone(host);
}

/** @} */

/** @defgroup FSPI_Exported_Functions_Group4 Init and DeInit Functions

 This section provides functions allowing to init and deinit the module:

 *  @{
 */

/**
 * @brief  Init FSPI.
 * @param  host: FSPI host.
 * @return HAL_Status.
 */
HAL_Status HAL_FSPI_Init(struct HAL_FSPI_HOST *host)
{
    int32_t timeout = 0;
    struct FSPI_REG *pReg;
    HAL_Status ret = HAL_OK;

    HAL_ASSERT(IS_FSPI_INSTANCE(host->instance));

    pReg = host->instance;
    pReg->MODE = 0;
    while (pReg->SR & FSPI_SR_SR_BUSY) {
        HAL_CPUDelayUs(1);
        if (timeout++ > 1000) {
            return HAL_TIMEOUT;
        }
    }
    FSPI_ContModeInit(host);
    pReg->CTRL0 = 0;
    FSPI_XmmcDevRegionInit(host);

#if (FSPI_VER > FSPI_VER_VER_3)
    WRITE_REG(pReg->LEN_CTRL, FSPI_LEN_CTRL_TRB_SEL_MASK);
#endif

    return ret;
}

/**
 * @brief  DeInit FSPI.
 * @param  host: FSPI host.
 * @return HAL_Status.
 */
HAL_Status HAL_FSPI_DeInit(struct HAL_FSPI_HOST *host)
{
    HAL_ASSERT(IS_FSPI_INSTANCE(host->instance));

    host->instance->MODE = 0;
    FSPI_ContModeDeInit(host);
    FSPI_Reset(host);

    return HAL_OK;
}

/** @} */

/** @defgroup FSPI_Exported_Functions_Group5 Other Functions
 *  @{
 */

/**
 * @brief  Mask internal data transmission finish interrupt.
 * @param  host: FSPI host.
 * @return HAL_Status.
 */
HAL_Status HAL_FSPI_MaskDMAInterrupt(struct HAL_FSPI_HOST *host)
{
    SET_BIT(host->instance->IMR, FSPI_IMR_DMAM_MASK);

    return HAL_OK;
}

/**
 * @brief  Unmask internal data transmission finish interrupt.
 * @param  host: FSPI host.
 * @return HAL_Status.
 */
HAL_Status HAL_FSPI_UnmaskDMAInterrupt(struct HAL_FSPI_HOST *host)
{
    CLEAR_BIT(host->instance->IMR, FSPI_IMR_DMAM_MASK);

    return HAL_OK;
}

/**
 * @brief  FSPI interrupt handler.
 * @param  host: FSPI host.
 * @return HAL_Status.
 */
HAL_Status HAL_FSPI_IRQHelper(struct HAL_FSPI_HOST *host)
{
    HAL_ASSERT(HAL_IS_BIT_SET(host->instance->ISR, FSPI_ISR_DMAS_ACTIVE)); /* Only support DMA IT */
    FSPI_ClearIsr(host);

    return HAL_OK;
}

/**
 * @brief  Set XMMC dev.
 * @param  host: FSPI host.
 * @param  op: flash operation protocol.
 * @return HAL_Status.
 * @attention Set host->cs to select chip.
 */
HAL_Status HAL_FSPI_XmmcSetting(struct HAL_FSPI_HOST *host, struct HAL_SPI_MEM_OP *op)
{
    FSPICMD_DATA FSPICmd;
    FSPICTRL_DATA FSPICtrl;
    FSPIXMMCCTRL_DATA xmmcCtrl;

    HAL_ASSERT(IS_FSPI_INSTANCE(host->instance));

    FSPICmd.d32 = 0;
    FSPICtrl.d32 = 0;

    /* set CMD */
    FSPICmd.b.cmd = op->cmd.opcode;
    FSPICtrl.b.cmdlines = op->cmd.buswidth == 4 ? FSPI_LINES_X4 : FSPI_LINES_X1;
    if (op->addr.buswidth == 4 && (host->xmmcDev[host->cs].type == DEV_NOR || host->xmmcDev[host->cs].type == DEV_SPINAND)) {
        FSPICmd.b.readmode = 1;
    }

    /* set ADDR */
    FSPICmd.b.addrbits = op->addr.nbytes == 4 ? FSPI_ADDR_32BITS : FSPI_ADDR_24BITS;
    FSPICtrl.b.addrlines = op->addr.buswidth == 4 ? FSPI_LINES_X4 : FSPI_LINES_X1;

    /* set DUMMY*/
    FSPICtrl.b.scic = op->dummy.a2dIdle;
    switch (op->dummy.buswidth) {
    case 4:
        FSPICmd.b.dummybits = op->dummy.nbytes * 2;
        break;
    case 2:
        FSPICmd.b.dummybits = op->dummy.nbytes * 4;
        break;
    default:
        FSPICmd.b.dummybits = op->dummy.nbytes * 8;
        break;
    }
    if (FSPICmd.b.readmode) {
        FSPICmd.b.dummybits -= 2; /* M7-0 ocuppy Dummy 2 cycles  */
    }
    /* set DATA */
    if (op->data.dir == HAL_SPI_MEM_DATA_OUT) {
        FSPICmd.b.rw = FSPI_WRITE;
    }
    if (op->data.buswidth == 4) {
        FSPICtrl.b.datalines = FSPI_LINES_X4;
    } else if (op->data.buswidth == 2) {
        FSPICtrl.b.datalines = FSPI_LINES_X2;
    } else {
        FSPICtrl.b.datalines = FSPI_LINES_X1;
    }

    /* set XMMC */
    if (host->xmmcDev[0].type == DEV_PSRAM || host->xmmcDev[1].type == DEV_PSRAM) {
        xmmcCtrl.b.devHwEn = 1;
        xmmcCtrl.b.prefetch = 0;
    } else {
        xmmcCtrl.b.devHwEn = 0;
#if !defined(HAL_DCACHE_MODULE_ENABLED) && (FSPI_VER == 3)
        xmmcCtrl.b.prefetch = 0;
#else
        xmmcCtrl.b.prefetch = 1;
        FSPI_TimeOutInit(host);
#endif
    }

    /* spetial setting */
    FSPICtrl.b.sps = host->mode & HAL_SPI_CPHA;

    /* FSPI_DBG("%s 1 %x %x %x\n", __func__, op->addr.nbytes, op->dummy.nbytes, op->data.nbytes); */
    /* FSPI_DBG("%s 2 %lx %lx %lx\n", __func__, FSPICtrl.d32, FSPICmd.d32, op->addr.val); */
    host->xmmcDev[host->cs].ctrl = FSPICtrl.d32;
    host->xmmcCtrl = xmmcCtrl.d32;
    if (op->data.dir == HAL_SPI_MEM_DATA_IN) {
        host->xmmcDev[host->cs].readCmd = FSPICmd.d32;
    } else {
        host->xmmcDev[host->cs].writeCmd = FSPICmd.d32;
    }

    return HAL_OK;
}

/**
 * @brief  Configure FSPI XIP mode.
 * @param  host: FSPI host.
 * @param  on: 1 enable, 0 disable.
 * @return HAL_Status.
 * If psram is initialized by preloader and work all the timer, set g_fspidev->xmmcDev[cs].type = DEV_PSRAM in hal_bsp.c.
 */
HAL_Status HAL_FSPI_XmmcRequest(struct HAL_FSPI_HOST *host, uint8_t on)
{
    struct FSPI_REG *pReg = host->instance;
    int timeout = 0;

    if (on) {
        if (pReg->MODE & 0x1) {
            return HAL_INVAL;
        }

        /* FSPI xmmcCtrl config */
        MODIFY_REG(pReg->XMMC_CTRL, FSPI_XMMC_CTRL_DEV_HWEN_MASK |
                   FSPI_XMMC_CTRL_PFT_EN_MASK,
                   host->xmmcCtrl);
        FSPI_ContModeInit(host);

        /* FSPI device config */
        /* FSPI_DBG("%s %p enable 3 %lx %lx %lx %lx\n", __func__,
                    host->instance,
                    host->xmmcDev[0].ctrl, host->cs,
                    host->xmmcDev[0].readCmd, host->xmmcDev[0].writeCmd); */
        WRITE_REG(pReg->CTRL0, host->xmmcDev[0].ctrl);
        WRITE_REG(pReg->XMMC_RCMD0, host->xmmcDev[0].readCmd);
        WRITE_REG(pReg->XMMC_WCMD0, host->xmmcDev[0].writeCmd);

        WRITE_REG(pReg->CTRL1, host->xmmcDev[1].ctrl);
        WRITE_REG(pReg->XMMC_RCMD1, host->xmmcDev[1].readCmd);
        WRITE_REG(pReg->XMMC_WCMD1, host->xmmcDev[1].writeCmd);

        WRITE_REG(pReg->MODE, 1);
    } else {
        /* FSPI_DBG("%s diable\n", __func__); */
        WRITE_REG(pReg->MODE, 0);
        while (READ_REG(pReg->SR) & FSPI_SR_SR_BUSY) {
            HAL_CPUDelayUs(1);
            if (timeout++ > 1000) {
                return HAL_TIMEOUT;
            }
        }
    }

    return HAL_OK;
}

/**
 * @brief  Set FSPI delay line.
 * @param  host: FSPI host.
 * @param  cells: delay line cells.
 * @return HAL_Status.
 */
HAL_Status HAL_FSPI_SetDelayLines(struct HAL_FSPI_HOST *host, uint16_t cells)
{
    HAL_ASSERT(IS_FSPI_INSTANCE(host->instance));

    if (cells > HAL_FSPI_GetMaxDllCells(host)) {
        return HAL_INVAL;
    }

    if (host->cs == 0) {
        WRITE_REG(host->instance->DLL_CTRL0, 1 << FSPI_DLL_CTRL0_SCLK_SMP_SEL_SHIFT | cells);
    } else {
#ifdef RKMCU_RK2108
        WRITE_REG(host->instance->DLL_CTRL0, 1 << FSPI_DLL_CTRL0_SCLK_SMP_SEL_SHIFT | cells);
#else
        WRITE_REG(host->instance->DLL_CTRL1, 1 << FSPI_DLL_CTRL0_SCLK_SMP_SEL_SHIFT | cells);
#endif
    }

    return HAL_OK;
}

/**
 * @brief  Disable FSPI delay line function.
 * @param  host: FSPI host.
 * @return HAL_Status.
 */
HAL_Status HAL_FSPI_DLLDisable(struct HAL_FSPI_HOST *host)
{
    HAL_ASSERT(IS_FSPI_INSTANCE(host->instance));
    if (host->cs == 0) {
        CLEAR_REG(host->instance->DLL_CTRL0);
    } else {
#ifdef RKMCU_RK2108
        CLEAR_REG(host->instance->DLL_CTRL0);
#else
        CLEAR_REG(host->instance->DLL_CTRL1);
#endif
    }

    return HAL_OK;
}

/**
 * @brief  Get FSPI XMMC status.
 * @param  host: FSPI host.
 * @return uiint32_t: XMMC status.
 */
uint32_t HAL_FSPI_GetXMMCStatus(struct HAL_FSPI_HOST *host)
{
    HAL_ASSERT(IS_FSPI_INSTANCE(host->instance));

    return (uint32_t)host->instance->XMMCSR;
}

uint32_t HAL_FSPI_GetMaxIoSize(struct HAL_FSPI_HOST *host)
{
    HAL_ASSERT(IS_FSPI_INSTANCE(host->instance));

#if (FSPI_VER > FSPI_VER_VER_3)

    return FSPI_MAX_IOSIZE_VER4;
#else

    return FSPI_MAX_IOSIZE_VER3;
#endif
}

uint32_t HAL_FSPI_GetMaxDllCells(struct HAL_FSPI_HOST *host)
{
    HAL_ASSERT(IS_FSPI_INSTANCE(host->instance));

#if (FSPI_VER >= FSPI_VER_VER_5)

    return FSPI_DLL_CTRL0_DLL_MAX_VER5;
#elif (FSPI_VER >= FSPI_VER_VER_3)

    return FSPI_DLL_CTRL0_DLL_MAX_VER3;
#else

    return 0;
#endif
}

/** @} */

/** @} */

/** @} */

#endif /* HAL_FSPI_MODULE_ENABLED */
