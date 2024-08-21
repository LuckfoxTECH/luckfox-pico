/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"
#include "hdmi_cec.h"

#ifdef HDMI_CEC_WKUP

struct GPIO_REG *gpio0_base = GPIO0;
struct GPIO_REG *gpio3_base = GPIO3;
struct GPIO_REG *gpio4_base = GPIO4;
struct BUS_IOC_REG *hdmi1_cec_m0 = BUS_IOC;
struct BUS_IOC_REG *hdmi0_cec_m0 = BUS_IOC;
struct PMU2_IOC_REG *hdmi0_cec_m1 = HDMI0_CEC_M1;

static int g_cec_status;
static int g_cec_wakeup_flag;
static int wakeup_cnt = 0;
uint32_t cec_ddr, cec_int_type, cec_bothedge, cec_mask, cec_int;
static int cec_flag;

__WEAK void Cec_IRQHandler(void);

static void GPIO_SetEOI(struct GPIO_REG *pGPIO, ePINCTRL_GPIO_PINS pin)
{
#if (GPIO_VER_ID == 0x01000C2BU)
    if (IS_GPIO_HIGH_PIN(pin)) {
        pin &= 0xFFFF0000;
        pGPIO->PORT_EOI_H = pin | (pin >> 16);
    } else {
        pin &= 0x0000FFFF;
        pGPIO->PORT_EOI_L = pin | (pin << 16);
    }
#else
    {
        pGPIO->PORTA_EOI = pin;
    }
#endif
}

static uint32_t GPIO_GetIntStatus(struct GPIO_REG *pGPIO)
{
    return pGPIO->INT_STATUS;
}

static void Cec_IdleWFI(void)
{
    uint32_t level;

    __asm volatile ("MRS     %0, PRIMASK\n"
                    "CPSID   I"
                    : "=r" (level) : : "memory", "cc");

    __asm volatile ("dsb");
    __asm volatile ("wfi");

    __asm volatile ("MSR     PRIMASK, %0"
                    : : "r" (level) : "memory", "cc");
}

static void Disable_CecGpio(void)
{
    CECDBG("disable cec gpio irq configtion\n");
#ifdef CEC_M0
    /* disable gpio4_c1 int */
    WRITE_REG(gpio4_base->INT_EN_H, 0x20000);
    /* reset gpio4_c1 int mask */
    WRITE_REG(gpio4_base->INT_MASK_H, 0x20002);
    /* reset gpio4_c1 direct input */
    HAL_GPIO_SetPinDirection(GPIO4, GPIO_PIN_C1, GPIO_IN);
    /* reset gpio4_c1 int trigger type */
    HAL_GPIO_SetIntType(GPIO4, GPIO_PIN_C1, GPIO_INT_TYPE_LEVEL_LOW);
    /* reset gpio4_c1 int enabled */
    WRITE_REG(gpio4_base->INT_EN_H, 0x20002);

    if (cec_flag == 1) {
        WRITE_REG(hdmi0_cec_m0->GPIO4C_IOMUX_SEL_L, 0xf00050);
    }
#else
    /* disable gpio0_d1 int */
    WRITE_REG(gpio0_base->INT_EN_H, 0x2000000);
    /* reset gpio0_d1 int mask */
    WRITE_REG(gpio0_base->INT_MASK_H, 0x2000200);
    /* reset gpio0_d1 int trigger type */
    HAL_GPIO_SetIntType(GPIO0, GPIO_PIN_D1, GPIO_INT_TYPE_LEVEL_LOW);
    /* reset gpio0_d1 int enabled */
    WRITE_REG(gpio0_base->INT_EN_H, 0x2000200);

    if (cec_flag == 1) {
        WRITE_REG(hdmi0_cec_m1->GPIO0D_IOMUX_SEL_L, 0xf00080);
    }

#ifdef DEBUG
    cec_ddr = HAL_GPIO_GetPinDirection(GPIO0, GPIO_PIN_D1);
    cec_int_type = GPIO_GetIntType(GPIO0);
    cec_mask = READ_REG(gpio0_base->INT_MASK_H);
    cec_int = READ_REG(gpio0_base->INT_EN_H);

    printf("cec config:%lx, %lx, %lx, %lx, %lx\n", cec_ddr, cec_int_type, cec_mask, cec_int);
#endif

#endif
}

static inline int Gpio_CecDetectBit(unsigned long l_period, unsigned long h_period)
{
    if ((l_period < BIT_START_LOWLEVEL_PERIOD_MAX) &&
        (l_period > BIT_START_LOWLEVEL_PERIOD_MIN)) {
        return BIT_START;
    } else if ((l_period < BIT_0_LOWLEVEL_PERIOD_MAX) &&
               (l_period > BIT_0_LOWLEVEL_PERIOD_MIN)) {
        return BIT_0;
    } else if ((l_period < BIT_1_LOWLEVEL_PERIOD_MAX) &&
               (l_period > BIT_1_LOWLEVEL_PERIOD_MIN)) {
        return BIT_1;
    } else {
        return BIT_UNKOWN;
    }
}

static int Gpio_BluetoothDetectWakeup(void)
{
    MBOX0->B2A[MBOX_CH_1].CMD = MBOX_CMD;
    MBOX0->B2A[MBOX_CH_1].DATA = MBOX_DATA;
    Disable_CecGpio();
    printf("bluetoothenter enter wfi\n");
    while (1) {
        Cec_IdleWFI();
    }

    return 0;
}

static inline int Is_CecWakeUp_Opcode(int opcode)
{
    CECDBG("GPIO CEC: opcode = 0x%x\n", opcode);
    printf("GPIO CEC: opcode = 0x%x\n", opcode);
    switch (opcode) {
    case 0x04:
    case 0x0d:
    case 0x41:
    case 0x42:
    case 0x44:
    case 0x6d:
    case 0x70:
    case 0x82:
    case 0x86:
        CECDBG("wakeup cpu, wakeup_cnt:%d\n", wakeup_cnt);
        if (cec_flag == 0) {
            return 0;
        }
        g_cec_wakeup_flag = 1;
        wakeup_cnt++;
        MBOX0->B2A[MBOX_CH_1].CMD = MBOX_CMD;
        MBOX0->B2A[MBOX_CH_1].DATA = MBOX_DATA;
        Disable_CecGpio();
        printf("cec enter wfi\n");
        while (1) {
            Cec_IdleWFI();
        }

        return 1;
        break;

    default:

        return 0;
        break;
    }
}

static void Gpio_CecDetectOpcode(unsigned long lowleval_time, unsigned long highlevel_time)
{
    static int cec_data_value = 0, cec_data_bit_cnt = 0;
    static int cec_broadcast_flag = 0;
    int val;
    int rxbit;

    rxbit = Gpio_CecDetectBit(lowleval_time, highlevel_time);
    if (rxbit == BIT_START) {
        g_cec_status = START_BIT_STATUS;
    } else if (rxbit == BIT_UNKOWN) {
        printf("GPIO CEC: recevied incorrect bit, low = %ld high=  %ld\n", lowleval_time, highlevel_time);
        g_cec_status = LISTEN_STATUS;
    }

    switch (g_cec_status) {
    case LISTEN_STATUS:
        CECDBG("GPIO CEC: listen status\n");
        break;

    case START_BIT_STATUS:
        CECDBG("GPIO CEC: start bit status\n");
        g_cec_status = DATA_BIT_STATUS;
        cec_data_value = 0;
        cec_data_bit_cnt = 0;
        break;

    case DATA_BIT_STATUS:
        CECDBG("GPIO CEC: reev data bit status\n");
        cec_data_bit_cnt++;

        if (cec_data_bit_cnt <= 8) {
            cec_data_value |= rxbit << (8 - cec_data_bit_cnt);
        } else if (cec_data_bit_cnt == 9) {
            if (rxbit == 1) {
                CECDBG("GPIO CEC: last frame\n");
                //g_cec_status = LISTEN_STATUS;
            }
        } else {
            printf("GPIO CEC: recv cec data block is 0x%x\n", cec_data_value);

            if (cec_broadcast_flag) {
                cec_broadcast_flag = 0;
                val = Is_CecWakeUp_Opcode(cec_data_value);
                if (val == 0x1) {
                    printf("#### GPIO CEC: RECV OPCODE IS WAKEUP !!!, opcode is 0x%x\n", cec_data_value);
                }
            }

            if (cec_data_value == 0xf) {
                cec_broadcast_flag = 1;
            }

            cec_data_value = 0;
            cec_data_bit_cnt = 0;
        }
        break;

    default:
        CECDBG("GPIO CEC: not all status,set to listen status\n");
        g_cec_status = LISTEN_STATUS;
        break;
    }
}

__WEAK void Cec_IRQHandler(void)
{
    static unsigned long cur_period, pre_period, h_period, l_period, duration_time;
    int cec_gpio_status;
    int val;

#ifdef CEC_M0
    val = GPIO_GetIntStatus(GPIO4);
    if ((val & 0x00020000) == 0) {
        CECDBG("GPIO CEC: not cec gpio int!!!\n");

        return;
    }
    if (val & 0x00020000) {
        /* clear gpio4_c1 int */
        GPIO_SetEOI(GPIO4, GPIO_PIN_C1);
#else
    val = GPIO_GetIntStatus(GPIO0);
    if (((val & 0x02000000) == 0) && ((val & 0x00000100) == 0)) {
        CECDBG("GPIO CEC: not cec or buletooth gpio int!!!\n");

        return;
    }

    if (val & 0x02000000) {
        /* clear gpio0_d1 int */
        GPIO_SetEOI(GPIO0, GPIO_PIN_D1);
#endif
        cur_period = HAL_GetSysTimerCount();
        cur_period /= 24;
        CECDBG("GPIO CEC: cur time is %ld\n", cur_period);
        duration_time = cur_period - pre_period;
        CECDBG("GPIO CEC: duration time is %ld\n", duration_time);
        pre_period = cur_period;

#ifdef CEC_M0
        /* we only care c1 bit , check gpio is hi or low */
        cec_gpio_status = HAL_GPIO_GetPinLevel(GPIO4, GPIO_PIN_C1);
#else
        /* we only care d1 bit , check gpio is hi or low */
        cec_gpio_status = HAL_GPIO_GetPinLevel(GPIO0, GPIO_PIN_D1);
#endif
        /*  check if cec gpio is hi */
        if (cec_gpio_status) {
            l_period = duration_time;
            CECDBG("GPIO CEC: low time is %ld\n", duration_time);
        } else {
            h_period = duration_time;
            CECDBG("GPIO CEC: high time is %ld\n", duration_time);

            return;
        }
        Gpio_CecDetectOpcode(l_period, h_period);
#ifdef CEC_M0
    }
#else
    }
#endif
    else if (val & 0x00000100) {
        printf("bule wake up int\n");
        /* clear gpio0_b0 int */
        GPIO_SetEOI(GPIO0, GPIO_PIN_B0);
        Gpio_BluetoothDetectWakeup();
    }
}

int HAL_CEC_Init(void)
{
    uint32_t ret;

    printf("GPIO CEC: RK3588 mcu cec gpio wakeup v1.1.3 \n");
    /*mailbox */
    WRITE_REG(MBOX0->B2A_INTEN, 0xf);

#ifdef CEC_M0
    ret = READ_REG(hdmi0_cec_m0->GPIO4C_IOMUX_SEL_L);
    ret = ret >> 4;
    if ((ret & 0xf) == 0x5) {
        printf("GPIO CEC: cec iomux\n");
        cec_flag = 1;
    } else {
        printf("GPIO CEC: gpio iomux\n");
        cec_flag = 0;
    }

    WRITE_REG(hdmi0_cec_m0->GPIO4C_IOMUX_SEL_L, 0xf00000);
#else
    ret = READ_REG(hdmi0_cec_m1->GPIO0D_IOMUX_SEL_L);
    ret = ret >> 4;
    if ((ret & 0xf) == 0x8) {
        printf("GPIO CEC: cec iomux\n");
        cec_flag = 1;
    } else {
        printf("GPIO CEC: gpio iomux\n");
        cec_flag = 0;
    }

    WRITE_REG(hdmi0_cec_m1->GPIO0D_IOMUX_SEL_L, 0xf00000);
#endif

#ifdef CEC_M0
    printf("set gpio4_c1 config\n");
    /* set gpio4_c1 direct input */
    HAL_GPIO_SetPinDirection(GPIO4, GPIO_PIN_C1, GPIO_IN);
    /* set gpio4_c1 int trigger type */
    HAL_GPIO_SetIntType(GPIO4, GPIO_PIN_C1, GPIO_INT_TYPE_EDGE_BOTH);
    /* set gpio4_c1 irq enable */
    HAL_GPIO_EnableIRQ(GPIO4, GPIO_PIN_C1);
#else
    printf("set gpio0_d1 config\n");
    /* set gpio0_d1 direct input */
    HAL_GPIO_SetPinDirection(GPIO0, GPIO_PIN_D1, GPIO_IN);
    /* set gpio0_d1 int trigger type */
    HAL_GPIO_SetIntType(GPIO0, GPIO_PIN_D1, GPIO_INT_TYPE_EDGE_BOTH);
    /* set gpio0_d1 int mask */
    WRITE_REG(gpio0_base->INT_MASK_H, 0x2000000);
    /* set gpio0_d1 int enabled */
    WRITE_REG(gpio0_base->INT_EN_H, 0x2000200);
#endif
    printf("enable mcu irq\n");
    HAL_NVIC_SetIRQHandler(INTMUX_OUT_START_IRQn + 4, Cec_IRQHandler);
    HAL_NVIC_EnableIRQ(INTMUX_OUT_START_IRQn + 4);
    //INTMUX1->INT_ENABLE_GROUP[7] = 0x000f0002;
#ifdef CEC_MO
    INTMUX1->INT_ENABLE_GROUP[7] = 0x000f000f;
#else
    INTMUX1->INT_ENABLE_GROUP[6] = 0x00200020;
#endif
    __set_PRIMASK(0);

    return 0;
}

#endif
