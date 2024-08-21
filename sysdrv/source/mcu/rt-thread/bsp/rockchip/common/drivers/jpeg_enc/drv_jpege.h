/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_jpege.h
  * @version V0.0.1
  * @brief   jpeg enc device abstract for swallow
  *
  * Change Logs:
  * Date           Author          Notes
  * 2020-09-12     jpeg enc Team   first implementation
  *
  ******************************************************************************
  */

#ifndef __DRV_JPEGE_H__
#define __DRV_JPEGE_H__

#include <rtthread.h>
#include <rtdevice.h>
#include <rtdef.h>
#include "jpeg_enc.h"
#include "jpege_hdr.h"

typedef void (*rt_isr_handler_t)(int vector, void *param);

enum control_cmd
{
    CMD_SET_CFG,
    CMD_JPEG_ENC_START,
    CMD_GET_STREAM_LEN,
    CMD_SET_JPEG_HEAD_TIME_STAMP,
    CMD_SET_JPEG_ACLK,
};

struct jpeg_cfg
{
    uint32_t              width;
    uint32_t              height;
    uint32_t              hor_stride;
    uint32_t              ver_stride;

    uint32_t              quality;
    int32_t               q_factor;
    uint8_t               *yuv_y_addr;
    uint8_t               *yuv_u_addr;
    uint8_t               *yuv_v_addr;
    int32_t               input_size;
    uint8_t               *jpeg_header_addr;
    uint8_t               *output_addr;
    enum frame_format     format;

};

enum jpeg_dev_status
{
    STATUS_IDLE,
    STATUS_OPEN,
    STATUS_START,
    STATUS_CLOSE,
};

struct rt_jpege_device
{
    struct rt_device    parent;
    struct jpege_ctx    *ctx;
    rt_sem_t            jpege_done_sem;
    IRQn_Type           irq_num;
    char                *name;
    rt_isr_handler_t    irq_handler;
    uint32_t            dev_id;
    enum jpeg_dev_status status;
    rt_isr_handler_t    timer_handler;
    IRQn_Type           timer_irq_num;
    struct TIMER_REG    *timer;
    struct clk_gate     *aclk;
    struct clk_gate     *hclk;
    struct clk_gate     *bus_pll;
    struct clk_gate     *bus_niu;
};



#endif
