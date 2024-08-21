/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2019-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#if defined(HAL_VICAP_MODULE_ENABLED)

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup VICAP
 *  @{
 */

#ifndef _HAL_VICAP_H_
#define _HAL_VICAP_H_

#include "hal_base.h"

/***************************** MACRO Definition ******************************/
/** @defgroup VICAP_Exported_Definition_Group1 Basic Definition
 *  @{
 */

/***************************** Structure Definition **************************/

/**
 * enum all axiburst type
 */
typedef enum {
    VICAP_AXIBURST_TYPE1 = 0x0,
    VICAP_AXIBURST_TYPE2,
    VICAP_AXIBURST_TYPE3,
    VICAP_AXIBURST_TYPE4,
    VICAP_AXIBURST_TYPE5,
    VICAP_AXIBURST_TYPE6,
    VICAP_AXIBURST_TYPE7,
    VICAP_AXIBURST_TYPE8,
    VICAP_AXIBURST_TYPE9,
    VICAP_AXIBURST_TYPE10,
    VICAP_AXIBURST_TYPE11,
    VICAP_AXIBURST_TYPE12,
    VICAP_AXIBURST_TYPE13,
    VICAP_AXIBURST_TYPE14,
    VICAP_AXIBURST_TYPE15,
    VICAP_AXIBURST_TYPE16,
} eVICAP_axiBurstType;

/**
 * enum all work mode of vicap
 */
typedef enum {
    VICAP_WORKMODE_FRAME_ONEFRAME = 0x0,
    VICAP_WORKMODE_FRAME_PINGPONG,
    VICAP_WORKMODE_BLOCK_PINGPONG,
} eVICAP_workMode;

/**
 * enum all uv stored type of vicap
 */
typedef enum {
    VICAP_UV_STORE_ORDER_UVUV = 0x0,
    VICAP_UV_STORE_ORDER_VUVU,
} eVICAP_uvStoreOrder;

/**
 * enum all raw end type
 */
typedef enum {
    VICAP_RAW_END_LITTLE = 0x0,
    VICAP_RAW_END_BIG,
} eVICAP_rawEnd;

/**
 * enum yuv420 uv output in even or odd line
 */
typedef enum {
    VICAP_OUT_420_ORDER_UV_EVEN = 0x0,
    VICAP_OUT_420_ORDER_UV_ODD,
} eVICAP_out420Order;

/**
 * enum yuv output data type
 */
typedef enum {
    VICAP_OUTPUT_IS_422 = 0x0,
    VICAP_OUTPUT_IS_420,
} eVICAP_outputFormat;

/**
 * enum raw width type
 */
typedef enum {
    VICAP_RAW_WIDTH_8BIT = 0x0,
    VICAP_RAW_WIDTH_10BIT,
    VICAP_RAW_WIDTH_12BIT,
} eVICAP_rawWidth;

/**
 * enum jpeg mode
 */
typedef enum {
    VICAP_JPEG_OTHERS = 0x0,
    VICAP_JPEG_MODE_1,
} eVICAP_jpegMode;

/**
 * enum field order type
 */
typedef enum {
    VICAP_FIELD_ORDER_ODD_FIRST = 0x0,
    VICAP_FIELD_ORDER_EVEN_FIRST,
} eVICAP_fieldOrder;

/**
 * enum yuv input order type
 */
typedef enum {
    VICAP_YUV_IN_ORDER_UYVY = 0x0,
    VICAP_YUV_IN_ORDER_YVYU,
    VICAP_YUV_IN_ORDER_VYUY,
    VICAP_YUV_IN_ORDER_YUYV,
} eVICAP_yuvInOrder;

/**
 * enum vicap input mode type
 */
typedef enum {
    VICAP_INPUTMODE_YUV = 0x00,
    VICAP_INPUTMODE_PAL = 0x02,
    VICAP_INPUTMODE_NTSC,
    VICAP_INPUTMODE_RAW,
    VICAP_INPUTMODE_JPEG,
    VICAP_INPUTMODE_BT1120,
} eVICAP_inputMode;

/**
 * enum polarity of vicap href
 */
typedef enum {
    VICAP_HREF_POL_HIGH_ACTIVE = 0x0,
    VICAP_HREF_POL_LOW_ACTIVE,
} eVICAP_hrefPol;

/**
 * enum polarity of vicap vsync
 */
typedef enum {
    VICAP_VSYNC_POL_LOW_ACTIVE = 0x0,
    VICAP_VSYNC_POL_HIGH_ACTIVE,
} eVICAP_vsyncPol;

/**
 * enum path of vicap input data
 */
typedef enum {
    VICAP_PATH_SEL_RAW = 0x0,
    VICAP_PATH_SEL_YUV,
} eVICAP_pathSel;

/**
 * enum water line type of vicap
 */
typedef enum {
    VICAP_WATER_LINE_75_PERCENT = 0x0,
    VICAP_WATER_LINE_50_PERCENT,
    VICAP_WATER_LINE_25_PERCENT,
    VICAP_WATER_LINE_0_PERCENT,
} eVICAP_waterLineLvl;

/**
 * struct VICAP_WATERLINE_INFO - the configuration of vicap water line.
 */
struct VICAP_WATERLINE_INFO {
    bool enable;
    uint8_t hurryValue;
    eVICAP_waterLineLvl level;
};

/** @} */
/***************************** Function Declare ******************************/
/** @defgroup VICAP_Public_Function_Declare Public Function Declare
 *  @{
 */
HAL_Status HAL_VICAP_SetAxiBurstType(struct VICAP_REG *pReg, eVICAP_axiBurstType type);
HAL_Status HAL_VICAP_SetCaptureEnable(struct VICAP_REG *pReg, bool enable);
HAL_Status HAL_VICAP_SetWorkmode(struct VICAP_REG *pReg, eVICAP_workMode workMode);
HAL_Status HAL_VICAP_SetIrqEnable(struct VICAP_REG *pReg);
HAL_Status HAL_VICAP_SetIrqDisable(struct VICAP_REG *pReg);
uint32_t HAL_VICAP_GetIrqStatus(struct VICAP_REG *pReg);
HAL_Status HAL_VICAP_ClearIrqStatus(struct VICAP_REG *pReg, uint32_t mask);
HAL_Status HAL_VICAP_SetUvStoreOrder(struct VICAP_REG *pReg, eVICAP_uvStoreOrder order);
HAL_Status HAL_VICAP_SetRawEnd(struct VICAP_REG *pReg, eVICAP_rawEnd type);
HAL_Status HAL_VICAP_SetOut420Order(struct VICAP_REG *pReg, eVICAP_out420Order type);
HAL_Status HAL_VICAP_SetOutFormat(struct VICAP_REG *pReg, eVICAP_outputFormat type);
HAL_Status HAL_VICAP_SetYmodeOnly(struct VICAP_REG *pReg, bool enable);
HAL_Status HAL_VICAP_SetRawWidth(struct VICAP_REG *pReg, eVICAP_rawWidth width);
HAL_Status HAL_VICAP_SetJpegMode(struct VICAP_REG *pReg, eVICAP_jpegMode mode);
HAL_Status HAL_VICAP_SetFieldOrder(struct VICAP_REG *pReg, eVICAP_fieldOrder order);
HAL_Status HAL_VICAP_SetYuvInOrder(struct VICAP_REG *pReg, eVICAP_yuvInOrder order);
HAL_Status HAL_VICAP_SetInputMode(struct VICAP_REG *pReg, eVICAP_inputMode mode);
HAL_Status HAL_VICAP_SetHrefVsynPol(struct VICAP_REG *pReg, eVICAP_hrefPol hpol,
                                    eVICAP_vsyncPol vpol);
HAL_Status HAL_VICAP_SetDMAStop(struct VICAP_REG *pReg, uint32_t stat);
HAL_Status HAL_VICAP_SetFrm0YAddr(struct VICAP_REG *pReg, uint32_t yAddr);
HAL_Status HAL_VICAP_SetFrm0UvAddr(struct VICAP_REG *pReg, uint32_t uvAddr);
HAL_Status HAL_VICAP_SetFrm1YAddr(struct VICAP_REG *pReg, uint32_t yAddr);
HAL_Status HAL_VICAP_SetFrm1UvAddr(struct VICAP_REG *pReg, uint32_t uvAddr);
HAL_Status HAL_VICAP_SetBlock0YAddr(struct VICAP_REG *pReg, uint32_t yAddr);
HAL_Status HAL_VICAP_SetBlock0UvAddr(struct VICAP_REG *pReg, uint32_t uvAddr);
HAL_Status HAL_VICAP_SetBlock1YAddr(struct VICAP_REG *pReg, uint32_t yAddr);
HAL_Status HAL_VICAP_SetBlock1UvAddr(struct VICAP_REG *pReg, uint32_t uvAddr);
HAL_Status HAL_VICAP_SetVirtualLineWidth(struct VICAP_REG *pReg, uint32_t width);
uint32_t HAL_VICAP_GetVirtualLineWidth(struct VICAP_REG *pReg);
HAL_Status HAL_VICAP_SetReceivedSize(struct VICAP_REG *pReg, uint32_t height,
                                     uint32_t width);
HAL_Status HAL_VICAP_SetBlockLineNum(struct VICAP_REG *pReg, uint32_t num);
uint32_t HAL_VICAP_GetBlockLineNum(struct VICAP_REG *pReg);
uint32_t HAL_VICAP_GetBlockStatus(struct VICAP_REG *pReg);
uint32_t HAL_VICAP_ClearBlockStatus(struct VICAP_REG *pReg, uint32_t mask);
HAL_Status HAL_VICAP_SetCropOrdinate(struct VICAP_REG *pReg, uint32_t startY,
                                     uint32_t startX);
HAL_Status HAL_VICAP_SetPath(struct VICAP_REG *pReg, eVICAP_pathSel path);
HAL_Status HAL_VICAP_SetLine1IntNum(struct VICAP_REG *pReg, uint32_t num);
HAL_Status HAL_VICAP_SetLine0IntNum(struct VICAP_REG *pReg, uint32_t num);
uint32_t HAL_VICAP_GetLine1IntNum(struct VICAP_REG *pReg);
uint32_t HAL_VICAP_GetLine0IntNum(struct VICAP_REG *pReg);
HAL_Status HAL_VICAP_SetWaterLine(struct VICAP_REG *pReg,
                                  struct VICAP_WATERLINE_INFO *info);
uint32_t HAL_VICAP_GetFifoUvEntry(struct VICAP_REG *pReg);
uint32_t HAL_VICAP_GetFifoYEntry(struct VICAP_REG *pReg);
uint32_t HAL_VICAP_GetFrameStatus(struct VICAP_REG *pReg);
uint32_t HAL_VICAP_ClearFrameStatus(struct VICAP_REG *pReg, uint32_t mask);
uint32_t HAL_VICAP_GetCurDstAddr(struct VICAP_REG *pReg);
uint32_t HAL_VICAP_GetLastFrameUvLineNum(struct VICAP_REG *pReg);
uint32_t HAL_VICAP_GetLastFrameYLineNum(struct VICAP_REG *pReg);
uint32_t HAL_VICAP_GetLastLineUvNum(struct VICAP_REG *pReg);
uint32_t HAL_VICAP_GetLastLineYNum(struct VICAP_REG *pReg);
uint32_t HAL_VICAP_GetDMAIdleStatus(struct VICAP_REG *pReg);
HAL_Status HAL_VICAP_IrqHandler(struct VICAP_REG *pReg);
HAL_Status HAL_VICAP_Init(struct VICAP_REG *pReg);
HAL_Status HAL_VICAP_DeInit(struct VICAP_REG *pReg);

/** @} */

#endif

/** @} */

/** @} */

#endif /* HAL_VICAP_MODULE_ENABLED */
