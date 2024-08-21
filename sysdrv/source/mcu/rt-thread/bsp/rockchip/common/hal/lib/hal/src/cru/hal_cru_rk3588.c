/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */
#include "hal_base.h"

#if defined(SOC_RK3588) && defined(HAL_CRU_MODULE_ENABLED)

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup CRU
 *  @{
 */

/** @defgroup CRU_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/
/********************* Private Structure Definition **************************/

static struct PLL_CONFIG PLL_TABLE[] =
{
    /* _mhz, _p, _m, _s, _k */
    RK3588_PLL_RATE(2400000000, 2, 200, 0, 0),
    RK3588_PLL_RATE(2304000000, 2, 192, 0, 0),
    RK3588_PLL_RATE(2208000000, 2, 368, 1, 0),
    RK3588_PLL_RATE(2112000000, 2, 352, 1, 0),
    RK3588_PLL_RATE(2016000000, 2, 336, 1, 0),
    RK3588_PLL_RATE(1920000000, 2, 320, 1, 0),
    RK3588_PLL_RATE(1800000000, 2, 300, 1, 0),
    RK3588_PLL_RATE(1704000000, 2, 284, 1, 0),
    RK3588_PLL_RATE(1608000000, 2, 268, 1, 0),
    RK3588_PLL_RATE(1512000000, 2, 252, 1, 0),
    RK3588_PLL_RATE(1500000000, 2, 250, 1, 0),
    RK3588_PLL_RATE(1416000000, 2, 236, 1, 0),
    RK3588_PLL_RATE(1200000000, 2, 200, 1, 0),
    RK3588_PLL_RATE(1188000000, 2, 198, 1, 0),
    RK3588_PLL_RATE(1008000000, 2, 336, 2, 0),
    RK3588_PLL_RATE(1000000000, 3, 500, 2, 0),
    RK3588_PLL_RATE(983040000, 4, 655, 2, 23592),
    RK3588_PLL_RATE(955520000, 3, 477, 2, 49806),
    RK3588_PLL_RATE(903168000, 6, 903, 2, 11009),
    RK3588_PLL_RATE(816000000, 2, 272, 2, 0),
    RK3588_PLL_RATE(786432000, 2, 262, 2, 9437),
    RK3588_PLL_RATE(786000000, 1, 131, 2, 0),
    RK3588_PLL_RATE(785560000, 3, 392, 2, 51117),
    RK3588_PLL_RATE(722534400, 8, 963, 2, 24850),
    RK3588_PLL_RATE(408000000, 2, 272, 3, 0),
    RK3588_PLL_RATE(100000000, 3, 400, 5, 0),
    { /* sentinel */ },
};

static struct PLL_SETUP LPLL =
{
    .conOffset0 = &(DSUCRU->LPLL_CON[0]),
    .conOffset1 = &(DSUCRU->LPLL_CON[1]),
    .conOffset2 = &(DSUCRU->LPLL_CON[2]),
    .conOffset3 = &(DSUCRU->LPLL_CON[3]),
    .conOffset6 = &(DSUCRU->LPLL_CON[6]),
    .modeOffset = &(DSUCRU->MODE_CON[0]),
    .modeShift = 0,
    .lockShift = 15,
    .modeMask = 0x3,
    .rateTable = PLL_TABLE,
};

static struct PLL_SETUP B0PLL =
{
    .conOffset0 = &(BIGCORE0CRU->B0PLL_CON[0]),
    .conOffset1 = &(BIGCORE0CRU->B0PLL_CON[1]),
    .conOffset2 = &(BIGCORE0CRU->B0PLL_CON[2]),
    .conOffset3 = &(BIGCORE0CRU->B0PLL_CON[3]),
    .conOffset6 = &(BIGCORE0CRU->B0PLL_CON[6]),
    .modeOffset = &(BIGCORE0CRU->MODE_CON[0]),
    .modeShift = 0,
    .lockShift = 15,
    .modeMask = 0x3,
    .rateTable = PLL_TABLE,
};

static struct PLL_SETUP B1PLL =
{
    .conOffset0 = &(BIGCORE1CRU->B1PLL_CON[0]),
    .conOffset1 = &(BIGCORE1CRU->B1PLL_CON[1]),
    .conOffset2 = &(BIGCORE1CRU->B1PLL_CON[2]),
    .conOffset3 = &(BIGCORE1CRU->B1PLL_CON[3]),
    .conOffset6 = &(BIGCORE1CRU->B1PLL_CON[6]),
    .modeOffset = &(BIGCORE1CRU->MODE_CON[0]),
    .modeShift = 0,
    .lockShift = 15,
    .modeMask = 0x3,
    .rateTable = PLL_TABLE,
};

static struct PLL_SETUP CPLL =
{
    .conOffset0 = &(CRU->CPLL_CON[0]),
    .conOffset1 = &(CRU->CPLL_CON[1]),
    .conOffset2 = &(CRU->CPLL_CON[2]),
    .conOffset3 = &(CRU->CPLL_CON[3]),
    .conOffset6 = &(CRU->CPLL_CON[6]),
    .modeOffset = &(CRU->MODE_CON[0]),
    .modeShift = 8,
    .lockShift = 15,
    .modeMask = 0x3 << 8,
    .rateTable = PLL_TABLE,
};

static struct PLL_SETUP GPLL =
{
    .conOffset0 = &(CRU->GPLL_CON[0]),
    .conOffset1 = &(CRU->GPLL_CON[1]),
    .conOffset2 = &(CRU->GPLL_CON[2]),
    .conOffset3 = &(CRU->GPLL_CON[3]),
    .conOffset6 = &(CRU->GPLL_CON[6]),
    .modeOffset = &(CRU->MODE_CON[0]),
    .modeShift = 2,
    .lockShift = 15,
    .modeMask = 0x3 << 2,
    .rateTable = PLL_TABLE,
};

static struct PLL_SETUP NPLL =
{
    .conOffset0 = &(CRU->NPLL_CON[0]),
    .conOffset1 = &(CRU->NPLL_CON[1]),
    .conOffset2 = &(CRU->NPLL_CON[2]),
    .conOffset3 = &(CRU->NPLL_CON[3]),
    .conOffset6 = &(CRU->NPLL_CON[6]),
    .modeOffset = &(CRU->MODE_CON[0]),
    .modeShift = 0,
    .lockShift = 15,
    .modeMask = 0x3 << 0,
    .rateTable = PLL_TABLE,
};

static struct PLL_SETUP V0PLL =
{
    .conOffset0 = &(CRU->V0PLL_CON[0]),
    .conOffset1 = &(CRU->V0PLL_CON[1]),
    .conOffset2 = &(CRU->V0PLL_CON[2]),
    .conOffset3 = &(CRU->V0PLL_CON[3]),
    .conOffset6 = &(CRU->V0PLL_CON[6]),
    .modeOffset = &(CRU->MODE_CON[0]),
    .modeShift = 4,
    .lockShift = 15,
    .modeMask = 0x3 << 4,
    .rateTable = PLL_TABLE,
};

static struct PLL_SETUP AUPLL =
{
    .conOffset0 = &(CRU->AUPLL_CON[0]),
    .conOffset1 = &(CRU->AUPLL_CON[1]),
    .conOffset2 = &(CRU->AUPLL_CON[2]),
    .conOffset3 = &(CRU->AUPLL_CON[3]),
    .conOffset6 = &(CRU->AUPLL_CON[6]),
    .modeOffset = &(CRU->MODE_CON[0]),
    .modeShift = 6,
    .lockShift = 15,
    .modeMask = 0x3 << 6,
    .rateTable = PLL_TABLE,
};

static struct PLL_SETUP PPLL =
{
    .conOffset0 = &(PHPTOPCRU->PPLL_CON[0]),
    .conOffset1 = &(PHPTOPCRU->PPLL_CON[1]),
    .conOffset2 = &(PHPTOPCRU->PPLL_CON[2]),
    .conOffset3 = &(PHPTOPCRU->PPLL_CON[3]),
    .conOffset6 = &(PHPTOPCRU->PPLL_CON[6]),
    .lockShift = 15,
    .rateTable = PLL_TABLE,
};

/********************* Private Variable Definition ***************************/

static uint32_t s_lpllFreq;
static uint32_t s_cpllFreq;
static uint32_t s_gpllFreq;
static uint32_t s_npllFreq;
static uint32_t s_v0pllFreq;
static uint32_t s_ppllFreq;
static uint32_t s_aupllFreq;
static uint32_t s_spllFreq = 702 * 1000 * 1000;

static uint32_t cru_suspend;

/********************* Private Function Definition ***************************/

/** @} */
/********************* Public Function Definition ****************************/
static uint32_t HAL_CRU_ClkGetUartFreq(eCLOCK_Name clockName)
{
    uint32_t mux = CLK_GET_MUX(clockName);
    uint32_t pRate = 0, rate, n, m;
    uint32_t divSrc, divFrac;
    uint32_t muxSrc;

    switch (clockName) {
    case CLK_UART0:
        muxSrc = CLK_GET_MUX(CLK_UART0_SRC);
        divSrc = CLK_GET_DIV(CLK_UART0_SRC);
        divFrac = CLK_GET_DIV(CLK_UART0_FRAC);
        pRate = s_cpllFreq / HAL_CRU_ClkGetDiv(divSrc);
        break;

    case CLK_UART1:
        muxSrc = CLK_GET_MUX(CLK_UART1_SRC);
        divSrc = CLK_GET_DIV(CLK_UART1_SRC);
        divFrac = CLK_GET_DIV(CLK_UART1_FRAC);
        break;

    case CLK_UART2:
        muxSrc = CLK_GET_MUX(CLK_UART2_SRC);
        divSrc = CLK_GET_DIV(CLK_UART2_SRC);
        divFrac = CLK_GET_DIV(CLK_UART2_FRAC);
        break;

    case CLK_UART3:
        muxSrc = CLK_GET_MUX(CLK_UART3_SRC);
        divSrc = CLK_GET_DIV(CLK_UART3_SRC);
        divFrac = CLK_GET_DIV(CLK_UART3_FRAC);
        break;

    case CLK_UART4:
        muxSrc = CLK_GET_MUX(CLK_UART4_SRC);
        divSrc = CLK_GET_DIV(CLK_UART4_SRC);
        divFrac = CLK_GET_DIV(CLK_UART4_FRAC);
        break;

    case CLK_UART5:
        muxSrc = CLK_GET_MUX(CLK_UART5_SRC);
        divSrc = CLK_GET_DIV(CLK_UART5_SRC);
        divFrac = CLK_GET_DIV(CLK_UART5_FRAC);
        break;

    case CLK_UART6:
        muxSrc = CLK_GET_MUX(CLK_UART6_SRC);
        divSrc = CLK_GET_DIV(CLK_UART6_SRC);
        divFrac = CLK_GET_DIV(CLK_UART6_FRAC);
        break;

    case CLK_UART7:
        muxSrc = CLK_GET_MUX(CLK_UART7_SRC);
        divSrc = CLK_GET_DIV(CLK_UART7_SRC);
        divFrac = CLK_GET_DIV(CLK_UART7_FRAC);
        break;

    case CLK_UART8:
        muxSrc = CLK_GET_MUX(CLK_UART8_SRC);
        divSrc = CLK_GET_DIV(CLK_UART8_SRC);
        divFrac = CLK_GET_DIV(CLK_UART8_FRAC);
        break;

    case CLK_UART9:
        muxSrc = CLK_GET_MUX(CLK_UART9_SRC);
        divSrc = CLK_GET_DIV(CLK_UART9_SRC);
        divFrac = CLK_GET_DIV(CLK_UART9_FRAC);
        break;

    default:

        return HAL_INVAL;
    }

    /* attention: UART0 src is fixed at CPLL, 'pRate' is initialized */
    if (pRate == 0) {
        pRate = HAL_CRU_MuxGetFreq2(muxSrc,
                                    s_gpllFreq / HAL_CRU_ClkGetDiv(divSrc),
                                    s_cpllFreq / HAL_CRU_ClkGetDiv(divSrc));
    }

    HAL_CRU_ClkGetFracDiv(divFrac, &n, &m);

    rate = HAL_CRU_MuxGetFreq3(mux, pRate, (pRate / m) * n, PLL_INPUT_OSC_RATE);

    HAL_CRU_DBG("%s: (0x%08lX|0x%08lX) => (0x%08lX|0U): rate=%ld, mux=%ld, "
                "prate0=%ld, prate1=%ld, prate2=%d\n",
                __func__, muxSrc, divSrc, mux, rate, HAL_CRU_ClkGetMux(mux),
                pRate, pRate / m * n, PLL_INPUT_OSC_RATE);

    return rate;
}

static HAL_Status HAL_CRU_ClkSetUartFreq(eCLOCK_Name clockName, uint32_t rate)
{
    uint32_t muxSrc, mux = CLK_GET_MUX(clockName);
    uint32_t divSrc, divFrac;
    uint32_t gateId, fracGateId;
    uint32_t n = 0, m = 0, maxDiv;

    switch (clockName) {
    case CLK_UART0:
        muxSrc = 0;
        divSrc = CLK_GET_DIV(CLK_UART0_SRC);
        divFrac = CLK_GET_DIV(CLK_UART0_FRAC);
        gateId = CLK_UART0_GATE;
        fracGateId = CLK_UART0_FRAC_GATE;
        break;

    case CLK_UART1:
        muxSrc = CLK_GET_MUX(CLK_UART1_SRC);
        divSrc = CLK_GET_DIV(CLK_UART1_SRC);
        divFrac = CLK_GET_DIV(CLK_UART1_FRAC);
        gateId = CLK_UART1_GATE;
        fracGateId = CLK_UART1_FRAC_GATE;
        break;

    case CLK_UART2:
        muxSrc = CLK_GET_MUX(CLK_UART2_SRC);
        divSrc = CLK_GET_DIV(CLK_UART2_SRC);
        divFrac = CLK_GET_DIV(CLK_UART2_FRAC);
        gateId = CLK_UART2_GATE;
        fracGateId = CLK_UART2_FRAC_GATE;
        break;

    case CLK_UART3:
        muxSrc = CLK_GET_MUX(CLK_UART3_SRC);
        divSrc = CLK_GET_DIV(CLK_UART3_SRC);
        divFrac = CLK_GET_DIV(CLK_UART3_FRAC);
        gateId = CLK_UART3_GATE;
        fracGateId = CLK_UART3_FRAC_GATE;
        break;

    case CLK_UART4:
        muxSrc = CLK_GET_MUX(CLK_UART4_SRC);
        divSrc = CLK_GET_DIV(CLK_UART4_SRC);
        divFrac = CLK_GET_DIV(CLK_UART4_FRAC);
        gateId = CLK_UART4_GATE;
        fracGateId = CLK_UART4_FRAC_GATE;
        break;

    case CLK_UART5:
        muxSrc = CLK_GET_MUX(CLK_UART5_SRC);
        divSrc = CLK_GET_DIV(CLK_UART5_SRC);
        divFrac = CLK_GET_DIV(CLK_UART5_FRAC);
        gateId = CLK_UART5_GATE;
        fracGateId = CLK_UART5_FRAC_GATE;
        break;

    case CLK_UART6:
        muxSrc = CLK_GET_MUX(CLK_UART6_SRC);
        divSrc = CLK_GET_DIV(CLK_UART6_SRC);
        divFrac = CLK_GET_DIV(CLK_UART6_FRAC);
        gateId = CLK_UART6_GATE;
        fracGateId = CLK_UART6_FRAC_GATE;
        break;

    case CLK_UART7:
        muxSrc = CLK_GET_MUX(CLK_UART7_SRC);
        divSrc = CLK_GET_DIV(CLK_UART7_SRC);
        divFrac = CLK_GET_DIV(CLK_UART7_FRAC);
        gateId = CLK_UART7_GATE;
        fracGateId = CLK_UART7_FRAC_GATE;
        break;

    case CLK_UART8:
        muxSrc = CLK_GET_MUX(CLK_UART8_SRC);
        divSrc = CLK_GET_DIV(CLK_UART8_SRC);
        divFrac = CLK_GET_DIV(CLK_UART8_FRAC);
        gateId = CLK_UART8_GATE;
        fracGateId = CLK_UART8_FRAC_GATE;
        break;

    case CLK_UART9:
        muxSrc = CLK_GET_MUX(CLK_UART9_SRC);
        divSrc = CLK_GET_DIV(CLK_UART9_SRC);
        divFrac = CLK_GET_DIV(CLK_UART9_FRAC);
        gateId = CLK_UART9_GATE;
        fracGateId = CLK_UART9_FRAC_GATE;
        break;
    default:

        return HAL_INVAL;
    }

    maxDiv = CLK_DIV_GET_MAXDIV(divSrc);

    HAL_CRU_ClkEnable(gateId);
    HAL_CRU_ClkEnable(fracGateId);

    if (PLL_INPUT_OSC_RATE == rate) {
        HAL_CRU_ClkSetMux(mux, 2);
        HAL_CRU_ClkDisable(fracGateId);
        /* attention: UART0 is fixed at CPLL, 'muxSrc' is set as 0 */
    } else if (DIV_NO_REM(s_cpllFreq, rate, maxDiv) || !muxSrc) {
        HAL_CRU_ClkSetDiv(divSrc, HAL_DIV_ROUND_UP(s_cpllFreq, rate));
        if (muxSrc) {
            HAL_CRU_ClkSetMux(muxSrc, 1);
        }
        HAL_CRU_ClkSetMux(mux, 0);
        HAL_CRU_ClkDisable(fracGateId);
    } else if (DIV_NO_REM(s_gpllFreq, rate, maxDiv)) {
        HAL_CRU_ClkSetDiv(divSrc, s_gpllFreq / rate);
        HAL_CRU_ClkSetMux(muxSrc, 0);
        HAL_CRU_ClkSetMux(mux, 0);
        HAL_CRU_ClkDisable(fracGateId);
    } else {
        HAL_CRU_FracdivGetConfig(rate, s_gpllFreq, &n, &m);
        HAL_CRU_ClkSetDiv(divSrc, 1);
        HAL_CRU_ClkSetMux(muxSrc, 0);
        HAL_CRU_ClkSetFracDiv(divFrac, n, m);
        HAL_CRU_ClkSetMux(mux, 1);
    }

    HAL_CRU_DBG("%s: (0x%08lX|0x%08lX) => (0x%08lX|0U): "
                "rate=%ld, pRate=%ld, muxSrc=%ld, divSrc=%ld, mux=%ld, "
                "maxdiv=%ld, n/m=%ld/%ld\n",
                __func__, muxSrc, divSrc, mux,
                rate, m ? (rate * m / n) : (rate * HAL_CRU_ClkGetDiv(divSrc)),
                HAL_CRU_ClkGetMux(muxSrc), HAL_CRU_ClkGetDiv(divSrc),
                HAL_CRU_ClkGetMux(mux), maxDiv, n, m);

    return HAL_OK;
}

static uint32_t HAL_CRU_ClkGetAudioFreq(eCLOCK_Name clockName)
{
    uint32_t mux = CLK_GET_MUX(clockName);
    uint32_t divSrc, divFrac;
    uint32_t pRate, rate, n, m;
    uint32_t muxSrc;

    switch (clockName) {
    case CLK_I2S0_8CH_TX:
        muxSrc = CLK_GET_MUX(CLK_I2S0_8CH_TX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S0_8CH_TX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S0_8CH_TX_FRAC);
        break;

    case CLK_I2S0_8CH_RX:
        muxSrc = CLK_GET_MUX(CLK_I2S0_8CH_RX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S0_8CH_RX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S0_8CH_RX_FRAC);
        break;

    case CLK_I2S1_8CH_TX:
        muxSrc = CLK_GET_MUX(CLK_I2S1_8CH_TX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S1_8CH_TX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S1_8CH_TX_FRAC);
        break;

    case CLK_I2S1_8CH_RX:
        muxSrc = CLK_GET_MUX(CLK_I2S1_8CH_RX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S1_8CH_RX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S1_8CH_RX_FRAC);
        break;

    case CLK_I2S2_2CH:
        muxSrc = CLK_GET_MUX(CLK_I2S2_2CH_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S2_2CH_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S2_2CH_FRAC);
        break;

    case CLK_I2S3_2CH:
        muxSrc = CLK_GET_MUX(CLK_I2S3_2CH_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S3_2CH_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S3_2CH_FRAC);
        break;

    case CLK_I2S4_8CH_TX:
        muxSrc = CLK_GET_MUX(CLK_I2S4_8CH_TX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S4_8CH_TX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S4_8CH_TX_FRAC);
        break;

    case CLK_I2S5_8CH_TX:
        muxSrc = CLK_GET_MUX(CLK_I2S5_8CH_TX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S5_8CH_TX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S5_8CH_TX_FRAC);
        break;

    case CLK_I2S6_8CH_TX:
        muxSrc = CLK_GET_MUX(CLK_I2S6_8CH_TX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S6_8CH_TX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S6_8CH_TX_FRAC);
        break;

    case CLK_I2S6_8CH_RX:
        muxSrc = CLK_GET_MUX(CLK_I2S6_8CH_RX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S6_8CH_RX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S6_8CH_RX_FRAC);
        break;

    case CLK_I2S7_8CH_RX:
        muxSrc = CLK_GET_MUX(CLK_I2S7_8CH_RX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S7_8CH_RX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S7_8CH_RX_FRAC);
        break;

    case CLK_I2S8_8CH_TX:
        muxSrc = CLK_GET_MUX(CLK_I2S8_8CH_TX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S8_8CH_TX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S8_8CH_TX_FRAC);
        break;

    case CLK_I2S9_8CH_RX:
        muxSrc = CLK_GET_MUX(CLK_I2S9_8CH_RX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S9_8CH_RX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S9_8CH_RX_FRAC);
        break;

    case CLK_I2S10_8CH_RX:
        muxSrc = CLK_GET_MUX(CLK_I2S10_8CH_RX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S10_8CH_RX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S10_8CH_RX_FRAC);
        break;

    case CLK_SPDIF0:
        muxSrc = CLK_GET_MUX(CLK_SPDIF0_SRC);
        divSrc = CLK_GET_DIV(CLK_SPDIF0_SRC);
        divFrac = CLK_GET_DIV(CLK_SPDIF0_FRAC);
        break;

    case CLK_SPDIF1:
        muxSrc = CLK_GET_MUX(CLK_SPDIF1_SRC);
        divSrc = CLK_GET_DIV(CLK_SPDIF1_SRC);
        divFrac = CLK_GET_DIV(CLK_SPDIF1_FRAC);
        break;

    case CLK_SPDIF2_DP0:
        muxSrc = CLK_GET_MUX(CLK_SPDIF2_DP0_SRC);
        divSrc = CLK_GET_DIV(CLK_SPDIF2_DP0_SRC);
        divFrac = CLK_GET_DIV(CLK_SPDIF2_DP0_FRAC);
        break;

    case CLK_SPDIF3:
        muxSrc = CLK_GET_MUX(CLK_SPDIF3_SRC);
        divSrc = CLK_GET_DIV(CLK_SPDIF3_SRC);
        divFrac = CLK_GET_DIV(CLK_SPDIF3_FRAC);
        break;

    case CLK_SPDIF4:
        muxSrc = CLK_GET_MUX(CLK_SPDIF4_SRC);
        divSrc = CLK_GET_DIV(CLK_SPDIF4_SRC);
        divFrac = CLK_GET_DIV(CLK_SPDIF4_FRAC);
        break;

    case CLK_SPDIF5_DP1:
        muxSrc = CLK_GET_MUX(CLK_SPDIF5_DP1_SRC);
        divSrc = CLK_GET_DIV(CLK_SPDIF5_DP1_SRC);
        divFrac = CLK_GET_DIV(CLK_SPDIF5_DP1_FRAC);
        break;
    default:

        return HAL_INVAL;
    }

    pRate = HAL_CRU_MuxGetFreq2(muxSrc,
                                s_gpllFreq / HAL_CRU_ClkGetDiv(divSrc),
                                s_aupllFreq / HAL_CRU_ClkGetDiv(divSrc));
    HAL_CRU_ClkGetFracDiv(divFrac, &n, &m);
    rate = HAL_CRU_MuxGetFreq4(mux, pRate, pRate / m * n,
                               HAL_INVAL, PLL_INPUT_OSC_RATE / 2);

    HAL_CRU_DBG("%s: (0x%08lX|0x%08lX) => (0x%08lX|0U): rate=%ld, mux=%ld, "
                "prate0=%ld, prate1=%ld, prate2=%d, prate3=%d\n",
                __func__, muxSrc, divSrc, mux, rate, HAL_CRU_ClkGetMux(mux),
                pRate, pRate / m * n, HAL_INVAL, PLL_INPUT_OSC_RATE / 2);

    return rate;
}

static HAL_Status HAL_CRU_ClkSetAudioFreq(eCLOCK_Name clockName, uint32_t rate)
{
    uint32_t muxSrc, mux = CLK_GET_MUX(clockName);
    uint32_t divSrc, divFrac;
    uint32_t gateId, fracGateId;
    uint32_t n = 0, m = 0, maxDiv;

    switch (clockName) {
    case CLK_I2S0_8CH_TX:
        muxSrc = CLK_GET_MUX(CLK_I2S0_8CH_TX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S0_8CH_TX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S0_8CH_TX_FRAC);
        gateId = CLK_I2S0_8CH_TX_GATE;
        fracGateId = CLK_I2S0_8CH_FRAC_TX_GATE;
        break;

    case CLK_I2S0_8CH_RX:
        muxSrc = CLK_GET_MUX(CLK_I2S0_8CH_RX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S0_8CH_RX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S0_8CH_RX_FRAC);
        gateId = CLK_I2S0_8CH_RX_GATE;
        fracGateId = CLK_I2S0_8CH_FRAC_RX_GATE;
        break;

    case CLK_I2S1_8CH_TX:
        muxSrc = CLK_GET_MUX(CLK_I2S1_8CH_TX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S1_8CH_TX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S1_8CH_TX_FRAC);
        gateId = CLK_I2S1_8CH_TX_GATE;
        fracGateId = CLK_I2S1_8CH_FRAC_TX_GATE;
        break;

    case CLK_I2S1_8CH_RX:
        muxSrc = CLK_GET_MUX(CLK_I2S1_8CH_RX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S1_8CH_RX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S1_8CH_RX_FRAC);
        gateId = CLK_I2S1_8CH_RX_GATE;
        fracGateId = CLK_I2S1_8CH_FRAC_RX_GATE;
        break;

    case CLK_I2S2_2CH:
        muxSrc = CLK_GET_MUX(CLK_I2S2_2CH_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S2_2CH_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S2_2CH_FRAC);
        gateId = CLK_I2S2_2CH_GATE;
        fracGateId = CLK_I2S2_2CH_FRAC_GATE;
        break;

    case CLK_I2S3_2CH:
        muxSrc = CLK_GET_MUX(CLK_I2S3_2CH_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S3_2CH_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S3_2CH_FRAC);
        gateId = CLK_I2S3_2CH_GATE;
        fracGateId = CLK_I2S3_2CH_FRAC_GATE;
        break;

    case CLK_I2S4_8CH_TX:
        muxSrc = CLK_GET_MUX(CLK_I2S4_8CH_TX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S4_8CH_TX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S4_8CH_TX_FRAC);
        gateId = CLK_I2S4_8CH_TX_GATE;
        fracGateId = CLK_I2S4_8CH_FRAC_TX_GATE;
        break;

    case CLK_I2S5_8CH_TX:
        muxSrc = CLK_GET_MUX(CLK_I2S5_8CH_TX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S5_8CH_TX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S5_8CH_TX_FRAC);
        gateId = CLK_I2S5_8CH_TX_GATE;
        fracGateId = CLK_I2S5_8CH_FRAC_TX_GATE;
        break;

    case CLK_I2S6_8CH_TX:
        muxSrc = CLK_GET_MUX(CLK_I2S6_8CH_TX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S6_8CH_TX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S6_8CH_TX_FRAC);
        gateId = CLK_I2S6_8CH_TX_GATE;
        fracGateId = CLK_I2S6_8CH_FRAC_TX_GATE;
        break;

    case CLK_I2S6_8CH_RX:
        muxSrc = CLK_GET_MUX(CLK_I2S6_8CH_RX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S6_8CH_RX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S6_8CH_RX_FRAC);
        gateId = CLK_I2S6_8CH_RX_GATE;
        fracGateId = CLK_I2S6_8CH_FRAC_RX_GATE;
        break;

    case CLK_I2S7_8CH_RX:
        muxSrc = CLK_GET_MUX(CLK_I2S7_8CH_RX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S7_8CH_RX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S7_8CH_RX_FRAC);
        gateId = CLK_I2S7_8CH_RX_GATE;
        fracGateId = CLK_I2S7_8CH_FRAC_RX_GATE;
        break;

    case CLK_I2S8_8CH_TX:
        muxSrc = CLK_GET_MUX(CLK_I2S8_8CH_TX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S8_8CH_TX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S8_8CH_TX_FRAC);
        gateId = CLK_I2S8_8CH_TX_GATE;
        fracGateId = CLK_I2S8_8CH_FRAC_TX_GATE;
        break;

    case CLK_I2S9_8CH_RX:
        muxSrc = CLK_GET_MUX(CLK_I2S9_8CH_RX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S9_8CH_RX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S9_8CH_RX_FRAC);
        gateId = CLK_I2S9_8CH_RX_GATE;
        fracGateId = CLK_I2S9_8CH_FRAC_RX_GATE;
        break;

    case CLK_I2S10_8CH_RX:
        muxSrc = CLK_GET_MUX(CLK_I2S10_8CH_RX_SRC);
        divSrc = CLK_GET_DIV(CLK_I2S10_8CH_RX_SRC);
        divFrac = CLK_GET_DIV(CLK_I2S10_8CH_RX_FRAC);
        gateId = CLK_I2S10_8CH_RX_GATE;
        fracGateId = CLK_I2S10_8CH_FRAC_RX_GATE;
        break;

    case CLK_SPDIF0:
        muxSrc = CLK_GET_MUX(CLK_SPDIF0_SRC);
        divSrc = CLK_GET_DIV(CLK_SPDIF0_SRC);
        divFrac = CLK_GET_DIV(CLK_SPDIF0_FRAC);
        gateId = CLK_SPDIF0_GATE;
        fracGateId = CLK_SPDIF0_FRAC_GATE;
        break;

    case CLK_SPDIF1:
        muxSrc = CLK_GET_MUX(CLK_SPDIF1_SRC);
        divSrc = CLK_GET_DIV(CLK_SPDIF1_SRC);
        divFrac = CLK_GET_DIV(CLK_SPDIF1_FRAC);
        gateId = CLK_SPDIF1_GATE;
        fracGateId = CLK_SPDIF1_FRAC_GATE;
        break;

    case CLK_SPDIF2_DP0:
        muxSrc = CLK_GET_MUX(CLK_SPDIF2_DP0_SRC);
        divSrc = CLK_GET_DIV(CLK_SPDIF2_DP0_SRC);
        divFrac = CLK_GET_DIV(CLK_SPDIF2_DP0_FRAC);
        gateId = CLK_SPDIF2_DP0_GATE;
        fracGateId = CLK_SPDIF2_DP0_FRAC_GATE;
        break;

    case CLK_SPDIF3:
        muxSrc = CLK_GET_MUX(CLK_SPDIF3_SRC);
        divSrc = CLK_GET_DIV(CLK_SPDIF3_SRC);
        divFrac = CLK_GET_DIV(CLK_SPDIF3_FRAC);
        gateId = CLK_SPDIF3_GATE;
        fracGateId = CLK_SPDIF3_FRAC_GATE;
        break;

    case CLK_SPDIF4:
        muxSrc = CLK_GET_MUX(CLK_SPDIF4_SRC);
        divSrc = CLK_GET_DIV(CLK_SPDIF4_SRC);
        divFrac = CLK_GET_DIV(CLK_SPDIF4_FRAC);
        gateId = CLK_SPDIF4_GATE;
        fracGateId = CLK_SPDIF4_FRAC_GATE;
        break;

    case CLK_SPDIF5_DP1:
        muxSrc = CLK_GET_MUX(CLK_SPDIF5_DP1_SRC);
        divSrc = CLK_GET_DIV(CLK_SPDIF5_DP1_SRC);
        divFrac = CLK_GET_DIV(CLK_SPDIF5_DP1_FRAC);
        gateId = CLK_SPDIF5_DP1_GATE;
        fracGateId = CLK_SPDIF5_DP1_FRAC_GATE;
        break;

    default:

        return HAL_INVAL;
    }

    maxDiv = CLK_DIV_GET_MAXDIV(divSrc);

    HAL_CRU_ClkEnable(gateId);
    HAL_CRU_ClkEnable(fracGateId);

    if (PLL_INPUT_OSC_RATE / 2 == rate) {
        HAL_CRU_ClkSetMux(mux, 3);
        HAL_CRU_ClkDisable(fracGateId);
        /*
         * AUPLL is designed for audio, we will pre-calculate
         * a common rate and set it for the most audio requirement.
         * So AUPLL will not be changed like vop to change v0pll.
         */
    } else if (DIV_NO_REM(s_aupllFreq, rate, maxDiv)) {
        HAL_CRU_ClkSetDiv(divSrc, s_aupllFreq / rate);
        HAL_CRU_ClkSetMux(muxSrc, 1);
        HAL_CRU_ClkSetMux(mux, 0);
        HAL_CRU_ClkDisable(fracGateId);
    } else if (DIV_NO_REM(s_gpllFreq, rate, maxDiv)) {
        HAL_CRU_ClkSetDiv(divSrc, s_gpllFreq / rate);
        HAL_CRU_ClkSetMux(muxSrc, 0);
        HAL_CRU_ClkSetMux(mux, 0);
        HAL_CRU_ClkDisable(fracGateId);
    } else {
        HAL_CRU_FracdivGetConfig(rate, s_gpllFreq, &n, &m);
        HAL_CRU_ClkSetDiv(divSrc, 1);
        HAL_CRU_ClkSetMux(muxSrc, 0);
        HAL_CRU_ClkSetFracDiv(divFrac, n, m);
        HAL_CRU_ClkSetMux(mux, 1);
        HAL_CRU_ClkGetFracDiv(divFrac, &n, &m);
    }

    HAL_CRU_DBG("%s: (0x%08lX|0x%08lX) => (0x%08lX|0U): "
                "rate=%ld, pRate=%ld, muxSrc=%ld, divSrc=%ld, mux=%ld, "
                "maxdiv=%ld, n/m=%ld/%ld\n",
                __func__, muxSrc, divSrc, mux, rate,
                m ? (rate * m / n) : (rate * HAL_CRU_ClkGetDiv(divSrc)),
                HAL_CRU_ClkGetMux(muxSrc),
                HAL_CRU_ClkGetDiv(divSrc), HAL_CRU_ClkGetMux(mux), maxDiv, n, m);

    return HAL_OK;
}

static uint32_t HAL_CRU_ClkGetVopFreq(eCLOCK_Name clockName)
{
    uint32_t mux = CLK_GET_MUX(clockName);
    uint32_t muxSrc, divSrc;
    uint32_t freq = HAL_INVAL;
    uint32_t muxVal;

    switch (clockName) {
    case DCLK_VOP0:
        muxSrc = CLK_GET_MUX(DCLK_VOP0_SRC);
        divSrc = CLK_GET_DIV(DCLK_VOP0_SRC);
        muxVal = HAL_CRU_ClkGetMux(mux);
        break;

    case DCLK_VOP1:
        muxSrc = CLK_GET_MUX(DCLK_VOP1_SRC);
        divSrc = CLK_GET_DIV(DCLK_VOP1_SRC);
        muxVal = HAL_CRU_ClkGetMux(mux);
        break;

    case DCLK_VOP2:
        muxSrc = CLK_GET_MUX(DCLK_VOP2_SRC);
        divSrc = CLK_GET_DIV(DCLK_VOP2_SRC);
        muxVal = HAL_CRU_ClkGetMux(mux);
        break;

    case DCLK_VOP3:
        muxSrc = CLK_GET_MUX(DCLK_VOP3);
        divSrc = CLK_GET_DIV(DCLK_VOP3);
        muxVal = 0;
        break;

    default:

        return HAL_INVAL;
    }

    if (muxVal == 0 && muxSrc && divSrc) {
        freq = HAL_CRU_MuxGetFreq4(muxSrc, s_gpllFreq, s_cpllFreq, s_v0pllFreq, s_aupllFreq);
        freq /= HAL_CRU_ClkGetDiv(divSrc);
    }

    HAL_CRU_DBG("%s: (0x%08lX|0x%08lX) => (0x%08lX|0U): srcMux=%ld, mux=%ld, "
                "rate=%ld, div=%ld, prate0=%ld, prate1=%ld, prate2=%ld, prate3=%ld\n",
                __func__, muxSrc, divSrc, muxVal, HAL_CRU_ClkGetMux(muxSrc),
                HAL_CRU_ClkGetMux(mux), freq, HAL_CRU_ClkGetDiv(divSrc),
                s_gpllFreq, s_cpllFreq, s_v0pllFreq, s_aupllFreq);

    return freq;
}

static uint32_t HAL_CRU_ClkSetVopFreq(eCLOCK_Name clockName, uint32_t rate)
{
    /* vop2Pll[3] can be set as PLL_AUPLL, if the product doesn't use audio */
    uint32_t vop2Pll[] = { 0, 0, PLL_V0PLL, 0, };
    uint32_t mux = CLK_GET_MUX(clockName);
    uint32_t muxSrc, divSrc;
    uint32_t maxDiv, pllFreq;
    uint32_t *pllTable = NULL;
    uint32_t pllTableCnt = 0;
    uint32_t curPll, i;
    int best = -1;

    switch (clockName) {
    case DCLK_VOP0:
        muxSrc = CLK_GET_MUX(DCLK_VOP0_SRC);
        divSrc = CLK_GET_DIV(DCLK_VOP0_SRC);
        break;

    case DCLK_VOP1:
        muxSrc = CLK_GET_MUX(DCLK_VOP1_SRC);
        divSrc = CLK_GET_DIV(DCLK_VOP1_SRC);
        break;

    case DCLK_VOP2:
        muxSrc = CLK_GET_MUX(DCLK_VOP2_SRC);
        divSrc = CLK_GET_DIV(DCLK_VOP2_SRC);
        pllTable = vop2Pll;
        pllTableCnt = HAL_ARRAY_SIZE(vop2Pll);
        break;

    case DCLK_VOP3:
        muxSrc = CLK_GET_MUX(DCLK_VOP3);
        divSrc = CLK_GET_DIV(DCLK_VOP3);
        break;

    default:

        return HAL_INVAL;
    }

    maxDiv = CLK_DIV_GET_MAXDIV(divSrc);

    if (DIV_NO_REM(s_v0pllFreq, rate, maxDiv)) {
        HAL_CRU_ClkSetDiv(divSrc, s_v0pllFreq / rate);
        HAL_CRU_ClkSetMux(muxSrc, 2);
        if (muxSrc != mux) {
            HAL_CRU_ClkSetMux(mux, 0);
        }
    } else if (DIV_NO_REM(s_gpllFreq, rate, maxDiv)) {
        HAL_CRU_ClkSetDiv(divSrc, s_gpllFreq / rate);
        HAL_CRU_ClkSetMux(muxSrc, 0);
        if (muxSrc != mux) {
            HAL_CRU_ClkSetMux(mux, 0);
        }
    } else if (DIV_NO_REM(s_cpllFreq, rate, maxDiv)) {
        HAL_CRU_ClkSetDiv(divSrc, s_cpllFreq / rate);
        HAL_CRU_ClkSetMux(muxSrc, 1);
        if (muxSrc != mux) {
            HAL_CRU_ClkSetMux(mux, 0);
        }
    } else if (DIV_NO_REM(s_aupllFreq, rate, maxDiv)) {
        HAL_CRU_ClkSetDiv(divSrc, s_aupllFreq / rate);
        HAL_CRU_ClkSetMux(muxSrc, 3);
        if (muxSrc != mux) {
            HAL_CRU_ClkSetMux(mux, 0);
        }
    } else {
        curPll = HAL_CRU_ClkGetMux(muxSrc);
        for (i = 0; i < pllTableCnt; i++) {
            if (pllTable[i]) {
                best = i;
                if (pllTable[i] == curPll) {
                    break;
                }
            }
        }

        /* No PLL reserved for vop ? */
        if (best < 0) {
            best = 0;
            pllFreq = s_gpllFreq;
        } else {
            HAL_CRU_ClkSetFreq(pllTable[best], rate);
            pllFreq = HAL_CRU_ClkGetFreq(pllTable[best]);
        }

        HAL_CRU_ClkSetDiv(divSrc, HAL_DIV_ROUND_UP(pllFreq, rate));
        HAL_CRU_ClkSetMux(muxSrc, best);
        if (muxSrc != mux) {
            HAL_CRU_ClkSetMux(mux, 0);
        }
    }

    HAL_CRU_DBG("%s: (0x%08lX|0x%08lX) => (0x%08lX|0U): best=%d, "
                "rate=%ld, pRate=%ld, muxSrc=%ld, divSrc=%ld, mux=%ld, maxdiv=%ld\n",
                __func__, muxSrc, divSrc, mux, best, rate,
                rate * HAL_CRU_ClkGetDiv(divSrc), HAL_CRU_ClkGetMux(muxSrc),
                HAL_CRU_ClkGetDiv(divSrc), HAL_CRU_ClkGetMux(mux), maxDiv);

    return HAL_OK;
}

static uint32_t HAL_CRU_ClkGetOtherFreq(eCLOCK_Name clockName)
{
    switch (clockName) {
    case CLK_I2S0_8CH_TX:
    case CLK_I2S0_8CH_RX:
    case CLK_I2S1_8CH_TX:
    case CLK_I2S1_8CH_RX:
    case CLK_I2S2_2CH:
    case CLK_I2S3_2CH:
    case CLK_I2S4_8CH_TX:
    case CLK_I2S5_8CH_TX:
    case CLK_I2S6_8CH_TX:
    case CLK_I2S6_8CH_RX:
    case CLK_I2S7_8CH_RX:
    case CLK_I2S8_8CH_TX:
    case CLK_I2S9_8CH_RX:
    case CLK_I2S10_8CH_RX:
    case CLK_SPDIF0:
    case CLK_SPDIF1:
    case CLK_SPDIF2_DP0:
    case CLK_SPDIF3:
    case CLK_SPDIF4:
    case CLK_SPDIF5_DP1:

        return HAL_CRU_ClkGetAudioFreq(clockName);

    case CLK_UART0:
    case CLK_UART1:
    case CLK_UART2:
    case CLK_UART3:
    case CLK_UART4:
    case CLK_UART5:
    case CLK_UART6:
    case CLK_UART7:
    case CLK_UART8:
    case CLK_UART9:

        return HAL_CRU_ClkGetUartFreq(clockName);

    case DCLK_VOP0:
    case DCLK_VOP1:
    case DCLK_VOP2:
    case DCLK_VOP3:

        return HAL_CRU_ClkGetVopFreq(clockName);

    default:
        break;
    }

    return HAL_INVAL;
}

static HAL_Status HAL_CRU_ClkSetOtherFreq(eCLOCK_Name clockName, uint32_t rate)
{
    switch (clockName) {
    case CLK_I2S0_8CH_TX:
    case CLK_I2S0_8CH_RX:
    case CLK_I2S1_8CH_TX:
    case CLK_I2S1_8CH_RX:
    case CLK_I2S2_2CH:
    case CLK_I2S3_2CH:
    case CLK_I2S4_8CH_TX:
    case CLK_I2S5_8CH_TX:
    case CLK_I2S6_8CH_TX:
    case CLK_I2S6_8CH_RX:
    case CLK_I2S7_8CH_RX:
    case CLK_I2S8_8CH_TX:
    case CLK_I2S9_8CH_RX:
    case CLK_I2S10_8CH_RX:
    case CLK_SPDIF0:
    case CLK_SPDIF1:
    case CLK_SPDIF2_DP0:
    case CLK_SPDIF3:
    case CLK_SPDIF4:
    case CLK_SPDIF5_DP1:

        return HAL_CRU_ClkSetAudioFreq(clockName, rate);

    case CLK_UART0:
    case CLK_UART1:
    case CLK_UART2:
    case CLK_UART3:
    case CLK_UART4:
    case CLK_UART5:
    case CLK_UART6:
    case CLK_UART7:
    case CLK_UART8:
    case CLK_UART9:

        return HAL_CRU_ClkSetUartFreq(clockName, rate);

    case DCLK_VOP0:
    case DCLK_VOP1:
    case DCLK_VOP2:
    case DCLK_VOP3:

        return HAL_CRU_ClkSetVopFreq(clockName, rate);

    default:
        break;
    }

    return HAL_INVAL;
}

static void CRU_InitPlls(void)
{
    s_cpllFreq = HAL_CRU_GetPllFreq(&CPLL);
    s_gpllFreq = HAL_CRU_GetPllFreq(&GPLL);
    s_v0pllFreq = HAL_CRU_GetPllFreq(&V0PLL);
    s_aupllFreq = HAL_CRU_GetPllFreq(&AUPLL);
    s_ppllFreq = HAL_CRU_GetPllFreq(&PPLL);
    s_lpllFreq = HAL_CRU_GetPllFreq(&LPLL);
    s_npllFreq = HAL_CRU_GetPllFreq(&NPLL);

    HAL_CRU_DBG("%s: cpll=%ld, gpll=%ld, v0pll=%ld, aupll=%ld, ppll=%ld, lpll=%ld, npll=%ld\n",
                __func__, s_cpllFreq, s_gpllFreq, s_v0pllFreq, s_aupllFreq, s_ppllFreq, s_lpllFreq, s_npllFreq);
}

uint32_t HAL_CRU_ClkGetFreq(eCLOCK_Name clockName)
{
    uint32_t clkMux = CLK_GET_MUX(clockName);
    uint32_t clkDiv = CLK_GET_DIV(clockName);
    uint32_t freq = 0;

    HAL_CRU_DBG("%s: (0x%08lX|0x%08lX)\n", __func__, clkMux, clkDiv);
    HAL_CRU_DBG("%s: cpll=%ld, gpll=%ld, v0pll=%ld, aupll=%ld, ppll=%ld\n",
                __func__, s_cpllFreq, s_gpllFreq, s_v0pllFreq, s_aupllFreq, s_ppllFreq);

    if (cru_suspend) {
        return PLL_INPUT_OSC_RATE;
    }

    if (!s_cpllFreq) {
        CRU_InitPlls();
    }

    switch (clockName) {
    case PLL_LPLL:
    case ARMCLK_L:
        freq = HAL_CRU_GetPllFreq(&LPLL);
        s_lpllFreq = freq;

        return freq;

    case PLL_B0PLL:
    case ARMCLK_B01:
        freq = HAL_CRU_GetPllFreq(&B0PLL);

        return freq;

    case PLL_B1PLL:
    case ARMCLK_B23:
        freq = HAL_CRU_GetPllFreq(&B1PLL);

        return freq;

    case PLL_CPLL:
        freq = HAL_CRU_GetPllFreq(&CPLL);
        s_cpllFreq = freq;

        return freq;

    case PLL_NPLL:
        freq = HAL_CRU_GetPllFreq(&NPLL);
        s_npllFreq = freq;

        return freq;

    case PLL_V0PLL:
        freq = HAL_CRU_GetPllFreq(&V0PLL);
        s_v0pllFreq = freq;

        return freq;

    case PLL_AUPLL:
        freq = HAL_CRU_GetPllFreq(&AUPLL);
        s_aupllFreq = freq;

        return freq;

    case PLL_PPLL:
        freq = HAL_CRU_GetPllFreq(&PPLL);
        s_ppllFreq = freq;

        return freq;

    case PLL_GPLL:
        freq = HAL_CRU_GetPllFreq(&GPLL);
        s_gpllFreq = freq;

        return freq;

    case CCLK_EMMC:
    case SCLK_SFC:
    case CCLK_SRC_SDIO:
        freq = HAL_CRU_MuxGetFreq3(clkMux, s_gpllFreq,
                                   s_cpllFreq, PLL_INPUT_OSC_RATE);
        break;

    case BCLK_EMMC:
        freq = HAL_CRU_MuxGetFreq2(clkMux, s_gpllFreq, s_cpllFreq);
        break;

    case CLK_REF_PIPE_PHY0:
    case CLK_REF_PIPE_PHY1:
    case CLK_REF_PIPE_PHY2:
        freq = HAL_CRU_MuxGetFreq2(clkMux, PLL_INPUT_OSC_RATE, s_ppllFreq);
        if (freq == PLL_INPUT_OSC_RATE) {
            clkDiv = 0;
        }
        break;

    case HCLK_VAD:
        freq = HAL_CRU_MuxGetFreq4(clkMux, _MHZ(200), _MHZ(100), _MHZ(50), PLL_INPUT_OSC_RATE);
        break;

    case MCLK_PDM0:
        freq = HAL_CRU_MuxGetFreq2(clkMux, _MHZ(300), _MHZ(200));
        break;

    case HCLK_PMU_CM0:
        freq = HAL_CRU_MuxGetFreq4(clkMux, _MHZ(400), _MHZ(200), _MHZ(100), PLL_INPUT_OSC_RATE);
        break;

    case CLK_SPI0:
    case CLK_SPI1:
    case CLK_SPI2:
    case CLK_SPI3:
    case CLK_SPI4:
        freq = HAL_CRU_MuxGetFreq3(clkMux, _MHZ(200), _MHZ(150), PLL_INPUT_OSC_RATE);
        break;

    case CLK_PWM1:
    case CLK_PWM2:
    case CLK_PWM3:
        freq = HAL_CRU_MuxGetFreq3(clkMux, _MHZ(100), _MHZ(50), PLL_INPUT_OSC_RATE);
        break;

    case CLK_I2C0:
    case CLK_I2C1:
    case CLK_I2C2:
    case CLK_I2C3:
    case CLK_I2C4:
    case CLK_I2C5:
    case CLK_I2C6:
    case CLK_I2C7:
    case CLK_I2C8:
        freq = HAL_CRU_MuxGetFreq2(clkMux, _MHZ(200), _MHZ(100));
        break;

    case CLK_GMAC_125M:
    case CLK_GMAC_50M:
    case REFCLKO25M_ETH0_OUT:
    case REFCLKO25M_ETH1_OUT:
    case DCLK_VICAP:
        freq = HAL_CRU_MuxGetFreq2(clkMux, s_gpllFreq, s_cpllFreq);
        break;

    case CLK_MIPI_CAMOUT0:
    case CLK_MIPI_CAMOUT1:
    case CLK_MIPI_CAMOUT2:
    case CLK_MIPI_CAMOUT3:
    case CLK_MIPI_CAMOUT4:
        freq = HAL_CRU_MuxGetFreq4(clkMux, PLL_INPUT_OSC_RATE, s_spllFreq, s_gpllFreq, s_cpllFreq);
        break;

    case CLK_CIFOUT:
        freq = HAL_CRU_MuxGetFreq4(clkMux, s_gpllFreq, s_cpllFreq, PLL_INPUT_OSC_RATE, s_spllFreq);
        break;
    case CLK_AUX16M_0:
    case CLK_AUX16M_1:
        freq = s_gpllFreq;
        break;

    default:

        return HAL_CRU_ClkGetOtherFreq(clockName);
    }

    if (!clkMux && !clkDiv) {
        return 0;
    }
    if (clkDiv) {
        freq /= (HAL_CRU_ClkGetDiv(clkDiv));
    }

    HAL_CRU_DBG("%s: (0x%08lX|0x%08lX): freq: %ld=%ld/%ld\n",
                __func__, clkMux, clkDiv, freq, freq * HAL_CRU_ClkGetDiv(clkDiv),
                HAL_CRU_ClkGetDiv(clkDiv));

    return freq;
}

HAL_Status HAL_CRU_ClkSetFreq(eCLOCK_Name clockName, uint32_t rate)
{
    HAL_Status error = HAL_OK;
    uint32_t clkMux = CLK_GET_MUX(clockName);
    uint32_t clkDiv = CLK_GET_DIV(clockName);
    uint32_t mux = 0, div = 1, pRate = 0;

    HAL_CRU_DBG("%s: (0x%08lX|0x%08lX): rate=%ld\n", __func__, clkMux, clkDiv, rate);
    HAL_CRU_DBG("%s: cpll=%ld, gpll=%ld, v0pll=%ld, aupll=%ld, ppll=%ld\n",
                __func__, s_cpllFreq, s_gpllFreq, s_v0pllFreq, s_aupllFreq, s_ppllFreq);

    if (cru_suspend) {
        return HAL_OK;
    }

    if (!s_cpllFreq) {
        CRU_InitPlls();
    }

    switch (clockName) {
    case PLL_LPLL:
        error = HAL_CRU_SetPllFreq(&LPLL, rate);
        s_lpllFreq = HAL_CRU_GetPllFreq(&LPLL);

        return error;

    case PLL_B0PLL:
        error = HAL_CRU_SetPllFreq(&B0PLL, rate);

        return error;

    case PLL_B1PLL:
        error = HAL_CRU_SetPllFreq(&B1PLL, rate);

        return error;

    case PLL_CPLL:
        error = HAL_CRU_SetPllFreq(&CPLL, rate);
        s_cpllFreq = HAL_CRU_GetPllFreq(&CPLL);

        return error;

    case PLL_PPLL:
        error = HAL_CRU_SetPllFreq(&PPLL, rate);
        s_ppllFreq = HAL_CRU_GetPllFreq(&PPLL);

        return error;

    case PLL_GPLL:
        error = HAL_CRU_SetPllFreq(&GPLL, rate);
        s_gpllFreq = HAL_CRU_GetPllFreq(&GPLL);

        return error;

    case PLL_NPLL:
        error = HAL_CRU_SetPllFreq(&NPLL, rate);
        s_npllFreq = HAL_CRU_GetPllFreq(&NPLL);

        return error;

    case PLL_AUPLL:
        error = HAL_CRU_SetPllFreq(&AUPLL, rate);
        s_aupllFreq = HAL_CRU_GetPllFreq(&AUPLL);

        return error;

    case PLL_V0PLL:
        error = HAL_CRU_SetPllFreq(&V0PLL, rate);
        s_v0pllFreq = HAL_CRU_GetPllFreq(&V0PLL);

        return error;

    case ARMCLK_L:
        HAL_CRU_ClkSetMux(CLK_CORE_L_SLOW_SRC_SEL, CLK_CORE_L_SLOW_SRC_SEL_XIN_OSC0_FUNC);
        HAL_CRU_ClkSetMux(CLK_CORE_L_SRC_SEL, CLK_CORE_L_SRC_SEL_CLK_CORE_L_GPLL_SRC);
        HAL_CRU_ClkSetMux(SCLK_DSU_DF_SRC_SEL, SCLK_DSU_DF_SRC_SEL_CLK_GPLL_MUX);
        error = HAL_CRU_SetPllFreq(&LPLL, rate);
        s_lpllFreq = HAL_CRU_GetPllFreq(&LPLL);
        HAL_CRU_ClkSetMux(SCLK_DSU_DF_SRC_SEL, SCLK_DSU_DF_SRC_SEL_CLK_LPLL_MUX);
        HAL_CRU_ClkSetMux(CLK_CORE_L0_SEL, CLK_CORE_L0_SEL_CLK_CORE_L0_CLEAN);
        HAL_CRU_ClkSetMux(CLK_CORE_L1_SEL, CLK_CORE_L1_SEL_CLK_CORE_L1_CLEAN);
        HAL_CRU_ClkSetMux(CLK_CORE_L_SRC_SEL, CLK_CORE_L_SRC_SEL_CLK_LPLL);

        return error;

    case ARMCLK_B01:
        HAL_CRU_ClkSetMux(CLK_CORE_B01_SLOW_SRC_SEL, CLK_CORE_B01_SLOW_SRC_SEL_XIN_OSC0_FUNC);
        HAL_CRU_ClkSetMux(CLK_CORE_B01_SRC_SEL, CLK_CORE_B01_SRC_SEL_CLK_CORE_B01_GPLL_SRC);
        error = HAL_CRU_SetPllFreq(&B0PLL, rate);
        HAL_CRU_ClkSetMux(CLK_CORE_B0_SEL, CLK_CORE_B0_SEL_CLK_CORE_B0_CLEAN);
        HAL_CRU_ClkSetMux(CLK_CORE_B1_SEL, CLK_CORE_B1_SEL_CLK_CORE_B1_CLEAN);
        HAL_CRU_ClkSetMux(CLK_CORE_B01_SRC_SEL, CLK_CORE_B01_SRC_SEL_CLK_B0PLL);

        return error;

    case ARMCLK_B23:
        HAL_CRU_ClkSetMux(CLK_CORE_B23_SLOW_SRC_SEL, CLK_CORE_B23_SLOW_SRC_SEL_XIN_OSC0_FUNC);
        HAL_CRU_ClkSetMux(CLK_CORE_B23_SRC_SEL, CLK_CORE_B23_SRC_SEL_CLK_CORE_B23_GPLL_SRC);
        error = HAL_CRU_SetPllFreq(&B1PLL, rate);
        HAL_CRU_ClkSetMux(CLK_CORE_B2_SEL, CLK_CORE_B2_SEL_CLK_CORE_B2_CLEAN);
        HAL_CRU_ClkSetMux(CLK_CORE_B3_SEL, CLK_CORE_B3_SEL_CLK_CORE_B3_CLEAN);
        HAL_CRU_ClkSetMux(CLK_CORE_B23_SRC_SEL, CLK_CORE_B23_SRC_SEL_CLK_B1PLL);

        return error;

    case SCLK_DSU:
        HAL_CRU_ClkSetMux(SCLK_DSU_DF_SRC_SEL, SCLK_DSU_DF_SRC_SEL_CLK_GPLL_MUX);
        error = HAL_CRU_SetPllFreq(&LPLL, rate);
        s_lpllFreq = HAL_CRU_GetPllFreq(&LPLL);
        HAL_CRU_ClkSetMux(SCLK_DSU_DF_SRC_SEL, SCLK_DSU_DF_SRC_SEL_CLK_LPLL_MUX);
        HAL_CRU_ClkSetMux(SCLK_DSU_SRC_T_SEL, SCLK_DSU_SRC_T_SEL_SCLK_DSU_SRC);

        return error;

    case CLK_REF_PIPE_PHY0:
    case CLK_REF_PIPE_PHY1:
    case CLK_REF_PIPE_PHY2:
        if (rate == PLL_INPUT_OSC_RATE) {
            mux = 0;
            pRate = PLL_INPUT_OSC_RATE;
        } else {
            mux = 1;
            pRate = s_ppllFreq;
        }
        break;

    case HCLK_VAD:
        mux = HAL_CRU_FreqGetMux4(rate, _MHZ(200), _MHZ(100), _MHZ(50), PLL_INPUT_OSC_RATE);
        break;

    case MCLK_PDM0:
        mux = HAL_CRU_FreqGetMux2(rate, _MHZ(300), _MHZ(200));
        break;

    case HCLK_PMU_CM0:
        mux = HAL_CRU_FreqGetMux4(rate, _MHZ(400), _MHZ(200), _MHZ(100), PLL_INPUT_OSC_RATE);
        break;

    case CCLK_EMMC:
    case SCLK_SFC:
    case CCLK_SRC_SDIO:
        mux = HAL_CRU_RoundFreqGetMux3(rate, s_gpllFreq, s_cpllFreq, PLL_INPUT_OSC_RATE, &pRate);
        break;

    case BCLK_EMMC:
        mux = HAL_CRU_RoundFreqGetMux2(rate, s_gpllFreq, s_cpllFreq, &pRate);
        break;

    case CLK_SPI0:
    case CLK_SPI1:
    case CLK_SPI2:
    case CLK_SPI3:
    case CLK_SPI4:
        mux = HAL_CRU_FreqGetMux3(rate, _MHZ(200), _MHZ(150), PLL_INPUT_OSC_RATE);
        break;

    case CLK_PWM1:
    case CLK_PWM2:
    case CLK_PWM3:
        mux = HAL_CRU_FreqGetMux3(rate, _MHZ(100), _MHZ(50), PLL_INPUT_OSC_RATE);
        break;

    case CLK_I2C0:
    case CLK_I2C1:
    case CLK_I2C2:
    case CLK_I2C3:
    case CLK_I2C4:
    case CLK_I2C5:
    case CLK_I2C6:
    case CLK_I2C7:
    case CLK_I2C8:
        mux = HAL_CRU_FreqGetMux2(rate, _MHZ(200), _MHZ(100));
        break;

    case CLK_GMAC_125M:
    case CLK_GMAC_50M:
    case REFCLKO25M_ETH0_OUT:
    case REFCLKO25M_ETH1_OUT:
    case DCLK_VICAP:
        mux = HAL_CRU_RoundFreqGetMux2(rate, s_gpllFreq, s_cpllFreq, &pRate);
        break;

    case CLK_MIPI_CAMOUT0:
    case CLK_MIPI_CAMOUT1:
    case CLK_MIPI_CAMOUT2:
    case CLK_MIPI_CAMOUT3:
    case CLK_MIPI_CAMOUT4:
        mux = HAL_CRU_RoundFreqGetMux4(rate, PLL_INPUT_OSC_RATE, s_spllFreq, s_gpllFreq, s_cpllFreq, &pRate);
        break;

    case CLK_CIFOUT:
        mux = HAL_CRU_RoundFreqGetMux4(rate, s_gpllFreq, s_cpllFreq, PLL_INPUT_OSC_RATE, s_spllFreq, &pRate);
        break;

    case CLK_AUX16M_0:
    case CLK_AUX16M_1:
        pRate = s_gpllFreq;
        break;
    default:

        return HAL_CRU_ClkSetOtherFreq(clockName, rate);
    }

    if (!clkMux && !clkDiv) {
        return HAL_INVAL;
    }

    if (pRate) {
        div = HAL_DIV_ROUND_UP(pRate, rate);
    }
    if (clkDiv) {
        div = (div > CLK_DIV_GET_MAXDIV(clkDiv) ?
               CLK_DIV_GET_MAXDIV(clkDiv) : (div));
        HAL_CRU_ClkSetDiv(clkDiv, div);
    }
    if (clkMux) {
        HAL_CRU_ClkSetMux(clkMux, mux);
    }

    HAL_CRU_DBG("%s: (0x%08lX|0x%08lX): mux=%ld, rate=%ld, pRate=%ld, div=%ld, maxdiv=%d\n",
                __func__, clkMux, clkDiv, mux, rate, pRate, div, CLK_DIV_GET_MAXDIV(clkDiv));

    return HAL_OK;
}

static uint32_t clk_matrix_300m_src_div;
static uint32_t clk_matrix_pmu1_300m_src_div;
static uint32_t clk_matrix_400m_src_div;
static uint32_t clk_matrix_pmu1_400m_src_div;
static uint32_t clk_matrix_pmu1_200m_src_div;
static uint32_t hclk_pmu1_root_i_sel;

HAL_Status HAL_CRU_Suspend(void)
{
    /*
     * After ATF system suspend, the system clock rate is 24M.
     * Set divider 1:1 to be 24M rate for some modules.
     */
    clk_matrix_300m_src_div = HAL_CRU_ClkGetDiv(CLK_MATRIX_300M_SRC_DIV);
    clk_matrix_pmu1_300m_src_div = HAL_CRU_ClkGetDiv(CLK_MATRIX_PMU1_300M_SRC_DIV);
    clk_matrix_400m_src_div = HAL_CRU_ClkGetDiv(CLK_MATRIX_400M_SRC_DIV);
    clk_matrix_pmu1_400m_src_div = HAL_CRU_ClkGetDiv(CLK_MATRIX_PMU1_400M_SRC_DIV);
    clk_matrix_pmu1_200m_src_div = HAL_CRU_ClkGetDiv(CLK_MATRIX_PMU1_200M_SRC_DIV);
    hclk_pmu1_root_i_sel = HAL_CRU_ClkGetMux(HCLK_PMU1_ROOT_I_SEL);

    /* clk_mclk_pdm0 div 1:1 */
    HAL_CRU_ClkSetDiv(CLK_MATRIX_300M_SRC_DIV, 1);
    HAL_CRU_ClkSetDiv(CLK_MATRIX_PMU1_300M_SRC_DIV, 1);
    HAL_CRU_ClkSetDiv(CLK_MATRIX_400M_SRC_DIV, 1);
    HAL_CRU_ClkSetDiv(CLK_MATRIX_PMU1_400M_SRC_DIV, 1);
    HAL_CRU_ClkSetDiv(CLK_MATRIX_PMU1_200M_SRC_DIV, 1);
    /* hclk_vad div 1:1 */
    HAL_CRU_ClkSetMux(HCLK_PMU1_ROOT_I_SEL, 3);

    cru_suspend = 1;

    return HAL_OK;
}

HAL_Status HAL_CRU_Resume(void)
{
    /* clk_mclk_pdm0 */
    HAL_CRU_ClkSetDiv(CLK_MATRIX_300M_SRC_DIV, clk_matrix_300m_src_div);
    HAL_CRU_ClkSetDiv(CLK_MATRIX_PMU1_300M_SRC_DIV, clk_matrix_pmu1_300m_src_div);
    HAL_CRU_ClkSetDiv(CLK_MATRIX_400M_SRC_DIV, clk_matrix_400m_src_div);
    HAL_CRU_ClkSetDiv(CLK_MATRIX_PMU1_400M_SRC_DIV, clk_matrix_pmu1_400m_src_div);
    HAL_CRU_ClkSetDiv(CLK_MATRIX_PMU1_200M_SRC_DIV, clk_matrix_pmu1_200m_src_div);
    /* hclk_vad */
    HAL_CRU_ClkSetMux(HCLK_PMU1_ROOT_I_SEL, hclk_pmu1_root_i_sel);

    cru_suspend = 0;

    return HAL_OK;
}

/** @} */

/** @} */

/** @} */

#endif /* SOC_RK3588 && HAL_CRU_MODULE_ENABLED */
