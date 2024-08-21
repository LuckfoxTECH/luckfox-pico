/**
  * Copyright (c) 2020 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    st7796h2_panel_cfg.h
  * @version V0.1
  * @brief   display panel config for st7796 h2 panel
  *
  * Change Logs:
  * Date           Author          Notes
  * 2020-06-06     Nickey Yang     first implementation
  *
  ******************************************************************************
  */

#ifndef __ST7796H2_PANEL_CFG_H__
#define __ST7796H2_PANEL_CFG_H__

#define RT_HW_LCD_XRES                320     /* LCD PIXEL WIDTH             */
#define RT_HW_LCD_YRES                385     /* LCD PIXEL HEIGHT            */
#define RT_HW_LCD_PIXEL_CLOCK         10300   /* Bit per pixel               */
#define RT_HW_LCD_LANE_MBPS           360     /* DSI per lane Mbps           */
#define RT_HW_LCD_LEFT_MARGIN         100     /* Horizontal back porch       */
#define RT_HW_LCD_RIGHT_MARGIN        38      /* Horizontal front porch      */
#define RT_HW_LCD_UPPER_MARGIN        10      /* Vertical back porch         */
#define RT_HW_LCD_LOWER_MARGIN        8       /* Vertical front porch        */
#define RT_HW_LCD_HSYNC_LEN           50      /* Horizontal synchronization  */
#define RT_HW_LCD_VSYNC_LEN           4       /* Vertical synchronization    */

#define RT_HW_LCD_CONN_TYPE           RK_DISPLAY_CONNECTOR_DSI
#define RT_HW_LCD_BUS_FORMAT          MEDIA_BUS_FMT_RGB888_1X24
#define RT_HW_LCD_INIT_CMD_TYPE       CMD_TYPE_DEFAULT
#define RT_HW_LCD_DSI_LANES           1
#define RT_HW_LCD_VMODE_FLAG          DSI_MODE_LPM
#define RT_HW_LCD_DISPLAY_MODE        DISPLAY_CMD_MODE
#define RT_HW_LCD_AREA_DISPLAY        DISABLE_AREA_DISPLAY

#define RT_HW_LCD_XACT_ALIGN          1
#define RT_HW_LCD_YACT_ALIGN          1
#define RT_HW_LCD_XPOS_ALIGN          1
#define RT_HW_LCD_YPOS_ALIGN          1

const static struct rockchip_cmd cmd_on[] =
{
    {0x15, 0x00, 0x02, {0x36, 0x48}},
    {0x15, 0x00, 0x02, {0x3a, 0x77}},
    {0x15, 0x00, 0x02, {0xf0, 0xc3}},
    {0x15, 0x00, 0x02, {0xf0, 0x96}},
    {0x29, 0x00, 0x03, {0xb1, 0x80, 0x10}},
    {0x29, 0x00, 0x09, {0xe8, 0x40, 0x8a, 0x1d, 0x21, 0x28, 0x14, 0x3f, 0x33}},
    {0x29, 0x00, 0x03, {0xc0, 0xa0, 0x00}},
    {0x15, 0x00, 0x02, {0xc2, 0xa7}},
    {0x15, 0x00, 0x02, {0xc5, 0x00}},
    {
        0x29, 0x00, 0xf, {
            0xe0, 0xf0, 0x00, 0x04, 0x0d, 0x10, 0x3b, 0x38, 0x43, 0x4e,
            0x39, 0x16, 0x15, 0x2f, 0x35
        }
    },
    {
        0x29, 0x00, 0xf, {
            0xe1, 0xf0, 0x00, 0x04, 0x0d, 0x10, 0x3a, 0x37, 0x33, 0x4d,
            0x39, 0x15, 0x15, 0x2f, 0x34
        }
    },
    {0x29, 0x00, 0x05, {0x2a, 0x00, 0x00, 0x01, 0x3f}},
    {0x29, 0x00, 0x05, {0x2b, 0x00, 0x00, 0x01, 0x81}},
    {0x15, 0x00, 0x02, {0xf0, 0x3c}},
    {0x15, 0x00, 0x02, {0xf0, 0x69}},
    {0x15, 0x00, 0x02, {0x35, 0x00}},
    {0x05, 0x78, 0x01, {0x11}},
    {0x05, 0x05, 0x01, {0x29}},
    {0x05, 0x05, 0x01, {0x21}},
};

const static struct rockchip_cmd cmd_off[] =
{
    {0x05, 0x00, 0x01, {0x28}},
    {0x05, 0x00, 0x01, {0x10}},
};

#endif /* __ST7796H2_PANEL_CFG_H__ */
