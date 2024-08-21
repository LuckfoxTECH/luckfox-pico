/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2019-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_VICAP_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup VICAP
 *  @{
 */

/** @defgroup VICAP_How_To_Use How To Use
 *  @{

 The VICAP hal driver interfaces can be called directly by os.

 @} */

/** @defgroup VICAP_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/
#define VICAP_DVP_WATER_LINE_HURRY_VAL_MAX (0x3)

/********************* Private Structure Definition **************************/

/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/

/** @} */
/********************* Public Function Definition ****************************/

/** @defgroup VICAP_Exported_Functions_Group2 State and Errors Functions
 @verbatim

 This section provides functions allowing to get state of the module:

 @endverbatim
 *  @{
 */

/**
 * @brief  Get the actived irq of vicap in running.
 * @param  pReg: VICAP reg base.
 * @return the value of interrupt status register.
 * Get VICAP irq has happened.
 */
uint32_t HAL_VICAP_GetIrqStatus(struct VICAP_REG *pReg)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    return READ_REG(pReg->DVP_INTSTAT);
}

/**
* @brief  Get vicap virtual line width.
* @param  pReg: VICAP reg base.
* @return the width in vir_line_width register.
* Get vicap virtual line width.
*/
uint32_t HAL_VICAP_GetVirtualLineWidth(struct VICAP_REG *pReg)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    return READ_REG(pReg->DVP_VIR_LINE_WIDTH);
}

/**
* @brief  Get vicap block line num.
* @param  pReg: VICAP reg base.
* @return block line num.
* Get vicap block line num.
*/
uint32_t HAL_VICAP_GetBlockLineNum(struct VICAP_REG *pReg)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    return READ_REG(pReg->DVP_BLOCK_LINE_NUM);
}

/**
* @brief  Get vicap block status.
* @param  pReg: VICAP reg base.
* @return block status.
* Get vicap block status.
*/
uint32_t HAL_VICAP_GetBlockStatus(struct VICAP_REG *pReg)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    return READ_REG(pReg->DVP_BLOCK_STATUS);
}

/**
* @brief  Get vicap line1 interrupt num.
* @param  pReg: VICAP reg base.
* @return interrupt num.
* Get vicap line1 interrupt num.
*/
uint32_t HAL_VICAP_GetLine1IntNum(struct VICAP_REG *pReg)
{
    uint32_t regVal;

    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    regVal = READ_REG(pReg->DVP_LINE_INT_NUM);
    regVal &= VICAP_DVP_LINE_INT_NUM_LINE1_INT_NUM_MASK;
    regVal >>= VICAP_DVP_LINE_INT_NUM_LINE1_INT_NUM_SHIFT;

    return regVal;
}

/**
* @brief  Get vicap line0 interrupt num.
* @param  pReg: VICAP reg base.
* @return interrupt num.
* Get vicap line0 interrupt num.
*/
uint32_t HAL_VICAP_GetLine0IntNum(struct VICAP_REG *pReg)
{
    uint32_t regVal;

    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    regVal = READ_REG(pReg->DVP_LINE_INT_NUM);
    regVal &= VICAP_DVP_LINE_INT_NUM_LINE0_INT_NUM_MASK;
    regVal >>= VICAP_DVP_LINE_INT_NUM_LINE0_INT_NUM_SHIFT;

    return regVal;
}

/**
* @brief  Get vicap fifo uv entry.
* @param  pReg: VICAP reg base.
* @return the value of uv fifo entry
* Get vicap fifo uv entry
*/
uint32_t HAL_VICAP_GetFifoUvEntry(struct VICAP_REG *pReg)
{
    uint32_t regVal;

    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    regVal = READ_REG(pReg->DVP_FIFO_ENTRY);
    regVal &= VICAP_DVP_FIFO_ENTRY_UV_FIFO_ENTRY_MASK;
    regVal >>= VICAP_DVP_FIFO_ENTRY_UV_FIFO_ENTRY_SHIFT;

    return regVal;
}

/**
* @brief  Get vicap fifo y entry.
* @param  pReg: VICAP reg base.
* @return the value of y fifo entry
* Get vicap fifo y entry
*/
uint32_t HAL_VICAP_GetFifoYEntry(struct VICAP_REG *pReg)
{
    uint32_t regVal;

    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    regVal = READ_REG(pReg->DVP_FIFO_ENTRY);
    regVal &= VICAP_DVP_FIFO_ENTRY_Y_FIFO_ENTRY_MASK;
    regVal >>= VICAP_DVP_FIFO_ENTRY_Y_FIFO_ENTRY_SHIFT;

    return regVal;
}

/**
* @brief  Get vicap fifo y entry.
* @param  pReg: VICAP reg base.
* @return the value of y frame status
* Get vicap frame status
*/
uint32_t HAL_VICAP_GetFrameStatus(struct VICAP_REG *pReg)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    return READ_REG(pReg->DVP_FRAME_STATUS);
}

/**
* @brief  Get vicap dvp current destination address.
* @param  pReg: VICAP reg base.
* @return the value of current destination address
* Get vicap dvp current destination address
*/
uint32_t HAL_VICAP_GetCurDstAddr(struct VICAP_REG *pReg)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    return READ_REG(pReg->DVP_CUR_DST);
}

/**
* @brief  Get vicap last frame uv line number
* @param  pReg: VICAP reg base.
* @return the value of last frame uv line number
* Get UV line number of last frame,only for bt1120 mode
*/
uint32_t HAL_VICAP_GetLastFrameUvLineNum(struct VICAP_REG *pReg)
{
    uint32_t regVal;

    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    regVal = READ_REG(pReg->DVP_LAST_LINE);
    regVal &= VICAP_DVP_LAST_LINE_LAST_UV_NUM_MASK;
    regVal >>= VICAP_DVP_LAST_LINE_LAST_UV_NUM_SHIFT;

    return regVal;
}

/**
* @brief  Get vicap last frame y line number
* @param  pReg: VICAP reg base.
* @return the value of last frame y line number
* Get y line number of last frame
*/
uint32_t HAL_VICAP_GetLastFrameYLineNum(struct VICAP_REG *pReg)
{
    uint32_t regVal;

    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    regVal = READ_REG(pReg->DVP_LAST_LINE);
    regVal &= VICAP_DVP_LAST_LINE_LAST_Y_NUM_MASK;
    regVal >>= VICAP_DVP_LAST_LINE_LAST_Y_NUM_SHIFT;

    return regVal;
}

/**
* @brief  Get vicap last line uv number
* @param  pReg: VICAP reg base.
* @return the value of last line uv number
* Get UV number of last line
*/
uint32_t HAL_VICAP_GetLastLineUvNum(struct VICAP_REG *pReg)
{
    uint32_t regVal;

    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    regVal = READ_REG(pReg->DVP_LAST_PIX);
    regVal &= VICAP_DVP_LAST_PIX_LAST_UV_NUM_MASK;
    regVal >>= VICAP_DVP_LAST_PIX_LAST_UV_NUM_SHIFT;

    return regVal;
}

/**
* @brief  Get vicap last line y number
* @param  pReg: VICAP reg base.
* @return the value of last line y number
* Get y number of last line
*/
uint32_t HAL_VICAP_GetLastLineYNum(struct VICAP_REG *pReg)
{
    uint32_t regVal;

    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    regVal = READ_REG(pReg->DVP_LAST_PIX);
    regVal &= VICAP_DVP_LAST_LINE_LAST_Y_NUM_MASK;
    regVal >>= VICAP_DVP_LAST_LINE_LAST_Y_NUM_SHIFT;

    return regVal;
}

/**
* @brief  Get vicap dma idle status
* @param  pReg: VICAP reg base.
* @return the value of dma idle status
*/
uint32_t HAL_VICAP_GetDMAIdleStatus(struct VICAP_REG *pReg)
{
    uint32_t regVal;

    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    regVal = READ_REG(pReg->DVP_DMA_IDLE_REQ);
    regVal &= VICAP_DVP_DMA_IDLE_REQ_DMA_IDLE_REQ_MASK;
    regVal >>= VICAP_DVP_DMA_IDLE_REQ_DMA_IDLE_REQ_SHIFT;

    return regVal;
}

/** @} */

/** @defgroup VICAP_Exported_Functions_Group3 IO Functions
 @verbatim

 This section provides functions allowing to IO controlling:

 @endverbatim
 *  @{
 */

/**
 * @brief  Set VICAP axi burst type.
 * @param  pReg: VICAP reg base.
 * @param  type: axi burst type
 * @return HAL_Status.
 * Just set VICAP axi burst type.
 */
HAL_Status HAL_VICAP_SetAxiBurstType(struct VICAP_REG *pReg,
                                     eVICAP_axiBurstType type)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    MODIFY_REG(pReg->DVP_CTRL, VICAP_DVP_CTRL_AXI_BURST_TYPE_MASK,
               type << VICAP_DVP_CTRL_AXI_BURST_TYPE_SHIFT);

    return HAL_OK;
}

/**
 * @brief  Set VICAP capture enable bit.
 * @param  pReg: VICAP reg base.
 * @param  enable: decide to set enable status
 * @return HAL_Status.
 * Enable or disable VICAP controller.
 */
HAL_Status HAL_VICAP_SetCaptureEnable(struct VICAP_REG *pReg, bool enable)
{
    uint32_t regVal;

    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    regVal = READ_REG(pReg->DVP_CTRL);
    if (enable) {
        regVal |= VICAP_DVP_CTRL_CAP_EN_MASK;
    } else {
        regVal &= ~VICAP_DVP_CTRL_CAP_EN_MASK;
    }
    WRITE_REG(pReg->DVP_CTRL, regVal);

    return HAL_OK;
}

/**
 * @brief  Set VICAP work enable bit.
 * @param  pReg: VICAP reg base.
 * @param  workMode: decide which work mode to be selcected
 * @return HAL_Status.
 * Select vicap work mode.
 */
HAL_Status HAL_VICAP_SetWorkmode(struct VICAP_REG *pReg, eVICAP_workMode workMode)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    MODIFY_REG(pReg->DVP_CTRL, VICAP_DVP_CTRL_WORK_MODE_MASK,
               workMode << VICAP_DVP_CTRL_WORK_MODE_SHIFT);

    return HAL_OK;
}

/**
 * @brief  Select uv store order.
 * @param  pReg: VICAP reg base.
 * @param  order: the uv store order type
 * @return HAL_Status.
 * Select uv store order.
 */
HAL_Status HAL_VICAP_SetUvStoreOrder(struct VICAP_REG *pReg,
                                     eVICAP_uvStoreOrder order)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    MODIFY_REG(pReg->DVP_FOR, VICAP_DVP_FOR_UV_STORE_ORDER_MASK,
               order << VICAP_DVP_FOR_UV_STORE_ORDER_SHIFT);

    return HAL_OK;
}

/**
 * @brief  Select raw end type.
 * @param  pReg: VICAP reg base.
 * @param  type: the raw end type
 * @return HAL_Status.
 * Select raw end type.
 */
HAL_Status HAL_VICAP_SetRawEnd(struct VICAP_REG *pReg, eVICAP_rawEnd type)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    MODIFY_REG(pReg->DVP_FOR, VICAP_DVP_FOR_RAW_END_MASK,
               type << VICAP_DVP_FOR_RAW_END_SHIFT);

    return HAL_OK;
}

/**
 * @brief  Select yuv420 output order.
 * @param  pReg: VICAP reg base.
 * @param  type: the order type
 * @return HAL_Status.
 * Select yuv420 output order.
 */
HAL_Status HAL_VICAP_SetOut420Order(struct VICAP_REG *pReg, eVICAP_out420Order type)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    MODIFY_REG(pReg->DVP_FOR, VICAP_DVP_FOR_OUT_420_ORDER_MASK,
               type << VICAP_DVP_FOR_OUT_420_ORDER_SHIFT);

    return HAL_OK;
}

/**
 * @brief  Select vicap output format.
 * @param  pReg: VICAP reg base.
 * @param  type: the format type
 * @return HAL_Status.
 * Select vicap output format.
 */
HAL_Status HAL_VICAP_SetOutFormat(struct VICAP_REG *pReg, eVICAP_outputFormat type)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    MODIFY_REG(pReg->DVP_FOR, VICAP_DVP_FOR_OUTPUT_420_MASK,
               type << VICAP_DVP_FOR_OUTPUT_420_SHIFT);

    return HAL_OK;
}

/**
 * @brief  Select vicap only receive y data for bt601.
 * @param  pReg: VICAP reg base.
 * @param  enable: whether to enable only_y_mode
 * @return HAL_Status.
 * Select vicap only receive y data for bt601.
 */
HAL_Status HAL_VICAP_SetYmodeOnly(struct VICAP_REG *pReg, bool enable)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    MODIFY_REG(pReg->DVP_FOR, VICAP_DVP_FOR_ONLY_Y_MODE_MASK,
               (uint32_t)enable << VICAP_DVP_FOR_ONLY_Y_MODE_SHIFT);

    return HAL_OK;
}

/**
 * @brief  Select vicap raw width.
 * @param  pReg: VICAP reg base.
 * @param  width: the width of raw type
 * @return HAL_Status.
 * Select vicap raw width.
 */
HAL_Status HAL_VICAP_SetRawWidth(struct VICAP_REG *pReg, eVICAP_rawWidth width)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    MODIFY_REG(pReg->DVP_FOR, VICAP_DVP_FOR_RAW_WIDTH_MASK,
               width << VICAP_DVP_FOR_RAW_WIDTH_SHIFT);

    return HAL_OK;
}

/**
 * @brief  Select vicap jpeg mode.
 * @param  pReg: VICAP reg base.
 * @param  mode: the mode of jpeg
 * @return HAL_Status.
 * Select vicap jpeg mode.
 */
HAL_Status HAL_VICAP_SetJpegMode(struct VICAP_REG *pReg, eVICAP_jpegMode mode)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    MODIFY_REG(pReg->DVP_FOR, VICAP_DVP_FOR_JPEG_MODE_MASK,
               mode << VICAP_DVP_FOR_JPEG_MODE_SHIFT);

    return HAL_OK;
}

/**
 * @brief  Select vicap field order.
 * @param  pReg: VICAP reg base.
 * @param  order: the order of vicap field
 * @return HAL_Status.
 * Select vicap field order.
 */
HAL_Status HAL_VICAP_SetFieldOrder(struct VICAP_REG *pReg, eVICAP_fieldOrder order)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    MODIFY_REG(pReg->DVP_FOR, VICAP_DVP_FOR_FIELD_ORDER_MASK,
               order << VICAP_DVP_FOR_FIELD_ORDER_SHIFT);

    return HAL_OK;
}

/**
 * @brief  Select vicap yuv input order.
 * @param  pReg: VICAP reg base.
 * @param  order: the order of vicap yuv input
 * @return HAL_Status.
 * Select vicap yuv input order.
 */
HAL_Status HAL_VICAP_SetYuvInOrder(struct VICAP_REG *pReg, eVICAP_yuvInOrder order)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    MODIFY_REG(pReg->DVP_FOR, VICAP_DVP_FOR_YUV_IN_ORDER_MASK,
               order << VICAP_DVP_FOR_YUV_IN_ORDER_SHIFT);

    return HAL_OK;
}

/**
 * @brief  Select vicap input mode.
 * @param  pReg: VICAP reg base.
 * @param  mode: the mode of vicap input
 * @return HAL_Status.
 * Select vicap input mode.
 */
HAL_Status HAL_VICAP_SetInputMode(struct VICAP_REG *pReg, eVICAP_inputMode mode)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    MODIFY_REG(pReg->DVP_FOR, VICAP_DVP_FOR_INPUT_MODE_MASK,
               mode << VICAP_DVP_FOR_INPUT_MODE_SHIFT);

    return HAL_OK;
}

/**
 * @brief  Select vicap polarity of href and vsync.
 * @param  pReg: VICAP reg base.
 * @param  hpol: the polarity of vicap href
 * @param  vpol: the polarity of vicap vsync
 * @return HAL_Status.
 * Select vicap polarity of href and vsync.
 */
HAL_Status HAL_VICAP_SetHrefVsynPol(struct VICAP_REG *pReg, eVICAP_hrefPol hpol,
                                    eVICAP_vsyncPol vpol)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    MODIFY_REG(pReg->DVP_FOR,
               VICAP_DVP_FOR_HREF_POL_MASK | VICAP_DVP_FOR_VSYNC_POL_MASK,
               hpol << VICAP_DVP_FOR_HREF_POL_SHIFT |
               vpol << VICAP_DVP_FOR_VSYNC_POL_SHIFT);

    return HAL_OK;
}

/**
* @brief  Set vicap dma to stop or not.
* @param  pReg: VICAP reg base.
* @param  stat: decides wheter vicap dma stops or not
* @return HAL_Status.
* Set vicap dma to stop or not.
*/
HAL_Status HAL_VICAP_SetDMAStop(struct VICAP_REG *pReg, uint32_t stat)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    MODIFY_REG(pReg->DVP_DMA_IDLE_REQ, VICAP_DVP_DMA_IDLE_REQ_DMA_IDLE_REQ_MASK,
               stat << VICAP_DVP_DMA_IDLE_REQ_DMA_IDLE_REQ_SHIFT);

    return HAL_OK;
}

/**
* @brief  Set vicap frame0 y addr.
* @param  pReg: VICAP reg base.
* @param  yAddr: the y address of yuv
* @return HAL_Status.
* Set vicap frame0 y addr.
*/
HAL_Status HAL_VICAP_SetFrm0YAddr(struct VICAP_REG *pReg, uint32_t yAddr)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));
    HAL_ASSERT(yAddr);

    MODIFY_REG(pReg->DVP_FRM0_ADDR_Y, VICAP_DVP_FRM0_ADDR_Y_FRM0_ADDR_Y_MASK,
               yAddr << VICAP_DVP_FRM0_ADDR_Y_FRM0_ADDR_Y_SHIFT);

    return HAL_OK;
}

/**
* @brief  Set vicap frame0 uv addr.
* @param  pReg: VICAP reg base.
* @param  uvAddr: the uv address of yuv
* @return HAL_Status.
* Set vicap frame0 uv addr.
*/
HAL_Status HAL_VICAP_SetFrm0UvAddr(struct VICAP_REG *pReg, uint32_t uvAddr)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));
    HAL_ASSERT(uvAddr);

    MODIFY_REG(pReg->DVP_FRM0_ADDR_UV, VICAP_DVP_FRM0_ADDR_UV_FRM0_ADDR_UV_MASK,
               uvAddr << VICAP_DVP_FRM0_ADDR_UV_FRM0_ADDR_UV_SHIFT);

    return HAL_OK;
}

/**
* @brief  Set vicap frame1 y addr.
* @param  pReg: VICAP reg base.
* @param  yAddr: the y address of yuv
* @return HAL_Status.
* Set vicap frame1 y addr.
*/
HAL_Status HAL_VICAP_SetFrm1YAddr(struct VICAP_REG *pReg, uint32_t yAddr)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));
    HAL_ASSERT(yAddr);

    MODIFY_REG(pReg->DVP_FRM1_ADDR_Y, VICAP_DVP_FRM1_ADDR_Y_FRM1_ADDR_Y_MASK,
               yAddr << VICAP_DVP_FRM1_ADDR_Y_FRM1_ADDR_Y_SHIFT);

    return HAL_OK;
}

/**
* @brief  Set vicap frame1 uv addr.
* @param  pReg: VICAP reg base.
* @param  uvAddr: the uv address of yuv
* @return HAL_Status.
* Set vicap frame1 uv addr.
*/
HAL_Status HAL_VICAP_SetFrm1UvAddr(struct VICAP_REG *pReg, uint32_t uvAddr)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));
    HAL_ASSERT(uvAddr);

    MODIFY_REG(pReg->DVP_FRM1_ADDR_UV, VICAP_DVP_FRM1_ADDR_UV_FRM1_ADDR_UV_MASK,
               uvAddr << VICAP_DVP_FRM1_ADDR_UV_FRM1_ADDR_UV_SHIFT);

    return HAL_OK;
}

/**
* @brief  Set vicap block0 y addr.
* @param  pReg: VICAP reg base.
* @param  yAddr: the y address of yuv
* @return HAL_Status.
* Set vicap block0 y addr.
*/
HAL_Status HAL_VICAP_SetBlock0YAddr(struct VICAP_REG *pReg, uint32_t yAddr)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));
    HAL_ASSERT(yAddr);

    MODIFY_REG(pReg->DVP_BLOCK0_ADDR_Y, VICAP_DVP_BLOCK0_ADDR_Y_BLOCK0_ADDR_Y_MASK,
               yAddr << VICAP_DVP_BLOCK0_ADDR_Y_BLOCK0_ADDR_Y_SHIFT);

    return HAL_OK;
}

/**
* @brief  Set vicap block0 uv addr.
* @param  pReg: VICAP reg base.
* @param  uvAddr: the uv address of yuv
* @return HAL_Status.
* Set vicap block0 uv addr.
*/
HAL_Status HAL_VICAP_SetBlock0UvAddr(struct VICAP_REG *pReg, uint32_t uvAddr)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));
    HAL_ASSERT(uvAddr);

    MODIFY_REG(pReg->DVP_BLOCK0_ADDR_UV, VICAP_DVP_BLOCK0_ADDR_UV_BLOCK0_ADDR_UV_MASK,
               uvAddr << VICAP_DVP_BLOCK0_ADDR_UV_BLOCK0_ADDR_UV_SHIFT);

    return HAL_OK;
}

/**
* @brief  Set vicap block1 y addr.
* @param  pReg: VICAP reg base.
* @param  yAddr: the y address of yuv
* @return HAL_Status.
* Set vicap block1 y addr.
*/
HAL_Status HAL_VICAP_SetBlock1YAddr(struct VICAP_REG *pReg, uint32_t yAddr)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));
    HAL_ASSERT(yAddr);

    MODIFY_REG(pReg->DVP_BLOCK1_ADDR_Y, VICAP_DVP_BLOCK1_ADDR_Y_BLOCK1_ADDR_Y_MASK,
               yAddr << VICAP_DVP_BLOCK1_ADDR_Y_BLOCK1_ADDR_Y_SHIFT);

    return HAL_OK;
}

/**
* @brief  Set vicap block0 uv addr.
* @param  pReg: VICAP reg base.
* @param  uvAddr: the uv address of yuv
* @return HAL_Status.
* Set vicap block1 uv addr.
*/
HAL_Status HAL_VICAP_SetBlock1UvAddr(struct VICAP_REG *pReg, uint32_t uvAddr)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));
    HAL_ASSERT(uvAddr);

    MODIFY_REG(pReg->DVP_BLOCK1_ADDR_UV, VICAP_DVP_BLOCK1_ADDR_UV_BLOCK1_ADDR_UV_MASK,
               uvAddr << VICAP_DVP_BLOCK1_ADDR_UV_BLOCK1_ADDR_UV_SHIFT);

    return HAL_OK;
}

/**
* @brief  Set vicap virtual line width.
* @param  pReg: VICAP reg base.
* @param  width: the width of virtual line
* @return HAL_Status.
* Set vicap virtual line width.
*/
HAL_Status HAL_VICAP_SetVirtualLineWidth(struct VICAP_REG *pReg, uint32_t width)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    MODIFY_REG(pReg->DVP_VIR_LINE_WIDTH, VICAP_DVP_VIR_LINE_WIDTH_VIR_LINE_WIDTH_MASK,
               width << VICAP_DVP_VIR_LINE_WIDTH_VIR_LINE_WIDTH_SHIFT);

    return HAL_OK;
}

/**
* @brief  Set vicap size of window.
* @param  pReg: VICAP reg base.
* @param  height: the height size of received image
* @param  width: the width size of received image
* @return HAL_Status.
* Set the expected height and width of received image.
*/
HAL_Status HAL_VICAP_SetReceivedSize(struct VICAP_REG *pReg, uint32_t height,
                                     uint32_t width)
{
    MODIFY_REG(pReg->DVP_SET_SIZE,
               VICAP_DVP_SET_SIZE_SET_HEIGHT_MASK |
               VICAP_DVP_SET_SIZE_SET_WIDTH_MASK,
               height << VICAP_DVP_SET_SIZE_SET_HEIGHT_SHIFT |
               width << VICAP_DVP_SET_SIZE_SET_WIDTH_SHIFT);

    return HAL_OK;
}

/**
* @brief  Set vicap block line num.
* @param  pReg: VICAP reg base.
* @param  num: the num of block to sample
* @return HAL_Status.
* Set vicap block line num.
*/
HAL_Status HAL_VICAP_SetBlockLineNum(struct VICAP_REG *pReg, uint32_t num)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    MODIFY_REG(pReg->DVP_BLOCK_LINE_NUM, VICAP_DVP_BLOCK_LINE_NUM_BLOCK_LINE_NUM_MASK,
               num << VICAP_DVP_BLOCK_LINE_NUM_BLOCK_LINE_NUM_SHIFT);

    return HAL_OK;
}

/**
* @brief  Clear block status,by write 0x1 for interrelated bit.
* @param  pReg: VICAP reg base.
* @param  mask: the block status to be cleared
* @return HAL_Status.
* Clear vicap block status.
*/
uint32_t HAL_VICAP_ClearBlockStatus(struct VICAP_REG *pReg, uint32_t mask)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    WRITE_REG(pReg->DVP_BLOCK_STATUS, mask);

    return HAL_OK;
}

/**
* @brief  Set vicap crop ordinate
* @param  pReg: VICAP reg base
* @param  startY: the y ordinate to crop
* @param  startX: the x ordinate to crop
* @return HAL_Status
* Set the ordinate to crop.
*/
HAL_Status HAL_VICAP_SetCropOrdinate(struct VICAP_REG *pReg,
                                     uint32_t startY,
                                     uint32_t startX)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    MODIFY_REG(pReg->DVP_CROP,
               VICAP_DVP_CROP_START_X_MASK |
               VICAP_DVP_CROP_START_Y_MASK,
               startX << VICAP_DVP_CROP_START_X_SHIFT |
               startY << VICAP_DVP_CROP_START_Y_SHIFT);

    return HAL_OK;
}

/**
* @brief  select vicap path
* @param  pReg: VICAP reg base
* @param  path: the y ordinate to crop
* @return HAL_Status
* Set the path to sample.
*/
HAL_Status HAL_VICAP_SetPath(struct VICAP_REG *pReg, eVICAP_pathSel path)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    MODIFY_REG(pReg->DVP_PATH_SEL,
               VICAP_DVP_PATH_SEL_RAW_SEL_MASK | VICAP_DVP_PATH_SEL_YUV_SEL_MASK,
               path == VICAP_PATH_SEL_YUV ? VICAP_DVP_PATH_SEL_YUV_SEL_MASK :
                                            VICAP_DVP_PATH_SEL_RAW_SEL_MASK);

    return HAL_OK;
}

/**
* @brief  select vicap path
* @param  pReg: VICAP reg base
* @param  num: the line num of line1 to trigger interrupt
* @return HAL_Status
* Set the line num to line1.
*/
HAL_Status HAL_VICAP_SetLine1IntNum(struct VICAP_REG *pReg, uint32_t num)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    MODIFY_REG(pReg->DVP_LINE_INT_NUM,
               VICAP_DVP_LINE_INT_NUM_LINE1_INT_NUM_MASK,
               (num & 0x1fff) << VICAP_DVP_LINE_INT_NUM_LINE1_INT_NUM_SHIFT);

    return HAL_OK;
}

/**
* @brief  select vicap path
* @param  pReg: VICAP reg base
* @param  num: the line num of line0 to trigger interrupt
* @return HAL_Status
* Set the line num to line0.
*/
HAL_Status HAL_VICAP_SetLine0IntNum(struct VICAP_REG *pReg, uint32_t num)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    MODIFY_REG(pReg->DVP_LINE_INT_NUM,
               VICAP_DVP_LINE_INT_NUM_LINE0_INT_NUM_MASK,
               (num & 0x1fff) << VICAP_DVP_LINE_INT_NUM_LINE0_INT_NUM_SHIFT);

    return HAL_OK;
}

/**
* @brief  Set vicap water line
* @param  pReg: VICAP reg base
* @param  info: the information about vicap water line
* @return HAL_Status
* Set vicap water line
*/
HAL_Status HAL_VICAP_SetWaterLine(struct VICAP_REG *pReg,
                                  struct VICAP_WATERLINE_INFO *info)
{
    uint32_t regVal;

    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    regVal = READ_REG(pReg->DVP_WATER_LINE);
    CLEAR_BIT(regVal, VICAP_DVP_WATER_LINE_HURRY_EN_MASK |
              VICAP_DVP_WATER_LINE_HURRY_VALUE_MASK |
              VICAP_DVP_WATER_LINE_WATER_LINE_MASK);
    if (info->enable) {
        SET_BIT(regVal, VICAP_DVP_WATER_LINE_HURRY_EN_MASK);
    } else {
        CLEAR_BIT(regVal, VICAP_DVP_WATER_LINE_HURRY_EN_MASK);
    }

    if (info->hurryValue > VICAP_DVP_WATER_LINE_HURRY_VAL_MAX) {
        info->hurryValue = VICAP_DVP_WATER_LINE_HURRY_VAL_MAX;
    }
    regVal |= (info->hurryValue << VICAP_DVP_WATER_LINE_HURRY_VALUE_SHIFT);

    regVal |= (info->level << VICAP_DVP_WATER_LINE_WATER_LINE_SHIFT);

    WRITE_REG(pReg->DVP_WATER_LINE, regVal);

    return HAL_OK;
}

/**
* @brief  Get vicap fifo y entry.
* @param  pReg: VICAP reg base.
* @param  mask: frame status to be cleared.
* @return the value of y frame status
* Get vicap frame status
*/
uint32_t HAL_VICAP_ClearFrameStatus(struct VICAP_REG *pReg, uint32_t mask)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    MODIFY_REG(pReg->DVP_FRAME_STATUS, mask, (0U));

    return HAL_OK;
}

/**
 * @brief  VICAP interrupt handler.
 * @param  pReg: VICAP reg base.
 * @return HAL_Status.
 */
HAL_Status HAL_VICAP_IrqHandler(struct VICAP_REG *pReg)
{
    /* TODO */
    return HAL_OK;
}

/** @} */

/** @defgroup VICAP_Exported_Functions_Group4 Init and DeInit Functions
 @verbatim

 This section provides functions allowing to init and deinit the module:

 @endverbatim
 *  @{
 */

/**
 * @brief  Init VICAP control register.
 */
HAL_Status HAL_VICAP_Init(struct VICAP_REG *pReg)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    WRITE_REG(pReg->DVP_CTRL,
              VICAP_AXIBURST_TYPE16 << VICAP_DVP_CTRL_AXI_BURST_TYPE_SHIFT);

    return HAL_OK;
}

/**
 * @brief  DeInit VICAP control register.
 */
HAL_Status HAL_VICAP_DeInit(struct VICAP_REG *pReg)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    MODIFY_REG(pReg->DVP_CTRL, VICAP_DVP_CTRL_CAP_EN_MASK,
               (0U) << VICAP_DVP_CTRL_CAP_EN_SHIFT);

    return HAL_OK;
}

/** @} */

/** @defgroup VICAP_Exported_Functions_Group5 Other Functions
 *  @{
 */

/**
 * @brief  Enable irq in vicap controller.
 * @param  pReg: VICAP reg base.
 * @return HAL_Status.
 * Enable VICAP irq.
 */
HAL_Status HAL_VICAP_SetIrqEnable(struct VICAP_REG *pReg)
{
    uint32_t regVal;

    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    regVal = VICAP_DVP_INTEN_BLOCK0_END_EN_MASK | VICAP_DVP_INTEN_BLOCK1_END_EN_MASK |
             VICAP_DVP_INTEN_BUS_ERR_EN_MASK | VICAP_DVP_INTEN_PIX_ERR_EN_MASK |
             VICAP_DVP_INTEN_BLOCK_ERR_EN_MASK | VICAP_DVP_INTEN_FRAME_START_EN_MASK |
             VICAP_DVP_INTEN_DMA_FRAME_END_EN_MASK | VICAP_DVP_INTEN_LINE_ERR_EN_MASK |
             VICAP_DVP_INTEN_PRE_INF_FRAME_END_EN_MASK |
             VICAP_DVP_INTEN_PST_INF_FRAME_END_EN_MASK;

    WRITE_REG(pReg->DVP_INTEN, regVal);

    return HAL_OK;
}

/**
 * @brief  Enable irq in vicap controller.
 * @param  pReg: VICAP reg base.
 * @return HAL_Status.
 * Disable VICAP irq.
 */
HAL_Status HAL_VICAP_SetIrqDisable(struct VICAP_REG *pReg)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    WRITE_REG(pReg->DVP_INTEN, 0);

    return HAL_OK;
}

/**
 * @brief  Clear the actived irq of vicap in running.
 * @param  pReg: VICAP reg base.
 * @param  mask: the mask bit of irq
 * @return HAL_Status.
 * Clear VICAP irq status has happened.
 */

HAL_Status HAL_VICAP_ClearIrqStatus(struct VICAP_REG *pReg, uint32_t mask)
{
    HAL_ASSERT(IS_VICAP_INSTANCE(pReg));

    WRITE_REG(pReg->DVP_INTSTAT, mask);

    return HAL_OK;
}

/** @} */

/** @} */

/** @} */

#endif /* HAL_VICAP_MODULE_ENABLED */
