// SPDX-License-Identifier:     GPL-2.0+
/*
 * Copyright (c) 2018 Rockchip Electronics Co., Ltd
 *
 */
#include <common.h>
#include <android_image.h>
#include <boot_rkimg.h>
#include <ramdisk.h>
#include <asm/io.h>
#include <asm/arch/grf_rk1808.h>
#include <asm/arch/hardware.h>
#include <asm/arch/rk_atags.h>
#include <asm/arch/rockchip_smccc.h>
#include <asm/gpio.h>
#include <debug_uart.h>

#include <asm/armv8/mmu.h>

#define NIU_CIF_ADDR		0xfe8a0188
#define NIU_ISP_ADDR		0xfe8a0008
#define QOS_PRIORITY_LEVEL(h, l)	((((h) & 3) << 8) | ((l) & 3))

static struct mm_region rk1808_mem_map[] = {
	{
		.virt = 0x0UL,
		.phys = 0x0UL,
		.size = 0xff000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) |
			 PTE_BLOCK_INNER_SHARE
	}, {
		.virt = 0xf8000000UL,
		.phys = 0xf8000000UL,
		.size = 0x08000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			 PTE_BLOCK_NON_SHARE |
			 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	}, {
		/* List terminator */
		0,
	}
};

struct mm_region *mem_map = rk1808_mem_map;

#define GRF_BASE	0xfe000000
#define PMUGRF_BASE	0xfe020000

enum {
	GPIO4A3_SHIFT           = 12,
	GPIO4A3_MASK            = GENMASK(15, 12),
	GPIO4A3_GPIO            = 0,
	GPIO4A3_SDMMC0_D1,
	GPIO4A3_UART2_RX_M0,

	GPIO4A2_SHIFT           = 8,
	GPIO4A2_MASK            = GENMASK(11, 8),
	GPIO4A2_GPIO            = 0,
	GPIO4A2_SDMMC0_D0,
	GPIO4A2_UART2_TX_M0,

	UART2_IO_SEL_SHIFT	= 14,
	UART2_IO_SEL_MASK	= GENMASK(15, 14),
	UART2_IO_SEL_M0		= 0,
	UART2_IO_SEL_M1,
	UART2_IO_SEL_M2,
	UART2_IO_SEL_USB,
};

#define SECURE_FIRE_WALL 0xff590040

int arch_cpu_init(void)
{
	/* Set cif qos priority */
	writel(QOS_PRIORITY_LEVEL(2, 2), NIU_CIF_ADDR);
	writel(QOS_PRIORITY_LEVEL(2, 2), NIU_ISP_ADDR);

	/* Set dram to unsecure */
#ifdef CONFIG_SPL_BUILD
	writel(0, SECURE_FIRE_WALL);
#endif

	return 0;
}

/*
 * Default use UART2_TX/RX_M0(TX: GPIO4_A2, RX: GPIO4_A3)
 */
void board_debug_uart_init(void)
{
#ifdef CONFIG_TPL_BUILD
	struct rk1808_grf * const grf = (void *)GRF_BASE;

	/* Enable early UART2 channel m0 on the rk1808 */
	rk_clrsetreg(&grf->iofunc_con0, UART2_IO_SEL_MASK,
		     UART2_IO_SEL_M0 << UART2_IO_SEL_SHIFT);

	/* Switch iomux */
	rk_clrsetreg(&grf->gpio4a_iomux_l,
		     GPIO4A3_MASK | GPIO4A2_MASK,
		     GPIO4A2_UART2_TX_M0 << GPIO4A2_SHIFT |
		     GPIO4A3_UART2_RX_M0 << GPIO4A3_SHIFT);
#endif
}

#if !defined(CONFIG_SPL_BUILD) && !defined(CONFIG_COPROCESSOR_RK1808)
#define PINCTRL_EMMC_BUS8_PATH		"/pinctrl/emmc/emmc-bus8"
#define PINCTRL_EMMC_CMD_PATH		"/pinctrl/emmc/emmc-cmd"
#define PINCTRL_EMMC_CLK_PATH		"/pinctrl/emmc/emmc-clk"
#define PINCTRL_PCFG_PU_2MA_PATH	"/pinctrl/pcfg-pull-up-2ma"
#define MAX_ROCKCHIP_PINS_ENTRIES	12

static int rockchip_pinctrl_cfg_fdt_fixup(const char *path, u32 new_phandle)
{
	u32 cells[MAX_ROCKCHIP_PINS_ENTRIES * 4];
	const u32 *data;
	int i, count;
	int node;

	node = fdt_path_offset(gd->fdt_blob, path);
	if (node < 0) {
		debug("%s: can't find: %s\n", __func__, path);
		return node;
	}

	data = fdt_getprop(gd->fdt_blob, node, "rockchip,pins", &count);
	if (!data) {
		debug("%s: can't find prop \"rockchip,pins\"\n", __func__);
		return -ENODATA;
	}

	count /= sizeof(u32);
	if (count > MAX_ROCKCHIP_PINS_ENTRIES * 4) {
		debug("%s: %d is over max count\n", __func__, count);
		return -EINVAL;
	}

	for (i = 0; i < count; i++)
		cells[i] = data[i];

	for (i = 0; i < (count >> 2); i++)
		cells[4 * i + 3] = cpu_to_fdt32(new_phandle);

	fdt_setprop((void *)gd->fdt_blob, node, "rockchip,pins",
		    &cells, count * sizeof(u32));

	return 0;
}

int rk_board_fdt_fixup(void *blob)
{
	struct tag *t;
	u32 ph_pu_2ma;
	int ret = 0;

	t = atags_get_tag(ATAG_SOC_INFO);
	if (!t)
		return 0;

	debug("soc=0x%x, flags=0x%x\n", t->u.soc.name, t->u.soc.flags);

	if (t->u.soc.flags != 0x45543030)
		return 0;

	ph_pu_2ma = fdt_get_phandle(gd->fdt_blob,
		    fdt_path_offset(gd->fdt_blob, PINCTRL_PCFG_PU_2MA_PATH));
	if (!ph_pu_2ma) {
		debug("Can't find: %s\n", PINCTRL_PCFG_PU_2MA_PATH);
		return -EINVAL;
	}

	ret |= rockchip_pinctrl_cfg_fdt_fixup(PINCTRL_EMMC_BUS8_PATH, ph_pu_2ma);
	ret |= rockchip_pinctrl_cfg_fdt_fixup(PINCTRL_EMMC_CMD_PATH, ph_pu_2ma);
	ret |= rockchip_pinctrl_cfg_fdt_fixup(PINCTRL_EMMC_CLK_PATH, ph_pu_2ma);

	return ret;
}
#endif

/*
 * rockchip: rk1808: fixup "ramdisk_addr_r" by real image address
 *
 * We fixup "ramdisk_addr_r" with real image address which has been already
 * loaded by pre-loader. This brings benefits:
 *
 * - It saves boot time when ramdisk is large;
 * - It avoids memory overlap with original ramdisk data.
 */
#if defined(CONFIG_DM_RAMDISK) && !defined(CONFIG_SPL_BUILD)
static int env_fixup_ramdisk_addr_r(void)
{
	const char *boot_part = PART_BOOT;
	struct blk_desc *dev_desc;
	struct andr_img_hdr *hdr;
	disk_partition_t info;
	ulong ramdisk_addr_r;
	int ret;

	/*
	 * Don't rely on CONFIG_DM_RAMDISK since it can be a default
	 * configuration after disk/part_rkram.c was introduced.
	 *
	 * This is compatible code.
	 */
	if (!dm_ramdisk_is_enabled())
		return 0;

	dev_desc = rockchip_get_bootdev();
	if (!dev_desc) {
		printf("%s: dev_desc is NULL!\n", __func__);
		return -ENODEV;
	}

	ret = part_get_info_by_name(dev_desc, boot_part, &info);
	if (ret < 0) {
		printf("%s: failed to get %s part, ret=%d\n",
		       __func__, boot_part, ret);
	}

	hdr = (struct andr_img_hdr *)(info.start * dev_desc->blksz);
	ramdisk_addr_r = (ulong)hdr;
	ramdisk_addr_r += hdr->page_size;
	ramdisk_addr_r += ALIGN(hdr->kernel_size, hdr->page_size);
	env_set_hex("ramdisk_addr_r", ramdisk_addr_r);

	return 0;
}
#endif

int rk_board_init(void)
{
	struct rk1808_pmugrf * const pmugrf = (void *)PMUGRF_BASE;

	/* Set GPIO0_C2 default to pull down from normal */
	rk_clrsetreg(&pmugrf->gpio0c_p, 0x30, 0x20);

#if defined(CONFIG_ROCKCHIP_SMCCC) && defined(CONFIG_ROCKCHIP_RK1806)
	sip_smc_get_sip_version();
#endif
	return 0;
}

int rk_board_late_init(void)
{
#if defined(CONFIG_DM_RAMDISK) && !defined(CONFIG_SPL_BUILD)
	env_fixup_ramdisk_addr_r();
#endif

	return 0;
}

void mmc_gpio_init_direct(void)
{
	struct rk1808_grf * const grf = (void *)GRF_BASE;

	/*
	 * The rk1808's pin drive strength control must set to 2ma.
	 */
	rk_clrsetreg(&grf->gpio1a_e, 0xffff, 0x5555);
	rk_clrsetreg(&grf->gpio1b_e, 0xff, 0x00);
}
