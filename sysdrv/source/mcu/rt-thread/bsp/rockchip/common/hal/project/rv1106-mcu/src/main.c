/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */

#include "hal_bsp.h"
#include "hal_base.h"

/********************* Private MACRO Definition ******************************/
//#define GPIO_TEST
//#define MBOX_TEST

/********************* Private Structure Definition **************************/

/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/

/********************* Public Function Definition ****************************/
static struct UART_REG *pUart = UART2;

static void HAL_IOMUX_Uart0M1Config(void)
{
    /* UART0 M1 RX-2B0 TX-2B1 */
    HAL_PINCTRL_SetIOMUX(GPIO_BANK2,
                         GPIO_PIN_B0 |
                         GPIO_PIN_B1,
                         PIN_CONFIG_MUX_FUNC1);
}

static void HAL_IOMUX_Uart2M0Config(void)
{
    /* UART2 M0 RX-3A3 TX-3A2 */
    HAL_PINCTRL_SetIOMUX(GPIO_BANK3,
                         GPIO_PIN_A2 |
                         GPIO_PIN_A3,
                         PIN_CONFIG_MUX_FUNC2);
}

#ifdef __GNUC__
__USED int _write(int fd, char *ptr, int len)
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

#ifdef GPIO_TEST
static void gpio_isr(int vector, void *param)
{
    printf("Enter GPIO IRQHander!\n");
    HAL_GPIO_IRQHandler(GPIO2, GPIO_BANK2);
    printf("Leave GPIO IRQHandler!\n");
}

static HAL_Status b0_call_back(eGPIO_bankId bank, uint32_t pin, void *args)
{
    printf("GPIO callback!\n");

    return HAL_OK;
}

static void gpio_test(void)
{
    uint32_t level;

    /* Test GPIO output */
    HAL_GPIO_SetPinDirection(GPIO2, GPIO_PIN_B0, GPIO_OUT);
    level = HAL_GPIO_GetPinLevel(GPIO2, GPIO_PIN_B0);
    printf("test_gpio level = %ld\n", level);
    HAL_DelayMs(5000);
    if (level == GPIO_HIGH) {
        HAL_GPIO_SetPinLevel(GPIO2, GPIO_PIN_B0, GPIO_LOW);
    } else {
        HAL_GPIO_SetPinLevel(GPIO2, GPIO_PIN_B0, GPIO_HIGH);
    }
    level = HAL_GPIO_GetPinLevel(GPIO2, GPIO_PIN_B0);
    printf("test_gpio level = %ld\n", level);
    HAL_DelayMs(5000);

    /* Test GPIO input */
    printf("test_gpio interrupt\n");
    HAL_GPIO_SetPinDirection(GPIO2, GPIO_PIN_B0, GPIO_IN);
    HAL_IRQ_HANDLER_SetIRQHandler(GPIO2_IRQn, gpio_isr, NULL);
    HAL_IRQ_HANDLER_SetGpioIRQHandler(GPIO_BANK2, GPIO_PIN_B0, b0_call_back, NULL);
    HAL_RISCVIC_EnableIRQ(GPIO2_IRQn);
    HAL_GPIO_SetIntType(GPIO2, GPIO_PIN_B0, GPIO_INT_TYPE_EDGE_RISING);
    HAL_GPIO_EnableIRQ(GPIO2, GPIO_PIN_B0);
}
#endif

#ifdef MBOX_TEST
#define MBOX_B2A 0
#define MBOX_A2B 1

static struct MBOX_REG *pMBox = MBOX;

static void mbox_isr(int vector, void *param)
{
    printf("Enter MBOX IRQHander!\n");
    HAL_MBOX_IrqHandler(MAILBOX0_BB_IRQn, pMBox);
    printf("Leave MBOX IRQHandler!\n");
}

static void mbox_rx_callback(struct MBOX_CMD_DAT *msg, void *args)
{
    struct MBOX_CMD_DAT rx_msg = *msg;

    printf("MBOX callback!\n");
    printf("test_mbox receive cmd=0x%lx data=0x%lx\n", rx_msg.CMD, rx_msg.DATA);
}

static void mbox_test(void)
{
    struct MBOX_CMD_DAT tx_msg;
    uint32_t tx_chan = MBOX_CH_1;
    uint32_t rx_chan = MBOX_CH_0;
    struct MBOX_CLIENT mbox_cl = { "mbox-test", MAILBOX0_BB_IRQn, mbox_rx_callback, (void *)rx_chan };
    int ret = 0;

    tx_msg.CMD = 0x12345678;
    tx_msg.DATA = 0x87654321;

    HAL_MBOX_Init(pMBox, MBOX_B2A);
    ret = HAL_MBOX_RegisterClient(pMBox, tx_chan, &mbox_cl);
    if (ret) {
        printf("mbox tx_chan register failed, ret=%d\n", ret);
    }
    ret = HAL_MBOX_RegisterClient(pMBox, rx_chan, &mbox_cl);
    if (ret) {
        printf("mbox rx_chan register failed, ret=%d\n", ret);
    }
    HAL_IRQ_HANDLER_SetIRQHandler(MAILBOX0_BB_IRQn, mbox_isr, NULL);
    HAL_RISCVIC_EnableIRQ(MAILBOX0_BB_IRQn);

    HAL_DelayMs(1000);
    printf("test_mbox send cmd=0x%lx data=0x%lx\n", tx_msg.CMD, tx_msg.DATA);
    HAL_MBOX_SendMsg2(pMBox, tx_chan, &tx_msg, MBOX_B2A);
}
#endif

void main(void)
{
    struct HAL_UART_CONFIG hal_uart_config = {
        .baudRate = UART_BR_115200,
        .dataBit = UART_DATA_8B,
        .stopBit = UART_ONE_STOPBIT,
        .parity = UART_PARITY_DISABLE,
    };

    /* HAL BASE Init */
    HAL_Init();

    /* BSP Init */
    BSP_Init();

    /* UART Init */
    HAL_IOMUX_Uart2M0Config();
    HAL_UART_Init(&g_uart2Dev, &hal_uart_config);

    printf("Hello RV1106 RISC-V\n");

#ifdef GPIO_TEST
    gpio_test();
#endif

#ifdef MBOX_TEST
    mbox_test();
#endif

    while (1) {
        ;
    }
}

void Main(void)
{
    main();
}
