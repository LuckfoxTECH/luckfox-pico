/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#if defined(RKMCU_RK2206)

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

#if defined(HAL_PM_SLEEP_MODULE_ENABLED)

/** @defgroup PM_Private_Definition Private Definition
 *  @{
 */

/********************* Private MACRO Definition ******************************/
/* sleep mode define */
#define SLP_PMU_HW_ARM_PD    HAL_BIT(0)
#define SLP_PMU_HW_PLLS_PD   HAL_BIT(8)
#define SLP_PMU_PMUALIVE_32K HAL_BIT(9)
#define SLP_PMU_DIS_OSC      HAL_BIT(10)
#define SLP_PLLS_DEEP        HAL_BIT(11)
#define SLP_CLK_GT           HAL_BIT(16)
#define SLP_PMIC_LP          HAL_BIT(17)
#define SLP_32K_EXT          HAL_BIT(24)
#define SLP_TIME_OUT_WKUP    HAL_BIT(25)
#define SLP_PMU_DBG          HAL_BIT(26)
#define SLP_PMU_TST_CLK      HAL_BIT(27)
#define SLP_PMU_VAD_WKUP     HAL_BIT(28)

#define PLL_CON(i)           ((i) * 4)
#define PLL_CON_CNT          5
#define PLL_LOCK_MSK         HAL_BIT(10)
#define pll_mode_shift(id)   ((id) * 2)
#define pll_mode_msk(id)     (0x3 << pll_mode_shift(id))
#define CRU_CLKGATES_CON_CNT 13

#define PLL_LOCKED_TIMEOUT    600000U
#define MAX_PMU_ST_WAIT_COUNT 500
#define MAX_IDLE_WAIT_COUNT   1000
#ifndef CPU_SLEEP_TIMEOUT_TIME
/*cpu deep sleep go on this time will auto wake up, the unit ms*/
#define CPU_SLEEP_TIMEOUT_TIME 2000
#endif

#define UART_FIFO_EMPTY (HAL_BIT(6) | HAL_BIT(5))

typedef enum {
    BUS_ACTIVE,
    BUS_IDLE,
} ePMU_BusState;

typedef enum {
    PMU_PD_WLAN,
    PMU_PD_PERI,
    PMU_PD_MCU,
} ePMU_PdId;

typedef enum {
    PMU_PD_ON  = 0,
    PMU_PD_OFF = 1
} ePMU_PdState;

typedef enum {
    PMU_BUS_WLAN,
    PMU_BUS_PERI,
    PMU_BUS_MCU,
    PMU_BUS_TOP,
} ePMU_BusId;

typedef enum {
    GPLL_ID,
    UPLL_ID,
    VPLL_ID,
    END_PLL_ID,
} ePLL_Id;

typedef enum {
    SLOW_MODE,
    NORMAL_MODE,
    DEEP_SLOW_MODE,
} ePLL_WorkMode;

/********************* Private Structure Definition **************************/
struct SCB_SAVE {
    uint32_t aircr;
    uint32_t ccr;
    uint8_t shp[12];
    uint32_t shcsr;
    uint32_t cpacr;
};

struct SYSTICK_SAVE {
    uint32_t ctrl;
    uint32_t load;
};

struct UART_SAVE {
    uint32_t dll;
    uint32_t dlh;
    uint32_t ier;
    uint32_t fcr;
    uint32_t mcr;
    uint32_t lcr;
};

struct RKPM_MODE_CTRL {
    uint32_t suspendMode;
    uint32_t suspendWkupCfg;
};

struct RK_SLEEP_DATA {
    uint32_t cruClkGateCon[CRU_CLKGATES_CON_CNT];
    uint32_t cruClkgtSave[CRU_CLKGATES_CON_CNT];
    uint32_t clkSelCon0, clkSelCon1, clkSelCon2;
    uint32_t cruModeSave;
    uint32_t cruPllsConSave[END_PLL_ID];
    uint32_t grfPvtmCon[2];
    uint32_t gpio0aIomuxL;
    uint32_t mpuCtrl;
};

struct TIMER_REG_SAVE {
    uint32_t loadCnt[2];
    uint32_t ctrlReg;
};

/********************* Private Variable Definition ***************************/
#ifdef HAL_PM_CPU_SLEEP_MODULE_ENABLED
static struct SLEEP_CONFIG_DATA *sleepConfig;
static struct RK_SLEEP_DATA sleepData;
static struct UART_SAVE debugPortSave;
static struct SCB_SAVE scbData;
static struct SYSTICK_SAVE systickData;
static struct TIMER_REG_SAVE systimerData;
static uint32_t pmuPowerdomainState;
static uint32_t clkUngtMsk[CRU_CLKGATES_CON_CNT] = {
    0x001f, 0x1fff, 0xffff, 0x3887,
    0xfff0, 0x7f3f, 0xffff, 0x000f,
    0x3b00, 0xffff, 0xffff, 0xffff,
    0x0310,
};

static struct PMU_REG *pPmu = PMU;
static struct UART_REG *pUart = UART0;
static struct CRU_REG *pCru = CRU;
static struct GRF_REG *pGrf = GRF;
static struct TIMER_REG *ptimer6 = TIMER6;

/********************* Private Function Definition ***************************/

static void PM_PutChar(char c)
{
    if (pUart) {
        pUart->THR = c;
        if (c == '\n') {
            pUart->THR = '\r';
        }
        while (!(pUart->USR & UART_USR_TX_FIFO_EMPTY)) {
            ;
        }
    }
}

static void PM_PrintHex(uint32_t hex)
{
    uint8_t c, i = 8;

    PM_PutChar('0');
    PM_PutChar('x');

    while (i--) {
        c = (hex & 0xf0000000) >> 28;
        PM_PutChar(c < 0xa ? c + '0' : c - 0xa + 'a');
        hex <<= 4;
    }
}

static void PM_PrintDec(int dec)
{
    int i, tmp = dec;

    if (dec < 0) {
        PM_PutChar('-');
        tmp = -dec;
        dec = -dec;
    }

    for (i = 1; tmp / 10; tmp /= 10, i *= 10) {
        ;
    }

    for (; i >= 1; i /= 10) {
        PM_PutChar('0' + (char)(dec / i));
        dec %= i;
    }
}

static void PM_UartInit(void)
{
    while (!(pUart->USR & UART_USR_TX_FIFO_EMPTY)) {
        ;
    }

    pUart->SRR = UART_SRR_XFR | UART_SRR_RFR | UART_SRR_UR;
    pUart->IER = 0;
    pUart->MCR = UART_MCR_LOOP;
    pUart->LCR = UART_LCR_DLAB;
    pUart->DLL = debugPortSave.dll;
    pUart->DLH = debugPortSave.dlh;
    pUart->LCR = debugPortSave.lcr;
    pUart->FCR = UART_FCR_ENABLE_FIFO;
    pUart->MCR = debugPortSave.mcr;
}

static void PM_UartSave(void)
{
    while (!(pUart->USR & UART_USR_TX_FIFO_EMPTY)) {
        ;
    }

    debugPortSave.lcr = pUart->LCR;
    debugPortSave.ier = pUart->IER;
    debugPortSave.mcr = pUart->MCR;
    pUart->LCR = debugPortSave.lcr | UART_LCR_DLAB;
    debugPortSave.dll = pUart->DLL;
    debugPortSave.dlh = pUart->DLH;
    pUart->LCR = debugPortSave.lcr;
}

static void PM_UartRestore(void)
{
    while (!(pUart->USR & UART_USR_TX_FIFO_EMPTY)) {
        ;
    }

    pUart->SRR = UART_SRR_XFR | UART_SRR_RFR | UART_SRR_UR;

    pUart->MCR = UART_MCR_LOOP;
    pUart->LCR = UART_LCR_DLAB;
    pUart->DLL = debugPortSave.dll;
    pUart->DLH = debugPortSave.dlh;
    pUart->LCR = debugPortSave.lcr;
    pUart->IER = debugPortSave.ier;
    pUart->FCR = UART_FCR_ENABLE_FIFO;
    pUart->MCR = debugPortSave.mcr;
}

static void PM_SystimerSave(void)
{
    struct TIMER_REG *sysTimer = SYS_TIMER;

    systimerData.loadCnt[0] = sysTimer->LOAD_COUNT[0];
    systimerData.loadCnt[1] = sysTimer->LOAD_COUNT[1];
    systimerData.ctrlReg = sysTimer->CONTROLREG;
}

static void PM_SystimerRestore(void)
{
    struct TIMER_REG *sysTimer = SYS_TIMER;

    sysTimer->LOAD_COUNT[0] = systimerData.loadCnt[0];
    sysTimer->LOAD_COUNT[1] = systimerData.loadCnt[1];
    sysTimer->CONTROLREG = systimerData.ctrlReg;
}

static int PM_ModeInit(void)
{
    if (!sleepConfig->suspendMode) {
        sleepConfig->suspendMode =
            SLP_PMU_HW_ARM_PD |
            SLP_PMU_DIS_OSC |
            SLP_PMU_PMUALIVE_32K |
            /* SLP_TIME_OUT_WKUP | */
            SLP_PMU_DBG
            /* | SLP_PMU_TST_CLK */
        ;
    }

    if (!sleepConfig->suspendWkupSrc) {
        sleepConfig->suspendWkupSrc =
            PMU_WAKEUP_CFG_M4_INT_EN_MASK |
            PMU_WAKEUP_CFG_M0_INT_EN_MASK
            /* | PMU_WAKEUP_CFG_TIMEOUT_EN_MASK */
        ;
    }

    return 0;
}

static void PMU_SleepEnterInfo(uint32_t mode)
{
    static uint32_t sleepCnt;

    PM_PutChar('(');
    PM_PrintHex(mode);
    PM_PutChar(' ');
    PM_PrintDec(++sleepCnt);
    PM_PutChar(')');
    PM_PutChar('\n');
}

static void PMU_SleepModeInfo(void)
{
    PM_PutChar('(');
    PM_PrintHex(pPmu->PWRMODE_CON);
    PM_PutChar(' ');
    PM_PrintHex(pPmu->WAKEUP_CFG);
    PM_PutChar(' ');
    PM_PrintHex(pPmu->PWRDN_ST);
    PM_PutChar(' ');
    PM_PrintHex(pPmu->SFT_CON);
    PM_PutChar(' ');
    PM_PrintHex(pPmu->RET_CON);
    PM_PutChar(')');
    PM_PutChar('\n');
}

static void PMU_SleepFinishInfo(void)
{
    PM_PutChar('(');
    PM_PrintHex(pPmu->INT_ST);
    PM_PutChar(')');
    PM_PutChar('\n');

    /* Clear all interrupt status */
    pPmu->INT_ST = 0x1eff;
}

static void SOC_ClkGateSuspend(void)
{
    int i;

    for (i = 0; i < CRU_CLKGATES_CON_CNT; i++) {
        sleepData.cruClkgtSave[i] =
            pCru->CRU_CLKGATE_CON[i];

        if (sleepConfig->suspendMode & SLP_PMU_DIS_OSC) {
            pCru->CRU_CLKGATE_CON[i] = 0xffff0000;
        } else {
            pCru->CRU_CLKGATE_CON[i] = 0xffff0000 | ~clkUngtMsk[i];
        }
    }
}

static void SOC_ClkGateResume(void)
{
    int i;

    for (i = 0; i < CRU_CLKGATES_CON_CNT; i++) {
        pCru->CRU_CLKGATE_CON[i] = 0xffff0000 | sleepData.cruClkgtSave[i];
    }
}

static void PMU_Clk32kConfig(void)
{
    sleepData.clkSelCon0 =
        pCru->CRU_CLKSEL_CON[0];
    sleepData.clkSelCon1 =
        pCru->CRU_CLKSEL_CON[1];

    pCru->CRU_CLKSEL_CON[1] = 0;
    pCru->CRU_CLKSEL_CON[0] =
        VAL_MASK_WE(0x3 << 5, 2 << 5);

    if (sleepConfig->suspendMode & SLP_PMU_TST_CLK) {
        pCru->CRU_CLKSEL_CON[35] = 0x01ff0010;
        pGrf->GPIO0A_IOMUX_H = 0xf0006000;
    }
}

static void PMU_Clk32kConfigRestore(void)
{
    pCru->CRU_CLKSEL_CON[0] = sleepData.clkSelCon0 | 0xffff0000;
    pCru->CRU_CLKSEL_CON[1] = sleepData.clkSelCon1 | 0xffff0000;
}

static inline uint32_t PMU_BusIdleSt(ePMU_BusId bus)
{
    uint32_t st = pPmu->BUS_IDLE_ST;

    return !!((st & HAL_BIT(bus)) &&
              (st & HAL_BIT(bus + 16)));
}

static inline uint32_t PMU_PowerDomainSt(ePMU_PdId pd)
{
    return pPmu->PWRDN_ST & HAL_BIT(pd + 7) ?
           PMU_PD_OFF :
           PMU_PD_ON;
}

static void PMU_BusIdleReq(ePMU_BusId bus, ePMU_BusState state)
{
    uint32_t waitCnt = 0;
    uint32_t busIdleReq;

    busIdleReq = pPmu->SFT_CON;

    if (state) {
        busIdleReq |= HAL_BIT(bus);
    } else {
        busIdleReq &= ~HAL_BIT(bus);
    }

    pPmu->SFT_CON = busIdleReq;

    while (PMU_BusIdleSt(bus) != state &&
           waitCnt < MAX_IDLE_WAIT_COUNT) {
        HAL_DelayUs(1);
        waitCnt++;
    }

    if (PMU_BusIdleSt(bus) != state) {
        HAL_DBG_WRN("%s:idle_st=0x%lx, bus_id=%d\n",
                    __func__,
                    pPmu->BUS_IDLE_ST,
                    bus);
    }
}

static int PMU_PowerDomainCtr(ePMU_PdId pd, ePMU_PdState pdState)
{
    uint32_t loop = 0;
    uint32_t val = 0;
    int ret = 0;

    val = pPmu->SFT_CON;

    if (pdState == PMU_PD_OFF) {
        val |= HAL_BIT(pd + 8);
    } else {
        val &= ~HAL_BIT(pd + 8);
    }

    pPmu->SFT_CON = val;
    __DSB();

    while ((PMU_PowerDomainSt(pd) != pdState) &&
           (loop < MAX_PMU_ST_WAIT_COUNT)) {
        HAL_DelayUs(1);
        loop++;
    }

    if (PMU_PowerDomainSt(pd) != pdState) {
        HAL_DBG_WRN("%s: %d, %d, error!\n", __func__, pd, pdState);
        ret = HAL_ERROR;
    }

    return ret;
}

static int PMU_SetPowerDomain(ePMU_PdId pdId, ePMU_PdState pdState)
{
    uint32_t state;

    if (PMU_PowerDomainSt(pdId) == pdState) {
        goto out;
    }

    if (pdState == PMU_PD_ON) {
        PMU_PowerDomainCtr(pdId, pdState);
    }

    state = (pdState == PMU_PD_OFF) ? BUS_IDLE : BUS_ACTIVE;

    switch (pdId) {
    case PMU_PD_WLAN:
        PMU_BusIdleReq(PMU_BUS_WLAN, state);
        break;
    case PMU_PD_PERI:
        PMU_BusIdleReq(PMU_BUS_PERI, state);
        break;
    case PMU_PD_MCU:
        PMU_BusIdleReq(PMU_BUS_MCU, state);
        break;
    default:
        break;
    }

    if (pdState == PMU_PD_OFF) {
        PMU_PowerDomainCtr(pdId, pdState);
    }

out:

    return 0;
}

static void PMU_PowerDomainsSuspend(void)
{
    uint32_t i;

    for (i = 0; i < CRU_CLKGATES_CON_CNT; i++) {
        sleepData.cruClkGateCon[i] =
            pCru->CRU_CLKGATE_CON[i];
        pCru->CRU_CLKGATE_CON[i] = 0xffff0000;
    }

    pmuPowerdomainState = pPmu->PWRDN_ST;

    PMU_SetPowerDomain(PMU_PD_PERI, PMU_PD_OFF);
}

static void PMU_PowerDomainsResume(void)
{
    uint32_t i;

    if (!(pmuPowerdomainState & HAL_BIT(PMU_PD_PERI + 7))) {
        PMU_SetPowerDomain(PMU_PD_PERI, PMU_PD_ON);
    }

    for (i = 0; i < CRU_CLKGATES_CON_CNT; i++) {
        pCru->CRU_CLKGATE_CON[i] = 0xffff0000 | sleepData.cruClkGateCon[i];
    }
}

static void PM_TimerCh6WkupInit(uint32_t cnt)
{
    /* select timer1 clk from 32k or 40M */
    if (sleepConfig->suspendMode & SLP_PMU_PMUALIVE_32K) {
        pCru->CRU_CLKSEL_CON[15] = 0xcf008000;
    } else {
        pCru->CRU_CLKSEL_CON[15] = 0xcf004000;
    }

    ptimer6->CONTROLREG = 0;
    ptimer6->LOAD_COUNT[0] = cnt;
    ptimer6->LOAD_COUNT[1] = 0;
    ptimer6->CONTROLREG = 0x7;
}

static int PMU_SleepConfig(void)
{
    uint32_t pwrmodeCon;
    uint32_t clkFreqKhz;
    uint32_t mode = sleepConfig->suspendMode;
    uint32_t pmuWkupCfg = sleepConfig->suspendWkupSrc;

    pGrf->GRF_FAST_BOOT_ADDR = (uint32_t)HAL_CPU_DoResume;
    pGrf->GRF_FAST_BOOT = 1;

    pwrmodeCon =
        PMU_PWRMODE_CON_POWER_MODE_EN_MASK |
        PMU_PWRMODE_CON_CLR_WIFI_MASK |
        PMU_PWRMODE_CON_CLR_PERI_MASK |
        PMU_PWRMODE_CON_CLR_MCU_MASK |
        PMU_PWRMODE_CON_CLR_TOP_MASK |
        /* PMU_PWRMODE_CON_NOC_AUTO_CON_WIFI_MASK | */
        /* PMU_PWRMODE_CON_NOC_AUTO_CON_PERI_MASK | */
        /* PMU_PWRMODE_CON_NOC_AUTO_CON_MCU_MASK | */
        /* PMU_PWRMODE_CON_NOC_AUTO_CON_TOP_MASK | */
        PMU_PWRMODE_CON_CLK_CORE_SRC_GATE_MASK |
        /* PMU_PWRMODE_CON_CLK_TOP_SRC_GATE_MASK | */
        PMU_PWRMODE_CON_GLOBAL_INT_DISABLE_MASK |
        /* PMU_PWRMODE_CON_MCU_PD_EN_MASK | */
        /* PMU_PWRMODE_CON_RF_RESET_EN_MASK | */
        /* PMU_PWRMODE_CON_AON_RESET_EN_MASK | */
        /* PMU_PWRMODE_CON_RF_VDD18_DIS_MASK | */
        PMU_PWRMODE_CON_WAIT_WAKEUP_BEGIN_MASK |
        PMU_PWRMODE_CON_PLL_PD_EN_MASK |
        PMU_PWRMODE_CON_GPLL_PD_EN_MASK |
        PMU_PWRMODE_CON_VPLL_PD_EN_MASK
        /* | PMU_PWRMODE_CON_PMU_SLEEP_POL_MASK */
        /* | PMU_PWRMODE_CON_SRAM_RET_EN_MASK */
    ;

    if (mode & SLP_PMU_HW_ARM_PD) {
        pwrmodeCon |=
            PMU_PWRMODE_CON_MCU_PD_EN_MASK;
    }

    /* pmu debug pin iomux */
    if (mode & SLP_PMU_DBG) {
        sleepData.gpio0aIomuxL = pGrf->GPIO0A_IOMUX_L;
        pGrf->GPIO0A_IOMUX_L = VAL_MASK_WE(0xf << 12, 5 << 12);
        pPmu->INFO_TX_CON = 0xffff4001;
    }

    if (mode & SLP_PMU_DIS_OSC) {
        pwrmodeCon |= PMU_PWRMODE_CON_OSC_40M_GATE_EN_MASK |
                      PMU_PWRMODE_CON_OSC_DIS_MASK |
                      PMU_PWRMODE_CON_RF_VDD18_DIS_MASK;

        pPmu->SFT_CON |= /* | PMU_SFT_CON_LF_ENA_CFG_MASK */
                         PMU_SFT_CON_LF_MODE_CFG_MASK |
                         PMU_SFT_CON_PMU_LF_SWITCH_CFG_MASK |
                         HAL_BIT(14);
    }

    if (mode & SLP_PMU_PMUALIVE_32K) {
        pwrmodeCon |= PMU_PWRMODE_CON_PMU_USE_LF_MASK;
        clkFreqKhz = 32;
    } else {
        clkFreqKhz = 40000;
    }

    /* config timer6 wakeup */
    if (sleepConfig->suspendMode & SLP_TIME_OUT_WKUP) {
        PM_TimerCh6WkupInit(clkFreqKhz * CPU_SLEEP_TIMEOUT_TIME);
    }

    if (pmuWkupCfg & PMU_WAKEUP_CFG_TIMEOUT_EN_MASK) {
        pPmu->TIMEOUT_CNT = clkFreqKhz * CPU_SLEEP_TIMEOUT_TIME;
    }

    pPmu->OSC_CNT = clkFreqKhz * 32;

    /* Pmu's clk has switched to 24M back When pmu FSM counts
     * the follow counters, so we should use 24M to calculate
     * these counters.
     */
    pPmu->PLLLOCK_CNT = clkFreqKhz * 5;
    pPmu->PLLRST_CNT = clkFreqKhz * 2;

    if (mode & SLP_PMU_VAD_WKUP) {
        pPmu->PWRMODE_CON =
            pwrmodeCon & ~(PMU_PWRMODE_CON_CLR_TOP_MASK | PMU_PWRMODE_CON_GPLL_PD_EN_MASK);
        pPmu->RET_CON = 0x0;
    } else {
        pPmu->PWRMODE_CON = pwrmodeCon;
        pPmu->RET_CON = 0xff0;
    }

    pPmu->WAKEUP_CFG = pmuWkupCfg;

    return 0;
}

static int PMU_SleepRestore(void)
{
    uint32_t mode = sleepConfig->suspendMode;

    if (mode & SLP_PMU_DBG) {
        pGrf->GPIO0A_IOMUX_L = VAL_MASK_WE(0xf << 12, sleepData.gpio0aIomuxL);
    }

    pPmu->PWRMODE_CON = 0x02;
    pPmu->WAKEUP_CFG = 0;

    return 0;
}

static inline void SOC_PllWaitLock(uint32_t pllId)
{
    uint32_t delay = PLL_LOCKED_TIMEOUT;
    uint32_t pllCon1 = 0;

    while (delay > 0) {
        if (pllId == GPLL_ID) {
            pllCon1 = pCru->GPLL_CON[1];
        } else if (pllId == VPLL_ID) {
            pllCon1 = pCru->VPLL_CON[1];
        }

        if (pllCon1 & PLL_LOCK_MSK) {
            break;
        }
        delay--;
    }

    if (delay == 0) {
        HAL_DBG_ERR("Can't wait pll:%lu lock\n", pllId);
    }
}

static inline void SOC_PllSetMode(uint32_t pllId, uint32_t mode)
{
    pCru->MODE_CON00 =
        VAL_MASK_WE(0x3, mode) << pll_mode_shift(pllId);
}

static inline void SOC_PllSuspend(uint32_t pllId)
{
    if (pllId == GPLL_ID) {
        sleepData.cruPllsConSave[pllId] = pCru->GPLL_CON[1];
    } else if (pllId == VPLL_ID) {
        sleepData.cruPllsConSave[pllId] = pCru->VPLL_CON[1];
    }

    /* slow mode */
    SOC_PllSetMode(pllId, SLOW_MODE);
}

static inline void SOC_PllResume(uint32_t pllId)
{
    uint32_t mode;

    mode = (sleepData.cruModeSave >> pll_mode_shift(pllId)) & 0x3;
    /* if pll locked before suspend, we should wait atfer resume */
    if (sleepData.cruPllsConSave[pllId] & PLL_LOCK_MSK) {
        SOC_PllWaitLock(pllId);
    }

    SOC_PllSetMode(pllId, mode);
}

static void PM_PllsSuspend(void)
{
    sleepData.cruModeSave = pCru->MODE_CON00;
    sleepData.clkSelCon2 = pCru->CRU_CLKSEL_CON[2];

    SOC_PllSuspend(VPLL_ID);

    if (!(sleepConfig->suspendMode & SLP_PMU_VAD_WKUP)) {
        SOC_PllSuspend(GPLL_ID);
        /* core */
        pCru->CRU_CLKSEL_CON[2] =
            VAL_MASK_WE(0x1f1f, 0);
    }
}

static void PM_PllsResume(void)
{
    if (!(sleepConfig->suspendMode & SLP_PMU_VAD_WKUP)) {
        /* core */
        pCru->CRU_CLKSEL_CON[2] =
            VAL_MASK_WE(0x1f1f, sleepData.clkSelCon2);

        SOC_PllResume(GPLL_ID);
    }

    SOC_PllResume(VPLL_ID);
}

static void SOC_MpuDis(void)
{
    ARM_MPU_Disable();
}

static void SOC_MpuEn(void)
{
    ARM_MPU_Enable(sleepData.mpuCtrl);
}

static void SOC_ScbSuspend(void)
{
    int i;

    for (i = 0; i < 12; i++) {
        scbData.shp[i] = SCB->SHP[i];
    }

    scbData.aircr = SCB->AIRCR;
    scbData.ccr = SCB->CCR;
    scbData.shcsr = SCB->SHCSR;
    scbData.cpacr = SCB->CPACR;
}

static void SOC_ScbResume(void)
{
    int i;

    for (i = 0; i < 12; i++) {
        SCB->SHP[i] = scbData.shp[i];
    }

    SCB->AIRCR = (scbData.aircr & 0x700) | 0x05fa0000;
    SCB->CCR = scbData.ccr;
    SCB->CPACR = scbData.cpacr;
    SCB->SHCSR = scbData.shcsr & 0x70000;
}

#ifdef HAL_SYSTICK_MODULE_ENABLED
static void SOC_SystickSuspend(void)
{
    systickData.ctrl = SysTick->CTRL;
    systickData.load = SysTick->LOAD;

    SysTick->CTRL &= (~SysTick_CTRL_ENABLE_Msk);
}

static void SOC_SystickResume(void)
{
    SysTick->LOAD = systickData.load;
    SysTick->VAL = 0;
    SysTick->CTRL = systickData.ctrl & 0x7;
}
#endif

static int SOC_SuspendEnter(uint32_t flag)
{
    PM_PutChar('a');

    HAL_DCACHE_CleanInvalidate();

    __WFI();
    PM_PutChar('b');

    HAL_CPU_DoResume();
    PM_PutChar('c');

    return HAL_OK;
}
#endif

/** @} */
/********************* Public Function Definition ****************************/

int HAL_SYS_Suspend(struct PM_SUSPEND_INFO *suspendInfo)
{
#ifdef HAL_PM_CPU_SLEEP_MODULE_ENABLED
    uint32_t mode;

    sleepConfig = HAL_SYS_GetSuspendConfig();

    PM_ModeInit();

    mode = sleepConfig->suspendMode;

    PM_UartSave();
    PM_UartInit();

    PMU_SleepEnterInfo(mode);
    PM_PutChar('\n');
    PM_PutChar('0');

#ifdef HAL_SYSTICK_MODULE_ENABLED
    SOC_SystickSuspend();
    PM_PutChar('1');
#endif

    PMU_PowerDomainsSuspend();
    PM_PutChar('2');

    SOC_ClkGateSuspend();
    PM_PutChar('3');

    if (mode & SLP_PMU_PMUALIVE_32K) {
        PMU_Clk32kConfig();
    }
    PM_PutChar('4');

    PMU_SleepConfig();
    PM_PutChar('5');

    PM_PllsSuspend();
    PM_PutChar('6');

    PMU_SleepModeInfo();

    SOC_MpuDis();
    PM_PutChar('7');

    if (mode & SLP_PMU_HW_ARM_PD) {
        HAL_NVIC_SuspendSave();
        PM_PutChar('8');
        SOC_ScbSuspend();
        PM_PutChar('9');
        PM_SystimerSave();
    }

    PM_PutChar('S');
    HAL_CPU_SuspendEnter(0, SOC_SuspendEnter);

    if (mode & SLP_PMU_HW_ARM_PD) {
        PM_UartInit();
        PM_PutChar('a');
        PM_SystimerRestore();

        HAL_DCACHE_Enable();
        HAL_ICACHE_Enable();
        PM_PutChar('9');

        SOC_ScbResume();
        PM_PutChar('8');

        HAL_NVIC_ResumeRestore();
        PM_PutChar('7');
    } else {
        SOC_MpuEn();
    }

    PM_PllsResume();
    PM_PutChar('6');

    PMU_SleepRestore();
    PM_PutChar('5');

    if (mode & SLP_PMU_PMUALIVE_32K) {
        PMU_Clk32kConfigRestore();
        PM_PutChar('4');
    }

    SOC_ClkGateResume();
    PM_PutChar('3');

    PMU_PowerDomainsResume();
    PM_PutChar('2');

#ifdef HAL_SYSTICK_MODULE_ENABLED
    SOC_SystickResume();
    PM_PutChar('1');
#endif

    PM_PutChar('0');
    PM_PutChar('\n');
    PMU_SleepFinishInfo();

    PM_UartRestore();
#endif

    return HAL_OK;
}
#endif

/** @} */

/** @} */

#endif /* RKMCU_RK2206 */
