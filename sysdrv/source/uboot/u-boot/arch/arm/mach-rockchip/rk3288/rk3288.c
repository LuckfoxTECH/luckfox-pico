/*
 * Copyright (c) 2016 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#include <common.h>
#include <clk.h>
#include <dm.h>
#include <asm/armv7.h>
#include <asm/io.h>
#include <asm/arch/bootrom.h>
#include <asm/arch/clock.h>
#include <asm/arch/cpu.h>
#include <asm/arch/hardware.h>
#include <asm/arch/periph.h>
#include <asm/arch/cru_rk3288.h>
#include <asm/arch/grf_rk3288.h>
#include <asm/arch/pmu_rk3288.h>
#include <asm/arch/qos_rk3288.h>
#include <asm/arch/sys_proto.h>
#include <asm/gpio.h>
#include <dm/pinctrl.h>
#include <dt-bindings/clock/rk3288-cru.h>
#include <power/regulator.h>

#define GRF_BASE	0xff770000

#define VIO0_VOP_QOS_BASE	0xffad0408
#define VIO1_VOP_QOS_BASE	0xffad0008
#define BUS_MSCH0_QOS_BASE	0xffac0014
#define BUS_MSCH1_QOS_BASE	0xffac0094

#define READLATENCY_VAL		0x34
#define CRU_CLKSEL_CON28	0Xff7600d0

#define CPU_AXI_QOS_PRIORITY_LEVEL(h, l) \
	((((h) & 3) << 8) | (((h) & 3) << 2) | ((l) & 3))

const char * const boot_devices[BROM_LAST_BOOTSOURCE + 1] = {
	[BROM_BOOTSOURCE_EMMC] = "/dwmmc@ff0f0000",
	[BROM_BOOTSOURCE_SD] = "/dwmmc@ff0c0000",
};

#ifndef CONFIG_TPL_BUILD
#ifdef CONFIG_SPL_BUILD
static void configure_l2ctlr(void)
{
	uint32_t l2ctlr;

	l2ctlr = read_l2ctlr();
	l2ctlr &= 0xfffc0000; /* clear bit0~bit17 */

	/*
	* Data RAM write latency: 2 cycles
	* Data RAM read latency: 2 cycles
	* Data RAM setup latency: 1 cycle
	* Tag RAM write latency: 1 cycle
	* Tag RAM read latency: 1 cycle
	* Tag RAM setup latency: 1 cycle
	*/
	l2ctlr |= (1 << 3 | 1 << 0);
	write_l2ctlr(l2ctlr);
}
#endif

int arch_cpu_init(void)
{
	/* We do some SoC one time setting here. */
#ifdef CONFIG_SPL_BUILD
	configure_l2ctlr();
#else
	struct rk3288_grf * const grf = (void *)GRF_BASE;

	/* Use rkpwm by default */
	rk_setreg(&grf->soc_con2, 1 << 0);

	/* Disable LVDS phy */
	rk_setreg(&grf->soc_con7, 1 << 15);

	/* Select EDP clock source 24M */
	rk_setreg(CRU_CLKSEL_CON28, 1 << 15);

	/* Read latency configure */
	writel(READLATENCY_VAL, BUS_MSCH0_QOS_BASE);
	writel(READLATENCY_VAL, BUS_MSCH1_QOS_BASE);

	/* Set vop qos to highest priority */
	writel(CPU_AXI_QOS_PRIORITY_LEVEL(2, 2), VIO0_VOP_QOS_BASE);
	writel(CPU_AXI_QOS_PRIORITY_LEVEL(2, 2), VIO1_VOP_QOS_BASE);
#endif
	return 0;
}
#endif

void board_debug_uart_init(void)
{
	struct rk3288_grf * const grf = (void *)GRF_BASE;

	rk_clrsetreg(&grf->gpio7ch_iomux, GPIO7C7_MASK << GPIO7C7_SHIFT |
		     GPIO7C6_MASK << GPIO7C6_SHIFT,
		     GPIO7C7_UART2DBG_SOUT << GPIO7C7_SHIFT |
		     GPIO7C6_UART2DBG_SIN << GPIO7C6_SHIFT);
}

#if defined(CONFIG_SPL_BUILD) && !defined(CONFIG_TPL_BUILD)
#ifdef CONFIG_SPL_MMC_SUPPORT
static int configure_emmc(void)
{
#if defined(CONFIG_TARGET_CHROMEBOOK_JERRY)

	struct gpio_desc desc;
	int ret;
	struct udevice *pinctrl;

	pinctrl_request_noflags(pinctrl, PERIPH_ID_EMMC);

	/*
	 * TODO(sjg@chromium.org): Pick this up from device tree or perhaps
	 * use the EMMC_PWREN setting.
	 */
	ret = dm_gpio_lookup_name("D9", &desc);
	if (ret) {
		debug("gpio ret=%d\n", ret);
		return ret;
	}
	ret = dm_gpio_request(&desc, "emmc_pwren");
	if (ret) {
		debug("gpio_request ret=%d\n", ret);
		return ret;
	}
	ret = dm_gpio_set_dir_flags(&desc, GPIOD_IS_OUT);
	if (ret) {
		debug("gpio dir ret=%d\n", ret);
		return ret;
	}
	ret = dm_gpio_set_value(&desc, 1);
	if (ret) {
		debug("gpio value ret=%d\n", ret);
		return ret;
	}
#endif
	return 0;
}

int rk_spl_board_init(void)
{
	struct udevice *pinctrl;
	int ret = 0;

	ret = uclass_get_device(UCLASS_PINCTRL, 0, &pinctrl);
	if (ret) {
		debug("%s: Cannot find pinctrl device\n", __func__);
		goto err;
	}
	/* TODO: we may need to check boot device first */
#ifdef CONFIG_SPL_MMC_SUPPORT
	ret = pinctrl_request_noflags(pinctrl, PERIPH_ID_SDCARD);
	if (ret) {
		debug("%s: Failed to set up SD card\n", __func__);
		goto err;
	}
#endif

	ret = configure_emmc();
	if (ret) {
		debug("%s: Failed to set up eMMC\n", __func__);
	}

err:
	return ret;
}
#endif
#endif

int rk3288_qos_init(void)
{
	int val = 2 << PRIORITY_HIGH_SHIFT | 2 << PRIORITY_LOW_SHIFT;
	/* set vop qos to higher priority */
	writel(val, CPU_AXI_QOS_PRIORITY + VIO0_VOP_QOS);
	writel(val, CPU_AXI_QOS_PRIORITY + VIO1_VOP_QOS);

	if (!fdt_node_check_compatible(gd->fdt_blob, 0,
				       "rockchip,rk3288-tinker"))
	{
		/* set isp qos to higher priority */
		writel(val, CPU_AXI_QOS_PRIORITY + VIO1_ISP_R_QOS);
		writel(val, CPU_AXI_QOS_PRIORITY + VIO1_ISP_W0_QOS);
		writel(val, CPU_AXI_QOS_PRIORITY + VIO1_ISP_W1_QOS);
	}
	return 0;
}

static void rk3288_detect_reset_reason(void)
{
	struct rk3288_cru *cru = rockchip_get_cru();
	const char *reason;

	if (IS_ERR(cru))
		return;

	switch (cru->cru_glb_rst_st) {
	case GLB_POR_RST:
		reason = "POR";
		break;
	case FST_GLB_RST_ST:
	case SND_GLB_RST_ST:
		reason = "RST";
		break;
	case FST_GLB_TSADC_RST_ST:
	case SND_GLB_TSADC_RST_ST:
		reason = "THERMAL";
		break;
	case FST_GLB_WDT_RST_ST:
	case SND_GLB_WDT_RST_ST:
		reason = "WDOG";
		break;
	default:
		reason = "unknown reset";
	}

	env_set("reset_reason", reason);

	/*
	 * Clear cru_glb_rst_st, so we can determine the last reset cause
	 * for following resets.
	 */
	rk_clrreg(&cru->cru_glb_rst_st, GLB_RST_ST_MASK);
}

__weak int rk3288_board_late_init(void)
{
	return 0;
}

int rk_board_late_init(void)
{
	rk3288_qos_init();
	rk3288_detect_reset_reason();

	return rk3288_board_late_init();
}

#if !CONFIG_IS_ENABLED(ROCKCHIP_BACK_TO_BROM)
static int veyron_init(void)
{
	struct udevice *dev;
	struct clk clk;
	int ret;

	ret = regulator_get_by_platname("vdd_arm", &dev);
	if (ret) {
		debug("Cannot set regulator name\n");
		return ret;
	}

	/* Slowly raise to max CPU voltage to prevent overshoot */
	ret = regulator_set_value(dev, 1200000);
	if (ret)
		return ret;
	udelay(175); /* Must wait for voltage to stabilize, 2mV/us */
	ret = regulator_set_value(dev, 1400000);
	if (ret)
		return ret;
	udelay(100); /* Must wait for voltage to stabilize, 2mV/us */

	ret = rockchip_get_clk(&clk.dev);
	if (ret)
		return ret;
	clk.id = PLL_APLL;
	ret = clk_set_rate(&clk, 1800000000);
	if (IS_ERR_VALUE(ret))
		return ret;

	return 0;
}

int rk_board_init(void)
{
	int ret;

	/* We do some SoC one time setting here */
	if (!fdt_node_check_compatible(gd->fdt_blob, 0, "google,veyron")) {
		ret = veyron_init();
		if (ret)
			return ret;
	}

	return 0;
}
#endif

static int do_clock(cmd_tbl_t *cmdtp, int flag, int argc,
		       char * const argv[])
{
	static const struct {
		char *name;
		int id;
	} clks[] = {
		{ "osc", CLK_OSC },
		{ "apll", CLK_ARM },
		{ "dpll", CLK_DDR },
		{ "cpll", CLK_CODEC },
		{ "gpll", CLK_GENERAL },
#ifdef CONFIG_ROCKCHIP_RK3036
		{ "mpll", CLK_NEW },
#else
		{ "npll", CLK_NEW },
#endif
	};
	int ret, i;
	struct udevice *dev;

	ret = rockchip_get_clk(&dev);
	if (ret) {
		printf("clk-uclass not found\n");
		return 0;
	}

	for (i = 0; i < ARRAY_SIZE(clks); i++) {
		struct clk clk;
		ulong rate;

		clk.id = clks[i].id;
		ret = clk_request(dev, &clk);
		if (ret < 0)
			continue;

		rate = clk_get_rate(&clk);
		printf("%s: %lu\n", clks[i].name, rate);

		clk_free(&clk);
	}

	return 0;
}

U_BOOT_CMD(
	clock, 2, 1, do_clock,
	"display information about clocks",
	""
);

#define GRF_SOC_CON2 0xff77024c

int board_early_init_f(void)
{
	struct udevice *pinctrl;
	struct udevice *dev;
	int ret;

	/*
	 * This init is done in SPL, but when chain-loading U-Boot SPL will
	 * have been skipped. Allow the clock driver to check if it needs
	 * setting up.
	 */
	ret = rockchip_get_clk(&dev);
	if (ret) {
		debug("CLK init failed: %d\n", ret);
		return ret;
	}
	ret = uclass_get_device(UCLASS_PINCTRL, 0, &pinctrl);
	if (ret) {
		debug("%s: Cannot find pinctrl device\n", __func__);
		return ret;
	}

	/* Enable debug UART */
	ret = pinctrl_request_noflags(pinctrl, PERIPH_ID_UART_DBG);
	if (ret) {
		debug("%s: Failed to set up console UART\n", __func__);
		return ret;
	}
	rk_setreg(GRF_SOC_CON2, 1 << 0);

	return 0;
}

#ifndef CONFIG_SPL_BUILD
int rk_board_fdt_fixup(void *blob)
{
	/* RK3288: Recognize RK3288W by HDMI Revision ID is 0x1A; */
	if (soc_is_rk3288w()) {
		if (fdt_setprop_string(blob, 0,
				       "compatible", "rockchip,rk3288w"))
			printf("RK3288w set compatible failed!\n");
	}

	return 0;
}
#endif
