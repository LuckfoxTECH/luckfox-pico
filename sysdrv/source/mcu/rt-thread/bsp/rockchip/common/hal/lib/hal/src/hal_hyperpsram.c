/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_HYPERPSRAM_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup HYPERPSRAM
 *  @{
 */

/** @defgroup HYPERPSRAM_How_To_Use How To Use
 *  @{

 The HYPERPSRAM driver can be used as follows:

  - Initialize the HYPERPSRAM (HAL_HYPERPSRAM_Init()):

 @} */
/** @defgroup HYPERPSRAM_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/
/** Memory Base Address  */
#define HYPERBUS_MBR0_BASE_ADDR_MASK (0xFF000000UL)

/** Memory Configuration Register0 */
#define HYPERBUS_MCR0_MAXEN_CONF_LOW   (0x1 << HYPERBUS_MCR0_MAXEN_SHIFT)
#define HYPERBUS_MCR0_MAXLEN_MAX       (0x1FF << HYPERBUS_MCR0_MAXLEN_SHIFT)
#define HYPERBUS_MCR0_CRT_CR_SPACE     (0x1 << HYPERBUS_MCR0_CRT_SHIFT)
#define HYPERBUS_MCR0_CRT_MEM_SPACE    (0x0 << HYPERBUS_MCR0_CRT_SHIFT)
#define HYPERBUS_MCR0_DEVTYPE_HYPERRAM (0x1 << HYPERBUS_MCR0_DEVTYPE_SHIFT)
#define HYPERBUS_MCR0_WRAPSIZE_32BYTE  (0x3 << HYPERBUS_MCR0_WRAPSIZE_SHIFT)

/** Memory Timing Register0 */
#define HYPERBUS_MTR0_LTCY_6_CLK (0x1)

#define PSRAM_MCR_TIMER(rcshi, wcshi, rcss, WCSS, RCSH, WCSH, LTCY) ((rcshi << HYPERBUS_MTR0_RCSHI_SHIFT) | \
    (wcshi << HYPERBUS_MTR0_WCSHI_SHIFT) |                                                                  \
    (rcss << HYPERBUS_MTR0_RCSS_SHIFT) |                                                                    \
    (WCSS << HYPERBUS_MTR0_WCSS_SHIFT) |                                                                    \
    (RCSH << HYPERBUS_MTR0_RCSH_SHIFT) |                                                                    \
    (WCSH << HYPERBUS_MTR0_WCSH_SHIFT) |                                                                    \
    (LTCY << HYPERBUS_MTR0_LTCY_SHIFT))
/** Loopback Register  */
#define HYPERBUS_LBR_DIS_LOOPBACK (0x0 << HYPERBUS_LBR_LOOPBACK_SHIFT)

/** Special Control Register  */
#define HYPERBUS_SPCSR_IS_W955D8  (0x3 << HYPERBUS_SPCSR_W955D8_CON_SHIFT)
#define HYPERBUS_SPCSR_NOT_W955D8 (0x0 << HYPERBUS_SPCSR_W955D8_CON_SHIFT)

/* device id bit mask */
#define HYPERBUS_DEV_ID_MASK (0xf)

/* TCSM */
#define HYPERBUS_DEV_TCSM_4U (4000)
#define HYPERBUS_DEV_TCSM_1U (1000)

/* Hyper psram register */
#define HYPERBUS_IR0_ADDR_OFFSET       (0x00)
#define HYPERBUS_IR1_ADDR_OFFSET       (0x01)
#define HYPERBUS_CR0_ADDR_OFFSET       (0x1000)
#define HYPERBUS_CR1_ADDR_OFFSET       (0x2000)
#define CR0_INITIAL_LATENCY_SHIFT      (0x4)
#define CR0_INITIAL_LATENCY_MASK       (0xf << CR0_INITIAL_LATENCY_SHIFT)
#define CR0_INITIAL_LATENCY_5_CLK      (0x0 << CR0_INITIAL_LATENCY_SHIFT)
#define CR0_INITIAL_LATENCY_6_CLK      (0x1 << CR0_INITIAL_LATENCY_SHIFT)
#define CR0_INITIAL_LATENCY_7_CLK      (0x2 << CR0_INITIAL_LATENCY_SHIFT)
#define CR0_FIXED_LATENCY_ENABLE_SHIFT (0x3)
#define CR0_FIXED_LATENCY_ENABLE_MASK  (0x1 << CR0_FIXED_LATENCY_ENABLE_SHIFT)
#define CR0_FIXED_LATENCY_ENABLE_VARIABLE_LATENCY \
    (0x0 << CR0_FIXED_LATENCY_ENABLE_SHIFT)
#define CR0_FIXED_LATENCY_ENABLE_FIXED_LATENCY \
    (0x1 << CR0_FIXED_LATENCY_ENABLE_SHIFT)
#define CR0_BURST_LENGTH_SHIFT   (0)
#define CR0_BURST_LENGTH_MASK    (0x3 << CR0_BURST_LENGTH_SHIFT)
#define CR0_BURST_LENGTH_128BYTE (0x0 << CR0_BURST_LENGTH_SHIFT)
#define CR0_BURST_LENGTH_64BYTE  (0x1 << CR0_BURST_LENGTH_SHIFT)
#define CR0_BURST_LENGTH_16BYTE  (0x2 << CR0_BURST_LENGTH_SHIFT)
#define CR0_BURST_LENGTH_32BYTE  (0x3 << CR0_BURST_LENGTH_SHIFT)

#define SIZE_128MBYTE         (0x08000000)
#define SIZE_1MBYTE           (0x00100000)
#define PATTERN1              (0x00000000)
#define PATTERN2              (0x5aa5f00f)
#define HYPERBUS_REG_AVA_FLAG (PATTERN2)

/********************* Private Structure Definition **************************/
/** HYPERBUS DEVICE Psram ID definition */
enum {
    S27KX0641 = 0x1,
    W955D8MKY = 0xf,
    W956X8MKY = 0x6,
};

enum HYPER_MTR_TIMING {
    MCR_TIMING_2_2_2_2_3_2_1 = PSRAM_MCR_TIMER(2, 2, 2, 2, 3, 2, 1),
    MCR_TIMING_2_2_2_2_3_2_2 = PSRAM_MCR_TIMER(2, 2, 2, 2, 3, 2, 2),
};

struct HYPER_PSTRAM {
    uint16_t id;
    uint32_t mtrTiming;
    uint32_t spc;
};

/** HYPWEBUS psram chip features */
static const struct HYPER_PSTRAM psramInfo[] =
{
    /**
     * W956X8MKY must init first, or it will report a ahb bus error
     * because of MTR error.
     */
    { W956X8MKY, MCR_TIMING_2_2_2_2_3_2_2, HYPERBUS_SPCSR_NOT_W955D8 },
    { S27KX0641, MCR_TIMING_2_2_2_2_3_2_1, HYPERBUS_SPCSR_NOT_W955D8 },
    { W955D8MKY, MCR_TIMING_2_2_2_2_3_2_1, HYPERBUS_SPCSR_IS_W955D8 },
};

/********************* Private Function Definition ***************************/

/**
 * @brief  Hyperbus psram read psram id.
 * @param  pReg: Choose HYPERBUS.
 * @param  psramBase: Choose psram map base addr.
 * @return PsramID: psram device id
 */
static uint16_t HYPERPSRAM_GetDevId(struct HYPERBUS_REG *pReg, uint32_t psramBase)
{
    uint16_t psramId;

    /* config to CR space */
    MODIFY_REG(pReg->MCR[0], HYPERBUS_MCR0_CRT_MASK,
               HYPERBUS_MCR0_CRT_CR_SPACE);
    psramId = (*(volatile uint16_t *)psramBase);
    /* config to memory space */
    MODIFY_REG(pReg->MCR[0], HYPERBUS_MCR0_CRT_MASK,
               HYPERBUS_MCR0_CRT_MEM_SPACE);

    return (psramId & HYPERBUS_DEV_ID_MASK);
}

/**
 * @brief  Hyperbus psram modify transaction maximum length/tcsm.
 * @param  pReg: Choose HYPERBUS.
 * @param  psramDev: psram device info.
 * @return HAL_Status.
 */
static HAL_Status HYPERPSRAM_ModifyTiming(struct HAL_HYPERPSRAM_DEV *pHyperPsramDev)
{
    uint32_t tmp, maxLength;
    uint32_t tcmd, tal;
    uint32_t freqMhz = pHyperPsramDev->psramFreq / 1000000;
    struct HYPERBUS_REG *pReg = pHyperPsramDev->pReg;

    if (pReg->MCR[0] & HYPERBUS_MCR0_MAXEN_CONF_LOW) {
        if (pHyperPsramDev->psramChip.id == W955D8MKY) {
            /*
             * The max length must less 63(128bytes) for W955D8MKY.
             * The peripherals like DMA access psram must be 128 address aligned.
             */
            maxLength = 63;
        } else {
            maxLength = 511;
        }

        tmp = pReg->MTR[0] & HYPERBUS_MTR0_LTCY_MASK;
        if (tmp < 3) {
            tal = 5 + tmp;
        } else if (tmp == 0xe) {
            tal = 3;
        } else if (tmp == 0xf) {
            tal = 4;
        } else {
            tal = 7;
        }
        tcmd = 3;

        tmp = (HYPERBUS_DEV_TCSM_1U * freqMhz + 999) / 1000;
        tmp = tmp - tcmd - 2 * tal - 4;

        if (tmp > maxLength) {
            tmp = maxLength;
        }
        MODIFY_REG(pReg->MCR[0], HYPERBUS_MCR0_MAXLEN_MASK,
                   tmp << HYPERBUS_MCR0_MAXLEN_SHIFT);
    }

    return HAL_OK;
}

static HAL_Status HYPERPSRAM_ModifyCR0(struct HAL_HYPERPSRAM_DEV *pHyperPsramDev)
{
    uint16_t cr0;
    struct HYPERBUS_REG *pReg = pHyperPsramDev->pReg;

    /* config to CR space */
    MODIFY_REG(pReg->MCR[0], HYPERBUS_MCR0_CRT_MASK,
               HYPERBUS_MCR0_CRT_CR_SPACE);
    cr0 = (*(volatile uint16_t *)(pHyperPsramDev->hyperMem[0] + HYPERBUS_CR0_ADDR_OFFSET));
    cr0 = (cr0 & (~CR0_INITIAL_LATENCY_MASK)) | CR0_INITIAL_LATENCY_6_CLK;
    cr0 = (cr0 & (~CR0_FIXED_LATENCY_ENABLE_MASK)) |
          CR0_FIXED_LATENCY_ENABLE_VARIABLE_LATENCY;
    cr0 = (cr0 & (~CR0_BURST_LENGTH_MASK)) | CR0_BURST_LENGTH_128BYTE;
    (*(volatile uint16_t *)(pHyperPsramDev->hyperMem[0] + HYPERBUS_CR0_ADDR_OFFSET)) = cr0;
    /* config to memory space */
    MODIFY_REG(pReg->MCR[0], HYPERBUS_MCR0_CRT_MASK,
               HYPERBUS_MCR0_CRT_MEM_SPACE);
    MODIFY_REG(pReg->MTR[0], HYPERBUS_MTR0_LTCY_MASK,
               HYPERBUS_MTR0_LTCY_6_CLK << HYPERBUS_MTR0_LTCY_SHIFT);

    return HAL_OK;
}

/**
 * @brief  Hyperbus init config.
 * @param  pReg: Choose HYPERBUS.
 * @param  psramBase: Choose psram map base addr.
 * @return HAL_Status.
 */
static HAL_Status HYPERBUS_Init(struct HYPERBUS_REG *pReg,
                                uint32_t hyperMem)
{
    WRITE_REG(pReg->MBR[0], hyperMem & HYPERBUS_MBR0_BASE_ADDR_MASK);
    WRITE_REG(pReg->RWDSIC, HYPERBUS_RWDSIC_RXEND_CTRL_MASK |
              HYPERBUS_RWDSIC_RXSTART_CTRL_MASK);
    WRITE_REG(pReg->LBR, HYPERBUS_LBR_DIS_LOOPBACK);
    WRITE_REG(pReg->MCR[0], HYPERBUS_MCR0_MAXEN_CONF_LOW |
              HYPERBUS_MCR0_MAXLEN_MAX | HYPERBUS_MCR0_CRT_MEM_SPACE |
              HYPERBUS_MCR0_DEVTYPE_HYPERRAM |
              HYPERBUS_MCR0_WRAPSIZE_32BYTE);

    return HAL_OK;
}

/**
 * @brief  Hyperbus psram cap detect.
 * @param  psramDev: psram device info.
 * @return HAL_Status.
 */
static HAL_Status HYPERPSRAM_CapDetect(struct HAL_HYPERPSRAM_DEV *pHyperPsramDev)
{
    uint32_t *p1 = (uint32_t *)pHyperPsramDev->hyperMem[0];
    uint32_t *p2;
    uint32_t cap = 0;

    for (cap = SIZE_128MBYTE; cap >= SIZE_1MBYTE; cap >>= 1) {
        p2 = (uint32_t *)((pHyperPsramDev->hyperMem[0] + cap - 0x4) & 0x3);
        *p1 = PATTERN1;
        *p2 = PATTERN2;
        if ((*p1 == PATTERN1) && (*p2 == PATTERN2)) {
            break;
        }
    }
    if (cap >= SIZE_1MBYTE) {
        pHyperPsramDev->psramChip.cap = cap;

        return HAL_OK;
    } else {
        return HAL_ERROR;
    }
}

/**
 * @brief  Hyperbus psram init.
 * @param  pReg: Choose HYPERBUS.
 * @param  psramDev: psram device info.
 * @return HAL_Status.
 */
static HAL_Status HYPERPSRAM_Init(struct HAL_HYPERPSRAM_DEV *pHyperPsramDev)
{
    uint32_t i;
    uint16_t detect_id;
    struct HYPERBUS_REG *pReg = pHyperPsramDev->pReg;
    struct HYPERPSRAM_CHIP_INFO *psramChip = &pHyperPsramDev->psramChip;

    for (i = 0; i < HAL_ARRAY_SIZE(psramInfo); i++) {
        WRITE_REG(pReg->SPCSR, psramInfo[i].spc);
        WRITE_REG(pReg->MTR[0], psramInfo[i].mtrTiming);
        detect_id = HYPERPSRAM_GetDevId(pReg, pHyperPsramDev->hyperMem[0]);
        if (detect_id == psramInfo[i].id) {
            break;
        }
    }

    psramChip->id = detect_id;
    HAL_DBG("HYPERBUS PSRAM id: %x\n", detect_id);
    if (i == HAL_ARRAY_SIZE(psramInfo)) {
        HAL_DBG("HYPERPSRAM: unknow psram device\n");

        return HAL_ERROR;
    } else {
        psramChip->spc = psramInfo[i].spc;
        HYPERPSRAM_CapDetect(pHyperPsramDev);

        return HAL_OK;
    }
}

static HAL_Status HYPERPSRAM_IdCheck(uint16_t id)
{
    uint32_t i;

    for (i = 0; i < HAL_ARRAY_SIZE(psramInfo); i++) {
        if (id == psramInfo[i].id) {
            return HAL_OK;
        }
    }

    return HAL_ERROR;
}

static HAL_Status HAL_HYPERPSRAM_SetDelayline(struct HAL_HYPERPSRAM_DEV *pHyperPsramDev)
{
    uint8_t delay_line;

    /*
     * The delay line range[0x0e, 0x00] in psram io clk 150MHz
     * [0x20, 0x00] in psram io clk 96MHz
     * failed in 85C,set to 0x3.
     */
    if (pHyperPsramDev->psramFreq >= 120000000) {
        delay_line = 0x0;
    } else {
        delay_line = 0x3;
    }

    WRITE_REG_MASK_WE(GRF->SOC_CON3, GRF_SOC_CON3_GRF_RPC_INITIAL_STATE_MASK |
                      GRF_SOC_CON3_GRF_RDS_CLK_SMP_SEL_MASK |
                      GRF_SOC_CON3_GRF_CON_RDS_DELAY_ADJ_MASK,
                      0x1 << GRF_SOC_CON3_GRF_RPC_INITIAL_STATE_SHIFT |
                      0x1 << GRF_SOC_CON3_GRF_RDS_CLK_SMP_SEL_SHIFT |
                      delay_line << GRF_SOC_CON3_GRF_CON_RDS_DELAY_ADJ_SHIFT);
    HAL_CPUDelayUs(1);
    WRITE_REG_MASK_WE(GRF->SOC_CON3, GRF_SOC_CON3_GRF_RPC_INITIAL_STATE_MASK,
                      0x0 << GRF_SOC_CON3_GRF_RPC_INITIAL_STATE_SHIFT);

    return HAL_OK;
}

/** @} */
/********************* Public Function Definition ****************************/

/** @defgroup HYPERPSRAM_Exported_Functions_Group4 Init and DeInit Functions

 This section provides functions allowing to init and deinit the module:

 *  @{
 */

/**
 * @brief  Hyperbus init.
 * @param  pHyperPsramDev: pointer to a HYPERPSRAM structure that contains
 *               the information for HYPERPSRAM module.
 * @return HAL_Status.
 */
HAL_Status HAL_HYPERPSRAM_Init(struct HAL_HYPERPSRAM_DEV *pHyperPsramDev)
{
    HAL_ASSERT(IS_HYPERBUS_INSTANCE(pHyperPsramDev->pReg));

    HYPERBUS_Init(pHyperPsramDev->pReg, pHyperPsramDev->hyperMem[0]);

    if (HYPERPSRAM_Init(pHyperPsramDev) == HAL_OK) {
        HYPERPSRAM_ModifyTiming(pHyperPsramDev);
        HYPERPSRAM_ModifyCR0(pHyperPsramDev);
        HAL_HYPERPSRAM_SetDelayline(pHyperPsramDev);
    } else {
        return HAL_ERROR;
    }

    return HAL_OK;
}

/**
 * @brief  Hyperbus reinit.
 * @param  pHyperPsramDev: pointer to a HYPERPSRAM structure that contains
 *               the information for HYPERPSRAM module.
 * @return HAL_Status
 */
HAL_Status HAL_HYPERPSRAM_DeInit(struct HAL_HYPERPSRAM_DEV *pHyperPsramDev)
{
    HAL_CRU_ClkDisable(pHyperPsramDev->aclkGateID);
    HAL_CRU_ClkDisable(pHyperPsramDev->sclkGateID);

    return HAL_OK;
}

/**
 * @brief  Hyperbus reinit.
 * @param  pHyperPsramDev: pointer to a HYPERPSRAM structure that contains
 *               the information for HYPERPSRAM module.
 * @return HAL_Status
 */
HAL_Status HAL_HYPERPSRAM_ReInit(struct HAL_HYPERPSRAM_DEV *pHyperPsramDev)
{
    HAL_ASSERT(IS_HYPERBUS_INSTANCE(pHyperPsramDev->pReg));

    HAL_CRU_ClkEnable(pHyperPsramDev->aclkGateID);
    HAL_CRU_ClkEnable(pHyperPsramDev->sclkGateID);
    pHyperPsramDev->psramChip.id = HYPERPSRAM_GetDevId(pHyperPsramDev->pReg,
                                                       pHyperPsramDev->hyperMem[0]);
    if (HYPERPSRAM_IdCheck(pHyperPsramDev->psramChip.id) == HAL_OK) {
        HAL_CRU_ClkSetFreq(pHyperPsramDev->clkID, pHyperPsramDev->hyperMaxFreq);
        pHyperPsramDev->psramFreq = HAL_CRU_ClkGetFreq(pHyperPsramDev->clkID) / 2;
        HYPERPSRAM_ModifyTiming(pHyperPsramDev);
        HAL_HYPERPSRAM_SetDelayline(pHyperPsramDev);

        return HAL_OK;
    }
    HAL_HYPERPSRAM_DeInit(pHyperPsramDev);

    return HAL_ERROR;
}

/**
 * @brief  Hyperbus psram modify transaction maximum length/tcsm.
 * @param  pHyperPsramDev: pointer to a HYPERPSRAM structure that contains
 *               the information for HYPERPSRAM module.
 * @return HAL_Status.
 */
HAL_Status HAL_HYPERPSRAM_ModifyTiming(struct HAL_HYPERPSRAM_DEV *pHyperPsramDev)
{
    HAL_ASSERT(IS_HYPERBUS_INSTANCE(pHyperPsramDev->pReg));

    return HYPERPSRAM_ModifyTiming(pHyperPsramDev);
}

/**
 * @brief  Hyperbus psram suspend.
 * @param  pHyperPsramDev: pointer to a HYPERPSRAM structure that contains
 *               the information for HYPERPSRAM module.
 * @return HAL_Status
 * save hyperbus register to sram
 */
HAL_Status HAL_HYPERPSRAM_Suspend(struct HAL_HYPERPSRAM_DEV *pHyperPsramDev)
{
    uint32_t i;
    volatile uint32_t *p1, *p2;

    HAL_ASSERT(IS_HYPERBUS_INSTANCE(pHyperPsramDev->pReg));

    p1 = (uint32_t *)&pHyperPsramDev->hyperResumeReg.hyperbus;
    p2 = (uint32_t *)pHyperPsramDev->pReg;
    for (i = 0; i < (sizeof(pHyperPsramDev->hyperResumeReg.hyperbus) / 4); i++) {
        *p1++ = *p2++;
    }

    pHyperPsramDev->hyperResumeReg.available = HYPERBUS_REG_AVA_FLAG;

    return HAL_OK;
}

/**
 * @brief  Hyperbus psram resume.
 * @param  pHyperPsramDev: pointer to a HYPERPSRAM structure that contains
 *               the information for HYPERPSRAM module.
 * @return HAL_Status
 * recovery hyperbus register
 */
HAL_Status HAL_HYPERPSRAM_Resume(struct HAL_HYPERPSRAM_DEV *pHyperPsramDev)
{
    uint32_t i;
    volatile uint32_t *p1, *p2;

    HAL_ASSERT(IS_HYPERBUS_INSTANCE(pHyperPsramDev->pReg));

    if (pHyperPsramDev->hyperResumeReg.available == HYPERBUS_REG_AVA_FLAG) {
        p1 = (uint32_t *)&pHyperPsramDev->hyperResumeReg.hyperbus;
        p2 = (uint32_t *)pHyperPsramDev->pReg;
        for (i = 0; i < (sizeof(pHyperPsramDev->hyperResumeReg.hyperbus) / 4); i++) {
            *p2++ = *p1++;
        }

        return HAL_OK;
    } else {
        return HAL_ERROR;
    }
}

/** @} */

/** @} */

/** @} */

#endif /* HAL_HYPERPSRAM_MODULE_ENABLED */
