/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-06-01     tyustli     the first version
 */

#include <rtthread.h>
#include <rtdevice.h>

#include <string.h>
#include <stdlib.h>

#include "hal_base.h"
#include "board.h"

#include "touch.h"
#include "gtxx.h"

//#define GTXX_DEBUG

static struct rt_i2c_client *gtxx_client;
static rt_touch_t touch_device = RT_NULL;

/* hardware section */
static rt_uint8_t GTP_CFG_TBL[] =
{
//GT5688 config
    /*    0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F     */
    /*    7    8    9    A    B    C    D    E    F    0    1    2    3    4    5    6     */
    0x97, 0xd0, 0x02, 0x00, 0x05, 0x05, 0x34, 0x10, 0x01, 0x00, 0x08, 0x08, 0x50, 0x3c, 0x53, 0x11, //0x8050 (0x8047):
    0x01, 0x01, 0x00, 0x00, 0x14, 0x14, 0x14, 0x22, 0x08, 0x04, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, //0x8060 (0x8057):
    0x53, 0x00, 0x14, 0x00, 0x00, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x1e, 0x1e, 0x8a, 0x2a, //0x8070 (0x8067):
    0x0c, 0x3c, 0x3e, 0xf8, 0x0a, 0x20, 0x33, 0x60, 0x12, 0x02, 0x24, 0x00, 0x00, 0x32, 0x64, 0x80, //0x8080 (0x8077):
    0x14, 0x02, 0x00, 0x00, 0x54, 0x80, 0x35, 0x7f, 0x3d, 0x7f, 0x46, 0x7f, 0x51, 0x7f, 0x5d, 0x7f, //0x8090 (0x8087):
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x80a0 (0x8097):
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x80b0 (0x80a7):
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x80c0 (0x80b7):
    0x00, 0x00, 0x32, 0x20, 0x50, 0x3c, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x08, 0x02, 0x09, //0x80d0 (0x80c7):
    0x03, 0x0a, 0x04, 0x0b, 0x05, 0x0c, 0x06, 0x0d, 0xff, 0xff, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, //0x80e0 (0x80d7):
    0x11, 0x12, 0x13, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00, 0xff, 0xff, //0x80f0 (0x80e7):
    0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x8100 (0x80f7):
    0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x8110 (0x8107):
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0x8120 (0x8117):
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x93, 0x05, 0x01 //0x8130 (0x8127):
};

static rt_err_t gtxx_write_reg(struct rt_i2c_client *dev, rt_uint8_t write_len, rt_uint8_t *write_data)
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

static rt_err_t gtxx_read_regs(struct rt_i2c_client *dev, rt_uint16_t cmd, rt_uint8_t read_len, rt_uint8_t *read_buf)
{
    struct rt_i2c_msg msgs[2];
    rt_uint8_t cmd_buf[2];

    cmd_buf[0] = (rt_uint8_t)(cmd >> 8);
    cmd_buf[1] = (rt_uint8_t)(cmd & 0xff);

    msgs[0].addr  = dev->client_addr;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = cmd_buf;
    msgs[0].len   = 2;

    msgs[1].addr  = dev->client_addr;
    msgs[1].flags = RT_I2C_RD;
    msgs[1].buf   = read_buf;
    msgs[1].len   = read_len;

    if (rt_i2c_transfer(dev->bus, msgs, 2) == 2)
    {
#ifdef GTXX_DEBUG
        rt_uint16_t i;
        rt_kprintf("start read regs(0x%04x):", cmd);
        for (i = 0; i < read_len; i++)
        {
            rt_kprintf(" 0x%02x", read_buf[i]);
        }
        rt_kprintf("\n");
#endif
        return RT_EOK;
    }

    return -RT_ERROR;
}

static rt_err_t gtxx_config_set(struct rt_i2c_client *dev)
{
    rt_err_t ret;
    rt_uint32_t i;
    rt_uint8_t *config;
    rt_uint8_t  cfg_num = sizeof(GTP_CFG_TBL);
    rt_uint16_t check_sum = 0;

    config = (rt_uint8_t *)rt_calloc(1, cfg_num + GTP_ADDR_LENGTH);
    RT_ASSERT(config != RT_NULL);

    config[0] = (rt_uint8_t)((GTP_CONFIG >> 8) & 0xFF);
    config[1] = (rt_uint8_t)(GTP_CONFIG & 0xFF);
    memcpy(&config[2], GTP_CFG_TBL, cfg_num);

    /* set x range */
    config[GTP_ADDR_LENGTH + 1] = (rt_uint8_t)(720 & 0xff);
    config[GTP_ADDR_LENGTH + 2] = (rt_uint8_t)(720 >> 8);

    /* set y range */
    config[GTP_ADDR_LENGTH + 3] = (rt_uint8_t)(1280 & 0xff);
    config[GTP_ADDR_LENGTH + 4] = (rt_uint8_t)(1280 >> 8);

    /* set max point */
    config[GTP_ADDR_LENGTH + 5] = GTP_MAX_TOUCH;

    /* change x y */
    //config[GTP_ADDR_LENGTH + 6] ^= (1 << 3);

    /* change int trig type */
    config[GTP_ADDR_LENGTH + 6] &= 0xFC;

    check_sum = 0;
    for (i = GTP_ADDR_LENGTH; i < (cfg_num + GTP_ADDR_LENGTH - 3); i += 2)
    {
        check_sum += (config[i] << 8) + config[i + 1];
    }
    check_sum = 0 - check_sum;
    config[(cfg_num + GTP_ADDR_LENGTH - 3)] = (rt_uint8_t)(check_sum >> 8);
    config[(cfg_num + GTP_ADDR_LENGTH - 2)] = (rt_uint8_t)(check_sum & 0xFF);
    config[(cfg_num + GTP_ADDR_LENGTH - 1)] = 0x01;
    rt_kprintf("sizeof CFG_TABLE = %d, check_sum = 0x%x\n", cfg_num, check_sum);

    ret = gtxx_write_reg(gtxx_client, cfg_num + GTP_ADDR_LENGTH, config);
    RT_ASSERT(ret == RT_EOK);

#ifdef GTXX_DEBUG
    {
        rt_uint32_t j, cnt = 0;
        rt_uint32_t count = cfg_num;
        rt_uint8_t cfgbuf[256] = {0,};

        ret = gtxx_read_regs(gtxx_client, GTP_CONFIG, count, cfgbuf);
        RT_ASSERT(ret == RT_EOK);

        rt_kprintf("\n");
        for (j = 0; j < 16; j++)
        {
            rt_kprintf("0x%04x: ", GTP_CONFIG + j * 16);
            for (i = 0; i < 16; i++)
            {
                rt_kprintf("0x%02x,", cfgbuf[j * 16 + i]);
                if (++cnt >= count)
                {
                    break;
                }
            }

            if (cnt >= count)
            {
                break;
            }

            rt_kprintf("\n");
        }
        rt_kprintf("\n");

        check_sum = 0;
        for (i = 0; i < (cfg_num - 3); i += 2)
        {
            check_sum += (cfgbuf[i] << 8) + cfgbuf[i + 1];
        }
        check_sum = 0 - check_sum;

        rt_kprintf("check_sum = 0x%x\n", check_sum);
    }
#endif

    rt_free(config);

    return RT_EOK;
}

static rt_err_t gtxx_control(struct rt_touch_device *device, int cmd, void *data)
{
    rt_err_t ret;

    if (cmd == RT_TOUCH_CTRL_GET_ID)
    {
        rt_uint8_t *id = (rt_uint8_t *)data;
        id[0] = 'g';
        id[1] = 't';
        ret = gtxx_read_regs(gtxx_client, GTP_PRODUCT_ID, 6, &id[2]);
        RT_ASSERT(ret == RT_EOK);
    }
    else if (cmd == RT_TOUCH_CTRL_GET_INFO)
    {
        struct rt_touch_info *info = (struct rt_touch_info *)data;
        rt_uint8_t opr_buf[7] = {0};
        ret = gtxx_read_regs(gtxx_client, GTP_CONFIG, 7, opr_buf);
        RT_ASSERT(ret == RT_EOK);
        info->range_x = (opr_buf[2] << 8) + opr_buf[1];
        info->range_y = (opr_buf[4] << 8) + opr_buf[3];
        info->point_num = opr_buf[5] & 0x0f;
    }

    return RT_EOK;
}

static int16_t pre_x[GTP_MAX_TOUCH] = {-1, -1, -1, -1, -1};
static int16_t pre_y[GTP_MAX_TOUCH] = {-1, -1, -1, -1, -1};
static int16_t pre_w[GTP_MAX_TOUCH] = {-1, -1, -1, -1, -1};
static rt_uint8_t s_tp_dowm[GTP_MAX_TOUCH];
static struct rt_touch_data *read_data;

static void gtxx_touch_up(void *buf, int8_t id)
{
    read_data = (struct rt_touch_data *)buf;

    if (s_tp_dowm[id] == 1)
    {
        s_tp_dowm[id] = 0;
        read_data[id].event = RT_TOUCH_EVENT_UP;
    }
    else
    {
        read_data[id].event = RT_TOUCH_EVENT_NONE;
    }

    read_data[id].timestamp = rt_touch_get_ts();
    read_data[id].width = pre_w[id];
    read_data[id].x_coordinate = pre_x[id];
    read_data[id].y_coordinate = pre_y[id];
    read_data[id].track_id = id;

    pre_x[id] = -1;  /* last point is none */
    pre_y[id] = -1;
    pre_w[id] = -1;
}

static void gtxx_touch_down(void *buf, int8_t id, int16_t x, int16_t y, int16_t w)
{
    read_data = (struct rt_touch_data *)buf;

    if (s_tp_dowm[id] == 1)
    {
        read_data[id].event = RT_TOUCH_EVENT_MOVE;

    }
    else
    {
        read_data[id].event = RT_TOUCH_EVENT_DOWN;
        s_tp_dowm[id] = 1;
    }

    read_data[id].timestamp = rt_touch_get_ts();
    read_data[id].width = w;
    read_data[id].x_coordinate = x;
    read_data[id].y_coordinate = y;
    read_data[id].track_id = id;

    pre_x[id] = x; /* save last point */
    pre_y[id] = y;
    pre_w[id] = w;
}

static rt_size_t gtxx_read_point(struct rt_touch_device *touch, void *buf, rt_size_t read_num)
{
    rt_uint8_t point_status = 0;
    rt_uint8_t touch_num = 0;
    rt_uint8_t write_buf[3];
    rt_uint8_t read_buf[8 * GTP_MAX_TOUCH] = {0};
    rt_uint8_t read_index = 0;
    int8_t read_id = 0;
    int16_t input_x = 0;
    int16_t input_y = 0;
    int16_t input_w = 0;

    static rt_uint8_t pre_touch = 0;
    static int8_t pre_id[GTP_MAX_TOUCH] = {0};

    rt_device_control(&touch->parent, RT_TOUCH_CTRL_DISABLE_INT, RT_NULL);

    /* point status register */
    if (gtxx_read_regs(gtxx_client, GTP_READ_STATUS, 1, &point_status) != RT_EOK)
    {
        rt_kprintf("read point failed\n");
        goto exit_;
    }

    if (point_status == 0)             /* no data */
    {
        goto exit_;
    }

    if ((point_status & 0x80) == 0)    /* data is not ready */
    {
        goto exit_;
    }

    touch_num = point_status & 0x0f;  /* get point num */

    if (touch_num > GTP_MAX_TOUCH) /* point num is not correct */
    {
        goto exit_;
    }

    /* read point num is read_num */
    if (gtxx_read_regs(gtxx_client, GTP_POINT1_REG, read_num * GTP_POINT_INFO_NUM, read_buf) != RT_EOK)
    {
        rt_kprintf("read point failed\n");
        goto exit_;
    }

#ifdef GTXX_DEBUG
    rt_kprintf("sta = 0x%02x, id = 0x%02x, ", point_status, read_buf[0]);
    rt_kprintf("x = 0x%02x%02x, 0x%04x, %d,  ", read_buf[2], read_buf[1], (read_buf[2] << 8) + read_buf[1], (read_buf[2] << 8) + read_buf[1]);
    rt_kprintf("y = 0x%02x%02x, 0x%04x, %d\n", read_buf[4], read_buf[3], (read_buf[4] << 8) + read_buf[3], (read_buf[4] << 8) + read_buf[3]);
#endif

    if (pre_touch > touch_num)                                       /* point up */
    {
        for (read_index = 0; read_index < pre_touch; read_index++)
        {
            rt_uint8_t j;

            for (j = 0; j < touch_num; j++)                          /* this time touch num */
            {
                read_id = read_buf[j * 8] & 0x0F;

                if (pre_id[read_index] == read_id)                   /* this id is not free */
                    break;

                if (j >= touch_num - 1)
                {
                    rt_uint8_t up_id;
                    up_id = pre_id[read_index];
                    gtxx_touch_up(buf, up_id);
                }
            }
        }
    }

    if (touch_num)                                                 /* point down */
    {
        rt_uint8_t off_set;

        for (read_index = 0; read_index < touch_num; read_index++)
        {
            off_set = read_index * 8;
            read_id = read_buf[off_set] & 0x0f;
            pre_id[read_index] = read_id;
            input_x = read_buf[off_set + 1] | (read_buf[off_set + 2] << 8); /* x */
            input_y = read_buf[off_set + 3] | (read_buf[off_set + 4] << 8); /* y */
            input_w = read_buf[off_set + 5] | (read_buf[off_set + 6] << 8); /* size */

            gtxx_touch_down(buf, read_id, input_x, input_y, input_w);
        }
    }
    else if (pre_touch)
    {
        for (read_index = 0; read_index < pre_touch; read_index++)
        {
            gtxx_touch_up(buf, pre_id[read_index]);
        }
    }

    pre_touch = touch_num;

exit_:
    write_buf[0] = (rt_uint8_t)((GTP_READ_STATUS >> 8) & 0xFF);
    write_buf[1] = (rt_uint8_t)(GTP_READ_STATUS & 0xFF);
    write_buf[2] = 0x00;
    gtxx_write_reg(gtxx_client, 3, write_buf);

    rt_device_control(&touch->parent, RT_TOUCH_CTRL_ENABLE_INT, RT_NULL);

    return read_index;
}

static struct rt_touch_ops touch_ops =
{
    .touch_readpoint = gtxx_read_point,
    .touch_control   = gtxx_control,
};

int rt_hw_gtxx_init(void)
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
    cfg.irq_pin.mode = PIN_MODE_INPUT_PULLDOWN;
    cfg.dev_name = TOUCH_I2C_DEV;

    /* gtxx hardware init */
    /* before here, IOMUX must be initialized in board_xxxx.c*/
    rt_pin_mode(rst_pin.pin, PIN_MODE_OUTPUT);
    rt_pin_mode(cfg.irq_pin.pin, PIN_MODE_OUTPUT);
    rt_pin_write(rst_pin.pin, PIN_LOW);
    rt_thread_mdelay(10);
    rt_pin_write(rst_pin.pin, PIN_HIGH);
    rt_thread_mdelay(10);
    rt_pin_mode(cfg.irq_pin.pin, PIN_MODE_INPUT_PULLDOWN);
    rt_thread_mdelay(100);

    /* i2c interface bus */
    gtxx_client = (struct rt_i2c_client *)rt_calloc(1, sizeof(struct rt_i2c_client));
    gtxx_client->client_addr = GTP_ADDRESS_HIGH;
    gtxx_client->bus = (struct rt_i2c_bus_device *)rt_device_find(cfg.dev_name);
    RT_ASSERT(gtxx_client->bus != RT_NULL);

    ret = rt_device_open((rt_device_t)gtxx_client->bus, RT_DEVICE_FLAG_RDWR);
    RT_ASSERT(ret == RT_EOK);

    /* init gtxx config */
    gtxx_config_set(gtxx_client);

    /* register touch device */
    touch_device = (rt_touch_t)rt_calloc(1, sizeof(struct rt_touch_device));
    RT_ASSERT(touch_device != RT_NULL);

    touch_device->info.type = RT_TOUCH_TYPE_CAPACITANCE;
    touch_device->info.vendor = RT_TOUCH_VENDOR_GT;
    rt_memcpy(&touch_device->config, &cfg, sizeof(struct rt_touch_config));
    touch_device->ops = &touch_ops;
    rt_hw_touch_register(touch_device, touch_name, RT_DEVICE_FLAG_INT_RX, RT_NULL);

    return RT_EOK;
}
INIT_DEVICE_EXPORT(rt_hw_gtxx_init);
