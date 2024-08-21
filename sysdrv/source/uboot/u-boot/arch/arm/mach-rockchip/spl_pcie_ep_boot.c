// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (c) 2023 Rockchip Electronics Co., Ltd
 */

#include <common.h>
#include <spl.h>
#include <asm/io.h>
#include <asm/arch/cpu.h>
#include <asm/arch/hardware.h>
#include <asm/arch/ioc_rk3588.h>
#include <dt-bindings/clock/rk3588-cru.h>
#include <pci.h>
#include <asm/arch/rk_atags.h>

#ifndef CONFIG_SPL_LOAD_FIT_ADDRESS
#error "SPL_LOAD_FIT_ADDRESS not defined!"
#endif

#define printep(fmt, ...) \
		do { \
			printf("RKEP: %d - ", readl(CONFIG_ROCKCHIP_STIMER_BASE + 0x2c) / 24); \
			printf(fmt, ##__VA_ARGS__); \
		} while (0)

#ifdef CONFIG_ROCKCHIP_RK3588
#define PCIE_SNPS_DBI_BASE	0xf5000000
#define PCIE_SNPS_APB_BASE	0xfe150000
#define PCIE_SNPS_IATU_BASE	0xa40300000

#define PCI_RESBAR		0x2e8
#elif CONFIG_ROCKCHIP_RK3568
#define PCIE_SNPS_DBI_BASE	0xf6000000
#define PCIE_SNPS_APB_BASE	0xfe280000
#define PCIE_SNPS_IATU_BASE	0x3c0b00000

#define PCI_RESBAR		0x2b8
#else
#error "this soc is not support pcie ep!"
#endif

#define RKEP_BAR0_ADDR		0x3c000000
#define RKEP_BAR2_ADDR		CONFIG_SPL_LOAD_FIT_ADDRESS
#define RKEP_BAR0_CMD_ADDR	(RKEP_BAR0_ADDR + 0x400)
#define RKEP_BOOT_MAGIC		0x524b4550 /* RKEP */
#define RKEP_CMD_LOADER_RUN	0x524b4501

#define PCI_EXP_LNKCAP		12	/* Link Capabilities */
#define PCI_EXP_LNKCTL2		48	/* Link Control 2 */
#define PCI_EXP_LNKCTL2_TLS		0x000f
#define PCI_EXP_LNKCAP_SLS		0x0000000f

#define PCI_EXP_LNKCTL2_TLS_2_5GT	0x0001 /* Supported Speed 2.5GT/s */
#define PCI_EXP_LNKCTL2_TLS_5_0GT	0x0002 /* Supported Speed 5GT/s */
#define PCI_EXP_LNKCTL2_TLS_8_0GT	0x0003 /* Supported Speed 8GT/s */

/* Synopsys-specific PCIe configuration registers */
#define PCIE_PORT_LINK_CONTROL		0x710
#define PORT_LINK_MODE_MASK		(0x3f << 16)
#define PORT_LINK_MODE_1_LANES		(0x1 << 16)
#define PORT_LINK_MODE_2_LANES		(0x3 << 16)
#define PORT_LINK_MODE_4_LANES		(0x7 << 16)
#define PORT_LINK_MODE_8_LANES		(0xf << 16)

#define PCIE_LINK_WIDTH_SPEED_CONTROL	0x80C
#define PORT_LOGIC_SPEED_CHANGE		(0x1 << 17)
#define PORT_LOGIC_LINK_WIDTH_MASK	(0x1f << 8)
#define PORT_LOGIC_LINK_WIDTH_1_LANES	(0x1 << 8)
#define PORT_LOGIC_LINK_WIDTH_2_LANES	(0x2 << 8)
#define PORT_LOGIC_LINK_WIDTH_4_LANES	(0x4 << 8)
#define PORT_LOGIC_LINK_WIDTH_8_LANES	(0x8 << 8)

#define PCIE_DIRECT_SPEED_CHANGE	(0x1 << 17)

#define LINK_WAIT_IATU			10000
#define PCIE_ATU_ENABLE			(0x1 << 31)
#define PCIE_ATU_BAR_MODE_ENABLE	(0x1 << 30 | 1 << 19)
#define PCIE_ATU_UNR_REGION_CTRL1	0x00
#define PCIE_ATU_UNR_REGION_CTRL2	0x04
#define PCIE_ATU_CPU_ADDR_LOW		0x14
#define PCIE_ATU_CPU_ADDR_HIGH		0x18

/* SRNS: Use Separate refclk(internal clock) instead of from RC */
// #define PCIE_ENABLE_SRNS_PLL_REFCLK

struct rkpcie_cmd {
	u32 cmd;
	u32 size;
	u32 data[6];
};

/* rkep device mode status definition */
#define RKEP_MODE_BOOTROM	1
#define RKEP_MODE_LOADER	2
#define RKEP_MODE_KERNEL	3

/* Common status */
#define RKEP_SMODE_INIT		0
#define RKEP_SMODE_LNKRDY	1
#define RKEP_SMODE_LNKUP	2
#define RKEP_SMODE_ERR		0xff
/* Firmware download status */
#define RKEP_SMODE_FWDLRDY	0x10
#define RKEP_SMODE_FWDLDONE	0x11
/* Application status*/
#define RKEP_SMODE_APPRDY	0x20

struct rkpcie_boot {
	/* magic: "RKEP" */
	u32 magic;
	u32 version;
	struct {
		u16 mode;
		u16 submode;
	} devmode;
	/* Size of ATAGS for cap */
	u32 cap_size;
	struct {
		u8 cmd;
		u8 status;
		/* Error code for current CMD */
		u16 opcode;
	} cmd_status;
	u32 reserved[2];
	/* RK ATAGS, for mem and other info */
	struct tag cap;
	/* offset 0x400 */
	struct rkpcie_cmd cmd;
};

static void pcie_inbound_config(void)
{
	u64 base = PCIE_SNPS_IATU_BASE + 0x100;
	u32 val;
	char i;

	/* BAR0: RKEP_BAR0_ADDR */
	writel(RKEP_BAR0_ADDR, base + PCIE_ATU_CPU_ADDR_LOW);
	writel(0, base + PCIE_ATU_CPU_ADDR_HIGH);
	writel(0, base + PCIE_ATU_UNR_REGION_CTRL1);
	/* PCIE_ATU_UNR_REGION_CTRL2 */
	writel(PCIE_ATU_ENABLE | PCIE_ATU_BAR_MODE_ENABLE | (0 << 8),
	       base + PCIE_ATU_UNR_REGION_CTRL2);
	for (i = 0; i < 5; i++) {
		val = readl(base + PCIE_ATU_UNR_REGION_CTRL2);
		if (val & PCIE_ATU_ENABLE)
			break;
		udelay(LINK_WAIT_IATU);
	}
	printep("BAR0: 0x%x\n", RKEP_BAR0_ADDR);

	/* BAR2: RKEP_BAR2_ADDR */
	writel(RKEP_BAR2_ADDR, base + PCIE_ATU_CPU_ADDR_LOW + 0x200);
	writel(0, base + PCIE_ATU_CPU_ADDR_HIGH + 0x200);
	writel(0, base + PCIE_ATU_UNR_REGION_CTRL1 + 0x200);
	writel(PCIE_ATU_ENABLE | PCIE_ATU_BAR_MODE_ENABLE | (2 << 8),
	       base + PCIE_ATU_UNR_REGION_CTRL2 + 0x200);
	for (i = 0; i < 5; i++) {
		val = readl(base + PCIE_ATU_UNR_REGION_CTRL2 + 0x200);
		if (val & PCIE_ATU_ENABLE)
			break;
		udelay(LINK_WAIT_IATU);
	}
	printep("BAR2: 0x%x%x\n", 0, RKEP_BAR2_ADDR);

	/* BAR4 is wired reg, no need iATU */
}

static int rockchip_pcie_ep_set_bar_flag(void *dbi_base, u32 barno, int flags)
{
	u32 reg;

	reg = PCI_BASE_ADDRESS_0 + (4 * barno);

	/* Disabled the upper 32bits BAR to make a 64bits bar pair */
	if (flags & PCI_BASE_ADDRESS_MEM_TYPE_64)
		writel(0, dbi_base + reg + 0x100000 + 4);

	writel(flags, dbi_base + reg);
	if (flags & PCI_BASE_ADDRESS_MEM_TYPE_64)
		writel(0, dbi_base + reg + 4);

	return 0;
}

static void pcie_bar_init(void *dbi_base)
{
	void *resbar_base;
	u32 val;

	writel(0, dbi_base + 0x10);
	writel(0, dbi_base + 0x14);
	writel(0, dbi_base + 0x18);
	writel(0, dbi_base + 0x1c);
	writel(0, dbi_base + 0x20);
	writel(0, dbi_base + 0x24);

	/* Disable ASPM */
	val = readl(dbi_base + 0x7c);
	val &= ~(3 << 10);
	writel(val, dbi_base + 0x7c);

	/* Resize BAR0 to support 4M 32bits */
	resbar_base = dbi_base + PCI_RESBAR;
	writel(0x40, resbar_base + 0x4);
	writel(0x2c0, resbar_base + 0x8);
	/* BAR2: 64M 64bits */
	writel(0x400, resbar_base + 0x14);
	writel(0x6c0, resbar_base + 0x18);
	/* BAR4: Fixed for EP wired register, 1M 32bits */
	writel(0x10, resbar_base + 0x24);
	writel(0xc0, resbar_base + 0x28);
	/* Set flags */
	rockchip_pcie_ep_set_bar_flag(dbi_base, 0, PCI_BASE_ADDRESS_MEM_TYPE_32);
	rockchip_pcie_ep_set_bar_flag(dbi_base, 2,
				      PCI_BASE_ADDRESS_MEM_PREFETCH | PCI_BASE_ADDRESS_MEM_TYPE_64);
	rockchip_pcie_ep_set_bar_flag(dbi_base, 4, PCI_BASE_ADDRESS_MEM_TYPE_32);

	/* Close bar1 bar5 */
	writel(0x0, dbi_base + 0x100000 + 0x14);
	//writel(0x0, dbi_base + 0x100000 + 0x18);
	//writel(0x0, dbi_base + 0x100000 + 0x1c);
	//writel(0x0, dbi_base + 0x100000 + 0x20);
	writel(0x0, dbi_base + 0x100000 + 0x24);
	/* Close ROM BAR */
	writel(0x0, dbi_base + 0x100000 + 0x30);
}

static void pcie_bar0_header_init(void)
{
	struct rkpcie_boot *bh = (struct rkpcie_boot *)RKEP_BAR0_ADDR;

	bh->magic = RKEP_BOOT_MAGIC;
	bh->version = 0x100;
	bh->devmode.mode = RKEP_MODE_LOADER;
	bh->devmode.submode = RKEP_SMODE_INIT;
	bh->cap_size = 0;

	memset((char *)RKEP_BAR0_CMD_ADDR, 0, sizeof(struct rkpcie_cmd));
}

static void pcie_link_set_max_speed(void *dbi_base, u32 link_gen)
{
	u32 cap, ctrl2, link_speed;
	u8 offset = 0x70;

	cap = readl(dbi_base + offset + PCI_EXP_LNKCAP);
	ctrl2 = readl(dbi_base + offset + PCI_EXP_LNKCTL2);
	ctrl2 &= ~PCI_EXP_LNKCTL2_TLS;

	link_speed = link_gen;

	cap &= ~((u32)PCI_EXP_LNKCAP_SLS);
	writel(ctrl2 | link_speed, dbi_base + offset + PCI_EXP_LNKCTL2);
	writel(cap | link_speed, dbi_base + offset + PCI_EXP_LNKCAP);
}

static void pcie_link_set_lanes(void *dbi_base, u32 lanes)
{
	u32 val;

	/* Set the number of lanes */
	val = readl(dbi_base + PCIE_PORT_LINK_CONTROL);
	val &= ~PORT_LINK_MODE_MASK;
	switch (lanes) {
	case 1:
		val |= PORT_LINK_MODE_1_LANES;
		break;
	case 2:
		val |= PORT_LINK_MODE_2_LANES;
		break;
	case 4:
		val |= PORT_LINK_MODE_4_LANES;
		break;
	default:
		printf("RKEP: num-lanes %u: invalid value\n", lanes);
		return;
	}
	writel(val, dbi_base + PCIE_PORT_LINK_CONTROL);

	/* Set link width speed control register */
	val = readl(dbi_base + PCIE_LINK_WIDTH_SPEED_CONTROL);
	val &= ~PORT_LOGIC_LINK_WIDTH_MASK;
	switch (lanes) {
	case 1:
		val |= PORT_LOGIC_LINK_WIDTH_1_LANES;
		break;
	case 2:
		val |= PORT_LOGIC_LINK_WIDTH_2_LANES;
		break;
	case 4:
		val |= PORT_LOGIC_LINK_WIDTH_4_LANES;
		break;
	}

	val |= PCIE_DIRECT_SPEED_CHANGE;

	writel(val, dbi_base + PCIE_LINK_WIDTH_SPEED_CONTROL);
}

static void pcie_devmode_update(int mode, int submode)
{
	struct rkpcie_boot *bh = (struct rkpcie_boot *)RKEP_BAR0_ADDR;

	bh->devmode.mode = mode;
	bh->devmode.submode = submode;
	flush_dcache_range(RKEP_BAR0_ADDR, RKEP_BAR0_ADDR + 64);
}

#ifdef CONFIG_SPL_RAM_DEVICE
static void pcie_wait_for_fw(void)
{
	struct rkpcie_cmd *cmd = (struct rkpcie_cmd *)(RKEP_BAR0_CMD_ADDR);
	int val;
	int i = 0;

	printep("Link ready! Waiting RC to download Firmware:\n");
	printep("Download uboot.img  to BAR2+0\n");
	printep("Download boot.img   to BAR2+0x400000\n");
	printep("Send CMD_LOADER_RUN to BAR0+0x400\n");
	while (1) {
		invalidate_dcache_range(RKEP_BAR0_CMD_ADDR,
					RKEP_BAR0_CMD_ADDR + 32);
		val = readl(&cmd->cmd);
		if (val == RKEP_CMD_LOADER_RUN)
			break;
		i++;
		if (!(i % 10))
			printep("Waiting for FW, CMD: %x\n", val);
		mdelay(100);
	}
	/* Invalidate Cache for firmware area: BAR2, 64MB */
	invalidate_dcache_range(RKEP_BAR2_ADDR, RKEP_BAR2_ADDR + 0x4000000);
	printep("Firmware Download complete!\n");
}

static void pcie_update_atags(void)
{
	struct tag_ram_partition t_ram_part;

	if (!atags_is_available()) {
		printep("RKEP: No ATAGS data found, create new!\n");
		atags_destroy();
	}

	/* ram partition */
	memset(&t_ram_part, 0, sizeof(t_ram_part));
	t_ram_part.version = 0;
	t_ram_part.count = 1;
	strcpy(t_ram_part.part[0].name, "boot");
	t_ram_part.part[0].start = RKEP_BAR2_ADDR + 0x400000;	/* 4M offset */
	t_ram_part.part[0].size  = 0x3c00000;	/* 60M size */
	atags_set_tag(ATAG_RAM_PARTITION, &t_ram_part);
}

void rockchip_pcie_ep_get_firmware(void)
{
	pcie_devmode_update(RKEP_MODE_LOADER, RKEP_SMODE_FWDLRDY);
	pcie_wait_for_fw();
	pcie_update_atags();
	pcie_devmode_update(RKEP_MODE_LOADER, RKEP_SMODE_FWDLDONE);
}
#endif

#ifdef CONFIG_ROCKCHIP_RK3588
#define BUS_IOC_GPIO3D_IOMUX_SEL_H	0xfd5f807c
#define GPIO3_BASE			0xfec40000
#define GPIO3_SWPORT_DR_H		(GPIO3_BASE + 0x4)
#define GPIO3_SWPORT_DDR_H		(GPIO3_BASE + 0xc)

static void pcie_board_init(void)
{
	/* Enable AU5426 buffer chip on EVB4v10 */
	/* Set GPIO3D4 to gpio output HIGH mode PCIE20_CLK_PWREN */
	writel(0xf << 16, BUS_IOC_GPIO3D_IOMUX_SEL_H);
	writel(0x10001000, GPIO3_SWPORT_DDR_H);
	writel(0x10001000, GPIO3_SWPORT_DR_H);
	udelay(100);
}

#define PHY_MODE_PCIE_AGGREGATION 4	/* PCIe3x4 */
#define PHY_MODE_PCIE_NANBNB	0	/* P1:PCIe3x2  +  P0:PCIe3x2 */
#define PHY_MODE_PCIE_NANBBI	1	/* P1:PCIe3x2  +  P0:PCIe3x1*2 */
#define PHY_MODE_PCIE_NABINB	2	/* P1:PCIe3x1*2 + P0:PCIe3x2 */
#define PHY_MODE_PCIE_NABIBI	3	/* P1:PCIe3x1*2 + P0:PCIe3x1*2 */

#define CRU_BASE_ADDR			0xfd7c0000
#define CRU_SOFTRST_CON32		(CRU_BASE_ADDR + 0x0a80)
#define CRU_SOFTRST_CON33		(CRU_BASE_ADDR + 0x0a84)
#define CRU_SOFTRST_CON34		(CRU_BASE_ADDR + 0x0a88)
#define CRU_GATE_CON32			(CRU_BASE_ADDR + 0x0880)
#define CRU_GATE_CON33			(CRU_BASE_ADDR + 0x0884)
#define CRU_GATE_CON34			(CRU_BASE_ADDR + 0x0888)
#define CRU_GATE_CON38			(CRU_BASE_ADDR + 0x0898)
#define CRU_GATE_CON39			(CRU_BASE_ADDR + 0x089c)
#define PHPTOPCRU_BASE_ADDR		0xfd7c8000
#define PHPTOPCRU_SOFTRST_CON00		(PHPTOPCRU_BASE_ADDR + 0x0a00)
#define PHPTOPCRU_GATE_CON00		(PHPTOPCRU_BASE_ADDR + 0x0800)
#define PCIE3PHY_GRF_BASE		0xfd5b8000
#define RK3588_PCIE3PHY_GRF_CMN_CON0	(PCIE3PHY_GRF_BASE + 0x0000)
#define PCIE3PHY_GRF_PHY0_CON6		(PCIE3PHY_GRF_BASE + 0x0118)
#define PCIE3PHY_GRF_PHY1_CON6		(PCIE3PHY_GRF_BASE + 0x0218)
#define PCIE3PHY_GRF_PHY0_LN0_CON1	(PCIE3PHY_GRF_BASE + 0x1004)
#define PCIE3PHY_GRF_PHY0_LN1_CON1	(PCIE3PHY_GRF_BASE + 0x1104)
#define PCIE3PHY_GRF_PHY1_LN0_CON1	(PCIE3PHY_GRF_BASE + 0x2004)
#define PCIE3PHY_GRF_PHY1_LN1_CON1	(PCIE3PHY_GRF_BASE + 0x2104)
#define FIREWALL_PCIE_MASTER_SEC	0xfe0300f0
#define FIREWALL_PCIE_ACCESS		0xfe586040
#define CRU_PHYREF_ALT_GATE_CON		(CRU_BASE_ADDR + 0x0c38)
#define PMU1_GRF_BASE			0xfd58a000
#define PMU_PWR_GATE_SFTCON1		0xfd8d8150
#define PMU1_IOC_BASE			0xfd5F0000
#define CRU_GLB_RST_CON_OFFSET		(0xC10U)
#define CRU_GLB_SRST_FST_VALUE_OFFSET	(0xC08U)

void pcie_first_reset(void)
{
	printep("Fst Reset\n");
	mdelay(1);

	writel(0xFFDF, CRU_BASE_ADDR + CRU_GLB_RST_CON_OFFSET);
	writel(0xffffffff, PMU1_GRF_BASE + 0x4); // reset width
	writel(0x30003000, PMU1_GRF_BASE + 0x1c); // pmu1_grf pmu1_ioc hiold
	writel(0x00f00020, PMU1_IOC_BASE + 0x0);   //select tsad_shut_m0 iomux
	writel(0xFDB9, CRU_BASE_ADDR + CRU_GLB_SRST_FST_VALUE_OFFSET);

	while (1)
		;
}

static void pcie_cru_init(void)
{
	u32 phy0_mplla, phy1_mplla, t0 = 0, t1 = 0;
	u32 i, timeout = 500;

	/* Enable power domain: PD_PCIE & PD_PHP */
	writel(0x1 << 23 | 0x1 << 21, PMU_PWR_GATE_SFTCON1);

	/* FixMe init 3.0 PHY */
	/* Phy mode: Aggregation NBNB */
	writel((0x7 << 16) | PHY_MODE_PCIE_AGGREGATION, RK3588_PCIE3PHY_GRF_CMN_CON0);
	printep("PHY Mode 0x%x\n", readl(RK3588_PCIE3PHY_GRF_CMN_CON0) & 7);
	/* Enable clock and sfreset for Controller and PHY */
	writel(0xffff0000, CRU_SOFTRST_CON32);
	writel(0xffff0000, CRU_SOFTRST_CON33);
	writel(0xffff0000, CRU_SOFTRST_CON34);
	writel(0xffff0000, CRU_GATE_CON32);
	writel(0xffff0000, CRU_GATE_CON33);
	writel(0xffff0000, CRU_GATE_CON34);
	writel(0xffff0000, CRU_GATE_CON38);
	writel(0xffff0000, CRU_GATE_CON39);

	writel((0x1 << 24), PHPTOPCRU_SOFTRST_CON00);
	writel(0xffff0000, PHPTOPCRU_GATE_CON00);

	/* PHY Reset */
	writel((0x1 << 10) | (0x1 << 26), PHPTOPCRU_SOFTRST_CON00);

	udelay(1);

#ifdef PCIE_ENABLE_SRNS_PLL_REFCLK
	writel(0x000f0000, CRU_PHYREF_ALT_GATE_CON);

	/* PHY0 & PHY1  use internal clock */
	writel(0x0 | (0x1 << 18), PCIE3PHY_GRF_PHY0_CON6);
	writel(0x0 | (0x1 << 18), PCIE3PHY_GRF_PHY1_CON6);

	/* phy0_rx0_cmn_refclk_mod */
	writel((0x0) | (0x1 << 23), PCIE3PHY_GRF_PHY0_LN0_CON1);
	/* phy1_rx0_cmn_refclk_mod */
	writel((0x0) | (0x1 << 23), PCIE3PHY_GRF_PHY0_LN1_CON1);
	/* phy0_rx0_cmn_refclk_mod */
	writel((0x0) | (0x1 << 23), PCIE3PHY_GRF_PHY1_LN0_CON1);
	/* phy1_rx0_cmn_refclk_mod */
	writel((0x0) | (0x1 << 23), PCIE3PHY_GRF_PHY1_LN1_CON1);
#endif

	udelay(1000);

	/* Deassert PCIe PMA output clamp mode */
	writel((0x1 << 8) | (0x1 << 24), RK3588_PCIE3PHY_GRF_CMN_CON0);

	/* Deassert PHY Reset */
	writel((0x1 << 26), PHPTOPCRU_SOFTRST_CON00);

	/* S-Phy: waiting for phy locked */
	for (i = 0; i < timeout; i++) {
		phy0_mplla = readl(PCIE3PHY_GRF_BASE + 0x904);
		phy1_mplla = readl(PCIE3PHY_GRF_BASE + 0xA04);

		if (phy0_mplla != t0 || phy1_mplla != t1) {
			printep("RKEP: GRF:904=%x, a04=%x...\n", phy0_mplla, phy1_mplla);

			t0 = phy0_mplla;
			t1 = phy1_mplla;
			if (phy0_mplla == 0xF && phy1_mplla == 0xF)
				break;
		}

		udelay(10);
	}

	if (i >= timeout) {
		printep("lock fail\n");
		mdelay(1);
		pcie_first_reset();
	}

	/* PHY config: no config need for snps3.0phy */
}

static void pcie_firewall_init(void)
{
	/* Enable PCIe Access in firewall and master secure mode */
	writel(0xffff0000, FIREWALL_PCIE_MASTER_SEC);
	writel(0x01800000, FIREWALL_PCIE_ACCESS);
}
#elif CONFIG_ROCKCHIP_RK3568

static void pcie_board_init(void)
{
	/* to-do */
}

static const u16 phy_fw[] = {
	#include "./../../../drivers/phy/phy-rockchip-snps-pcie3.fw"
};

#define GRF_PCIE30PHY_RK3568_CON1 0x4
#define GRF_PCIE30PHY_RK3568_CON3 0xC
#define GRF_PCIE30PHY_RK3568_CON4 0x10
#define GRF_PCIE30PHY_RK3568_CON5 0x14
#define GRF_PCIE30PHY_RK3568_CON6 0x18
#define GRF_PCIE30PHY_RK3568_CON9 0x24
#define GRF_PCIE30PHY_RK3568_STATUS0 0x80
#define RK3568_SRAM_INIT_DONE(reg) ((reg) & BIT(14))

#define PMUCRU_BASE			0xFDD00000
#define PMUCRU_PMUGATE_CON02		(PMUCRU_BASE + 0x188)

#define CRU_BASE			0xFDD20000
#define CRU_GATE_CON12			(CRU_BASE + 0x330)
#define CRU_GATE_CON13			(CRU_BASE + 0x334)
#define CRU_GATE_CON33			(CRU_BASE + 0x384)
#define CRU_SOFTRST_CON12		(CRU_BASE + 0x430)
#define CRU_SOFTRST_CON27		(CRU_BASE + 0x46c)
#define CRU_GLB_SRST_FST_OFFSET		(0xD4U)

#define PCIE30_PHY_GRF			0xFDCB8000

#define SYS_GRF_BASE			0xFDC60000

void pcie_first_reset(void)
{
	printep("Fst Reset\n");
	mdelay(1);

	writel(0x00040004, CRU_BASE + 0x104);
	writel(0x00700010, CRU_BASE);
	writel(0x00100010, SYS_GRF_BASE + 0x508);
	writel(0xFDB9, CRU_BASE + CRU_GLB_SRST_FST_OFFSET);

	while (1)
		;
}

void pcie_cru_init(void)
{
	u32 i, reg, timeout = 500;
	void __iomem *mmio = (void __iomem *)0xFE8C0000;
	u32 phy0_status0, phy0_status1, t0 = 0, t1 = 0;

	/* Enable phy and controoler clk */
	writel(0xffff0000, PMUCRU_PMUGATE_CON02);
	writel(0xffff0000, CRU_GATE_CON12);
	writel(0xffff0000, CRU_GATE_CON13);
	writel(0xffff0000, CRU_GATE_CON33);
	writel(0xffff0000, CRU_SOFTRST_CON27);

	writel(0x40004000, CRU_SOFTRST_CON27);
	writel(0x80008000, PCIE30_PHY_GRF + GRF_PCIE30PHY_RK3568_CON9);

	writel((0x1 << 15) | (0x1 << 31),
	       PCIE30_PHY_GRF + GRF_PCIE30PHY_RK3568_CON9); //map to access sram

#ifdef PCIE_ENABLE_SRNS_PLL_REFCLK
	/* use internal clock */
	writel(0x0 | (0x1 << 31), PCIE30_PHY_GRF + GRF_PCIE30PHY_RK3568_CON3);

	/* rx0_cmn_refclk_mode disabled */
	writel((0x0) | (0x1 << 25), PCIE30_PHY_GRF + GRF_PCIE30PHY_RK3568_CON5);
	/* rx1_cmn_refclk_mode disabled */
	writel((0x0) | (0x1 << 25), PCIE30_PHY_GRF + GRF_PCIE30PHY_RK3568_CON6);
#endif

	writel((0x0 << 14) | (0x1 << (14 + 16)),
	       PCIE30_PHY_GRF + GRF_PCIE30PHY_RK3568_CON4); //sdram_ld_done
	writel((0x0 << 13) | (0x1 << (13 + 16)),
	       PCIE30_PHY_GRF + GRF_PCIE30PHY_RK3568_CON4); //sdram_bypass

	writel(0x40000000, CRU_SOFTRST_CON27);

	udelay(5);
	printep("RKEP: sram initial\n");
	while (1) {
		reg = readl(PCIE30_PHY_GRF + GRF_PCIE30PHY_RK3568_STATUS0);
		if (RK3568_SRAM_INIT_DONE(reg))
			break;
	}
	printep("RKEP: sram init done\n");

	writel((0x3 << 8) | (0x3 << (8 + 16)),
	       PCIE30_PHY_GRF + GRF_PCIE30PHY_RK3568_CON9); //map to access sram
	for (i = 0; i < ARRAY_SIZE(phy_fw); i++)
		writel(phy_fw[i], mmio + (i << 2));

	printep("RKEP: snps pcie3phy FW update! size %ld\n", ARRAY_SIZE(phy_fw));
	writel((0x0 << 8) | (0x3 << (8 + 16)),
	       PCIE30_PHY_GRF + GRF_PCIE30PHY_RK3568_CON9);
	writel((0x1 << 14) | (0x1 << (14 + 16)),
	       PCIE30_PHY_GRF + GRF_PCIE30PHY_RK3568_CON4); //sdram_ld_done

	writel(0xffff0000, CRU_SOFTRST_CON12);
	writel(0x100010, PCIE_SNPS_APB_BASE + 0x180);

	/* S-Phy: waiting for phy locked */
	for (i = 0; i < timeout; i++) {
		phy0_status0 = readl(PCIE30_PHY_GRF + 0x80);
		phy0_status1 = readl(PCIE30_PHY_GRF + 0x84);

		if (phy0_status0 != t0 || phy0_status1 != t1) {
			printep("RKEP: GRF:0x80=%x, 0x84=%x...\n", phy0_status0, phy0_status1);

			t0 = phy0_status0;
			t1 = phy0_status1;
			if (RK3568_SRAM_INIT_DONE(phy0_status0))
				break;
		}

		udelay(10);
	}

	if (i >= timeout) {
		printep("lock fail\n");
		mdelay(1);
		pcie_first_reset();
	}

	udelay(1);
}

static void pcie_firewall_init(void)
{
}
#endif

static void pcie_ep_init(void)
{
	u32 val;
	void *dbi_base = (void *)PCIE_SNPS_DBI_BASE;
	u64 apb_base = PCIE_SNPS_APB_BASE;
	int i, retries = 0, phy_linkup;

#ifdef PCIE_ENABLE_SRNS_PLL_REFCLK
	printep("RefClock in SRNS clock mode\n");
#else
	printep("RefClock in common clock_mode\n");
#endif

	/*
	 * ltssm_enable enhance mode and enable delaying the link training
	 * after Hot Reset
	 */
	writel(0x120012, apb_base + 0x180);

	/* Unmask pm_turnoff_int */
	writel(0x04000000, apb_base + 0x18);

	/* PortLorgic DBI_RO_WR_EN */
	val = readl((dbi_base + 0x8bc));
	val |= 0x1;
	writel(val, dbi_base + 0x8bc);

reinit:
	pcie_bar_init(dbi_base);
	pcie_inbound_config();

	/* Device PID, DID */
	writel(0x1d87, dbi_base + 0x00);
	writel(0x356a, dbi_base + 0x02);
	/* Device Class: Processing accelerators */
	writel(0x1200, dbi_base + 0x0a);

	pcie_link_set_max_speed(dbi_base, PCI_EXP_LNKCTL2_TLS_8_0GT);

#ifdef	CONFIG_ROCKCHIP_RK3588
	pcie_link_set_lanes(dbi_base, 4);
#elif	CONFIG_ROCKCHIP_RK3568
	pcie_link_set_lanes(dbi_base, 2);
#endif

	/* EP mode */
	writel(0xf00000, apb_base);
	udelay(100);

	/* Enable EP mem/io access */
	val = readl(dbi_base + 0x4);
	writel(val | 0x6, dbi_base + 0x4);

	val = readl(apb_base + 0x10);
	if (val & 0x4) {
		printep("Link is reset, int status misc=%x\n", val);
		retries++;
	}

	if (retries)	/* Set app_dly2_done to enable app_ltssm_enable */
		writel(0x80008, apb_base + 0x180);
	else		/* Enable LTSSM */
		writel(0xc000c, apb_base);
	printep("init PCIe fast Link up\n");
	pcie_devmode_update(RKEP_MODE_LOADER, RKEP_SMODE_LNKRDY);

	/* Waiting for Link up */
	while (1) {
		val = readl(apb_base + 0x300);
		if (((val & 0x3ffff) & ((0x3 << 16))) == 0x30000)
			break;

		if (((val & 0x3ffff) & ((0x3 << 16))) == 0x10000)
			phy_linkup = 1;

		if (val == 0 && phy_linkup)
			pcie_first_reset();

		udelay(10);
	}
	printep("Link up %x\n", val);
	mdelay(3);

	/* Wait for link stable */
	for (i = 0; i < 10000; i++) {
		val = readl(apb_base + 0x10);
		if (val & 0x4) {
			writel(0x4, apb_base + 0x10);
			printep("Link is reset, int status misc=%x\n", val);
			if (retries < 3) {
				retries++;
				goto reinit;
			} else {
				break;
			}
		}

		/* L2 */
		val = readl(apb_base + 0x4);
		if (val & 0x400) {
			writel(0x4, apb_base + 0x10);
			pcie_first_reset();
		}
		udelay(1);
	}
	printep("Done\n");
	pcie_devmode_update(RKEP_MODE_LOADER, RKEP_SMODE_LNKUP);
}

void rockchip_pcie_ep_init(void)
{
	u32 val;

	printf("\nRKEP: Init PCIe EP\n");
	pcie_bar0_header_init();

#ifdef CONFIG_ROCKCHIP_RK3588
	writel(0x1 << 23 | 0x1 << 21, PMU_PWR_GATE_SFTCON1);
	udelay(10);
#endif

	pcie_firewall_init();
	/* Re-in pcie initial */
	val = readl(PCIE_SNPS_APB_BASE + 0x300);
	if (((val & 0x3ffff) & ((0x3 << 16))) == 0x30000) {
		printf("RKEP: already link up\n");
		pcie_devmode_update(RKEP_MODE_LOADER, RKEP_SMODE_LNKUP);
		return;
	}

	pcie_board_init();
	/* CRU and PHY Init */
	pcie_cru_init();

	pcie_ep_init();
}
