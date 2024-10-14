/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#if defined(HAL_VOP_MODULE_ENABLED) || defined(HAL_DSI_MODULE_ENABLED)

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup DISPLAY
 *  @{
 */

#ifndef _HAL_DISPLAY_H_
#define _HAL_DISPLAY_H_

/***************************** MACRO Definition ******************************/
/** @defgroup DISPLAY_Exported_Definition_Group1 Basic Definition
 *  @{
 */

/** RK display connector type */
#define RK_DISPLAY_CONNECTOR_UNKNOWN 0
#define RK_DISPLAY_CONNECTOR_RGB     1
#define RK_DISPLAY_CONNECTOR_DSI     2

/** video mode flag default polarity is positive */
#define VIDEO_MODE_FLAG_NHSYNC   HAL_BIT(0)
#define VIDEO_MODE_FLAG_NVSYNC   HAL_BIT(1)
#define VIDEO_MODE_FLAG_NPIXDATA HAL_BIT(2)
#define VIDEO_MODE_FLAG_DEN      HAL_BIT(3)
/** video mode */
#define DSI_MODE_VIDEO HAL_BIT(4)
/** video burst mode */
#define DSI_MODE_VIDEO_BURST HAL_BIT(5)
/** video pulse mode */
#define DSI_MODE_VIDEO_SYNC_PULSE HAL_BIT(6)
/** enable auto vertical count mode */
#define DSI_MODE_VIDEO_AUTO_VERT HAL_BIT(7)
/** disable EoT packets in HS mode */
#define DSI_MODE_EOT_PACKET HAL_BIT(8)
/** device supports non-continuous clock behavior (DSI spec 5.6.1) */
#define DSI_CLOCK_NON_CONTINUOUS HAL_BIT(9)
/** transmit data in low power */
#define DSI_MODE_LPM HAL_BIT(10)
#define DSC_ENABLE   HAL_BIT(11)

/** media bus format from DRM define */
#define MEDIA_BUS_FMT_RGB565_1X16        0x1017
#define MEDIA_BUS_FMT_BGR565_2X8_BE      0x1005
#define MEDIA_BUS_FMT_BGR565_2X8_LE      0x1006
#define MEDIA_BUS_FMT_RGB565_2X8_BE      0x1007
#define MEDIA_BUS_FMT_RGB565_2X8_LE      0x1008
#define MEDIA_BUS_FMT_RGB666_1X18        0x1009
#define MEDIA_BUS_FMT_SRGB666_3X8        0x101b
#define MEDIA_BUS_FMT_RBG888_1X24        0x100e
#define MEDIA_BUS_FMT_RGB666_1X24_CPADHI 0x1015
#define MEDIA_BUS_FMT_RGB666_1X7X3_SPWG  0x1010
#define MEDIA_BUS_FMT_BGR888_1X24        0x1013
#define MEDIA_BUS_FMT_GBR888_1X24        0x1014
#define MEDIA_BUS_FMT_RGB888_1X24        0x100a
#define MEDIA_BUS_FMT_RGB888_2X12_BE     0x100b
#define MEDIA_BUS_FMT_RGB888_2X12_LE     0x100c
#define MEDIA_BUS_FMT_RGB888_1X7X4_SPWG  0x1011
#define MEDIA_BUS_FMT_RGB888_1X7X4_JEIDA 0x1012
#define MEDIA_BUS_FMT_ARGB8888_1X32      0x100d
#define MEDIA_BUS_FMT_RGB888_1X32_PADHI  0x100f
#define MEDIA_BUS_FMT_SRGB888_3X8        0x101c
#define MEDIA_BUS_FMT_SBGR888_3X8        0x101d
#define MEDIA_BUS_FMT_SRBG888_3X8        0x101e
#define MEDIA_BUS_FMT_SRGB888_DUMMY_4X8  0x101f
#define MEDIA_BUS_FMT_SBGR888_DUMMY_4X8  0x1020
#define MEDIA_BUS_FMT_SRBG888_DUMMY_4X8  0x1021

/***************************** Structure Definition **************************/

/**
 *  Add multi line brief to here
 *  ...
 */
struct DISPLAY_MODE_INFO {
    uint32_t crtcClock;
    uint16_t crtcHdisplay;
    uint16_t crtcHsyncStart;
    uint16_t crtcHsyncEnd;
    uint16_t crtcHtotal;
    uint16_t crtcVdisplay;
    uint16_t crtcVsyncStart;
    uint16_t crtcVsyncEnd;
    uint16_t crtcVtotal;
    uint16_t flags;
    uint16_t mcuCsRw;
    uint16_t mcuRwPw;
    uint16_t mcuRwCs;
};

/**
 * struct DISPLAY_RECT
 */
struct DISPLAY_RECT {
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;
};

/** @} */
/***************************** Function Declare ******************************/
/** @defgroup DISPLAY_Public_Function_Declare Public Function Declare
 *  @{
 */

/** @} */

#endif

/** @} */

/** @} */

#endif /* HAL_VOP_MODULE_ENABLED || HAL_DSI_MODULE_ENABLED */
