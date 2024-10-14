/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_panel.h
  * @version V0.1
  * @brief   display panel struct
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-02-20     Huang Jiachai   first implementation
  *
  ******************************************************************************
  */

#ifndef __DRV_PANEL_H__
#define __DRV_PANEL_H__

#include <rtthread.h>
/*
 * RK_DISPLAY_MAX_CMD_PAYLOAD_NUM is the max length of cmd payload.
 * Such as, the settings of panel PPS needs more parameters.
 * So we need adjust this.
 */
#define RK_DISPLAY_MAX_CMD_PAYLOAD_NUM   90

struct rockchip_cmd_header
{
    uint8_t data_type;
    uint8_t delay_ms;
    uint8_t payload_length;
} __packed;

struct rockchip_cmd_desc
{
    struct rockchip_cmd_header header;
    const uint8_t *payload;
};

struct rockchip_panel_cmds
{
    struct rockchip_cmd_desc *cmds;
    uint16_t cmd_cnt;
};

struct rockchip_cmd
{
    uint8_t data_type;
    uint8_t delay_ms;
    uint8_t payload_length;
    uint8_t payload[RK_DISPLAY_MAX_CMD_PAYLOAD_NUM];
};

#endif /* __DRV_PANEL_H__ */
