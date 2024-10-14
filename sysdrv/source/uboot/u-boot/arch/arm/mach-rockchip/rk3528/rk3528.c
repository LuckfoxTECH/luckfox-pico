/*
 * Copyright (c) 2020 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#include <common.h>
#include <dm.h>
#include <misc.h>
#include <asm/io.h>
#include <asm/arch/cpu.h>
#include <asm/arch/hardware.h>
#include <asm/arch/grf_rk3528.h>
#include <asm/arch/ioc_rk3528.h>

DECLARE_GLOBAL_DATA_PTR;

#define FIREWALL_DDR_BASE	0xff2e0000
#define FW_DDR_MST1_REG 	0x44
#define FW_DDR_MST6_REG 	0x58
#define FW_DDR_MST7_REG 	0x5c
#define FW_DDR_MST11_REG 	0x6c
#define FW_DDR_MST14_REG	0x78
#define FW_DDR_MST16_REG 	0x80
#define FW_DDR_MST_REG		0xf0

#define VENC_GRF_BASE		0xff320000
#define VENC_GRF_CON1		0x4

#define VPU_GRF_BASE		0xff340000
#define VPU_GRF_CON4		0x14

#define PMU_SGRF_BASE		0xff440000
#define PMU_SGRF_SOC_CON4	0x10
#define PMU_SGRF_SOC_CON5	0x14
#define PMU_SGRF_SOC_CON6	0x18
#define PMU_SGRF_SOC_CON8	0x20
#define PMU_SGRF_SOC_CON11	0x2c

#define PMU_CRU_BASE		0xff4b0000
#define PMU_CRU_GATE_CON00	0x800
#define PMU_CRU_SOFTRST_CON00	0xa00

#define GPIO1C_IOMUX_SEL_H	0x034
#define GPIO1D_IOMUX_SEL_L	0x038
#define GPIO1D_IOMUX_SEL_H	0x03c

#define CPU_PRIORITY_REG	0xff210008
#define QOS_PRIORITY_LEVEL(h, l)	((((h) & 7) << 8) | ((l) & 7))

#ifdef CONFIG_ARM64
#include <asm/armv8/mmu.h>

static struct mm_region rk3528_mem_map[] = {
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
		/* List terminator */
		0,
	}
};

struct mm_region *mem_map = rk3528_mem_map;
#endif

#define	GPIO0_IOC_BASE			0xFF540000
#define	GPIO1_IOC_BASE			0xFF560000
#define	GPIO2_IOC_BASE			0xFF570000
#define	GPIO3_IOC_BASE			0xFF560000
#define	GPIO4_IOC_BASE			0xFF550000

#define GPIO1_IOC_GPIO1D_IOMUX_SEL_L	(GPIO1_IOC_BASE + 0x38)
#define GPIO1_IOC_GPIO1C_DS_2		(GPIO1_IOC_BASE + 0x148)
#define GPIO1_IOC_GPIO1C_DS_3		(GPIO1_IOC_BASE + 0x14C)
#define GPIO1_IOC_GPIO1D_DS_0		(GPIO1_IOC_BASE + 0x150)
#define GPIO1_IOC_GPIO1D_DS_1		(GPIO1_IOC_BASE + 0x154)
#define GPIO1_IOC_GPIO1D_DS_2		(GPIO1_IOC_BASE + 0x158)

/* uart0 iomux */
/* gpio4c7 */
#define UART0_RX_M0			1
#define UART0_RX_M0_OFFSET		12
#define UART0_RX_M0_ADDR		(GPIO4_IOC_BASE + 0x94)
/* gpio4d0 */
#define UART0_TX_M0			1
#define UART0_TX_M0_OFFSET		0
#define UART0_TX_M0_ADDR		(GPIO4_IOC_BASE + 0x98)

/* gpio2a0 */
#define UART0_RX_M1			2
#define UART0_RX_M1_OFFSET		0
#define UART0_RX_M1_ADDR		(GPIO2_IOC_BASE + 0x40)
/* gpio2a1 */
#define UART0_TX_M1			2
#define UART0_TX_M1_OFFSET		4
#define UART0_TX_M1_ADDR		(GPIO2_IOC_BASE + 0x40)

/* uart1 iomux */
/* gpio4a7 */
#define UART1_RX_M0			2
#define UART1_RX_M0_OFFSET		12
#define UART1_RX_M0_ADDR		(GPIO1_IOC_BASE + 0x84)
/* gpio4a6 */
#define UART1_TX_M0			2
#define UART1_TX_M0_OFFSET		8
#define UART1_TX_M0_ADDR		(GPIO1_IOC_BASE + 0x84)

/* gpio4c6 */
#define UART1_RX_M1			2
#define UART1_RX_M1_OFFSET		8
#define UART1_RX_M1_ADDR		(GPIO4_IOC_BASE + 0x94)
/* gpio4c5 */
#define UART1_TX_M1			2
#define UART1_TX_M1_OFFSET		4
#define UART1_TX_M1_ADDR		(GPIO4_IOC_BASE + 0x94)

/* uart2 iomux */
/* gpio3a0 */
#define UART2_RX_M0			1
#define UART2_RX_M0_OFFSET		0
#define UART2_RX_M0_ADDR		(GPIO3_IOC_BASE + 0x60)
/* gpio3a1 */
#define UART2_TX_M0			1
#define UART2_TX_M0_OFFSET		4
#define UART2_TX_M0_ADDR		(GPIO3_IOC_BASE + 0x60)

/* gpio1b0 */
#define UART2_RX_M1			1
#define UART2_RX_M1_OFFSET		0
#define UART2_RX_M1_ADDR		(GPIO1_IOC_BASE + 0x28)
/* gpio1b1 */
#define UART2_TX_M1			1
#define UART2_TX_M1_OFFSET		4
#define UART2_TX_M1_ADDR		(GPIO1_IOC_BASE + 0x28)

/* uart3 iomux */
/* gpio4b0 */
#define UART3_RX_M0			2
#define UART3_RX_M0_OFFSET		0
#define UART3_RX_M0_ADDR		(GPIO4_IOC_BASE + 0x88)
/* gpio4b1 */
#define UART3_TX_M0			2
#define UART3_TX_M0_OFFSET		4
#define UART3_TX_M0_ADDR		(GPIO4_IOC_BASE + 0x88)

/* gpio4b7 */
#define UART3_RX_M1			3
#define UART3_RX_M1_OFFSET		12
#define UART3_RX_M1_ADDR		(GPIO4_IOC_BASE + 0x8C)
/* gpio4c0 */
#define UART3_TX_M1			3
#define UART3_TX_M1_OFFSET		0
#define UART3_TX_M1_ADDR		(GPIO4_IOC_BASE + 0x90)

/* uart4 iomux */
/* gpio2a2 */
#define UART4_RX_M0			3
#define UART4_RX_M0_OFFSET		8
#define UART4_RX_M0_ADDR		(GPIO2_IOC_BASE + 0x40)
/* gpio2a3 */
#define UART4_TX_M0			3
#define UART4_TX_M0_OFFSET		12
#define UART4_TX_M0_ADDR		(GPIO2_IOC_BASE + 0x40)

/* uart5 iomux */
/* gpio1a2 */
#define UART5_RX_M0			2
#define UART5_RX_M0_OFFSET		8
#define UART5_RX_M0_ADDR		(GPIO1_IOC_BASE + 0x20)
/* gpio1a3 */
#define UART5_TX_M0			2
#define UART5_TX_M0_OFFSET		12
#define UART5_TX_M0_ADDR		(GPIO1_IOC_BASE + 0x20)

/* gpio1d4 */
#define UART5_RX_M1			2
#define UART5_RX_M1_OFFSET		0
#define UART5_RX_M1_ADDR		(GPIO1_IOC_BASE + 0x3c)
/* gpio1d7 */
#define UART5_TX_M1			2
#define UART5_TX_M1_OFFSET		12
#define UART5_TX_M1_ADDR		(GPIO1_IOC_BASE + 0x3c)

/* uart6 iomux */
/* gpio3a7 */
#define UART6_RX_M0			4
#define UART6_RX_M0_OFFSET		12
#define UART6_RX_M0_ADDR		(GPIO3_IOC_BASE + 0x64)
/* gpio3a6 */
#define UART6_TX_M0			4
#define UART6_TX_M0_OFFSET		8
#define UART6_TX_M0_ADDR		(GPIO3_IOC_BASE + 0x64)

/* gpio3c3 */
#define UART6_RX_M1			4
#define UART6_RX_M1_OFFSET		12
#define UART6_RX_M1_ADDR		(GPIO3_IOC_BASE + 0x70)
/* gpio3c1 */
#define UART6_TX_M1			4
#define UART6_TX_M1_OFFSET		4
#define UART6_TX_M1_ADDR		(GPIO3_IOC_BASE + 0x70)

/* uart7 iomux */
/* gpio3b3 */
#define UART7_RX_M0			4
#define UART7_RX_M0_OFFSET		12
#define UART7_RX_M0_ADDR		(GPIO3_IOC_BASE + 0x68)
/* gpio3b2 */
#define UART7_TX_M0			4
#define UART7_TX_M0_OFFSET		8
#define UART7_TX_M0_ADDR		(GPIO3_IOC_BASE + 0x68)

/* gpio1b3 */
#define UART7_RX_M1			4
#define UART7_RX_M1_OFFSET		12
#define UART7_RX_M1_ADDR		(GPIO1_IOC_BASE + 0x28)
/* gpio1b2 */
#define UART7_TX_M1			4
#define UART7_TX_M1_OFFSET		8
#define UART7_TX_M1_ADDR		(GPIO1_IOC_BASE + 0x28)


#define set_uart_iomux(bits_offset, bits_val, addr) \
	writel(GENMASK(bits_offset + 19, bits_offset + 16) | (bits_val << bits_offset) , addr)

#define set_uart_iomux_rx(ID, MODE) \
	set_uart_iomux(UART##ID##_RX_M##MODE##_OFFSET, UART##ID##_RX_M##MODE, UART##ID##_RX_M##MODE##_ADDR);
#define set_uart_iomux_tx(ID, MODE) \
	set_uart_iomux(UART##ID##_TX_M##MODE##_OFFSET, UART##ID##_TX_M##MODE, UART##ID##_TX_M##MODE##_ADDR);

void board_debug_uart_init(void)
{
/* UART 0 */
#if defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xff9f0000)
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
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xff9f8000)
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
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xffa00000)
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
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xffa08000)
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
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xffa10000)
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
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xffa18000)
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
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xffa20000)
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
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xffa28000)
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
#endif
}

#ifdef CONFIG_SPL_BUILD
void rockchip_stimer_init(void)
{
	/* If Timer already enabled, don't re-init it */
	u32 reg = readl(CONFIG_ROCKCHIP_STIMER_BASE + 0x4);

	if (reg & 0x1)
		return;

	asm volatile("msr CNTFRQ_EL0, %0" : : "r" (COUNTER_FREQUENCY));
	writel(0xffffffff, CONFIG_ROCKCHIP_STIMER_BASE + 0x14);
	writel(0xffffffff, CONFIG_ROCKCHIP_STIMER_BASE + 0x18);
	dsb();
	writel(0x1, CONFIG_ROCKCHIP_STIMER_BASE + 0x4);
}
#endif

int arch_cpu_init(void)
{
#if defined(CONFIG_SPL_BUILD)
	u32 val;

	/*
	 * Select clk_tx source as default for i2s2/i2s3
	 * Set I2Sx_MCLK as input default
	 *
	 * It's safe to set mclk as input default to avoid high freq glitch
	 * which may make devices work unexpected. And then enabled by
	 * kernel stage or any state where user use it.
	 */
	writel(0x00020002, VPU_GRF_BASE + VPU_GRF_CON4);
	writel(0x40004000, VENC_GRF_BASE + VENC_GRF_CON1);

	/* Set the emmc to access ddr memory */
	val = readl(FIREWALL_DDR_BASE + FW_DDR_MST6_REG);
	writel(val & 0x0000ffff, FIREWALL_DDR_BASE + FW_DDR_MST6_REG);

	/* Set the sdmmc to access ddr memory */
	val = readl(FIREWALL_DDR_BASE + FW_DDR_MST14_REG);
	writel(val & 0x0000ffff, FIREWALL_DDR_BASE + FW_DDR_MST14_REG);

	/* Set the crypto to access ddr memory */
	val = readl(FIREWALL_DDR_BASE + FW_DDR_MST1_REG);
	writel(val & 0x0000ffff, FIREWALL_DDR_BASE + FW_DDR_MST1_REG);

#if defined(CONFIG_ROCKCHIP_SFC)
	/* Set the fspi to access ddr memory */
	val = readl(FIREWALL_DDR_BASE + FW_DDR_MST7_REG);
	writel(val & 0xffff0000, FIREWALL_DDR_BASE + FW_DDR_MST7_REG);
#endif

#ifndef CONFIG_TPL_BUILD
	/* Set cpu qos priority, then
	 * Peri > VOP > CPU = RKVDEC/RKVENC/VPU > GPU/RGA/Other
	 */
	writel(QOS_PRIORITY_LEVEL(2, 2), CPU_PRIORITY_REG);
#endif

	if (readl(GPIO1_IOC_GPIO1D_IOMUX_SEL_L) == 0x1111) {
	       /*
		* set the emmc io drive strength:
		* data and cmd: level 3
		* clock: level 5
		*/
	       writel(0x3F3F0F0F, GPIO1_IOC_GPIO1C_DS_2);
	       writel(0x3F3F0F0F, GPIO1_IOC_GPIO1C_DS_3);
	       writel(0x3F3F0F0F, GPIO1_IOC_GPIO1D_DS_0);
	       writel(0x3F3F0F0F, GPIO1_IOC_GPIO1D_DS_1);
	       writel(0x3F3F3F0F, GPIO1_IOC_GPIO1D_DS_2);
	}

#elif defined(CONFIG_SUPPORT_USBPLUG)
	u32 val;

	/* Set the usb to access ddr memory */
	val = readl(FIREWALL_DDR_BASE + FW_DDR_MST16_REG);
	writel(val & 0xffff0000, FIREWALL_DDR_BASE + FW_DDR_MST16_REG);

	/* Set the emmc to access ddr memory */
	val = readl(FIREWALL_DDR_BASE + FW_DDR_MST6_REG);
	writel(val & 0x0000ffff, FIREWALL_DDR_BASE + FW_DDR_MST6_REG);

	/* Set emmc iomux */
	writel(0xffff1111, GPIO1_IOC_BASE + GPIO1C_IOMUX_SEL_H);
	writel(0xffff1111, GPIO1_IOC_BASE + GPIO1D_IOMUX_SEL_L);
	writel(0xffff1111, GPIO1_IOC_BASE + GPIO1D_IOMUX_SEL_H);

#if defined(CONFIG_ROCKCHIP_SFC)
	/* Set the fspi to access ddr memory */
	val = readl(FIREWALL_DDR_BASE + FW_DDR_MST7_REG);
	writel(val & 0xFFFF0000uL, FIREWALL_DDR_BASE + FW_DDR_MST7_REG);

	/* Set fspi iomux */
	writel(0xffff2222, GPIO1_IOC_BASE + GPIO1C_IOMUX_SEL_H);
	writel(0x000f0002, GPIO1_IOC_BASE + GPIO1D_IOMUX_SEL_L);
	writel(0x00f00020, GPIO1_IOC_BASE + GPIO1D_IOMUX_SEL_H);
#endif

#endif
	return 0;
}

#ifdef CONFIG_SPL_BUILD
int spl_fit_standalone_release(char *id, uintptr_t entry_point)
{
	u32 val;

	/* open clk_pmu_mcu_jtag / clk_mcu_32k_en / fclk_mcu_en */
	writel(0x05800000, PMU_CRU_BASE + PMU_CRU_GATE_CON00);
	/* set the mcu to access ddr memory */
	val = readl(FIREWALL_DDR_BASE + FW_DDR_MST11_REG);
	writel(val & 0x0000ffff, FIREWALL_DDR_BASE + FW_DDR_MST11_REG);
	/* writel(0x00000000, FIREWALL_DDR_BASE + FW_DDR_MST_REG); */
	/* set the mcu to secure */
	writel(0x00200000, PMU_SGRF_BASE + PMU_SGRF_SOC_CON4);
	/* open mcu_debug_en / mcu_dclk_en / mcu_hclk_en / mcu_sclk_en */
	writel(0x000f000f, PMU_SGRF_BASE + PMU_SGRF_SOC_CON5);
	/* set start addr, mcu_code_addr_start */
	writel(0xffff0000 | (entry_point >> 16), PMU_SGRF_BASE + PMU_SGRF_SOC_CON6);
	/* mcu_tcm_addr_start, multiplex pmu sram address */
	writel(0xffffff10, PMU_SGRF_BASE + PMU_SGRF_SOC_CON11);
	/* jtag_mcu_m0 gpio2a4/gpio2a5 iomux */
	/* writel(0x00ff0022, GPIO2_IOC_BASE + 0x44); */
	/* release the mcu */
	writel(0x00800000, PMU_CRU_BASE + PMU_CRU_SOFTRST_CON00);

	return 0;
}
#endif

#define CHIP_TYPE_OFF	40

static int fdt_fixup_modules(void *blob)
{
	struct udevice *dev;
	char *compat;
	u8 chip_type;
	int ret;

	ret = uclass_get_device_by_driver(UCLASS_MISC,
					  DM_GET_DRIVER(rockchip_otp), &dev);
	if (ret) {
		printf("can't get otp device, ret=%d\n", ret);
		return ret;
	}

	ret = misc_read(dev, CHIP_TYPE_OFF, &chip_type, 1);
	if (ret) {
		printf("can't read chip type, ret=%d\n", ret);
		return ret;
	}

	compat = (char *)fdt_getprop(blob, 0, "compatible", NULL);
	fdt_setprop_string(blob, 0, "compatible", compat);

	if (chip_type == 0x1)
		fdt_appendprop_string(blob, 0, "compatible", "rockchip,rk3528");
	else
		fdt_appendprop_string(blob, 0, "compatible", "rockchip,rk3528a");

	return 0;
}

int rk_board_dm_fdt_fixup(const void *blob)
{
	return fdt_fixup_modules((void *)blob);
}
