/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
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
    DMA_REQ_SPI0_TX      = 0,
    DMA_REQ_SPI0_RX      = 1,
    DMA_REQ_SPI1_TX      = 2,
    DMA_REQ_SPI1_RX      = 3,
    DMA_REQ_UART0_TX     = 4,
    DMA_REQ_UART0_RX     = 5,
    DMA_REQ_UART1_TX     = 6,
    DMA_REQ_UART1_RX     = 7,
    DMA_REQ_UART2_TX     = 8,
    DMA_REQ_UART2_RX     = 9,
    DMA_REQ_UART3_TX     = 10,
    DMA_REQ_UART3_RX     = 11,
    DMA_REQ_I2S_8CH_0_TX = 0,
    DMA_REQ_I2S_8CH_0_RX = 1,
    DMA_REQ_I2S_8CH_1_TX = 2,
    DMA_REQ_I2S_8CH_1_RX = 3,
    DMA_REQ_I2S_8CH_2_TX = 4,
    DMA_REQ_I2S_8CH_2_RX = 5,
    DMA_REQ_I2S_8CH_3_TX = 6,
    DMA_REQ_I2S_8CH_3_RX = 7,
    DMA_REQ_I2S_2CH_0_TX = 8,
    DMA_REQ_I2S_2CH_0_RX = 9,
    DMA_REQ_I2S_2CH_1_TX = 10,
    DMA_REQ_I2S_2CH_1_RX = 11,
    DMA_REQ_PDM0_RX      = 12,
    DMA_REQ_SPIDF_TX     = 13,
    DMA_REQ_SPIDF_RX     = 14,
    DMA_REQ_PWM0_RX      = 15,
    DMA_REQ_SPI2_TX      = 16,
    DMA_REQ_SPI2_RX      = 17,
    DMA_REQ_UART4_TX     = 18,
    DMA_REQ_UART4_RX     = 19,
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
    DMAC0_IRQn       = 32,            /*!< DMAC0 Interrupt             */
    DMAC0_ABORT_IRQn = 33,            /*!< DMAC0 ABORT Interrupt       */
    DMAC1_IRQn       = 34,            /*!< DMAC1 Interrupt             */
    DMAC1_ABORT_IRQn = 35,            /*!< DMAC1 ABORT Abort Interrupt */
    PWM1_IRQn        = 36,            /*!< PWM1 Interrupt              */
    PWM1_PWR_IRQn    = 37,            /*!< PWM1_PWR  Interrupt         */
    PWM2_IRQn        = 38,            /*!< PWM2 Interrupt              */
    PWM2_PWR_IRQn    = 39,            /*!< PWM2 PWR Interrupt          */
    DDR_PCTL_IRQn    = 40,            /*!< DDR PCTL Interrupt          */
    DDR_MON_IRQn     = 41,            /*!< DDR MON Interrupt           */
    WDT0_IRQn        = 42,            /*!< WDT0 Interrupt              */
    I2C0_IRQn        = 43,            /*!< I2C0 Interrupt              */
    I2C1_IRQn        = 44,            /*!< I2C1 Interrupt              */
    I2C2_IRQn        = 45,            /*!< I2C2 Interrupt              */
    I2C3_IRQn        = 46,            /*!< I2C3 Interrupt              */
    SPI0_IRQn        = 47,            /*!< SPI0 Interrupt              */
    SPI1_IRQn        = 48,            /*!< SPI1 Interrupt              */
    SPI2_IRQn        = 49,            /*!< SPI2 Interrupt              */
    UART0_IRQn       = 50,            /*!< UART0 Interrupt             */
    UART1_IRQn       = 51,            /*!< UART1 Interrupt             */
    UART2_IRQn       = 52,            /*!< UART2 Interrupt             */
    UART3_IRQn       = 53,            /*!< UART3 Interrupt             */
    UART4_IRQn       = 54,            /*!< UART4 Interrupt             */
    PWM0_IRQn        = 55,            /*!< PWM0 Interrupt              */
    PWM0_PWR_IRQn    = 56,            /*!< PWM0 PWR Interrupt          */
    TIMER0_IRQn      = 57,            /*!< TIMER0 Interrupt            */
    TIMER1_IRQn      = 58,            /*!< TIMER1 Interrupt            */
    TIMER2_IRQn      = 59,            /*!< TIMER2 Interrupt            */
    TIMER3_IRQn      = 60,            /*!< TIMER3 Interrupt            */
    TIMER4_IRQn      = 61,            /*!< TIMER4 Interrupt            */
    TIMER5_IRQn      = 62,            /*!< TIMER5 Interrupt            */
    TIMER6_IRQn      = 63,            /*!< TIMER6  Interrupt           */
    TIMER7_IRQn      = 64,            /*!< TIMER7  Interrupt           */
    TIMER8_IRQn      = 65,            /*!< TIMER8  Interrupt           */
    TIMER9_IRQn      = 66,            /*!< TIMER9  Interrupt           */
    TIMER10_IRQn     = 67,            /*!< TIMER10  Interrupt          */
    TIMER11_IRQn     = 68,            /*!< TIMER11  Interrupt          */
    SARADC_IRQn      = 69,            /*!< SARADC  Interrupt           */
    TSADC_IRQn       = 70,            /*!< TSADC  Interrupt            */
    GPIO0_IRQn       = 72,            /*!< GPIO0  Interrupt            */
    GPIO1_IRQn       = 73,            /*!< GPIO1  Interrupt            */
    GPIO2_IRQn       = 74,            /*!< GPIO2  Interrupt            */
    GPIO3_IRQn       = 75,            /*!< GPIO3 Interrupt             */
    GPIO4_IRQn       = 76,            /*!< GPIO4 Interrupt             */
    CRYPTO_IRQn      = 77,            /*!< CRYPTO Interrupt            */
    VOP_IRQn         = 78,            /*!< VOP Interrupt               */
    I2S_8CH_0_IRQn   = 80,            /*!< I2S_8CH_0 Interrupt         */
    I2S_8CH_1_IRQn   = 81,            /*!< I2S_8CH_1 Interrupt         */
    I2S_8CH_2_IRQn   = 82,            /*!< I2S_8CH_2 Interrupt         */
    I2S_8CH_3_IRQn   = 83,            /*!< I2S_8CH_3 Interrupt         */
    I2S_2CH_0_IRQn   = 84,            /*!< I2S_2CH_0 Interrupt         */
    I2S_2CH_1_IRQn   = 85,            /*!< I2S_2CH_1 Interrupt         */
    PDM0_IRQn        = 86,            /*!< PDM0 Interrupt              */
    SPDIF_TX_IRQn    = 87,            /*!< SPDIF TX Interrupt          */
    SPDIF_RX_IRQn    = 88,            /*!< SPDIF RX Interrupt          */
    VAD_IRQn         = 89,            /*!< VAD Interrupt               */
    OWIRE_IRQn       = 90,            /*!< OWIRE Interrupt             */
    CAN0_IRQn        = 91,            /*!< CAN0 Interrupt              */
    MAC_IRQn         = 96,            /*!< MAC Interrupt               */
    MAC_PMT_IRQn     = 97,            /*!< MAC_PMT Interrupt           */
    SDMMC_IRQn       = 108,           /*!< SDMMC Interrupt             */
    EMMC_IRQn        = 109,           /*!< EMMC Interrupt              */
    SDIO_IRQn        = 110,           /*!< SDIO Interrupt              */
    SFC_IRQn         = 114,           /*!< SFC Interrupt               */
    AMP0_IRQn        = 119,           /*!< AMP0   Interrupt            */
    AMP1_IRQn        = 120,           /*!< AMP1   Interrupt            */
    AMP2_IRQn        = 121,           /*!< AMP2   Interrupt            */
    AMP3_IRQn        = 122,           /*!< AMP3   Interrupt            */
    RPMSG_01_IRQn    = 130,           /*!< RPMSG 0->1 Interrupt        */
    RPMSG_02_IRQn    = 131,           /*!< RPMSG 0->2 Interrupt        */
    RPMSG_03_IRQn    = 132,           /*!< RPMSG 0->3 Interrupt        */
    RPMSG_10_IRQn    = 133,           /*!< RPMSG 1->0 Interrupt        */
    RPMSG_12_IRQn    = 134,           /*!< RPMSG 1->2 Interrupt        */
    RPMSG_13_IRQn    = 135,           /*!< RPMSG 1->3 Interrupt        */
    RPMSG_20_IRQn    = 136,           /*!< RPMSG 2->0 Interrupt        */
    RPMSG_21_IRQn    = 137,           /*!< RPMSG 2->1 Interrupt        */
    RPMSG_23_IRQn    = 138,           /*!< RPMSG 2->3 Interrupt        */
    RPMSG_30_IRQn    = 139,           /*!< RPMSG 3->0 Interrupt        */
    RPMSG_31_IRQn    = 140,           /*!< RPMSG 3->1 Interrupt        */
    RPMSG_32_IRQn    = 141,           /*!< RPMSG 3->2 Interrupt        */
    RSVD0_IRQn       = 148,           /*!< RSVD0  Interrupt            */
    NUM_INTERRUPTS   = 160
} IRQn_Type;

#define RSVD_IRQn(_N) (RSVD0_IRQn + (_N))

#define AMP_CPUOFF_REQ_IRQ(cpu) RSVD_IRQn(12 + (cpu))     /* gic irq: 160 */

#define GPIO_IRQ_GROUP_DIRQ_BASE RSVD_IRQn(52)    /* gic irq:  200 */
#define GPIO_IRQ_GROUP_DIRQ_NUM  (NUM_INTERRUPTS - GPIO_IRQ_GROUP_DIRQ_BASE)

#define GPIO_IRQ_GROUP_GPIO0_HWIRQ GPIO0_IRQn
#define GPIO_IRQ_GROUP_GPION_HWIRQ GPIO4_IRQn

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
#include "system_rk3308.h"
#endif /* __ASSEMBLY__ */
#include "rk3308.h"
#include "rk3308_acodec.h"

/****************************************************************************************/
/*                                                                                      */
/*                                Module Address Section                                */
/*                                                                                      */
/****************************************************************************************/
/* Memory Base */
#define GIC_DISTRIBUTOR_BASE   (0xFF581000)
#define GIC_CPU_INTERFACE_BASE (0xFF582000)

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

/******************************************CRU*******************************************/

/********Name=SOFTRST_CON0,Offset=0x400********/
#define SRST_COREPO0   0U
#define SRST_COREPO1   1U
#define SRST_COREPO2   2U
#define SRST_COREPO3   3U
#define SRST_CORE0     4U
#define SRST_CORE1     5U
#define SRST_CORE2     6U
#define SRST_CORE3     7U
#define SRST_TOPDBG    12U
#define SRST_CORE_NOC  13U
#define SRST_A_BUS_NIU 14U
#define SRST_L2        15U
/********Name=SOFTRST_CON1,Offset=0x404********/
#define SRST_DAP           16U
#define SRST_CORE_PVTM     17U
#define SRST_A_CORE_PRF    18U
#define SRST_P_CORE_GRF    19U
#define SRST_DDRUPCTL      20U
#define SRST_P_DDRUPCTL    22U
#define SRST_MSCH          23U
#define SRST_P_DDR_MONITOR 25U
#define SRST_P_DDR_STDBY   26U
#define SRST_DDR_STDBY     27U
#define SRST_DDRPHY        28U
#define SRST_DDRPHY_CLKDIV 29U
#define SRST_P_DDRPHY      30U
/********Name=SOFTRST_CON2,Offset=0x408********/
#define SRST_H_BUS_NIU   32U
#define SRST_P_BUS_NIU   33U
#define SRST_A_CRYPTO    34U
#define SRST_H_CRYPTO    35U
#define SRST_CRYPTO_CORE 36U
#define SRST_CRYPTO_APK  37U
#define SRST_A_VOP       38U
#define SRST_H_VOP       39U
#define SRST_D_VOP       40U
#define SRST_A_INTMEM    41U
#define SRST_A_GIC       42U
#define SRST_H_ROM       43U
#define SRST_P_UART0     44U
#define SRST_UART0       45U
#define SRST_P_UART1     46U
#define SRST_UART1       47U
/********Name=SOFTRST_CON3,Offset=0x40C********/
#define SRST_P_UART2 48U
#define SRST_UART2   49U
#define SRST_P_UART3 50U
#define SRST_UART3   51U
#define SRST_P_UART4 52U
#define SRST_UART4   53U
#define SRST_P_I2C0  54U
#define SRST_I2C0    55U
#define SRST_P_I2C1  56U
#define SRST_I2C1    57U
#define SRST_P_I2C2  58U
#define SRST_I2C2    59U
#define SRST_P_I2C3  60U
#define SRST_I2C3    61U
#define SRST_P_PWM   62U
#define SRST_PWM     63U
/********Name=SOFTRST_CON4,Offset=0x410********/
#define SRST_P_SPI0   64U
#define SRST_SPI0     65U
#define SRST_P_SPI1   66U
#define SRST_SPI1     67U
#define SRST_P_SPI2   68U
#define SRST_SPI2     69U
#define SRST_P_SARADC 70U
#define SRST_P_TSADC  71U
#define SRST_TSADC    72U
#define SRST_P_TIMER0 73U
#define SRST_TIMER0   74U
#define SRST_TIMER1   75U
#define SRST_TIMER2   76U
#define SRST_TIMER3   77U
#define SRST_TIMER4   78U
#define SRST_TIMER5   79U
/********Name=SOFTRST_CON5,Offset=0x414********/
#define SRST_P_OTP_NS       80U
#define SRST_OTP_NS_SBPI    81U
#define SRST_OTP_NS_USER    82U
#define SRST_P_OTP_PHY      83U
#define SRST_OTP_PHY_SRSTN  84U
#define SRST_P_GPIO0        86U
#define SRST_P_GPIO1        87U
#define SRST_P_GPIO2        88U
#define SRST_P_GPIO3        89U
#define SRST_P_GPIO4        90U
#define SRST_P_GRF          91U
#define SRST_P_USBSDMMC_DET 92U
#define SRST_PMU_SRSTN      93U
#define SRST_PMU_PVTM       94U
#define SRST_P_USB_GRF      95U
/********Name=SOFTRST_CON6,Offset=0x418********/
#define SRST_CPU_BOOST      96U
#define SRST_P_CPU_BOOST    97U
#define SRST_P_PWM1         98U
#define SRST_PWM1           99U
#define SRST_P_PWM2         100U
#define SRST_PWM2           101U
#define SRST_A_PERI_NIU     104U
#define SRST_H_PERI_NIU     105U
#define SRST_P_PERI_NIU     106U
#define SRST_H_USB2OTG      107U
#define SRST_USB2OTG        108U
#define SRST_USB2OTG_ADP    109U
#define SRST_H_USB2HOST     110U
#define SRST_H_USB2HOST_ARB 111U
/********Name=SOFTRST_CON7,Offset=0x41C********/
#define SRST_H_USB2HOST_AUX   112U
#define SRST_USB2HOST_EHCIPHY 113U
#define SRST_USB2HOST_UTMI    114U
#define SRST_USBPHYPOR_RST    115U
#define SRST_UTMI0_SRST       116U
#define SRST_UTMI1_SRST       117U
#define SRST_H_SDIO           118U
#define SRST_H_EMMC           119U
#define SRST_H_SFC            120U
#define SRST_SFC              121U
#define SRST_H_SDMMC          122U
#define SRST_H_NANDC          123U
#define SRST_N_NANDC          124U
#define SRST_A_MAC            125U
#define SRST_P_CAN            126U
#define SRST_P_OWIRE          127U
/********Name=SOFTRST_CON8,Offset=0x420********/
#define SRST_H_AUDIO_NIU   128U
#define SRST_P_AUDIO_NIU   129U
#define SRST_H_PDM         130U
#define SRST_M_PDM         131U
#define SRST_H_SPDIFTX     132U
#define SRST_M_SPDIFTX     133U
#define SRST_H_SPDIFRX     134U
#define SRST_M_SPDIFRX     135U
#define SRST_H_I2S0_8CH    136U
#define SRST_M_I2S0_8CH_TX 137U
#define SRST_M_I2S0_8CH_RX 138U
#define SRST_H_I2S1_8CH    139U
#define SRST_M_I2S1_8CH_TX 140U
#define SRST_M_I2S1_8CH_RX 141U
#define SRST_H_I2S2_8CH    142U
#define SRST_M_I2S2_8CH_TX 143U
/********Name=SOFTRST_CON9,Offset=0x424********/
#define SRST_M_I2S2_8CH_RX 144U
#define SRST_H_I2S3_8CH    145U
#define SRST_M_I2S3_8CH_TX 146U
#define SRST_M_I2S3_8CH_RX 147U
#define SRST_H_I2S0_2CH    148U
#define SRST_M_I2S0_2CH    149U
#define SRST_H_I2S1_2CH    150U
#define SRST_M_I2S1_2CH    151U
#define SRST_H_VAD         152U
#define SRST_P_ACODEC      153U

/********Name=CLKGATE_CON0,Offset=0x300********/
#define CORE_PLL_CLK_GATE               0U
#define ACLK_CORE_SRC_CLK_GATE          1U
#define PCLK_CORE_DBG_SRC_CLK_GATE      2U
#define CLK_JTAG_CORE_CLK_GATE          3U
#define CLK_CORE_PVTM_CLK_GATE          4U
#define ACLK_CORE_NIU_CLK_GATE          5U
#define PCLK_CORE_DBG_NIU_CLK_GATE      6U
#define PCLK_CORE_DBG_DAPLITE_CLK_GATE  7U
#define ACLK_CORE_PERF_CLK_GATE         8U
#define PCLK_CORE_GRF_CLK_GATE          9U
#define DDR_PHY_PLL_CLK_GATE            10U
#define DDR_PHY_DFI_CLK4X_GATE          11U
#define DDR_MONITOR_TIMER_CLK_GATE      12U
#define DDR_PHY_DFI_CLK4X_DIV4_CLK_GATE 13U
/********Name=CLKGATE_CON1,Offset=0x304********/
#define LOGIC_BUS_PLL_CLK_GATE      16U
#define ACLK_BUS_CLK_GATE           17U
#define HCLK_BUS_CLK_GATE           18U
#define PCLK_BUS_CLK_GATE           19U
#define CLK_CRYPTO_PLL_CLK_GATE     20U
#define CLK_CRYPTO_APK_PLL_CLK_GATE 21U
#define DCLKVOP_PLL_CLK_GATE        22U
#define DCLKVOP_FRACDIV_CLK_GATE    23U
#define DCLKVOP_CLK_GATE            24U
#define CLK_UART0_PLL_CLK_GATE      25U
#define CLK_UART0_DIVNP5_CLK_GATE   26U
#define CLK_UART0_DIVFRAC_CLK_GATE  27U
#define CLK_UART0_CLK_GATE          28U
#define CLK_UART1_PLL_CLK_GATE      29U
#define CLK_UART1_DIVNP5_CLK_GATE   30U
#define CLK_UART1_DIVFRAC_CLK_GATE  31U
/********Name=CLKGATE_CON2,Offset=0x308********/
#define CLK_UART1_CLK_GATE         32U
#define CLK_UART2_PLL_CLK_GATE     33U
#define CLK_UART2_DIVNP5_CLK_GATE  34U
#define CLK_UART2_DIVFRAC_CLK_GATE 35U
#define CLK_UART2_CLK_GATE         36U
#define CLK_UART3_PLL_CLK_GATE     37U
#define CLK_UART3_DIVNP5_CLK_GATE  38U
#define CLK_UART3_DIVFRAC_CLK_GATE 39U
#define CLK_UART3_CLK_GATE         40U
#define CLK_UART4_PLL_CLK_GATE     41U
#define CLK_UART4_DIVNP5_CLK_GATE  42U
#define CLK_UART4_DIVFRAC_CLK_GATE 43U
#define CLK_UART4_CLK_GATE         44U
#define CLK_I2C0_PLL_CLK_GATE      45U
#define CLK_I2C1_PLL_CLK_GATE      46U
#define CLK_I2C2_PLL_CLK_GATE      47U
/********Name=CLKGATE_CON3,Offset=0x30C********/
#define CLK_I2C3_PLL_CLK_GATE   48U
#define CLK_PWM_PLL_CLK_GATE    49U
#define CLK_SPI0_PLL_CLK_GATE   50U
#define CLK_SPI1_PLL_CLK_GATE   51U
#define CLK_SPI2_PLL_CLK_GATE   52U
#define CLK_TSADC_PLL_CLK_GATE  53U
#define CLK_SARADC_PLL_CLK_GATE 54U
#define CLK_OTP_PLL_CLK_GATE    55U
#define CLK_OTP_USR_CLK_GATE    56U
#define CLK_CPU_BOOST_CLK_GATE  57U
#define CLK_TIMER0_GATE         58U
#define CLK_TIMER1_GATE         59U
#define CLK_TIMER2_GATE         60U
#define CLK_TIMER3_GATE         61U
#define CLK_TIMER4_GATE         62U
#define CLK_TIMER5_GATE         63U
/********Name=CLKGATE_CON4,Offset=0x310********/
#define CLK_WIFI_PLL_CLK_GATE        64U
#define CLK_WIFI_CLK_GATE            65U
#define CLK_DIV32K_OUT_CLK_GATE      66U
#define CLK_DIV32K_FRAC_OUT_CLK_GATE 67U
#define CLK_PMU_PVTM_CLK_GATE        68U
#define PCLK_PMU_CLK_GATE            69U
#define CLK_PMU_CLK_GATE             70U
#define CLK_REF24M_PLL_CLK_GATE      71U
#define USBPHY_REF24M_CLK_GATE       72U
#define TESTCLK_GATE                 73U
#define CLK_DDR_MON_GATE             74U
#define CLK_DDR_UPCTRL_GATE          75U
#define CLK_DDR_MSCH_GATE            76U
#define CLK_DDR_MSCH_PERI_GATE       77U
#define CLK_DDR_STDBY_GATE           78U
/********Name=CLKGATE_CON5,Offset=0x314********/
#define ACLK_BUS_NIU_GATE 80U
#define ACLK_INTMEM_GATE  81U
#define ACLK_CRYPTO_GATE  82U
#define ACLK_VOP_GATE     83U
#define ACLK_GIC_GATE     84U
#define HCLK_BUS_NIU_GATE 85U
#define HCLK_ROM_GATE     86U
#define HCLK_CRYPTO_GATE  87U
#define HCLK_VOP_GATE     88U
#define PCLK_BUS_NIU_GATE 89U
#define PCLK_UART0_GATE   90U
#define PCLK_UART1_GATE   91U
#define PCLK_UART2_GATE   92U
#define PCLK_UART3_GATE   93U
#define PCLK_UART4_GATE   94U
#define PCLK_I2C0_GATE    95U
/********Name=CLKGATE_CON6,Offset=0x318********/
#define PCLK_I2C1_GATE   96U
#define PCLK_I2C2_GATE   97U
#define PCLK_I2C3_GATE   98U
#define PCLK_PWM_GATE    99U
#define PCLK_SPI0_GATE   100U
#define PCLK_SPI1_GATE   101U
#define PCLK_SPI2_GATE   102U
#define PCLK_SARADC_GATE 103U
#define PCLK_TSADC_GATE  104U
#define PCLK_TIMER_GATE  105U
#define PCLK_OTP_NS_GATE 106U
#define PCLK_GPIO0_GATE  108U
#define PCLK_GPIO1_GATE  109U
#define PCLK_GPIO2_GATE  110U
#define PCLK_GPIO3_GATE  111U
/********Name=CLKGATE_CON7,Offset=0x31C********/
#define PCLK_GPIO4_GATE        112U
#define PCLK_SGRF_GATE         113U
#define PCLK_GRF_GATE          114U
#define PCLK_USBSDMMC_DET_GATE 115U
#define PCLK_DDR_UPCTRL_GATE   116U
#define PCLK_DDR_MON_GATE      117U
#define PCLK_DDR_PHY_GATE      118U
#define PCLK_DDR_STDBY_GATE    119U
#define PCLK_USBGRF_GATE       120U
#define PCLK_CRU_GATE          121U
#define PCLK_OTP_PHY_GATE      122U
#define PCLK_CPU_BOOST_GATE    123U
#define PCLK_PWM1_GATE         124U
#define PCLK_PWM2_GATE         125U
#define PCLK_CAN_GATE          126U
#define PCLK_OWIRE_GATE        127U
/********Name=CLKGATE_CON8,Offset=0x320********/
#define LOGIC_PERI_PLL_CLK_GATE 128U
#define ACLK_PERI_CLK_GATE      129U
#define HCLK_PERI_CLK_GATE      130U
#define PCLK_PERI_CLK_GATE      131U
#define CLK_NANDC_PLL_CLK_GATE  132U
#define CLK_NANDC_CLK_GATE      133U
#define CLK_SDMMC_PLL_CLK_GATE  134U
#define CLK_SDMMC_CLK_GATE      135U
#define CLK_SDIO_PLL_CLK_GATE   136U
#define CLK_SDIO_GATE           137U
#define CLK_EMMC_PLL_CLK_GATE   138U
#define CLK_EMMC_CLK_GATE       139U
#define CLK_SFC_PLL_CLK_GATE    140U
#define CLK_OTG_ADP_CLK_GATE    141U
#define CLK_MAC_PLL_CLK_GATE    142U
#define CLK_OWIRE_PLL_CLK_GATE  143U
/********Name=CLKGATE_CON9,Offset=0x324********/
#define CLK_MAC_TXRX_GATE     144U
#define CLK_MAC_REF_GATE      145U
#define ACLK_PERI_NIU_GATE    146U
#define ACLK_PERIBUS_NIU_GATE 147U
#define ACLK_MAC_GATE         148U
#define HCLK_PERI_NIU_GATE    149U
#define HCLK_NANC_GATE        150U
#define HCLK_SDMMC_GATE       151U
#define HCLK_SDIO_GATE        152U
#define HCLK_EMMC_GATE        153U
#define HCLK_SFC_GATE         154U
#define HCLK_OTG_GATE         155U
#define HCLK_HOST_GATE        156U
#define HCLK_HOST_ARB_GATE    157U
#define PCLK_PERI_NIU_GATE    158U
#define PCLK_MAC_GATE         159U
/********Name=CLKGATE_CON10,Offset=0x328********/
#define LOGIC_AUDIO_PLL_CLK_GATE         160U
#define HCLK_AUDIO_CLK_GATE              161U
#define PCLK_AUDIO_CLK_GATE              162U
#define CLK_PDM_PLL_CLK_GATE             163U
#define CLK_PDM_DIVFRAC_CLK_GATE         164U
#define CLK_PDM_CLK_GATE                 165U
#define CLK_SPDIFTX_PLL_CLK_GATE         166U
#define CLK_SPDIFTX_DIVFRAC_CLK_GATE     167U
#define CLK_SPDIFTX_CLK_GATE             168U
#define CLK_SPDIFRX_PLL_CLK_GATE         169U
#define CLK_SPDIFRX_DIVFRAC_CLK_GATE     170U
#define CLK_SPDIFRX_CLK_GATE             171U
#define CLK_I2S0_8CH_TX_PLL_CLK_GATE     172U
#define CLK_I2S0_8CH_TX_DIVFRAC_CLK_GATE 173U
#define CLK_I2S0_8CH_TX_CLK_GATE         174U
#define CLK_I2S0_8CH_TX_OUT_CLK_GATE     175U
/********Name=CLKGATE_CON11,Offset=0x32C********/
#define CLK_I2S0_8CH_RX_PLL_CLK_GATE     176U
#define CLK_I2S0_8CH_RX_DIVFRAC_CLK_GATE 177U
#define CLK_I2S0_8CH_RX_CLK_GATE         178U
#define CLK_I2S0_8CH_RX_OUT_CLK_GATE     179U
#define CLK_I2S1_8CH_TX_PLL_CLK_GATE     180U
#define CLK_I2S1_8CH_TX_DIVFRAC_CLK_GATE 181U
#define CLK_I2S1_8CH_TX_CLK_GATE         182U
#define CLK_I2S1_8CH_TX_OUT_CLK_GATE     183U
#define CLK_I2S1_8CH_RX_PLL_CLK_GATE     184U
#define CLK_I2S1_8CH_RX_DIVFRAC_CLK_GATE 185U
#define CLK_I2S1_8CH_RX_CLK_GATE         186U
#define CLK_I2S1_8CH_RX_OUT_CLK_GATE     187U
#define CLK_I2S2_8CH_TX_PLL_CLK_GATE     188U
#define CLK_I2S2_8CH_TX_DIVFRAC_CLK_GATE 189U
#define CLK_I2S2_8CH_TX_CLK_GATE         190U
#define CLK_I2S2_8CH_TX_OUT_CLK_GATE     191U
/********Name=CLKGATE_CON12,Offset=0x330********/
#define CLK_I2S2_8CH_RX_PLL_CLK_GATE     192U
#define CLK_I2S2_8CH_RX_DIVFRAC_CLK_GATE 193U
#define CLK_I2S2_8CH_RX_CLK_GATE         194U
#define CLK_I2S2_8CH_RX_OUT_CLK_GATE     195U
#define CLK_I2S3_8CH_TX_PLL_CLK_GATE     196U
#define CLK_I2S3_8CH_TX_DIVFRAC_CLK_GATE 197U
#define CLK_I2S3_8CH_TX_CLK_GATE         198U
#define CLK_I2S3_8CH_TX_OUT_CLK_GATE     199U
#define CLK_I2S3_8CH_RX_PLL_CLK_GATE     200U
#define CLK_I2S3_8CH_RX_DIVFRAC_CLK_GATE 201U
#define CLK_I2S3_8CH_RX_CLK_GATE         202U
#define CLK_I2S3_8CH_RX_OUT_CLK_GATE     203U
#define CLK_I2S0_2CH_PLL_CLK_GATE        204U
#define CLK_I2S0_2CH_DIVFRAC_CLK_GATE    205U
#define CLK_I2S0_2CH_CLK_GATE            206U
#define CLK_I2S0_2CH_OUT_CLK_GATE        207U
/********Name=CLKGATE_CON13,Offset=0x334********/
#define CLK_I2S1_2CH_PLL_CLK_GATE     208U
#define CLK_I2S1_2CH_DIVFRAC_CLK_GATE 209U
#define CLK_I2S1_2CH_CLK_GATE         210U
#define CLK_I2S1_2CH_OUT_CLK_GATE     211U
#define CLK_I2S0_8CH_TX_OUT_IOE_GATE  212U
#define CLK_I2S0_8CH_RX_OUT_IOE_GATE  213U
#define CLK_I2S1_8CH_TX_OUT_IOE_GATE  214U
#define CLK_I2S1_8CH_RX_OUT_IOE_GATE  215U
#define CLK_I2S0_2CH_OUT_IOE_GATE     216U
/********Name=CLKGATE_CON14,Offset=0x338********/
#define HCLK_AUDIO_NIU_GATE 224U
#define HCLK_PDM_GATE       225U
#define HCLK_SPDIFTX_GATE   226U
#define HCLK_SPDIFRX_GATE   227U
#define HCLK_I2S0_8CH_GATE  228U
#define HCLK_I2S1_8CH_GATE  229U
#define HCLK_I2S2_8CH_GATE  230U
#define HCLK_I2S3_8CH_GATE  231U
#define HCLK_I2S0_2CH_GATE  232U
#define HCLK_I2S1_2CH_GATE  233U
#define HCLK_VAD_GATE       234U
#define PCLK_AUDIO_NIU_GATE 235U
#define PCLK_ACODEC_GATE    236U
/********Name=CLKGATE_CON15,Offset=0x33C********/
#define CLK_PWM1_PLL_CLK_GATE   240U
#define CLK_PWM2_PLL_CLK_GATE   241U
#define CLK_WIFI_DPLL_CLK_GATE  242U
#define CLK_WIFI_VPLL0_CLK_GATE 243U
#define CLK_WIFI_OSC_CLK_GATE   244U

/********Name=CLKSEL_CON0_S,Offset=0xF8********/
/********Name=CLKSEL_CON0,Offset=0x100********/
#define CLK_CORE_DIV  0x04000000U
#define CORE_DBG_DIV  0x04080000U
#define ACLK_CORE_DIV 0x030C0000U
/********Name=CLKSEL_CON1,Offset=0x104********/
#define DDRPHY_DFI_CLK4X_DIV 0x03000001U
/********Name=CLKSEL_CON2,Offset=0x108********/
#define FUNC_24M_DIV 0x05000002U
/********Name=CLKSEL_CON3,Offset=0x10C********/
#define CLK_32K_FRAC_DIV 0x20000003U
/********Name=CLKSEL_CON4,Offset=0x110********/
#define CLK_32K_DIV 0x10000004U
/********Name=CLKSEL_CON5,Offset=0x114********/
#define ACLK_BUS_DIV 0x05000005U
/********Name=CLKSEL_CON6,Offset=0x118********/
#define HCLK_BUS_DIV 0x05000006U
#define PCLK_BUS_DIV 0x05080006U
/********Name=CLKSEL_CON7,Offset=0x11C********/
#define CLK_CRYPTO_DIV     0x05000007U
#define CLK_CRYPTO_APK_DIV 0x05080007U
/********Name=CLKSEL_CON8,Offset=0x120********/
#define DCLK_VOP_DIV 0x08000008U
/********Name=CLKSEL_CON9,Offset=0x124********/
#define DCLK_VOP_FRAC_DIV 0x20000009U
/********Name=CLKSEL_CON10,Offset=0x128********/
#define CLK_UART0_DIV 0x0500000AU
/********Name=CLKSEL_CON11,Offset=0x12C********/
#define CLK_UART0_DIVNP5_DIV 0x0500000BU
/********Name=CLKSEL_CON12,Offset=0x130********/
#define CLK_UART0_FRAC_DIV 0x2000000CU
/********Name=CLKSEL_CON13,Offset=0x134********/
#define CLK_UART1_DIV 0x0500000DU
/********Name=CLKSEL_CON14,Offset=0x138********/
#define CLK_UART1_DIVNP5_DIV 0x0500000EU
/********Name=CLKSEL_CON15,Offset=0x13C********/
#define CLK_UART1_FRAC_DIV 0x2000000FU
/********Name=CLKSEL_CON16,Offset=0x140********/
#define CLK_UART2_DIV 0x05000010U
/********Name=CLKSEL_CON17,Offset=0x144********/
#define CLK_UART2_DIVNP5_DIV 0x05000011U
/********Name=CLKSEL_CON18,Offset=0x148********/
#define CLK_UART2_FRAC_DIV 0x20000012U
/********Name=CLKSEL_CON19,Offset=0x14C********/
#define CLK_UART3_DIV 0x05000013U
/********Name=CLKSEL_CON20,Offset=0x150********/
#define CLK_UART3_DIVNP5_DIV 0x05000014U
/********Name=CLKSEL_CON21,Offset=0x154********/
#define CLK_UART3_FRAC_DIV 0x20000015U
/********Name=CLKSEL_CON22,Offset=0x158********/
#define CLK_UART4_DIV 0x05000016U
/********Name=CLKSEL_CON23,Offset=0x15C********/
#define CLK_UART4_DIVNP5_DIV 0x05000017U
/********Name=CLKSEL_CON24,Offset=0x160********/
#define CLK_UART4_FRAC_DIV 0x20000018U
/********Name=CLKSEL_CON25,Offset=0x164********/
#define CLK_I2C0_DIV 0x07000019U
/********Name=CLKSEL_CON26,Offset=0x168********/
#define CLK_I2C1_DIV 0x0700001AU
/********Name=CLKSEL_CON27,Offset=0x16C********/
#define CLK_I2C2_DIV 0x0700001BU
/********Name=CLKSEL_CON28,Offset=0x170********/
#define CLK_I2C3_DIV 0x0700001CU
/********Name=CLKSEL_CON29,Offset=0x174********/
#define CLK_PWM_DIV 0x0700001DU
/********Name=CLKSEL_CON30,Offset=0x178********/
#define CLK_SPI0_DIV 0x0700001EU
/********Name=CLKSEL_CON31,Offset=0x17C********/
#define CLK_SPI1_DIV 0x0700001FU
/********Name=CLKSEL_CON32,Offset=0x180********/
#define CLK_SPI2_DIV 0x07000020U
/********Name=CLKSEL_CON33,Offset=0x184********/
#define CLK_TSADC_DIV 0x0B000021U
/********Name=CLKSEL_CON34,Offset=0x188********/
#define CLK_SARADC_DIV 0x0B000022U
/********Name=CLKSEL_CON35,Offset=0x18C********/
#define CLK_OTP_DIV     0x04000023U
#define CLK_OTP_USR_DIV 0x02040023U
/********Name=CLKSEL_CON36,Offset=0x190********/
#define ACLK_PERI_DIV 0x05000024U
/********Name=CLKSEL_CON37,Offset=0x194********/
#define HCLK_PERI_DIV 0x05000025U
#define PCLK_PERI_DIV 0x05080025U
/********Name=CLKSEL_CON38,Offset=0x198********/
#define CLK_NANDC_DIV 0x05000026U
/********Name=CLKSEL_CON39,Offset=0x19C********/
#define CLK_SDMMC_DIV 0x08000027U
/********Name=CLKSEL_CON40,Offset=0x1A0********/
#define CLK_SDIO_DIV 0x08000028U
/********Name=CLKSEL_CON41,Offset=0x1A4********/
#define CLK_EMMC_DIV 0x08000029U
/********Name=CLKSEL_CON42,Offset=0x1A8********/
#define CLK_SFC_DIV 0x0700002AU
/********Name=CLKSEL_CON43,Offset=0x1AC********/
#define CLK_MAC_DIV 0x0500002BU
/********Name=CLKSEL_CON44,Offset=0x1B0********/
#define CLK_WIFI_DIV  0x0600002CU
#define CLK_OWIRE_DIV 0x0608002CU
/********Name=CLKSEL_CON45,Offset=0x1B4********/
#define HCLK_AUDIO_DIV 0x0500002DU
#define PCLK_AUDIO_DIV 0x0508002DU
/********Name=CLKSEL_CON46,Offset=0x1B8********/
#define CLK_PDM_DIV 0x0700002EU
/********Name=CLKSEL_CON47,Offset=0x1BC********/
#define CLK_PDM_FRAC_DIV 0x2000002FU
/********Name=CLKSEL_CON48,Offset=0x1C0********/
#define CLK_SPDIFTX_DIV 0x07000030U
/********Name=CLKSEL_CON49,Offset=0x1C4********/
#define CLK_SPDIFTX_FRAC_DIV 0x20000031U
/********Name=CLKSEL_CON50,Offset=0x1C8********/
#define CLK_SPDIFRX_DIV 0x07000032U
/********Name=CLKSEL_CON51,Offset=0x1CC********/
#define CLK_SPDIFRX_FRAC_DIV 0x20000033U
/********Name=CLKSEL_CON52,Offset=0x1D0********/
#define CLK_I2S0_8CH_TX_DIV 0x07000034U
/********Name=CLKSEL_CON53,Offset=0x1D4********/
#define CLK_I2S0_8CH_TX_FRAC_DIV 0x20000035U
/********Name=CLKSEL_CON54,Offset=0x1D8********/
#define CLK_I2S0_8CH_RX_DIV 0x07000036U
/********Name=CLKSEL_CON55,Offset=0x1DC********/
#define CLK_I2S0_8CH_RX_FRAC_DIV 0x20000037U
/********Name=CLKSEL_CON56,Offset=0x1E0********/
#define CLK_I2S1_8CH_TX_DIV 0x07000038U
/********Name=CLKSEL_CON57,Offset=0x1E4********/
#define CLK_I2S1_8CH_TX_FRAC_DIV 0x20000039U
/********Name=CLKSEL_CON58,Offset=0x1E8********/
#define CLK_I2S1_8CH_RX_DIV 0x0700003AU
/********Name=CLKSEL_CON59,Offset=0x1EC********/
#define CLK_I2S1_8CH_RX_FRAC_DIV 0x2000003BU
/********Name=CLKSEL_CON60,Offset=0x1F0********/
#define CLK_I2S2_8CH_TX_DIV 0x0700003CU
/********Name=CLKSEL_CON61,Offset=0x1F4********/
#define CLK_I2S2_8CH_TX_FRAC_DIV 0x2000003DU
/********Name=CLKSEL_CON62,Offset=0x1F8********/
#define CLK_I2S2_8CH_RX_DIV 0x0700003EU
/********Name=CLKSEL_CON63,Offset=0x1FC********/
#define CLK_I2S2_8CH_RX_FRAC_DIV 0x2000003FU
/********Name=CLKSEL_CON64,Offset=0x200********/
#define CLK_I2S3_8CH_TX_DIV 0x07000040U
/********Name=CLKSEL_CON65,Offset=0x204********/
#define CLK_I2S3_8CH_TX_FRAC_DIV 0x20000041U
/********Name=CLKSEL_CON66,Offset=0x208********/
#define CLK_I2S3_8CH_RX_DIV 0x07000042U
/********Name=CLKSEL_CON67,Offset=0x20C********/
#define CLK_I2S3_8CH_RX_FRAC_DIV 0x20000043U
/********Name=CLKSEL_CON68,Offset=0x210********/
#define CLK_I2S0_2CH_DIV 0x07000044U
/********Name=CLKSEL_CON69,Offset=0x214********/
#define CLK_I2S0_2CH_FRAC_DIV 0x20000045U
/********Name=CLKSEL_CON70,Offset=0x218********/
#define CLK_I2S1_2CH_DIV 0x07000046U
/********Name=CLKSEL_CON71,Offset=0x21C********/
#define CLK_I2S1_2CH_FRAC_DIV 0x20000047U
/********Name=CLKSEL_CON72,Offset=0x220********/
#define CLK_REF24M_DIV 0x06000048U
/********Name=CLKSEL_CON73,Offset=0x224********/
#define TEST_DIV 0x05000049U
/********Name=CLKSEL_CON74,Offset=0x228********/
#define CLK_PWM1_DIV 0x0700004AU
/********Name=CLKSEL_CON75,Offset=0x22C********/
#define CLK_PWM2_DIV 0x0700004BU

/********Name=CLKSEL_CON0_S,Offset=0xF8********/
/********Name=CLKSEL_CON0,Offset=0x100********/
#define CORE_CLK_PLL_SEL          0x02060000U
#define CORE_CLK_PLL_SEL_APLL     0U
#define CORE_CLK_PLL_SEL_VPLL0    1U
#define CORE_CLK_PLL_SEL_VPLL1    2U
#define CORE_CLK_PLL_SEL_RESERVED 3U
/********Name=CLKSEL_CON1,Offset=0x104********/
#define DDRPHY_DFI_CLK4X_PLL_CLK_SEL          0x02060001U
#define DDRPHY_DFI_CLK4X_PLL_CLK_SEL_DPLL     0U
#define DDRPHY_DFI_CLK4X_PLL_CLK_SEL_VPLL0    1U
#define DDRPHY_DFI_CLK4X_PLL_CLK_SEL_VPLL1    2U
#define DDRPHY_DFI_CLK4X_PLL_CLK_SEL_RESERVED 3U
#define DDR_STDBY_CLK_SEL                     0x01080001U
#define DDR_STDBY_CLK_SEL_CLOCK               0U
/********Name=CLKSEL_CON2,Offset=0x108********/
#define CLK_32K_SEL                       0x02080002U
#define CLK_32K_SEL_CLK_32K_FROM_IO       0U
#define CLK_32K_SEL_CLK_32K_FROM_PMU_PVTM 1U
#define CLK_32K_SEL_CLK_32K_FRAC_DIV      2U
#define CLK_32K_SEL_CLK_32K_DIV           3U
#define CLK_32K_PLL_SEL                   0x010A0002U
#define CLK_32K_PLL_SEL_VPLL0             0U
#define CLK_32K_PLL_SEL_VPLL1             1U
/********Name=CLKSEL_CON3,Offset=0x10C********/
/********Name=CLKSEL_CON4,Offset=0x110********/
/********Name=CLKSEL_CON5,Offset=0x114********/
#define ACLK_HCLK_PCLK_BUS_PLL_SEL          0x02060005U
#define ACLK_HCLK_PCLK_BUS_PLL_SEL_DPLL     0U
#define ACLK_HCLK_PCLK_BUS_PLL_SEL_VPLL0    1U
#define ACLK_HCLK_PCLK_BUS_PLL_SEL_VPLL1    2U
#define ACLK_HCLK_PCLK_BUS_PLL_SEL_RESERVED 3U
/********Name=CLKSEL_CON6,Offset=0x118********/
/********Name=CLKSEL_CON7,Offset=0x11C********/
#define CLK_CRYPTO_PLL_SEL          0x02060007U
#define CLK_CRYPTO_PLL_SEL_DPLL     0U
#define CLK_CRYPTO_PLL_SEL_VPLL0    1U
#define CLK_CRYPTO_PLL_SEL_VPLL1    2U
#define CLK_CRYPTO_PLL_SEL_RESERVED 3U
#define CLK_CRYPTO_APK_SEL          0x020E0007U
#define CLK_CRYPTO_APK_SEL_DPLL     0U
#define CLK_CRYPTO_APK_SEL_VPLL0    1U
#define CLK_CRYPTO_APK_SEL_VPLL1    2U
#define CLK_CRYPTO_APK_SEL_RESERVED 3U
/********Name=CLKSEL_CON8,Offset=0x120********/
#define DCLK_VOP_PLL_SEL               0x020A0008U
#define DCLK_VOP_PLL_SEL_DPLL          0U
#define DCLK_VOP_PLL_SEL_VPLL0         1U
#define DCLK_VOP_PLL_SEL_VPLL1         2U
#define DCLK_VOP_PLL_SEL_RESERVED      3U
#define DCLK_VOP_SEL                   0x020E0008U
#define DCLK_VOP_SEL_DCLK_VOP          0U
#define DCLK_VOP_SEL_DCLK_VOP_FRAC_OUT 1U
#define DCLK_VOP_SEL_XIN_OSC0          2U
#define DCLK_VOP_SEL_RESERVED          3U
/********Name=CLKSEL_CON9,Offset=0x124********/
/********Name=CLKSEL_CON10,Offset=0x128********/
#define CLK_UART0_PLL_SEL            0x030D000AU
#define CLK_UART0_PLL_SEL_DPLL       0U
#define CLK_UART0_PLL_SEL_VPLL0      1U
#define CLK_UART0_PLL_SEL_VPLL1      2U
#define CLK_UART0_PLL_SEL_USBPHY480M 3U
#define CLK_UART0_PLL_SEL_XIN_OSC0   4U
#define CLK_UART0_PLL_SEL_RESERVED   5U
/********Name=CLKSEL_CON11,Offset=0x12C********/
#define CLK_UART0_SEL                    0x020E000BU
#define CLK_UART0_SEL_CLK_UART0          0U
#define CLK_UART0_SEL_CLK_UART0_NP5      1U
#define CLK_UART0_SEL_CLK_UART0_FRAC_OUT 2U
#define CLK_UART0_SEL_RESERVED           3U
/********Name=CLKSEL_CON12,Offset=0x130********/
/********Name=CLKSEL_CON13,Offset=0x134********/
#define CLK_UART1_PLL_SEL            0x030D000DU
#define CLK_UART1_PLL_SEL_DPLL       0U
#define CLK_UART1_PLL_SEL_VPLL0      1U
#define CLK_UART1_PLL_SEL_VPLL1      2U
#define CLK_UART1_PLL_SEL_USBPHY480M 3U
#define CLK_UART1_PLL_SEL_XIN_OSC0   4U
#define CLK_UART1_PLL_SEL_RESERVED   5U
/********Name=CLKSEL_CON14,Offset=0x138********/
#define CLK_UART1_SEL                    0x020E000EU
#define CLK_UART1_SEL_CLK_UART1          0U
#define CLK_UART1_SEL_CLK_UART1_NP5      1U
#define CLK_UART1_SEL_CLK_UART1_FRAC_OUT 2U
#define CLK_UART1_SEL_RESERVED           3U
/********Name=CLKSEL_CON15,Offset=0x13C********/
/********Name=CLKSEL_CON16,Offset=0x140********/
#define CLK_UART2_PLL_SEL            0x030D0010U
#define CLK_UART2_PLL_SEL_DPLL       0U
#define CLK_UART2_PLL_SEL_VPLL0      1U
#define CLK_UART2_PLL_SEL_VPLL1      2U
#define CLK_UART2_PLL_SEL_USBPHY480M 3U
#define CLK_UART2_PLL_SEL_XIN_OSC0   4U
#define CLK_UART2_PLL_SEL_RESERVED   5U
/********Name=CLKSEL_CON17,Offset=0x144********/
#define CLK_UART2_SEL                    0x020E0011U
#define CLK_UART2_SEL_CLK_UART2          0U
#define CLK_UART2_SEL_CLK_UART2_NP5      1U
#define CLK_UART2_SEL_CLK_UART2_FRAC_OUT 2U
#define CLK_UART2_SEL_RESERVED           3U
/********Name=CLKSEL_CON18,Offset=0x148********/
/********Name=CLKSEL_CON19,Offset=0x14C********/
#define CLK_UART3_PLL_SEL            0x030D0013U
#define CLK_UART3_PLL_SEL_DPLL       0U
#define CLK_UART3_PLL_SEL_VPLL0      1U
#define CLK_UART3_PLL_SEL_VPLL1      2U
#define CLK_UART3_PLL_SEL_USBPHY480M 3U
#define CLK_UART3_PLL_SEL_XIN_OSC0   4U
#define CLK_UART3_PLL_SEL_RESERVED   5U
/********Name=CLKSEL_CON20,Offset=0x150********/
#define CLK_UART3_SEL                    0x020E0014U
#define CLK_UART3_SEL_CLK_UART3          0U
#define CLK_UART3_SEL_CLK_UART3_NP5      1U
#define CLK_UART3_SEL_CLK_UART3_FRAC_OUT 2U
#define CLK_UART3_SEL_RESERVED           3U
/********Name=CLKSEL_CON21,Offset=0x154********/
/********Name=CLKSEL_CON22,Offset=0x158********/
#define CLK_UART4_PLL_SEL            0x030D0016U
#define CLK_UART4_PLL_SEL_DPLL       0U
#define CLK_UART4_PLL_SEL_VPLL0      1U
#define CLK_UART4_PLL_SEL_VPLL1      2U
#define CLK_UART4_PLL_SEL_USBPHY480M 3U
#define CLK_UART4_PLL_SEL_XIN_OSC0   4U
#define CLK_UART4_PLL_SEL_RESERVED   5U
/********Name=CLKSEL_CON23,Offset=0x15C********/
#define CLK_UART4_SEL                    0x020E0017U
#define CLK_UART4_SEL_CLK_UART4          0U
#define CLK_UART4_SEL_CLK_UART4_NP5      1U
#define CLK_UART4_SEL_CLK_UART4_FRAC_OUT 2U
#define CLK_UART4_SEL_RESERVED           3U
/********Name=CLKSEL_CON24,Offset=0x160********/
/********Name=CLKSEL_CON25,Offset=0x164********/
#define CLK_I2C0_PLL_SEL          0x020E0019U
#define CLK_I2C0_PLL_SEL_DPLL     0U
#define CLK_I2C0_PLL_SEL_VPLL0    1U
#define CLK_I2C0_PLL_SEL_XIN_OSC0 2U
#define CLK_I2C0_PLL_SEL_RESERVED 3U
/********Name=CLKSEL_CON26,Offset=0x168********/
#define CLK_I2C1_PLL_SEL          0x020E001AU
#define CLK_I2C1_PLL_SEL_DPLL     0U
#define CLK_I2C1_PLL_SEL_VPLL0    1U
#define CLK_I2C1_PLL_SEL_XIN_OSC0 2U
#define CLK_I2C1_PLL_SEL_RESERVED 3U
/********Name=CLKSEL_CON27,Offset=0x16C********/
#define CLK_I2C2_PLL_SEL          0x020E001BU
#define CLK_I2C2_PLL_SEL_DPLL     0U
#define CLK_I2C2_PLL_SEL_VPLL0    1U
#define CLK_I2C2_PLL_SEL_XIN_OSC0 2U
#define CLK_I2C2_PLL_SEL_RESERVED 3U
/********Name=CLKSEL_CON28,Offset=0x170********/
#define CLK_I2C3_PLL_SEL          0x020E001CU
#define CLK_I2C3_PLL_SEL_DPLL     0U
#define CLK_I2C3_PLL_SEL_VPLL0    1U
#define CLK_I2C3_PLL_SEL_XIN_OSC0 2U
#define CLK_I2C3_PLL_SEL_RESERVED 3U
/********Name=CLKSEL_CON29,Offset=0x174********/
#define CLK_PWM_PLL_SEL          0x020E001DU
#define CLK_PWM_PLL_SEL_DPLL     0U
#define CLK_PWM_PLL_SEL_VPLL0    1U
#define CLK_PWM_PLL_SEL_XIN_OSC0 2U
#define CLK_PWM_PLL_SEL_RESERVED 3U
/********Name=CLKSEL_CON30,Offset=0x178********/
#define CLK_SPI0_PLL_SEL          0x020E001EU
#define CLK_SPI0_PLL_SEL_DPLL     0U
#define CLK_SPI0_PLL_SEL_VPLL0    1U
#define CLK_SPI0_PLL_SEL_XIN_OSC0 2U
#define CLK_SPI0_PLL_SEL_RESERVED 3U
/********Name=CLKSEL_CON31,Offset=0x17C********/
#define CLK_SPI1_PLL_SEL          0x020E001FU
#define CLK_SPI1_PLL_SEL_DPLL     0U
#define CLK_SPI1_PLL_SEL_VPLL0    1U
#define CLK_SPI1_PLL_SEL_XIN_OSC0 2U
#define CLK_SPI1_PLL_SEL_RESERVED 3U
/********Name=CLKSEL_CON32,Offset=0x180********/
#define CLK_SPI2_PLL_SEL          0x020E0020U
#define CLK_SPI2_PLL_SEL_DPLL     0U
#define CLK_SPI2_PLL_SEL_VPLL0    1U
#define CLK_SPI2_PLL_SEL_XIN_OSC0 2U
#define CLK_SPI2_PLL_SEL_RESERVED 3U
/********Name=CLKSEL_CON33,Offset=0x184********/
/********Name=CLKSEL_CON34,Offset=0x188********/
/********Name=CLKSEL_CON35,Offset=0x18C********/
/********Name=CLKSEL_CON36,Offset=0x190********/
#define ACLK_HCLK_PCLK_PERI_PLL_SEL          0x02060024U
#define ACLK_HCLK_PCLK_PERI_PLL_SEL_DPLL     0U
#define ACLK_HCLK_PCLK_PERI_PLL_SEL_VPLL0    1U
#define ACLK_HCLK_PCLK_PERI_PLL_SEL_VPLL1    2U
#define ACLK_HCLK_PCLK_PERI_PLL_SEL_RESERVED 3U
/********Name=CLKSEL_CON37,Offset=0x194********/
/********Name=CLKSEL_CON38,Offset=0x198********/
#define CLK_NANDC_PLL_SEL          0x02060026U
#define CLK_NANDC_PLL_SEL_DPLL     0U
#define CLK_NANDC_PLL_SEL_VPLL0    1U
#define CLK_NANDC_PLL_SEL_VPLL1    2U
#define CLK_NANDC_PLL_SEL_RESERVED 3U
#define CLK_NANDC_SEL              0x010F0026U
#define CLK_NANDC_SEL_EVEN         0U
#define CLK_NANDC_SEL_DUTY50       1U
/********Name=CLKSEL_CON39,Offset=0x19C********/
#define CLK_SDMMC_PLL_SEL          0x02080027U
#define CLK_SDMMC_PLL_SEL_DPLL     0U
#define CLK_SDMMC_PLL_SEL_VPLL0    1U
#define CLK_SDMMC_PLL_SEL_VPLL1    2U
#define CLK_SDMMC_PLL_SEL_XIN_OSC0 3U
#define CLK_SDMMC_SEL              0x010F0027U
#define CLK_SDMMC_SEL_EVEN         0U
#define CLK_SDMMC_SEL_DUTY50       1U
/********Name=CLKSEL_CON40,Offset=0x1A0********/
#define CLK_SDIO_PLL_SEL          0x02080028U
#define CLK_SDIO_PLL_SEL_DPLL     0U
#define CLK_SDIO_PLL_SEL_VPLL0    1U
#define CLK_SDIO_PLL_SEL_VPLL1    2U
#define CLK_SDIO_PLL_SEL_XIN_OSC0 3U
#define CLK_SDIO_SEL              0x010F0028U
#define CLK_SDIO_SEL_EVEN         0U
#define CLK_SDIO_SEL_DUTY50       1U
/********Name=CLKSEL_CON41,Offset=0x1A4********/
#define CLK_EMMC_PLL_SEL          0x02080029U
#define CLK_EMMC_PLL_SEL_DPLL     0U
#define CLK_EMMC_PLL_SEL_VPLL0    1U
#define CLK_EMMC_PLL_SEL_VPLL1    2U
#define CLK_EMMC_PLL_SEL_XIN_OSC0 3U
#define CLK_EMMC_SEL              0x010F0029U
#define CLK_EMMC_SEL_EVEN         0U
#define CLK_EMMC_SEL_DUTY50       1U
/********Name=CLKSEL_CON42,Offset=0x1A8********/
#define CLK_SFC_PLL_SEL       0x020E002AU
#define CLK_SFC_PLL_SEL_DPLL  0U
#define CLK_SFC_PLL_SEL_VPLL0 1U
#define CLK_SFC_PLL_SEL_VPLL1 2U
/********Name=CLKSEL_CON43,Offset=0x1AC********/
#define CLK_MAC_PLL_SEL          0x0206002BU
#define CLK_MAC_PLL_SEL_DPLL     0U
#define CLK_MAC_PLL_SEL_VPLL0    1U
#define CLK_MAC_PLL_SEL_VPLL1    2U
#define CLK_MAC_PLL_SEL_RESERVED 3U
#define RMII_EXTCLKSRC_SEL       0x010E002BU
#define RMII_EXTCLKSRC_SEL_MAC   0U
#define RMII_CLK_SEL             0x010F002BU
#define RMII_CLK_SEL_10M         0U
#define RMII_CLK_SEL_100M        1U
/********Name=CLKSEL_CON44,Offset=0x1B0********/
#define CLK_WIFI_PLL_SEL           0x0106002CU
#define CLK_WIFI_PLL_SEL_DPLL      0U
#define CLK_WIFI_PLL_SEL_VPLL0     1U
#define CLK_WIFI_SEL               0x0107002CU
#define CLK_WIFI_SEL_CLK_WIFI_OUT  0U
#define CLK_OWIRE_PLL_SEL          0x020E002CU
#define CLK_OWIRE_PLL_SEL_DPLL     0U
#define CLK_OWIRE_PLL_SEL_VPLL0    1U
#define CLK_OWIRE_PLL_SEL_XIN_OSC0 2U
#define CLK_OWIRE_PLL_SEL_RESERVED 3U
/********Name=CLKSEL_CON45,Offset=0x1B4********/
#define HCLK_PCLK_AUDIO_PLL_SEL          0x0206002DU
#define HCLK_PCLK_AUDIO_PLL_SEL_VPLL0    0U
#define HCLK_PCLK_AUDIO_PLL_SEL_VPLL1    1U
#define HCLK_PCLK_AUDIO_PLL_SEL_XIN_OSC0 2U
#define HCLK_PCLK_AUDIO_PLL_SEL_RESERVED 3U
/********Name=CLKSEL_CON46,Offset=0x1B8********/
#define CLK_PDM_PLL_SEL              0x0208002EU
#define CLK_PDM_PLL_SEL_VPLL0        0U
#define CLK_PDM_PLL_SEL_VPLL1        1U
#define CLK_PDM_PLL_SEL_XIN_OSC0     2U
#define CLK_PDM_PLL_SEL_RESERVED     3U
#define CLK_PDM_SEL                  0x010F002EU
#define CLK_PDM_SEL_CLK_PDM          0U
#define CLK_PDM_SEL_CLK_PDM_FRAC_OUT 1U
/********Name=CLKSEL_CON47,Offset=0x1BC********/
/********Name=CLKSEL_CON48,Offset=0x1C0********/
#define CLK_SPDIFTX_PLL_SEL                  0x02080030U
#define CLK_SPDIFTX_PLL_SEL_VPLL0            0U
#define CLK_SPDIFTX_PLL_SEL_VPLL1            1U
#define CLK_SPDIFTX_PLL_SEL_XIN_OSC0         2U
#define CLK_SPDIFTX_PLL_SEL_RESERVED         3U
#define CLK_SPDIFTX_SRC_SEL                  0x010C0030U
#define CLK_SPDIFTX_SEL_EVEN                 0U
#define CLK_SPDIFTX_SEL_DUTY50               1U
#define CLK_SPDIFTX_SEL                      0x020E0030U
#define CLK_SPDIFTX_SEL_CLK_SPDIFTX          0U
#define CLK_SPDIFTX_SEL_CLK_SPDIFTX_FRAC_OUT 1U
#define CLK_SPDIFTX_SEL_MCLK_FROM_IO         2U
/********Name=CLKSEL_CON49,Offset=0x1C4********/
/********Name=CLKSEL_CON50,Offset=0x1C8********/
#define CLK_SPDIFRX_PLL_SEL                  0x02080032U
#define CLK_SPDIFRX_PLL_SEL_VPLL0            0U
#define CLK_SPDIFRX_PLL_SEL_VPLL1            1U
#define CLK_SPDIFRX_PLL_SEL_XIN_OSC0         2U
#define CLK_SPDIFRX_PLL_SEL_RESERVED         3U
#define CLK_SPDIFRX_SRC_SEL                  0x010E0032U
#define CLK_SPDIFRX_SEL_EVEN                 0U
#define CLK_SPDIFRX_SEL_DUTY50               1U
#define CLK_SPDIFRX_SEL                      0x010F0032U
#define CLK_SPDIFRX_SEL_CLK_SPDIFRX          0U
#define CLK_SPDIFRX_SEL_CLK_SPDIFRX_FRAC_OUT 1U
/********Name=CLKSEL_CON51,Offset=0x1CC********/
/********Name=CLKSEL_CON52,Offset=0x1D0********/
#define CLK_I2S0_8CH_TX_PLL_SEL                                 0x02080034U
#define CLK_I2S0_8CH_TX_PLL_SEL_VPLL0                           0U
#define CLK_I2S0_8CH_TX_PLL_SEL_VPLL1                           1U
#define CLK_I2S0_8CH_TX_PLL_SEL_XIN_OSC0                        2U
#define CLK_I2S0_8CH_TX_PLL_SEL_RESERVED                        3U
#define CLK_I2S0_8CH_TX_SEL                                     0x020A0034U
#define CLK_I2S0_8CH_TX_SEL_CLK_I2S0_8CH_TX                     0U
#define CLK_I2S0_8CH_TX_SEL_CLK_I2S0_8CH_TX_FRAC_OUT            1U
#define CLK_I2S0_8CH_TX_SEL_MCLK_I2S0_8CH_TX_IN                 2U
#define CLK_I2S0_8CH_TX_SEL_RESERVED                            3U
#define CLK_I2S0_8CH_TX_RX_CLK_SEL                              0x010C0034U
#define CLK_I2S0_8CH_TX_RX_CLK_SEL_CLK_I2S0_8CH_TX_CLK          0U
#define CLK_I2S0_8CH_TX_RX_CLK_SEL_CLK_I2S0_8CH_RX_CLK          1U
#define CLK_I2S0_8CH_TX_OUT_MCLK_SEL                            0x010F0034U
#define CLK_I2S0_8CH_TX_OUT_MCLK_SEL_CLK_I2S0_8CH_TX_RX_CLK_SEL 0U
#define CLK_I2S0_8CH_TX_OUT_MCLK_SEL_XIN_OSC0_HALF              1U
/********Name=CLKSEL_CON53,Offset=0x1D4********/
/********Name=CLKSEL_CON54,Offset=0x1D8********/
#define CLK_I2S0_8CH_RX_PLL_SEL                        0x02080036U
#define CLK_I2S0_8CH_RX_PLL_SEL_VPLL0                  0U
#define CLK_I2S0_8CH_RX_PLL_SEL_VPLL1                  1U
#define CLK_I2S0_8CH_RX_PLL_SEL_XIN_OSC0               2U
#define CLK_I2S0_8CH_RX_PLL_SEL_RESERVED               3U
#define CLK_I2S0_8CH_RX_SEL                            0x020A0036U
#define CLK_I2S0_8CH_RX_SEL_CLK_I2S0_8CH_RX            0U
#define CLK_I2S0_8CH_RX_SEL_CLK_I2S0_8CH_RX_FRAC_OUT   1U
#define CLK_I2S0_8CH_RX_SEL_MCLK_I2S0_8CH_RX_IN        2U
#define CLK_I2S0_8CH_RX_SEL_RESERVED                   3U
#define CLK_I2S0_8CH_RX_TX_CLK_SEL                     0x010C0036U
#define CLK_I2S0_8CH_RX_TX_CLK_SEL_CLK_I2S0_8CH_RX_CLK 0U
#define CLK_I2S0_8CH_RX_TX_CLK_SEL_CLK_I2S0_8CH_TX_CLK 1U
/********Name=CLKSEL_CON55,Offset=0x1DC********/
/********Name=CLKSEL_CON56,Offset=0x1E0********/
#define CLK_I2S1_8CH_TX_PLL_SEL                                 0x02080038U
#define CLK_I2S1_8CH_TX_PLL_SEL_VPLL0                           0U
#define CLK_I2S1_8CH_TX_PLL_SEL_VPLL1                           1U
#define CLK_I2S1_8CH_TX_PLL_SEL_XIN_OSC0                        2U
#define CLK_I2S1_8CH_TX_PLL_SEL_RESERVED                        3U
#define CLK_I2S1_8CH_TX_SEL                                     0x020A0038U
#define CLK_I2S1_8CH_TX_SEL_CLK_I2S1_8CH_TX                     0U
#define CLK_I2S1_8CH_TX_SEL_CLK_I2S1_8CH_TX_FRAC_OUT            1U
#define CLK_I2S1_8CH_TX_SEL_MCLK_I2S1_8CH_TX_IN                 2U
#define CLK_I2S1_8CH_TX_SEL_RESERVED                            3U
#define CLK_I2S1_8CH_TX_RX_CLK_SEL                              0x010C0038U
#define CLK_I2S1_8CH_TX_RX_CLK_SEL_CLK_I2S1_8CH_TX_CLK          0U
#define CLK_I2S1_8CH_TX_RX_CLK_SEL_CLK_I2S1_8CH_RX_CLK          1U
#define CLK_I2S1_8CH_TX_OUT_MCLK_SEL                            0x010F0038U
#define CLK_I2S1_8CH_TX_OUT_MCLK_SEL_CLK_I2S1_8CH_TX_RX_CLK_SEL 0U
#define CLK_I2S1_8CH_TX_OUT_MCLK_SEL_XIN_OSC0_HALF              1U
/********Name=CLKSEL_CON57,Offset=0x1E4********/
/********Name=CLKSEL_CON58,Offset=0x1E8********/
#define CLK_I2S1_8CH_RX_PLL_SEL                        0x0208003AU
#define CLK_I2S1_8CH_RX_PLL_SEL_VPLL0                  0U
#define CLK_I2S1_8CH_RX_PLL_SEL_VPLL1                  1U
#define CLK_I2S1_8CH_RX_PLL_SEL_XIN_OSC0               2U
#define CLK_I2S1_8CH_RX_PLL_SEL_RESERVED               3U
#define CLK_I2S1_8CH_RX_SEL                            0x020A003AU
#define CLK_I2S1_8CH_RX_SEL_CLK_I2S1_8CH_RX            0U
#define CLK_I2S1_8CH_RX_SEL_CLK_I2S1_8CH_RX_FRAC_OUT   1U
#define CLK_I2S1_8CH_RX_SEL_MCLK_I2S1_8CH_RX_IN        2U
#define CLK_I2S1_8CH_RX_SEL_RESERVED                   3U
#define CLK_I2S1_8CH_RX_TX_CLK_SEL                     0x010C003AU
#define CLK_I2S1_8CH_RX_TX_CLK_SEL_CLK_I2S1_8CH_RX_CLK 0U
#define CLK_I2S1_8CH_RX_TX_CLK_SEL_CLK_I2S1_8CH_TX_CLK 1U
/********Name=CLKSEL_CON59,Offset=0x1EC********/
/********Name=CLKSEL_CON60,Offset=0x1F0********/
#define CLK_I2S2_8CH_TX_PLL_SEL                                 0x0208003CU
#define CLK_I2S2_8CH_TX_PLL_SEL_VPLL0                           0U
#define CLK_I2S2_8CH_TX_PLL_SEL_VPLL1                           1U
#define CLK_I2S2_8CH_TX_PLL_SEL_XIN_OSC0                        2U
#define CLK_I2S2_8CH_TX_PLL_SEL_RESERVED                        3U
#define CLK_I2S2_8CH_TX_SEL                                     0x020A003CU
#define CLK_I2S2_8CH_TX_SEL_CLK_I2S2_8CH_TX                     0U
#define CLK_I2S2_8CH_TX_SEL_CLK_I2S2_8CH_TX_FRAC_OUT            1U
#define CLK_I2S2_8CH_TX_SEL_MCLK_I2S2_8CH_TX_IN                 2U
#define CLK_I2S2_8CH_TX_SEL_RESERVED                            3U
#define CLK_I2S2_8CH_TX_RX_CLK_SEL                              0x010C003CU
#define CLK_I2S2_8CH_TX_RX_CLK_SEL_CLK_I2S2_8CH_TX_CLK          0U
#define CLK_I2S2_8CH_TX_RX_CLK_SEL_CLK_I2S2_8CH_RX_CLK          1U
#define CLK_I2S2_8CH_TX_OUT_MCLK_SEL                            0x010F003CU
#define CLK_I2S2_8CH_TX_OUT_MCLK_SEL_CLK_I2S2_8CH_TX_RX_CLK_SEL 0U
#define CLK_I2S2_8CH_TX_OUT_MCLK_SEL_XIN_OSC0_HALF              1U
/********Name=CLKSEL_CON61,Offset=0x1F4********/
/********Name=CLKSEL_CON62,Offset=0x1F8********/
#define CLK_I2S2_8CH_RX_PLL_SEL                        0x0208003EU
#define CLK_I2S2_8CH_RX_PLL_SEL_VPLL0                  0U
#define CLK_I2S2_8CH_RX_PLL_SEL_VPLL1                  1U
#define CLK_I2S2_8CH_RX_PLL_SEL_XIN_OSC0               2U
#define CLK_I2S2_8CH_RX_PLL_SEL_RESERVED               3U
#define CLK_I2S2_8CH_RX_SEL                            0x020A003EU
#define CLK_I2S2_8CH_RX_SEL_CLK_I2S2_8CH_RX            0U
#define CLK_I2S2_8CH_RX_SEL_CLK_I2S2_8CH_RX_FRAC_OUT   1U
#define CLK_I2S2_8CH_RX_SEL_MCLK_I2S2_8CH_RX_IN        2U
#define CLK_I2S2_8CH_RX_SEL_RESERVED                   3U
#define CLK_I2S2_8CH_RX_TX_CLK_SEL                     0x010C003EU
#define CLK_I2S2_8CH_RX_TX_CLK_SEL_CLK_I2S2_8CH_RX_CLK 0U
#define CLK_I2S2_8CH_RX_TX_CLK_SEL_CLK_I2S2_8CH_TX_CLK 1U
/********Name=CLKSEL_CON63,Offset=0x1FC********/
/********Name=CLKSEL_CON64,Offset=0x200********/
#define CLK_I2S3_8CH_TX_PLL_SEL                                 0x02080040U
#define CLK_I2S3_8CH_TX_PLL_SEL_VPLL0                           0U
#define CLK_I2S3_8CH_TX_PLL_SEL_VPLL1                           1U
#define CLK_I2S3_8CH_TX_PLL_SEL_XIN_OSC0                        2U
#define CLK_I2S3_8CH_TX_PLL_SEL_RESERVED                        3U
#define CLK_I2S3_8CH_TX_SEL                                     0x020A0040U
#define CLK_I2S3_8CH_TX_SEL_CLK_I2S3_8CH_TX                     0U
#define CLK_I2S3_8CH_TX_SEL_CLK_I2S3_8CH_TX_FRAC_OUT            1U
#define CLK_I2S3_8CH_TX_SEL_MCLK_I2S3_8CH_TX_IN                 2U
#define CLK_I2S3_8CH_TX_SEL_RESERVED                            3U
#define CLK_I2S3_8CH_TX_RX_CLK_SEL                              0x010C0040U
#define CLK_I2S3_8CH_TX_RX_CLK_SEL_CLK_I2S3_8CH_TX_CLK          0U
#define CLK_I2S3_8CH_TX_RX_CLK_SEL_CLK_I2S3_8CH_RX_CLK          1U
#define CLK_I2S3_8CH_TX_OUT_MCLK_SEL                            0x010F0040U
#define CLK_I2S3_8CH_TX_OUT_MCLK_SEL_CLK_I2S3_8CH_TX_RX_CLK_SEL 0U
#define CLK_I2S3_8CH_TX_OUT_MCLK_SEL_XIN_OSC0_HALF              1U
/********Name=CLKSEL_CON65,Offset=0x204********/
/********Name=CLKSEL_CON66,Offset=0x208********/
#define CLK_I2S3_8CH_RX_PLL_SEL                        0x02080042U
#define CLK_I2S3_8CH_RX_PLL_SEL_VPLL0                  0U
#define CLK_I2S3_8CH_RX_PLL_SEL_VPLL1                  1U
#define CLK_I2S3_8CH_RX_PLL_SEL_XIN_OSC0               2U
#define CLK_I2S3_8CH_RX_PLL_SEL_RESERVED               3U
#define CLK_I2S3_8CH_RX_SEL                            0x020A0042U
#define CLK_I2S3_8CH_RX_SEL_CLK_I2S3_8CH_RX            0U
#define CLK_I2S3_8CH_RX_SEL_CLK_I2S3_8CH_RX_FRAC_OUT   1U
#define CLK_I2S3_8CH_RX_SEL_MCLK_I2S3_8CH_RX_IN        2U
#define CLK_I2S3_8CH_RX_SEL_RESERVED                   3U
#define CLK_I2S3_8CH_RX_TX_CLK_SEL                     0x010C0042U
#define CLK_I2S3_8CH_RX_TX_CLK_SEL_CLK_I2S3_8CH_RX_CLK 0U
#define CLK_I2S3_8CH_RX_TX_CLK_SEL_CLK_I2S3_8CH_TX_CLK 1U
/********Name=CLKSEL_CON67,Offset=0x20C********/
/********Name=CLKSEL_CON68,Offset=0x210********/
#define CLK_I2S0_2CH_PLL_SEL                       0x02080044U
#define CLK_I2S0_2CH_PLL_SEL_VPLL0                 0U
#define CLK_I2S0_2CH_PLL_SEL_VPLL1                 1U
#define CLK_I2S0_2CH_PLL_SEL_XIN_OSC0              2U
#define CLK_I2S0_2CH_PLL_SEL_RESERVED              3U
#define CLK_I2S0_2CH_SEL                           0x020A0044U
#define CLK_I2S0_2CH_SEL_CLK_I2S0_2CH              0U
#define CLK_I2S0_2CH_SEL_CLK_I2S0_2CH_FRAC_OUT     1U
#define CLK_I2S0_2CH_SEL_MCLK_I2S0_2CH_IN          2U
#define CLK_I2S0_2CH_SEL_RESERVED                  3U
#define CLK_I2S0_2CH_OUT_MCLK_SEL                  0x010F0044U
#define CLK_I2S0_2CH_OUT_MCLK_SEL_CLK_I2S0_2CH_SEL 0U
#define CLK_I2S0_2CH_OUT_MCLK_SEL_XIN_OSC0_HALF    1U
/********Name=CLKSEL_CON69,Offset=0x214********/
/********Name=CLKSEL_CON70,Offset=0x218********/
#define CLK_I2S1_2CH_PLL_SEL                       0x02080046U
#define CLK_I2S1_2CH_PLL_SEL_VPLL0                 0U
#define CLK_I2S1_2CH_PLL_SEL_VPLL1                 1U
#define CLK_I2S1_2CH_PLL_SEL_XIN_OSC0              2U
#define CLK_I2S1_2CH_PLL_SEL_RESERVED              3U
#define CLK_I2S1_2CH_SEL                           0x020A0046U
#define CLK_I2S1_2CH_SEL_CLK_I2S1_2CH              0U
#define CLK_I2S1_2CH_SEL_CLK_I2S1_2CH_FRAC_OUT     1U
#define CLK_I2S1_2CH_SEL_MCLK_I2S1_2CH_IN          2U
#define CLK_I2S1_2CH_SEL_RESERVED                  3U
#define CLK_I2S1_2CH_OUT_MCLK_SEL                  0x010F0046U
#define CLK_I2S1_2CH_OUT_MCLK_SEL_CLK_I2S1_2CH_SEL 0U
#define CLK_I2S1_2CH_OUT_MCLK_SEL_XIN_OSC0_HALF    1U
/********Name=CLKSEL_CON71,Offset=0x21C********/
/********Name=CLKSEL_CON72,Offset=0x220********/
#define CLK_REF24M_PLL_SEL       0x01060048U
#define CLK_REF24M_PLL_SEL_DPLL  0U
#define CLK_REF24M_PLL_SEL_VPLL0 1U
#define USBPHY_REF_SEL           0x01070048U
#define USBPHY_REF_SEL_CLOCK     0U
/********Name=CLKSEL_CON73,Offset=0x224********/
#define TESTCLK_SEL                   0x05080049U
#define TESTCLK_SEL_XIN_OSC0          0U
#define TESTCLK_SEL_CLK_CORE          1U
#define TESTCLK_SEL_CLK_DDRPHY4X      2U
#define TESTCLK_SEL_CLK_DDRPHY1X      3U
#define TESTCLK_SEL_ACLK_PERI         4U
#define TESTCLK_SEL_HCLK_PERI         5U
#define TESTCLK_SEL_CLK_NANDC         6U
#define TESTCLK_SEL_CLK_SDMMC         7U
#define TESTCLK_SEL_CLK_SDIO          8U
#define TESTCLK_SEL_CLK_EMMC          9U
#define TESTCLK_SEL_CLK_SFC           10U
#define TESTCLK_SEL_CLK_MAC_REF       11U
#define TESTCLK_SEL_ACLK_BUS          12U
#define TESTCLK_SEL_HCLK_BUS          13U
#define TESTCLK_SEL_CLK_CRYPTO        14U
#define TESTCLK_SEL_CLK_CRYPTO_APK    15U
#define TESTCLK_SEL_DCLK_VOP          16U
#define TESTCLK_SEL_CLK_UART0         17U
#define TESTCLK_SEL_CLK_I2C0          18U
#define TESTCLK_SEL_CLK_SPI0          19U
#define TESTCLK_SEL_CLK_TSADC         20U
#define TESTCLK_SEL_CLK_SARADC        21U
#define TESTCLK_SEL_CLK_OTP           22U
#define TESTCLK_SEL_HCLK_AUDIO        23U
#define TESTCLK_SEL_CLK_PDM           24U
#define TESTCLK_SEL_CLK_SPDIFRX       25U
#define TESTCLK_SEL_CLK_I2S0_8CH_TX   26U
#define TESTCLK_SEL_CLK_I2S0_8CH_RX   27U
#define TESTCLK_SEL_CLK_I2S0_2CH      28U
#define TESTCLK_SEL_CLK_RTC32K        29U
#define TESTCLK_SEL_CLK_USBPHY_REF24M 30U
#define TESTCLK_SEL_OTP_ISP_OSC_OUT   31U
/********Name=CLKSEL_CON74,Offset=0x228********/
#define CLK_PWM1_PLL_SEL          0x020E004AU
#define CLK_PWM1_PLL_SEL_DPLL     0U
#define CLK_PWM1_PLL_SEL_VPLL0    1U
#define CLK_PWM1_PLL_SEL_XIN_OSC0 2U
#define CLK_PWM1_PLL_SEL_RESERVED 3U
/********Name=CLKSEL_CON75,Offset=0x22C********/
#define CLK_PWM2_PLL_SEL          0x020E004BU
#define CLK_PWM2_PLL_SEL_DPLL     0U
#define CLK_PWM2_PLL_SEL_VPLL0    1U
#define CLK_PWM2_PLL_SEL_XIN_OSC0 2U
#define CLK_PWM2_PLL_SEL_RESERVED 3U

#define FAKE_CLOCK_GATE	          14U
#define PLL_INPUT_OSC_RATE (24 * 1000 * 1000)

#define CLK(mux, div) \
    (((mux) & 0x0F0F00FFU) | (((div) & 0xFFU) << 8) | (((div) & 0x0F0F0000U) << 4))

#ifndef __ASSEMBLY__
typedef enum CLOCK_Name {
    CLK_INVALID = 0U,
    PLL_APLL,
    PLL_DPLL,
    PLL_VPLL0,
    PLL_VPLL1,
    PCLK_DBG             = CLK(0U, CORE_DBG_DIV),
    ACLK_BUS             = CLK(ACLK_HCLK_PCLK_BUS_PLL_SEL, ACLK_BUS_DIV),
    HCLK_BUS             = CLK(ACLK_HCLK_PCLK_BUS_PLL_SEL, HCLK_BUS_DIV),
    PCLK_BUS             = CLK(ACLK_HCLK_PCLK_BUS_PLL_SEL, PCLK_BUS_DIV),
    CLK_CRYPTO           = CLK(CLK_CRYPTO_PLL_SEL, CLK_CRYPTO_DIV),
    CLK_CRYPTO_APK       = CLK(CLK_CRYPTO_APK_SEL, CLK_CRYPTO_APK_DIV),
    DCLK_VOP             = CLK(DCLK_VOP_PLL_SEL, DCLK_VOP_DIV),
    CLK_UART0_SRC        = CLK(CLK_UART0_PLL_SEL, CLK_UART0_DIV),
    CLK_UART0_FRAC       = CLK(0U, CLK_UART0_FRAC_DIV),
    CLK_UART0            = CLK(CLK_UART0_SEL, 0U),
    CLK_UART1_SRC        = CLK(CLK_UART1_PLL_SEL, CLK_UART1_DIV),
    CLK_UART1_FRAC       = CLK(0U, CLK_UART1_FRAC_DIV),
    CLK_UART1            = CLK(CLK_UART1_SEL, 0U),
    CLK_UART2_SRC        = CLK(CLK_UART2_PLL_SEL, CLK_UART2_DIV),
    CLK_UART2_FRAC       = CLK(0U, CLK_UART2_FRAC_DIV),
    CLK_UART2            = CLK(CLK_UART2_SEL, 0U),
    CLK_UART3_SRC        = CLK(CLK_UART3_PLL_SEL, CLK_UART3_DIV),
    CLK_UART3_FRAC       = CLK(0U, CLK_UART3_FRAC_DIV),
    CLK_UART3            = CLK(CLK_UART3_SEL, 0U),
    CLK_UART4_SRC        = CLK(CLK_UART4_PLL_SEL, CLK_UART4_DIV),
    CLK_UART4_FRAC       = CLK(0U, CLK_UART4_FRAC_DIV),
    CLK_UART4            = CLK(CLK_UART4_SEL, 0U),
    CLK_I2C0             = CLK(CLK_I2C0_PLL_SEL, CLK_I2C0_DIV),
    CLK_I2C1             = CLK(CLK_I2C1_PLL_SEL, CLK_I2C1_DIV),
    CLK_I2C2             = CLK(CLK_I2C2_PLL_SEL, CLK_I2C2_DIV),
    CLK_I2C3             = CLK(CLK_I2C3_PLL_SEL, CLK_I2C3_DIV),
    CLK_PWM              = CLK(CLK_PWM_PLL_SEL, CLK_PWM_DIV),
    CLK_PWM1             = CLK(CLK_PWM1_PLL_SEL, CLK_PWM1_DIV),
    CLK_PWM2             = CLK(CLK_PWM2_PLL_SEL, CLK_PWM2_DIV),
    CLK_SPI0             = CLK(CLK_SPI0_PLL_SEL, CLK_SPI0_DIV),
    CLK_SPI1             = CLK(CLK_SPI1_PLL_SEL, CLK_SPI1_DIV),
    CLK_SPI2             = CLK(CLK_SPI2_PLL_SEL, CLK_SPI2_DIV),
    CLK_TSADC            = CLK(0U, CLK_TSADC_DIV),
    CLK_SARADC           = CLK(0U, CLK_SARADC_DIV),
    CLK_OTP              = CLK(0U, CLK_OTP_DIV),
    CLK_OTP_USR          = CLK(0U, CLK_OTP_USR_DIV),
    ACLK_PERI            = CLK(ACLK_HCLK_PCLK_PERI_PLL_SEL, ACLK_PERI_DIV),
    HCLK_PERI            = CLK(ACLK_HCLK_PCLK_PERI_PLL_SEL, HCLK_PERI_DIV),
    PCLK_PERI            = CLK(ACLK_HCLK_PCLK_PERI_PLL_SEL, PCLK_PERI_DIV),
    CLK_NANDC            = CLK(CLK_NANDC_PLL_SEL, CLK_NANDC_DIV),
    CLK_SDMMC            = CLK(CLK_SDMMC_PLL_SEL, CLK_SDMMC_DIV),
    CLK_SDIO             = CLK(CLK_SDIO_PLL_SEL, CLK_SDIO_DIV),
    CLK_EMMC             = CLK(CLK_EMMC_PLL_SEL, CLK_EMMC_DIV),
    CLK_SFC              = CLK(CLK_SFC_PLL_SEL, CLK_SFC_DIV),
    HCLK_AUDIO           = CLK(HCLK_PCLK_AUDIO_PLL_SEL, HCLK_AUDIO_DIV),
    PCLK_AUDIO           = CLK(HCLK_PCLK_AUDIO_PLL_SEL, PCLK_AUDIO_DIV),
    CLK_PDM_SRC          = CLK(CLK_PDM_PLL_SEL, CLK_PDM_DIV),
    CLK_PDM_FRAC         = CLK(0U, CLK_PDM_FRAC_DIV),
    CLK_PDM              = CLK(CLK_PDM_SEL, 0U),
    CLK_SPDIFTX_SRC      = CLK(CLK_SPDIFTX_PLL_SEL, CLK_SPDIFTX_DIV),
    CLK_SPDIFTX_FRAC     = CLK(0U, CLK_SPDIFTX_FRAC_DIV),
    CLK_SPDIFTX          = CLK(CLK_SPDIFTX_SEL, 0U),
    CLK_SPDIFRX_SRC      = CLK(CLK_SPDIFRX_PLL_SEL, CLK_SPDIFRX_DIV),
    CLK_SPDIFRX_FRAC     = CLK(0U, CLK_SPDIFRX_FRAC_DIV),
    CLK_SPDIFRX          = CLK(CLK_SPDIFRX_SEL, 0U),
    CLK_I2S0_8CH_TX_SRC  = CLK(CLK_I2S0_8CH_TX_PLL_SEL, CLK_I2S0_8CH_TX_DIV),
    CLK_I2S0_8CH_TX_FRAC = CLK(0U, CLK_I2S0_8CH_TX_FRAC_DIV),
    CLK_I2S0_8CH_TX      = CLK(CLK_I2S0_8CH_TX_SEL, 0U),
    CLK_I2S0_8CH_RX_SRC  = CLK(CLK_I2S0_8CH_RX_PLL_SEL, CLK_I2S0_8CH_RX_DIV),
    CLK_I2S0_8CH_RX_FRAC = CLK(0U, CLK_I2S0_8CH_RX_FRAC_DIV),
    CLK_I2S0_8CH_RX      = CLK(CLK_I2S0_8CH_RX_SEL, 0U),
    CLK_I2S1_8CH_TX_SRC  = CLK(CLK_I2S1_8CH_TX_PLL_SEL, CLK_I2S1_8CH_TX_DIV),
    CLK_I2S1_8CH_TX_FRAC = CLK(0U, CLK_I2S1_8CH_TX_FRAC_DIV),
    CLK_I2S1_8CH_TX      = CLK(CLK_I2S1_8CH_TX_SEL, 0U),
    CLK_I2S1_8CH_RX_SRC  = CLK(CLK_I2S1_8CH_RX_PLL_SEL, CLK_I2S1_8CH_RX_DIV),
    CLK_I2S1_8CH_RX_FRAC = CLK(0U, CLK_I2S1_8CH_RX_FRAC_DIV),
    CLK_I2S1_8CH_RX      = CLK(CLK_I2S1_8CH_RX_SEL, 0U),
    CLK_I2S2_8CH_TX_SRC  = CLK(CLK_I2S2_8CH_TX_PLL_SEL, CLK_I2S2_8CH_TX_DIV),
    CLK_I2S2_8CH_TX_FRAC = CLK(0U, CLK_I2S2_8CH_TX_FRAC_DIV),
    CLK_I2S2_8CH_TX      = CLK(CLK_I2S2_8CH_TX_SEL, 0U),
    CLK_I2S2_8CH_RX_SRC  = CLK(CLK_I2S2_8CH_RX_PLL_SEL, CLK_I2S2_8CH_RX_DIV),
    CLK_I2S2_8CH_RX_FRAC = CLK(0U, CLK_I2S2_8CH_RX_FRAC_DIV),
    CLK_I2S2_8CH_RX      = CLK(CLK_I2S2_8CH_RX_SEL, 0U),
    CLK_I2S3_8CH_TX_SRC  = CLK(CLK_I2S3_8CH_TX_PLL_SEL, CLK_I2S3_8CH_TX_DIV),
    CLK_I2S3_8CH_TX_FRAC = CLK(0U, CLK_I2S3_8CH_TX_FRAC_DIV),
    CLK_I2S3_8CH_TX      = CLK(CLK_I2S3_8CH_TX_SEL, 0U),
    CLK_I2S3_8CH_RX_SRC  = CLK(CLK_I2S3_8CH_RX_PLL_SEL, CLK_I2S3_8CH_RX_DIV),
    CLK_I2S3_8CH_RX_FRAC = CLK(0U, CLK_I2S3_8CH_RX_FRAC_DIV),
    CLK_I2S3_8CH_RX      = CLK(CLK_I2S3_8CH_RX_SEL, 0U),
    CLK_I2S0_2CH_SRC     = CLK(CLK_I2S0_2CH_PLL_SEL, CLK_I2S0_2CH_DIV),
    CLK_I2S0_2CH_FRAC    = CLK(0U, CLK_I2S0_2CH_FRAC_DIV),
    CLK_I2S0_2CH         = CLK(CLK_I2S0_2CH_SEL, 0U),
    CLK_I2S1_2CH_SRC     = CLK(CLK_I2S1_2CH_PLL_SEL, CLK_I2S1_2CH_DIV),
    CLK_I2S1_2CH_FRAC    = CLK(0U, CLK_I2S1_2CH_FRAC_DIV),
    CLK_I2S1_2CH         = CLK(CLK_I2S1_2CH_SEL, 0U),
    CLK_MAC              = CLK(CLK_MAC_PLL_SEL, CLK_MAC_DIV),
} eCLOCK_Name;
#endif

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
/****************************************SFC*********************************************/
#define SFC_CHIP_CNT (1)
/****************************************SPI*********************************************/
#define SPI_VERSION    0x0B050001

#define PCLK_SARADC_CONTROL_GATE PCLK_SARADC_GATE
#define CLK_SARADC_GATE          CLK_SARADC_PLL_CLK_GATE

#define PCLK_WDT		 PCLK_BUS
#define PCLK_WDT0_GATE		 FAKE_CLOCK_GATE

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __SOC_H */
