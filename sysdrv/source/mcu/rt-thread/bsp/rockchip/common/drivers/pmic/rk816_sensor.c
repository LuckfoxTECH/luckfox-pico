/**
  * Copyright (c) 2020 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    rk816_sensor.c
  * @version V0.1
  * @brief   battery temperature sensor interface
  *
  * Change Logs:
  * Date           Author          Notes
  * 2020-05-02     Joseph Chen     first implementation
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>

#ifdef RT_USING_SENSOR_RK816
#include <drivers/sensor.h>
#include "drv_pmic.h"
#include "rk816.h"
#include "rk816_sensor.h"

#define TS_GPIO_FUNC               0x04
#define TS_FUN                     0x10
#define ADC_TS_EN                  0x20
#define ADC_CUR_MSK                0x03
#define ADC_CUR_20UA               0x00
#define ADC_CUR_40UA               0x01
#define ADC_CUR_60UA               0x02
#define ADC_CUR_80UA               0x03
#define NTC_FACTOR_80UA            80
#define NTC_FACTOR_60UA            60
#define NTC_FACTOR_40UA            40
#define NTC_FACTOR_20UA            20
#define NTC_80UA_MAX_RANGE         27500
#define NTC_60UA_MAX_RANGE         36666
#define NTC_40UA_MAX_RANGE         55000
#define NTC_20UA_MAX_RANGE         110000

struct sensor_config_info
{
    rt_int32_t ntc_factor;
    rt_int32_t voltage_k;
    rt_int32_t voltage_b;
};

static struct rk816_sensor_platform_data *sensor_pdata = &rk816_sensor_pdata;
static struct sensor_config_info sensor_info;
static struct pwr_i2c_desc *sensor_i2c;
static struct rt_device sensor_device;

static int rk816_sensor_init(struct sensor_config_info *si)
{
    struct rt_i2c_bus_device *bus = sensor_i2c->bus;
    rt_uint8_t addr = sensor_i2c->addr;
    rt_int32_t vcalib0 = 0;
    rt_int32_t vcalib1 = 0;
    rt_uint8_t buf;

    /* init voltage k/b */
    vcalib0 |= pmic_read(bus, addr, VCALIB0_REGL) << 0;
    vcalib0 |= pmic_read(bus, addr, VCALIB0_REGH) << 8;
    vcalib1 |= pmic_read(bus, addr, VCALIB1_REGL) << 0;
    vcalib1 |= pmic_read(bus, addr, VCALIB1_REGH) << 8;
    si->voltage_k = (4200 - 3000) * 1000 / (vcalib1 - vcalib0);
    si->voltage_b = 4200 - (si->voltage_k * vcalib1) / 1000;

    /* find best ntc_factor */
    if (sensor_pdata->ntc_table[0] < NTC_80UA_MAX_RANGE)
        si->ntc_factor = NTC_FACTOR_80UA;
    else if (sensor_pdata->ntc_table[0] < NTC_60UA_MAX_RANGE)
        si->ntc_factor = NTC_FACTOR_60UA;
    else if (sensor_pdata->ntc_table[0] < NTC_40UA_MAX_RANGE)
        si->ntc_factor = NTC_FACTOR_40UA;
    else
        si->ntc_factor = NTC_FACTOR_20UA;

    /* pin as ts-function */
    buf = pmic_read(bus, addr, GPIO_IO_POL_REG);
    buf &= ~TS_GPIO_FUNC;
    pmic_write(bus, addr, GPIO_IO_POL_REG, buf);

    /* select external temperature monitoring */
    buf = pmic_read(bus, addr, TS_CTRL_REG);
    buf &= ~TS_FUN;
    pmic_write(bus, addr, TS_CTRL_REG, buf);

    /* select uA */
    buf = pmic_read(bus, addr, TS_CTRL_REG);
    buf &= ~ADC_CUR_MSK;
    if (si->ntc_factor == NTC_FACTOR_80UA)
        buf |= ADC_CUR_80UA;
    else if (si->ntc_factor == NTC_FACTOR_60UA)
        buf |= ADC_CUR_60UA;
    else if (si->ntc_factor == NTC_FACTOR_40UA)
        buf |= ADC_CUR_40UA;
    else
        buf |= ADC_CUR_20UA;
    pmic_write(bus, addr, TS_CTRL_REG, buf);

    /* enable */
    buf = pmic_read(bus, addr, ADC_CTRL_REG);
    buf |= ADC_TS_EN;
    pmic_write(bus, addr, ADC_CTRL_REG, buf);

    return RT_EOK;
}

static rt_int32_t rk816_sensor_get_temp(struct sensor_config_info *si)
{
    const rt_uint32_t *ntc_table = sensor_pdata->ntc_table;
    struct rt_i2c_bus_device *bus = sensor_i2c->bus;
    rt_uint32_t ntc_num = sensor_pdata->ntc_num;
    rt_uint8_t addr = sensor_i2c->addr;
    rt_int32_t res, buf = 0;
    rt_int32_t i, temp;

    /* get ntc */
    buf |= pmic_read(bus, addr, TS_ADC_REGL) << 0;
    buf |= pmic_read(bus, addr, TS_ADC_REGH) << 8;
    res = ((si->voltage_k * buf) / 1000 + si->voltage_b) * 1000 / 2200;
    res = (res * 1000) / si->ntc_factor;

    /* decode ntc to temperature */
    if (res < ntc_table[ntc_num - 1])
    {
        temp = sensor_pdata->ntc_degree_min +
               sensor_pdata->ntc_degree_step * (ntc_num - 1);
        rt_kprintf("rk816 sensor: ntc %d over max %d\n", res, ntc_table[ntc_num - 1]);
    }
    else if (res > ntc_table[0])
    {
        temp = sensor_pdata->ntc_degree_min;
        rt_kprintf("rk816 sensor: ntc %d under min %d\n", res, ntc_table[0]);
    }
    else
    {
        for (i = 0; i < ntc_num; i++)
        {
            if (res >= ntc_table[i])
                break;
        }

        temp = sensor_pdata->ntc_degree_min +
               sensor_pdata->ntc_degree_step * i;
    }

    return temp * 10; /* uinit */
}

static rt_size_t rk816_sensor_fetch_data(struct rt_sensor_device *sensor,
        void *buf, rt_size_t len)
{
    struct rt_sensor_data *sensor_data = buf;

    sensor_data->data.temp =
        rk816_sensor_get_temp(sensor->config.intf.user_data);

    return len;
}

static rt_err_t rk816_sensor_control(struct rt_sensor_device *sensor,
                                     int cmd, void *arg)
{
    switch (cmd)
    {
    case RT_SENSOR_CTRL_GET_ID:
        if (arg)
        {
            *(rt_uint32_t *)arg = RK816_I2C_ADDR;
        }
        break;
    case RT_SENSOR_CTRL_GET_INFO:
        if (arg)
        {
            rt_memcpy(arg, &sensor->info, sizeof(struct rt_sensor_info));
        }
        break;
    default:
        return -RT_ERROR;
    }

    return RT_EOK;
}

static const struct rt_sensor_ops sensor_ops =
{
    .fetch_data = rk816_sensor_fetch_data,
    .control = rk816_sensor_control,
};

static struct rt_sensor_device rk816_sensor =
{
    .info = {
        .type = RT_SENSOR_CLASS_TEMP,
        .unit = RT_SENSOR_UNIT_DCELSIUS,
        .vendor = RT_SENSOR_VENDOR_UNKNOWN,
        .model = "battery temperature monitor",
        .intf_type = RT_SENSOR_INTF_I2C,
        .range_max = NTC_20UA_MAX_RANGE,
        .range_min = 0,
        .period_min = 0,
        .fifo_max = 0,
    },
    .ops = &sensor_ops,
    .data_len = 0,
    .data_buf = RT_NULL,
    .module = RT_NULL,
    .irq_handle = RT_NULL,
};

int rt_rk816_sensor_init(void)
{
    if (!sensor_pdata->ntc_num || !sensor_pdata->ntc_table)
        return -RT_EINVAL;

    sensor_i2c = pmic_get_i2c_desc();
    if (sensor_i2c == RT_NULL)
        return -RT_EINVAL;

    rk816_sensor_init(&sensor_info);
    rk816_sensor.parent = sensor_device;
    rk816_sensor.config.intf.user_data = (void *)&sensor_info;

    /* real device name after register: "temp_rk816" */
    return rt_hw_sensor_register(&rk816_sensor, "rk816",
                                 RT_DEVICE_FLAG_RDONLY, NULL);
}

INIT_DEVICE_EXPORT(rt_rk816_sensor_init);
#endif /* RT_USING_SENSOR_RK816 */

