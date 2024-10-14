/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */

#include "hal_bsp.h"
#include "hal_base.h"
#include "task_ipc.h"
#include <stdlib.h>

/********************* Private MACRO Definition ******************************/
//#define SOFTRST_TEST
//#define SOFTIRQ_TEST
//#define FAULTDBG_TEST
//#define TIMER_TEST
//#define SPINLOCK_TEST
//#define TSADC_TEST
//#define GPIO_TEST
//#define UART_TEST
//#define DMA_LINK_LIST_TEST
//#define PERF_TEST
//#define RPMSG_TEST
//#define UNITY_TEST
#define CPU_USAGE_TEST

/********************* Private Structure Definition **************************/

static struct GIC_AMP_IRQ_INIT_CFG irqsConfig[] = {
    /* The priority higher than 0x80 is non-secure interrupt. */

#if defined(TIMER_TEST) || defined(CPU_USAGE_TEST)
    GIC_AMP_IRQ_CFG_ROUTE(TIMER0_IRQn, 0xd0, CPU_GET_AFFINITY(0, 0)),
    GIC_AMP_IRQ_CFG_ROUTE(TIMER1_IRQn, 0xd0, CPU_GET_AFFINITY(1, 0)),
    GIC_AMP_IRQ_CFG_ROUTE(TIMER2_IRQn, 0xd0, CPU_GET_AFFINITY(2, 0)),
    GIC_AMP_IRQ_CFG_ROUTE(TIMER3_IRQn, 0xd0, CPU_GET_AFFINITY(3, 0)),
#endif

#ifdef GPIO_TEST
    GIC_AMP_IRQ_CFG_ROUTE(GPIO0_IRQn, 0xd0, CPU_GET_AFFINITY(1, 0)),
#endif

#ifdef SOFTIRQ_TEST
    GIC_AMP_IRQ_CFG_ROUTE(RSVD0_IRQn, 0xd0, CPU_GET_AFFINITY(1, 0)),
#endif

    GIC_AMP_IRQ_CFG_ROUTE(0, 0, CPU_GET_AFFINITY(1, 0)),   /* sentinel */
};

static struct GIC_IRQ_AMP_CTRL irqConfig = {
    .cpuAff = CPU_GET_AFFINITY(1, 0),
    .defPrio = 0xd0,
    .defRouteAff = CPU_GET_AFFINITY(1, 0),
    .irqsCfg = &irqsConfig[0],
};

/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/

/************************************************/
/*                                              */
/*                 HW Borad config              */
/*                                              */
/************************************************/

/************************************************/
/*                                              */
/*                  UART_TEST                   */
/*                                              */
/************************************************/
#ifdef UART_TEST
static struct UART_REG *pUart = UART4;      // UART2 or UART4, selected depend on hardware board
void uart_test(void)
{
    uint8_t buf[2];
    uint8_t input, cnt = 0;

    // must input 16 chars to exit the test
    for (input = 0; input < 16; input++) {
        while (1) {
            cnt = HAL_UART_SerialIn(pUart, buf, 1);
            if (cnt > 0) {
                break;
            }
        }
        buf[1] = 0;
        HAL_UART_SerialOutChar(pUart, (char)buf[0]);
    }
}
#endif

/************************************************/
/*                                              */
/*                 SOFTRST_TEST                 */
/*                                              */
/************************************************/
#ifdef SOFTRST_TEST
typedef enum {
    SOFT_SRST_DIRECT = 0,
    SOFT_SRST_MASKROM,
    SOFT_SRST_LOADER,
} st_RstType;

/* system reset test*/
void softrst_test(st_RstType mode)
{
    if (mode == SOFT_SRST_MASKROM) {
        /* Reset to maskrom */
        BSP_SetMaskRomFlag();
    } else if (mode == SOFT_SRST_LOADER) {
        /* Reset to Loader */
        BSP_SetLoaderFlag();
    } else {
        /* Direct reboot system */
    }

    HAL_CRU_SetGlbSrst(GLB_SRST_FST);
    printf("softrst test fail\n");
    while (1) {
        ;
    }
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
/*                FAULTDBG_TEST                 */
/*                                              */
/************************************************/
#ifdef FAULTDBG_TEST
static void fault_dbg_test(void)
{
    // If system fault happend, use "addr2line" command to debug
    // Such as follows cpu0 fault

    // This is an example for accessing invalid address
    // if fault happend, log output as followed:
    /*
        abort mode:
        pc : 02607684  lr : 02607674 cpsr: 600e0013
        sp : 02eff7e8  ip : 0260e5d0  fp : 00000000
        r10: 00000560  r9 : 1f58cdf8  r8 : 1f5ce540
        r7 : 00000004  r6 : 1ffefaf4  r5 : 1f5cd500  r4 : 02eff7e8
        r3 : aaaaaaaa  r2 : 90000000  r1 : 0000000a  r0 : 00000000

        stack:
        0x02eff7e8: 0x0016e360  0x00020008  0x00000000  0x026001b0
        0x02eff7f8: 0x00000000  0x02600318

        Show more call stack info by run: addr2line -e hal0.elf -a -f 02607684 02607674 026001b0 02600318
    */
    // use command to find errors:
    // cd hal/
    // addr2line -e project/rk3308/GCC/hal0.elf -a -f 02607684 02607674 026001b0 02600318
    volatile uint32_t *p_addr = (uint32_t *)0x90000000;

    *p_addr = 0xaaaaaaaa;
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
    desc_timer = false;
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

/************************************************/
/*                                              */
/*                 TSADC_TEST                   */
/*                                              */
/************************************************/
#ifdef TSADC_TEST
static void tsadc_test(void)
{
    HAL_CRU_ClkSetFreq(CLK_TSADC, 50000);
    HAL_TSADC_Enable_AUTO(0, TSHUT_LOW_ACTIVE, TSHUT_MODE_CRU);
    printf("GET TEMP %d!\n", HAL_TSADC_GetTemperature_AUTO(0));
}
#endif

/************************************************/
/*                                              */
/*                  GPIO_TEST                   */
/*                                              */
/************************************************/
#ifdef GPIO_TEST
static void gpio_isr(int vector, void *param)
{
    printf("Enter GPIO IRQHander!\n");
    HAL_GPIO_IRQHandler(GPIO0, GPIO_BANK0);
    printf("Leave GPIO IRQHandler!\n");
}

static HAL_Status c4_call_back(eGPIO_bankId bank, uint32_t pin, void *args)
{
    printf("GPIO callback!\n");

    return HAL_OK;
}

static void gpio_test(void)
{
    uint32_t level;

    /* Test GPIO output */
    HAL_GPIO_SetPinDirection(GPIO0, GPIO_PIN_C4, GPIO_OUT);
    level = HAL_GPIO_GetPinLevel(GPIO0, GPIO_PIN_C4);
    printf("test_gpio level = %ld\n", level);
    HAL_DelayMs(5000);
    if (level == GPIO_HIGH) {
        HAL_GPIO_SetPinLevel(GPIO0, GPIO_PIN_C4, GPIO_LOW);
    } else {
        HAL_GPIO_SetPinLevel(GPIO0, GPIO_PIN_C4, GPIO_HIGH);
    }
    level = HAL_GPIO_GetPinLevel(GPIO0, GPIO_PIN_C4);
    printf("test_gpio level = %ld\n", level);
    HAL_DelayMs(5000);

    /* Test GPIO input */
    HAL_GPIO_SetPinDirection(GPIO0, GPIO_PIN_C4, GPIO_IN);
    HAL_IRQ_HANDLER_SetIRQHandler(GPIO0_IRQn, gpio_isr, NULL);
    HAL_IRQ_HANDLER_SetGpioIRQHandler(GPIO_BANK0, GPIO_PIN_C4, c4_call_back, NULL);
    HAL_GIC_Enable(GPIO0_IRQn);
    HAL_GPIO_SetIntType(GPIO0, GPIO_PIN_C4, GPIO_INT_TYPE_EDGE_RISING);
    HAL_GPIO_EnableIRQ(GPIO0, GPIO_PIN_C4);
}
#endif

/************************************************/
/*                                              */
/*            DMA_LINK_LIST_TEST                */
/*                                              */
/************************************************/
#ifdef DMA_LINK_LIST_TEST
#define DMA_SIZE         64
#define DMA_TEST_CHANNEL 0
#define XFER_LIST_SIZE   128
__ALIGNED(64) uint8_t src[DMA_SIZE * 2] = { 0 };
__ALIGNED(64) uint8_t dst[DMA_SIZE * 2] = { 0 };
HAL_LIST_HEAD(pxfer_link_list);

static void HAL_PL330_Handler(uint32_t irq, void *args)
{
    struct HAL_PL330_DEV *pl330 = (struct HAL_PL330_DEV *)args;
    uint32_t irqStatus;

    irqStatus = HAL_PL330_IrqHandler(pl330);
    if (irqStatus & (1 << DMA_TEST_CHANNEL)) {
        if (pl330->chans[DMA_TEST_CHANNEL].desc.callback) {
            pl330->chans[DMA_TEST_CHANNEL].desc.callback(&pl330->chans[DMA_TEST_CHANNEL]);
        }

        if (pl330->chans[DMA_TEST_CHANNEL].pdesc->callback) {
            pl330->chans[DMA_TEST_CHANNEL].pdesc->callback(&pl330->chans[DMA_TEST_CHANNEL]);
        }
    }
}

static void MEMCPY_Callback(void *cparam)
{
    struct PL330_CHAN *pchan = cparam;
    uint32_t ret;
    int i;

    for (i = 0; i < DMA_SIZE * 2; i++) {
        if (src[i] != dst[i]) {
            printf("DMA transfor error, src[%d] is %x, dst[%d] is %x\n",
                   i, src[i], i, dst[i]);
            break;
        }
    }
    ret = HAL_PL330_Stop(pchan);
    if (ret) {
        printf("Stop DMA fail\n");

        return;
    }

    ret = HAL_PL330_ReleaseChannel(pchan);
    if (ret) {
        printf("Release DMA fail\n");

        return;
    }
}

static void xferdata_init(struct PL330_XFER_SPEC_LIST *xfer_list)
{
    struct PL330_XFER_SPEC_LIST *xfer_list_after = xfer_list;
    struct PL330_XFER_SPEC_LIST *xfer_list_befor = xfer_list;

    xfer_list_after->xfer.srcAddr = src;
    xfer_list_after->xfer.dstAddr = dst;
    xfer_list_after->xfer.length = DMA_SIZE;
    HAL_LIST_InsertAfter(&pxfer_link_list, &xfer_list_after->node);
    xfer_list_after += sizeof(struct PL330_XFER_SPEC_LIST);
    xfer_list_after->xfer.srcAddr = src + DMA_SIZE;
    xfer_list_after->xfer.dstAddr = dst + DMA_SIZE;
    xfer_list_after->xfer.length = DMA_SIZE;
    HAL_LIST_InsertAfter(&xfer_list_befor->node, &xfer_list_after->node);
}

static void dmalinklist_test(void)
{
    __ALIGNED(64) uint8_t buf[PL330_CHAN_BUF_LEN] = { 0 };
    __ALIGNED(64) static uint8_t pxferList[XFER_LIST_SIZE] = { 0 };
    __ALIGNED(64) static uint8_t pdesc[XFER_LIST_SIZE * 2] = { 0 };
    uint32_t timeout = 1000;
    struct PL330_CHAN *pchan;
    int ret, i;

#ifdef DMA0_BASE
    struct HAL_PL330_DEV *pl330 = &g_pl330Dev0;
#else
    struct HAL_PL330_DEV *pl330 = &g_pl330Dev;
#endif

    ret = HAL_PL330_Init(pl330);
    if (ret) {
        printf("HAL_PL330_Init fail!\n");

        return;
    }

    for (i = 0; i < DMA_SIZE * 2; i++) {
        src[i] = i;
    }

    HAL_IRQ_HANDLER_SetIRQHandler(pl330->irq[0], HAL_PL330_Handler, pl330);
    HAL_IRQ_HANDLER_SetIRQHandler(pl330->irq[1], HAL_PL330_Handler, pl330);
    HAL_GIC_Enable(pl330->irq[0]);
    HAL_GIC_Enable(pl330->irq[1]);
    pchan = HAL_PL330_RequestChannel(pl330, (DMA_REQ_Type)DMA_TEST_CHANNEL);
    if (!pchan) {
        printf("Can not find used channel!\n");

        return;
    }

    HAL_PL330_SetMcBuf(pchan, buf);
    xferdata_init(pxferList);
    ret = HAL_PL330_PrepDmaLinkList(pchan, pxferList,
                                    pdesc, DMA_MEM_TO_MEM,
                                    MEMCPY_Callback, pchan);
    ret = HAL_PL330_Start(pchan);
    if (ret) {
        printf("Start dma fail\n");
    }

    while (timeout--) {
        if (pl330->pReg->INTEN & (1 << DMA_TEST_CHANNEL) == 0) {
            break;
        }

        HAL_DelayUs(10);
    }

    if (!timeout) {
        printf("Wait DMA finish timeout\n");

        return;
    }

    ret = HAL_PL330_DeInit(pl330);
    if (ret) {
        printf("DeInit DMA fail\n");

        return;
    }

    printf("dmalinklist_test OK!\n");
}
#endif

/************************************************/
/*                                              */
/*                  PERF_TEST                   */
/*                                              */
/************************************************/
#ifdef PERF_TEST
#include "benchmark.h"

void config_freq(void)
{
    HAL_CRU_ClkSetFreq(PLL_APLL, 1008000000);
    HAL_SystemCoreClockUpdate(1008000000, HAL_SYSTICK_CLKSRC_EXT);
}

uint32_t g_sum = 0;
static void perf_test(void)
{
    uint32_t cpu_id, loop = 1000, size = 4 * 1024 * 1024;
    uint32_t *ptr;
    uint64_t start, end;
    double time_s;

    cpu_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();
    if (cpu_id == 0) {
        benchmark_main();
        config_freq();
        benchmark_main();

        ptr = (uint32_t *)malloc(size);
        if (ptr) {
            start = HAL_GetSysTimerCount();
            for (int i = 0; i < loop; i++) {
                memset(ptr, i, size);
            }
            end = HAL_GetSysTimerCount();
            time_s = ((end - start) * 1.0) / PLL_INPUT_OSC_RATE;
            printf("memset bw=%.2fMB/s, time_s=%.2f\n", (size * loop) / time_s / 1000000, time_s);

            for (int i = 0; i < size / sizeof(uint32_t); i++) {
                g_sum += ptr[i];
            }
            printf("sum=%d\n", g_sum);
            free(ptr);
        }
    }
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

#define EPT_M2R_ADDR(addr) (addr + VRING_SIZE)  // covert master endpoint number to remote endpoint number
#define EPT_R2M_ADDR(addr) (addr - VRING_SIZE)  // covert remote endpoint number to master endpoint number

struct rpmsg_block_t {
    uint32_t len;
    uint8_t buffer[32 - 4];
};

struct rpmsg_ept_map_t {
    uint32_t base;          // share memory base addr
    uint32_t size;          // share memory size
    uint32_t m_ept_addr;    // master endpoint number
    uint32_t r_ept_addr;    // remote endpoint number
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

// define endpoint number for test
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

    if (src == info->map->r_ept_addr) {     // check source ept addr
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
        info->instance = rpmsg_lite_master_init((void *)info->map->base, info->map->size,
                                                RL_PLATFORM_SET_LINK_ID(master_id, remote_id), RL_NO_FLAGS);
        rpmsg_lite_wait_for_link_up(info->instance);
        info->ept = rpmsg_lite_create_ept(info->instance, info->map->m_ept_addr, master_ept_cb, info);

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
            rpmsg_lite_send(info->instance, info->ept, info->map->r_ept_addr, block.buffer, block.len, RL_BLOCK);

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
        rk_printf("rpmsg_master_send: master[%d]-->remote[%d], remote ept addr = 0x%08x\n", master_id, remote_id, info->map->r_ept_addr);
        rpmsg_lite_send(info->instance, info->ept, info->map->r_ept_addr, block.buffer, block.len, RL_BLOCK);
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
                rk_printf("rpmsg_master_recv: master[%d]<--remote[%d], remote ept addr = 0x%08x\n", master_id, remote_id, info->map->r_ept_addr);
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
                rk_printf("rpmsg_master_recv: master[%d]<--remote[%d], remote ept addr = 0x%08x\n", master_id, remote_id, info->map->r_ept_addr);
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
                rk_printf("rpmsg_master_recv: master[%d]<--remote[%d], remote ept addr = 0x%08x\n", master_id, remote_id, info->map->r_ept_addr);
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

    if (src == info->map->m_ept_addr) {      // check source ept addr
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
    info->instance = rpmsg_lite_remote_init((void *)info->map->base,
                                            RL_PLATFORM_SET_LINK_ID(master_id, remote_id), RL_NO_FLAGS);
    rpmsg_lite_wait_for_link_up(info->instance);
    info->ept = rpmsg_lite_create_ept(info->instance, info->map->r_ept_addr, remote_ept_cb, info);

    while (1) {
        if (info->cb_sta == 1) {
            info->cb_sta = 0;

            block = (struct rpmsg_block_t *)info->private;

            // CMD(ACK): RPMSG_CMD_PROB
            if (block->buffer[0] == RPMSG_CMD_PROB) {
                block->buffer[0] = RPMSG_ACK_PROB;
                block->buffer[1] = remote_id;
                block->len = 2;
                rpmsg_lite_send(info->instance, info->ept, info->map->m_ept_addr, block->buffer, block->len, RL_BLOCK);
            }
            // CMD(ACK): RPMSG_CMD_TEST
            else if (block->buffer[0] == RPMSG_CMD_TEST) {
                rk_printf("rpmsg_remote_recv: remote[%d]<--master[%d], master ept addr = 0x%08x\n", remote_id, block->buffer[1], info->map->m_ept_addr);

                block->buffer[0] = RPMSG_ACK_TEST;
                block->buffer[1] = remote_id;

                i = rpmsg_get_remote_index(remote_id);
                block->buffer[i + 2] |= 0xaa;

                rk_printf("rpmsg_remote_send: remote[%d]-->master[%d], master ept addr = 0x%08x\n", remote_id, master_id, info->map->m_ept_addr);
                rpmsg_lite_send(info->instance, info->ept, info->map->m_ept_addr, block->buffer, block->len, RL_BLOCK);
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

#ifdef CPU_USAGE_TEST
struct timer_info {
    struct TIMER_REG *timer;
    uint32_t irq;
};

struct timer_info g_timer_info[4] = {
    { TIMER0, TIMER0_IRQn },
    { TIMER1, TIMER1_IRQn },
    { TIMER2, TIMER2_IRQn },
    { TIMER3, TIMER3_IRQn },
};
static void usage_isr(int vector, void *param)
{
    struct timer_info *info = (struct timer_info *)param;
    uint32_t cpu_id;
    uint64_t t1, t2;

    cpu_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();
    t1 = HAL_GetSysTimerCount();
    HAL_CPUDelayUs((cpu_id + 1) * 100000);
    t2 = HAL_GetSysTimerCount();
    rk_printf("cpu:%ld, irq: %d, HAL_GetCPUUsage: %ld, t=%lld\n", cpu_id, vector, HAL_GetCPUUsage(), t2 - t1);

    HAL_TIMER_ClrInt(info->timer);
    HAL_GIC_EndOfInterrupt(info->irq);
}

static void usage_test(void)
{
    uint32_t cpu_id;

    cpu_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();

    printf("cpu: %ld, usage_test\n", cpu_id);
    HAL_IRQ_HANDLER_SetIRQHandler(g_timer_info[cpu_id].irq, usage_isr, &g_timer_info[cpu_id]);
    HAL_GIC_Enable(g_timer_info[cpu_id].irq);
    HAL_TIMER_Init(g_timer_info[cpu_id].timer, TIMER_FREE_RUNNING);
    HAL_TIMER_SetCount(g_timer_info[cpu_id].timer, PLL_INPUT_OSC_RATE);
    HAL_TIMER_Start_IT(g_timer_info[cpu_id].timer);
}
#endif  /* CPU_USAGE_TEST */

#ifdef HAL_I2C_MODULE_ENABLED
static void HAL_IOMUX_I2C0M0Config(void)
{
    /* I2C0 */
    HAL_PINCTRL_SetIOMUX(GPIO_BANK0,
                         GPIO_PIN_B0 |        // i2c0_scl
                         GPIO_PIN_B1,         // i2c0_sda
                         PIN_CONFIG_MUX_FUNC1);
}
#endif

#ifdef HAL_SDIO_MODULE_ENABLED
static void HAL_IOMUX_MMCM0Config(void)
{
    /* MMC */
    HAL_PINCTRL_SetIOMUX(GPIO_BANK1,
                         GPIO_PIN_A0 |        // EMMC_D0
                         GPIO_PIN_A1 | // EMMC_D1
                         GPIO_PIN_A2 | // EMMC_D2
                         GPIO_PIN_A3 | // EMMC_D3
                         GPIO_PIN_A4 | // EMMC_D4
                         GPIO_PIN_A5 | // EMMC_D5
                         GPIO_PIN_A6 | // EMMC_D6
                         GPIO_PIN_A7 | // EMMC_D7
                         GPIO_PIN_B1 | // EMMC_CLKOUT
                         GPIO_PIN_B2 | // EMMC_CMD
                         GPIO_PIN_B3, // EMMC_RSTN
                         PIN_CONFIG_MUX_FUNC2);
}
#endif

#if defined(UNITY_TEST) && defined(PRIMARY_CPU)
static void unity_test_iomux_config(void)
{
    HAL_IOMUX_I2C0M0Config();
    HAL_IOMUX_MMCM0Config();
}
#endif

/********************* Public Function Definition ****************************/

void TEST_DEMO_GIC_Init(void)
{
    HAL_GIC_Init(&irqConfig);
}

void test_demo(void)
{
#ifdef RPMSG_TEST
#ifdef PRIMARY_CPU
    rpmsg_master_test();
#else
    rpmsg_remote_test();
#endif
#endif

#ifdef SPINLOCK_TEST
    spinlock_test();
#endif

#if defined(SOFTIRQ_TEST) && defined(PRIMARY_CPU)
    softirq_test();
#endif

#if defined(FAULTDBG_TEST) && defined(CPU0)
    fault_dbg_test();
#endif

#ifdef TIMER_TEST
    timer_test();
#endif

#ifdef TSADC_TEST
    tsadc_test();
#endif

#ifdef GPIO_TEST
    gpio_test();
#endif

#if defined(UART_TEST) && defined(PRIMARY_CPU)
    uart_test();
#endif

#if defined(DMA_LINK_LIST_TEST) && defined(PRIMARY_CPU)
    dmalinklist_test();
#endif

#ifdef PERF_TEST && defined(PRIMARY_CPU)
    perf_test();
#endif

#if defined(UNITY_TEST) && defined(PRIMARY_CPU)
    /* Unity Test */
    unity_test_iomux_config();
    test_main();
#endif

#if defined(CPU_USAGE_TEST)
    usage_test();
#endif

#if defined(SOFTRST_TEST) && defined(PRIMARY_CPU)
    HAL_DelayMs(10);
    softrst_test(SOFT_SRST_DIRECT);
#endif
}
