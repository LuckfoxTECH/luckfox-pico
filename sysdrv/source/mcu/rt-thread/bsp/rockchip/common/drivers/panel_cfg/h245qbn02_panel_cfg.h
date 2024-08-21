/**
  * Copyright (c) 2021 Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    h245qbn02_panel_cfg.h
  * @version V0.1
  * @brief   display panel config for h245qbn02 panel
  *
  * Change Logs:
  * Date           Author          Notes
  * 2021-03-15     Chris Zhong     first implementation
  *
  ******************************************************************************
  */

#ifndef __H245QBN02_PANEL_CFG_H__
#define __H245QBN02_PANEL_CFG_H__

#define RT_HW_LCD_XRES                240    /* LCD PIXEL WIDTH             */
#define RT_HW_LCD_YRES                432    /* LCD PIXEL HEIGHT            */
#define RT_HW_LCD_PIXEL_CLOCK         9600  /* pixel clock in khz          */
#define RT_HW_LCD_LANE_MBPS           240    /* DSI per lane Mbps           */
#define RT_HW_LCD_LEFT_MARGIN         52     /* Horizontal back porch       */
#define RT_HW_LCD_RIGHT_MARGIN        50     /* Horizontal front porch      */
#define RT_HW_LCD_UPPER_MARGIN        16      /* Vertical back porch         */
#define RT_HW_LCD_LOWER_MARGIN        14      /* Vertical front porch        */
#define RT_HW_LCD_HSYNC_LEN           2      /* Horizontal synchronization  */
#define RT_HW_LCD_VSYNC_LEN           2      /* Vertical synchronization    */

#define RT_HW_LCD_CONN_TYPE           RK_DISPLAY_CONNECTOR_DSI
#define RT_HW_LCD_BUS_FORMAT          MEDIA_BUS_FMT_RGB888_1X24
#define RT_HW_LCD_INIT_CMD_TYPE       CMD_TYPE_DEFAULT
#define RT_HW_LCD_DSI_LANES           1
#define RT_HW_LCD_VMODE_FLAG          DSI_MODE_LPM
#define RT_HW_LCD_DISPLAY_MODE        DISPLAY_CMD_MODE
#define RT_HW_LCD_AREA_DISPLAY        DISABLE_AREA_DISPLAY
#define RT_HW_LCD_DCS_BACKLIGHT       0
#define RT_HW_LCD_MAX_BRIGHTNESS      255

#define RT_HW_LCD_XACT_ALIGN          1
#define RT_HW_LCD_YACT_ALIGN          1
#define RT_HW_LCD_XPOS_ALIGN          1
#define RT_HW_LCD_YPOS_ALIGN          1

const static struct rockchip_cmd cmd_on[] =
{
    {0x05, 0x64, 0x01, {0x11}},
    {0x39, 0x00, 0x05, {0x2a, 0x00, 0x00, 0x00, 0xef}},
    {0x39, 0x00, 0x05, {0x2b, 0x00, 0x00, 0x01, 0xaf}},
    {0x15, 0x00, 0x02, {0x35, 0x00}},
    {0x05, 0x00, 0x01, {0x11}},
    {0x05, 0xc8, 0x01, {0x29}},
    {0x39, 0x00, 0x03, {0xbd, 0x11, 0x00}},
    {0x39, 0x00, 0x03, {0xbc, 0x00, 0x94}},
    {0x39, 0x00, 0x03, {0xbe, 0xd0, 0x02}},
    {0x05, 0x00, 0x01, {0x2c}},
};

const static struct rockchip_cmd cmd_off[] =
{
    {0x05, 0x00, 0x01, {0x28}},
    {0x05, 0x00, 0x01, {0x10}},
};

#endif /* __H245QBN02_PANEL_CFG_H__ */
