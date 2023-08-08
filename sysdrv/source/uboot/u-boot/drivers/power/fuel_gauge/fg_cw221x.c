/*
 * (C) Copyright 2022 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:    GPL-2.0+
 */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <i2c.h>
#include <power/fuel_gauge.h>
#include <power/pmic.h>
#include <linux/usb/phy-rockchip-usb2.h>

DECLARE_GLOBAL_DATA_PTR;

static int dbg_enable;

#define CW_DBG(args...) \
	do { \
		if (dbg_enable) { \
			printf(args); \
		} \
	} while (0)

#define REG_CHIP_ID			0x00
#define REG_VCELL_H			0x02
#define REG_VCELL_L			0x03
#define REG_SOC_INT			0x04
#define REG_SOC_DECIMAL			0x05
#define REG_TEMP			0x06
#define REG_MODE_CONFIG			0x08
#define REG_GPIO_CONFIG			0x0A
#define REG_SOC_ALERT			0x0B
#define REG_TEMP_MAX			0x0C
#define REG_TEMP_MIN			0x0D
#define REG_CURRENT_H			0x0E
#define REG_CURRENT_L			0x0F
#define REG_T_HOST_H			0xA0
#define REG_T_HOST_L			0xA1
#define REG_USER_CONF			0xA2
#define REG_CYCLE_H			0xA4
#define REG_CYCLE_L			0xA5
#define REG_SOH				0xA6
#define REG_IC_STATE			0xA7
#define REG_FW_VERSION			0xAB
#define REG_BAT_PROFILE			0x10

#define CONFIG_MODE_RESTART		0x30
#define CONFIG_MODE_ACTIVE		0x00
#define CONFIG_MODE_SLEEP		0xF0
#define CONFIG_UPDATE_FLG		0x80
#define IC_VCHIP_ID			0xA0
#define IC_READY_MARK			0x0C

#define GPIO_ENABLE_MIN_TEMP		0
#define GPIO_ENABLE_MAX_TEMP		0
#define GPIO_ENABLE_SOC_CHANGE		0
#define GPIO_SOC_IRQ_VALUE		0x0 /* 0x7F */
#define DEFINED_MAX_TEMP		45
#define DEFINED_MIN_TEMP		0

#define CWFG_NAME			"cw221X"
#define SIZE_OF_PROFILE			80

/* mhom rsense * 1000 for convenience calculation */
#define USER_RSENSE			1500

#define queue_delayed_work_time		5000
#define queue_start_work_time		50

#define CW_SLEEP_20MS			20
#define CW_SLEEP_10MS			10
#define CW_UI_FULL			100
#define COMPLEMENT_CODE_U16		0x8000
#define CW_SLEEP_100MS			100
#define CW_SLEEP_200MS			200
#define CW_SLEEP_COUNTS			50
#define CW_TRUE				1
#define CW_RETRY_COUNT			3
#define CW_VOL_UNIT			1000
#define CW_LOW_VOLTAGE_REF		2500
#define CW_LOW_VOLTAGE			3000
#define CW_LOW_VOLTAGE_STEP		10

#define CW221X_NOT_ACTIVE		1
#define CW221X_PROFILE_NOT_READY	2
#define CW221X_PROFILE_NEED_UPDATE	3

static u8 config_profile_info[SIZE_OF_PROFILE] = {
	0x5A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA0, 0xB2,
	0xC2, 0xCA, 0xC2, 0xBD, 0x9C, 0x5C, 0x38, 0xFF, 0xFF, 0xC4,
	0x86, 0x74, 0x60, 0x55, 0x4F, 0x4D, 0x4B, 0x80, 0xC0, 0xDB,
	0xCD, 0xD0, 0xCE, 0xD2, 0xD3, 0xD2, 0xD0, 0xCE, 0xC3, 0xD5,
	0xB9, 0xC9, 0xC5, 0xA3, 0x92, 0x8A, 0x80, 0x72, 0x63, 0x62,
	0x74, 0x90, 0xA6, 0x7E, 0x5F, 0x48, 0x80, 0x00, 0xAB, 0x10,
	0x00, 0xA1, 0xFB, 0x00, 0x00, 0x00, 0x64, 0x1E, 0xB1, 0x04,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4D,
};

enum charger_type {
	CHARGER_TYPE_NO = 0,
	CHARGER_TYPE_USB,
	CHARGER_TYPE_AC,
	CHARGER_TYPE_DC,
	CHARGER_TYPE_UNDEF,
};

struct cw221x_info {
	struct udevice *dev;
	int capacity;
	u8 *bat_profile;
	int chip_id;
	int voltage;
	int ic_soc_h;
	int ic_soc_l;
	int ui_soc;
	int temp;
	long cw_current;
	int cycle;
	int soh;
};

static u8 cw221x_read(struct cw221x_info *cw221x, u8 reg, u8 *buffer)
{
	u8 val;
	int ret;

	ret = dm_i2c_read(cw221x->dev, reg, &val, 1);
	if (ret) {
		printf("cw221x: read reg 0x%02x failed, ret=%d\n", reg, ret);
		return ret;
	}

	*buffer = val;
	return 0;
}

static int cw221x_write(struct cw221x_info *cw221x, u8 reg, u8 val)
{
	int ret;

	ret = dm_i2c_write(cw221x->dev, reg, &val, 1);
	if (ret)
		printf("cw221x: write reg 0x%02x failed, ret=%d\n", reg, ret);

	return ret;
}

static int cw221x_read_half_word(struct cw221x_info *cw221x,
				 int reg,
				 u16 *buffer)
{
	u8 vall, valh;
	int ret;

	ret = cw221x_read(cw221x, reg, &valh);
	if (ret)
		return ret;

	ret = cw221x_read(cw221x, reg + 1, &vall);
	if (ret)
		return ret;

	*buffer = ((u16)valh << 8) | vall;

	return 0;
}

static int cw221x_ofdata_to_platdata(struct udevice *dev)
{
	struct cw221x_info *cw221x = dev_get_priv(dev);
	int i, len, size;
	const u8 *info;

	if (!dev_read_prop(dev, "cellwise,battery-profile", &len) ||
	    (len != SIZE_OF_PROFILE)) {
		cw221x->bat_profile = config_profile_info;
		return 0;
	}

	CW_DBG("cw221x: use dts profile\n");
	size = sizeof(*cw221x->bat_profile) * len;
	cw221x->bat_profile = calloc(size, 1);
	if (!cw221x->bat_profile) {
		CW_DBG("cw221x: calloc bat_profile fail\n");
		return -EINVAL;
	}

	info = dev_read_u8_array_ptr(dev, "cellwise,battery-profile", len);
	if (!info) {
		CW_DBG("cw221x: fdtdec_get battery profile fail\n");
		return -EINVAL;
	}
	for (i = 0; i < len; i++) {
		cw221x->bat_profile[i] = info[i];
		CW_DBG("%#x ", cw221x->bat_profile[i]);
		if ((i + 1) % 8 == 0)
			CW_DBG("\n");
	}

	return 0;
}

static int cw221x_get_vol(struct cw221x_info *cw221x)
{
	u16 value16, value16_1, value16_2;
	int voltage;

	cw221x_read_half_word(cw221x, REG_VCELL_H, &value16);
	mdelay(10);

	cw221x_read_half_word(cw221x, REG_VCELL_H, &value16_1);
	mdelay(10);

	if (value16 != value16_1) {
		cw221x_read_half_word(cw221x, REG_VCELL_H, &value16_2);
		if (value16_2 < 0)
			return -1;
		voltage = value16_2 * 5 / 16;

		return voltage;
	}

	voltage = value16 * 5 / 16;
	CW_DBG("cw221x: voltage:%d\n", voltage);

	return voltage;
}

static int cw221x_dwc_otg_check_dpdm(void)
{
#if defined(CONFIG_PHY_ROCKCHIP_INNO_USB2) && !defined(CONFIG_SPL_BUILD)
	return rockchip_chg_get_type();
#else
	CW_DBG("cw221x: rockchip_chg_get_type() is not implement\n");
	return CHARGER_TYPE_NO;
#endif
}

static int cw221x_get_usb_state(struct cw221x_info *cw221x)
{
	int charger_type;

	switch (cw221x_dwc_otg_check_dpdm()) {
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

static bool cw221x_check_charge(struct cw221x_info *cw221x)
{
	if (cw221x_get_usb_state(cw221x) != CHARGER_TYPE_NO)
		return true;

	return false;
}

static int cw221x_get_soc(struct cw221x_info *cw221x)
{
	int ui_100 = CW_UI_FULL;
	int ui_soc;
	u16 value16, value16_1, value16_2;
	int ret;

	ret = cw221x_read_half_word(cw221x, REG_SOC_INT, &value16);
	if (ret) {
		printf("cw221x: get soc error!!!");
		return ret;
	}
	mdelay(10);

	ret = cw221x_read_half_word(cw221x, REG_SOC_INT, &value16_1);
	if (ret) {
		printf("cw221x: get soc error!!!");
		return ret;
	}
	mdelay(10);

	if (value16 != value16_1) {
		ret = cw221x_read_half_word(cw221x, REG_SOC_INT, &value16_2);
		if (ret) {
			printf("cw221x: get soc error!!!");
			return ret;
		}

		if (value16_2 < 0)
			return -1;

		value16 = value16_2;
	}

	ui_soc = value16 * 100 / (ui_100 * 256);

	if (ui_soc >= 100)
		ui_soc = 100;
	CW_DBG("soc: %d\n", ui_soc);

	return ui_soc;
}

static int cw221x_update_get_soc(struct udevice *dev)
{
	struct cw221x_info *cw221x = dev_get_priv(dev);

	return cw221x_get_soc(cw221x);
}

/*
 * The 0x00 register is an UNSIGNED 8bit read-only register. Its value is
 * fixed to 0xA0 in shutdown mode and active mode.
 */
static void cw221X_get_chip_id(struct cw221x_info *cw221x)
{
	u8 chip_id;
	int ret;

	ret = cw221x_read(cw221x, REG_CHIP_ID, &chip_id);
	if (ret) {
		printf("cw221x: get chip id error!!!");
		return;
	}

	cw221x->chip_id = chip_id;
}

static int cw221X_active(struct cw221x_info *cw221x)
{
	unsigned char reg_val = CONFIG_MODE_RESTART;

	cw221x_write(cw221x, REG_MODE_CONFIG, reg_val);
	mdelay(20);
	reg_val = CONFIG_MODE_ACTIVE;
	cw221x_write(cw221x, REG_MODE_CONFIG, reg_val);
	mdelay(20);

	return 0;
}

static int cw221X_sleep(struct cw221x_info *cw221x)
{
	unsigned char reg_val = CONFIG_MODE_RESTART;

	cw221x_write(cw221x, REG_MODE_CONFIG, reg_val);
	mdelay(20);
	reg_val = CONFIG_MODE_SLEEP;
	cw221x_write(cw221x, REG_MODE_CONFIG, reg_val);
	mdelay(20);

	return 0;
}

static int cw_write_profile(struct cw221x_info *cw221x, u8 buf[])
{
	int ret;
	int i;

	for (i = 0; i < SIZE_OF_PROFILE; i++) {
		ret = cw221x_write(cw221x, REG_BAT_PROFILE + i, buf[i]);
		if (ret < 0) {
			printf("cw221x: write profile error\n");
			return ret;
		}
	}

	return ret;
}

/*
 * Get the cw221X running state
 * Determine whether the profile needs to be updated
 */
static int cw221X_get_state(struct cw221x_info *cw221x)
{
	unsigned char reg_val;
	int ret;
	int i;

	ret = cw221x_read(cw221x, REG_MODE_CONFIG, &reg_val);
	if (ret) {
		printf("cw221x: get mode config error!!!");
		return ret;
	}

	if (reg_val != CONFIG_MODE_ACTIVE)
		return CW221X_NOT_ACTIVE;

	ret = cw221x_read(cw221x, REG_SOC_ALERT, &reg_val);
	if (ret) {
		printf("cw221x: get soc alert error!!!");
		return ret;
	}

	if (0x00 == (reg_val & CONFIG_UPDATE_FLG))
		return CW221X_PROFILE_NOT_READY;

	for (i = 0; i < SIZE_OF_PROFILE; i++) {
		ret = cw221x_read(cw221x, (REG_BAT_PROFILE + i), &reg_val);
		if (ret) {
			printf("cw221x: get battery profile error!!!");
			return ret;
		}

		if (cw221x->bat_profile[i] != reg_val)
			break;
	}

	if (i != SIZE_OF_PROFILE)
		return CW221X_PROFILE_NEED_UPDATE;

	return 0;
}

static int cw_config_start_ic(struct cw221x_info *cw221x)
{
	int count = 0;
	u8 reg_val;
	int ret;

	cw221X_sleep(cw221x);
	cw_write_profile(cw221x, cw221x->bat_profile);
	cw221X_active(cw221x);

	/* set UPDATE_FLAG AND SOC INTTERRUP VALUE */
	reg_val = CONFIG_UPDATE_FLG | GPIO_SOC_IRQ_VALUE;
	ret = cw221x_write(cw221x, REG_SOC_ALERT, reg_val);
	if (ret < 0)
		return ret;

	/* close all interruptes */
	reg_val = 0;
	ret = cw221x_write(cw221x, REG_GPIO_CONFIG, reg_val);
	if (ret < 0)
		return ret;

	while (CW_TRUE) {
		mdelay(CW_SLEEP_100MS);
		ret = cw221x_read(cw221x, REG_IC_STATE, &reg_val);
		if (ret) {
			printf("cw221x: get ic state error!!!");
			return ret;
		}

		if (IC_READY_MARK == (reg_val & IC_READY_MARK))
			break;
		count++;
		if (count >= CW_SLEEP_COUNTS) {
			cw221X_sleep(cw221x);
			return -1;
		}
	}
	return 0;
}

static int cw221X_init(struct cw221x_info *cw221x)
{
	int ret;

	cw221X_get_chip_id(cw221x);
	if (cw221x->chip_id != IC_VCHIP_ID) {
		printf("not cw221X\n");
		return -EINVAL;
	}

	CW_DBG("cw221X:\n");
	ret = cw221X_get_state(cw221x);

	if (ret != 0) {
		CW_DBG("cw221x: need update profile; %d\n", ret);
		ret = cw_config_start_ic(cw221x);
		if (ret < 0)
			return ret;
	} else {
		CW_DBG("cw221x: not need update profile\n");
	}

	CW_DBG("cw221X: init success!\n");

	return 0;
}

/* get complement code function, unsigned short must be U16 */
static long get_complement_code(unsigned short raw_code)
{
	long complement_code;
	int dir;

	if (0 != (raw_code & COMPLEMENT_CODE_U16)) {
		dir = -1;
		raw_code = (0xFFFF - raw_code) + 1;
	} else {
		dir = 1;
	}

	complement_code = (long)raw_code * dir;

	return complement_code;
}

static int cw221x_get_current(struct cw221x_info *cw221x)
{
	long long cw_current;
	u16 value16;
	int ret;

	ret = cw221x_read_half_word(cw221x, REG_CURRENT_H, &value16);
	if (ret) {
		printf("cw221x: get current error!!!");
		return ret;
	}

	cw_current = get_complement_code(value16);
	cw_current = cw_current * 1600 / USER_RSENSE;

	CW_DBG("cw221x: current: %lld\n", cw_current);

	return cw_current;
}

static int cw221x_update_get_current(struct udevice *dev)
{
	struct cw221x_info *cw221x = dev_get_priv(dev);

	return cw221x_get_current(cw221x);
}

static int cw221x_get_temperature(struct udevice *dev, int *temp)
{
	struct cw221x_info *cw221x = dev_get_priv(dev);
	u8 reg_val;
	int bat_temp;
	int ret;

	ret = cw221x_read(cw221x, REG_TEMP, &reg_val);
	if (ret)
		return ret;

	bat_temp = (int)reg_val * 10 / 2 - 400;
	CW_DBG("cw221x: temperature: %d\n", bat_temp);
	*temp = bat_temp;

	return 0;
}

static bool cw221x_update_get_chrg_online(struct udevice *dev)
{
	struct cw221x_info *cw221x = dev_get_priv(dev);

	return cw221x_check_charge(cw221x);
}

static int cw221x_update_get_voltage(struct udevice *dev)
{
	struct cw221x_info *cw221x = dev_get_priv(dev);
	int temperture;

	cw221x_get_current(cw221x);
	cw221x_get_temperature(dev, &temperture);

	return cw221x_get_vol(cw221x);
}

static int cw221x_capability(struct udevice *dev)
{
	return FG_CAP_FUEL_GAUGE;
}

static struct dm_fuel_gauge_ops cw221x_fg_ops = {
	.capability = cw221x_capability,
	.get_soc = cw221x_update_get_soc,
	.get_voltage = cw221x_update_get_voltage,
	.get_current = cw221x_update_get_current,
	.get_temperature = cw221x_get_temperature,
	.get_chrg_online = cw221x_update_get_chrg_online,
};

static int cw221x_fg_probe(struct udevice *dev)
{
	struct cw221x_info *cw221x = dev_get_priv(dev);

	cw221x->dev = dev;
	printf("cw221x driver version-20220903");
	cw221X_init(cw221x);
	printf("cw221x vol: %d, soc: %d\n",
	       cw221x_get_vol(cw221x), cw221x_get_soc(cw221x));

	return 0;
}

static const struct udevice_id cw221x_ids[] = {
	{ .compatible = "cellwise,cw221X" },
	{ }
};

U_BOOT_DRIVER(cw221x_fg) = {
	.name = "cw221x_fg",
	.id = UCLASS_FG,
	.of_match = cw221x_ids,
	.probe = cw221x_fg_probe,
	.ofdata_to_platdata = cw221x_ofdata_to_platdata,
	.ops = &cw221x_fg_ops,
	.priv_auto_alloc_size = sizeof(struct cw221x_info),
};
