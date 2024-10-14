/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    uac1.c
  * @version V0.1
  * @brief   usb audio class 1.0 driver
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-10-15     Liangfeng Wu    first implementation
  * 2019-10-15     Zhihua Wang     first implementation
  *
  ******************************************************************************
  */
#include <rthw.h>
#include <rtthread.h>
#include <rtservice.h>
#include <rtdevice.h>
#include "uac1.h"
#include "dma.h"
#include "rk_audio.h"
#include "drv_heap.h"

#define F_AUDIO_NUM_INTERFACES      2 /* Number of streaming interfaces */
#define USB_OUT_IT_ID               1
#define USB_OUT_IT_FU_ID            2
#define IO_OUT_OT_ID                3
#define IO_IN_IT_ID                 4
#define IO_IN_IT_FU_ID              5
#define USB_IN_OT_ID                6
#if defined(RKMCU_RK2106)
#define IN_EP_MAX_PACKET_SIZE       64 /* Max SamFreq 48KHz in HS */
#else
#define IN_EP_MAX_PACKET_SIZE       384 /* Max SamFreq 96KHz in FS */
#endif
#define OUT_EP_MAX_PACKET_SIZE      384 /* Max SamFreq 96KHz in FS */
#define SAMPLE_RATE_MAX             96000
#define SAMPLE_RATE_MIN             8000
#define VOLUME_RES                  0x0080 /* 0.5 dB */
#define VOLUME_MAX_USB              0x1900 /* 25 dB */
#define VOLUME_MIN_USB              0xE700 /* -25 dB */
#define VOLUME_NEGATIVE_INFINITY    0x8000

#define P_NR_CHANNEL                2 /* Number of playback channels */
#define P_BUFFER_NUM                18
#define P_BUFFER_TOP_NUM            9
#define P_BUFFER_READY_NUM          6
#define P_BUFFER_BOTTOM_NUM         3
#define P_DEFAULT_SAMPLE_RATE       48000
#define P_AUDIO_PERIOD_COUNT        2
#define P_AUDIO_SAMPLE_BITS         16
#define P_AUDIO_PERIOD_SIZE         1024
#define P_MB_POOL_SIZE              (P_BUFFER_NUM + 8)
#define P_TOTAL_SIZE                (P_AUDIO_PERIOD_SIZE * P_NR_CHANNEL * (P_AUDIO_SAMPLE_BITS >> 3))
#define P_PERIOD_SIZE_MS            10

#define C_NR_CHANNEL                2 /*Number of capture channels */
#define C_BUFFER_NUM                32
#define C_DEFAULT_SAMPLE_RATE       48000
#define C_AUDIO_PERIOD_COUNT        4
#define C_AUDIO_SAMPLE_BITS         16
#define C_AUDIO_PERIOD_SIZE         1024
#define C_MB_POOL_SIZE              (C_BUFFER_NUM + 8)
#define C_TOTAL_SIZE                (C_AUDIO_PERIOD_SIZE * C_NR_CHANNEL * (C_AUDIO_SAMPLE_BITS >> 3))

#define FREQ(f) { ((f) & 0xFF), ((f) >> 8 & 0xFF), ((f) >> 16 & 0xFF) }
#define UAC_DT_AC_HEADER_LENGTH UAC_DT_AC_HEADER_SIZE(F_AUDIO_NUM_INTERFACES)
/* 2 input terminal, 2 output terminal and 2 feature unit */
#define UAC_DT_TOTAL_LENGTH (UAC_DT_AC_HEADER_LENGTH \
        + UAC_DT_INPUT_TERMINAL_SIZE * 2 + UAC_DT_OUTPUT_TERMINAL_SIZE * 2 \
        + UAC_DT_FEATURE_UNIT_SIZE(0) * 2)
#define VOLUME_USB_TO_DB(v_usb) (v_usb <= 0x7FFF) ? v_usb: \
        (- (((rt_int16_t)0xFFFF - v_usb)+1))
#define VOLUME_DB_TO_PERCENT(v_db, v_max_db, v_min_db) \
        ((rt_uint8_t)((((rt_int16_t)(v_db) - v_min_db)*100) /\
        ((rt_int16_t)v_max_db - v_min_db)))
#define UAC_SET_VOL_MUTE_INTERVAL 1000
#define UAC_SOF_COUNT 250000
#undef  ABS
#define ABS(X)              (((X) < 0) ? (-(X)) : (X))

#ifdef RT_USB_AUDIO_PLL_COMPENSATION
#define CPLL_COMPENSATION   10
#define PLL_CPLL_8000       245760000
#define I2S1_MCLKOUT_8000   12288000
#define PLL_CPLL_44100      282240000
#define I2S1_MCLKOUT_44100  28224000
#endif

enum msg_type
{
    MSG_DATA_NOTIFY = 1,
    MSG_START,
    MSG_STOP,
};

struct usb_audio_buf
{
    char *buffer;
    rt_list_t list;
};

struct audio_play
{
    rt_bool_t enable;
    rt_uint32_t period_size;
    rt_uint32_t total_size;
    rt_uint8_t cur_mute;
    rt_uint8_t set_mute;
    rt_uint16_t cur_volume;
    rt_uint16_t set_volume;
    rt_uint16_t res_volume;
    rt_uint32_t sample_rate;
    rt_uint32_t mb_pool[P_MB_POOL_SIZE];
    rt_size_t recv_size;
    rt_bool_t set_sample_rate;
    rt_list_t free_list;
    rt_list_t ready_list;
    struct rt_device *audio_dev;
    struct audio_buf abuf;
    struct usb_audio_buf pbuf[P_BUFFER_NUM];
    struct rt_mailbox mb;
};

struct audio_capture
{
    rt_uint8_t frame_cnt;
    rt_uint8_t cur_mute;
    rt_uint8_t set_mute;
    rt_uint16_t cur_volume;
    rt_uint16_t set_volume;
    rt_uint16_t res_volume;
    rt_uint32_t sample_rate;
    rt_uint32_t mb_pool[C_MB_POOL_SIZE];
    rt_size_t send_size;
    rt_bool_t set_sample_rate;
    rt_list_t free_list;
    rt_list_t ready_list;
    struct rt_device *audio_dev;
    struct audio_buf abuf;
    struct usb_audio_buf cbuf[C_BUFFER_NUM];
    struct rt_mailbox mb;
};

struct uac1
{
    rt_uint8_t req;
    uep_t ep_out;
    uep_t ep_in;
    rt_uint8_t ac_intf, as_in_intf, as_out_intf;
    rt_uint8_t in_intf_alt;
    rt_uint8_t out_intf_alt;
    rt_uint8_t *ep0_buffer;
    struct audio_play p;
    struct audio_capture c;
};

static struct uac1 *g_uac1 = RT_NULL;

ALIGN(4)
const static char *_ustring[] =
{
    "Language",
    "Rockchip",
    "UAC1",
    "123456789",
    "Configuration",
};

ALIGN(4)
static struct udevice_descriptor dev_desc =
{
    USB_DESC_LENGTH_DEVICE,
    USB_DESC_TYPE_DEVICE,
    USB_DYNAMIC,
    0x00,
    0x00,
    0x00,
    0x40,
    _VENDOR_ID,
    _PRODUCT_ID,
    USB_BCD_DEVICE,
    USB_STRING_MANU_INDEX,
    USB_STRING_PRODUCT_INDEX,
    USB_STRING_SERIAL_INDEX,
    USB_DYNAMIC,
};

/*
 * USB-OUT -> IT_1 -> FU_2 -> OT_3 -> Playback
 * Capture -> IT_4 -> FU_5 -> OT_6 -> USB-IN
 */
ALIGN(4)
static struct uac1_control _control_desc =
{
#ifdef RT_USB_DEVICE_COMPOSITE
    /* Interface Association Descriptor */
    {
        USB_DESC_LENGTH_IAD,
        USB_DESC_TYPE_IAD,
        USB_DYNAMIC,
        0x03,
        USB_CLASS_AUDIO,
        USB_SUBCLASS_AUDIOSTREAMING,
        0x00,
        0x00,
    },
#endif
    /* Audio Control Interface Descriptor */
    {
        USB_DESC_LENGTH_INTERFACE,
        USB_DESC_TYPE_INTERFACE,
        USB_DYNAMIC,
        0x00,
        0x00,
        USB_CLASS_AUDIO,
        USB_SUBCLASS_AUDIOCONTROL,
        0x00,
        0x00,
    },
    /* Audio Control Interface Header Descriptor */
    {
        UAC_DT_AC_HEADER_LENGTH,
        USB_DT_CS_INTERFACE,
        UAC_HEADER,
        0x0100,
        UAC_DT_TOTAL_LENGTH,
        F_AUDIO_NUM_INTERFACES,
        {
            /* Interface number of the AudioStream interfaces */
            /* baInterfaceNr[0] = DYNAMIC, */
            /* baInterfaceNr[1] = DYNAMIC, */
        },
    },
    {
        UAC_DT_INPUT_TERMINAL_SIZE,
        USB_DT_CS_INTERFACE,
        UAC_INPUT_TERMINAL,
        USB_OUT_IT_ID,
        UAC_TERMINAL_STREAMING,
        IO_OUT_OT_ID,
        P_NR_CHANNEL,
        0x0003,
        0x00,
        0x00,
    },
    {
        UAC_DT_OUTPUT_TERMINAL_SIZE,
        USB_DT_CS_INTERFACE,
        UAC_OUTPUT_TERMINAL,
        IO_OUT_OT_ID,
        UAC_OUTPUT_TERMINAL_SPEAKER,
        USB_OUT_IT_ID,
        USB_OUT_IT_FU_ID,
        0x00,
    },
    {
        UAC_DT_FEATURE_UNIT_SIZE(0),
        USB_DT_CS_INTERFACE,
        UAC_FEATURE_UNIT,
        USB_OUT_IT_FU_ID,
        USB_OUT_IT_ID,
        0x02,
        {
            UAC_FU_MUTE | UAC_FU_VOLUME,
        },
        0x00,
    },
    {
        UAC_DT_INPUT_TERMINAL_SIZE,
        USB_DT_CS_INTERFACE,
        UAC_INPUT_TERMINAL,
        IO_IN_IT_ID,
        UAC_INPUT_TERMINAL_MICROPHONE,
        USB_IN_OT_ID,
        C_NR_CHANNEL,
        0x0003,
        0x00,
        0x00,
    },
    {
        UAC_DT_OUTPUT_TERMINAL_SIZE,
        USB_DT_CS_INTERFACE,
        UAC_OUTPUT_TERMINAL,
        USB_IN_OT_ID,
        UAC_TERMINAL_STREAMING,
        IO_IN_IT_ID,
        IO_IN_IT_FU_ID,
        0x00,
    },
    {
        UAC_DT_FEATURE_UNIT_SIZE(0),
        USB_DT_CS_INTERFACE,
        UAC_FEATURE_UNIT,
        IO_IN_IT_FU_ID,
        IO_IN_IT_ID,
        0x02,
        {
            UAC_FU_MUTE | UAC_FU_VOLUME,
        },
        0x00,
    }
};

ALIGN(4)
static struct uinterface_descriptor _as_out_intf_alt0_desc =
{
    USB_DESC_LENGTH_INTERFACE,
    USB_DESC_TYPE_INTERFACE,
    USB_DYNAMIC,
    0x00,
    0x00,
    USB_CLASS_AUDIO,
    USB_SUBCLASS_AUDIOSTREAMING,
    0x00,
    0x00,
};

ALIGN(4)
static struct uac1_interface_alt _as_out_intf_alt1_desc =
{
    {
        USB_DESC_LENGTH_INTERFACE,
        USB_DESC_TYPE_INTERFACE,
        USB_DYNAMIC,
        0x01,
        0x01,
        USB_CLASS_AUDIO,
        USB_SUBCLASS_AUDIOSTREAMING,
        0x00,
        0x00,
    },
    {
        UAC_DT_AS_HEADER_SIZE,
        USB_DT_CS_INTERFACE,
        UAC_AS_GENERAL,
        USB_OUT_IT_ID,
        0x01,
        UAC_FORMAT_TYPE_I_PCM,
    },
    {
        UAC_FORMAT_TYPE_I_DISCRETE_DESC_SIZE(6),
        USB_DT_CS_INTERFACE,
        UAC_FORMAT_TYPE,
        UAC_FORMAT_TYPE_I,
        P_NR_CHANNEL,
        0x02,
        0x10,
        0x06,
        {
            FREQ(96000),
            FREQ(48000),
            FREQ(44100),
            FREQ(32000),
            FREQ(16000),
            FREQ(8000),
        },
    },
    {
        USB_DESC_LENGTH_AUDIO_ENDPOINT,
        USB_DESC_TYPE_ENDPOINT,
        USB_DYNAMIC | USB_DIR_OUT,
        USB_ENDPOINT_SYNC_ADAPTIVE | USB_EP_ATTR_ISOC,
        OUT_EP_MAX_PACKET_SIZE,
        USB_DYNAMIC, /* bInterval */
        0x00,
        0x00,
    },
    {
        UAC_ISO_ENDPOINT_DESC_SIZE,
        USB_DT_CS_ENDPOINT,
        UAC_EP_GENERAL,
        0x01,
        0x01,
        0x01,
    }
};

ALIGN(4)
static struct uinterface_descriptor _as_in_intf_alt0_desc =
{
    USB_DESC_LENGTH_INTERFACE,
    USB_DESC_TYPE_INTERFACE,
    USB_DYNAMIC,
    0x00,
    0x00,
    USB_CLASS_AUDIO,
    USB_SUBCLASS_AUDIOSTREAMING,
    0x00,
    0x00,
};

ALIGN(4)
static struct uac1_interface_alt _as_in_intf_alt1_desc =
{
    {
        USB_DESC_LENGTH_INTERFACE,
        USB_DESC_TYPE_INTERFACE,
        USB_DYNAMIC,
        0x01,
        0x01,
        USB_CLASS_AUDIO,
        USB_SUBCLASS_AUDIOSTREAMING,
        0x00,
        0x00,
    },
    {
        UAC_DT_AS_HEADER_SIZE,
        USB_DT_CS_INTERFACE,
        UAC_AS_GENERAL,
        USB_IN_OT_ID,
        0x01,
        UAC_FORMAT_TYPE_I_PCM,
    },
    {
        UAC_FORMAT_TYPE_I_DISCRETE_DESC_SIZE(6),
        USB_DT_CS_INTERFACE,
        UAC_FORMAT_TYPE,
        UAC_FORMAT_TYPE_I,
        C_NR_CHANNEL,
        0x02,
        0x10,
        0x06,
        {
            FREQ(96000),
            FREQ(48000),
            FREQ(44100),
            FREQ(32000),
            FREQ(16000),
            FREQ(8000),
        },
    },
    {
        USB_DESC_LENGTH_AUDIO_ENDPOINT,
        USB_DESC_TYPE_ENDPOINT,
        USB_DYNAMIC | USB_DIR_IN,
        USB_ENDPOINT_SYNC_ASYNC | USB_EP_ATTR_ISOC,
        IN_EP_MAX_PACKET_SIZE,
        USB_DYNAMIC, /* bInterval*/
        0x00,
        0x00,
    },
    {
        UAC_ISO_ENDPOINT_DESC_SIZE,
        USB_DT_CS_ENDPOINT,
        UAC_EP_GENERAL,
        0x01,
        0x01,
        0x01,
    }
};

static rt_err_t _ep0_set_sample_rate_handler(udevice_t device, rt_size_t size);

static rt_err_t _audio_set_endpoint_req(ufunction_t func, ureq_t setup)
{
    rt_uint16_t len = setup->wLength;
    rt_uint16_t w_value = setup->wValue;
    rt_uint8_t intf;

    if ((setup->wIndex & 0x80) == (_as_out_intf_alt1_desc.endpoint_desc.bEndpointAddress & 0x80))
        intf = _as_out_intf_alt1_desc.interface_desc.bInterfaceNumber;
    else
        intf = _as_in_intf_alt1_desc.interface_desc.bInterfaceNumber;

    if (w_value == UAC_EP_CS_ATTR_SAMPLE_RATE << 8)
    {
        switch (setup->bRequest)
        {
        case UAC_SET_CUR:
            if (len > 0)
            {
                if (intf == g_uac1->as_out_intf)
                    g_uac1->p.set_sample_rate = true;
                else
                    g_uac1->c.set_sample_rate = true;
                rt_usbd_ep0_read(func->device, g_uac1->ep0_buffer, len,
                                 _ep0_set_sample_rate_handler);
            }
            break;
        case UAC_SET_MIN:
            /* fallthrough */
        case UAC_SET_MAX:
            /* fallthrough */
        case UAC_SET_RES:
            /* fallthrough */
        default:
            rt_kprintf("%s: no response to req %d", __func__, setup->bRequest);
            rt_usbd_ep0_set_stall(func->device);
            break;
        }
    }

    return RT_EOK;
}

static rt_err_t _audio_get_endpoint_req(ufunction_t func, ureq_t setup)
{
    rt_uint8_t len = 0;
    rt_uint16_t w_value = setup->wValue;
    ALIGN(4) rt_uint8_t buf[3];
    rt_uint8_t intf;

    if ((setup->wIndex & 0x80) == (_as_out_intf_alt1_desc.endpoint_desc.bEndpointAddress & 0x80))
        intf = _as_out_intf_alt1_desc.interface_desc.bInterfaceNumber;
    else
        intf = _as_in_intf_alt1_desc.interface_desc.bInterfaceNumber;

    rt_uint32_t rate = (intf == g_uac1->as_out_intf ? g_uac1->p.sample_rate :
                        g_uac1->c.sample_rate);
    if (w_value == UAC_EP_CS_ATTR_SAMPLE_RATE << 8)
    {
        switch (setup->bRequest)
        {
        case UAC_GET_CUR:
            buf[0] = rate;
            buf[1] = rate >> 8;
            buf[2] = rate >> 16;
            len = 3;
            break;

        case UAC_GET_MIN:
            buf[0] = (rt_uint8_t)SAMPLE_RATE_MIN;
            buf[1] = (rt_uint8_t)(SAMPLE_RATE_MIN >> 8);
            buf[2] = (rt_uint8_t)(SAMPLE_RATE_MIN >> 16);
            len = 3;
            break;

        case UAC_GET_MAX:
            buf[0] = (rt_uint8_t)SAMPLE_RATE_MAX;
            buf[1] = (rt_uint8_t)(SAMPLE_RATE_MAX >> 8);
            buf[2] = (rt_uint8_t)(SAMPLE_RATE_MAX >> 16);
            len = 3;
            break;

        case UAC_GET_RES:
            /* TODO */
            /* return our sample rate? */
            buf[0] = rate;
            buf[1] = rate >> 8;
            buf[2] = rate >> 16;
            len = 3;
            break;
        default:
            rt_usbd_ep0_set_stall(func->device);
            break;
        }
    }

    if (len > 0)
        rt_usbd_ep0_write(func->device, buf, len);

    return RT_EOK;
}

static rt_err_t _audio_playback_card_enable(struct audio_play *p)
{
    struct AUDIO_PARAMS param;
    rt_err_t ret;

    RT_DEBUG_LOG(RT_DEBUG_USB, ("%s\n", __func__));

    if (!p->audio_dev)
    {
        p->audio_dev = rt_device_find(RT_USB_AUDIO_P_NAME);

        if (!p->audio_dev)
        {
            rt_kprintf("can't find audio dev: %s\n", RT_USB_AUDIO_P_NAME);
            return -RT_ERROR;
        }
    }

    ret = rt_device_open(p->audio_dev, RT_DEVICE_OFLAG_WRONLY);
    if (ret != RT_EOK)
    {
        rt_kprintf("fail to do open audio dev: %s\n", RT_USB_AUDIO_P_NAME);
        return -RT_ERROR;
    }

    p->period_size = p->sample_rate / 1000 * P_PERIOD_SIZE_MS;
    if (p->period_size > P_AUDIO_PERIOD_SIZE)
    {
        rt_kprintf("play period_size %d greater than %d\n", p->period_size, P_AUDIO_PERIOD_SIZE);
        return -RT_ERROR;
    }
    p->total_size = p->period_size * P_NR_CHANNEL * (P_AUDIO_SAMPLE_BITS >> 3);
    p->abuf.period_size = p->period_size;
    p->abuf.buf_size = p->period_size * P_AUDIO_PERIOD_COUNT;
    ret = rt_device_control(p->audio_dev, RK_AUDIO_CTL_PCM_PREPARE, &g_uac1->p.abuf);
    if (ret != RT_EOK)
    {
        rt_kprintf("fail to do RK_AUDIO_CTL_PCM_PREPARE\n");
        return -RT_ERROR;
    }
#ifdef RT_USB_AUDIO_PLL_COMPENSATION
    if (p->sample_rate % 8000 == 0)
    {
        HAL_CRU_ClkSetFreq(PLL_CPLL, PLL_CPLL_8000);
        HAL_CRU_ClkSetFreq(I2S1_MCLKOUT, I2S1_MCLKOUT_8000);
    }
    else if (p->sample_rate % 44100 == 0)
    {
        HAL_CRU_ClkSetFreq(PLL_CPLL, PLL_CPLL_44100);
        HAL_CRU_ClkSetFreq(I2S1_MCLKOUT, I2S1_MCLKOUT_44100);
    }
    else
    {
        rt_kprintf("%s: sample rate: %u unsupported\n", __func__, p->sample_rate);
        return -RT_ERROR;
    }
#endif
    param.channels = P_NR_CHANNEL;
    param.sampleRate = p->sample_rate;
    param.sampleBits = P_AUDIO_SAMPLE_BITS;
    ret = rt_device_control(p->audio_dev, RK_AUDIO_CTL_HW_PARAMS, &param);
    if (ret != RT_EOK)
    {
        rt_kprintf("fail to do RK_AUDIO_CTL_HW_PARAMS\n");
        return -RT_ERROR;
    }

    p->recv_size = 0;
    p->enable = true;

    return RT_EOK;
}

static rt_err_t _audio_playback_card_disable(struct audio_play *p)
{
    rt_err_t ret = RT_EOK;
    struct usb_audio_buf *pbuf;
    rt_base_t level;

    RT_DEBUG_LOG(RT_DEBUG_USB, ("%s\n", __func__));

    p->enable = false;
    /* Clear ready list and insert to free list again */
    level = rt_hw_interrupt_disable();
    while (!rt_list_isempty(&p->ready_list))
    {
        pbuf = rt_list_first_entry(&p->ready_list, struct usb_audio_buf, list);
        rt_list_remove(&pbuf->list);
        rt_list_insert_before(&p->free_list, &pbuf->list);
    }
    rt_hw_interrupt_enable(level);

    if (p->audio_dev)
    {
        ret = rt_device_control(p->audio_dev, RK_AUDIO_CTL_STOP, NULL);
        if (ret != RT_EOK)
        {
            rt_kprintf("fail to do RK_AUDIO_CTL_STOP\n");
            return -RT_ERROR;
        }

        ret = rt_device_control(p->audio_dev, RK_AUDIO_CTL_PCM_RELEASE, NULL);
        if (ret != RT_EOK)
        {
            rt_kprintf("fail to do RK_AUDIO_CTL_PCM_RELEASE\n");
            return -RT_ERROR;
        }

        rt_device_close(p->audio_dev);
        p->audio_dev = RT_NULL;
    }

    return ret;
}

static rt_err_t _audio_playback_card_set_mute(struct audio_play *p)
{
    rt_err_t ret = RT_EOK;
    rt_int32_t volume = 0;
    rt_int16_t v_db, v_max_db, v_min_db;
    struct AUDIO_GAIN_INFO info;
    struct AUDIO_DB_CONFIG db_config;

    if (p->audio_dev && p->cur_mute != p->set_mute)
    {
        ret = rt_device_control(p->audio_dev, RK_AUDIO_CTL_GET_GAIN_INFO, &info);
        if (ret != RT_EOK)
        {
            rt_kprintf("fail to get gain info\n");
            return -RT_ERROR;
        }
        if (p->set_mute)
        {
            ret = rt_device_control(p->audio_dev, RK_AUDIO_CTL_GET_GAIN, &db_config);
            if (ret != RT_EOK)
            {
                rt_kprintf("fail to get gain\n");
                return -RT_ERROR;
            }
            db_config.dB = info.mindB;
            rt_kprintf("db_config.dB = %d\n", db_config.dB);
            ret = rt_device_control(p->audio_dev, RK_AUDIO_CTL_SET_GAIN, &db_config);
            if (ret != RT_EOK)
            {
                rt_kprintf("fail to set gain\n");
                return -RT_ERROR;
            }
        }
        else if (!p->set_mute && p->cur_volume)
        {
            v_db = VOLUME_USB_TO_DB(p->cur_volume);
            v_max_db = VOLUME_USB_TO_DB(VOLUME_MAX_USB);
            v_min_db = VOLUME_USB_TO_DB(VOLUME_MIN_USB);
            volume = VOLUME_DB_TO_PERCENT(v_db, v_max_db, v_min_db);

            ret = rt_device_control(p->audio_dev, RK_AUDIO_CTL_GET_GAIN, &db_config);
            if (ret != RT_EOK)
            {
                rt_kprintf("fail to get gain\n");
                return -RT_ERROR;
            }
            db_config.dB = info.mindB + volume * (info.maxdB - info.mindB) / 100;
            db_config.dB = db_config.dB - db_config.dB % info.step;
            rt_kprintf("db_config.dB = %d\n", db_config.dB);
            ret = rt_device_control(p->audio_dev, RK_AUDIO_CTL_SET_GAIN, &db_config);
            if (ret != RT_EOK)
            {
                rt_kprintf("fail to set gain\n");
                return -RT_ERROR;
            }
        }

        p->cur_mute = p->set_mute;
    }

    return ret;
}

static rt_err_t _audio_playback_card_set_volume(struct audio_play *p)
{
    rt_err_t ret = RT_EOK;
    rt_int16_t v_db, v_max_db, v_min_db;
    rt_int32_t volume;
    struct AUDIO_GAIN_INFO info;
    struct AUDIO_DB_CONFIG db_config;

    if (p->audio_dev && p->cur_volume != p->set_volume)
    {
        ret = rt_device_control(p->audio_dev, RK_AUDIO_CTL_GET_GAIN_INFO, &info);
        if (ret != RT_EOK)
        {
            rt_kprintf("fail to get gain info\n");
            return -RT_ERROR;
        }

        v_db = VOLUME_USB_TO_DB(p->set_volume);
        v_max_db = VOLUME_USB_TO_DB(VOLUME_MAX_USB);
        v_min_db = VOLUME_USB_TO_DB(VOLUME_MIN_USB);
        volume = VOLUME_DB_TO_PERCENT(v_db, v_max_db, v_min_db);

        ret = rt_device_control(p->audio_dev, RK_AUDIO_CTL_GET_GAIN, &db_config);
        if (ret != RT_EOK)
        {
            rt_kprintf("fail to get gain\n");
            return -RT_ERROR;
        }
        db_config.dB = info.mindB + volume * (info.maxdB - info.mindB) / 100;
        db_config.dB = db_config.dB - db_config.dB % info.step;
        rt_kprintf("db_config.dB = %d\n", db_config.dB);
        ret = rt_device_control(p->audio_dev, RK_AUDIO_CTL_SET_GAIN, &db_config);
        if (ret != RT_EOK)
        {
            rt_kprintf("fail to set gain\n");
            return -RT_ERROR;
        }

        p->cur_volume = p->set_volume;
    }

    return ret;
}

static void _audio_start_playback(ufunction_t func)
{
    RT_ASSERT(func != RT_NULL);
    RT_ASSERT(func->device != RT_NULL);

    RT_DEBUG_LOG(RT_DEBUG_USB, ("%s\n", __func__));

    g_uac1->ep_out->request.buffer = g_uac1->ep_out->buffer;
    g_uac1->ep_out->request.size = EP_MAXPACKET(g_uac1->ep_out);
    g_uac1->ep_out->request.req_type = UIO_REQUEST_READ_BEST;

    rt_usbd_io_request(func->device, g_uac1->ep_out, &g_uac1->ep_out->request);
}

static void _audio_stop_playback(ufunction_t func)
{
    rt_uint32_t msg;

    /* re-init playback mailbox before send MSG_STOP */
    rt_mb_control(&g_uac1->p.mb, RT_IPC_CMD_RESET, RT_NULL);

    msg = MSG_STOP;
    if (rt_mb_send(&g_uac1->p.mb, msg) != RT_EOK)
    {
        rt_kprintf("%s: mb send fail\n", __func__);
        return;
    }

    dcd_ep_disable(func->device->dcd, g_uac1->ep_out);
}

static rt_err_t _audio_capture_card_enable(struct audio_capture *c)
{
    struct AUDIO_PARAMS param;
    rt_err_t ret;

    RT_DEBUG_LOG(RT_DEBUG_USB, ("%s\n", __func__));

    if (!c->audio_dev)
    {
        c->audio_dev = rt_device_find(RT_USB_AUDIO_C_NAME);

        if (!c->audio_dev)
        {
            rt_kprintf("can't find audio device: %s\n", RT_USB_AUDIO_C_NAME);
            return -RT_ERROR;
        }
    }

    ret = rt_device_open(c->audio_dev, RT_DEVICE_OFLAG_RDONLY);
    if (ret != RT_EOK)
    {
        rt_kprintf("fail to do open audio dev: %s\n", RT_USB_AUDIO_C_NAME);
        return -RT_ERROR;
    }

    param.channels = C_NR_CHANNEL;
    param.sampleRate = c->sample_rate;
    param.sampleBits = C_AUDIO_SAMPLE_BITS;

    ret = rt_device_control(c->audio_dev, RK_AUDIO_CTL_PCM_PREPARE, &c->abuf);
    if (ret != RT_EOK)
    {
        rt_kprintf("fail to do RK_AUDIO_CTL_PCM_PREPARE\n");
        return -RT_ERROR;
    }

    ret = rt_device_control(c->audio_dev, RK_AUDIO_CTL_HW_PARAMS, &param);
    if (ret != RT_EOK)
    {
        rt_kprintf("fail to do RK_AUDIO_CTL_HW_PARAMS\n");
        return -RT_ERROR;
    }

    c->send_size = 0;

    return RT_EOK;
}

static rt_err_t _audio_capture_card_disable(struct audio_capture *c)
{
    rt_err_t ret = RT_EOK;
    struct usb_audio_buf *cbuf;
    rt_base_t level;

    RT_DEBUG_LOG(RT_DEBUG_USB, ("%s\n", __func__));

    /* Clear ready list and insert to free list again */
    level = rt_hw_interrupt_disable();
    while (!rt_list_isempty(&c->ready_list))
    {
        cbuf = rt_list_first_entry(&c->ready_list, struct usb_audio_buf, list);
        rt_list_remove(&cbuf->list);
        rt_list_insert_before(&c->free_list, &cbuf->list);
    }
    rt_hw_interrupt_enable(level);

    if (c->audio_dev != RT_NULL)
    {
        ret = rt_device_control(c->audio_dev, RK_AUDIO_CTL_STOP, NULL);
        if (ret != RT_EOK)
        {
            rt_kprintf("fail to do RK_AUDIO_CTL_STOP\n");
            return -RT_ERROR;
        }

        ret = rt_device_control(c->audio_dev, RK_AUDIO_CTL_PCM_RELEASE, NULL);
        if (ret != RT_EOK)
        {
            rt_kprintf("fail to do RK_AUDIO_CTL_PCM_RELEASE\n");
            return -RT_ERROR;
        }

        rt_device_close(c->audio_dev);
        c->audio_dev = RT_NULL;
    }

    return ret;
}

static rt_err_t _audio_capture_card_set_mute(struct audio_capture *c)
{
    /* TODO */
    return RT_EOK;
}

static rt_err_t _audio_capture_card_set_volume(struct audio_capture *c)
{
    /* TODO */
    return RT_EOK;
}

static void _audio_start_capture(ufunction_t func)
{
    uep_t ep_in = g_uac1->ep_in;
    rt_uint32_t factor, interval;
    rt_uint8_t framesize;

    rt_memset(ep_in->buffer, 0, IN_EP_MAX_PACKET_SIZE);
    factor = (func->device->dcd->device_is_hs) ? 8000 : 1000;
    interval = factor / (1 << (_as_in_intf_alt1_desc.endpoint_desc.bInterval - 1));
    framesize = C_NR_CHANNEL * (C_AUDIO_SAMPLE_BITS >> 3);
    ep_in->request.buffer = ep_in->buffer;
    ep_in->request.size = g_uac1->c.sample_rate * framesize / interval;
    ep_in->request.req_type = UIO_REQUEST_WRITE;
    rt_usbd_io_request(func->device, ep_in, &ep_in->request);
}

static void _audio_stop_capture(ufunction_t func)
{
    rt_uint32_t msg;

    /* re-init capture mailbox before send MSG_STOP */
    rt_mb_control(&g_uac1->c.mb, RT_IPC_CMD_RESET, RT_NULL);

    msg = MSG_STOP;
    if (rt_mb_send(&g_uac1->c.mb, msg) != RT_EOK)
    {
        rt_kprintf("%s: mb send fail\n", __func__);
        return;
    }

    dcd_ep_disable(func->device->dcd, g_uac1->ep_in);
}

static rt_err_t _function_enable(ufunction_t func)
{
    return RT_EOK;
}

static rt_err_t _function_disable(ufunction_t func)
{
    return RT_EOK;
}

static rt_err_t _function_setup(ufunction_t func, ureq_t setup)
{
    switch (setup->request_type)
    {
    case USB_DIR_OUT | USB_REQ_TYPE_CLASS | USB_REQ_TYPE_ENDPOINT:
        _audio_set_endpoint_req(func, setup);
        break;
    case USB_DIR_IN | USB_REQ_TYPE_CLASS | USB_REQ_TYPE_ENDPOINT:
        _audio_get_endpoint_req(func, setup);
        break;
    default:
        break;
    }

    return RT_EOK;
}

static rt_err_t _function_set_alt(ufunction_t func, rt_uint8_t intf, rt_uint8_t alt)
{
    /* No i/f has more than 2 alt settings */
    if (alt > 1)
    {
        rt_kprintf("%s: error alt %d more than 2!\n", __func__, alt);
        return  -RT_EINVAL;
    }

    if (intf == g_uac1->ac_intf)
    {
        /* Control I/f has only 1 AltSetting - 0 */
        if (alt)
        {
            rt_kprintf("%s: error alt %d more than 0!\n", __func__, alt);
            return  -RT_EINVAL;
        }
    }

    RT_DEBUG_LOG(RT_DEBUG_USB, ("set intf %u, alt %u\n", intf, alt));

    if (intf == g_uac1->as_out_intf)
    {
        if (alt == g_uac1->out_intf_alt)
        {
            RT_DEBUG_LOG(RT_DEBUG_USB, ("set same out intf alt %d\n", alt));
            return RT_EOK;
        }

        g_uac1->out_intf_alt = alt;

        if (alt)
            _audio_start_playback(func);
        else
            _audio_stop_playback(func);
    }
    else if (intf == g_uac1->as_in_intf)
    {
        if (alt == g_uac1->in_intf_alt)
        {
            RT_DEBUG_LOG(RT_DEBUG_USB, ("set same in intf alt %d\n", alt));
            return RT_EOK;
        }

        g_uac1->in_intf_alt = alt;

        if (alt)
            _audio_start_capture(func);
        else
            _audio_stop_capture(func);
    }
    else
    {
        rt_kprintf("%s: error intf %d\n", __func__, intf);
        return  -RT_EINVAL;
    }

    return RT_EOK;
}

static struct ufunction_ops ops =
{
    _function_enable,
    _function_disable,
    RT_NULL,
    _function_setup,
    _function_set_alt,
};

static rt_err_t _ep0_set_playback_mute_handler(udevice_t device, rt_size_t size)
{
    g_uac1->p.set_mute = g_uac1->ep0_buffer[0];

    dcd_ep0_send_status(device->dcd);

    return RT_EOK;
}

static rt_err_t _ep0_set_playback_volume_handler(udevice_t device, rt_size_t size)
{
    switch (g_uac1->req)
    {
    case UAC_SET_CUR:
        g_uac1->p.set_volume = g_uac1->ep0_buffer[0] +
                               ((rt_uint16_t)g_uac1->ep0_buffer[1] << 8);
        break;
    case UAC_SET_RES:
        g_uac1->p.res_volume = g_uac1->ep0_buffer[0] +
                               ((rt_uint16_t)g_uac1->ep0_buffer[1] << 8);
        break;
    default:
        rt_kprintf("%s: no response to req %d\n", __func__, g_uac1->req);
        rt_usbd_ep0_set_stall(device);
        return -RT_ERROR;
    }

    dcd_ep0_send_status(device->dcd);

    return RT_EOK;
}

static rt_err_t _ep0_set_capture_mute_handler(udevice_t device, rt_size_t size)
{
    g_uac1->c.set_mute = g_uac1->ep0_buffer[0];

    dcd_ep0_send_status(device->dcd);

    return RT_EOK;
}

static rt_err_t _ep0_set_capture_volume_handler(udevice_t device, rt_size_t size)
{
    switch (g_uac1->req)
    {
    case UAC_SET_CUR:
        g_uac1->c.set_volume = g_uac1->ep0_buffer[0] +
                               ((rt_uint16_t)g_uac1->ep0_buffer[1] << 8);
        break;
    case UAC_SET_RES:
        g_uac1->c.res_volume = g_uac1->ep0_buffer[0] +
                               ((rt_uint16_t)g_uac1->ep0_buffer[1] << 8);
        break;
    default:
        rt_kprintf("%s: no response to req %d\n", __func__, g_uac1->req);
        rt_usbd_ep0_set_stall(device);
        return -RT_ERROR;
    }

    dcd_ep0_send_status(device->dcd);

    return RT_EOK;
}

static rt_err_t _interface_handler(ufunction_t func, ureq_t setup)
{
    rt_uint8_t req, cs, entity_id, ch, len;

    RT_ASSERT(func != RT_NULL);
    RT_ASSERT(func->device != RT_NULL);
    RT_ASSERT(setup != RT_NULL);

    req = setup->bRequest;
    cs = setup->wValue >> 8;
    ch = setup->wValue & 0xFF;
    entity_id = setup->wIndex >> 8;
    len = setup->wLength;
    g_uac1->req = req;

    switch (entity_id)
    {
    case USB_OUT_IT_FU_ID:
        switch (cs)
        {
        case UAC_FU_MUTE:
            switch (req)
            {
            case UAC_SET_CUR:
                if (ch == 0)
                {
                    rt_usbd_ep0_read(func->device, g_uac1->ep0_buffer, len,
                                     _ep0_set_playback_mute_handler);
                }
                else
                {
                    rt_kprintf("playback mute: UAC_SET_CUR ch %d fail!\n", ch);
                    rt_usbd_ep0_set_stall(func->device);
                }
                break;
            case UAC_GET_CUR:
                if (ch == 0)
                {
                    g_uac1->ep0_buffer[0] = g_uac1->p.cur_mute;
                    rt_usbd_ep0_write(func->device, g_uac1->ep0_buffer, len);
                }
                else
                {
                    rt_kprintf("playback mute: UAC_GET_CUR ch %d fail!\n", ch);
                    rt_usbd_ep0_set_stall(func->device);
                }
                break;
            default:
                rt_usbd_ep0_set_stall(func->device);
                break;
            }
            break;
        case UAC_FU_VOLUME:
            switch (req)
            {
            case UAC_SET_CUR:
                if (ch == 0)
                {
                    rt_usbd_ep0_read(func->device, g_uac1->ep0_buffer, len,
                                     _ep0_set_playback_volume_handler);
                }
                else
                {
                    rt_kprintf("playback volume: UAC_SET_CUR ch %d fail!\n", ch);
                    rt_usbd_ep0_set_stall(func->device);
                }
                break;
            case UAC_SET_RES:
                if (ch == 0)
                {
                    rt_usbd_ep0_read(func->device, g_uac1->ep0_buffer, len,
                                     _ep0_set_playback_volume_handler);
                }
                else
                {
                    rt_kprintf("playback volume: UAC_SET_RES ch %d fail!\n", ch);
                    rt_usbd_ep0_set_stall(func->device);
                }
                break;
            case UAC_GET_CUR:
                if (ch == 0)
                {
                    g_uac1->ep0_buffer[0] = (rt_uint8_t)g_uac1->p.cur_volume;
                    g_uac1->ep0_buffer[1] = (rt_uint8_t)(g_uac1->p.cur_volume >> 8);
                    rt_usbd_ep0_write(func->device, g_uac1->ep0_buffer, len);
                }
                else
                {
                    rt_kprintf("playback volume: UAC_GET_CUR ch %d fail!\n", ch);
                    rt_usbd_ep0_set_stall(func->device);
                }
                break;
            case UAC_GET_MIN:
                if (ch == 0)
                {
                    g_uac1->ep0_buffer[0] = (rt_uint8_t)VOLUME_MIN_USB;
                    g_uac1->ep0_buffer[1] = (rt_uint8_t)(VOLUME_MIN_USB >> 8);
                    rt_usbd_ep0_write(func->device, g_uac1->ep0_buffer, len);
                }
                else
                {
                    rt_kprintf("playback volume: UAC_GET_MIN ch %d fail!\n", ch);
                    rt_usbd_ep0_set_stall(func->device);
                }
                break;
            case UAC_GET_MAX:
                if (ch == 0)
                {
                    g_uac1->ep0_buffer[0] = (rt_uint8_t)VOLUME_MAX_USB;
                    g_uac1->ep0_buffer[1] = (rt_uint8_t)(VOLUME_MAX_USB >> 8);
                    rt_usbd_ep0_write(func->device, g_uac1->ep0_buffer, len);
                }
                else
                {
                    rt_kprintf("playback volume: UAC_GET_MAX ch %d fail!\n", ch);
                    rt_usbd_ep0_set_stall(func->device);
                }
                break;
            case UAC_GET_RES:
                if (ch == 0)
                {
                    g_uac1->ep0_buffer[0] = (rt_uint8_t)VOLUME_RES;
                    g_uac1->ep0_buffer[1] = (rt_uint8_t)(VOLUME_RES >> 8);
                    rt_usbd_ep0_write(func->device, g_uac1->ep0_buffer, len);
                }
                else
                {
                    rt_kprintf("playback volume: UAC_GET_RES ch %d fail!\n", ch);
                    rt_usbd_ep0_set_stall(func->device);
                }
                break;
            default:
                rt_usbd_ep0_set_stall(func->device);
                break;
            }
            break;
        default:
            rt_usbd_ep0_set_stall(func->device);
            break;
        }
        break;
    case IO_IN_IT_FU_ID:
        switch (cs)
        {
        case UAC_FU_MUTE:
            switch (req)
            {
            case UAC_SET_CUR:
                if (ch == 0)
                {
                    rt_usbd_ep0_read(func->device, g_uac1->ep0_buffer, len,
                                     _ep0_set_capture_mute_handler);
                }
                else
                {
                    rt_kprintf("capture mute: UAC_SET_CUR ch %d fail!\n", ch);
                    rt_usbd_ep0_set_stall(func->device);
                }
                break;
            case UAC_GET_CUR:
                if (ch == 0)
                {
                    g_uac1->ep0_buffer[0] = g_uac1->c.cur_mute;
                    rt_usbd_ep0_write(func->device, g_uac1->ep0_buffer, len);
                }
                else
                {
                    rt_kprintf("capture mute: UAC_GET_CUR ch %d fail!\n", ch);
                    rt_usbd_ep0_set_stall(func->device);
                }
                break;
            default:
                rt_usbd_ep0_set_stall(func->device);
                break;
            }
            break;
        case UAC_FU_VOLUME:
            switch (req)
            {
            case UAC_SET_CUR:
                if (ch == 0)
                {
                    rt_usbd_ep0_read(func->device, g_uac1->ep0_buffer, len,
                                     _ep0_set_capture_volume_handler);
                }
                else
                {
                    rt_kprintf("capture volume: UAC_SET_CUR ch %d fail!\n", ch);
                    rt_usbd_ep0_set_stall(func->device);
                }
                break;
            case UAC_SET_RES:
                if (ch == 0)
                {
                    rt_usbd_ep0_read(func->device, g_uac1->ep0_buffer, len,
                                     _ep0_set_capture_volume_handler);
                }
                else
                {
                    rt_kprintf("capture volume: UAC_SET_RES ch %d fail!\n", ch);
                    rt_usbd_ep0_set_stall(func->device);
                }
                break;
            case UAC_GET_CUR:
                if (ch == 0)
                {
                    g_uac1->ep0_buffer[0] = (rt_uint8_t)g_uac1->c.cur_volume;
                    g_uac1->ep0_buffer[1] = (rt_uint8_t)(g_uac1->c.cur_volume >> 8);
                    rt_usbd_ep0_write(func->device, g_uac1->ep0_buffer, len);
                }
                else
                {
                    rt_kprintf("capture volume: UAC_GET_CUR ch %d fail!\n", ch);
                    rt_usbd_ep0_set_stall(func->device);
                }
                break;
            case UAC_GET_MIN:
                if (ch == 0)
                {
                    g_uac1->ep0_buffer[0] = (rt_uint8_t)VOLUME_MIN_USB;
                    g_uac1->ep0_buffer[1] = (rt_uint8_t)(VOLUME_MIN_USB >> 8);
                    rt_usbd_ep0_write(func->device, g_uac1->ep0_buffer, len);
                }
                else
                {
                    rt_kprintf("capture volume: UAC_GET_MIN ch %d fail!\n", ch);
                    rt_usbd_ep0_set_stall(func->device);
                }
                break;
            case UAC_GET_MAX:
                if (ch == 0)
                {
                    g_uac1->ep0_buffer[0] = (rt_uint8_t)VOLUME_MAX_USB;
                    g_uac1->ep0_buffer[1] = (rt_uint8_t)(VOLUME_MAX_USB >> 8);
                    rt_usbd_ep0_write(func->device, g_uac1->ep0_buffer, len);
                }
                else
                {
                    rt_kprintf("capture volume: UAC_GET_MAX ch %d fail!\n", ch);
                    rt_usbd_ep0_set_stall(func->device);
                }
                break;
            case UAC_GET_RES:
                if (ch == 0)
                {
                    g_uac1->ep0_buffer[0] = (rt_uint8_t)VOLUME_RES;
                    g_uac1->ep0_buffer[1] = (rt_uint8_t)(VOLUME_RES >> 8);
                    rt_usbd_ep0_write(func->device, g_uac1->ep0_buffer, len);
                }
                else
                {
                    rt_kprintf("capture volume: UAC_GET_RES ch %d fail!\n", ch);
                    rt_usbd_ep0_set_stall(func->device);
                }
                break;
            default:
                rt_usbd_ep0_set_stall(func->device);
                break;
            }
            break;
        default:
            rt_usbd_ep0_set_stall(func->device);
            break;
        }
        break;
    default:
        rt_usbd_ep0_set_stall(func->device);
        break;
    }

    return RT_EOK;
}

static rt_err_t _ep0_set_sample_rate_handler(udevice_t device, rt_size_t size)
{
    rt_uint32_t msg;
    rt_err_t ret = RT_EOK;

    if (g_uac1->p.set_sample_rate)
    {
        g_uac1->p.set_sample_rate = false;
        g_uac1->p.sample_rate = g_uac1->ep0_buffer[0] +
                                ((rt_uint32_t)g_uac1->ep0_buffer[1] << 8) +
                                ((rt_uint32_t)g_uac1->ep0_buffer[2] << 16);
        RT_DEBUG_LOG(RT_DEBUG_USB,
                     ("set playback sample_rate: %u\n", g_uac1->p.sample_rate));

        if (g_uac1->out_intf_alt)
        {
            msg = MSG_START;
            ret = rt_mb_send(&g_uac1->p.mb, msg);
            if (ret != RT_EOK)
            {
                rt_kprintf("%s: mb send fail\n", __func__);
                return ret;
            }
        }
    }

    if (g_uac1->c.set_sample_rate)
    {
        g_uac1->c.set_sample_rate = false;
        g_uac1->c.sample_rate = g_uac1->ep0_buffer[0] +
                                ((rt_uint32_t)g_uac1->ep0_buffer[1] << 8) +
                                ((rt_uint32_t)g_uac1->ep0_buffer[2] << 16);
        RT_DEBUG_LOG(RT_DEBUG_USB,
                     ("set capture sample_rate: %u\n", g_uac1->c.sample_rate));

        if (g_uac1->in_intf_alt)
        {
            msg = MSG_START;
            ret = rt_mb_send(&g_uac1->c.mb, msg);
            if (ret != RT_EOK)
            {
                rt_kprintf("%s: mb send fail\n", __func__);
                return ret;
            }
        }
    }

    dcd_ep0_send_status(device->dcd);

    return ret;
}

static void _ep_out_data_handler(const void *buffer, const rt_size_t size, struct audio_play *p)
{
    struct usb_audio_buf *pbuf;
    rt_size_t remain_size = 0;
    char *dst = NULL;
    rt_uint32_t msg;
    rt_err_t ret;

    /* list don't need protect here since this callback is in ISR */

    if (rt_list_isempty(&p->free_list))
    {
        rt_kprintf("playback free_list is empty\n");
        /* TODO */
        return;
    }

    pbuf = rt_list_first_entry(&p->free_list, struct usb_audio_buf, list);
    dst = pbuf->buffer;

    if (p->total_size - p->recv_size >= size)
    {
        rt_memcpy(dst + p->recv_size, buffer, size);
        p->recv_size += size;
    }
    else if (p->total_size - p->recv_size > 0)
    {
        rt_memcpy(dst + p->recv_size, buffer, p->total_size - p->recv_size);
        remain_size = size - (p->total_size - p->recv_size);
        p->recv_size = p->total_size;
    }

    if (p->total_size == p->recv_size)
    {
        rt_list_remove(&pbuf->list);
        rt_list_insert_before(&p->ready_list, &pbuf->list);

        msg = MSG_DATA_NOTIFY;
        ret = rt_mb_send(&p->mb, msg);
        if (ret != RT_EOK)
        {
            rt_kprintf("%s: mb send fail\n", __func__);
            /* TODO */
        }

        p->recv_size = 0;
    }

    if (remain_size)
    {
        if (rt_list_isempty(&p->free_list))
        {
            rt_kprintf("playback free_list is empty for remain size\n");
            return;
        }

        pbuf = rt_list_first_entry(&p->free_list, struct usb_audio_buf, list);
        dst = pbuf->buffer;

        if (p->total_size - p->recv_size >= remain_size)
        {
            rt_memcpy(dst, (char *)buffer + size - remain_size, remain_size);
            p->recv_size = remain_size;
        }
    }
}

static rt_err_t _ep_out_handler(ufunction_t func, rt_size_t size)
{
    RT_ASSERT(func != RT_NULL);
    RT_ASSERT(func->device != RT_NULL);
    struct audio_play *p = &g_uac1->p;

#ifdef UAC_DBG
    /* This is used to check if USB SOF is missed */
    static rt_int32_t count = 0;
    static uint32_t t1 = 0, t0 = 0;

    t1 = HAL_GetTick();
    if (++count == 1000)
    {
        rt_kprintf("ep out 1000 packets take %u ms\n", t1 - t0);
        t0 = HAL_GetTick();
        count = 0;
    }
#endif

    if (!g_uac1->out_intf_alt)
    {
        rt_kprintf("%s: stop usb io request\n", __func__);
        return RT_EOK;
    }

    if (size != 0 && p->enable)
        _ep_out_data_handler(g_uac1->ep_out->buffer, size, p);

    g_uac1->ep_out->request.buffer = g_uac1->ep_out->buffer;
    g_uac1->ep_out->request.size = EP_MAXPACKET(g_uac1->ep_out);
    g_uac1->ep_out->request.req_type = UIO_REQUEST_READ_BEST;
    rt_usbd_io_request(func->device, g_uac1->ep_out, &g_uac1->ep_out->request);

    return RT_EOK;
}

static void _ep_in_data_handler(void *buffer, const rt_size_t size, struct audio_capture *c)
{
    struct usb_audio_buf *cbuf;
    rt_uint32_t msg;
    rt_err_t ret;

    /* list don't need protect here since this callback is in ISR */

    if (rt_list_len(&c->ready_list) >= 2)
    {
        cbuf = rt_list_first_entry(&c->ready_list, struct usb_audio_buf, list);
        if (C_TOTAL_SIZE - c->send_size >= size)
        {
            rt_memcpy(buffer, cbuf->buffer + c->send_size, size);
            c->send_size += size;
            if (c->send_size == C_TOTAL_SIZE)
            {
                rt_list_remove(&cbuf->list);
                rt_list_insert_before(&c->free_list, &cbuf->list);
                c->send_size = 0;
                msg = MSG_DATA_NOTIFY;
                ret = rt_mb_send(&c->mb, msg);
                if (ret != RT_EOK)
                {
                    rt_kprintf("%s: mb send fail\n", __func__);
                    /* TODO */
                }
            }
        }
        else
        {
            rt_size_t temp = C_TOTAL_SIZE - c->send_size;
            rt_memcpy(buffer, cbuf->buffer + c->send_size, temp);
            rt_list_remove(&cbuf->list);
            rt_list_insert_before(&c->free_list, &cbuf->list);
            c->send_size = 0;
            msg = MSG_DATA_NOTIFY;
            ret = rt_mb_send(&c->mb, msg);
            if (ret != RT_EOK)
            {
                rt_kprintf("%s: mb send fail\n", __func__);
                /* TODO */
            }

            cbuf = rt_list_first_entry(&c->ready_list, struct usb_audio_buf, list);
            rt_memcpy((char *)buffer + temp, cbuf->buffer, size - temp);
            c->send_size = size - temp;
        }
    }
    else
    {
        rt_memset(buffer, 0, size);
    }
}

static rt_err_t _ep_in_handler(ufunction_t func, rt_size_t size)
{
    struct audio_capture *c = &g_uac1->c;
    uep_t ep_in = g_uac1->ep_in;
    rt_uint32_t factor, interval;
    rt_uint8_t framesize;

#ifdef UAC_DBG
    static rt_int32_t count = 0;
    static uint32_t t1 = 0, t0 = 0;

    t1 = HAL_GetTick();
    if (++count == 1000)
    {
        rt_kprintf("ep in 1000 packets take %u ms\n", t1 - t0);
        t0 = HAL_GetTick();
        count = 0;
    }
#endif

    factor = (func->device->dcd->device_is_hs) ? 8000 : 1000;
    interval = factor / (1 << (_as_in_intf_alt1_desc.endpoint_desc.bInterval - 1));
    framesize = C_NR_CHANNEL * (C_AUDIO_SAMPLE_BITS >> 3);
    size = c->sample_rate * framesize / interval;
    if (c->sample_rate == 44100 && ++c->frame_cnt % 10 == 0)
    {
        size = c->sample_rate * framesize / (interval / 10) - 9 * size;
        c->frame_cnt = 0;
    }

    _ep_in_data_handler(ep_in->buffer, size, c);

    ep_in->request.buffer = ep_in->buffer;
    ep_in->request.size = size;
    ep_in->request.req_type = UIO_REQUEST_WRITE;
    rt_usbd_io_request(func->device, ep_in, &ep_in->request);

    return RT_EOK;
}

/**
 * This function will configure hid descriptor.
 *
 * @param  uac1 the uac1 control descriptor
 * @param cintf_nr the uac audio control interface number.
 *
 * @return RT_EOK on successful
 */
static rt_err_t _uac1_descriptor_config(uac1_control_t uac1, rt_uint8_t cintf_nr,
                                        uac1_interface_alt_t as_out_intf_alt1,
                                        uac1_interface_alt_t as_in_intf_alt1,
                                        rt_uint8_t device_is_hs)
{
#ifdef RT_USB_DEVICE_COMPOSITE
    uac1->iad_desc.bFirstInterface = cintf_nr;
#endif

    dev_desc.bcdUSB = device_is_hs ? 0x0200 : 0x0110;

    as_out_intf_alt1->endpoint_desc.bInterval = device_is_hs ? 0x04 : 0x01;
    _as_out_intf_alt1_desc.endpoint_desc.bInterval = device_is_hs ? 0x04 : 0x01;
#if defined(RKMCU_RK2106)
    as_in_intf_alt1->endpoint_desc.bInterval = 0x02;
    _as_in_intf_alt1_desc.endpoint_desc.bInterval = 0x02;
#else
    as_in_intf_alt1->endpoint_desc.bInterval = device_is_hs ? 0x04 : 0x01;
    _as_in_intf_alt1_desc.endpoint_desc.bInterval = device_is_hs ? 0x04 : 0x01;
#endif

    return RT_EOK;
}

static void playback_thread_entry(void *parameter)
{
    struct audio_play *p = (struct audio_play *)parameter;
    struct usb_audio_buf *pbuf;
    rt_int32_t timeout = RT_WAITING_FOREVER;
    rt_uint32_t msg;
    rt_uint32_t tickstart = 0U;
    rt_uint8_t ready_len;
    rt_uint8_t up = P_BUFFER_TOP_NUM;
    rt_uint8_t down = P_BUFFER_BOTTOM_NUM;

    while (1)
    {
        msg = 0;
        rt_mb_recv(&p->mb, &msg, timeout);
        if (msg == MSG_START)
        {
            _audio_playback_card_enable(p);
            timeout = RT_WAITING_FOREVER;

            up = P_BUFFER_TOP_NUM;
            down = P_BUFFER_BOTTOM_NUM;
        }
        else if (msg == MSG_STOP)
        {
            _audio_playback_card_disable(p);
            timeout = RT_WAITING_FOREVER;
        }

        if (timeout == RT_WAITING_FOREVER && p->audio_dev)
        {
            rt_base_t level = rt_hw_interrupt_disable();
            if (rt_list_len(&p->ready_list) > P_BUFFER_READY_NUM)
                timeout = RT_WAITING_NO;
            rt_hw_interrupt_enable(level);
        }

        /* If set volume and mute frequently, maybe cause noise during setting process.
         * Use 1s interval will assure the last setting value.
         */
        if (HAL_GetTick() - tickstart >= UAC_SET_VOL_MUTE_INTERVAL)
        {
            _audio_playback_card_set_volume(p);
            _audio_playback_card_set_mute(p);
            tickstart = HAL_GetTick();
        }

        if (p->audio_dev && timeout == RT_WAITING_NO)
        {
            rt_base_t level = rt_hw_interrupt_disable();
            if (rt_list_isempty(&p->ready_list))
                pbuf = RT_NULL;
            else
                pbuf = rt_list_first_entry(&p->ready_list, struct usb_audio_buf, list);
            rt_hw_interrupt_enable(level);
            if (pbuf)
            {
                if (!rt_device_write(p->audio_dev, 0, pbuf->buffer, p->period_size))
                    rt_kprintf("Error play\n");

                level = rt_hw_interrupt_disable();
                rt_list_remove(&pbuf->list);
                rt_list_insert_before(&p->free_list, &pbuf->list);
                ready_len = rt_list_len(&p->ready_list);
                rt_hw_interrupt_enable(level);
#ifdef UAC_DBG
                static rt_int32_t cnt = 0;
                if (++cnt % 1000 == 0)
                    rt_kprintf("ready_len = %d\n", ready_len);
#endif
                if (ready_len >= up)
                {
                    up++;
#ifdef RT_USB_AUDIO_PLL_COMPENSATION
                    HAL_CRU_PllCompensation(PLL_CPLL, CPLL_COMPENSATION);
                    rt_kprintf("[%u]: PLL_CPLL up %dppm\n", HAL_GetTick(), CPLL_COMPENSATION);
#endif
                    down = P_BUFFER_BOTTOM_NUM;
                }
                else if (ready_len <= down && ready_len > 0)
                {
                    down--;
#ifdef RT_USB_AUDIO_PLL_COMPENSATION
                    HAL_CRU_PllCompensation(PLL_CPLL, -CPLL_COMPENSATION);
                    rt_kprintf("[%u]: PLL_CPLL down %dppm\n", HAL_GetTick(), CPLL_COMPENSATION);
#endif
                    up = P_BUFFER_TOP_NUM;
                }
                else if (ready_len == 0 || up >= P_BUFFER_NUM - 1)
                {
                    /*
                     * If we check the ready_list is empty, need to
                     * disable the playback card to avoid xrun issue,
                     * and then enable the card again.
                     * If ready_list is up to P_BUFFER_NUM - 1, it's
                     * unable to compensate Cpll, it's better disable
                     * the playback card and then enable it.
                     */
                    _audio_playback_card_disable(p);
                    _audio_playback_card_enable(p);
                    timeout = RT_WAITING_FOREVER;

                    up = P_BUFFER_TOP_NUM;
                    down = P_BUFFER_BOTTOM_NUM;
                }
            }
            else
            {
                timeout = RT_WAITING_FOREVER;
            }
        }
    }
}

static rt_err_t _audio_playback_setup(struct uac1 *uac1)
{
    struct audio_play *p = &uac1->p;
    rt_thread_t play_thread;
    rt_size_t size;
    rt_uint8_t i;
    rt_err_t ret = RT_EOK;

    rt_memset(p, 0, sizeof(struct audio_play));
    p->cur_volume = VOLUME_NEGATIVE_INFINITY;
    rt_list_init(&p->free_list);
    rt_list_init(&p->ready_list);

    uac1->ep_out->buffer = rt_dma_malloc(OUT_EP_MAX_PACKET_SIZE);
    if (!uac1->ep_out->buffer)
    {
        rt_kprintf("%s: alloc ep out buf fail!\n", __func__);
        ret = -RT_ENOMEM;
        goto err0;
    }

    p->abuf.period_size = P_AUDIO_PERIOD_SIZE;
    p->abuf.buf_size = P_AUDIO_PERIOD_SIZE * P_AUDIO_PERIOD_COUNT;
    size = p->abuf.buf_size * P_NR_CHANNEL * (P_AUDIO_SAMPLE_BITS >> 3);
#ifdef RT_USING_CACHE
    p->abuf.buf = rt_malloc_uncache(size);
#else
    p->abuf.buf = rt_malloc(size);
#endif
    if (!p->abuf.buf)
    {
        rt_kprintf("%s: alloc audio buf fail!\n", __func__);
        ret = -RT_ENOMEM;
        goto err1;
    }

    for (i = 0; i < P_BUFFER_NUM; i++)
    {
        p->pbuf[i].buffer = rt_malloc(P_TOTAL_SIZE);
        if (!p->pbuf[i].buffer)
        {
            rt_kprintf("%s: alloc playback buf fail!\n", __func__);
            ret = -RT_ENOMEM;
            goto err2;
        }

        rt_base_t level = rt_hw_interrupt_disable();
        rt_list_insert_before(&p->free_list, &p->pbuf[i].list);
        rt_hw_interrupt_enable(level);
    }

    ret = rt_mb_init(&p->mb, "play_mb", p->mb_pool,
                     P_MB_POOL_SIZE, RT_IPC_FLAG_FIFO);
    if (ret != RT_EOK)
    {
        rt_kprintf("%s: create play mb fail!\n", __func__);
        goto err3;
    }

    play_thread = rt_thread_create("play_usb", playback_thread_entry, p, 2048, 0, 8);
    if (play_thread != RT_NULL)
    {
        rt_thread_startup(play_thread);
    }
    else
    {
        rt_kprintf("%s: create play thread fail!\n", __func__);
        ret = -RT_ERROR;
        goto err4;
    }

    return ret;

err4:
    rt_mb_delete(&p->mb);
err3:
    for (i = 0; i < P_BUFFER_NUM; i++)
    {
        if (p->pbuf[i].buffer)
            rt_free(p->pbuf[i].buffer);
    }
err2:
#ifdef RT_USING_CACHE
    rt_free_uncache(p->abuf.buf);
#else
    rt_free(p->abuf.buf);
#endif
err1:
    rt_dma_free(uac1->ep_out->buffer);
err0:
    return ret;
}

static void capture_thread_entry(void *parameter)
{
    struct audio_capture *c = (struct audio_capture *)parameter;
    struct usb_audio_buf *cbuf;
    rt_int32_t timeout = RT_WAITING_FOREVER;
    rt_uint32_t msg;
    rt_uint32_t tickstart = 0U;

    while (1)
    {
        msg = 0;
        rt_mb_recv(&c->mb, &msg, timeout);
        if (msg == MSG_START)
        {
            _audio_capture_card_enable(c);
            timeout = RT_WAITING_NO;
        }
        else if (msg == MSG_STOP)
        {
            _audio_capture_card_disable(c);
            timeout = RT_WAITING_FOREVER;
        }

        if (timeout == RT_WAITING_NO)
        {
            rt_base_t level = rt_hw_interrupt_disable();
            if (rt_list_isempty(&c->free_list))
            {
                RT_DEBUG_LOG(RT_DEBUG_USB, ("%s: clear ready list\n", __func__));
                /* Clear ready list and insert to free list again */
                while (!rt_list_isempty(&c->ready_list))
                {
                    cbuf = rt_list_first_entry(&c->ready_list, struct usb_audio_buf, list);
                    rt_list_remove(&cbuf->list);
                    rt_list_insert_before(&c->free_list, &cbuf->list);
                }
            }
            rt_hw_interrupt_enable(level);
        }

        /* If set volume and mute frequently, maybe cause noise during setting process.
         * Use 1s interval will assure the last setting value.
         */
        if (HAL_GetTick() - tickstart >= UAC_SET_VOL_MUTE_INTERVAL)
        {
            _audio_capture_card_set_volume(c);
            _audio_capture_card_set_mute(c);
            tickstart = HAL_GetTick();
        }

        if (c->audio_dev && timeout == RT_WAITING_NO)
        {
            rt_base_t level = rt_hw_interrupt_disable();
            if (rt_list_isempty(&c->free_list))
                cbuf = RT_NULL;
            else
                cbuf = rt_list_first_entry(&c->free_list, struct usb_audio_buf, list);
            rt_hw_interrupt_enable(level);
            if (cbuf)
            {
                if (!rt_device_read(c->audio_dev, 0, cbuf->buffer, C_AUDIO_PERIOD_SIZE))
                    rt_kprintf("Error capture\n");

                level = rt_hw_interrupt_disable();
                rt_list_remove(&cbuf->list);
                rt_list_insert_before(&c->ready_list, &cbuf->list);
                rt_hw_interrupt_enable(level);
            }
            else
            {
                rt_kprintf("%s: unexpected error!\n", __func__);
            }
        }
    }
}

static rt_err_t _audio_capture_setup(struct uac1 *uac1)
{
    struct audio_capture *c = &uac1->c;
    rt_thread_t cap_thread;
    rt_size_t size;
    rt_uint8_t i;
    rt_err_t ret = RT_EOK;

    rt_memset(c, 0, sizeof(struct audio_capture));
    c->cur_volume = VOLUME_NEGATIVE_INFINITY;
    rt_list_init(&c->free_list);
    rt_list_init(&c->ready_list);

    uac1->ep_in->buffer = rt_dma_malloc(IN_EP_MAX_PACKET_SIZE);
    if (!uac1->ep_in->buffer)
    {
        rt_kprintf("%s: alloc ep in buf fail!\n", __func__);
        ret = -RT_ENOMEM;
        goto err0;
    }

    c->abuf.period_size = C_AUDIO_PERIOD_SIZE;
    c->abuf.buf_size = C_AUDIO_PERIOD_SIZE * C_AUDIO_PERIOD_COUNT;
    size = c->abuf.buf_size * C_NR_CHANNEL * (C_AUDIO_SAMPLE_BITS >> 3);
#ifdef RT_USING_CACHE
    c->abuf.buf = rt_malloc_uncache(size);
#else
    c->abuf.buf = rt_malloc(size);
#endif
    if (!c->abuf.buf)
    {
        rt_kprintf("%s: alloc audio buf fail!\n", __func__);
        ret = -RT_ENOMEM;
        goto err1;
    }

    for (i = 0; i < C_BUFFER_NUM; i++)
    {
        c->cbuf[i].buffer = rt_malloc(C_TOTAL_SIZE);
        if (!c->cbuf[i].buffer)
        {
            rt_kprintf("%s: alloc capture buf fail!\n", __func__);
            ret = -RT_ENOMEM;
            goto err2;
        }

        rt_base_t level = rt_hw_interrupt_disable();
        rt_list_insert_before(&c->free_list, &c->cbuf[i].list);
        rt_hw_interrupt_enable(level);
    }

    ret = rt_mb_init(&c->mb, "capt_mb", c->mb_pool,
                     C_MB_POOL_SIZE, RT_IPC_FLAG_FIFO);
    if (ret != RT_EOK)
    {
        rt_kprintf("%s: create capture mb fail!\n", __func__);
        goto err3;
    }

    cap_thread = rt_thread_create("capt_usb", capture_thread_entry, c, 2048, 0, 8);
    if (cap_thread != RT_NULL)
    {
        rt_thread_startup(cap_thread);
    }
    else
    {
        rt_kprintf("%s: create capture thread fail!\n", __func__);
        ret = -RT_ERROR;
        goto err4;
    }

    return ret;

err4:
    rt_mb_delete(&c->mb);
err3:
    for (i = 0; i < C_BUFFER_NUM; i++)
    {
        if (c->cbuf[i].buffer)
            rt_free(c->cbuf[i].buffer);
    }
err2:
#ifdef RT_USING_CACHE
    rt_free_uncache(c->abuf.buf);
#else
    rt_free(c->abuf.buf);
#endif
err1:
    rt_dma_free(uac1->ep_in->buffer);
err0:
    return ret;
}

ufunction_t rt_usbd_function_uac1_create(udevice_t device)
{
    ufunction_t func;
    uintf_t intf_ac, intf_as_out, intf_as_in;
    ualtsetting_t intf_ac_alt0, intf_as_out_alt0, intf_as_out_alt1;
    ualtsetting_t intf_as_in_alt0, intf_as_in_alt1;
    uac1_interface_alt_t  as_out_intf_desc, as_in_intf_desc;
    rt_err_t ret;

    /* parameter check */
    RT_ASSERT(device != RT_NULL);

    /* set usb device string description */
    rt_usbd_device_set_string(device, _ustring);

    /* create an uac1 function */
    func = rt_usbd_function_new(device, &dev_desc, &ops);

    /* allocate memory for uac1 g_uac1 */
    g_uac1 = (struct uac1 *)rt_malloc(sizeof(struct uac1));
    rt_memset(g_uac1, 0, sizeof(struct uac1));
    func->user_data = (void *)g_uac1;
    g_uac1->ep0_buffer = rt_dma_malloc(64);
    g_uac1->p.sample_rate = P_DEFAULT_SAMPLE_RATE;
    g_uac1->c.sample_rate = C_DEFAULT_SAMPLE_RATE;

    /*
     * create an uac audio control interface, an uac audio streaming
     * out interface, an uac audio streaming in interface
     */
    intf_ac = rt_usbd_interface_new(device, _interface_handler);
    intf_as_out = rt_usbd_interface_new(device, _interface_handler);
    intf_as_in = rt_usbd_interface_new(device, _interface_handler);

    g_uac1->ac_intf = intf_ac->intf_num;
    g_uac1->as_out_intf = intf_as_out->intf_num;
    g_uac1->as_in_intf = intf_as_in->intf_num;

    _control_desc.header_desc.baInterfaceNr[0] = g_uac1->as_out_intf;
    _control_desc.header_desc.baInterfaceNr[1] = g_uac1->as_in_intf;

    /*
     * create an an uac audio control alternate setting,
     * two audio streaming out alternate settings,
     * two audio streaming in alternate settingss.
     */
    intf_ac_alt0 = rt_usbd_altsetting_new(sizeof(struct uac1_control));
    intf_as_out_alt0 = rt_usbd_altsetting_new(sizeof(struct uinterface_descriptor));
    intf_as_out_alt1 = rt_usbd_altsetting_new(sizeof(struct uac1_interface_alt));
    intf_as_in_alt0 = rt_usbd_altsetting_new(sizeof(struct uinterface_descriptor));
    intf_as_in_alt1 = rt_usbd_altsetting_new(sizeof(struct uac1_interface_alt));

    /* config desc in alternate setting */
    rt_usbd_altsetting_config_descriptor(intf_ac_alt0, &_control_desc,
                                         (rt_off_t) & ((uac1_control_t)0)->interface_desc);
    rt_usbd_altsetting_config_descriptor(intf_as_out_alt0, &_as_out_intf_alt0_desc, 0);
    rt_usbd_altsetting_config_descriptor(intf_as_out_alt1, &_as_out_intf_alt1_desc, 0);
    rt_usbd_altsetting_config_descriptor(intf_as_in_alt0, &_as_in_intf_alt0_desc, 0);
    rt_usbd_altsetting_config_descriptor(intf_as_in_alt1, &_as_in_intf_alt1_desc, 0);

    /* configure the uac1 interface descriptor */
    _uac1_descriptor_config(intf_ac_alt0->desc, intf_ac->intf_num,
                            intf_as_out_alt1->desc, intf_as_in_alt1->desc,
                            device->dcd->device_is_hs);

    /* create an isoc out and an isoc in endpoint */
    as_out_intf_desc = (uac1_interface_alt_t)intf_as_out_alt1->desc;
    g_uac1->ep_out = rt_usbd_endpoint_new((uep_desc_t)&as_out_intf_desc->endpoint_desc,
                                          _ep_out_handler);
    as_in_intf_desc = (uac1_interface_alt_t)intf_as_in_alt1->desc;
    g_uac1->ep_in = rt_usbd_endpoint_new((uep_desc_t)&as_in_intf_desc->endpoint_desc,
                                         _ep_in_handler);

    rt_usbd_altsetting_add_endpoint(intf_as_out_alt1, g_uac1->ep_out);
    rt_usbd_altsetting_add_endpoint(intf_as_in_alt1, g_uac1->ep_in);

    rt_usbd_interface_add_altsetting(intf_ac, intf_ac_alt0);
    rt_usbd_interface_add_altsetting(intf_as_out, intf_as_out_alt0);
    rt_usbd_interface_add_altsetting(intf_as_out, intf_as_out_alt1);
    rt_usbd_interface_add_altsetting(intf_as_in, intf_as_in_alt0);
    rt_usbd_interface_add_altsetting(intf_as_in, intf_as_in_alt1);

    _as_out_intf_alt0_desc.bInterfaceNumber = intf_as_out->intf_num;
    _as_out_intf_alt1_desc.interface_desc.bInterfaceNumber = intf_as_out->intf_num;
    _as_in_intf_alt0_desc.bInterfaceNumber = intf_as_in->intf_num;
    _as_in_intf_alt1_desc.interface_desc.bInterfaceNumber = intf_as_in->intf_num;

    rt_usbd_set_altsetting(intf_ac, 0);
    rt_usbd_set_altsetting(intf_as_out, 0);
    rt_usbd_set_altsetting(intf_as_in, 0);

    /* add the interface to the uac1 function */
    rt_usbd_function_add_interface(func, intf_ac);
    rt_usbd_function_add_interface(func, intf_as_out);
    rt_usbd_function_add_interface(func, intf_as_in);

    ret = _audio_playback_setup(g_uac1);
    if (ret != RT_EOK)
        return RT_NULL;

    ret = _audio_capture_setup(g_uac1);
    if (ret != RT_EOK)
        return RT_NULL;

    return func;
}

struct udclass uac1_class =
{
    .rt_usbd_function_create = rt_usbd_function_uac1_create
};

int rt_usbd_uac1_class_register(void)
{
    rt_usbd_class_register(&uac1_class);
    return 0;
}
INIT_PREV_EXPORT(rt_usbd_uac1_class_register);
