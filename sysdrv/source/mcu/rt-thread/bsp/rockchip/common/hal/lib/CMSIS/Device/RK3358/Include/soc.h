/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2023 Rockchip Electronics Co., Ltd.
 */

#ifndef __SOC_H
#define __SOC_H
#ifdef __cplusplus
extern "C" {
#endif

#include "hal_conf.h"

/* IO definitions (access restrictions to peripheral registers) */
#ifdef __cplusplus
  #define   __I volatile                 /*!< brief Defines 'read only' permissions */
#else
  #define   __I volatile const           /*!< brief Defines 'read only' permissions */
#endif
#define     __O  volatile                /*!< brief Defines 'write only' permissions */
#define     __IO volatile                /*!< brief Defines 'read / write' permissions */

/* ================================================================================ */
/* ================                    DMA REQ                      =============== */
/* ================================================================================ */
typedef enum {
    DMA_REQ_UART0_TX     = 0,
    DMA_REQ_UART0_RX     = 1,
    DMA_REQ_UART1_TX     = 2,
    DMA_REQ_UART1_RX     = 3,
    DMA_REQ_UART2_TX     = 4,
    DMA_REQ_UART2_RX     = 5,
    DMA_REQ_UART3_TX     = 6,
    DMA_REQ_UART3_RX     = 7,
    DMA_REQ_UART4_TX     = 8,
    DMA_REQ_UART4_RX     = 9,
    DMA_REQ_UART5_TX     = 10,
    DMA_REQ_UART5_RX     = 11,
    DMA_REQ_SPI0_TX      = 12,
    DMA_REQ_SPI0_RX      = 13,
    DMA_REQ_SPI1_TX      = 14,
    DMA_REQ_SPI1_RX      = 15,
    DMA_REQ_I2S_8CH_0_TX = 16,
    DMA_REQ_I2S_8CH_0_RX = 17,
    DMA_REQ_I2S_2CH_0_TX = 18,
    DMA_REQ_I2S_2CH_0_RX = 19,
    DMA_REQ_I2S_2CH_1_TX = 20,
    DMA_REQ_I2S_2CH_1_RX = 21,
    DMA_REQ_PWM0_RX      = 22,
    DMA_REQ_PWM1_RX      = 23,
} DMA_REQ_Type;

/* ================================================================================ */
/* ================                       IRQ                      ================ */
/* ================================================================================ */
typedef enum {
    CNTHP_IRQn  = 26,
    CNTV_IRQn   = 27,
    CNTPS_IRQn  = 29,
    CNTPNS_IRQn = 30,

/******  Platform Exceptions Numbers ***************************************************/
    DCF_IRQn         = 32,            /*!< DCF Interrupt               */
    DMAC0_IRQn       = 33,            /*!< DMAC0 Interrupt             */
    DMAC0_ABORT_IRQn = 34,            /*!< DMAC0 ABORT Interrupt       */
    GPIO0_IRQn       = 35,            /*!< GPIO0  Interrupt            */
    GPIO1_IRQn       = 36,            /*!< GPIO1  Interrupt            */
    GPIO2_IRQn       = 37,            /*!< GPIO2  Interrupt            */
    GPIO3_IRQn       = 38,            /*!< GPIO3 Interrupt             */
    I2C0_IRQn        = 39,            /*!< I2C0 Interrupt              */
    I2C1_IRQn        = 40,            /*!< I2C1 Interrupt              */
    I2C2_IRQn        = 41,            /*!< I2C2 Interrupt              */
    I2C3_IRQn        = 42,            /*!< I2C3 Interrupt              */
    I2C4_IRQn        = 43,            /*!< I2C4 Interrupt              */
    I2S_8CH_0_IRQn   = 44,            /*!< I2S_8CH_0 Interrupt         */
    I2S_2CH_0_IRQn   = 45,            /*!< I2S_2CH_0 Interrupt         */
    I2S_2CH_1_IRQn   = 46,            /*!< I2S_2CH_1 Interrupt         */
    UART0_IRQn       = 47,            /*!< UART0 Interrupt             */
    UART1_IRQn       = 48,            /*!< UART1 Interrupt             */
    UART2_IRQn       = 49,            /*!< UART2 Interrupt             */
    UART3_IRQn       = 50,            /*!< UART3 Interrupt             */
    UART4_IRQn       = 51,            /*!< UART4 Interrupt             */
    UART5_IRQn       = 52,            /*!< UART5 Interrupt             */
    OTPC_IRQn        = 53,            /*!< OTPC_NS Interrupt           */
    PDM0_IRQn        = 55,            /*!< PDM0 Interrupt              */
    PWM0_IRQn        = 56,            /*!< PWM0 Interrupt              */
    PWM1_IRQn        = 57,            /*!< PWM1 Interrupt              */
    SPI0_IRQn        = 58,            /*!< SPI0 Interrupt              */
    SPI1_IRQn        = 59,            /*!< SPI1 Interrupt              */
    TIMER0_IRQn      = 62,            /*!< TIMER0 Interrupt            */
    TIMER1_IRQn      = 63,            /*!< TIMER1 Interrupt            */
    TIMER2_IRQn      = 64,            /*!< TIMER2 Interrupt            */
    TIMER3_IRQn      = 65,            /*!< TIMER3 Interrupt            */
    TIMER4_IRQn      = 66,            /*!< TIMER4 Interrupt            */
    TIMER5_IRQn      = 67,            /*!< TIMER5 Interrupt            */
    TSADC_IRQn       = 68,            /*!< TSADC  Interrupt            */
    WDT0_IRQn        = 69,            /*!< WDT0 Interrupt              */
    GMAC0_IRQn       = 75,            /*!< GMAC0 Interrupt             */
    EMMC_IRQn        = 85,            /*!< EMMC Interrupt              */
    SDEMMC_IRQn      = 86,            /*!< SDEMMC Interrupt            */
    SDIO_IRQn        = 87,            /*!< SDIO Interrupt              */
    SFC_IRQn         = 88,            /*!< SFC Interrupt               */
    NANDC_IRQn       = 89,            /*!< NANDC Interrupt             */
    PMU_IRQn         = 90,            /*!< PMU Interrupt               */
    EHCI_IRQn        = 92,            /*!< EHCI Interrupt              */
    OHCI_IRQn        = 93,            /*!< OHCI Interrupt              */
    OTG_IRQn         = 94,            /*!< OTG Interrupt          */
    PWM0_PWR_IRQn    = 121,           /*!< PWM0 PWR Interrupt          */
    PWM1_PWR_IRQn    = 122,           /*!< PWM1 PWR Interrupt          */
                                      /*!< IRQn: 136-139 for cpu off in Trust */
    RPMSG_01_IRQn    = 140,           /*!< RPMSG 0->1 Interrupt        */
    RPMSG_02_IRQn    = 141,           /*!< RPMSG 0->2 Interrupt        */
    RPMSG_03_IRQn    = 142,           /*!< RPMSG 0->3 Interrupt        */
    RPMSG_10_IRQn    = 143,           /*!< RPMSG 1->0 Interrupt        */
    RPMSG_12_IRQn    = 144,           /*!< RPMSG 1->2 Interrupt        */
    RPMSG_13_IRQn    = 145,           /*!< RPMSG 1->3 Interrupt        */
    RPMSG_20_IRQn    = 146,           /*!< RPMSG 2->0 Interrupt        */
    RPMSG_21_IRQn    = 147,           /*!< RPMSG 2->1 Interrupt        */
    RPMSG_23_IRQn    = 148,           /*!< RPMSG 2->3 Interrupt        */
    RPMSG_30_IRQn    = 149,           /*!< RPMSG 3->0 Interrupt        */
    RPMSG_31_IRQn    = 150,           /*!< RPMSG 3->1 Interrupt        */
    RSVD0_IRQn       = 151,           /*!< RSVD0  Interrupt            */
    NUM_INTERRUPTS   = 160
} IRQn_Type;

#define AMP_CPUOFF_REQ_IRQ(cpu)  (cpu + 136)           /* gic irq: 136-139 */

/* ================================================================================ */
/* ================      Processor and Core Peripheral Section     ================ */
/* ================================================================================ */

#define __CORTEX_A    35U                 /* Cortex-A35 Core                          */
#define __FPU_PRESENT 1U                  /* FPU present                              */
#define HAL_GIC_V2    1U                  /* GIC version 2                            */
#define __TIM_PRESENT 1U                  /* Generic Timer                            */

#define CACHE_LINE_SHIFT (6U)
#define CACHE_LINE_SIZE  (0x1U << CACHE_LINE_SHIFT)

#ifndef __ASSEMBLY__
#include "cmsis_compiler.h"               /* CMSIS compiler specific defines */
#ifdef __CORTEX_A
#include "core_ca.h"
#endif
#include "system_rk3358.h"
#endif /* __ASSEMBLY__ */
#include "rk3358.h"
#include "rk3358_usb.h"

/****************************************************************************************/
/*                                                                                      */
/*                                Module Address Section                                */
/*                                                                                      */
/****************************************************************************************/
/* Memory Base */
#define GIC_DISTRIBUTOR_BASE   (0xFF131000)
#define GIC_CPU_INTERFACE_BASE (0xFF132000)

#define OTG_BASE               (0xFF300000U) /* DWC2 base address */
#define EHCI_BASE              (0xFF340000U) /* EHCI base address */
#define OHCI_BASE              (0xFF350000U) /* OHCI base address */
/****************************************************************************************/
/*                                                                                      */
/*                               Register Bitmap Section                                */
/*                                                                                      */
/****************************************************************************************/
/********************************** CPU Topology ****************************************/
#define MPIDR_MT_MASK                      ((1U) << 24)
#define MPIDR_AFFLVL_MASK                  (0xFFU)
#define MPIDR_AFF0_SHIFT                   (0U)
#define MPIDR_AFF1_SHIFT                   (8U)
#define MPIDR_AFF2_SHIFT                   (16U)
#define MPIDR_AFFINITY_MASK                (0xFFFFFFU)
#define PLATFORM_CLUSTER0_CORE_COUNT       (4)
#define PLATFORM_CLUSTER1_CORE_COUNT       (0)
#define PLATFORM_CORE_COUNT                PLATFORM_CLUSTER0_CORE_COUNT
#define CPU_GET_AFFINITY(cpuId, clusterId) ((cpuId) << MPIDR_AFF0_SHIFT)

/****************************************GPIO********************************************/
#ifdef GPIO_VER_ID
#undef GPIO_VER_ID
#define GPIO_VER_ID (0x3231322AU)
#endif
/****************************************PMU*********************************************/
#ifndef __ASSEMBLY__
typedef enum PD_Id {
    PD_INVALID = 0U,
} ePD_Id;
#endif
/****************************************SFC********************************************/
#define SFC_CHIP_CNT (1)

/******************************************CRU*******************************************/

#define PCLK_I2C0_GATE           PCLK_I2C0_CLK_GATE
#define PCLK_I2C1_GATE           PCLK_I2C1_CLK_GATE
#define PCLK_I2C2_GATE           PCLK_I2C2_CLK_GATE
#define PCLK_I2C3_GATE           PCLK_I2C3_CLK_GATE
#define PCLK_SARADC_CONTROL_GATE PCLK_SARADC_GATE
#define CLK_SARADC_GATE          CLK_SARADC_PLL_CLK_GATE
#define PCLK_SPI0_GATE           PCLK_SPI0_CLK_GATE
#define PCLK_SPI1_GATE           PCLK_SPI1_CLK_GATE
#define PCLK_UART0_GATE          PCLK_PMU_UART0_CLK_GATE
#define PCLK_UART1_GATE          PCLK_UART1_CLK_GATE
#define PCLK_UART2_GATE          PCLK_UART2_CLK_GATE
#define PCLK_UART3_GATE          PCLK_UART3_CLK_GATE
#define PCLK_UART4_GATE          PCLK_UART4_CLK_GATE
#define PCLK_UART5_GATE          PCLK_UART5_CLK_GATE
#define PCLK_WDT0_GATE           PCLK_WDT_NS_GATE

#define PLL_INPUT_OSC_RATE (24 * 1000 * 1000)

#define CRU_GATE_CON_CNT    18
#define CRU_CLK_DIV_CON_CNT 60
#define CRU_CLK_SEL_CON_CNT 60

#define CLK(mux, div) \
    (((mux) & 0x0F0F00FFU) | (((div) & 0xFFU) << 8) | (((div) & 0x0F0F0000U) << 4))

#ifndef __ASSEMBLY__
typedef enum CLOCK_Name {
    CLK_INVALID = 0U,
    PLL_APLL,
    PLL_CPLL,
    PLL_GPLL,
    PLL_NPLL,
    CLK_I2C0         = CLK(CLK_I2C0_PLL_SEL, CLK_I2C0_DIV),
    CLK_I2C1         = CLK(CLK_I2C1_PLL_SEL, CLK_I2C1_DIV),
    CLK_I2C2         = CLK(CLK_I2C2_PLL_SEL, CLK_I2C2_DIV),
    CLK_I2C3         = CLK(CLK_I2C3_PLL_SEL, CLK_I2C3_DIV),
    CLK_PWM0         = CLK(CLK_PWM0_PLL_SEL, CLK_PWM0_DIV),
    CLK_PWM1         = CLK(CLK_PWM1_PLL_SEL, CLK_PWM1_DIV),
    CLK_SPI0         = CLK(CLK_SPI0_PLL_SEL, CLK_SPI0_DIV),
    CLK_SPI1         = CLK(CLK_SPI1_PLL_SEL, CLK_SPI1_DIV),
    CLK_TSADC        = CLK(0U, CLK_TSADC_DIV),
    CLK_SARADC       = CLK(0U, CLK_SARADC_DIV),
    CLK_OTP          = CLK(0U, CLK_OTP_DIV),
    CLK_UART1_SRC    = CLK(CLK_UART1_PLL_SEL, CLK_UART1_DIV),
    CLK_UART1_FRAC   = CLK(0U, CLK_UART1_FRAC_DIV),
    CLK_UART1        = CLK(CLK_UART1_SEL, 0U),
    CLK_UART2_SRC    = CLK(CLK_UART2_PLL_SEL, CLK_UART2_DIV),
    CLK_UART2_FRAC   = CLK(0U, CLK_UART2_FRAC_DIV),
    CLK_UART2        = CLK(CLK_UART2_SEL, 0U),
    CLK_UART3_SRC    = CLK(CLK_UART3_PLL_SEL, CLK_UART3_DIV),
    CLK_UART3_FRAC   = CLK(0U, CLK_UART3_FRAC_DIV),
    CLK_UART3        = CLK(CLK_UART3_SEL, 0U),
    CLK_UART4_SRC    = CLK(CLK_UART4_PLL_SEL, CLK_UART4_DIV),
    CLK_UART4_FRAC   = CLK(0U, CLK_UART4_FRAC_DIV),
    CLK_UART4        = CLK(CLK_UART4_SEL, 0U),
    CLK_UART5_SRC    = CLK(CLK_UART5_PLL_SEL, CLK_UART5_DIV),
    CLK_UART5_FRAC   = CLK(0U, CLK_UART5_FRAC_DIV),
    CLK_UART5        = CLK(CLK_UART5_SEL, 0U),
    CLK_UART0_SRC    = CLK(CLK_UART0_PLL_SEL, CLK_UART0_DIV),
    CLK_UART0_FRAC   = CLK(0U, CLK_UART0_FRAC_DIV),
    CLK_UART0        = CLK(CLK_UART0_SEL, 0U),
    CLK_I2S0_TX_SRC  = CLK(CLK_I2S0_TX_PLL_SEL, CLK_I2S0_TX_DIV),
    CLK_I2S0_TX_FRAC = CLK(0U, CLK_I2S0_TX_FRAC_DIV),
    CLK_I2S0_TX      = CLK(CLK_I2S0_TX_SEL, 0U),
    MCLK_I2S0_TX     = CLK(CLK_I2S0_TX_OUT_MCLK_SEL, 0U),
    CLK_I2S1_SRC     = CLK(CLK_I2S1_PLL_SEL, CLK_I2S1_DIV),
    CLK_I2S1_FRAC    = CLK(0U, CLK_I2S1_FRAC_DIV),
    CLK_I2S1         = CLK(CLK_I2S1_SEL, 0U),
    MCLK_I2S1        = CLK(CLK_I2S1_OUT_MCLK_SEL, 0U),
    CLK_I2S2_SRC     = CLK(CLK_I2S2_PLL_SEL, CLK_I2S2_DIV),
    CLK_I2S2_FRAC    = CLK(0U, CLK_I2S2_FRAC_DIV),
    CLK_I2S2         = CLK(CLK_I2S2_SEL, 0U),
    MCLK_I2S2        = CLK(CLK_I2S2_OUT_MCLK_SEL, 0U),
    CLK_NANDC        = CLK(CLK_NANDC_PLL_SEL, CLK_NANDC_DIV),
    CLK_SDMMC        = CLK(CLK_SDMMC_PLL_SEL, CLK_SDMMC_DIV),
    CLK_SDIO         = CLK(CLK_SDIO_PLL_SEL, CLK_SDIO_DIV),
    CLK_EMMC         = CLK(CLK_EMMC_PLL_SEL, CLK_EMMC_DIV),
    CLK_SFC          = CLK(CLK_SFC_PLL_SEL, CLK_SFC_DIV),
    CLK_GMAC         = CLK(CLK_GMAC_PLL_SEL, CLK_GMAC_DIV),
    CLK_RMII         = CLK(RMII_CLK_SEL, 0U),
    CLK_RMII_EXTSRC  = CLK(RMII_EXTCLKSRC_SEL, 0U),
    CLK_MAC_OUT      = CLK(CLK_GMAC_OUT_PLL_SEL, CLK_GMAC_OUT_DIV),
    ACLK_PERI        = CLK(ACLK_HCLK_PERI_PLL_SEL, ACLK_PERI_DIV),
    HCLK_PERI        = CLK(ACLK_HCLK_PERI_PLL_SEL, HCLK_PERI_DIV),
    ACLK_BUS         = CLK(ACLK_HCLK_PCLK_BUS_PLL_SEL, ACLK_BUS_DIV),
    HCLK_BUS         = CLK(ACLK_HCLK_PCLK_BUS_PLL_SEL, HCLK_BUS_DIV),
    PCLK_BUS         = CLK(0U, PCLK_BUS_DIV),
    PCLK_GMAC        = CLK(0U, PCLK_GMAC_DIV),
} eCLOCK_Name;
#endif
/****************************************SPI*********************************************/
#define SPI_VERSION    0x0B050001

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __SOC_H */
