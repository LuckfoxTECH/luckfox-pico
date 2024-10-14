/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_WDT_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup WDT
 *  @{
 */

/** @defgroup WDT_How_To_Use How To Use
 *  @{

 The WDT driver can be used as follows:

 @} */

/** @defgroup WDT_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/

#if 0
#define WDT_DYNCLK_DBG(fmt, ...) HAL_DBG(fmt, ##__VA_ARGS__)
#else
#define WDT_DYNCLK_DBG(fmt, ...)
#endif

#define WDOG_COUNTER_RESTART_KICK_VALUE 0x76

/* The maximum TOP (timeout period) value that can be set in the watchdog. */

#define WDT_MAX_TOP 15
#define WDT_TIMEOUT 500 /* ms */

#define HAL_TICK_TO_SEC(tick) ((tick) / 1000)
#define SEC_TO_HAL_TICK(sec)  ((sec) * 1000)
#define WDT_DYNFREQ_MARGIN_MS (100)
/********************* Private Structure Definition **************************/

struct DW_WDT {
    uint32_t freq;
};

#ifdef HAL_WDT_DYNFREQ_FEATURE_ENABLED
struct DW_WDT_DYNFREQ {
    uint32_t targetTick;
    uint32_t targetTickMargin;
    uint32_t limitVal;
    uint32_t topSec;
};
#endif
/********************* Private Variable Definition ***************************/

static struct DW_WDT dwWdt;
static struct WDT_REG *pWDT = NULL;

#ifdef HAL_WDT_DYNFREQ_FEATURE_ENABLED
static struct DW_WDT_DYNFREQ wdtDynFreq;
#endif
/********************* Private Function Definition ***************************/

__STATIC_INLINE uint32_t WDT_TopInSeconds(uint32_t top)
{
    /*
     * There are 16 possible timeout values in 0..15 where the number of
     * cycles is 2 ^ (16 + i) and the watchdog counts down.
     */
    uint32_t cycles = 1 << (16 + top);

    return cycles / dwWdt.freq;
}

static void WDT_SetTop(uint32_t top_s)
{
    uint32_t i, top_Val = WDT_MAX_TOP;

    /*
     * Iterate over the timeout values until we find the closest match. We
     * always look for >=.
     */
    for (i = 0; i <= WDT_MAX_TOP; ++i) {
        if (WDT_TopInSeconds(i) >= top_s) {
            top_Val = i;
            break;
        }
    }
    /*
     * Set the new value in the watchdog.  Some versions of dwWdt
     * have have TOPINIT in the TIMEOUT_RANGE register (as per
     * CP_WDT_DUAL_TOP in WDT_COMP_PARAMS_1).  On those we
     * effectively get a pat of the watchdog right here.
     */
    pWDT->TORR = top_Val | top_Val << WDT_TORR_TIMEOUT_PERIOD_SHIFT;
}

static uint32_t WDT_getTopMsecVal(uint32_t topMsec, uint32_t freq)
{
    uint32_t cycles, topVal;

    cycles = topMsec * (freq / 1000);

    if (cycles < 0xffff) {
        topVal = 0;
    } else {
        topVal = 16 - __CLZ(cycles);
    }

    HAL_ASSERT(topVal <= WDT_MAX_TOP);

    WDT_DYNCLK_DBG("  WDT_getTopMsecVal %u %u; 0x%x; %u,%u\n",
                   topMsec, (freq / 1000), cycles, 32 - __CLZ(cycles), topVal);

    return topVal;
}

#ifdef HAL_WDT_DYNFREQ_FEATURE_ENABLED
static void WDT_dynFreqSetTargetTick(void)
{
    wdtDynFreq.targetTick = HAL_GetTick() + SEC_TO_HAL_TICK(wdtDynFreq.topSec);
    wdtDynFreq.targetTickMargin = wdtDynFreq.targetTick - WDT_DYNFREQ_MARGIN_MS;

    WDT_DYNCLK_DBG("HAL_WDT_KeepAlive %u+%u=%u\n",
                   HAL_GetTick(), SEC_TO_HAL_TICK(wdtDynFreq.topSec),
                   wdtDynFreq.targetTick);
}
#endif

/** @} */
/********************* Public Function Definition ****************************/

/** @defgroup WDT_Exported_Functions_Group4 Init and DeInit

  This section provides functions allowing to init and deinit the module:

  *  @{
  */

/**
 * @brief  WDT enable
 * @return HAL_Status
 */
HAL_Status HAL_WDT_Init(uint32_t freq, struct WDT_REG *wdt)
{
    HAL_ASSERT(freq != 0);

    HAL_ASSERT(IS_WDT_INSTANCE(wdt));

    pWDT = wdt;

    dwWdt.freq = freq;

#ifdef HAL_WDT_DYNFREQ_FEATURE_ENABLED
    wdtDynFreq.targetTick = 0;
    wdtDynFreq.limitVal = WDT_getTopMsecVal(WDT_DYNFREQ_MARGIN_MS, freq);
#endif

    return HAL_OK;
}

/** @} */

/** @defgroup WDT_Exported_Functions_Group5 Other Functions

  This section provides functions allowing to other controlling:

  *  @{
  */

/**
 * @brief  Set WDT timeout period
 * @param  top: timeout period in seconds
 * @return HAL_Status
 */
HAL_Status HAL_WDT_SetTimeout(uint32_t top)
{
#ifdef HAL_WDT_DYNFREQ_FEATURE_ENABLED
    wdtDynFreq.topSec = top;
#endif
    WDT_SetTop(top);
    HAL_WDT_KeepAlive();

    return HAL_OK;
}

/**
 * @brief Set WDT timeout period in msec
 * @param topMsec: timeout period in msec
 * @param freq: clock frequency of watchdog
 * @return HAL_Status
 */
HAL_Status HAL_WDT_SetTopMsec(uint32_t topMsec, uint32_t freq)
{
    uint32_t topVal;

    topVal = WDT_getTopMsecVal(topMsec, freq);
    pWDT->TORR = topVal | topVal << WDT_TORR_TIMEOUT_PERIOD_SHIFT;

    HAL_WDT_KeepAlive();

    return HAL_OK;
}

/**
 * @brief  Get WDT timeout period
 * @return Timeout period
 */
uint32_t HAL_WDT_GetTimeout(void)
{
    return WDT_TopInSeconds(pWDT->TORR);
}

/**
 * @brief  Keep WDT alive
 * @return HAL_Status
 */
HAL_Status HAL_WDT_KeepAlive(void)
{
    pWDT->CRR = WDOG_COUNTER_RESTART_KICK_VALUE;

#ifdef HAL_WDT_DYNFREQ_FEATURE_ENABLED
    WDT_dynFreqSetTargetTick();
#endif

    return HAL_OK;
}

/**
 * @brief  Start WDT
 * @param  mode: Set WDT resp mode
 * @return HAL_Status
 */
HAL_Status HAL_WDT_Start(enum WDT_RESP_MODE mode)
{
    uint32_t tmp = pWDT->CR;

    if (mode == INDIRECT_SYSTEM_RESET) {
        tmp |= WDT_CR_RESP_MODE_MASK;
    } else if (mode == DIRECT_SYSTEM_RESET) {
        tmp &= ~WDT_CR_RESP_MODE_MASK;
    }

    tmp |= WDT_CR_WDT_EN_MASK;

    pWDT->CR = tmp;

#ifdef HAL_WDT_DYNFREQ_FEATURE_ENABLED
    WDT_dynFreqSetTargetTick();
#endif

    return HAL_OK;
}

/**
 * @brief  If the WDT freq changes, updata the new freq.
 * @param  freq: the new freq value.
 * @return HAL_Status
 */
HAL_Status HAL_WDT_DynFreqUpdata(uint32_t freq)
{
#ifdef HAL_WDT_DYNFREQ_FEATURE_ENABLED
    uint32_t topDynMs;
    uint32_t curTick;

    if (!wdtDynFreq.targetTick) {
        return HAL_INVAL;
    }

    curTick = HAL_GetTick();

    WDT_DYNCLK_DBG("**WDT_DynClkUpdata %u-%u = %u\n",
                   wdtDynFreq.targetTick, curTick,
                   wdtDynFreq.targetTick - curTick);

    if (curTick <= wdtDynFreq.targetTickMargin) {
        topDynMs = wdtDynFreq.targetTick - curTick;
        HAL_WDT_SetTopMsec(topDynMs, freq);
    } else {
        WDT_DYNCLK_DBG("  WDT_DynClkUpdata: %u %u %u\n",
                       curTick, wdtDynFreq.targetTick, wdtDynFreq.limitVal);
        pWDT->TORR = wdtDynFreq.limitVal | wdtDynFreq.limitVal << WDT_TORR_TIMEOUT_PERIOD_SHIFT;
        wdtDynFreq.targetTick = 0;
    }
    pWDT->CRR = WDOG_COUNTER_RESTART_KICK_VALUE;
#endif

    return HAL_OK;
}

/**
 * @brief  If the WDT freq has changed, then use it to resume freq.
 * @return HAL_Status
 */
HAL_Status HAL_WDT_DynFreqResume(void)
{
#ifdef HAL_WDT_DYNFREQ_FEATURE_ENABLED
    if (HAL_WDT_DynFreqUpdata(dwWdt.freq) == HAL_OK) {
        WDT_SetTop(wdtDynFreq.topSec);
    }
#endif

    return HAL_OK;
}

/**
 * @brief  Clear interrupt
 * @return WDT_EOI value
 */
uint32_t HAL_WDT_ClearInterrupt(void)
{
    return pWDT->EOI;
}

/**
 * @brief  Get interrupt status
 * @return Interrupt status
 */
uint32_t HAL_WDT_GetIntStatus(void)
{
    return pWDT->STAT;
}

/**
 * @brief  Get timeleft
 * @return Current count value that indicate Timeleft(second) = pWDT->CCVR / dwWdt.freq
 */
uint32_t HAL_WDT_GetTimeLeft(void)
{
    return pWDT->CCVR;
}

/** @} */

/** @} */

/** @} */

#endif /* HAL_WDT_MODULE_ENABLED */
