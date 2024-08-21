/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#if defined(SOC_RV1126) && defined(HAL_VOP_MODULE_ENABLED)

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup VOP
 *  @{
 */

/** @defgroup VOP_How_To_Use How To Use
 *  @{

 The VOP driver can be used as follows:

 @} */

#include "hal_math.h"

/** @defgroup VOP_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/

#define IS_YUV_FORMAT(x) (((x) >= VOP_FMT_YUV420SP) && ((x) <= VOP_FMT_VYUY422_4)) ? 1 : 0
#define IS_BPP_FORMAT(x) (((x) >= VOP_FMT_1BPP) && ((x) <= VOP_FMT_8BPP)) ? 1 : 0

#define MCU_CS_STR 1
#define MCU_CS_END 7
#define MCU_WR_STR 4
#define MCU_WR_END 6
#define MCU_TOTAL  8

#define MIPI_SWITCH_TIME_OUT 100

/********************* Private Structure Definition **************************/
typedef enum {
    VOP_WIN0,
    VOP_WIN1,
    VOP_WIN2,
} eVOP_WinId;

typedef enum {
    DCLK_DISABLE,
    DCLK_ENABLE,
} eVOP_DclkSwitch;

typedef enum {
    RGB888TORGB565,
    RGB888TORGB666,
} eVOP_DitherDownMode;

typedef enum {
    DITHER_DOWN_ALLEGRO,
    DITHER_DOWN_FRC,
} eVOP_DitherDownSel;

typedef enum {
    OUTPUT_MODE_888,
    OUTPUT_MODE_666,
    OUTPUT_MODE_565,
    OUT_MODE_S888       = 8,
    OUT_MODE_S888_DUMMY = 12,
} eVOP_OutputMode;

typedef enum {
    BCSH_BLACK_MODE,
    BCSH_BLUE_MODE,
    BCSH_COLOR_BAR_MODE,
    BCSH_NORMAL_MODE
} eVOP_BcshVideoMode;

/********************* Private Variable Definition ***************************/
static struct VOP_REG s_vopRegMir;
HAL_UNUSED static const char * const VOP_IRQs[] =
{
    "frame start interrupt status!",
    "new frame start interrupt status!",
    "same address interrupt status!",
    "line flag0 interrupt status!",
    "line flag1 interrupt status!",
    "bus error interrupt status!",
    "reserved interrupt status!",
    "win0 empty interrupt status!",
    "win1 empty interrupt status!",
    "win2 empty interrupt status!",
    "display hold interrupt status!"
    "dma finish interrupt status!"
    "post lb empty interrupt status!"
};

__STATIC_INLINE int16_t VOP_Interpolate(int16_t x1, int16_t y1, int16_t x2,
                                        int16_t y2, uint16_t x)
{
    return y1 + (y2 - y1) * (x - x1) / (x2 - x1);
}

HAL_UNUSED static uint32_t VOP_MaskRead(__IO uint32_t hwReg,
                                        uint32_t shift, uint32_t mask)
{
    return (READ_REG(hwReg) & mask) >> shift;
}

HAL_UNUSED static void VOP_MaskWriteNoBackup(__IO uint32_t *mirReg, __IO uint32_t *hwReg,
                                             uint32_t shift, uint32_t mask,
                                             uint32_t v)
{
    uint32_t mirVal = *mirReg;

    v = (mirVal & ~mask) | ((v << shift) & mask);

    WRITE_REG(*hwReg, v);
}

static void VOP_MaskWrite(__IO uint32_t *mirReg, __IO uint32_t *hwReg,
                          uint32_t shift, uint32_t mask,
                          uint32_t v)
{
    if (mirReg) {
        uint32_t mirVal = *mirReg;

        v = (mirVal & ~mask) | ((v << shift) & mask);
        *mirReg = v;
    } else {
        v = ((v << shift) & mask) | (mask << 16);
    }

    WRITE_REG(*hwReg, v);
}

static void VOP_Write(__IO uint32_t *mirReg, __IO uint32_t *hwReg,
                      uint32_t v)
{
    if (mirReg) {
        *mirReg = v;
    }
    WRITE_REG(*hwReg, v);
}

HAL_UNUSED static uint8_t VOP_GetHorSubSampling(uint8_t format)
{
    switch (format) {
    case VOP_FMT_YUV420SP:
    case VOP_FMT_YUV420SP_4:

        return 4;
        break;
    case VOP_FMT_YVYU422:
    case VOP_FMT_VYUY422:
    case VOP_FMT_YUV422SP:
    case VOP_FMT_YVYU422_4:
    case VOP_FMT_VYUY422_4:
    case VOP_FMT_YUV422SP_4:

        return 2;
        break;
    default:

        return 1;
    }
}

HAL_UNUSED static uint8_t VOP_GetVerSubSampling(uint8_t format)
{
    switch (format) {
    case VOP_FMT_YUV420SP:
    case VOP_FMT_YUV420SP_4:

        return 4;
        break;
    case VOP_FMT_YUV422SP:
    case VOP_FMT_YUV422SP_4:

        return 2;
        break;
    default:

        return 1;
    }
}

HAL_UNUSED static uint8_t VOP_GetFormatLength(uint8_t format, uint8_t plane)
{
    uint8_t val;

    switch (format) {
    case VOP_FMT_ARGB8888:
        val = 32;
        break;
    case VOP_FMT_RGB888:
        val = 24;
        break;
    case VOP_FMT_RGB565:
        val = 16;
        break;
    case VOP_FMT_YUV420SP:
        val = plane ? 16 : 8;
        break;
    case VOP_FMT_YUV422SP:
    case VOP_FMT_YUV444SP:
        val = 8;
        break;
    default:
        val = 8;
        HAL_DBG_ERR("Unsupport format: %d!\n", format);
        break;
    }

    return val;
}

static inline bool IS_Valid_Win(int winId)
{
    if ((winId != VOP_WIN0) && (winId != VOP_WIN2)) {
        return false;
    } else {
        return true;
    }
}
static void VOP_SetWin(struct VOP_REG *pReg,
                       struct CRTC_WIN_STATE *pWinState,
                       struct DISPLAY_MODE_INFO *pModeInfo)
{
    uint32_t val, actInfo, dspInfo, dspSt, dspStx, dspSty;
    uint32_t yStride, cbcrStride;
    uint32_t regOffset;

    if (!IS_Valid_Win(pWinState->winId)) {
        return;
    }

    regOffset = (pWinState->winId == VOP_WIN2) ? 0x50 : 0;

    VOP_MaskWrite(&s_vopRegMir.WIN0_CTRL0 + regOffset,
                  &pReg->WIN0_CTRL0 + regOffset,
                  VOP_WIN0_CTRL0_WIN0_EN_SHIFT,
                  VOP_WIN0_CTRL0_WIN0_EN_MASK, pWinState->winEn);

    if (!pWinState->winEn) {
        return;
    }

    if ((pWinState->winId == VOP_WIN2) && (pWinState->hwFormat > VOP_FMT_RGB565)) {
        return;
    }

    VOP_MaskWrite(&s_vopRegMir.WIN0_CTRL0 + regOffset,
                  &pReg->WIN0_CTRL0 + regOffset,
                  VOP_WIN0_CTRL0_WIN0_DATA_FMT_SHIFT,
                  VOP_WIN0_CTRL0_WIN0_DATA_FMT_MASK, pWinState->hwFormat);

    yStride = pWinState->stride;

    if ((pWinState->hwFormat == VOP_FMT_YUV444SP) ||
        (pWinState->hwFormat == VOP_FMT_YUV444SP_4)) {
        cbcrStride = 2 * yStride;
    } else {
        cbcrStride = yStride;
    }
    VOP_Write(&s_vopRegMir.WIN0_VIR + regOffset,
              &pReg->WIN0_VIR + regOffset, cbcrStride << 16 | yStride);
    VOP_Write(&s_vopRegMir.WIN0_YRGB_MST + regOffset,
              &pReg->WIN0_YRGB_MST + regOffset, pWinState->yrgbAddr);
    VOP_Write(&s_vopRegMir.WIN0_CBR_MST + regOffset,
              &pReg->WIN0_CBR_MST + regOffset, pWinState->cbcrAddr);

    dspInfo = (pWinState->crtcH - 1) << 16 | (pWinState->crtcW - 1);
    actInfo = (pWinState->srcH - 1) << 16 | (pWinState->srcW - 1);
    dspStx = pWinState->hwCrtcX + pModeInfo->crtcHtotal -
             pModeInfo->crtcHsyncStart;
    dspSty = pWinState->hwCrtcY + pModeInfo->crtcVtotal -
             pModeInfo->crtcVsyncStart;
    dspSt = dspSty << 16 | dspStx;
    VOP_Write(&s_vopRegMir.WIN0_DSP_INFO + regOffset,
              &pReg->WIN0_DSP_INFO + regOffset, dspInfo);
    VOP_Write(&s_vopRegMir.WIN0_ACT_INFO + regOffset,
              &pReg->WIN0_ACT_INFO + regOffset, actInfo);

    VOP_Write(&s_vopRegMir.WIN0_DSP_ST + regOffset,
              &pReg->WIN0_DSP_ST + regOffset, dspSt);
    val = pWinState->alphaEn << VOP_WIN0_ALPHA_CTRL_WIN0_ALPHA_EN_SHIFT |
          pWinState->alphaMode << VOP_WIN0_ALPHA_CTRL_WIN0_ALPHA_MODE_SHIFT |
          pWinState->alphaPreMul << VOP_WIN0_ALPHA_CTRL_WIN0_ALPHA_PRE_MUL_SHIFT |
          pWinState->alphaSatMode << VOP_WIN0_ALPHA_CTRL_WIN0_ALPHA_SAT_MODE_SHIFT |
          pWinState->globalAlphaValue << VOP_WIN0_ALPHA_CTRL_WIN0_ALPHA_VALUE_SHIFT;
    VOP_Write(&s_vopRegMir.WIN0_ALPHA_CTRL + regOffset,
              &pReg->WIN0_ALPHA_CTRL + regOffset, val);
    VOP_Write(&s_vopRegMir.WIN0_COLOR_KEY + regOffset,
              &pReg->WIN0_COLOR_KEY + regOffset, pWinState->colorKey);
}

/** @} */
/********************* Public Function Definition ****************************/

/** @defgroup VOP_Exported_Functions_Group2 get VOP Status and Errors Functions

 This section provides functions allowing to get VOP status:

 *  @{
 */
/**
 * @brief  Get VOP scane line number.
 * @param  pReg: VOP reg base.
 * @return HAL_Status.
 */
uint32_t HAL_VOP_GetScanLine(struct VOP_REG *pReg)
{
    return 0;
}

/**
 * @brief  Get the config done state before commit.
 * @param  pReg: VOP reg base.
 * @return HAL_Status.
 */
uint8_t HAL_VOP_CommitPrepare(struct VOP_REG *pReg)
{
    return VOP_MaskRead(pReg->INTR_STATUS,
                        VOP_INTR_STATUS_DSP_HOLD_VALID_INTR_RAW_STS_SHIFT,
                        VOP_INTR_STATUS_DSP_HOLD_VALID_INTR_RAW_STS_MASK);
}

/**
 * @brief  Get the current commit active state.
 * @param  pReg: VOP reg base.
 * @return HAL_Status.
 */
uint8_t HAL_VOP_CommitPost(struct VOP_REG *pReg)
{
    return VOP_MaskRead(pReg->REG_CFG_DONE,
                        VOP_REG_CFG_DONE_REG_LOAD_GLOBAL_EN_SHIFT,
                        VOP_REG_CFG_DONE_REG_LOAD_GLOBAL_EN_MASK);
}

/** @} */

/** @defgroup VOP_Exported_Functions_Group3 IO Functions

 This section provides functions allowing to IO controlling:

 *  @{
 */
/**
 * @brief  Load VOP bpp format lut.
 * @param  pReg: VOP reg base.
 * @param  winId: VOP win id.
 * @param  lut: look up table.
 * @param  lut_size: lut size.
 * @return HAL_Status.
 */
HAL_Status HAL_VOP_LoadLut(struct VOP_REG *pReg, uint8_t winId,
                           uint32_t *lut, uint16_t lut_size)
{
    return HAL_OK;
}

/**
 * @brief  Disable VOP bpp format lut.
 * @param  pReg: VOP reg base.
 * @param  winId: VOP win id.
 * @return HAL_Status.
 */
HAL_Status HAL_VOP_DisableLut(struct VOP_REG *pReg, uint8_t winId)
{
    return HAL_OK;
}

/**
 * @brief  VOP MIPI switch.
 * @param  pReg: VOP reg base.
 * @param  path: mipi switch direction.
 * @return HAL_Status.
 */
HAL_Status HAL_VOP_MipiSwitch(struct VOP_REG *pReg, eVOP_MipiSwitchPath path)
{
    return HAL_OK;
}

/**
 * @brief  Set display area.
 * @param  pReg: VOP reg base.
 * @param  display_rect: display rect.
 * @return HAL_Status.
 * TODO: dsc_imd_config_en
 */
HAL_Status HAL_VOP_SetArea(struct VOP_REG *pReg,
                           struct DISPLAY_RECT *display_rect)
{
    return HAL_OK;
}

/**
 * @brief  Set plane state.
 * @param  pReg: VOP reg base.
 * @param  pWinState: win state.
 * @param  pModeInfo: VOP output modeinfo.
 * @return HAL_Status.
 */
HAL_Status HAL_VOP_SetPlane(struct VOP_REG *pReg,
                            struct CRTC_WIN_STATE *pWinState,
                            struct DISPLAY_MODE_INFO *pModeInfo)
{
    uint16_t xVirByte;

    xVirByte = pWinState->xVir *
               VOP_GetFormatLength(pWinState->hwFormat, 0) / 8;
    if (xVirByte & 0x3) {
        HAL_DBG_ERR("VOP setplane xvir should be 4 byte align: %d\n", xVirByte);

        return HAL_INVAL;
    }
    pWinState->stride = xVirByte >> 2;
    VOP_SetWin(pReg, pWinState, pModeInfo);

    return HAL_OK;
}

/**
 * @brief  Set config done to commit frame.
 * @param  pReg: VOP reg base.
 * @return HAL_Status.
 */
HAL_Status HAL_VOP_Commit(struct VOP_REG *pReg)
{
    VOP_MaskWriteNoBackup(&s_vopRegMir.REG_CFG_DONE, &pReg->REG_CFG_DONE,
                          VOP_REG_CFG_DONE_REG_LOAD_GLOBAL_EN_SHIFT,
                          VOP_REG_CFG_DONE_REG_LOAD_GLOBAL_EN_MASK, 1);

    VOP_MaskWrite(NULL, &pReg->INTR_CLEAR,
                  VOP_INTR_CLEAR_DSP_HOLD_VALID_INTR_CLR_SHIFT,
                  VOP_INTR_CLEAR_DSP_HOLD_VALID_INTR_CLR_MASK, 1);

    return HAL_OK;
}

/**
 * @brief  Set WMS frame start.
 * @param  pReg: VOP reg base.
 * @return HAL_Status.
 */
HAL_Status HAL_VOP_EdpiFrmSt(struct VOP_REG *pReg)
{
    return HAL_OK;
}

/** @} */

/** @defgroup VOP_Exported_Functions_Group4 Init and DeInit Functions

 This section provides functions allowing to init and deinit the module:

 *  @{
 */

/**
 * @brief  VOP init.
 * @param  pReg: VOP reg base.
 * @param  pModeInfo: VOP putput mode info.
 * @return HAL_Status.
 */
HAL_Status HAL_VOP_Init(struct VOP_REG *pReg,
                        struct DISPLAY_MODE_INFO *pModeInfo)
{
    uint32_t i = 0;
    uint32_t *regMir = (uint32_t *)&s_vopRegMir;
    uint32_t *vopReg = (uint32_t *)pReg;
    uint32_t regLen = sizeof(s_vopRegMir) >> 2;

    for (i = 0; i < regLen; i++) {
        regMir[i] = vopReg[i];
    }

    VOP_MaskWrite(&s_vopRegMir.DSP_CTRL2, &pReg->DSP_CTRL2,
                  VOP_DSP_CTRL2_DSP_BLANK_EN_SHIFT,
                  VOP_DSP_CTRL2_DSP_BLANK_EN_MASK,
                  0);
    VOP_MaskWrite(&s_vopRegMir.DSP_CTRL2, &pReg->DSP_CTRL2,
                  VOP_DSP_CTRL2_DSP_LAYER1_SEL_SHIFT,
                  VOP_DSP_CTRL2_DSP_LAYER1_SEL_MASK,
                  0);
    VOP_MaskWrite(&s_vopRegMir.DSP_CTRL2, &pReg->DSP_CTRL2,
                  VOP_DSP_CTRL2_DSP_LAYER2_SEL_SHIFT,
                  VOP_DSP_CTRL2_DSP_LAYER2_SEL_MASK,
                  2);

    return HAL_OK;
}

/**
 * @brief  VOP deinit.
 * @param  pReg: VOP reg base.
 * @return HAL_Status.
 */
HAL_Status HAL_VOP_DeInit(struct VOP_REG *pReg)
{
    return HAL_OK;
}

/**
 * @brief  VOP mode init.
 * @param  pReg: VOP reg base.
 * @param  pModeInfo: VOP output mode info.
 * @param  pPostScaleInfo: VOP post scale info.
 * @return HAL_Status.
 */
HAL_Status HAL_VOP_ModeInit(struct VOP_REG *pReg,
                            struct DISPLAY_MODE_INFO *pModeInfo,
                            struct VOP_POST_SCALE_INFO *pPostScaleInfo)
{
    uint16_t hfp = pModeInfo->crtcHsyncStart - pModeInfo->crtcHdisplay;
    uint16_t hsyncLen = pModeInfo->crtcHsyncEnd -
                        pModeInfo->crtcHsyncStart;
    uint16_t hbp = pModeInfo->crtcHtotal - pModeInfo->crtcHsyncEnd;
    uint16_t vfp = pModeInfo->crtcVsyncStart - pModeInfo->crtcVdisplay;
    uint16_t vsyncLen = pModeInfo->crtcVsyncEnd -
                        pModeInfo->crtcVsyncStart;
    uint16_t vbp = pModeInfo->crtcVtotal - pModeInfo->crtcVsyncEnd;

    uint16_t dsp_hdisplay = pPostScaleInfo->dstW;
    uint16_t dsp_vdisplay = pPostScaleInfo->dstH;
    uint16_t dspHsync, dspHactSt, dspHactEnd, dspHtotal;
    uint16_t dspVsync, dspVactSt, dspVactEnd, dspVtotal;

    dspHsync = hsyncLen;
    dspHactSt = dspHsync + hbp;
    dspHactEnd = dspHactSt + dsp_hdisplay;
    dspHtotal = dspHactEnd + hfp;

    dspVsync = vsyncLen;
    dspVactSt = dspVsync + vbp;
    dspVactEnd = dspVactSt + dsp_vdisplay;
    dspVtotal = dspVactEnd + vfp;

    VOP_Write(&s_vopRegMir.DSP_HTOTAL_HS_END, &pReg->DSP_HTOTAL_HS_END,
              dspHtotal << 16 | dspHsync);
    VOP_Write(&s_vopRegMir.DSP_HACT_ST_END, &pReg->DSP_HACT_ST_END,
              dspHactSt << 16 | dspHactEnd);
    VOP_Write(&s_vopRegMir.DSP_VTOTAL_VS_END, &pReg->DSP_VTOTAL_VS_END,
              dspVtotal << 16 | dspVsync);
    VOP_Write(&s_vopRegMir.DSP_VACT_ST_END, &pReg->DSP_VACT_ST_END,
              dspVactSt << 16 | dspVactEnd);

    return HAL_OK;
}

/**
 * @brief  VOP MCU mode init.
 * @param  pReg: VOP reg base.
 * @return HAL_Status.
 */
HAL_Status HAL_VOP_McuModeInit(struct VOP_REG *pReg)
{
    VOP_MaskWrite(&s_vopRegMir.MCU, &pReg->MCU, VOP_MCU_MCU_TYPE_SHIFT,
                  VOP_MCU_MCU_TYPE_MASK, 1);

    VOP_MaskWrite(&s_vopRegMir.MCU, &pReg->MCU, VOP_MCU_MCU_PIX_TOTAL_SHIFT,
                  VOP_MCU_MCU_PIX_TOTAL_MASK, MCU_TOTAL);

    VOP_MaskWrite(&s_vopRegMir.MCU, &pReg->MCU, VOP_MCU_MCU_CS_PST_SHIFT,
                  VOP_MCU_MCU_CS_PST_MASK, MCU_CS_STR);

    VOP_MaskWrite(&s_vopRegMir.MCU, &pReg->MCU, VOP_MCU_MCU_CS_PEND_SHIFT,
                  VOP_MCU_MCU_CS_PEND_MASK, MCU_CS_END);

    VOP_MaskWrite(&s_vopRegMir.MCU, &pReg->MCU, VOP_MCU_MCU_RW_PST_SHIFT,
                  VOP_MCU_MCU_RW_PST_MASK, MCU_WR_STR);

    VOP_MaskWrite(&s_vopRegMir.MCU, &pReg->MCU, VOP_MCU_MCU_RW_PEND_SHIFT,
                  VOP_MCU_MCU_RW_PEND_MASK, MCU_WR_END);

    VOP_MaskWrite(&s_vopRegMir.MCU, &pReg->MCU, VOP_MCU_MCU_CLK_SEL_SHIFT,
                  VOP_MCU_MCU_CLK_SEL_MASK, 0);

    VOP_MaskWrite(&s_vopRegMir.MCU, &pReg->MCU, VOP_MCU_MCU_HOLD_MODE_SHIFT,
                  VOP_MCU_MCU_HOLD_MODE_MASK, 1);

    return HAL_OK;
}

/**
 * @brief  VOP post scale init.
 * @param  pReg: VOP reg base.
 * @param  pModeInfo: VOP output mode info.
 * @param  pPostScaleInfo: VOP post scale info.
 * @return HAL_Status.
 */
HAL_Status HAL_VOP_PostScaleInit(struct VOP_REG *pReg,
                                 struct DISPLAY_MODE_INFO *pModeInfo,
                                 struct VOP_POST_SCALE_INFO *pPostScaleInfo)
{
    return HAL_OK;
}

/**
 * @brief  VOP post BCSH init.
 * @param  pReg: VOP reg base.
 * @param  pBCSHInfo: VOP BCSH info.
 * @return HAL_Status.
 */
HAL_Status HAL_VOP_PostBCSH(struct VOP_REG *pReg,
                            struct VOP_BCSH_INFO *pBCSHInfo)
{
    uint32_t val;
    uint16_t brightness, contrast, saturation, hue, sinHue, cosHue;
    bool bcshEn = false;

    if ((pBCSHInfo->brightness != 50) || (pBCSHInfo->contrast != 50) ||
        (pBCSHInfo->satCon != 50) || (pBCSHInfo->hue != 50)) {
        bcshEn = true;
    }

    brightness = VOP_Interpolate(0, -31, 100, 31, pBCSHInfo->brightness);
    contrast = VOP_Interpolate(0, 0, 100, 511, pBCSHInfo->contrast);
    saturation = VOP_Interpolate(0, 0, 100, 511, pBCSHInfo->satCon);
    hue = VOP_Interpolate(0, -30, 100, 30, pBCSHInfo->hue);

    /*
     *  a:[-30~0):
     *    sinHue = 0x100 - sin(a)*256;
     *    cosHue = cos(a)*256;
     *  a:[0~30]
     *    sinHue = sin(a)*256;
     *    cosHue = cos(a)*256;
     */
    sinHue = HAL_Sin(hue) >> 23;
    cosHue = HAL_Cos(hue) >> 23;

    val = brightness << VOP_BCSH_BCS_BRIGHTNESS_SHIFT |
          contrast << VOP_BCSH_BCS_CONTRAST_SHIFT |
          saturation << VOP_BCSH_BCS_SAT_CON_SHIFT;
    VOP_Write(&s_vopRegMir.BCSH_BCS, &pReg->BCSH_BCS, val);

    val = sinHue << VOP_BCSH_H_SIN_HUE_SHIFT |
          cosHue << VOP_BCSH_H_COS_HUE_SHIFT;
    VOP_Write(&s_vopRegMir.BCSH_H, &pReg->BCSH_H, val);

    VOP_MaskWrite(&s_vopRegMir.BCSH_CTRL, &pReg->BCSH_CTRL,
                  VOP_BCSH_CTRL_SW_BCSH_R2Y_EN_SHIFT,
                  VOP_BCSH_CTRL_SW_BCSH_R2Y_EN_MASK,
                  bcshEn);

    VOP_MaskWrite(&s_vopRegMir.BCSH_CTRL, &pReg->BCSH_CTRL,
                  VOP_BCSH_CTRL_VIDEO_MODE_SHIFT,
                  VOP_BCSH_CTRL_VIDEO_MODE_MASK,
                  BCSH_NORMAL_MODE);

    VOP_MaskWrite(&s_vopRegMir.BCSH_CTRL, &pReg->BCSH_CTRL,
                  VOP_BCSH_CTRL_BCSH_EN_SHIFT,
                  VOP_BCSH_CTRL_BCSH_EN_MASK,
                  bcshEn);

    return HAL_OK;
}

/**
 * @brief  VOP post gamma init, the gamma adjust should be in yuv color space,
           so we need to enable bcsh r2y and post csc en.
 * @param  pReg: VOP reg base.
 * @param  pGammaInfo: VOP gamma info.
 * @return HAL_Status.
 */
HAL_Status HAL_VOP_PostGammaInit(struct VOP_REG *pReg,
                                 struct VOP_GAMMA_COE_INFO *pGammaInfo)
{
    return HAL_OK;
}

/**
 * @brief  VOP post color matrix init.
 * @param  pReg: VOP reg base.
 * @param  pColorMatrixInfo: VOP color matrix info.
 * @return HAL_Status.
 */
HAL_Status HAL_VOP_PostColorMatrixInit(struct VOP_REG *pReg,
                                       struct VOP_COLOR_MATRIX_INFO *pColorMatrixInfo)
{
    return HAL_OK;
}

/**
 * @brief  VOP post clip init.
 * @param  pReg: VOP reg base.
 * @param  pClipInfo: VOP clip info.
 * @return HAL_Status.
 */
HAL_Status HAL_VOP_PostClipInit(struct VOP_REG *pReg,
                                struct VOP_POST_CLIP_INFO *pClipInfo)
{
    return HAL_OK;
}

/**
 * @brief  VOP polarity init.
 * @param  pReg: VOP reg base.
 * @param  pModeInfo: VOP output mode info.
 * @param  ConnectorType: output connector type.
 * @return HAL_Status.
 */
HAL_Status HAL_VOP_PolarityInit(struct VOP_REG *pReg,
                                struct DISPLAY_MODE_INFO *pModeInfo,
                                uint16_t ConnectorType)
{
    uint16_t polarity;

    polarity = DCLK_ENABLE;
    polarity |= (pModeInfo->flags & VIDEO_MODE_FLAG_NPIXDATA ? 1 : 0) << 1;
    polarity |= (pModeInfo->flags & VIDEO_MODE_FLAG_NHSYNC ? 1 : 0) << 2;
    polarity |= (pModeInfo->flags & VIDEO_MODE_FLAG_NVSYNC ? 1 : 0) << 3;
    polarity |= (pModeInfo->flags & VIDEO_MODE_FLAG_DEN ? 1 : 0) << 4;
    switch (ConnectorType) {
    case RK_DISPLAY_CONNECTOR_RGB:
        VOP_Write(&s_vopRegMir.DSP_CTRL0, &pReg->DSP_CTRL0,
                  polarity);
        break;
    case RK_DISPLAY_CONNECTOR_DSI:
        VOP_Write(&s_vopRegMir.DSP_CTRL0, &pReg->DSP_CTRL0,
                  polarity << VOP_DSP_CTRL0_MIPI_DCLK_EN_SHIFT);
        break;
    default:
        HAL_DBG_ERR("Unknown Connector Type: %d\n", ConnectorType);
        break;
    }

    return HAL_OK;
}

/**
 * @brief  VOP output busformat init.
 * @param  pReg: VOP reg base.
 * @param  pModeInfo: VOP output mode info.
 * @param  BusFormat: output busformat.
 * @return HAL_Status.
 */
HAL_Status HAL_VOP_OutputInit(struct VOP_REG *pReg,
                              struct DISPLAY_MODE_INFO *pModeInfo,
                              uint16_t BusFormat)
{
    switch (BusFormat) {
    case MEDIA_BUS_FMT_RGB666_1X18:
    case MEDIA_BUS_FMT_RGB666_1X24_CPADHI:
        VOP_MaskWrite(&s_vopRegMir.DSP_CTRL2, &pReg->DSP_CTRL2,
                      VOP_DSP_CTRL2_DITHER_DOWN_MODE_SHIFT,
                      VOP_DSP_CTRL2_DITHER_DOWN_MODE_MASK,
                      RGB888TORGB666);
        VOP_MaskWrite(&s_vopRegMir.DSP_CTRL2, &pReg->DSP_CTRL2,
                      VOP_DSP_CTRL2_DITHER_DOWN_SEL_SHIFT,
                      VOP_DSP_CTRL2_DITHER_DOWN_SEL_MASK,
                      DITHER_DOWN_ALLEGRO);
        VOP_MaskWrite(&s_vopRegMir.DSP_CTRL2, &pReg->DSP_CTRL2,
                      VOP_DSP_CTRL2_DITHER_DOWN_SHIFT,
                      VOP_DSP_CTRL2_DITHER_DOWN_MASK,
                      1);
        VOP_MaskWrite(&s_vopRegMir.DSP_CTRL2, &pReg->DSP_CTRL2,
                      VOP_DSP_CTRL2_DSP_OUT_MODE_SHIFT,
                      VOP_DSP_CTRL2_DSP_OUT_MODE_MASK,
                      OUTPUT_MODE_666);
        break;
    case MEDIA_BUS_FMT_RGB888_1X24:
        VOP_MaskWrite(&s_vopRegMir.DSP_CTRL2, &pReg->DSP_CTRL2,
                      VOP_DSP_CTRL2_DITHER_DOWN_SHIFT,
                      VOP_DSP_CTRL2_DITHER_DOWN_MASK,
                      0);
        VOP_MaskWrite(&s_vopRegMir.DSP_CTRL2, &pReg->DSP_CTRL2,
                      VOP_DSP_CTRL2_DSP_OUT_MODE_SHIFT,
                      VOP_DSP_CTRL2_DSP_OUT_MODE_MASK,
                      OUTPUT_MODE_888);
        break;
    case MEDIA_BUS_FMT_SRGB888_3X8:
        VOP_MaskWrite(&s_vopRegMir.DSP_CTRL2, &pReg->DSP_CTRL2,
                      VOP_DSP_CTRL2_DITHER_DOWN_SHIFT,
                      VOP_DSP_CTRL2_DITHER_DOWN_MASK,
                      0);
        VOP_MaskWrite(&s_vopRegMir.DSP_CTRL2, &pReg->DSP_CTRL2,
                      VOP_DSP_CTRL2_DSP_OUT_MODE_SHIFT,
                      VOP_DSP_CTRL2_DSP_OUT_MODE_MASK,
                      OUT_MODE_S888);
        break;
    case MEDIA_BUS_FMT_SRGB888_DUMMY_4X8:
        VOP_MaskWrite(&s_vopRegMir.DSP_CTRL2, &pReg->DSP_CTRL2,
                      VOP_DSP_CTRL2_DITHER_DOWN_SHIFT,
                      VOP_DSP_CTRL2_DITHER_DOWN_MASK,
                      0);
        VOP_MaskWrite(&s_vopRegMir.DSP_CTRL2, &pReg->DSP_CTRL2,
                      VOP_DSP_CTRL2_DSP_OUT_MODE_SHIFT,
                      VOP_DSP_CTRL2_DSP_OUT_MODE_MASK,
                      OUT_MODE_S888_DUMMY);
        break;
    default:
        HAL_DBG_ERR("Unknown Bus Format: %d\n", BusFormat);
        break;
    }

    return HAL_OK;
}

/**
 * @brief  VOP edpi init.
 * @param  pReg: VOP reg base.
 * @param  triggerMode: VOP new frame trigger mode,default is 0,
 *         0: TE trigger, VOP commit new frame time depend on TE + wms_st
 *         1: gpio trigger, VOP commit new frame time depend on gpio connect to panel TE pin
 * @return HAL_Status.
 */
HAL_Status HAL_VOP_EdpiInit(struct VOP_REG *pReg, uint8_t triggerMode)
{
    return HAL_OK;
}

/**
 * @brief  Config VOP NOC QOS value.
 * @param  pReg: VOP reg base.
 * @param  nocQosValue: NOC qos value.
 * @return HAL_Status.
 */
HAL_Status HAL_VOP_NocQosInit(struct VOP_REG *pReg, uint32_t nocQosValue)
{
    VOP_MaskWrite(&s_vopRegMir.SYS_CTRL1, &pReg->SYS_CTRL1,
                  VOP_SYS_CTRL1_SW_NOC_QOS_EN_SHIFT,
                  VOP_SYS_CTRL1_SW_NOC_QOS_EN_MASK, 1);
    VOP_MaskWrite(&s_vopRegMir.SYS_CTRL1, &pReg->SYS_CTRL1,
                  VOP_SYS_CTRL1_SW_NOC_QOS_VALUE_SHIFT,
                  VOP_SYS_CTRL1_SW_NOC_QOS_VALUE_MASK, nocQosValue);

    return HAL_OK;
}

/**
 * @brief  Config VOP NOC hurry threshold value.
 * @param  pReg: VOP reg base.
 * @param  hurryValue: hurry value.
 * @param  hurryThreshold: hurry threshold.
 * @return HAL_Status.
 */
HAL_Status HAL_VOP_NocHurryInit(struct VOP_REG *pReg, uint32_t hurryValue,
                                uint32_t hurryThreshold)
{
    VOP_MaskWrite(&s_vopRegMir.SYS_CTRL1, &pReg->SYS_CTRL1,
                  VOP_SYS_CTRL1_SW_NOC_HURRY_EN_SHIFT,
                  VOP_SYS_CTRL1_SW_NOC_HURRY_EN_MASK, 1);
    VOP_MaskWrite(&s_vopRegMir.SYS_CTRL1, &pReg->SYS_CTRL1,
                  VOP_SYS_CTRL1_SW_NOC_HURRY_VALUE_SHIFT,
                  VOP_SYS_CTRL1_SW_NOC_HURRY_VALUE_MASK, hurryValue);
    VOP_MaskWrite(&s_vopRegMir.SYS_CTRL1, &pReg->SYS_CTRL1,
                  VOP_SYS_CTRL1_SW_NOC_HURRY_THRESHOLD_SHIFT,
                  VOP_SYS_CTRL1_SW_NOC_HURRY_THRESHOLD_MASK,
                  hurryThreshold);

    return HAL_OK;
}

/**
 * @brief  Config VOP AXI outstand number.
 * @param  pReg: VOP reg base.
 * @param  outStandNum: AXI outstand number.
 * @return HAL_Status.
 */
HAL_Status HAL_VOP_AxiOutstandInit(struct VOP_REG *pReg, uint32_t outStandNum)
{
    VOP_MaskWrite(&s_vopRegMir.SYS_CTRL1, &pReg->SYS_CTRL1,
                  VOP_SYS_CTRL1_SW_AXI_MAX_OUTSTAND_EN_SHIFT,
                  VOP_SYS_CTRL1_SW_AXI_MAX_OUTSTAND_EN_MASK, 1);
    VOP_MaskWrite(&s_vopRegMir.SYS_CTRL1, &pReg->SYS_CTRL1,
                  VOP_SYS_CTRL1_SW_AXI_MAX_OUTSTAND_NUM_SHIFT,
                  VOP_SYS_CTRL1_SW_AXI_MAX_OUTSTAND_NUM_MASK, outStandNum);

    return HAL_OK;
}

/** @} */

/** @defgroup VOP_Exported_Functions_Group5 Other Functions

 This section provides functions allowing to control uart:

 *  @{
 */
/**
 * @brief  Enable VOP frame start interrupt.
 * @param  pReg: VOP reg base.
 * @return HAL_Status.
 */
HAL_Status HAL_VOP_EnableFsIrq(struct VOP_REG *pReg)
{
    VOP_MaskWrite(NULL, &pReg->INTR_EN, VOP_INTR_EN_FS0_INTR_EN_SHIFT,
                  VOP_INTR_EN_FS0_INTR_EN_MASK, 1);

    return HAL_OK;
}

/**
 * @brief  Disable VOP frame start interrupt.
 * @param  pReg: VOP reg base.
 * @return HAL_Status.
 */
HAL_Status HAL_VOP_DisableFsIrq(struct VOP_REG *pReg)
{
    VOP_MaskWrite(NULL, &pReg->INTR_EN, VOP_INTR_EN_FS0_INTR_EN_SHIFT,
                  VOP_INTR_EN_FS0_INTR_EN_MASK, 0);

    return HAL_OK;
}

/**
 * @brief  Enable VOP line flag interrupt.
 * @param  pReg: VOP reg base.
 * @param  lineFlag0: Line flag0 position.
 * @param  lineFlag1: Line flag1 position.
 * @return HAL_Status.
 */
HAL_Status HAL_VOP_EnableLineIrq(struct VOP_REG *pReg,
                                 uint32_t lineFlag0, uint32_t lineFlag1)
{
    VOP_MaskWrite(NULL, &pReg->INTR_EN,
                  VOP_INTR_EN_LINE_FLAG0_INTR_EN_SHIFT,
                  VOP_INTR_EN_LINE_FLAG0_INTR_EN_MASK, 1);

    VOP_MaskWrite(NULL, &pReg->INTR_EN,
                  VOP_INTR_EN_LINE_FLAG1_INTR_EN_SHIFT,
                  VOP_INTR_EN_LINE_FLAG1_INTR_EN_MASK, 1);

    VOP_Write(&s_vopRegMir.LINE_FLAG, &pReg->LINE_FLAG,
              lineFlag1 << 16 | lineFlag0);

    return HAL_OK;
}

/**
 * @brief  Disable VOP line flag interrupt.
 * @param  pReg: VOP reg base.
 * @return HAL_Status.
 */
HAL_Status HAL_VOP_DisableLineIrq(struct VOP_REG *pReg)
{
    VOP_MaskWrite(NULL, &pReg->INTR_EN, VOP_INTR_EN_LINE_FLAG0_INTR_EN_SHIFT,
                  VOP_INTR_EN_LINE_FLAG0_INTR_EN_MASK, 0);

    VOP_MaskWrite(NULL, &pReg->INTR_EN, VOP_INTR_EN_LINE_FLAG1_INTR_EN_SHIFT,
                  VOP_INTR_EN_LINE_FLAG1_INTR_EN_MASK, 0);

    return HAL_OK;
}

/**
 * @brief  Enable VOP debug interrupt.
 * @param  pReg: VOP reg base.
 * @return HAL_Status.
 */
HAL_Status HAL_VOP_EnableDebugIrq(struct VOP_REG *pReg)
{
    VOP_MaskWrite(NULL, &pReg->INTR_EN, VOP_INTR_EN_BUS_ERROR_INTR_EN_SHIFT,
                  VOP_INTR_EN_BUS_ERROR_INTR_EN_MASK, 1);
    VOP_MaskWrite(NULL, &pReg->INTR_EN, VOP_INTR_EN_WIN0_EMPTY_INTR_EN_SHIFT,
                  VOP_INTR_EN_WIN0_EMPTY_INTR_EN_MASK, 1);
    VOP_MaskWrite(NULL, &pReg->INTR_EN, VOP_INTR_EN_WIN2_EMPTY_INTR_EN_SHIFT,
                  VOP_INTR_EN_WIN2_EMPTY_INTR_EN_MASK, 1);
    VOP_MaskWrite(NULL, &pReg->INTR_EN, VOP_INTR_EN_POST_EMPTY_INTR_EN_SHIFT,
                  VOP_INTR_EN_POST_EMPTY_INTR_EN_MASK, 1);

    return HAL_OK;
}

/**
 * @brief  Disable VOP debug interrupt.
 * @param  pReg: VOP reg base.
 * @return HAL_Status.
 */
HAL_Status HAL_VOP_DisableDebugIrq(struct VOP_REG *pReg)
{
    VOP_MaskWrite(NULL, &pReg->INTR_EN, VOP_INTR_EN_BUS_ERROR_INTR_EN_SHIFT,
                  VOP_INTR_EN_BUS_ERROR_INTR_EN_MASK, 0);
    VOP_MaskWrite(NULL, &pReg->INTR_EN, VOP_INTR_EN_WIN0_EMPTY_INTR_EN_SHIFT,
                  VOP_INTR_EN_WIN0_EMPTY_INTR_EN_MASK, 0);
    VOP_MaskWrite(NULL, &pReg->INTR_EN, VOP_INTR_EN_WIN2_EMPTY_INTR_EN_SHIFT,
                  VOP_INTR_EN_WIN2_EMPTY_INTR_EN_MASK, 0);
    VOP_MaskWrite(NULL, &pReg->INTR_EN, VOP_INTR_EN_POST_EMPTY_INTR_EN_SHIFT,
                  VOP_INTR_EN_POST_EMPTY_INTR_EN_MASK, 0);

    return HAL_OK;
}

/**
 * @brief  Enable VOP DSP hold interrupt.
 * @param  pReg: VOP reg base.
 * @return HAL_Status.
 */
HAL_Status HAL_VOP_EnableDspHoldIrq(struct VOP_REG *pReg)
{
    VOP_MaskWrite(NULL, &pReg->INTR_EN, VOP_INTR_EN_DSP_HOLD_VALID_INTR_EN_SHIFT,
                  VOP_INTR_EN_DSP_HOLD_VALID_INTR_EN_MASK, 1);

    return HAL_OK;
}

/**
 * @brief  Disable VOP DSP hold interrupt.
 * @param  pReg: VOP reg base.
 * @return HAL_Status.
 */
HAL_Status HAL_VOP_DisableDspHoldIrq(struct VOP_REG *pReg)
{
    VOP_MaskWrite(NULL, &pReg->INTR_EN, VOP_INTR_EN_DSP_HOLD_VALID_INTR_EN_SHIFT,
                  VOP_INTR_EN_DSP_HOLD_VALID_INTR_EN_MASK, 0);

    return HAL_OK;
}

/**
 * @brief  VOP interrupt handler.
 * @param  pReg: VOP reg base.
 * @return HAL_Status.
 */
uint32_t HAL_VOP_IrqHandler(struct VOP_REG *pReg)
{
    uint32_t i, val;

    val = pReg->INTR_STATUS;

    for (i = 0; i < HAL_ARRAY_SIZE(VOP_IRQs); i++) {
        /**
         * if (val & HAL_BIT(i))
         *     HAL_DBG_ERR("VOP Irq: %s\n", VOP_IRQs[i]);
         */
    }

    pReg->INTR_CLEAR = val & 0xffff;

    return val & 0xffff;
}

/**
 * @brief  VOP interrupt handler.
 * @param  pReg: VOP reg base.
 * @param  type: send cmd or data
 * @param  val: send value
 * @return HAL_Status.
 */
HAL_Status HAL_VOP_SendMcuCmd(struct VOP_REG *pReg, uint8_t type, uint32_t val)
{
    switch (type) {
    case MCU_WRCMD:
        VOP_MaskWrite(&s_vopRegMir.MCU, &pReg->MCU, VOP_MCU_MCU_RS_SHIFT,
                      VOP_MCU_MCU_RS_MASK, 0);
        VOP_Write(&s_vopRegMir.MCU_RW_BYPASS_PORT, &pReg->MCU_RW_BYPASS_PORT,
                  val);
        VOP_MaskWrite(&s_vopRegMir.MCU, &pReg->MCU, VOP_MCU_MCU_RS_SHIFT,
                      VOP_MCU_MCU_RS_MASK, 1);
        break;
    case MCU_WRDATA:
        VOP_MaskWrite(&s_vopRegMir.MCU, &pReg->MCU, VOP_MCU_MCU_RS_SHIFT,
                      VOP_MCU_MCU_RS_MASK, 1);
        VOP_Write(&s_vopRegMir.MCU_RW_BYPASS_PORT, &pReg->MCU_RW_BYPASS_PORT,
                  val);
        break;
    case MCU_SETBYPASS:
        VOP_MaskWrite(&s_vopRegMir.MCU, &pReg->MCU, VOP_MCU_MCU_BYPASS_SHIFT,
                      VOP_MCU_MCU_BYPASS_MASK, val ? 1 : 0);
        break;
    default:
        break;
    }

    return HAL_OK;
}

/** @} */

/** @} */

/** @} */

#endif /* HAL_VOP_MODULE_ENABLED */
