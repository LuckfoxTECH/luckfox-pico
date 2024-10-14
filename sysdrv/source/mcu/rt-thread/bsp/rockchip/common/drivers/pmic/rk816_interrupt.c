/**
  * Copyright (c) 2020 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    rk816_interrupt.c
  * @version V0.1
  * @brief   interrupt interface
  *
  * Change Logs:
  * Date           Author          Notes
  * 2020-04-12     Elaine Zhang     first implementation
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>
#ifdef RT_USING_PMIC_RK816
#include "hal_base.h"
#include "drv_pmic.h"
#include "rk816.h"
#include "rk816_interrupt.h"
#include "board.h"

#define WORK_QUEUE_STACK_SIZE    512
#define WORK_QUEUE_PRIORITY      0

static void (*rk816_isr_hook)(void);
struct rt_work rk816_int_work;
struct rt_workqueue *rk816_int_workqueue;

struct rt_messagequeue *rk816_mq;
static struct pwr_i2c_desc *int_i2c;
struct rt_i2c_bus_device *bus;
rt_uint8_t addr;

struct rk816_int_entry
{
    rk816_int routine;
    void *args;
};

static struct rk816_int_entry int_ctrl[RK816_INT_NUM];

void rk816_irq_register(int irq, rk816_int isr, void *args)
{
    rt_uint8_t base = 0;

    if (irq >= 16)
    {
        base = 0x5;
    }
    else if (irq >= 8)
    {
        base = 0x3;
    }
    /* enable interrupt */
    pmic_update_bits(bus, addr, INT_MSK_REG1 + base, 1 << (irq % 8), 0);

    int_ctrl[irq].routine = isr;
    int_ctrl[irq].args = args;
}

static void rk816_clr_int(void)
{
    unsigned int stat1_bit = 0,  stat2_bit = 0,  stat3_bit = 0;

    /* get interrupt status and clear them */
    stat1_bit = pmic_read(bus, addr, INT_STS_REG1);
    pmic_write(bus, addr, INT_STS_REG1, stat1_bit);
    stat2_bit = pmic_read(bus, addr, INT_STS_REG2);
    pmic_write(bus, addr, INT_STS_REG2, stat2_bit);
    stat3_bit = pmic_read(bus, addr, INT_STS_REG3);
    pmic_write(bus, addr, INT_STS_REG3, stat3_bit);

    /* Handle possible secondary interrupts */
    if (stat3_bit & PLUG_OUT_IM)
    {
        RT_ASSERT(int_ctrl[RK816_PLUG_OUT_INT_NUM].routine != RT_NULL);
        int_ctrl[RK816_PLUG_OUT_INT_NUM].routine(int_ctrl[RK816_PLUG_OUT_INT_NUM].args);
    }

    if (stat3_bit & PLUG_IN_IM)
    {
        RT_ASSERT(int_ctrl[RK816_PLUG_IN_INT_NUM].routine != RT_NULL);
        int_ctrl[RK816_PLUG_IN_INT_NUM].routine(int_ctrl[RK816_PLUG_IN_INT_NUM].args);
    }

    if (stat1_bit & POWERON_RISE_IM)
    {
        RT_ASSERT(int_ctrl[RK816_PWRON_RISE_INT_NUM].routine != RT_NULL);
        int_ctrl[RK816_PWRON_RISE_INT_NUM].routine(int_ctrl[RK816_PWRON_RISE_INT_NUM].args);
    }

    if (stat1_bit & POWERON_FALL_IM)
    {
        RT_ASSERT(int_ctrl[RK816_PWRON_FALL_INT_NUM].routine != RT_NULL);
        int_ctrl[RK816_PWRON_FALL_INT_NUM].routine(int_ctrl[RK816_PWRON_FALL_INT_NUM].args);
    }

    if (stat2_bit & POWERON_RTC_ALARM_IM)
    {
        RT_ASSERT(int_ctrl[RK816_RTC_ALARM_INT_NUM].routine != RT_NULL);
        int_ctrl[RK816_RTC_ALARM_INT_NUM].routine(int_ctrl[RK816_RTC_ALARM_INT_NUM].args);
    }

    if (stat2_bit & PWRON_LP_INT)
    {
        RT_ASSERT(int_ctrl[RK816_PWRON_LP_INT_NUM].routine != RT_NULL);
        int_ctrl[RK816_PWRON_LP_INT_NUM].routine(int_ctrl[RK816_PWRON_LP_INT_NUM].args);
    }
}

rt_err_t rk816_isr_sethook(void (*hook)(void))
{
    rt_base_t level;

    /* disable interrupt */
    level = rt_hw_interrupt_disable();

    rk816_isr_hook = hook;

    /* enable interrupt */
    rt_hw_interrupt_enable(level);

    return RT_EOK;
}

static void rk816_isr(void *args)
{
    rt_uint8_t rk816_sbufftemp;

    rt_pm_request(1);
    rk816_sbufftemp = 0x01;
    /* send the message */
    rt_mq_send(rk816_mq, &rk816_sbufftemp, 1);
    if (rk816_isr_hook != RT_NULL)
        rk816_isr_hook();
}

static void rk816_isr_thread(void *p)
{
    rt_uint8_t rk816_rbufftemp;

    while (1)
    {
        /* wait pdm message forever */
        rt_mq_recv(rk816_mq, &rk816_rbufftemp, 1, RT_WAITING_FOREVER);
        rk816_clr_int();
        rt_pm_release(1);
    }
}

static int rk816_thread_init(void)
{
    rt_thread_t rk816_thread = rt_thread_create("rk816_isr", rk816_isr_thread, RT_NULL, 1024, 5, 10);
    if (rk816_thread == RT_NULL)
    {
        rt_kprintf("thread create failed\n");
        return RT_EOK;
    }

    rk816_mq = rt_mq_create("rk816_mq", 1, 1, RT_IPC_FLAG_FIFO);
    RT_ASSERT(rk816_mq != RT_NULL);
    rt_thread_startup(rk816_thread);

    return 0;
}

int rt_rk816_interrupt_init(void)
{

    int_i2c = pmic_get_i2c_desc();
    if (int_i2c == RT_NULL)
    {
        return -RT_EINVAL;
    }

    bus = int_i2c->bus;
    addr = int_i2c->addr;

    /* get plug status */
    if (pmic_read(bus, addr, INT_STS_REG3) & PLUG_IN_IM ||
            pmic_read(bus, addr, SUP_STS_REG) & USB_EXIST)
    {
        plug_in_isr(RT_NULL);
    }

    if (pmic_read(bus, addr, INT_STS_REG3) & PLUG_OUT_IM)
    {
        plug_out_isr(RT_NULL);
    }

    /* mask all interrupt except rtc */
    pmic_write(bus, addr, INT_MSK_REG1, 0xff);
    pmic_write(bus, addr, INT_MSK_REG2, 0x9f);
    pmic_write(bus, addr, INT_MSK_REG3, 0xff);

    /* clear all interrupt status */
    pmic_write(bus, addr, INT_STS_REG1, 0xff);
    pmic_write(bus, addr, INT_STS_REG2, 0xff);
    pmic_write(bus, addr, INT_STS_REG3, 0xff);

    rk816_thread_init();
    rk816_isr_hook = RT_NULL;

    rk816_irq_register(RK816_PLUG_IN_INT_NUM, plug_in_isr, RT_NULL);
    rk816_irq_register(RK816_PLUG_OUT_INT_NUM, plug_out_isr, RT_NULL);
    rk816_irq_register(RK816_PWRON_RISE_INT_NUM, poweron_rise_isr, RT_NULL);
    rk816_irq_register(RK816_PWRON_FALL_INT_NUM, poweron_fall_isr, RT_NULL);
    rk816_irq_register(RK816_PWRON_LP_INT_NUM, poweron_long_isr, RT_NULL);

    rt_pin_mode(PMIC_INT_PIN, PIN_MODE_INPUT);
    rt_pin_attach_irq(PMIC_INT_PIN, PIN_IRQ_MODE_FALLING, rk816_isr, NULL);
    rt_pin_irq_enable(PMIC_INT_PIN, RT_TRUE);

    return 0;
}
INIT_DEVICE_EXPORT(rt_rk816_interrupt_init);
#endif
