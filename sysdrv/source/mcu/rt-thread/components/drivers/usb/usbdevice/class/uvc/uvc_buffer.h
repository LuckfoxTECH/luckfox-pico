/**
  * Copyright (c) 2020 Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    uvc_buffer.h
  * @version V0.1
  * @brief   usb buffer process header
  *
  * Change Logs:
  * Date           Author          Notes
  * 2020-07-01     Frank Wang      first implementation
  *
  ******************************************************************************
  */
#ifndef __UVC_BUFFER_H__
#define __UVC_BUFFER_H__

#include <rtdevice.h>
#include "drivers/usb_video.h"
#include "hal_base.h"

#define UVC_BUFFER_NUM    4
#define UVC_HEADER_SIZE   0x04 /* 4 Byte */

/* buffer manager related define */
enum bufmgr_wch
{
    BUFMGR_WCH0 = 0,
    BUFMGR_WCH1,
    BUFMGR_WCH2,
    BUFMGR_WCH3,
    BUFMGR_WCH_CNT
};

enum bufmgr_blk
{
    BUFMGR_BLK0 = 0,
    BUFMGR_BLK1,
    BUFMGR_BLK_CNT
};

enum bufmgr_irq
{
    BUFMGR_WCH_IRQ_WCH0  = BUFFER_MANAGER_WCH0_IRQn,
    BUFMGR_WCH_IRQ_WCH1  = BUFFER_MANAGER_WCH1_IRQn,
    BUFMGR_WCH_IRQ_WCH2  = BUFFER_MANAGER_WCH2_IRQn,
    BUFMGR_WCH_IRQ_WCH3  = BUFFER_MANAGER_WCH3_IRQn,
};

enum uvc_buffer_state
{
    BUF_STATE_EMPTY = 0,
    BUF_STATE_FULL,
    BUF_STATE_BUSY
};

struct uvc_buffer
{
    uint8_t *data;
    uint8_t  eoh;
    uint8_t  eof;
    uint8_t  err;
    uint32_t index;
    uint32_t length;
    enum uvc_buffer_state state;
    struct uvc_buffer *next;

    /* Some other properties defined in HW */
};
typedef struct uvc_buffer *uvc_buffer_t;

#endif /* __UVC_BUFFER_H__ */
