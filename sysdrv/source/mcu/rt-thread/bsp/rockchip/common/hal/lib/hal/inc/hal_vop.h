/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_VOP_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup VOP
 *  @{
 */

#ifndef _HAL_VOP_H_
#define _HAL_VOP_H_

/***************************** MACRO Definition ******************************/
/** @defgroup VOP_Exported_Definition_Group1 Basic Definition
 *  @{
 */

/** Add brief to here */
#define VOP_GAMMA_SIZE        4
#define VOP_COLOR_MATRIX_SIZE 3
/***************************** Structure Definition **************************/
/**
 *  Add multi line brief to here
 *  ...
 */
typedef enum {
    VOP_POST_SCL_MODE_RESEVED = 0,
    VOP_POST_SCL_MODE_X2,
    VOP_POST_SCL_MODE_X3,
    VOP_POST_SCL_MODE_X4,
} eVOP_PostSclMode;

typedef enum {
    VOP_POST_CSC_MPEG = 0,
    VOP_POST_CSC_HD,
    VOP_POST_CSC_JPEG,
} eVOP_PostCSCMode;

typedef enum {
    VOP_ALPHA_MODE_USER_DEFINED = 0,
    VOP_ALPHA_MODE_PER_PIXEL,
} eVOP_AlphaMode;

typedef enum {
    VOP_NON_PREMULT_ALPHA = 0,
    VOP_PREMULT_ALPHA,
} eVOP_PreMulAlphaMode;

typedef enum {
    VOP_MCU_SPLIT_8_BIT = 0,
    VOP_MCU_SPLIT_16_BIT,
    VOP_MCU_SPLIT_24_BIT,
    VOP_MCU_SPLIT_32_BIT,
} eVOP_McuSplit;

typedef enum vop_data_format {
    VOP_FMT_ARGB8888 = 0,
    VOP_FMT_RGB888,
    VOP_FMT_RGB565,
    VOP_FMT_RGB444,
    VOP_FMT_YUV420SP,
    VOP_FMT_YUV422SP,
    VOP_FMT_YUV444SP,
    VOP_FMT_YVYU422    = 0x8,
    VOP_FMT_VYUY422    = 0xa,
    VOP_FMT_YUV420SP_4 = 0x14,
    VOP_FMT_YUV422SP_4,
    VOP_FMT_YUV444SP_4,
    VOP_FMT_YVYU422_4 = 0x18,
    VOP_FMT_VYUY422_4 = 0x1a,
    VOP_FMT_1BPP      = 0x20,
    VOP_FMT_2BPP,
    VOP_FMT_4BPP,
    VOP_FMT_8BPP,
} eVOP_DataFormat;

typedef enum VOP_mcu_cmd {
    MCU_WRCMD = 0,
    MCU_WRDATA,
    MCU_SETBYPASS,
} eVOP_McuCmd;

typedef enum {
    SWITCH_TO_INTERNAL_DPHY,
    SWITCH_TO_AP_BYPASS,
} eVOP_MipiSwitchPath;

struct CRTC_WIN_STATE {
    uint32_t winEn : 1;
    uint32_t winUpdate : 1;
    uint32_t winId : 3;
    uint32_t zpos : 3;
    uint32_t format : 6;
    uint32_t alphaEn : 1;
    uint32_t alphaMode : 1;
    uint32_t globalAlphaValue : 8;
    uint32_t alphaPreMul : 1;
    uint32_t alphaSatMode : 1;

    uint32_t yrgbAddr;
    uint32_t cbcrAddr;
    uint32_t yrgbLength;
    uint32_t cbcrLength;
    uint16_t xVir;
    uint16_t srcX;
    uint16_t srcY;
    uint16_t srcW;
    uint16_t srcH;
    uint16_t crtcX;
    uint16_t crtcY;
    uint16_t crtcW;
    uint16_t crtcH;
    uint16_t scaleFacW;
    uint16_t scaleFacH;

    uint8_t hwFormat;
    uint16_t hwCrtcX;
    uint16_t hwCrtcY;
    uint16_t stride;

    uint16_t xLoopOffset;
    uint16_t yLoopOffset;

    uint32_t colorKey;
    uint32_t *lut;
    uint8_t cscMode;
    uint8_t uvSwap;
    uint8_t split;
};

struct VOP_BCSH_INFO {
    uint8_t brightness;
    uint8_t contrast;
    uint8_t satCon;
    uint8_t hue;
};

struct VOP_GAMMA_COE_INFO {
    bool gammaCoeEnable;
    uint8_t *gammaCoeWord;
};

struct VOP_POST_SCALE_INFO {
    uint16_t srcW;
    uint16_t srcH;

    uint16_t dstX;
    uint16_t dstY;
    uint16_t dstW;
    uint16_t dstH;
    bool postScaleEn;
    eVOP_PostSclMode postSclHmode;
    eVOP_PostSclMode postSclVmode;
};

struct VOP_POST_CSC_INFO {
    bool postCscEn;
    eVOP_PostCSCMode postCscMode;
};

struct VOP_COLOR_MATRIX_INFO {
    bool colorMatrixEn;
    uint8_t *colorMatrixCoe;
    uint8_t *colorMatrixOffset;
};

struct VOP_POST_CLIP_INFO {
    bool postClipEn;
    uint8_t postYThres;
};

/** @} */
/***************************** Function Declare ******************************/
/** @defgroup VOP_Public_Function_Declare Public Function Declare
 *  @{
 */

HAL_Status HAL_VOP_SetArea(struct VOP_REG *pReg,
                           struct DISPLAY_RECT *display_rect);
HAL_Status HAL_VOP_SetPlane(struct VOP_REG *pReg,
                            struct CRTC_WIN_STATE *pWinState,
                            struct DISPLAY_MODE_INFO *pModeInfo);

uint32_t HAL_VOP_GetScanLine(struct VOP_REG *pReg);
uint8_t HAL_VOP_CommitPrepare(struct VOP_REG *pReg);
uint8_t HAL_VOP_CommitPost(struct VOP_REG *pReg);
HAL_Status HAL_VOP_LoadLut(struct VOP_REG *pReg, uint8_t winId, uint32_t *lut,
                           uint16_t lut_size);
HAL_Status HAL_VOP_DisableLut(struct VOP_REG *pReg, uint8_t winId);
HAL_Status HAL_VOP_Commit(struct VOP_REG *pReg);
HAL_Status HAL_VOP_EdpiFrmSt(struct VOP_REG *pReg);
HAL_Status HAL_VOP_PowerOn(struct VOP_REG *pReg);
HAL_Status HAL_VOP_PowerOff(struct VOP_REG *pReg);
HAL_Status HAL_VOP_EnableFsIrq(struct VOP_REG *pReg);
HAL_Status HAL_VOP_DisableFsIrq(struct VOP_REG *pReg);
HAL_Status HAL_VOP_EnableFrmFshIrq(struct VOP_REG *pReg);
HAL_Status HAL_VOP_DisableFrmFshIrq(struct VOP_REG *pReg);
HAL_Status HAL_VOP_EnableLineIrq(struct VOP_REG *pReg,
                                 uint32_t lineFlag0, uint32_t lineFlag1);
HAL_Status HAL_VOP_DisableLineIrq(struct VOP_REG *pReg);
HAL_Status HAL_VOP_EnableDebugIrq(struct VOP_REG *pReg);
HAL_Status HAL_VOP_DisableDebugIrq(struct VOP_REG *pReg);
HAL_Status HAL_VOP_EnableDspHoldIrq(struct VOP_REG *pReg);
HAL_Status HAL_VOP_DisableDspHoldIrq(struct VOP_REG *pReg);
uint32_t HAL_VOP_IrqHandler(struct VOP_REG *pReg);
HAL_Status HAL_VOP_NocQosInit(struct VOP_REG *pReg, uint32_t nocQosValue);
HAL_Status HAL_VOP_NocHurryInit(struct VOP_REG *pReg, uint32_t hurryValue,
                                uint32_t hurryThreshold);
HAL_Status HAL_VOP_AxiOutstandInit(struct VOP_REG *pReg, uint32_t outStandNum);
HAL_Status HAL_VOP_Init(struct VOP_REG *pReg,
                        struct DISPLAY_MODE_INFO *pModeInfo);
HAL_Status HAL_VOP_DeInit(struct VOP_REG *pReg);
HAL_Status HAL_VOP_ModeInit(struct VOP_REG *pReg,
                            struct DISPLAY_MODE_INFO *pModeInfo,
                            struct VOP_POST_SCALE_INFO *pPostScaleInfo);
HAL_Status HAL_VOP_DscInit(struct VOP_REG *pReg,
                           struct DISPLAY_MODE_INFO *pModeInfo);
HAL_Status HAL_VOP_McuModeInit(struct VOP_REG *pReg);
HAL_Status HAL_VOP_MipiSwitch(struct VOP_REG *pReg, eVOP_MipiSwitchPath path);
HAL_Status HAL_VOP_PostScaleInit(struct VOP_REG *pReg,
                                 struct DISPLAY_MODE_INFO *pModeInfo,
                                 struct VOP_POST_SCALE_INFO *pPostScaleInfo);
HAL_Status HAL_VOP_PostBCSH(struct VOP_REG *pReg,
                            struct VOP_BCSH_INFO *pBCSHInfo);
HAL_Status HAL_VOP_PostGammaInit(struct VOP_REG *pReg,
                                 struct VOP_GAMMA_COE_INFO *pGammaInfo);
HAL_Status HAL_VOP_PostColorMatrixInit(struct VOP_REG *pReg,
                                       struct VOP_COLOR_MATRIX_INFO *pColorMatrixInfo);
HAL_Status HAL_VOP_PostClipInit(struct VOP_REG *pReg,
                                struct VOP_POST_CLIP_INFO *pClipInfo);
HAL_Status HAL_VOP_PolarityInit(struct VOP_REG *pReg,
                                struct DISPLAY_MODE_INFO *pModeInfo,
                                uint16_t ConnectorType);
HAL_Status HAL_VOP_OutputInit(struct VOP_REG *pReg,
                              struct DISPLAY_MODE_INFO *pModeInfo,
                              uint16_t BusFormat);
HAL_Status HAL_VOP_EdpiInit(struct VOP_REG *pReg, uint8_t triggerMode);
HAL_Status HAL_VOP_SendMcuCmd(struct VOP_REG *pReg, uint8_t type, uint32_t val);
HAL_Status HAL_VOP_SetSplit(struct VOP_REG *pReg, eVOP_McuSplit mode);

/** @} */

#endif

/** @} */

/** @} */

#endif /* HAL_VOP_MODULE_ENABLED */
