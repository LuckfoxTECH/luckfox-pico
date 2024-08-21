/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_DSP_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup DSP
 *  @{
 */

#ifndef _HAL_DSP_H_
#define _HAL_DSP_H_

#include "hal_def.h"

/***************************** MACRO Definition ******************************/
/** @defgroup DSP_Exported_Definition_Group1 Basic Definition
 *  @{
 */
#define DSP_IOCTL(a) (0x1000 + (a))

/** general cmd for dsp control */
#define DSP_IOCTL_SET_ITCM_SIZE  DSP_IOCTL(1)
#define DSP_IOCTL_SET_DTCM_SIZE  DSP_IOCTL(2)
#define DSP_IOCTL_SET_MEM_GATING DSP_IOCTL(3)
#define DSP_IOCTL_SET_DVFS_ST    DSP_IOCTL(4)

#define DSP_TCM_SEL(a) (0x1 << (a))

/**
 * enum DSP_TcmSel - dsp tcm select.
 */
#if (defined(RKMCU_RK2108) || defined(RKMCU_PISCES))
typedef enum {
    DSP_DTCM0 = DSP_TCM_SEL(0),    /**< 0x30200000 ~ 0x30210000 */
    DSP_DTCM1 = DSP_TCM_SEL(1),    /**< 0x30210000 ~ 0x30220000 */
    DSP_DTCM2 = DSP_TCM_SEL(2),    /**< 0x30220000 ~ 0x30230000 */
    DSP_DTCM3 = DSP_TCM_SEL(3),    /**< 0x30230000 ~ 0x30240000 */
    DSP_DTCM4 = DSP_TCM_SEL(4),    /**< 0x30240000 ~ 0x30250000 */
    DSP_DTCM5 = DSP_TCM_SEL(5),    /**< 0x30250000 ~ 0x30260000 */
    DSP_DTCM6 = DSP_TCM_SEL(6),    /**< 0x30260000 ~ 0x30270000 */
    DSP_DTCM7 = DSP_TCM_SEL(7),    /**< 0x30270000 ~ 0x30280000 */
    DSP_ITCM  = DSP_TCM_SEL(8),    /**< 0x30000000 ~ 0x30010000 */
} eDSP_tcmSel;

#define DSP_DTCM (DSP_DTCM0 | DSP_DTCM1 | DSP_DTCM2 | DSP_DTCM3 | \
                  DSP_DTCM4 | DSP_DTCM5 | DSP_DTCM6 | DSP_DTCM7)
#else
typedef enum {
    DSP_ITCM0  = DSP_TCM_SEL(0),    /**< 0x20400000 ~ 0x20404000 */
    DSP_ITCM1  = DSP_TCM_SEL(1),    /**< 0x20404000 ~ 0x20408000 */
    DSP_DTCM0  = DSP_TCM_SEL(4),    /**< 0x20600000 ~ 0x20604000 */
    DSP_DTCM1  = DSP_TCM_SEL(5),    /**< 0x20604000 ~ 0x20608000 */
    DSP_DTCM2  = DSP_TCM_SEL(6),    /**< 0x20608000 ~ 0x2060c000 */
    DSP_DTCM3  = DSP_TCM_SEL(7),    /**< 0x2060c000 ~ 0x20610000 */
    DSP_DTCM4  = DSP_TCM_SEL(8),    /**< 0x20610000 ~ 0x20614000 */
    DSP_DTCM5  = DSP_TCM_SEL(9),    /**< 0x20614000 ~ 0x20618000 */
    DSP_DTCM6  = DSP_TCM_SEL(10),   /**< 0x20618000 ~ 0x2061c000 */
    DSP_DTCM7  = DSP_TCM_SEL(11),   /**< 0x2061c000 ~ 0x20620000 */
    DSP_DTCM8  = DSP_TCM_SEL(12),   /**< 0x20620000 ~ 0x20624000 */
    DSP_DTCM9  = DSP_TCM_SEL(13),   /**< 0x20624000 ~ 0x20628000 */
    DSP_DTCM10 = DSP_TCM_SEL(14),   /**< 0x20628000 ~ 0x2062c000 */
    DSP_DTCM11 = DSP_TCM_SEL(15),   /**< 0x2062c000 ~ 0x20630000 */
} eDSP_tcmSel;

#define DSP_DTCM (DSP_DTCM0 | DSP_DTCM1 | DSP_DTCM2 | DSP_DTCM3 |  \
                  DSP_DTCM4 | DSP_DTCM5 | DSP_DTCM6 | DSP_DTCM7  | \
                  DSP_DTCM8 | DSP_DTCM9 | DSP_DTCM10 | DSP_DTCM11)
#define DSP_ITCM (DSP_ITCM0 | DSP_ITCM1)
#endif

#define DSP_TCM (DSP_DTCM | DSP_ITCM)

/***************************** Structure Definition **************************/

/**
 * enum DSP_TcmMode - dsp memory work mode.
 */
typedef enum    {
    NOR_MODE = 0,
    RET1N_MODE,
    RET2N_MODE,
    PWR_DOWN_MODE
} eDSP_tcmMode;

typedef enum {
    ST_DSPAPM_NORMAL = 0,
    ST_DSPAPM_IDLW,
    ST_DSPAPM_TCMPD_PRE,
    ST_DSPAPM_TCMPD,
    ST_DSPAPM_PD,
    ST_DSPAPM_WAIT,
    ST_DSPAPM_LDOADJ,
    ST_DSPAPM_TCMUP,
    ST_DSPAPM_UP,
    ST_DSPAPM_DEIDLE
} eDSP_powerSt;

typedef enum    {
    DSP_RESET_MODE_RESET_ALL_CLK  = 0, /**< reset all clk of dsp */
    DSP_RESET_MODE_RESET_PART_CLK = 1, /**< reset parts of clk of dsp */
    DSP_RESET_MODE_NOT_RESET      = 2, /**< do not reset any clk of dsp, dtcm is used by m3 */
} eDSP_resetMode;

struct DSP_OPS {
    int (*ioctl)(void *priv, int cmd, void *arg);
};

/**
 * struct DSP_DEV - dsp dev struct.
 */
struct DSP_DEV {
    struct GRF_REG *grfReg;    /**< grf register base */
    const struct DSP_OPS *ops; /**< dsp ops function */
    eDSP_resetMode resetFlag; /**< dsp have been reset flag */
    uint8_t mboxIsA2B;       /**< 1: AP to BB; 0: BB to AP */
    struct MBOX_REG *mboxReg;
    uint8_t errorIrq;
    int mboxIrq[MBOX_CHAN_CNT];
    eCLOCK_Name clkId;
    void *privData; /**< dsp dev privData */
};

/** @} */
/***************************** Function Declare ******************************/
/** @defgroup DSP_Public_Function_Declare Public Function Declare
 *  @{
 */

/** should hold lock to call these functions */

/**
 * @brief  Get dsp power state.
 * @return eDSP_powerSt
 */
eDSP_powerSt HAL_DSP_GetPowerSt(void);

/**
 * @brief  Wait dsp power status be set.
 * @param  status: wait status be set to this value.
 * @param  timeout: timeout valuse, unit is ms.
 * @return HAL_Status
 */
HAL_Status HAL_DSP_WaitForPowerSt(eDSP_powerSt status, uint32_t timeout);

/**
 * @brief  Soft interrupt wakeup dsp.
 * @return HAL_Status
 */
HAL_Status HAL_DSP_SoftWakeup(void);

/**
 * @brief  Set tcm work mode for retention or power down.
 * @param  tcmSel: tcm "|" combination, such as DSP_DTCM0 | DSP_ITCM.
 * @param  mode: work mode in eDSP_tCMMODE.
 * @return HAL_Status
 */
HAL_Status HAL_DSP_SetTcmMode(uint32_t tcmSel, eDSP_tcmMode mode);

/**
 * @brief  Control clk, pd and tcm on.
 * @param  dsp: dsp device point.
 * @return HAL_Status
 */
HAL_Status HAL_DSP_PowerOn(struct DSP_DEV *dsp);

/**
 * @brief  Control clk, pd and tcm off.
 * @param  dsp: dsp device point.
 * @return HAL_Status
 */
HAL_Status HAL_DSP_PowerOff(struct DSP_DEV *dsp);

/**
 * @brief  Init dsp and deassert reset.
 * @param  dsp: the handle of dsp.
 * @return HAL_Status
 */
HAL_Status HAL_DSP_Init(struct DSP_DEV *dsp);

/**
 * @brief  DeInit dsp.
 * @param  dsp: the handle of dsp.
 * @return HAL_Status
 */
HAL_Status HAL_DSP_DeInit(struct DSP_DEV *dsp);

/**
 * @brief  Enable dsp.
 * @param  dsp: the handle of dsp.
 * @param  altAddr: alternate reset vector address.
 * @return HAL_Status
 */
HAL_Status HAL_DSP_Enable(struct DSP_DEV *dsp, uint32_t altAddr);

/**
 * @brief  Disable dsp.
 * @param  dsp: the handle of dsp.
 * @return HAL_Status
 */
HAL_Status HAL_DSP_Disable(struct DSP_DEV *dsp);

/**
 * @brief  Start dsp.
 * @param  dsp: the handle of dsp.
 * @return HAL_Status
 */
HAL_Status HAL_DSP_Start(struct DSP_DEV *dsp);

/**
 * @brief  Stop dsp.
 * @param  dsp: the handle of dsp.
 * @return HAL_Status
 */
HAL_Status HAL_DSP_Stop(struct DSP_DEV *dsp);

/** @} */

#endif

/** @} */

/** @} */

#endif /* HAL_DSP_MODULE_ENABLED */
