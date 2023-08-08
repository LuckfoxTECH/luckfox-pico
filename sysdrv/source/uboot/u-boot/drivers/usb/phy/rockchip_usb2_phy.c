/*
 * Copyright 2016 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:    GPL-2.0+
 */

#include <common.h>
#include <asm/arch/clock.h>
#include <asm/io.h>
#include <dm.h>
#include <fdtdec.h>
#include <fdt_support.h>
#include <syscon.h>
#include <linux/libfdt.h>

#include "../gadget/dwc2_udc_otg_priv.h"

DECLARE_GLOBAL_DATA_PTR;

#define BIT_WRITEABLE_SHIFT	16

struct usb2phy_reg {
	unsigned int offset;
	unsigned int bitend;
	unsigned int bitstart;
	unsigned int disable;
	unsigned int enable;
};

/**
 * struct rockchip_usb2_phy_cfg: usb-phy port configuration
 * @port_reset: usb otg per-port reset register
 * @soft_con: software control usb otg register
 * @suspend: phy suspend register
 */
struct rockchip_usb2_phy_cfg {
	struct usb2phy_reg port_reset;
	struct usb2phy_reg siddq;
	struct usb2phy_reg soft_con;
	struct usb2phy_reg suspend;
};

struct rockchip_usb2_phy_dt_id {
	char		compatible[128];
	const void	*data;
};

static const struct rockchip_usb2_phy_cfg rk3288_pdata = {
	.port_reset     = {0x00, 12, 12, 0, 1},
	.siddq		= {0x00, 13, 13, 0, 1},
	.soft_con       = {0x08, 2, 2, 0, 1},
	.suspend	= {0x0c, 5, 0, 0x01, 0x2A},
};

static struct rockchip_usb2_phy_dt_id rockchip_usb2_phy_dt_ids[] = {
	{ .compatible = "rockchip,rk3288-usb-phy", .data = &rk3288_pdata },
	{}
};

static void property_enable(struct dwc2_plat_otg_data *pdata,
				  const struct usb2phy_reg *reg, bool en)
{
	unsigned int val, mask, tmp;

	tmp = en ? reg->enable : reg->disable;
	mask = GENMASK(reg->bitend, reg->bitstart);
	val = (tmp << reg->bitstart) | (mask << BIT_WRITEABLE_SHIFT);

	writel(val, pdata->regs_phy + reg->offset);
}

int rockchip_u2phy_vbus_detect(void)
{
	u32 val = 0;

#ifdef CONFIG_ROCKCHIP_RK3288
	u32 grf_base = (u32)syscon_get_first_range(ROCKCHIP_SYSCON_GRF);

	val = readl(grf_base + 0x288);
	val = (val & BIT(14)) >> 14;
#endif

	return val;
}

static int otg_phy_parse(struct dwc2_udc *dev)
{
	int node, phy_node;
	u32 grf_base, grf_offset;
	const void *blob = gd->fdt_blob;
	const fdt32_t *reg;
	fdt_addr_t addr;
	struct dwc2_plat_otg_data *pdata = dev->pdata;

	/* Find the usb_otg node */
	node = fdt_node_offset_by_compatible(blob, -1, "snps,dwc2");

#if defined(CONFIG_ROCKCHIP_RK3288)
retry:
#endif
	if (node > 0) {
		reg = fdt_getprop(blob, node, "reg", NULL);
		if (!reg)
			return -EINVAL;

		addr = fdt_translate_address(blob, node, reg);
		if (addr == OF_BAD_ADDR) {
			pr_err("Not found usb_otg address\n");
			return -EINVAL;
		}

#if defined(CONFIG_ROCKCHIP_RK3288)
		if (addr != 0xff580000) {
			node = fdt_node_offset_by_compatible(blob, node,
							     "snps,dwc2");
			goto retry;
		}
#endif
	} else {
		/*
		 * With kernel dtb support, rk3288 dwc2 otg node
		 * use the rockchip legacy dwc2 driver "dwc_otg_310"
		 * with the compatible "rockchip,rk3288_usb20_otg".
		 */
#if defined(CONFIG_ROCKCHIP_RK3288)
		node = fdt_node_offset_by_compatible(blob, -1,
				"rockchip,rk3288_usb20_otg");
#endif
		if (node < 0) {
			pr_err("Not found usb_otg device\n");
			return -ENODEV;
		}
	}

	/* Find the usb phy node */
	node = fdtdec_lookup_phandle(blob, node, "phys");
	if (node <= 0) {
		pr_err("Not found usbphy device\n");
		return -ENODEV;
	}

	/* Find the usb otg-phy node */
	phy_node = fdt_parent_offset(blob, node);
	if (phy_node <= 0) {
		pr_err("Not found sub usbphy device\n");
		return -ENODEV;
	}

	grf_base = (u32)syscon_get_first_range(ROCKCHIP_SYSCON_GRF);
	grf_offset = fdtdec_get_addr(blob, node, "reg");

	/* Pad dwc2_plat_otg_data related to phy */
	pdata->phy_of_node = phy_node;
	pdata->regs_phy = grf_base + grf_offset;

	return 0;
}

void otg_phy_init(struct dwc2_udc *dev)
{
	struct dwc2_plat_otg_data *pdata = dev->pdata;
	struct rockchip_usb2_phy_cfg *phy_cfg = NULL;
	struct rockchip_usb2_phy_dt_id *of_id;
	int i;

	if (!pdata->regs_phy && otg_phy_parse(dev)) {
		pr_err("otg-phy parse error\n");
		return;
	}

	for (i = 0; i < ARRAY_SIZE(rockchip_usb2_phy_dt_ids); i++) {
		of_id = &rockchip_usb2_phy_dt_ids[i];
		if (fdt_node_check_compatible(gd->fdt_blob, pdata->phy_of_node,
					      of_id->compatible) == 0) {
			phy_cfg = (struct rockchip_usb2_phy_cfg *)of_id->data;
			break;
		}
	}
	if (!phy_cfg) {
		debug("Can't find device platform data\n");

		hang();
		return;
	}
	pdata->priv = phy_cfg;

	/* power up usb phy analog blocks by set siddq 0 */
	property_enable(pdata, &phy_cfg->siddq, false);

	/* disable software control */
	property_enable(pdata, &phy_cfg->soft_con, false);

	/* reset otg port */
	property_enable(pdata, &phy_cfg->port_reset, true);
	mdelay(1);
	property_enable(pdata, &phy_cfg->port_reset, false);
	udelay(1);
}

void otg_phy_off(struct dwc2_udc *dev)
{
	struct dwc2_plat_otg_data *pdata = dev->pdata;
	struct rockchip_usb2_phy_cfg *phy_cfg = pdata->priv;

	if (!pdata->regs_phy && otg_phy_parse(dev)) {
		pr_err("otg-phy parse error\n");
		return;
	}

	/* enable software control */
	property_enable(pdata, &phy_cfg->soft_con, true);
	/* enter suspend */
	property_enable(pdata, &phy_cfg->suspend, true);
}
