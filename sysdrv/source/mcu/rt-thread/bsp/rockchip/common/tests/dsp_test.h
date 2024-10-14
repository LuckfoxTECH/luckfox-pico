/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    dsp_test.h
  * @author  Huaping Liao
  * @version V0.1
  * @date    20-Mar-2019
  * @brief   dsp test for pisces to receive voice wakeup
  *
  ******************************************************************************
  */

#ifndef _DSP_TEST_H_
#define _DSP_TEST_H_

#include <stdint.h>

typedef void (*dsp_wakeup_call)(uint32_t params);

uint32_t dsp_malloc(uint32_t size);
uint32_t dsp_get_period_size(void);
void dsp_wakeup_register_call(dsp_wakeup_call callback);

#endif // _DSP_TEST_H_
