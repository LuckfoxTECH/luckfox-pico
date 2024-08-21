/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#ifndef __SOC_H
#define __SOC_H
#ifdef __cplusplus
  extern "C" {
#endif

#ifndef __ASSEMBLY__
/* ================================================================================ */
/* ================                    DMA REQ                      =============== */
/* ================================================================================ */
typedef enum {
    DMA_REQ_UART0_TX = 0,
    DMA_REQ_UART0_RX = 1,
    DMA_REQ_UART1_TX = 2,
    DMA_REQ_UART1_RX = 3,
    DMA_REQ_SPI0_TX = 4,
    DMA_REQ_SPI0_RX = 5,
    DMA_REQ_SPI1_TX = 6,
    DMA_REQ_SPI1_RX = 7,
    DMA_REQ_PWM = 8,
    DMA_REQ_AUDIOPWM = 9,
    DMA_REQ_I2S0_TX = 10,
    DMA_REQ_I2S0_RX = 11,
    DMA_REQ_I2S1_TX = 12,
    DMA_REQ_I2S1_RX = 13,
    DMA_REQ_PDM = 14,
    DMA_REQ_VOP = 15,
} DMA_REQ_Type;

/* ================================================================================ */
/* ================                       IRQ                      ================ */
/* ================================================================================ */
typedef enum
{
    /******  Cortex-M4 Processor Exceptions Numbers ****************************************************************/
    NonMaskableInt_IRQn         = -14,    /*!< 2 Non Maskable Interrupt                                          */
    MemoryManagement_IRQn       = -12,    /*!< 4 Cortex-M4 Memory Management Interrupt                           */
    BusFault_IRQn               = -11,    /*!< 5 Cortex-M4 Bus Fault Interrupt                                   */
    UsageFault_IRQn             = -10,    /*!< 6 Cortex-M4 Usage Fault Interrupt                                 */
    SVCall_IRQn                 = -5,     /*!< 11 Cortex-M4 SV Call Interrupt                                    */
    DebugMonitor_IRQn           = -4,     /*!< 12 Cortex-M4 Debug Monitor Interrupt                              */
    PendSV_IRQn                 = -2,     /*!< 14 Cortex-M4 Pend SV Interrupt                                    */
    SysTick_IRQn                = -1,     /*!< 15 Cortex-M4 System Tick Interrupt                                */
    /******  RK2206 specific Interrupt Numbers **********************************************************************/
    DMAC_CH0_IRQn               = 0,
    WDT0_IRQn                   = 1,
    TOUCH_NEG_IRQn              = 2,
    DMAC_CH1_IRQn               = 3,
    TIMER0_IRQn                 = 4,
    TIMER1_IRQn                 = 5,
    TIMER2_IRQn                 = 6,
    TIMER3_IRQn                 = 7,
    TIMER4_IRQn                 = 8,
    TIMER5_IRQn                 = 9,
    TIMER6_IRQn                 = 10,
    I2C0_IRQn                   = 11,
    I2C1_IRQn                   = 12,
    I2C2_IRQn                   = 13,
    SPIMST0_IRQn                = 14,
    SPIMST1_IRQn                = 15,
    UART0_IRQn                  = 16,
    UART1_IRQn                  = 17,
    UART2_IRQn                  = 18,
    PWM_4CH_0_IRQn              = 19,
    PWM_4CH_1_IRQn              = 20,
    PWM_4CH_2_IRQn              = 21,
    SARADC_IRQn                 = 22,
    B2A0_INT0_IRQn              = 23,
    B2A0_INT1_IRQn              = 24,
    B2A0_INT2_IRQn              = 25,
    B2A0_INT3_IRQn              = 26,
    B2A1_INT0_IRQn              = 27,
    B2A1_INT1_IRQn              = 28,
    B2A1_INT2_IRQn              = 29,
    B2A1_INT3_IRQn              = 30,
    FSPI0_IRQn                  = 31,
    FSPI1_IRQn                  = 32,
    VOP_INTR_IRQn               = 33,
    CACHE0_I_IRQn               = 34,
    CACHE0_D_IRQn               = 35,
    DSP_ERROR_IRQn              = 36,
    HYPERX8_IRQn                = 37,
    USB2OTG_IRQn                = 38,
    USB2OTG_BVALID_IRQn         = 39,
    USB2OTG_ID_IRQn             = 40,
    USB2OTG_LINESTATE_IRQn      = 41,
    USB2OTG_DISCONNECT_IRQn     = 42,
    SD_MMC_IRQn                 = 43,
    ADUPWM_IRQn                 = 44,
    PVTM_IRQn                   = 45,
    CRYPTO_IRQn                 = 46,
    VICAP_IRQn                  = 47,
    PMU_IRQn                    = 48,
    GPIO0_IRQn                  = 49,
    GPIO1_IRQn                  = 50,
    TRIM_IRQn                   = 51,
    I2S0_IRQn                   = 52,
    I2S1_IRQn                   = 53,
    PDM_IRQn                    = 54,
    VAD_IRQn                    = 55,
    EFUSE_IRQn                  = 56,
    TOUCH_IRQn                  = 57,
    PMIC_IRQn                   = 58,
    SPISLV0_IRQn                = 59,
    DMAC_CH2_IRQn               = 60,
    DMAC_CH3_IRQn               = 61,
    DMAC_CH4_IRQn               = 62,
    DMAC_CH5_IRQn               = 63,
    TSADC_IRQn                  = 64,
    NUM_INTERRUPTS
} IRQn_Type;
#endif /* __ASSEMBLY__ */

#define NVIC_PERIPH_IRQ_NUM MAX_IRQn
#define NVIC_PERIPH_IRQ_OFFSET 16

/* ================================================================================ */
/* ================      Processor and Core Peripheral Section     ================ */
/* ================================================================================ */

#define __CM4_REV                 0x0001U  /* Core revision r0p1                         */
#define __MPU_PRESENT             1U       /* RK2206 provides an MPU                     */
#define __VTOR_PRESENT            1U       /* VTOR present */
#define __NVIC_PRIO_BITS          3U       /* RK2206 uses 3 Bits for the Priority Levels */
#define __Vendor_SysTickConfig    0U       /* Set to 1 if different SysTick Config is used  */
#define __FPU_PRESENT             1U       /* FPU present                                   */
#define PLL_INPUT_OSC_RATE        (40000000U) /* RK2206 OSC Rate */

#ifndef __ASSEMBLY__
#include "core_cm4.h"             /* Cortex-M4 processor and core peripherals */
#include "system_rk2206.h"
#endif /* __ASSEMBLY__ */
#include "rk2206.h"
#include "rk2206_usb.h"

/****************************************************************************************/
/*                                                                                      */
/*                               Module Structure Section                               */
/*                                                                                      */
/****************************************************************************************/
#ifndef __ASSEMBLY__
/* SPINLOCK Register Structure Define */
struct SPINLOCK_REG {
    __IO uint32_t STATUS[64];                         /* Address Offset: 0x0000 */
};
#endif
/****************************************************************************************/
/*                                                                                      */
/*                                Module Address Section                                */
/*                                                                                      */
/****************************************************************************************/
/* Memory Base */
#define XIP_MAP0_BASE0      0x10000000U /* FSPI0 map address0 */
#define XIP_MAP1_BASE0      0x18000000U /* HYPERBUS/FSPI1 map address0 */
#define DSP_ITCM_BASE       0x20400000U /* DSP itcm base address */
#define DSP_ITCM_END        0x20407fffU /* DSP itcm end address */
#define DSP_DTCM_BASE       0x20600000U /* DSP dtcm base address */
#define DSP_DTCM_END        0x2062ffffU /* DSP dtcm end address */
#define XIP_MAP0_BASE1      0x30000000U /* FSPI0 map address1 */
#define XIP_MAP1_BASE1      0x38000000U /* HYPERBUS/FSPI1 map address1 */
#define SDMMC_BASE          MMC_BASE    /* MMC base address */
#define USB_BASE            0x43040000U /* USB base address */
#define USB_SNPS_PHY_BASE   0x41050340U /* USB SYSNOPSYS PHY base address */
#define USB_PHY_CON_BASE    (GRF->SOC_UOC2) /* USB PHY control base address */
/****************************************************************************************/
/*                                                                                      */
/*                               Module Variable Section                                */
/*                                                                                      */
/****************************************************************************************/
/* Module Variable Define */
#define USB                 ((struct USB_GLOBAL_REG *) USB_BASE)
#define SPINLOCK            ((struct SPINLOCK_REG *) GRF->HW_SPINLOCK)

#define IS_PCD_INSTANCE(instance) ((instance) == USB)
#define IS_HCD_INSTANCE(instance) ((instance) == USB)
#define IS_SPINLOCK_INSTANCE(instance) ((instance) == SPINLOCK)

/****************************************************************************************/
/*                                                                                      */
/*                               Register Bitmap Section                                */
/*                                                                                      */
/****************************************************************************************/
/*****************************************CACHE******************************************/
/* CACHE LINE SIZE */
#define CACHE_LINE_SHIFT                (5U)
#define CACHE_LINE_SIZE                 (0x1U << CACHE_LINE_SHIFT)
#define CACHE_LINE_ADDR_MASK            (0xFFFFFFFFU << CACHE_LINE_SHIFT)
#define CACHE_M_CLEAN                   0x0U
#define CACHE_M_INVALID                 0x2U
#define CACHE_M_CLEAN_INVALID           0x4U
#define CACHE_M_INVALID_ALL             0x6U

#define CACHE_REVISION                  DCACHE_REVISION
/*****************************************TIMER******************************************/
#define TIMER_CHAN_CNT   7
/*****************************************MBOX*******************************************/
#define MBOX_CNT             2
#define MBOX_CHAN_CNT        4
/*****************************************DMA********************************************/
#define DMA_NUM_CHANNELS     6
/*****************************************FSPI*******************************************/
#define FSPI_CHIP_CNT                                       (0x2U)
/*****************************************PMU********************************************/
#ifndef __ASSEMBLY__
typedef enum PD_Id {
    PD_INVALID = 0U,
} ePD_Id;
#endif
/*****************************************USB********************************************/
#define USB_PHY_SUSPEND_MASK \
    (GRF_SOC_UOC2_OTGPHY_SOFT_CON_SEL_MASK | GRF_SOC_UOC2_GRF_CON_OTG_UTMI_SUSPEND_N_MASK)
#define USB_PHY_RESUME_MASK \
    (GRF_SOC_UOC2_OTGPHY_SOFT_CON_SEL_MASK | GRF_SOC_UOC2_GRF_CON_OTG_UTMI_SUSPEND_N_MASK)
#define USB_PHY_CON_SHIFT                       GRF_SOC_UOC2_OTGPHY_SOFT_CON_SEL_SHIFT
#define USB_PHY_SUSPEND_VAL                     0x01U
#define USB_PHY_RESUME_VAL                      0x02U

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __SOC_H */
