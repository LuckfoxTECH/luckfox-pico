/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  *
  ******************************************************************************
  * @file    es7243.h
  * @author  Xiaotan Luo
  * @version v0.1
  * @date    2019.10.16
  * @brief   The rt-thread codec driver for Rockchip
  ******************************************************************************
  */

#ifndef __ES7243_H__
#define __ES7243_H__

#define ES7243_MODECFG_REG00            0x00
#define ES7243_SDPFMT_REG01         0x01
#define ES7243_SDPFMT_REG01_ADCWL_SHIFT     2
#define ES7243_SDPFMT_REG01_ADCWL_MASK      (7 << 2)
#define ES7243_LRCDIV_REG02         0x02
#define ES7243_BCKDIV_REG03         0x03
#define ES7243_CLKDIV_REG04         0x04
#define ES7243_MUTECTL_REG05            0x05
#define ES7243_STATECTL_REG06           0x06
#define ES7243_ANACTL0_REG07            0x07
#define ES7243_ANACTL1_REG08            0x08
#define ES7243_ANACTL2_REG09            0x09
#define ES7243_ANACHARG_REG0A           0x0A
#define ES7243_INISTATE_REG0B           0x0B
#define ES7243_BIAS_REG0C           0x0C
#define ES7243_STMOSR_REG0D         0x0D
#define ES7243_CHIPID_REG0E         0x0E

#endif /* __ES7243_H__ */
