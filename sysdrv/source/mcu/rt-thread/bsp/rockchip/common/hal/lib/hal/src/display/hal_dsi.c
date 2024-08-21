/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_DSI_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup DSI
 *  @{
 */

/** @defgroup DSI_How_To_Use How To Use
 *  @{

 The DSI driver can be used as follows:

 @} */

/** @defgroup DSI_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/
#define DSI_UPDATE_BIT(REG, SHIFT, VAL) \
                 VAL ? SET_BIT(REG, SHIFT) : CLEAR_BIT(REG, SHIFT)
#define CMD_PKT_STATUS_TIMEOUT_RETRIES 100
#define WAIT_PHY_READY_MS              10

/********************* Private Structure Definition **************************/
typedef enum {
    DPI_COLOR_CODING_16BIT_1,
    DPI_COLOR_CODING_16BIT_2,
    DPI_COLOR_CODING_16BIT_3,
    DPI_COLOR_CODING_18BIT_1,
    DPI_COLOR_CODING_18BIT_2,
    DPI_COLOR_CODING_24BIT,
} eDSI_DpiColorCoding;

/********************* Private Variable Definition ***************************/
static const char * const DPHY_ERROR[] = {
    "ErrEsc escape entry error from Lane 0",
    "ErrSyncEsc low-power data transmission synchronization error from Lane 0",
    "the ErrControl error from Lane 0",
    "LP0 contention error ErrContentionLP0 from Lane 0",
    "LP1 contention error ErrContentionLP1 from Lane 0",
};
static const char * const ACK_WITH_ERR[] = {
    "the SoT error from the Acknowledge error report",
    "the SoT Sync error from the Acknowledge error report",
    "the EoT Sync error from the Acknowledge error report",
    "the Escape Mode Entry Command error from the Acknowledge error report",
    "the LP Transmit Sync error from the Acknowledge error report",
    "the Peripheral Timeout error from the Acknowledge Error report",
    "the False Control error from the Acknowledge error report",
    "the reserved (specific to device) from the Acknowledge error report",
    "the ECC error, single-bit (detected and corrected) from the Acknowledge error report",
    "the ECC error, multi-bit (detected, not corrected) from the Acknowledge error report",
    "the checksum error (long packet only) from the Acknowledge error report",
    "the not recognized DSI data type from the Acknowledge error report",
    "the DSI VC ID Invalid from the Acknowledge error report",
    "the invalid transmission length from the Acknowledge error report",
    "the reserved (specific to device) from the Acknowledge error report",
    "the DSI protocol violation from the Acknowledge error report",
};
static const char * const ERROR_REPORT[] = {
    "Host reports that the configured timeout counter for the high-speed transmission has expired",
    "Host reports that the configured timeout counter for the low-power reception has expired",
    "Host reports that a received packet contains a single bit error",
    "Host reports that a received packet contains multiple ECC errors",
    "Host reports that a received long packet has a CRC error in its payload",
    "Host receives a transmission that does not end in the expected by boundaries",
    "Host receives a transmission that does not end with an End of Transmission packet",
    "An overflow occurs in the DPI pixel payload FIFO",
    "An overflow occurs in the Generic command FIFO",
    "An overflow occurs in the Generic write payload FIFO",
    "An underflow occurs in the Generic write payload FIFO",
    "An underflow occurs in the Generic read FIFO",
    "An overflow occurs in the Generic read FIFO",
};

/********************* Private Function Definition ***************************/
static uint32_t DSI_CheckFifoStatus(struct DSI_REG *pReg, uint32_t flag)
{
    return READ_BIT(pReg->CMD_PKT_STATUS, flag);
}

static HAL_Status DSI_WaitFifoNotFull(struct DSI_REG *pReg, uint32_t flag)
{
    uint32_t retries = 0;

    while (DSI_CheckFifoStatus(pReg, flag)) {
        if (++retries > CMD_PKT_STATUS_TIMEOUT_RETRIES) {
            HAL_DBG_ERR("%s: flag:%lx timeout for wait not full\n", __func__, flag);

            return HAL_TIMEOUT;
        }
        HAL_DelayUs(1);
    }

    return HAL_OK;
}

static HAL_Status DSI_WaitFifoEmpty(struct DSI_REG *pReg)
{
    uint32_t retries = 0;

    while (!DSI_CheckFifoStatus(pReg, DSI_CMD_PKT_STATUS_GEN_CMD_EMPTY_MASK)) {
        if (++retries > CMD_PKT_STATUS_TIMEOUT_RETRIES) {
            HAL_DBG_ERR("%s: timeout for wait empty\n", __func__);

            return HAL_TIMEOUT;
        }
        HAL_DelayUs(1);
    }

    return HAL_OK;
}

static uint32_t DSI_GetHcomponentLbcc(uint32_t hcomponent, uint16_t laneMbps, uint32_t clk)
{
    uint32_t lbcc;
    uint64_t lbccTmp;

    lbccTmp = hcomponent * laneMbps * 1000 / 8;
    lbcc = HAL_DIV_ROUND_UP(lbccTmp, clk);

    return lbcc;
}

/** @} */
/********************* Public Function Definition ****************************/

/** @defgroup DSI_Exported_Functions_Group2 IO Functions

 This section provides functions allowing to IO controlling:

 *  @{
 */

/**
 * @brief  DSI Interrupt Handler.
 * @param  pReg: DSI reg base.
 * @return HAL_Status.
 */
HAL_Status HAL_DSI_IrqHandler(struct DSI_REG *pReg)
{
    uint32_t intSt0, intSt1, i;

    intSt0 = READ_REG(pReg->INT_ST0);
    intSt1 = READ_REG(pReg->INT_ST1);

    for (i = 0; i < HAL_ARRAY_SIZE(ACK_WITH_ERR); i++) {
        if (intSt0 & HAL_BIT(i)) {
            HAL_DBG_ERR("DSI Irq: %s\n", ACK_WITH_ERR[i]);
        }
    }

    for (i = 0; i < HAL_ARRAY_SIZE(DPHY_ERROR); i++) {
        if (intSt0 & HAL_BIT(16 + i)) {
            HAL_DBG_ERR("DSI Irq: %s\n", DPHY_ERROR[i]);
        }
    }

    for (i = 0; i < HAL_ARRAY_SIZE(ERROR_REPORT); i++) {
        if (intSt1 & HAL_BIT(i)) {
            HAL_DBG_ERR("DSI Irq: %s\n", ERROR_REPORT[i]);
        }
    }

    return HAL_OK;
}

/**
 * @brief  Send DSI Command Packet.
 * @param  pReg: DSI reg base.
 * @param  dataType: DSI Command Data Type.
 * @param  payloadLen: DSI Command Data Len.
 * @param  payload: DSI Command Data.
 * @return HAL_Status.
 */
HAL_Status HAL_DSI_SendPacket(struct DSI_REG *pReg, uint8_t dataType,
                              uint8_t payloadLen, const uint8_t *payload)
{
    uint32_t temp, val;
    HAL_Status ret;

    ret = DSI_WaitFifoNotFull(pReg, DSI_CMD_PKT_STATUS_GEN_CMD_FULL_MASK);
    if (ret != HAL_OK) {
        return ret;
    }
    val = (0x0 << 6) | dataType;
    switch (dataType) {
    case MIPI_DSI_DCS_COMPRESSION_MODE:
    case MIPI_DSI_DCS_SHORT_WRITE:
    case MIPI_DSI_DCS_SHORT_WRITE_PARAM:
    case MIPI_DSI_GENERIC_SHORT_WRITE_0_PARAM:
    case MIPI_DSI_GENERIC_SHORT_WRITE_1_PARAM:
    case MIPI_DSI_GENERIC_SHORT_WRITE_2_PARAM:
    {
        val |= (payloadLen > 0) ? (payload[0] << 8) : 0;
        val |= (payloadLen > 1) ? (payload[1] << 16) : 0;
        break;
    }
    case MIPI_DSI_DCS_LONG_WRITE:
    case MIPI_DSI_GENERIC_LONG_WRITE:
    case MIPI_DSI_PPS_LONG_WRITE:
    {
        /* Send payload */
        ret = DSI_WaitFifoNotFull(pReg, DSI_CMD_PKT_STATUS_GEN_PLD_W_FULL_MASK);
        if (ret != HAL_OK) {
            return ret;
        }

        val |= (payloadLen << 8);
        while (payloadLen) {
            if (payloadLen < 4) {
                temp = 0;
                memcpy(&temp, payload, payloadLen);
                WRITE_REG(pReg->GEN_PLD_DATA, temp);
                payloadLen = 0;
            } else {
                temp = *(uint32_t *)payload;
                WRITE_REG(pReg->GEN_PLD_DATA, temp);
                payload += 4;
                payloadLen -= 4;
            }
        }
        break;
    }
    default:
        HAL_DBG_ERR("%s, Unsupport dataType:0x%x\n", __func__, dataType);
    }

    /* Send packet header */
    WRITE_REG(pReg->GEN_HDR, val);

    ret = DSI_WaitFifoEmpty(pReg);

    return ret;
}

/**
 * @brief  Config DSI Send Message In Hs Mode.
 * @param  pReg: DSI reg base.
 * @return HAL_Status.
 */
HAL_Status HAL_DSI_MsgHsModeConfig(struct DSI_REG *pReg)
{
    uint32_t lpMask = DSI_CMD_MODE_CFG_GEN_SW_0P_TX_MASK | DSI_CMD_MODE_CFG_GEN_SW_1P_TX_MASK |
                      DSI_CMD_MODE_CFG_GEN_SW_2P_TX_MASK | DSI_CMD_MODE_CFG_GEN_SR_0P_TX_MASK |
                      DSI_CMD_MODE_CFG_GEN_SR_1P_TX_MASK | DSI_CMD_MODE_CFG_GEN_SR_2P_TX_MASK |
                      DSI_CMD_MODE_CFG_GEN_LW_TX_MASK | DSI_CMD_MODE_CFG_DCS_SW_0P_TX_MASK |
                      DSI_CMD_MODE_CFG_DCS_SW_1P_TX_MASK | DSI_CMD_MODE_CFG_DCS_SR_0P_TX_MASK |
                      DSI_CMD_MODE_CFG_DCS_LW_TX_MASK | DSI_CMD_MODE_CFG_MAX_RD_PKT_SIZE_MASK;

    DSI_UPDATE_BIT(pReg->VID_MODE_CFG, DSI_VID_MODE_CFG_LP_CMD_EN_MASK, 0);
    DSI_UPDATE_BIT(pReg->CMD_MODE_CFG, lpMask, 0);

    DSI_UPDATE_BIT(pReg->LPCLK_CTRL, DSI_LPCLK_CTRL_PHY_TXREQUESTCLKHS_MASK, 1);

    return HAL_OK;
}

/**
 * @brief  Config DSI Send Message In Lp Mode.
 * @param  pReg: DSI reg base.
 * @return HAL_Status.
 */
HAL_Status HAL_DSI_MsgLpModeConfig(struct DSI_REG *pReg)
{
    uint32_t lpMask = DSI_CMD_MODE_CFG_GEN_SW_0P_TX_MASK | DSI_CMD_MODE_CFG_GEN_SW_1P_TX_MASK |
                      DSI_CMD_MODE_CFG_GEN_SW_2P_TX_MASK | DSI_CMD_MODE_CFG_GEN_SR_0P_TX_MASK |
                      DSI_CMD_MODE_CFG_GEN_SR_1P_TX_MASK | DSI_CMD_MODE_CFG_GEN_SR_2P_TX_MASK |
                      DSI_CMD_MODE_CFG_GEN_LW_TX_MASK | DSI_CMD_MODE_CFG_DCS_SW_0P_TX_MASK |
                      DSI_CMD_MODE_CFG_DCS_SW_1P_TX_MASK | DSI_CMD_MODE_CFG_DCS_SR_0P_TX_MASK |
                      DSI_CMD_MODE_CFG_DCS_LW_TX_MASK | DSI_CMD_MODE_CFG_MAX_RD_PKT_SIZE_MASK;

    DSI_UPDATE_BIT(pReg->VID_MODE_CFG, DSI_VID_MODE_CFG_LP_CMD_EN_MASK, 1);
    DSI_UPDATE_BIT(pReg->CMD_MODE_CFG, lpMask, 1);

    return HAL_OK;
}

/** @} */

/** @defgroup DSI_Exported_Functions_Group3 Init and DeInit Functions

 This section provides functions allowing to init and deinit the module:

 *  @{
 */

/**
 * @brief  Disable DSI Interrupt.
 * @param  pReg: DSI reg base.
 * @return HAL_Status.
 */
HAL_Status HAL_DSI_IrqDisable(struct DSI_REG *pReg)
{
    WRITE_REG(pReg->INT_MSK0, 0);
    WRITE_REG(pReg->INT_MSK1, 0);

    return HAL_OK;
}

/**
 * @brief  Enable DSI Interrupt.
 * @param  pReg: DSI reg base.
 * @return HAL_Status.
 */
HAL_Status HAL_DSI_IrqEnable(struct DSI_REG *pReg)
{
    uint32_t intMsk0 = DSI_INT_ST0_ACK_WITH_ERR_0_MASK | DSI_INT_ST0_ACK_WITH_ERR_1_MASK | DSI_INT_ST0_ACK_WITH_ERR_2_MASK |
                       DSI_INT_ST0_ACK_WITH_ERR_3_MASK | DSI_INT_ST0_ACK_WITH_ERR_4_MASK | DSI_INT_ST0_ACK_WITH_ERR_5_MASK |
                       DSI_INT_ST0_ACK_WITH_ERR_6_MASK | DSI_INT_ST0_ACK_WITH_ERR_7_MASK | DSI_INT_ST0_ACK_WITH_ERR_8_MASK |
                       DSI_INT_ST0_ACK_WITH_ERR_9_MASK | DSI_INT_ST0_ACK_WITH_ERR_10_MASK | DSI_INT_ST0_ACK_WITH_ERR_11_MASK |
                       DSI_INT_ST0_ACK_WITH_ERR_12_MASK | DSI_INT_ST0_ACK_WITH_ERR_13_MASK | DSI_INT_ST0_ACK_WITH_ERR_14_MASK |
                       DSI_INT_ST0_ACK_WITH_ERR_15_MASK | DSI_INT_ST0_DPHY_ERRORS_0_MASK | DSI_INT_ST0_DPHY_ERRORS_1_MASK |
                       DSI_INT_ST0_DPHY_ERRORS_2_MASK | DSI_INT_ST0_DPHY_ERRORS_3_MASK | DSI_INT_ST0_DPHY_ERRORS_4_MASK;
    uint32_t intMsk1 = DSI_INT_ST1_TO_HS_TX_MASK | DSI_INT_ST1_TO_LP_RX_MASK | DSI_INT_ST1_ECC_SINGLE_ERR_MASK |
                       DSI_INT_ST1_ECC_MULTI_ERR_MASK | DSI_INT_ST1_CRC_ERR_MASK | DSI_INT_ST1_PKT_SIZE_ERR_MASK |
                       DSI_INT_ST1_EOPT_ERR_MASK | DSI_INT_ST1_DPI_PLD_WR_ERR_MASK | DSI_INT_ST1_GEN_CMD_WR_ERR_MASK |
                       DSI_INT_ST1_GEN_PLD_WR_ERR_MASK | DSI_INT_ST1_GEN_PLD_SEND_ERR_MASK | DSI_INT_ST1_GEN_PLD_RD_ERR_MASK |
                       DSI_INT_ST1_GEN_PLD_RECEV_ERR_MASK;

    WRITE_REG(pReg->INT_MSK0, intMsk0);
    WRITE_REG(pReg->INT_MSK1, intMsk1);

    return HAL_OK;
}

/**
 * @brief  DSI m31 Dphy Power Up.
 * @param  pReg: DSI reg base.
 * @return HAL_Status.
 */
HAL_Status HAL_DSI_M31DphyPowerUp(struct DSI_REG *pReg)
{
    uint32_t phyStopState = GRF_DSI_STATUS0_DPHY_PHYSTOPSTATECLKLANE_MASK |
                            GRF_DSI_STATUS0_DPHY_STOPSTATE_MASK;
    uint32_t start;

    WRITE_REG_MASK_WE(GRF->DSI_CON0, GRF_DSI_CON0_DPHY_PHYRSTZ_MASK, 1);

    start = HAL_GetTick();
    /* *
     * There is no phy_lock signal interface in m31 dphy
     * Use each lane's stop state to judge dphy is ready
     */
    while (READ_BIT(GRF->DSI_STATUS0, phyStopState) != phyStopState) {
        if (HAL_GetTick() - start > WAIT_PHY_READY_MS) {
            HAL_DBG_ERR("wait mipi dphy ready time out\n");

            return HAL_TIMEOUT;
        }
    }

    return HAL_OK;
}

/**
 * @brief  DSI m31 Dphy Power Down.
 * @param  pReg: DSI reg base.
 * @return HAL_Status.
 */
HAL_Status HAL_DSI_M31DphyPowerDown(struct DSI_REG *pReg)
{
    WRITE_REG_MASK_WE(GRF->DSI_CON0, GRF_DSI_CON0_DPHY_PHYRSTZ_MASK, 0);

    return HAL_OK;
}

/**
 * @brief  DSI m31 Dphy Init.
 * @param  pReg: DSI reg base.
 * @param  laneMbps: DSI per lane mbps.
 * @param  refClkIn: Dphy ref clk in rate.
 * @return Actually DSI per lane mbps..
 */
uint16_t HAL_DSI_M31DphyInit(struct DSI_REG *pReg, uint16_t laneMbps, eDSI_DphyRefClk refClkIn)
{
    uint32_t index, clkSel = 0;
    uint32_t lanebps = laneMbps * 1000000;

    /* Table 8-2 Relative setting for PLL output clock */
    const struct {
        uint32_t maxLanebps;
        uint32_t pllClkSel;
    } pllClkSelTable[] = {
        {   80000000, 52 },
        {   93125000, 94 },
        {  186250000, 243 },
        {  372500000, 392 },
        {  745000000, 541 },
        { 1500000000, 692 },
    };

    if (lanebps <= 80000000) {
        clkSel = 52;
        laneMbps = 80;
    } else if (lanebps > 1500000000) {
        HAL_DBG_ERR("dsi mbps is out of range, using 1500 mbps config\n");
        clkSel = 692;
        laneMbps = 1500;
    } else {
        for (index = 0; index < HAL_ARRAY_SIZE(pllClkSelTable); index++) {
            if (lanebps < pllClkSelTable[index].maxLanebps) {
                break;
            }
        }
        clkSel = HAL_DIV_ROUND_UP((lanebps - pllClkSelTable[index - 1 ].maxLanebps),
                                  (312500 << (index - 1)));

        lanebps = pllClkSelTable[index - 1 ].maxLanebps + clkSel * (312500 << (index - 1));
        laneMbps = lanebps / 1000000;

        clkSel += pllClkSelTable[index - 1 ].pllClkSel;
    }

    HAL_DBG("actually dsi mbps = %d, m31 dphy_ref_clk = %d\n", laneMbps, refClkIn);

    DSI_UPDATE_BIT(pReg->PHY_RSTZ, DSI_PHY_RSTZ_PHY_ENABLECLK_MASK, 0);
    DSI_UPDATE_BIT(pReg->PHY_RSTZ, DSI_PHY_RSTZ_PHY_RSTZ_MASK, 1);
    DSI_UPDATE_BIT(pReg->PHY_RSTZ, DSI_PHY_RSTZ_PHY_SHUTDOWNZ_MASK, 1);

    HAL_DSI_M31DphyPowerDown(pReg);

    WRITE_REG_MASK_WE(GRF->DSI_CON0, GRF_DSI_CON0_DPHY_PLL_CLK_SEL_MASK,
                      clkSel << GRF_DSI_CON0_DPHY_PLL_CLK_SEL_SHIFT);

    WRITE_REG_MASK_WE(GRF->DSI_CON0, GRF_DSI_CON0_DPHY_REFCLK_IN_SEL_MASK,
                      refClkIn << GRF_DSI_CON0_DPHY_REFCLK_IN_SEL_SHIFT);

    WRITE_REG_MASK_WE(GRF->DSI_CON2, GRF_DSI_CON2_DPHY_LANE_SWAP0_MASK |
                      GRF_DSI_CON2_DPHY_LANE_SWAP1_MASK | GRF_DSI_CON2_DPHY_LANE_SWAP2_MASK |
                      GRF_DSI_CON2_DPHY_LANE_SWAP3_MASK | GRF_DSI_CON2_DPHY_LANE_SWAP_CLK_MASK,
                      0 << GRF_DSI_CON2_DPHY_LANE_SWAP0_SHIFT | 1 << GRF_DSI_CON2_DPHY_LANE_SWAP1_SHIFT |
                      2 << GRF_DSI_CON2_DPHY_LANE_SWAP2_SHIFT | 3 << GRF_DSI_CON2_DPHY_LANE_SWAP3_SHIFT |
                      4 << GRF_DSI_CON2_DPHY_LANE_SWAP_CLK_SHIFT);

    WRITE_REG_MASK_WE(GRF->DSI_CON0, GRF_DSI_CON0_DPHY_PHYRSTZ_MASK, 1);

    DSI_UPDATE_BIT(pReg->PHY_RSTZ, DSI_PHY_RSTZ_PHY_SHUTDOWNZ_MASK, 0);
    DSI_UPDATE_BIT(pReg->PHY_RSTZ, DSI_PHY_RSTZ_PHY_RSTZ_MASK, 0);
    DSI_UPDATE_BIT(pReg->PHY_RSTZ, DSI_PHY_RSTZ_PHY_ENABLECLK_MASK, 1);

    HAL_DSI_M31DphyPowerUp(pReg);

    return laneMbps;
}

/**
 * @brief  DSI Init.
 * @param  pReg: DSI reg base.
 * @param  laneMbps: DSI per lane mbps.
 * @return HAL_Status.
 */
HAL_Status HAL_DSI_Init(struct DSI_REG *pReg, uint16_t laneMbps)
{
    uint32_t val;

    HAL_ASSERT(IS_DSI_INSTANCE(pReg));

    val = HAL_DIV_ROUND_UP(laneMbps >> 3, 20);
    WRITE_REG(pReg->PWR_UP, 0);
    WRITE_REG(pReg->CLKMGR_CFG, 10 << DSI_CLKMGR_CFG_TO_CLK_DIVISION_SHIFT |
              val << DSI_CLKMGR_CFG_TX_ESC_CLK_DIVISION_SHIFT);

    return HAL_OK;
}

/**
 * @brief  DSI Dpi Config.
 * @param  pReg: DSI reg base.
 * @param  pModeInfo: display mode info.
 * @param  busFormat: display busFormat.
 * @return HAL_Status.
 */
HAL_Status HAL_DSI_DpiConfig(struct DSI_REG *pReg,
                             struct DISPLAY_MODE_INFO *pModeInfo,
                             uint16_t busFormat)
{
    uint32_t lpcmdTime;
    uint32_t color = 0, polarity = 0;

    switch (busFormat) {
    case MEDIA_BUS_FMT_RGB888_1X24:
        color = DPI_COLOR_CODING_24BIT;
        break;
    case MEDIA_BUS_FMT_RGB666_1X24_CPADHI:
        color = DPI_COLOR_CODING_18BIT_2 | DSI_DPI_COLOR_CODING_LOOSELY18_EN_MASK;
        break;
    case MEDIA_BUS_FMT_RGB666_1X18:
        color = DPI_COLOR_CODING_18BIT_1;
        break;
    case MEDIA_BUS_FMT_RGB565_1X16:
        color = DPI_COLOR_CODING_16BIT_1;
        break;
    }

    polarity |= (pModeInfo->flags & VIDEO_MODE_FLAG_NHSYNC ? 1 : 0) << DSI_DPI_CFG_POL_HSYNC_ACTIVE_LOW_SHIFT;
    polarity |= (pModeInfo->flags & VIDEO_MODE_FLAG_NVSYNC ? 1 : 0) << DSI_DPI_CFG_POL_VSYNC_ACTIVE_LOW_SHIFT;
    lpcmdTime = 4 << DSI_DPI_LP_CMD_TIM_OUTVACT_LPCMD_TIME_SHIFT | 4 << DSI_DPI_LP_CMD_TIM_INVACT_LPCMD_TIME_SHIFT;

    WRITE_REG(pReg->DPI_COLOR_CODING, color);
    WRITE_REG(pReg->DPI_CFG_POL, polarity);
    WRITE_REG(pReg->DPI_LP_CMD_TIM, lpcmdTime);

    return HAL_OK;
}

/**
 * @brief  DSI Packet Handler Config.
 * @param  pReg: DSI reg base.
 * @param  pModeInfo: display mode info.
 * @return HAL_Status.
 */
HAL_Status HAL_DSI_PacketHandlerConfig(struct DSI_REG *pReg,
                                       struct DISPLAY_MODE_INFO *pModeInfo)
{
    uint32_t val = DSI_PCKHDL_CFG_BTA_EN_MASK | DSI_PCKHDL_CFG_ECC_RX_EN_MASK | DSI_PCKHDL_CFG_CRC_RX_EN_MASK | DSI_PCKHDL_CFG_EOTP_TX_EN_MASK;

    if (pModeInfo->flags & DSI_MODE_EOT_PACKET) {
        val &= ~DSI_PCKHDL_CFG_EOTP_TX_EN_MASK;
    }
    WRITE_REG(pReg->PCKHDL_CFG, val);

    return HAL_OK;
}

/**
 * @brief  DSI Mode Config.
 * @param  pReg: DSI reg base.
 * @param  pModeInfo: display mode info.
 * @return HAL_Status.
 */
HAL_Status HAL_DSI_ModeConfig(struct DSI_REG *pReg,
                              struct DISPLAY_MODE_INFO *pModeInfo)
{
    uint32_t val = DSI_VID_MODE_CFG_LP_VSA_EN_MASK | DSI_VID_MODE_CFG_LP_VFP_EN_MASK | DSI_VID_MODE_CFG_LP_VBP_EN_MASK |
                   DSI_VID_MODE_CFG_LP_VACT_EN_MASK | DSI_VID_MODE_CFG_LP_HBP_EN_MASK | DSI_VID_MODE_CFG_LP_HFP_EN_MASK;

    if (pModeInfo->flags & DSI_MODE_VIDEO) {
        if (pModeInfo->flags & DSI_MODE_VIDEO_BURST) {
            val |= 0x2 << DSI_VID_MODE_CFG_VID_MODE_TYPE_SHIFT;
        } else if (pModeInfo->flags & DSI_MODE_VIDEO_SYNC_PULSE) {
            val |= 0 << DSI_VID_MODE_CFG_VID_MODE_TYPE_SHIFT;
        } else {
            val |= DSI_VID_MODE_CFG_VID_MODE_TYPE_MASK;
        }

        WRITE_REG(pReg->VID_MODE_CFG, val);
        WRITE_REG(pReg->VID_PKT_SIZE, pModeInfo->crtcHdisplay);
    } else {
        WRITE_REG(pReg->TO_CNT_CFG, 1000 << DSI_TO_CNT_CFG_HSTX_TO_CNT_SHIFT |
                  1000 << DSI_TO_CNT_CFG_LPRX_TO_CNT_SHIFT);
        WRITE_REG(pReg->BTA_TO_CNT, 0xd00);
    }
    if (pModeInfo->flags & DSI_CLOCK_NON_CONTINUOUS) {
        DSI_UPDATE_BIT(pReg->LPCLK_CTRL, DSI_LPCLK_CTRL_AUTO_CLKLANE_CTRL_MASK, 1);
    }

    DSI_UPDATE_BIT(pReg->MODE_CFG, DSI_MODE_CFG_CMD_VIDEO_MODE_MASK, 1);
    WRITE_REG(pReg->PWR_UP, 0x1);

    return HAL_OK;
}

/**
 * @brief  DSI Line Timer Config.
 * @param  pReg: DSI reg base.
 * @param  laneMbps: DSI per lane mbps.
 * @param  pModeInfo: display mode info.
 * @return HAL_Status.
 */
HAL_Status HAL_DSI_LineTimerConfig(struct DSI_REG *pReg,
                                   uint16_t laneMbps,
                                   struct DISPLAY_MODE_INFO *pModeInfo)
{
    uint16_t hsa = pModeInfo->crtcHsyncEnd - pModeInfo->crtcHsyncStart;
    uint16_t hbp = pModeInfo->crtcHtotal - pModeInfo->crtcHsyncEnd;
    uint32_t clk = pModeInfo->crtcClock;
    uint32_t lpcc;

    lpcc = DSI_GetHcomponentLbcc(hsa, laneMbps, clk);
    WRITE_REG(pReg->VID_HSA_TIME, lpcc);
    lpcc = DSI_GetHcomponentLbcc(hbp, laneMbps, clk);
    WRITE_REG(pReg->VID_HBP_TIME, lpcc);
    lpcc = DSI_GetHcomponentLbcc(pModeInfo->crtcHtotal, laneMbps, clk);
    WRITE_REG(pReg->VID_HLINE_TIME, lpcc);

    return HAL_OK;
}

HAL_Status HAL_DSI_UpdateLineTimer(struct DSI_REG *pReg,
                                   uint16_t laneMbps,
                                   struct DISPLAY_MODE_INFO *pModeInfo,
                                   struct DISPLAY_RECT *pDisplayRect)
{
    uint16_t hsa = pModeInfo->crtcHsyncEnd - pModeInfo->crtcHsyncStart;
    uint16_t hbp = pDisplayRect->w - pModeInfo->crtcHsyncEnd;
    uint32_t clk = pModeInfo->crtcClock;
    uint32_t lpcc;

    DSI_UPDATE_BIT(pReg->CMD_MODE_CFG, DSI_CMD_MODE_CFG_TEAR_FX_EN_MASK, 0);
    WRITE_REG(pReg->EDPI_CMD_SIZE, pDisplayRect->w);
    DSI_UPDATE_BIT(pReg->CMD_MODE_CFG, DSI_CMD_MODE_CFG_DCS_LW_TX_MASK, 0);
    DSI_UPDATE_BIT(pReg->CMD_MODE_CFG, DSI_CMD_MODE_CFG_TEAR_FX_EN_MASK, 1);

    lpcc = DSI_GetHcomponentLbcc(hsa, laneMbps, clk);
    WRITE_REG(pReg->VID_HSA_TIME, lpcc);
    lpcc = DSI_GetHcomponentLbcc(hbp, laneMbps, clk);
    WRITE_REG(pReg->VID_HBP_TIME, lpcc);
    lpcc = DSI_GetHcomponentLbcc(pDisplayRect->w, laneMbps, clk);
    WRITE_REG(pReg->VID_HLINE_TIME, lpcc);

    return HAL_OK;
}

/**
 * @brief  DSI Vertical Timing Config.
 * @param  pReg: DSI reg base.
 * @param  pModeInfo: display mode info.
 * @return HAL_Status.
 */
HAL_Status HAL_DSI_VerticalTimingConfig(struct DSI_REG *pReg,
                                        struct DISPLAY_MODE_INFO *pModeInfo)
{
    uint16_t vfp = pModeInfo->crtcVsyncStart - pModeInfo->crtcVdisplay;
    uint16_t vbp = pModeInfo->crtcVtotal - pModeInfo->crtcVsyncEnd;
    uint16_t vsa = pModeInfo->crtcVsyncEnd - pModeInfo->crtcVsyncStart;

    WRITE_REG(pReg->VID_VSA_LINES, vsa);
    WRITE_REG(pReg->VID_VBP_LINES, vbp);
    WRITE_REG(pReg->VID_VFP_LINES, vfp);
    WRITE_REG(pReg->VID_VACTIVE_LINES, pModeInfo->crtcVdisplay);

    return HAL_OK;
}

/**
 * @brief  DSI Dphy Timing Config.
 * @param  pReg: DSI reg base.
 * @param  pModeInfo: display mode info.
 * @param  lanes: number of mipi data lines.
 * @return HAL_Status.
 */
HAL_Status HAL_DSI_DphyTimingConfig(struct DSI_REG *pReg,
                                    struct DISPLAY_MODE_INFO *pModeInfo,
                                    uint8_t lanes)
{
    WRITE_REG(pReg->PHY_TMR_LPCLK_CFG, 0x40 << DSI_PHY_TMR_LPCLK_CFG_PHY_CLKLP2HS_TIME_SHIFT |
              0x40 << DSI_PHY_TMR_LPCLK_CFG_PHY_CLKHS2LP_TIME_SHIFT);
    WRITE_REG(pReg->PHY_TMR_CFG, 0x14 << DSI_PHY_TMR_CFG_PHY_HS2LP_TIME_SHIFT |
              0x10 << DSI_PHY_TMR_CFG_PHY_LP2HS_TIME_SHIFT | 10000 << DSI_PHY_TMR_CFG_MAX_RD_TIME_SHIFT);

    WRITE_REG(pReg->PHY_IF_CFG, 0x5 << DSI_PHY_IF_CFG_PHY_STOP_WAIT_TIME_SHIFT |
              (lanes - 1) << DSI_PHY_IF_CFG_N_LANES_SHIFT);

    return HAL_OK;
}

/**
 * @brief  DSI Enable.
 * @param  pReg: DSI reg base.
 * @param  pModeInfo: display mode info.
 * @return HAL_Status.
 */
HAL_Status HAL_DSI_Enable(struct DSI_REG *pReg,
                          struct DISPLAY_MODE_INFO *pModeInfo)
{
    DSI_UPDATE_BIT(pReg->LPCLK_CTRL, DSI_LPCLK_CTRL_PHY_TXREQUESTCLKHS_MASK, 1);
    if (pModeInfo->flags & DSI_MODE_VIDEO) {
        DSI_UPDATE_BIT(pReg->MODE_CFG, DSI_MODE_CFG_CMD_VIDEO_MODE_MASK, 0);
    } else {
        DSI_UPDATE_BIT(pReg->MODE_CFG, DSI_MODE_CFG_CMD_VIDEO_MODE_MASK, 1);
        WRITE_REG(pReg->EDPI_CMD_SIZE, pModeInfo->crtcHdisplay);
    }
    WRITE_REG(pReg->PWR_UP, 0x1);

    return HAL_OK;
}

/**
 * @brief  DSI Disable.
 * @param  pReg: DSI reg base.
 * @param  pModeInfo: display mode info.
 * @return HAL_Status.
 */
HAL_Status HAL_DSI_Disable(struct DSI_REG *pReg,
                           struct DISPLAY_MODE_INFO *pModeInfo)
{
    DSI_UPDATE_BIT(pReg->LPCLK_CTRL, DSI_LPCLK_CTRL_PHY_TXREQUESTCLKHS_MASK, 0);
    if (pModeInfo->flags & DSI_MODE_VIDEO) {
        DSI_UPDATE_BIT(pReg->MODE_CFG, DSI_MODE_CFG_CMD_VIDEO_MODE_MASK, 1);
    } else {
        WRITE_REG(pReg->EDPI_CMD_SIZE, 0);
    }
    WRITE_REG(pReg->PWR_UP, 0x0);

    return HAL_OK;
}

/** @} */

/** @} */

/** @} */

#endif /* HAL_DSI_MODULE_ENABLED */
