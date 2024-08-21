/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_DSI_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup DSI
 *  @{
 */

#ifndef _HAL_DSI_H_
#define _HAL_DSI_H_

/***************************** MACRO Definition ******************************/
/** @defgroup DSI_Exported_Definition_Group1 Basic Definition
 *  @{
 */

/***************************** Structure Definition **************************/

typedef enum {
    MIPI_DSI_DCS_COMPRESSION_MODE        = 0x07,
    MIPI_DSI_GENERIC_SHORT_WRITE_0_PARAM = 0x03,
    MIPI_DSI_GENERIC_SHORT_WRITE_1_PARAM = 0x13,
    MIPI_DSI_GENERIC_SHORT_WRITE_2_PARAM = 0x23,
    MIPI_DSI_DCS_SHORT_WRITE             = 0x05,
    MIPI_DSI_DCS_SHORT_WRITE_PARAM       = 0x15,
    MIPI_DSI_NULL_PACKET                 = 0x09,
    MIPI_DSI_BLANKING_PACKET             = 0x19,
    MIPI_DSI_GENERIC_LONG_WRITE          = 0x29,
    MIPI_DSI_DCS_LONG_WRITE              = 0x39,
    MIPI_DSI_PPS_LONG_WRITE              = 0x0A,
} eDSI_DataType;

typedef enum {
    MIPI_DCS_NOP                    = 0x00,
    MIPI_DCS_SOFT_RESET             = 0x01,
    MIPI_DCS_GET_DISPLAY_STATUS     = 0x09,
    MIPI_DCS_GET_POWER_MODE         = 0x0A,
    MIPI_DCS_GET_ADDRESS_MODE       = 0x0B,
    MIPI_DCS_GET_PIXEL_FORMAT       = 0x0C,
    MIPI_DCS_GET_DISPLAY_MODE       = 0x0D,
    MIPI_DCS_GET_SIGNAL_MODE        = 0x0E,
    MIPI_DCS_ENTER_SLEEP_MODE       = 0x10,
    MIPI_DCS_EXIT_SLEEP_MODE        = 0x11,
    MIPI_DCS_ENTER_PARTIAL_MODE     = 0x12,
    MIPI_DCS_ENTER_NORMAL_MODE      = 0x13,
    MIPI_DCS_SET_GAMMA_CURVE        = 0x26,
    MIPI_DCS_SET_DISPLAY_OFF        = 0x28,
    MIPI_DCS_SET_DISPLAY_ON         = 0x29,
    MIPI_DCS_SET_COLUMN_ADDRESS     = 0x2A,
    MIPI_DCS_SET_PAGE_ADDRESS       = 0x2B,
    MIPI_DCS_WRITE_MEMORY_START     = 0x2C,
    MIPI_DCS_WRITE_LUT              = 0x2D,
    MIPI_DCS_READ_MEMORY_START      = 0x2E,
    MIPI_DCS_SET_PARTIAL_ROWS       = 0x30,
    MIPI_DCS_SET_PARTIAL_COLUMNS    = 0x31,
    MIPI_DCS_SET_SCROLL_AREA        = 0x33,
    MIPI_DCS_SET_TEAR_OFF           = 0x34,
    MIPI_DCS_SET_TEAR_ON            = 0x35,
    MIPI_DCS_SET_ADDRESS_MODE       = 0x36,
    MIPI_DCS_SET_SCROLL_START       = 0x37,
    MIPI_DCS_EXIT_IDLE_MODE         = 0x38,
    MIPI_DCS_ENTER_IDLE_MODE        = 0x39,
    MIPI_DCS_SET_PIXEL_FORMAT       = 0x3A,
    MIPI_DCS_SET_TEAR_SCANLINE      = 0x44,
    MIPI_DCS_SET_DISPLAY_BRIGHTNESS = 0x51,
} eDSI_DcsCommand;

typedef enum {
    MIPI_DSI_DCS_TEAR_MODE_VBLANK,
    MIPI_DSI_DCS_TEAR_MODE_VHBLANK,
} eDSI_DcsTearMode;

typedef enum {
    DPHY_REF_CLK_IN_6000000,
    DPHY_REF_CLK_IN_12000000,
    DPHY_REF_CLK_IN_19200000,
    DPHY_REF_CLK_IN_25000000,
    DPHY_REF_CLK_IN_26000000,
    DPHY_REF_CLK_IN_27000000,
    DPHY_REF_CLK_IN_38400000,
    DPHY_REF_CLK_IN_40000000,
} eDSI_DphyRefClk;

/** @} */
/***************************** Function Declare ******************************/
/** @defgroup DSI_Public_Function_Declare Public Function Declare
 *  @{
 */
HAL_Status HAL_DSI_IrqEnable(struct DSI_REG *pReg);
HAL_Status HAL_DSI_IrqDisable(struct DSI_REG *pReg);
HAL_Status HAL_DSI_IrqHandler(struct DSI_REG *pReg);
HAL_Status HAL_DSI_M31DphyPowerUp(struct DSI_REG *pReg);
HAL_Status HAL_DSI_M31DphyPowerDown(struct DSI_REG *pReg);
uint16_t HAL_DSI_M31DphyInit(struct DSI_REG *pReg, uint16_t laneMbps,
                             eDSI_DphyRefClk refClkIn);
HAL_Status HAL_DSI_DphyTimingConfig(struct DSI_REG *pReg,
                                    struct DISPLAY_MODE_INFO *pModeInfo, uint8_t lanes);
HAL_Status HAL_DSI_Init(struct DSI_REG *pReg, uint16_t laneMbps);
HAL_Status HAL_DSI_MsgHsModeConfig(struct DSI_REG *pReg);
HAL_Status HAL_DSI_MsgLpModeConfig(struct DSI_REG *pReg);
HAL_Status HAL_DSI_Enable(struct DSI_REG *pReg,
                          struct DISPLAY_MODE_INFO *pModeInfo);
HAL_Status HAL_DSI_Disable(struct DSI_REG *pReg,
                           struct DISPLAY_MODE_INFO *pModeInfo);

HAL_Status HAL_DSI_SendPacket(struct DSI_REG *pReg, uint8_t dataType,
                              uint8_t payloadLen, const uint8_t *payload);
HAL_Status HAL_DSI_ModeConfig(struct DSI_REG *pReg,
                              struct DISPLAY_MODE_INFO *pModeInfo);
HAL_Status HAL_DSI_PacketHandlerConfig(struct DSI_REG *pReg,
                                       struct DISPLAY_MODE_INFO *pModeInfo);
HAL_Status HAL_DSI_VerticalTimingConfig(struct DSI_REG *pReg,
                                        struct DISPLAY_MODE_INFO *pModeInfo);
HAL_Status HAL_DSI_DpiConfig(struct DSI_REG *pReg,
                             struct DISPLAY_MODE_INFO *pModeInfo,
                             uint16_t busFormat);
HAL_Status HAL_DSI_LineTimerConfig(struct DSI_REG *pReg,
                                   uint16_t laneMbps,
                                   struct DISPLAY_MODE_INFO *pModeInfo);
HAL_Status HAL_DSI_UpdateLineTimer(struct DSI_REG *pReg,
                                   uint16_t laneMbps,
                                   struct DISPLAY_MODE_INFO *pModeInfo,
                                   struct DISPLAY_RECT *pDisplayRect);

/** @} */

#endif

/** @} */

/** @} */

#endif /* HAL_DSI_MODULE_ENABLED */
