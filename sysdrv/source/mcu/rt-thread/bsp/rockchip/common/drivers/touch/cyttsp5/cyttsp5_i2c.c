/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-09-30     Chris Zhong       the first version
 */

#include <rtthread.h>
#include <rtdevice.h>

#include <string.h>
#include <stdlib.h>

#include "hal_base.h"
#include "board.h"

#define DBG_TAG "cyttsp5"
#define DBG_LVL DBG_INFO
#define CY_PIP_1P7_EMPTY_BUF    0xFF00
#define TOUCH_REPORT_SIZE           10
#define TOUCH_INPUT_HEADER_SIZE     7
#define TOUCH_COUNT_BYTE_OFFSET     5
#define CY_MAX_INPUT        512
#define MAX_FINGER_NUM      (0x0A)
#define TOUCH_SLAVE_ADDRESS 0x24
#include <rtdbg.h>

#include "touch.h"

struct mt_data
{
    int8_t touch_type;
    int8_t id;
    int16_t x;
    int16_t y;
    int8_t pressure;
    int8_t major_axis_length;
    int8_t minor_axis_length;
    int8_t orientation;
};

static struct rt_i2c_client *cyttsp5_client;
static rt_touch_t touch_device = RT_NULL;
static int16_t pre_x[MAX_FINGER_NUM] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
static int16_t pre_y[MAX_FINGER_NUM] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
static rt_uint8_t s_tp_dowm[MAX_FINGER_NUM];

static void cyttsp5_touch_up(void *buf, int8_t id, int16_t timestamp)
{
    struct rt_touch_data *read_data = (struct rt_touch_data *)buf;

    if (s_tp_dowm[id] == 1)
    {
        s_tp_dowm[id] = 0;
        read_data[id].event = RT_TOUCH_EVENT_UP;
    }
    else
    {
        read_data[id].event = RT_TOUCH_EVENT_NONE;
    }

    read_data[id].timestamp = timestamp;
    read_data[id].width = 1;
    read_data[id].x_coordinate = pre_x[id];
    read_data[id].y_coordinate = pre_y[id];
    read_data[id].track_id = id;

    pre_x[id] = -1;  /* last point is none */
    pre_y[id] = -1;
}

static void cyttsp5_touch_down(void *buf, int8_t id, int16_t timestamp, int16_t x, int16_t y)
{
    struct rt_touch_data *read_data = (struct rt_touch_data *)buf;

    if (s_tp_dowm[id] == 1)
    {
        read_data[id].event = RT_TOUCH_EVENT_MOVE;
    }
    else
    {
        read_data[id].event = RT_TOUCH_EVENT_DOWN;
        s_tp_dowm[id] = 1;
    }

    read_data[id].timestamp = timestamp;
    read_data[id].width = 1;
    read_data[id].x_coordinate = x;
    read_data[id].y_coordinate = y;
    read_data[id].track_id = id;

    pre_x[id] = x; /* save last point */
    pre_y[id] = y;
}

static int cyttsp5_i2c_read_default_nosize(struct rt_i2c_client *dev, rt_uint8_t *buf, size_t max)
{
    struct rt_i2c_msg msgs[2];
    rt_uint8_t msg_count = 1;
    int rc;
    size_t size;

    if (!buf)
        return -RT_ERROR;

    msgs[0].addr = dev->client_addr;
    msgs[0].flags =  RT_I2C_RD;
    msgs[0].len = 2;
    msgs[0].buf = buf;
    rc = rt_i2c_transfer(dev->bus, msgs, msg_count);
    if (rc < 0 || rc != msg_count)
        return (rc < 0) ? rc : -RT_ERROR;

    size = buf[0] | buf[1] << 8;
    if (!size || size == 2 || size >= CY_PIP_1P7_EMPTY_BUF)
        return -RT_ERROR;

    if (size > max)
        return -RT_ERROR;

    rc = rt_i2c_master_recv(dev->bus, dev->client_addr, 0, buf, size);

    return (rc < 0) ? rc : rc != (int)size ? -RT_ERROR : 0;
}

static rt_size_t cyttsp5_read_point(struct rt_touch_device *touch, void *buf, rt_size_t read_num)
{
    rt_uint8_t read_buf[CY_MAX_INPUT] = {0};
    int rc;
    int num_cur_tch, length, i;
    rt_uint16_t time;

    rc = cyttsp5_i2c_read_default_nosize(cyttsp5_client, read_buf, CY_MAX_INPUT);
    if (rc < 0)
    {
        return rc;
    }

    num_cur_tch = read_buf[5] & 0x1f;

    length = num_cur_tch * TOUCH_REPORT_SIZE;

    if (length > TOUCH_INPUT_HEADER_SIZE + CY_MAX_INPUT)
    {
        return RT_ERROR;
    }

    time = read_buf[3] | read_buf[4] << 8;
    for (i = 0; i < num_cur_tch; i++)
    {
        struct mt_data *data = (struct mt_data *)&read_buf[TOUCH_INPUT_HEADER_SIZE + i * TOUCH_REPORT_SIZE];

        if (data->id & 0x80)
            cyttsp5_touch_down(buf, data->id & 0x1f, time, RT_TOUCH_X_RANGE - data->x, RT_TOUCH_Y_RANGE - data->y);
        else
            cyttsp5_touch_up(buf, data->id & 0x1f, time);
    }

    return num_cur_tch;
}

static rt_err_t cyttsp5_control(struct rt_touch_device *device, int cmd, void *data)
{
    rt_err_t ret = RT_EOK;

    if (cmd == RT_TOUCH_CTRL_GET_ID)
    {
        char *id = (char *)data;

        strcpy(id, "cyttsp5");
    }
    else if (cmd == RT_TOUCH_CTRL_GET_INFO)
    {
        struct rt_touch_info *info = (struct rt_touch_info *)data;
        info->range_x = RT_TOUCH_X_RANGE;
        info->range_y = RT_TOUCH_Y_RANGE;
        info->point_num = MAX_FINGER_NUM;
    }

    return ret;
}

static struct rt_touch_ops touch_ops =
{
    .touch_readpoint = cyttsp5_read_point,
    .touch_control   = cyttsp5_control,
};

int rt_hw_cyttsp5_init(void)
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

    /* cyttsp5 hardware init */
    /* before here, IOMUX must be initialized in board_xxxx.c*/
    rt_pin_mode(rst_pin.pin, PIN_MODE_OUTPUT);
    rt_pin_write(rst_pin.pin, PIN_LOW);
    rt_thread_mdelay(1);
    rt_pin_write(rst_pin.pin, PIN_HIGH);
    rt_pin_mode(cfg.irq_pin.pin, PIN_MODE_INPUT_PULLUP);

    /* i2c interface bus */
    cyttsp5_client = (struct rt_i2c_client *)rt_calloc(1, sizeof(struct rt_i2c_client));
    cyttsp5_client->client_addr = TOUCH_SLAVE_ADDRESS;
    cyttsp5_client->bus = (struct rt_i2c_bus_device *)rt_device_find(cfg.dev_name);
    RT_ASSERT(cyttsp5_client->bus != RT_NULL);

    ret = rt_device_open((rt_device_t)cyttsp5_client->bus, RT_DEVICE_FLAG_RDWR);
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

INIT_DEVICE_EXPORT(rt_hw_cyttsp5_init);
