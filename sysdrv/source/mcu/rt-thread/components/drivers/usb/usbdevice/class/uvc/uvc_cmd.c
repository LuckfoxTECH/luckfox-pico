/**
  * Copyright (c) 2020 Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    uvc_cmd.c
  * @version V0.1
  * @brief   uvc command implementation
  *
  * Change Logs:
  * Date           Author          Notes
  * 2020-07-01     Frank Wang      first implementation
  *
  ******************************************************************************
  */
#include <rtthread.h>
#include <rthw.h>
#include <rtservice.h>
#include <rtdevice.h>
#include "drivers/usb_common.h"
#include "uvc.h"
#include "uvc_cmd.h"

rt_err_t uvc_cmd_video_stream_on(int id)
{
    return uvc_video_stream_enable(uvc_id_to_uvc(id), 1);
}

rt_err_t uvc_cmd_video_stream_off(int id)
{
    return uvc_video_stream_enable(uvc_id_to_uvc(id), 0);
}

rt_err_t uvc_cmd_video_stop()
{
    uvc_video_stop_all();
    return RT_EOK;
}
