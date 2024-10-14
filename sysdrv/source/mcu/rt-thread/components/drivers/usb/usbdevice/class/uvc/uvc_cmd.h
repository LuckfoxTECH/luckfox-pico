/**
  * Copyright (c) 2020 Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    uvc_cmd.h
  * @version V0.1
  * @brief   usb video class request header
  *
  * Change Logs:
  * Date           Author          Notes
  * 2020-07-01     Frank Wang      first implementation
  *
  ******************************************************************************
  */
#ifndef __UVC_CMD_H__
#define __UVC_CMD_H__

rt_err_t uvc_cmd_video_stop();
rt_err_t uvc_cmd_video_stream_on(int id);
rt_err_t uvc_cmd_video_stream_off(int id);

#endif /* __UVC_CMD_H__ */
