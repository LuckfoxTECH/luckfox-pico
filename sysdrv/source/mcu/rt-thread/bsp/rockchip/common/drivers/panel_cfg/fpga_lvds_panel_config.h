/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    fpga_lvds_panel_config.h
  * @version V0.1
  * @brief   display panel config for fpga lvds
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-02-20     Huang Jiachai   first implementation
  *
  ******************************************************************************
  */

#ifndef __FPGA_LVDS_PANEL_CONFIG_H__
#define __FPGA_LVDS_PANEL_CONFIG_H__

#define RT_HW_LCD_XRES                800     /* LCD PIXEL WIDTH             */
#define RT_HW_LCD_YRES                1280    /* LCD PIXEL HEIGHT            */
#define RT_HW_LCD_PIXEL_CLOCK         96      /* Pixel clock Khz             */
#define RT_HW_LCD_LANE_MBPS           0       /* DSI per lane Mbps           */
#define RT_HW_LCD_LEFT_MARGIN         39      /* Horizontal back porch       */
#define RT_HW_LCD_RIGHT_MARGIN        24      /* Horizontal front porch      */
#define RT_HW_LCD_UPPER_MARGIN        2       /* Vertical back porch         */
#define RT_HW_LCD_LOWER_MARGIN        5       /* Vertical front porch        */
#define RT_HW_LCD_HSYNC_LEN           1       /* Horizontal synchronization  */
#define RT_HW_LCD_VSYNC_LEN           1       /* Vertical synchronization    */

#define RT_HW_LCD_CONN_TYPE           RK_DISPLAY_CONNECTOR_DSI
#define RT_HW_LCD_BUS_FORMAT          MEDIA_BUS_FMT_RGB888_1X24
#define RT_HW_LCD_DSI_LANES           4
#define RT_HW_LCD_VMODE_FLAG          0
#define RT_HW_LCD_INIT_CMD_TYPE       CMD_TYPE_MCU
#define RT_HW_LCD_DISPLAY_MODE        DISPLAY_VIDEO_MODE
#define RT_HW_LCD_AREA_DISPLAY        DISABLE_AREA_DISPLAY

#define RT_HW_LCD_XACT_ALIGN          1
#define RT_HW_LCD_YACT_ALIGN          1
#define RT_HW_LCD_XPOS_ALIGN          1
#define RT_HW_LCD_YPOS_ALIGN          1

const static struct rockchip_cmd cmd_on[] =
{
    {0x05, 0x05, 0x01, {0x11}},
    {0x29, 0x00, 0x03, {0xf0, 0x5a, 0x5a}},
    {0x23, 0x00, 0x02, {0xb0, 0x01}},
};

const static struct rockchip_cmd cmd_off[] =
{
    {0x05, 0x14, 0x01, {28}},
    {0x39, 0x00, 0x03, {0x12, 0x34, 0x56}},
    {0x05, 0x00, 0x01, {10}},
};

#endif /* __FPGA_LVDS_PANEL_CONFIG_H__ */
