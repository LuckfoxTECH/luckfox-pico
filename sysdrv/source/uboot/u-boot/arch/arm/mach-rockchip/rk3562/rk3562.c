/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#include <common.h>
#include <clk.h>
#include <dm.h>
#include <asm/io.h>
#include <asm/arch/cpu.h>
#include <asm/arch/hardware.h>
#include <asm/arch/grf_rk3562.h>
#include <asm/arch/ioc_rk3562.h>
#include <asm/arch/rk_atags.h>
#include <linux/libfdt.h>
#include <fdt_support.h>
#include <asm/arch/clock.h>
#include <dt-bindings/clock/rk3562-cru.h>
#include <asm/arch-rockchip/rockchip_smccc.h>

DECLARE_GLOBAL_DATA_PTR;

#define FIREWALL_DDR_BASE	0xfef00000
#define FW_DDR_MST3_REG		0x2c	/* usb */
#define FW_DDR_MST4_REG		0x30	/* emmc */
#define FW_DDR_MST5_REG		0x34	/* fspi */
#define FW_DDR_MST6_REG		0x38	/* sdmmc mcu */
#define FW_DDR_CON_REG		0x80

#define PMU_GRF_BASE		0xff010000
#define PMU_GRF_SOC_CON9	0x0124

#define SYS_GRF_BASE		0xff030000
#define SYS_GRF_SOC_CON5	0x0414
#define SYS_GRF_SOC_CON6	0x0418

#define PERI_GRF_BASE		0xff040000
#define PERI_GRF_AUDIO_CON	0x0070

#define PIPEPHY_GRF_BASE	0xff098000
#define PIPEPHY_PIPE_CON5	0x0014

#define TOP_CRU_BASE		0xff100000
#define TOP_CRU_GATE_CON23	0x035c
#define TOP_CRU_SOFTRST_CON23	0x045c
#define TOP_CRU_CM0_GATEMASK	0x0680

#define PMU0_CRU_BASE		0xff110000

#define PMU1_CRU_BASE		0xff118000
#define PMU1_CRU_GATE_CON02	0x0188
#define PMU1_CRU_SOFTRST_CON02	0x0208
#define PMU1_CRU_CM0_GATEMASK	0x0420

#define PMU_BASE_ADDR		0xff258000
#define PMU2_BIU_IDLE_SFTCON0	0x110
#define PMU2_BIU_IDLE_ACK_STS0	0x120
#define PMU2_BIT_IDLE_STS0	0x128
#define PMU2_PWR_GATE_SFTCON0	0x210
#define PMU2_PWR_GATE_STS0	0x230
#define PMU2_MEM_SD_SFTCON0	0x300
/* PMU2_PWR_GATE_SFTCON0 */
#define PD_GPU_DWN_SFTENA	BIT(0)
#define PD_VI_DWN_SFTENA	BIT(5)
#define PD_VO_DWN_SFTENA	BIT(6)
/* PMU2_BIU_IDLE_SFTCON0 */
#define IDLE_REQ_GPU_SFTENA	BIT(1)
#define IDLE_REQ_VI_SFTENA	BIT(3)
#define IDLE_REQ_VO_SFTENA	BIT(4)
/* PMU2_BIU_IDLE_ACK_STS0 */
#define IDLE_ACK_GPU		BIT(1)
#define IDLE_ACK_VI		BIT(3)
#define IDLE_ACK_VO		BIT(4)
/* PMU2_BIT_IDLE_STS0 */
#define IDLE_GPU		BIT(1)
#define IDLE_VI			BIT(3)
#define IDLE_VO			BIT(4)

#define CRYPTO_PRIORITY_REG	0xfeeb0108
#define DCF_PRIORITY_REG	0xfee10408
#define DMA2DDR_PRIORITY_REG	0xfee03808
#define DMAC_PRIORITY_REG	0xfeeb0208
#define EMMC_PRIORITY_REG	0xfeeb0308
#define FSPI_PRIORITY_REG	0xfeeb0408
#define GMAC_PRIORITY_REG	0xfee10208
#define GPU_PRIORITY_REG	0xfee30008
#define ISP_PRIORITY_REG	0xfee70008
#define MAC100_PRIORITY_REG	0xfee10308
#define MCU_PRIORITY_REG	0xfee10008
#define PCIE_PRIORITY_REG	0xfeea0008
#define RKDMA_PRIORITY_REG	0xfeeb0508
#define SDMMC0_PRIORITY_REG	0xfeeb0608
#define SDMMC1_PRIORITY_REG	0xfeeb0708
#define USB2_PRIORITY_REG	0xfeeb0808
#define USB3_PRIORITY_REG	0xfeea0108
#define VICAP_PRIORITY_REG	0xfee70108
#define VOP_PRIORITY_REG	0xfee80008

#define PCIE_SHAPING_REG	0xfeea0088

#define QOS_PRIORITY_LEVEL(h, l)	((((h) & 7) << 8) | ((l) & 7))

#ifdef CONFIG_ARM64
#include <asm/armv8/mmu.h>

static struct mm_region rk3562_mem_map[] = {
	{
		.virt = 0x0UL,
		.phys = 0x0UL,
		.size = 0xfc000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) |
			 PTE_BLOCK_INNER_SHARE
	}, {
		.virt = 0xfc000000UL,
		.phys = 0xfc000000UL,
		.size = 0x04000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			 PTE_BLOCK_NON_SHARE |
			 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	}, {
		.virt = 0x100000000UL,
		.phys = 0x100000000UL,
		.size = 0x100000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) |
			 PTE_BLOCK_INNER_SHARE
	}, {
		/* List terminator */
		0,
	}
};

struct mm_region *mem_map = rk3562_mem_map;
#endif

#define	GPIO0_IOC_BASE			0xFF080000
#define	GPIO1_IOC_BASE			0xFF060000
#define	GPIO1A_IOMUX_SEL_L		0x0
#define	GPIO1A_IOMUX_SEL_H		0x4
#define	GPIO1B_IOMUX_SEL_L		0x8
#define	GPIO1_IOC_GPIO1A_DS0		0x200
#define	GPIO1_IOC_GPIO1A_DS1		0x204
#define	GPIO1_IOC_GPIO1B_DS0		0x210

#define	GPIO2_IOC_BASE			0xFF060000
#define	GPIO2_IOC_IO_VSEL0		0x300
/* GPIO2_IOC_IO_VSEL0 */
#define	POC_VCCIO2_VD_3V3		BIT(12)

#define	GPIO3_IOC_BASE			0xFF070000
#define	GPIO4_IOC_BASE			0xFF070000

/* UART0 iomux */
/* gpio0d0_sel */
#define UART0_RX_M0			1
#define UART0_RX_M0_OFFSET		0
#define UART0_RX_M0_ADDR		(GPIO0_IOC_BASE + 0x18)
/* gpio0d1_sel */
#define UART0_TX_M0			1
#define UART0_TX_M0_OFFSET		4
#define UART0_TX_M0_ADDR		(GPIO0_IOC_BASE + 0x18)

/* gpio1b3_sel */
#define UART0_RX_M1			2
#define UART0_RX_M1_OFFSET		12
#define UART0_RX_M1_ADDR		(GPIO1_IOC_BASE + 0x08)
/* gpio1b4_sel */
#define UART0_TX_M1			2
#define UART0_TX_M1_OFFSET		0
#define UART0_TX_M1_ADDR		(GPIO1_IOC_BASE + 0x0C)

/* UART1 iomux */
/* gpio1d1_sel */
#define UART1_RX_M0			1
#define UART1_RX_M0_OFFSET		4
#define UART1_RX_M0_ADDR		(GPIO1_IOC_BASE + 0x18)
/* gpio1d2_sel */
#define UART1_TX_M0			1
#define UART1_TX_M0_OFFSET		8
#define UART1_TX_M0_ADDR		(GPIO1_IOC_BASE + 0x18)

/* gpio4a6_sel */
#define UART1_RX_M1			4
#define UART1_RX_M1_OFFSET		8
#define UART1_RX_M1_ADDR		(GPIO4_IOC_BASE + 0x64)
/* gpio4a5_sel */
#define UART1_TX_M1			4
#define UART1_TX_M1_OFFSET		4
#define UART1_TX_M1_ADDR		(GPIO4_IOC_BASE + 0x64)

/* UART2 iomux */
/* gpio0c1_sel */
#define UART2_RX_M0			1
#define UART2_RX_M0_OFFSET		4
#define UART2_RX_M0_ADDR		(GPIO0_IOC_BASE + 0x10)
/* gpio0c0_sel */
#define UART2_TX_M0			1
#define UART2_TX_M0_OFFSET		0
#define UART2_TX_M0_ADDR		(GPIO0_IOC_BASE + 0x10)

/* gpio3a1_sel */
#define UART2_RX_M1			2
#define UART2_RX_M1_OFFSET		4
#define UART2_RX_M1_ADDR		(GPIO3_IOC_BASE + 0x40)
/* gpio3a0_sel */
#define UART2_TX_M1			2
#define UART2_TX_M1_OFFSET		0
#define UART2_TX_M1_ADDR		(GPIO3_IOC_BASE + 0x40)

/* UART3 iomux */
/* gpio4b5_sel */
#define UART3_RX_M0			7
#define UART3_RX_M0_OFFSET		4
#define UART3_RX_M0_ADDR		(GPIO4_IOC_BASE + 0x6C)
/* gpio4b4_sel */
#define UART3_TX_M0			7
#define UART3_TX_M0_OFFSET		0
#define UART3_TX_M0_ADDR		(GPIO4_IOC_BASE + 0x6C)

/* gpio3c0_sel */
#define UART3_RX_M1			3
#define UART3_RX_M1_OFFSET		0
#define UART3_RX_M1_ADDR		(GPIO3_IOC_BASE + 0x50)
/* gpio3b7_sel */
#define UART3_TX_M1			3
#define UART3_TX_M1_OFFSET		12
#define UART3_TX_M1_ADDR		(GPIO3_IOC_BASE + 0x4C)

/* UART4 iomux */
/* gpio3d1_sel */
#define UART4_RX_M0			4
#define UART4_RX_M0_OFFSET		4
#define UART4_RX_M0_ADDR		(GPIO3_IOC_BASE + 0x58)
/* gpio3d0_sel */
#define UART4_TX_M0			4
#define UART4_TX_M0_OFFSET		0
#define UART4_TX_M0_ADDR		(GPIO3_IOC_BASE + 0x58)

/* gpio1d5_sel */
#define UART4_RX_M1			3
#define UART4_RX_M1_OFFSET		4
#define UART4_RX_M1_ADDR		(GPIO1_IOC_BASE + 0x1C)
/* gpio1d6_sel */
#define UART4_TX_M1			3
#define UART4_TX_M1_OFFSET		8
#define UART4_TX_M1_ADDR		(GPIO1_IOC_BASE + 0x1C)

/* UART5 iomux */
/* gpio1b7_sel */
#define UART5_RX_M0			3
#define UART5_RX_M0_OFFSET		12
#define UART5_RX_M0_ADDR		(GPIO1_IOC_BASE + 0xC)
/* gpio1c0_sel */
#define UART5_TX_M0			3
#define UART5_TX_M0_OFFSET		0
#define UART5_TX_M0_ADDR		(GPIO1_IOC_BASE + 0x10)

/* gpio3a7_sel */
#define UART5_RX_M1			5
#define UART5_RX_M1_OFFSET		12
#define UART5_RX_M1_ADDR		(GPIO3_IOC_BASE + 0x44)
/* gpio3a6_sel */
#define UART5_TX_M1			5
#define UART5_TX_M1_OFFSET		8
#define UART5_TX_M1_ADDR		(GPIO3_IOC_BASE + 0x44)

/* UART6 iomux */
/* gpio0c7_sel */
#define UART6_RX_M0			1
#define UART6_RX_M0_OFFSET		12
#define UART6_RX_M0_ADDR		(GPIO0_IOC_BASE + 0x14)
/* gpio0c6_sel */
#define UART6_TX_M0			1
#define UART6_TX_M0_OFFSET		8
#define UART6_TX_M0_ADDR		(GPIO0_IOC_BASE + 0x14)

/* gpio4b0_sel */
#define UART6_RX_M1			6
#define UART6_RX_M1_OFFSET		0
#define UART6_RX_M1_ADDR		(GPIO4_IOC_BASE + 0x68)
/* gpio4a7_sel */
#define UART6_TX_M1			6
#define UART6_TX_M1_OFFSET		12
#define UART6_TX_M1_ADDR		(GPIO4_IOC_BASE + 0x64)

/* UART7 iomux */
/* gpio3c7_sel */
#define UART7_RX_M0			4
#define UART7_RX_M0_OFFSET		12
#define UART7_RX_M0_ADDR		(GPIO3_IOC_BASE + 0x54)
/* gpio3c4_sel */
#define UART7_TX_M0			4
#define UART7_TX_M0_OFFSET		0
#define UART7_TX_M0_ADDR		(GPIO3_IOC_BASE + 0x54)

/* gpio1b3_sel */
#define UART7_RX_M1			3
#define UART7_RX_M1_OFFSET		12
#define UART7_RX_M1_ADDR		(GPIO1_IOC_BASE + 0x08)
/* gpio1b4_sel */
#define UART7_TX_M1			3
#define UART7_TX_M1_OFFSET		0
#define UART7_TX_M1_ADDR		(GPIO1_IOC_BASE + 0x0C)

/* UART8 iomux */
/* gpio3b3_sel */
#define UART8_RX_M0			3
#define UART8_RX_M0_OFFSET		12
#define UART8_RX_M0_ADDR		(GPIO3_IOC_BASE + 0x48)
/* gpio3b2_sel */
#define UART8_TX_M0			3
#define UART8_TX_M0_OFFSET		8
#define UART8_TX_M0_ADDR		(GPIO3_IOC_BASE + 0x48)

/* gpio3d5_sel */
#define UART8_RX_M1			4
#define UART8_RX_M1_OFFSET		4
#define UART8_RX_M1_ADDR		(GPIO3_IOC_BASE + 0x5C)
/* gpio3d4_sel */
#define UART8_TX_M1			4
#define UART8_TX_M1_OFFSET		0
#define UART8_TX_M1_ADDR		(GPIO3_IOC_BASE + 0x5C)

/* UART9 iomux */
/* gpio4b3_sel */
#define UART9_RX_M0			4
#define UART9_RX_M0_OFFSET		12
#define UART9_RX_M0_ADDR		(GPIO4_IOC_BASE + 0x68)
/* gpio4b2_sel */
#define UART9_TX_M0			4
#define UART9_TX_M0_OFFSET		8
#define UART9_TX_M0_ADDR		(GPIO4_IOC_BASE + 0x68)

/* gpio3c3_sel */
#define UART9_RX_M1			3
#define UART9_RX_M1_OFFSET		12
#define UART9_RX_M1_ADDR		(GPIO3_IOC_BASE + 0x50)
/* gpio3c2_sel */
#define UART9_TX_M1			3
#define UART9_TX_M1_OFFSET		8
#define UART9_TX_M1_ADDR		(GPIO3_IOC_BASE + 0x50)

#define set_uart_iomux(bits_offset, bits_val, addr) \
	writel(GENMASK(bits_offset + 19, bits_offset + 16) | (bits_val << bits_offset) , addr)

#define set_uart_iomux_rx(ID, MODE) \
	set_uart_iomux(UART##ID##_RX_M##MODE##_OFFSET, UART##ID##_RX_M##MODE, UART##ID##_RX_M##MODE##_ADDR);
#define set_uart_iomux_tx(ID, MODE) \
	set_uart_iomux(UART##ID##_TX_M##MODE##_OFFSET, UART##ID##_TX_M##MODE, UART##ID##_TX_M##MODE##_ADDR);

void board_debug_uart_init(void)
{
/* UART 0 */
#if defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xff210000)

#if defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 0)

	/* UART0_M0 Switch iomux */
	set_uart_iomux_rx(0, 0);
	set_uart_iomux_tx(0, 0);
#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 1)

	/* UART0_M1 Switch iomux */
	set_uart_iomux_rx(0, 1);
	set_uart_iomux_tx(0, 1);
#endif
/* UART 1 */
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xff670000)

#if defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 0)

	/* UART1_M0 Switch iomux */
	set_uart_iomux_rx(1, 0);
	set_uart_iomux_tx(1, 0);
#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 1)

	/* UART1_M1 Switch iomux */
	set_uart_iomux_rx(1, 1);
	set_uart_iomux_tx(1, 1);
#endif
/* UART 2 */
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xff680000)

#if defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 0)

	/* UART2_M0 Switch iomux */
	set_uart_iomux_rx(2, 0);
	set_uart_iomux_tx(2, 0);
#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 1)

	/* UART2_M1 Switch iomux */
	set_uart_iomux_rx(2, 1);
	set_uart_iomux_tx(2, 1);
#endif
/* UART 3 */
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xff690000)
#if defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 0)

	/* UART3_M0 Switch iomux */
	set_uart_iomux_rx(3, 0);
	set_uart_iomux_tx(3, 0);
#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 1)

	/* UART3_M1 Switch iomux */
	set_uart_iomux_rx(3, 1);
	set_uart_iomux_tx(3, 1);
#endif
/* UART 4 */
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xff6a0000)
#if defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 0)

	/* UART4_M0 Switch iomux */
	set_uart_iomux_rx(4, 0);
	set_uart_iomux_tx(4, 0);
#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 1)

	/* UART4_M1 Switch iomux */
	set_uart_iomux_rx(4, 1);
	set_uart_iomux_tx(4, 1);
#endif
/* UART 5 */
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xff6b0000)
#if defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 0)

	/* UART5_M0 Switch iomux */
	set_uart_iomux_rx(5, 0);
	set_uart_iomux_tx(5, 0);
#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 1)

	/* UART5_M1 Switch iomux */
	set_uart_iomux_rx(5, 1);
	set_uart_iomux_tx(5, 1);
#endif
/* UART 6 */
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xff6c0000)
#if defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 0)

	/* UART6_M0 Switch iomux */
	set_uart_iomux_rx(6, 0);
	set_uart_iomux_tx(6, 0);
#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 1)

	/* UART6_M1 Switch iomux */
	set_uart_iomux_rx(6, 1);
	set_uart_iomux_tx(6, 1);
#endif
/* UART 7 */
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xff6d0000)
#if defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 0)

	/* UART7_M0 Switch iomux */
	set_uart_iomux_rx(7, 0);
	set_uart_iomux_tx(7, 0);
#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 1)

	/* UART7_M1 Switch iomux */
	set_uart_iomux_rx(7, 1);
	set_uart_iomux_tx(7, 1);
#endif
/* UART 8 */
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xff6e0000)
#if defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 0)

	/* UART8_M0 Switch iomux */
	set_uart_iomux_rx(8, 0);
	set_uart_iomux_tx(8, 0);
#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 1)

	/* UART8_M1 Switch iomux */
	set_uart_iomux_rx(8, 1);
	set_uart_iomux_tx(8, 1);
#endif
/* UART 9 */
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xff6f0000)
#if defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 0)

	/* UART9_M0 Switch iomux */
	set_uart_iomux_rx(9, 0);
	set_uart_iomux_tx(9, 0);
#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 1)

	/* UART9_M1 Switch iomux */
	set_uart_iomux_rx(9, 1);
	set_uart_iomux_tx(9, 1);
#endif
#endif
}

int fit_standalone_release(char *id, uintptr_t entry_point)
{
	/* bus m0 configuration: */
	/* open hclk_dcache / hclk_icache / clk_bus m0 rtc / fclk_bus_m0_core */
	writel(0x03180000, TOP_CRU_BASE + TOP_CRU_GATE_CON23);

	/* open bus m0 sclk / bus m0 hclk / bus m0 dclk */
	writel(0x00070000, TOP_CRU_BASE + TOP_CRU_CM0_GATEMASK);

	/*
	 * mcu_cache_peripheral_addr
	 * The uncache area ranges from 0x7c00000 to 0xffb400000
	 * and contains rpmsg shared memory
	 */
	writel(0x07c00000, SYS_GRF_BASE + SYS_GRF_SOC_CON5);
	writel(0xffb40000, SYS_GRF_BASE + SYS_GRF_SOC_CON6);

	sip_smc_mcu_config(ROCKCHIP_SIP_CONFIG_BUSMCU_0_ID,
			   ROCKCHIP_SIP_CONFIG_MCU_CODE_START_ADDR,
			   0xffff0000 | (entry_point >> 16));

	/* release dcache / icache / bus m0 jtag / bus m0 */
	writel(0x03280000, TOP_CRU_BASE + TOP_CRU_SOFTRST_CON23);

	/* release pmu m0 jtag / pmu m0 */
	/* writel(0x00050000, PMU1_CRU_BASE + PMU1_CRU_SOFTRST_CON02); */

	return 0;
}

void board_set_iomux(enum if_type if_type, int devnum, int routing)
{
	switch (if_type) {
	case IF_TYPE_MTD:
		/* FSPI */
		writel(0xffff2222, GPIO1_IOC_BASE + GPIO1A_IOMUX_SEL_L);
		writel(0x00ff0022, GPIO1_IOC_BASE + GPIO1B_IOMUX_SEL_L);
		break;
	default:
		printf("Bootdev 0x%x is not support\n", if_type);
	}
}

#if defined(CONFIG_SPL_BUILD) && !defined(CONFIG_TPL_BUILD)
static void qos_priority_init(void)
{
	u32 delay;
	u32 i;

	/* power up vo,vi */
	rk_clrreg(PMU_BASE_ADDR + PMU2_PWR_GATE_SFTCON0,
		  PD_VO_DWN_SFTENA | PD_VI_DWN_SFTENA);
	delay = 1000;
	do {
		udelay(1);
		delay--;
		if (delay == 0) {
			printf("Fail to set domain. PMU2_PWR_GATE_STS0=0x%x\n",
			       readl(PMU_BASE_ADDR + PMU2_PWR_GATE_STS0));
			hang();
		}
	} while (readl(PMU_BASE_ADDR + PMU2_PWR_GATE_STS0) &
		 (PD_VO_DWN_SFTENA | PD_VI_DWN_SFTENA));
	/* power up vop memory */
	for (i = 0; i < 16; i++)
		rk_clrreg(PMU_BASE_ADDR + PMU2_MEM_SD_SFTCON0, BIT(i));

	/* release vo,vi,gpu idle request */
	rk_clrreg(PMU_BASE_ADDR + PMU2_BIU_IDLE_SFTCON0,
		  (IDLE_REQ_VO_SFTENA | IDLE_REQ_VI_SFTENA));

	delay = 1000;
	/* wait ack status */
	do {
		udelay(1);
		delay--;
		if (delay == 0) {
			printf("Fail to get ack on domain. PMU2_BIU_IDLE_ACK_STS0=0x%x\n",
			       readl(PMU_BASE_ADDR + PMU2_BIU_IDLE_ACK_STS0));
			hang();
		}
	} while (readl(PMU_BASE_ADDR + PMU2_BIU_IDLE_ACK_STS0) &
		 (IDLE_ACK_VO | IDLE_ACK_VI));

	delay = 1000;
	/* wait idle status */
	do {
		udelay(1);
		delay--;
		if (delay == 0) {
			printf("Fail to set idle on domain. PMU2_BIT_IDLE_STS0=0x%x\n",
			       readl(PMU_BASE_ADDR + PMU2_BIT_IDLE_STS0));
			hang();
		}
	} while (readl(PMU_BASE_ADDR + PMU2_BIT_IDLE_STS0) &
		 (IDLE_VO | IDLE_VI));

	/*
	 * modify default qos priority setting, then
	 * Peri > VOP/ISP/VICAP > CPU > GPU/NPU/RKVDEC/RGA/Other
	 * (5)    (4)             (3)   (2)
	 *
	 * NOTE: GPU qos init is in kernel, in case that vdd gpu is off now.
	 */
	writel(QOS_PRIORITY_LEVEL(5, 5), CRYPTO_PRIORITY_REG);
	writel(QOS_PRIORITY_LEVEL(5, 5), DMAC_PRIORITY_REG);
	writel(QOS_PRIORITY_LEVEL(5, 5), EMMC_PRIORITY_REG);
	writel(QOS_PRIORITY_LEVEL(5, 5), FSPI_PRIORITY_REG);
	writel(QOS_PRIORITY_LEVEL(5, 5), GMAC_PRIORITY_REG);
	writel(QOS_PRIORITY_LEVEL(5, 5), MAC100_PRIORITY_REG);
	writel(QOS_PRIORITY_LEVEL(5, 5), MCU_PRIORITY_REG);
	writel(QOS_PRIORITY_LEVEL(5, 5), RKDMA_PRIORITY_REG);
	writel(QOS_PRIORITY_LEVEL(5, 5), SDMMC0_PRIORITY_REG);
	writel(QOS_PRIORITY_LEVEL(5, 5), SDMMC1_PRIORITY_REG);
	writel(QOS_PRIORITY_LEVEL(5, 5), USB2_PRIORITY_REG);
	writel(QOS_PRIORITY_LEVEL(5, 5), USB3_PRIORITY_REG);
	writel(QOS_PRIORITY_LEVEL(4, 4), ISP_PRIORITY_REG);
	writel(QOS_PRIORITY_LEVEL(4, 4), VICAP_PRIORITY_REG);
	writel(QOS_PRIORITY_LEVEL(4, 4), VOP_PRIORITY_REG);
	writel(QOS_PRIORITY_LEVEL(2, 2), DCF_PRIORITY_REG);
	writel(QOS_PRIORITY_LEVEL(2, 2), DMA2DDR_PRIORITY_REG);
	writel(QOS_PRIORITY_LEVEL(2, 2), PCIE_PRIORITY_REG);

	writel(0x5, PCIE_SHAPING_REG);
}
#endif

int arch_cpu_init(void)
{
#if defined(CONFIG_SPL_BUILD) || defined(CONFIG_SUPPORT_USBPLUG)
	u32 val;

	/* Set the emmc to access ddr memory */
	val = readl(FIREWALL_DDR_BASE + FW_DDR_MST4_REG);
	writel(val & 0x0000ffff, FIREWALL_DDR_BASE + FW_DDR_MST4_REG);

	/* Set the sdmmc to access ddr memory */
	val = readl(FIREWALL_DDR_BASE + FW_DDR_MST6_REG);
	writel(val & 0xff0000ff, FIREWALL_DDR_BASE + FW_DDR_MST6_REG);

	/*
	 * Set SAIx_MCLK as input default
	 *
	 * It's safe to set mclk as input default to avoid high freq glitch
	 * which may make devices work unexpected. And then enabled by
	 * kernel stage or any state where user use it.
	 */
	writel(0x0a100000, PERI_GRF_BASE + PERI_GRF_AUDIO_CON);

	/* Assert reset the pipe phy to save power and de-assert when in use */
	writel(0x00030001, PIPEPHY_GRF_BASE + PIPEPHY_PIPE_CON5);

#if defined(CONFIG_SUPPORT_USBPLUG)
	/* Set emmc iomux */
	writel(0xffff1111, GPIO1_IOC_BASE + GPIO1A_IOMUX_SEL_L);
	writel(0xffff1111, GPIO1_IOC_BASE + GPIO1A_IOMUX_SEL_H);
	writel(0xffff1111, GPIO1_IOC_BASE + GPIO1B_IOMUX_SEL_L);
#endif

#if defined(CONFIG_ROCKCHIP_SFC)
	/* Set the fspi to access ddr memory */
	val = readl(FIREWALL_DDR_BASE + FW_DDR_MST5_REG);
	writel(val & 0x00ffffff, FIREWALL_DDR_BASE + FW_DDR_MST5_REG);

	/*
	 * Fix fspi io ds level:
	 *
	 * level 2 for 1V8
	 * level 3 for 3V3
	 */
	if (readl(GPIO1_IOC_BASE + GPIO1A_IOMUX_SEL_L) == 0x2222) {
		if (readl(GPIO2_IOC_BASE + GPIO2_IOC_IO_VSEL0) & POC_VCCIO2_VD_3V3) {
			writel(0x3f3f0f0f, GPIO1_IOC_BASE + GPIO1_IOC_GPIO1A_DS0);
			writel(0x3f3f0f0f, GPIO1_IOC_BASE + GPIO1_IOC_GPIO1A_DS1);
			writel(0x3f3f0f0f, GPIO1_IOC_BASE + GPIO1_IOC_GPIO1B_DS0);
		} else {
			writel(0x3f3f0707, GPIO1_IOC_BASE + GPIO1_IOC_GPIO1A_DS0);
			writel(0x3f3f0707, GPIO1_IOC_BASE + GPIO1_IOC_GPIO1A_DS1);
			writel(0x3f3f0707, GPIO1_IOC_BASE + GPIO1_IOC_GPIO1B_DS0);
		}
	}
#endif

#if !defined(CONFIG_TPL_BUILD)
	qos_priority_init();
#endif
#endif /* #if defined(CONFIG_SPL_BUILD) || defined(CONFIG_SUPPORT_USBPLUG) */

	return 0;
}
