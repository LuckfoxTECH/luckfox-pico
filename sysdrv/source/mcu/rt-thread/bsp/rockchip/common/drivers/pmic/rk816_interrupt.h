/**
  * Copyright (c) 2020 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    rk816_INTERRUPT.h
  * @version V0.1
  * @brief   rk816 INTERRUPT interface
  *
  * Change Logs:
  * Date           Author          Notes
  * 2020-04-12     Elaine Zhang     first implementation
  *
  ******************************************************************************
  */

#define RK816_PWRON_FALL_INT_NUM      5
#define RK816_PWRON_RISE_INT_NUM      6
#define RK816_VB_LO_INT_NUM           9
#define RK816_PWRON_INT_NUM           10
#define RK816_PWRON_LP_INT_NUM        11
#define RK816_HOTDIE_INT_NUM          12
#define RK816_RTC_ALARM_INT_NUM       13
#define RK816_RTC_PERIOD_INT_NUM      14
#define RK816_USB_OV_INT_NUM          15
#define RK816_PLUG_IN_INT_NUM         16
#define RK816_PLUG_OUT_INT_NUM        17
#define RK816_CHGOK_INT_NUM           18
#define RK816_CHGTE_INT_NUM           19
#define RK816_CHGTS_INT_NUM           20
#define RK816_CHG_CVTLIM_INT_NUM      23
#define RK816_DISCHG_ILIM_INT_NUM     24

#define RK816_INT_NUM                 25

typedef void (*rk816_int)(void *args);
rt_err_t rk816_isr_sethook(void (*hook)(void));
extern void rk816_irq_register(int irq, rk816_int isr, void *args);
