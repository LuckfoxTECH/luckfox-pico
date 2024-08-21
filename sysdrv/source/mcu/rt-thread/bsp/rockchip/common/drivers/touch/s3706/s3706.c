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

#include <rtdbg.h>

#include "touch.h"
#include "s3706.h"

static s3706_device_t *g_s3706_dev;

//#define S3706_TP_DEBUG
#ifdef S3706_TP_DEBUG
#define s3706_tp_dbg(...) \
    do { \
        rt_kprintf("%s: ", __func__);    \
        rt_kprintf(__VA_ARGS__);         \
    } while(0)
#else
#define s3706_tp_dbg(...)
#endif

static void s3706_int_handler(int vector, void *param)
{
    s3706_device_t *dev = g_s3706_dev;

    s3706_tp_dbg("s3706_int_handler\n");

    rt_interrupt_enter();

    rt_hw_interrupt_mask(TP_IRQn);

    rt_sem_release(dev->irq_sem);

    rt_interrupt_leave();
}

static rt_err_t s3706_touch_read(struct rt_i2c_client *dev, void *cmd_buf, size_t cmd_len, void *data_buf, size_t data_len)
{
    struct rt_i2c_msg msgs[2];

    msgs[0].addr  = dev->client_addr;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = cmd_buf;
    msgs[0].len   = cmd_len;

    msgs[1].addr  = dev->client_addr;
    msgs[1].flags = RT_I2C_RD;
    msgs[1].buf   = data_buf;
    msgs[1].len   = data_len;

    if (rt_i2c_transfer(dev->bus, msgs, 2) == 2)
    {
        return RT_EOK;
    }

    return RT_ERROR;
}

static rt_err_t s3706_touch_read_word(struct rt_i2c_client *dev, uint16_t addr, int32_t *value)
{
    rt_err_t retval = 0;
    uint16_t regaddr = addr;
    uint8_t buf[2] = {0};

    retval = s3706_touch_read(dev, &regaddr, 1, buf, 2);
    if (retval != RT_ERROR)
    {
        *value = buf[1] << 8 | buf[0];
        return RT_EOK;
    }
    else
    {
        *value = 0;
        return RT_ERROR;
    }
}

static rt_err_t s3706_touch_write(struct rt_i2c_client *dev, uint16_t regaddr, size_t cmd_len, uint32_t data_buf_arg, size_t data_len)
{
    char *data_buf = NULL;
    uint32_t data_buf_tmp;
    struct rt_i2c_msg msgs[1];
    int i, ret = 0;

    data_buf = (char *)rt_calloc(1, data_len + cmd_len);
    if (!data_buf)
    {
        s3706_tp_dbg("spi write alloc buf size %d fail\n", data_len);
        return RT_ERROR;
    }

    for (i = 0; i < cmd_len; i++)
    {
        data_buf[i] = (regaddr >> (8 * i)) & 0xff;
        s3706_tp_dbg("send[%x]: 0x%x\n", i, data_buf[i]);
    }

    for (i = cmd_len; i < data_len + cmd_len; i++)
    {
        data_buf_tmp = (uint8_t)(data_buf_arg & 0x000000ff);
        data_buf[i] = data_buf_tmp;
        data_buf_arg = data_buf_arg >> 8;
    }

    msgs[0].addr  = dev->client_addr;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = (rt_uint8_t *)data_buf;
    msgs[0].len   = data_len;

    ret = rt_i2c_transfer(dev->bus, msgs, 1);

    rt_free(data_buf);

    if (ret == 1)
        return RT_EOK;
    else
        return RT_ERROR;
}

static void print_s3706_reg(s3706_device_t *dev)
{
    s3706_tp_dbg("s3706_reg.F12_2D_QUERY_BASE = 0x%x.\n", dev->reg.F12_2D_QUERY_BASE);
    s3706_tp_dbg("s3706_reg.F12_2D_CMD_BASE = 0x%x.\n", dev->reg.F12_2D_CMD_BASE);
    s3706_tp_dbg("s3706_reg.F12_2D_CTRL_BASE = 0x%x.\n", dev->reg.F12_2D_CTRL_BASE);
    s3706_tp_dbg("s3706_reg.F12_2D_DATA_BASE = 0x%x.\n", dev->reg.F12_2D_DATA_BASE);

    s3706_tp_dbg("s3706_reg.F01_RMI_QUERY_BASE = 0x%x.\n", dev->reg.F01_RMI_QUERY_BASE);
    s3706_tp_dbg("s3706_reg.F01_RMI_CMD_BASE = 0x%x.\n", dev->reg.F01_RMI_CMD_BASE);
    s3706_tp_dbg("s3706_reg.F01_RMI_CTRL_BASE = 0x%x.\n", dev->reg.F01_RMI_CTRL_BASE);
    s3706_tp_dbg("s3706_reg.F01_RMI_DATA_BASE = 0x%x.\n", dev->reg.F01_RMI_DATA_BASE);

    s3706_tp_dbg("s3706_reg.F34_FLASH_QUERY_BASE = 0x%x.\n", dev->reg.F34_FLASH_QUERY_BASE);
    s3706_tp_dbg("s3706_reg.F34_FLASH_CMD_BASE = 0x%x.\n", dev->reg.F34_FLASH_CMD_BASE);
    s3706_tp_dbg("s3706_reg.F34_FLASH_CTRL_BASE = 0x%x.\n", dev->reg.F34_FLASH_CTRL_BASE);
    s3706_tp_dbg("s3706_reg.F34_FLASH_DATA_BASE = 0x%x.\n", dev->reg.F34_FLASH_DATA_BASE);
}

rt_err_t s3706_touch_device_init(s3706_device_t *dev)
{
    uint16_t regaddr = 0;
    char *data_buf = NULL;
    uint32_t data_len = 0;

    /* page 0 */
    s3706_touch_write(dev->i2c_client, 0xff, 1, 0, 1);

    /* read register info */
    data_len = 4;
    data_buf = (char *)rt_calloc(1, data_len);

    /* read register info */
    regaddr = 0xdd;
    s3706_touch_read(dev->i2c_client, &regaddr, 1, data_buf, data_len);
    dev->reg.F12_2D_QUERY_BASE = data_buf[0];
    dev->reg.F12_2D_CMD_BASE = data_buf[1];
    dev->reg.F12_2D_CTRL_BASE = data_buf[2];
    dev->reg.F12_2D_DATA_BASE = data_buf[3];

    regaddr = 0xe3;
    s3706_touch_read(dev->i2c_client, &regaddr, 1, data_buf, data_len);
    dev->reg.F01_RMI_QUERY_BASE = data_buf[0];
    dev->reg.F01_RMI_CMD_BASE = data_buf[1];
    dev->reg.F01_RMI_CTRL_BASE = data_buf[2];
    dev->reg.F01_RMI_DATA_BASE = data_buf[3];

    regaddr = 0xe9;
    s3706_touch_read(dev->i2c_client, &regaddr, 1, data_buf, data_len);
    dev->reg.F34_FLASH_QUERY_BASE = data_buf[0];
    dev->reg.F34_FLASH_CMD_BASE = data_buf[1];
    dev->reg.F34_FLASH_CTRL_BASE =  data_buf[2];
    dev->reg.F34_FLASH_DATA_BASE = data_buf[3];

    dev->reg.F01_RMI_QUERY11 = dev->reg.F01_RMI_QUERY_BASE + 0x0b;    // product id
    dev->reg.F01_RMI_CTRL00 = dev->reg.F01_RMI_CTRL_BASE;
    dev->reg.F01_RMI_CTRL01 = dev->reg.F01_RMI_CTRL_BASE + 1;
    dev->reg.F01_RMI_CTRL02 = dev->reg.F01_RMI_CTRL_BASE + 2;
    dev->reg.F01_RMI_CMD00  = dev->reg.F01_RMI_CMD_BASE;
    dev->reg.F01_RMI_DATA01 = dev->reg.F01_RMI_DATA_BASE + 1;

    dev->reg.F12_2D_CTRL08 = dev->reg.F12_2D_CTRL_BASE;               // max XY Coordinate
    dev->reg.F12_2D_CTRL23 = dev->reg.F12_2D_CTRL_BASE + 9;           //glove enable
    dev->reg.F12_2D_CTRL32 = dev->reg.F12_2D_CTRL_BASE + 0x0f;        //moisture enable
    dev->reg.F12_2D_DATA04 = dev->reg.F12_2D_DATA_BASE + 1;           //gesture type
    dev->reg.F12_2D_DATA15 = dev->reg.F12_2D_DATA_BASE + 3;           //object attention
    dev->reg.F12_2D_CMD00  = dev->reg.F12_2D_CMD_BASE;
    dev->reg.F12_2D_CTRL20 = dev->reg.F12_2D_CTRL_BASE + 0x07;        //motion suppression
    dev->reg.F12_2D_CTRL27 = dev->reg.F12_2D_CTRL_BASE + 0x0b;        // wakeup Gesture enable

    print_s3706_reg(dev);

    s3706_touch_write(dev->i2c_client, 0xff, 1, 0x4, 1); /* page 4*/

    /* read firmware info */
    regaddr = dev->reg.F34_FLASH_CTRL_BASE;
    s3706_tp_dbg("F34_FLASH_CTRL_BASE = 0x%x.\n", regaddr);
    s3706_touch_read(dev->i2c_client, &regaddr, 1, data_buf, 4);

#ifdef TP_DEBUG
    {
        uint32_t current_firmware = 0;
        current_firmware = (data_buf[0] << 24) | (data_buf[1] << 16) | (data_buf[2] << 8) | data_buf[3];
        s3706_tp_dbg("S3706 init CURRENT_FIRMWARE_ID = 0x%x\n", current_firmware);
    }
#endif

    rt_hw_interrupt_install(TP_IRQn,
                            (rt_isr_handler_t)s3706_int_handler,
                            RT_NULL,
                            RT_NULL);
    rt_hw_interrupt_umask(TP_IRQn);

    return RT_EOK;
}

static uint32_t s3706_trigger_reason(s3706_device_t *dev, int32_t gesture_enable, int32_t is_suspended)
{
    rt_err_t ret_code = 0;
    int32_t ret_value = 0;
    uint8_t device_status = 0, interrupt_status = 0;
    uint32_t result_event = 0;

    ret_code = s3706_touch_write(dev->i2c_client, 0xff, 1, 0, 1);  /* page 0*/
    if (ret_code != RT_EOK)
    {
        s3706_tp_dbg("s3706_touch_write error, ret_code = %d\n", ret_code);
        return IRQ_EXCEPTION;
    }

    ret_code = s3706_touch_read_word(dev->i2c_client, dev->reg.F01_RMI_DATA_BASE, &ret_value);
    if (ret_code != RT_EOK)
    {
        s3706_tp_dbg("s3706_touch_write error, ret_code = %d\n", ret_code);
        return IRQ_EXCEPTION;
    }
    device_status = ret_value & 0xff;
    interrupt_status = (ret_value & 0x7f00) >> 8;
    s3706_tp_dbg("interrupt_status = 0x%x, device_status = 0x%x\n", interrupt_status, device_status);

    if (device_status && device_status != 0x81)
    {
        s3706_tp_dbg("interrupt_status = 0x%x, device_status = 0x%x\n", interrupt_status, device_status);
        return IRQ_EXCEPTION;
    }

    if (interrupt_status & 0x04)
    {
        if (gesture_enable && is_suspended)
        {
            return IRQ_GESTURE;
        }
        else if (is_suspended)
        {
            return IRQ_IGNORE;
        }
        TP_SET_BIT(result_event, IRQ_TOUCH);
        TP_SET_BIT(result_event, IRQ_FINGERPRINT);
    }
    if (interrupt_status & 0x10)
    {
        TP_SET_BIT(result_event, IRQ_BTN_KEY);
    }
    if (interrupt_status & 0x20)
    {
        TP_SET_BIT(result_event, IRQ_FACE_STATE);
        TP_SET_BIT(result_event, IRQ_FW_HEALTH);
    }

    return result_event;
}

static rt_err_t s3706_get_touch_points(s3706_device_t *dev, struct point_info *points, int32_t max_num)
{
    rt_err_t   ret_code = 0;
    int32_t    i, obj_attention, atten;
    rt_uint8_t fingers_to_process;
    char *buf = dev->point_buf;;
    struct point_info *p = points;

    ret_code = s3706_touch_read_word(dev->i2c_client, dev->reg.F12_2D_DATA15, &obj_attention);
    if (ret_code != RT_EOK)
    {
        rt_kprintf("s3706_touch_read_word error, ret_code = %d\n", ret_code);
        return RT_ERROR;
    }
    s3706_tp_dbg("obj_attention = 0x%x\n", obj_attention);

    obj_attention = obj_attention & ((0x01L << max_num) - 1);
    if (obj_attention == 0)
    {
        rt_mutex_take(dev->read_mutex, RT_WAITING_FOREVER);
        dev->obtain_fingers = 0;
        rt_mutex_release(dev->read_mutex);
        return RT_EOK;
    }

    for (fingers_to_process = 0, atten = obj_attention; atten != 0;
            fingers_to_process++,   atten = atten >> 1);

    memset(buf, 0, sizeof(dev->point_buf));
    ret_code = s3706_touch_read(dev->i2c_client, &dev->reg.F12_2D_DATA_BASE, 1, buf, 8 * fingers_to_process);
    if (ret_code == RT_ERROR)
    {
        rt_kprintf("touch i2c read block failed\n");
        return RT_ERROR;
    }

    rt_mutex_take(dev->read_mutex, RT_WAITING_FOREVER);

    dev->obtain_fingers = 0;
    for (i = 0; i < fingers_to_process; i++)
    {
        if (obj_attention & (0x01L << i))
        {
            p->finger_id = i;
            p->status = buf[i * 8];
            p->x = ((buf[i * 8 + 2] & 0x0f) << 8) | (buf[i * 8 + 1] & 0xff);
            p->y = ((buf[i * 8 + 4] & 0x0f) << 8) | (buf[i * 8 + 3] & 0xff);
            p->z = buf[i * 8 + 5];
            p->touch_major = max(buf[i * 8 + 6], buf[i * 8 + 7]);
            p->width_major = ((buf[i * 8 + 6] & 0x0f) + (buf[i * 8 + 7] & 0x0f)) / 2;
            dev->obtain_fingers++;

            s3706_tp_dbg("id = %d, status = 0x%x, x = %d, y = %d, z = %d, width = %d, touch_major = 0x%x\n",
                         p->finger_id,
                         p->status,
                         p->x,
                         p->y,
                         p->z,
                         p->width_major,
                         p->touch_major);
            p++;
        }
    }

    rt_mutex_release(dev->read_mutex);

    return RT_EOK;
}

static rt_err_t s3706_touch_handle(s3706_device_t *dev)
{
    rt_err_t ret = RT_EOK;

    ret = s3706_get_touch_points(dev, dev->points, MAX_FINGER_NUM);
    if (ret != RT_EOK)
    {
        s3706_tp_dbg("Invalid points, ignore..\n");
        return RT_ERROR;
    }

    /* indicate to read data */
    rt_touch_t touch_dev   = &dev->touch_dev;
    if (touch_dev->parent.rx_indicate)
    {
        touch_dev->parent.rx_indicate(&touch_dev->parent, 1);
    }

    return ret;
}

static int16_t pre_x[MAX_FINGER_NUM] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
static int16_t pre_y[MAX_FINGER_NUM] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
static int16_t pre_w[MAX_FINGER_NUM] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
static rt_uint8_t s_tp_dowm[MAX_FINGER_NUM];
static struct rt_touch_data *read_data;

static void s3706_touch_up(void *buf, int8_t id)
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

static void s3706_touch_down(void *buf, int8_t id, int16_t x, int16_t y, int16_t w)
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

static rt_size_t s3706_read_point(struct rt_touch_device *touch, void *buf, rt_size_t read_num)
{
    static rt_uint8_t pre_touch = 0;
    static int8_t pre_id[MAX_FINGER_NUM] = {0};

    s3706_device_t *dev  = g_s3706_dev;
    rt_uint8_t touch_num = dev->obtain_fingers;
    struct point_info    *points  = dev->points;
    uint8_t read_index = 0;
    int8_t  read_id = 0;
    int16_t input_x = 0;
    int16_t input_y = 0;
    int16_t input_w = 0;

    if (touch_num > read_num)
    {
        touch_num = read_num;
    }

    rt_mutex_take(dev->read_mutex, RT_WAITING_FOREVER);

    if (pre_touch > touch_num)                                       /* point up */
    {
        for (read_index = 0; read_index < pre_touch; read_index++)
        {
            rt_uint8_t j;

            for (j = 0; j < touch_num; j++)                          /* this time touch num */
            {
                read_id = points[j].finger_id;

                if (pre_id[read_index] == read_id)                   /* this id is not free */
                    break;

                if (j >= touch_num - 1)
                {
                    rt_uint8_t up_id;
                    up_id = pre_id[read_index];
                    s3706_touch_up(buf, up_id);
                }
            }
        }
    }

    if (touch_num)                                                 /* point down */
    {
        for (read_index = 0; read_index < touch_num; read_index++)
        {
            read_id = points[read_index].finger_id;
            pre_id[read_index] = read_id;
            input_x = points[read_index].x;
            input_y = points[read_index].y;
            input_w = points[read_index].width_major;

            s3706_touch_down(buf, read_id, input_x, input_y, input_w);
        }
    }
    else if (pre_touch)
    {
        for (read_index = 0; read_index < pre_touch; read_index++)
        {
            s3706_touch_up(buf, pre_id[read_index]);
        }
    }
    pre_touch = touch_num;

    rt_mutex_release(dev->read_mutex);

    return read_index;
}

static rt_err_t s3706_control(struct rt_touch_device *device, int cmd, void *data)
{
    rt_err_t ret = RT_EOK;

    if (cmd == RT_TOUCH_CTRL_GET_ID)
    {
        char *id = (char *)data;
        id[0] = 's';
        id[1] = '3';
        id[2] = '7';
        id[3] = '0';
        id[4] = '6';
        id[5] = 0;
    }
    else if (cmd == RT_TOUCH_CTRL_GET_INFO)
    {
        struct rt_touch_info *info = (struct rt_touch_info *)data;
        info->range_x   = 1080;
        info->range_y   = 2340;
        info->point_num = MAX_FINGER_NUM;
    }

    return ret;
}

static struct rt_touch_ops touch_ops =
{
    .touch_readpoint = s3706_read_point,
    .touch_control   = s3706_control,
};

static void s3706_touch_thread(void *parameter)
{
    s3706_device_t *dev = g_s3706_dev;
    rt_uint32_t cur_event = 0;

    while (1)
    {
        if (rt_sem_take(dev->irq_sem, RT_WAITING_FOREVER) != RT_EOK)
        {
            continue;
        }

        s3706_tp_dbg("touch thread take sem.\n");

        cur_event = s3706_trigger_reason(dev, 0, 0);

        if (TP_CHK_BIT(cur_event, IRQ_TOUCH) || TP_CHK_BIT(cur_event, IRQ_BTN_KEY) || TP_CHK_BIT(cur_event, IRQ_FW_HEALTH) || \
                TP_CHK_BIT(cur_event, IRQ_FACE_STATE) || TP_CHK_BIT(cur_event, IRQ_FINGERPRINT))
        {
            if (TP_CHK_BIT(cur_event, IRQ_BTN_KEY))
            {
                // TODO tp_btnkey_handle(ts);
            }
            if (TP_CHK_BIT(cur_event, IRQ_TOUCH))
            {
                s3706_touch_handle(dev);
            }
            if (TP_CHK_BIT(cur_event, IRQ_FW_HEALTH) /*&& (!ts->is_suspended)*/)
            {
                // TODO health_monitor_handle(ts);
            }
            if (TP_CHK_BIT(cur_event, IRQ_FACE_STATE) /*&& ts->fd_enable*/)
            {
                // TODO tp_face_detect_handle(ts);
            }
            if (TP_CHK_BIT(cur_event, IRQ_FINGERPRINT) /*&& ts->fp_enable*/)
            {
                // TODO tp_fingerprint_handle(ts);
            }
        }
        else if (TP_CHK_BIT(cur_event, IRQ_GESTURE))
        {
            // TODO tp_gesture_handle(ts);
        }
        else if (TP_CHK_BIT(cur_event, IRQ_EXCEPTION))
        {
            // TODO tp_exception_handle(ts);
        }
        else if (TP_CHK_BIT(cur_event, IRQ_FW_CONFIG))
        {
            // TODO tp_config_handle(ts);
        }
        else if (TP_CHK_BIT(cur_event, IRQ_FW_AUTO_RESET))
        {
            // TODO tp_fw_auto_reset_handle(ts);
        }
        else
        {
            s3706_tp_dbg("unknown irq trigger reason.\n");
        }

        rt_hw_interrupt_umask(TP_IRQn);
    }
}

int rt_hw_s3706_init(void)
{
    rt_err_t ret;
    s3706_device_t *dev;
    struct rt_touch_config cfg;
    const char *touch_name = TOUCH_DEV_NAME;

    cfg.irq_pin.pin = RT_PIN_NONE;
    cfg.dev_name    = TOUCH_I2C_DEV;

    /* init s3706 device */
    g_s3706_dev = dev = (s3706_device_t *)rt_malloc(sizeof(s3706_device_t));
    RT_ASSERT(dev != RT_NULL);
    rt_memset((void *)dev, 0, sizeof(s3706_device_t));

    /* i2c interface bus */
    struct rt_i2c_client *i2c_client;
    dev->i2c_client = i2c_client = (struct rt_i2c_client *)rt_malloc(sizeof(struct rt_i2c_client));
    RT_ASSERT(i2c_client != RT_NULL);

    i2c_client->client_addr = S3706_ADDR;
    i2c_client->bus = (struct rt_i2c_bus_device *)rt_device_find(cfg.dev_name);
    RT_ASSERT(i2c_client->bus != RT_NULL);

    ret = rt_device_open((rt_device_t)i2c_client->bus, RT_DEVICE_FLAG_RDWR);
    RT_ASSERT(ret == RT_EOK);

    /* register rt_touch */
    rt_touch_t touch_dev   = &dev->touch_dev;
    touch_dev->info.type   = RT_TOUCH_TYPE_CAPACITANCE;
    touch_dev->info.vendor = RT_TOUCH_VENDOR_UNKNOWN;
    touch_dev->ops         = &touch_ops;
    rt_memcpy(&touch_dev->config, &cfg, sizeof(struct rt_touch_config));
    rt_hw_touch_register(touch_dev, touch_name, RT_DEVICE_FLAG_INT_RX, RT_NULL);

    /* create irq sem */
    dev->irq_sem = rt_sem_create("s3706_isr_sem", 0, RT_IPC_FLAG_FIFO);
    RT_ASSERT(dev->irq_sem != RT_NULL);

    /* create read mutex */
    dev->read_mutex = rt_mutex_create("s3706_read_mutex", RT_IPC_FLAG_FIFO);
    RT_ASSERT(dev->read_mutex != RT_NULL);

    s3706_touch_device_init(dev);

    rt_thread_t thread;
    thread = rt_thread_create("tpdrv", s3706_touch_thread, RT_NULL, 2048, 4, 10);
    RT_ASSERT(thread != RT_NULL);
    rt_thread_startup(thread);

    return RT_EOK;
}
INIT_DEVICE_EXPORT(rt_hw_s3706_init);
