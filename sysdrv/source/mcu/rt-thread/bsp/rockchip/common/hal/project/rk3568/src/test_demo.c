/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */

#include "hal_bsp.h"
#include "hal_base.h"
#include "task_ipc.h"
#include "unity_runner.h"
#include <stdlib.h>

/********************* Private MACRO Definition ******************************/
//#define GPIO_TEST
//#define GPIO_IRQ_GROUP_TEST
//#define IPI_SGI_TEST
//#define MBOX_TEST
//#define PERF_TEST
//#define PWM_TEST
//#define RPMSG_TEST
//#define RPMSG_LINUX_TEST
//#define RPMSG_PERF_TEST
//#define SOFTIRQ_TEST
//#define SPINLOCK_TEST
//#define TIMER_TEST
//#define UNITY_TEST

#ifdef GPIO_IRQ_GROUP_TEST

/* TODO: Modify GPIO group and priority here */
#define LEVEL0_CPU0_GPIO_BITS (0)
#define LEVEL0_CPU1_GPIO_BITS (0)
#define LEVEL0_CPU2_GPIO_BITS (0)
#define LEVEL0_CPU3_GPIO_BITS (GPIO_PIN_C4 | GPIO_PIN_C5)

#define LEVEL1_CPU0_GPIO_BITS (0)
#define LEVEL1_CPU1_GPIO_BITS (0)
#define LEVEL1_CPU2_GPIO_BITS (0)
#define LEVEL1_CPU3_GPIO_BITS (GPIO_PIN_C6 | GPIO_PIN_C7)

#define LEVEL2_CPU0_GPIO_BITS (0)
#define LEVEL2_CPU1_GPIO_BITS (0)
#define LEVEL2_CPU2_GPIO_BITS (0)
#define LEVEL2_CPU3_GPIO_BITS (0)

#endif

/********************* Private Structure Definition **************************/

static struct GIC_AMP_IRQ_INIT_CFG irqsConfig[] = {
/* TODO: Config the irqs here.
 * GIC version: GICv3
 * The priority higher than 0x80 is non-secure interrupt.
 */
#ifdef GPIO_TEST
    GIC_AMP_IRQ_CFG(GPIO3_IRQn, 0xd0),
#endif

#ifdef MBOX_TEST
    GIC_AMP_IRQ_CFG(MBOX0_CH2_B2A_IRQn, 0xd0),
    GIC_AMP_IRQ_CFG(MBOX0_CH2_A2B_IRQn, 0xd0),
#endif

#ifdef SOFTIRQ_TEST
    GIC_AMP_IRQ_CFG(RSVD0_IRQn, 0xd0),
#endif

#ifdef TIMER_TEST
    GIC_AMP_IRQ_CFG(TIMER0_IRQn, 0xd0),
    GIC_AMP_IRQ_CFG(TIMER1_IRQn, 0xd0),
    GIC_AMP_IRQ_CFG(TIMER2_IRQn, 0xd0),
    GIC_AMP_IRQ_CFG(TIMER3_IRQn, 0xd0),
#endif

    GIC_AMP_IRQ_CFG(0, 0),   /* sentinel */
};

static struct GIC_IRQ_AMP_CTRL irqConfig = {
    .cpuAff = CPU_GET_AFFINITY(1, 0),
    .defPrio = 0xd0,
    .defRouteAff = CPU_GET_AFFINITY(1, 0),
    .irqsCfg = &irqsConfig[0],
};

#ifdef GPIO_IRQ_GROUP_TEST
static const struct GPIO_IRQ_GROUP_CFG gpioIrqCfg[GPIO_BANK_NUM] = {
    [GPIO_BANK0] = {
        GPIO_IRQ_GROUP_GPIO_CFG(GPIO_BANK0, GPIO0_IRQn, GPIO0),
        .groupIrqEn = GPIO_IRQ_GROUP_EN_BANK_TYPE,
        .bankTypeCfg = {
            .hwIrqCpuAff = CPU_GET_AFFINITY(3, 0),
            .prio = 0xd0,
        },
    },
    [GPIO_BANK1] = {
        GPIO_IRQ_GROUP_GPIO_CFG(GPIO_BANK1, GPIO1_IRQn, GPIO1),
        .groupIrqEn = GPIO_IRQ_GROUP_EN_BANK_TYPE,
        .bankTypeCfg = {
            .hwIrqCpuAff = CPU_GET_AFFINITY(3, 0),
            .prio = 0xd0,
        },
    },
    [GPIO_BANK2] = {
        GPIO_IRQ_GROUP_GPIO_CFG(GPIO_BANK2, GPIO2_IRQn, GPIO2),
        .groupIrqEn = GPIO_IRQ_GROUP_EN_BANK_TYPE,
        .bankTypeCfg = {
            .hwIrqCpuAff = CPU_GET_AFFINITY(3, 0),
            .prio = 0xd0,
        },
    },
    [GPIO_BANK3] = {
        GPIO_IRQ_GROUP_GPIO_CFG(GPIO_BANK3, GPIO3_IRQn, GPIO3),
        .groupIrqEn = GPIO_IRQ_GROUP_EN_BANK_TYPE,
        .bankTypeCfg = {
            .hwIrqCpuAff = CPU_GET_AFFINITY(3, 0),
            .prio = 0xd0,
        },
    },
    [GPIO_BANK4] = {
        GPIO_IRQ_GROUP_GPIO_CFG(GPIO_BANK4, GPIO4_IRQn, GPIO4),
        .groupIrqEn = GPIO_IRQ_GROUP_EN_GROUP_TYPE,
        .prioGroup[0] = {
            .cpuGroup = { LEVEL0_CPU0_GPIO_BITS, LEVEL0_CPU1_GPIO_BITS,
                          LEVEL0_CPU2_GPIO_BITS, LEVEL0_CPU3_GPIO_BITS },
            .GIRQId = { RSVD_IRQn(39), RSVD_IRQn(40),
                        RSVD_IRQn(41), GPIO4_IRQn },
            .prio = 0x80,
        },
        .prioGroup[1] = {
            .cpuGroup = { LEVEL1_CPU0_GPIO_BITS, LEVEL1_CPU1_GPIO_BITS,
                          LEVEL1_CPU2_GPIO_BITS, LEVEL1_CPU3_GPIO_BITS },
            .GIRQId = { RSVD_IRQn(42), RSVD_IRQn(43),
                        RSVD_IRQn(44), RSVD_IRQn(45) },
            .prio = 0x90,
        },
        .prioGroup[2] = {
            .cpuGroup = { LEVEL2_CPU0_GPIO_BITS, LEVEL2_CPU1_GPIO_BITS,
                          LEVEL2_CPU2_GPIO_BITS, LEVEL2_CPU3_GPIO_BITS },
            .GIRQId = { RSVD_IRQn(46), RSVD_IRQn(47),
                        RSVD_IRQn(48), RSVD_IRQn(49) },
            .prio = 0xa0,
        },
    },
};
#endif

/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/

/************************************************/
/*                                              */
/*                 HW Borad config              */
/*                                              */
/************************************************/

/* TODO: Set Module IOMUX Function Here */

/************************************************/
/*                                              */
/*                  GPIO_TEST                   */
/*                                              */
/************************************************/
#ifdef GPIO_TEST
static void gpio3_isr(int vector, void *param)
{
    HAL_GPIO_IRQHandler(GPIO3, GPIO_BANK3);
}

static HAL_Status c0_call_back(eGPIO_bankId bank, uint32_t pin, void *args)
{
    printf("GPIO3C0 callback!\n");

    return HAL_OK;
}

static void gpio_test(void)
{
    uint32_t level;

    /* set pinctrl function */
    HAL_PINCTRL_SetParam(GPIO_BANK3,
                         GPIO_PIN_C0,
                         PIN_CONFIG_MUX_FUNC0);
    /* Test GPIO output */
    HAL_GPIO_SetPinDirection(GPIO3, GPIO_PIN_C0, GPIO_OUT);
    level = HAL_GPIO_GetPinLevel(GPIO3, GPIO_PIN_C0);
    printf("test_gpio 3c0 level = %ld\n", level);
    HAL_DelayMs(3000);
    if (level == GPIO_HIGH) {
        HAL_GPIO_SetPinLevel(GPIO3, GPIO_PIN_C0, GPIO_LOW);
    } else {
        HAL_GPIO_SetPinLevel(GPIO3, GPIO_PIN_C0, GPIO_HIGH);
    }
    level = HAL_GPIO_GetPinLevel(GPIO3, GPIO_PIN_C0);
    printf("test_gpio 3c0 level = %ld\n", level);
    HAL_DelayMs(3000);

    /* Test GPIO input */
    HAL_GPIO_SetPinDirection(GPIO3, GPIO_PIN_C0, GPIO_IN);
    HAL_IRQ_HANDLER_SetIRQHandler(GPIO3_IRQn, gpio3_isr, NULL);
    HAL_IRQ_HANDLER_SetGpioIRQHandler(GPIO_BANK3, GPIO_PIN_C0, c0_call_back, NULL);
    HAL_GIC_Enable(GPIO3_IRQn);
    HAL_GPIO_SetIntType(GPIO3, GPIO_PIN_C0, GPIO_INT_TYPE_EDGE_BOTH);
    HAL_GPIO_EnableIRQ(GPIO3, GPIO_PIN_C0);
    printf("test_gpio interrupt ready\n");
}
#endif

/************************************************/
/*                                              */
/*             GPIO_IRQ_GROUP_TEST              */
/*                                              */
/************************************************/
#ifdef GPIO_IRQ_GROUP_TEST
static HAL_Status c5_call_back(eGPIO_bankId bank, uint32_t pin, void *args)
{
    uint32_t priority;

    priority = HAL_GIC_GetPriority(GPIO4_IRQn);
    printf("GPIO4C5 high priority 0x%lx.\n", priority);

    return HAL_OK;
}

static HAL_Status c6_call_back(eGPIO_bankId bank, uint32_t pin, void *args)
{
    uint32_t priority;

    priority = HAL_GIC_GetPriority(RSVD_IRQn(45));
    printf("GPIO4C6 low priority 0x%lx.\n", priority);
    printf("GPIO4C6 low priority callback enter! delay 8s\n");
    HAL_DelayMs(8000);
    printf("GPIO4C6 low priority callback exit!\n");

    return HAL_OK;
}

static void gpio_irq_group_test(void)
{
    printf("GPIO irq group test start!\n");
    HAL_GPIO_SetPinDirection(GPIO4, GPIO_PIN_C5, GPIO_IN);
    HAL_GPIO_SetPinDirection(GPIO4, GPIO_PIN_C6, GPIO_IN);
    HAL_GPIO_SetIntType(GPIO4, GPIO_PIN_C5, GPIO_INT_TYPE_EDGE_RISING);
    HAL_GPIO_SetIntType(GPIO4, GPIO_PIN_C6, GPIO_INT_TYPE_EDGE_RISING);
    HAL_IRQ_HANDLER_SetGpioIRQHandler(GPIO_BANK4, GPIO_PIN_C5, c5_call_back, NULL);
    HAL_IRQ_HANDLER_SetGpioIRQHandler(GPIO_BANK4, GPIO_PIN_C6, c6_call_back, NULL);
    HAL_GPIO_EnableIRQ(GPIO4, GPIO_PIN_C5);
    HAL_GPIO_EnableIRQ(GPIO4, GPIO_PIN_C6);
    printf("GPIO irq group test ready!\n");
}
#endif

/************************************************/
/*                                              */
/*                IPI_SGI_TEST                  */
/*                                              */
/************************************************/
#ifdef IPI_SGI_TEST

#define IPI_CPU0               0x01
#define IPI_CPU1               0x02
#define IPI_CPU2               0x04
#define IPI_CPU3               0x08
#define IPI_TO_TARGETLIST      0
#define IPI_TO_ALL_EXCEPT_SELF 1

static void ipi_sgi_isr(int vector, void *param)
{
    uint32_t cpu_id;

    cpu_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();
    if (cpu_id == 2) {
        HAL_DelayMs(1000);
    } else if (cpu_id == 3) {
        HAL_DelayMs(2000);
    }
    printf("ipi sgi: cpu_id=%ld vector = %d\n", cpu_id, vector);
    HAL_GIC_EndOfInterrupt(vector);
}

static void ipi_sgi_test(void)
{
    uint32_t cpu_id;

    cpu_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();
    HAL_IRQ_HANDLER_SetIRQHandler(IPI_SGI7, ipi_sgi_isr, NULL);
    HAL_GIC_Enable(IPI_SGI7);

    if (cpu_id == 1) {
        printf("ipi sgi: cpu_id=%ld test start\n", cpu_id);
        HAL_DelayMs(2000);
        HAL_GIC_SendSGI(IPI_SGI7, 0, IPI_TO_ALL_EXCEPT_SELF);
        HAL_DelayMs(4000);
        HAL_GIC_SendSGI(IPI_SGI7, IPI_CPU3, IPI_TO_TARGETLIST);
        HAL_DelayMs(4000);
        HAL_GIC_SendSGI(IPI_SGI7, IPI_CPU3 | IPI_CPU2, IPI_TO_TARGETLIST);
        HAL_DelayMs(4000);
        HAL_GIC_SendSGI(IPI_SGI7, IPI_CPU3 | IPI_CPU2 | IPI_CPU0, IPI_TO_TARGETLIST);
    }
}
#endif

/************************************************/
/*                                              */
/*                  MBOX_TEST                   */
/*                                              */
/************************************************/
#ifdef MBOX_TEST

/* master core uses MBOX_A2B and remote core uses MBOX_B2A */
#define MBOX_B2A 0
#define MBOX_A2B 1

static struct MBOX_REG *pMBox = MBOX0;

#ifdef PRIMARY_CPU
static void mbox_master_isr(int vector, void *param)
{
    HAL_MBOX_IrqHandler(vector, pMBox);
    HAL_GIC_EndOfInterrupt(vector);
}

static void mbox_master_cb(struct MBOX_CMD_DAT *msg, void *args)
{
    uint32_t cpu_id;
    struct MBOX_CMD_DAT rx_msg = *msg;

    cpu_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();
    printf("mbox master: recieve cpu-%ld cmd=0x%lx data=0x%lx\n", cpu_id, rx_msg.CMD, rx_msg.DATA);
}
#else
static void mbox_remote_isr(int vector, void *param)
{
    HAL_MBOX_IrqHandler(vector, pMBox);
    HAL_GIC_EndOfInterrupt(vector);
}

static void mbox_remote_cb(struct MBOX_CMD_DAT *msg, void *args)
{
    uint32_t cpu_id;
    struct MBOX_CMD_DAT rx_msg = *msg;

    cpu_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();
    printf("mbox remote: recieve cpu-%ld cmd=0x%lx data=0x%lx\n", cpu_id, rx_msg.CMD, rx_msg.DATA);
}
#endif

/* Use CPU1 as Master Core and CPU2 as Remote Core*/
#ifdef PRIMARY_CPU
static struct MBOX_CLIENT mbox_client2_m = { "mbox-cl2m", MBOX0_CH2_B2A_IRQn, mbox_master_cb, (void *)MBOX_CH_2 };

static void mbox_master_test(void)
{
    struct MBOX_CLIENT *mbox_cl2m;
    struct MBOX_CMD_DAT tx_msg;
    uint32_t cpu_id;
    int ret = 0;

    cpu_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();
    mbox_cl2m = &mbox_client2_m;
    tx_msg.CMD = cpu_id & 0xFU;
    tx_msg.DATA = 0x12345678;

    HAL_MBOX_Init(pMBox, MBOX_A2B);
    ret = HAL_MBOX_RegisterClient(pMBox, MBOX_CH_2, mbox_cl2m);
    if (ret) {
        printf("mbox_cl2m register failed, ret=%d\n", ret);
    }
    HAL_IRQ_HANDLER_SetIRQHandler(MBOX0_CH2_B2A_IRQn, mbox_master_isr, NULL);
    HAL_GIC_Enable(MBOX0_CH2_B2A_IRQn);
    HAL_DelayMs(4000);
    printf("mbox master: send cmd=0x%lx data=0x%lx\n", tx_msg.CMD, tx_msg.DATA);
    HAL_MBOX_SendMsg(pMBox, MBOX_CH_2, &tx_msg);
}
#endif

#ifdef CPU2
static struct MBOX_CLIENT mbox_client2_r = { "mbox-cl2r", MBOX0_CH2_A2B_IRQn, mbox_remote_cb, (void *)MBOX_CH_2 };

static void mbox_remote_test(void)
{
    struct MBOX_CLIENT *mbox_cl2r;
    struct MBOX_CMD_DAT tx_msg;
    uint32_t cpu_id;
    int ret = 0;

    cpu_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();
    mbox_cl2r = &mbox_client2_r;
    tx_msg.CMD = cpu_id & 0xFU;
    tx_msg.DATA = 0x98765432;

    HAL_MBOX_Init(pMBox, MBOX_B2A);
    ret = HAL_MBOX_RegisterClient(pMBox, MBOX_CH_2, mbox_cl2r);
    if (ret) {
        printf("mbox_cl2r register failed, ret=%d\n", ret);
    }
    HAL_IRQ_HANDLER_SetIRQHandler(MBOX0_CH2_A2B_IRQn, mbox_remote_isr, NULL);
    HAL_GIC_Enable(MBOX0_CH2_A2B_IRQn);
    HAL_DelayMs(2000);
    printf("mbox remote: send cmd=0x%lx data=0x%lx\n", tx_msg.CMD, tx_msg.DATA);
    HAL_MBOX_SendMsg(pMBox, MBOX_CH_2, &tx_msg);
}
#endif

#endif

/************************************************/
/*                                              */
/*                  PERF_TEST                   */
/*                                              */
/************************************************/
#ifdef PERF_TEST
#include "benchmark.h"

uint32_t g_sum = 0;
static void perf_test(void)
{
    uint32_t cpu_id, loop = 1000, size = 4 * 1024 * 1024;
    uint32_t *ptr;
    uint64_t start, end;
    double time_s;

    cpu_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();
    printf("perftest: cpu-%ld\n", cpu_id);

    benchmark_main();

    printf("test memset start!\n");
    ptr = (uint32_t *)malloc(size);
    if (ptr) {
        start = HAL_GetSysTimerCount();
        for (int i = 0; i < loop; i++) {
            memset(ptr, i, size);
        }
        end = HAL_GetSysTimerCount();
        time_s = ((end - start) * 1.0) / PLL_INPUT_OSC_RATE;
        printf("\nmemset bw=%.2fMB/s, time_s=%.2f\n", (size * loop) / time_s / 1000000, time_s);

        /* prevent optimization */
        for (int i = 0; i < size / sizeof(uint32_t); i++) {
            g_sum += ptr[i];
        }
        printf("sum=%d\n", g_sum);
        free(ptr);
    }
    printf("test memset end!\n");
}
#endif

/************************************************/
/*                                              */
/*                  PWM_TEST                    */
/*                                              */
/************************************************/
#ifdef PWM_TEST
static uint32_t hal_pwm1_clk = 100000000;
static struct PWM_HANDLE hal_pwm1_handle;
struct HAL_PWM_CONFIG hal_channel0_config = {
    .channel = 0,
    .periodNS = 100000,
    .dutyNS = 40000,
    .polarity = true,
};

struct HAL_PWM_CONFIG hal_channel1_config = {
    .channel = 1,
    .periodNS = 100000,
    .dutyNS = 20000,
    .polarity = false,
};

static void HAL_IOMUX_PWM1_Config(void)
{
    /* PWM1 chanel0-0C3 */
    HAL_PINCTRL_SetIOMUX(GPIO_BANK0, GPIO_PIN_C3, PIN_CONFIG_MUX_FUNC1);

    /* PWM1 chanel1-0C4 */
    HAL_PINCTRL_SetIOMUX(GPIO_BANK0, GPIO_PIN_C4, PIN_CONFIG_MUX_FUNC1);
}

static void pwm_test(void)
{
    printf("pwm_test: test start:\n");

    HAL_PWM_Init(&hal_pwm1_handle, g_pwm1Dev.pReg, hal_pwm1_clk);

    HAL_IOMUX_PWM1_Config();

    HAL_CRU_ClkSetFreq(g_pwm1Dev.clkID, hal_pwm1_clk);

    HAL_PWM_SetConfig(&hal_pwm1_handle, hal_channel0_config.channel, &hal_channel0_config);
    HAL_PWM_SetConfig(&hal_pwm1_handle, hal_channel1_config.channel, &hal_channel1_config);

    HAL_PWM_Enable(&hal_pwm1_handle, hal_channel0_config.channel, HAL_PWM_CONTINUOUS);
    HAL_PWM_Enable(&hal_pwm1_handle, hal_channel1_config.channel, HAL_PWM_CONTINUOUS);
}
#endif

/************************************************/
/*                                              */
/*                 RPMSG_TEST                   */
/*                                              */
/************************************************/
#ifdef RPMSG_TEST
#include "rpmsg_lite.h"

#define MASTER_ID   ((uint32_t)1)
#define REMOTE_ID_1 ((uint32_t)0)
#define REMOTE_ID_2 ((uint32_t)2)
#define REMOTE_ID_3 ((uint32_t)3)

#define RPMSG_CMD_PROB ((uint8_t)0x80)
#define RPMSG_ACK_PROB ((uint8_t)0x81)
#define RPMSG_CMD_TEST ((uint8_t)0x82)
#define RPMSG_ACK_TEST ((uint8_t)0x83)

extern uint32_t __share_rpmsg_start__[];
extern uint32_t __share_rpmsg_end__[];

#define RPMSG_MEM_BASE ((uint32_t)&__share_rpmsg_start__)
#define RPMSG_MEM_END  ((uint32_t)&__share_rpmsg_end__)

#define EPT_M2R_ADDR(id) (id + VRING_SIZE)  // covert master endpoint number to remote endpoint number
#define EPT_R2M_ADDR(id) (id - VRING_SIZE)  // covert remote endpoint number to master endpoint number

struct rpmsg_block_t {
    uint32_t len;
    uint8_t buffer[32 - 4];
};

struct rpmsg_ept_map_t {
    uint32_t base;          // share memory base addr
    uint32_t size;          // share memory size
    uint32_t m_ept_id;    // master endpoint id
    uint32_t r_ept_id;    // remote endpoint id
};

struct rpmsg_info_t {
    struct rpmsg_lite_instance *instance;
    struct rpmsg_lite_endpoint *ept;
    struct rpmsg_ept_map_t *map;
    uint32_t cb_sta;    // callback status flags
    void * private;
};

#define RPMSG_TEST_MEM_SIZE (2UL * RL_VRING_OVERHEAD)
#define RPMSG_TEST0_BASE    (RPMSG_MEM_BASE + 0 * RPMSG_TEST_MEM_SIZE)
#define RPMSG_TEST1_BASE    (RPMSG_MEM_BASE + 1 * RPMSG_TEST_MEM_SIZE)
#define RPMSG_TEST2_BASE    (RPMSG_MEM_BASE + 2 * RPMSG_TEST_MEM_SIZE)
#define RPMSG_TEST3_BASE    (RPMSG_MEM_BASE + 3 * RPMSG_TEST_MEM_SIZE)

// define endpoint id for test
#define RPMSG_TEST0_EPT 0x80000000UL
#define RPMSG_TEST1_EPT 0x80000001UL
#define RPMSG_TEST2_EPT 0x80000002UL
#define RPMSG_TEST3_EPT 0x80000003UL

static struct rpmsg_ept_map_t rpmsg_ept_map_table[4] = {
    { RPMSG_TEST0_BASE, RPMSG_TEST_MEM_SIZE, RPMSG_TEST0_EPT, EPT_M2R_ADDR(RPMSG_TEST0_EPT) },
    { RPMSG_TEST1_BASE, RPMSG_TEST_MEM_SIZE, RPMSG_TEST1_EPT, EPT_M2R_ADDR(RPMSG_TEST1_EPT) },
    { RPMSG_TEST2_BASE, RPMSG_TEST_MEM_SIZE, RPMSG_TEST2_EPT, EPT_M2R_ADDR(RPMSG_TEST2_EPT) },
    { RPMSG_TEST3_BASE, RPMSG_TEST_MEM_SIZE, RPMSG_TEST3_EPT, EPT_M2R_ADDR(RPMSG_TEST3_EPT) },
};

static void rpmsg_share_mem_check(void)
{
    if ((RPMSG_TEST3_BASE + RPMSG_TEST_MEM_SIZE) > RPMSG_MEM_END) {
        rk_printf("share memory size error: (RPMSG_TEST3_BASE + RPMSG_TEST_MEM_SIZE)(0x%x08x) > RPMSG_MEM_END(0x%x08x)\n",
                  RPMSG_TEST3_BASE + RPMSG_TEST_MEM_SIZE, RPMSG_MEM_END);
        while (1) {
            ;
        }
    }
}

static uint32_t remote_id_table[3] = { REMOTE_ID_1, REMOTE_ID_2, REMOTE_ID_3 };
static uint32_t rpmsg_get_remote_index(uint32_t cpu_id)
{
    uint32_t i;

    for (i = 0; i < 3; i++) {
        if (remote_id_table[i] == cpu_id) {
            return i;
        }
    }

    return -1;
}

#ifdef PRIMARY_CPU /*CPU1*/
/*
 payload:       received message data
 payload_len:   received message len
 src:           source ept memory addr
 priv:          private data used in callback
*/
static int32_t master_ept_cb(void *payload, uint32_t payload_len, uint32_t src, void *priv)
{
    uint32_t i, cpu_id;
    struct rpmsg_info_t *info = (struct rpmsg_info_t *)priv;
    struct rpmsg_block_t *block = (struct rpmsg_block_t *)info->private;

    cpu_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();
    //rk_printf("master_ept_cb: master[%d]", cpu_id);

    if (src == info->map->r_ept_id) {     // check source ept addr
        block->len = payload_len;
        memcpy(block->buffer, payload, payload_len);
        info->cb_sta = 1;
        //    printf("<--remote[%d] OK", (uint8_t)block->buffer[0]);
    }
    //printf(", remote ept addr = 0x%08x", src);
    //printf("\n");

    return RL_RELEASE;
}

static void rpmsg_master_test(void)
{
    uint32_t i, j;
    uint32_t master_id, remote_id;
    struct rpmsg_info_t *info;
    struct rpmsg_info_t *p_rpmsg_info[3];
    struct rpmsg_block_t block, *rblock;

    rpmsg_share_mem_check();
    master_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();

    /****************** Initial rpmsg ept **************/
    for (i = 0; i < 3; i++) {
        remote_id = remote_id_table[i];

        info = malloc(sizeof(struct rpmsg_info_t));
        if (info == NULL) {
            rk_printf("info malloc error!\n");
            while (1) {
                ;
            }
        }
        info->private = malloc(sizeof(struct rpmsg_block_t));
        if (info->private == NULL) {
            rk_printf("info malloc error!\n");
            while (1) {
                ;
            }
        }

        info->map = &rpmsg_ept_map_table[remote_id];
        info->instance = rpmsg_master_get_instance(master_id, remote_id);
        info->ept = rpmsg_lite_create_ept(info->instance, info->map->m_ept_id, master_ept_cb, info);

        p_rpmsg_info[i] = info;
    }

    /** probe remote ept, wait for remote ept initialized **/
    block.buffer[0] = RPMSG_CMD_PROB;
    block.buffer[1] = (uint8_t)master_id;
    block.len = 2;

    for (i = 0; i < 3; i++) {
        uint32_t timeout;
        info = p_rpmsg_info[i];
        remote_id = remote_id_table[i];
        rblock = (struct rpmsg_block_t *)info->private;

        for (j = 0; j < 20; j++) {
            rpmsg_lite_send(info->instance, info->ept, info->map->r_ept_id, block.buffer, block.len, RL_BLOCK);

            // wait for remote response
            timeout = 10;
            while (timeout) {
                HAL_DelayMs(1);
                if (info->cb_sta == 1) {
                    info->cb_sta = 0;
                    if (rblock->buffer[0] == RPMSG_ACK_PROB) {
                        rk_printf("rpmsg probe remote cpu(%d) sucess!\n", rblock->buffer[1]);
                        break;
                    }
                }
                timeout--;
            }

            if (timeout) {
                break;
            }
        }

        if (j >= 20) {
            rk_printf("rpmsg probe remote cpu(%d) error!\n", remote_id);
        }
    }

    /****************** rpmsg test run **************/
    block.buffer[0] = RPMSG_CMD_TEST;
    block.buffer[1] = (uint8_t)master_id;
    block.buffer[2] = 0x55;
    block.buffer[3] = 0x55;
    block.buffer[4] = 0x55;
    block.len = 5;
    for (i = 0; i < 3; i++) {
        info = p_rpmsg_info[i];
        info->cb_sta = 0;
        remote_id = remote_id_table[i];
        rk_printf("rpmsg_master_send: master[%d]-->remote[%d], remote ept id = 0x%08x\n", master_id, remote_id, info->map->r_ept_id);
        rpmsg_lite_send(info->instance, info->ept, info->map->r_ept_id, block.buffer, block.len, RL_BLOCK);
    }

    while (1) {
        if (p_rpmsg_info[0]->cb_sta == 1) {
            p_rpmsg_info[0]->cb_sta = 0;

            rblock = (struct rpmsg_block_t *)p_rpmsg_info[0]->private;

            // CMD(ACK): RPMSG_ACK_TEST
            if (rblock->buffer[0] == RPMSG_ACK_TEST) {
                remote_id = rblock->buffer[1];
                block.buffer[0 + 2] = rblock->buffer[0 + 2];
                //rk_printf("0: 0x%x\n", rblock->buffer[0 + 2]);
                rk_printf("rpmsg_master_recv: master[%d]<--remote[%d], remote ept id = 0x%08x\n", master_id, remote_id, info->map->r_ept_id);
            }
            // CMD(ACK): ......
            else {
                //......
            }
        }
        if (p_rpmsg_info[1]->cb_sta == 1) {
            p_rpmsg_info[1]->cb_sta = 0;

            rblock = (struct rpmsg_block_t *)p_rpmsg_info[1]->private;

            // CMD(ACK): RPMSG_ACK_TEST
            if (rblock->buffer[0] == RPMSG_ACK_TEST) {
                remote_id = rblock->buffer[1];
                block.buffer[1 + 2] = rblock->buffer[1 + 2];
                //rk_printf("1: 0x%x\n", rblock->buffer[1 + 2]);
                rk_printf("rpmsg_master_recv: master[%d]<--remote[%d], remote ept id = 0x%08x\n", master_id, remote_id, info->map->r_ept_id);
            }
            // CMD(ACK): ......
            else {
                //......
            }
        }
        if (p_rpmsg_info[2]->cb_sta == 1) {
            p_rpmsg_info[2]->cb_sta = 0;

            rblock = (struct rpmsg_block_t *)p_rpmsg_info[2]->private;

            // CMD(ACK): RPMSG_ACK_TEST
            if (rblock->buffer[0] == RPMSG_ACK_TEST) {
                remote_id = rblock->buffer[1];
                block.buffer[2 + 2] = rblock->buffer[2 + 2];
                //rk_printf("2: 0x%x\n", rblock->buffer[2 + 2]);
                rk_printf("rpmsg_master_recv: master[%d]<--remote[%d], remote ept id = 0x%08x\n", master_id, remote_id, info->map->r_ept_id);
            }
            // CMD(ACK): ......
            else {
                //......
            }
        }
        if ((block.buffer[0 + 2] == 0xff) &&
            (block.buffer[1 + 2] == 0xff) &&
            (block.buffer[2 + 2] == 0xff)) {
            rk_printf("rpmsg test OK!\n");
            while (1) {
                ;
            }
            //break;
        }
        HAL_DelayMs(100);
    }

    for (i = 0; i < 3; i++) {
        free(p_rpmsg_info[i]->private);
        free(p_rpmsg_info[i]);
    }
}
//#endif
#else
//#ifdef CPU0, CPU2, CPU3
/*
 payload:       received message data
 payload_len:   received message len
 src:           source ept memory addr
 priv:          private data used in callback
*/
static int32_t remote_ept_cb(void *payload, uint32_t payload_len, uint32_t src, void *priv)
{
    uint32_t i, cpu_id;
    struct rpmsg_info_t *info = (struct rpmsg_info_t *)priv;
    struct rpmsg_block_t *block = (struct rpmsg_block_t *)info->private;

    cpu_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();
    //rk_printf("remote_ept_cb: remote[%d]", cpu_id);

    if (src == info->map->m_ept_id) {      // check source ept addr
        block->len = payload_len;
        memcpy(block->buffer, payload, payload_len);
        info->cb_sta = 1;
        //    printf("<--master[%d] OK", (uint8_t)block->buffer[0]);
    }
    //printf(", master ept addr = 0x%08x", src);
    //printf("\n");

    return RL_RELEASE;
}

static void rpmsg_remote_test(void)
{
    uint32_t i, master_id, remote_id;
    struct rpmsg_info_t *info;
    struct rpmsg_block_t *block;

    rpmsg_share_mem_check();
    master_id = MASTER_ID;
    remote_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();

    info = malloc(sizeof(struct rpmsg_info_t));
    if (info == NULL) {
        rk_printf("info malloc error!\n");
        while (1) {
            ;
        }
    }
    info->private = malloc(sizeof(struct rpmsg_block_t));
    if (info->private == NULL) {
        rk_printf("info malloc error!\n");
        while (1) {
            ;
        }
    }

    info->map = &rpmsg_ept_map_table[remote_id];
    info->cb_sta = 0;
    info->instance = rpmsg_remote_get_instance(master_id, remote_id);
    info->ept = rpmsg_lite_create_ept(info->instance, info->map->r_ept_id, remote_ept_cb, info);

    while (1) {
        if (info->cb_sta == 1) {
            info->cb_sta = 0;

            block = (struct rpmsg_block_t *)info->private;

            // CMD(ACK): RPMSG_CMD_PROB
            if (block->buffer[0] == RPMSG_CMD_PROB) {
                block->buffer[0] = RPMSG_ACK_PROB;
                block->buffer[1] = remote_id;
                block->len = 2;
                rpmsg_lite_send(info->instance, info->ept, info->map->m_ept_id, block->buffer, block->len, RL_BLOCK);
            }
            // CMD(ACK): RPMSG_CMD_TEST
            else if (block->buffer[0] == RPMSG_CMD_TEST) {
                rk_printf("rpmsg_remote_recv: remote[%d]<--master[%d], master ept id = 0x%08x\n", remote_id, block->buffer[1], info->map->m_ept_id);

                block->buffer[0] = RPMSG_ACK_TEST;
                block->buffer[1] = remote_id;

                i = rpmsg_get_remote_index(remote_id);
                block->buffer[i + 2] |= 0xaa;

                rk_printf("rpmsg_remote_send: remote[%d]-->master[%d], master ept id = 0x%08x\n", remote_id, master_id, info->map->m_ept_id);
                rpmsg_lite_send(info->instance, info->ept, info->map->m_ept_id, block->buffer, block->len, RL_BLOCK);
            }
            // CMD(ACK): ......
            else {
                //......
            }
        }
        HAL_DelayMs(100);
    }
}
#endif

#endif

/************************************************/
/*                                              */
/*              RPMSG_LINUX_TEST                */
/*                                              */
/************************************************/
#ifdef RPMSG_LINUX_TEST
#include "rpmsg_lite.h"
#include "rpmsg_ns.h"

// test is CPU0 as master and CPU3 as remote.
#define MASTER_ID   ((uint32_t)0)
#define REMOTE_ID_3 ((uint32_t)3)

// define remote endpoint id for test
#define RPMSG_HAL_REMOTE_TEST3_EPT      0x3003U
#define RPMSG_HAL_REMOTE_TEST_EPT3_NAME "rpmsg-ap3-ch0"

#define RPMSG_HAL_TEST_MSG "Rockchip rpmsg linux test!"

/* TODO: These are defined in the linked script gcc_arm.ld.S */
extern uint32_t __linux_share_rpmsg_start__[];
extern uint32_t __linux_share_rpmsg_end__[];

#define RPMSG_LINUX_MEM_BASE ((uint32_t)&__linux_share_rpmsg_start__)
#define RPMSG_LINUX_MEM_END  ((uint32_t)&__linux_share_rpmsg_end__)
#define RPMSG_LINUX_MEM_SIZE (2UL * RL_VRING_OVERHEAD)

struct rpmsg_block_t {
    uint32_t len;
    uint8_t buffer[496 - 4];
};

struct rpmsg_info_t {
    struct rpmsg_lite_instance *instance;
    struct rpmsg_lite_endpoint *ept;
    uint32_t cb_sta;    // callback status flags
    void * private;
    uint32_t m_ept_id;
};

static void rpmsg_share_mem_check(void)
{
    if ((RPMSG_LINUX_MEM_BASE + RPMSG_LINUX_MEM_SIZE) > RPMSG_LINUX_MEM_END) {
        rk_printf("shared memory size error!\n");
        while (1) {
            ;
        }
    }
}

rpmsg_ns_new_ept_cb rpmsg_ns_cb(uint32_t new_ept, const char *new_ept_name, uint32_t flags, void *user_data)
{
    uint32_t cpu_id;
    char ept_name[RL_NS_NAME_SIZE];

    cpu_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();
    strncpy(ept_name, new_ept_name, RL_NS_NAME_SIZE);
    printf("rpmsg remote: new_ept-0x%lx name-%s\n", new_ept, ept_name);
}

static int32_t remote_ept_cb(void *payload, uint32_t payload_len, uint32_t src, void *priv)
{
    int ret;
    uint32_t cpu_id;
    struct rpmsg_info_t *info = (struct rpmsg_info_t *)priv;
    struct rpmsg_block_t *block = (struct rpmsg_block_t *)info->private;

    cpu_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();

    info->m_ept_id = src;
    block->len = payload_len;
    memcpy(block->buffer, payload, payload_len);
    info->cb_sta = 1;
    //printf("rpmsg remote: msg: %s\n", block->buffer);
    ret = rpmsg_lite_send(info->instance, info->ept, info->m_ept_id, RPMSG_HAL_TEST_MSG, strlen(RPMSG_HAL_TEST_MSG), RL_BLOCK);

    if (ret) {
        rk_printf("rpmsg_lite_send error!\n");
    }

    return ret;
}

/* Comments are used to test hal name service functionality as master */
#if 0
static void rpmsg_ns_hal_master_test(void)
{
    uint32_t master_id, remote_id;
    struct rpmsg_info_t *info;
    struct rpmsg_block_t block, *rblock;

    rpmsg_share_mem_check();
    master_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();
    rk_printf("rpmsg master: master core cpu_id-%ld\n", master_id);

    /****************** Initial rpmsg ept **************/
    remote_id = REMOTE_ID_3;
    info = malloc(sizeof(struct rpmsg_info_t));
    if (info == NULL) {
        rk_printf("info malloc error!\n");
        while (1) {
            ;
        }
    }
    info->private = malloc(sizeof(struct rpmsg_block_t));
    if (info->private == NULL) {
        rk_printf("info malloc error!\n");
        while (1) {
            ;
        }
    }
    info->map = &rpmsg_ept_map_table[remote_id];
    rk_printf("master init before!\n");
    info->instance = rpmsg_lite_master_init((void *)RPMSG_LINUX_MEM_BASE, RPMSG_LINUX_MEM_SIZE,
                                            RL_PLATFORM_SET_LINK_ID(master_id, remote_id), RL_NO_FLAGS);
    rpmsg_ns_bind(info->instance, rpmsg_ns_cb, info);
}
#endif

static void rpmsg_linux_test(void)
{
    uint32_t master_id, remote_id;
    struct rpmsg_info_t *info;
    struct rpmsg_block_t *block;
    uint32_t ept_flags;
    void *ns_cb_data;

    rpmsg_share_mem_check();
    master_id = MASTER_ID;
    remote_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();
    rk_printf("rpmsg remote: remote core cpu_id-%ld\n", remote_id);
//    rk_printf("rpmsg remote: shmem_base-0x%lx shmem_end-%lx\n", RPMSG_LINUX_MEM_BASE, RPMSG_LINUX_MEM_END);

    info = malloc(sizeof(struct rpmsg_info_t));
    if (info == NULL) {
        rk_printf("info malloc error!\n");
        while (1) {
            ;
        }
    }
    info->private = malloc(sizeof(struct rpmsg_block_t));
    if (info->private == NULL) {
        rk_printf("info malloc error!\n");
        while (1) {
            ;
        }
    }

    info->instance = rpmsg_lite_remote_init((void *)RPMSG_LINUX_MEM_BASE, RL_PLATFORM_SET_LINK_ID(master_id, remote_id), RL_NO_FLAGS);
    rpmsg_lite_wait_for_link_up(info->instance);
    rk_printf("rpmsg remote: link up! link_id-0x%lx\n", info->instance->link_id);
    rpmsg_ns_bind(info->instance, rpmsg_ns_cb, &ns_cb_data);
    info->ept = rpmsg_lite_create_ept(info->instance, RPMSG_HAL_REMOTE_TEST3_EPT, remote_ept_cb, info);
    ept_flags = RL_NS_CREATE;
    rpmsg_ns_announce(info->instance, info->ept, RPMSG_HAL_REMOTE_TEST_EPT3_NAME, ept_flags);
    while (info->cb_sta != 1) {
    }
    rpmsg_lite_send(info->instance, info->ept, info->m_ept_id, RPMSG_HAL_TEST_MSG, sizeof(RPMSG_HAL_TEST_MSG), RL_BLOCK);
}
#endif

/************************************************/
/*                                              */
/*               RPMSG_PERF_TEST                */
/*                                              */
/************************************************/
#ifdef RPMSG_PERF_TEST
#include "rpmsg_lite.h"
#include "rpmsg_ns.h"
#include "rpmsg_perf.h"

/* TODO: Configure RPMSG PERF TEST shared memory base */
#define RPMSG_PERF_MEM_BASE 0x7a00000
#define RPMSG_PERF_MEM_SIZE 0x20000

static void rpmsg_perf_master_test(void)
{
    uint32_t cpu_id;
    struct rpmsg_lite_instance *master_rpmsg;

    cpu_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();
    rk_printf("rpmsg master: master core cpu_id-%ld\n", cpu_id);
    master_rpmsg = rpmsg_lite_master_init((void *)RPMSG_PERF_MEM_BASE, RPMSG_PERF_MEM_SIZE,
                                          RL_PLATFORM_SET_LINK_ID(0, 3), RL_NO_FLAGS);
    rpmsg_perf_master_main(master_rpmsg);
}

static void rpmsg_perf_remote_test(void)
{
    uint32_t cpu_id;
    struct rpmsg_lite_instance *remote_rpmsg;

    cpu_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();
    rk_printf("rpmsg remote: remote core cpu_id-%ld\n", cpu_id);
    remote_rpmsg = rpmsg_lite_remote_init((void *)RPMSG_PERF_MEM_BASE,
                                          RL_PLATFORM_SET_LINK_ID(0, 3), RL_NO_FLAGS);
    rpmsg_lite_wait_for_link_up(remote_rpmsg);
    rk_printf("rpmsg remote: link up! link_id-0x%lx\n", remote_rpmsg->link_id);
    rpmsg_perf_remote_main(remote_rpmsg);
}
#endif

/************************************************/
/*                                              */
/*                SOFTIRQ_TEST                  */
/*                                              */
/************************************************/
#ifdef SOFTIRQ_TEST
static void soft_isr(int vector, void *param)
{
    printf("soft_isr, vector = %d\n", vector);
    HAL_GIC_EndOfInterrupt(vector);
}

static void softirq_test(void)
{
    HAL_IRQ_HANDLER_SetIRQHandler(RSVD0_IRQn, soft_isr, NULL);
    HAL_GIC_Enable(RSVD0_IRQn);

    HAL_GIC_SetPending(RSVD0_IRQn);
}
#endif

/************************************************/
/*                                              */
/*                SPINLOCK_TEST                 */
/*                                              */
/************************************************/
#ifdef SPINLOCK_TEST
static void spinlock_test(void)
{
    uint32_t cpu_id, owner;
    HAL_Check ret;

    cpu_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();
    printf("begin spinlock test: cpu=%ld\n", cpu_id);

    while (1) {
        ret = HAL_SPINLOCK_TryLock(0);
        if (ret) {
            printf("try lock success: %ld\n", cpu_id);
            HAL_SPINLOCK_Unlock(0);
        } else {
            printf("try lock failed: %ld\n", cpu_id);
        }
        HAL_SPINLOCK_Lock(0);
        printf("enter cpu%ld\n", cpu_id);
        HAL_CPUDelayUs(rand() % 2000000);
        owner = HAL_SPINLOCK_GetOwner(0);
        if ((owner >> 1) != cpu_id) {
            printf("owner id is not matched(%ld, %ld)\n", cpu_id, owner);
        }
        printf("leave cpu%ld\n", cpu_id);
        HAL_SPINLOCK_Unlock(0);
        HAL_CPUDelayUs(10);
    }
}
#endif

/************************************************/
/*                                              */
/*                  TIMER_TEST                  */
/*                                              */
/************************************************/
#ifdef TIMER_TEST
static int timer_int_count = 0;
static uint32_t latency_sum = 0;
struct TIMER_REG *timer = NULL;
static bool desc_timer = true;
static int fixed_spend = 0;
static uint32_t latency_max = 0;
static struct TIMER_REG *g_timer[4] = { TIMER0, TIMER1, TIMER2, TIMER3 };
static uint32_t g_timer_irq[4] = { TIMER0_IRQn, TIMER1_IRQn, TIMER2_IRQn, TIMER3_IRQn };

static void timer_isr(int vector, void *param)
{
    uint32_t count, cpu_id;
    uint32_t latency;

    cpu_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();
    count = (uint32_t)HAL_TIMER_GetCount(timer);
    if (desc_timer) {
        count = 24000000 - count;
    }
    if (count > fixed_spend) {
        count -= fixed_spend;
    }
    /* 24M timer: 41.67ns per count */
    latency = count * 41;
    printf("count=%d\n", count);
    printf("cpu_id=%d: latency=%dns(count=%d)\n", cpu_id, latency, count);
    timer_int_count++;
    latency_sum += latency;
    latency_max = latency_max > latency ? latency_max : latency;
    if (timer_int_count == 100) {
        printf("cpu_id=%d: latency avg=%d,max=%d\n", cpu_id, latency_sum / timer_int_count, latency_max);
        timer_int_count = 0;
        latency_sum = 0;
        latency_max = 0;
        HAL_TIMER_ClrInt(timer);
        HAL_GIC_EndOfInterrupt(g_timer_irq[cpu_id]);
        HAL_TIMER_Stop_IT(timer);
    }

    HAL_TIMER_ClrInt(timer);
    HAL_GIC_EndOfInterrupt(g_timer_irq[cpu_id]);
}

static void timer_test(void)
{
    uint64_t start, end;
    uint32_t count, cpu_id;

    cpu_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();

    start = HAL_GetSysTimerCount();
    HAL_CPUDelayUs(1000000);
    end = HAL_GetSysTimerCount();
    count = (uint32_t)(end - start);
    printf("systimer 1s count: %ld(%lld, %lld)\n", count, start, end);

    printf("\n\ncpu_id=%ld: test internal irq\n", cpu_id);
    timer = g_timer[cpu_id];
    /* Pay attention to the timer type */
    desc_timer = true;
    HAL_TIMER_Init(timer, TIMER_FREE_RUNNING);
    HAL_TIMER_SetCount(timer, 2000000000);
    HAL_TIMER_Start(timer);
    start = HAL_TIMER_GetCount(timer);
    HAL_CPUDelayUs(1000000);
    end = HAL_TIMER_GetCount(timer);
    count = (uint32_t)(end - start);
    fixed_spend = start;
    printf("cpu_id=%ld: internal timer 1s count: %ld(%lld, %lld), fixed_spend=%d\n",
           cpu_id, count, start, end, fixed_spend);
    HAL_TIMER_Stop(timer);

    HAL_IRQ_HANDLER_SetIRQHandler(g_timer_irq[cpu_id], timer_isr, NULL);
    HAL_GIC_Enable(g_timer_irq[cpu_id]);
    HAL_TIMER_Init(timer, TIMER_FREE_RUNNING);
    HAL_TIMER_SetCount(timer, 24000000);
    HAL_TIMER_Start_IT(timer);
}
#endif

/********************* Public Function Definition ****************************/

void TEST_DEMO_GIC_Init(void)
{
    HAL_GIC_Init(&irqConfig);
}

void test_demo(void)
{
#if defined(GPIO_TEST) && defined(PRIMARY_CPU)
    gpio_test();
#endif

#ifdef GPIO_IRQ_GROUP_TEST
/* Each core needs to initialize the GPIO IRQ Group */
    HAL_GPIO_IRQ_GROUP_Init(CPU_GET_AFFINITY(1, 0),
                            gpioIrqCfg,
                            HAL_IRQ_HANDLER_GetGpioIrqGroupOps());
#ifdef CPU3
    HAL_DelayMs(4000);
    gpio_irq_group_test();
#endif
#endif

#ifdef IPI_SGI_TEST
    ipi_sgi_test();
#endif

#ifdef MBOX_TEST
#ifdef PRIMARY_CPU /*CPU1*/
    mbox_master_test();
#endif
#ifdef CPU2
    mbox_remote_test();
#endif
#endif

#if defined(PERF_TEST) && defined(PRIMARY_CPU)
    perf_test();
#endif

#if defined(PWM_TEST) && defined(PRIMARY_CPU)
    pwm_test();
#endif

#ifdef RPMSG_TEST
#ifdef PRIMARY_CPU /*CPU1*/
    rk_printf("PRIMARY_CPU:%d\n", PRIMARY_CPU);
    rpmsg_master_test();
//#endif
#else
//#ifdef CPU0
    rpmsg_remote_test();
#endif
#endif

#if defined(RPMSG_LINUX_TEST) && defined(CPU3)
    rpmsg_linux_test();
#endif

#ifdef RPMSG_PERF_TEST
#ifdef CPU0
    rpmsg_perf_master_test();
#elif CPU3
    rpmsg_perf_remote_test();
#endif
#endif

#if defined(SOFTIRQ_TEST) && defined(PRIMARY_CPU)
    softirq_test();
#endif

#ifdef SPINLOCK_TEST
    spinlock_test();
#endif

#ifdef TIMER_TEST
    timer_test();
#endif

#if defined(UNITY_TEST) && defined(PRIMARY_CPU)
    /* Unity Test */
    test_main();
#endif
}
