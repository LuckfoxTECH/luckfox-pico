/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_GIC_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup GIC
 *  @{
 */

#ifndef HAL_GIC_H_
#define HAL_GIC_H_

/***************************** MACRO Definition ******************************/
/** @defgroup GIC_Exported_Definition_Group1 Basic Definition
 *  @{
 */
#define HAL_GIC_IRQ_LINE_COUNT     (1020U)
#define GICR_WAKER_PROCESSOR_SLEEP (1U << 1)
#define GICR_WAKER_CHILDREN_ASLEEP (1U << 2)

typedef void (*GIC_IRQHandler) (uint32_t irq);

#define GIC_AMP_IRQ_CFG(_irq, _prio) \
  {                                  \
    .irq = (_irq),                   \
    .prio = (_prio),                 \
  }

#define GIC_AMP_IRQ_CFG_ROUTE(_irq, _prio, aff) \
  {                                             \
    .irq = (_irq),                              \
    .prio = (_prio),                            \
    .routeAff = (aff),                          \
  }
/***************************** Structure Definition **************************/
/** config information for a irq */
struct GIC_AMP_IRQ_INIT_CFG {
    IRQn_Type irq; /**< irq id */
    uint32_t prio; /**< irq priority, as fallows:
                     * 0x80, 0x90, 0xa0, 0xb0, 0xc0, 0xd0, 0xe0 */
    uint32_t routeAff; /**< routting affinity */
};

/** For the AMP system, need to specify a cpu to initialize
  * the Common configurations for the gic.
  */
struct GIC_IRQ_AMP_CTRL {
    uint32_t cpuAff; /**< the affinity of the cpu specified to init gic */
    uint32_t defPrio; /**< the default priority for all the irqs */
    uint32_t defRouteAff; /**< the default routting affinity for all the irqs */
    struct GIC_AMP_IRQ_INIT_CFG *irqsCfg; /**< some irqs need to be initialized
                                            * with their config instead of default */
};

/** @} */
/***************************** Function Declare ******************************/
/** @defgroup GIC_Public_Function_Declare Public Function Declare
 *  @{
 */
HAL_Status HAL_GIC_Enable(uint32_t irq);
HAL_Status HAL_GIC_Disable(uint32_t irq);
uint32_t HAL_GIC_GetEnableState(uint32_t irq);
uint32_t HAL_GIC_GetActiveIRQ(void);
HAL_Status HAL_GIC_EndOfInterrupt(uint32_t irq);
HAL_Status HAL_GIC_SetPending(uint32_t irq);
int32_t HAL_GIC_GetPending(uint32_t irq);
HAL_Status HAL_GIC_ClearPending(uint32_t irq);
uint32_t HAL_GIC_GetIRQStatus(uint32_t irq);
HAL_Status HAL_GIC_SetPriority(uint32_t irq, uint32_t priority);
HAL_Status HAL_GIC_SetPriorityMask(uint32_t priority);
uint32_t HAL_GIC_GetPriorityMask(void);
uint32_t HAL_GIC_GetPriority(uint32_t irq);
HAL_Status HAL_GIC_SetIRouter(uint32_t irq, uint32_t aff);
HAL_Status HAL_GIC_Init(struct GIC_IRQ_AMP_CTRL *ampCtrl);
HAL_Status HAL_GIC_SendSGI(IRQn_Type irq, uint32_t targetList, uint32_t routMode);
HAL_Status HAL_GIC_SetDir(uint32_t irq);

/** @} */

#endif

/** @} */

/** @} */

#endif
