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
#include <linux/usb/phy-rockchip-usb2.h>
#include <power/power_delivery/power_delivery.h>

DECLARE_GLOBAL_DATA_PTR;

static int dbg_enable;

#define SGM_DBG(args...) \
	do { \
		if (dbg_enable) { \
			printf(args); \
		} \
	} while (0)

/* define register */
#define SGM4154x_CHRG_CTRL_0			0x00
#define SGM4154x_CHRG_CTRL_1			0x01
#define SGM4154x_CHRG_CTRL_2			0x02
#define SGM4154x_CHRG_CTRL_3			0x03
#define SGM4154x_CHRG_CTRL_4			0x04
#define SGM4154x_CHRG_CTRL_5			0x05
#define SGM4154x_CHRG_CTRL_6			0x06
#define SGM4154x_CHRG_CTRL_7			0x07
#define SGM4154x_CHRG_STAT			0x08
#define SGM4154x_CHRG_FAULT			0x09
#define SGM4154x_CHRG_CTRL_a			0x0a
#define SGM4154x_CHRG_CTRL_b			0x0b
#define SGM4154x_CHRG_CTRL_c			0x0c
#define SGM4154x_CHRG_CTRL_d			0x0d
#define SGM4154x_INPUT_DET			0x0e
#define SGM4154x_CHRG_CTRL_f			0x0f

/* charge status flags */
#define SGM4154x_CHRG_EN			BIT(4)
#define SGM4154x_HIZ_EN				BIT(7)
#define SGM4154x_TERM_EN			BIT(7)
#define SGM4154x_VAC_OVP_MASK			GENMASK(7, 6)
#define SGM4154x_DPDM_ONGOING			BIT(7)
#define SGM4154x_VBUS_GOOD			BIT(7)

#define SGM4154x_BOOSTV				GENMASK(5, 4)
#define SGM4154x_BOOST_LIM			BIT(7)
#define SGM4154x_OTG_EN				BIT(5)

/* Part ID */
#define SGM4154x_PN_MASK			GENMASK(6, 3)

/* WDT TIMER SET */
#define SGM4154x_WDT_TIMER_MASK			GENMASK(5, 4)
#define SGM4154x_WDT_TIMER_DISABLE		0
#define SGM4154x_WDT_TIMER_40S			BIT(4)
#define SGM4154x_WDT_TIMER_80S			BIT(5)
#define SGM4154x_WDT_TIMER_160S			(BIT(4) | BIT(5))

#define SGM4154x_WDT_RST_MASK			BIT(6)

/* SAFETY TIMER SET */
#define SGM4154x_SAFETY_TIMER_MASK		GENMASK(3, 3)
#define SGM4154x_SAFETY_TIMER_DISABLE		0
#define SGM4154x_SAFETY_TIMER_EN		BIT(3)
#define SGM4154x_SAFETY_TIMER_5H		0
#define SGM4154x_SAFETY_TIMER_10H		BIT(2)

/* recharge voltage */
#define SGM4154x_VRECHARGE			BIT(0)
#define SGM4154x_VRECHRG_STEP_mV		100
#define SGM4154x_VRECHRG_OFFSET_mV		100

/* charge status */
#define SGM4154x_VSYS_STAT			BIT(0)
#define SGM4154x_THERM_STAT			BIT(1)
#define SGM4154x_PG_STAT			BIT(2)
#define SGM4154x_CHG_STAT_MASK			GENMASK(4, 3)
#define SGM4154x_PRECHRG			BIT(3)
#define SGM4154x_FAST_CHRG			BIT(4)
#define SGM4154x_TERM_CHRG			(BIT(3) | BIT(4))

/* charge type */
#define SGM4154x_VBUS_STAT_MASK			GENMASK(7, 5)
#define SGM4154x_NOT_CHRGING			0
#define SGM4154x_USB_SDP			BIT(5)
#define SGM4154x_USB_CDP			BIT(6)
#define SGM4154x_USB_DCP			(BIT(5) | BIT(6))
#define SGM4154x_UNKNOWN			(BIT(7) | BIT(5))
#define SGM4154x_NON_STANDARD			(BIT(7) | BIT(6))
#define SGM4154x_OTG_MODE			(BIT(7) | BIT(6) | BIT(5))

/* TEMP Status */
#define SGM4154x_TEMP_MASK			GENMASK(2, 0)
#define SGM4154x_TEMP_NORMAL			BIT(0)
#define SGM4154x_TEMP_WARM			BIT(1)
#define SGM4154x_TEMP_COOL			(BIT(0) | BIT(1))
#define SGM4154x_TEMP_COLD			(BIT(0) | BIT(3))
#define SGM4154x_TEMP_HOT			(BIT(2) | BIT(3))

/* precharge current */
#define SGM4154x_PRECHRG_CUR_MASK		GENMASK(7, 4)
#define SGM4154x_PRECHRG_CURRENT_STEP_uA	60000
#define SGM4154x_PRECHRG_I_MIN_uA		60000
#define SGM4154x_PRECHRG_I_MAX_uA		780000
#define SGM4154x_PRECHRG_I_DEF_uA		180000

/* termination current */
#define SGM4154x_TERMCHRG_CUR_MASK		GENMASK(3, 0)
#define SGM4154x_TERMCHRG_CURRENT_STEP_uA	60000
#define SGM4154x_TERMCHRG_I_MIN_uA		60000
#define SGM4154x_TERMCHRG_I_MAX_uA		960000
#define SGM4154x_TERMCHRG_I_DEF_uA		180000

/* charge current */
#define SGM4154x_ICHRG_CUR_MASK			GENMASK(5, 0)
#define SGM4154x_ICHRG_I_STEP_uA		60000
#define SGM4154x_ICHRG_I_MIN_uA			0
#define SGM4154x_ICHRG_I_MAX_uA			3780000
#define SGM4154x_ICHRG_I_DEF_uA			2040000

/* charge voltage */
#define SGM4154x_VREG_V_MASK			GENMASK(7, 3)
#define SGM4154x_VREG_V_MAX_uV			4624000
#define SGM4154x_VREG_V_MIN_uV			3856000
#define SGM4154x_VREG_V_DEF_uV			4208000
#define SGM4154x_VREG_V_STEP_uV			32000

/* VREG Fine Tuning */
#define SGM4154x_VREG_FT_MASK			GENMASK(7, 6)
#define SGM4154x_VREG_FT_UP_8mV			BIT(6)
#define SGM4154x_VREG_FT_DN_8mV			BIT(7)
#define SGM4154x_VREG_FT_DN_16mV		(BIT(7) | BIT(6))

/* iindpm current */
#define SGM4154x_IINDPM_I_MASK			GENMASK(4, 0)
#define SGM4154x_IINDPM_I_MIN_uA		100000
#define SGM4154x_IINDPM_I_MAX_uA		2000000
#define SGM4154x_IINDPM_STEP_uA			100000
#define SGM4154x_IINDPM_DEF_uA			2400000

#define SGM4154x_VINDPM_INT_MASK		BIT(1)
#define SGM4154x_VINDPM_INT_DIS			BIT(1)
#define SGM4154x_IINDPM_INT_MASK		BIT(0)
#define SGM4154x_IINDPM_INT_DIS			BIT(0)

/* vindpm voltage */
#define SGM4154x_VINDPM_V_MASK			GENMASK(3, 0)
#define SGM4154x_VINDPM_V_MIN_uV		3900000
#define SGM4154x_VINDPM_V_MAX_uV		12000000
#define SGM4154x_VINDPM_STEP_uV			100000
#define SGM4154x_VINDPM_DEF_uV			4500000
#define SGM4154x_VINDPM_OS_MASK			GENMASK(1, 0)

/* DP DM SEL */
#define SGM4154x_DP_VSEL_MASK			GENMASK(4, 3)
#define SGM4154x_DM_VSEL_MASK			GENMASK(2, 1)

/* PUMPX SET */
#define SGM4154x_EN_PUMPX			BIT(7)
#define SGM4154x_PUMPX_UP			BIT(6)
#define SGM4154x_PUMPX_DN			BIT(5)

struct sgm41542 {
	struct udevice *dev;
	struct udevice *pd;
	bool pd_online;
	u32 init_count;
	u32 ichg;
	u32 vchg;
	int irq;
};

enum power_supply_type {
	POWER_SUPPLY_TYPE_UNKNOWN = 0,
	POWER_SUPPLY_TYPE_USB,          /* Standard Downstream Port */
	POWER_SUPPLY_TYPE_USB_DCP,      /* Dedicated Charging Port */
	POWER_SUPPLY_TYPE_USB_CDP,      /* Charging Downstream Port */
	POWER_SUPPLY_TYPE_USB_FLOATING, /* DCP without shorting D+/D- */
};

static int sgm41542_read(struct sgm41542 *charger, uint reg, u8 *buffer)
{
	u8 val;
	int ret;

	ret = dm_i2c_read(charger->dev, reg, &val, 1);
	if (ret) {
		printf("sgm41542: read %#x error, ret=%d", reg, ret);
		return ret;
	}

	*buffer = val;
	return 0;
}

static int sgm41542_write(struct sgm41542 *charger, uint reg, u8 val)
{
	int ret;

	ret = dm_i2c_write(charger->dev, reg, &val, 1);
	if (ret)
		printf("sgm41542: write %#x error, ret=%d", reg, ret);

	return ret;
}

static int sgm41542_update_bits(struct sgm41542 *charger,
				u8 offset,
				u8 mask,
				u8 val)
{
	u8 reg;

	sgm41542_read(charger, offset, &reg);

	reg &= ~mask;

	return sgm41542_write(charger, offset, reg | val);
}

static int sgm4154x_set_input_curr_lim(struct sgm41542 *charger, int iindpm)
{
	u8 reg_val;
	int ret;

	if (iindpm < SGM4154x_IINDPM_I_MIN_uA)
		iindpm = SGM4154x_IINDPM_I_MIN_uA;
	if (iindpm >= SGM4154x_IINDPM_I_MAX_uA)
		iindpm = SGM4154x_IINDPM_I_MAX_uA;

	if (iindpm >= SGM4154x_IINDPM_I_MIN_uA &&
	    iindpm <= SGM4154x_IINDPM_I_MAX_uA)
		reg_val = (iindpm - SGM4154x_IINDPM_I_MIN_uA) / SGM4154x_IINDPM_STEP_uA;

	ret = sgm41542_update_bits(charger,
				   SGM4154x_CHRG_CTRL_0,
				   SGM4154x_IINDPM_I_MASK,
				   reg_val);

	return ret;
}

static int sgm41542_get_usb_type(void)
{
#ifdef CONFIG_PHY_ROCKCHIP_INNO_USB2
	return rockchip_chg_get_type();
#else
	return 0;
#endif
}

static int sgm41542_charger_capability(struct udevice *dev)
{
	return FG_CAP_CHARGER;
}

static int sgm4154x_set_ichrg_curr(struct sgm41542 *charger, int uA)
{
	u8 reg_val;
	int ret;

	if (uA < SGM4154x_ICHRG_I_MIN_uA)
		uA = SGM4154x_ICHRG_I_MIN_uA;
	else if (uA > charger->ichg)
		uA = uA > charger->ichg;

	reg_val = uA / SGM4154x_ICHRG_I_STEP_uA;

	ret = sgm41542_update_bits(charger,
				   SGM4154x_CHRG_CTRL_2,
				   SGM4154x_ICHRG_CUR_MASK,
				   reg_val);
	if (ret)
		printf("sgm41542: set icharge current error!\n");

	return ret;
}

static int sgm4154x_set_prechrg_curr(struct sgm41542 *charger, int uA)
{
	u8 reg_val;
	int ret;

	if (uA < SGM4154x_PRECHRG_I_MIN_uA)
		uA = SGM4154x_PRECHRG_I_MIN_uA;
	else if (uA > SGM4154x_PRECHRG_I_MAX_uA)
		uA = SGM4154x_PRECHRG_I_MAX_uA;

	reg_val = (uA - SGM4154x_PRECHRG_I_MIN_uA) / SGM4154x_PRECHRG_CURRENT_STEP_uA;

	reg_val = reg_val << 4;
	ret = sgm41542_update_bits(charger,
				   SGM4154x_CHRG_CTRL_3,
				   SGM4154x_PRECHRG_CUR_MASK,
				   reg_val);
	if (ret)
		printf("sgm41542: set icharge current error!\n");

	return ret;
}

static int sgm4154x_set_chrg_volt(struct sgm41542 *charger, int chrg_volt)
{
	u8 reg_val;
	int ret;

	if (chrg_volt < SGM4154x_VREG_V_MIN_uV)
		chrg_volt = SGM4154x_VREG_V_MIN_uV;
	else if (chrg_volt > SGM4154x_VREG_V_MAX_uV)
		chrg_volt = SGM4154x_VREG_V_MAX_uV;

	reg_val = (chrg_volt - SGM4154x_VREG_V_MIN_uV) / SGM4154x_VREG_V_STEP_uV;
	reg_val = reg_val << 3;
	SGM_DBG("reg_val: 0x%x\n", reg_val);
	ret = sgm41542_update_bits(charger,
				   SGM4154x_CHRG_CTRL_4,
				   SGM4154x_VREG_V_MASK,
				   reg_val);

	return ret;
}

static int sgm41542_set_charger_voltage(struct udevice *dev, int uV)
{
	struct sgm41542 *charger = dev_get_priv(dev);

	SGM_DBG("SGM41542: charger voltage %d\n", uV);
	return sgm4154x_set_chrg_volt(charger, uV);
}

static int sgm41542_charger_enable(struct udevice *dev)
{
	struct sgm41542 *charger = dev_get_priv(dev);

	SGM_DBG("SGM41542: charger enable\n");
	sgm41542_update_bits(charger, SGM4154x_CHRG_CTRL_1,
			     SGM4154x_CHRG_EN,
			     SGM4154x_CHRG_EN);
	return 0;
}

static int sgm41542_charger_disable(struct udevice *dev)
{
	struct sgm41542 *charger = dev_get_priv(dev);

	SGM_DBG("SGM41542: charger disable\n");
	sgm41542_update_bits(charger, SGM4154x_CHRG_CTRL_1,
			     SGM4154x_CHRG_EN,
			     0);
	return 0;
}

static int sgm41542_iprechg_current(struct udevice *dev, int iprechrg_uA)
{
	struct sgm41542 *charger = dev_get_priv(dev);

	SGM_DBG("SGM41542: charger current:iprechrg_uA: %d\n",
		iprechrg_uA);

	return sgm4154x_set_prechrg_curr(charger, iprechrg_uA);
}

static int sgm41542_charger_current(struct udevice *dev, int ichrg_uA)
{
	struct sgm41542 *charger = dev_get_priv(dev);

	SGM_DBG("SGM41542: charger current:ichrg_uA%d\n",
		ichrg_uA);

	return sgm4154x_set_ichrg_curr(charger, ichrg_uA);
}

static int sgm41542_get_pd_output_val(struct sgm41542 *charger,
				      int *vol,
				      int *cur)
{
	struct power_delivery_data pd_data;
	int ret;

	if (!charger->pd)
		return -EINVAL;

	memset(&pd_data, 0, sizeof(pd_data));
	ret = power_delivery_get_data(charger->pd, &pd_data);
	if (ret)
		return ret;
	if (!pd_data.online || !pd_data.voltage || !pd_data.current)
		return -EINVAL;

	*vol = pd_data.voltage;
	*cur = pd_data.current;
	charger->pd_online = pd_data.online;

	return 0;
}

static void sgm41542_charger_input_current_init(struct sgm41542 *charger)
{
	int sdp_inputcurrent = 500 * 1000;
	int dcp_inputcurrent = 2000 * 1000;
	int pd_inputvol, pd_inputcurrent;
	int ret;

	if (!charger->pd) {
		ret = uclass_get_device(UCLASS_PD, 0, &charger->pd);
		if (ret) {
			if (ret == -ENODEV)
				printf("sgm41542: Can't find PD\n");
			else
				printf("sgm41542: Get UCLASS PD failed: %d\n", ret);
			charger->pd = NULL;
		}
	}

	if (!sgm41542_get_pd_output_val(charger, &pd_inputvol, &pd_inputcurrent)) {
		SGM_DBG("pd adapter\n");
		sgm4154x_set_input_curr_lim(charger, pd_inputcurrent);
	} else {
		SGM_DBG("normal adapter: %d\n", sgm41542_get_usb_type());
		if (sgm41542_get_usb_type() == POWER_SUPPLY_TYPE_USB_DCP)
			sgm4154x_set_input_curr_lim(charger, dcp_inputcurrent);
		else if (sgm41542_get_usb_type() == POWER_SUPPLY_TYPE_USB_CDP)
			sgm4154x_set_input_curr_lim(charger, dcp_inputcurrent);
		else if (sgm41542_get_usb_type() == POWER_SUPPLY_TYPE_USB_FLOATING)
			sgm4154x_set_input_curr_lim(charger, dcp_inputcurrent);
		else
			sgm4154x_set_input_curr_lim(charger, sdp_inputcurrent);
	}
}

static bool sgm41542_charger_status(struct udevice *dev)
{
	struct sgm41542 *charger = dev_get_priv(dev);
	int state_of_charger;
	u8 value;
	int i = 0;

__retry:
	sgm41542_read(charger, SGM4154x_CHRG_STAT, &value);
	state_of_charger = !!(value & SGM4154x_PG_STAT);
	if (!state_of_charger && charger->pd_online) {
		if (i < 3) {
			i++;
			mdelay(20);
			goto __retry;
		}
	}

	if ((state_of_charger) && (charger->init_count < 5)) {
		sgm41542_charger_input_current_init(charger);
		sgm41542_update_bits(charger,
				     SGM4154x_CHRG_CTRL_1,
				     SGM4154x_CHRG_EN,
				     SGM4154x_CHRG_EN);
		charger->init_count++;
	}

	if (!state_of_charger)
		sgm4154x_set_prechrg_curr(charger, SGM4154x_PRECHRG_I_DEF_uA);

	SGM_DBG("dump register:\n");
	for (i = SGM4154x_CHRG_CTRL_0; i < SGM4154x_CHRG_CTRL_f; i++) {
		sgm41542_read(charger, i, &value);
		SGM_DBG("[%d]: 0x%x\n", i, value);
	}
	return state_of_charger;
}

static void sgm41542_irq_handler(int irq, void *data)
{
}

static int sgm41542_ofdata_to_platdata(struct udevice *dev)
{
	struct sgm41542 *charger = dev_get_priv(dev);
	u32 interrupt, phandle;
	int ret;

	charger->dev = dev;
	charger->ichg = dev_read_u32_default(dev,
					     "vbat-current-limit-microamp",
					     0);
	if (charger->ichg == 0)
		charger->ichg = 3000 * 1000;
	charger->vchg = dev_read_u32_default(dev,
					     "vbat-voltage-limit-microamp",
					     0);
	if (charger->vchg == 0)
		charger->vchg = 4400 * 1000;
	SGM_DBG("charger->ichg: %d\n", charger->ichg);
	SGM_DBG("charger->vchg: %d\n", charger->vchg);

	phandle = dev_read_u32_default(dev, "interrupt-parent", -ENODATA);
	if (phandle == -ENODATA) {
		printf("sgm41542: read 'interrupt-parent' failed, ret=%d\n",
		       phandle);
		return phandle;
	}

	ret = dev_read_u32_array(dev, "interrupts", &interrupt, 1);
	if (ret) {
		printf("sgm41542: read 'interrupts' failed, ret=%d\n", ret);
		return ret;
	}

	charger->irq = phandle_gpio_to_irq(phandle, interrupt);
	if (charger->irq < 0)
		printf("sgm41542: failed to request irq: %d\n", charger->irq);

	return 0;
}

static int sgm41542_probe(struct udevice *dev)
{
	struct sgm41542 *charger = dev_get_priv(dev);
	u8 value;
	int i;

	SGM_DBG("sgm41542: driver version-20220903\n");
	SGM_DBG("sgm41542: dump register:\n");
	for (i = SGM4154x_CHRG_CTRL_0; i < SGM4154x_CHRG_CTRL_f; i++) {
		sgm41542_read(charger, i, &value);
		SGM_DBG("gm41542: [%d]: 0x%x\n", i, value);
	}

	charger->dev = dev;
	/* disable watchdog */
	sgm41542_update_bits(charger, SGM4154x_CHRG_CTRL_5,
			     SGM4154x_WDT_TIMER_MASK,
			     SGM4154x_WDT_TIMER_DISABLE);

	sgm41542_update_bits(charger, SGM4154x_CHRG_CTRL_a,
			     SGM4154x_VINDPM_INT_MASK | SGM4154x_IINDPM_INT_MASK,
			     SGM4154x_VINDPM_INT_DIS | SGM4154x_IINDPM_INT_DIS);

	sgm4154x_set_ichrg_curr(charger, charger->ichg);
	sgm4154x_set_chrg_volt(charger, charger->vchg);

	if (0 && charger->irq) {
		SGM_DBG("sgm41542: enable sgm42542 irq\n");
		irq_install_handler(charger->irq, sgm41542_irq_handler, dev);
		irq_handler_enable(charger->irq);
	}

	return 0;
}

static const struct udevice_id charger_ids[] = {
	{ .compatible = "sgm,sgm41542" },
	{ },
};

static struct dm_fuel_gauge_ops charger_ops = {
	.get_chrg_online = sgm41542_charger_status,
	.capability = sgm41542_charger_capability,
	.set_charger_voltage = sgm41542_set_charger_voltage,
	.set_charger_enable = sgm41542_charger_enable,
	.set_charger_disable = sgm41542_charger_disable,
	.set_charger_current = sgm41542_charger_current,
	.set_iprechg_current = sgm41542_iprechg_current,

};

U_BOOT_DRIVER(sgm41542_charger) = {
	.name = "sgm41542_charger",
	.id = UCLASS_FG,
	.probe = sgm41542_probe,
	.of_match = charger_ids,
	.ops = &charger_ops,
	.ofdata_to_platdata = sgm41542_ofdata_to_platdata,
	.priv_auto_alloc_size = sizeof(struct sgm41542),
};
