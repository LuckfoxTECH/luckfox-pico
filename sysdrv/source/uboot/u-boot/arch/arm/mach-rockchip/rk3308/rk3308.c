// SPDX-License-Identifier:     GPL-2.0+
/*
 * Copyright (C) 2020 Rockchip Electronics Co., Ltd
 */

#include <common.h>
#include <fdt_support.h>
#include <ram.h>
#include <asm/io.h>
#include <asm/arch/boot_mode.h>
#include <asm/arch/bootrom.h>
#include <asm/arch/cru_rk3308.h>
#include <asm/arch/cpu.h>
#include <asm/arch/grf_rk3308.h>
#include <asm/arch/hardware.h>
#include <asm/arch/rk_atags.h>
#include <asm/gpio.h>
#include <asm/arch/sdram_common.h>
#include <debug_uart.h>

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_ARM64
#include <asm/armv8/mmu.h>
static struct mm_region rk3308_mem_map[] = {
	{
		.virt = 0x0UL,
		.phys = 0x0UL,
		.size = 0xff000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) |
			 PTE_BLOCK_INNER_SHARE
	}, {
		.virt = 0xff000000UL,
		.phys = 0xff000000UL,
		.size = 0x01000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			 PTE_BLOCK_NON_SHARE |
			 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	}, {
		/* List terminator */
		0,
	}
};

struct mm_region *mem_map = rk3308_mem_map;
#endif

#define GRF_BASE	0xff000000
#define SGRF_BASE	0xff2b0000
#define CRU_BASE	0xff500000

enum {
	GPIO1C7_SHIFT		= 8,
	GPIO1C7_MASK		= GENMASK(11, 8),
	GPIO1C7_GPIO		= 0,
	GPIO1C7_UART1_RTSN,
	GPIO1C7_UART2_TX_M0,
	GPIO1C7_SPI2_MOSI,
	GPIO1C7_JTAG_TMS,

	GPIO1C6_SHIFT		= 4,
	GPIO1C6_MASK		= GENMASK(7, 4),
	GPIO1C6_GPIO		= 0,
	GPIO1C6_UART1_CTSN,
	GPIO1C6_UART2_RX_M0,
	GPIO1C6_SPI2_MISO,
	GPIO1C6_JTAG_TCLK,

	GPIO4D3_SHIFT           = 6,
	GPIO4D3_MASK            = GENMASK(7, 6),
	GPIO4D3_GPIO            = 0,
	GPIO4D3_SDMMC_D3,
	GPIO4D3_UART2_TX_M1,

	GPIO4D2_SHIFT           = 4,
	GPIO4D2_MASK            = GENMASK(5, 4),
	GPIO4D2_GPIO            = 0,
	GPIO4D2_SDMMC_D2,
	GPIO4D2_UART2_RX_M1,

	UART2_IO_SEL_SHIFT	= 2,
	UART2_IO_SEL_MASK	= GENMASK(3, 2),
	UART2_IO_SEL_M0		= 0,
	UART2_IO_SEL_M1,
	UART2_IO_SEL_USB,

	GPIO3B3_SEL_SRC_CTRL_SHIFT	= 7,
	GPIO3B3_SEL_SRC_CTRL_MASK	= BIT(7),
	GPIO3B3_SEL_SRC_CTRL_IOMUX	= 0,
	GPIO3B3_SEL_SRC_CTRL_SEL_PLUS,

	GPIO3B3_SEL_PLUS_SHIFT		= 4,
	GPIO3B3_SEL_PLUS_MASK		= GENMASK(6, 4),
	GPIO3B3_SEL_PLUS_GPIO3_B3	= 0,
	GPIO3B3_SEL_PLUS_FLASH_ALE,
	GPIO3B3_SEL_PLUS_EMMC_PWREN,
	GPIO3B3_SEL_PLUS_SPI1_CLK,
	GPIO3B3_SEL_PLUS_LCDC_D23_M1,

	GPIO3B2_SEL_SRC_CTRL_SHIFT	= 3,
	GPIO3B2_SEL_SRC_CTRL_MASK	= BIT(3),
	GPIO3B2_SEL_SRC_CTRL_IOMUX	= 0,
	GPIO3B2_SEL_SRC_CTRL_SEL_PLUS,

	GPIO3B2_SEL_PLUS_SHIFT		= 0,
	GPIO3B2_SEL_PLUS_MASK		= GENMASK(2, 0),
	GPIO3B2_SEL_PLUS_GPIO3_B2	= 0,
	GPIO3B2_SEL_PLUS_FLASH_RDN,
	GPIO3B2_SEL_PLUS_EMMC_RSTN,
	GPIO3B2_SEL_PLUS_SPI1_MISO,
	GPIO3B2_SEL_PLUS_LCDC_D22_M1,
};

enum {
	IOVSEL3_CTRL_SHIFT	= 8,
	IOVSEL3_CTRL_MASK	= BIT(8),
	VCCIO3_SEL_BY_GPIO	= 0,
	VCCIO3_SEL_BY_IOVSEL3,

	IOVSEL3_SHIFT		= 3,
	IOVSEL3_MASK		= BIT(3),
	VCCIO3_3V3		= 0,
	VCCIO3_1V8,
};

enum {
	SND_GLB_WDT_RST		= BIT(3),
	FST_GLB_WDT_RST		= BIT(2),
};

/*
 * The voltage of VCCIO3(which is the voltage domain of emmc/flash/sfc
 * interface) can indicated by GPIO0_A4 or io_vsel3. The SOC defaults
 * use GPIO0_A4 to indicate power supply voltage for VCCIO3 by hardware,
 * then we can switch to io_vsel3 after system power on, and release GPIO0_A4
 * for other usage.
 */

#define GPIO0_A4	4

int rk_board_init(void)
{
	static struct rk3308_grf * const grf = (void *)GRF_BASE;
	u32 val;
	int ret;

	ret = gpio_request(GPIO0_A4, "gpio0_a4");
	if (ret < 0) {
		printf("request for gpio0_a4 failed:%d\n", ret);
		return 0;
	}

	gpio_direction_input(GPIO0_A4);

	if (gpio_get_value(GPIO0_A4))
		val = VCCIO3_SEL_BY_IOVSEL3 << IOVSEL3_CTRL_SHIFT |
		      VCCIO3_1V8 << IOVSEL3_SHIFT;
	else
		val = VCCIO3_SEL_BY_IOVSEL3 << IOVSEL3_CTRL_SHIFT |
		      VCCIO3_3V3 << IOVSEL3_SHIFT;
	rk_clrsetreg(&grf->soc_con0, IOVSEL3_CTRL_MASK | IOVSEL3_MASK, val);

	gpio_free(GPIO0_A4);
	return 0;
}

#define SERVICE_CPU_BASE		0xff5c0000
#define SERVICE_VOICE_BASE		0xff5d0000
#define SERVICE_LOGIC_BASE		0xff5d8000
#define SERVICE_PERI_BASE		0xff5e0000
#define SERVICE_CPU_ADDR		(SERVICE_CPU_BASE + 0x80)
#define SERVICE_VOP_ADDR		(SERVICE_LOGIC_BASE + 0x100)
#define SERVICE_DMAC0_ADDR		(SERVICE_LOGIC_BASE + 0x0)
#define SERVICE_DMAC1_ADDR		(SERVICE_LOGIC_BASE + 0x80)
#define SERVICE_CRYPTO_ADDR		(SERVICE_LOGIC_BASE + 0x180)
#define SERVICE_VAD_ADDR		(SERVICE_VOICE_BASE + 0x80)
#define SERVICE_EMMC_ADDR		(SERVICE_PERI_BASE + 0x80)
#define SERVICE_GMAC_ADDR		(SERVICE_PERI_BASE + 0x100)
#define SERVICE_NAND_ADDR		(SERVICE_PERI_BASE + 0x180)
#define SERVICE_SDIO_ADDR		(SERVICE_PERI_BASE + 0x200)
#define SERVICE_SDMMC_ADDR		(SERVICE_PERI_BASE + 0x280)
#define SERVICE_SFC_ADDR		(SERVICE_PERI_BASE + 0x300)
#define SERVICE_USB_HOST_ADDR		(SERVICE_PERI_BASE + 0x380)
#define SERVICE_USB_OTG_ADDR		(SERVICE_PERI_BASE + 0x400)

#define DOS_PRIORITY_OFFSET		0x8
#define QOS_PRIORITY_P1_P0(p1, p0)	((((p1) & 0x3) << 8) |\
					(((p0) & 0x3) << 0))

enum {
	IOVSEL4_SHIFT           = 4,
	IOVSEL4_MASK            = BIT(4),
	VCCIO4_3V3              = 0,
	VCCIO4_1V8,
};

int arch_cpu_init(void)
{
#ifndef CONFIG_TPL_BUILD
#ifdef CONFIG_SPL_BUILD
	static struct rk3308_sgrf * const sgrf = (void *)SGRF_BASE;

	/* Set CRYPTO SDMMC EMMC NAND SFC USB master bus to be secure access */
	rk_clrreg(&sgrf->con_secure0, 0x2b83);
#endif
#else /* defined(CONFIG_TPL_BUILD) */
	static struct rk3308_cru * const cru = (void *)CRU_BASE;
	static struct rk3308_grf * const grf = (void *)GRF_BASE;
	u32 glb_rst_st;

	/*
	 * RK3308 internally default select 1.8v for VCCIO4 on reset state,
	 * but some boards may give a 3.3V power supply for VCCIO4, this may
	 * bring a risk of chip damage through overvoltage. So we internally
	 * select 3.3V for VCCIO4 as early as possiple to reduces this risk.
	 */
	rk_clrsetreg(&grf->soc_con0, IOVSEL4_MASK, VCCIO4_3V3 << IOVSEL4_SHIFT);

	/*
	 * write BOOT_WATCHDOG to boot mode register, if we are reset by wdt
	 */
	glb_rst_st = readl(&cru->glb_rst_st);
	writel(FST_GLB_WDT_RST | SND_GLB_WDT_RST, &cru->glb_rst_st);
	if (glb_rst_st & (FST_GLB_WDT_RST | SND_GLB_WDT_RST))
		writel(BOOT_WATCHDOG, CONFIG_ROCKCHIP_BOOT_MODE_REG);

	/* set wdt tsadc first global reset*/
	writel(WDT_GLB_SRST_CTRL << WDT_GLB_SRST_CTRL_SHIFT |
	       TSADC_GLB_SRST_CTRL << TSADC_GLB_SRST_CTRL_SHIFT,
	       &cru->glb_rst_con);

	/* Set qos priority level */
	writel(QOS_PRIORITY_P1_P0(1, 1),
	       SERVICE_CPU_ADDR + DOS_PRIORITY_OFFSET);
	writel(QOS_PRIORITY_P1_P0(3, 3),
	       SERVICE_VOP_ADDR + DOS_PRIORITY_OFFSET);
	writel(QOS_PRIORITY_P1_P0(2, 2),
	       SERVICE_DMAC0_ADDR + DOS_PRIORITY_OFFSET);
	writel(QOS_PRIORITY_P1_P0(2, 2),
	       SERVICE_DMAC1_ADDR + DOS_PRIORITY_OFFSET);
	writel(QOS_PRIORITY_P1_P0(2, 2),
	       SERVICE_CRYPTO_ADDR + DOS_PRIORITY_OFFSET);
	writel(QOS_PRIORITY_P1_P0(3, 3),
	       SERVICE_VAD_ADDR + DOS_PRIORITY_OFFSET);
	writel(QOS_PRIORITY_P1_P0(2, 2),
	       SERVICE_EMMC_ADDR + DOS_PRIORITY_OFFSET);
	writel(QOS_PRIORITY_P1_P0(2, 2),
	       SERVICE_GMAC_ADDR + DOS_PRIORITY_OFFSET);
	writel(QOS_PRIORITY_P1_P0(2, 2),
	       SERVICE_NAND_ADDR + DOS_PRIORITY_OFFSET);
	writel(QOS_PRIORITY_P1_P0(2, 2),
	       SERVICE_SDIO_ADDR + DOS_PRIORITY_OFFSET);
	writel(QOS_PRIORITY_P1_P0(2, 2),
	       SERVICE_SDMMC_ADDR + DOS_PRIORITY_OFFSET);
	writel(QOS_PRIORITY_P1_P0(2, 2),
	       SERVICE_SFC_ADDR + DOS_PRIORITY_OFFSET);
	writel(QOS_PRIORITY_P1_P0(2, 2),
	       SERVICE_USB_HOST_ADDR + DOS_PRIORITY_OFFSET);
	writel(QOS_PRIORITY_P1_P0(2, 2),
	       SERVICE_USB_OTG_ADDR + DOS_PRIORITY_OFFSET);
#endif
	return 0;
}

#ifdef CONFIG_SPL_BUILD
int rk_board_init_f(void)
{
	static struct rk3308_grf * const grf = (void *)GRF_BASE;
	unsigned long mask;
	unsigned long value;

	mask = GPIO3B2_SEL_PLUS_MASK | GPIO3B2_SEL_SRC_CTRL_MASK |
		GPIO3B3_SEL_PLUS_MASK | GPIO3B3_SEL_SRC_CTRL_MASK;
	value = (GPIO3B2_SEL_PLUS_FLASH_RDN << GPIO3B2_SEL_PLUS_SHIFT) |
		(GPIO3B2_SEL_SRC_CTRL_SEL_PLUS << GPIO3B2_SEL_SRC_CTRL_SHIFT) |
		(GPIO3B3_SEL_PLUS_FLASH_ALE << GPIO3B3_SEL_PLUS_SHIFT) |
		(GPIO3B3_SEL_SRC_CTRL_SEL_PLUS << GPIO3B3_SEL_SRC_CTRL_SHIFT);

	if (get_bootdev_by_brom_bootsource() == BOOT_TYPE_NAND) {
		if (soc_is_rk3308b() || soc_is_rk3308bs())
			rk_clrsetreg(&grf->soc_con15, mask, value);
	}

	return 0;
}
#endif

void board_debug_uart_init(void)
{
	static struct rk3308_cru * const cru = (void *)CRU_BASE;
	static struct rk3308_grf * const grf = (void *)GRF_BASE;

#if defined(CONFIG_DEBUG_UART_BASE)
#if (CONFIG_DEBUG_UART_BASE == 0xFF0C0000)
	/*select 24M clock to UART2 */
	rk_clrsetreg(&cru->clksel_con[16],
		     CLK_UART2_PLL_SEL_MASK | CLK_UART2_DIV_CON_MASK,
		     CLK_UART2_PLL_SEL_XIN_OSC0 << CLK_UART2_PLL_SEL_SHIFT |
		     CLK_UART2_DIV_CON << CLK_UART2_DIV_CON_SHIFT);

#if (CONFIG_ROCKCHIP_UART_MUX_SEL_M == 0)
	/* Enable early UART2 channel m0 on the rk3308 */
	rk_clrsetreg(&grf->soc_con5, UART2_IO_SEL_MASK,
		     UART2_IO_SEL_M0 << UART2_IO_SEL_SHIFT);
	rk_clrsetreg(&grf->gpio1ch_iomux, GPIO1C7_MASK | GPIO1C6_MASK,
		     GPIO1C7_UART2_TX_M0 << GPIO1C7_SHIFT |
		     GPIO1C6_UART2_RX_M0 << GPIO1C6_SHIFT);

#elif (CONFIG_ROCKCHIP_UART_MUX_SEL_M == 1)
	/* Enable early UART2 channel m1 on the rk3308 */
	rk_clrsetreg(&grf->soc_con5, UART2_IO_SEL_MASK,
		     UART2_IO_SEL_M1 << UART2_IO_SEL_SHIFT);
	if (readl(BROM_BOOTSOURCE_ID_ADDR) != BROM_BOOTSOURCE_SD)
		rk_clrsetreg(&grf->gpio4d_iomux,
			     GPIO4D3_MASK | GPIO4D2_MASK,
			     GPIO4D2_UART2_RX_M1 << GPIO4D2_SHIFT |
			     GPIO4D3_UART2_TX_M1 << GPIO4D3_SHIFT);
#else
	#error "Please select M0 or M1 for uart2 !!!"
#endif

#elif (CONFIG_DEBUG_UART_BASE == 0xFF0E0000)
	/*select 24M clock to UART4 */
	rk_clrsetreg(&cru->clksel_con[22],
		     CLK_UART4_PLL_SEL_MASK | CLK_UART4_DIV_CON_MASK,
		     CLK_UART4_PLL_SEL_XIN_OSC0 << CLK_UART4_PLL_SEL_SHIFT |
		     CLK_UART4_DIV_CON << CLK_UART4_DIV_CON_SHIFT);

	rk_clrsetreg(&grf->gpio4b_iomux,
		     GPIO4B1_SEL_MASK | GPIO4B0_SEL_MASK,
		     GPIO4B1_SEL_UART4_TX << GPIO4B1_SEL_SHIFT |
		     GPIO4B0_SEL_UART4_RX << GPIO4B0_SEL_SHIFT);
#elif (CONFIG_DEBUG_UART_BASE == 0xFF0A0000)
	/*select 24M clock to UART0 */
	rk_clrsetreg(&cru->clksel_con[10],
		     CLK_UART0_PLL_SEL_MASK | CLK_UART0_DIV_CON_MASK,
		     CLK_UART0_PLL_SEL_XIN_OSC0 << CLK_UART0_PLL_SEL_SHIFT |
		     CLK_UART0_DIV_CON << CLK_UART0_DIV_CON_SHIFT);

	rk_clrsetreg(&grf->gpio2a_iomux,
		     GPIO2A1_SEL_MASK | GPIO2A0_SEL_MASK,
		     GPIO2A1_SEL_UART0_TX << GPIO2A1_SEL_SHIFT |
		     GPIO2A0_SEL_UART0_RX << GPIO2A0_SEL_SHIFT);
#elif (CONFIG_DEBUG_UART_BASE == 0xFF0B0000)
	/*select 24M clock to UART1 */
	rk_clrsetreg(&cru->clksel_con[13],
		     CLK_UART1_PLL_SEL_MASK | CLK_UART1_DIV_CON_MASK,
		     CLK_UART1_PLL_SEL_XIN_OSC0 << CLK_UART1_PLL_SEL_SHIFT |
		     CLK_UART1_DIV_CON << CLK_UART1_DIV_CON_SHIFT);

	rk_clrsetreg(&grf->gpio1d_iomux,
		     GPIO1D1_SEL_MASK | GPIO1D0_SEL_MASK,
		     GPIO1D1_SEL_UART1_TX << GPIO1D1_SEL_SHIFT |
		     GPIO1D1_SEL_UART1_RX << GPIO1D0_SEL_SHIFT);
#elif (CONFIG_DEBUG_UART_BASE == 0xFF0D0000)
	/*select 24M clock to UART3 */
	rk_clrsetreg(&cru->clksel_con[19],
		     CLK_UART3_PLL_SEL_MASK | CLK_UART3_DIV_CON_MASK,
		     CLK_UART3_PLL_SEL_XIN_OSC0 << CLK_UART3_PLL_SEL_SHIFT |
		     CLK_UART3_DIV_CON << CLK_UART3_DIV_CON_SHIFT);

	rk_clrsetreg(&grf->gpio3b_iomux,
		     GPIO3B5_SEL_MASK | GPIO3B4_SEL_MASK,
		     GPIO3B5_SEL_UART3_TX << GPIO3B5_SEL_SHIFT |
		     GPIO3B4_SEL_UART3_RX << GPIO3B4_SEL_SHIFT);
#else
	#error "Please select proper uart as debug uart !!!"
#endif
#endif /* defined(CONFIG_DEBUG_UART_BASE) */
}

static int fdt_fixup_cpu_idle(const void *blob)
{
	int cpu_node, sub_node, len;
	u32 *pp;

	cpu_node = fdt_path_offset(blob, "/cpus");
	if (cpu_node < 0) {
		printf("Failed to get cpus node\n");
		return -EINVAL;
	}

	fdt_for_each_subnode(sub_node, blob, cpu_node) {
		pp = (u32 *)fdt_getprop(blob, sub_node, "cpu-idle-states",
					&len);
		if (!pp)
			continue;
		if (fdt_delprop((void *)blob, sub_node, "cpu-idle-states") < 0)
			printf("Failed to del cpu-idle-states prop\n");
	}

	return 0;
}

static int fdt_fixup_cpu_opp_table(const void *blob)
{
	int opp_node, cpu_node, sub_node;
	int len;
	u32 phandle;
	u32 *pp;

	/* Replace opp table */
	opp_node = fdt_path_offset(blob, "/rk3308bs-cpu0-opp-table");
	if (opp_node < 0) {
		printf("Failed to get rk3308bs-cpu0-opp-table node\n");
		return -EINVAL;
	}

	phandle = fdt_get_phandle(blob, opp_node);
	if (!phandle) {
		printf("Failed to get cpu opp table phandle\n");
		return -EINVAL;
	}

	cpu_node = fdt_path_offset(blob, "/cpus");
	if (cpu_node < 0) {
		printf("Failed to get cpus node\n");
		return -EINVAL;
	}

	fdt_for_each_subnode(sub_node, blob, cpu_node) {
		pp = (u32 *)fdt_getprop(blob, sub_node, "operating-points-v2",
					&len);
		if (!pp)
			continue;
		pp[0] = cpu_to_fdt32(phandle);
	}

	return 0;
}

static int fdt_fixup_dmc_opp_table(const void *blob)
{
	int opp_node, dmc_node;
	int len;
	u32 phandle;
	u32 *pp;

	opp_node = fdt_path_offset(blob, "/rk3308bs-dmc-opp-table");
	if (opp_node < 0) {
		printf("Failed to get rk3308bs-dmc-opp-table node\n");
		return -EINVAL;
	}

	phandle = fdt_get_phandle(blob, opp_node);
	if (!phandle) {
		printf("Failed to get dmc opp table phandle\n");
		return -EINVAL;
	}

	dmc_node = fdt_path_offset(blob, "/dmc");
	if (dmc_node < 0) {
		printf("Failed to get dmc node\n");
		return -EINVAL;
	}

	pp = (u32 *)fdt_getprop(blob, dmc_node, "operating-points-v2", &len);
	if (!pp)
		return 0;
	pp[0] = cpu_to_fdt32(phandle);

	return 0;
}

static void fixup_pcfg_drive_strength(const void *blob, int noffset)
{
	u32 *ds, *dss;
	u32 val;

	dss = (u32 *)fdt_getprop(blob, noffset, "drive-strength-s", NULL);
	if (!dss)
		return;

	val = dss[0];
	ds = (u32 *)fdt_getprop(blob, noffset, "drive-strength", NULL);
	if (ds) {
		ds[0] = val;
	} else {
		if (fdt_setprop((void *)blob, noffset,
				"drive-strength", &val, 4) < 0)
			printf("Failed to add drive-strength prop\n");
	}
}

static int fdt_fixup_pcfg(const void *blob)
{
	int depth1_node;
	int depth2_node;
	int root_node, i;
	const char *path[] = { "/", "/pinctrl" };

	for (i = 0; i < ARRAY_SIZE(path); i++) {
		root_node = fdt_path_offset(blob, path[i]);
		if (root_node < 0)
			return root_node;

		fdt_for_each_subnode(depth1_node, blob, root_node) {
			debug("depth1: %s\n", fdt_get_name(blob, depth1_node, NULL));
			fixup_pcfg_drive_strength(blob, depth1_node);
			fdt_for_each_subnode(depth2_node, blob, depth1_node) {
				debug("    depth2: %s\n",
				      fdt_get_name(blob, depth2_node, NULL));
				fixup_pcfg_drive_strength(blob, depth2_node);
			}
		}
	}
	return 0;
}

static int fdt_fixup_thermal_zones(const void *blob)
{
	int thermal_node;
	int len;
	u32 *pp;
	u32 val;

	thermal_node = fdt_path_offset(blob, "/thermal-zones/soc-thermal");
	if (thermal_node < 0) {
		printf("Failed to get soc thermal node\n");
		return -EINVAL;
	}

	pp = (u32 *)fdt_getprop(blob, thermal_node,
				"rk3308bs-sustainable-power", &len);
	if (pp) {
		val = fdt32_to_cpu(*pp);
		pp = (u32 *)fdt_getprop(blob, thermal_node,
					"sustainable-power", &len);
		if (pp)
			pp[0] = cpu_to_fdt32(val);
	}

	return 0;
}

int rk_board_fdt_fixup(const void *blob)
{
	if (soc_is_rk3308bs()) {
		fdt_increase_size((void *)blob, SZ_8K);
		fdt_fixup_cpu_idle(blob);
		fdt_fixup_cpu_opp_table(blob);
		fdt_fixup_dmc_opp_table(blob);
		fdt_fixup_pcfg(blob);
		fdt_fixup_thermal_zones(blob);
	}

	return 0;
}

int rk_board_early_fdt_fixup(const void *blob)
{
	rk_board_fdt_fixup(blob);

	return 0;
}

