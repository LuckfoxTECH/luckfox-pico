/*
 * (C) Copyright 2022 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <dm.h>
#include <i2c.h>
#include <irq-generic.h>
#include <power/fuel_gauge.h>
#include <power/power_delivery/power_delivery.h>
#include <linux/usb/phy-rockchip-usb2.h>

DECLARE_GLOBAL_DATA_PTR;

static int dbg_enable;

#define SC8551_DBG(args...) \
	do { \
		if (dbg_enable) { \
			printf(args); \
		} \
	} while (0)

struct sc8551 {
	struct udevice *dev;
	struct udevice *pd;
	bool pd_online;
	u32 init_count;
	u32 ichg;
	u32 vchg;
	int irq;
};

enum charger_type {
	CHARGER_TYPE_NO = 0,
	CHARGER_TYPE_USB,
	CHARGER_TYPE_AC,
	CHARGER_TYPE_DC,
	CHARGER_TYPE_UNDEF,
};

/* Register 05h */
#define SC8551_REG_05				0x05
#define SC8551_AC_OVP_STAT_MASK			0x80
#define SC8551_AC_OVP_STAT_SHIFT		7

#define SC8551_AC_OVP_MASK			0x07
#define SC8551_AC_OVP_SHIFT			0
#define SC8551_AC_OVP_BASE			11
#define SC8551_AC_OVP_LSB			1
#define SC8551_AC_OVP_6P5V			65

/* Register 0Bh */
#define SC8551_REG_0B				0x0B
#define SC8551_REG_RST_MASK			0x80
#define SC8551_REG_RST_SHIFT			7
#define SC8551_REG_RST_ENABLE			1
#define SC8551_REG_RST_DISABLE			0
#define SC8551_AC_OVP_VOLTAGE			13

static int sc8551_read(struct sc8551 *charger, uint reg, u8 *buffer)
{
	u16 val;
	int ret;

	ret = dm_i2c_read(charger->dev, reg, (u8 *)&val, 1);
	if (ret) {
		printf("sc8551: read %#x error, ret=%d", reg, ret);
		return ret;
	}

	*buffer = val;
	return 0;
}

static int sc8551_write(struct sc8551 *charger, uint reg, u16 val)
{
	int ret;

	ret = dm_i2c_write(charger->dev, reg, (u8 *)&val, 1);
	if (ret)
		printf("sc8551: write %#x error, ret=%d", reg, ret);

	return ret;
}

static int sc8551_update_bits(struct sc8551 *charger,
			      uint offset,
			      uint mask,
			      uint val)
{
	u8 reg;
	int ret;

	ret = sc8551_read(charger, offset, &reg);
	if (ret)
		return ret;

	reg &= ~mask;

	return sc8551_write(charger, offset, reg | val);
}

static int sc8551_charger_capability(struct udevice *dev)
{
	return 0;
}

static int sc8551_dwc_otg_check_dpdm(void)
{
#if defined(CONFIG_PHY_ROCKCHIP_INNO_USB2) && !defined(CONFIG_SPL_BUILD)
	return rockchip_chg_get_type();
#else
	debug("rockchip_chg_get_type() is not implement\n");
	return CHARGER_TYPE_NO;
#endif
}

static int sc8551_get_usb_state(struct sc8551 *sc8551)
{
	int charger_type;

	switch (sc8551_dwc_otg_check_dpdm()) {
	case 0:
		charger_type = CHARGER_TYPE_NO;
		break;
	case 1:
	case 3:
		charger_type = CHARGER_TYPE_USB;
		break;
	case 2:
		charger_type = CHARGER_TYPE_AC;
		break;
	default:
		charger_type = CHARGER_TYPE_NO;
		break;
	}

	return charger_type;
}

static bool sc8551_check_charge(struct sc8551 *sc8551)
{
	if (sc8551_get_usb_state(sc8551) != CHARGER_TYPE_NO)
		return true;

	return false;
}

static bool sc8551_update_get_chrg_online(struct udevice *dev)
{
	struct sc8551 *sc8551 = dev_get_priv(dev);

	return sc8551_check_charge(sc8551);
}

static int sc8551_ofdata_to_platdata(struct udevice *dev)
{
	return 0;
}

static int sc8551_set_acovp_th(struct sc8551 *sc, int threshold)
{
	int ret;
	u8 val;

	if (threshold < SC8551_AC_OVP_BASE)
		threshold = SC8551_AC_OVP_BASE;

	if (threshold == SC8551_AC_OVP_6P5V)
		val = 0x07;
	else
		val = (threshold - SC8551_AC_OVP_BASE) / SC8551_AC_OVP_LSB;

	val <<= SC8551_AC_OVP_SHIFT;

	ret = sc8551_update_bits(sc, SC8551_REG_05,
				 SC8551_AC_OVP_MASK, val);

	return ret;
}

static int sc8551_probe(struct udevice *dev)
{
	struct sc8551 *charger = dev_get_priv(dev);

	SC8551_DBG("sc8551: driver version-20220903\n");

	charger->dev = dev;

	SC8551_DBG("sc8551: not reset\n");

	sc8551_set_acovp_th(charger, SC8551_AC_OVP_VOLTAGE);

	return 0;
}

static const struct udevice_id charger_ids[] = {
	{ .compatible = "sc,sc8551-standalone" },
	{ },
};

static struct dm_fuel_gauge_ops charger_ops = {
	.get_chrg_online = sc8551_update_get_chrg_online,
	.capability = sc8551_charger_capability,
};

U_BOOT_DRIVER(sc8551_charger) = {
	.name = "sc8551_charger",
	.id = UCLASS_FG,
	.probe = sc8551_probe,
	.of_match = charger_ids,
	.ops = &charger_ops,
	.ofdata_to_platdata = sc8551_ofdata_to_platdata,
	.priv_auto_alloc_size = sizeof(struct sc8551),
};
