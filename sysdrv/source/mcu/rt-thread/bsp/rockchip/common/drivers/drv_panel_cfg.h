/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_panel_cfg.h
  * @version V0.1
  * @brief   display panel config for pisces
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-02-20     Huang Jiachai   first implementation
  *
  ******************************************************************************
  */
#include <rtthread.h>

#ifndef __DRV_PANEL_CFG_H__
#define __DRV_PANEL_CFG_H__

#ifdef RT_USING_PANEL_GC9306_CTC28
#include "panel_cfg/gc9306_ctc28_panel_cfg.h"
#elif defined(RT_USING_PANEL_ST7789V_XF20)
#include "panel_cfg/st7789v_xf20_panel_cfg.h"
#elif defined(RT_USING_PANEL_FPGA_LVDS)
#include "panel_cfg/fpga_lvds_panel_config.h"
#elif defined(RT_USING_PANEL_SS)
#include "panel_cfg/ss_panel_cfg.h"
#elif defined(RT_USING_PANEL_S6E3FC2X01)
#include "panel_cfg/s6e3fc2x01_panel_cfg.h"
#elif defined(RT_USING_PANEL_S6E3HC2_X1)
#include "panel_cfg/s6e3hc2_x1_panel_cfg.h"
#elif defined(RT_USING_PANEL_S6E3HC2_X4)
#include "panel_cfg/s6e3hc2_x4_panel_cfg.h"
#elif defined(RT_USING_PANEL_ST7703_DS)
#include "panel_cfg/st7703_ds_panel_cfg.h"
#elif defined(RT_USING_PANEL_ST7796H2)
#include "panel_cfg/st7796h2_panel_cfg.h"
#elif defined(RT_USING_PANEL_AM018RT90211)
#include "panel_cfg/am018rt90211_panel_cfg.h"
#elif defined(RT_USING_PANEL_AM014RT90327V0)
#include "panel_cfg/am014rt90327v0_panel_cfg.h"
#elif defined(RT_USING_PANEL_H245QBN02)
#include "panel_cfg/h245qbn02_panel_cfg.h"
#endif

#if defined(RT_USING_PANEL_KGM281G44PVAA)
#include "panel_cfg/kgm281g44pvaa_panel_cfg.h"
#elif defined(RT_USING_PANEL_H20B1301A)
#include "panel_cfg/h20b1301a_panel_cfg.h"
#endif

#endif /* __DRV_PANEL_CFG_H__ */
