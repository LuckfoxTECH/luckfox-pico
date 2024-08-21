/**
  * Copyright (c) 2020 Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    am018rt90211_panel_cfg.h
  * @version V0.1
  * @brief   display panel config for am018rt90211 panel
  *
  * Change Logs:
  * Date           Author          Notes
  * 2020-09-30     Nickey Yang     first implementation
  *
  ******************************************************************************
  */

#ifndef __AM018RT90211_PANEL_CFG_H__
#define __AM018RT90211_PANEL_CFG_H__

#define RT_HW_LCD_XRES                368    /* LCD PIXEL WIDTH             */
#define RT_HW_LCD_YRES                448    /* LCD PIXEL HEIGHT            */
#define RT_HW_LCD_PIXEL_CLOCK         12400  /* pixel clock in khz          */
#define RT_HW_LCD_LANE_MBPS           380    /* DSI per lane Mbps           */
#define RT_HW_LCD_LEFT_MARGIN         20     /* Horizontal back porch       */
#define RT_HW_LCD_RIGHT_MARGIN        40     /* Horizontal front porch      */
#define RT_HW_LCD_UPPER_MARGIN        8      /* Vertical back porch         */
#define RT_HW_LCD_LOWER_MARGIN        6      /* Vertical front porch        */
#define RT_HW_LCD_HSYNC_LEN           2      /* Horizontal synchronization  */
#define RT_HW_LCD_VSYNC_LEN           2      /* Vertical synchronization    */

#define RT_HW_LCD_CONN_TYPE           RK_DISPLAY_CONNECTOR_DSI
#define RT_HW_LCD_BUS_FORMAT          MEDIA_BUS_FMT_RGB888_1X24
#define RT_HW_LCD_INIT_CMD_TYPE       CMD_TYPE_DEFAULT
#define RT_HW_LCD_DSI_LANES           1
#define RT_HW_LCD_VMODE_FLAG          DSI_MODE_LPM
#define RT_HW_LCD_DISPLAY_MODE        DISPLAY_CMD_MODE
#define RT_HW_LCD_AREA_DISPLAY        DISABLE_AREA_DISPLAY
#define RT_HW_LCD_DCS_BACKLIGHT       1
#define RT_HW_LCD_MAX_BRIGHTNESS      255

#define RT_HW_LCD_XACT_ALIGN          1
#define RT_HW_LCD_YACT_ALIGN          1
#define RT_HW_LCD_XPOS_ALIGN          1
#define RT_HW_LCD_YPOS_ALIGN          1

const static struct rockchip_cmd cmd_on[] =
{
    {0x15, 0x00, 0x02, {0xfe, 0x00}},
    {0x15, 0x00, 0x02, {0x35, 0x00}},
    {0x15, 0x00, 0x02, {0x51, 0xff}},
    {0x39, 0x00, 0x05, {0x2a, 0x00, 0x10, 0x01, 0x7f}},
    {0x39, 0x00, 0x05, {0x2b, 0x00, 0x00, 0x01, 0xbf}},
    {0x05, 0x08, 0x01, {0x11}},
    {0x05, 0x08, 0x01, {0x29}},
    /* HBM MODE ON */
    {0x15, 0x00, 0x02, {0xfe, 0x00}},
    {0x15, 0x00, 0x02, {0x66, 0x02}},
    /* EXIT IDLE AND AOD */
    {0x05, 0x05, 0x01, {0x38}},
    {0x15, 0x00, 0x02, {0x53, 0x20}},
};

const static struct rockchip_cmd cmd_off[] =
{
    {0x05, 0x00, 0x01, {0x28}},
    {0x05, 0x00, 0x01, {0x10}},
    {0x15, 0x00, 0x02, {0x4f, 0x01}},
};

#endif /* __AM018RT90211_PANEL_CFG_H__ */
