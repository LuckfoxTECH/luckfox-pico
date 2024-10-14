/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_PMU_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup PD
 *  @{
 */

/** @defgroup PD_How_To_Use How To Use
 *  @{

 The PD driver can be used as follows:

 - Invoke HAL_PD_Setting in each device power on/off its own Pd.
 - The order of setting power domain.

       power on:
           set power domain on
           leave idle
       power off:
           request ilde
           set power domain off

 - The PD ID is include shift information:

       [3:0]: power on shift
       [7:4]: power on status shift
       [11:8]: idle request shift
       [15:12]: idle status shift
       [19:16]: ack status shift

 - PD driver is just responsible for passing simple command data, And
   the usecount is the user's responsibility. Protection the usecount at the driver layer.
 - More details refer to APIs' descriptions as below.

 @} */

/** @defgroup PD_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/
#if defined(SOC_RK3588)
/*
 * The bitmap[0] of PM_DOMAIN_INFO is include shift information:
 *    [0]:     power on valid
 *    [2:1]:   power on offset
 *    [3]:     power on write mask
 *    [8:4]:   power on shift
 *    [10:9]:  power on status offset
 *    [15:11]: power on status shift
 *    [16]:    idle request valid
 *    [18:17]: idle request offset
 *    [19]:    idle request write mask
 *    [24:20]: idle request shift
 *    [26:25]: idle status offset
 *    [31:27]: idle status shift
 *    [33:32]: idle ack offset
 *    [38:34]: idle ack shift
 *    [40:39]: 0: independent reg
 *             1: idle ack reg  = idle status reg
 *             2: idle aclk reg = idle status reg = power status reg
 *             3: reserved
 *    [41]:    mem power on valid
 *    [43:42]: mem power on offset
 *    [48:44]: mem power on shift
 *    [63:49]: reserved
 *
 * The bitmap[1] of PM_DOMAIN_INFO is include shift information:
 *    [0]:     power mem status valid
 *    [2:1]:   power mem offset
 *    [7:3]:   power mem shift
 *    [8]:     power chain status valid
 *    [10:9]:  power chain status offset
 *    [15:11]: power chain status shift
 *    [16]:    power repair status valid
 *    [18:17]: power repair status offset
 *    [23:19]: power repair status shift
 *    [24]:    idle1 request valid
 *    [26:25]: idle1 request offset
 *    [31:27]: idle1 request shift
 *    [33:32]: idle1 status offset
 *    [38:34]: idle1 status shift
 *    [40:39]: idle1 ack offset
 *    [45:41]: idle1 ack shift
 *    [63:46]: reserved
 */
#define PD_GET_PWR_VALID(x)      HAL_GENVAL_ULL(x, 0, 0)
#define PD_GET_PWR_OFFSET(x)     HAL_GENVAL_ULL(x, 2, 1)
#define PD_GET_PWR_MASK(x)       HAL_GENVAL_ULL(x, 3, 3)
#define PD_GET_PWR_SHIFT(x)      HAL_GENVAL_ULL(x, 8, 4)
#define PD_GET_ST_OFFSET(x)      HAL_GENVAL_ULL(x, 10, 9)
#define PD_GET_ST_SHIFT(x)       HAL_GENVAL_ULL(x, 15, 11)
#define PD_GET_REQ_VALID(x)      HAL_GENVAL_ULL(x, 16, 16)
#define PD_GET_REQ_OFFSET(x)     HAL_GENVAL_ULL(x, 18, 17)
#define PD_GET_REQ_MASK(x)       HAL_GENVAL_ULL(x, 19, 19)
#define PD_GET_REQ_SHIFT(x)      HAL_GENVAL_ULL(x, 24, 20)
#define PD_GET_IDLE_OFFSET(x)    HAL_GENVAL_ULL(x, 26, 25)
#define PD_GET_IDLE_SHIFT(x)     HAL_GENVAL_ULL(x, 31, 27)
#define PD_GET_ACK_OFFSET(x)     HAL_GENVAL_ULL(x, 33, 32)
#define PD_GET_ACK_SHIFT(x)      HAL_GENVAL_ULL(x, 38, 34)
#define PD_GET_IDLE_ACK_REG(x)   HAL_GENVAL_ULL(x, 40, 39)
#define PD_GET_MEM_PWR_VALID(x)  HAL_GENVAL_ULL(x, 41, 41)
#define PD_GET_MEM_PWR_OFFSET(x) HAL_GENVAL_ULL(x, 43, 42)
#define PD_GET_MEM_PWR_SHIFT(x)  HAL_GENVAL_ULL(x, 48, 44)

#define PD_GET_MEM_ST_VALID(x)     HAL_GENVAL_ULL(x, 0, 0)
#define PD_GET_MEM_ST_OFFSET(x)    HAL_GENVAL_ULL(x, 2, 1)
#define PD_GET_MEM_ST_SHIFT(x)     HAL_GENVAL_ULL(x, 7, 3)
#define PD_GET_CHAIN_ST_VALID(x)   HAL_GENVAL_ULL(x, 8, 8)
#define PD_GET_CHAIN_ST_OFFSET(x)  HAL_GENVAL_ULL(x, 10, 9)
#define PD_GET_CHAIN_ST_SHIFT(x)   HAL_GENVAL_ULL(x, 15, 11)
#define PD_GET_REPAIR_VALID(x)     HAL_GENVAL_ULL(x, 16, 16)
#define PD_GET_REPAIR_ST_OFFSET(x) HAL_GENVAL_ULL(x, 18, 17)
#define PD_GET_REPAIR_ST_SHIFT(x)  HAL_GENVAL_ULL(x, 23, 19)
#define PD_GET_REQ1_VALID(x)       HAL_GENVAL_ULL(x, 24, 24)
#define PD_GET_REQ1_OFFSET(x)      HAL_GENVAL_ULL(x, 26, 25)
#define PD_GET_REQ1_SHIFT(x)       HAL_GENVAL_ULL(x, 31, 27)
#define PD_GET_IDLE1_OFFSET(x)     HAL_GENVAL_ULL(x, 33, 32)
#define PD_GET_IDLE1_SHIFT(x)      HAL_GENVAL_ULL(x, 38, 34)
#define PD_GET_ACK1_OFFSET(x)      HAL_GENVAL_ULL(x, 40, 39)
#define PD_GET_ACK1_SHIFT(x)       HAL_GENVAL_ULL(x, 45, 41)

#define PD_IS_INVALID(x)  (!HAL_GENVAL(x, 31, 31))
#define PD_GET_VALIDID(x) HAL_GENVAL(x, 30, 0)

#define PD_PWR_VALID      0
#define PD_PWR_OFFSET     1
#define PD_PWR_MASK       3
#define PD_PWR_SHIFT      4
#define PD_ST_OFFSET      9
#define PD_ST_SHIFT       11
#define PD_REQ_VALID      16
#define PD_REQ_OFFSET     17
#define PD_REQ_MASK       19
#define PD_REQ_SHIFT      20
#define PD_IDLE_OFFSET    25
#define PD_IDLE_SHIFT     27
#define PD_ACK_OFFSET     32
#define PD_ACK_SHIFT      34
#define PD_MEM_PWR_VALID  41
#define PD_MEM_PWR_OFFSET 42
#define PD_MEM_PWR_SHIFT  44

#define PD_MEM_ST_VALID     0
#define PD_MEM_ST_OFFSET    1
#define PD_MEM_ST_SHIFT     3
#define PD_CHAIN_ST_VALID   8
#define PD_CHAIN_ST_OFFSET  9
#define PD_CHAIN_ST_SHFIT   11
#define PD_REPAIR_VALID     16
#define PD_REPAIR_ST_OFFSET 17
#define PD_REPAIR_ST_SHIFT  19
#define PD_REQ1_VALID       24
#define PD_REQ1_OFFSET      25
#define PD_REQ1_SHIFT       27
#define PD_IDLE1_OFFSET     32
#define PD_IDLE1_SHIFT      34
#define PD_ACK1_OFFSET      39
#define PD_ACK1_SHIFT       41

#define DOMAIN_RK3588_ULL(p_off, pwr, pwr_st, m_valid, m_off, m_st, r_valid, r_st, \
                          req_valid, req_off, req, idle, req1_valid, req1, idle1)  \
{                                                                                  \
    .bitmap[0] = (1 <<  PD_PWR_VALID) | (1 << PD_PWR_MASK) |                       \
             (p_off << PD_PWR_OFFSET) | (pwr << PD_PWR_SHIFT) |                    \
             (pwr_st << PD_ST_SHIFT) | (p_off << PD_MEM_PWR_OFFSET) |              \
             (pwr << PD_MEM_PWR_SHIFT) | (req_valid << PD_REQ_VALID) |             \
             (req_off << PD_REQ_OFFSET) | (req << PD_REQ_SHIFT) |                  \
             (1 << PD_REQ_MASK) | (idle << PD_IDLE_SHIFT) |                        \
             (idle << PD_ACK_SHIFT),                                               \
    .bitmap[1] = (r_valid << PD_REPAIR_VALID) | (r_st << PD_REPAIR_ST_SHIFT) |     \
             (m_valid << PD_MEM_ST_VALID) | (m_off << PD_MEM_ST_OFFSET) |          \
             (m_st << PD_MEM_ST_SHIFT) | (m_valid << PD_CHAIN_ST_VALID) |          \
             (m_off << PD_CHAIN_ST_OFFSET) | (m_st << PD_CHAIN_ST_SHFIT) |         \
             (req1_valid << PD_REQ1_VALID) | (req1 << PD_REQ1_SHIFT) |             \
             (req_off << PD_REQ1_OFFSET) | (idle1 << PD_IDLE1_SHIFT) |             \
             (idle1 << PD_ACK1_SHIFT),                                             \
}

#define DOMAIN_RK3588(p_off, pwr, pwr_st, m_valid, m_off, m_st, r_valid, r_st, \
                      req_valid, req_off, req, idle, req1_valid, req1, idle1)  \
    DOMAIN_RK3588_ULL(UINT64_C(p_off), UINT64_C(pwr), UINT64_C(pwr_st),        \
              UINT64_C(m_valid), UINT64_C(m_off), UINT64_C(m_st),              \
              UINT64_C(r_valid), UINT64_C(r_st), UINT64_C(req_valid),          \
              UINT64_C(req_off), UINT64_C(req), UINT64_C(idle),                \
              UINT64_C(req1_valid), UINT64_C(req1), UINT64_C(idle1))           \

#else
#define PD_PWR_SHIFT   0U
#define PD_PWR_MASK    0x0000000FU
#define PD_ST_SHIFT    4U
#define PD_ST_MASK     0x000000F0U
#define PD_REQ_SHIFT   8U
#define PD_REQ_MASK    0x00000F00U
#define PD_IDLE_SHIFT  12U
#define PD_IDLE_MASK   0x0000F000U
#define PD_ACK_SHIFT   16U
#define PD_ACK_MASK    0x000F0000U
#define PD_VALID_SHIFT 31U
#define PD_VALID_MASK  0x80000000U

#define PD_GET_PWR_SHIFT(x) (((uint32_t)(x)&PD_PWR_MASK) >> PD_PWR_SHIFT)
#define PD_GET_ST_SHIFT(x)  (((uint32_t)(x)&PD_ST_MASK) >> PD_ST_SHIFT)
#define PD_GET_REQ_SHIFT(x) (((uint32_t)(x)&PD_REQ_MASK) >> PD_REQ_SHIFT)
#if defined(SOC_RK1808)
#define PD_GET_IDLE_SHIFT(x) ((((uint32_t)(x)&PD_IDLE_MASK) >> PD_IDLE_SHIFT) + 16)
#else
#define PD_GET_IDLE_SHIFT(x) (((uint32_t)(x)&PD_IDLE_MASK) >> PD_IDLE_SHIFT)
#endif
#define PD_GET_ACK_SHIFT(x) (((uint32_t)(x)&PD_ACK_MASK) >> PD_ACK_SHIFT)

#define PD_IS_INVALID(x) (!(((uint32_t)(x)&PD_VALID_MASK) >> PD_VALID_SHIFT))

#endif
/********************* Private Structure Definition **************************/

#if defined(SOC_RK3588)
struct POWER_DOMAIN_INFO {
    uint64_t bitmap[2];
};

static const struct POWER_DOMAIN_INFO powerDomains[] = {
    /* p_off pwr pwr_st m_valid m_off m_st r_valid r_st req_valid req_off req  idle req1_valid req1 idle1 */
    [RK3588_PD_GPU] = DOMAIN_RK3588(0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0),
    [RK3588_PD_NPU] = DOMAIN_RK3588(0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
    [RK3588_PD_VCODEC] = DOMAIN_RK3588(0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
    [RK3588_PD_NPUTOP] = DOMAIN_RK3588(0, 3, 0, 1, 0, 11, 1, 2, 1, 0, 1, 1, 0, 0, 0),
    [RK3588_PD_NPU1] = DOMAIN_RK3588(0, 4, 0, 1, 0, 12, 1, 3, 1, 0, 2, 2, 0, 0, 0),
    [RK3588_PD_NPU2] = DOMAIN_RK3588(0, 5, 0, 1, 0, 13, 1, 4, 1, 0, 3, 3, 0, 0, 0),
    [RK3588_PD_VENC0] = DOMAIN_RK3588(0, 6, 0, 1, 0, 14, 1, 5, 1, 0, 4, 4, 0, 0, 0),
    [RK3588_PD_VENC1] = DOMAIN_RK3588(0, 7, 0, 1, 0, 15, 1, 6, 1, 0, 5, 5, 0, 0, 0),
    [RK3588_PD_RKVDEC0] = DOMAIN_RK3588(0, 8, 0, 1, 0, 16, 1, 7, 1, 0, 6, 6, 0, 0, 0),
    [RK3588_PD_RKVDEC1] = DOMAIN_RK3588(0, 9, 0, 1, 0, 17, 1, 8, 1, 0, 7, 7, 0, 0, 0),
    [RK3588_PD_VDPU] = DOMAIN_RK3588(0, 10, 0, 1, 0, 18, 1, 9, 1, 0, 8, 8, 0, 0, 0),
    [RK3588_PD_RGA30] = DOMAIN_RK3588(0, 11, 0, 1, 0, 19, 1, 10, 0, 0, 0, 0, 0, 0, 0),
    [RK3588_PD_AV1] = DOMAIN_RK3588(0, 12, 0, 1, 0, 20, 1, 11, 1, 0, 9, 9, 0, 0, 0),
    [RK3588_PD_VI] = DOMAIN_RK3588(0, 13, 0, 1, 0, 21, 1, 12, 1, 0, 10, 10, 0, 0, 0),
    [RK3588_PD_FEC] = DOMAIN_RK3588(0, 14, 0, 1, 0, 22, 1, 13, 0, 0, 0, 0, 0, 0, 0),
    [RK3588_PD_ISP1] = DOMAIN_RK3588(0, 15, 0, 1, 0, 23, 1, 14, 1, 0, 11, 11, 0, 0, 0),
    [RK3588_PD_RGA31] = DOMAIN_RK3588(1, 0, 0, 1, 0, 24, 1, 15, 1, 0, 12, 12, 0, 0, 0),
    [RK3588_PD_VOP] = DOMAIN_RK3588(1, 1, 0, 1, 0, 25, 1, 16, 1, 0, 13, 13, 1, 14, 14),
    [RK3588_PD_VO0] = DOMAIN_RK3588(1, 2, 0, 1, 0, 26, 1, 17, 1, 0, 15, 15, 0, 0, 0),
    [RK3588_PD_VO1] = DOMAIN_RK3588(1, 3, 0, 1, 0, 27, 1, 18, 1, 1, 0, 16, 0, 0, 0),
    [RK3588_PD_AUDIO] = DOMAIN_RK3588(1, 4, 0, 1, 0, 28, 1, 19, 1, 1, 1, 17, 0, 0, 0),
    [RK3588_PD_PHP] = DOMAIN_RK3588(1, 5, 0, 1, 0, 29, 1, 20, 1, 1, 5, 21, 0, 0, 0),
    [RK3588_PD_GMAC] = DOMAIN_RK3588(1, 6, 0, 1, 0, 30, 1, 21, 0, 0, 0, 0, 0, 0, 0),
    [RK3588_PD_PCIE] = DOMAIN_RK3588(1, 7, 0, 1, 0, 31, 1, 22, 0, 0, 0, 0, 0, 0, 0),
    [RK3588_PD_NVM] = DOMAIN_RK3588(1, 8, 24, 0, 1, 0, 0, 0, 1, 1, 2, 18, 0, 0, 0),
    [RK3588_PD_NVM0] = DOMAIN_RK3588(1, 9, 0, 1, 1, 1, 1, 23, 0, 0, 0, 0, 0, 0, 0),
    [RK3588_PD_SDIO] = DOMAIN_RK3588(1, 10, 0, 1, 1, 2, 1, 24, 1, 1, 3, 19, 0, 0, 0),
    [RK3588_PD_USB] = DOMAIN_RK3588(1, 11, 0, 1, 1, 3, 1, 25, 1, 1, 4, 20, 0, 0, 0),
    [RK3588_PD_SDMMC] = DOMAIN_RK3588(1, 13, 0, 1, 1, 5, 1, 26, 0, 0, 0, 0, 0, 0, 0),
};
#endif
/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/
#if defined(SOC_RK3588)
static HAL_Check PD_IsIdle(ePD_Id pd, HAL_Check idle)
{
    uint32_t id = PD_GET_VALIDID(pd);
    const struct POWER_DOMAIN_INFO *domain = &powerDomains[id];
    uint32_t index, index1, shift, shift1;
    uint32_t status, status1;

    index = PD_GET_IDLE_OFFSET(domain->bitmap[0]);
    shift = PD_GET_IDLE_SHIFT(domain->bitmap[0]);
    status = (PMU->BUS_IDLE_ST[index] & (1 << shift)) >> shift;

    if (!PD_GET_REQ1_VALID(domain->bitmap[1])) {
        return (HAL_Check)(status == idle);
    }

    index1 = PD_GET_IDLE1_OFFSET(domain->bitmap[1]);
    shift1 = PD_GET_IDLE1_SHIFT(domain->bitmap[1]);
    status1 = (PMU->BUS_IDLE_ST[index1] & (1 << shift1)) >> shift1;

    return (HAL_Check)((status == idle) && (status1 == idle));
}

static HAL_Check PD_ReadAck(ePD_Id pd, HAL_Check idle)
{
    uint32_t id = PD_GET_VALIDID(pd);
    const struct POWER_DOMAIN_INFO *domain = &powerDomains[id];
    uint32_t index, shift, index1, shift1;
    uint32_t ack, ack1;

    index = PD_GET_ACK_OFFSET(domain->bitmap[0]);
    shift = PD_GET_ACK_SHIFT(domain->bitmap[0]);
    ack = (PMU->BUS_IDLE_ACK[index] & (1 << shift)) >> shift;

    if (!PD_GET_REQ1_VALID(domain->bitmap[1])) {
        return (HAL_Check)(ack == idle);
    }

    index1 = PD_GET_ACK1_OFFSET(domain->bitmap[1]);
    shift1 = PD_GET_ACK1_SHIFT(domain->bitmap[1]);
    ack1 = (PMU->BUS_IDLE_ACK[index1] & (1 << shift1)) >> shift1;

    return (HAL_Check)((ack == idle) && (ack1 == idle));
}

static HAL_Check PD_IsMemOn(ePD_Id pd)
{
    uint32_t id = PD_GET_VALIDID(pd);
    const struct POWER_DOMAIN_INFO *domain = &powerDomains[id];
    uint32_t index, shift;

    index = PD_GET_MEM_ST_OFFSET(domain->bitmap[1]);
    shift = PD_GET_MEM_ST_SHIFT(domain->bitmap[1]);

    return (HAL_Check)(!((PMU->PWR_MEM_ST[index] & (1 << shift)) >> shift));
}

static HAL_Check PD_IsChainOn(ePD_Id pd)
{
    uint32_t id = PD_GET_VALIDID(pd);
    const struct POWER_DOMAIN_INFO *domain = &powerDomains[id];
    uint32_t index, shift;

    index = PD_GET_CHAIN_ST_OFFSET(domain->bitmap[1]);
    shift = PD_GET_CHAIN_ST_SHIFT(domain->bitmap[1]);

    return (HAL_Check)(((PMU->PWR_CHAIN1_ST[index] & (1 << shift)) >> shift));
}

static HAL_Check PD_IsOn(ePD_Id pd)
{
    uint32_t id = PD_GET_VALIDID(pd);
    const struct POWER_DOMAIN_INFO *domain = &powerDomains[id];
    uint32_t index, shift;

    if (PD_GET_REPAIR_VALID(domain->bitmap[1])) {
        index = PD_GET_REPAIR_ST_OFFSET(domain->bitmap[1]);
        shift = PD_GET_REPAIR_ST_SHIFT(domain->bitmap[1]);

        return (HAL_Check)(((PMU->PWR_REPAIR_ST[index] & (1 << shift)) >> shift));
    }

    index = PD_GET_ST_OFFSET(domain->bitmap[0]);
    shift = PD_GET_ST_SHIFT(domain->bitmap[0]);

    return (HAL_Check)(!((PMU->PWRDN_ST[index] & (1 << shift)) >> shift));
}

static HAL_Status PD_IdleRequest(ePD_Id pd, HAL_Check idle)
{
    uint32_t id = PD_GET_VALIDID(pd);
    const struct POWER_DOMAIN_INFO *domain = &powerDomains[id];
    uint32_t index, shift, index1, shift1;
    uint32_t start, timeoutMs = 1000;

    if (!PD_GET_REQ_VALID(domain->bitmap[0])) {
        return HAL_OK;
    }

    index = PD_GET_REQ_OFFSET(domain->bitmap[0]);
    shift = PD_GET_REQ_SHIFT(domain->bitmap[0]);
    PMU->BUS_IDLE_REQ[index] = VAL_MASK_WE(1U << shift, (idle ? 1U : 0U) << shift);

    if (PD_GET_REQ1_VALID(domain->bitmap[1])) {
        index1 = PD_GET_REQ1_OFFSET(domain->bitmap[1]);
        shift1 = PD_GET_REQ1_SHIFT(domain->bitmap[1]);
        PMU->BUS_IDLE_REQ[index1] = VAL_MASK_WE(1U << shift1, (idle ? 1U : 0U) << shift1);
    }

    start = HAL_GetTick();
    while (!PD_ReadAck(pd, idle)) {
        if ((HAL_GetTick() - start) > timeoutMs) {
            return HAL_TIMEOUT;
        }
    }

    start = HAL_GetTick();
    while (!PD_IsIdle(pd, idle)) {
        if ((HAL_GetTick() - start) > timeoutMs) {
            return HAL_TIMEOUT;
        }
    }

    return HAL_OK;
}

static HAL_Status PD_MemRest(ePD_Id pd)
{
    uint32_t id = PD_GET_VALIDID(pd);
    const struct POWER_DOMAIN_INFO *domain = &powerDomains[id];
    uint32_t index = PD_GET_PWR_OFFSET(domain->bitmap[0]);
    uint32_t shift = PD_GET_PWR_SHIFT(domain->bitmap[0]);
    uint32_t start, timeoutMs = 1000;

    start = HAL_GetTick();
    while (!PD_IsChainOn(pd)) {
        if ((HAL_GetTick() - start) > timeoutMs) {
            return HAL_TIMEOUT;
        }
    }

    HAL_CPUDelayUs(60);

    PMU->PWR_MEM_CON[index] = VAL_MASK_WE(1U << shift, 0U << shift);

    start = HAL_GetTick();
    while (PD_IsMemOn(pd)) {
        if ((HAL_GetTick() - start) > timeoutMs) {
            return HAL_TIMEOUT;
        }
    }

    PMU->PWR_MEM_CON[index] = VAL_MASK_WE(1U << shift, 1U << shift);

    start = HAL_GetTick();
    while (!PD_IsMemOn(pd)) {
        if ((HAL_GetTick() - start) > timeoutMs) {
            return HAL_TIMEOUT;
        }
    }

    return HAL_OK;
}

static HAL_Status PD_PowerOn(ePD_Id pd, HAL_Check on)
{
    uint32_t id = PD_GET_VALIDID(pd);
    const struct POWER_DOMAIN_INFO *domain = &powerDomains[id];
    uint32_t index = PD_GET_PWR_OFFSET(domain->bitmap[0]);
    uint32_t shift = PD_GET_PWR_SHIFT(domain->bitmap[0]);
    uint32_t start, timeoutMs = 1000;
    bool isMemReset = false;

    if (PD_GET_MEM_ST_VALID(domain->bitmap[1]) && on) {
        isMemReset = PD_IsMemOn(pd);
    }

    PMU->PWRDN_CON[index] = VAL_MASK_WE(1U << shift, (on ? 0U : 1U) << shift);

    if (isMemReset) {
        if (PD_MemRest(pd) != HAL_OK) {
            return HAL_ERROR;
        }
    }

    start = HAL_GetTick();
    while (PD_IsOn(pd) != on) {
        if ((HAL_GetTick() - start) > timeoutMs) {
            return HAL_TIMEOUT;
        }
    }

    return HAL_OK;
}

#else
static HAL_Check PD_IsIdle(ePD_Id pd)
{
    uint32_t idleShift = PD_GET_IDLE_SHIFT(pd);

    return (HAL_Check)((PMU->BUS_IDLE_ST & (1 << idleShift)) >> idleShift);
}

#if defined(SOC_RK1808)
static HAL_Check PD_ReadAck(ePD_Id pd)
{
    uint32_t ackShift = PD_GET_ACK_SHIFT(pd);

    return (HAL_Check)((PMU->BUS_IDLE_ST & (1 << ackShift)) >> ackShift);
}
#else
static HAL_Check PD_ReadAck(ePD_Id pd)
{
    uint32_t ackShift = PD_GET_ACK_SHIFT(pd);

    return (HAL_Check)((PMU->BUS_IDLE_ACK & (1 << ackShift)) >> ackShift);
}
#endif

static HAL_Check PD_IsOn(ePD_Id pd)
{
    uint32_t stShift = PD_GET_ST_SHIFT(pd);

    /* check idle status for idle-only domains */

    if (stShift > 16) {
        return PD_IsIdle(pd) ? HAL_FALSE : HAL_TRUE;
    }

    return (HAL_Check)(!((PMU->PWRDN_ST & (1 << stShift)) >> stShift));
}

static HAL_Status PD_IdleRequest(ePD_Id pd, HAL_Check idle)
{
    uint32_t reqShift = PD_GET_REQ_SHIFT(pd);
    uint32_t start, timeoutMs = 1000;

    if (reqShift > 16) {
        return HAL_INVAL;
    }

    PMU->BUS_IDLE_REQ = VAL_MASK_WE(1U << reqShift, (idle ? 1U : 0U) << reqShift);

    /* Wait util idle_ack = 1 */
    start = HAL_GetTick();

    while (PD_ReadAck(pd) != idle) {
        if ((HAL_GetTick() - start) > timeoutMs) {
            return HAL_TIMEOUT;
        }
    }

    start = HAL_GetTick();
    while (PD_IsIdle(pd) != idle) {
        if ((HAL_GetTick() - start) > timeoutMs) {
            return HAL_TIMEOUT;
        }
    }

    return HAL_OK;
}

static HAL_Status PD_PowerOn(ePD_Id pd, HAL_Check on)
{
    uint32_t pwrShift = PD_GET_PWR_SHIFT(pd);
    uint32_t start, timeoutMs = 1000;

    if (pwrShift > 16) {
        return HAL_INVAL;
    }

    PMU->PWRDN_CON = VAL_MASK_WE(1U << pwrShift, (on ? 0U : 1U) << pwrShift);

    start = HAL_GetTick();

    while (PD_IsOn(pd) != on) {
        if ((HAL_GetTick() - start) > timeoutMs) {
            return HAL_TIMEOUT;
        }
    }

    return HAL_OK;
}
#endif

/** @} */
/********************* Public Function Definition ****************************/

/** @defgroup PD_Exported_Functions_Group5 Other Functions
 *  @attention these APIs allow direct use in the HAL layer.
 *  @{
 */

/**
 * @brief  Pd setting on
 * @param  pd: pd id
 * @return HAL_Status
 */
HAL_Status HAL_PD_On(ePD_Id pd)
{
    HAL_Status error;

    if (PD_IS_INVALID(pd)) {
        return HAL_INVAL;
    }

    if (PD_IsOn(pd)) {
        return HAL_OK;
    }

    error = PD_PowerOn(pd, HAL_TRUE);
    if (error) {
        return error;
    }

    /* if powering up, leave idle mode */
    error = PD_IdleRequest(pd, HAL_FALSE);

    return error;
}

/**
 * @brief  Pd setting off
 * @param  pd: pd id
 * @return HAL_Status
 */
HAL_Status HAL_PD_Off(ePD_Id pd)
{
    HAL_Status error;

    if (PD_IS_INVALID(pd)) {
        return HAL_INVAL;
    }

    if (!PD_IsOn(pd)) {
        return HAL_OK;
    }

    /* if powering down, idle request to NIU first */
    error = PD_IdleRequest(pd, HAL_TRUE);
    if (error) {
        return error;
    }

    error = PD_PowerOn(pd, HAL_FALSE);

    return error;
}

/** @} */

/** @} */

/** @} */

#endif /* HAL_PMU_MODULE_ENABLED */
