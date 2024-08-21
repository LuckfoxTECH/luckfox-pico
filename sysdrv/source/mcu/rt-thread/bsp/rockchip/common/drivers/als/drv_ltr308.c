/**
  * Copyright (c) 2022 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_ltr308.c
  *
  * Change Logs:
  *
  *
  ******************************************************************************
  */

#include <rthw.h>
#include <rtdevice.h>
#include <rtthread.h>
#include <rtconfig.h>
#include "drv_als.h"
#include "board.h"

#ifdef RT_USING_ALS_LTR308

#define LTR308_ADDRESS          0x53

#define LTR308_MAIN_CTRL        0x00 /* ALS operation mode control, SW reset */
#define LTR308_ALS_MEAS_RATE    0x04 /* ALS measurement rate and resolution in Active Mode */
#define LTR308_ALS_GAIN         0x05 /* ALS analog Gain*/
#define LTR308_PART_ID          0x06 /* Part number ID and revision ID */
#define LTR308_MAIN_STATUS      0x07 /* Power-On status, Interrupt status, Data status */
#define LTR308_ALS_DATA_0       0x0D /* ALS ADC measurement ltr308, LSB */
#define LTR308_ALS_DATA_1       0x0E /* ALS ADC measurement ltr308 */
#define LTR308_ALS_DATA_2       0x0F /* ALS ADC measurement ltr308, MSB */
#define LTR308_INT_CFG          0x19 /* Interrupt configuration */
#define LTR308_INT_PST          0x1A /* Interrupt persist setting */
#define LTR308_ALS_THRES_UP_0   0x21 /* ALS interrupt upper threshold, LSB */
#define LTR308_ALS_THRES_UP_1   0x22 /* ALS interrupt upper threshold, intervening bits */
#define LTR308_ALS_THRES_UP_2   0x23 /* ALS interrupt upper threshold, MSB */
#define LTR308_ALS_THRES_LOW_0  0x24 /* ALS interrupt lower threshold, LSB */
#define LTR308_ALS_THRES_LOW_1  0x25 /* ALS interrupt lower threshold, intervening bits */
#define LTR308_ALS_THRES_LOW_2  0x26 /* ALS interrupt lower threshold, MSB */

#define LTR308_CTRL_ALS_ENABLE  (1 << 1)
#define LTR308_CTRL_SW_RESET    (1 << 4)

#define LTR308_DATA_READY       (1 << 3)

struct ltr308_dev
{
    struct rt_device dev;
    struct rt_i2c_bus_device *i2c_bus;
    float window_factor;
    uint8_t i2c_addr;
};

static struct ltr308_dev ltr308;

#define ltr308_info(...)            rt_kprintf("[ltr308]: ");rt_kprintf(__VA_ARGS__)
#if LTR308_DBG_EN
#define ltr308_dbg                  rt_kprintf("[ltr308]: ");rt_kprintf(__VA_ARGS__)
#else
#define ltr308_dbg(...)
#endif

static rt_err_t ltr308_read_regs(struct rt_i2c_bus_device *bus,
                                 uint8_t reg, uint8_t *data, int size)
{
    struct rt_i2c_msg msgs[2];
    uint8_t send_buf[2];

    RT_ASSERT(bus != RT_NULL);

    send_buf[0] = reg;
    msgs[0].addr = LTR308_ADDRESS;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].len = 1;
    msgs[0].buf = send_buf;

    msgs[1].addr = LTR308_ADDRESS;
    msgs[1].flags = RT_I2C_RD;
    msgs[1].len = size;
    msgs[1].buf = data;

    return (rt_i2c_transfer(bus, msgs, 2) == 2) ? RT_EOK : -RT_EIO;
}

static rt_err_t ltr308_read_reg(struct rt_i2c_bus_device *bus,
                                uint8_t reg, uint8_t *data)
{
    return ltr308_read_regs(bus, reg, data, 1);
}

static rt_err_t ltr308_write_reg(struct rt_i2c_bus_device *bus,
                                 uint8_t reg, uint8_t data)
{
    uint8_t send_buf[3];
    struct rt_i2c_msg msg;

    RT_ASSERT(bus != RT_NULL);

    send_buf[0] = reg;
    send_buf[1] = data;

    msg.addr = LTR308_ADDRESS;
    msg.flags = RT_I2C_WR;
    msg.buf = send_buf;
    msg.len = 2;

    return (rt_i2c_transfer(bus, &msg, 1) == 1) ? RT_EOK : -RT_EIO;
}

static rt_err_t ltr308_control(struct rt_device *dev, int cmd, void *args)
{
    uint8_t val;
    rt_err_t ret;
    struct ltr308_dev *ltr308;

    RT_ASSERT(dev != RT_NULL);
    ltr308 = (struct ltr308_dev *)dev;

    switch (cmd)
    {
    case RK_DEVICE_CTRL_ALS_INIT:
        break;
    case RK_DEVICE_CTRL_ALS_DATA_READY:
    {
        RT_ASSERT(args != RT_NULL);
        bool *ready = (bool *)args;
        ret = ltr308_read_reg(ltr308->i2c_bus, LTR308_MAIN_STATUS, &val);
        if (ret < 0)
            return -RT_EIO;
        *ready = (val & LTR308_DATA_READY) ? true : false;
    }
    break;
    case RK_DEVICE_CTRL_ALS_GET_VAL:
    {
        uint8_t buf[3];
        char log[64];
        uint32_t data;
        RT_ASSERT(args != RT_NULL);
        float *lux = (float *)args;

        ret = ltr308_read_regs(ltr308->i2c_bus, LTR308_ALS_DATA_0, buf, sizeof(buf));
        if (ret < 0)
            return -RT_EIO;
        data = buf[0] | (buf[1] << 8) | (buf[2] << 16);
        *lux = 0.2 * data / ltr308->window_factor;
        sprintf(log, "buf: %x, %x, %x, data=%ld, lux:%f\n", buf[0], buf[1], buf[2], data, *lux);
        ltr308_dbg("%s", log);
    }
    break;
    default:
        return -RT_EINVAL;
    }

    return RT_EOK;
}

int ltr308_init(void)
{
    struct als_desc *als_dev = (struct als_desc *)&als_ltr308;
    struct rt_device *dev;
    rt_err_t ret;
    uint8_t partid;

    ltr308.window_factor = als_dev->window_factor;
    RT_ASSERT(ltr308.window_factor);
    ltr308.i2c_bus = rt_i2c_bus_device_find(als_dev->i2c_bus);
    RT_ASSERT(ltr308.i2c_bus);

    ltr308.i2c_bus->timeout = 100;

    ret = ltr308_read_reg(ltr308.i2c_bus, LTR308_PART_ID, &partid);
    if (ret < 0 || (partid >> 4) != 0x0B)
        return -RT_ENOSYS;

    ltr308_write_reg(ltr308.i2c_bus, LTR308_MAIN_CTRL, LTR308_CTRL_ALS_ENABLE);

    dev = &(ltr308.dev);
    rt_memset(dev, 0x0, sizeof(struct rt_device));
    dev->type       = RT_Device_Class_Miscellaneous;
    dev->control    = ltr308_control;
    dev->user_data  = &ltr308;

    return rt_device_register(dev, "als", RT_DEVICE_FLAG_RDWR);
}

INIT_DEVICE_EXPORT(ltr308_init);

#endif
