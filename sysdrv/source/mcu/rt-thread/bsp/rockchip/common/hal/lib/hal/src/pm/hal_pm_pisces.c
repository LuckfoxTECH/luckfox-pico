/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#if defined(RKMCU_PISCES)

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup PM
 *  @{
 */

/** @defgroup PM_How_To_Use How To Use
 *  @{

 The PM driver can be used as follows:

 - Invoke HAL_SYS_Suspend() when system will enter suspend.

 @} */

/** @defgroup PM_Private_Definition Private Definition
 *  @{
 */

/********************* Private MACRO Definition ******************************/

/* for pm_runtime */
#define SLEEP_INPUT_RATE     32000
#define EXPONENT_OF_FRAC_PLL 24
#define RK_PLL_MODE_SLOW     0
#define RK_PLL_MODE_NORMAL   1
#define RK_PLL_MODE_DEEP     2

#define PLL_POSTDIV1_SHIFT 12
#define PLL_POSTDIV1_MASK  0x7 << PLL_POSTDIV1_SHIFT
#define PLL_POSTDIV2_SHIFT 6
#define PLL_POSTDIV2_MASK  0x7 << PLL_POSTDIV2_SHIFT

#define PLL_GET_POSTDIV1(x) \
        (((uint32_t)(x) & (PLL_POSTDIV1_MASK)) >> PLL_POSTDIV1_SHIFT)
#define PLL_GET_POSTDIV2(x) \
    (((uint32_t)(x) & (PLL_POSTDIV2_MASK)) >> PLL_POSTDIV2_SHIFT)
#define UART_CLK_GET_MUX(clk) HAL_CRU_ClkGetMux(CLK_GET_MUX((clk)))
#define GPLL_RUNTIME_RATE     (PLL_INPUT_OSC_RATE * 2)

#define SLEEP_COUNT_TO_MS(ms) ((ms) * SLEEP_INPUT_RATE / 1000)
/********************* Private Structure Definition **************************/
struct UART_REG_SAVE {
    uint32_t DLL;
    uint32_t DLH;
    uint32_t IER;
    uint32_t LCR;
    uint32_t MCR;
};
/********************* Private Variable Definition ***************************/
#ifdef HAL_PM_SLEEP_MODULE_ENABLED
static uint64_t pmTimerLastCount;
#endif

/********************* Private Function Definition ***************************/
#ifdef HAL_PM_RUNTIME_MODULE_ENABLED
static uint32_t PM_GetPllPostDivEven(uint32_t rateIn, uint32_t rateOut, uint32_t *postDiv1, uint32_t *postDiv2)
{
    uint32_t div1, div2, div;

    div = rateIn / rateOut;
    if (div < 7) {
        *postDiv1 = div;
        *postDiv2 = 1;

        return 0;
    }

    for (div2 = 2; div2 <= 6;) {
        div1 = div / div2;
        if (div1 <= 7) {
            break;
        }
        div2 += 2;
    }
    if (div1 < div2) {
        return 2;
    }

    *postDiv1 = div1;
    *postDiv2 = div2;

    if ((div1 * div2) != div) {
        return 1;
    } else {
        return 0;
    }
}

static void PM_CruAsEnable(uint8_t en)
{
#ifdef HAL_CRU_AS_FEATURE_ENABLED
    HAL_CRU_AsEnable(1, en);
    HAL_CRU_AsEnable(2, en);
    HAL_CRU_AsEnable(3, en);
    HAL_CRU_AsEnable(4, en);
#endif
}
#endif

#ifdef HAL_PM_CPU_SLEEP_MODULE_ENABLED

static void SOC_GetWakeupStatus(struct PMU_REG *pPmu)
{
    HAL_DBG("\nwakeup source:\n");
    if (pPmu->WAKEUP_STATUS & (1 << PMU_WAKEUP_STATUS_WAKEUP_PWRMODE_INT_STATUS_SHIFT)) {
        HAL_DBG("\tPower mode state machine wakeup status by interrupt\n");
    }
    if (pPmu->WAKEUP_STATUS & (1 << PMU_WAKEUP_STATUS_PWRMODE_WAKEUP_GPIO_INT_STATUS_SHIFT)) {
        HAL_DBG("\tPower mode state machine wakeup status by gpio interrupt\n");
    }
    if (pPmu->WAKEUP_STATUS & (1 << PMU_WAKEUP_STATUS_PWRMODE_WAKEUP_TIMEOUT_STATUS_SHIFT)) {
        HAL_DBG("\tPower mode state machine wakeup status by timeout\n");
    }
    if (pPmu->WAKEUP_STATUS & (1 << PMU_WAKEUP_STATUS_PWRMODE_WAKEUP_DSP_SFT_STATUS_SHIFT)) {
        HAL_DBG("\tPower mode state machine wakeup status by DSP software\n");
    }
    if (pPmu->WAKEUP_STATUS & (1 << PMU_WAKEUP_STATUS_PWRMODE_WAKEUP_TIMER_STATUS_SHIFT)) {
        HAL_DBG("\tPower mode state machine wakeup status by timer interrupt\n");
    }
    if (pPmu->WAKEUP_STATUS & (1 << PMU_WAKEUP_STATUS_PWRMODE_WAKEUP_VAD_STATUS_SHIFT)) {
        HAL_DBG("\tPower mode state machine wakeup status by vad\n");
    }
    if (pPmu->WAKEUP_STATUS & (1 << PMU_WAKEUP_STATUS_WAKEUP_DSP_INT_STATUS_SHIFT)) {
        HAL_DBG("\tDSP auto power down state machine wakeup status by interrupt\n");
    }
    if (pPmu->WAKEUP_STATUS & (1 << PMU_WAKEUP_STATUS_DSP_WAKEUP_GPIO_INT_STATUS_SHIFT)) {
        HAL_DBG("\tDSP auto power down state machine wakeup status by gpio interrupt\n");
    }
    if (pPmu->WAKEUP_STATUS & (1 << PMU_WAKEUP_STATUS_DSP_WAKEUP_TIMEOUT_STATUS_SHIFT)) {
        HAL_DBG("\tDSP auto power down state machine wakeup status by timeout\n");
    }
    if (pPmu->WAKEUP_STATUS & (1 << PMU_WAKEUP_STATUS_DSP_WAKEUP_SFT_STATUS_SHIFT)) {
        HAL_DBG("\tDSP auto power down state machine wakeup status by MCU software\n");
    }
    if (pPmu->WAKEUP_STATUS & (1 << PMU_WAKEUP_STATUS_DSP_WAKEUP_TIMER_STATUS_SHIFT)) {
        HAL_DBG("\tauto power down state machine wakeup status by timer\n");
    }
    if (pPmu->WAKEUP_STATUS & (1 << PMU_WAKEUP_STATUS_DSP_WAKEUP_VAD_STATUS_SHIFT)) {
        HAL_DBG("\tDSP auto power down state machine wakeup status by vad\n");
    }

    pPmu->WAKEUP_STATUS = (1 << PMU_WAKEUP_STATUS_WAKEUP_PWRMODE_INT_STATUS_SHIFT) |
                          (1 << PMU_WAKEUP_STATUS_PWRMODE_WAKEUP_GPIO_INT_STATUS_SHIFT) |
                          (1 << PMU_WAKEUP_STATUS_PWRMODE_WAKEUP_TIMEOUT_STATUS_SHIFT) |
                          (1 << PMU_WAKEUP_STATUS_PWRMODE_WAKEUP_DSP_SFT_STATUS_SHIFT) |
                          (1 << PMU_WAKEUP_STATUS_PWRMODE_WAKEUP_TIMER_STATUS_SHIFT) |
                          (1 << PMU_WAKEUP_STATUS_PWRMODE_WAKEUP_VAD_STATUS_SHIFT) |
                          (1 << PMU_WAKEUP_STATUS_WAKEUP_DSP_INT_STATUS_SHIFT) |
                          (1 << PMU_WAKEUP_STATUS_DSP_WAKEUP_GPIO_INT_STATUS_SHIFT) |
                          (1 << PMU_WAKEUP_STATUS_DSP_WAKEUP_TIMEOUT_STATUS_SHIFT) |
                          (1 << PMU_WAKEUP_STATUS_DSP_WAKEUP_SFT_STATUS_SHIFT) |
                          (1 << PMU_WAKEUP_STATUS_DSP_WAKEUP_TIMER_STATUS_SHIFT) |
                          (1 << PMU_WAKEUP_STATUS_DSP_WAKEUP_VAD_STATUS_SHIFT);
}

static void SOC_FastBootConfig(struct GRF_REG *pGrf)
{
    pGrf->FAST_BOOT_ADDR = (uint32_t)HAL_CPU_DoResume;
    pGrf->FAST_BOOT_EN = 1;
}

static void SOC_SleepModeInit(struct PMU_REG *pPmu)
{
    uint32_t mask = 0, value = 0;

    mask = PMU_PWRMODE_CON_POWER_MODE_EN_MASK |
           PMU_PWRMODE_CON_PMU_USE_LF_MASK |
           PMU_PWRMODE_CON_PLL_PD_EN_MASK |
           PMU_PWRMODE_CON_LOGIC_PD_EN_MASK |
           PMU_PWRMODE_CON_PWRMODE_LDO_ADJ_EN_MASK |
           PMU_PWRMODE_CON_BYPASS_PLL_LOCK_MASK |
           PMU_PWRMODE_CON_BYPASS_HF_EN_MASK |
           PMU_PWRMODE_CON_GLOBAL_INT_DISABLE_CFG_MASK |
           PMU_PWRMODE_CON_SHRM_PD_EN_MASK |
           PMU_PWRMODE_CON_SHRM_MEM_RETPD_EN_MASK;

    value = (1 << PMU_PWRMODE_CON_POWER_MODE_EN_SHIFT) |
            /*(1 << PMU_PWRMODE_CON_PMU_USE_LF_SHIFT) |*/
            (1 << PMU_PWRMODE_CON_PLL_PD_EN_SHIFT) |
            (1 << PMU_PWRMODE_CON_LOGIC_PD_EN_SHIFT) |
            (1 << PMU_PWRMODE_CON_PWRMODE_LDO_ADJ_EN_SHIFT) |
            /*(1 << PMU_PWRMODE_CON_BYPASS_PLL_LOCK_SHIFT) |*/
            /*(1 << PMU_PWRMODE_CON_BYPASS_HF_EN_SHIFT) |*/
            (1 << PMU_PWRMODE_CON_GLOBAL_INT_DISABLE_CFG_SHIFT) |
            (1 << PMU_PWRMODE_CON_SHRM_PD_EN_SHIFT) |
            (1 << PMU_PWRMODE_CON_SHRM_MEM_RETPD_EN_SHIFT);

    /* if PD_DSP and PD_AUDIO power down, PMU low frequency mode enable */
    if (pPmu->PWRDN_ST &
        ((1 << PMU_PWRDN_ST_PD_AUDIO_PWR_STAT_SHIFT) | (1 << PMU_PWRDN_ST_PD_DSP_PWR_STAT_SHIFT))) {
        value |= (1 << PMU_PWRMODE_CON_PMU_USE_LF_SHIFT);
        pPmu->PWRMODE_LDO_ADJ_CNT = SLEEP_COUNT_TO_MS(1);
        pPmu->PLLLOCK_CNT = SLEEP_COUNT_TO_MS(1);
        pPmu->DSP_LDO_ADJ_CNT = SLEEP_COUNT_TO_MS(1);
    }
    pPmu->PWRMODE_CON = VAL_MASK_WE(mask, value);

    if (pPmu->PWRMODE_CON & (1 << PMU_PWRMODE_CON_LOGIC_PD_EN_SHIFT)) {
        mask = PMU_BUS_CLR_CLR_LOGIC_MASK;
        value = (1 << PMU_BUS_CLR_CLR_LOGIC_SHIFT);
        pPmu->BUS_CLR |= VAL_MASK_WE(mask, value);
    }

    if (pPmu->PWRMODE_CON & (1 << PMU_PWRMODE_CON_SHRM_MEM_RETPD_EN_SHIFT)) {
        mask = PMU_SHRM_CON1_PWRMODE_SHRM_PWRDWN_EN_MASK |
               PMU_SHRM_CON1_PWRMODE_SHRM_RET2N_MASK;
        value = (0xf << PMU_SHRM_CON1_PWRMODE_SHRM_PWRDWN_EN_SHIFT);
        pPmu->SHRM_CON1 = VAL_MASK_WE(mask, value);

        mask = PMU_BUS_CLR_CLR_SHRM_MASK;
        value = (1 << PMU_BUS_CLR_CLR_SHRM_SHIFT);
        pPmu->BUS_CLR |= VAL_MASK_WE(mask, value);
    }

    if (pPmu->PWRMODE_CON & (1 << PMU_PWRMODE_CON_PWRMODE_LDO_ADJ_EN_SHIFT)) {
        mask = PMU_LDO_CON1_PWRMODE_LDOCORE_ADJ_MASK;
        pPmu->LDO_CON[1] = VAL_MASK_WE(mask, 0x10);
    }

    if (pPmu->PWRMODE_CON & (1 << PMU_PWRMODE_CON_PLL_PD_EN_SHIFT)) {
        mask = PMU_PLL_CON_PLL_PD_CFG_MASK;
        /* if PD_DSP and PD_AUDIO power down, CPLL, GPLL and 32K PLL power down by hardware */
        if (pPmu->PWRDN_ST &
            ((1 << PMU_PWRDN_ST_PD_AUDIO_PWR_STAT_SHIFT) | (1 << PMU_PWRDN_ST_PD_DSP_PWR_STAT_SHIFT))) {
            pPmu->PLL_CON = VAL_MASK_WE(mask, 0x07);
        } else {
            pPmu->PLL_CON = VAL_MASK_WE(mask, 0x05);
        }
    }

    if (pPmu->PWRMODE_CON & (1 << PMU_PWRMODE_CON_PMU_USE_LF_SHIFT)) {
        mask = PMU_SFT_CON_PMU_LF_MODE_CFG_MASK;
        value = (1 << PMU_SFT_CON_PMU_LF_MODE_CFG_SHIFT);
        pPmu->SFT_CON = VAL_MASK_WE(mask, value);
    }
}

static void SOC_WakeupSourceConfig(struct PMU_REG *pPmu)
{
    uint32_t mask = 0, value = 0;

    mask = PMU_WAKEUP_CFG6_GPIO_INT_EN_MASK |
           PMU_WAKEUP_CFG6_TIMER_EN_MASK;
    value = (1 << PMU_WAKEUP_CFG6_GPIO_INT_EN_SHIFT) |
            (1 << PMU_WAKEUP_CFG6_TIMER_EN_SHIFT);
    pPmu->WAKEUP_CFG6 = VAL_MASK_WE(mask, value);
}

static void SOC_SleepModeReinit(struct PMU_REG *pPmu)
{
    uint32_t mask = 0, value = 0;

    mask = PMU_PWRMODE_CON_POWER_MODE_EN_MASK |
           PMU_PWRMODE_CON_PLL_PD_EN_MASK |
           PMU_PWRMODE_CON_LOGIC_PD_EN_MASK |
           PMU_PWRMODE_CON_PWRMODE_LDO_ADJ_EN_MASK |
           PMU_PWRMODE_CON_BYPASS_PLL_LOCK_MASK |
           PMU_PWRMODE_CON_BYPASS_HF_EN_MASK |
           PMU_PWRMODE_CON_GLOBAL_INT_DISABLE_CFG_MASK |
           PMU_PWRMODE_CON_SHRM_PD_EN_MASK |
           PMU_PWRMODE_CON_SHRM_MEM_RETPD_EN_MASK;
    pPmu->PWRMODE_CON = VAL_MASK_WE(mask, value);
}

static void SOC_PutChar(char c, struct UART_REG *pUart)
{
    if (pUart) {
        pUart->THR = c;
        while (!(pUart->USR & UART_USR_TX_FIFO_EMPTY)) {
            ;
        }
    }
}

static void SOC_UartSave(struct UART_REG_SAVE *pUartSave, struct UART_REG *pUart)
{
    if (pUartSave && pUart) {
        while (!(pUart->USR & UART_USR_TX_FIFO_EMPTY)) {
            ;
        }
        pUartSave->LCR = pUart->LCR;
        pUartSave->IER = pUart->IER;
        pUartSave->MCR = pUart->MCR;
        if (pUart->USR & UART_USR_BUSY) {
            HAL_DelayMs(10);
        }
        if (pUart->USR & UART_USR_BUSY) {
            pUart->SRR = UART_SRR_XFR | UART_SRR_RFR;
        }
        pUart->LCR = UART_LCR_DLAB;
        pUartSave->DLL = pUart->DLL;
        pUartSave->DLH = pUart->DLH;
        pUart->LCR = pUartSave->LCR;
    }
}

static void SOC_UartRestore(struct UART_REG_SAVE *pUartSave, struct UART_REG *pUart)
{
    if (pUartSave && pUart) {
        pUart->SRR = UART_SRR_XFR | UART_SRR_RFR | UART_SRR_UR;
        pUart->MCR = UART_MCR_LOOP;
        pUart->LCR = UART_LCR_DLAB;
        pUart->DLL = pUartSave->DLL;
        pUart->DLH = pUartSave->DLH;
        pUart->LCR = pUartSave->LCR;
        pUart->IER = pUartSave->IER;
        pUart->FCR = UART_FCR_ENABLE_FIFO;
        pUart->MCR = pUartSave->MCR;
    }
}

static int SOC_SuspendEnter(uint32_t flag)
{
    HAL_DCACHE_CleanInvalidate();
    __WFI();
    HAL_CPU_DoResume();

    return HAL_OK;
}
#endif

#ifdef HAL_PM_RUNTIME_MODULE_ENABLED
static uint32_t PM_RuntimeEnter(ePM_RUNTIME_idleMode idleMode)
{
    uint32_t gpllCon1, gpllDiv2, gpllDiv2New;
    uint32_t gpllCon0 = 0, gpllDiv1, gpllDiv1New;
    uint32_t clkSelCon2 = 0, clkSelCon33 = 0, clkSelCon40 = 0;
    uint32_t cruMode;
    uint32_t gpllRateNew;
    uint32_t mDiv;
    static uint32_t gpllRate;
    const struct PM_RUNTIME_INFO *pdata = HAL_PM_RuntimeGetData();

#ifdef HAL_WDT_DYNFREQ_FEATURE_ENABLED
    uint32_t tmpDiv;
    static uint32_t pmDynWdtFreqNor;
    static uint32_t pmWdtFreq;
    static uint32_t pmDynWdtFreq;
#endif

    if (PM_DISPLAY_REQUESTED(pdata)) {
        return HAL_BIT(PM_RUNTIME_TYPE_DISPLAY);
    }

    if (PM_UART_REQUESTED(pdata)) {
        if ((PM_UART_REQUESTED(pdata) & HAL_BIT(0)) &&
            (UART_CLK_GET_MUX(CLK_UART0) != SCLK_UART0_SEL_XIN_OSC0_FUNC)) {
            return HAL_BIT(PM_RUNTIME_TYPE_UART);
        } else if ((PM_UART_REQUESTED(pdata) & HAL_BIT(1)) &&
                   (UART_CLK_GET_MUX(CLK_UART1) != SCLK_UART1_SEL_XIN_OSC0_FUNC)) {
            return HAL_BIT(PM_RUNTIME_TYPE_UART);
        }
    }

    if (PM_INTF_REQUESTED(pdata) &
        PM_RUNTIME_ID_TYPE_BIT_MSK(PM_RUNTIME_ID_SPI_APB)) {
        return HAL_BIT(PM_RUNTIME_TYPE_INTF);
    }

    if (PM_I2C_REQUESTED(pdata)) {
        return HAL_BIT(PM_RUNTIME_TYPE_I2C);
    }

    if (PM_HS_INTF_REQUESTED(pdata)) {
        return HAL_BIT(PM_RUNTIME_TYPE_HS_INTF);
    }

    if (PM_SPI_REQUESTED(pdata)) {
        return HAL_BIT(PM_RUNTIME_TYPE_SPI);
    }

    if (PM_CIF_REQUESTED(pdata)) {
        return HAL_BIT(PM_RUNTIME_TYPE_CIF);
    }

    PM_CruAsEnable(0);

    if (!gpllRate) {
        gpllRate = HAL_CRU_ClkGetFreq(PLL_GPLL);
    }

#ifdef HAL_WDT_DYNFREQ_FEATURE_ENABLED
    if (!pmWdtFreq) {
        pmWdtFreq = HAL_CRU_ClkGetFreq(PCLK_WDT);
        tmpDiv = (gpllRate / pmWdtFreq);
        pmDynWdtFreq = PLL_INPUT_OSC_RATE / tmpDiv;
        pmDynWdtFreqNor = GPLL_RUNTIME_RATE / tmpDiv;
    }
#endif

    if (idleMode == PM_RUNTIME_IDLE_DEEP) {
        cruMode = CRU->CRU_MODE_CON00 |
                  MASK_TO_WE(CRU_CRU_MODE_CON00_CLK_GPLL_MODE_MASK);
        gpllCon1 = CRU->GPLL_CON[1] |
                   MASK_TO_WE(CRU_GPLL_CON1_PLLPD0_MASK);

        clkSelCon33 = CRU->CRU_CLKSEL_CON[33] |
                      MASK_TO_WE(CRU_CRU_CLKSEL_CON33_HCLK_M4_DIV_MASK);
        clkSelCon2 = CRU->CRU_CLKSEL_CON[2] |
                     MASK_TO_WE(CRU_CRU_CLKSEL_CON02_SCLK_SHRM_DIV_MASK);
        clkSelCon40 = CRU->CRU_CLKSEL_CON[40] |
                      MASK_TO_WE(CRU_CRU_CLKSEL_CON40_ACLK_LOGIC_DIV_MASK);

        CRU->CRU_MODE_CON00 =
            VAL_MASK_WE(CRU_CRU_MODE_CON00_CLK_GPLL_MODE_MASK,
                        RK_PLL_MODE_SLOW << CRU_CRU_MODE_CON00_CLK_GPLL_MODE_SHIFT);

        CRU->CRU_CLKSEL_CON[33] =
            VAL_MASK_WE(CRU_CRU_CLKSEL_CON33_HCLK_M4_DIV_MASK, 0);
        CRU->CRU_CLKSEL_CON[2] =
            VAL_MASK_WE(CRU_CRU_CLKSEL_CON02_SCLK_SHRM_DIV_MASK, 0);
        CRU->CRU_CLKSEL_CON[40] =
            VAL_MASK_WE(CRU_CRU_CLKSEL_CON40_ACLK_LOGIC_DIV_MASK, 0);

        HAL_ASSERT(!(CRU->GPLL_CON[1] & CRU_GPLL_CON1_PLLPD0_MASK));
        HAL_ASSERT(!(CRU->GPLL_CON[1] & CRU_GPLL_CON1_PLLPDSEL_MASK));

#ifdef HAL_WDT_DYNFREQ_FEATURE_ENABLED
        HAL_WDT_DynFreqUpdata(pmDynWdtFreq);
#endif
        CRU->GPLL_CON[1] = VAL_MASK_WE(CRU_GPLL_CON1_PLLPD0_MASK,
                                       CRU_GPLL_CON1_PLLPD0_MASK);
    } else if (idleMode == PM_RUNTIME_IDLE_NORMAL) {
        cruMode = 0;
        clkSelCon2 = 0;

        HAL_ASSERT(HAL_CRU_ClkGetFreq(HCLK_M4) > GPLL_RUNTIME_RATE);

        gpllCon1 = CRU->GPLL_CON[1] |
                   MASK_TO_WE(CRU_GPLL_CON1_POSTDIV2_MASK);
        gpllCon0 = CRU->GPLL_CON[0] |
                   MASK_TO_WE(CRU_GPLL_CON0_POSTDIV1_MASK);

        gpllDiv1 = PLL_GET_POSTDIV1(gpllCon0);
        gpllDiv2 = PLL_GET_POSTDIV2(gpllCon1);

        gpllRate = (gpllRate * gpllDiv1 * gpllDiv2);

        if (PM_GetPllPostDivEven(gpllRate, GPLL_RUNTIME_RATE, &gpllDiv1New, &gpllDiv2New) >= 2) {
            return UINT32_MAX;
        }

        gpllRateNew = gpllRate / (gpllDiv1New * gpllDiv2New);
        mDiv = gpllRateNew / GPLL_RUNTIME_RATE;
        HAL_ASSERT(mDiv > 0);
        HAL_ASSERT((gpllRateNew * mDiv) >= GPLL_RUNTIME_RATE);
        if (mDiv > 0) {
            mDiv -= 1;
        }

#ifdef HAL_WDT_DYNFREQ_FEATURE_ENABLED
        HAL_WDT_DynFreqUpdata(pmDynWdtFreqNor);
#endif
        clkSelCon33 = CRU->CRU_CLKSEL_CON[33] |
                      MASK_TO_WE(CRU_CRU_CLKSEL_CON33_HCLK_M4_DIV_MASK);

        CRU->GPLL_CON[0] = VAL_MASK_WE(CRU_GPLL_CON0_POSTDIV1_MASK,
                                       gpllDiv1New << CRU_GPLL_CON0_POSTDIV1_SHIFT);

        CRU->GPLL_CON[1] = VAL_MASK_WE(CRU_GPLL_CON1_POSTDIV2_MASK,
                                       gpllDiv2New << CRU_GPLL_CON1_POSTDIV2_SHIFT);

        CRU->CRU_CLKSEL_CON[33] = VAL_MASK_WE(CRU_CRU_CLKSEL_CON33_HCLK_M4_DIV_MASK,
                                              (mDiv) << CRU_CRU_CLKSEL_CON33_HCLK_M4_DIV_SHIFT);
    } else {
        goto _ret_err;
    }

    __DSB();
    __WFI();

    if (idleMode == PM_RUNTIME_IDLE_DEEP) {
        CRU->CRU_MODE_CON00 =
            VAL_MASK_WE(CRU_CRU_MODE_CON00_CLK_GPLL_MODE_MASK,
                        RK_PLL_MODE_SLOW << CRU_CRU_MODE_CON00_CLK_GPLL_MODE_SHIFT);
        CRU->GPLL_CON[1] = gpllCon1;
        CRU->CRU_CLKSEL_CON[33] = clkSelCon33;
        CRU->CRU_CLKSEL_CON[2] = clkSelCon2;
        CRU->CRU_CLKSEL_CON[40] = clkSelCon40;

        while ((CRU->GPLL_CON[1] & CRU_CPLL_CON1_PLL_LOCK_MASK) !=
               CRU_CPLL_CON1_PLL_LOCK_MASK) {
            ;
        }

        CRU->CRU_MODE_CON00 = cruMode;
    } else if (idleMode == PM_RUNTIME_IDLE_NORMAL) {
        CRU->CRU_CLKSEL_CON[33] = clkSelCon33;
        CRU->GPLL_CON[1] = gpllCon1;
        CRU->GPLL_CON[0] = gpllCon0;
    }
#ifdef HAL_WDT_DYNFREQ_FEATURE_ENABLED
    HAL_WDT_DynFreqResume();
#endif
    PM_CruAsEnable(1);

    return 0;
_ret_err:

    return UINT32_MAX;
}

/** @} */
/********************* Public Function Definition ****************************/
/** @defgroup PM_Exported_Functions_Group5 Other Functions
 *  @{
 */

uint32_t HAL_PM_RuntimeEnter(ePM_RUNTIME_idleMode idleMode)
{
    uint32_t ret;

    if (idleMode) {
        ret = PM_RuntimeEnter(idleMode);
    } else {
        ret = UINT32_MAX;
    }

    if (!idleMode || ret) {
        __DSB();
        __WFI();
    }

    return UINT32_MAX;
}

#endif

#ifdef HAL_PM_SLEEP_MODULE_ENABLED
HAL_Status HAL_PM_TimerStart(uint64_t timeoutCount, bool needTimeout)
{
    pmTimerLastCount = HAL_GetSysTimerCount();

    return 0;
}

HAL_Status HAL_PM_TimerStop(void)
{
    return 0;
}

uint64_t HAL_PM_GetTimerCount(void)
{
    uint64_t timerCount;

    timerCount = HAL_GetSysTimerCount() - pmTimerLastCount;

    return timerCount;
}

int HAL_SYS_Suspend(struct PM_SUSPEND_INFO *suspendInfo)
{
#ifdef HAL_PM_CPU_SLEEP_MODULE_ENABLED
    struct PMU_REG *pPmu = PMU;
    struct GRF_REG *pGrf = GRF;
    struct UART_REG *pUart = NULL;
    struct UART_REG_SAVE pUartSave;

    HAL_ASSERT(suspendInfo != NULL);

#ifdef HAL_UART_MODULE_ENABLED
    if (suspendInfo->flag.uartValid) {
        if (suspendInfo->flag.uartChannel == 0) {
            pUart = UART0;
        } else if (suspendInfo->flag.uartChannel == 1) {
            pUart = UART1;
        }
    }
#endif

    SOC_PutChar('0', pUart);
    SOC_SleepModeInit(pPmu);
    SOC_PutChar('1', pUart);
    SOC_FastBootConfig(pGrf);
    SOC_PutChar('2', pUart);
    SOC_WakeupSourceConfig(pPmu);
    SOC_PutChar('3', pUart);
    HAL_NVIC_SuspendSave();
    SOC_PutChar('4', pUart);
    HAL_SCB_SuspendSave();
    SOC_PutChar('5', pUart);
    SOC_UartSave(&pUartSave, pUart);
    HAL_CPU_SuspendEnter(suspendInfo->suspendFlag, SOC_SuspendEnter);
    SOC_SleepModeReinit(pPmu);
    SOC_UartRestore(&pUartSave, pUart);
    SOC_PutChar('4', pUart);
    HAL_DCACHE_Enable();
    HAL_ICACHE_Enable();
    SOC_PutChar('3', pUart);
    HAL_SCB_ResumeRestore();
    SOC_PutChar('2', pUart);
    HAL_NVIC_ResumeRestore();
    SOC_PutChar('1', pUart);
    SOC_GetWakeupStatus(pPmu);
    SOC_PutChar('0', pUart);
    HAL_DBG("\n");
#endif

    return HAL_OK;
}

/** @} */
#endif

/** @} */

/** @} */

#endif /* RKMCU_PISCES */
