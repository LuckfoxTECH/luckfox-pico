/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    rk816_charger.c
  * @version V0.1
  * @brief   rk816 charger interface
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-12-26     Joseph Chen     first implementation
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>

#ifdef RT_USING_CHARGER_RK816
#include <drivers/charger.h>
#include "drv_pmic.h"
#include "rk816.h"
#include "rk816_charger.h"

static struct rk816_charger_platform_data *charger_pdata = &rk816_charger_pdata;
static struct pwr_i2c_desc *charger_i2c;
static rt_uint32_t resistor_mohm;
static rt_uint32_t resistor_fac;

static rt_err_t rk816_charger_get_limit(struct rt_charger_limit *lmt)
{
    lmt->src_cmin = src_cur_level[1];  /* 80mA */
    lmt->src_cmax = src_cur_level[ARRAY_SIZE(src_cur_level) - 1];
    lmt->bat_cmin = bat_cur_level[0];
    lmt->bat_cmax = bat_cur_level[ARRAY_SIZE(bat_cur_level) - 1];
    lmt->bat_vmin = bat_vol_level[0];
    lmt->bat_vmax = bat_vol_level[ARRAY_SIZE(bat_vol_level) - 1];
    lmt->fns_cmin = fns_cur_level[0];
    lmt->fns_cmax = fns_cur_level[ARRAY_SIZE(fns_cur_level) - 1];

    if (resistor_mohm < 20)
    {
        lmt->bat_cmin = RES_FAC_MUX(lmt->bat_cmin, resistor_fac);
        lmt->bat_cmax = RES_FAC_MUX(lmt->bat_cmax, resistor_fac);
        lmt->fns_cmin = RES_FAC_MUX(lmt->fns_cmin, resistor_fac);
        lmt->fns_cmax = RES_FAC_MUX(lmt->fns_cmax, resistor_fac);
    }
    else if (resistor_mohm > 20)
    {
        lmt->bat_cmin = RES_FAC_DIV(lmt->bat_cmin, resistor_fac);
        lmt->bat_cmax = RES_FAC_DIV(lmt->bat_cmax, resistor_fac);
        lmt->fns_cmin = RES_FAC_DIV(lmt->fns_cmin, resistor_fac);
        lmt->fns_cmax = RES_FAC_DIV(lmt->fns_cmax, resistor_fac);
    }

    return RT_EOK;
}

static rt_err_t rk816_charger_get_status(struct rt_charger_status *sta)
{

    struct rt_i2c_bus_device *bus = charger_i2c->bus;
    rt_uint8_t addr = charger_i2c->addr;
    rt_uint8_t val, sel;

    val = pmic_read(bus, addr, CHRG_CTRL_REG1);
    sel = (val & BAT_VOL_MASK) >> 4;
    sta->bat_vol = bat_vol_level[sel];

    sel = val & BAT_CUR_MASK;
    sta->bat_cur = bat_cur_level[sel];

    val = pmic_read(bus, addr, CHRG_CTRL_REG2);
    sel = (val & FNS_CUR_MASK) >> 6;
    sta->fns_cur = fns_cur_level[sel];

    val = pmic_read(bus, addr, USB_CTRL_REG);
    sel = val & SRC_CUR_MASK;
    sta->src_cur = src_cur_level[sel];

    if (resistor_mohm < 20)
    {
        sta->bat_cur = RES_FAC_MUX(sta->bat_cur, resistor_fac);
        sta->fns_cur = RES_FAC_MUX(sta->fns_cur, resistor_fac);

    }
    else if (resistor_mohm > 20)
    {
        sta->bat_cur = RES_FAC_DIV(sta->bat_cur, resistor_fac);
        sta->fns_cur = RES_FAC_DIV(sta->fns_cur, resistor_fac);
    }

    return RT_EOK;
}

static rt_err_t rk816_charger_init(void)
{
    struct rt_i2c_bus_device *bus = charger_i2c->bus;
    rt_uint8_t addr = charger_i2c->addr;

    pmic_update_bits(bus, addr, BAT_CTRL_REG, USB_SYS_EN, USB_SYS_EN);
    pmic_update_bits(bus, addr, GGSTS_REG, VOL_INSTANT, VOL_INSTANT);
    pmic_update_bits(bus, addr, CHRG_CTRL_REG1, CHRG_EN, CHRG_EN);
    pmic_update_bits(bus, addr, CHRG_CTRL_REG2, FINISH_CUR_MASK, 0);  /* 150mA */
    pmic_update_bits(bus, addr, CHRG_CTRL_REG3,
                     CHRG_DIG_SIGNAL | CHRG_T_CCCV_EN, CHRG_DIG_SIGNAL);
    pmic_update_bits(bus, addr, SUP_STS_REG,
                     USB_VLIMIT_EN | USB_VLIMIT_EN,
                     USB_VLIMIT_EN | USB_VLIMIT_EN);
    pmic_update_bits(bus, addr, USB_CTRL_REG, CHRG_CT_EN, CHRG_CT_EN);
    pmic_update_bits(bus, addr, THERMAL_REG, TEMP_MASK, TEMP_115C);

    resistor_mohm = charger_pdata->sample_res;
    if (!resistor_mohm)
        resistor_mohm = 20;

    if (resistor_mohm == 20)
        resistor_fac = 10;
    else if (resistor_mohm > 20)
        resistor_fac = resistor_mohm * 10 / 20;
    else
        resistor_fac = 20 * 10 / resistor_mohm;

    return RT_EOK;
}

static rt_err_t rk816_charger_set_bat_voltage(rt_int32_t bat_vol)
{
    struct rt_i2c_bus_device *bus = charger_i2c->bus;
    rt_uint8_t addr = charger_i2c->addr;
    rt_uint8_t i;

    if ((bat_vol > bat_vol_level[ARRAY_SIZE(bat_vol_level) - 1]) ||
            (bat_vol < bat_vol_level[0]))
        return -RT_EINVAL;

    for (i = 0; i < ARRAY_SIZE(bat_vol_level); i++)
    {
        if (bat_vol <= bat_vol_level[i])
        {
            if (bat_vol < bat_vol_level[i])
                i = (i > 0) ? (i - 1) : i;
            break;
        }
    }

    return pmic_update_bits(bus, addr, CHRG_CTRL_REG1,
                            BAT_VOL_MASK, BAT_VOL_REGVAL(i));
}

static rt_err_t rk816_charger_set_bat_current(rt_int32_t bat_cur)
{
    struct rt_i2c_bus_device *bus = charger_i2c->bus;
    rt_uint8_t addr = charger_i2c->addr;
    rt_int32_t hw_bat_cur = bat_cur;
    rt_uint8_t i;

    if (resistor_mohm < 20)
        hw_bat_cur = RES_FAC_DIV(bat_cur, resistor_fac);
    else if (resistor_mohm > 20)
        hw_bat_cur = RES_FAC_MUX(bat_cur, resistor_fac);

    rt_kprintf("rk816 charge cur: %d, R: %d\n", bat_cur, resistor_mohm);

    if ((hw_bat_cur > bat_cur_level[ARRAY_SIZE(bat_cur_level) - 1]) ||
            (hw_bat_cur < bat_cur_level[0]))
        return -RT_EINVAL;

    for (i = 0; i < ARRAY_SIZE(bat_cur_level); i++)
    {
        if (hw_bat_cur <= bat_cur_level[i])
        {
            if (hw_bat_cur < bat_cur_level[i])
                i = (i > 0) ? (i - 1) : i;
            break;
        }
    }

    return pmic_update_bits(bus, addr, CHRG_CTRL_REG1,
                            BAT_CUR_MASK, BAT_CUR_REGVAL(i));
}

static rt_err_t rk816_charger_set_finish_current(rt_int32_t fns_cur)
{
    struct rt_i2c_bus_device *bus = charger_i2c->bus;
    rt_uint8_t addr = charger_i2c->addr;
    rt_int32_t hw_fns_cur = fns_cur;
    rt_uint8_t i;

    if (resistor_mohm < 20)
        hw_fns_cur = RES_FAC_DIV(fns_cur, resistor_fac);
    else if (resistor_mohm > 20)
        hw_fns_cur = RES_FAC_MUX(fns_cur, resistor_fac);

    rt_kprintf("rk816 finish cur: %d, R: %d\n", fns_cur, resistor_mohm);

    if ((hw_fns_cur > fns_cur_level[ARRAY_SIZE(fns_cur_level) - 1]) ||
            (hw_fns_cur < fns_cur_level[0]))
        return -RT_EINVAL;

    for (i = 0; i < ARRAY_SIZE(fns_cur_level); i++)
    {
        if (hw_fns_cur <= fns_cur_level[i])
        {
            if (hw_fns_cur < fns_cur_level[i])
                i = (i > 0) ? (i - 1) : i;
            break;
        }
    }

    return pmic_update_bits(bus, addr, CHRG_CTRL_REG2,
                            FNS_CUR_MASK, FNS_CUR_REGVAL(i));
}

static rt_err_t rk816_charger_set_src_current(rt_int32_t src_cur)
{
    struct rt_i2c_bus_device *bus = charger_i2c->bus;
    rt_uint8_t addr = charger_i2c->addr;
    rt_uint8_t i;

    rt_kprintf("rk816 src cur: %d, R: %d\n", src_cur, resistor_mohm);

    if ((src_cur > src_cur_level[ARRAY_SIZE(bat_cur_level) - 1]) ||
            (src_cur < src_cur_level[1]))
        return -RT_EINVAL;

    if (src_cur >= 80 && src_cur < 450)
    {
        i = 1; /* 80mA */
    }
    else if (src_cur < 850)
    {
        i = 0; /* 450mA */
    }
    else
    {
        for (i = 2; i < ARRAY_SIZE(src_cur_level); i++)
        {
            if (src_cur <= src_cur_level[i])
            {
                if (src_cur < src_cur_level[i])
                    i = (i > 0) ? (i - 1) : i;
                break;
            }
        }
    }

    return pmic_update_bits(bus, addr, USB_CTRL_REG,
                            SRC_CUR_MASK, SRC_CUR_REGVAL(i));
}

static rt_err_t rk816_charger_control(rt_device_t dev, int cmd, void *args)
{
    RT_ASSERT(dev != RT_NULL);

    switch (cmd)
    {
    case RT_DEVICE_CTRL_CHAGER_LIMIT_GET:
    {
        rk816_charger_get_limit((struct rt_charger_limit *)args);
    }
    break;

    case RT_DEVICE_CTRL_CHAGER_STATUS_GET:
    {
        rk816_charger_get_status((struct rt_charger_status *)args);
    }
    break;

    case RT_DEVICE_CTRL_CHAGER_BATVOL_SET:
    {
        rk816_charger_set_bat_voltage(*(rt_int32_t *)args);
    }
    break;

    case RT_DEVICE_CTRL_CHAGER_BATCUR_SET:
    {
        rk816_charger_set_bat_current(*(rt_int32_t *)args);
    }
    break;

    case RT_DEVICE_CTRL_CHAGER_FNSCUR_SET:
    {
        rk816_charger_set_finish_current(*(rt_int32_t *)args);
    }
    break;

    case RT_DEVICE_CTRL_CHAGER_SRCCUR_SET:
    {
        rk816_charger_set_src_current(*(rt_int32_t *)args);
    }
    break;

    default:
        return RT_EINVAL;
    }

    return RT_EOK;
}

#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops charger_ops =
{
    RT_NULL,
    RT_NULL,
    RT_NULL,
    RT_NULL,
    RT_NULL,
    rk816_charger_control
};
#endif

int rt_rk816_charger_init(void)
{
    static struct rt_device charger_dev;
    int ret;

    charger_i2c = pmic_get_i2c_desc();
    if (charger_i2c == RT_NULL)
        return -RT_EINVAL;

    RT_ASSERT(!rt_device_find("charger"));

    charger_dev.type    = RT_Device_Class_PM;
#ifdef RT_USING_DEVICE_OPS
    charger_dev.ops     = &charger_ops;
#else
    charger_dev.init    = RT_NULL;
    charger_dev.open    = RT_NULL;
    charger_dev.close   = RT_NULL;
    charger_dev.read    = RT_NULL;
    charger_dev.write   = RT_NULL;
    charger_dev.control = rk816_charger_control;
#endif

    ret = rt_device_register(&charger_dev, "charger", RT_DEVICE_FLAG_RDWR);
    if (ret)
        return ret;

    return rk816_charger_init();
}
INIT_DEVICE_EXPORT(rt_rk816_charger_init);
#endif
