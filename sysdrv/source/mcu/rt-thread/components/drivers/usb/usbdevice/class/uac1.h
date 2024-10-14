/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    uac1.h
  * @version V0.1
  * @brief   usb audio class 1.0 driver
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-09-17     Liangfeng Wu    first implementation
  *
  ******************************************************************************
  */
#ifndef __UAC1_H__
#define __UAC1_H__

#include <rtdevice.h>
#include "drivers/usb_audio.h"

DECLARE_UAC_AC_HEADER_DESCRIPTOR(2);
DECLARE_UAC_FEATURE_UNIT_DESCRIPTOR(0);
DECLARE_UAC_FORMAT_TYPE_I_DISCRETE_DESC(6);

struct uac1_control
{
#ifdef RT_USB_DEVICE_COMPOSITE
    struct uiad_descriptor iad_desc;
#endif
    struct uinterface_descriptor interface_desc;
    struct uac1_ac_header_descriptor_2 header_desc;
    struct uac_input_terminal_descriptor usb_out_it_desc;
    struct uac1_output_terminal_descriptor io_out_ot_desc;
    struct uac_feature_unit_descriptor_0 usb_out_it_feature_desc;
    struct uac_input_terminal_descriptor io_in_it_desc;
    struct uac1_output_terminal_descriptor usb_in_ot_desc;
    struct uac_feature_unit_descriptor_0 io_in_it_feature_desc;
};
typedef struct uac1_control *uac1_control_t;

struct uac1_interface_alt
{
    struct uinterface_descriptor interface_desc;
    struct uac1_as_header_descriptor as_header_desc;
    struct uac_format_type_i_discrete_descriptor_6 type_i_desc;
    struct uaudio_endpoint_descriptor endpoint_desc;
    struct uac_iso_endpoint_descriptor iso_endpoint_desc;
};
typedef struct uac1_interface_alt *uac1_interface_alt_t;

#endif /* __UAC1_H__ */
