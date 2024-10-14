/**
  * Copyright (c) 2020 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    timer_test.c
  * @version V1.0
  * @brief   timer test
  *
  * Change Logs:
  * Date           Author          Notes
  * 2020-07-01     Dingqiang Lin   the first version
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>
#include <rthw.h>
#include <rtdef.h>

#ifdef RT_USING_COMMON_TEST_TIMER

#include "hal_base.h"

static void timer_test_show_usage(void)
{
    rt_kprintf("1. timer_test test timerN\n");
    rt_kprintf("2. timer_test all\n");
    rt_kprintf("like:\n");
    rt_kprintf("\ttimer_test test 0\n");
    rt_kprintf("\ttimer_test all\n");
}

static uint32_t isr_active, time_out;

void timer_isr_helper(uint32_t dev_id);

#ifdef TIMER0
void timer_isr0(void)
{
    timer_isr_helper(0);
}
#endif

#ifdef TIMER1
void timer_isr1(void)
{
    timer_isr_helper(1);
}
#endif

#ifdef TIMER2
void timer_isr2(void)
{
    timer_isr_helper(2);
}
#endif

#ifdef TIMER3
void timer_isr3(void)
{
    timer_isr_helper(3);
}
#endif

#ifdef TIMER4
void timer_isr4(void)
{
    timer_isr_helper(4);
}
#endif

#ifdef TIMER5
void timer_isr5(void)
{
    timer_isr_helper(5);
}
#endif

#ifdef TIMER6
void timer_isr6(void)
{
    timer_isr_helper(6);
}
#endif

typedef struct _TIMER_DEV
{
    const uint32_t irqNum;
    struct TIMER_REG *pReg;
    void (*isr)(void);

} TIMER_DEV;

#define DEFINE_TIMER_DEV(ID) \
{ \
    .irqNum = TIMER##ID##_IRQn, \
    .pReg =  TIMER##ID, \
    .isr = timer_isr##ID, \
}

static TIMER_DEV s_timer[] =
{
#ifdef TIMER0
    DEFINE_TIMER_DEV(0),
#endif
#ifdef TIMER1
    DEFINE_TIMER_DEV(1),
#endif
#ifdef TIMER2
    DEFINE_TIMER_DEV(2),
#endif
#ifdef TIMER3
    DEFINE_TIMER_DEV(3),
#endif
#ifdef TIMER4
    DEFINE_TIMER_DEV(4),
#endif
#ifdef TIMER5
    DEFINE_TIMER_DEV(5),
#endif
#ifdef TIMER6
    DEFINE_TIMER_DEV(6),
#endif
};

#define TIMER_CHAN_CNT (HAL_ARRAY_SIZE(s_timer))

void timer_isr_helper(uint32_t dev_id)
{
    if (HAL_TIMER_ClrInt(s_timer[dev_id].pReg))
        time_out++;
    isr_active++;
}

static uint64_t timer_get_reload_num(struct TIMER_REG *pReg)
{
    uint64_t loadCount = 0;

    loadCount = (((uint64_t)pReg->LOAD_COUNT[1]) << 32) | (pReg->LOAD_COUNT[0] & 0xffffffff);

    return loadCount;
}

static HAL_Status timer_set_reload_num(struct TIMER_REG *pReg, uint64_t currentVal)
{
    pReg->LOAD_COUNT[0] = (currentVal & 0xffffffff);
    pReg->LOAD_COUNT[1] = ((currentVal >> 32) & 0xffffffff);

    return HAL_OK;
}


void timer_gating_enable(void)
{
#if defined(RKMCU_RK2206)
    CRU->CRU_CLKGATE_CON[5] = ((0x3f << 9) << 16) | (0x3f << 9);
#elif defined(SOC_SWALLOW)
    CRU->CRU_CLKGATE_CON[5] = ((0x3f << 6) << 16) | (0x3f << 6);
#elif defined(BSP_RK3308)
    CRU->CRU_CLKGATE_CON[3] = ((0x3f << 10) << 16) | (0x3f << 10);
#else
    RT_ASSERT(0);
#endif
}

void timer_gating_all_enable(void)
{
#if defined(RKMCU_RK2206)
    CRU->CRU_CLKGATE_CON[5] = ((1 << 8) << 16) | (1 << 8);
#elif defined(SOC_SWALLOW)
    CRU->CRU_CLKGATE_CON[5] = ((1 << 5) << 16) | (1 << 5);
#elif defined(BSP_RK3308)
    CRU->CRU_CLKGATE_CON[3] = ((0x3f << 10) << 16) | (0x3f << 10);
#else
    RT_ASSERT(0);
#endif
}

void timer_gating_disable(void)
{
#if defined(RKMCU_RK2206)
    CRU->CRU_CLKGATE_CON[5] = ((0x3f << 9) << 16) | (0 << 9);
#elif defined(SOC_SWALLOW)
    CRU->CRU_CLKGATE_CON[5] = ((0x3f << 6) << 16) | (0 << 6);
#elif defined(BSP_RK3308)
    CRU->CRU_CLKGATE_CON[3] = ((0x3f << 10) << 16) | (0 << 10);
#else
    RT_ASSERT(0);
#endif
}

void timer_gating_all_disable(void)
{
#if defined(RKMCU_RK2206)
    CRU->CRU_CLKGATE_CON[5] = ((1 << 8) << 16) | (0 << 8);
#elif defined(SOC_SWALLOW)
    CRU->CRU_CLKGATE_CON[5] = ((1 << 5) << 16) | (0 << 5);
#elif defined(BSP_RK3308)
    CRU->CRU_CLKGATE_CON[3] = ((0x3f << 10) << 16) | (0 << 10);
#else
    RT_ASSERT(0);
#endif
}

void timer_cru_softrst(void)
{
#if defined(RKMCU_RK2206)
    CRU->CRU_SOFTRST_CON[3] = ((1 << 7) << 16) | (1 << 7);
#elif defined(SOC_SWALLOW)
    CRU->CRU_SOFTRST_CON[3] = ((1 << 9) << 16) | (1 << 9);
#elif defined(BSP_RK3308)
    CRU->CRU_SOFTRST_CON[4] = ((0x3f << 9) << 16) | (0x3f << 9);
#else
    RT_ASSERT(0);
#endif
}

void timer_cru_softrst_remove(void)
{
#if defined(RKMCU_RK2206)
    CRU->CRU_SOFTRST_CON[3] = ((1 << 7) << 16) | (0 << 7);
#elif defined(SOC_SWALLOW)
    CRU->CRU_SOFTRST_CON[3] = ((1 << 9) << 16) | (0 << 9);
#elif defined(BSP_RK3308)
    CRU->CRU_SOFTRST_CON[4] = ((0x3f << 9) << 16) | (0 << 9);
#else
    RT_ASSERT(0);
#endif
}

int32_t timer_start_stop(struct TIMER_REG *timer_dev)
{
    uint64_t count, count1;

    timer_gating_disable();
    timer_gating_all_disable();
    timer_cru_softrst_remove();

    /* test timer_dev stop in normalc mode */
    isr_active = 0;
    time_out = 0;
    HAL_TIMER_Init(timer_dev, TIMER_FREE_RUNNING);
    HAL_TIMER_SetCount(timer_dev, (uint64_t)PLL_INPUT_OSC_RATE); /* Ms count */
    HAL_TIMER_Start(timer_dev);
    count = HAL_TIMER_GetCount(timer_dev);
    HAL_DelayMs(1);
    if (count < HAL_TIMER_GetCount(timer_dev))
        rt_kprintf("[increasing]\n");
    else
        rt_kprintf("[decreasing]\n");
    rt_kprintf("[set finish value]\n");
    count = HAL_TIMER_GetCount(timer_dev);
    RT_ASSERT(count != HAL_TIMER_GetCount(timer_dev)); /* test start*/
    HAL_TIMER_Stop(timer_dev);
    count = HAL_TIMER_GetCount(timer_dev);
    RT_ASSERT(count == HAL_TIMER_GetCount(timer_dev)); /* test stop*/
    rt_kprintf("[support irq mask]\n");
    RT_ASSERT(isr_active == 0 && isr_active == 0);

    /* test timer_dev stop in IT mode */
    isr_active = 0;
    time_out = 0;
    HAL_TIMER_Init(timer_dev, TIMER_FREE_RUNNING);
    HAL_TIMER_SetCount(timer_dev, HAL_DivU64((uint64_t)PLL_INPUT_OSC_RATE, 1000)); /* Ms count */
    rt_kprintf("1\n");
    HAL_TIMER_Start_IT(timer_dev);
    HAL_DelayMs(10000);
    count = HAL_TIMER_GetCount(timer_dev);
    RT_ASSERT(count != HAL_TIMER_GetCount(timer_dev)); /* test start*/
    HAL_TIMER_Stop_IT(timer_dev);
    count = HAL_TIMER_GetCount(timer_dev);
    RT_ASSERT(count == HAL_TIMER_GetCount(timer_dev)); /* test stop*/
    rt_kprintf("2\n");
    rt_kprintf("[support irq, irq unmask, irq clear]\nisr_active %lu\n", isr_active);
    RT_ASSERT(isr_active >= 10000 && isr_active <= 10050 && time_out == 0);

    rt_kprintf("[set current value]\n");
    count = 0x5a5a5a5aa5a5a5a5;
    timer_set_reload_num(timer_dev, count);
    count1 = timer_get_reload_num(timer_dev);
    RT_ASSERT(count == count1);

    return 0;
}

int32_t timer_cru_test(struct TIMER_REG *timer_dev)
{
    uint64_t count;

    timer_gating_disable();
    timer_gating_all_disable();

    /* test timer_dev stop in normalc mode */
    HAL_TIMER_Init(timer_dev, TIMER_FREE_RUNNING);
    HAL_TIMER_SetCount(timer_dev, (uint64_t)PLL_INPUT_OSC_RATE);
    HAL_TIMER_Start(timer_dev);

    rt_kprintf("[gate one timer]\n");
    count = HAL_TIMER_GetCount(timer_dev);
    RT_ASSERT(count != HAL_TIMER_GetCount(timer_dev));
    timer_gating_enable();
    count = HAL_TIMER_GetCount(timer_dev);
    RT_ASSERT(count == HAL_TIMER_GetCount(timer_dev));
    timer_gating_disable();

    rt_kprintf("[gate all timer]\n");
    count = HAL_TIMER_GetCount(timer_dev);
    RT_ASSERT(count != HAL_TIMER_GetCount(timer_dev));
    timer_gating_all_enable();
    count = HAL_TIMER_GetCount(timer_dev);
    RT_ASSERT(count == HAL_TIMER_GetCount(timer_dev));
    timer_gating_all_disable();
    HAL_TIMER_Stop(timer_dev);

    HAL_TIMER_Init(timer_dev, TIMER_FREE_RUNNING);
    HAL_TIMER_SetCount(timer_dev, (uint64_t)PLL_INPUT_OSC_RATE);
    HAL_TIMER_Start(timer_dev);
    rt_kprintf("[softrst all timer]\n");
    timer_cru_softrst();
    RT_ASSERT(HAL_TIMER_GetCount(timer_dev) == 0);
    timer_cru_softrst_remove();

#ifdef SYS_TIMER
    HAL_TIMER_SysTimerInit(SYS_TIMER);
#endif

    return 0;
}

static void timer_test_loop(int32_t num)
{
    if (num >= TIMER_CHAN_CNT)
    {
        rt_kprintf("only %d timer\n", TIMER_CHAN_CNT);
        return;
    }

    if (s_timer[num].pReg == SYS_TIMER)
    {
        rt_kprintf("test timer should not equals sys_timer\n");
        return;
    }

    rt_kprintf("TIMER%ld: %s begin\n", num, __func__);

    rt_hw_interrupt_install(s_timer[num].irqNum, (void *)s_timer[num].isr, RT_NULL, RT_NULL);
    rt_hw_interrupt_umask(s_timer[num].irqNum);
    if (timer_start_stop(s_timer[num].pReg) == 0)
        rt_kprintf("TIMER%ld: function pass\n", num);

    if (timer_cru_test(s_timer[num].pReg) == 0)
        rt_kprintf("TIMER%ld: cru pass\n\n", num);
}

void hw_timer_test(int32_t argc, char **argv)
{
    char *cmd;

    if (argc < 2)
        goto out;

    cmd = argv[1];
    if (!rt_strcmp(cmd, "test"))
    {
        int32_t num;

        if (argc < 3)
            goto out;

        num = atoi(argv[2]);
        timer_test_loop(num);
    }
    else if (!rt_strcmp(cmd, "all"))
    {
        int32_t i;

        for (i = 0; i < HAL_ARRAY_SIZE(s_timer); i++)
            timer_test_loop(i);
    }
    else
    {
        goto out;
    }

    return;
out:
    timer_test_show_usage();
    return;
}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(hw_timer_test, timer test cmd);
#endif

#endif
