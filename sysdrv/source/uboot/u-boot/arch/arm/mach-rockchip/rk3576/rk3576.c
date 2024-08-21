/*
 * Copyright (c) 2023 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#include <common.h>
#include <boot_rkimg.h>
#include <dm.h>
#include <fdt_support.h>
#include <misc.h>
#include <mmc.h>
#include <scsi.h>
#include <spl.h>
#include <asm/io.h>
#include <asm/arch/bootrom.h>
#include <asm/arch/cpu.h>
#include <asm/arch/hardware.h>
#include <asm/arch/ioc_rk3576.h>
#include <asm/system.h>

DECLARE_GLOBAL_DATA_PTR;

#define SYS_GRF_BASE		0x2600A000
#define SYS_GRF_SOC_CON2	0x0008
#define SYS_GRF_SOC_CON11	0x002c
#define SYS_GRF_SOC_CON12	0x0030

#define GPIO0_IOC_BASE		0x26040000
#define GPIO0B_PULL_L		0x0024
#define GPIO0B_IE_L		0x002C

#define TOP_IOC_BASE		0x26044000
#define GPIO1A_IOMUX_SEL_L	0x0020
#define GPIO1A_IOMUX_SEL_H	0x0024
#define GPIO1B_IOMUX_SEL_L	0x0028
#define GPIO1B_IOMUX_SEL_H	0x002c
#define GPIO1C_IOMUX_SEL_L	0x0030
#define GPIO1C_IOMUX_SEL_H	0x0034
#define GPIO1D_IOMUX_SEL_L	0x0038
#define GPIO1D_IOMUX_SEL_H	0x003c
#define GPIO2A_IOMUX_SEL_L	0x0040
#define GPIO2A_IOMUX_SEL_H	0x0044

#define VCCIO_IOC_BASE		0x26046000
#define VCCIO_IOC_GPIO1C_PUL	0x118
#define VCCIO_IOC_GPIO1D_PUL	0x11C
#define VCCIO_IOC_GPIO2A_PUL	0x120

#define VCCIO6_IOC_BASE		0x2604a000
#define VCCIO7_IOC_BASE		0x2604b000
#define VCCIO7_IOC_GPIO4D_IOMUX_SEL_L	0x0398
#define VCCIO7_IOC_XIN_UFS_CON	0x0400

#define PMU1_SGRF_BASE		0x26002000
#define PMU1_SGRF_SOC_CON10	0x0028
#define PMU1_SGRF_SOC_CON11	0x002C

#define SYS_SGRF_BASE		0x26004000
#define SYS_SGRF_SOC_CON14	0x0058
#define SYS_SGRF_SOC_CON15	0x005C
#define SYS_SGRF_SOC_CON20	0x0070

#define FW_SYS_SGRF_BASE	0x26005000
#define SGRF_DOMAIN_CON1	0x4
#define SGRF_DOMAIN_CON2	0x8
#define SGRF_DOMAIN_CON3	0xc
#define SGRF_DOMAIN_CON4	0x10
#define SGRF_DOMAIN_CON5	0x14

#define USBGRF_BASE		0x2601e000
#define USB_GRF_USB3OTG0_CON1	0x0030

#define USB2PHY0_GRF_BASE	0x2602e000
#define USB2PHY1_GRF_BASE	0x26030000
#define USB2PHY_GRF_CON4	0x0010
#define USB2PHY_GRF_DBG_CON	0x0040
#define USB2PHY_GRF_LS_TIMEOUT	0x0044
#define USB2PHY_GRF_LS_DEB	0x0048
#define USB2PHY_GRF_RX_TIMEOUT	0x004c
#define USB2PHY_GRF_SEQ_LIMT	0x0050

#define TOP_CRU_BASE		0x27200000
#define TOP_CRU_GATE_CON19	0x084C
#define TOP_CRU_SOFTRST_CON19	0x0a4C
#define PHPPHYSOFTRST_CON01	0x8a04

#define PMU1_CRU_BASE		0x27220000
#define PMU1_CRU_CLKSEL_CON03	0x030c
#define PMU1_CRU_GATE_CON03	0x080C
#define PMU1_CRU_SOFTRST_CON03	0x0a0C

#ifdef CONFIG_ARM64
#include <asm/armv8/mmu.h>

static struct mm_region rk3576_mem_map[] = {
	{
		.virt = 0x20000000UL,
		.phys = 0x20000000UL,
		.size = 0xb080000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			 PTE_BLOCK_NON_SHARE |
			 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	}, {
		.virt = 0x3fe70000UL,
		.phys = 0x3fe70000UL,
		.size = 0x190000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			 PTE_BLOCK_NON_SHARE |
			 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	}, {
		.virt = 0x40000000UL,
		.phys = 0x40000000UL,
		.size = 0x100000000UL - 0x40000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) |
			 PTE_BLOCK_INNER_SHARE
	}, {
		.virt = 0x100000000UL,
		.phys = 0x100000000UL,
		.size = 0x300000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) |
			 PTE_BLOCK_INNER_SHARE
	}, {
		.virt = 0x900000000UL,
		.phys = 0x900000000UL,
		.size = 0x100800000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			 PTE_BLOCK_NON_SHARE |
			 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	}, {
		/* List terminator */
		0,
	}
};

struct mm_region *mem_map = rk3576_mem_map;
#endif

void board_debug_uart_init(void)
{
}

const char * const boot_devices[BROM_LAST_BOOTSOURCE + 1] = {
	[BROM_BOOTSOURCE_EMMC] = "/mmc@2a330000",
	[BROM_BOOTSOURCE_SD] = "/mmc@2a310000",
	[BROM_BOOTSOURCE_UFS] = "/ufs@2a2d0000",
};

#ifdef CONFIG_SPL_BUILD
void rockchip_stimer_init(void)
{
	u32 reg;

	/* If Timer already enabled, don't re-init it */
	reg = readl(CONFIG_ROCKCHIP_STIMER_BASE + 0x4);
	if (reg & 0x1)
		return;

	asm volatile("msr CNTFRQ_EL0, %0" : : "r" (COUNTER_FREQUENCY));
	writel(0xffffffff, CONFIG_ROCKCHIP_STIMER_BASE + 0x14);
	writel(0xffffffff, CONFIG_ROCKCHIP_STIMER_BASE + 0x18);
	writel(0x00010001, CONFIG_ROCKCHIP_STIMER_BASE + 0x04);
}
#endif

void board_set_iomux(enum if_type if_type, int devnum, int routing)
{
	switch (if_type) {
	case IF_TYPE_MMC:
		if (devnum == 0) {
			writel(0xffff1111, TOP_IOC_BASE + GPIO1A_IOMUX_SEL_L);
			writel(0xffff1111, TOP_IOC_BASE + GPIO1A_IOMUX_SEL_H);
			writel(0xffff1111, TOP_IOC_BASE + GPIO1B_IOMUX_SEL_L);
		} else if (devnum == 1) {
			writel(0xffff1111, TOP_IOC_BASE + GPIO2A_IOMUX_SEL_L);
			writel(0x00ff0011, TOP_IOC_BASE + GPIO2A_IOMUX_SEL_H);
			/* Pull up */
			writel(0x0FFF0FFF, VCCIO_IOC_BASE + VCCIO_IOC_GPIO2A_PUL);
		}
		break;
	case IF_TYPE_MTD:
		if (routing == 0) {
			/* FSPI0 M0 */
			writel(0xffff2222, TOP_IOC_BASE + GPIO1A_IOMUX_SEL_L);
			writel(0xffff2020, TOP_IOC_BASE + GPIO1B_IOMUX_SEL_L);
		} else if (routing == 1) {
			/* FSPI1 M0 */
			writel(0xffff2222, TOP_IOC_BASE + GPIO2A_IOMUX_SEL_L);
			writel(0x00ff0022, TOP_IOC_BASE + GPIO2A_IOMUX_SEL_H);
			/* Pull up */
			writel(0x03ff03ff, VCCIO_IOC_BASE + VCCIO_IOC_GPIO2A_PUL);
		} else if (routing == 2) {
			/* FSPI1 M1 */
			writel(0xf0003000, TOP_IOC_BASE + GPIO1C_IOMUX_SEL_L);
			writel(0xffff3333, TOP_IOC_BASE + GPIO1C_IOMUX_SEL_H);
			writel(0x00f00030, TOP_IOC_BASE + GPIO1D_IOMUX_SEL_H);
			/* Pull up */
			writel(0xffc0ffc0, VCCIO_IOC_BASE + VCCIO_IOC_GPIO1C_PUL);
		}
		break;
	default:
		printf("Bootdev 0x%x is not support\n", if_type);
	}
}

void board_unset_iomux(enum if_type if_type, int devnum, int routing)
{
	switch (if_type) {
	case IF_TYPE_MTD:
		if (routing == 0) {
			/* FSPI0 M0 -> GPIO */
			writel(0xffff0000, TOP_IOC_BASE + GPIO1A_IOMUX_SEL_L);
			writel(0xffff0000, TOP_IOC_BASE + GPIO1B_IOMUX_SEL_L);
		} else if (routing == 1) {
			/* FSPI1 M0 -> GPIO */
			writel(0xffff0000, TOP_IOC_BASE + GPIO2A_IOMUX_SEL_L);
			writel(0x00ff0000, TOP_IOC_BASE + GPIO2A_IOMUX_SEL_H);
		} else if (routing == 2) {
			/* FSPI1 M1 -> GPIO */
			writel(0xf0000000, TOP_IOC_BASE + GPIO1C_IOMUX_SEL_L);
			writel(0xffff0000, TOP_IOC_BASE + GPIO1C_IOMUX_SEL_H);
			writel(0x00f00000, TOP_IOC_BASE + GPIO1D_IOMUX_SEL_H);
		}
		break;
	default:
		break;
	}
}

int fit_standalone_release(char *id, uintptr_t entry_point)
{
	/*
	 * bus m0 configuration:
	 * open bus m0 rtc / core / biu / root
	 */
	writel(0x5c000000, TOP_CRU_BASE + TOP_CRU_GATE_CON19);

	/* select bus m0 jtag GPIO2A2 GPIO2A3 */
	/* writel(0x001f0010, 0x2600a01c); */
	/* writel(0xff009900, 0x26044040); */

	/* release bus m0 jtag / core / biu */
	writel(0x38000000, TOP_CRU_BASE + TOP_CRU_SOFTRST_CON19);

	/* pmu m0 configuration: */
	/* open pmu m0 rtc / core / biu / root */
	/* writel(0x59020000, PMU1_CRU_BASE + PMU1_CRU_GATE_CON03); */

	/* select pmu m0 jtag */
	/* writel(0x001f0008, 0x2600a01c); */
	/* writel(0xff009900, 0x26044040); */

	/* release pmu m0 jtag / core / biu */
	/* writel(0x38000000, PMU1_CRU_BASE + PMU1_CRU_SOFTRST_CON03); */

	return 0;
}

#ifndef CONFIG_TPL_BUILD
int arch_cpu_init(void)
{
#if defined(CONFIG_SPL_BUILD) || defined(CONFIG_SUPPORT_USBPLUG)
	u32 val;

	/* Set the emmc to access ddr memory */
	val = readl(FW_SYS_SGRF_BASE + SGRF_DOMAIN_CON2);
	writel(val | 0x7, FW_SYS_SGRF_BASE + SGRF_DOMAIN_CON2);

	/* Set the sdmmc0 to access ddr memory */
	val = readl(FW_SYS_SGRF_BASE + SGRF_DOMAIN_CON5);
	writel(val | 0x700, FW_SYS_SGRF_BASE + SGRF_DOMAIN_CON5);

	/* Set the UFS to access ddr memory */
	val = readl(FW_SYS_SGRF_BASE + SGRF_DOMAIN_CON3);
	writel(val | 0x70000, FW_SYS_SGRF_BASE + SGRF_DOMAIN_CON3);

	/* Set the fspi0 and fspi1 to access ddr memory */
	val = readl(FW_SYS_SGRF_BASE + SGRF_DOMAIN_CON4);
	writel(val | 0x7700, FW_SYS_SGRF_BASE + SGRF_DOMAIN_CON4);

	/* Set the decom to access ddr memory */
	val = readl(FW_SYS_SGRF_BASE + SGRF_DOMAIN_CON1);
	writel(val | 0x700, FW_SYS_SGRF_BASE + SGRF_DOMAIN_CON1);

	/* Set the sdmmc0 iomux */
	board_set_iomux(IF_TYPE_MMC, 1, 0);

	/* UFS PHY select 26M from ppll */
	writel(0x00030002, PMU1_CRU_BASE + PMU1_CRU_CLKSEL_CON03);

	/* set iomux UFS_REFCLK, UFS_RSTN */
	writel(0x00FF0011, VCCIO7_IOC_BASE + VCCIO7_IOC_GPIO4D_IOMUX_SEL_L);
	/* set UFS_RSTN to high */
	udelay(20);
	writel(0x00100010, VCCIO7_IOC_BASE + VCCIO7_IOC_XIN_UFS_CON);

	/*
	 * Set the GPIO0B0~B3 pull up and input enable.
	 * Keep consistent with other IO.
	 */
	writel(0x00ff00ff, GPIO0_IOC_BASE + GPIO0B_PULL_L);
	writel(0x000f000f, GPIO0_IOC_BASE + GPIO0B_IE_L);

	/*
	 * bus mcu_cache_peripheral_addr
	 * The uncache area ranges from 0x20000000 to 0x48200000
	 * and contains rpmsg shared memory
	 */
	writel(0x20000000, SYS_SGRF_BASE + SYS_SGRF_SOC_CON14);
	writel(0x48200000, SYS_SGRF_BASE + SYS_SGRF_SOC_CON15);

	/* TODO: bus mcu code addr need bl31 support */
	writel(0x48200000, 0x26004060);

	/*
	 * pmu mcu_cache_peripheral_addr
	 * The uncache area ranges from 0x20000000 to 0x48200000
	 * and contains rpmsg shared memory
	 */
	/* writel(0x20000000, PMU1_SGRF_BASE + PMU1_SGRF_SOC_CON10); */
	/* writel(0x48200000, PMU1_SGRF_BASE + PMU1_SGRF_SOC_CON11); */

	/* TODO: pmu mcu code addr need bl31 support */
	/* writel(0x48200000, 0x26002030); */

	/*
	 * Set SYS_GRF_SOC_CON2[12](input of pwm2_ch0) as 0,
	 * keep consistent with other pwm.
	 */
	writel(0x10000000, SYS_GRF_BASE + SYS_GRF_SOC_CON2);

	/*
	 * Assert reset the combophy0 and combophy1,
	 * de-assert reset in Kernel combophy driver.
	 */
	writel(0x01200120, TOP_CRU_BASE + PHPPHYSOFTRST_CON01);

	/*
	 * Assert SIDDQ for USB 2.0 PHY1 to power down
	 * PHY1 analog block to save power. And let the
	 * PHY0 for OTG0 interface still in normal mode.
	 */
	writel(0x20002000, USB2PHY1_GRF_BASE + USB2PHY_GRF_CON4);

	/*
	 * Enable USB to DEBUG
	 * 1. Set linestate timeout 8ms
	 * 2. Set linestate fiter time 500us
	 * 3. Set Rx timeout counter for RX pulldown 2s
	 * 4. Set handshake counter number for SE0 and
	 *    SE1 sequence at least 5.
	 */
	writel(0xff, USB2PHY0_GRF_BASE + USB2PHY_GRF_LS_TIMEOUT);
	writel(0x10, USB2PHY0_GRF_BASE + USB2PHY_GRF_LS_DEB);
	writel(0xffff, USB2PHY0_GRF_BASE + USB2PHY_GRF_RX_TIMEOUT);
	writel(0x05, USB2PHY0_GRF_BASE + USB2PHY_GRF_SEQ_LIMT);
	writel(0x00010001, USB2PHY0_GRF_BASE + USB2PHY_GRF_DBG_CON);

	/* Enable noc slave response timeout */
	writel(0x80008000, SYS_GRF_BASE + SYS_GRF_SOC_CON11);
	writel(0xffffffe0, SYS_GRF_BASE + SYS_GRF_SOC_CON12);

	/*
	 * Select usb otg0 pipe phy status to 0 that
	 * ensure rockusb can work at high-speed even
	 * if usb3 phy isn't ready.
	 */
	writel(0x000c0008, USBGRF_BASE + USB_GRF_USB3OTG0_CON1);

	/*
	 * Enable cci channels for below module AXI R/W
	 * Module: GMAC0/1, MMU0/1(PCIe, SATA, USB3)
	 */
	writel(0xffffff00, SYS_SGRF_BASE + SYS_SGRF_SOC_CON20);
#endif

#if defined(CONFIG_ROCKCHIP_EMMC_IOMUX)
	board_set_iomux(IF_TYPE_MMC, 0, 0);
#elif defined(CONFIG_ROCKCHIP_SFC_IOMUX)
	/*
	 * (IF_TYPE_MTD, 0, 0) FSPI0
	 * (IF_TYPE_MTD, 1, 0) FSPI1 M0
	 * (IF_TYPE_MTD, 2, 0) FSPI1 M1
	 */
	board_set_iomux(IF_TYPE_MTD, 0, 0);
#endif /* #if defined(CONFIG_ROCKCHIP_EMMC_IOMUX) */

	return 0;
}
#endif

#if defined(CONFIG_SCSI) && defined(CONFIG_CMD_SCSI) && defined(CONFIG_UFS)
int rk_board_dm_fdt_fixup(const void *blob)
{
	struct blk_desc *desc = rockchip_get_bootdev();
	const char *status = NULL;
	int node = -1;

	/*
	 * 1. Kernel DTS will enable UFS by default.
	 *
	 * 2. It hangs if Kernel UFS driver tries to access UFS registers when there
	 * is no power supply for UFS.
	 *
	 * So generally, disable UFS when detect fail.
	 *
	 * To save time spent on detecting UFS, you can disable UFS in kernel dts or
	 * U-Boot defconfig.
	 *
	 */
	if (desc->if_type != IF_TYPE_SCSI) {
		node = fdt_path_offset(blob, "/ufs@2a2d0000");
		if (node >= 0) {
			status = fdt_getprop(blob, node, "status", NULL);
			if (status && strcmp(status, "disabled")) {
				if (scsi_scan(true)) {
					fdt_setprop((void *)blob, node, "status", "disabled", 9);
					printf("FDT: UFS was not detected, disabling UFS.\n");
				}
			}
		}
	}

	return 0;
}
#endif

