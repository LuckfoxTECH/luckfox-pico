/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_INTC_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup INTC
 *  @{
 */

#ifndef _HAL_INTC_H_
#define _HAL_INTC_H_

#include "hal_base.h"
#include "hal_def.h"

/***************************** MACRO Definition ******************************/

#define L32                    32U
#define IS_VALID_IRQ(n)        ((uint32_t)(n) < 64)
#define NUM_INT_PER_CONTROLLER 64U

/***************************** Structure Definition **************************/

/********************* Public Function Definition ****************************/
/** @defgroup INTC_Exported_Functions_Group5 Other Functions
 *  @{
 */

/**
 * @brief  INTC Enable Interrupt.
 * @param  IRQn: Device specific interrupt number.
 * @return None.
 */
__STATIC_INLINE void HAL_INTC_EnableIRQ(struct INTC_REG *INTC, uint8_t IRQn)
{
    HAL_ASSERT(IS_VALID_IRQ(IRQn));

    if (IRQn < L32) {
        INTC->IRQ_INTEN_L |= (1UL << IRQn);
    } else {
        INTC->IRQ_INTEN_H |= (1UL << (IRQn - L32));
    }
}

/**
 * @brief  INTC Disable Interrupt.
 * @param  IRQn: Device specific interrupt number.
 * @return None.
 */
__STATIC_INLINE void HAL_INTC_DisableIRQ(struct INTC_REG *INTC, uint8_t IRQn)
{
    HAL_ASSERT(IS_VALID_IRQ(IRQn));

    if (IRQn < L32) {
        INTC->IRQ_INTEN_L &= ~(1UL << IRQn);
    } else {
        INTC->IRQ_INTEN_H &= ~(1UL << (IRQn - L32));
    }
}

/**
 * @brief  INTC Mask Interrupt.
 * @param  IRQn: Device specific interrupt number.
 * @return None.
 */
__STATIC_INLINE void HAL_INTC_MaskIRQ(struct INTC_REG *INTC, uint8_t IRQn)
{
    HAL_ASSERT(IS_VALID_IRQ(IRQn));

    if (IRQn < L32) {
        INTC->IRQ_INTMASK_L |= (1UL << IRQn);
    } else {
        INTC->IRQ_INTMASK_H |= (1UL << (IRQn - L32));
    }
}

/**
 * @brief  INTC Unmask Interrupt.
 * @param  IRQn: Device specific interrupt number.
 * @return None.
 */
__STATIC_INLINE void HAL_INTC_UnmaskIRQ(struct INTC_REG *INTC, uint8_t IRQn)
{
    HAL_ASSERT(IS_VALID_IRQ(IRQn));

    if (IRQn < L32) {
        INTC->IRQ_INTMASK_L &= ~(1UL << IRQn);
    } else {
        INTC->IRQ_INTMASK_H &= ~(1UL << (IRQn - L32));
    }
}

/**
 * @brief  INTC Set a Software Interrupt.
 * @param  IRQn: Device specific interrupt number.
 * @return None.
 */
__STATIC_INLINE void HAL_INTC_SetSoftwareIRQ(struct INTC_REG *INTC, uint8_t IRQn)
{
    HAL_ASSERT(IS_VALID_IRQ(IRQn));

    if (IRQn < L32) {
        INTC->IRQ_INTFORCE_L |= (1UL << IRQn);
        while (!(INTC->IRQ_INTFORCE_L & (1UL << IRQn))) {
            ;
        }
    } else {
        INTC->IRQ_INTFORCE_H |= (1UL << (IRQn - L32));
        while (!(INTC->IRQ_INTFORCE_H & (1UL << (IRQn - L32)))) {
            ;
        }
    }
}

/**
 * @brief  INTC Clear a Software Interrupt.
 * @param  IRQn: Device specific interrupt number.
 * @return None.
 */
__STATIC_INLINE void HAL_INTC_ClearSoftwareIRQ(struct INTC_REG *INTC, uint8_t IRQn)
{
    HAL_ASSERT(IS_VALID_IRQ(IRQn));

    if (IRQn < L32) {
        INTC->IRQ_INTFORCE_L &= ~(1UL << IRQn);
        while (INTC->IRQ_INTFORCE_L & (1UL << IRQn)) {
            ;
        }
    } else {
        INTC->IRQ_INTFORCE_H &= ~(1UL << (IRQn - L32));
        while (INTC->IRQ_INTFORCE_H & (1UL << (IRQn - L32))) {
            ;
        }
    }
}

/**
 * @brief  INTC Get Interrupt Status.
 * @param  IRQn: Device specific interrupt number.
 * @return uint32_t: If is active return 1, or 0.
 */
__STATIC_INLINE uint32_t HAL_INTC_GetStatus(struct INTC_REG *INTC, uint8_t IRQn)
{
    HAL_ASSERT(IS_VALID_IRQ(IRQn));

    if (IRQn < L32) {
        return (INTC->IRQ_STATUS_L & (1UL << IRQn)) ? 1UL : 0UL;
    }

    return (INTC->IRQ_STATUS_H & (1UL << (IRQn - L32))) ? 1UL : 0UL;
}

/**
 * @brief  INTC Get Interrupt Raw Status.
 * @param  IRQn: Device specific interrupt number.
 * @return uint32_t: If is active return 1, or 0.
 */
__STATIC_INLINE uint32_t HAL_INTC_GetRawStatus(struct INTC_REG *INTC, uint8_t IRQn)
{
    HAL_ASSERT(IS_VALID_IRQ(IRQn));

    if (IRQn < L32) {
        return (INTC->IRQ_RAWSTATUS_L & (1UL << IRQn)) ? 1UL : 0UL;
    }

    return (INTC->IRQ_RAWSTATUS_H & (1UL << (IRQn - L32))) ? 1UL : 0UL;
}

/**
 * @brief  INTC Get Interrupt Mask Status.
 * @param  IRQn: Device specific interrupt number.
 * @return uint32_t: If is active return 1, or 0.
 */
__STATIC_INLINE uint32_t HAL_INTC_GetMaskStatus(struct INTC_REG *INTC, uint8_t IRQn)
{
    HAL_ASSERT(IS_VALID_IRQ(IRQn));

    if (IRQn < L32) {
        return (INTC->IRQ_MASKSTATUS_L & (1UL << IRQn)) ? 1UL : 0UL;
    }

    return (INTC->IRQ_MASKSTATUS_H & (1UL << (IRQn - L32))) ? 1UL : 0UL;
}

/**
 * @brief  INTC Get Interrupt Final Status.
 * @param  IRQn: Device specific interrupt number.
 * @return uint32_t: If is active return 1, or 0.
 */
__STATIC_INLINE uint32_t HAL_INTC_GetFinalStatus(struct INTC_REG *INTC, uint8_t IRQn)
{
    HAL_ASSERT(IS_VALID_IRQ(IRQn));

    if (IRQn < L32) {
        return (INTC->IRQ_FINALSTATUS_L & (1UL << IRQn)) ? 1UL : 0UL;
    }

    return (INTC->IRQ_FINALSTATUS_H & (1UL << (IRQn - L32))) ? 1UL : 0UL;
}

/**
 * @brief  INTC Set System Priority Level.
 * @param  plevel: System specific priority level.
 * @return None.
 */
__STATIC_INLINE void HAL_INTC_SetPriorityLevel(struct INTC_REG *INTC, uint8_t plevel)
{
    if (plevel >= 0 && plevel <= 0x0f) {
        INTC->IRQ_PLEVEL = plevel & 0x0fUL;
    }
}

/**
 * @brief  INTC Set Interrupt Priority.
 * @param  IRQn: Device specific interrupt number.
 * @param  priority: Priority to set.
 * @return None.
 */
__STATIC_INLINE void HAL_INTC_SetPriority(struct INTC_REG *INTC, uint8_t IRQn, uint8_t priority)
{
    HAL_ASSERT(IS_VALID_IRQ(IRQn));

    if (priority >= 0 && priority <= 0x0f) {
        INTC->IRQ_PR_OFFSET[IRQn] = priority & 0x0fUL;
    }
}

/**
 * @brief  INTC Enable All Interrupt.
 * @return None.
 */
__STATIC_INLINE void HAL_INTC_EnableAllRQ(struct INTC_REG *INTC)
{
    INTC->IRQ_INTEN_L = 0xffffffff;
    INTC->IRQ_INTEN_H = 0xffffffff;
}

/**
 * @brief  INTC Disable All Interrupt.
 * @return None.
 */
__STATIC_INLINE void HAL_INTC_DisableAlIRQ(struct INTC_REG *INTC)
{
    INTC->IRQ_INTEN_L = 0x00000000;
    INTC->IRQ_INTEN_H = 0x00000000;
}

/**
 * @brief  INTC Mask All Interrupt.
 * @return None.
 */
__STATIC_INLINE void HAL_INTC_MaskAllRQ(struct INTC_REG *INTC)
{
    INTC->IRQ_INTMASK_L = 0xffffffff;
    INTC->IRQ_INTMASK_H = 0xffffffff;
}

/**
 * @brief  INTC Unmask All Interrupt.
 * @return None.
 */
__STATIC_INLINE void HAL_INTC_UnmaskAllRQ(struct INTC_REG *INTC)
{
    INTC->IRQ_INTMASK_L = 0x00000000;
    INTC->IRQ_INTMASK_H = 0x00000000;
}

/** @} */

#endif

/** @} */

/** @} */

#endif /* HAL_INTC_MODULE_ENABLED */
