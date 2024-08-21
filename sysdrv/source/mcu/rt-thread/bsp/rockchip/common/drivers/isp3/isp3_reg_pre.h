/**
  * Copyright (c) 2022 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    isp3_reg_pre.h
  * @version V0.0.1
  * @date    6-July-2022
  * @brief
  *
  ******************************************************************************
  */

#if defined(RT_USING_ISP3)

#ifndef _ISP3_REG_PRE_H_
#define _ISP3_REG_PRE_H_

/***************************** Structure Definition **************************/

/**
 * enum all work mode of vicap
 */
typedef enum
{
    ISP_WORKMODE_FRAME_ONEFRAME = 0x0,
    ISP_WORKMODE_FRAME_PINGPONG,
    ISP_WORKMODE_BLOCK_PINGPONG,
} eISP_workMode;

/**
 * enum all uv stored type of vicap
 */
typedef enum
{
    ISP_UV_STORE_ORDER_UVUV = 0x0,
    ISP_UV_STORE_ORDER_VUVU,
} eISP_uvStoreOrder;

/**
 * enum all raw end type
 */
typedef enum
{
    ISP_RAW_END_LITTLE = 0x0,
    ISP_RAW_END_BIG,
} eISP_rawEnd;

/**
 * enum yuv420 uv output in even or odd line
 */
typedef enum
{
    ISP_OUT_420_ORDER_UV_EVEN = 0x0,
    ISP_OUT_420_ORDER_UV_ODD,
} eISP_out420Order;

/**
 * enum yuv output data type
 */
typedef enum
{
    ISP_OUTPUT_IS_422 = 0x0,
    ISP_OUTPUT_IS_420,
} eISP_outputFormat;

/**
 * enum raw width type
 */
typedef enum
{
    ISP_RAW_WIDTH_8BIT = 0x0,
    ISP_RAW_WIDTH_10BIT,
    ISP_RAW_WIDTH_12BIT,
} eISP_rawWidth;

/**
 * enum jpeg mode
 */
typedef enum
{
    ISP_JPEG_OTHERS = 0x0,
    ISP_JPEG_MODE_1,
} eISP_jpegMode;

/**
 * enum field order type
 */
typedef enum
{
    ISP_FIELD_ORDER_ODD_FIRST = 0x0,
    ISP_FIELD_ORDER_EVEN_FIRST,
} eISP_fieldOrder;

/**
 * enum yuv input order type
 */
typedef enum
{
    ISP_YUV_IN_ORDER_UYVY = 0x0,
    ISP_YUV_IN_ORDER_YVYU,
    ISP_YUV_IN_ORDER_VYUY,
    ISP_YUV_IN_ORDER_YUYV,
} eISP_yuvInOrder;

/**
 * enum vicap input mode type
 */
typedef enum
{
    ISP_INPUTMODE_YUV = 0x00,
    ISP_INPUTMODE_PAL = 0x02,
    ISP_INPUTMODE_NTSC,
    ISP_INPUTMODE_RAW,
    ISP_INPUTMODE_JPEG,
    ISP_INPUTMODE_BT1120,
} eISP_inputMode;

/**
 * enum polarity of vicap href
 */
typedef enum
{
    ISP_HREF_POL_HIGH_ACTIVE = 0x0,
    ISP_HREF_POL_LOW_ACTIVE,
} eISP_hrefPol;

/**
 * enum polarity of vicap vsync
 */
typedef enum
{
    ISP_VSYNC_POL_LOW_ACTIVE = 0x0,
    ISP_VSYNC_POL_HIGH_ACTIVE,
} eISP_vsyncPol;

/**
 * enum path of vicap input data
 */
typedef enum
{
    ISP_PATH_SEL_RAW = 0x0,
    ISP_PATH_SEL_YUV,
} eISP_pathSel;

/**
 * enum water line type of vicap
 */
typedef enum
{
    ISP_WATER_LINE_75_PERCENT = 0x0,
    ISP_WATER_LINE_50_PERCENT,
    ISP_WATER_LINE_25_PERCENT,
    ISP_WATER_LINE_0_PERCENT,
} eISP_waterLineLvl;

/**
 * struct ISP_WATERLINE_INFO - the configuration of vicap water line.
 */
struct ISP_WATERLINE_INFO
{
    bool enable;
    uint8_t hurryValue;
    eISP_waterLineLvl level;
};

/***************************** Function Declare ******************************/


#endif

#endif
