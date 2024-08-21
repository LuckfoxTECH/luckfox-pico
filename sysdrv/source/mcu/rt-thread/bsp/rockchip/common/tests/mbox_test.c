/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    mbox_test.c
  * @author  Frank Wang
  * @version V0.1
  * @date    15-Aug-2019
  * @brief   Test for MBox HAL
  *
  ******************************************************************************
  */

#include <rtthread.h>

#ifdef RT_USING_COMMON_TEST_MBOX
#include <errno.h>
#include <stdbool.h>
#include <rtdevice.h>
#include <rthw.h>
#include "hal_base.h"

#define MBOX_TEST_ARGC      5U
#define MBOX_TEST_CMD       0x94949490U
#define MBOX_TEST_DATA      0xa5a5a5a0U
#define MBOX_TEST_DELAY     (3 * RT_TICK_PER_SECOND) /* 3 Sec */

#define IS_BOOL_VALUE(v)    (v == 0 || v == 1)

static void mbox_test_callback(struct MBOX_CMD_DAT *msg, void *arg);

struct mbox_tdev
{
    struct MBOX_REG *reg;
    struct MBOX_CLIENT *clt[MBOX_CHAN_CNT];
    uint8_t single_irq;
    uint8_t a2b;
};

static struct mbox_tdev g_mbox_tdev;

#define MBOX_TEST_ISR_INSTALL(ID) \
{ \
    rt_hw_interrupt_install(g_mbox_tdev.clt[ID]->irq, mbox_test_isr##ID, NULL, NULL); \
    rt_hw_interrupt_umask(g_mbox_tdev.clt[ID]->irq); \
}

#define MBOX_TEST_ISR_DEFINE(ID) \
static void mbox_test_isr##ID(int irq, void *param) \
{ \
    rt_interrupt_enter(); \
    rt_kprintf("IRQ: %d\n", g_mbox_tdev.clt[ID]->irq); \
    HAL_MBOX_IrqHandler(g_mbox_tdev.clt[ID]->irq, g_mbox_tdev.reg); \
    rt_interrupt_leave(); \
}

MBOX_TEST_ISR_DEFINE(0)
MBOX_TEST_ISR_DEFINE(1)
MBOX_TEST_ISR_DEFINE(2)
MBOX_TEST_ISR_DEFINE(3)

static struct MBOX_CLIENT mbox_client[MBOX_CHAN_CNT] =
{
    {"ch0", 0, mbox_test_callback, (void *)MBOX_CH_0},
    {"ch1", 0, mbox_test_callback, (void *)MBOX_CH_1},
    {"ch2", 0, mbox_test_callback, (void *)MBOX_CH_2},
    {"ch3", 0, mbox_test_callback, (void *)MBOX_CH_3},
};

static void mbox_test_show_usage()
{
    /* mbox_test mbox_reg mbox_chan_irq_start mbox_is_single_irq mbox_is_a2b */
    rt_kprintf("Usage: \n");
    rt_kprintf("mbox_test 0x44050000 10, 1, 1\n");
    rt_kprintf("means: \n");
    rt_kprintf("mbox ip address: 0x44050000\n");
    rt_kprintf("mbox channel irq number start: 10\n");
    rt_kprintf("mbox channel single irq flag: 1 means single, 0 means multi\n");
    rt_kprintf("mbox send direction: 1 means A2B, 0 means B2A\n");
}

static void mbox_test_callback(struct MBOX_CMD_DAT *msg, void *arg)
{
    rt_kprintf("RX: cmd=0x%x data=0x%x from chan %d\n", msg->CMD, msg->DATA, (eMBOX_CH)arg);
}

static rt_err_t mbox_test_send(struct mbox_tdev *mbox, eMBOX_CH chan,
                               uint32_t cmd, uint32_t data)
{
    struct MBOX_CMD_DAT cmdData;

    cmdData.CMD  = cmd;
    cmdData.DATA = data;

    return HAL_MBOX_SendMsg2(mbox->reg, chan, &cmdData, !mbox->a2b);
}

static void mbox_test_exec(struct mbox_tdev *mbox)
{
    int chan;
    uint32_t cmd, data;

    rt_kprintf("mbox test start, direction: %s\n", !mbox->a2b ? "A2B" : "B2A");

    for (chan = 0; chan < MBOX_CHAN_CNT; chan++)
    {
        cmd  = MBOX_TEST_CMD + chan;
        data = MBOX_TEST_DATA + chan;

        rt_kprintf("TX: cmd=0x%x data=0x%x to chan %d\n", cmd, data, chan);

        mbox_test_send(mbox, chan, cmd, data);
        rt_thread_delay(MBOX_TEST_DELAY);
    }

    rt_kprintf("mbox test end\n");
}

static int mbox_test_init(struct mbox_tdev *mbox)
{
    uint32_t chan;
    int ret = 0;

    /* Invoke HAL to init */
    HAL_MBOX_Init(mbox->reg, mbox->a2b);

    for (chan = 0; chan < MBOX_CHAN_CNT; chan++)
    {
        ret = HAL_MBOX_RegisterClient(mbox->reg, chan,
                                      mbox->clt[chan]);
        if (ret)
        {
            rt_kprintf("Mbox client register failed, ret=%d\n", ret);
            return ret;
        }
    }

    MBOX_TEST_ISR_INSTALL(0);

    if (!mbox->single_irq)
    {
        MBOX_TEST_ISR_INSTALL(1);
        MBOX_TEST_ISR_INSTALL(2);
        MBOX_TEST_ISR_INSTALL(3);
    }

    return ret;
}

static int mbox_test_deinit(struct mbox_tdev *mbox)
{
    uint32_t chan;
    int ret = 0;

    for (chan = 0; chan < MBOX_CHAN_CNT; chan++)
    {
        ret = HAL_MBOX_UnregisterClient(mbox->reg, chan,
                                        mbox->clt[chan]);
        if (ret)
        {
            rt_kprintf("Mbox client unregister failed, ret=%d\n", ret);
            goto out;
        }
    }

    ret = HAL_MBOX_DeInit(mbox->reg);
    if (ret)
        rt_kprintf("Mbox deinit failed, ret=%d\n", ret);

out:
    return ret;
}

static int mbox_test_parse(struct mbox_tdev *mbox, int argc, char **argv)
{
    uint32_t reg_addr, irqn, chan;
    uint8_t single_irq, is_a2b;

    if (argc < MBOX_TEST_ARGC)
        goto err;

    /* Parse MBox base address */
    reg_addr = (uint32_t)strtoll(argv[1], NULL, 16);
    if (!reg_addr || !IS_MBOX_INSTANCE((struct MBOX_REG *)reg_addr))
        goto err;

    mbox->reg = (struct MBOX_REG *)reg_addr;

    /* Parse MBox channel IRQ */
    irqn = strtol(argv[2], NULL, 10);
    if (irqn < 0)
        goto err;

    single_irq = strtol(argv[3], NULL, 10);
    if (!IS_BOOL_VALUE(single_irq))
        goto err;

    mbox->single_irq = single_irq;

    for (chan = 0; chan < MBOX_CHAN_CNT; chan++)
    {
        mbox->clt[chan] = &mbox_client[chan];

        if (single_irq)
        {
            mbox_client[chan].irq = irqn;
            continue;
        }

        mbox_client[chan].irq = irqn + chan;
    }

    /* Parse MBox direction */
    is_a2b = strtol(argv[4], NULL, 10);
    if (!IS_BOOL_VALUE(is_a2b))
        goto err;

    mbox->a2b = !is_a2b; /* loopback test */
    return 0;

err:
    mbox_test_show_usage();
    return -EINVAL;
}

static void mbox_test(int argc, char **argv)
{
    int ret;

    ret = mbox_test_parse(&g_mbox_tdev, argc, argv);
    if (ret)
        goto out;

    ret = mbox_test_init(&g_mbox_tdev);
    if (ret)
        goto out;

    mbox_test_exec(&g_mbox_tdev);

    mbox_test_deinit(&g_mbox_tdev);

out:
    return;
}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(mbox_test, mbox test cmd);
#endif

#endif
