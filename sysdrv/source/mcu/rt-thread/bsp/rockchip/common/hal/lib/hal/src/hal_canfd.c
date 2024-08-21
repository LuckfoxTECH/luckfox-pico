/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#if defined(HAL_CANFD_MODULE_ENABLED)

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup CANFD
 *  @{
 */

/** @defgroup CANFD_How_To_Use How To Use
 *  @{

 The CANFD driver can be used as follows:

 - Init: set work mode: HAL_CANFD_Init()

 - Config: config bit rate: HAL_CANFD_Config()

 - Start: start can bus: HAL_CANFD_Start()

 - Stop: stop can bus: HAL_CANFD_Stop()

 - Tx: HAL_CANFD_Transmit()

 - Rx: HAL_CANFD_Receive()

 @} */

/** @defgroup CANFD_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/
/********************* Private Structure Definition **************************/
/********************* Private Variable Definition ***************************/

static const uint8_t dlc2len[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64 };

static const uint8_t len2dlc[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8,        /* 0 - 8 */
                                   9, 9, 9, 9, /* 9 - 12 */
                                   10, 10, 10, 10, /* 13 - 16 */
                                   11, 11, 11, 11, /* 17 - 20 */
                                   12, 12, 12, 12, /* 21 - 24 */
                                   13, 13, 13, 13, 13, 13, 13, 13, /* 25 - 32 */
                                   14, 14, 14, 14, 14, 14, 14, 14, /* 33 - 40 */
                                   14, 14, 14, 14, 14, 14, 14, 14, /* 41 - 48 */
                                   15, 15, 15, 15, 15, 15, 15, 15, /* 49 - 56 */
                                   15, 15, 15, 15, 15, 15, 15, 15 }; /* 57 - 64 */

/********************* Private Function Definition ***************************/

/**
 * @brief Get data length from can_dlc with sanitized can_dlc.
 * @param  dlc: can_dlc
 * @return length.
 */
static uint8_t CANFD_Dlc2Len(uint8_t dlc)
{
    return dlc2len[dlc & 0x0F];
}

/**
 * @brief Map the sanitized data length to an appropriate data length code.
 * @param  len: can length
 * @return dlc.
 */
static uint8_t CANFD_Len2Dlc(uint8_t len)
{
    if (len > 64) {
        return 0xF;
    }

    return len2dlc[len];
}

/**
 * @brief Set can to reset mode.
 * @param  pReg: can base
 * @return HAL_OK.
 */
static HAL_Status CANFD_SetResetMode(struct CAN_REG *pReg)
{
    pReg->MODE = 0;
    WRITE_REG(pReg->INT_MASK, 0xffff);

    return HAL_OK;
}

/**
 * @brief Set can to normal mode.
 * @param  pReg: can base
 * @return HAL_OK.
 */
static HAL_Status CANFD_SetNormalMode(struct CAN_REG *pReg)
{
    SET_BIT(pReg->MODE, CAN_MODE_WORK_MODE_MASK);

    return HAL_OK;
}

/** @} */
/********************* Public Function Definition ****************************/

/** @defgroup CANFD_Exported_Functions_Group5 Other Functions
 *  @{
 */

/**
 * @brief CANFD get normal bps.
 * @param  pReg: can base
 * @param  bps: can BaudRate
 * @return HAL_OK.
 * How to calculate the can bit rate:
 *     BaudRate= clk_can/(2*(brq+1)/(1+(tseg1+1)+(tseg2+1)))
 *     TDC = 0
 *     For example(clk_can=200M):
 *     CLK_CAN    BPS    BRQ    TSEG1    TSEG2    TDC
 *     200M       1M     4      13       4        0
 *     200M       800K   4      18       4        0
 *     200M       500K   4      33       4        0
 *     200M       250K   4      68       9        0
 *     200M       200K   9      43       4        0
 *     200M       125K   9      68       9        0
 *     200M       100K   24     33       4        0
 *     200M       50K    24     68       9        0
 */
HAL_Status HAL_CANFD_SetNBps(struct CAN_REG *pReg, eCANFD_Bps bps)
{
    uint32_t sjw, brq, tseg1, tseg2;

    HAL_ASSERT(IS_CAN_INSTANCE(pReg));

    switch (bps) {
    case CANFD_BPS_1MBAUD:
        brq = 4;
        tseg1 = 13;
        tseg2 = 4;
        break;
    case CANFD_BPS_800KBAUD:
        brq = 4;
        tseg1 = 18;
        tseg2 = 4;
        break;
    case CANFD_BPS_500KBAUD:
        brq = 4;
        tseg1 = 33;
        tseg2 = 4;
        break;
    case CANFD_BPS_250KBAUD:
        brq = 4;
        tseg1 = 68;
        tseg2 = 9;
        break;
    case CANFD_BPS_200KBAUD:
        brq = 9;
        tseg1 = 43;
        tseg2 = 4;
        break;
    case CANFD_BPS_125KBAUD:
        brq = 9;
        tseg1 = 68;
        tseg2 = 9;
        break;
    case CANFD_BPS_100KBAUD:
        brq = 24;
        tseg1 = 33;
        tseg2 = 4;
        break;
    case CANFD_BPS_50KBAUD:
        brq = 24;
        tseg1 = 68;
        tseg2 = 9;
        break;
    default:
        brq = 4;
        tseg1 = 13;
        tseg2 = 4;
        break;
    }
    sjw = 0;

    SET_BIT(pReg->FD_NOMINAL_BITTIMING,
            (sjw << CAN_FD_NOMINAL_BITTIMING_SJW_SHIFT) |
            (brq << CAN_FD_NOMINAL_BITTIMING_BRQ_SHIFT) |
            (tseg1 << CAN_FD_NOMINAL_BITTIMING_TSEG1_SHIFT) |
            (tseg2 << CAN_FD_NOMINAL_BITTIMING_TSEG2_SHIFT));

    return HAL_OK;
}

/**
 * @brief CANFD get data bps.
 * @param  pReg: can base
 * @param  bps: can BaudRate
 * @return HAL_OK.
 * How to calculate the can bit rate:
 *     BaudRate= clk_can/(2*(brq+1)/(1+(tseg1+1)+(tseg2+1)))
 *     For example(clk_can=200M):
 *     CLK_CAN    BPS    BRQ    TSEG1    TSEG2    TDC
 *     200M       5M     0      13       4        26
 *     200M       4M     0      16       6        33
 *     200M       2M     1      16       6        0
 *     200M       1M     4      13       4        0
 *     200M       800K   4      18       4        0
 *     200M       500K   9      13       4        0
 *     200M       250K   9      24       13       0
 *     200M       200K   24     13       4        0
 *     200M       125K   19     33       4        0
 *     200M       100K   24     33       4        0
 *     200M       50K    49     13       4        0
 */
HAL_Status HAL_CANFD_SetDBps(struct CAN_REG *pReg, eCANFD_Bps bps)
{
    uint32_t sjw, brq, tseg1, tseg2, tdc;

    HAL_ASSERT(IS_CAN_INSTANCE(pReg));

    switch (bps) {
    case CANFD_BPS_5MBAUD:
        brq = 0;
        tseg1 = 13;
        tseg2 = 4;
        tdc = 26;
        break;
    case CANFD_BPS_4MBAUD:
        brq = 0;
        tseg1 = 16;
        tseg2 = 6;
        tdc = 33;
        break;
    case CANFD_BPS_2MBAUD:
        brq = 1;
        tseg1 = 16;
        tseg2 = 6;
        tdc = 0;
        break;
    case CANFD_BPS_1MBAUD:
        brq = 4;
        tseg1 = 13;
        tseg2 = 4;
        tdc = 0;
        break;
    case CANFD_BPS_800KBAUD:
        brq = 4;
        tseg1 = 18;
        tseg2 = 4;
        tdc = 0;
        break;
    case CANFD_BPS_500KBAUD:
        brq = 9;
        tseg1 = 13;
        tseg2 = 4;
        tdc = 0;
        break;
    case CANFD_BPS_250KBAUD:
        brq = 9;
        tseg1 = 24;
        tseg2 = 13;
        tdc = 0;
        break;
    case CANFD_BPS_200KBAUD:
        brq = 24;
        tseg1 = 13;
        tseg2 = 4;
        tdc = 0;
        break;
    case CANFD_BPS_125KBAUD:
        brq = 19;
        tseg1 = 33;
        tseg2 = 4;
        tdc = 0;
        break;
    case CANFD_BPS_100KBAUD:
        brq = 24;
        tseg1 = 33;
        tseg2 = 4;
        tdc = 0;
        break;
    case CANFD_BPS_50KBAUD:
        brq = 49;
        tseg1 = 13;
        tseg2 = 4;
        tdc = 0;
        break;
    default:
        brq = 4;
        tseg1 = 13;
        tseg2 = 4;
        tdc = 0;
        break;
    }
    sjw = 0;
    WRITE_REG(pReg->FD_DATA_BITTIMING,
              (sjw << CAN_FD_DATA_BITTIMING_SJW_SHIFT) |
              (brq << CAN_FD_DATA_BITTIMING_BRQ_SHIFT) |
              (tseg1 << CAN_FD_DATA_BITTIMING_TSEG1_SHIFT) |
              (tseg2 << CAN_FD_DATA_BITTIMING_TSEG2_SHIFT));
    if (tdc) {
        WRITE_REG(pReg->TRANSMIT_DELAY_COMPENSATION,
                  (tdc << CAN_TRANSMIT_DELAY_COMPENSATION_TDC_OFFSET_SHIFT) | CAN_TRANSMIT_DELAY_COMPENSATION_TDC_ENABLE_MASK);
    }

    return HAL_OK;
}

/**
 * @brief CANFD config bit rate.
 * @param  pReg: can base
 * @param  nbps: can normal bit rate
 * @param  dbps: can date bit rate
 * @return HAL_OK.
 */
HAL_Status HAL_CANFD_Config(struct CAN_REG *pReg, eCANFD_Bps nbps, eCANFD_Bps dbps)
{
    HAL_ASSERT(IS_CAN_INSTANCE(pReg));

    HAL_CANFD_SetNBps(pReg, nbps);
    HAL_CANFD_SetDBps(pReg, dbps);

    return HAL_OK;
}

/**
 * @brief CANFD init.
 * @param  pReg: can base
 * @param  initStrust: can init parameter
 * @return HAL_OK.
 */
HAL_Status HAL_CANFD_Init(struct CAN_REG *pReg, struct CANFD_CONFIG *initStrust)
{
    uint32_t filterID, filterIDMask;

    HAL_ASSERT(IS_CAN_INSTANCE(pReg));
    HAL_ASSERT(initStrust != NULL);

    CANFD_SetResetMode(pReg);
    pReg->INT_MASK = 0;

    filterID = 0;
    filterIDMask = 0x1fffffff;

    pReg->IDCODE = filterID;
    pReg->IDMASK = filterIDMask;
    pReg->IDCODE0 = filterID;
    pReg->IDMASK0 = filterIDMask;
    pReg->IDCODE1 = filterID;
    pReg->IDMASK1 = filterIDMask;
    pReg->IDCODE2 = filterID;
    pReg->IDMASK2 = filterIDMask;
    pReg->IDCODE3 = filterID;
    pReg->IDMASK3 = filterIDMask;
    pReg->IDCODE4 = filterID;
    pReg->IDMASK4 = filterIDMask;

    pReg->RX_FIFO_CTRL |= CAN_RX_FIFO_CTRL_RX_FIFO_ENABLE_MASK;

    if ((initStrust->canfdMode & CANFD_MODE_LOOPBACK) == CANFD_MODE_LOOPBACK) {
        SET_BIT(pReg->MODE, CAN_MODE_LBACK_MODE_MASK | CAN_MODE_SELF_TEST_MASK);
    }
    if ((initStrust->canfdMode & CANFD_MODE_FD) == CANFD_MODE_FD) {
        SET_BIT(pReg->MODE, CAN_MODE_CAN_FD_MODE_ENABLE_MASK);
    }
    if ((initStrust->canfdMode & CANFD_MODE_FD_NON_ISO) == CANFD_MODE_FD_NON_ISO) {
        SET_BIT(pReg->MODE, CAN_MODE_CAN_FD_MODE_ENABLE_MASK);
    }
    if ((initStrust->canfdMode & CANFD_MODE_LISTENONLY) == CANFD_MODE_LISTENONLY) {
        SET_BIT(pReg->MODE, CAN_MODE_SILENT_MODE_MASK);
    }
    if ((initStrust->canfdMode & CANFD_MODE_3_SAMPLES) == CANFD_MODE_3_SAMPLES) {
        SET_BIT(pReg->FD_NOMINAL_BITTIMING, CAN_FD_NOMINAL_BITTIMING_SAMPLE_MODE_MASK);
        SET_BIT(pReg->FD_DATA_BITTIMING, CAN_FD_DATA_BITTIMING_SAMPLE_MODE_MASK);
    }
    SET_BIT(pReg->MODE, CAN_MODE_AUTO_RETX_MODE_MASK | CAN_MODE_CAN_FD_MODE_ENABLE_MASK);

    HAL_CANFD_Config(pReg, initStrust->bps, initStrust->bps);

    return HAL_OK;
}

/**
 * @brief CANFD start.
 * @param  pReg: can base
 * @return HAL_OK.
 */
HAL_Status HAL_CANFD_Start(struct CAN_REG *pReg)
{
    HAL_ASSERT(IS_CAN_INSTANCE(pReg));

    CANFD_SetNormalMode(pReg);

    return HAL_OK;
}

/**
 * @brief CANFD stop.
 * @param  pReg: can base
 * @return HAL_OK.
 */
HAL_Status HAL_CANFD_Stop(struct CAN_REG *pReg)
{
    HAL_ASSERT(IS_CAN_INSTANCE(pReg));

    CANFD_SetResetMode(pReg);
    WRITE_REG(pReg->INT_MASK, 0xffff);

    return HAL_OK;
}

/**
 * @brief CANFD tx.
 * @param  pReg: can base
 * @param  TxMsg: Tx message
 * @return HAL_OK.
 */
HAL_Status HAL_CANFD_Transmit(struct CAN_REG *pReg, struct CANFD_MSG *TxMsg)
{
    uint8_t cmd = CAN_CMD_TX0_REQ_MASK;
    int i;

    HAL_ASSERT(IS_CAN_INSTANCE(pReg));
    HAL_ASSERT(TxMsg != NULL);

    if (READ_BIT(pReg->CMD, CAN_CMD_TX0_REQ_MASK)) {
        cmd = CAN_CMD_TX1_REQ_MASK;
    }

    if (TxMsg->ide == CANFD_ID_EXTENDED) {
        WRITE_REG(pReg->FD_TXID, TxMsg->extId);
        SET_BIT(pReg->FD_TXFRAMEINFO, CAN_FD_TXFRAMEINFO_TXFRAME_FORMAT_MASK);
    } else {
        WRITE_REG(pReg->FD_TXID, TxMsg->stdId);
    }

    if (TxMsg->rtr == CANFD_RTR_REMOTE) {
        SET_BIT(pReg->FD_TXFRAMEINFO, CAN_FD_TXFRAMEINFO_TX_RTR_MASK);
    }

    if (TxMsg->fdf == CANFD_FD_FORMAT) {
        SET_BIT(pReg->FD_TXFRAMEINFO, CAN_FD_TXFRAMEINFO_TX_FDF_MASK | CAN_FD_TXFRAMEINFO_TX_BRS_MASK);
    }

    SET_BIT(pReg->FD_TXFRAMEINFO, CANFD_Len2Dlc(TxMsg->dlc));

    for (i = 0; i < TxMsg->dlc / 4; i++) {
        WRITE_REG(pReg->FD_TXDATA[i],
                  (TxMsg->data[i * 4] << 24) | (TxMsg->data[i * 4 + 1] << 16) | (TxMsg->data[i * 4 + 2] << 8) | TxMsg->data[i * 4 + 3]);
    }
    WRITE_REG(pReg->CMD, cmd);

    return HAL_OK;
}

/**
 * @brief CANFD rx.
 * @param  pReg: can base
 * @param  RxMsg: Rx message
 * @return HAL_OK.
 */
HAL_Status HAL_CANFD_Receive(struct CAN_REG *pReg, struct CANFD_MSG *RxMsg)
{
    uint32_t info, id, len, data[16];
    int i = 0;

    HAL_ASSERT(IS_CAN_INSTANCE(pReg));

    info = READ_REG(pReg->RX_FIFO_RDATA);
    id = READ_REG(pReg->RX_FIFO_RDATA);
    READ_REG(pReg->RX_FIFO_RDATA);
    for (i = 0; i < 16; i++) {
        data[i] = READ_REG(pReg->RX_FIFO_RDATA);
    }

    len = info & CAN_FD_RXFRAMEINFO_RXDATA_LENGTH_MASK;
    RxMsg->ide = (info & CAN_FD_RXFRAMEINFO_RXFRAME_FORMAT_MASK) >> CAN_FD_RXFRAMEINFO_RXFRAME_FORMAT_SHIFT;
    RxMsg->rtr = (info & CAN_FD_RXFRAMEINFO_RX_RTR_MASK) >> CAN_FD_RXFRAMEINFO_RX_RTR_SHIFT;
    RxMsg->fdf = (info & CAN_FD_RXFRAMEINFO_RX_FDF_MASK) >> CAN_FD_RXFRAMEINFO_RX_FDF_SHIFT;
    RxMsg->dlc = CANFD_Dlc2Len(len);
    if (RxMsg->ide == CANFD_ID_EXTENDED) {
        RxMsg->extId = id & CAN_FD_RXID_RX_ID_MASK;
    } else {
        RxMsg->stdId = id & 0x3ff;
    }

    for (i = 0; i < RxMsg->dlc; i += 4) {
        RxMsg->data[i] = (data[i / 4] & 0xff000000) >> 24;
        RxMsg->data[i + 1] = (data[i / 4] & 0x00ff0000) >> 16;
        RxMsg->data[i + 2] = (data[i / 4] & 0xff00) >> 8;
        RxMsg->data[i + 3] = (data[i / 4] & 0xff);
    }

    return HAL_OK;
}

/**
 * @brief CANFD get interrupt.
 * @param  pReg: can base
 * @return interrupt status.
 */
uint32_t HAL_CANFD_GetInterrupt(struct CAN_REG *pReg)
{
    uint32_t isr;

    HAL_ASSERT(IS_CAN_INSTANCE(pReg));

    isr = READ_REG(pReg->INT);

    /* set 1 to clear interrupt */
    WRITE_REG(pReg->INT, isr);

    return isr;
}

/**
 * @brief CANFD get interrupt mask combin.
 * @param type: interrupt type
 * @return err interrupt mask combin.
 */
uint32_t HAL_CANFD_GetErrInterruptMaskCombin(eCANFD_IntType type)
{
    uint32_t isr = 0;

    switch (type) {
    case CANFD_INT_ERR:
        isr = CAN_INT_BUS_OFF_INT_MASK |
              CAN_INT_ERROR_INT_MASK |
              CAN_INT_TX_ARBIT_FAIL_INT_MASK |
              CAN_INT_PASSIVE_ERROR_INT_MASK |
              CAN_INT_OVERLOAD_INT_MASK |
              CAN_INT_ERROR_WARNING_INT_MASK;
        break;
    case CANFD_INT_RX_OF:
        isr = CAN_INT_RX_FIFO_OVERFLOW_INT_MASK | CAN_INT_RX_FIFO_FULL_INT_MASK;
        break;
    case CANFD_INT_TX_FINISH:
        isr = CAN_INT_TX_FINISH_INT_MASK;
        break;
    case CANFD_INT_RX_FINISH:
        isr = CAN_INT_RX_FINISH_INT_MASK;
        break;
    default:
        break;
    }

    return isr;
}

/** @} */

/** @} */

/** @} */

#endif
