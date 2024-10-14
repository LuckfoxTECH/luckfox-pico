/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020 Rockchip Electronics Co., Ltd.
 */

#include "hal_bsp.h"
#include "hal_base.h"
#include "unity_runner.h"

/********************* Private MACRO Definition ******************************/

/********************* Private Structure Definition **************************/

/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/

/********************* Public Function Definition ****************************/
static struct UART_REG *pUart = UART2;

int __wrap_puts(const char *s)
{
    while (*s != '\0') {
        if (*s == '\n') {
            HAL_UART_SerialOutChar(pUart, '\r');
        }

        HAL_UART_SerialOutChar(pUart, *s);
        ++s;
    }

    return 0;
}

int main(void)
{
    /* Uart is initialized by pre-loader,
     * use it to print debug info directly.
     */
    printf("Hello RISC-V\n");

    return 0;
}

int Main(void)
{
    return main();
}
