/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_IRQ_HANDLER_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup IRQ_HANDLER
 *  @{
 */

/** @defgroup IRQ_HANDLER_How_To_Use How To Use
 *  @{

 The IRQ handler implement can be used as follows:
 - Invoke HAL_IRQ_HANDLER_GetGpioIrqGroupOps() to get a defualt ops for gpio_irq_group.
 - Invoke HAL_IRQ_HANDLER_IRQHandler() to dispatch handler for a irq.
 - Invoke HAL_IRQ_HANDLER_SetIRQHandler() to set handler for a irq.
 - invoke HAL_IRQ_HANDLER_SetGpioIRQHandler() set the gpio irq handler callback for a gpio pin.

 @} */

/** @defgroup IRQ_HANDLER_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/

/********************* Private Structure Definition **************************/
struct HAL_IRQ_HANDLER_CTRL {
    HAL_IRQ_HANDLER handler;
    void *args;
};

#ifdef HAL_GPIO_MODULE_ENABLED
struct HAL_GPIO_IRQ_HANDLER_CTRL {
    HAL_IRQ_GPIO_HANDLER handler;
    void *args;
};
#endif
/********************* Private Variable Definition ***************************/
static struct HAL_IRQ_HANDLER_CTRL s_irqsHandler[NUM_INTERRUPTS];
#ifdef HAL_GPIO_MODULE_ENABLED
static struct HAL_GPIO_IRQ_HANDLER_CTRL s_gpioHandler[GPIO_BANK_NUM][PIN_NUMBER_PER_BANK];
#endif

#ifdef HAL_GPIO_IRQ_GROUP_MODULE_ENABLED
static HAL_Status GPIO_IRQ_GROUP_SetGIRQHandler(uint32_t irq,
                                                eGPIO_IRQ_GROUP_Type irqType,
                                                bool isHwIrq,
                                                void *args);

static struct GPIO_IRQ_GROUP_OPS defualtGicOps = {
    .setIrqPriority = HAL_GIC_SetPriority,
    .enableIrq = HAL_GIC_Enable,
    .getIrqStatus = HAL_GIC_GetIRQStatus,
    .setIrqPending = HAL_GIC_SetPending,
    .setIrqHandler = GPIO_IRQ_GROUP_SetGIRQHandler,
};
#endif
/********************* Private Function Definition ***************************/
#ifdef HAL_GPIO_IRQ_GROUP_MODULE_ENABLED
static HAL_Status GPIO_IRQ_GROUP_HWIRQDispatchBankIrqs(uint32_t irq, void *args)
{
    struct GPIO_IRQ_GROUP_CFG const *gpioIrqCfg = args;

    HAL_GPIO_IRQHandler(gpioIrqCfg->gpioReg, gpioIrqCfg->bank);

    return HAL_OK;
}

static HAL_Status GPIO_IRQ_GROUP_SetGIRQHandler(uint32_t irq,
                                                eGPIO_IRQ_GROUP_Type irqType,
                                                bool isHwIrq,
                                                void *args)
{
    HAL_Status ret = HAL_INVAL;

    if (irqType == GPIO_IRQ_GROUP_GROUP_TYPE) {
        if (isHwIrq) {
            ret = HAL_IRQ_HANDLER_SetIRQHandler(irq, HAL_GPIO_IRQ_GROUP_HWIRQDispatchIrqs, args);
        } else {
            ret = HAL_IRQ_HANDLER_SetIRQHandler(irq, HAL_GPIO_IRQ_GROUP_DIRQDispatchIrqs, args);
        }
    } else if (irqType == GPIO_IRQ_GROUP_BANK_TYPE) {
        ret = HAL_IRQ_HANDLER_SetIRQHandler(irq, GPIO_IRQ_GROUP_HWIRQDispatchBankIrqs, args);
    }

    return ret;
}
#endif
/** @} */

/********************* Public Function Definition ****************************/

/** @defgroup IRQ_HANDLER_Exported_Functions_Group5 Other Functions
 *  @{
 */

/**
 * @brief  get a defualt ops for  GPIO_IRQ_GROUP
 * @return Pointer to the struct GPIO_IRQ_GROUP_OPS
 */
struct GPIO_IRQ_GROUP_OPS *HAL_IRQ_HANDLER_GetGpioIrqGroupOps(void)
{
#ifdef HAL_GPIO_IRQ_GROUP_MODULE_ENABLED

    return &defualtGicOps;
#else

    return NULL;
#endif
}

/**
 * @brief  Dispatch handler for a irq
 * @param  irq: irq id.
 * @return HAL_Status.
 */
HAL_Status HAL_IRQ_HANDLER_IRQHandler(uint32_t irq)
{
    if (irq >= NUM_INTERRUPTS) {
        return HAL_INVAL;
    }

    if (s_irqsHandler[irq].handler) {
        s_irqsHandler[irq].handler(irq, s_irqsHandler[irq].args);
    }

    return HAL_OK;
}

/**
 * @brief  set handler for a irq
 * @param  irq: irq id.
 * @param  handler: handler callback
 * @param  args: private parameters
 * @return HAL_Status.
 */
HAL_Status HAL_IRQ_HANDLER_SetIRQHandler(uint32_t irq, HAL_IRQ_HANDLER handler, void *args)
{
    HAL_ASSERT(irq < NUM_INTERRUPTS);
    HAL_ASSERT(handler);

    s_irqsHandler[irq].handler = handler;
    s_irqsHandler[irq].args = args;

    return HAL_OK;
}

#ifdef HAL_GPIO_MODULE_ENABLED
void HAL_GPIO_IRQDispatch(eGPIO_bankId bank, uint32_t pin)
{
    HAL_ASSERT(bank < GPIO_BANK_NUM);
    HAL_ASSERT(pin < PIN_NUMBER_PER_BANK);

    if (s_gpioHandler[bank][pin].handler) {
        s_gpioHandler[bank][pin].handler(bank, pin, s_gpioHandler[bank][pin].args);
    }
}

/**
 * @brief  Set the gpio irq handler callback for a gpio pin.
 * @param  bank: gpio bank id
 * @param  pin: The pin bit defined in @ref ePINCTRL_GPIO_PINS.
 * @param  handler: irq handler callback.
 * @param  args: private parameters
 * @return HAL_Status.
 */
HAL_Status HAL_IRQ_HANDLER_SetGpioIRQHandler(eGPIO_bankId bank,
                                             ePINCTRL_GPIO_PINS pin,
                                             HAL_IRQ_GPIO_HANDLER handler,
                                             void *args)
{
    uint32_t pinId;

    HAL_ASSERT(bank < GPIO_BANK_NUM);
    HAL_ASSERT(handler);

    pinId = 31U - __CLZ(pin);
    HAL_ASSERT(pinId < PIN_NUMBER_PER_BANK);

    s_gpioHandler[bank][pinId].handler = handler;
    s_gpioHandler[bank][pinId].args = args;

    return HAL_OK;
}
#endif /* HAL_GPIO_MODULE_ENABLED */

/** @} */

/** @} */

/** @} */
#endif
