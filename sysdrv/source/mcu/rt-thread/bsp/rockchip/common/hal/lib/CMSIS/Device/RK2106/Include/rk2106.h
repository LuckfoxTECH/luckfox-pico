/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2021 Rockchip Electronics Co., Ltd.
 */

#ifndef __RK2106_H
#define __RK2106_H
#ifdef __cplusplus
  extern "C" {
#endif
/****************************************************************************************/
/*                                                                                      */
/*                               Module Structure Section                               */
/*                                                                                      */
/****************************************************************************************/
#ifndef __ASSEMBLY__
/* CRU Register Structure Define */
struct CRU_REG {
    __IO uint32_t APLL_CON[3];                        /* Address Offset: 0x0000 */
         uint32_t RESERVED000C;                       /* Address Offset: 0x000C */
    __IO uint32_t MODE_CON00;                         /* Address Offset: 0x0010 */
    __IO uint32_t CRU_CLKSEL_CON[17];                 /* Address Offset: 0x0014 */
         uint32_t RESERVED0058[10];                   /* Address Offset: 0x0058 */
    __IO uint32_t CRU_CLKGATE_CON[10];                /* Address Offset: 0x0080 */
         uint32_t RESERVED00A8[6];                    /* Address Offset: 0x00A8 */
    __IO uint32_t CRU_SOFTRST_CON[5];                 /* Address Offset: 0x00C0 */
         uint32_t RESERVED00D4[3];                    /* Address Offset: 0x00D4 */
    __IO uint32_t STCLK_CON[2];                       /* Address Offset: 0x00E0 */
         uint32_t RESERVED00E8[3];                    /* Address Offset: 0x00E8 */
    __IO uint32_t GLB_SRST_FST_VALUE;                 /* Address Offset: 0x00F4 */
    __IO uint32_t GLB_CNT_TH;                         /* Address Offset: 0x00F8 */
};
/* I2S Register Structure Define */
struct I2S_REG {
    __IO uint32_t TXCR;                               /* Address Offset: 0x0000 */
    __IO uint32_t RXCR;                               /* Address Offset: 0x0004 */
    __IO uint32_t CKR;                                /* Address Offset: 0x0008 */
    __I  uint32_t FIFOLR;                             /* Address Offset: 0x000C */
    __IO uint32_t DMACR;                              /* Address Offset: 0x0010 */
    __IO uint32_t INTCR;                              /* Address Offset: 0x0014 */
    __I  uint32_t INTSR;                              /* Address Offset: 0x0018 */
    __IO uint32_t XFER;                               /* Address Offset: 0x001C */
    __IO uint32_t CLR;                                /* Address Offset: 0x0020 */
    __O  uint32_t TXDR;                               /* Address Offset: 0x0024 */
    __I  uint32_t RXDR;                               /* Address Offset: 0x0028 */
};
/* I2C Register Structure Define */
struct I2C_REG {
    __IO uint32_t CON;                                /* Address Offset: 0x0000 */
    __IO uint32_t CLKDIV;                             /* Address Offset: 0x0004 */
    __IO uint32_t MRXADDR;                            /* Address Offset: 0x0008 */
    __IO uint32_t MRXRADDR;                           /* Address Offset: 0x000C */
    __IO uint32_t MTXCNT;                             /* Address Offset: 0x0010 */
    __IO uint32_t MRXCNT;                             /* Address Offset: 0x0014 */
    __IO uint32_t IEN;                                /* Address Offset: 0x0018 */
    __IO uint32_t IPD;                                /* Address Offset: 0x001C */
    __IO uint32_t FCNT;                               /* Address Offset: 0x0020 */
         uint32_t RESERVED0024[55];                   /* Address Offset: 0x0024 */
    __IO uint32_t TXDATA0;                            /* Address Offset: 0x0100 */
    __IO uint32_t TXDATA1;                            /* Address Offset: 0x0104 */
    __IO uint32_t TXDATA2;                            /* Address Offset: 0x0108 */
    __IO uint32_t TXDATA3;                            /* Address Offset: 0x010C */
    __IO uint32_t TXDATA4;                            /* Address Offset: 0x0110 */
    __IO uint32_t TXDATA5;                            /* Address Offset: 0x0114 */
    __IO uint32_t TXDATA6;                            /* Address Offset: 0x0118 */
    __IO uint32_t TXDATA7;                            /* Address Offset: 0x011C */
         uint32_t RESERVED0120[56];                   /* Address Offset: 0x0120 */
    __IO uint32_t RXDATA0;                            /* Address Offset: 0x0200 */
    __IO uint32_t RXDATA1;                            /* Address Offset: 0x0204 */
    __IO uint32_t RXDATA2;                            /* Address Offset: 0x0208 */
    __IO uint32_t RXDATA3;                            /* Address Offset: 0x020C */
    __IO uint32_t RXDATA4;                            /* Address Offset: 0x0210 */
    __IO uint32_t RXDATA5;                            /* Address Offset: 0x0214 */
    __IO uint32_t RXDATA6;                            /* Address Offset: 0x0218 */
    __IO uint32_t RXDATA7;                            /* Address Offset: 0x021C */
};
/* SPI Register Structure Define */
struct SPI_REG {
    __IO uint32_t CTRLR0;                             /* Address Offset: 0x0000 */
    __IO uint32_t CTRLR1;                             /* Address Offset: 0x0004 */
    __IO uint32_t ENR;                                /* Address Offset: 0x0008 */
    __IO uint32_t SER;                                /* Address Offset: 0x000C */
    __IO uint32_t BAUDR;                              /* Address Offset: 0x0010 */
    __IO uint32_t TXFTLR;                             /* Address Offset: 0x0014 */
    __IO uint32_t RXFTLR;                             /* Address Offset: 0x0018 */
    __I  uint32_t TXFLR;                              /* Address Offset: 0x001C */
    __I  uint32_t RXFLR;                              /* Address Offset: 0x0020 */
    __I  uint32_t SR;                                 /* Address Offset: 0x0024 */
    __IO uint32_t IPR;                                /* Address Offset: 0x0028 */
    __IO uint32_t IMR;                                /* Address Offset: 0x002C */
    __I  uint32_t ISR;                                /* Address Offset: 0x0030 */
    __I  uint32_t RISR;                               /* Address Offset: 0x0034 */
    __O  uint32_t ICR;                                /* Address Offset: 0x0038 */
    __IO uint32_t DMACR;                              /* Address Offset: 0x003C */
    __IO uint32_t DMATDLR;                            /* Address Offset: 0x0040 */
    __IO uint32_t DMARDLR;                            /* Address Offset: 0x0044 */
    __O  uint32_t TXDR;                               /* Address Offset: 0x0048 */
    __IO uint32_t RXDR;                               /* Address Offset: 0x004C */
};
/* UART Register Structure Define */
struct UART_REG {
    union {
        __IO uint32_t RBR;                                /* Address Offset: 0x0000 */
        __IO uint32_t THR;                                /* Address Offset: 0x0000 */
        __IO uint32_t DLL;                                /* Address Offset: 0x0000 */
    };
    union {
        __IO uint32_t DLH;                                /* Address Offset: 0x0004 */
        __IO uint32_t IER;                                /* Address Offset: 0x0004 */
    };
    union {
        __I  uint32_t IIR;                                /* Address Offset: 0x0008 */
        __O  uint32_t FCR;                                /* Address Offset: 0x0008 */
    };
    __IO uint32_t LCR;                                /* Address Offset: 0x000C */
    __IO uint32_t MCR;                                /* Address Offset: 0x0010 */
    __I  uint32_t LSR;                                /* Address Offset: 0x0014 */
    __I  uint32_t MSR;                                /* Address Offset: 0x0018 */
    __IO uint32_t SCR;                                /* Address Offset: 0x001C */
         uint32_t RESERVED0020[4];                    /* Address Offset: 0x0020 */
    __I  uint32_t SRBR;                               /* Address Offset: 0x0030 */
         uint32_t RESERVED0034[14];                   /* Address Offset: 0x0034 */
    __I  uint32_t STHR;                               /* Address Offset: 0x006C */
    __IO uint32_t FAR;                                /* Address Offset: 0x0070 */
    __I  uint32_t TFR;                                /* Address Offset: 0x0074 */
    __O  uint32_t RFW;                                /* Address Offset: 0x0078 */
    __I  uint32_t USR;                                /* Address Offset: 0x007C */
    __IO uint32_t TFL;                                /* Address Offset: 0x0080 */
    __I  uint32_t RFL;                                /* Address Offset: 0x0084 */
    __O  uint32_t SRR;                                /* Address Offset: 0x0088 */
    __IO uint32_t SRTS;                               /* Address Offset: 0x008C */
    __IO uint32_t SBCR;                               /* Address Offset: 0x0090 */
    __IO uint32_t SDMAM;                              /* Address Offset: 0x0094 */
    __IO uint32_t SFE;                                /* Address Offset: 0x0098 */
    __IO uint32_t SRT;                                /* Address Offset: 0x009C */
    __IO uint32_t STET;                               /* Address Offset: 0x00A0 */
    __IO uint32_t HTX;                                /* Address Offset: 0x00A4 */
    __O  uint32_t DMASA;                              /* Address Offset: 0x00A8 */
         uint32_t RESERVED00AC[18];                   /* Address Offset: 0x00AC */
    __I  uint32_t CPR;                                /* Address Offset: 0x00F4 */
    __I  uint32_t UCV;                                /* Address Offset: 0x00F8 */
    __I  uint32_t CTR;                                /* Address Offset: 0x00FC */
};
/* SARADC Register Structure Define */
struct SARADC_REG {
    __I  uint32_t DATA;                               /* Address Offset: 0x0000 */
    __I  uint32_t STAS;                               /* Address Offset: 0x0004 */
    __IO uint32_t CTRL;                               /* Address Offset: 0x0008 */
};
/* TIMER Register Structure Define */
struct TIMER_REG {
    __IO uint32_t LOAD_COUNT[2];                      /* Address Offset: 0x0000 */
    __I  uint32_t CURRENT_VALUE[2];                   /* Address Offset: 0x0008 */
    __IO uint32_t CONTROLREG;                         /* Address Offset: 0x0010 */
         uint32_t RESERVED0014;                       /* Address Offset: 0x0014 */
    __IO uint32_t INTSTATUS;                          /* Address Offset: 0x0018 */
};
/* PWM Register Structure Define */
struct PWM_REG {
    __IO uint32_t CNTR;                               /* Address Offset: 0x0000 */
    __IO uint32_t HRC;                                /* Address Offset: 0x0004 */
    __IO uint32_t LRC;                                /* Address Offset: 0x0008 */
    __IO uint32_t CTRL;                               /* Address Offset: 0x000C */
};
/* WDT Register Structure Define */
struct WDT_REG {
    __IO uint32_t CR;                                 /* Address Offset: 0x0000 */
    __IO uint32_t TORR;                               /* Address Offset: 0x0004 */
    __I  uint32_t CCVR;                               /* Address Offset: 0x0008 */
    __IO uint32_t CRR;                                /* Address Offset: 0x000C */
    __I  uint32_t STAT;                               /* Address Offset: 0x0010 */
    __I  uint32_t EOI;                                /* Address Offset: 0x0014 */
};
/* GPIO Register Structure Define */
struct GPIO_REG {
    __IO uint32_t SWPORTA_DR;                         /* Address Offset: 0x0000 */
    __IO uint32_t SWPORTA_DDR;                        /* Address Offset: 0x0004 */
         uint32_t RESERVED0008[10];                   /* Address Offset: 0x0008 */
    __IO uint32_t INTEN;                              /* Address Offset: 0x0030 */
    __IO uint32_t INTMASK;                            /* Address Offset: 0x0034 */
    __IO uint32_t INTTYPE_LEVEL;                      /* Address Offset: 0x0038 */
    __IO uint32_t INT_POLARITY;                       /* Address Offset: 0x003C */
    __I  uint32_t INT_STATUS;                         /* Address Offset: 0x0040 */
    __I  uint32_t INT_RAWSTATUS;                      /* Address Offset: 0x0044 */
    __IO uint32_t DEBOUNCE;                           /* Address Offset: 0x0048 */
    __O  uint32_t PORTA_EOI;                          /* Address Offset: 0x004C */
    __I  uint32_t EXT_PORTA;                          /* Address Offset: 0x0050 */
         uint32_t RESERVED0054[3];                    /* Address Offset: 0x0054 */
    __IO uint32_t LS_SYNC;                            /* Address Offset: 0x0060 */
};
/* GRF Register Structure Define */
struct GRF_REG {
    __IO uint32_t GPIO0A_IOMUX;                       /* Address Offset: 0x0000 */
    __IO uint32_t GPIO0B_IOMUX;                       /* Address Offset: 0x0004 */
    __IO uint32_t GPIO0C_IOMUX;                       /* Address Offset: 0x0008 */
    __IO uint32_t GPIO0D_IOMUX;                       /* Address Offset: 0x000C */
    __IO uint32_t GPIO0A_P;                           /* Address Offset: 0x0010 */
    __IO uint32_t GPIO0B_P;                           /* Address Offset: 0x0014 */
    __IO uint32_t GPIO0C_P;                           /* Address Offset: 0x0018 */
    __IO uint32_t GPIO0D_P;                           /* Address Offset: 0x001C */
    __IO uint32_t GPIO1A_IOMUX;                       /* Address Offset: 0x0020 */
    __IO uint32_t GPIO1B_IOMUX;                       /* Address Offset: 0x0024 */
    __IO uint32_t GPIO1C_IOMUX;                       /* Address Offset: 0x0028 */
    __IO uint32_t GPIO1D_IOMUX;                       /* Address Offset: 0x002C */
    __IO uint32_t GPIO1A_P;                           /* Address Offset: 0x0030 */
    __IO uint32_t GPIO1B_P;                           /* Address Offset: 0x0034 */
    __IO uint32_t GPIO1C_P;                           /* Address Offset: 0x0038 */
    __IO uint32_t GPIO1D_P;                           /* Address Offset: 0x003C */
    __IO uint32_t GPIO2A_IOMUX;                       /* Address Offset: 0x0040 */
    __IO uint32_t GPIO2B_IOMUX;                       /* Address Offset: 0x0044 */
    __IO uint32_t GPIO2C_IOMUX;                       /* Address Offset: 0x0048 */
    __IO uint32_t GPIO2D_IOMUX;                       /* Address Offset: 0x004C */
    __IO uint32_t GPIO2A_P;                           /* Address Offset: 0x0050 */
    __IO uint32_t GPIO2B_P;                           /* Address Offset: 0x0054 */
    __IO uint32_t GPIO2C_P;                           /* Address Offset: 0x0058 */
    __IO uint32_t GPIO2D_P;                           /* Address Offset: 0x005C */
    __IO uint32_t SOFT_CON0;                          /* Address Offset: 0x0060 */
    __IO uint32_t SOFT_CON1;                          /* Address Offset: 0x0064 */
         uint32_t RESERVED0068[6];                    /* Address Offset: 0x0068 */
    __O  uint32_t USBPHY_CON11;                       /* Address Offset: 0x0080 */
    __O  uint32_t USBPHY_CON10;                       /* Address Offset: 0x0084 */
    __O  uint32_t USBPHY_CON9;                        /* Address Offset: 0x0088 */
    __O  uint32_t USBPHY_CON8;                        /* Address Offset: 0x008C */
    __IO uint32_t USBPHY_CON7;                        /* Address Offset: 0x0090 */
    __IO uint32_t USBPHY_CON6;                        /* Address Offset: 0x0094 */
    __O  uint32_t USBPHY_CON5;                        /* Address Offset: 0x0098 */
    __IO uint32_t USBPHY_CON4;                        /* Address Offset: 0x009C */
    __IO uint32_t USBPHY_CON3;                        /* Address Offset: 0x00A0 */
    __IO uint32_t USBPHY_CON2;                        /* Address Offset: 0x00A4 */
    __IO uint32_t USBPHY_CON1;                        /* Address Offset: 0x00A8 */
    __IO uint32_t USBPHY_CON0;                        /* Address Offset: 0x00AC */
    __IO uint32_t UOC_CON0;                           /* Address Offset: 0x00B0 */
    __IO uint32_t UOC_CON1;                           /* Address Offset: 0x00B4 */
    __IO uint32_t UOC_CON2;                           /* Address Offset: 0x00B8 */
    __IO uint32_t IOFUNC_CON0;                        /* Address Offset: 0x00BC */
         uint32_t RESERVED00C0;                       /* Address Offset: 0x00C0 */
    __IO uint32_t INTER_CON0;                         /* Address Offset: 0x00C4 */
         uint32_t RESERVED00C8[6];                    /* Address Offset: 0x00C8 */
    __IO uint32_t SOC_STATUS0;                        /* Address Offset: 0x00E0 */
    __IO uint32_t SOC_STATUS1;                        /* Address Offset: 0x00E4 */
    __IO uint32_t SOC_USB_STATUS;                     /* Address Offset: 0x00E8 */
         uint32_t RESERVED00EC[3];                    /* Address Offset: 0x00EC */
    __I  uint32_t PRJ_ID;                             /* Address Offset: 0x00F8 */
    __I  uint32_t CPU_ID;                             /* Address Offset: 0x00FC */
};
/* DMA Register Structure Define */
struct DMA_CHAN_REGS {
    __IO uint32_t SAR;
         uint32_t RESERVED0004;
    __IO uint32_t DAR;
         uint32_t RESERVED000C;
    __IO uint32_t LLP;
         uint32_t RESERVED0014;
    __IO uint32_t CTL_LO;
    __IO uint32_t CTL_HI;
    __IO uint32_t SSTAT;
         uint32_t RESERVED0024;
    __IO uint32_t DSTAT;
         uint32_t RESERVED002C;
    __IO uint32_t SSTATAR;
         uint32_t RESERVED0034;
    __IO uint32_t DSTATAR;
         uint32_t RESERVED003C;
    __IO uint32_t CFG_LO;
    __IO uint32_t CFG_HI;
    __IO uint32_t SGR;
         uint32_t RESERVED004C;
    __IO uint32_t DSR;
         uint32_t RESERVED;
};
struct DMA_IRQ_REGS {
    __IO uint32_t TFR;
         uint32_t RESERVED02C4;
    __IO uint32_t BLOCK;
         uint32_t RESERVED02CC;
    __IO uint32_t SRCTRAN;
         uint32_t RESERVED02D4;
    __IO uint32_t DSTTRAN;
         uint32_t RESERVED02DC;
    __IO uint32_t ERR;
         uint32_t RESERVED;
};
struct DMA_REG {
    struct DMA_CHAN_REGS CHAN[6];                     /* Address Offset: 0x0000 */
         uint32_t RESERVED0210[44];                   /* Address Offset: 0x0210 */
    struct DMA_IRQ_REGS RAW;                          /* Address Offset: 0x02C0 */
    struct DMA_IRQ_REGS STATUS;                       /* Address Offset: 0x02E8 */
    struct DMA_IRQ_REGS MASK;                         /* Address Offset: 0x0310 */
    struct DMA_IRQ_REGS CLEAR;                        /* Address Offset: 0x0338 */
    __I  uint32_t STATUSINT;                          /* Address Offset: 0x0360 */
         uint32_t RESERVED0364[13];                   /* Address Offset: 0x0364 */
    __IO uint32_t DMACFGREG;                          /* Address Offset: 0x0398 */
         uint32_t RESERVED039C;                       /* Address Offset: 0x039C */
    __IO uint32_t CHENREG;                            /* Address Offset: 0x03A0 */
};
/* SFC Register Structure Define */
struct SFC_REG {
    __IO uint32_t CTRL;                               /* Address Offset: 0x0000 */
    __IO uint32_t IMR;                                /* Address Offset: 0x0004 */
    __IO uint32_t ICLR;                               /* Address Offset: 0x0008 */
    __IO uint32_t FTLR;                               /* Address Offset: 0x000C */
    __IO uint32_t RCVR;                               /* Address Offset: 0x0010 */
    __I  uint32_t AX;                                 /* Address Offset: 0x0014 */
    __IO uint32_t ABIT;                               /* Address Offset: 0x0018 */
    __IO uint32_t ISR;                                /* Address Offset: 0x001C */
    __IO uint32_t FSR;                                /* Address Offset: 0x0020 */
    __O  uint32_t SR;                                 /* Address Offset: 0x0024 */
         uint32_t RESERVED0028[22];                   /* Address Offset: 0x0028 */
    __IO uint32_t DMAADDR;                            /* Address Offset: 0x0080 */
    __IO uint32_t DMATR;                              /* Address Offset: 0x0084 */
         uint32_t RESERVED0088[30];                   /* Address Offset: 0x0088 */
    __IO uint32_t CMD;                                /* Address Offset: 0x0100 */
    __IO uint32_t ADDR;                               /* Address Offset: 0x0104 */
    __IO uint32_t DATA;                               /* Address Offset: 0x0108 */
};
#endif /*  __ASSEMBLY__  */
/****************************************************************************************/
/*                                                                                      */
/*                                Module Address Section                                */
/*                                                                                      */
/****************************************************************************************/
/* Memory Base */
#define CRU_BASE            0x40180000U /* CRU base address */
#define I2S0_BASE           0x40020000U /* I2S0 base address */
#define I2S1_BASE           0x40030000U /* I2S1 base address */
#define I2C0_BASE           0x40040000U /* I2C0 base address */
#define I2C1_BASE           0x40050000U /* I2C1 base address */
#define I2C2_BASE           0x40060000U /* I2C2 base address */
#define SPI0_BASE           0x40070000U /* SPI0 base address */
#define SPI1_BASE           0x40080000U /* SPI1 base address */
#define UART0_BASE          0x400A0000U /* UART0 base address */
#define UART1_BASE          0x400B0000U /* UART1 base address */
#define UART2_BASE          0x400C0000U /* UART2 base address */
#define SARADC_BASE         0x400D0000U /* SARADC base address */
#define TIMER0_BASE         0x400E0000U /* TIMER0 base address */
#define TIMER1_BASE         0x400E0020U /* TIMER1 base address */
#define PWM0_BASE           0x400F0000U /* PWM0 base address */
#define PWM1_BASE           0x40100000U /* PWM1 base address */
#define WDT_BASE            0x40120000U /* WDT base address */
#define UART3_BASE          0x40130000U /* UART3 base address */
#define UART4_BASE          0x40140000U /* UART4 base address */
#define UART5_BASE          0x40150000U /* UART5 base address */
#define GPIO0_BASE          0x40160000U /* GPIO0 base address */
#define GPIO1_BASE          0x40170000U /* GPIO1 base address */
#define GRF_BASE            0x50010000U /* GRF base address */
#define GPIO2_BASE          0x50030000U /* GPIO2 base address */
#define DMA1_BASE           0x60000000U /* DMA1 base address */
#define SFC_BASE            0x60060000U /* SFC base address */
/****************************************************************************************/
/*                                                                                      */
/*                               Module Variable Section                                */
/*                                                                                      */
/****************************************************************************************/
/* Module Variable Define */

#define CRU                 ((struct CRU_REG *) CRU_BASE)
#define I2S0                ((struct I2S_REG *) I2S0_BASE)
#define I2S1                ((struct I2S_REG *) I2S1_BASE)
#define I2C0                ((struct I2C_REG *) I2C0_BASE)
#define I2C1                ((struct I2C_REG *) I2C1_BASE)
#define I2C2                ((struct I2C_REG *) I2C2_BASE)
#define SPI0                ((struct SPI_REG *) SPI0_BASE)
#define SPI1                ((struct SPI_REG *) SPI1_BASE)
#define UART0               ((struct UART_REG *) UART0_BASE)
#define UART1               ((struct UART_REG *) UART1_BASE)
#define UART2               ((struct UART_REG *) UART2_BASE)
#define SARADC              ((struct SARADC_REG *) SARADC_BASE)
#define TIMER0              ((struct TIMER_REG *) TIMER0_BASE)
#define TIMER1              ((struct TIMER_REG *) TIMER1_BASE)
#define PWM0                ((struct PWM_REG *) PWM0_BASE)
#define PWM1                ((struct PWM_REG *) PWM1_BASE)
#define WDT                 ((struct WDT_REG *) WDT_BASE)
#define UART3               ((struct UART_REG *) UART3_BASE)
#define UART4               ((struct UART_REG *) UART4_BASE)
#define UART5               ((struct UART_REG *) UART5_BASE)
#define GPIO0               ((struct GPIO_REG *) GPIO0_BASE)
#define GPIO1               ((struct GPIO_REG *) GPIO1_BASE)
#define GRF                 ((struct GRF_REG *) GRF_BASE)
#define GPIO2               ((struct GPIO_REG *) GPIO2_BASE)
#define DMA1                ((struct DMA_REG *) DMA1_BASE)
#define SFC                 ((struct SFC_REG *) SFC_BASE)

#define IS_CRU_INSTANCE(instance) ((instance) == CRU)
#define IS_SARADC_INSTANCE(instance) ((instance) == SARADC)
#define IS_WDT_INSTANCE(instance) ((instance) == WDT)
#define IS_GRF_INSTANCE(instance) ((instance) == GRF)
#define IS_SFC_INSTANCE(instance) ((instance) == SFC)
#define IS_I2S_INSTANCE(instance) (((instance) == I2S0) || ((instance) == I2S1))
#define IS_I2C_INSTANCE(instance) (((instance) == I2C0) || ((instance) == I2C1) || ((instance) == I2C2))
#define IS_SPI_INSTANCE(instance) (((instance) == SPI0) || ((instance) == SPI1))
#define IS_UART_INSTANCE(instance) (((instance) == UART0) || ((instance) == UART1) || ((instance) == UART2) || ((instance) == UART3) || ((instance) == UART4) || ((instance) == UART5))
#define IS_TIMER_INSTANCE(instance) (((instance) == TIMER0) || ((instance) == TIMER1))
#define IS_PWM_INSTANCE(instance) (((instance) == PWM0) || ((instance) == PWM1))
#define IS_GPIO_INSTANCE(instance) (((instance) == GPIO0) || ((instance) == GPIO1) || ((instance) == GPIO2))
#define IS_DMA_INSTANCE(instance) ((instance) == DMA1)
/****************************************************************************************/
/*                                                                                      */
/*                               Register Bitmap Section                                */
/*                                                                                      */
/****************************************************************************************/
/******************************************CRU*******************************************/
/* APLL_CON0 */
#define CRU_APLL_CON0_OFFSET                               (0x0U)
#define CRU_APLL_CON0_FBDIV_SHIFT                          (0U)
#define CRU_APLL_CON0_FBDIV_MASK                           (0xFFFU << CRU_APLL_CON0_FBDIV_SHIFT)                        /* 0x00000FFF */
#define CRU_APLL_CON0_POSTDIV1_SHIFT                       (12U)
#define CRU_APLL_CON0_POSTDIV1_MASK                        (0x7U << CRU_APLL_CON0_POSTDIV1_SHIFT)                       /* 0x00007000 */
#define CRU_APLL_CON0_BP_SHIFT                             (15U)
#define CRU_APLL_CON0_BP_MASK                              (0x1U << CRU_APLL_CON0_BP_SHIFT)                             /* 0x00008000 */
/* APLL_CON1 */
#define CRU_APLL_CON1_OFFSET                               (0x4U)
#define CRU_APLL_CON1_REFDIV_SHIFT                         (0U)
#define CRU_APLL_CON1_REFDIV_MASK                          (0x3FU << CRU_APLL_CON1_REFDIV_SHIFT)                        /* 0x0000003F */
#define CRU_APLL_CON1_POSTDIV2_SHIFT                       (6U)
#define CRU_APLL_CON1_POSTDIV2_MASK                        (0x7U << CRU_APLL_CON1_POSTDIV2_SHIFT)                       /* 0x000001C0 */
#define CRU_APLL_CON1_LOCK_SHIFT                           (10U)
#define CRU_APLL_CON1_LOCK_MASK                            (0x1U << CRU_APLL_CON1_LOCK_SHIFT)                           /* 0x00000400 */
#define CRU_APLL_CON1_DSMPD_SHIFT                          (12U)
#define CRU_APLL_CON1_DSMPD_MASK                           (0x1U << CRU_APLL_CON1_DSMPD_SHIFT)                          /* 0x00001000 */
#define CRU_APLL_CON1_PD_SHIFT                             (13U)
#define CRU_APLL_CON1_PD_MASK                              (0x1U << CRU_APLL_CON1_PD_SHIFT)                             /* 0x00002000 */
#define CRU_APLL_CON1_RST_SHIFT                            (14U)
#define CRU_APLL_CON1_RST_MASK                             (0x1U << CRU_APLL_CON1_RST_SHIFT)                            /* 0x00004000 */
#define CRU_APLL_CON1_RSTMODE_SHIFT                        (15U)
#define CRU_APLL_CON1_RSTMODE_MASK                         (0x1U << CRU_APLL_CON1_RSTMODE_SHIFT)                        /* 0x00008000 */
/* APLL_CON2 */
#define CRU_APLL_CON2_OFFSET                               (0x8U)
#define CRU_APLL_CON2_FRAC_SHIFT                           (0U)
#define CRU_APLL_CON2_FRAC_MASK                            (0xFFFFFFU << CRU_APLL_CON2_FRAC_SHIFT)                      /* 0x00FFFFFF */
#define CRU_APLL_CON2_DACPD_SHIFT                          (24U)
#define CRU_APLL_CON2_DACPD_MASK                           (0x1U << CRU_APLL_CON2_DACPD_SHIFT)                          /* 0x01000000 */
#define CRU_APLL_CON2_FOUTPOSTDIVPD_SHIFT                  (25U)
#define CRU_APLL_CON2_FOUTPOSTDIVPD_MASK                   (0x1U << CRU_APLL_CON2_FOUTPOSTDIVPD_SHIFT)                  /* 0x02000000 */
#define CRU_APLL_CON2_FOUTVCOPD_SHIFT                      (26U)
#define CRU_APLL_CON2_FOUTVCOPD_MASK                       (0x1U << CRU_APLL_CON2_FOUTVCOPD_SHIFT)                      /* 0x04000000 */
#define CRU_APLL_CON2_FOUT4PHASEPD_SHIFT                   (27U)
#define CRU_APLL_CON2_FOUT4PHASEPD_MASK                    (0x1U << CRU_APLL_CON2_FOUT4PHASEPD_SHIFT)                   /* 0x08000000 */
/* MODE_CON00 */
#define CRU_MODE_CON00_OFFSET                              (0x10U)
#define CRU_MODE_CON00_APLL_WORK_MODE_SHIFT                (0U)
#define CRU_MODE_CON00_APLL_WORK_MODE_MASK                 (0x1U << CRU_MODE_CON00_APLL_WORK_MODE_SHIFT)                /* 0x00000001 */
#define CRU_MODE_CON00_USB480M_SEL_SHIFT                   (4U)
#define CRU_MODE_CON00_USB480M_SEL_MASK                    (0x1U << CRU_MODE_CON00_USB480M_SEL_SHIFT)                   /* 0x00000010 */
#define CRU_MODE_CON00_DIV_CON_24M_SHIFT                   (8U)
#define CRU_MODE_CON00_DIV_CON_24M_MASK                    (0x3FU << CRU_MODE_CON00_DIV_CON_24M_SHIFT)                  /* 0x00003F00 */
/* CLKSEL0_CON */
#define CRU_CLKSEL0_CON_OFFSET                             (0x14U)
#define CRU_CLKSEL0_CON_SYS_CORE_DIV_CON_SHIFT             (0U)
#define CRU_CLKSEL0_CON_SYS_CORE_DIV_CON_MASK              (0x7U << CRU_CLKSEL0_CON_SYS_CORE_DIV_CON_SHIFT)             /* 0x00000007 */
#define CRU_CLKSEL0_CON_SYS_STCLK_DIV_CON_SHIFT            (4U)
#define CRU_CLKSEL0_CON_SYS_STCLK_DIV_CON_MASK             (0x7U << CRU_CLKSEL0_CON_SYS_STCLK_DIV_CON_SHIFT)            /* 0x00000070 */
#define CRU_CLKSEL0_CON_SYS_CORE_SRC_SEL_SHIFT             (7U)
#define CRU_CLKSEL0_CON_SYS_CORE_SRC_SEL_MASK              (0x1U << CRU_CLKSEL0_CON_SYS_CORE_SRC_SEL_SHIFT)             /* 0x00000080 */
#define CRU_CLKSEL0_CON_CAL_STCLK_DIV_CON_SHIFT            (12U)
#define CRU_CLKSEL0_CON_CAL_STCLK_DIV_CON_MASK             (0x7U << CRU_CLKSEL0_CON_CAL_STCLK_DIV_CON_SHIFT)            /* 0x00007000 */
/* CLKSEL1_CON */
#define CRU_CLKSEL1_CON_OFFSET                             (0x18U)
#define CRU_CLKSEL1_CON_HCLK_LOGIC_DIV_CON_SHIFT           (4U)
#define CRU_CLKSEL1_CON_HCLK_LOGIC_DIV_CON_MASK            (0xFU << CRU_CLKSEL1_CON_HCLK_LOGIC_DIV_CON_SHIFT)           /* 0x000000F0 */
#define CRU_CLKSEL1_CON_PCLK_LOGIC_DIV_CON_SHIFT           (8U)
#define CRU_CLKSEL1_CON_PCLK_LOGIC_DIV_CON_MASK            (0xFU << CRU_CLKSEL1_CON_PCLK_LOGIC_DIV_CON_SHIFT)           /* 0x00000F00 */
/* CLKSEL2_CON */
#define CRU_CLKSEL2_CON_OFFSET                             (0x1CU)
#define CRU_CLKSEL2_CON_TIMER0_DIV_CON_SHIFT               (0U)
#define CRU_CLKSEL2_CON_TIMER0_DIV_CON_MASK                (0x1FU << CRU_CLKSEL2_CON_TIMER0_DIV_CON_SHIFT)              /* 0x0000001F */
#define CRU_CLKSEL2_CON_TIMER0_PLL_SEL_SHIFT               (7U)
#define CRU_CLKSEL2_CON_TIMER0_PLL_SEL_MASK                (0x1U << CRU_CLKSEL2_CON_TIMER0_PLL_SEL_SHIFT)               /* 0x00000080 */
#define CRU_CLKSEL2_CON_TIMER1_DIV_CON_SHIFT               (8U)
#define CRU_CLKSEL2_CON_TIMER1_DIV_CON_MASK                (0x1FU << CRU_CLKSEL2_CON_TIMER1_DIV_CON_SHIFT)              /* 0x00001F00 */
#define CRU_CLKSEL2_CON_TIMER1_PLL_SEL_SHIFT               (15U)
#define CRU_CLKSEL2_CON_TIMER1_PLL_SEL_MASK                (0x1U << CRU_CLKSEL2_CON_TIMER1_PLL_SEL_SHIFT)               /* 0x00008000 */
/* CLKSEL3_CON */
#define CRU_CLKSEL3_CON_OFFSET                             (0x20U)
#define CRU_CLKSEL3_CON_I2S1_DIV_CON_SHIFT                 (0U)
#define CRU_CLKSEL3_CON_I2S1_DIV_CON_MASK                  (0x7FU << CRU_CLKSEL3_CON_I2S1_DIV_CON_SHIFT)                /* 0x0000007F */
#define CRU_CLKSEL3_CON_I2S1_CLK_SEL_SHIFT                 (8U)
#define CRU_CLKSEL3_CON_I2S1_CLK_SEL_MASK                  (0x3U << CRU_CLKSEL3_CON_I2S1_CLK_SEL_SHIFT)                 /* 0x00000300 */
#define CRU_CLKSEL3_CON_I2S1_OUT_SEL_SHIFT                 (13U)
#define CRU_CLKSEL3_CON_I2S1_OUT_SEL_MASK                  (0x1U << CRU_CLKSEL3_CON_I2S1_OUT_SEL_SHIFT)                 /* 0x00002000 */
/* CLKSEL4_CON */
#define CRU_CLKSEL4_CON_OFFSET                             (0x24U)
#define CRU_CLKSEL4_CON_I2S0_DIV_CON_SHIFT                 (0U)
#define CRU_CLKSEL4_CON_I2S0_DIV_CON_MASK                  (0x7FU << CRU_CLKSEL4_CON_I2S0_DIV_CON_SHIFT)                /* 0x0000007F */
#define CRU_CLKSEL4_CON_I2S0_CLK_SEL_SHIFT                 (8U)
#define CRU_CLKSEL4_CON_I2S0_CLK_SEL_MASK                  (0x3U << CRU_CLKSEL4_CON_I2S0_CLK_SEL_SHIFT)                 /* 0x00000300 */
/* CLKSEL5_CON */
#define CRU_CLKSEL5_CON_OFFSET                             (0x28U)
#define CRU_CLKSEL5_CON_EBC_DCLK_DIV_CON_SHIFT             (0U)
#define CRU_CLKSEL5_CON_EBC_DCLK_DIV_CON_MASK              (0xFFU << CRU_CLKSEL5_CON_EBC_DCLK_DIV_CON_SHIFT)            /* 0x000000FF */
#define CRU_CLKSEL5_CON_EBC_DCLK_PLL_SEL_SHIFT             (8U)
#define CRU_CLKSEL5_CON_EBC_DCLK_PLL_SEL_MASK              (0x1U << CRU_CLKSEL5_CON_EBC_DCLK_PLL_SEL_SHIFT)             /* 0x00000100 */
#define CRU_CLKSEL5_CON_HIFI_DIV_CON_SHIFT                 (12U)
#define CRU_CLKSEL5_CON_HIFI_DIV_CON_MASK                  (0x7U << CRU_CLKSEL5_CON_HIFI_DIV_CON_SHIFT)                 /* 0x00007000 */
#define CRU_CLKSEL5_CON_HIFI_CLK_PLL_SEL_SHIFT             (15U)
#define CRU_CLKSEL5_CON_HIFI_CLK_PLL_SEL_MASK              (0x1U << CRU_CLKSEL5_CON_HIFI_CLK_PLL_SEL_SHIFT)             /* 0x00008000 */
/* CLKSEL6_CON */
#define CRU_CLKSEL6_CON_OFFSET                             (0x2CU)
#define CRU_CLKSEL6_CON_SARADC_DIV_CON_SHIFT               (0U)
#define CRU_CLKSEL6_CON_SARADC_DIV_CON_MASK                (0xFFU << CRU_CLKSEL6_CON_SARADC_DIV_CON_SHIFT)              /* 0x000000FF */
/* CLKSEL7_CON */
#define CRU_CLKSEL7_CON_OFFSET                             (0x30U)
#define CRU_CLKSEL7_CON_SDMMC_DIV_CON_SHIFT                (0U)
#define CRU_CLKSEL7_CON_SDMMC_DIV_CON_MASK                 (0x1FU << CRU_CLKSEL7_CON_SDMMC_DIV_CON_SHIFT)               /* 0x0000001F */
#define CRU_CLKSEL7_CON_SDMMC_CLK_PLL_SEL_SHIFT            (7U)
#define CRU_CLKSEL7_CON_SDMMC_CLK_PLL_SEL_MASK             (0x1U << CRU_CLKSEL7_CON_SDMMC_CLK_PLL_SEL_SHIFT)            /* 0x00000080 */
#define CRU_CLKSEL7_CON_EMMC_DIV_CON_SHIFT                 (8U)
#define CRU_CLKSEL7_CON_EMMC_DIV_CON_MASK                  (0x1FU << CRU_CLKSEL7_CON_EMMC_DIV_CON_SHIFT)                /* 0x00001F00 */
#define CRU_CLKSEL7_CON_EMMC_PLL_SEL_SHIFT                 (15U)
#define CRU_CLKSEL7_CON_EMMC_PLL_SEL_MASK                  (0x1U << CRU_CLKSEL7_CON_EMMC_PLL_SEL_SHIFT)                 /* 0x00008000 */
/* CLKSEL8_CON */
#define CRU_CLKSEL8_CON_OFFSET                             (0x34U)
#define CRU_CLKSEL8_CON_SPI0_DIV_CON_SHIFT                 (0U)
#define CRU_CLKSEL8_CON_SPI0_DIV_CON_MASK                  (0x3FU << CRU_CLKSEL8_CON_SPI0_DIV_CON_SHIFT)                /* 0x0000003F */
#define CRU_CLKSEL8_CON_CORE_CLK_PLL_SEL_SHIFT             (7U)
#define CRU_CLKSEL8_CON_CORE_CLK_PLL_SEL_MASK              (0x1U << CRU_CLKSEL8_CON_CORE_CLK_PLL_SEL_SHIFT)             /* 0x00000080 */
#define CRU_CLKSEL8_CON_SPI1_DIV_CON_SHIFT                 (8U)
#define CRU_CLKSEL8_CON_SPI1_DIV_CON_MASK                  (0x3FU << CRU_CLKSEL8_CON_SPI1_DIV_CON_SHIFT)                /* 0x00003F00 */
#define CRU_CLKSEL8_CON_CPU_CLK_PLL_SEL_SHIFT              (15U)
#define CRU_CLKSEL8_CON_CPU_CLK_PLL_SEL_MASK               (0x1U << CRU_CLKSEL8_CON_CPU_CLK_PLL_SEL_SHIFT)              /* 0x00008000 */
/* CLKSEL9_CON */
#define CRU_CLKSEL9_CON_OFFSET                             (0x38U)
#define CRU_CLKSEL9_CON_UART1_DIV_CON_SHIFT                (0U)
#define CRU_CLKSEL9_CON_UART1_DIV_CON_MASK                 (0x3FU << CRU_CLKSEL9_CON_UART1_DIV_CON_SHIFT)               /* 0x0000003F */
#define CRU_CLKSEL9_CON_UART1_PLL_SEL_SHIFT                (6U)
#define CRU_CLKSEL9_CON_UART1_PLL_SEL_MASK                 (0x3U << CRU_CLKSEL9_CON_UART1_PLL_SEL_SHIFT)                /* 0x000000C0 */
#define CRU_CLKSEL9_CON_UART0_DIV_CON_SHIFT                (8U)
#define CRU_CLKSEL9_CON_UART0_DIV_CON_MASK                 (0x3FU << CRU_CLKSEL9_CON_UART0_DIV_CON_SHIFT)               /* 0x00003F00 */
#define CRU_CLKSEL9_CON_CPU_CLK_PLL_SEL_SHIFT              (15U)
#define CRU_CLKSEL9_CON_CPU_CLK_PLL_SEL_MASK               (0x1U << CRU_CLKSEL9_CON_CPU_CLK_PLL_SEL_SHIFT)              /* 0x00008000 */
/* CLKSEL10_CON */
#define CRU_CLKSEL10_CON_OFFSET                            (0x3CU)
#define CRU_CLKSEL10_CON_UART2_DIV_CON_SHIFT               (0U)
#define CRU_CLKSEL10_CON_UART2_DIV_CON_MASK                (0x3FU << CRU_CLKSEL10_CON_UART2_DIV_CON_SHIFT)              /* 0x0000003F */
#define CRU_CLKSEL10_CON_UART2_PLL_SEL_SHIFT               (7U)
#define CRU_CLKSEL10_CON_UART2_PLL_SEL_MASK                (0x1U << CRU_CLKSEL10_CON_UART2_PLL_SEL_SHIFT)               /* 0x00000080 */
#define CRU_CLKSEL10_CON_SFC_DIV_CON_SHIFT                 (8U)
#define CRU_CLKSEL10_CON_SFC_DIV_CON_MASK                  (0x1FU << CRU_CLKSEL10_CON_SFC_DIV_CON_SHIFT)                /* 0x00001F00 */
#define CRU_CLKSEL10_CON_SFC_PLL_SEL_SHIFT                 (15U)
#define CRU_CLKSEL10_CON_SFC_PLL_SEL_MASK                  (0x1U << CRU_CLKSEL10_CON_SFC_PLL_SEL_SHIFT)                 /* 0x00008000 */
/* CLKSEL11_CON */
#define CRU_CLKSEL11_CON_OFFSET                            (0x40U)
#define CRU_CLKSEL11_CON_TEST_DIV_CON_SHIFT                (0U)
#define CRU_CLKSEL11_CON_TEST_DIV_CON_MASK                 (0x1FU << CRU_CLKSEL11_CON_TEST_DIV_CON_SHIFT)               /* 0x0000001F */
#define CRU_CLKSEL11_CON_CORE_CLK_PLL_SEL_SHIFT            (5U)
#define CRU_CLKSEL11_CON_CORE_CLK_PLL_SEL_MASK             (0x7U << CRU_CLKSEL11_CON_CORE_CLK_PLL_SEL_SHIFT)            /* 0x000000E0 */
#define CRU_CLKSEL11_CON_UART3_DIV_CON_SHIFT               (8U)
#define CRU_CLKSEL11_CON_UART3_DIV_CON_MASK                (0x3FU << CRU_CLKSEL11_CON_UART3_DIV_CON_SHIFT)              /* 0x00003F00 */
#define CRU_CLKSEL11_CON_UART3_PLL_SEL_SHIFT               (15U)
#define CRU_CLKSEL11_CON_UART3_PLL_SEL_MASK                (0x1U << CRU_CLKSEL11_CON_UART3_PLL_SEL_SHIFT)               /* 0x00008000 */
/* CLKSEL12_CON */
#define CRU_CLKSEL12_CON_OFFSET                            (0x44U)
#define CRU_CLKSEL12_CON_UART4_DIV_CON_SHIFT               (0U)
#define CRU_CLKSEL12_CON_UART4_DIV_CON_MASK                (0x3FU << CRU_CLKSEL12_CON_UART4_DIV_CON_SHIFT)              /* 0x0000003F */
#define CRU_CLKSEL12_CON_UART4_PLL_SEL_SHIFT               (7U)
#define CRU_CLKSEL12_CON_UART4_PLL_SEL_MASK                (0x1U << CRU_CLKSEL12_CON_UART4_PLL_SEL_SHIFT)               /* 0x00000080 */
#define CRU_CLKSEL12_CON_UART5_DIV_CON_SHIFT               (8U)
#define CRU_CLKSEL12_CON_UART5_DIV_CON_MASK                (0x3FU << CRU_CLKSEL12_CON_UART5_DIV_CON_SHIFT)              /* 0x00003F00 */
#define CRU_CLKSEL12_CON_UART5_PLL_SEL_SHIFT               (15U)
#define CRU_CLKSEL12_CON_UART5_PLL_SEL_MASK                (0x1U << CRU_CLKSEL12_CON_UART5_PLL_SEL_SHIFT)               /* 0x00008000 */
/* CLKSEL15_CON */
#define CRU_CLKSEL15_CON_OFFSET                            (0x50U)
#define CRU_CLKSEL15_CON_I2S0_FRAC_FACTOR_SHIFT            (0U)
#define CRU_CLKSEL15_CON_I2S0_FRAC_FACTOR_MASK             (0xFFFFFFFFU << CRU_CLKSEL15_CON_I2S0_FRAC_FACTOR_SHIFT)     /* 0xFFFFFFFF */
/* CLKSEL16_CON */
#define CRU_CLKSEL16_CON_OFFSET                            (0x54U)
#define CRU_CLKSEL16_CON_I2S1_FRAC_FACTOR_SHIFT            (0U)
#define CRU_CLKSEL16_CON_I2S1_FRAC_FACTOR_MASK             (0xFFFFFFFFU << CRU_CLKSEL16_CON_I2S1_FRAC_FACTOR_SHIFT)     /* 0xFFFFFFFF */
/* CLKGATE0_CON */
#define CRU_CLKGATE0_CON_OFFSET                            (0x80U)
#define CRU_CLKGATE0_CON_CLK_SYS_CORE_GATE_EN_SHIFT        (0U)
#define CRU_CLKGATE0_CON_CLK_SYS_CORE_GATE_EN_MASK         (0x1U << CRU_CLKGATE0_CON_CLK_SYS_CORE_GATE_EN_SHIFT)        /* 0x00000001 */
#define CRU_CLKGATE0_CON_HCLK_SYS_CORE_GATE_EN_SHIFT       (1U)
#define CRU_CLKGATE0_CON_HCLK_SYS_CORE_GATE_EN_MASK        (0x1U << CRU_CLKGATE0_CON_HCLK_SYS_CORE_GATE_EN_SHIFT)       /* 0x00000002 */
#define CRU_CLKGATE0_CON_HCLK_CAL_CORE_GATE_EN_SHIFT       (2U)
#define CRU_CLKGATE0_CON_HCLK_CAL_CORE_GATE_EN_MASK        (0x1U << CRU_CLKGATE0_CON_HCLK_CAL_CORE_GATE_EN_SHIFT)       /* 0x00000004 */
#define CRU_CLKGATE0_CON_HCLK_LOGIC_GATE_EN_SHIFT          (3U)
#define CRU_CLKGATE0_CON_HCLK_LOGIC_GATE_EN_MASK           (0x1U << CRU_CLKGATE0_CON_HCLK_LOGIC_GATE_EN_SHIFT)          /* 0x00000008 */
#define CRU_CLKGATE0_CON_PCLK_LOGIC_GATE_EN_SHIFT          (4U)
#define CRU_CLKGATE0_CON_PCLK_LOGIC_GATE_EN_MASK           (0x1U << CRU_CLKGATE0_CON_PCLK_LOGIC_GATE_EN_SHIFT)          /* 0x00000010 */
#define CRU_CLKGATE0_CON_HCLK_HIGH_GATE_EN_SHIFT           (5U)
#define CRU_CLKGATE0_CON_HCLK_HIGH_GATE_EN_MASK            (0x1U << CRU_CLKGATE0_CON_HCLK_HIGH_GATE_EN_SHIFT)           /* 0x00000020 */
#define CRU_CLKGATE0_CON_HCLK_PMU_GATE_EN_SHIFT            (6U)
#define CRU_CLKGATE0_CON_HCLK_PMU_GATE_EN_MASK             (0x1U << CRU_CLKGATE0_CON_HCLK_PMU_GATE_EN_SHIFT)            /* 0x00000040 */
#define CRU_CLKGATE0_CON_CLK_USBPHY_GATE_EN_SHIFT          (7U)
#define CRU_CLKGATE0_CON_CLK_USBPHY_GATE_EN_MASK           (0x1U << CRU_CLKGATE0_CON_CLK_USBPHY_GATE_EN_SHIFT)          /* 0x00000080 */
#define CRU_CLKGATE0_CON_CLK_SFC_GATE_EN_SHIFT             (8U)
#define CRU_CLKGATE0_CON_CLK_SFC_GATE_EN_MASK              (0x1U << CRU_CLKGATE0_CON_CLK_SFC_GATE_EN_SHIFT)             /* 0x00000100 */
#define CRU_CLKGATE0_CON_CLK_HIFI_GATE_EN_SHIFT            (9U)
#define CRU_CLKGATE0_CON_CLK_HIFI_GATE_EN_MASK             (0x1U << CRU_CLKGATE0_CON_CLK_HIFI_GATE_EN_SHIFT)            /* 0x00000200 */
#define CRU_CLKGATE0_CON_CLK_SARADC_GATE_EN_SHIFT          (10U)
#define CRU_CLKGATE0_CON_CLK_SARADC_GATE_EN_MASK           (0x1U << CRU_CLKGATE0_CON_CLK_SARADC_GATE_EN_SHIFT)          /* 0x00000400 */
#define CRU_CLKGATE0_CON_CLK_TIMER0_GATE_EN_SHIFT          (11U)
#define CRU_CLKGATE0_CON_CLK_TIMER0_GATE_EN_MASK           (0x1U << CRU_CLKGATE0_CON_CLK_TIMER0_GATE_EN_SHIFT)          /* 0x00000800 */
#define CRU_CLKGATE0_CON_CLK_TIMER1_GATE_EN_SHIFT          (12U)
#define CRU_CLKGATE0_CON_CLK_TIMER1_GATE_EN_MASK           (0x1U << CRU_CLKGATE0_CON_CLK_TIMER1_GATE_EN_SHIFT)          /* 0x00001000 */
#define CRU_CLKGATE0_CON_PCLK_PMU_GATE_EN_SHIFT            (13U)
#define CRU_CLKGATE0_CON_PCLK_PMU_GATE_EN_MASK             (0x1U << CRU_CLKGATE0_CON_PCLK_PMU_GATE_EN_SHIFT)            /* 0x00002000 */
#define CRU_CLKGATE0_CON_CLK_ACODEC_GATE_EN_SHIFT          (14U)
#define CRU_CLKGATE0_CON_CLK_ACODEC_GATE_EN_MASK           (0x1U << CRU_CLKGATE0_CON_CLK_ACODEC_GATE_EN_SHIFT)          /* 0x00004000 */
/* CLKGATE1_CON */
#define CRU_CLKGATE1_CON_OFFSET                            (0x84U)
#define CRU_CLKGATE1_CON_CLK_I2S1_SRC_GATE_EN_SHIFT        (0U)
#define CRU_CLKGATE1_CON_CLK_I2S1_SRC_GATE_EN_MASK         (0x1U << CRU_CLKGATE1_CON_CLK_I2S1_SRC_GATE_EN_SHIFT)        /* 0x00000001 */
#define CRU_CLKGATE1_CON_CLK_I2S1_FRAC_SRC_GATE_EN_SHIFT   (1U)
#define CRU_CLKGATE1_CON_CLK_I2S1_FRAC_SRC_GATE_EN_MASK    (0x1U << CRU_CLKGATE1_CON_CLK_I2S1_FRAC_SRC_GATE_EN_SHIFT)   /* 0x00000002 */
#define CRU_CLKGATE1_CON_CLK_I2S1_OUT_GATE_EN_SHIFT        (2U)
#define CRU_CLKGATE1_CON_CLK_I2S1_OUT_GATE_EN_MASK         (0x1U << CRU_CLKGATE1_CON_CLK_I2S1_OUT_GATE_EN_SHIFT)        /* 0x00000004 */
#define CRU_CLKGATE1_CON_CLK_I2S1_GATE_EN_SHIFT            (3U)
#define CRU_CLKGATE1_CON_CLK_I2S1_GATE_EN_MASK             (0x1U << CRU_CLKGATE1_CON_CLK_I2S1_GATE_EN_SHIFT)            /* 0x00000008 */
#define CRU_CLKGATE1_CON_CLK_I2S0_SRC_GATE_EN_SHIFT        (4U)
#define CRU_CLKGATE1_CON_CLK_I2S0_SRC_GATE_EN_MASK         (0x1U << CRU_CLKGATE1_CON_CLK_I2S0_SRC_GATE_EN_SHIFT)        /* 0x00000010 */
#define CRU_CLKGATE1_CON_CLK_I2S_FRAC_SRC_GATE_EN_SHIFT    (5U)
#define CRU_CLKGATE1_CON_CLK_I2S_FRAC_SRC_GATE_EN_MASK     (0x1U << CRU_CLKGATE1_CON_CLK_I2S_FRAC_SRC_GATE_EN_SHIFT)    /* 0x00000020 */
#define CRU_CLKGATE1_CON_CLK_I2S_GATE_EN_SHIFT             (6U)
#define CRU_CLKGATE1_CON_CLK_I2S_GATE_EN_MASK              (0x1U << CRU_CLKGATE1_CON_CLK_I2S_GATE_EN_SHIFT)             /* 0x00000040 */
#define CRU_CLKGATE1_CON_DCLK_EBC_SRC_GATE_EN_SHIFT        (7U)
#define CRU_CLKGATE1_CON_DCLK_EBC_SRC_GATE_EN_MASK         (0x1U << CRU_CLKGATE1_CON_DCLK_EBC_SRC_GATE_EN_SHIFT)        /* 0x00000080 */
#define CRU_CLKGATE1_CON_CLK_SDMMC_GATE_EN_SHIFT           (8U)
#define CRU_CLKGATE1_CON_CLK_SDMMC_GATE_EN_MASK            (0x1U << CRU_CLKGATE1_CON_CLK_SDMMC_GATE_EN_SHIFT)           /* 0x00000100 */
#define CRU_CLKGATE1_CON_CLK_EMMC_GATE_EN_SHIFT            (9U)
#define CRU_CLKGATE1_CON_CLK_EMMC_GATE_EN_MASK             (0x1U << CRU_CLKGATE1_CON_CLK_EMMC_GATE_EN_SHIFT)            /* 0x00000200 */
#define CRU_CLKGATE1_CON_CLK_SPI0_GATE_EN_SHIFT            (10U)
#define CRU_CLKGATE1_CON_CLK_SPI0_GATE_EN_MASK             (0x1U << CRU_CLKGATE1_CON_CLK_SPI0_GATE_EN_SHIFT)            /* 0x00000400 */
#define CRU_CLKGATE1_CON_CLK_SPI1_GATE_EN_SHIFT            (11U)
#define CRU_CLKGATE1_CON_CLK_SPI1_GATE_EN_MASK             (0x1U << CRU_CLKGATE1_CON_CLK_SPI1_GATE_EN_SHIFT)            /* 0x00000800 */
#define CRU_CLKGATE1_CON_CLK_UART0_GATE_EN_SHIFT           (12U)
#define CRU_CLKGATE1_CON_CLK_UART0_GATE_EN_MASK            (0x1U << CRU_CLKGATE1_CON_CLK_UART0_GATE_EN_SHIFT)           /* 0x00001000 */
#define CRU_CLKGATE1_CON_CLK_UART1_GATE_EN_SHIFT           (13U)
#define CRU_CLKGATE1_CON_CLK_UART1_GATE_EN_MASK            (0x1U << CRU_CLKGATE1_CON_CLK_UART1_GATE_EN_SHIFT)           /* 0x00002000 */
#define CRU_CLKGATE1_CON_CLK_UART2_GATE_EN_SHIFT           (14U)
#define CRU_CLKGATE1_CON_CLK_UART2_GATE_EN_MASK            (0x1U << CRU_CLKGATE1_CON_CLK_UART2_GATE_EN_SHIFT)           /* 0x00004000 */
#define CRU_CLKGATE1_CON_CLK_TEST_GATE_EN_SHIFT            (15U)
#define CRU_CLKGATE1_CON_CLK_TEST_GATE_EN_MASK             (0x1U << CRU_CLKGATE1_CON_CLK_TEST_GATE_EN_SHIFT)            /* 0x00008000 */
/* CLKGATE2_CON */
#define CRU_CLKGATE2_CON_OFFSET                            (0x88U)
#define CRU_CLKGATE2_CON_CLK_UART3_GATE_EN_SHIFT           (0U)
#define CRU_CLKGATE2_CON_CLK_UART3_GATE_EN_MASK            (0x1U << CRU_CLKGATE2_CON_CLK_UART3_GATE_EN_SHIFT)           /* 0x00000001 */
#define CRU_CLKGATE2_CON_CLK_UART4_GATE_EN_SHIFT           (1U)
#define CRU_CLKGATE2_CON_CLK_UART4_GATE_EN_MASK            (0x1U << CRU_CLKGATE2_CON_CLK_UART4_GATE_EN_SHIFT)           /* 0x00000002 */
#define CRU_CLKGATE2_CON_CLK_UART5_GATE_EN_SHIFT           (2U)
#define CRU_CLKGATE2_CON_CLK_UART5_GATE_EN_MASK            (0x1U << CRU_CLKGATE2_CON_CLK_UART5_GATE_EN_SHIFT)           /* 0x00000004 */
/* CLKGATE3_CON */
#define CRU_CLKGATE3_CON_OFFSET                            (0x8CU)
#define CRU_CLKGATE3_CON_HDRAM0_GATE_EN_SHIFT              (0U)
#define CRU_CLKGATE3_CON_HDRAM0_GATE_EN_MASK               (0x1U << CRU_CLKGATE3_CON_HDRAM0_GATE_EN_SHIFT)              /* 0x00000001 */
#define CRU_CLKGATE3_CON_HDRAM1_GATE_EN_SHIFT              (1U)
#define CRU_CLKGATE3_CON_HDRAM1_GATE_EN_MASK               (0x1U << CRU_CLKGATE3_CON_HDRAM1_GATE_EN_SHIFT)              /* 0x00000002 */
#define CRU_CLKGATE3_CON_HDRAM2_GATE_EN_SHIFT              (2U)
#define CRU_CLKGATE3_CON_HDRAM2_GATE_EN_MASK               (0x1U << CRU_CLKGATE3_CON_HDRAM2_GATE_EN_SHIFT)              /* 0x00000004 */
#define CRU_CLKGATE3_CON_HDRAM3_GATE_EN_SHIFT              (3U)
#define CRU_CLKGATE3_CON_HDRAM3_GATE_EN_MASK               (0x1U << CRU_CLKGATE3_CON_HDRAM3_GATE_EN_SHIFT)              /* 0x00000008 */
#define CRU_CLKGATE3_CON_HDRAM4_GATE_EN_SHIFT              (4U)
#define CRU_CLKGATE3_CON_HDRAM4_GATE_EN_MASK               (0x1U << CRU_CLKGATE3_CON_HDRAM4_GATE_EN_SHIFT)              /* 0x00000010 */
#define CRU_CLKGATE3_CON_HDRAM5_GATE_EN_SHIFT              (5U)
#define CRU_CLKGATE3_CON_HDRAM5_GATE_EN_MASK               (0x1U << CRU_CLKGATE3_CON_HDRAM5_GATE_EN_SHIFT)              /* 0x00000020 */
#define CRU_CLKGATE3_CON_HDRAM6_GATE_EN_SHIFT              (6U)
#define CRU_CLKGATE3_CON_HDRAM6_GATE_EN_MASK               (0x1U << CRU_CLKGATE3_CON_HDRAM6_GATE_EN_SHIFT)              /* 0x00000040 */
#define CRU_CLKGATE3_CON_HDRAM7_GATE_EN_SHIFT              (7U)
#define CRU_CLKGATE3_CON_HDRAM7_GATE_EN_MASK               (0x1U << CRU_CLKGATE3_CON_HDRAM7_GATE_EN_SHIFT)              /* 0x00000080 */
#define CRU_CLKGATE3_CON_HDRAM_INTERFACE_GATE_EN_SHIFT     (8U)
#define CRU_CLKGATE3_CON_HDRAM_INTERFACE_GATE_EN_MASK      (0x1U << CRU_CLKGATE3_CON_HDRAM_INTERFACE_GATE_EN_SHIFT)     /* 0x00000100 */
#define CRU_CLKGATE3_CON_HCLK_HIFI_GATE_EN_SHIFT           (9U)
#define CRU_CLKGATE3_CON_HCLK_HIFI_GATE_EN_MASK            (0x1U << CRU_CLKGATE3_CON_HCLK_HIFI_GATE_EN_SHIFT)           /* 0x00000200 */
/* CLKGATE4_CON */
#define CRU_CLKGATE4_CON_OFFSET                            (0x90U)
#define CRU_CLKGATE4_CON_HIRAM0_GATE_EN_SHIFT              (0U)
#define CRU_CLKGATE4_CON_HIRAM0_GATE_EN_MASK               (0x1U << CRU_CLKGATE4_CON_HIRAM0_GATE_EN_SHIFT)              /* 0x00000001 */
#define CRU_CLKGATE4_CON_HIRAM1_GATE_EN_SHIFT              (1U)
#define CRU_CLKGATE4_CON_HIRAM1_GATE_EN_MASK               (0x1U << CRU_CLKGATE4_CON_HIRAM1_GATE_EN_SHIFT)              /* 0x00000002 */
#define CRU_CLKGATE4_CON_HIRAM2_GATE_EN_SHIFT              (2U)
#define CRU_CLKGATE4_CON_HIRAM2_GATE_EN_MASK               (0x1U << CRU_CLKGATE4_CON_HIRAM2_GATE_EN_SHIFT)              /* 0x00000004 */
#define CRU_CLKGATE4_CON_HIRAM3_GATE_EN_SHIFT              (3U)
#define CRU_CLKGATE4_CON_HIRAM3_GATE_EN_MASK               (0x1U << CRU_CLKGATE4_CON_HIRAM3_GATE_EN_SHIFT)              /* 0x00000008 */
#define CRU_CLKGATE4_CON_HIRAM4_GATE_EN_SHIFT              (4U)
#define CRU_CLKGATE4_CON_HIRAM4_GATE_EN_MASK               (0x1U << CRU_CLKGATE4_CON_HIRAM4_GATE_EN_SHIFT)              /* 0x00000010 */
#define CRU_CLKGATE4_CON_HIRAM5_GATE_EN_SHIFT              (5U)
#define CRU_CLKGATE4_CON_HIRAM5_GATE_EN_MASK               (0x1U << CRU_CLKGATE4_CON_HIRAM5_GATE_EN_SHIFT)              /* 0x00000020 */
#define CRU_CLKGATE4_CON_HIRAM6_GATE_EN_SHIFT              (6U)
#define CRU_CLKGATE4_CON_HIRAM6_GATE_EN_MASK               (0x1U << CRU_CLKGATE4_CON_HIRAM6_GATE_EN_SHIFT)              /* 0x00000040 */
#define CRU_CLKGATE4_CON_HIRAM7_GATE_EN_SHIFT              (7U)
#define CRU_CLKGATE4_CON_HIRAM7_GATE_EN_MASK               (0x1U << CRU_CLKGATE4_CON_HIRAM7_GATE_EN_SHIFT)              /* 0x00000080 */
#define CRU_CLKGATE4_CON_HIRAM_INTERFACE_GATE_EN_SHIFT     (8U)
#define CRU_CLKGATE4_CON_HIRAM_INTERFACE_GATE_EN_MASK      (0x1U << CRU_CLKGATE4_CON_HIRAM_INTERFACE_GATE_EN_SHIFT)     /* 0x00000100 */
/* CLKGATE5_CON */
#define CRU_CLKGATE5_CON_OFFSET                            (0x94U)
#define CRU_CLKGATE5_CON_PCLK_PMU_GATE_EN_SHIFT            (0U)
#define CRU_CLKGATE5_CON_PCLK_PMU_GATE_EN_MASK             (0x1U << CRU_CLKGATE5_CON_PCLK_PMU_GATE_EN_SHIFT)            /* 0x00000001 */
#define CRU_CLKGATE5_CON_PCLK_GRF_GATE_EN_SHIFT            (3U)
#define CRU_CLKGATE5_CON_PCLK_GRF_GATE_EN_MASK             (0x1U << CRU_CLKGATE5_CON_PCLK_GRF_GATE_EN_SHIFT)            /* 0x00000008 */
#define CRU_CLKGATE5_CON_PCLK_GPIO2_GATE_EN_SHIFT          (4U)
#define CRU_CLKGATE5_CON_PCLK_GPIO2_GATE_EN_MASK           (0x1U << CRU_CLKGATE5_CON_PCLK_GPIO2_GATE_EN_SHIFT)          /* 0x00000010 */
#define CRU_CLKGATE5_CON_PMU_RAM_INTERFACE_GATE_EN_SHIFT   (6U)
#define CRU_CLKGATE5_CON_PMU_RAM_INTERFACE_GATE_EN_MASK    (0x1U << CRU_CLKGATE5_CON_PMU_RAM_INTERFACE_GATE_EN_SHIFT)   /* 0x00000040 */
#define CRU_CLKGATE5_CON_PMU_RAM0_GATE_EN_SHIFT            (7U)
#define CRU_CLKGATE5_CON_PMU_RAM0_GATE_EN_MASK             (0x1U << CRU_CLKGATE5_CON_PMU_RAM0_GATE_EN_SHIFT)            /* 0x00000080 */
#define CRU_CLKGATE5_CON_PMU_RAM1_GATE_EN_SHIFT            (8U)
#define CRU_CLKGATE5_CON_PMU_RAM1_GATE_EN_MASK             (0x1U << CRU_CLKGATE5_CON_PMU_RAM1_GATE_EN_SHIFT)            /* 0x00000100 */
#define CRU_CLKGATE5_CON_HCLK_APB_BRG_GATE_EN_SHIFT        (9U)
#define CRU_CLKGATE5_CON_HCLK_APB_BRG_GATE_EN_MASK         (0x1U << CRU_CLKGATE5_CON_HCLK_APB_BRG_GATE_EN_SHIFT)        /* 0x00000200 */
/* CLKGATE6_CON */
#define CRU_CLKGATE6_CON_OFFSET                            (0x98U)
#define CRU_CLKGATE6_CON_DRAM0_GATE_EN_SHIFT               (0U)
#define CRU_CLKGATE6_CON_DRAM0_GATE_EN_MASK                (0x1U << CRU_CLKGATE6_CON_DRAM0_GATE_EN_SHIFT)               /* 0x00000001 */
#define CRU_CLKGATE6_CON_DRAM1_GATE_EN_SHIFT               (1U)
#define CRU_CLKGATE6_CON_DRAM1_GATE_EN_MASK                (0x1U << CRU_CLKGATE6_CON_DRAM1_GATE_EN_SHIFT)               /* 0x00000002 */
#define CRU_CLKGATE6_CON_DRAM2_GATE_EN_SHIFT               (2U)
#define CRU_CLKGATE6_CON_DRAM2_GATE_EN_MASK                (0x1U << CRU_CLKGATE6_CON_DRAM2_GATE_EN_SHIFT)               /* 0x00000004 */
#define CRU_CLKGATE6_CON_DRAM3_GATE_EN_SHIFT               (3U)
#define CRU_CLKGATE6_CON_DRAM3_GATE_EN_MASK                (0x1U << CRU_CLKGATE6_CON_DRAM3_GATE_EN_SHIFT)               /* 0x00000008 */
#define CRU_CLKGATE6_CON_DRAM4_GATE_EN_SHIFT               (4U)
#define CRU_CLKGATE6_CON_DRAM4_GATE_EN_MASK                (0x1U << CRU_CLKGATE6_CON_DRAM4_GATE_EN_SHIFT)               /* 0x00000010 */
#define CRU_CLKGATE6_CON_DRAM5_GATE_EN_SHIFT               (5U)
#define CRU_CLKGATE6_CON_DRAM5_GATE_EN_MASK                (0x1U << CRU_CLKGATE6_CON_DRAM5_GATE_EN_SHIFT)               /* 0x00000020 */
#define CRU_CLKGATE6_CON_DRAM6_GATE_EN_SHIFT               (6U)
#define CRU_CLKGATE6_CON_DRAM6_GATE_EN_MASK                (0x1U << CRU_CLKGATE6_CON_DRAM6_GATE_EN_SHIFT)               /* 0x00000040 */
#define CRU_CLKGATE6_CON_DRAM7_GATE_EN_SHIFT               (7U)
#define CRU_CLKGATE6_CON_DRAM7_GATE_EN_MASK                (0x1U << CRU_CLKGATE6_CON_DRAM7_GATE_EN_SHIFT)               /* 0x00000080 */
#define CRU_CLKGATE6_CON_DRAM_INTERFACE_GATE_EN_SHIFT      (8U)
#define CRU_CLKGATE6_CON_DRAM_INTERFACE_GATE_EN_MASK       (0x1U << CRU_CLKGATE6_CON_DRAM_INTERFACE_GATE_EN_SHIFT)      /* 0x00000100 */
#define CRU_CLKGATE6_CON_HCLK_APB_BRG_GATE_EN_SHIFT        (9U)
#define CRU_CLKGATE6_CON_HCLK_APB_BRG_GATE_EN_MASK         (0x1U << CRU_CLKGATE6_CON_HCLK_APB_BRG_GATE_EN_SHIFT)        /* 0x00000200 */
#define CRU_CLKGATE6_CON_HCLK_USBC_GATE_EN_SHIFT           (10U)
#define CRU_CLKGATE6_CON_HCLK_USBC_GATE_EN_MASK            (0x1U << CRU_CLKGATE6_CON_HCLK_USBC_GATE_EN_SHIFT)           /* 0x00000400 */
#define CRU_CLKGATE6_CON_HCLK_SDMMC_GATE_EN_SHIFT          (11U)
#define CRU_CLKGATE6_CON_HCLK_SDMMC_GATE_EN_MASK           (0x1U << CRU_CLKGATE6_CON_HCLK_SDMMC_GATE_EN_SHIFT)          /* 0x00000800 */
#define CRU_CLKGATE6_CON_HCLK_EMMC_GATE_EN_SHIFT           (12U)
#define CRU_CLKGATE6_CON_HCLK_EMMC_GATE_EN_MASK            (0x1U << CRU_CLKGATE6_CON_HCLK_EMMC_GATE_EN_SHIFT)           /* 0x00001000 */
#define CRU_CLKGATE6_CON_HCLK_EBC_GATE_EN_SHIFT            (13U)
#define CRU_CLKGATE6_CON_HCLK_EBC_GATE_EN_MASK             (0x1U << CRU_CLKGATE6_CON_HCLK_EBC_GATE_EN_SHIFT)            /* 0x00002000 */
#define CRU_CLKGATE6_CON_HCLK_SYNTH_GATE_EN_SHIFT          (14U)
#define CRU_CLKGATE6_CON_HCLK_SYNTH_GATE_EN_MASK           (0x1U << CRU_CLKGATE6_CON_HCLK_SYNTH_GATE_EN_SHIFT)          /* 0x00004000 */
#define CRU_CLKGATE6_CON_HCLK_SFC_GATE_EN_SHIFT            (15U)
#define CRU_CLKGATE6_CON_HCLK_SFC_GATE_EN_MASK             (0x1U << CRU_CLKGATE6_CON_HCLK_SFC_GATE_EN_SHIFT)            /* 0x00008000 */
/* CLKGATE7_CON */
#define CRU_CLKGATE7_CON_OFFSET                            (0x9CU)
#define CRU_CLKGATE7_CON_IRAM0_GATE_EN_SHIFT               (0U)
#define CRU_CLKGATE7_CON_IRAM0_GATE_EN_MASK                (0x1U << CRU_CLKGATE7_CON_IRAM0_GATE_EN_SHIFT)               /* 0x00000001 */
#define CRU_CLKGATE7_CON_IRAM1_GATE_EN_SHIFT               (1U)
#define CRU_CLKGATE7_CON_IRAM1_GATE_EN_MASK                (0x1U << CRU_CLKGATE7_CON_IRAM1_GATE_EN_SHIFT)               /* 0x00000002 */
#define CRU_CLKGATE7_CON_IRAM2_GATE_EN_SHIFT               (2U)
#define CRU_CLKGATE7_CON_IRAM2_GATE_EN_MASK                (0x1U << CRU_CLKGATE7_CON_IRAM2_GATE_EN_SHIFT)               /* 0x00000004 */
#define CRU_CLKGATE7_CON_IRAM3_GATE_EN_SHIFT               (3U)
#define CRU_CLKGATE7_CON_IRAM3_GATE_EN_MASK                (0x1U << CRU_CLKGATE7_CON_IRAM3_GATE_EN_SHIFT)               /* 0x00000008 */
#define CRU_CLKGATE7_CON_IRAM4_GATE_EN_SHIFT               (4U)
#define CRU_CLKGATE7_CON_IRAM4_GATE_EN_MASK                (0x1U << CRU_CLKGATE7_CON_IRAM4_GATE_EN_SHIFT)               /* 0x00000010 */
#define CRU_CLKGATE7_CON_IRAM5_GATE_EN_SHIFT               (5U)
#define CRU_CLKGATE7_CON_IRAM5_GATE_EN_MASK                (0x1U << CRU_CLKGATE7_CON_IRAM5_GATE_EN_SHIFT)               /* 0x00000020 */
#define CRU_CLKGATE7_CON_IRAM6_GATE_EN_SHIFT               (6U)
#define CRU_CLKGATE7_CON_IRAM6_GATE_EN_MASK                (0x1U << CRU_CLKGATE7_CON_IRAM6_GATE_EN_SHIFT)               /* 0x00000040 */
#define CRU_CLKGATE7_CON_IRAM7_GATE_EN_SHIFT               (7U)
#define CRU_CLKGATE7_CON_IRAM7_GATE_EN_MASK                (0x1U << CRU_CLKGATE7_CON_IRAM7_GATE_EN_SHIFT)               /* 0x00000080 */
#define CRU_CLKGATE7_CON_IRAM_INTERFACE_GATE_EN_SHIFT      (8U)
#define CRU_CLKGATE7_CON_IRAM_INTERFACE_GATE_EN_MASK       (0x1U << CRU_CLKGATE7_CON_IRAM_INTERFACE_GATE_EN_SHIFT)      /* 0x00000100 */
#define CRU_CLKGATE7_CON_HCLK_BOOTROM_GATE_EN_SHIFT        (9U)
#define CRU_CLKGATE7_CON_HCLK_BOOTROM_GATE_EN_MASK         (0x1U << CRU_CLKGATE7_CON_HCLK_BOOTROM_GATE_EN_SHIFT)        /* 0x00000200 */
#define CRU_CLKGATE7_CON_HCLK_BUS_MATRIX_GATE_EN_SHIFT     (10U)
#define CRU_CLKGATE7_CON_HCLK_BUS_MATRIX_GATE_EN_MASK      (0x1U << CRU_CLKGATE7_CON_HCLK_BUS_MATRIX_GATE_EN_SHIFT)     /* 0x00000400 */
#define CRU_CLKGATE7_CON_HCLK_AHB_BRG_GATE_EN_SHIFT        (11U)
#define CRU_CLKGATE7_CON_HCLK_AHB_BRG_GATE_EN_MASK         (0x1U << CRU_CLKGATE7_CON_HCLK_AHB_BRG_GATE_EN_SHIFT)        /* 0x00000800 */
#define CRU_CLKGATE7_CON_HCLK_1TOM_BRG_GATE_EN_SHIFT       (12U)
#define CRU_CLKGATE7_CON_HCLK_1TOM_BRG_GATE_EN_MASK        (0x1U << CRU_CLKGATE7_CON_HCLK_1TOM_BRG_GATE_EN_SHIFT)       /* 0x00001000 */
#define CRU_CLKGATE7_CON_HCLK_DMA_GATE_EN_SHIFT            (13U)
#define CRU_CLKGATE7_CON_HCLK_DMA_GATE_EN_MASK             (0x1U << CRU_CLKGATE7_CON_HCLK_DMA_GATE_EN_SHIFT)            /* 0x00002000 */
#define CRU_CLKGATE7_CON_HCLK_IMDCT_GATE_EN_SHIFT          (14U)
#define CRU_CLKGATE7_CON_HCLK_IMDCT_GATE_EN_MASK           (0x1U << CRU_CLKGATE7_CON_HCLK_IMDCT_GATE_EN_SHIFT)          /* 0x00004000 */
#define CRU_CLKGATE7_CON_HCLK_LCDC_GATE_EN_SHIFT           (15U)
#define CRU_CLKGATE7_CON_HCLK_LCDC_GATE_EN_MASK            (0x1U << CRU_CLKGATE7_CON_HCLK_LCDC_GATE_EN_SHIFT)           /* 0x00008000 */
/* CLKGATE8_CON */
#define CRU_CLKGATE8_CON_OFFSET                            (0xA0U)
#define CRU_CLKGATE8_CON_PCLK_I2S0_GATE_EN_SHIFT           (1U)
#define CRU_CLKGATE8_CON_PCLK_I2S0_GATE_EN_MASK            (0x1U << CRU_CLKGATE8_CON_PCLK_I2S0_GATE_EN_SHIFT)           /* 0x00000002 */
#define CRU_CLKGATE8_CON_PCLK_I2S1_GATE_EN_SHIFT           (2U)
#define CRU_CLKGATE8_CON_PCLK_I2S1_GATE_EN_MASK            (0x1U << CRU_CLKGATE8_CON_PCLK_I2S1_GATE_EN_SHIFT)           /* 0x00000004 */
#define CRU_CLKGATE8_CON_PCLK_I2C0_GATE_EN_SHIFT           (3U)
#define CRU_CLKGATE8_CON_PCLK_I2C0_GATE_EN_MASK            (0x1U << CRU_CLKGATE8_CON_PCLK_I2C0_GATE_EN_SHIFT)           /* 0x00000008 */
#define CRU_CLKGATE8_CON_PCLK_I2C1_GATE_EN_SHIFT           (4U)
#define CRU_CLKGATE8_CON_PCLK_I2C1_GATE_EN_MASK            (0x1U << CRU_CLKGATE8_CON_PCLK_I2C1_GATE_EN_SHIFT)           /* 0x00000010 */
#define CRU_CLKGATE8_CON_PCLK_I2C2_GATE_EN_SHIFT           (5U)
#define CRU_CLKGATE8_CON_PCLK_I2C2_GATE_EN_MASK            (0x1U << CRU_CLKGATE8_CON_PCLK_I2C2_GATE_EN_SHIFT)           /* 0x00000020 */
#define CRU_CLKGATE8_CON_PCLK_SPI0_GATE_EN_SHIFT           (6U)
#define CRU_CLKGATE8_CON_PCLK_SPI0_GATE_EN_MASK            (0x1U << CRU_CLKGATE8_CON_PCLK_SPI0_GATE_EN_SHIFT)           /* 0x00000040 */
#define CRU_CLKGATE8_CON_PCLK_SPI1_GATE_EN_SHIFT           (7U)
#define CRU_CLKGATE8_CON_PCLK_SPI1_GATE_EN_MASK            (0x1U << CRU_CLKGATE8_CON_PCLK_SPI1_GATE_EN_SHIFT)           /* 0x00000080 */
#define CRU_CLKGATE8_CON_PCLK_UART0_GATE_EN_SHIFT          (8U)
#define CRU_CLKGATE8_CON_PCLK_UART0_GATE_EN_MASK           (0x1U << CRU_CLKGATE8_CON_PCLK_UART0_GATE_EN_SHIFT)          /* 0x00000100 */
#define CRU_CLKGATE8_CON_PCLK_UART1_GATE_EN_SHIFT          (9U)
#define CRU_CLKGATE8_CON_PCLK_UART1_GATE_EN_MASK           (0x1U << CRU_CLKGATE8_CON_PCLK_UART1_GATE_EN_SHIFT)          /* 0x00000200 */
#define CRU_CLKGATE8_CON_PCLK_UART2_GATE_EN_SHIFT          (10U)
#define CRU_CLKGATE8_CON_PCLK_UART2_GATE_EN_MASK           (0x1U << CRU_CLKGATE8_CON_PCLK_UART2_GATE_EN_SHIFT)          /* 0x00000400 */
#define CRU_CLKGATE8_CON_PCLK_UART3_GATE_EN_SHIFT          (11U)
#define CRU_CLKGATE8_CON_PCLK_UART3_GATE_EN_MASK           (0x1U << CRU_CLKGATE8_CON_PCLK_UART3_GATE_EN_SHIFT)          /* 0x00000800 */
#define CRU_CLKGATE8_CON_PCLK_UART4_GATE_EN_SHIFT          (12U)
#define CRU_CLKGATE8_CON_PCLK_UART4_GATE_EN_MASK           (0x1U << CRU_CLKGATE8_CON_PCLK_UART4_GATE_EN_SHIFT)          /* 0x00001000 */
#define CRU_CLKGATE8_CON_PCLK_UART5_GATE_EN_SHIFT          (13U)
#define CRU_CLKGATE8_CON_PCLK_UART5_GATE_EN_MASK           (0x1U << CRU_CLKGATE8_CON_PCLK_UART5_GATE_EN_SHIFT)          /* 0x00002000 */
#define CRU_CLKGATE8_CON_PCLK_TIMER_GATE_EN_SHIFT          (14U)
#define CRU_CLKGATE8_CON_PCLK_TIMER_GATE_EN_MASK           (0x1U << CRU_CLKGATE8_CON_PCLK_TIMER_GATE_EN_SHIFT)          /* 0x00004000 */
#define CRU_CLKGATE8_CON_PCLK_WDT_GATE_EN_SHIFT            (15U)
#define CRU_CLKGATE8_CON_PCLK_WDT_GATE_EN_MASK             (0x1U << CRU_CLKGATE8_CON_PCLK_WDT_GATE_EN_SHIFT)            /* 0x00008000 */
/* CLKGATE9_CON */
#define CRU_CLKGATE9_CON_OFFSET                            (0xA4U)
#define CRU_CLKGATE9_CON_PCLK_PWM0_GATE_EN_SHIFT           (0U)
#define CRU_CLKGATE9_CON_PCLK_PWM0_GATE_EN_MASK            (0x1U << CRU_CLKGATE9_CON_PCLK_PWM0_GATE_EN_SHIFT)           /* 0x00000001 */
#define CRU_CLKGATE9_CON_PCLK_PWM1_GATE_EN_SHIFT           (1U)
#define CRU_CLKGATE9_CON_PCLK_PWM1_GATE_EN_MASK            (0x1U << CRU_CLKGATE9_CON_PCLK_PWM1_GATE_EN_SHIFT)           /* 0x00000002 */
#define CRU_CLKGATE9_CON_PCLK_MAILBOX_GATE_EN_SHIFT        (2U)
#define CRU_CLKGATE9_CON_PCLK_MAILBOX_GATE_EN_MASK         (0x1U << CRU_CLKGATE9_CON_PCLK_MAILBOX_GATE_EN_SHIFT)        /* 0x00000004 */
#define CRU_CLKGATE9_CON_PCLK_SARADC_GATE_EN_SHIFT         (3U)
#define CRU_CLKGATE9_CON_PCLK_SARADC_GATE_EN_MASK          (0x1U << CRU_CLKGATE9_CON_PCLK_SARADC_GATE_EN_SHIFT)         /* 0x00000008 */
#define CRU_CLKGATE9_CON_PCLK_GPIO0_GATE_EN_SHIFT          (4U)
#define CRU_CLKGATE9_CON_PCLK_GPIO0_GATE_EN_MASK           (0x1U << CRU_CLKGATE9_CON_PCLK_GPIO0_GATE_EN_SHIFT)          /* 0x00000010 */
#define CRU_CLKGATE9_CON_PCLK_GPIO1_GATE_EN_SHIFT          (5U)
#define CRU_CLKGATE9_CON_PCLK_GPIO1_GATE_EN_MASK           (0x1U << CRU_CLKGATE9_CON_PCLK_GPIO1_GATE_EN_SHIFT)          /* 0x00000020 */
/* SOFTRST0_CON */
#define CRU_SOFTRST0_CON_OFFSET                            (0xC0U)
#define CRU_SOFTRST0_CON_CORE_SRST_WDT_SEL_SHIFT           (0U)
#define CRU_SOFTRST0_CON_CORE_SRST_WDT_SEL_MASK            (0x1U << CRU_SOFTRST0_CON_CORE_SRST_WDT_SEL_SHIFT)           /* 0x00000001 */
/* SOFTRST1_CON */
#define CRU_SOFTRST1_CON_OFFSET                            (0xC4U)
#define CRU_SOFTRST1_CON_CORE_SRST_WDT_SEL_SHIFT           (0U)
#define CRU_SOFTRST1_CON_CORE_SRST_WDT_SEL_MASK            (0x1U << CRU_SOFTRST1_CON_CORE_SRST_WDT_SEL_SHIFT)           /* 0x00000001 */
/* SOFTRST2_CON */
#define CRU_SOFTRST2_CON_OFFSET                            (0xC8U)
#define CRU_SOFTRST2_CON_CORE_SRST_WDT_SEL_SHIFT           (0U)
#define CRU_SOFTRST2_CON_CORE_SRST_WDT_SEL_MASK            (0x1U << CRU_SOFTRST2_CON_CORE_SRST_WDT_SEL_SHIFT)           /* 0x00000001 */
/* SOFTRST3_CON */
#define CRU_SOFTRST3_CON_OFFSET                            (0xCCU)
#define CRU_SOFTRST3_CON_CORE_SRST_WDT_SEL_SHIFT           (0U)
#define CRU_SOFTRST3_CON_CORE_SRST_WDT_SEL_MASK            (0x1U << CRU_SOFTRST3_CON_CORE_SRST_WDT_SEL_SHIFT)           /* 0x00000001 */
/* SOFTRST4_CON */
#define CRU_SOFTRST4_CON_OFFSET                            (0xD0U)
#define CRU_SOFTRST4_CON_CORE_SRST_WDT_SEL_SHIFT           (0U)
#define CRU_SOFTRST4_CON_CORE_SRST_WDT_SEL_MASK            (0x1U << CRU_SOFTRST4_CON_CORE_SRST_WDT_SEL_SHIFT)           /* 0x00000001 */
/* STCLK_CON0 */
#define CRU_STCLK_CON0_OFFSET                              (0xE0U)
#define CRU_STCLK_CON0_STCLK_COUNTER_SHIFT                 (0U)
#define CRU_STCLK_CON0_STCLK_COUNTER_MASK                  (0xFFFFFFU << CRU_STCLK_CON0_STCLK_COUNTER_SHIFT)            /* 0x00FFFFFF */
/* STCLK_CON1 */
#define CRU_STCLK_CON1_OFFSET                              (0xE4U)
#define CRU_STCLK_CON1_STCLK_COUNTER_SHIFT                 (0U)
#define CRU_STCLK_CON1_STCLK_COUNTER_MASK                  (0xFFFFFFU << CRU_STCLK_CON1_STCLK_COUNTER_SHIFT)            /* 0x00FFFFFF */
/* GLB_SRST_FST_VALUE */
#define CRU_GLB_SRST_FST_VALUE_OFFSET                      (0xF4U)
#define CRU_GLB_SRST_FST_VALUE_GLB_SRST_FST_VALUE_SHIFT    (0U)
#define CRU_GLB_SRST_FST_VALUE_GLB_SRST_FST_VALUE_MASK     (0xFFFFU << CRU_GLB_SRST_FST_VALUE_GLB_SRST_FST_VALUE_SHIFT) /* 0x0000FFFF */
/* GLB_CNT_TH */
#define CRU_GLB_CNT_TH_OFFSET                              (0xF8U)
#define CRU_GLB_CNT_TH_GLB_RST_CNT_TH_SHIFT                (0U)
#define CRU_GLB_CNT_TH_GLB_RST_CNT_TH_MASK                 (0x3FFU << CRU_GLB_CNT_TH_GLB_RST_CNT_TH_SHIFT)              /* 0x000003FF */
#define CRU_GLB_CNT_TH_PLL_LOCK_PERIOD_SHIFT               (16U)
#define CRU_GLB_CNT_TH_PLL_LOCK_PERIOD_MASK                (0xFFFFU << CRU_GLB_CNT_TH_PLL_LOCK_PERIOD_SHIFT)            /* 0xFFFF0000 */
/******************************************I2S*******************************************/
/* TXCR */
#define I2S_TXCR_OFFSET                                    (0x0U)
#define I2S_TXCR_VDW_SHIFT                                 (0U)
#define I2S_TXCR_VDW_MASK                                  (0x1FU << I2S_TXCR_VDW_SHIFT)                                /* 0x0000001F */
#define I2S_TXCR_TFS_SHIFT                                 (5U)
#define I2S_TXCR_TFS_MASK                                  (0x1U << I2S_TXCR_TFS_SHIFT)                                 /* 0x00000020 */
#define I2S_TXCR_PBM_SHIFT                                 (7U)
#define I2S_TXCR_PBM_MASK                                  (0x3U << I2S_TXCR_PBM_SHIFT)                                 /* 0x00000180 */
#define I2S_TXCR_IBM_SHIFT                                 (9U)
#define I2S_TXCR_IBM_MASK                                  (0x3U << I2S_TXCR_IBM_SHIFT)                                 /* 0x00000600 */
#define I2S_TXCR_FBM_SHIFT                                 (11U)
#define I2S_TXCR_FBM_MASK                                  (0x1U << I2S_TXCR_FBM_SHIFT)                                 /* 0x00000800 */
#define I2S_TXCR_SJM_SHIFT                                 (12U)
#define I2S_TXCR_SJM_MASK                                  (0x1U << I2S_TXCR_SJM_SHIFT)                                 /* 0x00001000 */
#define I2S_TXCR_HWT_SHIFT                                 (14U)
#define I2S_TXCR_HWT_MASK                                  (0x1U << I2S_TXCR_HWT_SHIFT)                                 /* 0x00004000 */
#define I2S_TXCR_TCSR_SHIFT                                (15U)
#define I2S_TXCR_TCSR_MASK                                 (0x3U << I2S_TXCR_TCSR_SHIFT)                                /* 0x00018000 */
#define I2S_TXCR_RCNT_SHIFT                                (17U)
#define I2S_TXCR_RCNT_MASK                                 (0x3FU << I2S_TXCR_RCNT_SHIFT)                               /* 0x007E0000 */
/* RXCR */
#define I2S_RXCR_OFFSET                                    (0x4U)
#define I2S_RXCR_VDW_SHIFT                                 (0U)
#define I2S_RXCR_VDW_MASK                                  (0x1FU << I2S_RXCR_VDW_SHIFT)                                /* 0x0000001F */
#define I2S_RXCR_TFS_SHIFT                                 (5U)
#define I2S_RXCR_TFS_MASK                                  (0x1U << I2S_RXCR_TFS_SHIFT)                                 /* 0x00000020 */
#define I2S_RXCR_PBM_SHIFT                                 (7U)
#define I2S_RXCR_PBM_MASK                                  (0x3U << I2S_RXCR_PBM_SHIFT)                                 /* 0x00000180 */
#define I2S_RXCR_IBM_SHIFT                                 (9U)
#define I2S_RXCR_IBM_MASK                                  (0x3U << I2S_RXCR_IBM_SHIFT)                                 /* 0x00000600 */
#define I2S_RXCR_FBM_SHIFT                                 (11U)
#define I2S_RXCR_FBM_MASK                                  (0x1U << I2S_RXCR_FBM_SHIFT)                                 /* 0x00000800 */
#define I2S_RXCR_SJM_SHIFT                                 (12U)
#define I2S_RXCR_SJM_MASK                                  (0x1U << I2S_RXCR_SJM_SHIFT)                                 /* 0x00001000 */
#define I2S_RXCR_HWT_SHIFT                                 (14U)
#define I2S_RXCR_HWT_MASK                                  (0x1U << I2S_RXCR_HWT_SHIFT)                                 /* 0x00004000 */
#define I2S_RXCR_RCSR_SHIFT                                (15U)
#define I2S_RXCR_RCSR_MASK                                 (0x3U << I2S_RXCR_RCSR_SHIFT)                                /* 0x00018000 */
/* CKR */
#define I2S_CKR_OFFSET                                     (0x8U)
#define I2S_CKR_TSD_SHIFT                                  (0U)
#define I2S_CKR_TSD_MASK                                   (0xFFU << I2S_CKR_TSD_SHIFT)                                 /* 0x000000FF */
#define I2S_CKR_RSD_SHIFT                                  (8U)
#define I2S_CKR_RSD_MASK                                   (0xFFU << I2S_CKR_RSD_SHIFT)                                 /* 0x0000FF00 */
#define I2S_CKR_MDIV_SHIFT                                 (16U)
#define I2S_CKR_MDIV_MASK                                  (0xFFU << I2S_CKR_MDIV_SHIFT)                                /* 0x00FF0000 */
#define I2S_CKR_TLP_SHIFT                                  (24U)
#define I2S_CKR_TLP_MASK                                   (0x1U << I2S_CKR_TLP_SHIFT)                                  /* 0x01000000 */
#define I2S_CKR_RLP_SHIFT                                  (25U)
#define I2S_CKR_RLP_MASK                                   (0x1U << I2S_CKR_RLP_SHIFT)                                  /* 0x02000000 */
#define I2S_CKR_CKP_SHIFT                                  (26U)
#define I2S_CKR_CKP_MASK                                   (0x1U << I2S_CKR_CKP_SHIFT)                                  /* 0x04000000 */
#define I2S_CKR_MSS_SHIFT                                  (27U)
#define I2S_CKR_MSS_MASK                                   (0x1U << I2S_CKR_MSS_SHIFT)                                  /* 0x08000000 */
/* FIFOLR */
#define I2S_FIFOLR_OFFSET                                  (0xCU)
#define I2S_FIFOLR                                         (0x0U)
#define I2S_FIFOLR_TFL_SHIFT                               (0U)
#define I2S_FIFOLR_TFL_MASK                                (0x3FU << I2S_FIFOLR_TFL_SHIFT)                              /* 0x0000003F */
#define I2S_FIFOLR_RFL_SHIFT                               (24U)
#define I2S_FIFOLR_RFL_MASK                                (0x3FU << I2S_FIFOLR_RFL_SHIFT)                              /* 0x3F000000 */
/* DMACR */
#define I2S_DMACR_OFFSET                                   (0x10U)
#define I2S_DMACR_TDL_SHIFT                                (0U)
#define I2S_DMACR_TDL_MASK                                 (0x1FU << I2S_DMACR_TDL_SHIFT)                               /* 0x0000001F */
#define I2S_DMACR_TDE_SHIFT                                (8U)
#define I2S_DMACR_TDE_MASK                                 (0x1U << I2S_DMACR_TDE_SHIFT)                                /* 0x00000100 */
#define I2S_DMACR_RDL_SHIFT                                (16U)
#define I2S_DMACR_RDL_MASK                                 (0x1FU << I2S_DMACR_RDL_SHIFT)                               /* 0x001F0000 */
#define I2S_DMACR_RDE_SHIFT                                (24U)
#define I2S_DMACR_RDE_MASK                                 (0x1U << I2S_DMACR_RDE_SHIFT)                                /* 0x01000000 */
/* INTCR */
#define I2S_INTCR_OFFSET                                   (0x14U)
#define I2S_INTCR_TXEIE_SHIFT                              (0U)
#define I2S_INTCR_TXEIE_MASK                               (0x1U << I2S_INTCR_TXEIE_SHIFT)                              /* 0x00000001 */
#define I2S_INTCR_TXUIE_SHIFT                              (1U)
#define I2S_INTCR_TXUIE_MASK                               (0x1U << I2S_INTCR_TXUIE_SHIFT)                              /* 0x00000002 */
#define I2S_INTCR_TXUIC_SHIFT                              (2U)
#define I2S_INTCR_TXUIC_MASK                               (0x1U << I2S_INTCR_TXUIC_SHIFT)                              /* 0x00000004 */
#define I2S_INTCR_TFT_SHIFT                                (4U)
#define I2S_INTCR_TFT_MASK                                 (0x1FU << I2S_INTCR_TFT_SHIFT)                               /* 0x000001F0 */
#define I2S_INTCR_RXFIE_SHIFT                              (16U)
#define I2S_INTCR_RXFIE_MASK                               (0x1U << I2S_INTCR_RXFIE_SHIFT)                              /* 0x00010000 */
#define I2S_INTCR_RXOIE_SHIFT                              (17U)
#define I2S_INTCR_RXOIE_MASK                               (0x1U << I2S_INTCR_RXOIE_SHIFT)                              /* 0x00020000 */
#define I2S_INTCR_RXOIC_SHIFT                              (18U)
#define I2S_INTCR_RXOIC_MASK                               (0x1U << I2S_INTCR_RXOIC_SHIFT)                              /* 0x00040000 */
#define I2S_INTCR_RFT_SHIFT                                (20U)
#define I2S_INTCR_RFT_MASK                                 (0x1FU << I2S_INTCR_RFT_SHIFT)                               /* 0x01F00000 */
/* INTSR */
#define I2S_INTSR_OFFSET                                   (0x18U)
#define I2S_INTSR                                          (0x0U)
#define I2S_INTSR_TXEI_SHIFT                               (0U)
#define I2S_INTSR_TXEI_MASK                                (0x1U << I2S_INTSR_TXEI_SHIFT)                               /* 0x00000001 */
#define I2S_INTSR_TXUI_SHIFT                               (1U)
#define I2S_INTSR_TXUI_MASK                                (0x1U << I2S_INTSR_TXUI_SHIFT)                               /* 0x00000002 */
#define I2S_INTSR_RXFI_SHIFT                               (16U)
#define I2S_INTSR_RXFI_MASK                                (0x1U << I2S_INTSR_RXFI_SHIFT)                               /* 0x00010000 */
#define I2S_INTSR_RXOI_SHIFT                               (17U)
#define I2S_INTSR_RXOI_MASK                                (0x1U << I2S_INTSR_RXOI_SHIFT)                               /* 0x00020000 */
/* XFER */
#define I2S_XFER_OFFSET                                    (0x1CU)
#define I2S_XFER_TXS_SHIFT                                 (0U)
#define I2S_XFER_TXS_MASK                                  (0x1U << I2S_XFER_TXS_SHIFT)                                 /* 0x00000001 */
#define I2S_XFER_RXS_SHIFT                                 (1U)
#define I2S_XFER_RXS_MASK                                  (0x1U << I2S_XFER_RXS_SHIFT)                                 /* 0x00000002 */
/* CLR */
#define I2S_CLR_OFFSET                                     (0x20U)
#define I2S_CLR_TXC_SHIFT                                  (0U)
#define I2S_CLR_TXC_MASK                                   (0x1U << I2S_CLR_TXC_SHIFT)                                  /* 0x00000001 */
#define I2S_CLR_RXC_SHIFT                                  (1U)
#define I2S_CLR_RXC_MASK                                   (0x1U << I2S_CLR_RXC_SHIFT)                                  /* 0x00000002 */
/* TXDR */
#define I2S_TXDR_OFFSET                                    (0x24U)
#define I2S_TXDR_TXDR_SHIFT                                (0U)
#define I2S_TXDR_TXDR_MASK                                 (0xFFFFFFFFU << I2S_TXDR_TXDR_SHIFT)                         /* 0xFFFFFFFF */
/* RXDR */
#define I2S_RXDR_OFFSET                                    (0x28U)
#define I2S_RXDR                                           (0x0U)
#define I2S_RXDR_RXDR_SHIFT                                (0U)
#define I2S_RXDR_RXDR_MASK                                 (0xFFFFFFFFU << I2S_RXDR_RXDR_SHIFT)                         /* 0xFFFFFFFF */
/******************************************I2C*******************************************/
/* CON */
#define I2C_CON_OFFSET                                     (0x0U)
#define I2C_CON_I2C_EN_SHIFT                               (0U)
#define I2C_CON_I2C_EN_MASK                                (0x1U << I2C_CON_I2C_EN_SHIFT)                               /* 0x00000001 */
#define I2C_CON_I2C_MODE_SHIFT                             (1U)
#define I2C_CON_I2C_MODE_MASK                              (0x3U << I2C_CON_I2C_MODE_SHIFT)                             /* 0x00000006 */
#define I2C_CON_START_SHIFT                                (3U)
#define I2C_CON_START_MASK                                 (0x1U << I2C_CON_START_SHIFT)                                /* 0x00000008 */
#define I2C_CON_STOP_SHIFT                                 (4U)
#define I2C_CON_STOP_MASK                                  (0x1U << I2C_CON_STOP_SHIFT)                                 /* 0x00000010 */
#define I2C_CON_ACK_SHIFT                                  (5U)
#define I2C_CON_ACK_MASK                                   (0x1U << I2C_CON_ACK_SHIFT)                                  /* 0x00000020 */
#define I2C_CON_ACT2NAK_SHIFT                              (6U)
#define I2C_CON_ACT2NAK_MASK                               (0x1U << I2C_CON_ACT2NAK_SHIFT)                              /* 0x00000040 */
/* CLKDIV */
#define I2C_CLKDIV_OFFSET                                  (0x4U)
#define I2C_CLKDIV_CLKDIVL_SHIFT                           (0U)
#define I2C_CLKDIV_CLKDIVL_MASK                            (0xFFFFU << I2C_CLKDIV_CLKDIVL_SHIFT)                        /* 0x0000FFFF */
#define I2C_CLKDIV_CLKDIVH_SHIFT                           (16U)
#define I2C_CLKDIV_CLKDIVH_MASK                            (0xFFFFU << I2C_CLKDIV_CLKDIVH_SHIFT)                        /* 0xFFFF0000 */
/* MRXADDR */
#define I2C_MRXADDR_OFFSET                                 (0x8U)
#define I2C_MRXADDR_SADDR_SHIFT                            (0U)
#define I2C_MRXADDR_SADDR_MASK                             (0xFFFFFFU << I2C_MRXADDR_SADDR_SHIFT)                       /* 0x00FFFFFF */
#define I2C_MRXADDR_ADDLVLD_SHIFT                          (24U)
#define I2C_MRXADDR_ADDLVLD_MASK                           (0x1U << I2C_MRXADDR_ADDLVLD_SHIFT)                          /* 0x01000000 */
#define I2C_MRXADDR_ADDMVLD_SHIFT                          (25U)
#define I2C_MRXADDR_ADDMVLD_MASK                           (0x1U << I2C_MRXADDR_ADDMVLD_SHIFT)                          /* 0x02000000 */
#define I2C_MRXADDR_ADDHVLD_SHIFT                          (26U)
#define I2C_MRXADDR_ADDHVLD_MASK                           (0x1U << I2C_MRXADDR_ADDHVLD_SHIFT)                          /* 0x04000000 */
/* MRXRADDR */
#define I2C_MRXRADDR_OFFSET                                (0xCU)
#define I2C_MRXRADDR_SRADDR_SHIFT                          (0U)
#define I2C_MRXRADDR_SRADDR_MASK                           (0xFFFFFFU << I2C_MRXRADDR_SRADDR_SHIFT)                     /* 0x00FFFFFF */
#define I2C_MRXRADDR_SRADDLVLD_SHIFT                       (24U)
#define I2C_MRXRADDR_SRADDLVLD_MASK                        (0x1U << I2C_MRXRADDR_SRADDLVLD_SHIFT)                       /* 0x01000000 */
#define I2C_MRXRADDR_SRADDMVLD_SHIFT                       (25U)
#define I2C_MRXRADDR_SRADDMVLD_MASK                        (0x1U << I2C_MRXRADDR_SRADDMVLD_SHIFT)                       /* 0x02000000 */
#define I2C_MRXRADDR_SRADDHVLD_SHIFT                       (26U)
#define I2C_MRXRADDR_SRADDHVLD_MASK                        (0x1U << I2C_MRXRADDR_SRADDHVLD_SHIFT)                       /* 0x04000000 */
/* MTXCNT */
#define I2C_MTXCNT_OFFSET                                  (0x10U)
#define I2C_MTXCNT_MTXCNT_SHIFT                            (0U)
#define I2C_MTXCNT_MTXCNT_MASK                             (0x3FU << I2C_MTXCNT_MTXCNT_SHIFT)                           /* 0x0000003F */
/* MRXCNT */
#define I2C_MRXCNT_OFFSET                                  (0x14U)
#define I2C_MRXCNT_MRXCNT_SHIFT                            (0U)
#define I2C_MRXCNT_MRXCNT_MASK                             (0x3FU << I2C_MRXCNT_MRXCNT_SHIFT)                           /* 0x0000003F */
/* IEN */
#define I2C_IEN_OFFSET                                     (0x18U)
#define I2C_IEN_BTFIEN_SHIFT                               (0U)
#define I2C_IEN_BTFIEN_MASK                                (0x1U << I2C_IEN_BTFIEN_SHIFT)                               /* 0x00000001 */
#define I2C_IEN_BRFIEN_SHIFT                               (1U)
#define I2C_IEN_BRFIEN_MASK                                (0x1U << I2C_IEN_BRFIEN_SHIFT)                               /* 0x00000002 */
#define I2C_IEN_MBTFIEN_SHIFT                              (2U)
#define I2C_IEN_MBTFIEN_MASK                               (0x1U << I2C_IEN_MBTFIEN_SHIFT)                              /* 0x00000004 */
#define I2C_IEN_MBRFIEN_SHIFT                              (3U)
#define I2C_IEN_MBRFIEN_MASK                               (0x1U << I2C_IEN_MBRFIEN_SHIFT)                              /* 0x00000008 */
#define I2C_IEN_STARTIEN_SHIFT                             (4U)
#define I2C_IEN_STARTIEN_MASK                              (0x1U << I2C_IEN_STARTIEN_SHIFT)                             /* 0x00000010 */
#define I2C_IEN_STOPIEN_SHIFT                              (5U)
#define I2C_IEN_STOPIEN_MASK                               (0x1U << I2C_IEN_STOPIEN_SHIFT)                              /* 0x00000020 */
#define I2C_IEN_NAKRCVIEN_SHIFT                            (6U)
#define I2C_IEN_NAKRCVIEN_MASK                             (0x1U << I2C_IEN_NAKRCVIEN_SHIFT)                            /* 0x00000040 */
/* IPD */
#define I2C_IPD_OFFSET                                     (0x1CU)
#define I2C_IPD_BTFIPD_SHIFT                               (0U)
#define I2C_IPD_BTFIPD_MASK                                (0x1U << I2C_IPD_BTFIPD_SHIFT)                               /* 0x00000001 */
#define I2C_IPD_BRFIPD_SHIFT                               (1U)
#define I2C_IPD_BRFIPD_MASK                                (0x1U << I2C_IPD_BRFIPD_SHIFT)                               /* 0x00000002 */
#define I2C_IPD_MBTFIPD_SHIFT                              (2U)
#define I2C_IPD_MBTFIPD_MASK                               (0x1U << I2C_IPD_MBTFIPD_SHIFT)                              /* 0x00000004 */
#define I2C_IPD_MBRFIPD_SHIFT                              (3U)
#define I2C_IPD_MBRFIPD_MASK                               (0x1U << I2C_IPD_MBRFIPD_SHIFT)                              /* 0x00000008 */
#define I2C_IPD_STARTIPD_SHIFT                             (4U)
#define I2C_IPD_STARTIPD_MASK                              (0x1U << I2C_IPD_STARTIPD_SHIFT)                             /* 0x00000010 */
#define I2C_IPD_STOPIPD_SHIFT                              (5U)
#define I2C_IPD_STOPIPD_MASK                               (0x1U << I2C_IPD_STOPIPD_SHIFT)                              /* 0x00000020 */
#define I2C_IPD_NAKRCVIPD_SHIFT                            (6U)
#define I2C_IPD_NAKRCVIPD_MASK                             (0x1U << I2C_IPD_NAKRCVIPD_SHIFT)                            /* 0x00000040 */
/* FCNT */
#define I2C_FCNT_OFFSET                                    (0x20U)
#define I2C_FCNT_FCNT_SHIFT                                (0U)
#define I2C_FCNT_FCNT_MASK                                 (0x3FU << I2C_FCNT_FCNT_SHIFT)                               /* 0x0000003F */
/* TXDATA0 */
#define I2C_TXDATA0_OFFSET                                 (0x100U)
#define I2C_TXDATA0_TXDATA0_SHIFT                          (0U)
#define I2C_TXDATA0_TXDATA0_MASK                           (0xFFFFFFFFU << I2C_TXDATA0_TXDATA0_SHIFT)                   /* 0xFFFFFFFF */
/* TXDATA1 */
#define I2C_TXDATA1_OFFSET                                 (0x104U)
#define I2C_TXDATA1_TXDATA1_SHIFT                          (0U)
#define I2C_TXDATA1_TXDATA1_MASK                           (0xFFFFFFFFU << I2C_TXDATA1_TXDATA1_SHIFT)                   /* 0xFFFFFFFF */
/* TXDATA2 */
#define I2C_TXDATA2_OFFSET                                 (0x108U)
#define I2C_TXDATA2_TXDATA2_SHIFT                          (0U)
#define I2C_TXDATA2_TXDATA2_MASK                           (0xFFFFFFFFU << I2C_TXDATA2_TXDATA2_SHIFT)                   /* 0xFFFFFFFF */
/* TXDATA3 */
#define I2C_TXDATA3_OFFSET                                 (0x10CU)
#define I2C_TXDATA3_TXDATA3_SHIFT                          (0U)
#define I2C_TXDATA3_TXDATA3_MASK                           (0xFFFFFFFFU << I2C_TXDATA3_TXDATA3_SHIFT)                   /* 0xFFFFFFFF */
/* TXDATA4 */
#define I2C_TXDATA4_OFFSET                                 (0x110U)
#define I2C_TXDATA4_TXDATA4_SHIFT                          (0U)
#define I2C_TXDATA4_TXDATA4_MASK                           (0xFFFFFFFFU << I2C_TXDATA4_TXDATA4_SHIFT)                   /* 0xFFFFFFFF */
/* TXDATA5 */
#define I2C_TXDATA5_OFFSET                                 (0x114U)
#define I2C_TXDATA5_TXDATA5_SHIFT                          (0U)
#define I2C_TXDATA5_TXDATA5_MASK                           (0xFFFFFFFFU << I2C_TXDATA5_TXDATA5_SHIFT)                   /* 0xFFFFFFFF */
/* TXDATA6 */
#define I2C_TXDATA6_OFFSET                                 (0x118U)
#define I2C_TXDATA6_TXDATA6_SHIFT                          (0U)
#define I2C_TXDATA6_TXDATA6_MASK                           (0xFFFFFFFFU << I2C_TXDATA6_TXDATA6_SHIFT)                   /* 0xFFFFFFFF */
/* TXDATA7 */
#define I2C_TXDATA7_OFFSET                                 (0x11CU)
#define I2C_TXDATA7_TXDATA7_SHIFT                          (0U)
#define I2C_TXDATA7_TXDATA7_MASK                           (0xFFFFFFFFU << I2C_TXDATA7_TXDATA7_SHIFT)                   /* 0xFFFFFFFF */
/* RXDATA0 */
#define I2C_RXDATA0_OFFSET                                 (0x200U)
#define I2C_RXDATA0_RXDATA0_SHIFT                          (0U)
#define I2C_RXDATA0_RXDATA0_MASK                           (0xFFFFFFFFU << I2C_RXDATA0_RXDATA0_SHIFT)                   /* 0xFFFFFFFF */
/* RXDATA1 */
#define I2C_RXDATA1_OFFSET                                 (0x204U)
#define I2C_RXDATA1_RXDATA1_SHIFT                          (0U)
#define I2C_RXDATA1_RXDATA1_MASK                           (0xFFFFFFFFU << I2C_RXDATA1_RXDATA1_SHIFT)                   /* 0xFFFFFFFF */
/* RXDATA2 */
#define I2C_RXDATA2_OFFSET                                 (0x208U)
#define I2C_RXDATA2_RXDATA2_SHIFT                          (0U)
#define I2C_RXDATA2_RXDATA2_MASK                           (0xFFFFFFFFU << I2C_RXDATA2_RXDATA2_SHIFT)                   /* 0xFFFFFFFF */
/* RXDATA3 */
#define I2C_RXDATA3_OFFSET                                 (0x20CU)
#define I2C_RXDATA3_RXDATA3_SHIFT                          (0U)
#define I2C_RXDATA3_RXDATA3_MASK                           (0xFFFFFFFFU << I2C_RXDATA3_RXDATA3_SHIFT)                   /* 0xFFFFFFFF */
/* RXDATA4 */
#define I2C_RXDATA4_OFFSET                                 (0x210U)
#define I2C_RXDATA4_RXDATA4_SHIFT                          (0U)
#define I2C_RXDATA4_RXDATA4_MASK                           (0xFFFFFFFFU << I2C_RXDATA4_RXDATA4_SHIFT)                   /* 0xFFFFFFFF */
/* RXDATA5 */
#define I2C_RXDATA5_OFFSET                                 (0x214U)
#define I2C_RXDATA5_RXDATA5_SHIFT                          (0U)
#define I2C_RXDATA5_RXDATA5_MASK                           (0xFFFFFFFFU << I2C_RXDATA5_RXDATA5_SHIFT)                   /* 0xFFFFFFFF */
/* RXDATA6 */
#define I2C_RXDATA6_OFFSET                                 (0x218U)
#define I2C_RXDATA6_RXDATA6_SHIFT                          (0U)
#define I2C_RXDATA6_RXDATA6_MASK                           (0xFFFFFFFFU << I2C_RXDATA6_RXDATA6_SHIFT)                   /* 0xFFFFFFFF */
/* RXDATA7 */
#define I2C_RXDATA7_OFFSET                                 (0x21CU)
#define I2C_RXDATA7_RXDATA7_SHIFT                          (0U)
#define I2C_RXDATA7_RXDATA7_MASK                           (0xFFFFFFFFU << I2C_RXDATA7_RXDATA7_SHIFT)                   /* 0xFFFFFFFF */
/******************************************SPI*******************************************/
/* CTRLR0 */
#define SPI_CTRLR0_OFFSET                                  (0x0U)
#define SPI_CTRLR0_DFS_SHIFT                               (0U)
#define SPI_CTRLR0_DFS_MASK                                (0x3U << SPI_CTRLR0_DFS_SHIFT)                               /* 0x00000003 */
#define SPI_CTRLR0_CFS_SHIFT                               (2U)
#define SPI_CTRLR0_CFS_MASK                                (0xFU << SPI_CTRLR0_CFS_SHIFT)                               /* 0x0000003C */
#define SPI_CTRLR0_SCPH_SHIFT                              (6U)
#define SPI_CTRLR0_SCPH_MASK                               (0x1U << SPI_CTRLR0_SCPH_SHIFT)                              /* 0x00000040 */
#define SPI_CTRLR0_SCPOL_SHIFT                             (7U)
#define SPI_CTRLR0_SCPOL_MASK                              (0x1U << SPI_CTRLR0_SCPOL_SHIFT)                             /* 0x00000080 */
#define SPI_CTRLR0_CSM_SHIFT                               (8U)
#define SPI_CTRLR0_CSM_MASK                                (0x3U << SPI_CTRLR0_CSM_SHIFT)                               /* 0x00000300 */
#define SPI_CTRLR0_SSD_SHIFT                               (10U)
#define SPI_CTRLR0_SSD_MASK                                (0x1U << SPI_CTRLR0_SSD_SHIFT)                               /* 0x00000400 */
#define SPI_CTRLR0_EM_SHIFT                                (11U)
#define SPI_CTRLR0_EM_MASK                                 (0x1U << SPI_CTRLR0_EM_SHIFT)                                /* 0x00000800 */
#define SPI_CTRLR0_FBM_SHIFT                               (12U)
#define SPI_CTRLR0_FBM_MASK                                (0x1U << SPI_CTRLR0_FBM_SHIFT)                               /* 0x00001000 */
#define SPI_CTRLR0_BHT_SHIFT                               (13U)
#define SPI_CTRLR0_BHT_MASK                                (0x1U << SPI_CTRLR0_BHT_SHIFT)                               /* 0x00002000 */
#define SPI_CTRLR0_RSD_SHIFT                               (14U)
#define SPI_CTRLR0_RSD_MASK                                (0x3U << SPI_CTRLR0_RSD_SHIFT)                               /* 0x0000C000 */
#define SPI_CTRLR0_FRF_SHIFT                               (16U)
#define SPI_CTRLR0_FRF_MASK                                (0x3U << SPI_CTRLR0_FRF_SHIFT)                               /* 0x00030000 */
#define SPI_CTRLR0_XFM_SHIFT                               (18U)
#define SPI_CTRLR0_XFM_MASK                                (0x3U << SPI_CTRLR0_XFM_SHIFT)                               /* 0x000C0000 */
#define SPI_CTRLR0_OPM_SHIFT                               (20U)
#define SPI_CTRLR0_OPM_MASK                                (0x1U << SPI_CTRLR0_OPM_SHIFT)                               /* 0x00100000 */
#define SPI_CTRLR0_MTM_SHIFT                               (21U)
#define SPI_CTRLR0_MTM_MASK                                (0x1U << SPI_CTRLR0_MTM_SHIFT)                               /* 0x00200000 */
/* CTRLR1 */
#define SPI_CTRLR1_OFFSET                                  (0x4U)
#define SPI_CTRLR1_NDM_SHIFT                               (0U)
#define SPI_CTRLR1_NDM_MASK                                (0xFFFFU << SPI_CTRLR1_NDM_SHIFT)                            /* 0x0000FFFF */
/* ENR */
#define SPI_ENR_OFFSET                                     (0x8U)
#define SPI_ENR_ENR_SHIFT                                  (0U)
#define SPI_ENR_ENR_MASK                                   (0x1U << SPI_ENR_ENR_SHIFT)                                  /* 0x00000001 */
/* SER */
#define SPI_SER_OFFSET                                     (0xCU)
#define SPI_SER_SER_SHIFT                                  (0U)
#define SPI_SER_SER_MASK                                   (0x3U << SPI_SER_SER_SHIFT)                                  /* 0x00000003 */
/* BAUDR */
#define SPI_BAUDR_OFFSET                                   (0x10U)
#define SPI_BAUDR_BAUDR_SHIFT                              (0U)
#define SPI_BAUDR_BAUDR_MASK                               (0xFFFFU << SPI_BAUDR_BAUDR_SHIFT)                           /* 0x0000FFFF */
/* TXFTLR */
#define SPI_TXFTLR_OFFSET                                  (0x14U)
#define SPI_TXFTLR_TXFTLR_SHIFT                            (0U)
#define SPI_TXFTLR_TXFTLR_MASK                             (0x1FU << SPI_TXFTLR_TXFTLR_SHIFT)                           /* 0x0000001F */
/* RXFTLR */
#define SPI_RXFTLR_OFFSET                                  (0x18U)
#define SPI_RXFTLR_RXFTLR_SHIFT                            (0U)
#define SPI_RXFTLR_RXFTLR_MASK                             (0x1FU << SPI_RXFTLR_RXFTLR_SHIFT)                           /* 0x0000001F */
/* TXFLR */
#define SPI_TXFLR_OFFSET                                   (0x1CU)
#define SPI_TXFLR                                          (0x0U)
#define SPI_TXFLR_TXFLR_SHIFT                              (0U)
#define SPI_TXFLR_TXFLR_MASK                               (0x3FU << SPI_TXFLR_TXFLR_SHIFT)                             /* 0x0000003F */
/* RXFLR */
#define SPI_RXFLR_OFFSET                                   (0x20U)
#define SPI_RXFLR                                          (0x0U)
#define SPI_RXFLR_RXFLR_SHIFT                              (0U)
#define SPI_RXFLR_RXFLR_MASK                               (0x3FU << SPI_RXFLR_RXFLR_SHIFT)                             /* 0x0000003F */
/* SR */
#define SPI_SR_OFFSET                                      (0x24U)
#define SPI_SR                                             (0xCU)
#define SPI_SR_BSF_SHIFT                                   (0U)
#define SPI_SR_BSF_MASK                                    (0x1U << SPI_SR_BSF_SHIFT)                                   /* 0x00000001 */
#define SPI_SR_TFF_SHIFT                                   (1U)
#define SPI_SR_TFF_MASK                                    (0x1U << SPI_SR_TFF_SHIFT)                                   /* 0x00000002 */
#define SPI_SR_TFE_SHIFT                                   (2U)
#define SPI_SR_TFE_MASK                                    (0x1U << SPI_SR_TFE_SHIFT)                                   /* 0x00000004 */
#define SPI_SR_RFE_SHIFT                                   (3U)
#define SPI_SR_RFE_MASK                                    (0x1U << SPI_SR_RFE_SHIFT)                                   /* 0x00000008 */
#define SPI_SR_RFF_SHIFT                                   (4U)
#define SPI_SR_RFF_MASK                                    (0x1U << SPI_SR_RFF_SHIFT)                                   /* 0x00000010 */
/* IPR */
#define SPI_IPR_OFFSET                                     (0x28U)
#define SPI_IPR_IPR_SHIFT                                  (0U)
#define SPI_IPR_IPR_MASK                                   (0x1U << SPI_IPR_IPR_SHIFT)                                  /* 0x00000001 */
/* IMR */
#define SPI_IMR_OFFSET                                     (0x2CU)
#define SPI_IMR_TFEIM_SHIFT                                (0U)
#define SPI_IMR_TFEIM_MASK                                 (0x1U << SPI_IMR_TFEIM_SHIFT)                                /* 0x00000001 */
#define SPI_IMR_TFOIM_SHIFT                                (1U)
#define SPI_IMR_TFOIM_MASK                                 (0x1U << SPI_IMR_TFOIM_SHIFT)                                /* 0x00000002 */
#define SPI_IMR_RFUIM_SHIFT                                (2U)
#define SPI_IMR_RFUIM_MASK                                 (0x1U << SPI_IMR_RFUIM_SHIFT)                                /* 0x00000004 */
#define SPI_IMR_RFOIM_SHIFT                                (3U)
#define SPI_IMR_RFOIM_MASK                                 (0x1U << SPI_IMR_RFOIM_SHIFT)                                /* 0x00000008 */
#define SPI_IMR_RFFIM_SHIFT                                (4U)
#define SPI_IMR_RFFIM_MASK                                 (0x1U << SPI_IMR_RFFIM_SHIFT)                                /* 0x00000010 */
/* ISR */
#define SPI_ISR_OFFSET                                     (0x30U)
#define SPI_ISR                                            (0x0U)
#define SPI_ISR_TFEIS_SHIFT                                (0U)
#define SPI_ISR_TFEIS_MASK                                 (0x1U << SPI_ISR_TFEIS_SHIFT)                                /* 0x00000001 */
#define SPI_ISR_TFOIS_SHIFT                                (1U)
#define SPI_ISR_TFOIS_MASK                                 (0x1U << SPI_ISR_TFOIS_SHIFT)                                /* 0x00000002 */
#define SPI_ISR_RFUIS_SHIFT                                (2U)
#define SPI_ISR_RFUIS_MASK                                 (0x1U << SPI_ISR_RFUIS_SHIFT)                                /* 0x00000004 */
#define SPI_ISR_RFOIS_SHIFT                                (3U)
#define SPI_ISR_RFOIS_MASK                                 (0x1U << SPI_ISR_RFOIS_SHIFT)                                /* 0x00000008 */
#define SPI_ISR_RFFIS_SHIFT                                (4U)
#define SPI_ISR_RFFIS_MASK                                 (0x1U << SPI_ISR_RFFIS_SHIFT)                                /* 0x00000010 */
/* RISR */
#define SPI_RISR_OFFSET                                    (0x34U)
#define SPI_RISR                                           (0x1U)
#define SPI_RISR_TFERIS_SHIFT                              (0U)
#define SPI_RISR_TFERIS_MASK                               (0x1U << SPI_RISR_TFERIS_SHIFT)                              /* 0x00000001 */
#define SPI_RISR_TFORIS_SHIFT                              (1U)
#define SPI_RISR_TFORIS_MASK                               (0x1U << SPI_RISR_TFORIS_SHIFT)                              /* 0x00000002 */
#define SPI_RISR_RFURIS_SHIFT                              (2U)
#define SPI_RISR_RFURIS_MASK                               (0x1U << SPI_RISR_RFURIS_SHIFT)                              /* 0x00000004 */
#define SPI_RISR_RFORIS_SHIFT                              (3U)
#define SPI_RISR_RFORIS_MASK                               (0x1U << SPI_RISR_RFORIS_SHIFT)                              /* 0x00000008 */
#define SPI_RISR_RFFRIS_SHIFT                              (4U)
#define SPI_RISR_RFFRIS_MASK                               (0x1U << SPI_RISR_RFFRIS_SHIFT)                              /* 0x00000010 */
/* ICR */
#define SPI_ICR_OFFSET                                     (0x38U)
#define SPI_ICR_CCI_SHIFT                                  (0U)
#define SPI_ICR_CCI_MASK                                   (0x1U << SPI_ICR_CCI_SHIFT)                                  /* 0x00000001 */
#define SPI_ICR_CRFUI_SHIFT                                (1U)
#define SPI_ICR_CRFUI_MASK                                 (0x1U << SPI_ICR_CRFUI_SHIFT)                                /* 0x00000002 */
#define SPI_ICR_CRFOI_SHIFT                                (2U)
#define SPI_ICR_CRFOI_MASK                                 (0x1U << SPI_ICR_CRFOI_SHIFT)                                /* 0x00000004 */
#define SPI_ICR_CTFOI_SHIFT                                (3U)
#define SPI_ICR_CTFOI_MASK                                 (0x1U << SPI_ICR_CTFOI_SHIFT)                                /* 0x00000008 */
/* DMACR */
#define SPI_DMACR_OFFSET                                   (0x3CU)
#define SPI_DMACR_RDE_SHIFT                                (0U)
#define SPI_DMACR_RDE_MASK                                 (0x1U << SPI_DMACR_RDE_SHIFT)                                /* 0x00000001 */
#define SPI_DMACR_TDE_SHIFT                                (1U)
#define SPI_DMACR_TDE_MASK                                 (0x1U << SPI_DMACR_TDE_SHIFT)                                /* 0x00000002 */
/* DMATDLR */
#define SPI_DMATDLR_OFFSET                                 (0x40U)
#define SPI_DMATDLR_TDL_SHIFT                              (0U)
#define SPI_DMATDLR_TDL_MASK                               (0x1FU << SPI_DMATDLR_TDL_SHIFT)                             /* 0x0000001F */
/* DMARDLR */
#define SPI_DMARDLR_OFFSET                                 (0x44U)
#define SPI_DMARDLR_RDL_SHIFT                              (0U)
#define SPI_DMARDLR_RDL_MASK                               (0x1FU << SPI_DMARDLR_RDL_SHIFT)                             /* 0x0000001F */
/* TXDR */
#define SPI_TXDR_OFFSET                                    (0x48U)
#define SPI_TXDR_TXDR_SHIFT                                (0U)
#define SPI_TXDR_TXDR_MASK                                 (0xFFFFU << SPI_TXDR_TXDR_SHIFT)                             /* 0x0000FFFF */
/* RXDR */
#define SPI_RXDR_OFFSET                                    (0x4CU)
#define SPI_RXDR_RXDR_SHIFT                                (0U)
#define SPI_RXDR_RXDR_MASK                                 (0xFFFFU << SPI_RXDR_RXDR_SHIFT)                             /* 0x0000FFFF */
/******************************************UART******************************************/
/* RBR */
#define UART_RBR_OFFSET                                    (0x0U)
#define UART_RBR_DATA_INPUT_SHIFT                          (0U)
#define UART_RBR_DATA_INPUT_MASK                           (0xFFU << UART_RBR_DATA_INPUT_SHIFT)                         /* 0x000000FF */
/* THR */
#define UART_THR_OFFSET                                    (0x0U)
#define UART_THR_DATA_OUTPUT_SHIFT                         (0U)
#define UART_THR_DATA_OUTPUT_MASK                          (0xFFU << UART_THR_DATA_OUTPUT_SHIFT)                        /* 0x000000FF */
/* DLL */
#define UART_DLL_OFFSET                                    (0x0U)
#define UART_DLL_BAUD_RATE_DIVISOR_L_SHIFT                 (0U)
#define UART_DLL_BAUD_RATE_DIVISOR_L_MASK                  (0xFFU << UART_DLL_BAUD_RATE_DIVISOR_L_SHIFT)                /* 0x000000FF */
/* DLH */
#define UART_DLH_OFFSET                                    (0x4U)
#define UART_DLH_BAUD_RATE_DIVISOR_H_SHIFT                 (0U)
#define UART_DLH_BAUD_RATE_DIVISOR_H_MASK                  (0xFFU << UART_DLH_BAUD_RATE_DIVISOR_H_SHIFT)                /* 0x000000FF */
/* IER */
#define UART_IER_OFFSET                                    (0x4U)
#define UART_IER_RECEIVE_DATA_AVAILABLE_INT_EN_SHIFT       (0U)
#define UART_IER_RECEIVE_DATA_AVAILABLE_INT_EN_MASK        (0x1U << UART_IER_RECEIVE_DATA_AVAILABLE_INT_EN_SHIFT)       /* 0x00000001 */
#define UART_IER_TRANS_HOLD_EMPTY_INT_EN_SHIFT             (1U)
#define UART_IER_TRANS_HOLD_EMPTY_INT_EN_MASK              (0x1U << UART_IER_TRANS_HOLD_EMPTY_INT_EN_SHIFT)             /* 0x00000002 */
#define UART_IER_RECEIVE_LINE_STATUS_INT_EN_SHIFT          (2U)
#define UART_IER_RECEIVE_LINE_STATUS_INT_EN_MASK           (0x1U << UART_IER_RECEIVE_LINE_STATUS_INT_EN_SHIFT)          /* 0x00000004 */
#define UART_IER_MODEM_STATUS_INT_EN_SHIFT                 (3U)
#define UART_IER_MODEM_STATUS_INT_EN_MASK                  (0x1U << UART_IER_MODEM_STATUS_INT_EN_SHIFT)                 /* 0x00000008 */
#define UART_IER_PROG_THRE_INT_EN_SHIFT                    (7U)
#define UART_IER_PROG_THRE_INT_EN_MASK                     (0x1U << UART_IER_PROG_THRE_INT_EN_SHIFT)                    /* 0x00000080 */
/* IIR */
#define UART_IIR_OFFSET                                    (0x8U)
#define UART_IIR                                           (0x0U)
#define UART_IIR_INT_ID_SHIFT                              (0U)
#define UART_IIR_INT_ID_MASK                               (0xFU << UART_IIR_INT_ID_SHIFT)                              /* 0x0000000F */
#define UART_IIR_FIFOS_EN_SHIFT                            (6U)
#define UART_IIR_FIFOS_EN_MASK                             (0x3U << UART_IIR_FIFOS_EN_SHIFT)                            /* 0x000000C0 */
/* FCR */
#define UART_FCR_OFFSET                                    (0x8U)
#define UART_FCR_FIFO_EN_SHIFT                             (0U)
#define UART_FCR_FIFO_EN_MASK                              (0x1U << UART_FCR_FIFO_EN_SHIFT)                             /* 0x00000001 */
#define UART_FCR_RCVR_FIFO_RESET_SHIFT                     (1U)
#define UART_FCR_RCVR_FIFO_RESET_MASK                      (0x1U << UART_FCR_RCVR_FIFO_RESET_SHIFT)                     /* 0x00000002 */
#define UART_FCR_XMIT_FIFO_RESET_SHIFT                     (2U)
#define UART_FCR_XMIT_FIFO_RESET_MASK                      (0x1U << UART_FCR_XMIT_FIFO_RESET_SHIFT)                     /* 0x00000004 */
#define UART_FCR_DMA_MODE_SHIFT                            (3U)
#define UART_FCR_DMA_MODE_MASK                             (0x1U << UART_FCR_DMA_MODE_SHIFT)                            /* 0x00000008 */
#define UART_FCR_TX_EMPTY_TRIGGER_SHIFT                    (4U)
#define UART_FCR_TX_EMPTY_TRIGGER_MASK                     (0x3U << UART_FCR_TX_EMPTY_TRIGGER_SHIFT)                    /* 0x00000030 */
#define UART_FCR_RCVR_TRIGGER_SHIFT                        (6U)
#define UART_FCR_RCVR_TRIGGER_MASK                         (0x3U << UART_FCR_RCVR_TRIGGER_SHIFT)                        /* 0x000000C0 */
/* LCR */
#define UART_LCR_OFFSET                                    (0xCU)
#define UART_LCR_DATA_LENGTH_SEL_SHIFT                     (0U)
#define UART_LCR_DATA_LENGTH_SEL_MASK                      (0x3U << UART_LCR_DATA_LENGTH_SEL_SHIFT)                     /* 0x00000003 */
#define UART_LCR_STOP_BITS_NUM_SHIFT                       (2U)
#define UART_LCR_STOP_BITS_NUM_MASK                        (0x1U << UART_LCR_STOP_BITS_NUM_SHIFT)                       /* 0x00000004 */
#define UART_LCR_PARITY_EN_SHIFT                           (3U)
#define UART_LCR_PARITY_EN_MASK                            (0x1U << UART_LCR_PARITY_EN_SHIFT)                           /* 0x00000008 */
#define UART_LCR_EVEN_PARITY_SEL_SHIFT                     (4U)
#define UART_LCR_EVEN_PARITY_SEL_MASK                      (0x1U << UART_LCR_EVEN_PARITY_SEL_SHIFT)                     /* 0x00000010 */
#define UART_LCR_BREAK_CTRL_SHIFT                          (6U)
#define UART_LCR_BREAK_CTRL_MASK                           (0x1U << UART_LCR_BREAK_CTRL_SHIFT)                          /* 0x00000040 */
#define UART_LCR_DIV_LAT_ACCESS_SHIFT                      (7U)
#define UART_LCR_DIV_LAT_ACCESS_MASK                       (0x1U << UART_LCR_DIV_LAT_ACCESS_SHIFT)                      /* 0x00000080 */
/* MCR */
#define UART_MCR_OFFSET                                    (0x10U)
#define UART_MCR_DATA_TERMINAL_READY_SHIFT                 (0U)
#define UART_MCR_DATA_TERMINAL_READY_MASK                  (0x1U << UART_MCR_DATA_TERMINAL_READY_SHIFT)                 /* 0x00000001 */
#define UART_MCR_REQ_TO_SEND_SHIFT                         (1U)
#define UART_MCR_REQ_TO_SEND_MASK                          (0x1U << UART_MCR_REQ_TO_SEND_SHIFT)                         /* 0x00000002 */
#define UART_MCR_OUT1_SHIFT                                (2U)
#define UART_MCR_OUT1_MASK                                 (0x1U << UART_MCR_OUT1_SHIFT)                                /* 0x00000004 */
#define UART_MCR_OUT2_SHIFT                                (3U)
#define UART_MCR_OUT2_MASK                                 (0x1U << UART_MCR_OUT2_SHIFT)                                /* 0x00000008 */
#define UART_MCR_LOOPBACK_SHIFT                            (4U)
#define UART_MCR_LOOPBACK_MASK                             (0x1U << UART_MCR_LOOPBACK_SHIFT)                            /* 0x00000010 */
#define UART_MCR_AUTO_FLOW_CTRL_EN_SHIFT                   (5U)
#define UART_MCR_AUTO_FLOW_CTRL_EN_MASK                    (0x1U << UART_MCR_AUTO_FLOW_CTRL_EN_SHIFT)                   /* 0x00000020 */
#define UART_MCR_SIR_MODE_EN_SHIFT                         (6U)
#define UART_MCR_SIR_MODE_EN_MASK                          (0x1U << UART_MCR_SIR_MODE_EN_SHIFT)                         /* 0x00000040 */
/* LSR */
#define UART_LSR_OFFSET                                    (0x14U)
#define UART_LSR                                           (0x0U)
#define UART_LSR_DATA_READY_SHIFT                          (0U)
#define UART_LSR_DATA_READY_MASK                           (0x1U << UART_LSR_DATA_READY_SHIFT)                          /* 0x00000001 */
#define UART_LSR_OVERRUN_ERROR_SHIFT                       (1U)
#define UART_LSR_OVERRUN_ERROR_MASK                        (0x1U << UART_LSR_OVERRUN_ERROR_SHIFT)                       /* 0x00000002 */
#define UART_LSR_PARITY_EROR_SHIFT                         (2U)
#define UART_LSR_PARITY_EROR_MASK                          (0x1U << UART_LSR_PARITY_EROR_SHIFT)                         /* 0x00000004 */
#define UART_LSR_FRAMING_ERROR_SHIFT                       (3U)
#define UART_LSR_FRAMING_ERROR_MASK                        (0x1U << UART_LSR_FRAMING_ERROR_SHIFT)                       /* 0x00000008 */
#define UART_LSR_BREAK_INT_SHIFT                           (4U)
#define UART_LSR_BREAK_INT_MASK                            (0x1U << UART_LSR_BREAK_INT_SHIFT)                           /* 0x00000010 */
#define UART_LSR_TRANS_HOLD_REG_EMPTY_SHIFT                (5U)
#define UART_LSR_TRANS_HOLD_REG_EMPTY_MASK                 (0x1U << UART_LSR_TRANS_HOLD_REG_EMPTY_SHIFT)                /* 0x00000020 */
#define UART_LSR_TRANS_EMPTY_SHIFT                         (6U)
#define UART_LSR_TRANS_EMPTY_MASK                          (0x1U << UART_LSR_TRANS_EMPTY_SHIFT)                         /* 0x00000040 */
#define UART_LSR_RECEIVER_FIFO_ERROR_SHIFT                 (7U)
#define UART_LSR_RECEIVER_FIFO_ERROR_MASK                  (0x1U << UART_LSR_RECEIVER_FIFO_ERROR_SHIFT)                 /* 0x00000080 */
/* MSR */
#define UART_MSR_OFFSET                                    (0x18U)
#define UART_MSR                                           (0x0U)
#define UART_MSR_DELTA_CLEAR_TO_SEND_SHIFT                 (0U)
#define UART_MSR_DELTA_CLEAR_TO_SEND_MASK                  (0x1U << UART_MSR_DELTA_CLEAR_TO_SEND_SHIFT)                 /* 0x00000001 */
#define UART_MSR_DELTA_DATA_SET_READY_SHIFT                (1U)
#define UART_MSR_DELTA_DATA_SET_READY_MASK                 (0x1U << UART_MSR_DELTA_DATA_SET_READY_SHIFT)                /* 0x00000002 */
#define UART_MSR_TRAILING_EDGE_RING_INDICATOR_SHIFT        (2U)
#define UART_MSR_TRAILING_EDGE_RING_INDICATOR_MASK         (0x1U << UART_MSR_TRAILING_EDGE_RING_INDICATOR_SHIFT)        /* 0x00000004 */
#define UART_MSR_DELTA_DATA_CARRIER_DETECT_SHIFT           (3U)
#define UART_MSR_DELTA_DATA_CARRIER_DETECT_MASK            (0x1U << UART_MSR_DELTA_DATA_CARRIER_DETECT_SHIFT)           /* 0x00000008 */
#define UART_MSR_CLEAR_TO_SEND_SHIFT                       (4U)
#define UART_MSR_CLEAR_TO_SEND_MASK                        (0x1U << UART_MSR_CLEAR_TO_SEND_SHIFT)                       /* 0x00000010 */
#define UART_MSR_DATA_SET_READY_SHIFT                      (5U)
#define UART_MSR_DATA_SET_READY_MASK                       (0x1U << UART_MSR_DATA_SET_READY_SHIFT)                      /* 0x00000020 */
#define UART_MSR_RING_INDICATOR_SHIFT                      (6U)
#define UART_MSR_RING_INDICATOR_MASK                       (0x1U << UART_MSR_RING_INDICATOR_SHIFT)                      /* 0x00000040 */
#define UART_MSR_DATA_CARRIOR_DETECT_SHIFT                 (7U)
#define UART_MSR_DATA_CARRIOR_DETECT_MASK                  (0x1U << UART_MSR_DATA_CARRIOR_DETECT_SHIFT)                 /* 0x00000080 */
/* SCR */
#define UART_SCR_OFFSET                                    (0x1CU)
#define UART_SCR_TEMP_STORE_SPACE_SHIFT                    (0U)
#define UART_SCR_TEMP_STORE_SPACE_MASK                     (0xFFU << UART_SCR_TEMP_STORE_SPACE_SHIFT)                   /* 0x000000FF */
/* SRBR */
#define UART_SRBR_OFFSET                                   (0x30U)
#define UART_SRBR                                          (0x0U)
#define UART_SRBR_SHADOW_RBR_SHIFT                         (0U)
#define UART_SRBR_SHADOW_RBR_MASK                          (0xFFU << UART_SRBR_SHADOW_RBR_SHIFT)                        /* 0x000000FF */
/* STHR */
#define UART_STHR_OFFSET                                   (0x6CU)
#define UART_STHR                                          (0x0U)
#define UART_STHR_SHADOW_THR_SHIFT                         (0U)
#define UART_STHR_SHADOW_THR_MASK                          (0xFFU << UART_STHR_SHADOW_THR_SHIFT)                        /* 0x000000FF */
/* FAR */
#define UART_FAR_OFFSET                                    (0x70U)
#define UART_FAR_FIFO_ACCESS_TEST_EN_SHIFT                 (0U)
#define UART_FAR_FIFO_ACCESS_TEST_EN_MASK                  (0x1U << UART_FAR_FIFO_ACCESS_TEST_EN_SHIFT)                 /* 0x00000001 */
/* TFR */
#define UART_TFR_OFFSET                                    (0x74U)
#define UART_TFR                                           (0x0U)
#define UART_TFR_TRANS_FIFO_READ_SHIFT                     (0U)
#define UART_TFR_TRANS_FIFO_READ_MASK                      (0xFFU << UART_TFR_TRANS_FIFO_READ_SHIFT)                    /* 0x000000FF */
/* RFW */
#define UART_RFW_OFFSET                                    (0x78U)
#define UART_RFW_RECEIVE_FIFO_WRITE_SHIFT                  (0U)
#define UART_RFW_RECEIVE_FIFO_WRITE_MASK                   (0xFFU << UART_RFW_RECEIVE_FIFO_WRITE_SHIFT)                 /* 0x000000FF */
#define UART_RFW_RECEIVE_FIFO_PARITY_ERROR_SHIFT           (8U)
#define UART_RFW_RECEIVE_FIFO_PARITY_ERROR_MASK            (0x1U << UART_RFW_RECEIVE_FIFO_PARITY_ERROR_SHIFT)           /* 0x00000100 */
#define UART_RFW_RECEIVE_FIFO_FRAMING_ERROR_SHIFT          (9U)
#define UART_RFW_RECEIVE_FIFO_FRAMING_ERROR_MASK           (0x1U << UART_RFW_RECEIVE_FIFO_FRAMING_ERROR_SHIFT)          /* 0x00000200 */
/* USR */
#define UART_USR_OFFSET                                    (0x7CU)
#define UART_USR                                           (0x0U)
#define UART_USR_UART_BUSY_SHIFT                           (0U)
#define UART_USR_UART_BUSY_MASK                            (0x1U << UART_USR_UART_BUSY_SHIFT)                           /* 0x00000001 */
#define UART_USR_TRANS_FIFO_NOT_FULL_SHIFT                 (1U)
#define UART_USR_TRANS_FIFO_NOT_FULL_MASK                  (0x1U << UART_USR_TRANS_FIFO_NOT_FULL_SHIFT)                 /* 0x00000002 */
#define UART_USR_TRASN_FIFO_EMPTY_SHIFT                    (2U)
#define UART_USR_TRASN_FIFO_EMPTY_MASK                     (0x1U << UART_USR_TRASN_FIFO_EMPTY_SHIFT)                    /* 0x00000004 */
#define UART_USR_RECEIVE_FIFO_NOT_EMPTY_SHIFT              (3U)
#define UART_USR_RECEIVE_FIFO_NOT_EMPTY_MASK               (0x1U << UART_USR_RECEIVE_FIFO_NOT_EMPTY_SHIFT)              /* 0x00000008 */
#define UART_USR_RECEIVE_FIFO_FULL_SHIFT                   (4U)
#define UART_USR_RECEIVE_FIFO_FULL_MASK                    (0x1U << UART_USR_RECEIVE_FIFO_FULL_SHIFT)                   /* 0x00000010 */
/* TFL */
#define UART_TFL_OFFSET                                    (0x80U)
#define UART_TFL_TRANS_FIFO_LEVEL_SHIFT                    (0U)
#define UART_TFL_TRANS_FIFO_LEVEL_MASK                     (0x1FU << UART_TFL_TRANS_FIFO_LEVEL_SHIFT)                   /* 0x0000001F */
/* RFL */
#define UART_RFL_OFFSET                                    (0x84U)
#define UART_RFL                                           (0x0U)
#define UART_RFL_RECEIVE_FIFO_LEVEL_SHIFT                  (0U)
#define UART_RFL_RECEIVE_FIFO_LEVEL_MASK                   (0x1FU << UART_RFL_RECEIVE_FIFO_LEVEL_SHIFT)                 /* 0x0000001F */
/* SRR */
#define UART_SRR_OFFSET                                    (0x88U)
#define UART_SRR_UART_RESET_SHIFT                          (0U)
#define UART_SRR_UART_RESET_MASK                           (0x1U << UART_SRR_UART_RESET_SHIFT)                          /* 0x00000001 */
#define UART_SRR_RCVR_FIFO_RESET_SHIFT                     (1U)
#define UART_SRR_RCVR_FIFO_RESET_MASK                      (0x1U << UART_SRR_RCVR_FIFO_RESET_SHIFT)                     /* 0x00000002 */
#define UART_SRR_XMIT_FIFO_RESET_SHIFT                     (2U)
#define UART_SRR_XMIT_FIFO_RESET_MASK                      (0x1U << UART_SRR_XMIT_FIFO_RESET_SHIFT)                     /* 0x00000004 */
/* SRTS */
#define UART_SRTS_OFFSET                                   (0x8CU)
#define UART_SRTS_SHADOW_REQ_TO_SEND_SHIFT                 (0U)
#define UART_SRTS_SHADOW_REQ_TO_SEND_MASK                  (0x1U << UART_SRTS_SHADOW_REQ_TO_SEND_SHIFT)                 /* 0x00000001 */
/* SBCR */
#define UART_SBCR_OFFSET                                   (0x90U)
#define UART_SBCR_SHADOW_BREAK_CTRL_SHIFT                  (0U)
#define UART_SBCR_SHADOW_BREAK_CTRL_MASK                   (0x1U << UART_SBCR_SHADOW_BREAK_CTRL_SHIFT)                  /* 0x00000001 */
/* SDMAM */
#define UART_SDMAM_OFFSET                                  (0x94U)
#define UART_SDMAM_SHADOW_DMA_MODE_SHIFT                   (0U)
#define UART_SDMAM_SHADOW_DMA_MODE_MASK                    (0x1U << UART_SDMAM_SHADOW_DMA_MODE_SHIFT)                   /* 0x00000001 */
/* SFE */
#define UART_SFE_OFFSET                                    (0x98U)
#define UART_SFE_SHADOW_FIFO_EN_SHIFT                      (0U)
#define UART_SFE_SHADOW_FIFO_EN_MASK                       (0x1U << UART_SFE_SHADOW_FIFO_EN_SHIFT)                      /* 0x00000001 */
/* SRT */
#define UART_SRT_OFFSET                                    (0x9CU)
#define UART_SRT_SHADOW_RCVR_TRIGGER_SHIFT                 (0U)
#define UART_SRT_SHADOW_RCVR_TRIGGER_MASK                  (0x1U << UART_SRT_SHADOW_RCVR_TRIGGER_SHIFT)                 /* 0x00000001 */
/* STET */
#define UART_STET_OFFSET                                   (0xA0U)
#define UART_STET_SHADOW_TX_EMPTY_TRIGGER_SHIFT            (0U)
#define UART_STET_SHADOW_TX_EMPTY_TRIGGER_MASK             (0x1U << UART_STET_SHADOW_TX_EMPTY_TRIGGER_SHIFT)            /* 0x00000001 */
/* HTX */
#define UART_HTX_OFFSET                                    (0xA4U)
#define UART_HTX_HALT_TX_EN_SHIFT                          (0U)
#define UART_HTX_HALT_TX_EN_MASK                           (0x1U << UART_HTX_HALT_TX_EN_SHIFT)                          /* 0x00000001 */
/* DMASA */
#define UART_DMASA_OFFSET                                  (0xA8U)
#define UART_DMASA_DMA_SOFTWARE_ACK_SHIFT                  (0U)
#define UART_DMASA_DMA_SOFTWARE_ACK_MASK                   (0x1U << UART_DMASA_DMA_SOFTWARE_ACK_SHIFT)                  /* 0x00000001 */
/* CPR */
#define UART_CPR_OFFSET                                    (0xF4U)
#define UART_CPR                                           (0x0U)
#define UART_CPR_APB_DATA_WIDTH_SHIFT                      (0U)
#define UART_CPR_APB_DATA_WIDTH_MASK                       (0x3U << UART_CPR_APB_DATA_WIDTH_SHIFT)                      /* 0x00000003 */
#define UART_CPR_AFCE_MODE_SHIFT                           (4U)
#define UART_CPR_AFCE_MODE_MASK                            (0x1U << UART_CPR_AFCE_MODE_SHIFT)                           /* 0x00000010 */
#define UART_CPR_THRE_MODE_SHIFT                           (5U)
#define UART_CPR_THRE_MODE_MASK                            (0x1U << UART_CPR_THRE_MODE_SHIFT)                           /* 0x00000020 */
#define UART_CPR_SIR_MODE_SHIFT                            (6U)
#define UART_CPR_SIR_MODE_MASK                             (0x1U << UART_CPR_SIR_MODE_SHIFT)                            /* 0x00000040 */
#define UART_CPR_SIR_LP_MODE_SHIFT                         (7U)
#define UART_CPR_SIR_LP_MODE_MASK                          (0x1U << UART_CPR_SIR_LP_MODE_SHIFT)                         /* 0x00000080 */
#define UART_CPR_NEW_FEAT_SHIFT                            (8U)
#define UART_CPR_NEW_FEAT_MASK                             (0x1U << UART_CPR_NEW_FEAT_SHIFT)                            /* 0x00000100 */
#define UART_CPR_FIFO_ACCESS_SHIFT                         (9U)
#define UART_CPR_FIFO_ACCESS_MASK                          (0x1U << UART_CPR_FIFO_ACCESS_SHIFT)                         /* 0x00000200 */
#define UART_CPR_FIFO_STAT_SHIFT                           (10U)
#define UART_CPR_FIFO_STAT_MASK                            (0x1U << UART_CPR_FIFO_STAT_SHIFT)                           /* 0x00000400 */
#define UART_CPR_SHADOW_SHIFT                              (11U)
#define UART_CPR_SHADOW_MASK                               (0x1U << UART_CPR_SHADOW_SHIFT)                              /* 0x00000800 */
#define UART_CPR_UART_ADD_ENCODED_PARAMS_SHIFT             (12U)
#define UART_CPR_UART_ADD_ENCODED_PARAMS_MASK              (0x1U << UART_CPR_UART_ADD_ENCODED_PARAMS_SHIFT)             /* 0x00001000 */
#define UART_CPR_DMA_EXTRA_SHIFT                           (13U)
#define UART_CPR_DMA_EXTRA_MASK                            (0x1U << UART_CPR_DMA_EXTRA_SHIFT)                           /* 0x00002000 */
#define UART_CPR_FIFO_MODE_SHIFT                           (16U)
#define UART_CPR_FIFO_MODE_MASK                            (0xFFU << UART_CPR_FIFO_MODE_SHIFT)                          /* 0x00FF0000 */
/* UCV */
#define UART_UCV_OFFSET                                    (0xF8U)
#define UART_UCV                                           (0x330372AU)
#define UART_UCV_VER_SHIFT                                 (0U)
#define UART_UCV_VER_MASK                                  (0xFFFFFFFFU << UART_UCV_VER_SHIFT)                          /* 0xFFFFFFFF */
/* CTR */
#define UART_CTR_OFFSET                                    (0xFCU)
#define UART_CTR                                           (0x44570110U)
#define UART_CTR_PERIPHERAL_ID_SHIFT                       (0U)
#define UART_CTR_PERIPHERAL_ID_MASK                        (0xFFFFFFFFU << UART_CTR_PERIPHERAL_ID_SHIFT)                /* 0xFFFFFFFF */
/*****************************************SARADC*****************************************/
/* DATA */
#define SARADC_DATA_OFFSET                                 (0x0U)
#define SARADC_DATA                                        (0x0U)
#define SARADC_DATA_ADC_DATA_SHIFT                         (0U)
#define SARADC_DATA_ADC_DATA_MASK                          (0x3FFU << SARADC_DATA_ADC_DATA_SHIFT)                       /* 0x000003FF */
/* STAS */
#define SARADC_STAS_OFFSET                                 (0x4U)
#define SARADC_STAS                                        (0x0U)
#define SARADC_STAS_ADC_STATUS_SHIFT                       (0U)
#define SARADC_STAS_ADC_STATUS_MASK                        (0x1U << SARADC_STAS_ADC_STATUS_SHIFT)                       /* 0x00000001 */
/* CTRL */
#define SARADC_CTRL_OFFSET                                 (0x8U)
#define SARADC_CTRL_ADC_INPUT_SRC_SEL_SHIFT                (0U)
#define SARADC_CTRL_ADC_INPUT_SRC_SEL_MASK                 (0x7U << SARADC_CTRL_ADC_INPUT_SRC_SEL_SHIFT)                /* 0x00000007 */
#define SARADC_CTRL_ADC_POWER_CTRL_SHIFT                   (3U)
#define SARADC_CTRL_ADC_POWER_CTRL_MASK                    (0x1U << SARADC_CTRL_ADC_POWER_CTRL_SHIFT)                   /* 0x00000008 */
#define SARADC_CTRL_START_CONVERT_SHIFT                    (4U)
#define SARADC_CTRL_START_CONVERT_MASK                     (0x1U << SARADC_CTRL_START_CONVERT_SHIFT)                    /* 0x00000010 */
#define SARADC_CTRL_INT_EN_SHIFT                           (5U)
#define SARADC_CTRL_INT_EN_MASK                            (0x1U << SARADC_CTRL_INT_EN_SHIFT)                           /* 0x00000020 */
#define SARADC_CTRL_INT_STATUS_SHIFT                       (6U)
#define SARADC_CTRL_INT_STATUS_MASK                        (0x1U << SARADC_CTRL_INT_STATUS_SHIFT)                       /* 0x00000040 */
/*****************************************TIMER******************************************/
/* LOAD_COUNT0 */
#define TIMER_LOAD_COUNT0_OFFSET                           (0x0U)
#define TIMER_LOAD_COUNT0_COUNT0_SHIFT                     (0U)
#define TIMER_LOAD_COUNT0_COUNT0_MASK                      (0xFFFFFFFFU << TIMER_LOAD_COUNT0_COUNT0_SHIFT)              /* 0xFFFFFFFF */
/* LOAD_COUNT1 */
#define TIMER_LOAD_COUNT1_OFFSET                           (0x4U)
#define TIMER_LOAD_COUNT1_COUNT1_SHIFT                     (0U)
#define TIMER_LOAD_COUNT1_COUNT1_MASK                      (0xFFFFFFFFU << TIMER_LOAD_COUNT1_COUNT1_SHIFT)              /* 0xFFFFFFFF */
/* CURRENT_VALUE0 */
#define TIMER_CURRENT_VALUE0_OFFSET                        (0x8U)
#define TIMER_CURRENT_VALUE0                               (0x0U)
#define TIMER_CURRENT_VALUE0_CURRENT_VALUE0_SHIFT          (0U)
#define TIMER_CURRENT_VALUE0_CURRENT_VALUE0_MASK           (0xFFFFFFFFU << TIMER_CURRENT_VALUE0_CURRENT_VALUE0_SHIFT)   /* 0xFFFFFFFF */
/* CURRENT_VALUE1 */
#define TIMER_CURRENT_VALUE1_OFFSET                        (0xCU)
#define TIMER_CURRENT_VALUE1                               (0x0U)
#define TIMER_CURRENT_VALUE1_CURRENT_VALUE1_SHIFT          (0U)
#define TIMER_CURRENT_VALUE1_CURRENT_VALUE1_MASK           (0xFFFFFFFFU << TIMER_CURRENT_VALUE1_CURRENT_VALUE1_SHIFT)   /* 0xFFFFFFFF */
/* CONTROLREG */
#define TIMER_CONTROLREG_OFFSET                            (0x10U)
#define TIMER_CONTROLREG_TIMER_ENABLE_SHIFT                (0U)
#define TIMER_CONTROLREG_TIMER_ENABLE_MASK                 (0x1U << TIMER_CONTROLREG_TIMER_ENABLE_SHIFT)                /* 0x00000001 */
#define TIMER_CONTROLREG_TIMER_MODE_SHIFT                  (1U)
#define TIMER_CONTROLREG_TIMER_MODE_MASK                   (0x1U << TIMER_CONTROLREG_TIMER_MODE_SHIFT)                  /* 0x00000002 */
#define TIMER_CONTROLREG_TIMER_INT_MASK_SHIFT              (2U)
#define TIMER_CONTROLREG_TIMER_INT_MASK_MASK               (0x1U << TIMER_CONTROLREG_TIMER_INT_MASK_SHIFT)              /* 0x00000004 */
/* INTSTATUS */
#define TIMER_INTSTATUS_OFFSET                             (0x18U)
#define TIMER_INTSTATUS_INT_PD_SHIFT                       (0U)
#define TIMER_INTSTATUS_INT_PD_MASK                        (0x1U << TIMER_INTSTATUS_INT_PD_SHIFT)                       /* 0x00000001 */
/******************************************PWM*******************************************/
/* CNTR */
#define PWM_CNTR_OFFSET                                    (0x0U)
#define PWM_CNTR_MAIN_PWM_TIMER_CNT_SHIFT                  (0U)
#define PWM_CNTR_MAIN_PWM_TIMER_CNT_MASK                   (0xFFFFFFFFU << PWM_CNTR_MAIN_PWM_TIMER_CNT_SHIFT)           /* 0xFFFFFFFF */
/* HRC */
#define PWM_HRC_OFFSET                                     (0x4U)
#define PWM_HRC_HIGH_REFERENCE_CAP_SHIFT                   (0U)
#define PWM_HRC_HIGH_REFERENCE_CAP_MASK                    (0xFFFFFFFFU << PWM_HRC_HIGH_REFERENCE_CAP_SHIFT)            /* 0xFFFFFFFF */
/* LRC */
#define PWM_LRC_OFFSET                                     (0x8U)
#define PWM_LRC_LOE_REFERENCE_CAP_SHIFT                    (0U)
#define PWM_LRC_LOE_REFERENCE_CAP_MASK                     (0xFFFFFFFFU << PWM_LRC_LOE_REFERENCE_CAP_SHIFT)             /* 0xFFFFFFFF */
/* CTRL */
#define PWM_CTRL_OFFSET                                    (0xCU)
#define PWM_CTRL_PWM_TIMER_EN_SHIFT                        (0U)
#define PWM_CTRL_PWM_TIMER_EN_MASK                         (0x1U << PWM_CTRL_PWM_TIMER_EN_SHIFT)                        /* 0x00000001 */
#define PWM_CTRL_PWM_OUTPUT_EN_SHIFT                       (3U)
#define PWM_CTRL_PWM_OUTPUT_EN_MASK                        (0x1U << PWM_CTRL_PWM_OUTPUT_EN_SHIFT)                       /* 0x00000008 */
#define PWM_CTRL_SINGLE_CNT_MODE_SHIFT                     (4U)
#define PWM_CTRL_SINGLE_CNT_MODE_MASK                      (0x1U << PWM_CTRL_SINGLE_CNT_MODE_SHIFT)                     /* 0x00000010 */
#define PWM_CTRL_PWM_TIMER_INT_EN_SHIFT                    (5U)
#define PWM_CTRL_PWM_TIMER_INT_EN_MASK                     (0x1U << PWM_CTRL_PWM_TIMER_INT_EN_SHIFT)                    /* 0x00000020 */
#define PWM_CTRL_INT_STATUS_CLR_SHIFT                      (6U)
#define PWM_CTRL_INT_STATUS_CLR_MASK                       (0x1U << PWM_CTRL_INT_STATUS_CLR_SHIFT)                      /* 0x00000040 */
#define PWM_CTRL_PWM_RESET_SHIFT                           (7U)
#define PWM_CTRL_PWM_RESET_MASK                            (0x1U << PWM_CTRL_PWM_RESET_SHIFT)                           /* 0x00000080 */
#define PWM_CTRL_CAP_MODE_EN_SHIFT                         (8U)
#define PWM_CTRL_CAP_MODE_EN_MASK                          (0x1U << PWM_CTRL_CAP_MODE_EN_SHIFT)                         /* 0x00000100 */
#define PWM_CTRL_PRESCALE_FACTOR_SHIFT                     (9U)
#define PWM_CTRL_PRESCALE_FACTOR_MASK                      (0xFU << PWM_CTRL_PRESCALE_FACTOR_SHIFT)                     /* 0x00001E00 */
/******************************************WDT*******************************************/
/* CR */
#define WDT_CR_OFFSET                                      (0x0U)
#define WDT_CR_WDT_EN_SHIFT                                (0U)
#define WDT_CR_WDT_EN_MASK                                 (0x1U << WDT_CR_WDT_EN_SHIFT)                                /* 0x00000001 */
#define WDT_CR_RESP_MODE_SHIFT                             (1U)
#define WDT_CR_RESP_MODE_MASK                              (0x1U << WDT_CR_RESP_MODE_SHIFT)                             /* 0x00000002 */
#define WDT_CR_RST_PLUSE_LENTH_SHIFT                       (2U)
#define WDT_CR_RST_PLUSE_LENTH_MASK                        (0x7U << WDT_CR_RST_PLUSE_LENTH_SHIFT)                       /* 0x0000001C */
/* TORR */
#define WDT_TORR_OFFSET                                    (0x4U)
#define WDT_TORR_TIMEOUT_PERIOD_SHIFT                      (0U)
#define WDT_TORR_TIMEOUT_PERIOD_MASK                       (0xFU << WDT_TORR_TIMEOUT_PERIOD_SHIFT)                      /* 0x0000000F */
/* CCVR */
#define WDT_CCVR_OFFSET                                    (0x8U)
#define WDT_CCVR                                           (0x0U)
#define WDT_CCVR_CUR_CNT_SHIFT                             (0U)
#define WDT_CCVR_CUR_CNT_MASK                              (0xFFFFFFFFU << WDT_CCVR_CUR_CNT_SHIFT)                      /* 0xFFFFFFFF */
/* CRR */
#define WDT_CRR_OFFSET                                     (0xCU)
#define WDT_CRR_CNT_RESTART_SHIFT                          (0U)
#define WDT_CRR_CNT_RESTART_MASK                           (0xFFU << WDT_CRR_CNT_RESTART_SHIFT)                         /* 0x000000FF */
/* STAT */
#define WDT_STAT_OFFSET                                    (0x10U)
#define WDT_STAT                                           (0x0U)
#define WDT_STAT_WDT_STATUS_SHIFT                          (0U)
#define WDT_STAT_WDT_STATUS_MASK                           (0x1U << WDT_STAT_WDT_STATUS_SHIFT)                          /* 0x00000001 */
/* EOI */
#define WDT_EOI_OFFSET                                     (0x14U)
#define WDT_EOI                                            (0x0U)
#define WDT_EOI_WDT_INT_CLR_SHIFT                          (0U)
#define WDT_EOI_WDT_INT_CLR_MASK                           (0x1U << WDT_EOI_WDT_INT_CLR_SHIFT)                          /* 0x00000001 */
/******************************************GPIO******************************************/
/* SWPORTA_DR */
#define GPIO_SWPORTA_DR_OFFSET                             (0x0U)
#define GPIO_SWPORTA_DR_GPIO_SWPORTA_DR_SHIFT              (0U)
#define GPIO_SWPORTA_DR_GPIO_SWPORTA_DR_MASK               (0xFFFFFFFFU << GPIO_SWPORTA_DR_GPIO_SWPORTA_DR_SHIFT)       /* 0xFFFFFFFF */
/* SWPORTA_DDR */
#define GPIO_SWPORTA_DDR_OFFSET                            (0x4U)
#define GPIO_SWPORTA_DDR_GPIO_SWPORTA_DDR_SHIFT            (0U)
#define GPIO_SWPORTA_DDR_GPIO_SWPORTA_DDR_MASK             (0xFFFFFFFFU << GPIO_SWPORTA_DDR_GPIO_SWPORTA_DDR_SHIFT)     /* 0xFFFFFFFF */
/* INTEN */
#define GPIO_INTEN_OFFSET                                  (0x30U)
#define GPIO_INTEN_GPIO_INT_EN_SHIFT                       (0U)
#define GPIO_INTEN_GPIO_INT_EN_MASK                        (0xFFFFFFFFU << GPIO_INTEN_GPIO_INT_EN_SHIFT)                /* 0xFFFFFFFF */
/* INTMASK */
#define GPIO_INTMASK_OFFSET                                (0x34U)
#define GPIO_INTMASK_GPIO_INT_MASK_SHIFT                   (0U)
#define GPIO_INTMASK_GPIO_INT_MASK_MASK                    (0xFFFFFFFFU << GPIO_INTMASK_GPIO_INT_MASK_SHIFT)            /* 0xFFFFFFFF */
/* INTTYPE_LEVEL */
#define GPIO_INTTYPE_LEVEL_OFFSET                          (0x38U)
#define GPIO_INTTYPE_LEVEL_GPIO_INTTYPE_LEVEL_SHIFT        (0U)
#define GPIO_INTTYPE_LEVEL_GPIO_INTTYPE_LEVEL_MASK         (0xFFFFFFFFU << GPIO_INTTYPE_LEVEL_GPIO_INTTYPE_LEVEL_SHIFT) /* 0xFFFFFFFF */
/* INT_POLARITY */
#define GPIO_INT_POLARITY_OFFSET                           (0x3CU)
#define GPIO_INT_POLARITY_GPIO_INT_POLARITY_SHIFT          (0U)
#define GPIO_INT_POLARITY_GPIO_INT_POLARITY_MASK           (0xFFFFFFFFU << GPIO_INT_POLARITY_GPIO_INT_POLARITY_SHIFT)   /* 0xFFFFFFFF */
/* INT_STATUS */
#define GPIO_INT_STATUS_OFFSET                             (0x40U)
#define GPIO_INT_STATUS                                    (0x0U)
#define GPIO_INT_STATUS_GPIO_INT_STATUS_SHIFT              (0U)
#define GPIO_INT_STATUS_GPIO_INT_STATUS_MASK               (0xFFFFFFFFU << GPIO_INT_STATUS_GPIO_INT_STATUS_SHIFT)       /* 0xFFFFFFFF */
/* INT_RAWSTATUS */
#define GPIO_INT_RAWSTATUS_OFFSET                          (0x44U)
#define GPIO_INT_RAWSTATUS                                 (0x0U)
#define GPIO_INT_RAWSTATUS_GPIO_INT_RAWSTATUS_SHIFT        (0U)
#define GPIO_INT_RAWSTATUS_GPIO_INT_RAWSTATUS_MASK         (0xFFFFFFFFU << GPIO_INT_RAWSTATUS_GPIO_INT_RAWSTATUS_SHIFT) /* 0xFFFFFFFF */
/* DEBOUNCE */
#define GPIO_DEBOUNCE_OFFSET                               (0x48U)
#define GPIO_DEBOUNCE_GPIO_DEBOUNCE_SHIFT                  (0U)
#define GPIO_DEBOUNCE_GPIO_DEBOUNCE_MASK                   (0xFFFFFFFFU << GPIO_DEBOUNCE_GPIO_DEBOUNCE_SHIFT)           /* 0xFFFFFFFF */
/* PORTA_EOI */
#define GPIO_PORTA_EOI_OFFSET                              (0x4CU)
#define GPIO_PORTA_EOI_GPIO_PORTA_EOI_SHIFT                (0U)
#define GPIO_PORTA_EOI_GPIO_PORTA_EOI_MASK                 (0xFFFFFFFFU << GPIO_PORTA_EOI_GPIO_PORTA_EOI_SHIFT)         /* 0xFFFFFFFF */
/* EXT_PORTA */
#define GPIO_EXT_PORTA_OFFSET                              (0x50U)
#define GPIO_EXT_PORTA                                     (0x0U)
#define GPIO_EXT_PORTA_GPIO_EXT_PORTA_SHIFT                (0U)
#define GPIO_EXT_PORTA_GPIO_EXT_PORTA_MASK                 (0xFFFFFFFFU << GPIO_EXT_PORTA_GPIO_EXT_PORTA_SHIFT)         /* 0xFFFFFFFF */
/* LS_SYNC */
#define GPIO_LS_SYNC_OFFSET                                (0x60U)
#define GPIO_LS_SYNC_GPIO_LS_SYNC_SHIFT                    (0U)
#define GPIO_LS_SYNC_GPIO_LS_SYNC_MASK                     (0x1U << GPIO_LS_SYNC_GPIO_LS_SYNC_SHIFT)                    /* 0x00000001 */
/******************************************GRF*******************************************/
/* GPIO0A_IOMUX */
#define GRF_GPIO0A_IOMUX_OFFSET                            (0x0U)
#define GRF_GPIO0A_IOMUX_GPIO0A0_SEL_SHIFT                 (0U)
#define GRF_GPIO0A_IOMUX_GPIO0A0_SEL_MASK                  (0x3U << GRF_GPIO0A_IOMUX_GPIO0A0_SEL_SHIFT)                 /* 0x00000003 */
#define GRF_GPIO0A_IOMUX_GPIO0A1_SEL_SHIFT                 (2U)
#define GRF_GPIO0A_IOMUX_GPIO0A1_SEL_MASK                  (0x3U << GRF_GPIO0A_IOMUX_GPIO0A1_SEL_SHIFT)                 /* 0x0000000C */
#define GRF_GPIO0A_IOMUX_GPIO0A2_SEL_SHIFT                 (4U)
#define GRF_GPIO0A_IOMUX_GPIO0A2_SEL_MASK                  (0x3U << GRF_GPIO0A_IOMUX_GPIO0A2_SEL_SHIFT)                 /* 0x00000030 */
#define GRF_GPIO0A_IOMUX_GPIO0A3_SEL_SHIFT                 (6U)
#define GRF_GPIO0A_IOMUX_GPIO0A3_SEL_MASK                  (0x3U << GRF_GPIO0A_IOMUX_GPIO0A3_SEL_SHIFT)                 /* 0x000000C0 */
#define GRF_GPIO0A_IOMUX_GPIO0A4_SEL_SHIFT                 (8U)
#define GRF_GPIO0A_IOMUX_GPIO0A4_SEL_MASK                  (0x3U << GRF_GPIO0A_IOMUX_GPIO0A4_SEL_SHIFT)                 /* 0x00000300 */
#define GRF_GPIO0A_IOMUX_GPIO0A5_SEL_SHIFT                 (10U)
#define GRF_GPIO0A_IOMUX_GPIO0A5_SEL_MASK                  (0x3U << GRF_GPIO0A_IOMUX_GPIO0A5_SEL_SHIFT)                 /* 0x00000C00 */
#define GRF_GPIO0A_IOMUX_GPIO0A6_SEL_SHIFT                 (12U)
#define GRF_GPIO0A_IOMUX_GPIO0A6_SEL_MASK                  (0x3U << GRF_GPIO0A_IOMUX_GPIO0A6_SEL_SHIFT)                 /* 0x00003000 */
#define GRF_GPIO0A_IOMUX_GPIO0A7_SEL_SHIFT                 (14U)
#define GRF_GPIO0A_IOMUX_GPIO0A7_SEL_MASK                  (0x3U << GRF_GPIO0A_IOMUX_GPIO0A7_SEL_SHIFT)                 /* 0x0000C000 */
/* GPIO0B_IOMUX */
#define GRF_GPIO0B_IOMUX_OFFSET                            (0x4U)
#define GRF_GPIO0B_IOMUX_GPIO0B0_SEL_SHIFT                 (0U)
#define GRF_GPIO0B_IOMUX_GPIO0B0_SEL_MASK                  (0x3U << GRF_GPIO0B_IOMUX_GPIO0B0_SEL_SHIFT)                 /* 0x00000003 */
#define GRF_GPIO0B_IOMUX_GPIO0B1_SEL_SHIFT                 (2U)
#define GRF_GPIO0B_IOMUX_GPIO0B1_SEL_MASK                  (0x3U << GRF_GPIO0B_IOMUX_GPIO0B1_SEL_SHIFT)                 /* 0x0000000C */
#define GRF_GPIO0B_IOMUX_GPIO0B2_SEL_SHIFT                 (4U)
#define GRF_GPIO0B_IOMUX_GPIO0B2_SEL_MASK                  (0x3U << GRF_GPIO0B_IOMUX_GPIO0B2_SEL_SHIFT)                 /* 0x00000030 */
#define GRF_GPIO0B_IOMUX_GPIO0B3_SEL_SHIFT                 (6U)
#define GRF_GPIO0B_IOMUX_GPIO0B3_SEL_MASK                  (0x3U << GRF_GPIO0B_IOMUX_GPIO0B3_SEL_SHIFT)                 /* 0x000000C0 */
/* GPIO0C_IOMUX */
#define GRF_GPIO0C_IOMUX_OFFSET                            (0x8U)
#define GRF_GPIO0C_IOMUX_GPIO0C0_SEL_SHIFT                 (0U)
#define GRF_GPIO0C_IOMUX_GPIO0C0_SEL_MASK                  (0x3U << GRF_GPIO0C_IOMUX_GPIO0C0_SEL_SHIFT)                 /* 0x00000003 */
#define GRF_GPIO0C_IOMUX_GPIO0C1_SEL_SHIFT                 (2U)
#define GRF_GPIO0C_IOMUX_GPIO0C1_SEL_MASK                  (0x3U << GRF_GPIO0C_IOMUX_GPIO0C1_SEL_SHIFT)                 /* 0x0000000C */
#define GRF_GPIO0C_IOMUX_GPIO0C2_SEL_SHIFT                 (4U)
#define GRF_GPIO0C_IOMUX_GPIO0C2_SEL_MASK                  (0x3U << GRF_GPIO0C_IOMUX_GPIO0C2_SEL_SHIFT)                 /* 0x00000030 */
#define GRF_GPIO0C_IOMUX_GPIO0C3_SEL_SHIFT                 (6U)
#define GRF_GPIO0C_IOMUX_GPIO0C3_SEL_MASK                  (0x3U << GRF_GPIO0C_IOMUX_GPIO0C3_SEL_SHIFT)                 /* 0x000000C0 */
#define GRF_GPIO0C_IOMUX_GPIO0C4_SEL_SHIFT                 (8U)
#define GRF_GPIO0C_IOMUX_GPIO0C4_SEL_MASK                  (0x3U << GRF_GPIO0C_IOMUX_GPIO0C4_SEL_SHIFT)                 /* 0x00000300 */
#define GRF_GPIO0C_IOMUX_GPIO0C5_SEL_SHIFT                 (10U)
#define GRF_GPIO0C_IOMUX_GPIO0C5_SEL_MASK                  (0x3U << GRF_GPIO0C_IOMUX_GPIO0C5_SEL_SHIFT)                 /* 0x00000C00 */
#define GRF_GPIO0C_IOMUX_GPIO0C6_SEL_SHIFT                 (12U)
#define GRF_GPIO0C_IOMUX_GPIO0C6_SEL_MASK                  (0x3U << GRF_GPIO0C_IOMUX_GPIO0C6_SEL_SHIFT)                 /* 0x00003000 */
#define GRF_GPIO0C_IOMUX_GPIO0C7_SEL_SHIFT                 (14U)
#define GRF_GPIO0C_IOMUX_GPIO0C7_SEL_MASK                  (0x3U << GRF_GPIO0C_IOMUX_GPIO0C7_SEL_SHIFT)                 /* 0x0000C000 */
/* GPIO0D_IOMUX */
#define GRF_GPIO0D_IOMUX_OFFSET                            (0xCU)
#define GRF_GPIO0D_IOMUX_GPIO0D0_SEL_SHIFT                 (0U)
#define GRF_GPIO0D_IOMUX_GPIO0D0_SEL_MASK                  (0x3U << GRF_GPIO0D_IOMUX_GPIO0D0_SEL_SHIFT)                 /* 0x00000003 */
#define GRF_GPIO0D_IOMUX_GPIO0D1_SEL_SHIFT                 (2U)
#define GRF_GPIO0D_IOMUX_GPIO0D1_SEL_MASK                  (0x3U << GRF_GPIO0D_IOMUX_GPIO0D1_SEL_SHIFT)                 /* 0x0000000C */
#define GRF_GPIO0D_IOMUX_GPIO0D2_SEL_SHIFT                 (4U)
#define GRF_GPIO0D_IOMUX_GPIO0D2_SEL_MASK                  (0x3U << GRF_GPIO0D_IOMUX_GPIO0D2_SEL_SHIFT)                 /* 0x00000030 */
#define GRF_GPIO0D_IOMUX_GPIO0D3_SEL_SHIFT                 (6U)
#define GRF_GPIO0D_IOMUX_GPIO0D3_SEL_MASK                  (0x3U << GRF_GPIO0D_IOMUX_GPIO0D3_SEL_SHIFT)                 /* 0x000000C0 */
#define GRF_GPIO0D_IOMUX_GPIO0D4_SEL_SHIFT                 (8U)
#define GRF_GPIO0D_IOMUX_GPIO0D4_SEL_MASK                  (0x3U << GRF_GPIO0D_IOMUX_GPIO0D4_SEL_SHIFT)                 /* 0x00000300 */
#define GRF_GPIO0D_IOMUX_GPIO0D5_SEL_SHIFT                 (10U)
#define GRF_GPIO0D_IOMUX_GPIO0D5_SEL_MASK                  (0x3U << GRF_GPIO0D_IOMUX_GPIO0D5_SEL_SHIFT)                 /* 0x00000C00 */
#define GRF_GPIO0D_IOMUX_GPIO0D6_SEL_SHIFT                 (12U)
#define GRF_GPIO0D_IOMUX_GPIO0D6_SEL_MASK                  (0x3U << GRF_GPIO0D_IOMUX_GPIO0D6_SEL_SHIFT)                 /* 0x00003000 */
#define GRF_GPIO0D_IOMUX_GPIO0D7_SEL_SHIFT                 (14U)
#define GRF_GPIO0D_IOMUX_GPIO0D7_SEL_MASK                  (0x3U << GRF_GPIO0D_IOMUX_GPIO0D7_SEL_SHIFT)                 /* 0x0000C000 */
/* GPIO0A_P */
#define GRF_GPIO0A_P_OFFSET                                (0x10U)
#define GRF_GPIO0A_P_PULL_SHIFT                            (0U)
#define GRF_GPIO0A_P_PULL_MASK                             (0xFFU << GRF_GPIO0A_P_PULL_SHIFT)                           /* 0x000000FF */
/* GPIO0B_P */
#define GRF_GPIO0B_P_OFFSET                                (0x14U)
#define GRF_GPIO0B_P_PULL_SHIFT                            (0U)
#define GRF_GPIO0B_P_PULL_MASK                             (0xFFU << GRF_GPIO0B_P_PULL_SHIFT)                           /* 0x000000FF */
/* GPIO0C_P */
#define GRF_GPIO0C_P_OFFSET                                (0x18U)
#define GRF_GPIO0C_P_PULL_SHIFT                            (0U)
#define GRF_GPIO0C_P_PULL_MASK                             (0xFFU << GRF_GPIO0C_P_PULL_SHIFT)                           /* 0x000000FF */
/* GPIO0D_P */
#define GRF_GPIO0D_P_OFFSET                                (0x1CU)
#define GRF_GPIO0D_P_PULL_SHIFT                            (0U)
#define GRF_GPIO0D_P_PULL_MASK                             (0xFFU << GRF_GPIO0D_P_PULL_SHIFT)                           /* 0x000000FF */
/* GPIO1A_IOMUX */
#define GRF_GPIO1A_IOMUX_OFFSET                            (0x20U)
#define GRF_GPIO1A_IOMUX_GPIO1A0_SEL_SHIFT                 (0U)
#define GRF_GPIO1A_IOMUX_GPIO1A0_SEL_MASK                  (0x3U << GRF_GPIO1A_IOMUX_GPIO1A0_SEL_SHIFT)                 /* 0x00000003 */
#define GRF_GPIO1A_IOMUX_GPIO1A1_SEL_SHIFT                 (2U)
#define GRF_GPIO1A_IOMUX_GPIO1A1_SEL_MASK                  (0x3U << GRF_GPIO1A_IOMUX_GPIO1A1_SEL_SHIFT)                 /* 0x0000000C */
#define GRF_GPIO1A_IOMUX_GPIO1A2_SEL_SHIFT                 (4U)
#define GRF_GPIO1A_IOMUX_GPIO1A2_SEL_MASK                  (0x3U << GRF_GPIO1A_IOMUX_GPIO1A2_SEL_SHIFT)                 /* 0x00000030 */
#define GRF_GPIO1A_IOMUX_GPIO1A3_SEL_SHIFT                 (6U)
#define GRF_GPIO1A_IOMUX_GPIO1A3_SEL_MASK                  (0x3U << GRF_GPIO1A_IOMUX_GPIO1A3_SEL_SHIFT)                 /* 0x000000C0 */
#define GRF_GPIO1A_IOMUX_GPIO1A4_SEL_SHIFT                 (8U)
#define GRF_GPIO1A_IOMUX_GPIO1A4_SEL_MASK                  (0x3U << GRF_GPIO1A_IOMUX_GPIO1A4_SEL_SHIFT)                 /* 0x00000300 */
#define GRF_GPIO1A_IOMUX_GPIO1A5_SEL_SHIFT                 (10U)
#define GRF_GPIO1A_IOMUX_GPIO1A5_SEL_MASK                  (0x3U << GRF_GPIO1A_IOMUX_GPIO1A5_SEL_SHIFT)                 /* 0x00000C00 */
/* GPIO1B_IOMUX */
#define GRF_GPIO1B_IOMUX_OFFSET                            (0x24U)
#define GRF_GPIO1B_IOMUX_GPIO1B0_SEL_SHIFT                 (0U)
#define GRF_GPIO1B_IOMUX_GPIO1B0_SEL_MASK                  (0x3U << GRF_GPIO1B_IOMUX_GPIO1B0_SEL_SHIFT)                 /* 0x00000003 */
#define GRF_GPIO1B_IOMUX_GPIO1B1_SEL_SHIFT                 (2U)
#define GRF_GPIO1B_IOMUX_GPIO1B1_SEL_MASK                  (0x3U << GRF_GPIO1B_IOMUX_GPIO1B1_SEL_SHIFT)                 /* 0x0000000C */
#define GRF_GPIO1B_IOMUX_GPIO1B2_SEL_SHIFT                 (4U)
#define GRF_GPIO1B_IOMUX_GPIO1B2_SEL_MASK                  (0x3U << GRF_GPIO1B_IOMUX_GPIO1B2_SEL_SHIFT)                 /* 0x00000030 */
#define GRF_GPIO1B_IOMUX_GPIO1B3_SEL_SHIFT                 (6U)
#define GRF_GPIO1B_IOMUX_GPIO1B3_SEL_MASK                  (0x3U << GRF_GPIO1B_IOMUX_GPIO1B3_SEL_SHIFT)                 /* 0x000000C0 */
#define GRF_GPIO1B_IOMUX_GPIO1B4_SEL_SHIFT                 (8U)
#define GRF_GPIO1B_IOMUX_GPIO1B4_SEL_MASK                  (0x3U << GRF_GPIO1B_IOMUX_GPIO1B4_SEL_SHIFT)                 /* 0x00000300 */
#define GRF_GPIO1B_IOMUX_GPIO1B5_SEL_SHIFT                 (10U)
#define GRF_GPIO1B_IOMUX_GPIO1B5_SEL_MASK                  (0x3U << GRF_GPIO1B_IOMUX_GPIO1B5_SEL_SHIFT)                 /* 0x00000C00 */
#define GRF_GPIO1B_IOMUX_GPIO1B6_SEL_SHIFT                 (12U)
#define GRF_GPIO1B_IOMUX_GPIO1B6_SEL_MASK                  (0x3U << GRF_GPIO1B_IOMUX_GPIO1B6_SEL_SHIFT)                 /* 0x00003000 */
#define GRF_GPIO1B_IOMUX_GPIO1B7_SEL_SHIFT                 (14U)
#define GRF_GPIO1B_IOMUX_GPIO1B7_SEL_MASK                  (0x3U << GRF_GPIO1B_IOMUX_GPIO1B7_SEL_SHIFT)                 /* 0x0000C000 */
/* GPIO1C_IOMUX */
#define GRF_GPIO1C_IOMUX_OFFSET                            (0x28U)
#define GRF_GPIO1C_IOMUX_GPIO1C0_SEL_SHIFT                 (0U)
#define GRF_GPIO1C_IOMUX_GPIO1C0_SEL_MASK                  (0x3U << GRF_GPIO1C_IOMUX_GPIO1C0_SEL_SHIFT)                 /* 0x00000003 */
#define GRF_GPIO1C_IOMUX_GPIO1C1_SEL_SHIFT                 (2U)
#define GRF_GPIO1C_IOMUX_GPIO1C1_SEL_MASK                  (0x3U << GRF_GPIO1C_IOMUX_GPIO1C1_SEL_SHIFT)                 /* 0x0000000C */
#define GRF_GPIO1C_IOMUX_GPIO1C2_SEL_SHIFT                 (4U)
#define GRF_GPIO1C_IOMUX_GPIO1C2_SEL_MASK                  (0x3U << GRF_GPIO1C_IOMUX_GPIO1C2_SEL_SHIFT)                 /* 0x00000030 */
#define GRF_GPIO1C_IOMUX_GPIO1C3_SEL_SHIFT                 (6U)
#define GRF_GPIO1C_IOMUX_GPIO1C3_SEL_MASK                  (0x3U << GRF_GPIO1C_IOMUX_GPIO1C3_SEL_SHIFT)                 /* 0x000000C0 */
#define GRF_GPIO1C_IOMUX_GPIO1C4_SEL_SHIFT                 (8U)
#define GRF_GPIO1C_IOMUX_GPIO1C4_SEL_MASK                  (0x3U << GRF_GPIO1C_IOMUX_GPIO1C4_SEL_SHIFT)                 /* 0x00000300 */
#define GRF_GPIO1C_IOMUX_GPIO1C5_SEL_SHIFT                 (10U)
#define GRF_GPIO1C_IOMUX_GPIO1C5_SEL_MASK                  (0x3U << GRF_GPIO1C_IOMUX_GPIO1C5_SEL_SHIFT)                 /* 0x00000C00 */
#define GRF_GPIO1C_IOMUX_GPIO1C6_SEL_SHIFT                 (12U)
#define GRF_GPIO1C_IOMUX_GPIO1C6_SEL_MASK                  (0x3U << GRF_GPIO1C_IOMUX_GPIO1C6_SEL_SHIFT)                 /* 0x00003000 */
#define GRF_GPIO1C_IOMUX_GPIO1C7_SEL_SHIFT                 (14U)
#define GRF_GPIO1C_IOMUX_GPIO1C7_SEL_MASK                  (0x3U << GRF_GPIO1C_IOMUX_GPIO1C7_SEL_SHIFT)                 /* 0x0000C000 */
/* GPIO1D_IOMUX */
#define GRF_GPIO1D_IOMUX_OFFSET                            (0x2CU)
#define GRF_GPIO1D_IOMUX_GPIO1D0_SEL_SHIFT                 (0U)
#define GRF_GPIO1D_IOMUX_GPIO1D0_SEL_MASK                  (0x3U << GRF_GPIO1D_IOMUX_GPIO1D0_SEL_SHIFT)                 /* 0x00000003 */
#define GRF_GPIO1D_IOMUX_GPIO1D1_SEL_SHIFT                 (2U)
#define GRF_GPIO1D_IOMUX_GPIO1D1_SEL_MASK                  (0x3U << GRF_GPIO1D_IOMUX_GPIO1D1_SEL_SHIFT)                 /* 0x0000000C */
#define GRF_GPIO1D_IOMUX_GPIO1D2_SEL_SHIFT                 (4U)
#define GRF_GPIO1D_IOMUX_GPIO1D2_SEL_MASK                  (0x3U << GRF_GPIO1D_IOMUX_GPIO1D2_SEL_SHIFT)                 /* 0x00000030 */
#define GRF_GPIO1D_IOMUX_GPIO1D3_SEL_SHIFT                 (6U)
#define GRF_GPIO1D_IOMUX_GPIO1D3_SEL_MASK                  (0x3U << GRF_GPIO1D_IOMUX_GPIO1D3_SEL_SHIFT)                 /* 0x000000C0 */
#define GRF_GPIO1D_IOMUX_GPIO1D4_SEL_SHIFT                 (8U)
#define GRF_GPIO1D_IOMUX_GPIO1D4_SEL_MASK                  (0x3U << GRF_GPIO1D_IOMUX_GPIO1D4_SEL_SHIFT)                 /* 0x00000300 */
#define GRF_GPIO1D_IOMUX_GPIO1D5_SEL_SHIFT                 (10U)
#define GRF_GPIO1D_IOMUX_GPIO1D5_SEL_MASK                  (0x3U << GRF_GPIO1D_IOMUX_GPIO1D5_SEL_SHIFT)                 /* 0x00000C00 */
#define GRF_GPIO1D_IOMUX_GPIO1D6_SEL_SHIFT                 (12U)
#define GRF_GPIO1D_IOMUX_GPIO1D6_SEL_MASK                  (0x3U << GRF_GPIO1D_IOMUX_GPIO1D6_SEL_SHIFT)                 /* 0x00003000 */
#define GRF_GPIO1D_IOMUX_GPIO1D7_SEL_SHIFT                 (14U)
#define GRF_GPIO1D_IOMUX_GPIO1D7_SEL_MASK                  (0x3U << GRF_GPIO1D_IOMUX_GPIO1D7_SEL_SHIFT)                 /* 0x0000C000 */
/* GPIO1A_P */
#define GRF_GPIO1A_P_OFFSET                                (0x30U)
#define GRF_GPIO1A_P_PULL_SHIFT                            (0U)
#define GRF_GPIO1A_P_PULL_MASK                             (0xFFU << GRF_GPIO1A_P_PULL_SHIFT)                           /* 0x000000FF */
/* GPIO1B_P */
#define GRF_GPIO1B_P_OFFSET                                (0x34U)
#define GRF_GPIO1B_P_PULL_SHIFT                            (0U)
#define GRF_GPIO1B_P_PULL_MASK                             (0xFFU << GRF_GPIO1B_P_PULL_SHIFT)                           /* 0x000000FF */
/* GPIO1C_P */
#define GRF_GPIO1C_P_OFFSET                                (0x38U)
#define GRF_GPIO1C_P_PULL_SHIFT                            (0U)
#define GRF_GPIO1C_P_PULL_MASK                             (0xFFU << GRF_GPIO1C_P_PULL_SHIFT)                           /* 0x000000FF */
/* GPIO1D_P */
#define GRF_GPIO1D_P_OFFSET                                (0x3CU)
#define GRF_GPIO1D_P_PULL_SHIFT                            (0U)
#define GRF_GPIO1D_P_PULL_MASK                             (0xFFU << GRF_GPIO1D_P_PULL_SHIFT)                           /* 0x000000FF */
/* GPIO2A_IOMUX */
#define GRF_GPIO2A_IOMUX_OFFSET                            (0x40U)
#define GRF_GPIO2A_IOMUX_GPIO2A0_SEL_SHIFT                 (0U)
#define GRF_GPIO2A_IOMUX_GPIO2A0_SEL_MASK                  (0x3U << GRF_GPIO2A_IOMUX_GPIO2A0_SEL_SHIFT)                 /* 0x00000003 */
#define GRF_GPIO2A_IOMUX_GPIO2A1_SEL_SHIFT                 (2U)
#define GRF_GPIO2A_IOMUX_GPIO2A1_SEL_MASK                  (0x3U << GRF_GPIO2A_IOMUX_GPIO2A1_SEL_SHIFT)                 /* 0x0000000C */
#define GRF_GPIO2A_IOMUX_GPIO2A2_SEL_SHIFT                 (4U)
#define GRF_GPIO2A_IOMUX_GPIO2A2_SEL_MASK                  (0x3U << GRF_GPIO2A_IOMUX_GPIO2A2_SEL_SHIFT)                 /* 0x00000030 */
#define GRF_GPIO2A_IOMUX_GPIO2A3_SEL_SHIFT                 (6U)
#define GRF_GPIO2A_IOMUX_GPIO2A3_SEL_MASK                  (0x3U << GRF_GPIO2A_IOMUX_GPIO2A3_SEL_SHIFT)                 /* 0x000000C0 */
#define GRF_GPIO2A_IOMUX_GPIO2A4_SEL_SHIFT                 (8U)
#define GRF_GPIO2A_IOMUX_GPIO2A4_SEL_MASK                  (0x3U << GRF_GPIO2A_IOMUX_GPIO2A4_SEL_SHIFT)                 /* 0x00000300 */
#define GRF_GPIO2A_IOMUX_GPIO2A5_SEL_SHIFT                 (10U)
#define GRF_GPIO2A_IOMUX_GPIO2A5_SEL_MASK                  (0x3U << GRF_GPIO2A_IOMUX_GPIO2A5_SEL_SHIFT)                 /* 0x00000C00 */
#define GRF_GPIO2A_IOMUX_GPIO2A6_SEL_SHIFT                 (12U)
#define GRF_GPIO2A_IOMUX_GPIO2A6_SEL_MASK                  (0x3U << GRF_GPIO2A_IOMUX_GPIO2A6_SEL_SHIFT)                 /* 0x00003000 */
#define GRF_GPIO2A_IOMUX_GPIO2A7_SEL_SHIFT                 (14U)
#define GRF_GPIO2A_IOMUX_GPIO2A7_SEL_MASK                  (0x3U << GRF_GPIO2A_IOMUX_GPIO2A7_SEL_SHIFT)                 /* 0x0000C000 */
/* GPIO2B_IOMUX */
#define GRF_GPIO2B_IOMUX_OFFSET                            (0x44U)
#define GRF_GPIO2B_IOMUX_GPIO2B0_SEL_SHIFT                 (0U)
#define GRF_GPIO2B_IOMUX_GPIO2B0_SEL_MASK                  (0x3U << GRF_GPIO2B_IOMUX_GPIO2B0_SEL_SHIFT)                 /* 0x00000003 */
#define GRF_GPIO2B_IOMUX_GPIO2B1_SEL_SHIFT                 (2U)
#define GRF_GPIO2B_IOMUX_GPIO2B1_SEL_MASK                  (0x3U << GRF_GPIO2B_IOMUX_GPIO2B1_SEL_SHIFT)                 /* 0x0000000C */
#define GRF_GPIO2B_IOMUX_GPIO2B2_SEL_SHIFT                 (4U)
#define GRF_GPIO2B_IOMUX_GPIO2B2_SEL_MASK                  (0x3U << GRF_GPIO2B_IOMUX_GPIO2B2_SEL_SHIFT)                 /* 0x00000030 */
#define GRF_GPIO2B_IOMUX_GPIO2B3_SEL_SHIFT                 (6U)
#define GRF_GPIO2B_IOMUX_GPIO2B3_SEL_MASK                  (0x3U << GRF_GPIO2B_IOMUX_GPIO2B3_SEL_SHIFT)                 /* 0x000000C0 */
#define GRF_GPIO2B_IOMUX_GPIO2B4_SEL_SHIFT                 (8U)
#define GRF_GPIO2B_IOMUX_GPIO2B4_SEL_MASK                  (0x3U << GRF_GPIO2B_IOMUX_GPIO2B4_SEL_SHIFT)                 /* 0x00000300 */
#define GRF_GPIO2B_IOMUX_GPIO2B5_SEL_SHIFT                 (10U)
#define GRF_GPIO2B_IOMUX_GPIO2B5_SEL_MASK                  (0x3U << GRF_GPIO2B_IOMUX_GPIO2B5_SEL_SHIFT)                 /* 0x00000C00 */
#define GRF_GPIO2B_IOMUX_GPIO2B6_SEL_SHIFT                 (12U)
#define GRF_GPIO2B_IOMUX_GPIO2B6_SEL_MASK                  (0x3U << GRF_GPIO2B_IOMUX_GPIO2B6_SEL_SHIFT)                 /* 0x00003000 */
#define GRF_GPIO2B_IOMUX_GPIO2B7_SEL_SHIFT                 (14U)
#define GRF_GPIO2B_IOMUX_GPIO2B7_SEL_MASK                  (0x3U << GRF_GPIO2B_IOMUX_GPIO2B7_SEL_SHIFT)                 /* 0x0000C000 */
/* GPIO2C_IOMUX */
#define GRF_GPIO2C_IOMUX_OFFSET                            (0x48U)
#define GRF_GPIO2C_IOMUX_GPIO2C0_SEL_SHIFT                 (0U)
#define GRF_GPIO2C_IOMUX_GPIO2C0_SEL_MASK                  (0x3U << GRF_GPIO2C_IOMUX_GPIO2C0_SEL_SHIFT)                 /* 0x00000003 */
#define GRF_GPIO2C_IOMUX_GPIO2C1_SEL_SHIFT                 (2U)
#define GRF_GPIO2C_IOMUX_GPIO2C1_SEL_MASK                  (0x3U << GRF_GPIO2C_IOMUX_GPIO2C1_SEL_SHIFT)                 /* 0x0000000C */
#define GRF_GPIO2C_IOMUX_GPIO2C2_SEL_SHIFT                 (4U)
#define GRF_GPIO2C_IOMUX_GPIO2C2_SEL_MASK                  (0x3U << GRF_GPIO2C_IOMUX_GPIO2C2_SEL_SHIFT)                 /* 0x00000030 */
#define GRF_GPIO2C_IOMUX_GPIO2C3_SEL_SHIFT                 (6U)
#define GRF_GPIO2C_IOMUX_GPIO2C3_SEL_MASK                  (0x3U << GRF_GPIO2C_IOMUX_GPIO2C3_SEL_SHIFT)                 /* 0x000000C0 */
#define GRF_GPIO2C_IOMUX_GPIO2C4_SEL_SHIFT                 (8U)
#define GRF_GPIO2C_IOMUX_GPIO2C4_SEL_MASK                  (0x3U << GRF_GPIO2C_IOMUX_GPIO2C4_SEL_SHIFT)                 /* 0x00000300 */
#define GRF_GPIO2C_IOMUX_GPIO2C5_SEL_SHIFT                 (10U)
#define GRF_GPIO2C_IOMUX_GPIO2C5_SEL_MASK                  (0x3U << GRF_GPIO2C_IOMUX_GPIO2C5_SEL_SHIFT)                 /* 0x00000C00 */
#define GRF_GPIO2C_IOMUX_GPIO2C6_SEL_SHIFT                 (12U)
#define GRF_GPIO2C_IOMUX_GPIO2C6_SEL_MASK                  (0x3U << GRF_GPIO2C_IOMUX_GPIO2C6_SEL_SHIFT)                 /* 0x00003000 */
#define GRF_GPIO2C_IOMUX_GPIO2C7_SEL_SHIFT                 (14U)
#define GRF_GPIO2C_IOMUX_GPIO2C7_SEL_MASK                  (0x3U << GRF_GPIO2C_IOMUX_GPIO2C7_SEL_SHIFT)                 /* 0x0000C000 */
/* GPIO2D_IOMUX */
#define GRF_GPIO2D_IOMUX_OFFSET                            (0x4CU)
#define GRF_GPIO2D_IOMUX_GPIO2D0_SEL_SHIFT                 (0U)
#define GRF_GPIO2D_IOMUX_GPIO2D0_SEL_MASK                  (0x3U << GRF_GPIO2D_IOMUX_GPIO2D0_SEL_SHIFT)                 /* 0x00000003 */
#define GRF_GPIO2D_IOMUX_GPIO2D1_SEL_SHIFT                 (2U)
#define GRF_GPIO2D_IOMUX_GPIO2D1_SEL_MASK                  (0x3U << GRF_GPIO2D_IOMUX_GPIO2D1_SEL_SHIFT)                 /* 0x0000000C */
#define GRF_GPIO2D_IOMUX_GPIO2D2_SEL_SHIFT                 (4U)
#define GRF_GPIO2D_IOMUX_GPIO2D2_SEL_MASK                  (0x3U << GRF_GPIO2D_IOMUX_GPIO2D2_SEL_SHIFT)                 /* 0x00000030 */
#define GRF_GPIO2D_IOMUX_GPIO2D3_SEL_SHIFT                 (6U)
#define GRF_GPIO2D_IOMUX_GPIO2D3_SEL_MASK                  (0x3U << GRF_GPIO2D_IOMUX_GPIO2D3_SEL_SHIFT)                 /* 0x000000C0 */
#define GRF_GPIO2D_IOMUX_GPIO2D4_SEL_SHIFT                 (8U)
#define GRF_GPIO2D_IOMUX_GPIO2D4_SEL_MASK                  (0x3U << GRF_GPIO2D_IOMUX_GPIO2D4_SEL_SHIFT)                 /* 0x00000300 */
#define GRF_GPIO2D_IOMUX_GPIO2D5_SEL_SHIFT                 (10U)
#define GRF_GPIO2D_IOMUX_GPIO2D5_SEL_MASK                  (0x3U << GRF_GPIO2D_IOMUX_GPIO2D5_SEL_SHIFT)                 /* 0x00000C00 */
#define GRF_GPIO2D_IOMUX_GPIO2D6_SEL_SHIFT                 (12U)
#define GRF_GPIO2D_IOMUX_GPIO2D6_SEL_MASK                  (0x3U << GRF_GPIO2D_IOMUX_GPIO2D6_SEL_SHIFT)                 /* 0x00003000 */
#define GRF_GPIO2D_IOMUX_GPIO2D7_SEL_SHIFT                 (14U)
#define GRF_GPIO2D_IOMUX_GPIO2D7_SEL_MASK                  (0x3U << GRF_GPIO2D_IOMUX_GPIO2D7_SEL_SHIFT)                 /* 0x0000C000 */
/* GPIO2A_P */
#define GRF_GPIO2A_P_OFFSET                                (0x50U)
#define GRF_GPIO2A_P_PULL_SHIFT                            (0U)
#define GRF_GPIO2A_P_PULL_MASK                             (0xFFU << GRF_GPIO2A_P_PULL_SHIFT)                           /* 0x000000FF */
/* GPIO2B_P */
#define GRF_GPIO2B_P_OFFSET                                (0x54U)
#define GRF_GPIO2B_P_PULL_SHIFT                            (0U)
#define GRF_GPIO2B_P_PULL_MASK                             (0xFFU << GRF_GPIO2B_P_PULL_SHIFT)                           /* 0x000000FF */
/* GPIO2C_P */
#define GRF_GPIO2C_P_OFFSET                                (0x58U)
#define GRF_GPIO2C_P_PULL_SHIFT                            (0U)
#define GRF_GPIO2C_P_PULL_MASK                             (0xFFU << GRF_GPIO2C_P_PULL_SHIFT)                           /* 0x000000FF */
/* GPIO2D_P */
#define GRF_GPIO2D_P_OFFSET                                (0x5CU)
#define GRF_GPIO2D_P_PULL_SHIFT                            (0U)
#define GRF_GPIO2D_P_PULL_MASK                             (0xFFU << GRF_GPIO2D_P_PULL_SHIFT)                           /* 0x000000FF */
/* SOFT_CON0 */
#define GRF_SOFT_CON0_OFFSET                               (0x60U)
#define GRF_SOFT_CON0_DBG_SHIFT                            (0U)
#define GRF_SOFT_CON0_DBG_MASK                             (0xFFFFU << GRF_SOFT_CON0_DBG_SHIFT)                         /* 0x0000FFFF */
/* SOFT_CON1 */
#define GRF_SOFT_CON1_OFFSET                               (0x64U)
#define GRF_SOFT_CON1_DBG_SHIFT                            (0U)
#define GRF_SOFT_CON1_DBG_MASK                             (0xFFFFU << GRF_SOFT_CON1_DBG_SHIFT)                         /* 0x0000FFFF */
/* USBPHY_CON11 */
#define GRF_USBPHY_CON11_OFFSET                            (0x80U)
/* USBPHY_CON10 */
#define GRF_USBPHY_CON10_OFFSET                            (0x84U)
/* USBPHY_CON9 */
#define GRF_USBPHY_CON9_OFFSET                             (0x88U)
/* USBPHY_CON8 */
#define GRF_USBPHY_CON8_OFFSET                             (0x8CU)
/* USBPHY_CON7 */
#define GRF_USBPHY_CON7_OFFSET                             (0x90U)
#define GRF_USBPHY_CON7_VBUS_VALID_CON_SHIFT               (0U)
#define GRF_USBPHY_CON7_VBUS_VALID_CON_MASK                (0x7U << GRF_USBPHY_CON7_VBUS_VALID_CON_SHIFT)               /* 0x00000007 */
#define GRF_USBPHY_CON7_HALF_BIT_PRE_EMPHA_EN_SHIFT        (6U)
#define GRF_USBPHY_CON7_HALF_BIT_PRE_EMPHA_EN_MASK         (0x1U << GRF_USBPHY_CON7_HALF_BIT_PRE_EMPHA_EN_SHIFT)        /* 0x00000040 */
#define GRF_USBPHY_CON7_BYPASS_SQUELCH_TRIGGER_SHIFT       (7U)
#define GRF_USBPHY_CON7_BYPASS_SQUELCH_TRIGGER_MASK        (0x1U << GRF_USBPHY_CON7_BYPASS_SQUELCH_TRIGGER_SHIFT)       /* 0x00000080 */
#define GRF_USBPHY_CON7_HOST_DISCON_CON_SHIFT              (11U)
#define GRF_USBPHY_CON7_HOST_DISCON_CON_MASK               (0xFU << GRF_USBPHY_CON7_HOST_DISCON_CON_SHIFT)              /* 0x00007800 */
/* USBPHY_CON6 */
#define GRF_USBPHY_CON6_OFFSET                             (0x94U)
#define GRF_USBPHY_CON6_TURN_OFF_DIFF_RECEIVER_SHIFT       (2U)
#define GRF_USBPHY_CON6_TURN_OFF_DIFF_RECEIVER_MASK        (0x1U << GRF_USBPHY_CON6_TURN_OFF_DIFF_RECEIVER_SHIFT)       /* 0x00000004 */
#define GRF_USBPHY_CON6_FORCE_A_SESSION_VAL_SHIFT          (3U)
#define GRF_USBPHY_CON6_FORCE_A_SESSION_VAL_MASK           (0x1U << GRF_USBPHY_CON6_FORCE_A_SESSION_VAL_SHIFT)          /* 0x00000008 */
#define GRF_USBPHY_CON6_FORCE_B_SESSION_VAL_SHIFT          (4U)
#define GRF_USBPHY_CON6_FORCE_B_SESSION_VAL_MASK           (0x1U << GRF_USBPHY_CON6_FORCE_B_SESSION_VAL_SHIFT)          /* 0x00000010 */
#define GRF_USBPHY_CON6_FORCE_SESSION_END_VAL_SHIFT        (5U)
#define GRF_USBPHY_CON6_FORCE_SESSION_END_VAL_MASK         (0x1U << GRF_USBPHY_CON6_FORCE_SESSION_END_VAL_SHIFT)        /* 0x00000020 */
#define GRF_USBPHY_CON6_FORCE_VBUS_VALID_SHIFT             (6U)
#define GRF_USBPHY_CON6_FORCE_VBUS_VALID_MASK              (0x1U << GRF_USBPHY_CON6_FORCE_VBUS_VALID_SHIFT)             /* 0x00000040 */
#define GRF_USBPHY_CON6_A_SESSION_CON_SHIFT                (7U)
#define GRF_USBPHY_CON6_A_SESSION_CON_MASK                 (0x7U << GRF_USBPHY_CON6_A_SESSION_CON_SHIFT)                /* 0x00000380 */
#define GRF_USBPHY_CON6_B_SESSION_CON_SHIFT                (10U)
#define GRF_USBPHY_CON6_B_SESSION_CON_MASK                 (0x7U << GRF_USBPHY_CON6_B_SESSION_CON_SHIFT)                /* 0x00001C00 */
#define GRF_USBPHY_CON6_SESSION_END_CON_SHIFT              (13U)
#define GRF_USBPHY_CON6_SESSION_END_CON_MASK               (0x7U << GRF_USBPHY_CON6_SESSION_END_CON_SHIFT)              /* 0x0000E000 */
/* USBPHY_CON5 */
#define GRF_USBPHY_CON5_OFFSET                             (0x98U)
/* USBPHY_CON4 */
#define GRF_USBPHY_CON4_OFFSET                             (0x9CU)
#define GRF_USBPHY_CON4_BYPASS_5V_TOLERANCE_DET_SHIFT      (1U)
#define GRF_USBPHY_CON4_BYPASS_5V_TOLERANCE_DET_MASK       (0x1U << GRF_USBPHY_CON4_BYPASS_5V_TOLERANCE_DET_SHIFT)      /* 0x00000002 */
/* USBPHY_CON3 */
#define GRF_USBPHY_CON3_OFFSET                             (0xA0U)
#define GRF_USBPHY_CON3_ODT_COMPENSATION_SHIFT             (0U)
#define GRF_USBPHY_CON3_ODT_COMPENSATION_MASK              (0x3U << GRF_USBPHY_CON3_ODT_COMPENSATION_SHIFT)             /* 0x00000003 */
#define GRF_USBPHY_CON3_BIAS_CURRENT_REF_SHIFT             (2U)
#define GRF_USBPHY_CON3_BIAS_CURRENT_REF_MASK              (0x7U << GRF_USBPHY_CON3_BIAS_CURRENT_REF_SHIFT)             /* 0x0000001C */
#define GRF_USBPHY_CON3_COMPEN_CURRENT_REF_SHIFT           (5U)
#define GRF_USBPHY_CON3_COMPEN_CURRENT_REF_MASK            (0x7U << GRF_USBPHY_CON3_COMPEN_CURRENT_REF_SHIFT)           /* 0x000000E0 */
#define GRF_USBPHY_CON3_BG_OUT_VOLTAGE_ADJUST_SHIFT        (8U)
#define GRF_USBPHY_CON3_BG_OUT_VOLTAGE_ADJUST_MASK         (0x1U << GRF_USBPHY_CON3_BG_OUT_VOLTAGE_ADJUST_SHIFT)        /* 0x00000100 */
#define GRF_USBPHY_CON3_ODT_AUTO_REFRESH_SHIFT             (9U)
#define GRF_USBPHY_CON3_ODT_AUTO_REFRESH_MASK              (0x1U << GRF_USBPHY_CON3_ODT_AUTO_REFRESH_SHIFT)             /* 0x00000200 */
#define GRF_USBPHY_CON3_VOL_TOLERAN_DET_CON_SHIFT          (14U)
#define GRF_USBPHY_CON3_VOL_TOLERAN_DET_CON_MASK           (0x1U << GRF_USBPHY_CON3_VOL_TOLERAN_DET_CON_SHIFT)          /* 0x00004000 */
/* USBPHY_CON2 */
#define GRF_USBPHY_CON2_OFFSET                             (0xA4U)
#define GRF_USBPHY_CON2_HS_EYE_HEIGHT_SHIFT                (0U)
#define GRF_USBPHY_CON2_HS_EYE_HEIGHT_MASK                 (0x1FU << GRF_USBPHY_CON2_HS_EYE_HEIGHT_SHIFT)               /* 0x0000001F */
#define GRF_USBPHY_CON2_HFS_DRIVER_STRENGTH_SHIFT          (5U)
#define GRF_USBPHY_CON2_HFS_DRIVER_STRENGTH_MASK           (0x1FU << GRF_USBPHY_CON2_HFS_DRIVER_STRENGTH_SHIFT)         /* 0x000003E0 */
#define GRF_USBPHY_CON2_AUTO_COMPENSATION_BYPASS_SHIFT     (10U)
#define GRF_USBPHY_CON2_AUTO_COMPENSATION_BYPASS_MASK      (0x3U << GRF_USBPHY_CON2_AUTO_COMPENSATION_BYPASS_SHIFT)     /* 0x00000C00 */
#define GRF_USBPHY_CON2_VOLTAGE_TOLERANCE_ADJUST_SHIFT     (13U)
#define GRF_USBPHY_CON2_VOLTAGE_TOLERANCE_ADJUST_MASK      (0x3U << GRF_USBPHY_CON2_VOLTAGE_TOLERANCE_ADJUST_SHIFT)     /* 0x00006000 */
#define GRF_USBPHY_CON2_ODT_COMPENSATION_SHIFT             (15U)
#define GRF_USBPHY_CON2_ODT_COMPENSATION_MASK              (0x1U << GRF_USBPHY_CON2_ODT_COMPENSATION_SHIFT)             /* 0x00008000 */
/* USBPHY_CON1 */
#define GRF_USBPHY_CON1_OFFSET                             (0xA8U)
#define GRF_USBPHY_CON1_SQUEL_TRIGGER_CON_SHIFT            (0U)
#define GRF_USBPHY_CON1_SQUEL_TRIGGER_CON_MASK             (0x1U << GRF_USBPHY_CON1_SQUEL_TRIGGER_CON_SHIFT)            /* 0x00000001 */
#define GRF_USBPHY_CON1_RES_COMP_EN_SHIFT                  (1U)
#define GRF_USBPHY_CON1_RES_COMP_EN_MASK                   (0x1U << GRF_USBPHY_CON1_RES_COMP_EN_SHIFT)                  /* 0x00000002 */
#define GRF_USBPHY_CON1_CURRENT_COMP_EN_SHIFT              (2U)
#define GRF_USBPHY_CON1_CURRENT_COMP_EN_MASK               (0x1U << GRF_USBPHY_CON1_CURRENT_COMP_EN_SHIFT)              /* 0x00000004 */
#define GRF_USBPHY_CON1_HS_EYE_HEIGHT_SHIFT                (13U)
#define GRF_USBPHY_CON1_HS_EYE_HEIGHT_MASK                 (0x7U << GRF_USBPHY_CON1_HS_EYE_HEIGHT_SHIFT)                /* 0x0000E000 */
/* USBPHY_CON0 */
#define GRF_USBPHY_CON0_OFFSET                             (0xACU)
#define GRF_USBPHY_CON0_HS_EYE_DIAG_ADJUST_SHIFT           (0U)
#define GRF_USBPHY_CON0_HS_EYE_DIAG_ADJUST_MASK            (0x7U << GRF_USBPHY_CON0_HS_EYE_DIAG_ADJUST_SHIFT)           /* 0x00000007 */
#define GRF_USBPHY_CON0_FLS_EYE_HEIGHT_SHIFT               (3U)
#define GRF_USBPHY_CON0_FLS_EYE_HEIGHT_MASK                (0x3U << GRF_USBPHY_CON0_FLS_EYE_HEIGHT_SHIFT)               /* 0x00000018 */
#define GRF_USBPHY_CON0_RX_CLK_PHASE_CON_SHIFT             (5U)
#define GRF_USBPHY_CON0_RX_CLK_PHASE_CON_MASK              (0x7U << GRF_USBPHY_CON0_RX_CLK_PHASE_CON_SHIFT)             /* 0x000000E0 */
#define GRF_USBPHY_CON0_TX_CLK_PHASE_CON_SHIFT             (8U)
#define GRF_USBPHY_CON0_TX_CLK_PHASE_CON_MASK              (0x7U << GRF_USBPHY_CON0_TX_CLK_PHASE_CON_SHIFT)             /* 0x00000700 */
#define GRF_USBPHY_CON0_NON_DRIVING_SHIFT                  (11U)
#define GRF_USBPHY_CON0_NON_DRIVING_MASK                   (0x3U << GRF_USBPHY_CON0_NON_DRIVING_SHIFT)                  /* 0x00001800 */
#define GRF_USBPHY_CON0_SQUEL_TRIGGER_CON_SHIFT            (13U)
#define GRF_USBPHY_CON0_SQUEL_TRIGGER_CON_MASK             (0x7U << GRF_USBPHY_CON0_SQUEL_TRIGGER_CON_SHIFT)            /* 0x0000E000 */
/* UOC_CON0 */
#define GRF_UOC_CON0_OFFSET                                (0xB0U)
#define GRF_UOC_CON0_OTGDISABLE0_SHIFT                     (0U)
#define GRF_UOC_CON0_OTGDISABLE0_MASK                      (0x1U << GRF_UOC_CON0_OTGDISABLE0_SHIFT)                     /* 0x00000001 */
#define GRF_UOC_CON0_BYPASSDMEN0_SHIFT                     (1U)
#define GRF_UOC_CON0_BYPASSDMEN0_MASK                      (0x1U << GRF_UOC_CON0_BYPASSDMEN0_SHIFT)                     /* 0x00000002 */
#define GRF_UOC_CON0_BYPASSSEL0_SHIFT                      (2U)
#define GRF_UOC_CON0_BYPASSSEL0_MASK                       (0x1U << GRF_UOC_CON0_BYPASSSEL0_SHIFT)                      /* 0x00000004 */
/* UOC_CON1 */
#define GRF_UOC_CON1_OFFSET                                (0xB4U)
#define GRF_UOC_CON1_USB_CTRL_SEL_SHIFT                    (0U)
#define GRF_UOC_CON1_USB_CTRL_SEL_MASK                     (0x1U << GRF_UOC_CON1_USB_CTRL_SEL_SHIFT)                    /* 0x00000001 */
#define GRF_UOC_CON1_UTMI_SUSPEND_N_SHIFT                  (1U)
#define GRF_UOC_CON1_UTMI_SUSPEND_N_MASK                   (0x1U << GRF_UOC_CON1_UTMI_SUSPEND_N_SHIFT)                  /* 0x00000002 */
#define GRF_UOC_CON1_UTMI_OPMODE_SHIFT                     (2U)
#define GRF_UOC_CON1_UTMI_OPMODE_MASK                      (0x3U << GRF_UOC_CON1_UTMI_OPMODE_SHIFT)                     /* 0x0000000C */
#define GRF_UOC_CON1_UTMI_XCVRSELECT_SHIFT                 (4U)
#define GRF_UOC_CON1_UTMI_XCVRSELECT_MASK                  (0x3U << GRF_UOC_CON1_UTMI_XCVRSELECT_SHIFT)                 /* 0x00000030 */
#define GRF_UOC_CON1_UTMI_TERMSELECT_SHIFT                 (6U)
#define GRF_UOC_CON1_UTMI_TERMSELECT_MASK                  (0x1U << GRF_UOC_CON1_UTMI_TERMSELECT_SHIFT)                 /* 0x00000040 */
#define GRF_UOC_CON1_UTMIOTG_IDDIG_SEL_SHIFT               (9U)
#define GRF_UOC_CON1_UTMIOTG_IDDIG_SEL_MASK                (0x1U << GRF_UOC_CON1_UTMIOTG_IDDIG_SEL_SHIFT)               /* 0x00000200 */
#define GRF_UOC_CON1_UTMIOTG_IDDIG_SHIFT                   (10U)
#define GRF_UOC_CON1_UTMIOTG_IDDIG_MASK                    (0x1U << GRF_UOC_CON1_UTMIOTG_IDDIG_SHIFT)                   /* 0x00000400 */
/* UOC_CON2 */
#define GRF_UOC_CON2_OFFSET                                (0xB8U)
#define GRF_UOC_CON2_USBPHY_COMMONON_SHIFT                 (0U)
#define GRF_UOC_CON2_USBPHY_COMMONON_MASK                  (0x1U << GRF_UOC_CON2_USBPHY_COMMONON_SHIFT)                 /* 0x00000001 */
#define GRF_UOC_CON2_UTMI_DPPULLDOWN_SHIFT                 (1U)
#define GRF_UOC_CON2_UTMI_DPPULLDOWN_MASK                  (0x1U << GRF_UOC_CON2_UTMI_DPPULLDOWN_SHIFT)                 /* 0x00000002 */
#define GRF_UOC_CON2_UTMI_DMPULLDOWN_SHIFT                 (2U)
#define GRF_UOC_CON2_UTMI_DMPULLDOWN_MASK                  (0x1U << GRF_UOC_CON2_UTMI_DMPULLDOWN_SHIFT)                 /* 0x00000004 */
#define GRF_UOC_CON2_UTMI_IDPULLUP_SHIFT                   (3U)
#define GRF_UOC_CON2_UTMI_IDPULLUP_MASK                    (0x1U << GRF_UOC_CON2_UTMI_IDPULLUP_SHIFT)                   /* 0x00000008 */
#define GRF_UOC_CON2_UTMI_DRVVBUS_SHIFT                    (4U)
#define GRF_UOC_CON2_UTMI_DRVVBUS_MASK                     (0x1U << GRF_UOC_CON2_UTMI_DRVVBUS_SHIFT)                    /* 0x00000010 */
#define GRF_UOC_CON2_UTMI_CHRGBUS_SHIFT                    (5U)
#define GRF_UOC_CON2_UTMI_CHRGBUS_MASK                     (0x1U << GRF_UOC_CON2_UTMI_CHRGBUS_SHIFT)                    /* 0x00000020 */
#define GRF_UOC_CON2_UTMI_DISCHRGBUS_SHIFT                 (6U)
#define GRF_UOC_CON2_UTMI_DISCHRGBUS_MASK                  (0x1U << GRF_UOC_CON2_UTMI_DISCHRGBUS_SHIFT)                 /* 0x00000040 */
/* IOFUNC_CON0 */
#define GRF_IOFUNC_CON0_OFFSET                             (0xBCU)
#define GRF_IOFUNC_CON0_I2C0_SEL_SHIFT                     (0U)
#define GRF_IOFUNC_CON0_I2C0_SEL_MASK                      (0x3U << GRF_IOFUNC_CON0_I2C0_SEL_SHIFT)                     /* 0x00000003 */
#define GRF_IOFUNC_CON0_I2C1_SEL_SHIFT                     (2U)
#define GRF_IOFUNC_CON0_I2C1_SEL_MASK                      (0x3U << GRF_IOFUNC_CON0_I2C1_SEL_SHIFT)                     /* 0x0000000C */
#define GRF_IOFUNC_CON0_I2C2_SEL_SHIFT                     (4U)
#define GRF_IOFUNC_CON0_I2C2_SEL_MASK                      (0x3U << GRF_IOFUNC_CON0_I2C2_SEL_SHIFT)                     /* 0x00000030 */
#define GRF_IOFUNC_CON0_SPI0_SEL_SHIFT                     (6U)
#define GRF_IOFUNC_CON0_SPI0_SEL_MASK                      (0x1U << GRF_IOFUNC_CON0_SPI0_SEL_SHIFT)                     /* 0x00000040 */
#define GRF_IOFUNC_CON0_SPI1_SEL_SHIFT                     (7U)
#define GRF_IOFUNC_CON0_SPI1_SEL_MASK                      (0x1U << GRF_IOFUNC_CON0_SPI1_SEL_SHIFT)                     /* 0x00000080 */
#define GRF_IOFUNC_CON0_UART0_SEL_SHIFT                    (8U)
#define GRF_IOFUNC_CON0_UART0_SEL_MASK                     (0x1U << GRF_IOFUNC_CON0_UART0_SEL_SHIFT)                    /* 0x00000100 */
#define GRF_IOFUNC_CON0_UART1_SEL_SHIFT                    (9U)
#define GRF_IOFUNC_CON0_UART1_SEL_MASK                     (0x1U << GRF_IOFUNC_CON0_UART1_SEL_SHIFT)                    /* 0x00000200 */
#define GRF_IOFUNC_CON0_UART2_SEL_SHIFT                    (10U)
#define GRF_IOFUNC_CON0_UART2_SEL_MASK                     (0x3U << GRF_IOFUNC_CON0_UART2_SEL_SHIFT)                    /* 0x00000C00 */
#define GRF_IOFUNC_CON0_I2S0_SEL_SHIFT                     (12U)
#define GRF_IOFUNC_CON0_I2S0_SEL_MASK                      (0x1U << GRF_IOFUNC_CON0_I2S0_SEL_SHIFT)                     /* 0x00001000 */
#define GRF_IOFUNC_CON0_I2S1_SEL_SHIFT                     (13U)
#define GRF_IOFUNC_CON0_I2S1_SEL_MASK                      (0x1U << GRF_IOFUNC_CON0_I2S1_SEL_SHIFT)                     /* 0x00002000 */
#define GRF_IOFUNC_CON0_I2S1PAD_SEL_SHIFT                  (14U)
#define GRF_IOFUNC_CON0_I2S1PAD_SEL_MASK                   (0x1U << GRF_IOFUNC_CON0_I2S1PAD_SEL_SHIFT)                  /* 0x00004000 */
/* INTER_CON0 */
#define GRF_INTER_CON0_OFFSET                              (0xC4U)
#define GRF_INTER_CON0_BOOTROM_RESP_CYCLE_SHIFT            (0U)
#define GRF_INTER_CON0_BOOTROM_RESP_CYCLE_MASK             (0x1U << GRF_INTER_CON0_BOOTROM_RESP_CYCLE_SHIFT)            /* 0x00000001 */
#define GRF_INTER_CON0_DMA_UART02_SEL_SHIFT                (1U)
#define GRF_INTER_CON0_DMA_UART02_SEL_MASK                 (0x1U << GRF_INTER_CON0_DMA_UART02_SEL_SHIFT)                /* 0x00000002 */
#define GRF_INTER_CON0_M3_BRG_MTTICK_SHIFT                 (2U)
#define GRF_INTER_CON0_M3_BRG_MTTICK_MASK                  (0x1U << GRF_INTER_CON0_M3_BRG_MTTICK_SHIFT)                 /* 0x00000004 */
#define GRF_INTER_CON0_M3_SYNC_BYPASS_SHIFT                (3U)
#define GRF_INTER_CON0_M3_SYNC_BYPASS_MASK                 (0x1U << GRF_INTER_CON0_M3_SYNC_BYPASS_SHIFT)                /* 0x00000008 */
#define GRF_INTER_CON0_M3_SHSEL_ID_SHIFT                   (4U)
#define GRF_INTER_CON0_M3_SHSEL_ID_MASK                    (0x1U << GRF_INTER_CON0_M3_SHSEL_ID_SHIFT)                   /* 0x00000010 */
#define GRF_INTER_CON0_RESP_CYCLE_SHIFT                    (5U)
#define GRF_INTER_CON0_RESP_CYCLE_MASK                     (0x1U << GRF_INTER_CON0_RESP_CYCLE_SHIFT)                    /* 0x00000020 */
#define GRF_INTER_CON0_NOC_REMAP_SHIFT                     (8U)
#define GRF_INTER_CON0_NOC_REMAP_MASK                      (0x1U << GRF_INTER_CON0_NOC_REMAP_SHIFT)                     /* 0x00000100 */
/* SOC_STATUS0 */
#define GRF_SOC_STATUS0_OFFSET                             (0xE0U)
#define GRF_SOC_STATUS0_SYSCORE_SLEEPING_SHIFT             (0U)
#define GRF_SOC_STATUS0_SYSCORE_SLEEPING_MASK              (0x1U << GRF_SOC_STATUS0_SYSCORE_SLEEPING_SHIFT)             /* 0x00000001 */
#define GRF_SOC_STATUS0_SYSCORE_SLEEPDEEP_SHIFT            (1U)
#define GRF_SOC_STATUS0_SYSCORE_SLEEPDEEP_MASK             (0x1U << GRF_SOC_STATUS0_SYSCORE_SLEEPDEEP_SHIFT)            /* 0x00000002 */
#define GRF_SOC_STATUS0_SYSCORE_LOCKUP_SHIFT               (2U)
#define GRF_SOC_STATUS0_SYSCORE_LOCKUP_MASK                (0x1U << GRF_SOC_STATUS0_SYSCORE_LOCKUP_SHIFT)               /* 0x00000004 */
#define GRF_SOC_STATUS0_SYSCORE_HALTED_SHIFT               (3U)
#define GRF_SOC_STATUS0_SYSCORE_HALTED_MASK                (0x1U << GRF_SOC_STATUS0_SYSCORE_HALTED_SHIFT)               /* 0x00000008 */
/* SOC_STATUS1 */
#define GRF_SOC_STATUS1_OFFSET                             (0xE4U)
#define GRF_SOC_STATUS1_CALCORE_SLEEPING_SHIFT             (0U)
#define GRF_SOC_STATUS1_CALCORE_SLEEPING_MASK              (0x1U << GRF_SOC_STATUS1_CALCORE_SLEEPING_SHIFT)             /* 0x00000001 */
#define GRF_SOC_STATUS1_CALCORE_SLEEPDEEP_SHIFT            (1U)
#define GRF_SOC_STATUS1_CALCORE_SLEEPDEEP_MASK             (0x1U << GRF_SOC_STATUS1_CALCORE_SLEEPDEEP_SHIFT)            /* 0x00000002 */
#define GRF_SOC_STATUS1_CALCORE_LOCKUP_SHIFT               (2U)
#define GRF_SOC_STATUS1_CALCORE_LOCKUP_MASK                (0x1U << GRF_SOC_STATUS1_CALCORE_LOCKUP_SHIFT)               /* 0x00000004 */
#define GRF_SOC_STATUS1_CALCORE_HALTED_SHIFT               (3U)
#define GRF_SOC_STATUS1_CALCORE_HALTED_MASK                (0x1U << GRF_SOC_STATUS1_CALCORE_HALTED_SHIFT)               /* 0x00000008 */
/* SOC_USB_STATUS */
#define GRF_SOC_USB_STATUS_OFFSET                          (0xE8U)
#define GRF_SOC_USB_STATUS_UTMIOTG_VBUSVALID_SHIFT         (0U)
#define GRF_SOC_USB_STATUS_UTMIOTG_VBUSVALID_MASK          (0x1U << GRF_SOC_USB_STATUS_UTMIOTG_VBUSVALID_SHIFT)         /* 0x00000001 */
#define GRF_SOC_USB_STATUS_UTMISRP_BVALID_SHIFT            (1U)
#define GRF_SOC_USB_STATUS_UTMISRP_BVALID_MASK             (0x1U << GRF_SOC_USB_STATUS_UTMISRP_BVALID_SHIFT)            /* 0x00000002 */
#define GRF_SOC_USB_STATUS_UTMIOTG_AVALID_SHIFT            (2U)
#define GRF_SOC_USB_STATUS_UTMIOTG_AVALID_MASK             (0x1U << GRF_SOC_USB_STATUS_UTMIOTG_AVALID_SHIFT)            /* 0x00000004 */
#define GRF_SOC_USB_STATUS_UTMISRP_SESSEND_SHIFT           (3U)
#define GRF_SOC_USB_STATUS_UTMISRP_SESSEND_MASK            (0x1U << GRF_SOC_USB_STATUS_UTMISRP_SESSEND_SHIFT)           /* 0x00000008 */
/* PRJ_ID */
#define GRF_PRJ_ID_OFFSET                                  (0xF8U)
#define GRF_PRJ_ID                                         (0x4E414E4FU)
#define GRF_PRJ_ID_ID_SHIFT                                (0U)
#define GRF_PRJ_ID_ID_MASK                                 (0xFFFFFFFFU << GRF_PRJ_ID_ID_SHIFT)                         /* 0xFFFFFFFF */
/* CPU_ID */
#define GRF_CPU_ID_OFFSET                                  (0xFCU)
#define GRF_CPU_ID                                         (0x0U)
#define GRF_CPU_ID_CPU_ID_SHIFT                            (0U)
#define GRF_CPU_ID_CPU_ID_MASK                             (0x1U << GRF_CPU_ID_CPU_ID_SHIFT)                            /* 0x00000001 */
/******************************************DMA*******************************************/
/* SAR0 */
#define DMA_SAR0_OFFSET                                    (0x0U)
#define DMA_SAR0_SAR_SHIFT                                 (0U)
#define DMA_SAR0_SAR_MASK                                  (0xFFFFFFFFU << DMA_SAR0_SAR_SHIFT)                          /* 0xFFFFFFFF */
/* DAR0 */
#define DMA_DAR0_OFFSET                                    (0x8U)
#define DMA_DAR0_DAR_SHIFT                                 (0U)
#define DMA_DAR0_DAR_MASK                                  (0xFFFFFFFFU << DMA_DAR0_DAR_SHIFT)                          /* 0xFFFFFFFF */
/* LLP0 */
#define DMA_LLP0_OFFSET                                    (0x10U)
#define DMA_LLP0_LMS_SHIFT                                 (0U)
#define DMA_LLP0_LMS_MASK                                  (0x3U << DMA_LLP0_LMS_SHIFT)                                 /* 0x00000003 */
#define DMA_LLP0_LOC_SHIFT                                 (2U)
#define DMA_LLP0_LOC_MASK                                  (0x3FFFFFFFU << DMA_LLP0_LOC_SHIFT)                          /* 0xFFFFFFFC */
/* CTL0 */
#define DMA_CTL0_OFFSET                                    (0x18U)
#define DMA_CTL0_INT_EN_SHIFT                              (0U)
#define DMA_CTL0_INT_EN_MASK                               (0x1U << DMA_CTL0_INT_EN_SHIFT)                              /* 0x00000001 */
#define DMA_CTL0_DST_TR_WIDTH_SHIFT                        (1U)
#define DMA_CTL0_DST_TR_WIDTH_MASK                         (0x7U << DMA_CTL0_DST_TR_WIDTH_SHIFT)                        /* 0x0000000E */
#define DMA_CTL0_SRC_TR_WIDTH_SHIFT                        (4U)
#define DMA_CTL0_SRC_TR_WIDTH_MASK                         (0x7U << DMA_CTL0_SRC_TR_WIDTH_SHIFT)                        /* 0x00000070 */
#define DMA_CTL0_DINC_SHIFT                                (7U)
#define DMA_CTL0_DINC_MASK                                 (0x3U << DMA_CTL0_DINC_SHIFT)                                /* 0x00000180 */
#define DMA_CTL0_SINC_SHIFT                                (9U)
#define DMA_CTL0_SINC_MASK                                 (0x3U << DMA_CTL0_SINC_SHIFT)                                /* 0x00000600 */
#define DMA_CTL0_DEST_MSIZE_SHIFT                          (11U)
#define DMA_CTL0_DEST_MSIZE_MASK                           (0x7U << DMA_CTL0_DEST_MSIZE_SHIFT)                          /* 0x00003800 */
#define DMA_CTL0_SRC_MSIZE_SHIFT                           (14U)
#define DMA_CTL0_SRC_MSIZE_MASK                            (0x7U << DMA_CTL0_SRC_MSIZE_SHIFT)                           /* 0x0001C000 */
#define DMA_CTL0_SRC_GATHER_EN_SHIFT                       (17U)
#define DMA_CTL0_SRC_GATHER_EN_MASK                        (0x1U << DMA_CTL0_SRC_GATHER_EN_SHIFT)                       /* 0x00020000 */
#define DMA_CTL0_DST_SCATTER_EN_SHIFT                      (18U)
#define DMA_CTL0_DST_SCATTER_EN_MASK                       (0x1U << DMA_CTL0_DST_SCATTER_EN_SHIFT)                      /* 0x00040000 */
#define DMA_CTL0_TT_FC_SHIFT                               (20U)
#define DMA_CTL0_TT_FC_MASK                                (0x7U << DMA_CTL0_TT_FC_SHIFT)                               /* 0x00700000 */
#define DMA_CTL0_DMS_SHIFT                                 (23U)
#define DMA_CTL0_DMS_MASK                                  (0x3U << DMA_CTL0_DMS_SHIFT)                                 /* 0x01800000 */
#define DMA_CTL0_SMS_SHIFT                                 (25U)
#define DMA_CTL0_SMS_MASK                                  (0x3U << DMA_CTL0_SMS_SHIFT)                                 /* 0x06000000 */
#define DMA_CTL0_LLP_DST_EN_SHIFT                          (27U)
#define DMA_CTL0_LLP_DST_EN_MASK                           (0x1U << DMA_CTL0_LLP_DST_EN_SHIFT)                          /* 0x08000000 */
#define DMA_CTL0_LLP_SRC_EN_SHIFT                          (28U)
#define DMA_CTL0_LLP_SRC_EN_MASK                           (0x1U << DMA_CTL0_LLP_SRC_EN_SHIFT)                          /* 0x10000000 */
#define DMA_CTL0_BLOCK_TS_SHIFT                            (32U)
#define DMA_CTL0_BLOCK_TS_MASK                             (0xFFFU << DMA_CTL0_BLOCK_TS_SHIFT)                          /* 0xFFF00000000 */
#define DMA_CTL0_DONE_SHIFT                                (44U)
#define DMA_CTL0_DONE_MASK                                 (0x1U << DMA_CTL0_DONE_SHIFT)                                /* 0x100000000000 */
/* CTL_HI */
#define DMA_CTL_HI_OFFSET                                  (0x1CU)
#define DMA_CTL_HI_FIELD001_SHIFT                          (0U)
#define DMA_CTL_HI_FIELD001_MASK                           (0xFFFFFFFFU << DMA_CTL_HI_FIELD001_SHIFT)                   /* 0xFFFFFFFF */
/* SSTAT0 */
#define DMA_SSTAT0_OFFSET                                  (0x20U)
#define DMA_SSTAT0_SSTAT_SHIFT                             (0U)
#define DMA_SSTAT0_SSTAT_MASK                              (0xFFFFFFFFU << DMA_SSTAT0_SSTAT_SHIFT)                      /* 0xFFFFFFFF */
/* DSTAT0 */
#define DMA_DSTAT0_OFFSET                                  (0x28U)
#define DMA_DSTAT0_DSTAT_SHIFT                             (0U)
#define DMA_DSTAT0_DSTAT_MASK                              (0xFFFFFFFFU << DMA_DSTAT0_DSTAT_SHIFT)                      /* 0xFFFFFFFF */
/* SSTATAR0 */
#define DMA_SSTATAR0_OFFSET                                (0x30U)
#define DMA_SSTATAR0_SSTATAR_SHIFT                         (0U)
#define DMA_SSTATAR0_SSTATAR_MASK                          (0xFFFFFFFFU << DMA_SSTATAR0_SSTATAR_SHIFT)                  /* 0xFFFFFFFF */
/* DSTATAR0 */
#define DMA_DSTATAR0_OFFSET                                (0x38U)
#define DMA_DSTATAR0_DSTATAR_SHIFT                         (0U)
#define DMA_DSTATAR0_DSTATAR_MASK                          (0xFFFFFFFFU << DMA_DSTATAR0_DSTATAR_SHIFT)                  /* 0xFFFFFFFF */
/* CFG0 */
#define DMA_CFG0_OFFSET                                    (0x40U)
#define DMA_CFG0_CH_PRIOR_SHIFT                            (5U)
#define DMA_CFG0_CH_PRIOR_MASK                             (0x7U << DMA_CFG0_CH_PRIOR_SHIFT)                            /* 0x000000E0 */
#define DMA_CFG0_CH_SUSP_SHIFT                             (8U)
#define DMA_CFG0_CH_SUSP_MASK                              (0x1U << DMA_CFG0_CH_SUSP_SHIFT)                             /* 0x00000100 */
#define DMA_CFG0_FIFO_EMPTY_SHIFT                          (9U)
#define DMA_CFG0_FIFO_EMPTY_MASK                           (0x1U << DMA_CFG0_FIFO_EMPTY_SHIFT)                          /* 0x00000200 */
#define DMA_CFG0_HS_SEL_DST_SHIFT                          (10U)
#define DMA_CFG0_HS_SEL_DST_MASK                           (0x1U << DMA_CFG0_HS_SEL_DST_SHIFT)                          /* 0x00000400 */
#define DMA_CFG0_HS_SEL_SRC_SHIFT                          (11U)
#define DMA_CFG0_HS_SEL_SRC_MASK                           (0x1U << DMA_CFG0_HS_SEL_SRC_SHIFT)                          /* 0x00000800 */
#define DMA_CFG0_LOCK_CH_L_SHIFT                           (12U)
#define DMA_CFG0_LOCK_CH_L_MASK                            (0x3U << DMA_CFG0_LOCK_CH_L_SHIFT)                           /* 0x00003000 */
#define DMA_CFG0_LOCK_B_L_SHIFT                            (14U)
#define DMA_CFG0_LOCK_B_L_MASK                             (0x3U << DMA_CFG0_LOCK_B_L_SHIFT)                            /* 0x0000C000 */
#define DMA_CFG0_LOCK_CH_SHIFT                             (16U)
#define DMA_CFG0_LOCK_CH_MASK                              (0x1U << DMA_CFG0_LOCK_CH_SHIFT)                             /* 0x00010000 */
#define DMA_CFG0_LOCK_B_SHIFT                              (17U)
#define DMA_CFG0_LOCK_B_MASK                               (0x1U << DMA_CFG0_LOCK_B_SHIFT)                              /* 0x00020000 */
#define DMA_CFG0_DST_HS_POL_SHIFT                          (18U)
#define DMA_CFG0_DST_HS_POL_MASK                           (0x1U << DMA_CFG0_DST_HS_POL_SHIFT)                          /* 0x00040000 */
#define DMA_CFG0_SRC_HS_POL_SHIFT                          (19U)
#define DMA_CFG0_SRC_HS_POL_MASK                           (0x1U << DMA_CFG0_SRC_HS_POL_SHIFT)                          /* 0x00080000 */
#define DMA_CFG0_MAX_ABRST_SHIFT                           (20U)
#define DMA_CFG0_MAX_ABRST_MASK                            (0x3FFU << DMA_CFG0_MAX_ABRST_SHIFT)                         /* 0x3FF00000 */
#define DMA_CFG0_RELOAD_SRC_SHIFT                          (30U)
#define DMA_CFG0_RELOAD_SRC_MASK                           (0x1U << DMA_CFG0_RELOAD_SRC_SHIFT)                          /* 0x40000000 */
#define DMA_CFG0_RELOAD_DST_SHIFT                          (31U)
#define DMA_CFG0_RELOAD_DST_MASK                           (0x1U << DMA_CFG0_RELOAD_DST_SHIFT)                          /* 0x80000000 */
#define DMA_CFG0_FCMODE_SHIFT                              (32U)
#define DMA_CFG0_FCMODE_MASK                               (0x1U << DMA_CFG0_FCMODE_SHIFT)                              /* 0x100000000 */
#define DMA_CFG0_FIFO_MODE_SHIFT                           (33U)
#define DMA_CFG0_FIFO_MODE_MASK                            (0x1U << DMA_CFG0_FIFO_MODE_SHIFT)                           /* 0x200000000 */
#define DMA_CFG0_PROTCTL_SHIFT                             (34U)
#define DMA_CFG0_PROTCTL_MASK                              (0x7U << DMA_CFG0_PROTCTL_SHIFT)                             /* 0x1C00000000 */
#define DMA_CFG0_DS_UPD_EN_SHIFT                           (37U)
#define DMA_CFG0_DS_UPD_EN_MASK                            (0x1U << DMA_CFG0_DS_UPD_EN_SHIFT)                           /* 0x2000000000 */
#define DMA_CFG0_SS_UPD_EN_SHIFT                           (38U)
#define DMA_CFG0_SS_UPD_EN_MASK                            (0x1U << DMA_CFG0_SS_UPD_EN_SHIFT)                           /* 0x4000000000 */
#define DMA_CFG0_SRC_PER_SHIFT                             (39U)
#define DMA_CFG0_SRC_PER_MASK                              (0xFU << DMA_CFG0_SRC_PER_SHIFT)                             /* 0x78000000000 */
#define DMA_CFG0_DEST_PER_SHIFT                            (43U)
#define DMA_CFG0_DEST_PER_MASK                             (0xFU << DMA_CFG0_DEST_PER_SHIFT)                            /* 0x780000000000 */
/* CFG_HI */
#define DMA_CFG_HI_OFFSET                                  (0x44U)
#define DMA_CFG_HI_FIELD001_SHIFT                          (0U)
#define DMA_CFG_HI_FIELD001_MASK                           (0xFFFFFFFFU << DMA_CFG_HI_FIELD001_SHIFT)                   /* 0xFFFFFFFF */
/* SGR0 */
#define DMA_SGR0_OFFSET                                    (0x48U)
#define DMA_SGR0_SGI_SHIFT                                 (0U)
#define DMA_SGR0_SGI_MASK                                  (0xFFFFFU << DMA_SGR0_SGI_SHIFT)                             /* 0x000FFFFF */
#define DMA_SGR0_SGC_SHIFT                                 (20U)
#define DMA_SGR0_SGC_MASK                                  (0xFFFU << DMA_SGR0_SGC_SHIFT)                               /* 0xFFF00000 */
/* DSR0 */
#define DMA_DSR0_OFFSET                                    (0x50U)
#define DMA_DSR0_DSI_SHIFT                                 (0U)
#define DMA_DSR0_DSI_MASK                                  (0xFFFFFU << DMA_DSR0_DSI_SHIFT)                             /* 0x000FFFFF */
#define DMA_DSR0_DSC_SHIFT                                 (20U)
#define DMA_DSR0_DSC_MASK                                  (0xFFFU << DMA_DSR0_DSC_SHIFT)                               /* 0xFFF00000 */
/* RESERVED0054 */
#define DMA_RESERVED0054_OFFSET                            (0x54U)
/* SAR1 */
#define DMA_SAR1_OFFSET                                    (0x58U)
#define DMA_SAR1_SAR_SHIFT                                 (0U)
#define DMA_SAR1_SAR_MASK                                  (0xFFFFFFFFU << DMA_SAR1_SAR_SHIFT)                          /* 0xFFFFFFFF */
/* DAR1 */
#define DMA_DAR1_OFFSET                                    (0x60U)
#define DMA_DAR1_DAR_SHIFT                                 (0U)
#define DMA_DAR1_DAR_MASK                                  (0xFFFFFFFFU << DMA_DAR1_DAR_SHIFT)                          /* 0xFFFFFFFF */
/* LLP1 */
#define DMA_LLP1_OFFSET                                    (0x68U)
#define DMA_LLP1_LMS_SHIFT                                 (0U)
#define DMA_LLP1_LMS_MASK                                  (0x3U << DMA_LLP1_LMS_SHIFT)                                 /* 0x00000003 */
#define DMA_LLP1_LOC_SHIFT                                 (2U)
#define DMA_LLP1_LOC_MASK                                  (0x3FFFFFFFU << DMA_LLP1_LOC_SHIFT)                          /* 0xFFFFFFFC */
/* CTL1 */
#define DMA_CTL1_OFFSET                                    (0x70U)
#define DMA_CTL1_INT_EN_SHIFT                              (0U)
#define DMA_CTL1_INT_EN_MASK                               (0x1U << DMA_CTL1_INT_EN_SHIFT)                              /* 0x0000000000000001 */
#define DMA_CTL1_DST_TR_WIDTH_SHIFT                        (1U)
#define DMA_CTL1_DST_TR_WIDTH_MASK                         (0x7U << DMA_CTL1_DST_TR_WIDTH_SHIFT)                        /* 0x000000000000000E */
#define DMA_CTL1_SRC_TR_WIDTH_SHIFT                        (4U)
#define DMA_CTL1_SRC_TR_WIDTH_MASK                         (0x7U << DMA_CTL1_SRC_TR_WIDTH_SHIFT)                        /* 0x0000000000000070 */
#define DMA_CTL1_DINC_SHIFT                                (7U)
#define DMA_CTL1_DINC_MASK                                 (0x3U << DMA_CTL1_DINC_SHIFT)                                /* 0x0000000000000180 */
#define DMA_CTL1_SINC_SHIFT                                (9U)
#define DMA_CTL1_SINC_MASK                                 (0x3U << DMA_CTL1_SINC_SHIFT)                                /* 0x0000000000000600 */
#define DMA_CTL1_DEST_MSIZE_SHIFT                          (11U)
#define DMA_CTL1_DEST_MSIZE_MASK                           (0x7U << DMA_CTL1_DEST_MSIZE_SHIFT)                          /* 0x0000000000003800 */
#define DMA_CTL1_SRC_MSIZE_SHIFT                           (14U)
#define DMA_CTL1_SRC_MSIZE_MASK                            (0x7U << DMA_CTL1_SRC_MSIZE_SHIFT)                           /* 0x000000000001C000 */
#define DMA_CTL1_SRC_GATHER_EN_SHIFT                       (17U)
#define DMA_CTL1_SRC_GATHER_EN_MASK                        (0x1U << DMA_CTL1_SRC_GATHER_EN_SHIFT)                       /* 0x0000000000020000 */
#define DMA_CTL1_DST_SCATTER_EN_SHIFT                      (18U)
#define DMA_CTL1_DST_SCATTER_EN_MASK                       (0x1U << DMA_CTL1_DST_SCATTER_EN_SHIFT)                      /* 0x0000000000040000 */
#define DMA_CTL1_TT_FC_SHIFT                               (20U)
#define DMA_CTL1_TT_FC_MASK                                (0x7U << DMA_CTL1_TT_FC_SHIFT)                               /* 0x0000000000700000 */
#define DMA_CTL1_DMS_SHIFT                                 (23U)
#define DMA_CTL1_DMS_MASK                                  (0x3U << DMA_CTL1_DMS_SHIFT)                                 /* 0x0000000001800000 */
#define DMA_CTL1_SMS_SHIFT                                 (25U)
#define DMA_CTL1_SMS_MASK                                  (0x3U << DMA_CTL1_SMS_SHIFT)                                 /* 0x0000000006000000 */
#define DMA_CTL1_LLP_DST_EN_SHIFT                          (27U)
#define DMA_CTL1_LLP_DST_EN_MASK                           (0x1U << DMA_CTL1_LLP_DST_EN_SHIFT)                          /* 0x0000000008000000 */
#define DMA_CTL1_LLP_SRC_EN_SHIFT                          (28U)
#define DMA_CTL1_LLP_SRC_EN_MASK                           (0x1U << DMA_CTL1_LLP_SRC_EN_SHIFT)                          /* 0x0000000010000000 */
#define DMA_CTL1_BLOCK_TS_SHIFT                            (32U)
#define DMA_CTL1_BLOCK_TS_MASK                             (0xFFFU << DMA_CTL1_BLOCK_TS_SHIFT)                          /* 0x00000FFF00000000 */
#define DMA_CTL1_DONE_SHIFT                                (44U)
#define DMA_CTL1_DONE_MASK                                 (0x1U << DMA_CTL1_DONE_SHIFT)                                /* 0x0000100000000000 */
/* SSTAT1 */
#define DMA_SSTAT1_OFFSET                                  (0x78U)
#define DMA_SSTAT1_SSTAT_SHIFT                             (0U)
#define DMA_SSTAT1_SSTAT_MASK                              (0xFFFFFFFFU << DMA_SSTAT1_SSTAT_SHIFT)                      /* 0xFFFFFFFF */
/* DSTAT1 */
#define DMA_DSTAT1_OFFSET                                  (0x80U)
#define DMA_DSTAT1_DSTAT_SHIFT                             (0U)
#define DMA_DSTAT1_DSTAT_MASK                              (0xFFFFFFFFU << DMA_DSTAT1_DSTAT_SHIFT)                      /* 0xFFFFFFFF */
/* SSTATAR1 */
#define DMA_SSTATAR1_OFFSET                                (0x88U)
#define DMA_SSTATAR1_SSTATAR_SHIFT                         (0U)
#define DMA_SSTATAR1_SSTATAR_MASK                          (0xFFFFFFFFU << DMA_SSTATAR1_SSTATAR_SHIFT)                  /* 0xFFFFFFFF */
/* DSTATAR1 */
#define DMA_DSTATAR1_OFFSET                                (0x90U)
#define DMA_DSTATAR1_DSTATAR_SHIFT                         (0U)
#define DMA_DSTATAR1_DSTATAR_MASK                          (0xFFFFFFFFU << DMA_DSTATAR1_DSTATAR_SHIFT)                  /* 0xFFFFFFFF */
/* CFG1 */
#define DMA_CFG1_OFFSET                                    (0x98U)
#define DMA_CFG1_CH_PRIOR_SHIFT                            (5U)
#define DMA_CFG1_CH_PRIOR_MASK                             (0x7U << DMA_CFG1_CH_PRIOR_SHIFT)                            /* 0x00000000000000E0 */
#define DMA_CFG1_CH_SUSP_SHIFT                             (8U)
#define DMA_CFG1_CH_SUSP_MASK                              (0x1U << DMA_CFG1_CH_SUSP_SHIFT)                             /* 0x0000000000000100 */
#define DMA_CFG1_FIFO_EMPTY_SHIFT                          (9U)
#define DMA_CFG1_FIFO_EMPTY_MASK                           (0x1U << DMA_CFG1_FIFO_EMPTY_SHIFT)                          /* 0x0000000000000200 */
#define DMA_CFG1_HS_SEL_DST_SHIFT                          (10U)
#define DMA_CFG1_HS_SEL_DST_MASK                           (0x1U << DMA_CFG1_HS_SEL_DST_SHIFT)                          /* 0x0000000000000400 */
#define DMA_CFG1_HS_SEL_SRC_SHIFT                          (11U)
#define DMA_CFG1_HS_SEL_SRC_MASK                           (0x1U << DMA_CFG1_HS_SEL_SRC_SHIFT)                          /* 0x0000000000000800 */
#define DMA_CFG1_LOCK_CH_L_SHIFT                           (12U)
#define DMA_CFG1_LOCK_CH_L_MASK                            (0x3U << DMA_CFG1_LOCK_CH_L_SHIFT)                           /* 0x0000000000003000 */
#define DMA_CFG1_LOCK_B_L_SHIFT                            (14U)
#define DMA_CFG1_LOCK_B_L_MASK                             (0x3U << DMA_CFG1_LOCK_B_L_SHIFT)                            /* 0x000000000000C000 */
#define DMA_CFG1_LOCK_CH_SHIFT                             (16U)
#define DMA_CFG1_LOCK_CH_MASK                              (0x1U << DMA_CFG1_LOCK_CH_SHIFT)                             /* 0x0000000000010000 */
#define DMA_CFG1_LOCK_B_SHIFT                              (17U)
#define DMA_CFG1_LOCK_B_MASK                               (0x1U << DMA_CFG1_LOCK_B_SHIFT)                              /* 0x0000000000020000 */
#define DMA_CFG1_DST_HS_POL_SHIFT                          (18U)
#define DMA_CFG1_DST_HS_POL_MASK                           (0x1U << DMA_CFG1_DST_HS_POL_SHIFT)                          /* 0x0000000000040000 */
#define DMA_CFG1_SRC_HS_POL_SHIFT                          (19U)
#define DMA_CFG1_SRC_HS_POL_MASK                           (0x1U << DMA_CFG1_SRC_HS_POL_SHIFT)                          /* 0x0000000000080000 */
#define DMA_CFG1_MAX_ABRST_SHIFT                           (20U)
#define DMA_CFG1_MAX_ABRST_MASK                            (0x3FFU << DMA_CFG1_MAX_ABRST_SHIFT)                         /* 0x000000003FF00000 */
#define DMA_CFG1_RELOAD_SRC_SHIFT                          (30U)
#define DMA_CFG1_RELOAD_SRC_MASK                           (0x1U << DMA_CFG1_RELOAD_SRC_SHIFT)                          /* 0x0000000040000000 */
#define DMA_CFG1_RELOAD_DST_SHIFT                          (31U)
#define DMA_CFG1_RELOAD_DST_MASK                           (0x1U << DMA_CFG1_RELOAD_DST_SHIFT)                          /* 0x0000000080000000 */
#define DMA_CFG1_FCMODE_SHIFT                              (32U)
#define DMA_CFG1_FCMODE_MASK                               (0x1U << DMA_CFG1_FCMODE_SHIFT)                              /* 0x0000000100000000 */
#define DMA_CFG1_FIFO_MODE_SHIFT                           (33U)
#define DMA_CFG1_FIFO_MODE_MASK                            (0x1U << DMA_CFG1_FIFO_MODE_SHIFT)                           /* 0x0000000200000000 */
#define DMA_CFG1_PROTCTL_SHIFT                             (34U)
#define DMA_CFG1_PROTCTL_MASK                              (0x7U << DMA_CFG1_PROTCTL_SHIFT)                             /* 0x0000001C00000000 */
#define DMA_CFG1_DS_UPD_EN_SHIFT                           (37U)
#define DMA_CFG1_DS_UPD_EN_MASK                            (0x1U << DMA_CFG1_DS_UPD_EN_SHIFT)                           /* 0x0000002000000000 */
#define DMA_CFG1_SS_UPD_EN_SHIFT                           (38U)
#define DMA_CFG1_SS_UPD_EN_MASK                            (0x1U << DMA_CFG1_SS_UPD_EN_SHIFT)                           /* 0x0000004000000000 */
#define DMA_CFG1_SRC_PER_SHIFT                             (39U)
#define DMA_CFG1_SRC_PER_MASK                              (0xFU << DMA_CFG1_SRC_PER_SHIFT)                             /* 0x0000078000000000 */
#define DMA_CFG1_DEST_PER_SHIFT                            (43U)
#define DMA_CFG1_DEST_PER_MASK                             (0xFU << DMA_CFG1_DEST_PER_SHIFT)                            /* 0x0000780000000000 */
/* SGR1 */
#define DMA_SGR1_OFFSET                                    (0xA0U)
#define DMA_SGR1_SGI_SHIFT                                 (0U)
#define DMA_SGR1_SGI_MASK                                  (0xFFFFFU << DMA_SGR1_SGI_SHIFT)                             /* 0x000FFFFF */
#define DMA_SGR1_SGC_SHIFT                                 (20U)
#define DMA_SGR1_SGC_MASK                                  (0xFFFU << DMA_SGR1_SGC_SHIFT)                               /* 0xFFF00000 */
/* DSR1 */
#define DMA_DSR1_OFFSET                                    (0xA8U)
#define DMA_DSR1_DSI_SHIFT                                 (0U)
#define DMA_DSR1_DSI_MASK                                  (0xFFFFFU << DMA_DSR1_DSI_SHIFT)                             /* 0x000FFFFF */
#define DMA_DSR1_DSC_SHIFT                                 (20U)
#define DMA_DSR1_DSC_MASK                                  (0xFFFU << DMA_DSR1_DSC_SHIFT)                               /* 0xFFF00000 */
/* RESERVED00AC */
#define DMA_RESERVED00AC_OFFSET                            (0xACU)
/* SAR2 */
#define DMA_SAR2_OFFSET                                    (0xB0U)
#define DMA_SAR2_SAR_SHIFT                                 (0U)
#define DMA_SAR2_SAR_MASK                                  (0xFFFFFFFFU << DMA_SAR2_SAR_SHIFT)                          /* 0xFFFFFFFF */
/* DAR2 */
#define DMA_DAR2_OFFSET                                    (0xB8U)
#define DMA_DAR2_DAR_SHIFT                                 (0U)
#define DMA_DAR2_DAR_MASK                                  (0xFFFFFFFFU << DMA_DAR2_DAR_SHIFT)                          /* 0xFFFFFFFF */
/* LLP2 */
#define DMA_LLP2_OFFSET                                    (0xC0U)
#define DMA_LLP2_LMS_SHIFT                                 (0U)
#define DMA_LLP2_LMS_MASK                                  (0x3U << DMA_LLP2_LMS_SHIFT)                                 /* 0x00000003 */
#define DMA_LLP2_LOC_SHIFT                                 (2U)
#define DMA_LLP2_LOC_MASK                                  (0x3FFFFFFFU << DMA_LLP2_LOC_SHIFT)                          /* 0xFFFFFFFC */
/* CTL2 */
#define DMA_CTL2_OFFSET                                    (0xC8U)
#define DMA_CTL2_INT_EN_SHIFT                              (0U)
#define DMA_CTL2_INT_EN_MASK                               (0x1U << DMA_CTL2_INT_EN_SHIFT)                              /* 0x0000000000000001 */
#define DMA_CTL2_DST_TR_WIDTH_SHIFT                        (1U)
#define DMA_CTL2_DST_TR_WIDTH_MASK                         (0x7U << DMA_CTL2_DST_TR_WIDTH_SHIFT)                        /* 0x000000000000000E */
#define DMA_CTL2_SRC_TR_WIDTH_SHIFT                        (4U)
#define DMA_CTL2_SRC_TR_WIDTH_MASK                         (0x7U << DMA_CTL2_SRC_TR_WIDTH_SHIFT)                        /* 0x0000000000000070 */
#define DMA_CTL2_DINC_SHIFT                                (7U)
#define DMA_CTL2_DINC_MASK                                 (0x3U << DMA_CTL2_DINC_SHIFT)                                /* 0x0000000000000180 */
#define DMA_CTL2_SINC_SHIFT                                (9U)
#define DMA_CTL2_SINC_MASK                                 (0x3U << DMA_CTL2_SINC_SHIFT)                                /* 0x0000000000000600 */
#define DMA_CTL2_DEST_MSIZE_SHIFT                          (11U)
#define DMA_CTL2_DEST_MSIZE_MASK                           (0x7U << DMA_CTL2_DEST_MSIZE_SHIFT)                          /* 0x0000000000003800 */
#define DMA_CTL2_SRC_MSIZE_SHIFT                           (14U)
#define DMA_CTL2_SRC_MSIZE_MASK                            (0x7U << DMA_CTL2_SRC_MSIZE_SHIFT)                           /* 0x000000000001C000 */
#define DMA_CTL2_SRC_GATHER_EN_SHIFT                       (17U)
#define DMA_CTL2_SRC_GATHER_EN_MASK                        (0x1U << DMA_CTL2_SRC_GATHER_EN_SHIFT)                       /* 0x0000000000020000 */
#define DMA_CTL2_DST_SCATTER_EN_SHIFT                      (18U)
#define DMA_CTL2_DST_SCATTER_EN_MASK                       (0x1U << DMA_CTL2_DST_SCATTER_EN_SHIFT)                      /* 0x0000000000040000 */
#define DMA_CTL2_TT_FC_SHIFT                               (20U)
#define DMA_CTL2_TT_FC_MASK                                (0x7U << DMA_CTL2_TT_FC_SHIFT)                               /* 0x0000000000700000 */
#define DMA_CTL2_DMS_SHIFT                                 (23U)
#define DMA_CTL2_DMS_MASK                                  (0x3U << DMA_CTL2_DMS_SHIFT)                                 /* 0x0000000001800000 */
#define DMA_CTL2_SMS_SHIFT                                 (25U)
#define DMA_CTL2_SMS_MASK                                  (0x3U << DMA_CTL2_SMS_SHIFT)                                 /* 0x0000000006000000 */
#define DMA_CTL2_LLP_DST_EN_SHIFT                          (27U)
#define DMA_CTL2_LLP_DST_EN_MASK                           (0x1U << DMA_CTL2_LLP_DST_EN_SHIFT)                          /* 0x0000000008000000 */
#define DMA_CTL2_LLP_SRC_EN_SHIFT                          (28U)
#define DMA_CTL2_LLP_SRC_EN_MASK                           (0x1U << DMA_CTL2_LLP_SRC_EN_SHIFT)                          /* 0x0000000010000000 */
#define DMA_CTL2_BLOCK_TS_SHIFT                            (32U)
#define DMA_CTL2_BLOCK_TS_MASK                             (0xFFFU << DMA_CTL2_BLOCK_TS_SHIFT)                          /* 0x00000FFF00000000 */
#define DMA_CTL2_DONE_SHIFT                                (44U)
#define DMA_CTL2_DONE_MASK                                 (0x1U << DMA_CTL2_DONE_SHIFT)                                /* 0x0000100000000000 */
/* SSTAT2 */
#define DMA_SSTAT2_OFFSET                                  (0xD0U)
#define DMA_SSTAT2_SSTAT_SHIFT                             (0U)
#define DMA_SSTAT2_SSTAT_MASK                              (0xFFFFFFFFU << DMA_SSTAT2_SSTAT_SHIFT)                      /* 0xFFFFFFFF */
/* DSTAT2 */
#define DMA_DSTAT2_OFFSET                                  (0xD8U)
#define DMA_DSTAT2_DSTAT_SHIFT                             (0U)
#define DMA_DSTAT2_DSTAT_MASK                              (0xFFFFFFFFU << DMA_DSTAT2_DSTAT_SHIFT)                      /* 0xFFFFFFFF */
/* SSTATAR2 */
#define DMA_SSTATAR2_OFFSET                                (0xE0U)
#define DMA_SSTATAR2_SSTATAR_SHIFT                         (0U)
#define DMA_SSTATAR2_SSTATAR_MASK                          (0xFFFFFFFFU << DMA_SSTATAR2_SSTATAR_SHIFT)                  /* 0xFFFFFFFF */
/* DSTATAR2 */
#define DMA_DSTATAR2_OFFSET                                (0xE8U)
#define DMA_DSTATAR2_DSTATAR_SHIFT                         (0U)
#define DMA_DSTATAR2_DSTATAR_MASK                          (0xFFFFFFFFU << DMA_DSTATAR2_DSTATAR_SHIFT)                  /* 0xFFFFFFFF */
/* CFG2 */
#define DMA_CFG2_OFFSET                                    (0xF0U)
#define DMA_CFG2_CH_PRIOR_SHIFT                            (5U)
#define DMA_CFG2_CH_PRIOR_MASK                             (0x7U << DMA_CFG2_CH_PRIOR_SHIFT)                            /* 0x00000000000000E0 */
#define DMA_CFG2_CH_SUSP_SHIFT                             (8U)
#define DMA_CFG2_CH_SUSP_MASK                              (0x1U << DMA_CFG2_CH_SUSP_SHIFT)                             /* 0x0000000000000100 */
#define DMA_CFG2_FIFO_EMPTY_SHIFT                          (9U)
#define DMA_CFG2_FIFO_EMPTY_MASK                           (0x1U << DMA_CFG2_FIFO_EMPTY_SHIFT)                          /* 0x0000000000000200 */
#define DMA_CFG2_HS_SEL_DST_SHIFT                          (10U)
#define DMA_CFG2_HS_SEL_DST_MASK                           (0x1U << DMA_CFG2_HS_SEL_DST_SHIFT)                          /* 0x0000000000000400 */
#define DMA_CFG2_HS_SEL_SRC_SHIFT                          (11U)
#define DMA_CFG2_HS_SEL_SRC_MASK                           (0x1U << DMA_CFG2_HS_SEL_SRC_SHIFT)                          /* 0x0000000000000800 */
#define DMA_CFG2_LOCK_CH_L_SHIFT                           (12U)
#define DMA_CFG2_LOCK_CH_L_MASK                            (0x3U << DMA_CFG2_LOCK_CH_L_SHIFT)                           /* 0x0000000000003000 */
#define DMA_CFG2_LOCK_B_L_SHIFT                            (14U)
#define DMA_CFG2_LOCK_B_L_MASK                             (0x3U << DMA_CFG2_LOCK_B_L_SHIFT)                            /* 0x000000000000C000 */
#define DMA_CFG2_LOCK_CH_SHIFT                             (16U)
#define DMA_CFG2_LOCK_CH_MASK                              (0x1U << DMA_CFG2_LOCK_CH_SHIFT)                             /* 0x0000000000010000 */
#define DMA_CFG2_LOCK_B_SHIFT                              (17U)
#define DMA_CFG2_LOCK_B_MASK                               (0x1U << DMA_CFG2_LOCK_B_SHIFT)                              /* 0x0000000000020000 */
#define DMA_CFG2_DST_HS_POL_SHIFT                          (18U)
#define DMA_CFG2_DST_HS_POL_MASK                           (0x1U << DMA_CFG2_DST_HS_POL_SHIFT)                          /* 0x0000000000040000 */
#define DMA_CFG2_SRC_HS_POL_SHIFT                          (19U)
#define DMA_CFG2_SRC_HS_POL_MASK                           (0x1U << DMA_CFG2_SRC_HS_POL_SHIFT)                          /* 0x0000000000080000 */
#define DMA_CFG2_MAX_ABRST_SHIFT                           (20U)
#define DMA_CFG2_MAX_ABRST_MASK                            (0x3FFU << DMA_CFG2_MAX_ABRST_SHIFT)                         /* 0x000000003FF00000 */
#define DMA_CFG2_RELOAD_SRC_SHIFT                          (30U)
#define DMA_CFG2_RELOAD_SRC_MASK                           (0x1U << DMA_CFG2_RELOAD_SRC_SHIFT)                          /* 0x0000000040000000 */
#define DMA_CFG2_RELOAD_DST_SHIFT                          (31U)
#define DMA_CFG2_RELOAD_DST_MASK                           (0x1U << DMA_CFG2_RELOAD_DST_SHIFT)                          /* 0x0000000080000000 */
#define DMA_CFG2_FCMODE_SHIFT                              (32U)
#define DMA_CFG2_FCMODE_MASK                               (0x1U << DMA_CFG2_FCMODE_SHIFT)                              /* 0x0000000100000000 */
#define DMA_CFG2_FIFO_MODE_SHIFT                           (33U)
#define DMA_CFG2_FIFO_MODE_MASK                            (0x1U << DMA_CFG2_FIFO_MODE_SHIFT)                           /* 0x0000000200000000 */
#define DMA_CFG2_PROTCTL_SHIFT                             (34U)
#define DMA_CFG2_PROTCTL_MASK                              (0x7U << DMA_CFG2_PROTCTL_SHIFT)                             /* 0x0000001C00000000 */
#define DMA_CFG2_DS_UPD_EN_SHIFT                           (37U)
#define DMA_CFG2_DS_UPD_EN_MASK                            (0x1U << DMA_CFG2_DS_UPD_EN_SHIFT)                           /* 0x0000002000000000 */
#define DMA_CFG2_SS_UPD_EN_SHIFT                           (38U)
#define DMA_CFG2_SS_UPD_EN_MASK                            (0x1U << DMA_CFG2_SS_UPD_EN_SHIFT)                           /* 0x0000004000000000 */
#define DMA_CFG2_SRC_PER_SHIFT                             (39U)
#define DMA_CFG2_SRC_PER_MASK                              (0xFU << DMA_CFG2_SRC_PER_SHIFT)                             /* 0x0000078000000000 */
#define DMA_CFG2_DEST_PER_SHIFT                            (43U)
#define DMA_CFG2_DEST_PER_MASK                             (0xFU << DMA_CFG2_DEST_PER_SHIFT)                            /* 0x0000780000000000 */
/* SGR2 */
#define DMA_SGR2_OFFSET                                    (0xF8U)
#define DMA_SGR2_SGI_SHIFT                                 (0U)
#define DMA_SGR2_SGI_MASK                                  (0xFFFFFU << DMA_SGR2_SGI_SHIFT)                             /* 0x000FFFFF */
#define DMA_SGR2_SGC_SHIFT                                 (20U)
#define DMA_SGR2_SGC_MASK                                  (0xFFFU << DMA_SGR2_SGC_SHIFT)                               /* 0xFFF00000 */
/* DSR2 */
#define DMA_DSR2_OFFSET                                    (0x100U)
#define DMA_DSR2_DSI_SHIFT                                 (0U)
#define DMA_DSR2_DSI_MASK                                  (0xFFFFFU << DMA_DSR2_DSI_SHIFT)                             /* 0x000FFFFF */
#define DMA_DSR2_DSC_SHIFT                                 (20U)
#define DMA_DSR2_DSC_MASK                                  (0xFFFU << DMA_DSR2_DSC_SHIFT)                               /* 0xFFF00000 */
/* RESERVED0104 */
#define DMA_RESERVED0104_OFFSET                            (0x104U)
/* SAR3 */
#define DMA_SAR3_OFFSET                                    (0x108U)
#define DMA_SAR3_SAR_SHIFT                                 (0U)
#define DMA_SAR3_SAR_MASK                                  (0xFFFFFFFFU << DMA_SAR3_SAR_SHIFT)                          /* 0xFFFFFFFF */
/* DAR3 */
#define DMA_DAR3_OFFSET                                    (0x110U)
#define DMA_DAR3_DAR_SHIFT                                 (0U)
#define DMA_DAR3_DAR_MASK                                  (0xFFFFFFFFU << DMA_DAR3_DAR_SHIFT)                          /* 0xFFFFFFFF */
/* LLP3 */
#define DMA_LLP3_OFFSET                                    (0x118U)
#define DMA_LLP3_LMS_SHIFT                                 (0U)
#define DMA_LLP3_LMS_MASK                                  (0x3U << DMA_LLP3_LMS_SHIFT)                                 /* 0x00000003 */
#define DMA_LLP3_LOC_SHIFT                                 (2U)
#define DMA_LLP3_LOC_MASK                                  (0x3FFFFFFFU << DMA_LLP3_LOC_SHIFT)                          /* 0xFFFFFFFC */
/* CTL3 */
#define DMA_CTL3_OFFSET                                    (0x120U)
#define DMA_CTL3_INT_EN_SHIFT                              (0U)
#define DMA_CTL3_INT_EN_MASK                               (0x1U << DMA_CTL3_INT_EN_SHIFT)                              /* 0x0000000000000001 */
#define DMA_CTL3_DST_TR_WIDTH_SHIFT                        (1U)
#define DMA_CTL3_DST_TR_WIDTH_MASK                         (0x7U << DMA_CTL3_DST_TR_WIDTH_SHIFT)                        /* 0x000000000000000E */
#define DMA_CTL3_SRC_TR_WIDTH_SHIFT                        (4U)
#define DMA_CTL3_SRC_TR_WIDTH_MASK                         (0x7U << DMA_CTL3_SRC_TR_WIDTH_SHIFT)                        /* 0x0000000000000070 */
#define DMA_CTL3_DINC_SHIFT                                (7U)
#define DMA_CTL3_DINC_MASK                                 (0x3U << DMA_CTL3_DINC_SHIFT)                                /* 0x0000000000000180 */
#define DMA_CTL3_SINC_SHIFT                                (9U)
#define DMA_CTL3_SINC_MASK                                 (0x3U << DMA_CTL3_SINC_SHIFT)                                /* 0x0000000000000600 */
#define DMA_CTL3_DEST_MSIZE_SHIFT                          (11U)
#define DMA_CTL3_DEST_MSIZE_MASK                           (0x7U << DMA_CTL3_DEST_MSIZE_SHIFT)                          /* 0x0000000000003800 */
#define DMA_CTL3_SRC_MSIZE_SHIFT                           (14U)
#define DMA_CTL3_SRC_MSIZE_MASK                            (0x7U << DMA_CTL3_SRC_MSIZE_SHIFT)                           /* 0x000000000001C000 */
#define DMA_CTL3_SRC_GATHER_EN_SHIFT                       (17U)
#define DMA_CTL3_SRC_GATHER_EN_MASK                        (0x1U << DMA_CTL3_SRC_GATHER_EN_SHIFT)                       /* 0x0000000000020000 */
#define DMA_CTL3_DST_SCATTER_EN_SHIFT                      (18U)
#define DMA_CTL3_DST_SCATTER_EN_MASK                       (0x1U << DMA_CTL3_DST_SCATTER_EN_SHIFT)                      /* 0x0000000000040000 */
#define DMA_CTL3_TT_FC_SHIFT                               (20U)
#define DMA_CTL3_TT_FC_MASK                                (0x7U << DMA_CTL3_TT_FC_SHIFT)                               /* 0x0000000000700000 */
#define DMA_CTL3_DMS_SHIFT                                 (23U)
#define DMA_CTL3_DMS_MASK                                  (0x3U << DMA_CTL3_DMS_SHIFT)                                 /* 0x0000000001800000 */
#define DMA_CTL3_SMS_SHIFT                                 (25U)
#define DMA_CTL3_SMS_MASK                                  (0x3U << DMA_CTL3_SMS_SHIFT)                                 /* 0x0000000006000000 */
#define DMA_CTL3_LLP_DST_EN_SHIFT                          (27U)
#define DMA_CTL3_LLP_DST_EN_MASK                           (0x1U << DMA_CTL3_LLP_DST_EN_SHIFT)                          /* 0x0000000008000000 */
#define DMA_CTL3_LLP_SRC_EN_SHIFT                          (28U)
#define DMA_CTL3_LLP_SRC_EN_MASK                           (0x1U << DMA_CTL3_LLP_SRC_EN_SHIFT)                          /* 0x0000000010000000 */
#define DMA_CTL3_BLOCK_TS_SHIFT                            (32U)
#define DMA_CTL3_BLOCK_TS_MASK                             (0xFFFU << DMA_CTL3_BLOCK_TS_SHIFT)                          /* 0x00000FFF00000000 */
#define DMA_CTL3_DONE_SHIFT                                (44U)
#define DMA_CTL3_DONE_MASK                                 (0x1U << DMA_CTL3_DONE_SHIFT)                                /* 0x0000100000000000 */
/* SSTAT3 */
#define DMA_SSTAT3_OFFSET                                  (0x128U)
#define DMA_SSTAT3_SSTAT_SHIFT                             (0U)
#define DMA_SSTAT3_SSTAT_MASK                              (0xFFFFFFFFU << DMA_SSTAT3_SSTAT_SHIFT)                      /* 0xFFFFFFFF */
/* DSTAT3 */
#define DMA_DSTAT3_OFFSET                                  (0x130U)
#define DMA_DSTAT3_DSTAT_SHIFT                             (0U)
#define DMA_DSTAT3_DSTAT_MASK                              (0xFFFFFFFFU << DMA_DSTAT3_DSTAT_SHIFT)                      /* 0xFFFFFFFF */
/* SSTATAR3 */
#define DMA_SSTATAR3_OFFSET                                (0x138U)
#define DMA_SSTATAR3_SSTATAR_SHIFT                         (0U)
#define DMA_SSTATAR3_SSTATAR_MASK                          (0xFFFFFFFFU << DMA_SSTATAR3_SSTATAR_SHIFT)                  /* 0xFFFFFFFF */
/* DSTATAR3 */
#define DMA_DSTATAR3_OFFSET                                (0x140U)
#define DMA_DSTATAR3_DSTATAR_SHIFT                         (0U)
#define DMA_DSTATAR3_DSTATAR_MASK                          (0xFFFFFFFFU << DMA_DSTATAR3_DSTATAR_SHIFT)                  /* 0xFFFFFFFF */
/* CFG3 */
#define DMA_CFG3_OFFSET                                    (0x148U)
#define DMA_CFG3_CH_PRIOR_SHIFT                            (5U)
#define DMA_CFG3_CH_PRIOR_MASK                             (0x7U << DMA_CFG3_CH_PRIOR_SHIFT)                            /* 0x00000000000000E0 */
#define DMA_CFG3_CH_SUSP_SHIFT                             (8U)
#define DMA_CFG3_CH_SUSP_MASK                              (0x1U << DMA_CFG3_CH_SUSP_SHIFT)                             /* 0x0000000000000100 */
#define DMA_CFG3_FIFO_EMPTY_SHIFT                          (9U)
#define DMA_CFG3_FIFO_EMPTY_MASK                           (0x1U << DMA_CFG3_FIFO_EMPTY_SHIFT)                          /* 0x0000000000000200 */
#define DMA_CFG3_HS_SEL_DST_SHIFT                          (10U)
#define DMA_CFG3_HS_SEL_DST_MASK                           (0x1U << DMA_CFG3_HS_SEL_DST_SHIFT)                          /* 0x0000000000000400 */
#define DMA_CFG3_HS_SEL_SRC_SHIFT                          (11U)
#define DMA_CFG3_HS_SEL_SRC_MASK                           (0x1U << DMA_CFG3_HS_SEL_SRC_SHIFT)                          /* 0x0000000000000800 */
#define DMA_CFG3_LOCK_CH_L_SHIFT                           (12U)
#define DMA_CFG3_LOCK_CH_L_MASK                            (0x3U << DMA_CFG3_LOCK_CH_L_SHIFT)                           /* 0x0000000000003000 */
#define DMA_CFG3_LOCK_B_L_SHIFT                            (14U)
#define DMA_CFG3_LOCK_B_L_MASK                             (0x3U << DMA_CFG3_LOCK_B_L_SHIFT)                            /* 0x000000000000C000 */
#define DMA_CFG3_LOCK_CH_SHIFT                             (16U)
#define DMA_CFG3_LOCK_CH_MASK                              (0x1U << DMA_CFG3_LOCK_CH_SHIFT)                             /* 0x0000000000010000 */
#define DMA_CFG3_LOCK_B_SHIFT                              (17U)
#define DMA_CFG3_LOCK_B_MASK                               (0x1U << DMA_CFG3_LOCK_B_SHIFT)                              /* 0x0000000000020000 */
#define DMA_CFG3_DST_HS_POL_SHIFT                          (18U)
#define DMA_CFG3_DST_HS_POL_MASK                           (0x1U << DMA_CFG3_DST_HS_POL_SHIFT)                          /* 0x0000000000040000 */
#define DMA_CFG3_SRC_HS_POL_SHIFT                          (19U)
#define DMA_CFG3_SRC_HS_POL_MASK                           (0x1U << DMA_CFG3_SRC_HS_POL_SHIFT)                          /* 0x0000000000080000 */
#define DMA_CFG3_MAX_ABRST_SHIFT                           (20U)
#define DMA_CFG3_MAX_ABRST_MASK                            (0x3FFU << DMA_CFG3_MAX_ABRST_SHIFT)                         /* 0x000000003FF00000 */
#define DMA_CFG3_RELOAD_SRC_SHIFT                          (30U)
#define DMA_CFG3_RELOAD_SRC_MASK                           (0x1U << DMA_CFG3_RELOAD_SRC_SHIFT)                          /* 0x0000000040000000 */
#define DMA_CFG3_RELOAD_DST_SHIFT                          (31U)
#define DMA_CFG3_RELOAD_DST_MASK                           (0x1U << DMA_CFG3_RELOAD_DST_SHIFT)                          /* 0x0000000080000000 */
#define DMA_CFG3_FCMODE_SHIFT                              (32U)
#define DMA_CFG3_FCMODE_MASK                               (0x1U << DMA_CFG3_FCMODE_SHIFT)                              /* 0x0000000100000000 */
#define DMA_CFG3_FIFO_MODE_SHIFT                           (33U)
#define DMA_CFG3_FIFO_MODE_MASK                            (0x1U << DMA_CFG3_FIFO_MODE_SHIFT)                           /* 0x0000000200000000 */
#define DMA_CFG3_PROTCTL_SHIFT                             (34U)
#define DMA_CFG3_PROTCTL_MASK                              (0x7U << DMA_CFG3_PROTCTL_SHIFT)                             /* 0x0000001C00000000 */
#define DMA_CFG3_DS_UPD_EN_SHIFT                           (37U)
#define DMA_CFG3_DS_UPD_EN_MASK                            (0x1U << DMA_CFG3_DS_UPD_EN_SHIFT)                           /* 0x0000002000000000 */
#define DMA_CFG3_SS_UPD_EN_SHIFT                           (38U)
#define DMA_CFG3_SS_UPD_EN_MASK                            (0x1U << DMA_CFG3_SS_UPD_EN_SHIFT)                           /* 0x0000004000000000 */
#define DMA_CFG3_SRC_PER_SHIFT                             (39U)
#define DMA_CFG3_SRC_PER_MASK                              (0xFU << DMA_CFG3_SRC_PER_SHIFT)                             /* 0x0000078000000000 */
#define DMA_CFG3_DEST_PER_SHIFT                            (43U)
#define DMA_CFG3_DEST_PER_MASK                             (0xFU << DMA_CFG3_DEST_PER_SHIFT)                            /* 0x0000780000000000 */
/* SGR3 */
#define DMA_SGR3_OFFSET                                    (0x150U)
#define DMA_SGR3_SGI_SHIFT                                 (0U)
#define DMA_SGR3_SGI_MASK                                  (0xFFFFFU << DMA_SGR3_SGI_SHIFT)                             /* 0x000FFFFF */
#define DMA_SGR3_SGC_SHIFT                                 (20U)
#define DMA_SGR3_SGC_MASK                                  (0xFFFU << DMA_SGR3_SGC_SHIFT)                               /* 0xFFF00000 */
/* DSR3 */
#define DMA_DSR3_OFFSET                                    (0x158U)
#define DMA_DSR3_DSI_SHIFT                                 (0U)
#define DMA_DSR3_DSI_MASK                                  (0xFFFFFU << DMA_DSR3_DSI_SHIFT)                             /* 0x000FFFFF */
#define DMA_DSR3_DSC_SHIFT                                 (20U)
#define DMA_DSR3_DSC_MASK                                  (0xFFFU << DMA_DSR3_DSC_SHIFT)                               /* 0xFFF00000 */
/* RESERVED015C */
#define DMA_RESERVED015C_OFFSET                            (0x15CU)
/* SAR4 */
#define DMA_SAR4_OFFSET                                    (0x160U)
#define DMA_SAR4_SAR_SHIFT                                 (0U)
#define DMA_SAR4_SAR_MASK                                  (0xFFFFFFFFU << DMA_SAR4_SAR_SHIFT)                          /* 0xFFFFFFFF */
/* DAR4 */
#define DMA_DAR4_OFFSET                                    (0x168U)
#define DMA_DAR4_DAR_SHIFT                                 (0U)
#define DMA_DAR4_DAR_MASK                                  (0xFFFFFFFFU << DMA_DAR4_DAR_SHIFT)                          /* 0xFFFFFFFF */
/* LLP4 */
#define DMA_LLP4_OFFSET                                    (0x170U)
#define DMA_LLP4_LMS_SHIFT                                 (0U)
#define DMA_LLP4_LMS_MASK                                  (0x3U << DMA_LLP4_LMS_SHIFT)                                 /* 0x00000003 */
#define DMA_LLP4_LOC_SHIFT                                 (2U)
#define DMA_LLP4_LOC_MASK                                  (0x3FFFFFFFU << DMA_LLP4_LOC_SHIFT)                          /* 0xFFFFFFFC */
/* CTL4 */
#define DMA_CTL4_OFFSET                                    (0x178U)
#define DMA_CTL4_INT_EN_SHIFT                              (0U)
#define DMA_CTL4_INT_EN_MASK                               (0x1U << DMA_CTL4_INT_EN_SHIFT)                              /* 0x0000000000000001 */
#define DMA_CTL4_DST_TR_WIDTH_SHIFT                        (1U)
#define DMA_CTL4_DST_TR_WIDTH_MASK                         (0x7U << DMA_CTL4_DST_TR_WIDTH_SHIFT)                        /* 0x000000000000000E */
#define DMA_CTL4_SRC_TR_WIDTH_SHIFT                        (4U)
#define DMA_CTL4_SRC_TR_WIDTH_MASK                         (0x7U << DMA_CTL4_SRC_TR_WIDTH_SHIFT)                        /* 0x0000000000000070 */
#define DMA_CTL4_DINC_SHIFT                                (7U)
#define DMA_CTL4_DINC_MASK                                 (0x3U << DMA_CTL4_DINC_SHIFT)                                /* 0x0000000000000180 */
#define DMA_CTL4_SINC_SHIFT                                (9U)
#define DMA_CTL4_SINC_MASK                                 (0x3U << DMA_CTL4_SINC_SHIFT)                                /* 0x0000000000000600 */
#define DMA_CTL4_DEST_MSIZE_SHIFT                          (11U)
#define DMA_CTL4_DEST_MSIZE_MASK                           (0x7U << DMA_CTL4_DEST_MSIZE_SHIFT)                          /* 0x0000000000003800 */
#define DMA_CTL4_SRC_MSIZE_SHIFT                           (14U)
#define DMA_CTL4_SRC_MSIZE_MASK                            (0x7U << DMA_CTL4_SRC_MSIZE_SHIFT)                           /* 0x000000000001C000 */
#define DMA_CTL4_SRC_GATHER_EN_SHIFT                       (17U)
#define DMA_CTL4_SRC_GATHER_EN_MASK                        (0x1U << DMA_CTL4_SRC_GATHER_EN_SHIFT)                       /* 0x0000000000020000 */
#define DMA_CTL4_DST_SCATTER_EN_SHIFT                      (18U)
#define DMA_CTL4_DST_SCATTER_EN_MASK                       (0x1U << DMA_CTL4_DST_SCATTER_EN_SHIFT)                      /* 0x0000000000040000 */
#define DMA_CTL4_TT_FC_SHIFT                               (20U)
#define DMA_CTL4_TT_FC_MASK                                (0x7U << DMA_CTL4_TT_FC_SHIFT)                               /* 0x0000000000700000 */
#define DMA_CTL4_DMS_SHIFT                                 (23U)
#define DMA_CTL4_DMS_MASK                                  (0x3U << DMA_CTL4_DMS_SHIFT)                                 /* 0x0000000001800000 */
#define DMA_CTL4_SMS_SHIFT                                 (25U)
#define DMA_CTL4_SMS_MASK                                  (0x3U << DMA_CTL4_SMS_SHIFT)                                 /* 0x0000000006000000 */
#define DMA_CTL4_LLP_DST_EN_SHIFT                          (27U)
#define DMA_CTL4_LLP_DST_EN_MASK                           (0x1U << DMA_CTL4_LLP_DST_EN_SHIFT)                          /* 0x0000000008000000 */
#define DMA_CTL4_LLP_SRC_EN_SHIFT                          (28U)
#define DMA_CTL4_LLP_SRC_EN_MASK                           (0x1U << DMA_CTL4_LLP_SRC_EN_SHIFT)                          /* 0x0000000010000000 */
#define DMA_CTL4_BLOCK_TS_SHIFT                            (32U)
#define DMA_CTL4_BLOCK_TS_MASK                             (0xFFFU << DMA_CTL4_BLOCK_TS_SHIFT)                          /* 0x00000FFF00000000 */
#define DMA_CTL4_DONE_SHIFT                                (44U)
#define DMA_CTL4_DONE_MASK                                 (0x1U << DMA_CTL4_DONE_SHIFT)                                /* 0x0000100000000000 */
/* SSTAT4 */
#define DMA_SSTAT4_OFFSET                                  (0x180U)
#define DMA_SSTAT4_SSTAT_SHIFT                             (0U)
#define DMA_SSTAT4_SSTAT_MASK                              (0xFFFFFFFFU << DMA_SSTAT4_SSTAT_SHIFT)                      /* 0xFFFFFFFF */
/* DSTAT4 */
#define DMA_DSTAT4_OFFSET                                  (0x188U)
#define DMA_DSTAT4_DSTAT_SHIFT                             (0U)
#define DMA_DSTAT4_DSTAT_MASK                              (0xFFFFFFFFU << DMA_DSTAT4_DSTAT_SHIFT)                      /* 0xFFFFFFFF */
/* SSTATAR4 */
#define DMA_SSTATAR4_OFFSET                                (0x190U)
#define DMA_SSTATAR4_SSTATAR_SHIFT                         (0U)
#define DMA_SSTATAR4_SSTATAR_MASK                          (0xFFFFFFFFU << DMA_SSTATAR4_SSTATAR_SHIFT)                  /* 0xFFFFFFFF */
/* DSTATAR4 */
#define DMA_DSTATAR4_OFFSET                                (0x198U)
#define DMA_DSTATAR4_DSTATAR_SHIFT                         (0U)
#define DMA_DSTATAR4_DSTATAR_MASK                          (0xFFFFFFFFU << DMA_DSTATAR4_DSTATAR_SHIFT)                  /* 0xFFFFFFFF */
/* CFG4 */
#define DMA_CFG4_OFFSET                                    (0x1A0U)
#define DMA_CFG4_CH_PRIOR_SHIFT                            (5U)
#define DMA_CFG4_CH_PRIOR_MASK                             (0x7U << DMA_CFG4_CH_PRIOR_SHIFT)                            /* 0x00000000000000E0 */
#define DMA_CFG4_CH_SUSP_SHIFT                             (8U)
#define DMA_CFG4_CH_SUSP_MASK                              (0x1U << DMA_CFG4_CH_SUSP_SHIFT)                             /* 0x0000000000000100 */
#define DMA_CFG4_FIFO_EMPTY_SHIFT                          (9U)
#define DMA_CFG4_FIFO_EMPTY_MASK                           (0x1U << DMA_CFG4_FIFO_EMPTY_SHIFT)                          /* 0x0000000000000200 */
#define DMA_CFG4_HS_SEL_DST_SHIFT                          (10U)
#define DMA_CFG4_HS_SEL_DST_MASK                           (0x1U << DMA_CFG4_HS_SEL_DST_SHIFT)                          /* 0x0000000000000400 */
#define DMA_CFG4_HS_SEL_SRC_SHIFT                          (11U)
#define DMA_CFG4_HS_SEL_SRC_MASK                           (0x1U << DMA_CFG4_HS_SEL_SRC_SHIFT)                          /* 0x0000000000000800 */
#define DMA_CFG4_LOCK_CH_L_SHIFT                           (12U)
#define DMA_CFG4_LOCK_CH_L_MASK                            (0x3U << DMA_CFG4_LOCK_CH_L_SHIFT)                           /* 0x0000000000003000 */
#define DMA_CFG4_LOCK_B_L_SHIFT                            (14U)
#define DMA_CFG4_LOCK_B_L_MASK                             (0x3U << DMA_CFG4_LOCK_B_L_SHIFT)                            /* 0x000000000000C000 */
#define DMA_CFG4_LOCK_CH_SHIFT                             (16U)
#define DMA_CFG4_LOCK_CH_MASK                              (0x1U << DMA_CFG4_LOCK_CH_SHIFT)                             /* 0x0000000000010000 */
#define DMA_CFG4_LOCK_B_SHIFT                              (17U)
#define DMA_CFG4_LOCK_B_MASK                               (0x1U << DMA_CFG4_LOCK_B_SHIFT)                              /* 0x0000000000020000 */
#define DMA_CFG4_DST_HS_POL_SHIFT                          (18U)
#define DMA_CFG4_DST_HS_POL_MASK                           (0x1U << DMA_CFG4_DST_HS_POL_SHIFT)                          /* 0x0000000000040000 */
#define DMA_CFG4_SRC_HS_POL_SHIFT                          (19U)
#define DMA_CFG4_SRC_HS_POL_MASK                           (0x1U << DMA_CFG4_SRC_HS_POL_SHIFT)                          /* 0x0000000000080000 */
#define DMA_CFG4_MAX_ABRST_SHIFT                           (20U)
#define DMA_CFG4_MAX_ABRST_MASK                            (0x3FFU << DMA_CFG4_MAX_ABRST_SHIFT)                         /* 0x000000003FF00000 */
#define DMA_CFG4_RELOAD_SRC_SHIFT                          (30U)
#define DMA_CFG4_RELOAD_SRC_MASK                           (0x1U << DMA_CFG4_RELOAD_SRC_SHIFT)                          /* 0x0000000040000000 */
#define DMA_CFG4_RELOAD_DST_SHIFT                          (31U)
#define DMA_CFG4_RELOAD_DST_MASK                           (0x1U << DMA_CFG4_RELOAD_DST_SHIFT)                          /* 0x0000000080000000 */
#define DMA_CFG4_FCMODE_SHIFT                              (32U)
#define DMA_CFG4_FCMODE_MASK                               (0x1U << DMA_CFG4_FCMODE_SHIFT)                              /* 0x0000000100000000 */
#define DMA_CFG4_FIFO_MODE_SHIFT                           (33U)
#define DMA_CFG4_FIFO_MODE_MASK                            (0x1U << DMA_CFG4_FIFO_MODE_SHIFT)                           /* 0x0000000200000000 */
#define DMA_CFG4_PROTCTL_SHIFT                             (34U)
#define DMA_CFG4_PROTCTL_MASK                              (0x7U << DMA_CFG4_PROTCTL_SHIFT)                             /* 0x0000001C00000000 */
#define DMA_CFG4_DS_UPD_EN_SHIFT                           (37U)
#define DMA_CFG4_DS_UPD_EN_MASK                            (0x1U << DMA_CFG4_DS_UPD_EN_SHIFT)                           /* 0x0000002000000000 */
#define DMA_CFG4_SS_UPD_EN_SHIFT                           (38U)
#define DMA_CFG4_SS_UPD_EN_MASK                            (0x1U << DMA_CFG4_SS_UPD_EN_SHIFT)                           /* 0x0000004000000000 */
#define DMA_CFG4_SRC_PER_SHIFT                             (39U)
#define DMA_CFG4_SRC_PER_MASK                              (0xFU << DMA_CFG4_SRC_PER_SHIFT)                             /* 0x0000078000000000 */
#define DMA_CFG4_DEST_PER_SHIFT                            (43U)
#define DMA_CFG4_DEST_PER_MASK                             (0xFU << DMA_CFG4_DEST_PER_SHIFT)                            /* 0x0000780000000000 */
/* SGR4 */
#define DMA_SGR4_OFFSET                                    (0x1A8U)
#define DMA_SGR4_SGI_SHIFT                                 (0U)
#define DMA_SGR4_SGI_MASK                                  (0xFFFFFU << DMA_SGR4_SGI_SHIFT)                             /* 0x000FFFFF */
#define DMA_SGR4_SGC_SHIFT                                 (20U)
#define DMA_SGR4_SGC_MASK                                  (0xFFFU << DMA_SGR4_SGC_SHIFT)                               /* 0xFFF00000 */
/* DSR4 */
#define DMA_DSR4_OFFSET                                    (0x1B0U)
#define DMA_DSR4_DSI_SHIFT                                 (0U)
#define DMA_DSR4_DSI_MASK                                  (0xFFFFFU << DMA_DSR4_DSI_SHIFT)                             /* 0x000FFFFF */
#define DMA_DSR4_DSC_SHIFT                                 (20U)
#define DMA_DSR4_DSC_MASK                                  (0xFFFU << DMA_DSR4_DSC_SHIFT)                               /* 0xFFF00000 */
/* RESERVED01B4 */
#define DMA_RESERVED01B4_OFFSET                            (0x1B4U)
/* SAR5 */
#define DMA_SAR5_OFFSET                                    (0x1B8U)
#define DMA_SAR5_SAR_SHIFT                                 (0U)
#define DMA_SAR5_SAR_MASK                                  (0xFFFFFFFFU << DMA_SAR5_SAR_SHIFT)                          /* 0xFFFFFFFF */
/* DAR5 */
#define DMA_DAR5_OFFSET                                    (0x1C0U)
#define DMA_DAR5_DAR_SHIFT                                 (0U)
#define DMA_DAR5_DAR_MASK                                  (0xFFFFFFFFU << DMA_DAR5_DAR_SHIFT)                          /* 0xFFFFFFFF */
/* LLP5 */
#define DMA_LLP5_OFFSET                                    (0x1C8U)
#define DMA_LLP5_LMS_SHIFT                                 (0U)
#define DMA_LLP5_LMS_MASK                                  (0x3U << DMA_LLP5_LMS_SHIFT)                                 /* 0x00000003 */
#define DMA_LLP5_LOC_SHIFT                                 (2U)
#define DMA_LLP5_LOC_MASK                                  (0x3FFFFFFFU << DMA_LLP5_LOC_SHIFT)                          /* 0xFFFFFFFC */
/* CTL5 */
#define DMA_CTL5_OFFSET                                    (0x1D0U)
#define DMA_CTL5_INT_EN_SHIFT                              (0U)
#define DMA_CTL5_INT_EN_MASK                               (0x1U << DMA_CTL5_INT_EN_SHIFT)                              /* 0x0000000000000001 */
#define DMA_CTL5_DST_TR_WIDTH_SHIFT                        (1U)
#define DMA_CTL5_DST_TR_WIDTH_MASK                         (0x7U << DMA_CTL5_DST_TR_WIDTH_SHIFT)                        /* 0x000000000000000E */
#define DMA_CTL5_SRC_TR_WIDTH_SHIFT                        (4U)
#define DMA_CTL5_SRC_TR_WIDTH_MASK                         (0x7U << DMA_CTL5_SRC_TR_WIDTH_SHIFT)                        /* 0x0000000000000070 */
#define DMA_CTL5_DINC_SHIFT                                (7U)
#define DMA_CTL5_DINC_MASK                                 (0x3U << DMA_CTL5_DINC_SHIFT)                                /* 0x0000000000000180 */
#define DMA_CTL5_SINC_SHIFT                                (9U)
#define DMA_CTL5_SINC_MASK                                 (0x3U << DMA_CTL5_SINC_SHIFT)                                /* 0x0000000000000600 */
#define DMA_CTL5_DEST_MSIZE_SHIFT                          (11U)
#define DMA_CTL5_DEST_MSIZE_MASK                           (0x7U << DMA_CTL5_DEST_MSIZE_SHIFT)                          /* 0x0000000000003800 */
#define DMA_CTL5_SRC_MSIZE_SHIFT                           (14U)
#define DMA_CTL5_SRC_MSIZE_MASK                            (0x7U << DMA_CTL5_SRC_MSIZE_SHIFT)                           /* 0x000000000001C000 */
#define DMA_CTL5_SRC_GATHER_EN_SHIFT                       (17U)
#define DMA_CTL5_SRC_GATHER_EN_MASK                        (0x1U << DMA_CTL5_SRC_GATHER_EN_SHIFT)                       /* 0x0000000000020000 */
#define DMA_CTL5_DST_SCATTER_EN_SHIFT                      (18U)
#define DMA_CTL5_DST_SCATTER_EN_MASK                       (0x1U << DMA_CTL5_DST_SCATTER_EN_SHIFT)                      /* 0x0000000000040000 */
#define DMA_CTL5_TT_FC_SHIFT                               (20U)
#define DMA_CTL5_TT_FC_MASK                                (0x7U << DMA_CTL5_TT_FC_SHIFT)                               /* 0x0000000000700000 */
#define DMA_CTL5_DMS_SHIFT                                 (23U)
#define DMA_CTL5_DMS_MASK                                  (0x3U << DMA_CTL5_DMS_SHIFT)                                 /* 0x0000000001800000 */
#define DMA_CTL5_SMS_SHIFT                                 (25U)
#define DMA_CTL5_SMS_MASK                                  (0x3U << DMA_CTL5_SMS_SHIFT)                                 /* 0x0000000006000000 */
#define DMA_CTL5_LLP_DST_EN_SHIFT                          (27U)
#define DMA_CTL5_LLP_DST_EN_MASK                           (0x1U << DMA_CTL5_LLP_DST_EN_SHIFT)                          /* 0x0000000008000000 */
#define DMA_CTL5_LLP_SRC_EN_SHIFT                          (28U)
#define DMA_CTL5_LLP_SRC_EN_MASK                           (0x1U << DMA_CTL5_LLP_SRC_EN_SHIFT)                          /* 0x0000000010000000 */
#define DMA_CTL5_BLOCK_TS_SHIFT                            (32U)
#define DMA_CTL5_BLOCK_TS_MASK                             (0xFFFU << DMA_CTL5_BLOCK_TS_SHIFT)                          /* 0x00000FFF00000000 */
#define DMA_CTL5_DONE_SHIFT                                (44U)
#define DMA_CTL5_DONE_MASK                                 (0x1U << DMA_CTL5_DONE_SHIFT)                                /* 0x0000100000000000 */
/* SSTAT5 */
#define DMA_SSTAT5_OFFSET                                  (0x1D8U)
#define DMA_SSTAT5_SSTAT_SHIFT                             (0U)
#define DMA_SSTAT5_SSTAT_MASK                              (0xFFFFFFFFU << DMA_SSTAT5_SSTAT_SHIFT)                      /* 0xFFFFFFFF */
/* DSTAT5 */
#define DMA_DSTAT5_OFFSET                                  (0x1E0U)
#define DMA_DSTAT5_DSTAT_SHIFT                             (0U)
#define DMA_DSTAT5_DSTAT_MASK                              (0xFFFFFFFFU << DMA_DSTAT5_DSTAT_SHIFT)                      /* 0xFFFFFFFF */
/* SSTATAR5 */
#define DMA_SSTATAR5_OFFSET                                (0x1E8U)
#define DMA_SSTATAR5_SSTATAR_SHIFT                         (0U)
#define DMA_SSTATAR5_SSTATAR_MASK                          (0xFFFFFFFFU << DMA_SSTATAR5_SSTATAR_SHIFT)                  /* 0xFFFFFFFF */
/* DSTATAR5 */
#define DMA_DSTATAR5_OFFSET                                (0x1F0U)
#define DMA_DSTATAR5_DSTATAR_SHIFT                         (0U)
#define DMA_DSTATAR5_DSTATAR_MASK                          (0xFFFFFFFFU << DMA_DSTATAR5_DSTATAR_SHIFT)                  /* 0xFFFFFFFF */
/* CFG5 */
#define DMA_CFG5_OFFSET                                    (0x1F8U)
#define DMA_CFG5_CH_PRIOR_SHIFT                            (5U)
#define DMA_CFG5_CH_PRIOR_MASK                             (0x7U << DMA_CFG5_CH_PRIOR_SHIFT)                            /* 0x00000000000000E0 */
#define DMA_CFG5_CH_SUSP_SHIFT                             (8U)
#define DMA_CFG5_CH_SUSP_MASK                              (0x1U << DMA_CFG5_CH_SUSP_SHIFT)                             /* 0x0000000000000100 */
#define DMA_CFG5_FIFO_EMPTY_SHIFT                          (9U)
#define DMA_CFG5_FIFO_EMPTY_MASK                           (0x1U << DMA_CFG5_FIFO_EMPTY_SHIFT)                          /* 0x0000000000000200 */
#define DMA_CFG5_HS_SEL_DST_SHIFT                          (10U)
#define DMA_CFG5_HS_SEL_DST_MASK                           (0x1U << DMA_CFG5_HS_SEL_DST_SHIFT)                          /* 0x0000000000000400 */
#define DMA_CFG5_HS_SEL_SRC_SHIFT                          (11U)
#define DMA_CFG5_HS_SEL_SRC_MASK                           (0x1U << DMA_CFG5_HS_SEL_SRC_SHIFT)                          /* 0x0000000000000800 */
#define DMA_CFG5_LOCK_CH_L_SHIFT                           (12U)
#define DMA_CFG5_LOCK_CH_L_MASK                            (0x3U << DMA_CFG5_LOCK_CH_L_SHIFT)                           /* 0x0000000000003000 */
#define DMA_CFG5_LOCK_B_L_SHIFT                            (14U)
#define DMA_CFG5_LOCK_B_L_MASK                             (0x3U << DMA_CFG5_LOCK_B_L_SHIFT)                            /* 0x000000000000C000 */
#define DMA_CFG5_LOCK_CH_SHIFT                             (16U)
#define DMA_CFG5_LOCK_CH_MASK                              (0x1U << DMA_CFG5_LOCK_CH_SHIFT)                             /* 0x0000000000010000 */
#define DMA_CFG5_LOCK_B_SHIFT                              (17U)
#define DMA_CFG5_LOCK_B_MASK                               (0x1U << DMA_CFG5_LOCK_B_SHIFT)                              /* 0x0000000000020000 */
#define DMA_CFG5_DST_HS_POL_SHIFT                          (18U)
#define DMA_CFG5_DST_HS_POL_MASK                           (0x1U << DMA_CFG5_DST_HS_POL_SHIFT)                          /* 0x0000000000040000 */
#define DMA_CFG5_SRC_HS_POL_SHIFT                          (19U)
#define DMA_CFG5_SRC_HS_POL_MASK                           (0x1U << DMA_CFG5_SRC_HS_POL_SHIFT)                          /* 0x0000000000080000 */
#define DMA_CFG5_MAX_ABRST_SHIFT                           (20U)
#define DMA_CFG5_MAX_ABRST_MASK                            (0x3FFU << DMA_CFG5_MAX_ABRST_SHIFT)                         /* 0x000000003FF00000 */
#define DMA_CFG5_RELOAD_SRC_SHIFT                          (30U)
#define DMA_CFG5_RELOAD_SRC_MASK                           (0x1U << DMA_CFG5_RELOAD_SRC_SHIFT)                          /* 0x0000000040000000 */
#define DMA_CFG5_RELOAD_DST_SHIFT                          (31U)
#define DMA_CFG5_RELOAD_DST_MASK                           (0x1U << DMA_CFG5_RELOAD_DST_SHIFT)                          /* 0x0000000080000000 */
#define DMA_CFG5_FCMODE_SHIFT                              (32U)
#define DMA_CFG5_FCMODE_MASK                               (0x1U << DMA_CFG5_FCMODE_SHIFT)                              /* 0x0000000100000000 */
#define DMA_CFG5_FIFO_MODE_SHIFT                           (33U)
#define DMA_CFG5_FIFO_MODE_MASK                            (0x1U << DMA_CFG5_FIFO_MODE_SHIFT)                           /* 0x0000000200000000 */
#define DMA_CFG5_PROTCTL_SHIFT                             (34U)
#define DMA_CFG5_PROTCTL_MASK                              (0x7U << DMA_CFG5_PROTCTL_SHIFT)                             /* 0x0000001C00000000 */
#define DMA_CFG5_DS_UPD_EN_SHIFT                           (37U)
#define DMA_CFG5_DS_UPD_EN_MASK                            (0x1U << DMA_CFG5_DS_UPD_EN_SHIFT)                           /* 0x0000002000000000 */
#define DMA_CFG5_SS_UPD_EN_SHIFT                           (38U)
#define DMA_CFG5_SS_UPD_EN_MASK                            (0x1U << DMA_CFG5_SS_UPD_EN_SHIFT)                           /* 0x0000004000000000 */
#define DMA_CFG5_SRC_PER_SHIFT                             (39U)
#define DMA_CFG5_SRC_PER_MASK                              (0xFU << DMA_CFG5_SRC_PER_SHIFT)                             /* 0x0000078000000000 */
#define DMA_CFG5_DEST_PER_SHIFT                            (43U)
#define DMA_CFG5_DEST_PER_MASK                             (0xFU << DMA_CFG5_DEST_PER_SHIFT)                            /* 0x0000780000000000 */
/* SGR5 */
#define DMA_SGR5_OFFSET                                    (0x200U)
#define DMA_SGR5_SGI_SHIFT                                 (0U)
#define DMA_SGR5_SGI_MASK                                  (0xFFFFFU << DMA_SGR5_SGI_SHIFT)                             /* 0x000FFFFF */
#define DMA_SGR5_SGC_SHIFT                                 (20U)
#define DMA_SGR5_SGC_MASK                                  (0xFFFU << DMA_SGR5_SGC_SHIFT)                               /* 0xFFF00000 */
/* DSR5 */
#define DMA_DSR5_OFFSET                                    (0x208U)
#define DMA_DSR5_DSI_SHIFT                                 (0U)
#define DMA_DSR5_DSI_MASK                                  (0xFFFFFU << DMA_DSR5_DSI_SHIFT)                             /* 0x000FFFFF */
#define DMA_DSR5_DSC_SHIFT                                 (20U)
#define DMA_DSR5_DSC_MASK                                  (0xFFFU << DMA_DSR5_DSC_SHIFT)                               /* 0xFFF00000 */
/* RESERVED020C */
#define DMA_RESERVED020C_OFFSET                            (0x20CU)
/* RAWTFR */
#define DMA_RAWTFR_OFFSET                                  (0x2C0U)
#define DMA_RAWTFR_RAW_SHIFT                               (0U)
#define DMA_RAWTFR_RAW_MASK                                (0x3FU << DMA_RAWTFR_RAW_SHIFT)                              /* 0x0000003F */
/* RAWBLOCK */
#define DMA_RAWBLOCK_OFFSET                                (0x2C8U)
#define DMA_RAWBLOCK_RAW_SHIFT                             (0U)
#define DMA_RAWBLOCK_RAW_MASK                              (0x3FU << DMA_RAWBLOCK_RAW_SHIFT)                            /* 0x0000003F */
/* RAWSRCTRAN */
#define DMA_RAWSRCTRAN_OFFSET                              (0x2D0U)
#define DMA_RAWSRCTRAN_RAW_SHIFT                           (0U)
#define DMA_RAWSRCTRAN_RAW_MASK                            (0x3FU << DMA_RAWSRCTRAN_RAW_SHIFT)                          /* 0x0000003F */
/* RAWDSTTRAN */
#define DMA_RAWDSTTRAN_OFFSET                              (0x2D8U)
#define DMA_RAWDSTTRAN_RAW_SHIFT                           (0U)
#define DMA_RAWDSTTRAN_RAW_MASK                            (0x3FU << DMA_RAWDSTTRAN_RAW_SHIFT)                          /* 0x0000003F */
/* RAWERR */
#define DMA_RAWERR_OFFSET                                  (0x2E0U)
#define DMA_RAWERR_RAW_SHIFT                               (0U)
#define DMA_RAWERR_RAW_MASK                                (0x3FU << DMA_RAWERR_RAW_SHIFT)                              /* 0x0000003F */
/* RESERVED02E4 */
#define DMA_RESERVED02E4_OFFSET                            (0x2E4U)
/* STATUSTFR */
#define DMA_STATUSTFR_OFFSET                               (0x2E8U)
#define DMA_STATUSTFR                                      (0x0U)
#define DMA_STATUSTFR_STATUS_SHIFT                         (0U)
#define DMA_STATUSTFR_STATUS_MASK                          (0x3FU << DMA_STATUSTFR_STATUS_SHIFT)                        /* 0x0000003F */
/* STATUSBLOCK */
#define DMA_STATUSBLOCK_OFFSET                             (0x2F0U)
#define DMA_STATUSBLOCK                                    (0x0U)
#define DMA_STATUSBLOCK_STATUS_SHIFT                       (0U)
#define DMA_STATUSBLOCK_STATUS_MASK                        (0x3FU << DMA_STATUSBLOCK_STATUS_SHIFT)                      /* 0x0000003F */
/* STATUSSRCTRAN */
#define DMA_STATUSSRCTRAN_OFFSET                           (0x2F8U)
#define DMA_STATUSSRCTRAN                                  (0x0U)
#define DMA_STATUSSRCTRAN_STATUS_SHIFT                     (0U)
#define DMA_STATUSSRCTRAN_STATUS_MASK                      (0x3FU << DMA_STATUSSRCTRAN_STATUS_SHIFT)                    /* 0x0000003F */
/* STATUSDSTTRAN */
#define DMA_STATUSDSTTRAN_OFFSET                           (0x300U)
#define DMA_STATUSDSTTRAN                                  (0x0U)
#define DMA_STATUSDSTTRAN_STATUS_SHIFT                     (0U)
#define DMA_STATUSDSTTRAN_STATUS_MASK                      (0x3FU << DMA_STATUSDSTTRAN_STATUS_SHIFT)                    /* 0x0000003F */
/* STATUSERR */
#define DMA_STATUSERR_OFFSET                               (0x308U)
#define DMA_STATUSERR                                      (0x0U)
#define DMA_STATUSERR_STATUS_SHIFT                         (0U)
#define DMA_STATUSERR_STATUS_MASK                          (0x3FU << DMA_STATUSERR_STATUS_SHIFT)                        /* 0x0000003F */
/* RESERVED030C */
#define DMA_RESERVED030C_OFFSET                            (0x30CU)
/* MASKTFR */
#define DMA_MASKTFR_OFFSET                                 (0x310U)
#define DMA_MASKTFR_INT_MASK_SHIFT                         (0U)
#define DMA_MASKTFR_INT_MASK_MASK                          (0x3FU << DMA_MASKTFR_INT_MASK_SHIFT)                        /* 0x0000003F */
#define DMA_MASKTFR_INT_MASK_WE_SHIFT                      (8U)
#define DMA_MASKTFR_INT_MASK_WE_MASK                       (0x3FU << DMA_MASKTFR_INT_MASK_WE_SHIFT)                     /* 0x00003F00 */
/* MASKBLOCK */
#define DMA_MASKBLOCK_OFFSET                               (0x318U)
#define DMA_MASKBLOCK_INT_MASK_SHIFT                       (0U)
#define DMA_MASKBLOCK_INT_MASK_MASK                        (0x3FU << DMA_MASKBLOCK_INT_MASK_SHIFT)                      /* 0x0000003F */
#define DMA_MASKBLOCK_INT_MASK_WE_SHIFT                    (8U)
#define DMA_MASKBLOCK_INT_MASK_WE_MASK                     (0x3FU << DMA_MASKBLOCK_INT_MASK_WE_SHIFT)                   /* 0x00003F00 */
/* MASKSRCTRAN */
#define DMA_MASKSRCTRAN_OFFSET                             (0x320U)
#define DMA_MASKSRCTRAN_INT_MASK_SHIFT                     (0U)
#define DMA_MASKSRCTRAN_INT_MASK_MASK                      (0x3FU << DMA_MASKSRCTRAN_INT_MASK_SHIFT)                    /* 0x0000003F */
#define DMA_MASKSRCTRAN_INT_MASK_WE_SHIFT                  (8U)
#define DMA_MASKSRCTRAN_INT_MASK_WE_MASK                   (0x3FU << DMA_MASKSRCTRAN_INT_MASK_WE_SHIFT)                 /* 0x00003F00 */
/* MASKDSTTRAN */
#define DMA_MASKDSTTRAN_OFFSET                             (0x328U)
#define DMA_MASKDSTTRAN_INT_MASK_SHIFT                     (0U)
#define DMA_MASKDSTTRAN_INT_MASK_MASK                      (0x3FU << DMA_MASKDSTTRAN_INT_MASK_SHIFT)                    /* 0x0000003F */
#define DMA_MASKDSTTRAN_INT_MASK_WE_SHIFT                  (8U)
#define DMA_MASKDSTTRAN_INT_MASK_WE_MASK                   (0x3FU << DMA_MASKDSTTRAN_INT_MASK_WE_SHIFT)                 /* 0x00003F00 */
/* MASKERR */
#define DMA_MASKERR_OFFSET                                 (0x330U)
#define DMA_MASKERR_INT_MASK_SHIFT                         (0U)
#define DMA_MASKERR_INT_MASK_MASK                          (0x3FU << DMA_MASKERR_INT_MASK_SHIFT)                        /* 0x0000003F */
#define DMA_MASKERR_INT_MASK_WE_SHIFT                      (8U)
#define DMA_MASKERR_INT_MASK_WE_MASK                       (0x3FU << DMA_MASKERR_INT_MASK_WE_SHIFT)                     /* 0x00003F00 */
/* RESERVED0334 */
#define DMA_RESERVED0334_OFFSET                            (0x334U)
/* CLEARTFR */
#define DMA_CLEARTFR_OFFSET                                (0x338U)
#define DMA_CLEARTFR_CLEAR_SHIFT                           (0U)
#define DMA_CLEARTFR_CLEAR_MASK                            (0x3FU << DMA_CLEARTFR_CLEAR_SHIFT)                          /* 0x0000003F */
/* CLEARBLOCK */
#define DMA_CLEARBLOCK_OFFSET                              (0x340U)
#define DMA_CLEARBLOCK_CLEAR_SHIFT                         (0U)
#define DMA_CLEARBLOCK_CLEAR_MASK                          (0x3FU << DMA_CLEARBLOCK_CLEAR_SHIFT)                        /* 0x0000003F */
/* CLEARSRCTRAN */
#define DMA_CLEARSRCTRAN_OFFSET                            (0x348U)
#define DMA_CLEARSRCTRAN_CLEAR_SHIFT                       (0U)
#define DMA_CLEARSRCTRAN_CLEAR_MASK                        (0x3FU << DMA_CLEARSRCTRAN_CLEAR_SHIFT)                      /* 0x0000003F */
/* CLEARDSTTRAN */
#define DMA_CLEARDSTTRAN_OFFSET                            (0x350U)
#define DMA_CLEARDSTTRAN_CLEAR_SHIFT                       (0U)
#define DMA_CLEARDSTTRAN_CLEAR_MASK                        (0x3FU << DMA_CLEARDSTTRAN_CLEAR_SHIFT)                      /* 0x0000003F */
/* CLEARERR */
#define DMA_CLEARERR_OFFSET                                (0x358U)
#define DMA_CLEARERR_CLEAR_SHIFT                           (0U)
#define DMA_CLEARERR_CLEAR_MASK                            (0x3FU << DMA_CLEARERR_CLEAR_SHIFT)                          /* 0x0000003F */
/* RESERVED035C */
#define DMA_RESERVED035C_OFFSET                            (0x35CU)
/* STATUSINT */
#define DMA_STATUSINT_OFFSET                               (0x360U)
#define DMA_STATUSINT                                      (0x0U)
#define DMA_STATUSINT_TFR_SHIFT                            (0U)
#define DMA_STATUSINT_TFR_MASK                             (0x1U << DMA_STATUSINT_TFR_SHIFT)                            /* 0x00000001 */
#define DMA_STATUSINT_BLOCK_SHIFT                          (1U)
#define DMA_STATUSINT_BLOCK_MASK                           (0x1U << DMA_STATUSINT_BLOCK_SHIFT)                          /* 0x00000002 */
#define DMA_STATUSINT_SRCT_SHIFT                           (2U)
#define DMA_STATUSINT_SRCT_MASK                            (0x1U << DMA_STATUSINT_SRCT_SHIFT)                           /* 0x00000004 */
#define DMA_STATUSINT_DSTT_SHIFT                           (3U)
#define DMA_STATUSINT_DSTT_MASK                            (0x1U << DMA_STATUSINT_DSTT_SHIFT)                           /* 0x00000008 */
#define DMA_STATUSINT_ERR_SHIFT                            (4U)
#define DMA_STATUSINT_ERR_MASK                             (0x1U << DMA_STATUSINT_ERR_SHIFT)                            /* 0x00000010 */
/* DMACFGREG */
#define DMA_DMACFGREG_OFFSET                               (0x398U)
#define DMA_DMACFGREG_DMA_EN_SHIFT                         (0U)
#define DMA_DMACFGREG_DMA_EN_MASK                          (0x1U << DMA_DMACFGREG_DMA_EN_SHIFT)                         /* 0x00000001 */
/* CHENREG */
#define DMA_CHENREG_OFFSET                                 (0x3A0U)
#define DMA_CHENREG_CH_EN_SHIFT                            (0U)
#define DMA_CHENREG_CH_EN_MASK                             (0x3FU << DMA_CHENREG_CH_EN_SHIFT)                           /* 0x0000003F */
#define DMA_CHENREG_CH_EN_WE_SHIFT                         (8U)
#define DMA_CHENREG_CH_EN_WE_MASK                          (0x3FU << DMA_CHENREG_CH_EN_WE_SHIFT)                        /* 0x00003F00 */
/******************************************SFC*******************************************/
/* CTRL */
#define SFC_CTRL_OFFSET                                    (0x0U)
#define SFC_CTRL_SPIM_SHIFT                                (0U)
#define SFC_CTRL_SPIM_MASK                                 (0x1U << SFC_CTRL_SPIM_SHIFT)                                /* 0x00000001 */
#define SFC_CTRL_SHIFTPHASE_SHIFT                          (1U)
#define SFC_CTRL_SHIFTPHASE_MASK                           (0x1U << SFC_CTRL_SHIFTPHASE_SHIFT)                          /* 0x00000002 */
#define SFC_CTRL_IDLE_CYCLE_SHIFT                          (4U)
#define SFC_CTRL_IDLE_CYCLE_MASK                           (0xFU << SFC_CTRL_IDLE_CYCLE_SHIFT)                          /* 0x000000F0 */
#define SFC_CTRL_CMDB_SHIFT                                (8U)
#define SFC_CTRL_CMDB_MASK                                 (0x3U << SFC_CTRL_CMDB_SHIFT)                                /* 0x00000300 */
#define SFC_CTRL_ADRB_SHIFT                                (10U)
#define SFC_CTRL_ADRB_MASK                                 (0x3U << SFC_CTRL_ADRB_SHIFT)                                /* 0x00000C00 */
#define SFC_CTRL_DATB_SHIFT                                (12U)
#define SFC_CTRL_DATB_MASK                                 (0x3U << SFC_CTRL_DATB_SHIFT)                                /* 0x00003000 */
/* IMR */
#define SFC_IMR_OFFSET                                     (0x4U)
#define SFC_IMR_RXFM_SHIFT                                 (0U)
#define SFC_IMR_RXFM_MASK                                  (0x1U << SFC_IMR_RXFM_SHIFT)                                 /* 0x00000001 */
#define SFC_IMR_RXUM_SHIFT                                 (1U)
#define SFC_IMR_RXUM_MASK                                  (0x1U << SFC_IMR_RXUM_SHIFT)                                 /* 0x00000002 */
#define SFC_IMR_TXOM_SHIFT                                 (2U)
#define SFC_IMR_TXOM_MASK                                  (0x1U << SFC_IMR_TXOM_SHIFT)                                 /* 0x00000004 */
#define SFC_IMR_TXEM_SHIFT                                 (3U)
#define SFC_IMR_TXEM_MASK                                  (0x1U << SFC_IMR_TXEM_SHIFT)                                 /* 0x00000008 */
#define SFC_IMR_TRANSM_SHIFT                               (4U)
#define SFC_IMR_TRANSM_MASK                                (0x1U << SFC_IMR_TRANSM_SHIFT)                               /* 0x00000010 */
#define SFC_IMR_AHBM_SHIFT                                 (5U)
#define SFC_IMR_AHBM_MASK                                  (0x1U << SFC_IMR_AHBM_SHIFT)                                 /* 0x00000020 */
#define SFC_IMR_NSPIM_SHIFT                                (6U)
#define SFC_IMR_NSPIM_MASK                                 (0x1U << SFC_IMR_NSPIM_SHIFT)                                /* 0x00000040 */
#define SFC_IMR_DMAM_SHIFT                                 (7U)
#define SFC_IMR_DMAM_MASK                                  (0x1U << SFC_IMR_DMAM_SHIFT)                                 /* 0x00000080 */
/* ICLR */
#define SFC_ICLR_OFFSET                                    (0x8U)
#define SFC_ICLR_RXFC_SHIFT                                (0U)
#define SFC_ICLR_RXFC_MASK                                 (0x1U << SFC_ICLR_RXFC_SHIFT)                                /* 0x00000001 */
#define SFC_ICLR_RXUC_SHIFT                                (1U)
#define SFC_ICLR_RXUC_MASK                                 (0x1U << SFC_ICLR_RXUC_SHIFT)                                /* 0x00000002 */
#define SFC_ICLR_TXOC_SHIFT                                (2U)
#define SFC_ICLR_TXOC_MASK                                 (0x1U << SFC_ICLR_TXOC_SHIFT)                                /* 0x00000004 */
#define SFC_ICLR_TXEC_SHIFT                                (3U)
#define SFC_ICLR_TXEC_MASK                                 (0x1U << SFC_ICLR_TXEC_SHIFT)                                /* 0x00000008 */
#define SFC_ICLR_TRANSC_SHIFT                              (4U)
#define SFC_ICLR_TRANSC_MASK                               (0x1U << SFC_ICLR_TRANSC_SHIFT)                              /* 0x00000010 */
#define SFC_ICLR_AHBC_SHIFT                                (5U)
#define SFC_ICLR_AHBC_MASK                                 (0x1U << SFC_ICLR_AHBC_SHIFT)                                /* 0x00000020 */
#define SFC_ICLR_NSPIC_SHIFT                               (6U)
#define SFC_ICLR_NSPIC_MASK                                (0x1U << SFC_ICLR_NSPIC_SHIFT)                               /* 0x00000040 */
#define SFC_ICLR_DMAC_SHIFT                                (7U)
#define SFC_ICLR_DMAC_MASK                                 (0x1U << SFC_ICLR_DMAC_SHIFT)                                /* 0x00000080 */
/* FTLR */
#define SFC_FTLR_OFFSET                                    (0xCU)
#define SFC_FTLR_TXFTLR_SHIFT                              (0U)
#define SFC_FTLR_TXFTLR_MASK                               (0xFFU << SFC_FTLR_TXFTLR_SHIFT)                             /* 0x000000FF */
#define SFC_FTLR_RXFTLR_SHIFT                              (8U)
#define SFC_FTLR_RXFTLR_MASK                               (0xFFU << SFC_FTLR_RXFTLR_SHIFT)                             /* 0x0000FF00 */
/* RCVR */
#define SFC_RCVR_OFFSET                                    (0x10U)
#define SFC_RCVR_RCVR_SHIFT                                (0U)
#define SFC_RCVR_RCVR_MASK                                 (0x1U << SFC_RCVR_RCVR_SHIFT)                                /* 0x00000001 */
/* AX */
#define SFC_AX_OFFSET                                      (0x14U)
#define SFC_AX                                             (0x0U)
#define SFC_AX_AX_SHIFT                                    (0U)
#define SFC_AX_AX_MASK                                     (0xFFU << SFC_AX_AX_SHIFT)                                   /* 0x000000FF */
/* ABIT */
#define SFC_ABIT_OFFSET                                    (0x18U)
#define SFC_ABIT_ABIT_SHIFT                                (0U)
#define SFC_ABIT_ABIT_MASK                                 (0x1U << SFC_ABIT_ABIT_SHIFT)                                /* 0x00000001 */
/* ISR */
#define SFC_ISR_OFFSET                                     (0x1CU)
#define SFC_ISR_RXFS_SHIFT                                 (0U)
#define SFC_ISR_RXFS_MASK                                  (0x1U << SFC_ISR_RXFS_SHIFT)                                 /* 0x00000001 */
#define SFC_ISR_RXUS_SHIFT                                 (1U)
#define SFC_ISR_RXUS_MASK                                  (0x1U << SFC_ISR_RXUS_SHIFT)                                 /* 0x00000002 */
#define SFC_ISR_TXOS_SHIFT                                 (2U)
#define SFC_ISR_TXOS_MASK                                  (0x1U << SFC_ISR_TXOS_SHIFT)                                 /* 0x00000004 */
#define SFC_ISR_TXES_SHIFT                                 (3U)
#define SFC_ISR_TXES_MASK                                  (0x1U << SFC_ISR_TXES_SHIFT)                                 /* 0x00000008 */
#define SFC_ISR_TRANSS_SHIFT                               (4U)
#define SFC_ISR_TRANSS_MASK                                (0x1U << SFC_ISR_TRANSS_SHIFT)                               /* 0x00000010 */
#define SFC_ISR_AHBS_SHIFT                                 (5U)
#define SFC_ISR_AHBS_MASK                                  (0x1U << SFC_ISR_AHBS_SHIFT)                                 /* 0x00000020 */
#define SFC_ISR_NSPIS_SHIFT                                (6U)
#define SFC_ISR_NSPIS_MASK                                 (0x1U << SFC_ISR_NSPIS_SHIFT)                                /* 0x00000040 */
#define SFC_ISR_DMAS_SHIFT                                 (7U)
#define SFC_ISR_DMAS_MASK                                  (0x1U << SFC_ISR_DMAS_SHIFT)                                 /* 0x00000080 */
/* FSR */
#define SFC_FSR_OFFSET                                     (0x20U)
#define SFC_FSR_TXES_SHIFT                                 (0U)
#define SFC_FSR_TXES_MASK                                  (0x1U << SFC_FSR_TXES_SHIFT)                                 /* 0x00000001 */
#define SFC_FSR_TXFS_SHIFT                                 (1U)
#define SFC_FSR_TXFS_MASK                                  (0x1U << SFC_FSR_TXFS_SHIFT)                                 /* 0x00000002 */
#define SFC_FSR_RXES_SHIFT                                 (2U)
#define SFC_FSR_RXES_MASK                                  (0x1U << SFC_FSR_RXES_SHIFT)                                 /* 0x00000004 */
#define SFC_FSR_RXFS_SHIFT                                 (3U)
#define SFC_FSR_RXFS_MASK                                  (0x1U << SFC_FSR_RXFS_SHIFT)                                 /* 0x00000008 */
#define SFC_FSR_TXWLVL_SHIFT                               (8U)
#define SFC_FSR_TXWLVL_MASK                                (0x1FU << SFC_FSR_TXWLVL_SHIFT)                              /* 0x00001F00 */
#define SFC_FSR_RXWLVL_SHIFT                               (16U)
#define SFC_FSR_RXWLVL_MASK                                (0x1FU << SFC_FSR_RXWLVL_SHIFT)                              /* 0x001F0000 */
/* SR */
#define SFC_SR_OFFSET                                      (0x24U)
#define SFC_SR_SR_SHIFT                                    (0U)
#define SFC_SR_SR_MASK                                     (0x1U << SFC_SR_SR_SHIFT)                                    /* 0x00000001 */
/* DMAADDR */
#define SFC_DMAADDR_OFFSET                                 (0x80U)
#define SFC_DMAADDR_DMAADDR_SHIFT                          (0U)
#define SFC_DMAADDR_DMAADDR_MASK                           (0xFFFFFFFFU << SFC_DMAADDR_DMAADDR_SHIFT)                   /* 0xFFFFFFFF */
/* DMATR */
#define SFC_DMATR_OFFSET                                   (0x84U)
#define SFC_DMATR_DMATR_SHIFT                              (0U)
#define SFC_DMATR_DMATR_MASK                               (0x1U << SFC_DMATR_DMATR_SHIFT)                              /* 0x00000001 */
/* CMD */
#define SFC_CMD_OFFSET                                     (0x100U)
#define SFC_CMD_CMD_SHIFT                                  (0U)
#define SFC_CMD_CMD_MASK                                   (0xFFU << SFC_CMD_CMD_SHIFT)                                 /* 0x000000FF */
#define SFC_CMD_DUMM_SHIFT                                 (8U)
#define SFC_CMD_DUMM_MASK                                  (0xFU << SFC_CMD_DUMM_SHIFT)                                 /* 0x00000F00 */
#define SFC_CMD_WR_SHIFT                                   (12U)
#define SFC_CMD_WR_MASK                                    (0x1U << SFC_CMD_WR_SHIFT)                                   /* 0x00001000 */
#define SFC_CMD_CONT_SHIFT                                 (13U)
#define SFC_CMD_CONT_MASK                                  (0x1U << SFC_CMD_CONT_SHIFT)                                 /* 0x00002000 */
#define SFC_CMD_ADDRB_SHIFT                                (14U)
#define SFC_CMD_ADDRB_MASK                                 (0x3U << SFC_CMD_ADDRB_SHIFT)                                /* 0x0000C000 */
#define SFC_CMD_TRB_SHIFT                                  (16U)
#define SFC_CMD_TRB_MASK                                   (0x3FFFU << SFC_CMD_TRB_SHIFT)                               /* 0x3FFF0000 */
#define SFC_CMD_CS_SHIFT                                   (30U)
#define SFC_CMD_CS_MASK                                    (0x3U << SFC_CMD_CS_SHIFT)                                   /* 0xC0000000 */
/* ADDR */
#define SFC_ADDR_OFFSET                                    (0x104U)
#define SFC_ADDR_ADDR_SHIFT                                (0U)
#define SFC_ADDR_ADDR_MASK                                 (0xFFFFFFFFU << SFC_ADDR_ADDR_SHIFT)                         /* 0xFFFFFFFF */
/* DATA */
#define SFC_DATA_OFFSET                                    (0x108U)
#define SFC_DATA_ADDR_SHIFT                                (0U)
#define SFC_DATA_ADDR_MASK                                 (0xFFFFFFFFU << SFC_DATA_ADDR_SHIFT)                         /* 0xFFFFFFFF */

/********Name=SOFTRST0_CON,Offset=0xC0********/
#define SRST_USBOTG   0U
#define SRST_USBPHY   1U
#define SRST_USBGLB   2U
#define SRST_I2S0     3U
#define SRST_I2S1     4U
#define SRST_SPI0     5U
#define SRST_SPI1     6U
#define SRST_UART0    7U
#define SRST_UART1    8U
#define SRST_UART2    9U
#define SRST_BOOTROOM 10U
#define SRST_SYSARM1  11U
#define SRST_SYSARM0  12U
#define SRST_LCDC     13U
#define SRST_IMDCT    14U
#define SRST_SYNTH    15U
/********Name=SOFTRST1_CON,Offset=0xC4********/
#define SRST_SYSDMA    16U
#define SRST_SDMMC     17U
#define SRST_SDIO      18U
#define SRST_SYSMATRIX 19U
#define SRST_TIMER0    20U
#define SRST_TIMER1    21U
#define SRST_SARADC    22U
#define SRST_PWM0      23U
#define SRST_PWM1      24U
#define SRST_SFC       25U
#define SRST_I2C0      26U
#define SRST_I2C1      27U
#define SRST_I2C2      28U
#define SRST_EBC       29U
#define SRST_MAILBOX   30U
#define SRST_WDT       31U
/********Name=SOFTRST2_CON,Offset=0xC8********/
#define SRST_HIGH_MATRIX 32U
#define SRST_HIGHRAM1    33U
#define SRST_HIGHRAM0    34U
#define SRST_UART3       37U
#define SRST_UART4       38U
#define SRST_UART5       39U
#define SRST_GPIO0       40U
#define SRST_GPIO1       41U
/********Name=SOFTRST3_CON,Offset=0xCC********/
#define SRST_ACODEC     48U
#define SRST_SYS_CORE   49U
#define SRST_CAL_CORE   50U
#define SRST_BRG_TO_PMU 51U
#define SRST_PMU        53U
#define SRST_ASYNC_BRG  54U
#define SRST_DMA2       55U

/********Name=CLKGATE0_CON,Offset=0x80********/
#define CLK_SYS_CORE_GATE  0U
#define HCLK_SYS_CORE_GATE 1U
#define HCLK_CAL_CORE_GATE 2U
#define HCLK_LOGIC_GATE    3U
#define PCLK_LOGIC_GATE    4U
#define HCLK_PMU_GATE      6U
#define CLK_USBPHY_GATE    7U
#define CLK_SFC_GATE       8U
#define CLK_SARADC_GATE    10U
#define CLK_TIMER0_GATE    11U
#define CLK_TIMER1_GATE    12U
#define PCLK_PMU_GATE      13U
#define CLK_ACODEC_GATE    14U
/********Name=CLKGATE1_CON,Offset=0x84********/
#define CLK_I2S1_SRC_GATE      16U
#define CLK_I2S1_FRAC_SRC_GATE 17U
#define CLK_I2S1_OUT_GATE      18U
#define CLK_I2S1_GATE          19U
#define CLK_I2S0_SRC_GATE      20U
#define CLK_I2S0_FRAC_SRC_GATE 21U
#define CLK_I2S0_GATE          22U
#define DCLK_EBC_SRC_GATE      23U
#define CLK_SDMMC_GATE         24U
#define CLK_EMMC_GATE          25U
#define CLK_SPI0_GATE          26U
#define CLK_SPI1_GATE          27U
#define CLK_UART0_GATE         28U
#define CLK_UART1_GATE         29U
#define CLK_UART2_GATE         30U
#define CLK_TEST_GATE          31U
/********Name=CLKGATE2_CON,Offset=0x88********/
#define CLK_UART3_GATE 32U
#define CLK_UART4_GATE 33U
#define CLK_UART5_GATE 34U
/********Name=CLKGATE3_CON,Offset=0x8C********/
#define HDRAM0_GATE          48U
#define HDRAM1_GATE          49U
#define HDRAM2_GATE          50U
#define HDRAM3_GATE          51U
#define HDRAM4_GATE          52U
#define HDRAM5_GATE          53U
#define HDRAM6_GATE          54U
#define HDRAM7_GATE          55U
#define HDRAM_INTERFACE_GATE 56U
#define HCLK_HIFI_GATE       57U
/********Name=CLKGATE4_CON,Offset=0x90********/
#define HIRAM0_GATE          64U
#define HIRAM1_GATE          65U
#define HIRAM2_GATE          66U
#define HIRAM3_GATE          67U
#define HIRAM4_GATE          68U
#define HIRAM5_GATE          69U
#define HIRAM6_GATE          70U
#define HIRAM7_GATE          71U
#define HIRAM_INTERFACE_GATE 72U
/********Name=CLKGATE5_CON,Offset=0x94********/
#define PCLK_GRF_GATE          83U
#define PCLK_GPIO2_GATE        84U
#define CLK24M_GATE            85U
#define PMU_RAM_INTERFACE_GATE 86U
#define PMU_RAM0_GATE          87U
#define PMU_RAM1_GATE          88U
#define CLK_PVTM_GATE          90U
/********Name=CLKGATE6_CON,Offset=0x98********/
#define DRAM0_GATE          96U
#define DRAM1_GATE          97U
#define DRAM2_GATE          98U
#define DRAM3_GATE          99U
#define DRAM4_GATE          100U
#define DRAM5_GATE          101U
#define DRAM6_GATE          102U
#define DRAM7_GATE          103U
#define DRAM_INTERFACE_GATE 104U
#define HCLK_APB_BRG_GATE   105U
#define HCLK_USBC_GATE      106U
#define HCLK_SDMMC_GATE     107U
#define HCLK_EMMC_GATE      108U
#define HCLK_EBC_GAT        109U
#define HCLK_SYNTH_GATE     110U
#define HCLK_SFC_GATE       111U
/********Name=CLKGATE7_CON,Offset=0x9C********/
#define IRAM0_GATE           112U
#define IRAM1_GATE           113U
#define IRAM2_GATE           114U
#define IRAM3_GATE           115U
#define IRAM4_GATE           116U
#define IRAM5_GATE           117U
#define IRAM6_GATE           118U
#define IRAM7_GATE           119U
#define IRAM_INTERFACE_GATE  120U
#define HCLK_BOOTROM_GATE    121U
#define HCLK_BUS_MATRIX_GATE 122U
#define HCLK_AHB_BRG_GATE    123U
#define HCLK_1TOM_BRG_GATE   124U
#define HCLK_DMA_GATE        125U
#define HCLK_IMDCT_GATE      126U
#define HCLK_LCDC_GATE       127U
/********Name=CLKGATE8_CON,Offset=0xA0********/
#define HCLK_PMU_APB_BRG_GATE 128U
#define PCLK_I2S0_GATE        129U
#define PCLK_I2S1_GATE        130U
#define PCLK_I2C0_GATE        131U
#define PCLK_I2C1_GATE        132U
#define PCLK_I2C2_GATE        133U
#define PCLK_SPI0_GATE        134U
#define PCLK_SPI1_GATE        135U
#define PCLK_UART0_GATE       136U
#define PCLK_UART1_GATE       137U
#define PCLK_UART2_GATE       138U
#define PCLK_UART3_GATE       139U
#define PCLK_UART4_GATE       140U
#define PCLK_UART5_GATE       141U
#define PCLK_TIMER_GATE       142U
#define PCLK_WDT_GATE         143U
/********Name=CLKGATE9_CON,Offset=0xA4********/
#define PCLK_PWM0_GATE       144U
#define PCLK_PWM1_GATE       145U
#define PCLK_MAILBOX_GATE    146U
#define PCLK_SARADC_GATE     147U
#define PCLK_GPIO0_GATE      148U
#define PCLK_GPIO1_GATE      149U
#define PCLK_ACODEC_GATE     150U
#define PCLK_ASYNC_BRG_GATE  151U
#define PCLK_LGC_MATRIX_GATE 152U

/********Name=CLKSEL0_CON,Offset=0x14********/
#define SYS_CORE_DIV  0x03000000U
#define SYS_STCLK_DIV 0x03040000U
#define CAL_STCLK_DIV 0x03080000U
#define CAL_CORE_DIV  0x030C0000U
/********Name=CLKSEL1_CON,Offset=0x18********/
#define PCLK_LOGIC_DIV 0x02080001U
/********Name=CLKSEL2_CON,Offset=0x1C********/
#define TIMER0_DIV 0x05000002U
#define TIMER1_DIV 0x05080002U
/********Name=CLKSEL3_CON,Offset=0x20********/
#define I2S1_DIV 0x07000003U
/********Name=CLKSEL4_CON,Offset=0x24********/
#define I2S0_DIV 0x07000004U
/********Name=CLKSEL5_CON,Offset=0x28********/
#define EBC_DCLK_DIV 0x08000005U
/********Name=CLKSEL6_CON,Offset=0x2C********/
#define SARADC_DIV 0x08000006U
/********Name=CLKSEL7_CON,Offset=0x30********/
#define SDMMC_DIV 0x05000007U
#define EMMC_DIV  0x05080007U
/********Name=CLKSEL8_CON,Offset=0x34********/
#define SPI0_DIV 0x06000008U
#define SPI1_DIV 0x06080008U
/********Name=CLKSEL9_CON,Offset=0x38********/
#define UART1_DIV 0x06000009U
#define UART0_DIV 0x06080009U
/********Name=CLKSEL10_CON,Offset=0x3C********/
#define UART2_DIV 0x0600000AU
#define SFC_DIV   0x0508000AU
/********Name=CLKSEL11_CON,Offset=0x40********/
#define TEST_DIV  0x0500000BU
#define UART3_DIV 0x0608000BU
/********Name=CLKSEL12_CON,Offset=0x44********/
#define UART4_DIV 0x0600000CU
#define UART5_DIV 0x0608000CU
/********Name=CLKSEL15_CON,Offset=0x50********/
#define CLK_I2S0_FRAC_DIV 0x2000000FU
/********Name=CLKSEL16_CON,Offset=0x54********/
#define CLK_I2S1_FRAC_DIV 0x20000010U

/********Name=CLKSEL0_CON,Offset=0x14********/
#define SYS_CORE_SRC_SEL            0x01030000U
#define CAL_CORE_SRC_SEL            0x010f0000U
#define SYS_CORE_SRC_SEL_PLL        0U
#define SYS_CORE_SRC_SEL_USBPHY480M 1U
/********Name=CLKSEL1_CON,Offset=0x18********/
/********Name=CLKSEL2_CON,Offset=0x1C********/
#define TIMER0_PLL_SEL            0x01070002U
#define TIMER0_PLL_SEL_24M        0U
#define TIMER0_PLL_SEL_PCLK_LOGIC 1U
#define TIMER1_PLL_SEL            0x010F0002U
#define TIMER1_PLL_SEL_24M        0U
#define TIMER1_PLL_SEL_PCLK_LOGIC 1U
/********Name=CLKSEL3_CON,Offset=0x20********/
#define I2S1_CLK_SEL          0x02080003U
#define I2S1_CLK_SEL_CLOCK    0U
#define I2S1_CLK_SEL_OUTPUT   1U
#define I2S1_CLK_SEL_12M      2U
#define I2S1_OUT_SEL          0x010C0003U
#define I2S1_OUT_SEL_CLK_I2S1 0U
#define I2S1_OUT_SEL_12M      1U
#define I2S1_PLL_SEL          0x010F0003U
#define I2S1_PLL_SEL_APLL     0U
#define I2S1_PLL_SEL_AUPLL    1U
/********Name=CLKSEL4_CON,Offset=0x24********/
#define I2S0_CLK_SEL        0x02080004U
#define I2S0_CLK_SEL_CLOCK  0U
#define I2S0_CLK_SEL_OUTPUT 1U
#define I2S0_CLK_SEL_12M    2U
#define I2S0_PLL_SEL        0x010F0004U
#define I2S0_PLL_SEL_APLL   0U
#define I2S0_PLL_SEL_AUPLL  1U
/********Name=CLKSEL5_CON,Offset=0x28********/
#define EBC_DCLK_PLL_SEL      0x01080005U
#define EBC_DCLK_PLL_SEL_PLL  0U
#define EBC_DCLK_PLL_SEL_480M 1U
/********Name=CLKSEL6_CON,Offset=0x2C********/
/********Name=CLKSEL7_CON,Offset=0x30********/
#define SDMMC_CLK_PLL_SEL     0x01070007U
#define SDMMC_CLK_PLL_SEL_PLL 0U
#define SDMMC_CLK_PLL_SEL_24M 1U
#define EMMC_PLL_SEL          0x010F0007U
#define EMMC_PLL_SEL_PLL      0U
#define EMMC_PLL_SEL_24M      1U
/********Name=CLKSEL8_CON,Offset=0x34********/
#define SPI0_CLK_PLL_SEL     0x01070008U
#define SPI0_CLK_PLL_SEL_PLL 0U
#define SPI0_CLK_PLL_SEL_24M 1U
#define SPI1_CLK_PLL_SEL     0x010F0008U
#define SPI1_CLK_PLL_SEL_PLL 0U
#define SPI1_CLK_PLL_SEL_24M 1U
/********Name=CLKSEL9_CON,Offset=0x38********/
#define UART1_PLL_SEL         0x02060009U
#define UART1_PLL_SEL_PLL     0U
#define UART1_PLL_SEL_24M     1U
#define UART1_PLL_SEL_USB480M 2U
#define UART0_PLL_SEL         0x010F0009U
#define UART0_PLL_SEL_PLL     0U
#define UART0_PLL_SEL_24M     1U
/********Name=CLKSEL10_CON,Offset=0x3C********/
#define UART2_PLL_SEL     0x0107000AU
#define UART2_PLL_SEL_PLL 0U
#define UART2_PLL_SEL_24M 1U
#define SFC_PLL_SEL       0x010F000AU
#define SFC_PLL_SEL_PLL   0U
#define SFC_PLL_SEL_24M   1U
/********Name=CLKSEL11_CON,Offset=0x40********/
#define CORE_CLK_PLL_SEL              0x0305000BU
#define CORE_CLK_PLL_SEL_CLK_HIFI     0U
#define CORE_CLK_PLL_SEL_CLK_EBC      1U
#define CORE_CLK_PLL_SEL_CLK_SYS_CORE 2U
#define CORE_CLK_PLL_SEL_CLK_CAL_CORE 3U
#define CORE_CLK_PLL_SEL_HCLK_LOGIC   4U
#define CORE_CLK_PLL_SEL_PCLK_LOGIC   5U
#define CORE_CLK_PLL_SEL_CLK_I2S0     6U
#define CORE_CLK_PLL_SEL_CLK_I2S1     7U
#define UART3_PLL_SEL                 0x010F000BU
#define UART3_PLL_SEL_PLL             0U
#define UART3_PLL_SEL_24M             1U
/********Name=CLKSEL12_CON,Offset=0x44********/
#define UART4_PLL_SEL     0x0107000CU
#define UART4_PLL_SEL_PLL 0U
#define UART4_PLL_SEL_24M 1U
#define UART5_PLL_SEL     0x010F000CU
#define UART5_PLL_SEL_PLL 0U
#define UART5_PLL_SEL_24M 1U

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __RK2106_H */
