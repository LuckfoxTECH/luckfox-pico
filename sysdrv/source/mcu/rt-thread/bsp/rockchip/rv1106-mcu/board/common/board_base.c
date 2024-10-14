/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-01-11     Jason Zhu    first implementation
 * 2022-07-08     Steven Liu   Add rt_hw_iomux_config
*/

#include <rthw.h>
#include <rtthread.h>

#include "board.h"
#include "iomux.h"
#include "hal_base.h"
#include "hal_bsp.h"
#include "timer.h"
#include "board_base.h"

#ifdef RT_USING_UART
#include "drv_uart.h"
#endif

#ifdef RT_USING_I2C
#include "drv_i2c.h"
#endif

extern RT_UNUSED uint32_t __DATA_START__[];
extern RT_UNUSED uint32_t _tdata_end[];
extern RT_UNUSED uint32_t __SAVE_DATA_START__[];

SECTION(".data") static int data_fixup_flag = 0;

void data_section_fixup(void)
{
    uint32_t *save = __DATA_START__;
    uint32_t *restore = __SAVE_DATA_START__;
    int size, i;

    size = _tdata_end - __DATA_START__;

    if (data_fixup_flag == 0)
    {
        data_fixup_flag++;
        for (i = 0; i < size; i++)
        {
            restore[i] = save[i];
        }
    }
    else
    {
        for (i = 0; i < size; i++)
        {
            save[i] = restore[i];
        }
    }
}

#if defined(RT_USING_UART0)
RT_WEAK const struct uart_board g_uart0_board =
{
    .baud_rate = UART_BR_1500000,
    .dev_flag = ROCKCHIP_UART_SUPPORT_FLAG_DEFAULT,
    .bufer_size = RT_SERIAL_RB_BUFSZ,
    .name = "uart0",
};
#endif /* RT_USING_UART0 */

#if defined(RT_USING_UART2)
RT_WEAK const struct uart_board g_uart2_board =
{
    .baud_rate = UART_BR_1500000,
    .dev_flag = ROCKCHIP_UART_SUPPORT_FLAG_DEFAULT,
    .bufer_size = RT_SERIAL_RB_BUFSZ,
    .name = "uart2",
};
#endif /* RT_USING_UART2 */

#ifdef RT_USING_I2C
RT_WEAK const struct rockchip_i2c_config rockchip_i2c_config_table[] =
{
    {
        .id = I2C4,
        .speed = I2C_400K,
    },
    { /* sentinel */ }
};
#endif

RT_WEAK void cif_hw_config(void)
{
}

static rt_bool_t sirq = RT_FALSE;
static void board_softirq_handler(int vector, void *param)
{
    COREGRF->MCU_CACHE_MISC = 0x00080000;
    sirq = RT_TRUE;
}

/**
 * @brief  return soft irq status
 * RT_TRUE means soft irq was triggered, otherwise not.
 */
rt_bool_t sirq_status(void)
{
    return sirq;
}

void rt_hw_board_init(void)
{
    rt_system_heap_init((void *)HEAP_BEGIN, (void *)HEAP_END);
    rt_hw_interrupt_init();
    rt_soft_interrupt_install(board_softirq_handler, RT_NULL, "soft irq");
    sysTick_config(SCR1_CORE_FREQUECY / RT_TICK_PER_SECOND);
    /* initialize uart */
#ifdef RT_USING_UART
    rt_hw_usart_init();
#endif

#ifdef RT_USING_CONSOLE
#ifndef RT_USING_RK_AOV
    rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif
#endif

#ifdef RT_USING_PIN
    rt_hw_iomux_config();
#endif

#ifdef RT_USING_ISP3
    cif_hw_config();
#endif

#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

#ifdef RT_USING_RK_AOV
    rt_hw_interrupt_mask(GPIO0_IRQn);
#endif
}
