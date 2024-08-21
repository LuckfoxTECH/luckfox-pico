/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#if defined(HAL_GPIO_MODULE_ENABLED) && defined(HAL_GPIO_IRQ_GROUP_MODULE_ENABLED)

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup GPIO_IRQ_GROUP
 *  @{
 */

#ifndef __HAL_GPIO_IRQ_GROUP_H
#define __HAL_GPIO_IRQ_GROUP_H

/***************************** MACRO Definition ******************************/
/** @defgroup GPIO_IRQ_GROUP_Exported_Definition_Group1 Basic Definition
 *  @{
 */
#define GROUP_PRIO_LEVEL_MAX HAL_GPIO_IRQ_GROUP_PRIO_LEVEL_MAX

#if (GPIO_VER_ID != 0x01000C2BU)
 #warning : Not support this gpio vervion
#endif

/** define the type of IRQ_GROUP
  * BANK_TYPE: all of pins irq in a bank are dispatched to the same the cpu with the same irq priority.
  * GROUP_TYPE: any pin irq in a bank can be dispatched to any cpu with different irq priorities.
  */
typedef enum {
    GPIO_IRQ_GROUP_BANK_TYPE  = 0x1,
    GPIO_IRQ_GROUP_GROUP_TYPE = 0x2,
} eGPIO_IRQ_GROUP_Type;

/** enable control for IRQ_GROUP */
typedef enum {
    GPIO_IRQ_GROUP_DISABLE       = 0x0, /**< disable irq function */
    GPIO_IRQ_GROUP_EN_BANK_TYPE  = 0x1, /**< enabe irq function with BANK_TYPE */
    GPIO_IRQ_GROUP_EN_GROUP_TYPE = 0x2, /**< enable irq function with GROUP_TYPE */
} eGPIO_IRQ_GROUP_IrqEnable;

#define GPIO_IRQ_GROUP_GPIO_CFG(__bank, __irq, __reg) \
    .bank = __bank,                                   \
    .hwIrq = __irq,                                   \
    .gpioReg = (__reg)

/***************************** Structure Definition **************************/
/** information about a bank type of gpio irq.
  * for this type, all of gpios in a bank are dispatched by the hwirq of the bank */
struct GPIO_IRQ_GROUP_BANK_CFG {
    uint32_t hwIrqCpuAff; /**< the routting affinity for a gpio hwirq,
                            * from CPU_GET_AFFINITY() */
    uint32_t prio; /**< the priority for a gpio hwirq, as fallows:
                     * 0x80, 0x90, 0xa0, 0xb0, 0xc0, 0xd0, 0xe0 */
};

/** allocate a group of PINs from a GPIO BANK with the same priority.
  * the irq priority for this group of PINs Configured with the prio of this struct.
  * the IRQ of this group of PINs can dispatch Different cpus according to
  * cpuGroup[PLATFORM_CORE_COUNT] of this struct.
  * if cpuGroup[0] = GPIO_PIN_A1 | GPIO_PIN_B1; it means the IRQ of
  * GPIO_PIN_A1 and GPIO_PIN_B1 is dispatched by GIRQId[0] of this struct,
  * and the GIRQId[0] will be dispatched to cpu0. */
struct GPIO_IRQ_GROUP_PRIO_GROUP {
    uint32_t cpuGroup[PLATFORM_CORE_COUNT]; /**< mask bits for groups of PINs */
    IRQn_Type GIRQId[PLATFORM_CORE_COUNT]; /**< a GIRQ is a GPIO HWIRQ or DIRQ
                                            * to dispatching IRQ for a group
                                            * of PINs assigned in cpuGroup[] */
    uint32_t prio; /**< irq priority of GIRQId[] of this struct  */
};

/** information about groups of PINs */
struct GPIO_IRQ_GROUP_CFG {
    eGPIO_IRQ_GROUP_IrqEnable groupIrqEn; /**< enable a type of group irq to
                                            * dispatch pins irq */
    struct GPIO_REG *gpioReg; /**< the base addr */
    IRQn_Type hwIrq; /**< the hw irq of this bank */
    eGPIO_bankId bank; /**< the bank index of this gpio bank */
    struct GPIO_IRQ_GROUP_BANK_CFG bankTypeCfg; /**< the bank type of gpio irq
                                                  * config */
    struct GPIO_IRQ_GROUP_PRIO_GROUP prioGroup[GROUP_PRIO_LEVEL_MAX];
    /**< a group of PINs with the same priority */
};

/** DIRQ information, DIRQ is reserved IRQ for dipatching PIN IRQs */
struct GPIO_IRQ_GROUP_DIRQ_CTRL {
    uint32_t enable; /**< enable the DIRQ to dispatch pins irq */
    uint32_t gpioBank; /**< the gpio bank this DIRQ belong to */
    uint32_t cpu; /**< the cpu this DIRQ belong to */
    uint32_t prioLevel; /**< the prioLevel this DIRQ belong to,
                          * 0~GROUP_PRIO_LEVEL_MAX */
    IRQn_Type irq; /**< this DIRQ irq id */
    struct GPIO_IRQ_GROUP_CFG const *gpioIrqCfg;/**< GPIO_IRQ_GROUP_CFG
                                                  * information */
};

/** operation callback for irq operation */
struct GPIO_IRQ_GROUP_OPS {
    HAL_Status (*setIrqPriority)(uint32_t irq, uint32_t prio); /**< set irq  priority value */
    HAL_Status (*enableIrq)(uint32_t irq); /**< enalbe a irq */
    uint32_t (*getIrqStatus)(uint32_t irq); /**< get a irq status: active and pending status */
    HAL_Status (*setIrqPending)(uint32_t irq); /**< set a irq pending status */
    HAL_Status (*setIrqHandler)(uint32_t irq,
                                eGPIO_IRQ_GROUP_Type groupIrqType,
                                bool isHwIrq, void *args);
    /**< set irq handlers for given irq */
};

/** @} */
/***************************** Function Declare ******************************/
/** @defgroup GPIO_IRQ_GROUP_Public_Function_Declare Public Function Declare
 *  @{
 */
HAL_Status HAL_GPIO_IRQ_GROUP_Init(uint32_t cfgCpuAff,
                                   struct GPIO_IRQ_GROUP_CFG const *cfg,
                                   struct GPIO_IRQ_GROUP_OPS *ops);
HAL_Status HAL_GPIO_IRQ_GROUP_GpioCtrlExit(struct GPIO_REG *pGPIO,
                                           uint32_t pin, uint32_t irqStatus);
int HAL_GPIO_IRQ_GROUP_GpioCtrlEnter(struct GPIO_REG *pGPIO,
                                     uint32_t pin, uint32_t *irqStatus);
HAL_Status HAL_GPIO_IRQ_GROUP_DIRQDispatchIrqs(uint32_t irq, void *args);
HAL_Status HAL_GPIO_IRQ_GROUP_HWIRQDispatchIrqs(uint32_t irq, void *args);
HAL_Status HAL_GPIO_IRQ_GROUP_DispatchGIRQs(uint32_t irq);

/** @} */

#endif

/** @} */

/** @} */

#endif
