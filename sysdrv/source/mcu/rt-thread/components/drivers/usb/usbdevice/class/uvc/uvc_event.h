/**
  * Copyright (c) 2020 Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    uvc_event.h
  * @version V0.1
  * @brief   usb video class request header
  *
  * Change Logs:
  * Date           Author          Notes
  * 2020-07-01     Frank Wang      first implementation
  *
  ******************************************************************************
  */
#ifndef __UVC_EVENT_H__
#define __UVC_EVENT_H__

#include <rtdevice.h>
#include "drivers/usb_video.h"

#ifndef CACHE_LINE_SIZE
#define CACHE_LINE_SIZE         32
#endif

/* Four-character-code (FOURCC) */
#define v4l2_fourcc(a, b, c, d)\
    ((uint32_t)(a) | ((uint32_t)(b) << 8) | ((uint32_t)(c) << 16) | ((uint32_t)(d) << 24))

/* compressed formats */
#define V4L2_PIX_FMT_MJPEG    v4l2_fourcc('M', 'J', 'P', 'G') /* Motion-JPEG */
#define V4L2_PIX_FMT_YUYV     v4l2_fourcc('Y', 'U', 'Y', 'V') /* 16  YUV 4:2:2 */

#define MAX_UVC_REQUEST_DATA_LENGTH 4096

struct uvc_frame_info
{
    unsigned int width;
    unsigned int height;
    unsigned int intervals[8];
};

struct uvc_format_info
{
    unsigned int fcc;
    const struct uvc_frame_info *frames;
};

struct uvc_request_data
{
    int length;
    ALIGN(CACHE_LINE_SIZE) uint8_t data[60];
};

struct uvc_event_device
{
    int video_id;

    /* uvc control request specific */
    struct uvc_streaming_control probe;
    struct uvc_streaming_control commit;
    int control;
    struct uvc_request_data request_error_code;
    unsigned int brightness_val;
    uint16_t contrast_val;
    uint16_t hue_val;
    unsigned short saturation_val;
    unsigned int sharpness_val;
    unsigned int gamma_val;
    unsigned int white_balance_temperature_val;
    unsigned int gain_val;
    unsigned int hue_auto_val;
    unsigned char power_line_frequency_val;
    unsigned char ext1[4];
    unsigned int ext1_value;
    unsigned char ex_ctrl[16];//byte:1 command, 2-3 data length, 4-5 checksum
//    unsigned char ex_data[MAX_UVC_REQUEST_DATA_LENGTH];
    unsigned int ex_index;
//    unsigned short ex_checksum;
    unsigned short ex_length;
    char ex_result;
    unsigned short get_checksum;
    unsigned char ext4[60];
    unsigned int ext4_value;

    unsigned int fcc;
    unsigned int width;
    unsigned int height;

    unsigned int imgsize;

    /* v4l2 device hook */
    uint8_t cs;
    uint8_t entity_id;

    uint8_t req_out;
};

enum uvc_event_type
{
    UVC_EVENT_FIRST         = 0,
    UVC_EVENT_CONNECT       = 0,
    UVC_EVENT_DISCONNECT    = 1,
    UVC_EVENT_STREAMON      = 2,
    UVC_EVENT_STREAMOFF     = 3,
    UVC_EVENT_SETUP         = 4,
    UVC_EVENT_DATA          = 5,
    UVC_EVENT_SUSPEND       = 6,
    UVC_EVENT_RESUME        = 7,
    UVC_EVENT_EXCEPTION     = 8,
    UVC_EVENT_LAST          = 8
};
typedef enum uvc_event_type uvc_event_type;

struct uvc_event
{
    uvc_event_type type;

    union
    {
        struct urequest req;
        struct uvc_request_data content;
    };
};
typedef struct uvc_event *uvc_event_t;

rt_err_t uvc_event_signal(struct uvc_event *msg, int id);
rt_err_t uvc_event_init(struct uvc_event_device *dev);

#if defined(RT_USING_CAMERA) || defined(CONFIG_DRIVER_CAMERA)
#include "camera_inf.h"
#include "rksl_inf.h"
#else
static inline rt_err_t rk_camera_inf_open(int id)
{
    return RT_EOK;
}

static inline rt_err_t rk_camera_inf_close(int id)
{
    return RT_EOK;
}

static inline rt_err_t rk_camera_inf_set_fcc(int id, uint32_t format)
{
    return RT_EOK;
}

static inline rt_err_t
rk_camera_inf_set_width_height(int id, uint16_t width, uint16_t height)
{
    return RT_EOK;
}

static inline rt_err_t
rk_camera_inf_clear_buffer_manage_irq(uint32_t wchid, uint32_t block)
{
    return RT_EOK;
}

static inline uint8_t *
rk_camera_inf_get_enc_head_addr(int sensor_id)
{
    return NULL;
}

static inline  uint32_t
rk_camera_inf_get_enc_length(int sensor_id)
{
    return 0;
}

static inline rt_err_t
rksl_inf_ext_ctrl_cmd_func(uint32_t cmd, int id, void *data)
{
    return RT_EOK;
}

static inline rt_err_t
rksl_inf_std_ctrl_cmd_func(unsigned int cmd, void *data)
{
    return RT_EOK;
}
#endif /* RT_USING_CAMERA */

#endif /* __UVC_EVENT_H__ */
