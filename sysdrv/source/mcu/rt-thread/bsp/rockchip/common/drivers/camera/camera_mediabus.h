/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    camera_mediabus.h
  * @version V0.0.1
  * @brief   information of camera format for rk2108
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-03-29     ISP Team      first implementation
  *
  ******************************************************************************
  */
#ifndef __CAMERA_MEDIABUS_H__
#define __CAMERA_MEDIABUS_H__
#if defined(__RT_THREAD__)
#include "adapter/adapter_type.h"
#elif defined(__RK_OS__)
#include "driver/adapter_type.h"
#endif
/* Exported macro ------------------------------------------------------------*/
/* pixelcode:copy from linux:media-bus-format.h ----------------------------------------*/
#define MEDIA_BUS_FMT_FIXED         0x0001

/* RGB - next is    0x101c */
#define MEDIA_BUS_FMT_RGB444_1X12       0x1016
#define MEDIA_BUS_FMT_RGB444_2X8_PADHI_BE   0x1001
#define MEDIA_BUS_FMT_RGB444_2X8_PADHI_LE   0x1002
#define MEDIA_BUS_FMT_RGB555_2X8_PADHI_BE   0x1003
#define MEDIA_BUS_FMT_RGB555_2X8_PADHI_LE   0x1004
#define MEDIA_BUS_FMT_RGB565_1X16       0x1017
#define MEDIA_BUS_FMT_BGR565_2X8_BE     0x1005
#define MEDIA_BUS_FMT_BGR565_2X8_LE     0x1006
#define MEDIA_BUS_FMT_RGB565_2X8_BE     0x1007
#define MEDIA_BUS_FMT_RGB565_2X8_LE     0x1008
#define MEDIA_BUS_FMT_RGB666_1X18       0x1009
#define MEDIA_BUS_FMT_RBG888_1X24       0x100e
#define MEDIA_BUS_FMT_RGB666_1X24_CPADHI    0x1015
#define MEDIA_BUS_FMT_RGB666_1X7X3_SPWG     0x1010
#define MEDIA_BUS_FMT_BGR888_1X24       0x1013
#define MEDIA_BUS_FMT_GBR888_1X24       0x1014
#define MEDIA_BUS_FMT_RGB888_1X24       0x100a
#define MEDIA_BUS_FMT_RGB888_2X12_BE        0x100b
#define MEDIA_BUS_FMT_RGB888_2X12_LE        0x100c
#define MEDIA_BUS_FMT_RGB888_1X7X4_SPWG     0x1011
#define MEDIA_BUS_FMT_RGB888_1X7X4_JEIDA    0x1012
#define MEDIA_BUS_FMT_ARGB8888_1X32     0x100d
#define MEDIA_BUS_FMT_RGB888_1X32_PADHI     0x100f
#define MEDIA_BUS_FMT_RGB101010_1X30        0x1018
#define MEDIA_BUS_FMT_RGB121212_1X36        0x1019
#define MEDIA_BUS_FMT_RGB161616_1X48        0x101a
#define MEDIA_BUS_FMT_RGB666_1X7X3_JEIDA    0x101b

/* YUV (including grey) - next is   0x2026 */
#define MEDIA_BUS_FMT_Y8_1X8            0x2001
#define MEDIA_BUS_FMT_UV8_1X8           0x2015
#define MEDIA_BUS_FMT_UYVY8_1_5X8       0x2002
#define MEDIA_BUS_FMT_VYUY8_1_5X8       0x2003
#define MEDIA_BUS_FMT_YUYV8_1_5X8       0x2004
#define MEDIA_BUS_FMT_YVYU8_1_5X8       0x2005
#define MEDIA_BUS_FMT_UYVY8_2X8         0x2006
#define MEDIA_BUS_FMT_VYUY8_2X8         0x2007
#define MEDIA_BUS_FMT_YUYV8_2X8         0x2008
#define MEDIA_BUS_FMT_YVYU8_2X8         0x2009
#define MEDIA_BUS_FMT_Y10_1X10          0x200a
#define MEDIA_BUS_FMT_UYVY10_2X10       0x2018
#define MEDIA_BUS_FMT_VYUY10_2X10       0x2019
#define MEDIA_BUS_FMT_YUYV10_2X10       0x200b
#define MEDIA_BUS_FMT_YVYU10_2X10       0x200c
#define MEDIA_BUS_FMT_Y12_1X12          0x2013
#define MEDIA_BUS_FMT_UYVY12_2X12       0x201c
#define MEDIA_BUS_FMT_VYUY12_2X12       0x201d
#define MEDIA_BUS_FMT_YUYV12_2X12       0x201e
#define MEDIA_BUS_FMT_YVYU12_2X12       0x201f
#define MEDIA_BUS_FMT_UYVY8_1X16        0x200f
#define MEDIA_BUS_FMT_VYUY8_1X16        0x2010
#define MEDIA_BUS_FMT_YUYV8_1X16        0x2011
#define MEDIA_BUS_FMT_YVYU8_1X16        0x2012
#define MEDIA_BUS_FMT_YDYUYDYV8_1X16        0x2014
#define MEDIA_BUS_FMT_UYVY10_1X20       0x201a
#define MEDIA_BUS_FMT_VYUY10_1X20       0x201b
#define MEDIA_BUS_FMT_YUYV10_1X20       0x200d
#define MEDIA_BUS_FMT_YVYU10_1X20       0x200e
#define MEDIA_BUS_FMT_VUY8_1X24         0x2024
#define MEDIA_BUS_FMT_YUV8_1X24         0x2025
#define MEDIA_BUS_FMT_UYVY12_1X24       0x2020
#define MEDIA_BUS_FMT_VYUY12_1X24       0x2021
#define MEDIA_BUS_FMT_YUYV12_1X24       0x2022
#define MEDIA_BUS_FMT_YVYU12_1X24       0x2023
#define MEDIA_BUS_FMT_YUV10_1X30        0x2016
#define MEDIA_BUS_FMT_AYUV8_1X32        0x2017

/* Bayer - next is  0x3019 */
#define MEDIA_BUS_FMT_SBGGR8_1X8        0x3001
#define MEDIA_BUS_FMT_SGBRG8_1X8        0x3013
#define MEDIA_BUS_FMT_SGRBG8_1X8        0x3002
#define MEDIA_BUS_FMT_SRGGB8_1X8        0x3014
#define MEDIA_BUS_FMT_SBGGR10_ALAW8_1X8     0x3015
#define MEDIA_BUS_FMT_SGBRG10_ALAW8_1X8     0x3016
#define MEDIA_BUS_FMT_SGRBG10_ALAW8_1X8     0x3017
#define MEDIA_BUS_FMT_SRGGB10_ALAW8_1X8     0x3018
#define MEDIA_BUS_FMT_SBGGR10_DPCM8_1X8     0x300b
#define MEDIA_BUS_FMT_SGBRG10_DPCM8_1X8     0x300c
#define MEDIA_BUS_FMT_SGRBG10_DPCM8_1X8     0x3009
#define MEDIA_BUS_FMT_SRGGB10_DPCM8_1X8     0x300d
#define MEDIA_BUS_FMT_SBGGR10_2X8_PADHI_BE  0x3003
#define MEDIA_BUS_FMT_SBGGR10_2X8_PADHI_LE  0x3004
#define MEDIA_BUS_FMT_SBGGR10_2X8_PADLO_BE  0x3005
#define MEDIA_BUS_FMT_SBGGR10_2X8_PADLO_LE  0x3006
#define MEDIA_BUS_FMT_SBGGR10_1X10      0x3007
#define MEDIA_BUS_FMT_SGBRG10_1X10      0x300e
#define MEDIA_BUS_FMT_SGRBG10_1X10      0x300a
#define MEDIA_BUS_FMT_SRGGB10_1X10      0x300f
#define MEDIA_BUS_FMT_SBGGR12_1X12      0x3008
#define MEDIA_BUS_FMT_SGBRG12_1X12      0x3010
#define MEDIA_BUS_FMT_SGRBG12_1X12      0x3011
#define MEDIA_BUS_FMT_SRGGB12_1X12      0x3012

/* JPEG compressed formats - next is    0x4002 */
#define MEDIA_BUS_FMT_JPEG_1X8          0x4001

/* Vendor specific formats - next is    0x5002 */

/* S5C73M3 sensor specific interleaved UYVY and JPEG */
#define MEDIA_BUS_FMT_S5C_UYVY_JPEG_1X8     0x5001

/* HSV - next is    0x6002 */
#define MEDIA_BUS_FMT_AHSV8888_1X32     0x6001

#define MEDIA_BUS_FLAGS_VSYNC_POLA_SHIFT        (0x0)
#define MEDIA_BUS_FLAGS_VSYNC_POLA_LOW          (0x0 << MEDIA_BUS_FLAGS_VSYNC_POLA_SHIFT)
#define MEDIA_BUS_FLAGS_VSYNC_POLA_HIGHT        (0x1 << MEDIA_BUS_FLAGS_VSYNC_POLA_SHIFT)
#define MEDIA_BUS_FLAGS_VSYNC_MASK              (0x1 << MEDIA_BUS_FLAGS_VSYNC_POLA_SHIFT)

#define MEDIA_BUS_FLAGS_HSYNC_POLA_SHIFT        (0x1)
#define MEDIA_BUS_FLAGS_HSYNC_POLA_LOW          (0x0 << MEDIA_BUS_FLAGS_HSYNC_POLA_SHIFT)
#define MEDIA_BUS_FLAGS_HSYNC_POLA_HIGHT        (0x1 << MEDIA_BUS_FLAGS_HSYNC_POLA_SHIFT)
#define MEDIA_BUS_FLAGS_HSYNC_MASK              (0x1 << MEDIA_BUS_FLAGS_HSYNC_POLA_SHIFT)

/************************************PARALLEL***********************************************************/
/* 1BIT */
#define MEDIA_BUS_FLAGS_PARALL_VSYNC_POLA_SHIFT                (0x0)
#define MEDIA_BUS_FLAGS_PARALL_VSYNC_POLA_LOW                  (0x0 << MEDIA_BUS_FLAGS_PARALL_VSYNC_POLA_SHIFT)
#define MEDIA_BUS_FLAGS_PARALL_VSYNC_POLA_HIGHT                (0x1 << MEDIA_BUS_FLAGS_PARALL_VSYNC_POLA_SHIFT)
#define MEDIA_BUS_FLAGS_PARALL_VSYNC_POLA_MASK                 (0x1 << MEDIA_BUS_FLAGS_PARALL_VSYNC_POLA_SHIFT)

/* 1BIT */
#define MEDIA_BUS_FLAGS_PARALL_HSYNC_POLA_SHIFT                (0x1)
#define MEDIA_BUS_FLAGS_PARALL_HSYNC_POLA_LOW                  (0x0 << MEDIA_BUS_FLAGS_PARALL_HSYNC_POLA_SHIFT)
#define MEDIA_BUS_FLAGS_PARALL_HSYNC_POLA_HIGHT                (0x1 << MEDIA_BUS_FLAGS_PARALL_HSYNC_POLA_SHIFT)
#define MEDIA_BUS_FLAGS_PARALL_HSYNC_POLA_MASK                 (0x1 << MEDIA_BUS_FLAGS_PARALL_HSYNC_POLA_SHIFT)

/* 1BIT */
#define MEDIA_BUS_FLAGS_PARALL_SAMPLE_EDGE_SHIFT               (0x2)
#define MEDIA_BUS_FLAGS_PARALL_SAMPLE_EDGE_RISING              (0x0 << MEDIA_BUS_FLAGS_PARALL_SAMPLE_EDGE_SHIFT)
#define MEDIA_BUS_FLAGS_PARALL_SAMPLE_EDGE_TRAILING            (0x1 << MEDIA_BUS_FLAGS_PARALL_SAMPLE_EDGE_SHIFT)
#define MEDIA_BUS_FLAGS_PARALL_SAMPLE_EDGE_MASK                (0x1 << MEDIA_BUS_FLAGS_PARALL_SAMPLE_EDGE_SHIFT)

/************************************MIPI_LVDS***********************************************************/
/* Flag function:
 * Bit[0-1]:the lane num of sensor,4 at most;
 * Bit[2-5]:the main lane for sensor
 *          Bit2 for lane0, Bit3 for lane1,
 *          Bit4 for lane2, Bit5 for lane3.
 * Bit[6]:clock type, 1 for continue, 0 for non-conti.
 * Bit[7-8]:bits mode
 * Bit[9]:lvds sync mode
 */
/* 2BITS */
#define MEDIA_BUS_FLAGS_CSI2_LVDS_LANES_SHIFT           (0x0)
#define MEDIA_BUS_FLAGS_CSI2_LVDS_LANES_1               (0x0 << MEDIA_BUS_FLAGS_CSI2_LVDS_LANES_SHIFT)
#define MEDIA_BUS_FLAGS_CSI2_LVDS_LANES_2               (0x1 << MEDIA_BUS_FLAGS_CSI2_LVDS_LANES_SHIFT)
#define MEDIA_BUS_FLAGS_CSI2_LVDS_LANES_3               (0x2 << MEDIA_BUS_FLAGS_CSI2_LVDS_LANES_SHIFT)
#define MEDIA_BUS_FLAGS_CSI2_LVDS_LANES_4               (0x3 << MEDIA_BUS_FLAGS_CSI2_LVDS_LANES_SHIFT)
#define MEDIA_BUS_FLAGS_CSI2_LVDS_LANES_MASK            (0x3 << MEDIA_BUS_FLAGS_CSI2_LVDS_LANES_SHIFT)

/* 4BITS */
#define MEDIA_BUS_FLAGS_CSI2_LVDS_MAIN_LANE_SHIFT       (0x2)
#define MEDIA_BUS_FLAGS_CSI2_LVDS_MAIN_LANE_0           (0x1 << MEDIA_BUS_FLAGS_CSI2_LVDS_MAIN_LANE_SHIFT)
#define MEDIA_BUS_FLAGS_CSI2_LVDS_MAIN_LANE_1           (0x2 << MEDIA_BUS_FLAGS_CSI2_LVDS_MAIN_LANE_SHIFT)
#define MEDIA_BUS_FLAGS_CSI2_LVDS_MAIN_LANE_2           (0x4 << MEDIA_BUS_FLAGS_CSI2_LVDS_MAIN_LANE_SHIFT)
#define MEDIA_BUS_FLAGS_CSI2_LVDS_MAIN_LANE_3           (0x8 << MEDIA_BUS_FLAGS_CSI2_LVDS_MAIN_LANE_SHIFT)
#define MEDIA_BUS_FLAGS_CSI2_LVDS_MAIN_LANE_MASK        (0xf << MEDIA_BUS_FLAGS_CSI2_LVDS_MAIN_LANE_SHIFT)

/* 1BIT */
#define MEDIA_BUS_FLAGS_CSI2_LVDS_CLOCK_MODE_SHIFT      (0x6)
#define MEDIA_BUS_FLAGS_CSI2_LVDS_CLOCK_MODE_CONTIN     (0x1 << MEDIA_BUS_FLAGS_CSI2_LVDS_CLOCK_MODE_SHIFT)
#define MEDIA_BUS_FLAGS_CSI2_LVDS_CLOCK_MODE_NONCON     (0x0 << MEDIA_BUS_FLAGS_CSI2_LVDS_CLOCK_MODE_SHIFT)
#define MEDIA_BUS_FLAGS_CSI2_LVDS_CLOCK_MODE_MASK       (0x1 << MEDIA_BUS_FLAGS_CSI2_LVDS_CLOCK_MODE_SHIFT)

/* 2BITS */
#define MEDIA_BUS_FLAGS_CSI2_LVDS_BITS_MODE_SHIFT       (0x7)
#define MEDIA_BUS_FLAGS_CSI2_LVDS_BITS_MODE_8BITS       (0x1 << MEDIA_BUS_FLAGS_CSI2_LVDS_BITS_MODE_SHIFT)
#define MEDIA_BUS_FLAGS_CSI2_LVDS_BITS_MODE_10BITS      (0x2 << MEDIA_BUS_FLAGS_CSI2_LVDS_BITS_MODE_SHIFT)
#define MEDIA_BUS_FLAGS_CSI2_LVDS_BITS_MODE_12BITS      (0x3 << MEDIA_BUS_FLAGS_CSI2_LVDS_BITS_MODE_SHIFT)
#define MEDIA_BUS_FLAGS_CSI2_LVDS_BITS_MODE_MASK        (0x3 << MEDIA_BUS_FLAGS_CSI2_LVDS_BITS_MODE_SHIFT)

/* 1BITS */
#define MEDIA_BUS_FLAGS_CSI2_LVDS_SYNC_CODE_MODE_SHIFT              (0x9)
#define MEDIA_BUS_FLAGS_CSI2_LVDS_SYNC_CODE_MODE_SAV_EAV_INTERVAL   (0x0 << MEDIA_BUS_FLAGS_CSI2_LVDS_SYNC_CODE_MODE_SHIFT)
#define MEDIA_BUS_FLAGS_CSI2_LVDS_SYNC_CODE_MODE_FS_LE_LS_FE        (0x1 << MEDIA_BUS_FLAGS_CSI2_LVDS_SYNC_CODE_MODE_SHIFT)
#define MEDIA_BUS_FLAGS_CSI2_LVDS_SYNC_CODE_MODE_MASK               (0x1 << MEDIA_BUS_FLAGS_CSI2_LVDS_SYNC_CODE_MODE_SHIFT)

/* 2BIT2 */
#define MEDIA_STD_ID_PAL_SHIFT                          (0x0)
#define MEDIA_STD_ID_PAL_MASK                           (0x1 << MEDIA_STD_ID_PAL_SHIFT)
#define MEDIA_STD_ID_NTSC_SHIFT                         (0x1)
#define MEDIA_STD_ID_NTSC_MASK                          (0x1 << MEDIA_STD_ID_NTSC_SHIFT)

/* Four-character-code:copy from linux:videodev2.h ----------------------------------------*/
/* Four-character-code (FOURCC) */
#define v4l2_fourcc(a, b, c, d)\
    ((uint32_t)(a) | ((uint32_t)(b) << 8) | ((uint32_t)(c) << 16) | ((uint32_t)(d) << 24))
#define v4l2_fourcc_be(a, b, c, d)  (v4l2_fourcc(a, b, c, d) | (1 << 31))

enum v4l2_field
{
    V4L2_FIELD_ANY           = 0, /* driver can choose from none,
                                     top, bottom, interlaced
                                     depending on whatever it thinks
                                     is approximate ... */
    V4L2_FIELD_NONE          = 1, /* this device has no fields ... */
    V4L2_FIELD_TOP           = 2, /* top field only */
    V4L2_FIELD_BOTTOM        = 3, /* bottom field only */
    V4L2_FIELD_INTERLACED    = 4, /* both fields interlaced */
    V4L2_FIELD_SEQ_TB        = 5, /* both fields sequential into one
                                     buffer, top-bottom order */
    V4L2_FIELD_SEQ_BT        = 6, /* same as above + bottom-top order */
    V4L2_FIELD_ALTERNATE     = 7, /* both fields alternating into
                                     separate buffers */
    V4L2_FIELD_INTERLACED_TB = 8, /* both fields interlaced, top field
                                     first and the top field is
                                     transmitted first */
    V4L2_FIELD_INTERLACED_BT = 9, /* both fields interlaced, top field
                                     first and the bottom field is
                                     transmitted first */
};

/*      Pixel format         FOURCC                          depth  Description  */
/* RGB formats */
#define V4L2_PIX_FMT_RGB332  v4l2_fourcc('R', 'G', 'B', '1') /*  8  RGB-3-3-2     */
#define V4L2_PIX_FMT_RGB444  v4l2_fourcc('R', '4', '4', '4') /* 16  xxxxrrrr ggggbbbb */
#define V4L2_PIX_FMT_ARGB444 v4l2_fourcc('A', 'R', '1', '2') /* 16  aaaarrrr ggggbbbb */
#define V4L2_PIX_FMT_XRGB444 v4l2_fourcc('X', 'R', '1', '2') /* 16  xxxxrrrr ggggbbbb */
#define V4L2_PIX_FMT_RGB555  v4l2_fourcc('R', 'G', 'B', 'O') /* 16  RGB-5-5-5     */
#define V4L2_PIX_FMT_ARGB555 v4l2_fourcc('A', 'R', '1', '5') /* 16  ARGB-1-5-5-5  */
#define V4L2_PIX_FMT_XRGB555 v4l2_fourcc('X', 'R', '1', '5') /* 16  XRGB-1-5-5-5  */
#define V4L2_PIX_FMT_RGB565  v4l2_fourcc('R', 'G', 'B', 'P') /* 16  RGB-5-6-5     */
#define V4L2_PIX_FMT_RGB555X v4l2_fourcc('R', 'G', 'B', 'Q') /* 16  RGB-5-5-5 BE  */
#define V4L2_PIX_FMT_ARGB555X v4l2_fourcc_be('A', 'R', '1', '5') /* 16  ARGB-5-5-5 BE */
#define V4L2_PIX_FMT_XRGB555X v4l2_fourcc_be('X', 'R', '1', '5') /* 16  XRGB-5-5-5 BE */
#define V4L2_PIX_FMT_RGB565X v4l2_fourcc('R', 'G', 'B', 'R') /* 16  RGB-5-6-5 BE  */
#define V4L2_PIX_FMT_BGR666  v4l2_fourcc('B', 'G', 'R', 'H') /* 18  BGR-6-6-6     */
#define V4L2_PIX_FMT_BGR24   v4l2_fourcc('B', 'G', 'R', '3') /* 24  BGR-8-8-8     */
#define V4L2_PIX_FMT_RGB24   v4l2_fourcc('R', 'G', 'B', '3') /* 24  RGB-8-8-8     */
#define V4L2_PIX_FMT_BGR32   v4l2_fourcc('B', 'G', 'R', '4') /* 32  BGR-8-8-8-8   */
#define V4L2_PIX_FMT_ABGR32  v4l2_fourcc('A', 'R', '2', '4') /* 32  BGRA-8-8-8-8  */
#define V4L2_PIX_FMT_XBGR32  v4l2_fourcc('X', 'R', '2', '4') /* 32  BGRX-8-8-8-8  */
#define V4L2_PIX_FMT_RGB32   v4l2_fourcc('R', 'G', 'B', '4') /* 32  RGB-8-8-8-8   */
#define V4L2_PIX_FMT_ARGB32  v4l2_fourcc('B', 'A', '2', '4') /* 32  ARGB-8-8-8-8  */
#define V4L2_PIX_FMT_XRGB32  v4l2_fourcc('B', 'X', '2', '4') /* 32  XRGB-8-8-8-8  */

/* Grey formats */
#define V4L2_PIX_FMT_GREY    v4l2_fourcc('G', 'R', 'E', 'Y') /*  8  Greyscale     */
#define V4L2_PIX_FMT_Y4      v4l2_fourcc('Y', '0', '4', ' ') /*  4  Greyscale     */
#define V4L2_PIX_FMT_Y6      v4l2_fourcc('Y', '0', '6', ' ') /*  6  Greyscale     */
#define V4L2_PIX_FMT_Y10     v4l2_fourcc('Y', '1', '0', ' ') /* 10  Greyscale     */
#define V4L2_PIX_FMT_Y12     v4l2_fourcc('Y', '1', '2', ' ') /* 12  Greyscale     */
#define V4L2_PIX_FMT_Y16     v4l2_fourcc('Y', '1', '6', ' ') /* 16  Greyscale     */
#define V4L2_PIX_FMT_Y16_BE  v4l2_fourcc_be('Y', '1', '6', ' ') /* 16  Greyscale BE  */

/* Grey bit-packed formats */
#define V4L2_PIX_FMT_Y10BPACK    v4l2_fourcc('Y', '1', '0', 'B') /* 10  Greyscale bit-packed */

/* Palette formats */
#define V4L2_PIX_FMT_PAL8    v4l2_fourcc('P', 'A', 'L', '8') /*  8  8-bit palette */

/* Chrominance formats */
#define V4L2_PIX_FMT_UV8     v4l2_fourcc('U', 'V', '8', ' ') /*  8  UV 4:4 */

/* Luminance+Chrominance formats */
#define V4L2_PIX_FMT_YVU410  v4l2_fourcc('Y', 'V', 'U', '9') /*  9  YVU 4:1:0     */
#define V4L2_PIX_FMT_YVU420  v4l2_fourcc('Y', 'V', '1', '2') /* 12  YVU 4:2:0     */
#define V4L2_PIX_FMT_YUYV    v4l2_fourcc('Y', 'U', 'Y', 'V') /* 16  YUV 4:2:2     */
#define V4L2_PIX_FMT_YYUV    v4l2_fourcc('Y', 'Y', 'U', 'V') /* 16  YUV 4:2:2     */
#define V4L2_PIX_FMT_YVYU    v4l2_fourcc('Y', 'V', 'Y', 'U') /* 16 YVU 4:2:2 */
#define V4L2_PIX_FMT_UYVY    v4l2_fourcc('U', 'Y', 'V', 'Y') /* 16  YUV 4:2:2     */
#define V4L2_PIX_FMT_VYUY    v4l2_fourcc('V', 'Y', 'U', 'Y') /* 16  YUV 4:2:2     */
#define V4L2_PIX_FMT_YUV422P v4l2_fourcc('4', '2', '2', 'P') /* 16  YVU422 planar */
#define V4L2_PIX_FMT_YUV411P v4l2_fourcc('4', '1', '1', 'P') /* 16  YVU411 planar */
#define V4L2_PIX_FMT_Y41P    v4l2_fourcc('Y', '4', '1', 'P') /* 12  YUV 4:1:1     */
#define V4L2_PIX_FMT_YUV444  v4l2_fourcc('Y', '4', '4', '4') /* 16  xxxxyyyy uuuuvvvv */
#define V4L2_PIX_FMT_YUV555  v4l2_fourcc('Y', 'U', 'V', 'O') /* 16  YUV-5-5-5     */
#define V4L2_PIX_FMT_YUV565  v4l2_fourcc('Y', 'U', 'V', 'P') /* 16  YUV-5-6-5     */
#define V4L2_PIX_FMT_YUV32   v4l2_fourcc('Y', 'U', 'V', '4') /* 32  YUV-8-8-8-8   */
#define V4L2_PIX_FMT_YUV410  v4l2_fourcc('Y', 'U', 'V', '9') /*  9  YUV 4:1:0     */
#define V4L2_PIX_FMT_YUV420  v4l2_fourcc('Y', 'U', '1', '2') /* 12  YUV 4:2:0     */
#define V4L2_PIX_FMT_HI240   v4l2_fourcc('H', 'I', '2', '4') /*  8  8-bit color   */
#define V4L2_PIX_FMT_HM12    v4l2_fourcc('H', 'M', '1', '2') /*  8  YUV 4:2:0 16x16 macroblocks */
#define V4L2_PIX_FMT_M420    v4l2_fourcc('M', '4', '2', '0') /* 12  YUV 4:2:0 2 lines y, 1 line uv interleaved */

/* two planes -- one Y, one Cr + Cb interleaved  */
#define V4L2_PIX_FMT_NV12    v4l2_fourcc('N', 'V', '1', '2') /* 12  Y/CbCr 4:2:0  */
#define V4L2_PIX_FMT_NV21    v4l2_fourcc('N', 'V', '2', '1') /* 12  Y/CrCb 4:2:0  */
#define V4L2_PIX_FMT_NV16    v4l2_fourcc('N', 'V', '1', '6') /* 16  Y/CbCr 4:2:2  */
#define V4L2_PIX_FMT_NV61    v4l2_fourcc('N', 'V', '6', '1') /* 16  Y/CrCb 4:2:2  */
#define V4L2_PIX_FMT_NV24    v4l2_fourcc('N', 'V', '2', '4') /* 24  Y/CbCr 4:4:4  */
#define V4L2_PIX_FMT_NV42    v4l2_fourcc('N', 'V', '4', '2') /* 24  Y/CrCb 4:4:4  */

/* two non contiguous planes - one Y, one Cr + Cb interleaved  */
#define V4L2_PIX_FMT_NV12M   v4l2_fourcc('N', 'M', '1', '2') /* 12  Y/CbCr 4:2:0  */
#define V4L2_PIX_FMT_NV21M   v4l2_fourcc('N', 'M', '2', '1') /* 21  Y/CrCb 4:2:0  */
#define V4L2_PIX_FMT_NV16M   v4l2_fourcc('N', 'M', '1', '6') /* 16  Y/CbCr 4:2:2  */
#define V4L2_PIX_FMT_NV61M   v4l2_fourcc('N', 'M', '6', '1') /* 16  Y/CrCb 4:2:2  */
#define V4L2_PIX_FMT_NV12MT  v4l2_fourcc('T', 'M', '1', '2') /* 12  Y/CbCr 4:2:0 64x32 macroblocks */
#define V4L2_PIX_FMT_NV12MT_16X16 v4l2_fourcc('V', 'M', '1', '2') /* 12  Y/CbCr 4:2:0 16x16 macroblocks */

/* three non contiguous planes - Y, Cb, Cr */
#define V4L2_PIX_FMT_YUV420M v4l2_fourcc('Y', 'M', '1', '2') /* 12  YUV420 planar */
#define V4L2_PIX_FMT_YVU420M v4l2_fourcc('Y', 'M', '2', '1') /* 12  YVU420 planar */
#define V4L2_PIX_FMT_YUV422M v4l2_fourcc('Y', 'M', '1', '6') /* 16  YUV422 planar */
#define V4L2_PIX_FMT_YVU422M v4l2_fourcc('Y', 'M', '6', '1') /* 16  YVU422 planar */
#define V4L2_PIX_FMT_YUV444M v4l2_fourcc('Y', 'M', '2', '4') /* 24  YUV444 planar */
#define V4L2_PIX_FMT_YVU444M v4l2_fourcc('Y', 'M', '4', '2') /* 24  YVU444 planar */

/* Bayer formats - see http://www.siliconimaging.com/RGB%20Bayer.htm */
#define V4L2_PIX_FMT_SBGGR8  v4l2_fourcc('B', 'A', '8', '1') /*  8  BGBG.. GRGR.. */
#define V4L2_PIX_FMT_SGBRG8  v4l2_fourcc('G', 'B', 'R', 'G') /*  8  GBGB.. RGRG.. */
#define V4L2_PIX_FMT_SGRBG8  v4l2_fourcc('G', 'R', 'B', 'G') /*  8  GRGR.. BGBG.. */
#define V4L2_PIX_FMT_SRGGB8  v4l2_fourcc('R', 'G', 'G', 'B') /*  8  RGRG.. GBGB.. */
#define V4L2_PIX_FMT_SBGGR10 v4l2_fourcc('B', 'G', '1', '0') /* 10  BGBG.. GRGR.. */
#define V4L2_PIX_FMT_SGBRG10 v4l2_fourcc('G', 'B', '1', '0') /* 10  GBGB.. RGRG.. */
#define V4L2_PIX_FMT_SGRBG10 v4l2_fourcc('B', 'A', '1', '0') /* 10  GRGR.. BGBG.. */
#define V4L2_PIX_FMT_SRGGB10 v4l2_fourcc('R', 'G', '1', '0') /* 10  RGRG.. GBGB.. */
/* 10bit raw bayer packed, 5 bytes for every 4 pixels */
#define V4L2_PIX_FMT_SBGGR10P v4l2_fourcc('p', 'B', 'A', 'A')
#define V4L2_PIX_FMT_SGBRG10P v4l2_fourcc('p', 'G', 'A', 'A')
#define V4L2_PIX_FMT_SGRBG10P v4l2_fourcc('p', 'g', 'A', 'A')
#define V4L2_PIX_FMT_SRGGB10P v4l2_fourcc('p', 'R', 'A', 'A')
/* 10bit raw bayer a-law compressed to 8 bits */
#define V4L2_PIX_FMT_SBGGR10ALAW8 v4l2_fourcc('a', 'B', 'A', '8')
#define V4L2_PIX_FMT_SGBRG10ALAW8 v4l2_fourcc('a', 'G', 'A', '8')
#define V4L2_PIX_FMT_SGRBG10ALAW8 v4l2_fourcc('a', 'g', 'A', '8')
#define V4L2_PIX_FMT_SRGGB10ALAW8 v4l2_fourcc('a', 'R', 'A', '8')
/* 10bit raw bayer DPCM compressed to 8 bits */
#define V4L2_PIX_FMT_SBGGR10DPCM8 v4l2_fourcc('b', 'B', 'A', '8')
#define V4L2_PIX_FMT_SGBRG10DPCM8 v4l2_fourcc('b', 'G', 'A', '8')
#define V4L2_PIX_FMT_SGRBG10DPCM8 v4l2_fourcc('B', 'D', '1', '0')
#define V4L2_PIX_FMT_SRGGB10DPCM8 v4l2_fourcc('b', 'R', 'A', '8')
#define V4L2_PIX_FMT_SBGGR12 v4l2_fourcc('B', 'G', '1', '2') /* 12  BGBG.. GRGR.. */
#define V4L2_PIX_FMT_SGBRG12 v4l2_fourcc('G', 'B', '1', '2') /* 12  GBGB.. RGRG.. */
#define V4L2_PIX_FMT_SGRBG12 v4l2_fourcc('B', 'A', '1', '2') /* 12  GRGR.. BGBG.. */
#define V4L2_PIX_FMT_SRGGB12 v4l2_fourcc('R', 'G', '1', '2') /* 12  RGRG.. GBGB.. */
#define V4L2_PIX_FMT_SBGGR16 v4l2_fourcc('B', 'Y', 'R', '2') /* 16  BGBG.. GRGR.. */

/* compressed formats */
#define V4L2_PIX_FMT_MJPEG    v4l2_fourcc('M', 'J', 'P', 'G') /* Motion-JPEG   */
#define V4L2_PIX_FMT_JPEG     v4l2_fourcc('J', 'P', 'E', 'G') /* JFIF JPEG     */
#define V4L2_PIX_FMT_DV       v4l2_fourcc('d', 'v', 's', 'd') /* 1394          */
#define V4L2_PIX_FMT_MPEG     v4l2_fourcc('M', 'P', 'E', 'G') /* MPEG-1/2/4 Multiplexed */
#define V4L2_PIX_FMT_H264     v4l2_fourcc('H', '2', '6', '4') /* H264 with start codes */
#define V4L2_PIX_FMT_H264_NO_SC v4l2_fourcc('A', 'V', 'C', '1') /* H264 without start codes */
#define V4L2_PIX_FMT_H264_MVC v4l2_fourcc('M', '2', '6', '4') /* H264 MVC */
#define V4L2_PIX_FMT_H264_SLICE v4l2_fourcc('S', '2', '6', '4') /* H264 parsed slices */
#define V4L2_PIX_FMT_H263     v4l2_fourcc('H', '2', '6', '3') /* H263          */
#define V4L2_PIX_FMT_MPEG1    v4l2_fourcc('M', 'P', 'G', '1') /* MPEG-1 ES     */
#define V4L2_PIX_FMT_MPEG2    v4l2_fourcc('M', 'P', 'G', '2') /* MPEG-2 ES     */
#define V4L2_PIX_FMT_MPEG4    v4l2_fourcc('M', 'P', 'G', '4') /* MPEG-4 part 2 ES */
#define V4L2_PIX_FMT_XVID     v4l2_fourcc('X', 'V', 'I', 'D') /* Xvid           */
#define V4L2_PIX_FMT_VC1_ANNEX_G v4l2_fourcc('V', 'C', '1', 'G') /* SMPTE 421M Annex G compliant stream */
#define V4L2_PIX_FMT_VC1_ANNEX_L v4l2_fourcc('V', 'C', '1', 'L') /* SMPTE 421M Annex L compliant stream */
#define V4L2_PIX_FMT_VP8      v4l2_fourcc('V', 'P', '8', '0') /* VP8 */
#define V4L2_PIX_FMT_VP8_FRAME v4l2_fourcc('V', 'P', '8', 'F') /* VP8 parsed frames */

/*  Vendor-specific formats   */
#define V4L2_PIX_FMT_CPIA1    v4l2_fourcc('C', 'P', 'I', 'A') /* cpia1 YUV */
#define V4L2_PIX_FMT_WNVA     v4l2_fourcc('W', 'N', 'V', 'A') /* Winnov hw compress */
#define V4L2_PIX_FMT_SN9C10X  v4l2_fourcc('S', '9', '1', '0') /* SN9C10x compression */
#define V4L2_PIX_FMT_SN9C20X_I420 v4l2_fourcc('S', '9', '2', '0') /* SN9C20x YUV 4:2:0 */
#define V4L2_PIX_FMT_PWC1     v4l2_fourcc('P', 'W', 'C', '1') /* pwc older webcam */
#define V4L2_PIX_FMT_PWC2     v4l2_fourcc('P', 'W', 'C', '2') /* pwc newer webcam */
#define V4L2_PIX_FMT_ET61X251 v4l2_fourcc('E', '6', '2', '5') /* ET61X251 compression */
#define V4L2_PIX_FMT_SPCA501  v4l2_fourcc('S', '5', '0', '1') /* YUYV per line */
#define V4L2_PIX_FMT_SPCA505  v4l2_fourcc('S', '5', '0', '5') /* YYUV per line */
#define V4L2_PIX_FMT_SPCA508  v4l2_fourcc('S', '5', '0', '8') /* YUVY per line */
#define V4L2_PIX_FMT_SPCA561  v4l2_fourcc('S', '5', '6', '1') /* compressed GBRG bayer */
#define V4L2_PIX_FMT_PAC207   v4l2_fourcc('P', '2', '0', '7') /* compressed BGGR bayer */
#define V4L2_PIX_FMT_MR97310A v4l2_fourcc('M', '3', '1', '0') /* compressed BGGR bayer */
#define V4L2_PIX_FMT_JL2005BCD v4l2_fourcc('J', 'L', '2', '0') /* compressed RGGB bayer */
#define V4L2_PIX_FMT_SN9C2028 v4l2_fourcc('S', 'O', 'N', 'X') /* compressed GBRG bayer */
#define V4L2_PIX_FMT_SQ905C   v4l2_fourcc('9', '0', '5', 'C') /* compressed RGGB bayer */
#define V4L2_PIX_FMT_PJPG     v4l2_fourcc('P', 'J', 'P', 'G') /* Pixart 73xx JPEG */
#define V4L2_PIX_FMT_OV511    v4l2_fourcc('O', '5', '1', '1') /* ov511 JPEG */
#define V4L2_PIX_FMT_OV518    v4l2_fourcc('O', '5', '1', '8') /* ov518 JPEG */
#define V4L2_PIX_FMT_STV0680  v4l2_fourcc('S', '6', '8', '0') /* stv0680 bayer */
#define V4L2_PIX_FMT_TM6000   v4l2_fourcc('T', 'M', '6', '0') /* tm5600/tm60x0 */
#define V4L2_PIX_FMT_CIT_YYVYUY v4l2_fourcc('C', 'I', 'T', 'V') /* one line of Y then 1 line of VYUY */
#define V4L2_PIX_FMT_KONICA420  v4l2_fourcc('K', 'O', 'N', 'I') /* YUV420 planar in blocks of 256 pixels */
#define V4L2_PIX_FMT_JPGL   v4l2_fourcc('J', 'P', 'G', 'L') /* JPEG-Lite */
#define V4L2_PIX_FMT_SE401      v4l2_fourcc('S', '4', '0', '1') /* se401 janggu compressed rgb */
#define V4L2_PIX_FMT_S5C_UYVY_JPG v4l2_fourcc('S', '5', 'C', 'I') /* S5C73M3 interleaved UYVY/JPEG */
#define V4L2_PIX_FMT_Y8I      v4l2_fourcc('Y', '8', 'I', ' ') /* Greyscale 8-bit L/R interleaved */
#define V4L2_PIX_FMT_Y12I     v4l2_fourcc('Y', '1', '2', 'I') /* Greyscale 12-bit L/R interleaved */
#define V4L2_PIX_FMT_Z16      v4l2_fourcc('Z', '1', '6', ' ') /* Depth data 16-bit */

/* SDR formats - used only for Software Defined Radio devices */
#define V4L2_SDR_FMT_CU8          v4l2_fourcc('C', 'U', '0', '8') /* IQ u8 */
#define V4L2_SDR_FMT_CU16LE       v4l2_fourcc('C', 'U', '1', '6') /* IQ u16le */
#define V4L2_SDR_FMT_CS8          v4l2_fourcc('C', 'S', '0', '8') /* complex s8 */
#define V4L2_SDR_FMT_CS14LE       v4l2_fourcc('C', 'S', '1', '4') /* complex s14le */
#define V4L2_SDR_FMT_RU12LE       v4l2_fourcc('R', 'U', '1', '2') /* real u12le */

/* Vendor specific - used for Rockchip ISP1 camera sub-system */
#define V4L2_META_FMT_RK_ISP1_PARAMS    v4l2_fourcc('R', 'K', '1', 'P') /* Rockchip ISP1 params */
#define V4L2_META_FMT_RK_ISP1_STAT_3A   v4l2_fourcc('R', 'K', '1', 'S') /* Rockchip ISP1 3A statistics */

/* Meta-data formats */
#define V4L2_META_FMT_UVC         v4l2_fourcc('U', 'V', 'C', 'H') /* UVC Payload Header metadata */

/* priv field value to indicates that subsequent fields are valid. */
#define V4L2_PIX_FMT_PRIV_MAGIC     0xfeedcafe

/* Flags */
#define V4L2_PIX_FMT_FLAG_PREMUL_ALPHA  0x00000001


/* Exported types ------------------------------------------------------------*/
typedef enum camera_mbus_type
{
    CAMERA_MBUS_UNKNOWN,
    CAMERA_MBUS_PARALLEL,
    CAMERA_MBUS_BT601,
    CAMERA_MBUS_BT656,
    CAMERA_MBUS_BT1120,
    CAMERA_MBUS_CSI2_DPHY,
    CAMERA_MBUS_LVDS_DPHY
} eCAMERA_Mbustype;

/**
 * rk_camera_crop_rect - set the crop parameter for camera window
 */
struct rk_camera_crop_rect
{
    uint32_t x_axis;     /**< x_axis: Horizontal offset of the top, left corner of the rectangle in pixels */
    uint32_t y_axis;     /**< y_axis: Vertical offset of the top, left corner of the rectangle  in pixels */
    uint32_t width;      /**< width: Width of the rectangle in pixels */
    uint32_t height;     /**< height: Height of the rectangle in pixels.*/
};

/* copy from v4l2:struct v4l2_mbus_framefmt */
struct rk_camera_mbus_framefmt
{
    uint32_t width;
    uint32_t height;
    uint32_t bitsperpixel;
    uint32_t bytesperline;
    uint32_t pixelcode;
    uint32_t field;
    uint32_t colorspace;
    /*
    uint16_t ycbcr_enc;
    uint16_t quantization;
    uint16_t xfer_func;
    uint16_t reserved[11];
    */
};

struct rk_camera_mbus_lvds
{
    uint16_t sync_code_eav_blk;
    uint16_t sync_code_sav_blk;
    uint16_t sync_code_eav_act;
    uint16_t sync_code_sav_act;
};
struct rk_camera_mbus_config
{
    uint32_t flags;/* depends on mbus_type */
    uint32_t std_id;
    uint32_t linked_freq; /* mipi data rate (hz) */
    eCAMERA_Mbustype mbus_type;
    struct rk_camera_mbus_lvds lvds_config;
};

/**
 * rk_camera_pix_format - set the image foramt of frame
 */
struct rk_camera_pix_format
{
    uint32_t width;      /**< width: Image width in pixels */
    uint32_t height;     /**< height: Image height in pixels */
    uint32_t pixelformat; /**< pixelformat: The pixel format or type of compression,et by the application. This is a little fourcc,the same as the fourcc of struct cif_output_fmt */
    uint32_t field;      /**< field: enum v4l2_field */
    uint32_t bytesperline;   /**< bytesperline: Distance in bytes between the leftmost */
    uint32_t sizeimage;  /**< sizeimage: Size in bytes of the buffer to hold a complete image, set by the driver */
    uint32_t colorspace; /**< colorspace: Image colorspace, from enum v4l2_colorspace */
    uint32_t priv;       /**< priv: This field indicates whether the remaining fields of the struct v4l2_pix_format, also called the extended fields, are valid */
    uint32_t flags;      /**< flags: Flags set by the application or driver */
    uint32_t ycbcr_enc;  /**< ycbcr_enc: ’CbCr encoding, from enum v4l2_ycbcr_encoding.This information supplements the colorspace and must be set */
    uint32_t quantization;   /**< quantization: Quantization range, from enum v4l2_quantization */
    uint32_t xfer_func;  /**< xfer_func: Transfer function, from enum v4l2_xfer_func */
};

#endif

