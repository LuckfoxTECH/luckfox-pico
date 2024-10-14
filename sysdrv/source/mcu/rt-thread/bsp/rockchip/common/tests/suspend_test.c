#include <rtthread.h>
#include <rtdevice.h>

#ifdef RT_USING_COMMON_TEST_SUSPEND
#include "hal_base.h"

struct TIMER_REG *pTimer = TIMER0;
static uint32_t sleep_count = 0 ;

void timer0_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    pTimer->INTSTATUS = 0x01;
    rt_kprintf("suspend count: %d\n", sleep_count++);

    /* leave interrupt */
    rt_interrupt_leave();
}

static void timer0_wakeup_init(int argc, char **argv)
{
    int time_sec = 0;
    int sec_to_count = 0;
    if (argc >= 2)
    {
        time_sec = atoi(argv[1]);
    }

    sec_to_count = time_sec * PLL_INPUT_OSC_RATE;

    HAL_NVIC_SetIRQHandler(TIMER0_IRQn, (NVIC_IRQHandler) & timer0_IRQHandler);
    HAL_NVIC_EnableIRQ(TIMER0_IRQn);

    rt_kprintf("timer0 auto wake up:\n");
    pTimer->INTSTATUS = 0x01;
    pTimer->CONTROLREG = 0x00;
    pTimer->LOAD_COUNT[0] = sec_to_count;
    pTimer->LOAD_COUNT[1] = 0x0000;
    pTimer->CONTROLREG = 0x05;

    return;
}

/* Pisces EVB use the GPIO1_B1 to wakeup system */
#define GPIO_BANKx      (GPIO_BANK1)
#define PINNUM_INT  (9)
#define GPIOx  (GPIO1)
#define GPIO_PIN_INT  (GPIO_PIN_B1)

static void gpio_isr(void *args)
{
    /* enter interrupt */
    rt_interrupt_enter();

    if (HAL_GPIO_GetPinLevel(GPIOx, GPIO_PIN_INT) == GPIO_HIGH)
    {
        rt_kprintf("%s high 0\n", args);
        rt_pm_request(1);
    }
    else
    {
        rt_kprintf("%s low 1\n", args);
        rt_pm_release(1);
    }

    /* leave interrupt */
    rt_interrupt_leave();
}

void gpio_test_interrupt(void)
{
    rt_err_t ret;
    struct GPIO_REG *gpio = GPIO1;

    rt_kprintf("gpio_test_interruptgpio_test_interruptgpio_test_interrupt\n");

    HAL_PINCTRL_SetIOMUX(GPIO_BANKx, GPIO_PIN_INT, PIN_CONFIG_MUX_FUNC0);
    HAL_GPIO_SetPinDirection(GPIOx, GPIO_PIN_INT, GPIO_IN);
    ret = rt_pin_attach_irq(BANK_PIN(GPIO_BANKx, PINNUM_INT), PIN_IRQ_MODE_RISING_FALLING, gpio_isr, "gpio isr");
    RT_ASSERT(ret == RT_EOK);
    ret = rt_pin_irq_enable(BANK_PIN(GPIO_BANKx, PINNUM_INT),  PIN_IRQ_ENABLE);
    RT_ASSERT(ret == RT_EOK);

    gpio->INT_EN_L = 0x02000200;
}

void gpio_wake(int argc, char **argv)
{
    gpio_test_interrupt();
}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT_ALIAS(timer0_wakeup_init, timer_wakeup, timer wakeup cmd);
MSH_CMD_EXPORT(gpio_wake, gpio test cmd);
#endif

#endif
