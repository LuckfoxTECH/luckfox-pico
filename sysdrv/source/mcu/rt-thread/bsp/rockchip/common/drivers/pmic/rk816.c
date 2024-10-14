/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    rk816.c
  * @version V0.1
  * @brief   rk816 interface
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-12-26     Joseph Chen     first implementation
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>

#ifdef RT_USING_PMIC_RK816
#include "drv_pmic.h"
#include "rk816.h"

/*
 * bat charge current:
 *               10mR:    [2000, 2400, 2800, 3200, 3600, 4000, 4500, 4800]
 *               20mR:    [1000, 1200, 1400, 1600, 1800, 2000, 2250, 2400]
 *               40mR:    [500,   600,  700,  800,  900, 1000, 1125, 1200]
 *              100mR:    [200,   240,  280,  320,  360,  400,  450,  480]
 *
 * bat charge voltage:    [4050, 4100, 4150, 4200, 4250, 4300, 4350]
 *
 * src input current:     [450, 80, 850, 1000, 1250, 1500, 1750, 2000]
 *
 * finish charge current:
 *               10mR:    [300, 400, 600, 800]
 *               20mR:    [150, 200, 300, 400]
 *               40mR:    [75,  100, 150, 200]
 *              100mR:    [30,   40,  60,  80]
 *
 *
 * NOTE:
 *
 * 1. Developer should [never update] the following array definition, they
 *    are the values of 20mR of sample resistor.
 *
 * 2. Developer pass the value showing on the above to rt_device_control()
 *    from user space.
 *
 *  Example: set 200ma charge current on board with 100mR sample resistor:
 *
 *  void foo(void)
 *  {
 *      rt_uint32_t charge_current= 200;
 *      ......
 *      ret = rt_device_control(device, RT_DEVICE_CTRL_CHAGER_BATCUR_SET, &charge_current);
 *      ......
 *  }
 *
 */
const rt_int16_t bat_vol_level[7] =
{
    4050, 4100, 4150, 4200, 4250, 4300, 4350
};

const rt_int16_t bat_cur_level[8] =
{
    1000, 1200, 1400, 1600, 1800, 2000, 2250, 2400
};

const rt_int16_t fns_cur_level[4] =
{
    150, 200, 300, 400,
};

const rt_int16_t src_cur_level[8] =
{
    450, 80, 850, 1000, 1250, 1500, 1750, 2000
};

static const struct reg_data init_reg[] =
{
    { BAT_CTRL_REG, BAT_DIS_ILIM_MASK, BAT_DIS_ILIM_DIS, },
};

static int rt_rk816_init(void)
{
    struct pwr_i2c_desc *i2c;
    rt_int32_t val;
    int i, ret;

    i2c = pmic_get_i2c_desc();
    if (i2c == RT_NULL)
        return -RT_EINVAL;

    rt_kprintf("rk816 source: on=0x%02x, off=0x%02x\n",
               pmic_read(i2c->bus, i2c->addr, ON_SOURCE_REG),
               pmic_read(i2c->bus, i2c->addr, OFF_SOURCE_REG));

    for (i = 0; i < ARRAY_SIZE(init_reg); i++)
    {
        val = pmic_read(i2c->bus, i2c->addr, init_reg[i].reg);
        if (val < 0)
        {
            rt_kprintf("failed to read reg: 0x%x\n", init_reg[i].reg);
            return val;
        }

        val &= ~init_reg[i].mask;
        val |= init_reg[i].val;
        ret = pmic_write(i2c->bus, i2c->addr, init_reg[i].reg, val);
        if (ret < 0)
        {
            rt_kprintf("failed to write reg: 0x%x\n", init_reg[i].reg);
            return ret;
        }
    }

    return RT_EOK;
}

INIT_DEVICE_EXPORT(rt_rk816_init);

#if defined(RT_USING_FINSH) && defined(FINSH_USING_MSH)
#define MAX_REG(x)      ((x) > 0xf2 ? 0xf2 : (x))

static void show_reg(struct pwr_i2c_desc *i2c, uint32_t start, uint32_t end)
{
    uint32_t reg;
    int num = 0;

    start = MAX_REG(start);
    end = MAX_REG(end);
    rt_kprintf("%02x:  ", start);

    for (reg = start; reg <= end; reg++)
    {
        if (num >= 16)
        {
            rt_kprintf("\n%02x:  ", reg);
            num = 0;
        }
        num++;
        rt_kprintf("%02x ", pmic_read(i2c->bus, i2c->addr, reg));
    }
    rt_kprintf("\n");
}

static int rk816_dbg(uint8_t argc, char **argv)
{
    struct pwr_i2c_desc *i2c;
    uint32_t start, end, reg;
    uint32_t val;
    char *cmd;

    i2c = pmic_get_i2c_desc();
    if (i2c == RT_NULL)
        return -RT_EINVAL;

    if (argc == 1)
    {
        rt_kprintf("Usage example: \n"
                   "    rk816_dbg r            - dump all\n"
                   "    rk816_dbg r 0x10       - dump 0x10 address\n"
                   "    rk816_dbg r 0x10 0x20  - dump 0x10~0x20 address"
                   "    rk816_dbg w 0x10 0x55  - set 0x10 address as value 0x55");
        return RT_EOK;
    }

    cmd = argv[1];
    if (!rt_strncmp(cmd, "r", 1))
    {
        argc -= 2;
        argv += 2;
        if (argc == 0)
        {
            show_reg(i2c, 0x00, 0xf2);
        }
        else if (argc == 1)
        {
            reg = strtol(argv[0], NULL, 16);
            show_reg(i2c, reg, reg);
        }
        else if (argc == 2)
        {
            start = strtol(argv[0], NULL, 16);
            end = strtol(argv[1], NULL, 16);
            show_reg(i2c, start, end);
        }
    }
    else if (!rt_strncmp(cmd, "w", 1))
    {
        reg = strtol(argv[2], NULL, 16);
        val = strtol(argv[3], NULL, 16);
        if (reg > 0xf2)
        {
            rt_kprintf("error: 0x%02x > 0xf2(max)\n", reg);
        }
        else
        {
            rt_kprintf("write: 0x%02x 0x%02x\n", reg, val);
            pmic_write(i2c->bus, i2c->addr, reg, val);
        }
    }
    else
    {
        rt_kprintf("UNK CMD\n");
    }

    return RT_EOK;
}

MSH_CMD_EXPORT(rk816_dbg, read or write registers);
#endif

#endif
