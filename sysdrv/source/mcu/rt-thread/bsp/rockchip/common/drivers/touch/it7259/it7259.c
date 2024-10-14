/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-11-04     Chris.Zhong       the first version
 */

#include <rtthread.h>
#include <rtdevice.h>

#include <string.h>
#include <stdlib.h>

#include "hal_base.h"
#include "board.h"

#define DBG_TAG "it7259"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

#include "touch.h"
#include "it7259.h"

static struct rt_i2c_client *it7259_client;
static rt_touch_t touch_device = RT_NULL;

static rt_err_t it7259_write_reg(struct rt_i2c_client *dev, rt_uint8_t write_len, rt_uint8_t *write_data)
{
    struct rt_i2c_msg msgs;

    msgs.addr  = dev->client_addr;
    msgs.flags = RT_I2C_WR;
    msgs.buf   = write_data;
    msgs.len   = write_len;

    if (rt_i2c_transfer(dev->bus, &msgs, 1) == 1)
    {
        return RT_EOK;
    }
    else
    {
        return -RT_ERROR;
    }
}

static rt_err_t it7259_read_regs(struct rt_i2c_client *dev, rt_uint8_t cmd, rt_uint8_t read_len, rt_uint8_t *read_buf)
{
    struct rt_i2c_msg msgs[2];
    rt_uint8_t cmd_buf = cmd;

    msgs[0].addr  = dev->client_addr;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = &cmd_buf;
    msgs[0].len   = 1;

    msgs[1].addr  = dev->client_addr;
    msgs[1].flags = RT_I2C_RD;
    msgs[1].buf   = read_buf;
    msgs[1].len   = read_len;

    if (rt_i2c_transfer(dev->bus, msgs, 2) == 2)
    {
        return RT_EOK;
    }

    rt_kprintf("[it7259] failed to read the register!\n");

    return -RT_ERROR;
}

static rt_err_t it7259_control(struct rt_touch_device *device, int cmd, void *data)
{
    rt_err_t ret = RT_EOK;

    if (cmd == RT_TOUCH_CTRL_GET_ID)
    {
        rt_uint8_t cmd[10] = {COMMAND_BUFFER_INDEX, 0};

        ret = it7259_write_reg(it7259_client, 2, cmd);
        ret = it7259_read_regs(it7259_client, COMMAND_RESPONSE_BUFFER_INDEX, 10, cmd);
    }
    else if (cmd == RT_TOUCH_CTRL_GET_INFO)
    {
        struct rt_touch_info *info = (struct rt_touch_info *)data;
        info->range_x = RT_TOUCH_X_RANGE;
        info->range_y = RT_TOUCH_Y_RANGE;
        info->point_num = FTS_MAX_TOUCH_POINT;
    }

    return ret;
}

static int16_t last_x[FTS_MAX_TOUCH_POINT] = { -1};
static int16_t last_y[FTS_MAX_TOUCH_POINT] = { -1};
static rt_uint8_t s_tp_down[FTS_MAX_TOUCH_POINT];
static struct rt_touch_data *read_data;

static void it7259_touch_up(void *buf, int8_t id)
{
    read_data = (struct rt_touch_data *)buf;

    if (s_tp_down[id] == 1)
    {
        s_tp_down[id] = 0;
        read_data[id].event = RT_TOUCH_EVENT_UP;
    }
    else
    {
        read_data[id].event = RT_TOUCH_EVENT_NONE;
    }

    read_data[id].timestamp = rt_touch_get_ts();
    read_data[id].x_coordinate = last_x[id];
    read_data[id].y_coordinate = last_y[id];
    read_data[id].track_id = id;

    last_x[id] = -1;  /* last point is none */
    last_y[id] = -1;
}

static void it7259_touch_down(void *buf, int8_t id, int16_t x, int16_t y)
{
    read_data = (struct rt_touch_data *)buf;

    if (s_tp_down[id] == 1)
    {
        read_data[id].event = RT_TOUCH_EVENT_MOVE;
    }
    else
    {
        read_data[id].event = RT_TOUCH_EVENT_DOWN;
        s_tp_down[id] = 1;
    }

    read_data[id].timestamp = rt_touch_get_ts();
    read_data[id].x_coordinate = x;
    read_data[id].y_coordinate = y;
    read_data[id].track_id = id;

    last_x[id] = x; /* save last point */
    last_y[id] = y;
}

static rt_size_t it7259_read_point(struct rt_touch_device *touch, void *buf, rt_size_t read_num)
{
    rt_uint8_t touch_num = 0;
    rt_uint8_t read_buf[14 * FTS_MAX_TOUCH_POINT] = {0};
    int32_t touch_x = 0;
    int32_t touch_y = 0;

    if (read_num < FTS_MAX_TOUCH_POINT)
    {
        goto _exit;
    }

    if (it7259_read_regs(it7259_client, QUERY_BUFFER_INDEX, 1, &touch_num) != RT_EOK)
    {
        rt_kprintf("[it7259] failed to read touch_num data!!!\n");
        goto _exit;
    }

    if (it7259_read_regs(it7259_client, POINT_BUFFER_INDEX, 14, read_buf) != RT_EOK)
    {
        rt_kprintf("[it7259]failed to read point data!!!\n");
        goto _exit;
    }

    if (touch_num == 0xc0)
    {
        touch_x = ((read_buf[3] & 0x0f) << 8) + read_buf[2];
        touch_y = ((read_buf[3] & 0xf0) << 4) + read_buf[4];
        touch_x = RT_TOUCH_X_RANGE - touch_x;
        touch_y = RT_TOUCH_Y_RANGE - touch_y;
        it7259_touch_down(buf, 0, touch_x, touch_y);
        touch_num = 1;
    }
    else
    {
        it7259_touch_up(buf, 0);
        touch_num = 1;
    }

_exit:
    return touch_num;
}

static struct rt_touch_ops touch_ops =
{
    .touch_readpoint = it7259_read_point,
    .touch_control   = it7259_control,
};

int rt_hw_it7259_init(void)
{
    rt_err_t ret;
    struct rt_touch_config cfg;
    struct rt_device_pin_mode rst_pin;
    const char *touch_name = TOUCH_DEV_NAME;

    RT_ASSERT(TOUCH_RST_PIN != 0);
    RT_ASSERT(TOUCH_IRQ_PIN != 0);
    RT_ASSERT(TOUCH_I2C_DEV != 0);

    /* touch config init */
    rst_pin.pin = TOUCH_RST_PIN;
    cfg.irq_pin.pin  = TOUCH_IRQ_PIN;
    cfg.irq_pin.mode = PIN_MODE_INPUT_PULLUP;
    cfg.dev_name = TOUCH_I2C_DEV;

    /* it7259 hardware init */
    /* before here, IOMUX must be initialized in board_xxxx.c*/
    rt_pin_mode(rst_pin.pin, PIN_MODE_OUTPUT);
    rt_pin_mode(cfg.irq_pin.pin, PIN_MODE_OUTPUT);
    rt_pin_write(rst_pin.pin, PIN_LOW);
    rt_thread_mdelay(1);
    rt_pin_write(rst_pin.pin, PIN_HIGH);
    rt_pin_mode(cfg.irq_pin.pin, PIN_MODE_INPUT_PULLUP);
    rt_thread_mdelay(20);

    /* i2c interface bus */
    it7259_client = (struct rt_i2c_client *)rt_calloc(1, sizeof(struct rt_i2c_client));
    it7259_client->client_addr = TOUCH_SLAVE_ADDRESS;
    it7259_client->bus = (struct rt_i2c_bus_device *)rt_device_find(cfg.dev_name);
    RT_ASSERT(it7259_client->bus != RT_NULL);

    ret = rt_device_open((rt_device_t)it7259_client->bus, RT_DEVICE_FLAG_RDWR);
    RT_ASSERT(ret == RT_EOK);

    /* register touch device */
    touch_device = (rt_touch_t)rt_calloc(1, sizeof(struct rt_touch_device));
    RT_ASSERT(touch_device != RT_NULL);

    touch_device->info.type = RT_TOUCH_TYPE_CAPACITANCE;
    touch_device->info.vendor = 123;
    rt_memcpy(&touch_device->config, &cfg, sizeof(struct rt_touch_config));
    touch_device->ops = &touch_ops;
    rt_hw_touch_register(touch_device, touch_name, RT_DEVICE_FLAG_INT_RX, RT_NULL);

    return RT_EOK;
}

INIT_DEVICE_EXPORT(rt_hw_it7259_init);
