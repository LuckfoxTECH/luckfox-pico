/**
  * Copyright (c) 2022 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @version V0.0.1
  *
  * Change Logs:
  * Date           Author          Notes
  * 2022-07-07     ISP Team      first implementation
  *
  ******************************************************************************
  */

#ifndef _FAST_AWB_H_
#define _FAST_AWB_H_

#include "../../common/drivers/isp3/drv_isp_module.h"

int int_wbgain(struct isp_awbgain_cfg  *wbgain);
int calc_wbgain(struct isp_awbgain_cfg  *wbgain, const struct isp_rawawb_stat *awb_stat);

#endif