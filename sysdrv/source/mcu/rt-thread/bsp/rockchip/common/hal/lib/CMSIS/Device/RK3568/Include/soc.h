/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2021 Rockchip Electronics Co., Ltd.
 */

#ifndef __SOC_H
#define __SOC_H
#ifdef __cplusplus
extern "C" {
#endif

#include "hal_conf.h"

/* IO definitions (access restrictions to peripheral registers) */
#ifdef __cplusplus
#define     __I     volatile             /*!< brief Defines 'read only' permissions */
#else
#define     __I     volatile const       /*!< brief Defines 'read only' permissions */
#endif
#define     __O     volatile             /*!< brief Defines 'write only' permissions */
#define     __IO    volatile             /*!< brief Defines 'read / write' permissions */

/* ================================================================================ */
/* ================                    DMA REQ                      =============== */
/* ================================================================================ */
typedef enum {
    DMA_REQ_UART0_TX = 0,
    DMA_REQ_UART0_RX = 1,
    DMA_REQ_UART1_TX = 2,
    DMA_REQ_UART1_RX = 3,
    DMA_REQ_UART2_TX = 4,
    DMA_REQ_UART2_RX = 5,
    DMA_REQ_UART3_TX = 6,
    DMA_REQ_UART3_RX = 7,
    DMA_REQ_UART4_TX = 8,
    DMA_REQ_UART4_RX = 9,
    DMA_REQ_UART5_TX = 10,
    DMA_REQ_UART5_RX = 11,
    DMA_REQ_UART6_TX = 12,
    DMA_REQ_UART6_RX = 13,
    DMA_REQ_UART7_TX = 14,
    DMA_REQ_UART7_RX = 15,
    DMA_REQ_UART8_TX = 16,
    DMA_REQ_UART8_RX = 17,
    DMA_REQ_UART9_TX = 18,
    DMA_REQ_UART9_RX = 19,
    DMA_REQ_SPI0_TX = 20,
    DMA_REQ_SPI0_RX = 21,
    DMA_REQ_SPI1_TX = 22,
    DMA_REQ_SPI1_RX = 23,
    DMA_REQ_SPI2_TX = 24,
    DMA_REQ_SPI2_RX = 25,
    DMA_REQ_SPI3_TX = 26,
    DMA_REQ_SPI3_RX = 27,
} DMA_REQ_Type;

/* ================================================================================ */
/* ================                       IRQ                      ================ */
/* ================================================================================ */
#ifdef HAL_MCU_CORE
#define INTMUX_IRQ_START_NUM      32
#define INTMUX_OUT_IRQ_START_NUM  0
#define INTMUX_NUM_INT_PER_OUT    64
#define INTMUX_NUM_GROUP_PER_CON  8
#define INTMUX_NUM_GROUP_PER_OUT  8
#define INTMUX_NUM_INT_PER_GROUP  8
#define NUM_EXT_INTERRUPTS        320

typedef enum
{
/******  Platform Exceptions Numbers ***************************************************/
    NUM_INTERRUPTS         = 0,
    DMAC0_ABORT_IRQn       = 45,       /*!< DMAC0 Abort Interrupt       */
    DMAC0_IRQn             = 46,       /*!< DMAC0 Interrupt             */
    DMAC1_ABORT_IRQn       = 47,       /*!< DMAC1 Abort Interrupt       */
    DMAC1_IRQn             = 48,       /*!< DMAC1 Interrupt             */
    GPIO0_IRQn             = 65,       /*!< GPIO0 Interrupt             */
    GPIO1_IRQn             = 66,       /*!< GPIO1 Interrupt             */
    GPIO2_IRQn             = 67,       /*!< GPIO2 Interrupt             */
    GPIO3_IRQn             = 68,       /*!< GPIO3 Interrupt             */
    GPIO4_IRQn             = 69,       /*!< GPIO4 Interrupt             */
    I2C0_IRQn              = 78,       /*!< I2C0 Interrupt              */
    I2C1_IRQn              = 79,       /*!< I2C1 Interrupt              */
    I2C2_IRQn              = 80,       /*!< I2C2 Interrupt              */
    I2C3_IRQn              = 81,       /*!< I2C3 Interrupt              */
    I2C4_IRQn              = 82,       /*!< I2C4 Interrupt              */
    I2C5_IRQn              = 83,       /*!< I2C5 Interrupt              */
    PWM_PMU_IRQn           = 114,      /*!< PWM_PMU Interrupt           */
    PWM1_IRQn              = 115,      /*!< PWM1 Interrupt              */
    PWM2_IRQn              = 116,      /*!< PWM2 Interrupt              */
    PWM3_IRQn              = 117,      /*!< PWM3 Interrupt              */
    PWM_PMU_PWR_IRQn       = 118,      /*!< PWM_PMU PWR Interrupt       */
    PWM1_PWR_IRQn          = 119,      /*!< PWM1 PWR Interrupt          */
    PWM2_PWR_IRQn          = 120,      /*!< PWM2 PWR Interrupt          */
    PWM3_PWR_IRQn          = 121,      /*!< PWM3 PWR Interrupt          */
    SARADC_IRQn            = 125,       /*!< SARADC Interrupt            */
    FSPI0_IRQn             = 133,      /*!< FSPI Interrupt              */
    SPI0_IRQn              = 135,      /*!< SPI0 Interrupt              */
    SPI1_IRQn              = 136,      /*!< SPI1 Interrupt              */
    SPI2_IRQn              = 137,      /*!< SPI2 Interrupt              */
    SPI3_IRQn              = 138,      /*!< SPI3 Interrupt              */
    TIMER0_IRQn            = 141,      /*!< TIMER0 Interrupt            */
    TIMER1_IRQn            = 142,      /*!< TIMER1 Interrupt            */
    TIMER2_IRQn            = 143,      /*!< TIMER2 Interrupt            */
    TIMER3_IRQn            = 144,      /*!< TIMER3 Interrupt            */
    TIMER4_IRQn            = 145,      /*!< TIMER4 Interrupt            */
    TIMER5_IRQn            = 146,      /*!< TIMER5 Interrupt            */
    UART0_IRQn             = 148,      /*!< UART0  Interrupt            */
    UART1_IRQn             = 149,      /*!< UART1  Interrupt            */
    UART2_IRQn             = 150,      /*!< UART2  Interrupt            */
    UART3_IRQn             = 151,      /*!< UART3  Interrupt            */
    UART4_IRQn             = 152,      /*!< UART4  Interrupt            */
    UART5_IRQn             = 153,      /*!< UART5  Interrupt            */
    UART6_IRQn             = 154,      /*!< UART6  Interrupt            */
    UART7_IRQn             = 155,      /*!< UART7  Interrupt            */
    UART8_IRQn             = 156,      /*!< UART8  Interrupt            */
    UART9_IRQn             = 157,      /*!< UART9  Interrupt            */
    WDT0_IRQn              = 181,      /*!< WDT0  Interrupt             */
    MBOX0_CH0_B2A_IRQn     = 215,      /*!< MBOX0 CH0 B2A Interrupt     */
    MBOX0_CH1_B2A_IRQn     = 216,      /*!< MBOX0 CH1 B2A Interrupt     */
    MBOX0_CH2_B2A_IRQn     = 217,      /*!< MBOX0 CH2 B2A Interrupt     */
    MBOX0_CH3_B2A_IRQn     = 218,      /*!< MBOX0 CH3 B2A Interrupt     */
    MBOX0_CH0_A2B_IRQn     = 219,      /*!< MBOX0 CH0 A2B Interrupt     */
    MBOX0_CH1_A2B_IRQn     = 220,      /*!< MBOX0 CH1 A2B Interrupt     */
    MBOX0_CH2_A2B_IRQn     = 221,      /*!< MBOX0 CH2 A2B Interrupt     */
    MBOX0_CH3_A2B_IRQn     = 222,      /*!< MBOX0 CH3 A2B Interrupt     */
    TOTAL_INTERRUPTS       = (INTMUX_IRQ_START_NUM + NUM_INTERRUPTS + NUM_EXT_INTERRUPTS),
} IRQn_Type;
#else
typedef enum
{
/* When IPI_SGIs are used in AMP mode, you need to pay attention to whether it conflicts
 * with SMP mode. Especially in the case of Linux OS as The Master Core.
 * IPI_SGI 0~7 for non-secure and IPI_SGI 8~15 for secure.
 */
    IPI_SGI0               = 0,
    IPI_SGI1               = 1,
    IPI_SGI2               = 2,
    IPI_SGI3               = 3,
    IPI_SGI4               = 4,
    IPI_SGI5               = 5,
    IPI_SGI6               = 6,
    IPI_SGI7               = 7,
    IPI_SGI8               = 8,
    IPI_SGI9               = 9,
    IPI_SGI10              = 10,
    IPI_SGI11              = 11,
    IPI_SGI12              = 12,
    IPI_SGI13              = 13,
    IPI_SGI14              = 14,
    IPI_SGI15              = 15,

    CNTHP_IRQn             = 26,
    CNTV_IRQn              = 27,
    CNTPS_IRQn             = 29,
    CNTPNS_IRQn            = 30,

/******  Platform Exceptions Numbers ***************************************************/
    CAN0_IRQn              = 33,       /*!< CAN0 Interrupt              */
    CAN1_IRQn              = 34,       /*!< CAN1 Interrupt              */
    CAN2_IRQn              = 35,       /*!< CAN2 Interrupt              */
    DMAC0_ABORT_IRQn       = 45,       /*!< DMAC0 Abort Interrupt       */
    DMAC0_IRQn             = 46,       /*!< DMAC0 Interrupt             */
    DMAC1_ABORT_IRQn       = 47,       /*!< DMAC1 Abort Interrupt       */
    DMAC1_IRQn             = 48,       /*!< DMAC1 Interrupt             */
    GMAC0_IRQn             = 59,       /*!< GMAC0 Interrupt             */
    GMAC1_IRQn             = 64,       /*!< GMAC1 Interrupt             */
    GPIO0_IRQn             = 65,       /*!< GPIO0 Interrupt             */
    GPIO1_IRQn             = 66,       /*!< GPIO1 Interrupt             */
    GPIO2_IRQn             = 67,       /*!< GPIO2 Interrupt             */
    GPIO3_IRQn             = 68,       /*!< GPIO3 Interrupt             */
    GPIO4_IRQn             = 69,       /*!< GPIO4 Interrupt             */
    I2C0_IRQn              = 78,       /*!< I2C0 Interrupt              */
    I2C1_IRQn              = 79,       /*!< I2C1 Interrupt              */
    I2C2_IRQn              = 80,       /*!< I2C2 Interrupt              */
    I2C3_IRQn              = 81,       /*!< I2C3 Interrupt              */
    I2C4_IRQn              = 82,       /*!< I2C4 Interrupt              */
    I2C5_IRQn              = 83,       /*!< I2C5 Interrupt              */
    PWM_PMU_IRQn           = 114,      /*!< PWM_PMU Interrupt           */
    PWM1_IRQn              = 115,      /*!< PWM1 Interrupt              */
    PWM2_IRQn              = 116,      /*!< PWM2 Interrupt              */
    PWM3_IRQn              = 117,      /*!< PWM3 Interrupt              */
    PWM_PMU_PWR_IRQn       = 118,      /*!< PWM_PMU PWR Interrupt       */
    PWM1_PWR_IRQn          = 119,      /*!< PWM1 PWR Interrupt          */
    PWM2_PWR_IRQn          = 120,      /*!< PWM2 PWR Interrupt          */
    PWM3_PWR_IRQn          = 121,      /*!< PWM3 PWR Interrupt          */
    SARADC_IRQn            = 125,      /*!< SARADC Interrupt            */
    FSPI0_IRQn             = 133,      /*!< FSPI Interrupt              */
    SPI0_IRQn              = 135,      /*!< SPI0 Interrupt              */
    SPI1_IRQn              = 136,      /*!< SPI1 Interrupt              */
    SPI2_IRQn              = 137,      /*!< SPI2 Interrupt              */
    SPI3_IRQn              = 138,      /*!< SPI3 Interrupt              */
    TIMER0_IRQn            = 141,      /*!< TIMER0 Interrupt            */
    TIMER1_IRQn            = 142,      /*!< TIMER1 Interrupt            */
    TIMER2_IRQn            = 143,      /*!< TIMER2 Interrupt            */
    TIMER3_IRQn            = 144,      /*!< TIMER3 Interrupt            */
    TIMER4_IRQn            = 145,      /*!< TIMER4 Interrupt            */
    TIMER5_IRQn            = 146,      /*!< TIMER5 Interrupt            */
    UART0_IRQn             = 148,      /*!< UART0  Interrupt            */
    UART1_IRQn             = 149,      /*!< UART1  Interrupt            */
    UART2_IRQn             = 150,      /*!< UART2  Interrupt            */
    UART3_IRQn             = 151,      /*!< UART3  Interrupt            */
    UART4_IRQn             = 152,      /*!< UART4  Interrupt            */
    UART5_IRQn             = 153,      /*!< UART5  Interrupt            */
    UART6_IRQn             = 154,      /*!< UART6  Interrupt            */
    UART7_IRQn             = 155,      /*!< UART7  Interrupt            */
    UART8_IRQn             = 156,      /*!< UART8  Interrupt            */
    UART9_IRQn             = 157,      /*!< UART9  Interrupt            */
    WDT0_IRQn              = 181,      /*!< WDT0  Interrupt             */
    PCIE30x2_LEGACY_IRQn   = 194,      /*!< PCIe3x2_legacy Interrupt    */
    DDR_ECC_CE_IRQn        = 205,      /*!< DDR ECC correctable fault Interrupt */
    DDR_ECC_UE_IRQn        = 207,      /*!< DDR ECC uncorrectable fault Interrupt */
    MBOX0_CH0_B2A_IRQn     = 215,      /*!< MBOX0 CH0 B2A Interrupt     */
    MBOX0_CH1_B2A_IRQn     = 216,      /*!< MBOX0 CH1 B2A Interrupt     */
    MBOX0_CH2_B2A_IRQn     = 217,      /*!< MBOX0 CH2 B2A Interrupt     */
    MBOX0_CH3_B2A_IRQn     = 218,      /*!< MBOX0 CH3 B2A Interrupt     */
    MBOX0_CH0_A2B_IRQn     = 219,      /*!< MBOX0 CH0 A2B Interrupt     */
    MBOX0_CH1_A2B_IRQn     = 220,      /*!< MBOX0 CH1 A2B Interrupt     */
    MBOX0_CH2_A2B_IRQn     = 221,      /*!< MBOX0 CH2 A2B Interrupt     */
    MBOX0_CH3_A2B_IRQn     = 222,      /*!< MBOX0 CH3 A2B Interrupt     */
    NFAULT0_IRQn           = 272,      /*!< DSU L3 CACHE ECC FAULT Interrupt */
    NFAULT1_IRQn           = 273,      /*!< CPU0 L1-L2 CACHE ECC FAULT Interrupt */
    NFAULT2_IRQn           = 274,      /*!< CPU1 L1-L2 CACHE ECC FAULT Interrupt */
    NFAULT3_IRQn           = 275,      /*!< CPU2 L1-L2 CACHE ECC FAULT Interrupt */
    NFAULT4_IRQn           = 276,      /*!< CPU3 L1-L2 CACHE ECC FAULT Interrupt */
    NERR0_IRQn             = 277,      /*!< DSU L3 CACHE ECC ERROR Interrupt */
    NERR1_IRQn             = 278,      /*!< CPU0 L1-L2 CACHE ECC ERROR Interrupt */
    NERR2_IRQn             = 279,      /*!< CPU1 L1-L2 CACHE ECC ERROR Interrupt */
    NERR3_IRQn             = 280,      /*!< CPU2 L1-L2 CACHE ECC ERROR Interrupt */
    NERR4_IRQn             = 281,      /*!< CPU3 L1-L2 CACHE ECC ERROR Interrupt */
    RSVD0_IRQn             = 283,      /*!< RSVD0  Interrupt            */
    RPMSG_01_IRQn          = 285,      /*!< RPMSG 0->1 Interrupt        */
    RPMSG_02_IRQn          = 286,      /*!< RPMSG 0->2 Interrupt        */
    RPMSG_03_IRQn          = 287,      /*!< RPMSG 0->3 Interrupt        */
    RPMSG_10_IRQn          = 288,      /*!< RPMSG 1->0 Interrupt        */
    RPMSG_12_IRQn          = 289,      /*!< RPMSG 1->2 Interrupt        */
    RPMSG_13_IRQn          = 290,      /*!< RPMSG 1->3 Interrupt        */
    RPMSG_20_IRQn          = 291,      /*!< RPMSG 2->0 Interrupt        */
    RPMSG_21_IRQn          = 292,      /*!< RPMSG 2->1 Interrupt        */
    RPMSG_23_IRQn          = 293,      /*!< RPMSG 2->3 Interrupt        */
    RPMSG_30_IRQn          = 294,      /*!< RPMSG 3->0 Interrupt        */
    RPMSG_31_IRQn          = 295,      /*!< RPMSG 3->1 Interrupt        */
    RPMSG_32_IRQn          = 296,      /*!< RPMSG 3->2 Interrupt        */
    NUM_INTERRUPTS         = 352,
} IRQn_Type;
#endif

#define RSVD_IRQn(_N)               (RSVD0_IRQn + (_N))

#define AMP_CPUOFF_REQ_IRQ(cpu)     RSVD_IRQn(11 + (cpu)) /* gic irq: 294 */
#define GIC_TOUCH_REQ_IRQ(cpu)      (AMP_CPUOFF_REQ_IRQ(4) + cpu) /* gic irq: 298 */

#define GPIO_IRQ_GROUP_DIRQ_BASE    RSVD_IRQn(37) /* gic irq: 320 */
#define GPIO_IRQ_GROUP_DIRQ_NUM     (NUM_INTERRUPTS - GPIO_IRQ_GROUP_DIRQ_BASE)

#define GPIO_IRQ_GROUP_GPIO0_HWIRQ  GPIO0_IRQn
#define GPIO_IRQ_GROUP_GPION_HWIRQ  GPIO4_IRQn

/* ================================================================================ */
/* ================      Processor and Core Peripheral Section     ================ */
/* ================================================================================ */

#if defined(HAL_AP_CORE) && defined(HAL_MCU_CORE)
#error "HAL_AP_CORE and HAL_MCU_CORE only one of them can be enabled"
#endif

#if !defined(HAL_AP_CORE) && !defined(HAL_MCU_CORE)
#error "Please define HAL_AP_CORE or HAL_MCU_CORE on hal_conf.h"
#endif

#ifdef HAL_AP_CORE
#define __CORTEX_A           55U          /* Cortex-A55 Core                          */
#define __FPU_PRESENT         1U          /* FPU present                              */
#define __TIM_PRESENT         1U          /* Generic Timer                            */

#define CACHE_LINE_SHIFT    (6U)
#define CACHE_LINE_SIZE     (0x1U << CACHE_LINE_SHIFT)

#else
#define __RISC_V
#endif

#ifndef __ASSEMBLY__
#include "cmsis_compiler.h"               /* CMSIS compiler specific defines */
#ifdef __CORTEX_A
#include "core_ca.h"
#endif
#include "system_rk3568.h"
#endif /* __ASSEMBLY__ */
#include "rk3568.h"

/****************************************************************************************/
/*                                                                                      */
/*                                Module Address Section                                */
/*                                                                                      */
/****************************************************************************************/
/* Memory Base */
#define PCIE3X2_DBI_BASE        0xF6000000 /* PCIe dbi base address */
#define GIC_DISTRIBUTOR_BASE    0xFD400000 /* GICD base address */
#define GIC_REDISTRIBUTOR_BASE  0xFD460000 /* GICR base address */
#define PCIE3PHY_GRF_BASE       0xFDCB8000 /* S-PHY_GRF base address */
#define PCIE3X2_APB_BASE        0xFE280000 /* PCIe apb base address */

/****************************************************************************************/
/*                                                                                      */
/*                               Register Bitmap Section                                */
/*                                                                                      */
/****************************************************************************************/
/********************************** CPU Topology ****************************************/
#define MPIDR_MT_MASK       ((1U) << 24)
#define MPIDR_AFFLVL_MASK   (0xFFU)
#define MPIDR_AFF0_SHIFT    (0U)
#define MPIDR_AFF1_SHIFT    (8U)
#define MPIDR_AFF2_SHIFT    (16U)
#define MPIDR_AFF3_SHIFT    (32U)
#define MPIDR_AFFINITY_MASK (0xFFFFFFU)
#define PLATFORM_CLUSTER0_CORE_COUNT (4)
#define PLATFORM_CLUSTER1_CORE_COUNT (0)
#define PLATFORM_CORE_COUNT PLATFORM_CLUSTER0_CORE_COUNT
#define CPU_GET_AFFINITY(cpuId, clusterId) ((cpuId) << MPIDR_AFF1_SHIFT)

/********************************** CACHE ECC ****************************************/
#ifndef __ASSEMBLY__
/** @addtogroup CACHE_ECC_Exported_Definition_Group1
 *  @{
 */
/** the fault value will be injected */
typedef enum {
    CACHE_ECC_INJECT_UC     = 0x80000002, /**< Uncontainable Error generation */
    CACHE_ECC_INJECT_DE_UER = 0x8000000a, /**< for l1~l2, generate a exception */
} eCACHE_ECC_InjectFault;
/** @} */
#endif
/******************************************CRU*******************************************/
#define PLL_INPUT_OSC_RATE (24 * 1000 * 1000)

#define CRU_SRST_CON_CNT    29
#define CRU_GATE_CON_CNT    35
#define CRU_CLK_DIV_CON_CNT 84
#define CRU_CLK_SEL_CON_CNT 84

#define CLK(mux, div) \
    (((mux) & 0x0F0F00FFU) | (((div) & 0xFFU) << 8) | (((div) & 0x0F0F0000U) << 4))

#ifndef __ASSEMBLY__
typedef enum CLOCK_Name {
    CLK_INVALID = 0U,
    PLL_APLL,
    PLL_CPLL,
    PLL_GPLL,
    PLL_NPLL,
    PLL_VPLL,
    PLL_PPLL,
    PLL_HPLL,
    CLK_WDT,
    CLK_UART0,
    CLK_I2C        = CLK(CLK_I2C_SEL, 0U),
    CLK_PWM1       = CLK(CLK_PWM1_SEL, 0U),
    CLK_PWM2       = CLK(CLK_PWM2_SEL, 0U),
    CLK_PWM3       = CLK(CLK_PWM3_SEL, 0U),
    CLK_SPI0       = CLK(CLK_SPI0_SEL, 0U),
    CLK_SPI1       = CLK(CLK_SPI1_SEL, 0U),
    CLK_SPI2       = CLK(CLK_SPI2_SEL, 0U),
    CLK_SPI3       = CLK(CLK_SPI3_SEL, 0U),
    CLK_UART1_SRC  = CLK(CLK_UART1_SRC_SEL, CLK_UART1_SRC_DIV),
    CLK_UART1_FRAC = CLK(0U, CLK_UART1_FRAC_DIV),
    CLK_UART1      = CLK(SCLK_UART1_SEL, 0U),
    CLK_UART2_SRC  = CLK(CLK_UART2_SRC_SEL, CLK_UART2_SRC_DIV),
    CLK_UART2_FRAC = CLK(0U, CLK_UART2_FRAC_DIV),
    CLK_UART2      = CLK(SCLK_UART2_SEL, 0U),
    CLK_UART3_SRC  = CLK(CLK_UART3_SRC_SEL, CLK_UART3_SRC_DIV),
    CLK_UART3_FRAC = CLK(0U, CLK_UART3_FRAC_DIV),
    CLK_UART3      = CLK(SCLK_UART3_SEL, 0U),
    CLK_UART4_SRC  = CLK(CLK_UART4_SRC_SEL, CLK_UART4_SRC_DIV),
    CLK_UART4_FRAC = CLK(0U, CLK_UART4_FRAC_DIV),
    CLK_UART4      = CLK(SCLK_UART4_SEL, 0U),
    CLK_UART5_SRC  = CLK(CLK_UART5_SRC_SEL, CLK_UART5_SRC_DIV),
    CLK_UART5_FRAC = CLK(0U, CLK_UART5_FRAC_DIV),
    CLK_UART5      = CLK(SCLK_UART5_SEL, 0U),
    CLK_UART6_SRC  = CLK(CLK_UART6_SRC_SEL, CLK_UART6_SRC_DIV),
    CLK_UART6_FRAC = CLK(0U, CLK_UART6_FRAC_DIV),
    CLK_UART6      = CLK(SCLK_UART6_SEL, 0U),
    CLK_UART7_SRC  = CLK(CLK_UART7_SRC_SEL, CLK_UART7_SRC_DIV),
    CLK_UART7_FRAC = CLK(0U, CLK_UART7_FRAC_DIV),
    CLK_UART7      = CLK(SCLK_UART7_SEL, 0U),
    CLK_UART8_SRC  = CLK(CLK_UART8_SRC_SEL, CLK_UART8_SRC_DIV),
    CLK_UART8_FRAC = CLK(0U, CLK_UART8_FRAC_DIV),
    CLK_UART8      = CLK(SCLK_UART8_SEL, 0U),
    CLK_UART9_SRC  = CLK(CLK_UART9_SRC_SEL, CLK_UART9_SRC_DIV),
    CLK_UART9_FRAC = CLK(0U, CLK_UART9_FRAC_DIV),
    CLK_UART9      = CLK(SCLK_UART9_SEL, 0U),
    CLK_CAN0       = CLK(CLK_CAN0_SEL, CLK_CAN0_DIV),
    CLK_CAN1       = CLK(CLK_CAN1_SEL, CLK_CAN1_DIV),
    CLK_CAN2       = CLK(CLK_CAN2_SEL, CLK_CAN2_DIV),
    CLK_TSADC_TSEN = CLK(CLK_TSADC_TSEN_SEL, CLK_TSADC_TSEN_DIV),
    CLK_TSADC      = CLK(0, CLK_TSADC_DIV),
    SCLK_SFC       = CLK(SCLK_SFC_SEL, 0U),
    CLK_MAC0_2TOP          = CLK(CLK_MAC0_2TOP_SEL, 0U),
    CLK_MAC1_2TOP          = CLK(CLK_MAC1_2TOP_SEL, 0U),
    CLK_MAC0_OUT           = CLK(CLK_MAC0_OUT_SEL, 0U),
    CLK_MAC1_OUT           = CLK(CLK_MAC1_OUT_SEL, 0U),
    CLK_GMAC0_PTP_REF      = CLK(CLK_GMAC0_PTP_REF_SEL, 0U),
    CLK_GMAC1_PTP_REF      = CLK(CLK_GMAC1_PTP_REF_SEL, 0U),
    SCLK_GMAC0             = CLK(RMII0_EXTCLK_SEL, 0U),
    SCLK_GMAC1             = CLK(RMII1_EXTCLK_SEL, 0U),
    SCLK_GMAC0_RGMII_SPEED = CLK(RGMII0_CLK_SEL, 0U),
    SCLK_GMAC1_RGMII_SPEED = CLK(RGMII1_CLK_SEL, 0U),
    SCLK_GMAC0_RMII_SPEED  = CLK(RMII0_CLK_SEL, 0U),
    SCLK_GMAC1_RMII_SPEED  = CLK(RMII1_CLK_SEL, 0U),
    SCLK_GMAC0_RX_TX       = CLK(CLK_GMAC0_RX_TX_SEL, 0U),
    SCLK_GMAC1_RX_TX       = CLK(CLK_GMAC1_RX_TX_SEL, 0U),
    ACLK_PHP               = CLK(ACLK_PHP_SEL, 0U),
    HCLK_PHP               = CLK(HCLK_PHP_SEL, 0U),
    PCLK_PHP               = CLK(0U, PCLK_PHP_DIV),
    ACLK_USB               = CLK(ACLK_USB_SEL, 0U),
    HCLK_USB               = CLK(HCLK_USB_SEL, 0U),
    PCLK_USB               = CLK(0U, PCLK_USB_DIV),
} eCLOCK_Name;
#endif
/****************************************MBOX********************************************/
#define MBOX_CNT             2
#define MBOX_CHAN_CNT        4
/****************************************GRF*********************************************/
#define GRF_IOMUX_BIT_PER_PIN	(4)
#define GRF_DS_BIT_PER_PIN	(8)
#define GRF_PULL_BIT_PER_PIN	(2)
/****************************************GPIO********************************************/
#ifdef GPIO_VER_ID
#undef GPIO_VER_ID
#define GPIO_VER_ID             (0x01000C2BU)
#endif
/****************************************PMU*********************************************/
#ifndef __ASSEMBLY__
typedef enum PD_Id {
    PD_INVALID = 0U,
} ePD_Id;
#endif
/****************************************FSPI********************************************/
#define FSPI_CHIP_CNT                            (2)

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __SOC_H */
