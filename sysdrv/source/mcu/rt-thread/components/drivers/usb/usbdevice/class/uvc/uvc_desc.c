/**
  * Copyright (c) 2021 Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    uvc_desc.c
  * @version V0.1
  * @brief   usb video class 1.1 driver
  *
  * Change Logs:
  * Date           Author          Notes
  * 2021-04-06     Frank Wang      first implementation
  *
  ******************************************************************************
  */
#include <rtthread.h>
#include "drivers/usb_common.h"
#include "drivers/usb_video.h"
#include "uvc.h"

ALIGN(4)
static const char *uvc_ustring[] =
{
    "Language",
    "Rockchip",
    "RK625",
    "123456789",
    "Configuration",
    "UVC RGB",
    "Video Streaming",
    "UVC DEPTH",
    "Video Streaming",
    "UVC IR",
    "Video Streaming",
    USB_STRING_OS
};

/* Configuration descriptor */
ALIGN(4)
static struct uvc_ctrl_intf_desc ctrl_intf_desc =
{
#ifdef RT_USB_DEVICE_COMPOSITE
    /* Interface Association Descriptor */
    {
        USB_DESC_LENGTH_IAD,
        USB_DESC_TYPE_IAD,
        0,
        0x02,
        USB_CLASS_VIDEO,
        UVC_SC_VIDEO_INTERFACE_COLLECTION,
        0x00,
        0,
    },
#endif

    /* Interface Descriptor */
    {
        USB_DESC_LENGTH_INTERFACE,
        USB_DESC_TYPE_INTERFACE,
        0,
        0,
        0x01,
        USB_CLASS_VIDEO,
        UVC_SC_VIDEOCONTROL,
        0x00,
        0,
    },

    /* VideoControl Interface Descriptor */

    /* uvc_control_header */
    {
        UVC_DT_HEADER_SIZE(1),
        USB_DT_CS_INTERFACE,
        UVC_VC_HEADER,
        cpu_to_le16(0x0100),
        0, /* dynamic */
        cpu_to_le32(48000000),
        0, /* dynamic */
        {0}, /* dynamic */
    },

    /* uvc_extension_unit */
    {
        UVC_DT_EXTENSION_UNIT_SIZE(1, 1),
        USB_DT_CS_INTERFACE,
        UVC_VC_EXTENSION_UNIT,
        6,
        {
            0xa2, 0x9e, 0x76, 0x41, 0xde, 0x04, 0x47, 0xe3,
            0x8b, 0x2b, 0xf4, 0x34, 0x1a, 0xff, 0x00, 0x3b
        },
        4,
        1,
        {2},
        1,
        {0x0F},
        0,
    },

    /* uvc_camera_terminal */
    {
        UVC_DT_CAMERA_TERMINAL_SIZE(3),
        USB_DT_CS_INTERFACE,
        UVC_VC_INPUT_TERMINAL,
        1,
        cpu_to_le16(0x0201),
        0,
        0,
        cpu_to_le16(0),
        cpu_to_le16(0),
        cpu_to_le16(0),
        3,
        {2, 0, 0},
    },

    /* uvc_processing */
    {
        UVC_DT_PROCESSING_UNIT_SIZE(2),
        USB_DT_CS_INTERFACE,
        UVC_VC_PROCESSING_UNIT,
        2,
        1,
        cpu_to_le16(16 * 1024),
        2,
        {0x4f, 0x06},
        0,
    },

    /* uvc_output_terminal */
    {
        UVC_DT_OUTPUT_TERMINAL_SIZE,
        USB_DT_CS_INTERFACE,
        UVC_VC_OUTPUT_TERMINAL,
        3,
        cpu_to_le16(0x0101),
        0,
        2,
        0,
    },

    /* uvc_control_ep */
    {
        USB_DESC_LENGTH_ENDPOINT,
        USB_DESC_TYPE_ENDPOINT,
        USB_DYNAMIC | USB_DIR_IN,
        USB_EP_ATTR_INT,
        cpu_to_le16(16),
        8,
    },

    /* uvc_control_cs_ep */
    {
        UVC_DT_CONTROL_ENDPOINT_SIZE,
        USB_DT_CS_ENDPOINT,
        UVC_EP_INTERRUPT,
        cpu_to_le16(16),
    },
};

static const struct uinterface_descriptor uvc_bulk_streaming_intf =
{
    USB_DESC_LENGTH_INTERFACE,
    USB_DESC_TYPE_INTERFACE,
    1,
    0,
    1,
    USB_CLASS_VIDEO,
    UVC_SC_VIDEOSTREAMING,
    0x00,
    0,
};

static const struct uendpoint_descriptor uvc_hs_bulk_streaming_ep =
{
    USB_DESC_LENGTH_ENDPOINT,
    USB_DESC_TYPE_ENDPOINT,
    USB_DYNAMIC | USB_DIR_IN,
    USB_EP_ATTR_BULK,
    cpu_to_le16(512),
    0,
};

const struct uvc_color_matching_descriptor uvc_color_matching =
{
    UVC_DT_COLOR_MATCHING_SIZE,
    USB_DT_CS_INTERFACE,
    UVC_VS_COLORFORMAT,
    1,
    1,
    4,
};

/* format yuv support list */
const struct UVC_FRAME_UNCOMPRESSED(5) uvc_frame_yuv_640x400 =
{
    UVC_DT_FRAME_UNCOMPRESSED_SIZE(5),
    USB_DT_CS_INTERFACE,
    UVC_VS_FRAME_UNCOMPRESSED,
    2,
    0,
    cpu_to_le16(640),
    cpu_to_le16(400),
    cpu_to_le32(30720000),
    cpu_to_le32(122880000),
    cpu_to_le32(512000),
    cpu_to_le32(333333),
    5,
    {
        cpu_to_le32(333333),
        cpu_to_le32(500000),
        cpu_to_le32(666666),
        cpu_to_le32(1000000),
        cpu_to_le32(1333333),
    },
};

const struct UVC_FRAME_UNCOMPRESSED(5) uvc_frame_yuv_640x480 =
{
    UVC_DT_FRAME_UNCOMPRESSED_SIZE(5),
    USB_DT_CS_INTERFACE,
    UVC_VS_FRAME_UNCOMPRESSED,
    3,
    0,
    cpu_to_le16(640),
    cpu_to_le16(480),
    cpu_to_le32(36864000),
    cpu_to_le32(147456000),
    cpu_to_le32(614400),
    cpu_to_le32(333333),
    5,
    {
        cpu_to_le32(333333),
        cpu_to_le32(500000),
        cpu_to_le32(666666),
        cpu_to_le32(1000000),
        cpu_to_le32(1333333),
    }
};

const struct UVC_FRAME_UNCOMPRESSED(2) uvc_frame_yuv_1280x720 =
{
    UVC_DT_FRAME_UNCOMPRESSED_SIZE(2),
    USB_DT_CS_INTERFACE,
    UVC_VS_FRAME_UNCOMPRESSED,
    4,
    0,
    cpu_to_le16(1280),
    cpu_to_le16(720),
    cpu_to_le32(110592000),
    cpu_to_le32(147456000),
    cpu_to_le32(1843200),
    cpu_to_le32(1000000),
    2,
    {
        cpu_to_le32(1000000),
        cpu_to_le32(1333333),
    }
};

const struct UVC_FRAME_UNCOMPRESSED(2) uvc_frame_yuv_1280x800 =
{
    UVC_DT_FRAME_UNCOMPRESSED_SIZE(2),
    USB_DT_CS_INTERFACE,
    UVC_VS_FRAME_UNCOMPRESSED,
    5,
    0,
    cpu_to_le16(1280),
    cpu_to_le16(800),
    cpu_to_le32(122880000),
    cpu_to_le32(163840000),
    cpu_to_le32(2048000),
    cpu_to_le32(1000000),
    2,
    {
        cpu_to_le32(1000000),
        cpu_to_le32(1333333),
    }
};

const struct UVC_FRAME_UNCOMPRESSED(2) uvc_frame_yuv_1920x1080 =
{
    UVC_DT_FRAME_UNCOMPRESSED_SIZE(2),
    USB_DT_CS_INTERFACE,
    UVC_VS_FRAME_UNCOMPRESSED,
    6,
    0,
    cpu_to_le16(1920),
    cpu_to_le16(1080),
    cpu_to_le32(248832000),
    cpu_to_le32(331776000),
    cpu_to_le32(4147200),
    cpu_to_le32(1000000),
    2,
    {
        cpu_to_le32(5000000),
        cpu_to_le32(10000000),
    }
};

const struct UVC_FRAME_UNCOMPRESSED(2) uvc_frame_yuv_2592x1944 =
{
    UVC_DT_FRAME_UNCOMPRESSED_SIZE(2),
    USB_DT_CS_INTERFACE,
    UVC_VS_FRAME_UNCOMPRESSED,
    8,
    0,
    cpu_to_le16(2592),
    cpu_to_le16(1944),
    cpu_to_le32(604661760),
    cpu_to_le32(806215680),
    cpu_to_le32(4147200),
    cpu_to_le32(10077696),
    2,
    {
        cpu_to_le32(10000000),
        cpu_to_le32(20000000),
    }
};

/* format mjpeg support list */
const struct UVC_FRAME_MJPEG(5) uvc_frame_mjpg_160x120 =
{
    UVC_DT_FRAME_MJPEG_SIZE(5),
    USB_DT_CS_INTERFACE,
    UVC_VS_FRAME_MJPEG,
    8,
    0,
    cpu_to_le16(160),
    cpu_to_le16(120),
    cpu_to_le32(2304000),
    cpu_to_le32(9216000),
    cpu_to_le32(38400),
    cpu_to_le32(333333),
    5,
    {
        cpu_to_le32(333333),
        cpu_to_le32(500000),
        cpu_to_le32(666666),
        cpu_to_le32(1000000),
        cpu_to_le32(1333333),
    }
};

const struct UVC_FRAME_MJPEG(5) uvc_frame_mjpg_320x240 =
{
    UVC_DT_FRAME_MJPEG_SIZE(5),
    USB_DT_CS_INTERFACE,
    UVC_VS_FRAME_MJPEG,
    9,
    0,
    cpu_to_le16(320),
    cpu_to_le16(240),
    cpu_to_le32(9216000),
    cpu_to_le32(36864000),
    cpu_to_le32(153600),
    cpu_to_le32(333333),
    5,
    {
        cpu_to_le32(333333),
        cpu_to_le32(500000),
        cpu_to_le32(666666),
        cpu_to_le32(1000000),
        cpu_to_le32(1333333),
    }
};

const struct UVC_FRAME_MJPEG(5) uvc_frame_mjpg_352x288 =
{
    UVC_DT_FRAME_MJPEG_SIZE(5),
    USB_DT_CS_INTERFACE,
    UVC_VS_FRAME_MJPEG,
    10,
    0,
    cpu_to_le16(352),
    cpu_to_le16(288),
    cpu_to_le32(12165120),
    cpu_to_le32(48660480),
    cpu_to_le32(202752),
    cpu_to_le32(333333),
    5,
    {
        cpu_to_le32(333333),
        cpu_to_le32(500000),
        cpu_to_le32(666666),
        cpu_to_le32(1000000),
        cpu_to_le32(1333333),
    }
};

const struct UVC_FRAME_MJPEG(5) uvc_frame_mjpg_640x360 =
{
    UVC_DT_FRAME_MJPEG_SIZE(5),
    USB_DT_CS_INTERFACE,
    UVC_VS_FRAME_MJPEG,
    11,
    0,
    cpu_to_le16(640),
    cpu_to_le16(360),
    cpu_to_le32(27648000),
    cpu_to_le32(110592000),
    cpu_to_le32(460800),
    cpu_to_le32(333333),
    5,
    {
        cpu_to_le32(333333),
        cpu_to_le32(500000),
        cpu_to_le32(666666),
        cpu_to_le32(1000000),
        cpu_to_le32(1333333),
    }
};

const struct UVC_FRAME_MJPEG(5) uvc_frame_mjpg_640x480 =
{
    UVC_DT_FRAME_MJPEG_SIZE(5),
    USB_DT_CS_INTERFACE,
    UVC_VS_FRAME_MJPEG,
    3,
    0,
    cpu_to_le16(640),
    cpu_to_le16(480),
    cpu_to_le32(36864000),
    cpu_to_le32(147456000),
    cpu_to_le32(614400),
    cpu_to_le32(333333),
    5,
    {
        cpu_to_le32(333333),
        cpu_to_le32(500000),
        cpu_to_le32(666666),
        cpu_to_le32(1000000),
        cpu_to_le32(1333333),
    }
};

const struct UVC_FRAME_MJPEG(5) uvc_frame_mjpg_800x600 =
{
    UVC_DT_FRAME_MJPEG_SIZE(5),
    USB_DT_CS_INTERFACE,
    UVC_VS_FRAME_MJPEG,
    12,
    0,
    cpu_to_le16(800),
    cpu_to_le16(600),
    cpu_to_le32(57600000),
    cpu_to_le32(230400000),
    cpu_to_le32(960000),
    cpu_to_le32(333333),
    5,
    {
        cpu_to_le32(333333),
        cpu_to_le32(500000),
        cpu_to_le32(666666),
        cpu_to_le32(1000000),
        cpu_to_le32(1333333),
    }
};

const struct UVC_FRAME_MJPEG(5) uvc_frame_mjpg_1024x768 =
{
    UVC_DT_FRAME_MJPEG_SIZE(5),
    USB_DT_CS_INTERFACE,
    UVC_VS_FRAME_MJPEG,
    13,
    0,
    cpu_to_le16(1024),
    cpu_to_le16(768),
    cpu_to_le32(94371840),
    cpu_to_le32(377487360),
    cpu_to_le32(1572864),
    cpu_to_le32(333333),
    5,
    {
        cpu_to_le32(333333),
        cpu_to_le32(500000),
        cpu_to_le32(666666),
        cpu_to_le32(1000000),
        cpu_to_le32(1333333),
    }
};

const struct UVC_FRAME_MJPEG(5) uvc_frame_mjpg_1280x720 =
{
    UVC_DT_FRAME_MJPEG_SIZE(5),
    USB_DT_CS_INTERFACE,
    UVC_VS_FRAME_MJPEG,
    4,
    0,
    cpu_to_le16(1280),
    cpu_to_le16(720),
    cpu_to_le32(110592000),
    cpu_to_le32(442368000),
    cpu_to_le32(1843200),
    cpu_to_le32(333333),
    5,
    {
        cpu_to_le32(333333),
        cpu_to_le32(500000),
        cpu_to_le32(666666),
        cpu_to_le32(1000000),
        cpu_to_le32(1333333),
    }
};

const struct UVC_FRAME_MJPEG(5) uvc_frame_mjpg_1280x800 =
{
    UVC_DT_FRAME_MJPEG_SIZE(5),
    USB_DT_CS_INTERFACE,
    UVC_VS_FRAME_MJPEG,
    5,
    0,
    cpu_to_le16(1280),
    cpu_to_le16(800),
    cpu_to_le32(122880000),
    cpu_to_le32(491520000),
    cpu_to_le32(2048000),
    cpu_to_le32(333333),
    5,
    {
        cpu_to_le32(333333),
        cpu_to_le32(500000),
        cpu_to_le32(666666),
        cpu_to_le32(1000000),
        cpu_to_le32(1333333),
    }
};

const struct UVC_FRAME_MJPEG(5) uvc_frame_mjpg_1280x960 =
{
    UVC_DT_FRAME_MJPEG_SIZE(5),
    USB_DT_CS_INTERFACE,
    UVC_VS_FRAME_MJPEG,
    14,
    0,
    cpu_to_le16(1280),
    cpu_to_le16(960),
    cpu_to_le32(147456000),
    cpu_to_le32(589824000),
    cpu_to_le32(2457600),
    cpu_to_le32(333333),
    5,
    {
        cpu_to_le32(333333),
        cpu_to_le32(500000),
        cpu_to_le32(666666),
        cpu_to_le32(1000000),
        cpu_to_le32(1333333),
    }
};

const struct UVC_FRAME_MJPEG(5) uvc_frame_mjpg_1600x900 =
{
    UVC_DT_FRAME_MJPEG_SIZE(5),
    USB_DT_CS_INTERFACE,
    UVC_VS_FRAME_MJPEG,
    15,
    0,
    cpu_to_le16(1600),
    cpu_to_le16(900),
    cpu_to_le32(172800000),
    cpu_to_le32(691200000),
    cpu_to_le32(2880000),
    cpu_to_le32(333333),
    5,
    {
        cpu_to_le32(333333),
        cpu_to_le32(500000),
        cpu_to_le32(666666),
        cpu_to_le32(1000000),
        cpu_to_le32(1333333),
    }
};

const struct UVC_FRAME_MJPEG(5) uvc_frame_mjpg_1920x1080 =
{
    UVC_DT_FRAME_MJPEG_SIZE(5),
    USB_DT_CS_INTERFACE,
    UVC_VS_FRAME_MJPEG,
    6,
    0,
    cpu_to_le16(1920),
    cpu_to_le16(1080),
    cpu_to_le32(248832000),
    cpu_to_le32(995328000),
    cpu_to_le32(4147200),
    cpu_to_le32(333333),
    5,
    {
        cpu_to_le32(333333),
        cpu_to_le32(500000),
        cpu_to_le32(666666),
        cpu_to_le32(1000000),
        cpu_to_le32(1333333),
    }
};

const struct UVC_FRAME_MJPEG(5) uvc_frame_mjpg_1920x1440 =
{
    UVC_DT_FRAME_MJPEG_SIZE(5),
    USB_DT_CS_INTERFACE,
    UVC_VS_FRAME_MJPEG,
    7,
    0,
    cpu_to_le16(1920),
    cpu_to_le16(1440),
    cpu_to_le32(331776000),
    cpu_to_le32(1327104000),
    cpu_to_le32(5529600),
    cpu_to_le32(333333),
    5,
    {
        cpu_to_le32(333333),
        cpu_to_le32(500000),
        cpu_to_le32(666666),
        cpu_to_le32(1000000),
        cpu_to_le32(1333333),
    }
};

#ifdef RT_USB_DEVICE_UVC_DEPTH
static const struct UVC_INPUT_HEADER_DESCRIPTOR(1, 1) uvc_input_header_depth =
{
    UVC_DT_INPUT_HEADER_SIZE(1, 1),
    USB_DT_CS_INTERFACE,
    UVC_VS_INPUT_HEADER,
    1,
    0, /* dynamic */
    0, /* dynamic */
    0,
    3,
    0,
    0,
    0,
    1,
    {{27}},
};

static const struct uvc_format_uncompressed uvc_format_yuv_depth =
{
    UVC_DT_FORMAT_UNCOMPRESSED_SIZE,
    USB_DT_CS_INTERFACE,
    UVC_VS_FORMAT_UNCOMPRESSED,
    2,
    2,
    {
        'Y',  'U',  'Y',  '2', 0x00, 0x00, 0x10, 0x00,
        0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71
    },
    16,
    1,
    0,
    0,
    0,
    0,
};

static const struct uvc_descriptor_header *const uvc_hs_streaming_cls_depth[] =
{
    (const struct uvc_descriptor_header *) &uvc_input_header_depth,

    (const struct uvc_descriptor_header *) &uvc_format_yuv_depth,
    (const struct uvc_descriptor_header *) &uvc_frame_yuv_640x400,
    (const struct uvc_descriptor_header *) &uvc_frame_yuv_640x480,
    (const struct uvc_descriptor_header *) &uvc_color_matching,

    RT_NULL,
};
#endif

#ifdef RT_USB_DEVICE_UVC_RGB
static const struct UVC_INPUT_HEADER_DESCRIPTOR(1, 2) uvc_input_header_rgb =
{
    UVC_DT_INPUT_HEADER_SIZE(1, 2),
    USB_DT_CS_INTERFACE,
    UVC_VS_INPUT_HEADER,
    2,
    0, /* dynamic */
    0, /* dynamic */
    0,
    3,
    0,
    0,
    0,
    1,
    {{0}, {4}},
};

static const struct uvc_format_uncompressed uvc_format_yuv_rgb =
{
    UVC_DT_FORMAT_UNCOMPRESSED_SIZE,
    USB_DT_CS_INTERFACE,
    UVC_VS_FORMAT_UNCOMPRESSED,
    2,
    4,
    {
        'Y',  'U',  'Y',  '2', 0x00, 0x00, 0x10, 0x00,
        0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71
    },
    16,
    1,
    0,
    0,
    0,
    0,
};

static const struct uvc_format_mjpeg uvc_format_mjpg_rgb =
{
    UVC_DT_FORMAT_MJPEG_SIZE,
    USB_DT_CS_INTERFACE,
    UVC_VS_FORMAT_MJPEG,
    1,
    4,
    0,
    1,
    0,
    0,
    0,
    0,
};

static const struct uvc_descriptor_header *const uvc_hs_streaming_cls_rgb[] =
{
    (const struct uvc_descriptor_header *) &uvc_input_header_rgb,

    (const struct uvc_descriptor_header *) &uvc_format_yuv_rgb,
    (const struct uvc_descriptor_header *) &uvc_frame_yuv_2592x1944,
    (const struct uvc_descriptor_header *) &uvc_frame_yuv_1920x1080,
    (const struct uvc_descriptor_header *) &uvc_frame_yuv_1280x720,
    (const struct uvc_descriptor_header *) &uvc_frame_yuv_640x480,
    (const struct uvc_descriptor_header *) &uvc_color_matching,

    (const struct uvc_descriptor_header *) &uvc_format_mjpg_rgb,
    (const struct uvc_descriptor_header *) &uvc_frame_mjpg_1920x1440,
    (const struct uvc_descriptor_header *) &uvc_frame_mjpg_1920x1080,
    (const struct uvc_descriptor_header *) &uvc_frame_mjpg_1280x720,
    (const struct uvc_descriptor_header *) &uvc_frame_mjpg_640x480,
    (const struct uvc_descriptor_header *) &uvc_color_matching,

    RT_NULL,
};
#endif

#ifdef RT_USB_DEVICE_UVC_IR
static const struct UVC_INPUT_HEADER_DESCRIPTOR(1, 2) uvc_input_header_ir =
{
    UVC_DT_INPUT_HEADER_SIZE(1, 2),
    USB_DT_CS_INTERFACE,
    UVC_VS_INPUT_HEADER,
    2,
    0, /* dynamic */
    0, /* dynamic */
    0,
    3,
    0,
    0,
    0,
    1,
    {{0}, {4}},
};

static const struct uvc_format_uncompressed uvc_format_yuv_ir =
{
    UVC_DT_FORMAT_UNCOMPRESSED_SIZE,
    USB_DT_CS_INTERFACE,
    UVC_VS_FORMAT_UNCOMPRESSED,
    2,
    2,
    {
        'Y',  'U',  'Y',  '2', 0x00, 0x00, 0x10, 0x00,
        0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71
    },
    16,
    1,
    0,
    0,
    0,
    0,
};

static const struct uvc_format_mjpeg uvc_format_mjpg_ir =
{
    UVC_DT_FORMAT_MJPEG_SIZE,
    USB_DT_CS_INTERFACE,
    UVC_VS_FORMAT_MJPEG,
    1,
    2,
    0,
    1,
    0,
    0,
    0,
    0,
};

static const struct uvc_descriptor_header *const uvc_hs_streaming_cls_ir[] =
{
    (const struct uvc_descriptor_header *) &uvc_input_header_ir,

    (const struct uvc_descriptor_header *) &uvc_format_yuv_ir,
    (const struct uvc_descriptor_header *) &uvc_frame_yuv_1280x800,
    (const struct uvc_descriptor_header *) &uvc_frame_yuv_640x480,
    (const struct uvc_descriptor_header *) &uvc_color_matching,

    (const struct uvc_descriptor_header *) &uvc_format_mjpg_ir,
    (const struct uvc_descriptor_header *) &uvc_frame_mjpg_1280x800,
    (const struct uvc_descriptor_header *) &uvc_frame_mjpg_640x480,
    (const struct uvc_descriptor_header *) &uvc_color_matching,

    RT_NULL,
};
#endif

#define UVC_COPY_DESCRIPTOR(mem, desc) \
    do { \
        rt_memcpy(mem, desc, (desc)->bLength); \
        mem += (desc)->bLength; \
    } while (0);

#define UVC_COPY_DESCRIPTORS(mem, src) \
    do { \
        const struct usb_descriptor_header * const *__src; \
        for (__src = src; *__src; ++__src) { \
            rt_memcpy(mem, *__src, (*__src)->bLength); \
            mem += (*__src)->bLength; \
        } \
    } while (0)

/*
 * The UVC streaming interface descriptor layout
 * uvc_streaming_intf
 * uvc_streaming_cls
 *   uvc_input_header
 *   uvc_format_mjpeg/uvc_format_uncompressed
 *   uvc_frame_mjpeg/uvc_frame_uncompressed
 *   uvc_color_matching
 * uvc_hs_bulk_streaming_ep
 */
static struct usb_descriptor_header *
uvc_copy_descriptors(const struct uinterface_descriptor *intf,
                     const struct uvc_descriptor_header *const *cls,
                     const struct uendpoint_descriptor *ep)
{
    struct uvc_input_header_descriptor *uvc_streaming_header;
    const struct usb_descriptor_header *const *src;
    struct usb_descriptor_header *hdr;
    uint32_t streaming_size = 0;
    uint32_t bytes = 0;
    void *mem;

    RT_ASSERT(intf && cls && ep);

    bytes = intf->bLength;

    /* Iterator to calculate length of uvc_streaming_cls  */
    for (src = (const struct usb_descriptor_header **)cls; *src; ++src)
    {
        streaming_size += (*src)->bLength;
        bytes += (*src)->bLength;
    }

    bytes += ep->bLength;

    mem = rt_malloc(bytes);
    if (!mem)
        return RT_NULL;

    rt_memset(mem, 0, bytes);
    hdr = mem;

    /* Copy the descriptors. */
    UVC_COPY_DESCRIPTOR(mem, intf);

    uvc_streaming_header = mem;
    UVC_COPY_DESCRIPTORS(mem, (const struct usb_descriptor_header **)cls);
    uvc_streaming_header->wTotalLength = cpu_to_le16(streaming_size);

    UVC_COPY_DESCRIPTOR(mem, ep);

    return hdr;
}

RT_WEAK const char **uvc_desc_get_ustring(void)
{
    return (const char **)&uvc_ustring;
}

RT_WEAK void **uvc_desc_get_streaming_cls(int id)
{
    RT_ASSERT(id >= UVC_ID_DEPTH && id < UVC_ID_MAX);

#ifdef RT_USB_DEVICE_UVC_RGB
    if (id == UVC_ID_RGB)
        return (void **)&uvc_hs_streaming_cls_rgb;
#endif
#ifdef RT_USB_DEVICE_UVC_DEPTH
    if (id == UVC_ID_DEPTH)
        return (void **)&uvc_hs_streaming_cls_depth;
#endif
#ifdef RT_USB_DEVICE_UVC_IR
    if (id == UVC_ID_IR)
        return (void **)&uvc_hs_streaming_cls_ir;
#endif
}

struct uvc_ctrl_intf_desc *uvc_desc_get_ctrl_intf()
{
    return (uvc_ctrl_desc_t)&ctrl_intf_desc;
}

struct usb_descriptor_header *
uvc_desc_get_streaming_intf(int id)
{
    const struct uvc_descriptor_header *const *cls;

    cls = (const struct uvc_descriptor_header * const *)uvc_desc_get_streaming_cls(id);
    return uvc_copy_descriptors(&uvc_bulk_streaming_intf, cls,
                                &uvc_hs_bulk_streaming_ep);
}
