/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_CRU_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup CRU
 *  @{
 */

/** @defgroup CRU_How_To_Use How To Use
 *  @{

 The CRU driver can be used as follows:

 - Invoke cru functions to set clk rate, enable or disable clk, reset clk in each device.
 - The gate and soft reset id is include register offset and shift information:

       con_offset: id /16
       shift: id %16

 - The mux and div id is include register offset, shift, mask information:

       [15:0]: con
       [23:16]: shift
       [31:24]: width

 - CRU driver is just responsible for passing simple command data, And
 the usecount is the user's responsibility. Protection the usecount at the driver layer.
 - More details refer to APIs' descriptions as below.

 @} */

/** @defgroup CRU_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/
#if defined(SOC_RV1108)
#define PWRDOWN_SHIFT      0
#define PWRDOWN_MASK       1 << PWRDOWN_SHIFT
#define PLL_POSTDIV1_SHIFT 8
#define PLL_POSTDIV1_MASK  0x7 << PLL_POSTDIV1_SHIFT
#define PLL_FBDIV_SHIFT    0
#define PLL_FBDIV_MASK     0xfff << PLL_FBDIV_SHIFT
#define PLL_POSTDIV2_SHIFT 12
#define PLL_POSTDIV2_MASK  0x7 << PLL_POSTDIV2_SHIFT
#define PLL_REFDIV_SHIFT   0
#define PLL_REFDIV_MASK    0x3f << PLL_REFDIV_SHIFT
#define PLL_DSMPD_SHIFT    3
#define PLL_DSMPD_MASK     1 << PLL_DSMPD_SHIFT
#define PLL_FRAC_SHIFT     0
#define PLL_FRAC_MASK      0xffffff << PLL_FRAC_SHIFT
#elif defined(SOC_RK3588)
#define PLLCON0_M_SHIFT     0
#define PLLCON0_M_MASK      0x3ff << PLLCON0_M_SHIFT
#define PLLCON1_P_SHIFT     0
#define PLLCON1_P_MASK      0x3f << PLLCON1_P_SHIFT
#define PLLCON1_S_SHIFT     6
#define PLLCON1_S_MASK      0x7 << PLLCON1_S_SHIFT
#define PLLCON2_K_SHIFT     0
#define PLLCON2_K_MASK      0xffff << PLLCON2_K_SHIFT
#define PLLCON1_PWRDOWN     BIT(13)
#define PLLCON6_LOCK_STATUS BIT(15)

#define BYPASS_SHIFT 15
#define BYPASS_MASK  (1 << BYPASS_SHIFT)

#define PWRDOWN_SHIFT 13
#define PWRDOWN_MASK  (1 << PWRDOWN_SHIFT)

#else
#define PWRDOWN_SHIFT      13
#define PWRDOWN_MASK       1 << PWRDOWN_SHIFT
#define PLL_POSTDIV1_SHIFT 12
#define PLL_POSTDIV1_MASK  0x7 << PLL_POSTDIV1_SHIFT
#define PLL_FBDIV_SHIFT    0
#define PLL_FBDIV_MASK     0xfff << PLL_FBDIV_SHIFT
#define PLL_POSTDIV2_SHIFT 6
#define PLL_POSTDIV2_MASK  0x7 << PLL_POSTDIV2_SHIFT
#define PLL_REFDIV_SHIFT   0
#define PLL_REFDIV_MASK    0x3f << PLL_REFDIV_SHIFT
#define PLL_DSMPD_SHIFT    12
#define PLL_DSMPD_MASK     1 << PLL_DSMPD_SHIFT
#define PLL_FRAC_SHIFT     0
#define PLL_FRAC_MASK      0xffffff << PLL_FRAC_SHIFT
#endif

#define MIN_FOUTVCO_FREQ (800 * MHZ)
#define MAX_FOUTVCO_FREQ (2000 * MHZ)
#define MIN_FOUT_FREQ    (24 * MHZ)
#define MAX_FOUT_FREQ    (1600 * MHZ)

#define EXPONENT_OF_FRAC_PLL              24
#define RK_PLL_MODE_SLOW                  0
#define RK_PLL_MODE_NORMAL                1
#define RK_PLL_MODE_DEEP                  2
#define PLL_GET_PLLMODE(val, shift, mask) (((uint32_t)(val) & mask) >> shift)

#define PLL_GET_FBDIV(x) (((uint32_t)(x) & (PLL_FBDIV_MASK)) >> PLL_FBDIV_SHIFT)
#define PLL_GET_REFDIV(x) \
    (((uint32_t)(x) & (PLL_REFDIV_MASK)) >> PLL_REFDIV_SHIFT)
#define PLL_GET_POSTDIV1(x) \
    (((uint32_t)(x) & (PLL_POSTDIV1_MASK)) >> PLL_POSTDIV1_SHIFT)
#define PLL_GET_POSTDIV2(x) \
    (((uint32_t)(x) & (PLL_POSTDIV2_MASK)) >> PLL_POSTDIV2_SHIFT)
#define PLL_GET_DSMPD(x) (((uint32_t)(x) & (PLL_DSMPD_MASK)) >> PLL_DSMPD_SHIFT)
#define PLL_GET_FRAC(x)  (((uint32_t)(x) & (PLL_FRAC_MASK)) >> PLL_FRAC_SHIFT)

#define CRU_PLL_ROUND_UP_TO_KHZ(x) (HAL_DIV_ROUND_UP((x), KHZ) * KHZ)

#define CRU_READ(r)           (*(volatile uint32_t *)(r))
#define CRU_WRITE(r, b, w, v) (*(volatile uint32_t *)(r) = ((w) << (16) | (v) << (b)))

/********************* Private Structure Definition **************************/
static struct PLL_CONFIG g_rockchipAutoTable;
__WEAK const struct HAL_CRU_DEV g_cruDev;

/********************* Private Variable Definition ***************************/
/********************* Private Function Definition ***************************/

/** Calculate the greatest common divisor */
static uint32_t CRU_Gcd(uint32_t m, uint32_t n)
{
    int t;

    while (m > 0) {
        if (n > m) {
            t = m;
            m = n;
            n = t;
        }
        m -= n;
    }

    return n;
}

static int isBetterFreq(uint32_t now, uint32_t new, uint32_t best)
{
    return (new <= now && new > best);
}

int HAL_CRU_FreqGetMux4(uint32_t freq, uint32_t freq0, uint32_t freq1,
                        uint32_t freq2, uint32_t freq3)
{
    uint32_t best = 0;

    if (isBetterFreq(freq, freq0, best)) {
        best = freq0;
    }

    if (isBetterFreq(freq, freq1, best)) {
        best = freq1;
    }

    if (isBetterFreq(freq, freq2, best)) {
        best = freq2;
    }

    if (isBetterFreq(freq, freq3, best)) {
        best = freq3;
    }

    if (best == freq0) {
        return 0;
    } else if (best == freq1) {
        return 1;
    } else if (best == freq2) {
        return 2;
    } else if (best == freq3) {
        return 3;
    }

    return HAL_INVAL;
}

int HAL_CRU_FreqGetMux3(uint32_t freq, uint32_t freq0, uint32_t freq1, uint32_t freq2)
{
    return HAL_CRU_FreqGetMux4(freq, freq0, freq1, freq2, freq2);
}

int HAL_CRU_FreqGetMux2(uint32_t freq, uint32_t freq0, uint32_t freq1)
{
    return HAL_CRU_FreqGetMux4(freq, freq0, freq1, freq1, freq1);
}

uint32_t HAL_CRU_MuxGetFreq4(uint32_t muxName, uint32_t freq0, uint32_t freq1,
                             uint32_t freq2, uint32_t freq3)
{
    switch (HAL_CRU_ClkGetMux(muxName)) {
    case 0:

        return freq0;
    case 1:

        return freq1;
    case 2:

        return freq2;
    case 3:

        return freq3;
    }

    return HAL_INVAL;
}

uint32_t HAL_CRU_MuxGetFreq3(uint32_t muxName, uint32_t freq0,
                             uint32_t freq1, uint32_t freq2)
{
    return HAL_CRU_MuxGetFreq4(muxName, freq0, freq1, freq2, freq2);
}

uint32_t HAL_CRU_MuxGetFreq2(uint32_t muxName, uint32_t freq0, uint32_t freq1)
{
    return HAL_CRU_MuxGetFreq4(muxName, freq0, freq1, freq1, freq1);
}

int HAL_CRU_RoundFreqGetMux4(uint32_t freq, uint32_t pFreq0,
                             uint32_t pFreq1, uint32_t pFreq2,
                             uint32_t pFreq3, uint32_t *pFreqOut)
{
    uint32_t mux;

    if (pFreq3 && (pFreq3 % freq == 0)) {
        *pFreqOut = pFreq3;
        mux = 3;
    } else if (pFreq2 && (pFreq2 % freq == 0)) {
        *pFreqOut = pFreq2;
        mux = 2;
    } else if (pFreq1 % freq == 0) {
        *pFreqOut = pFreq1;
        mux = 1;
    } else {
        *pFreqOut = pFreq0;
        mux = 0;
    }

    return mux;
}

int HAL_CRU_RoundFreqGetMux3(uint32_t freq, uint32_t pFreq0,
                             uint32_t pFreq1, uint32_t pFreq2, uint32_t *pFreqOut)
{
    return HAL_CRU_RoundFreqGetMux4(freq, pFreq0, pFreq1, pFreq2, 0, pFreqOut);
}

int HAL_CRU_RoundFreqGetMux2(uint32_t freq, uint32_t pFreq0, uint32_t pFreq1, uint32_t *pFreqOut)
{
    return HAL_CRU_RoundFreqGetMux4(freq, pFreq0, pFreq1, 0, 0, pFreqOut);
}

#if defined(SOC_RK3588)
/**
 * @brief Get pll parameter by auto.
 * @param  finHz: pll intput freq
 * @param  foutHz: pll output freq
 * @return struct PLL_CONFIG.
 * How to calculate the PLL:
 *     FFVCO = ((m + k / 65536) * FFIN) / p
 *     FFOUT = ((m + k / 65536) * FFIN) / (p * 2^s)
 */
static const struct PLL_CONFIG *CRU_PllSetByAuto(uint32_t finHz, uint32_t foutHz)
{
    struct PLL_CONFIG *rateTable = &g_rockchipAutoTable;
    uint64_t fvcoMin = 2250ULL * MHZ, fvcoMax = 4500ULL * MHZ;
    uint64_t foutMin = 37ULL * MHZ, foutMax = 4500ULL * MHZ;
    uint64_t fvco, fref, fout, ffrac;
    uint32_t p, m, s;

    if (finHz == 0 || foutHz == 0 || foutHz == finHz) {
        return NULL;
    }

    if (foutHz > foutMax || foutHz < foutMin) {
        return NULL;
    }

    if (finHz / MHZ * MHZ == finHz && foutHz / MHZ * MHZ == foutHz) {
        for (s = 0; s <= 6; s++) {
            fvco = (uint64_t)foutHz << s;
            if (fvco < fvcoMin || fvco > fvcoMax) {
                continue;
            }

            for (p = 2; p <= 4; p++) {
                for (m = 64; m <= 1023; m++) {
                    if (fvco == m * finHz / p) {
                        rateTable->p = p;
                        rateTable->m = m;
                        rateTable->s = s;
                        rateTable->k = 0;

                        return rateTable;
                    }
                }
            }
        }
    } else {
        for (s = 0; s <= 6; s++) {
            fvco = (uint64_t)foutHz << s;
            if (fvco < fvcoMin || fvco > fvcoMax) {
                continue;
            }

            for (p = 1; p <= 4; p++) {
                for (m = 64; m <= 1023; m++) {
                    if ((fvco >= m * finHz / p) && (fvco < (m + 1) * finHz / p)) {
                        rateTable->p = p;
                        rateTable->m = m;
                        rateTable->s = s;
                        fref = finHz / p;
                        ffrac = fvco - (m * fref);
                        fout = ffrac * 65536;
                        rateTable->k = fout / fref;

                        return rateTable;
                    }
                }
            }
        }
    }

    return NULL;
}

#else
/**
 * @brief Rockchip pll clk set postdiv.
 * @param  foutHz: output freq
 * @param  *postDiv1: pll postDiv1
 * @param  *postDiv2: pll postDiv2
 * @param  *foutVco: pll vco
 * @return HAL_Status.
 * How to calculate the PLL:
 *     Formulas also embedded within the fractional PLL Verilog model:
 *     If DSMPD = 1 (DSM is disabled, "integer mode")
 *     FOUTVCO = FREF / REFDIV * FBDIV
 *     FOUTPOSTDIV = FOUTVCO / POSTDIV1 / POSTDIV2
 *     Where:
 *     FOUTVCO = fractional PLL non-divided output frequency
 *     FOUTPOSTDIV = fractional PLL divided output frequency
 *               (output of second post divider)
 */
static HAL_Status CRU_PllSetPostDiv(uint32_t foutHz, uint32_t *postDiv1,
                                    uint32_t *postDiv2, uint32_t *foutVco)
{
    uint32_t freq;

    if (foutHz < MIN_FOUTVCO_FREQ) {
        for (*postDiv1 = 1; *postDiv1 <= 7; (*postDiv1)++) {
            for (*postDiv2 = 1; *postDiv2 <= 7; (*postDiv2)++) {
                freq = foutHz * (*postDiv1) * (*postDiv2);
                if (freq >= MIN_FOUTVCO_FREQ && freq <= MAX_FOUTVCO_FREQ) {
                    *foutVco = freq;

                    return HAL_OK;
                }
            }
        }

        return HAL_ERROR;
    } else {
        *postDiv1 = 1;
        *postDiv2 = 1;

        return HAL_OK;
    }
}

/**
 * @brief Get pll parameter by auto.
 * @param  finHz: pll intput freq
 * @param  foutHz: pll output freq
 * @return struct PLL_CONFIG.
 * How to calculate the PLL:
 *     Formulas also embedded within the fractional PLL Verilog model:
 *     If DSMPD = 1 (DSM is disabled, "integer mode")
 *     FOUTVCO = FREF / REFDIV * FBDIV
 *     FOUTPOSTDIV = FOUTVCO / POSTDIV1 / POSTDIV2
 *     Where:
 *     FOUTVCO = fractional PLL non-divided output frequency
 *     FOUTPOSTDIV = fractional PLL divided output frequency
 *               (output of second post divider)
 *     FREF = fractional PLL input reference frequency, (the OSC_HZ 24MHz input)
 *     REFDIV = fractional PLL input reference clock divider
 *     FBDIV = Integer value programmed into feedback divide
 */
static const struct PLL_CONFIG *CRU_PllSetByAuto(uint32_t finHz,
                                                 uint32_t foutHz)
{
    struct PLL_CONFIG *rateTable = &g_rockchipAutoTable;
    uint32_t foutVco = foutHz;
    uint64_t fin64, frac64;
    uint32_t postDiv1, postDiv2;
    uint32_t clkGcd = 0;
    HAL_Status error;

    if (finHz == 0 || foutHz == 0 || foutHz == finHz) {
        return NULL;
    }

    error = CRU_PllSetPostDiv(foutHz, &postDiv1, &postDiv2, &foutVco);
    if (error) {
        return NULL;
    }
    rateTable->postDiv1 = postDiv1;
    rateTable->postDiv2 = postDiv2;
    rateTable->dsmpd = 1;

    if (finHz / MHZ * MHZ == finHz && foutHz / MHZ * MHZ == foutHz) {
        finHz /= MHZ;
        foutVco /= MHZ;
        clkGcd = CRU_Gcd(finHz, foutVco);
        rateTable->refDiv = finHz / clkGcd;
        rateTable->fbDiv = foutVco / clkGcd;

        rateTable->frac = 0;
    } else if (finHz / MHZ * MHZ != finHz) {
        clkGcd = foutHz / finHz;
        rateTable->fbDiv = clkGcd;
        rateTable->refDiv = 1;
        rateTable->postDiv1 = 1;
        rateTable->postDiv2 = 1;

        fin64 = foutHz * rateTable->refDiv;
        fin64 = HAL_DivU64(fin64 << EXPONENT_OF_FRAC_PLL, finHz);
        frac64 = rateTable->fbDiv;
        frac64 = rateTable->fbDiv << EXPONENT_OF_FRAC_PLL;
        frac64 = fin64 - frac64;
        rateTable->frac = frac64;
        if (rateTable->frac > 0) {
            rateTable->dsmpd = 0;
        }
    } else {
        clkGcd = CRU_Gcd(finHz / MHZ, foutVco / MHZ);
        rateTable->refDiv = finHz / MHZ / clkGcd;
        rateTable->fbDiv = foutVco / MHZ / clkGcd;
        rateTable->frac = 0;

        frac64 = (foutVco % MHZ);
        fin64 = finHz;
        frac64 = frac64 * rateTable->refDiv;
        frac64 = HAL_DivU64(frac64 << EXPONENT_OF_FRAC_PLL, fin64);
        rateTable->frac = frac64;
        if (rateTable->frac > 0) {
            rateTable->dsmpd = 0;
        }
    }

    return rateTable;
}
#endif

/**
 * @brief Get pll parameter by rateTable.
 * @param  *pSetup: struct PLL_SETUP struct, Contains PLL register parameters
 * @param  rate: pll target rate.
 * @return struct PLL_CONFIG.
 * How to calculate the PLL:
 *     Look up the rateTable to get the PLL config parameter
 */
static const struct PLL_CONFIG *CRU_PllGetSettings(struct PLL_SETUP *pSetup,
                                                   uint32_t rate)
{
    const struct PLL_CONFIG *rateTable = pSetup->rateTable;

    if (rateTable == NULL) {
        return CRU_PllSetByAuto(PLL_INPUT_OSC_RATE, rate);
    }

    while (rateTable->rate) {
        if (rateTable->rate == rate) {
            break;
        }
        rateTable++;
    }
    if (rateTable->rate != rate) {
        return CRU_PllSetByAuto(PLL_INPUT_OSC_RATE, rate);
    } else {
        return rateTable;
    }
}
/** @} */
/********************* Public Function Definition ****************************/

/** @defgroup CRU_Exported_Functions_Group5 Other Functions
 *  @attention these APIs allow direct use in the HAL layer.
 *  @{
 */

#if defined(SOC_RV1108)
/*
 * Formulas also embedded within the fractional PLL Verilog model:
 * If DSMPD = 1 (DSM is disabled, "integer mode")
 * FOUTVCO = FREF / REFDIV * FBDIV
 * FOUT = FOUTVCO / POSTDIV1 / POSTDIV2
 * If DSMPD = 0 (DSM is enabled, "fractional mode")
 * FOUTVCO = (FREF / REFDIV) * (FBDIV + FRAC / (2^24))
 * FOUTPOSTDIV = FOUTVCO / (POSTDIV1*POSTDIV2)
 * FOUT = FOUTVCO / POSTDIV1 / POSTDIV2
 */
uint32_t HAL_CRU_GetPllFreq(struct PLL_SETUP *pSetup)
{
    uint32_t refDiv, fbDiv, postdDv1, postDiv2, frac, dsmpd;
    uint32_t mode = 0, rate = PLL_INPUT_OSC_RATE;

    mode = PLL_GET_PLLMODE(READ_REG(*(pSetup->modeOffset)), pSetup->modeShift,
                           pSetup->modeMask);

    switch (mode) {
    case RK_PLL_MODE_SLOW:
        rate = PLL_INPUT_OSC_RATE;
        break;
    case RK_PLL_MODE_NORMAL:
        fbDiv = PLL_GET_FBDIV(READ_REG(*(pSetup->conOffset0)));
        postdDv1 = PLL_GET_POSTDIV1(READ_REG(*(pSetup->conOffset1)));
        postDiv2 = PLL_GET_POSTDIV2(READ_REG(*(pSetup->conOffset1)));
        refDiv = PLL_GET_REFDIV(READ_REG(*(pSetup->conOffset1)));
        dsmpd = PLL_GET_DSMPD(READ_REG(*(pSetup->conOffset3)));
        frac = PLL_GET_FRAC(READ_REG(*(pSetup->conOffset2)));
        rate = (rate / refDiv) * fbDiv;
        if (dsmpd == 0) {
            uint64_t fracRate = PLL_INPUT_OSC_RATE;

            fracRate *= frac;
            fracRate = fracRate >> EXPONENT_OF_FRAC_PLL;
            fracRate = fracRate / refDiv;
            rate += fracRate;
        }
        rate = rate / (postdDv1 * postDiv2);
        rate = CRU_PLL_ROUND_UP_TO_KHZ(rate);
        break;
    case RK_PLL_MODE_DEEP:
    default:
        rate = 32768;
        break;
    }

    return rate;
}

/*
 * Force PLL into slow mode
 * Pll Power down
 * Pll Config fbDiv, refDiv, postdDv1, postDiv2, dsmpd, frac
 * Pll Power up
 * Waiting for pll lock
 * Force PLL into normal mode
 */
HAL_Status HAL_CRU_SetPllFreq(struct PLL_SETUP *pSetup, uint32_t rate)
{
    const struct PLL_CONFIG *pConfig;
    int delay = 2400;

    if (rate == HAL_CRU_GetPllFreq(pSetup)) {
        return HAL_OK;
    } else if (rate < MIN_FOUT_FREQ) {
        return HAL_INVAL;
    } else if (rate > MAX_FOUT_FREQ) {
        return HAL_INVAL;
    }

    pConfig = CRU_PllGetSettings(pSetup, rate);
    if (!pConfig) {
        return HAL_ERROR;
    }

    /* Force PLL into slow mode to ensure output stable clock */
    WRITE_REG_MASK_WE(*(pSetup->modeOffset), pSetup->modeMask, RK_PLL_MODE_SLOW << pSetup->modeShift);

    /* Pll Power down */
    WRITE_REG_MASK_WE(*(pSetup->conOffset3), PWRDOWN_MASK, 1 << PWRDOWN_SHIFT);

    /* Pll Config */
    WRITE_REG_MASK_WE(*(pSetup->conOffset1), PLL_POSTDIV2_MASK, pConfig->postDiv2 << PLL_POSTDIV2_SHIFT);
    WRITE_REG_MASK_WE(*(pSetup->conOffset1), PLL_REFDIV_MASK, pConfig->refDiv << PLL_REFDIV_SHIFT);
    WRITE_REG_MASK_WE(*(pSetup->conOffset1), PLL_POSTDIV1_MASK, pConfig->postDiv1 << PLL_POSTDIV1_SHIFT);
    WRITE_REG_MASK_WE(*(pSetup->conOffset0), PLL_FBDIV_MASK, pConfig->fbDiv << PLL_FBDIV_SHIFT);
    WRITE_REG_MASK_WE(*(pSetup->conOffset3), PLL_DSMPD_MASK, pConfig->dsmpd << PLL_DSMPD_SHIFT);

    if (pConfig->frac) {
        WRITE_REG(*(pSetup->conOffset2), (READ_REG(*(pSetup->conOffset2)) & 0xff000000) | pConfig->frac);
    }

    WRITE_REG_MASK_WE(*(pSetup->conOffset3), PWRDOWN_MASK, 0 << PWRDOWN_SHIFT);

    /* Waiting for pll lock */
    while (delay > 0) {
        if (READ_REG(*(pSetup->conOffset2)) & (1 << pSetup->lockShift)) {
            break;
        }
        HAL_CPUDelayUs(1000);
        delay--;
    }
    if (delay == 0) {
        return HAL_TIMEOUT;
    }

    /* Force PLL into normal mode */
    WRITE_REG_MASK_WE(*(pSetup->modeOffset), pSetup->modeMask, RK_PLL_MODE_NORMAL << pSetup->modeShift);

    return HAL_OK;
}

HAL_Status HAL_CRU_SetPllPowerUp(struct PLL_SETUP *pSetup)
{
    int delay = 2400;

    /* Pll Power up */
    WRITE_REG_MASK_WE(*(pSetup->conOffset3), PWRDOWN_MASK, 0 << PWRDOWN_SHIFT);

    /* Waiting for pll lock */
    while (delay > 0) {
        if (READ_REG(*(pSetup->conOffset2)) & (1 << pSetup->lockShift)) {
            break;
        }
        HAL_CPUDelayUs(1000);
        delay--;
    }
    if (delay == 0) {
        return HAL_TIMEOUT;
    }

    return HAL_OK;
}

HAL_Status HAL_CRU_SetPllPowerDown(struct PLL_SETUP *pSetup)
{
    /* Pll Power down */
    WRITE_REG_MASK_WE(*(pSetup->conOffset3), PWRDOWN_MASK, 1 << PWRDOWN_SHIFT);

    return HAL_OK;
}

#elif defined(SOC_RK3588)
/*
 * Formulas also embedded within the fractional PLL Verilog model:
 * If K = 0 (DSM is disabled, "integer mode")
 * FOUTVCO = FREF / P * M
 * FOUT = FOUTVCO / 2^S
 * If K > 0 (DSM is enabled, "fractional mode")
 * FOUTVCO = (FREF / P) * (M + K / 65536)
 * FOUT = FOUTVCO / 2^S
 */
uint32_t HAL_CRU_GetPllFreq(struct PLL_SETUP *pSetup)
{
    uint32_t m, p, s, k;
    uint64_t rate = PLL_INPUT_OSC_RATE;
    uint32_t mode = 0;

    if (pSetup->modeMask) {
        mode = PLL_GET_PLLMODE(READ_REG(*(pSetup->modeOffset)), pSetup->modeShift,
                               pSetup->modeMask);
    } else {
        mode = RK_PLL_MODE_NORMAL;
    }

    switch (mode) {
    case RK_PLL_MODE_SLOW:
        rate = PLL_INPUT_OSC_RATE;
        break;

    case RK_PLL_MODE_NORMAL:
        m = (READ_REG(*(pSetup->conOffset0)) & PLLCON0_M_MASK) >> PLLCON0_M_SHIFT;
        p = (READ_REG(*(pSetup->conOffset1)) & PLLCON1_P_MASK) >> PLLCON1_P_SHIFT;
        s = (READ_REG(*(pSetup->conOffset1)) & PLLCON1_S_MASK) >> PLLCON1_S_SHIFT;
        k = (READ_REG(*(pSetup->conOffset2)) & PLLCON2_K_MASK) >> PLLCON2_K_SHIFT;

        rate *= m;
        rate = rate / p;
        if (k) {
            /* fractional mode */
            uint64_t frac = PLL_INPUT_OSC_RATE / p;

            frac *= k;
            frac = frac / 65536;
            rate += frac;
        }
        rate = rate >> s;
        break;

    case RK_PLL_MODE_DEEP:
    default:
        rate = 32768;
        break;
    }

    return rate;
}

/*
 * Force PLL into slow mode
 * Pll Power down
 * Pll Config M, P, S, K
 * Pll Power up
 * Waiting for pll lock
 * Force PLL into normal mode
 */
HAL_Status HAL_CRU_SetPllFreq(struct PLL_SETUP *pSetup, uint32_t rate)
{
    const struct PLL_CONFIG *pConfig;
    int delay = 24000000;

    if (rate == HAL_CRU_GetPllFreq(pSetup)) {
        return HAL_OK;
    } else if (rate < MIN_FOUT_FREQ) {
        return HAL_INVAL;
    } else if (rate > MAX_FOUT_FREQ) {
        return HAL_INVAL;
    }

    pConfig = CRU_PllGetSettings(pSetup, rate);
    if (!pConfig) {
        return HAL_ERROR;
    }

    /* Force PLL into slow mode to ensure output stable clock */
    if (pSetup->modeMask) {
        WRITE_REG_MASK_WE(*(pSetup->modeOffset), pSetup->modeMask, RK_PLL_MODE_SLOW << pSetup->modeShift);
    }

    /* Pll Power down */
    WRITE_REG_MASK_WE(*(pSetup->conOffset1), PWRDOWN_MASK, 1 << PWRDOWN_SHIFT);

    /* Pll Config */
    WRITE_REG_MASK_WE(*(pSetup->conOffset0), PLLCON0_M_MASK, pConfig->m << PLLCON0_M_SHIFT);
    WRITE_REG_MASK_WE(*(pSetup->conOffset1), PLLCON1_P_MASK, pConfig->p << PLLCON1_P_SHIFT);
    WRITE_REG_MASK_WE(*(pSetup->conOffset1), PLLCON1_S_MASK, pConfig->s << PLLCON1_S_SHIFT);
    if (pConfig->k) {
        WRITE_REG_MASK_WE(*(pSetup->conOffset2), PLLCON2_K_MASK, pConfig->k << PLLCON2_K_SHIFT);
    }

    /* Pll Power up */
    WRITE_REG_MASK_WE(*(pSetup->conOffset1), PWRDOWN_MASK, 0 << PWRDOWN_SHIFT);

    /* Waiting for pll lock */
    while (delay > 0) {
        if (READ_REG(*(pSetup->conOffset6)) & (1 << pSetup->lockShift)) {
            break;
        }
        delay--;
    }
    if (delay == 0) {
        return HAL_TIMEOUT;
    }

    /* Force PLL into normal mode */
    if (pSetup->modeMask) {
        WRITE_REG_MASK_WE(*(pSetup->modeOffset), pSetup->modeMask, RK_PLL_MODE_NORMAL << pSetup->modeShift);
    }

    /*
     * PLL operates normally.
     *
     * ATF system suspend requires memory repair operation which would
     * bypass the pll, let's ensure it's on normal mode.
     */
    WRITE_REG_MASK_WE(*(pSetup->conOffset0), BYPASS_MASK, 0 << BYPASS_SHIFT);

    return HAL_OK;
}

HAL_Status HAL_CRU_SetPllPowerUp(struct PLL_SETUP *pSetup)
{
    int delay = 2400;

    /* Pll Power up */
    WRITE_REG_MASK_WE(*(pSetup->conOffset1), PWRDOWN_MASK, 0 << PWRDOWN_SHIFT);

    /* Waiting for pll lock */
    while (delay > 0) {
        if (READ_REG(*(pSetup->conOffset6)) & (1 << pSetup->lockShift)) {
            break;
        }
        HAL_CPUDelayUs(1000);
        delay--;
    }
    if (delay == 0) {
        return HAL_TIMEOUT;
    }

    return HAL_OK;
}

HAL_Status HAL_CRU_SetPllPowerDown(struct PLL_SETUP *pSetup)
{
    /* Pll Power down */
    WRITE_REG_MASK_WE(*(pSetup->conOffset1), PWRDOWN_MASK, 1 << PWRDOWN_SHIFT);

    return HAL_OK;
}

#else
/*
 * Formulas also embedded within the fractional PLL Verilog model:
 * If DSMPD = 1 (DSM is disabled, "integer mode")
 * FOUTVCO = FREF / REFDIV * FBDIV
 * FOUT = FOUTVCO / POSTDIV1 / POSTDIV2
 * If DSMPD = 0 (DSM is enabled, "fractional mode")
 * FOUTVCO = (FREF / REFDIV) * (FBDIV + FRAC / (2^24))
 * FOUTPOSTDIV = FOUTVCO / (POSTDIV1*POSTDIV2)
 * FOUT = FOUTVCO / POSTDIV1 / POSTDIV2
 */
uint32_t HAL_CRU_GetPllFreq(struct PLL_SETUP *pSetup)
{
    uint32_t refDiv, fbDiv, postdDv1, postDiv2, frac, dsmpd;
    uint32_t mode = 0, rate = PLL_INPUT_OSC_RATE;

    mode = PLL_GET_PLLMODE(READ_REG(*(pSetup->modeOffset)), pSetup->modeShift,
                           pSetup->modeMask);

    switch (mode) {
    case RK_PLL_MODE_SLOW:
        rate = PLL_INPUT_OSC_RATE;
        break;
    case RK_PLL_MODE_NORMAL:
        postdDv1 = PLL_GET_POSTDIV1(READ_REG(*(pSetup->conOffset0)));
        fbDiv = PLL_GET_FBDIV(READ_REG(*(pSetup->conOffset0)));
        postDiv2 = PLL_GET_POSTDIV2(READ_REG(*(pSetup->conOffset1)));
        refDiv = PLL_GET_REFDIV(READ_REG(*(pSetup->conOffset1)));
        dsmpd = PLL_GET_DSMPD(READ_REG(*(pSetup->conOffset1)));
        frac = PLL_GET_FRAC(READ_REG(*(pSetup->conOffset2)));
        rate = (rate / refDiv) * fbDiv;
        if (dsmpd == 0) {
            uint64_t fracRate = PLL_INPUT_OSC_RATE;

            fracRate *= frac;
            fracRate = fracRate >> EXPONENT_OF_FRAC_PLL;
            fracRate = fracRate / refDiv;
            rate += fracRate;
        }
        rate = rate / (postdDv1 * postDiv2);
        rate = CRU_PLL_ROUND_UP_TO_KHZ(rate);
        break;
    case RK_PLL_MODE_DEEP:
    default:
        rate = 32768;
        break;
    }

    return rate;
}

/*
 * Force PLL into slow mode
 * Pll Power down
 * Pll Config fbDiv, refDiv, postdDv1, postDiv2, dsmpd, frac
 * Pll Power up
 * Waiting for pll lock
 * Force PLL into normal mode
 */
HAL_Status HAL_CRU_SetPllFreq(struct PLL_SETUP *pSetup, uint32_t rate)
{
    const struct PLL_CONFIG *pConfig;
    int delay = 2400;

    if (rate == HAL_CRU_GetPllFreq(pSetup)) {
        return HAL_OK;
    } else if (rate < MIN_FOUT_FREQ) {
        return HAL_INVAL;
    } else if (rate > MAX_FOUT_FREQ) {
        return HAL_INVAL;
    }

    pConfig = CRU_PllGetSettings(pSetup, rate);
    if (!pConfig) {
        return HAL_ERROR;
    }

    /* Force PLL into slow mode to ensure output stable clock */
    WRITE_REG_MASK_WE(*(pSetup->modeOffset), pSetup->modeMask, RK_PLL_MODE_SLOW << pSetup->modeShift);

    /* Pll Power down */
    WRITE_REG_MASK_WE(*(pSetup->conOffset1), PWRDOWN_MASK, 1 << PWRDOWN_SHIFT);

    /* Pll Config */
    WRITE_REG_MASK_WE(*(pSetup->conOffset1), PLL_POSTDIV2_MASK, pConfig->postDiv2 << PLL_POSTDIV2_SHIFT);
    WRITE_REG_MASK_WE(*(pSetup->conOffset1), PLL_REFDIV_MASK, pConfig->refDiv << PLL_REFDIV_SHIFT);
    WRITE_REG_MASK_WE(*(pSetup->conOffset0), PLL_POSTDIV1_MASK, pConfig->postDiv1 << PLL_POSTDIV1_SHIFT);
    WRITE_REG_MASK_WE(*(pSetup->conOffset0), PLL_FBDIV_MASK, pConfig->fbDiv << PLL_FBDIV_SHIFT);
    WRITE_REG_MASK_WE(*(pSetup->conOffset1), PLL_DSMPD_MASK, pConfig->dsmpd << PLL_DSMPD_SHIFT);

    if (pConfig->frac) {
        WRITE_REG(*(pSetup->conOffset2), (READ_REG(*(pSetup->conOffset2)) & 0xff000000) | pConfig->frac);
    }

    /* Pll Power up */
    WRITE_REG_MASK_WE(*(pSetup->conOffset1), PWRDOWN_MASK, 0 << PWRDOWN_SHIFT);

    /* Waiting for pll lock */
    while (delay > 0) {
        if (pSetup->stat0) {
            if (READ_REG(*(pSetup->stat0)) & (1 << pSetup->lockShift)) {
                break;
            }
        } else {
            if (READ_REG(*(pSetup->conOffset1)) & (1 << pSetup->lockShift)) {
                break;
            }
        }
        HAL_CPUDelayUs(1000);
        delay--;
    }
    if (delay == 0) {
        return HAL_TIMEOUT;
    }

    /* Force PLL into normal mode */
    WRITE_REG_MASK_WE(*(pSetup->modeOffset), pSetup->modeMask, RK_PLL_MODE_NORMAL << pSetup->modeShift);

    return HAL_OK;
}

HAL_Status HAL_CRU_SetPllPowerUp(struct PLL_SETUP *pSetup)
{
    int delay = 2400;

    /* Pll Power up */
    WRITE_REG_MASK_WE(*(pSetup->conOffset1), PWRDOWN_MASK, 0 << PWRDOWN_SHIFT);

    /* Waiting for pll lock */
    while (delay > 0) {
        if (pSetup->stat0) {
            if (READ_REG(*(pSetup->stat0)) & (1 << pSetup->lockShift)) {
                break;
            }
        } else {
            if (READ_REG(*(pSetup->conOffset1)) & (1 << pSetup->lockShift)) {
                break;
            }
        }
        HAL_CPUDelayUs(1000);
        delay--;
    }
    if (delay == 0) {
        return HAL_TIMEOUT;
    }

    return HAL_OK;
}

HAL_Status HAL_CRU_SetPllPowerDown(struct PLL_SETUP *pSetup)
{
    /* Pll Power down */
    WRITE_REG_MASK_WE(*(pSetup->conOffset1), PWRDOWN_MASK, 1 << PWRDOWN_SHIFT);

    return HAL_OK;
}
#endif

#ifdef CRU_CLK_USE_CON_BANK
static const struct HAL_CRU_DEV *CRU_GetInfo(void)
{
    return &g_cruDev;
}

HAL_Check HAL_CRU_ClkIsEnabled(uint32_t clk)
{
    const struct HAL_CRU_DEV *ctrl = CRU_GetInfo();
    uint32_t index = CLK_GATE_GET_REG_OFFSET(clk);
    uint32_t shift = CLK_GATE_GET_BITS_SHIFT(clk);
    uint32_t bank = CLK_GATE_GET_REG_BANK(clk);
    uint32_t reg;
    HAL_Check ret;

    reg = ctrl->banks[bank].cruBase + ctrl->banks[bank].gateOffset + index * 4;
    ret = (HAL_Check)(!((CRU_READ(reg) & (1 << shift)) >> shift));

    return ret;
}

HAL_Status HAL_CRU_ClkEnable(uint32_t clk)
{
    const struct HAL_CRU_DEV *ctrl = CRU_GetInfo();
    uint32_t index = CLK_GATE_GET_REG_OFFSET(clk);
    uint32_t shift = CLK_GATE_GET_BITS_SHIFT(clk);
    uint32_t bank = CLK_GATE_GET_REG_BANK(clk);
    uint32_t reg;

    reg = ctrl->banks[bank].cruBase + ctrl->banks[bank].gateOffset + index * 4;
    CRU_WRITE(reg, shift, 1U << shift, 0U);

    return HAL_OK;
}

HAL_Status HAL_CRU_ClkDisable(uint32_t clk)
{
    const struct HAL_CRU_DEV *ctrl = CRU_GetInfo();
    uint32_t index = CLK_GATE_GET_REG_OFFSET(clk);
    uint32_t shift = CLK_GATE_GET_BITS_SHIFT(clk);
    uint32_t bank = CLK_GATE_GET_REG_BANK(clk);
    uint32_t reg;

    reg = ctrl->banks[bank].cruBase + ctrl->banks[bank].gateOffset + index * 4;
    CRU_WRITE(reg, shift, 1U << shift, 1U);

    return HAL_OK;
}

HAL_Status HAL_CRU_ClkDisableUnused(uint32_t bank, uint32_t index, uint32_t val)
{
    const struct HAL_CRU_DEV *ctrl = CRU_GetInfo();
    uint32_t reg;

    reg = ctrl->banks[bank].cruBase + ctrl->banks[bank].gateOffset + index * 4;
    CRU_WRITE(reg, 0, 0, val);

    return HAL_OK;
}

HAL_Check HAL_CRU_ClkIsReset(uint32_t clk)
{
    const struct HAL_CRU_DEV *ctrl = CRU_GetInfo();
    uint32_t index = CLK_GATE_GET_REG_OFFSET(clk);
    uint32_t shift = CLK_GATE_GET_BITS_SHIFT(clk);
    uint32_t bank = CLK_GATE_GET_REG_BANK(clk);
    uint32_t reg;
    HAL_Check ret;

    reg = ctrl->banks[bank].cruBase + ctrl->banks[bank].softOffset + index * 4;
    ret = (HAL_Check)((CRU_READ(reg) & (1 << shift)) >> shift);

    return ret;
}

HAL_Status HAL_CRU_ClkResetAssert(uint32_t clk)
{
    const struct HAL_CRU_DEV *ctrl = CRU_GetInfo();
    uint32_t index = CLK_RESET_GET_REG_OFFSET(clk);
    uint32_t shift = CLK_RESET_GET_BITS_SHIFT(clk);
    uint32_t bank = CLK_GATE_GET_REG_BANK(clk);
    uint32_t reg;

    HAL_ASSERT(shift < 16);
    reg = ctrl->banks[bank].cruBase + ctrl->banks[bank].softOffset + index * 4;
    CRU_WRITE(reg, shift, 1U << shift, 1U);

    return HAL_OK;
}

HAL_Status HAL_CRU_ClkResetDeassert(uint32_t clk)
{
    const struct HAL_CRU_DEV *ctrl = CRU_GetInfo();
    uint32_t index = CLK_RESET_GET_REG_OFFSET(clk);
    uint32_t shift = CLK_RESET_GET_BITS_SHIFT(clk);
    uint32_t bank = CLK_GATE_GET_REG_BANK(clk);
    uint32_t reg;

    HAL_ASSERT(shift < 16);
    reg = ctrl->banks[bank].cruBase + ctrl->banks[bank].softOffset + index * 4;
    CRU_WRITE(reg, shift, 1U << shift, 0U);

    return HAL_OK;
}

HAL_Status HAL_CRU_ClkResetSyncAssert(int numClks, uint32_t *clks)
{
    const struct HAL_CRU_DEV *ctrl = CRU_GetInfo();
    uint32_t index = CLK_RESET_GET_REG_OFFSET(clks[0]);
    uint32_t bank = CLK_GATE_GET_REG_BANK(clks[0]);
    uint32_t val = 0;
    uint32_t reg;
    int i;

    for (i = 0; i < numClks; i++) {
        val |= HAL_BIT(CLK_RESET_GET_BITS_SHIFT(clks[i]));
        if (index != CLK_RESET_GET_REG_OFFSET(clks[i])) {
            return HAL_ERROR;
        }
    }

    reg = ctrl->banks[bank].cruBase + ctrl->banks[bank].softOffset + index * 4;
    CRU_WRITE(reg, 0, val, val);
    HAL_DBG("%s: index: 0x%lx, val: 0x%lx\n", __func__, index, val);

    return HAL_OK;
}

HAL_Status HAL_CRU_ClkResetSyncDeassert(int numClks, uint32_t *clks)
{
    const struct HAL_CRU_DEV *ctrl = CRU_GetInfo();
    uint32_t index = CLK_RESET_GET_REG_OFFSET(clks[0]);
    uint32_t bank = CLK_GATE_GET_REG_BANK(clks[0]);
    uint32_t val = 0;
    uint32_t reg;
    int i;

    for (i = 0; i < numClks; i++) {
        val |= HAL_BIT(CLK_RESET_GET_BITS_SHIFT(clks[i]));
        if (index != CLK_RESET_GET_REG_OFFSET(clks[i])) {
            return HAL_ERROR;
        }
    }

    reg = ctrl->banks[bank].cruBase + ctrl->banks[bank].softOffset + index * 4;
    CRU_WRITE(reg, 0, val, 0);
    HAL_DBG("%s: index: 0x%lx, val: 0x%lx\n", __func__, index, val);

    return HAL_OK;
}

HAL_Status HAL_CRU_ClkSetDiv(uint32_t divName, uint32_t divValue)
{
    const struct HAL_CRU_DEV *ctrl = CRU_GetInfo();
    uint32_t shift, mask, index;
    uint32_t reg, bank;

    index = CLK_DIV_GET_REG_OFFSET(divName);
    shift = CLK_DIV_GET_BITS_SHIFT(divName);
    HAL_ASSERT(shift < 16);
    mask = CLK_DIV_GET_MASK(divName);
    if (divValue > mask) {
        divValue = mask;
    }

    bank = CLK_DIV_GET_BANK(divName);
    reg = ctrl->banks[bank].cruBase + ctrl->banks[bank].selOffset + index * 4;
    CRU_WRITE(reg, shift, mask, (divValue - 1U));

    return HAL_OK;
}

uint32_t HAL_CRU_ClkGetDiv(uint32_t divName)
{
    const struct HAL_CRU_DEV *ctrl = CRU_GetInfo();
    uint32_t shift, mask, index, divValue;
    uint32_t reg, bank;

    index = CLK_DIV_GET_REG_OFFSET(divName);
    shift = CLK_DIV_GET_BITS_SHIFT(divName);
    HAL_ASSERT(shift < 16);
    mask = CLK_DIV_GET_MASK(divName);
    bank = CLK_DIV_GET_BANK(divName);
    reg = ctrl->banks[bank].cruBase + ctrl->banks[bank].selOffset + index * 4;
    divValue = ((CRU_READ(reg) & mask) >> shift) + 1;

    return divValue;
}

HAL_SECTION_SRAM_CODE
HAL_Status HAL_CRU_ClkSetMux(uint32_t muxName, uint32_t muxValue)
{
    const struct HAL_CRU_DEV *ctrl = CRU_GetInfo();
    uint32_t shift, mask, index;
    uint32_t reg, bank;

    index = CLK_MUX_GET_REG_OFFSET(muxName);
    shift = CLK_MUX_GET_BITS_SHIFT(muxName);
    HAL_ASSERT(shift < 16);
    mask = CLK_MUX_GET_MASK(muxName);
    bank = CLK_MUX_GET_BANK(muxName);
    reg = ctrl->banks[bank].cruBase + ctrl->banks[bank].selOffset + index * 4;
    CRU_WRITE(reg, shift, mask, muxValue);

    return HAL_OK;
}

HAL_SECTION_SRAM_CODE
uint32_t HAL_CRU_ClkGetMux(uint32_t muxName)
{
    const struct HAL_CRU_DEV *ctrl = CRU_GetInfo();
    uint32_t shift, mask, index, muxValue;
    uint32_t reg, bank;

    index = CLK_MUX_GET_REG_OFFSET(muxName);
    shift = CLK_MUX_GET_BITS_SHIFT(muxName);
    HAL_ASSERT(shift < 16);
    mask = CLK_MUX_GET_MASK(muxName);
    bank = CLK_MUX_GET_BANK(muxName);
    reg = ctrl->banks[bank].cruBase + ctrl->banks[bank].selOffset + index * 4;
    muxValue = ((CRU_READ(reg) & mask) >> shift);

    return muxValue;
}

HAL_Status HAL_CRU_ClkSetFracDiv(uint32_t fracDivName,
                                 uint32_t numerator,
                                 uint32_t denominator)
{
    const struct HAL_CRU_DEV *ctrl = CRU_GetInfo();
    uint32_t reg, bank;
    uint32_t index;

    index = CLK_DIV_GET_REG_OFFSET(fracDivName);
    bank = CLK_DIV_GET_BANK(fracDivName);
    reg = ctrl->banks[bank].cruBase + ctrl->banks[bank].selOffset + index * 4;
    CRU_WRITE(reg, 0, 0, ((numerator << 16) | denominator));

    return HAL_OK;
}

HAL_Status HAL_CRU_ClkGetFracDiv(uint32_t fracDivName,
                                 uint32_t *numerator,
                                 uint32_t *denominator)
{
    const struct HAL_CRU_DEV *ctrl = CRU_GetInfo();
    uint32_t reg, bank;
    uint32_t index;
    uint32_t val;

    index = CLK_DIV_GET_REG_OFFSET(fracDivName);
    bank = CLK_DIV_GET_BANK(fracDivName);
    reg = ctrl->banks[bank].cruBase + ctrl->banks[bank].selOffset + index * 4;
    val = CRU_READ(reg);

    *numerator = (val & 0xffff0000) >> 16;
    *denominator = (val & 0x0000ffff);

    return HAL_OK;
}
#else /* CRU_CLK_USE_CON_BANK */

HAL_Check HAL_CRU_ClkIsEnabled(uint32_t clk)
{
    uint32_t index = CLK_GATE_GET_REG_OFFSET(clk);
    uint32_t shift = CLK_GATE_GET_BITS_SHIFT(clk);
    HAL_Check ret;

#ifdef CRU_GATE_CON_CNT
    if (index < CRU_GATE_CON_CNT) {
        ret = (HAL_Check)(!((CRU->CRU_CLKGATE_CON[index] & (1 << shift)) >> shift));
    } else {
#ifdef PMUCRU_BASE
        ret = (HAL_Check)(!((PMUCRU->CRU_CLKGATE_CON[index - CRU_GATE_CON_CNT] & (1 << shift)) >> shift));
#else
        ret = (HAL_Check)(!((CRU->PMU_CLKGATE_CON[index - CRU_GATE_CON_CNT] & (1 << shift)) >> shift));
#endif
    }
#else
    ret = (HAL_Check)(!((CRU->CRU_CLKGATE_CON[index] & (1 << shift)) >> shift));
#endif

    return ret;
}

HAL_Status HAL_CRU_ClkEnable(uint32_t clk)
{
    uint32_t index = CLK_GATE_GET_REG_OFFSET(clk);
    uint32_t shift = CLK_GATE_GET_BITS_SHIFT(clk);

#ifdef CRU_GATE_CON_CNT
    if (index < CRU_GATE_CON_CNT) {
        CRU->CRU_CLKGATE_CON[index] = VAL_MASK_WE(1U << shift, 0U << shift);
    } else {
#ifdef PMUCRU_BASE
        PMUCRU->CRU_CLKGATE_CON[index - CRU_GATE_CON_CNT] = VAL_MASK_WE(1U << shift, 0U << shift);
#else
        CRU->PMU_CLKGATE_CON[index - CRU_GATE_CON_CNT] = VAL_MASK_WE(1U << shift, 0U << shift);
#endif
    }
#else
    CRU->CRU_CLKGATE_CON[index] = VAL_MASK_WE(1U << shift, 0U << shift);
#endif

    return HAL_OK;
}

HAL_Status HAL_CRU_ClkDisable(uint32_t clk)
{
    uint32_t index = CLK_GATE_GET_REG_OFFSET(clk);
    uint32_t shift = CLK_GATE_GET_BITS_SHIFT(clk);

#ifdef CRU_GATE_CON_CNT
    if (index < CRU_GATE_CON_CNT) {
        CRU->CRU_CLKGATE_CON[index] = VAL_MASK_WE(1U << shift, 1U << shift);
    } else {
#ifdef PMUCRU_BASE
        PMUCRU->CRU_CLKGATE_CON[index - CRU_GATE_CON_CNT] = VAL_MASK_WE(1U << shift, 1U << shift);
#else
        CRU->PMU_CLKGATE_CON[index - CRU_GATE_CON_CNT] = VAL_MASK_WE(1U << shift, 1U << shift);
#endif
    }
#else
    CRU->CRU_CLKGATE_CON[index] = VAL_MASK_WE(1U << shift, 1U << shift);
#endif

    return HAL_OK;
}

HAL_Status HAL_CRU_ClkDisableUnused(uint32_t bank, uint32_t index, uint32_t val)
{
#ifdef CRU_GATE_CON_CNT
    if (index < CRU_GATE_CON_CNT) {
        CRU->CRU_CLKGATE_CON[index] = val;
    } else {
#ifdef PMUCRU_BASE
        PMUCRU->CRU_CLKGATE_CON[index - CRU_GATE_CON_CNT] = val;
#else
        CRU->PMU_CLKGATE_CON[index - CRU_GATE_CON_CNT] = val;
#endif
    }
#else
    CRU->CRU_CLKGATE_CON[index] = val;
#endif

    return HAL_OK;
}

HAL_Check HAL_CRU_ClkIsReset(uint32_t clk)
{
    uint32_t index = CLK_GATE_GET_REG_OFFSET(clk);
    uint32_t shift = CLK_GATE_GET_BITS_SHIFT(clk);
    HAL_Check ret;

#ifdef CRU_SRST_CON_CNT
    if (index < CRU_SRST_CON_CNT) {
        ret = (HAL_Check)((CRU->CRU_CLKGATE_CON[index] & (1 << shift)) >> shift);
    } else {
        ret = (HAL_Check)((PMUCRU->CRU_CLKGATE_CON[index - CRU_SRST_CON_CNT] & (1 << shift)) >> shift);
    }
#else
    ret = (HAL_Check)((CRU->CRU_CLKGATE_CON[index] & (1 << shift)) >> shift);
#endif

    return ret;
}

HAL_Status HAL_CRU_ClkResetAssert(uint32_t clk)
{
    uint32_t index = CLK_RESET_GET_REG_OFFSET(clk);
    uint32_t shift = CLK_RESET_GET_BITS_SHIFT(clk);

    HAL_ASSERT(shift < 16);
#ifdef CRU_SRST_CON_CNT
    if (index < CRU_SRST_CON_CNT) {
        CRU->CRU_SOFTRST_CON[index] = VAL_MASK_WE(1U << shift, 1U << shift);
    } else {
        PMUCRU->CRU_SOFTRST_CON[index - CRU_SRST_CON_CNT] = VAL_MASK_WE(1U << shift, 1U << shift);
    }
#else
    CRU->CRU_SOFTRST_CON[index] = VAL_MASK_WE(1U << shift, 1U << shift);
#endif

    return HAL_OK;
}

HAL_Status HAL_CRU_ClkResetDeassert(uint32_t clk)
{
    uint32_t index = CLK_RESET_GET_REG_OFFSET(clk);
    uint32_t shift = CLK_RESET_GET_BITS_SHIFT(clk);

    HAL_ASSERT(shift < 16);
#ifdef CRU_SRST_CON_CNT
    if (index < CRU_SRST_CON_CNT) {
        CRU->CRU_SOFTRST_CON[index] = VAL_MASK_WE(1U << shift, 0U << shift);
    } else {
        PMUCRU->CRU_SOFTRST_CON[index - CRU_SRST_CON_CNT] = VAL_MASK_WE(1U << shift, 0U << shift);
    }
#else
    CRU->CRU_SOFTRST_CON[index] = VAL_MASK_WE(1U << shift, 0U << shift);
#endif

    return HAL_OK;
}

HAL_Status HAL_CRU_ClkResetSyncAssert(int numClks, uint32_t *clks)
{
    uint32_t index = CLK_RESET_GET_REG_OFFSET(clks[0]);
    uint32_t val = 0;
    int i;

    for (i = 0; i < numClks; i++) {
        val |= HAL_BIT(CLK_RESET_GET_BITS_SHIFT(clks[i]));
        if (index != CLK_RESET_GET_REG_OFFSET(clks[i])) {
            return HAL_ERROR;
        }
    }

#ifdef CRU_SRST_CON_CNT
    if (index < CRU_SRST_CON_CNT) {
        CRU->CRU_SOFTRST_CON[index] = VAL_MASK_WE(val, val);
    } else {
        PMUCRU->CRU_SOFTRST_CON[index - CRU_SRST_CON_CNT] = VAL_MASK_WE(val, val);
    }
#else
    CRU->CRU_SOFTRST_CON[index] = VAL_MASK_WE(val, val);
#endif
    HAL_DBG("%s: index: 0x%lx, val: 0x%lx\n", __func__, index, val);

    return HAL_OK;
}

HAL_Status HAL_CRU_ClkResetSyncDeassert(int numClks, uint32_t *clks)
{
    uint32_t index = CLK_RESET_GET_REG_OFFSET(clks[0]);
    uint32_t val = 0;
    int i;

    for (i = 0; i < numClks; i++) {
        val |= HAL_BIT(CLK_RESET_GET_BITS_SHIFT(clks[i]));
        if (index != CLK_RESET_GET_REG_OFFSET(clks[i])) {
            return HAL_ERROR;
        }
    }

#ifdef CRU_SRST_CON_CNT
    if (index < CRU_SRST_CON_CNT) {
        CRU->CRU_SOFTRST_CON[index] = VAL_MASK_WE(val, 0);
    } else {
        PMUCRU->CRU_SOFTRST_CON[index - CRU_SRST_CON_CNT] = VAL_MASK_WE(val, 0);
    }
#else
    CRU->CRU_SOFTRST_CON[index] = VAL_MASK_WE(val, 0);
#endif
    HAL_DBG("%s: index: 0x%lx, val: 0x%lx\n", __func__, index, val);

    return HAL_OK;
}

HAL_Status HAL_CRU_ClkSetDiv(uint32_t divName, uint32_t divValue)
{
    uint32_t shift, mask, index;

    index = CLK_DIV_GET_REG_OFFSET(divName);
    shift = CLK_DIV_GET_BITS_SHIFT(divName);
    HAL_ASSERT(shift < 16);
    mask = CLK_DIV_GET_MASK(divName);
    if (divValue > mask) {
        divValue = mask;
    }

#ifdef CRU_CLK_DIV_CON_CNT
    if (index < CRU_CLK_DIV_CON_CNT) {
        CRU->CRU_CLKSEL_CON[index] = VAL_MASK_WE(mask, (divValue - 1U) << shift);
    } else {
#ifdef PMUCRU_BASE
        PMUCRU->CRU_CLKSEL_CON[index - CRU_CLK_DIV_CON_CNT] = VAL_MASK_WE(mask, (divValue - 1U) << shift);
#else
        CRU->PMU_CLKSEL_CON[index - CRU_CLK_DIV_CON_CNT] = VAL_MASK_WE(mask, (divValue - 1U) << shift);
#endif
    }
#else
    CRU->CRU_CLKSEL_CON[index] = VAL_MASK_WE(mask, (divValue - 1U) << shift);
#endif

    return HAL_OK;
}

uint32_t HAL_CRU_ClkGetDiv(uint32_t divName)
{
    uint32_t shift, mask, index, divValue;

    index = CLK_DIV_GET_REG_OFFSET(divName);
    shift = CLK_DIV_GET_BITS_SHIFT(divName);
    HAL_ASSERT(shift < 16);
    mask = CLK_DIV_GET_MASK(divName);

#ifdef CRU_CLK_DIV_CON_CNT
    if (index < CRU_CLK_DIV_CON_CNT) {
        divValue = ((((CRU->CRU_CLKSEL_CON[index]) & mask) >> shift) + 1);
    } else {
#ifdef PMUCRU_BASE
        divValue = ((((PMUCRU->CRU_CLKSEL_CON[index - CRU_CLK_DIV_CON_CNT]) & mask) >> shift) + 1);
#else
        divValue = ((((CRU->PMU_CLKSEL_CON[index - CRU_CLK_DIV_CON_CNT]) & mask) >> shift) + 1);
#endif
    }
#else
    divValue = ((((CRU->CRU_CLKSEL_CON[index]) & mask) >> shift) + 1);
#endif

    return divValue;
}

HAL_SECTION_SRAM_CODE
HAL_Status HAL_CRU_ClkSetMux(uint32_t muxName, uint32_t muxValue)
{
    uint32_t shift, mask, index;

    index = CLK_MUX_GET_REG_OFFSET(muxName);
    shift = CLK_MUX_GET_BITS_SHIFT(muxName);
    HAL_ASSERT(shift < 16);
    mask = CLK_MUX_GET_MASK(muxName);

#ifdef CRU_CLK_SEL_CON_CNT
    if (index < CRU_CLK_DIV_CON_CNT) {
        CRU->CRU_CLKSEL_CON[index] = VAL_MASK_WE(mask, muxValue << shift);
    } else {
#ifdef PMUCRU_BASE
        PMUCRU->CRU_CLKSEL_CON[index - CRU_CLK_SEL_CON_CNT] = VAL_MASK_WE(mask, muxValue << shift);
#else
        CRU->PMU_CLKSEL_CON[index - CRU_CLK_SEL_CON_CNT] = VAL_MASK_WE(mask, muxValue << shift);
#endif
    }
#else
    CRU->CRU_CLKSEL_CON[index] = VAL_MASK_WE(mask, muxValue << shift);
#endif

    return HAL_OK;
}

HAL_SECTION_SRAM_CODE
uint32_t HAL_CRU_ClkGetMux(uint32_t muxName)
{
    uint32_t shift, mask, index, muxValue;

    index = CLK_MUX_GET_REG_OFFSET(muxName);
    shift = CLK_MUX_GET_BITS_SHIFT(muxName);
    HAL_ASSERT(shift < 16);
    mask = CLK_MUX_GET_MASK(muxName);

#ifdef CRU_CLK_SEL_CON_CNT
    if (index < CRU_CLK_SEL_CON_CNT) {
        muxValue = ((CRU->CRU_CLKSEL_CON[index] & mask) >> shift);
    } else {
#ifdef PMUCRU_BASE
        muxValue = ((PMUCRU->CRU_CLKSEL_CON[index - CRU_CLK_SEL_CON_CNT] & mask) >> shift);
#else
        muxValue = ((CRU->PMU_CLKSEL_CON[index - CRU_CLK_SEL_CON_CNT] & mask) >> shift);
#endif
    }
#else
    muxValue = ((CRU->CRU_CLKSEL_CON[index] & mask) >> shift);
#endif

    return muxValue;
}

HAL_Status HAL_CRU_ClkSetFracDiv(uint32_t fracDivName,
                                 uint32_t numerator,
                                 uint32_t denominator)
{
    uint32_t index;

    index = CLK_DIV_GET_REG_OFFSET(fracDivName);
#ifdef CRU_CLK_DIV_CON_CNT
    if (index < CRU_CLK_DIV_CON_CNT) {
        CRU->CRU_CLKSEL_CON[index] = (numerator << 16) | denominator;
    } else {
#ifdef PMUCRU_BASE
        PMUCRU->CRU_CLKSEL_CON[index - CRU_CLK_DIV_CON_CNT] = (numerator << 16) | denominator;
#else
        CRU->PMU_CLKSEL_CON[index - CRU_CLK_DIV_CON_CNT] = (numerator << 16) | denominator;
#endif
    }
#else
    CRU->CRU_CLKSEL_CON[index] = (numerator << 16) | denominator;
#endif

    return HAL_OK;
}

HAL_Status HAL_CRU_ClkGetFracDiv(uint32_t fracDivName,
                                 uint32_t *numerator,
                                 uint32_t *denominator)
{
    uint32_t index;
    uint32_t val;

    index = CLK_DIV_GET_REG_OFFSET(fracDivName);
#ifdef CRU_CLK_DIV_CON_CNT
    if (index < CRU_CLK_DIV_CON_CNT) {
        val = CRU->CRU_CLKSEL_CON[index];
    } else {
#ifdef PMUCRU_BASE
        val = PMUCRU->CRU_CLKSEL_CON[index - CRU_CLK_DIV_CON_CNT];
#else
        val = CRU->CRU_CLKSEL_CON[index - CRU_CLK_DIV_CON_CNT];
#endif
    }
#else
    val = CRU->CRU_CLKSEL_CON[index];
#endif

    *numerator = (val & 0xffff0000) >> 16;
    *denominator = (val & 0x0000ffff);

    return HAL_OK;
}
#endif /* CRU_CLK_USE_CON_BANK */

HAL_Status HAL_CRU_FracdivGetConfig(uint32_t rateOut, uint32_t rate,
                                    uint32_t *numerator,
                                    uint32_t *denominator)
{
    uint32_t gcdVal;

    gcdVal = CRU_Gcd(rate, rateOut);
    if (!gcdVal) {
        return HAL_ERROR;
    }

    *numerator = rateOut / gcdVal;
    *denominator = rate / gcdVal;

    if (*numerator < 4) {
        *numerator *= 4;
        *denominator *= 4;
    }
    if (*numerator > 0xffff || *denominator > 0xffff) {
        return HAL_INVAL;
    }

    return HAL_OK;
}

HAL_Status HAL_CRU_ClkNp5BestDiv(eCLOCK_Name clockName, uint32_t rate, uint32_t pRate, uint32_t *bestdiv)
{
    uint32_t div = CLK_GET_DIV(clockName);
    uint32_t maxDiv = CLK_DIV_GET_MASK(div);
    uint32_t i;

    for (i = 0; i < maxDiv; i++) {
        if (((pRate * 2) == (rate * (i * 2 + 3)))) {
            *bestdiv = i;

            return HAL_OK;
        }
    }

    return HAL_ERROR;
}

__WEAK HAL_Status HAL_CRU_VopDclkEnable(uint32_t gateId)
{
    HAL_CRU_ClkEnable(gateId);

    return HAL_OK;
}

__WEAK HAL_Status HAL_CRU_VopDclkDisable(uint32_t gateId)
{
    HAL_CRU_ClkDisable(gateId);

    return HAL_OK;
}

HAL_Status HAL_CRU_SetGlbSrst(eCRU_GlbSrstType type)
{
#ifdef CRU_GLB_SRST_FST_VALUE_OFFSET
    if (type == GLB_SRST_FST) {
        CRU->GLB_SRST_FST_VALUE = GLB_SRST_FST;
    }
#endif
#ifdef CRU_GLB_SRST_SND_VALUE_OFFSET
    if (type == GLB_SRST_SND) {
        CRU->GLB_SRST_SND_VALUE = GLB_SRST_SND;
    }
#endif

    return HAL_INVAL;
}

/** @} */

/** @} */

/** @} */

#endif /* HAL_CRU_MODULE_ENABLED */
