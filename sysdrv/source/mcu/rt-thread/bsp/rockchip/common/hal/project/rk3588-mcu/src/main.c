/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2023 Rockchip Electronics Co., Ltd.
 */

#include "hal_bsp.h"
#include "hal_base.h"
#include "hdmi_cec.h"

/********************* Private MACRO Definition ******************************/
//#define TEST_DEMO
//#define TEST_USE_UART5M0

/********************* Private Structure Definition **************************/

#ifdef TEST_USE_UART5M0
static void HAL_IOMUX_Uart5m0Config(void)
{
    HAL_PINCTRL_SetIOMUX(GPIO_BANK4,
                         GPIO_PIN_D4,
                         PIN_CONFIG_MUX_FUNC10);
    HAL_PINCTRL_SetIOMUX(GPIO_BANK4,
                         GPIO_PIN_D5,
                         PIN_CONFIG_MUX_FUNC10);
}
#endif

/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/

/********************* Public Function Definition ****************************/
#ifdef TEST_USE_UART5M0
static struct UART_REG *pUart = UART5;
#else
static struct UART_REG *pUart = UART2;
#endif

#ifdef __GNUC__
int _write(int fd, char *ptr, int len)
{
    int i = 0;

    /*
     * write "len" of char from "ptr" to file id "fd"
     * Return number of char written.
     *
    * Only work for STDOUT, STDIN, and STDERR
     */
    if (fd > 2) {
        return -1;
    }

    while (*ptr && (i < len)) {
        if (*ptr == '\n') {
            HAL_UART_SerialOutChar(pUart, '\r');
        }
        HAL_UART_SerialOutChar(pUart, *ptr);

        i++;
        ptr++;
    }

    return i;
}
#else
int fputc(int ch, FILE *f)
{
    if (ch == '\n') {
        HAL_UART_SerialOutChar(pUart, '\r');
    }

    HAL_UART_SerialOutChar(pUart, (char)ch);

    return 0;
}
#endif

int main(void)
{
    struct HAL_UART_CONFIG hal_uart_config = {
        .baudRate = UART_BR_1500000,
        .dataBit = UART_DATA_8B,
        .stopBit = UART_ONE_STOPBIT,
        .parity = UART_PARITY_DISABLE,
    };

    /* HAL BASE Init */
    HAL_Init();

    /* BSP Init */
    BSP_Init();

    /* UART Init */
#ifdef TEST_USE_UART5M0
    HAL_IOMUX_Uart5m0Config();
    HAL_UART_Init(&g_uart5Dev, &hal_uart_config);
#else
    HAL_UART_Init(&g_uart2Dev, &hal_uart_config);
#endif

    printf("Hello RK3588 mcu\n");

    /* HDMI CEC Init */
#ifdef HDMI_CEC_WKUP
    HAL_CEC_Init();
#endif

#ifdef TEST_DEMO
    test_demo();
#endif

    while (1) {
        ;
    }
}

int entry(void)
{
    return main();
}
