/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_CRU_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup CRU
 *  @{
 */

#ifndef _HAL_CRU_H_
#define _HAL_CRU_H_

#include "hal_def.h"

/*************************** MACRO Definition ****************************/
/** @defgroup CRU_Exported_Definition_Group1 Basic Definition
 *  @{
 */

#ifdef HAL_CRU_DBG_ON
#define HAL_CRU_DBG(fmt, arg...) HAL_SYSLOG("[HAL CRU] " fmt, ##arg)
#else
#define HAL_CRU_DBG(fmt, arg...) do { if (0) HAL_SYSLOG("[HAL CRU] " fmt, ##arg); } while (0)
#endif

#define MHZ 1000000
#define KHZ 1000

#ifndef PLL_INPUT_OSC_RATE
#define PLL_INPUT_OSC_RATE (24 * MHZ)
#endif

#define GENVAL_D16(x, h, l)     ((uint32_t)(((x) & HAL_GENMASK(h, l)) / 16))
#define GENVAL_D16_REM(x, h, l) ((uint32_t)(((x) & HAL_GENMASK(h, l)) % 16))
#define WIDTH_TO_MASK(w)        ((1 << (w)) - 1)

/*
 * RESET/GATE fields:
 *   [31:16]: reserved
 *   [15:12]: bank
 *   [11:0]:  id
 */
#define CLK_RESET_GET_REG_OFFSET(x) GENVAL_D16(x, 11, 0)
#define CLK_RESET_GET_BITS_SHIFT(x) GENVAL_D16_REM(x, 11, 0)
#define CLK_RESET_GET_REG_BANK(x)   HAL_GENVAL(x, 15, 12)

#define CLK_GATE_GET_REG_OFFSET(x) CLK_RESET_GET_REG_OFFSET(x)
#define CLK_GATE_GET_BITS_SHIFT(x) CLK_RESET_GET_BITS_SHIFT(x)
#define CLK_GATE_GET_REG_BANK(x)   CLK_RESET_GET_REG_BANK(x)

/*
 * MUX/DIV fields:
 *   [31:24]: width
 *   [23:16]: shift
 *   [15:12]: reserved
 *   [11:8]:  bank
 *   [7:0]:   reg
 */
#define CLK_MUX_GET_REG_OFFSET(x) HAL_GENVAL(x, 7,  0)
#define CLK_MUX_GET_BANK(x)       HAL_GENVAL(x, 11, 8)
#define CLK_MUX_GET_BITS_SHIFT(x) HAL_GENVAL(x, 23, 16)
#define CLK_MUX_GET_WIDTH(x)      HAL_GENVAL(x, 31, 24)
#define CLK_MUX_GET_MASK(x)       (WIDTH_TO_MASK(CLK_MUX_GET_WIDTH(x)) << CLK_MUX_GET_BITS_SHIFT(x))

#define CLK_DIV_GET_REG_OFFSET(x) CLK_MUX_GET_REG_OFFSET(x)
#define CLK_DIV_GET_BANK(x)       CLK_MUX_GET_BANK(x)
#define CLK_DIV_GET_BITS_SHIFT(x) CLK_MUX_GET_BITS_SHIFT(x)
#define CLK_DIV_GET_WIDTH(x)      CLK_MUX_GET_WIDTH(x)
#define CLK_DIV_GET_MASK(x)       CLK_MUX_GET_MASK(x)
#define CLK_DIV_GET_MAXDIV(x)     ((1 << CLK_DIV_GET_WIDTH(x)) - 1)

/*
 * v64 mux = v32 | bank(in bit[35:32])
 * v64 div = v32 | bank(in bit[39:36])
 */
#ifdef CRU_CLK_USE_CON_BANK
#define CLK_GET_MUX(v64) ((uint32_t)(((v64) & 0xFFFFFFFF00000000) >> 32))
#define CLK_GET_DIV(v64) ((uint32_t)((v64) & 0x00000000FFFFFFFF))
#else
#define CLK_GET_MUX(v32) ((uint32_t)((v32) & 0x0F0F00FFU))
#define CLK_GET_DIV(v32) ((uint32_t)((((v32) & 0x0000FF00U) >> 8) | \
                                     (((v32) & 0xF0F00000U) >> 4)))
#endif

#define RK_PLL_RATE(_rate, _refdiv, _fbdiv, _postdiv1, _postdiv2, _dsmpd, \
                    _frac)                                                \
    {                                                                     \
        .rate = _rate##U, .fbDiv = _fbdiv, .postDiv1 = _postdiv1,         \
        .refDiv = _refdiv, .postDiv2 = _postdiv2, .dsmpd = _dsmpd,        \
        .frac = _frac,                                                    \
    }

#define RK3588_PLL_RATE(_rate, _p, _m, _s, _k) \
    {                                          \
        .rate = _rate##U, .p = _p, .m = _m,    \
        .s = _s, .k = _k,                      \
    }

#define CRU_BANK_CFG_FLAGS(reg, sel, gate, soft) \
    {                                            \
        .cruBase = reg,                          \
        .selOffset = sel,                        \
        .gateOffset = gate,                      \
        .softOffset = soft,                      \
    }

struct PLL_CONFIG {
    uint32_t rate;

    union {
        struct {
            uint32_t fbDiv;
            uint32_t postDiv1;
            uint32_t refDiv;
            uint32_t postDiv2;
            uint32_t dsmpd;
            uint32_t frac;
        };
        struct {
            uint32_t m;
            uint32_t p;
            uint32_t s;
            uint32_t k;
        };
    };
};

struct PLL_SETUP {
    __IO uint32_t *conOffset0;
    __IO uint32_t *conOffset1;
    __IO uint32_t *conOffset2;
    __IO uint32_t *conOffset3;
    __IO uint32_t *conOffset6;
    __IO uint32_t *modeOffset;
    __I uint32_t *stat0;
    uint32_t modeShift;
    uint32_t lockShift;
    uint32_t modeMask;
    const struct PLL_CONFIG *rateTable;
};

typedef enum {
    GLB_SRST_FST = 0xfdb9,
    GLB_SRST_SND = 0xeca8,
} eCRU_GlbSrstType;

typedef enum {
    GLB_RST_FST_WDT0 = 0U,
    GLB_RST_SND_WDT0,
    GLB_RST_FST_WDT1,
    GLB_RST_SND_WDT1,
    GLB_RST_FST_WDT2,
    GLB_RST_SND_WDT2,
} eCRU_WdtRstType;

struct CRU_BANK_INFO {
    uint32_t cruBase;
    uint32_t selOffset;
    uint32_t gateOffset;
    uint32_t softOffset;
};

struct HAL_CRU_DEV {
    const struct CRU_BANK_INFO *banks;
    uint8_t banksNum;
};

extern const struct HAL_CRU_DEV g_cruDev;

/***************************** Structure Definition **************************/

/** @} */
/***************************** Function Declare ******************************/
/** @defgroup CRU_Private_Function_Declare Private Function Declare
 *  @{
 */

#define _MHZ(n) ((n) * 1000000)
#define DIV_NO_REM(pFreq, freq, maxDiv) \
    ((!((pFreq) % (freq))) && ((pFreq) / (freq) <= (maxDiv)))

int HAL_CRU_FreqGetMux4(uint32_t freq, uint32_t freq0, uint32_t freq1,
                        uint32_t freq2, uint32_t freq3);
int HAL_CRU_FreqGetMux3(uint32_t freq, uint32_t freq0, uint32_t freq1,
                        uint32_t freq2);
int HAL_CRU_FreqGetMux2(uint32_t freq, uint32_t freq0, uint32_t freq1);

uint32_t HAL_CRU_MuxGetFreq4(uint32_t muxName, uint32_t freq0, uint32_t freq1,
                             uint32_t freq2, uint32_t freq3);
uint32_t HAL_CRU_MuxGetFreq3(uint32_t muxName, uint32_t freq0, uint32_t freq1,
                             uint32_t freq2);
uint32_t HAL_CRU_MuxGetFreq2(uint32_t muxName, uint32_t freq0, uint32_t freq1);

int HAL_CRU_RoundFreqGetMux4(uint32_t freq, uint32_t pFreq0, uint32_t pFreq1,
                             uint32_t pFreq2, uint32_t pFreq3, uint32_t *pFreqOut);
int HAL_CRU_RoundFreqGetMux3(uint32_t freq, uint32_t pFreq0, uint32_t pFreq1,
                             uint32_t pFreq2, uint32_t *pFreqOut);
int HAL_CRU_RoundFreqGetMux2(uint32_t freq, uint32_t pFreq0, uint32_t pFreq1,
                             uint32_t *pFreqOut);

/** @} */

/** @defgroup CRU_Public_Function_Declare Public Function Declare
 *  @{
 */

/**
 * @brief Get pll freq.
 * @param  pSetup: Contains PLL register parameters
 * @return pll rate.
 */
uint32_t HAL_CRU_GetPllFreq(struct PLL_SETUP *pSetup);

/**
 * @brief Set pll freq.
 * @param  pSetup: Contains PLL register parameters
 * @param  rate: pll set
 * @return HAL_Status.
 */
HAL_Status HAL_CRU_SetPllFreq(struct PLL_SETUP *pSetup, uint32_t rate);

/**
 * @brief Set pll power up.
 * @param  pSetup: Contains PLL register parameters
 * @return HAL_Status.
 */
HAL_Status HAL_CRU_SetPllPowerUp(struct PLL_SETUP *pSetup);

/**
 * @brief Set pll power down.
 * @param  pSetup: Contains PLL register parameters
 * @return HAL_Status.
 */
HAL_Status HAL_CRU_SetPllPowerDown(struct PLL_SETUP *pSetup);

/**
 * @brief Check if clk is enabled
 * @param  clk: clock to check
 * @return HAL_Check.
 */
HAL_Check HAL_CRU_ClkIsEnabled(uint32_t clk);

/**
 * @brief Enable clk
 * @param  clk: clock to set
 * @return HAL_Status.
 */
HAL_Status HAL_CRU_ClkEnable(uint32_t clk);

/**
 * @brief Disable unused clk
 * @param  bank: cru bank id
 * @param  index: gate con offset
 * @param  val: gate value
 * @return HAL_Status.
 */
HAL_Status HAL_CRU_ClkDisableUnused(uint32_t bank, uint32_t index, uint32_t val);

/**
 * @brief Disable clk
 * @param  clk: clock to set
 * @return HAL_Status.
 */
HAL_Status HAL_CRU_ClkDisable(uint32_t clk);

/**
 * @brief Check if clk is reset
 * @param  clk: clock to check
 * @return HAL_Check.
 */
HAL_Check HAL_CRU_ClkIsReset(uint32_t clk);

/**
 * @brief Assert the reset to the clk
 * @param  clk: clock to assert
 * @return HAL_Status.
 */
HAL_Status HAL_CRU_ClkResetAssert(uint32_t clk);

/**
 * @brief Deassert the reset to the clk
 * @param  clk: clock to deassert
 * @return HAL_Status.
 */
HAL_Status HAL_CRU_ClkResetDeassert(uint32_t clk);

/**
 * @brief Sync Assert the resets to the clk
 * @param  numClks: num clocks to assert
 * @param  clks: clocks to assert
 * @return HAL_Status.
 */
HAL_Status HAL_CRU_ClkResetSyncAssert(int numClks, uint32_t *clks);

/**
 * @brief Sync Deassert the resets to the clk
 * @param  numClks: num clocks to assert
 * @param  clks: clocks to deassert
 * @return HAL_Status.
 */
HAL_Status HAL_CRU_ClkResetSyncDeassert(int numClks, uint32_t *clks);

/**
 * @brief  Set frac div
 * @param  fracDivName: frac div id(Contains div offset, shift, mask information)
 * @param  numerator: the numerator to set.
 * @param  denominator: the denominator to set.
 * @return HAL_Status
 */
HAL_Status HAL_CRU_ClkSetFracDiv(uint32_t fracDivName,
                                 uint32_t numerator,
                                 uint32_t denominator);

/**
 * @brief  Get frac div
 * @param  fracDivName: frac div id(Contains div offset, shift, mask information)
 * @param  numerator: the returned numerator.
 * @param  denominator: the returned denominator.
 * @return HAL_Status
 */
HAL_Status HAL_CRU_ClkGetFracDiv(uint32_t fracDivName,
                                 uint32_t *numerator,
                                 uint32_t *denominator);

/**
 * @brief  Set integer div
 * @param  divName: div id(Contains div offset, shift, mask information)
 * @param  divValue: div value
 * @return NONE
 */
HAL_Status HAL_CRU_ClkSetDiv(uint32_t divName, uint32_t divValue);

/**
 * @brief  Get integer div
 * @param  divName: div id (Contains div offset, shift, mask information)
 * @return div value
 */
uint32_t HAL_CRU_ClkGetDiv(uint32_t divName);

/**
 * @brief  Set mux
 * @param  muxName: mux id (Contains mux offset, shift, mask information)
 * @param  muxValue: mux value
 * @return NONE
 */
HAL_Status HAL_CRU_ClkSetMux(uint32_t muxName, uint32_t muxValue);

/**
 * @brief  Get mux
 * @param  muxName: mux id (Contains mux offset, shift, mask information)
 * @return mux value
 */
uint32_t HAL_CRU_ClkGetMux(uint32_t muxName);

/**
 * @brief  Get frac div config.
 * @param  rateOut: clk out rate.
 * @param  rate: clk src rate.
 * @param  numerator: the returned numerator.
 * @param  denominator: the returned denominator.
 * @return HAL_Status.
 */
HAL_Status HAL_CRU_FracdivGetConfig(uint32_t rateOut, uint32_t rate,
                                    uint32_t *numerator,
                                    uint32_t *denominator);
/**
 * @brief Get clk freq.
 * @param  clockName: CLOCK_Name id.
 * @return rate.
 * @attention these APIs allow direct use in the HAL layer.
 */
uint32_t HAL_CRU_ClkGetFreq(eCLOCK_Name clockName);

/**
 * @brief Set clk freq.
 * @param  clockName: CLOCK_Name id.
 * @param  rate: clk rate.
 * @return HAL_Status.
 * @attention these APIs allow direct use in the HAL layer.
 */
HAL_Status HAL_CRU_ClkSetFreq(eCLOCK_Name clockName, uint32_t rate);

/**
 * @brief vop dclk enable.
 * @param  gateId: gate id
 * @return HAL_Status.
 * @attention these APIs allow direct use in the HAL layer.
 */
HAL_Status HAL_CRU_VopDclkEnable(uint32_t gateId);

/**
 * @brief vop dclk disable.
 * @param  gateId: gate id
 * @return HAL_Status.
 * @attention these APIs allow direct use in the HAL layer.
 */
HAL_Status HAL_CRU_VopDclkDisable(uint32_t gateId);

/**
 * @brief  Get Np5 best div.
 * @param  clockName: clk id.
 * @param  rate: clk rate.
 * @param  pRate: clk parent rate
 * @param  bestdiv: the returned bestdiv.
 * @return HAL_Status.
 */
HAL_Status HAL_CRU_ClkNp5BestDiv(eCLOCK_Name clockName, uint32_t rate, uint32_t pRate, uint32_t *bestdiv);

/**
 * @brief  assert CRU global software reset.
 * @param  type: global software reset type.
 * @return HAL_INVAL if the SoC does not support.
 */
HAL_Status HAL_CRU_SetGlbSrst(eCRU_GlbSrstType type);

/**
 * @brief wdt glbrst enable.
 * @param  wdtType: wdt reset type.
 * @return HAL_OK.
 * @attention these APIs allow direct use in the HAL layer.
 */
HAL_Status HAL_CRU_WdtGlbRstEnable(eCRU_WdtRstType wdtType);

/**
 * @brief pll output freq Compensation.
 * @param  clockName: CLOCK_Name id.
 * @param  ppm: Efforts to compensate.
 * @return HAL_OK.
 * @attention these APIs allow direct use in the HAL layer.
 */
HAL_Status HAL_CRU_PllCompensation(eCLOCK_Name clockName, int ppm);

/**
 * @brief CRU suspend.
 * @return HAL_Status.
 * @attention these APIs allow direct use in the HAL layer.
 */
HAL_Status HAL_CRU_Suspend(void);

/**
 * @brief CRU resume.
 * @return HAL_Status.
 * @attention these APIs allow direct use in the HAL layer.
 */
HAL_Status HAL_CRU_Resume(void);

#ifdef HAL_CRU_AS_FEATURE_ENABLED
/**
 * @brief  it is for AS init.
 */
void HAL_CRU_AsInit(void);

/**
 * @brief  it is for AS enable.
 * @param  ch: channel
 * @param  en: 1 is enable, 0 is disable.
 */
void HAL_CRU_AsEnable(uint8_t ch, uint8_t en);
#endif

/** @} */

#endif

/** @} */

/** @} */

#endif /* HAL_CRU_MODULE_ENABLED */
