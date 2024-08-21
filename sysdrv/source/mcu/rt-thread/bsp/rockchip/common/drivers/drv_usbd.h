/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_usbd.h
  * @version V1.0
  * @brief   usb device driver headfile
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-11-12     Liangfeng Wu    the first version
  *
  ******************************************************************************
  */

#ifndef __DRV_USB_DEVICE_H__
#define __DRV_USB_DEVICE_H__

#include <rtdevice.h>

extern struct ep_id g_usb_ep_pool[];
ePCD_bcdMsg usb_get_bc_info(void);

#endif /* __DRV_USB_DEVICE_H__ */
