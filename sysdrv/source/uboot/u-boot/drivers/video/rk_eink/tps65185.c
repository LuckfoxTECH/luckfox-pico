/*
 * (C) Copyright 2020 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <dm.h>
#include <i2c.h>
#include <asm/gpio.h>
#include "rk_ebc.h"

#define msleep(a)		udelay((a) * 1000)

struct tps65185_priv_data {
	struct udevice *dev;
	struct gpio_desc pwr_up_gpio;
	struct gpio_desc pwr_en_gpio;
	struct gpio_desc vcom_gpio;
	struct gpio_desc wake_up_gpio;
	u8 rev_id;
	u8 vadj;
	u8 vcom1;
	u8 vcom2;
	u8 upseq0;
	u8 upseq1;
	u8 dwnseq0;
	u8 dwnseq1;
	u8 shadow_en;
};

#define REG_TMST_VALUE		0x00
#define REG_ENABLE		0x01
#define REG_VADJ		0x02
#define REG_VCOM1_ADJUST	0x03
#define REG_VCOM2_ADJUST	0x04
#define REG_INT_ENABLE1		0x05
#define REG_INT_ENABLE2		0x06
#define REG_INT_STATUS1		0x07
#define REG_INT_STATUS2		0x08
#define REG_UPSEQ0		0x09
#define REG_UPSEQ1		0x0a
#define REG_DWNSEQ0		0x0b
#define REG_DWNSEQ1		0x0c
#define REG_TMST1		0x0d
#define REG_TMST2		0x0e
#define REG_PG_STATUS		0x0f
#define REG_REVID		0x10
#define mv_to_vcom1_reg(mv)	(((mv) / 10) & 0xff)
#define mv_to_vcom2_reg(mv)	((((mv) / 10) & 0x100) >> 8)

/*
 * After waking up from sleep, Papyrus
 * waits for VN to be discharged and all
 * voltage ref to startup before loading
 * the default EEPROM settings. So accessing
 * registers too early after WAKEUP could
 * cause the register to be overridden by
 * default values
 */
#define PAPYRUS_EEPROM_DELAY_MS 50
/*
 * Papyrus WAKEUP pin must stay low for
 * a minimum time
 */
#define PAPYRUS_SLEEP_MINIMUM_MS 110
/*
 * Temp sensor might take a little time to
 * settle even though the status bit in TMST1
 * state conversion is done - if read too early
 * 0C will be returned instead of the right temp
 */
#define PAPYRUS_TEMP_READ_TIME_MS 10

/*
 * Powerup sequence takes at least 24 ms
 * - no need to poll too frequently
 */
#define HW_GET_STATE_INTERVAL_MS 24

#define SEQ_VDD(index)	(((index) % 4) << 6)
#define SEQ_VPOS(index)	(((index) % 4) << 4)
#define SEQ_VEE(index)	(((index) % 4) << 2)
#define SEQ_VNEG(index)	(((index) % 4) << 0)

/* power up seq delay time */
#define UDLY_3ms(index)		(0x00 << (((index) % 4) * 2))
#define UDLY_6ms(index)		(0x01 << (((index) % 4) * 2))
#define UDLY_9ms(index)		(0x10 << (((index) % 4) * 2))
#define UDLY_12ms(index)	(0x11 << (((index) % 4) * 2))

/* power down seq delay time */
#define DDLY_6ms(index)		(0x00 << (((index) % 4) * 2))
#define DDLY_12ms(index)	(0x01 << (((index) % 4) * 2))
#define DDLY_24ms(index)	(0x10 << (((index) % 4) * 2))
#define DDLY_48ms(index)	(0x11 << (((index) % 4) * 2))

#define NUMBER_PMIC_REGS		10
// INT_ENABLE1
#define PAPYRUS_INT_ENABLE1_ACQC_EN	1
#define PAPYRUS_INT_ENABLE1_PRGC_EN	0

// INT_STATUS1
#define PAPYRUS_INT_STATUS1_ACQC	1
#define PAPYRUS_INT_STATUS1_PRGC	0

// VCOM2_ADJUST
#define PAPYRUS_VCOM2_ACQ		7
#define PAPYRUS_VCOM2_PROG		6
#define PAPYRUS_VCOM2_HIZ		5
#define V3P3_EN_MASK			0x20

#define PAPYRUS_V3P3OFF_DELAY_MS	20//100

static struct udevice *pmic_dev;

int tps65185_i2c_write(struct tps65185_priv_data *priv_data, u8 reg, u8 val)
{
	int ret;
	u8 buf[2];
	struct i2c_msg msg;
	struct dm_i2c_chip *chip = dev_get_parent_platdata(priv_data->dev);

	buf[0] = reg;
	buf[1] = val;
	msg.addr = chip->chip_addr;
	msg.flags = 0;
	msg.len = 2;
	msg.buf = buf;

	ret = dm_i2c_xfer(priv_data->dev, &msg, 1);
	if (ret) {
		printf("tps65185 i2c write failed: %d\n", ret);
		return ret;
	}

	return 0;
}

int tps65185_i2c_read(struct tps65185_priv_data *priv_data, u8 reg, u8 *val)
{
	int ret;
	u8 data;
	struct dm_i2c_chip *chip = dev_get_parent_platdata(priv_data->dev);
	struct i2c_msg msg[] = {
		{
			.addr = chip->chip_addr,
			.flags = 0,
			.buf = (u8 *)&reg,
			.len = 1,
		}, {
			.addr = chip->chip_addr,
			.flags = I2C_M_RD,
			.buf = (u8 *)&data,
			.len = 1,
		}
	};

	ret = dm_i2c_xfer(priv_data->dev, msg, 2);
	if (ret) {
		printf("tps65185 i2c read failed: %d\n", ret);
		return ret;
	}

	*val = data;

	return 0;
}

void tps65185_dump_registers(void)
{
	u8 i, reg = 0;
	struct tps65185_priv_data *priv_data = dev_get_priv(pmic_dev);

	for (i = 0; i <= REG_REVID; i++) {
		tps65185_i2c_read(priv_data, i, &reg);
		printf("0x%x\t", reg);
	}
	printf("\n");
}

static int tps65185_read_vcom_value(struct tps65185_priv_data *priv_data,
				    u32 *vcom_read)
{
	int ret;
	u8 vcom_reg;

	dm_gpio_set_value(&priv_data->wake_up_gpio, 0);
	msleep(10);
	dm_gpio_set_value(&priv_data->wake_up_gpio, 1);
	msleep(10);
	ret = tps65185_i2c_read(priv_data, REG_VCOM1_ADJUST, &vcom_reg);
	if (ret) {
		printf("read vcom failed: %d\n", ret);
		return ret;
	}
	*vcom_read = vcom_reg;
	ret = tps65185_i2c_read(priv_data, REG_VCOM2_ADJUST, &vcom_reg);
	if (ret) {
		printf("read vcom failed: %d\n", ret);
		return ret;
	}
	*vcom_read += (vcom_reg & 0x1) << 8;

	printf("read vcom value: %d\n", *vcom_read);

	return 0;
}

static int tps65185_set_vcom_value(struct udevice *dev, u32 set_value)
{
	int ret = 0;
	u32 vcom_readback = 0;
	u8 vcom1_val, vcom2_val, int_stat = 0;
	struct tps65185_priv_data *priv_data = dev_get_priv(dev);

	ret = tps65185_read_vcom_value(priv_data, &vcom_readback);
	if (ret < 0) {
		printf("tps65185 read vcom value failed\n");
	} else {
		if (vcom_readback == set_value / 10) {
			printf("Same as pmic default value, just return.\n");
			return 0;
		}
	}

	vcom1_val = mv_to_vcom1_reg(set_value);
	vcom2_val = mv_to_vcom2_reg(set_value);

	dm_gpio_set_value(&priv_data->wake_up_gpio, 1);
	msleep(20);
	// Set vcom voltage
	tps65185_i2c_write(priv_data, REG_VCOM1_ADJUST, vcom1_val);
	tps65185_i2c_write(priv_data, REG_VCOM2_ADJUST, vcom2_val);
	// PROGRAMMING
	tps65185_i2c_write(priv_data, REG_VCOM2_ADJUST,
			   vcom2_val | (1 << PAPYRUS_VCOM2_PROG));
	do {
		msleep(20);
		ret = tps65185_i2c_read(priv_data, REG_INT_STATUS1, &int_stat);
		if (ret) {
			printf("read status1 failed: %d\n", ret);
			break;
		}
	} while (!(int_stat & (1 << PAPYRUS_INT_STATUS1_PRGC)));

	// VERIFICATION
	tps65185_read_vcom_value(priv_data, &vcom_readback);

	if (vcom_readback != set_value / 10) {
		printf("vcom set failed, expect: %d, readback: %d\n",
		       set_value, vcom_readback);
		return -1;
	}

	return 0;
}

static bool tps65185_hw_power_ack(struct tps65185_priv_data *priv_data, int up)
{
	u8 pg_status;
	int st, ret, retries_left = 10;

	do {
		ret = tps65185_i2c_read(priv_data, REG_PG_STATUS, &pg_status);
		if (ret)
			printf("read REG_PG_STATUS failed: %d\n", ret);

		pg_status &= 0xfa;
		if (pg_status == 0xfa && up == 1) {
			st = 1;
		} else if (pg_status == 0x00 && up == 0) {
			st = 0;
		} else {
			st = -1;	/* not settled yet */
			msleep(HW_GET_STATE_INTERVAL_MS);
		}
		retries_left--;
	} while ((st == -1) && retries_left);

	if ((st == -1) && !retries_left)
		printf("power %s settle error (PG = %02x)\n",
		       up ? "up" : "down", pg_status);

	return (st == up);
}

static int tps65185_power_on(struct udevice *dev)
{
	struct tps65185_priv_data *priv_data = dev_get_priv(dev);

	tps65185_i2c_write(priv_data, REG_VADJ, priv_data->vadj);
	tps65185_i2c_write(priv_data, REG_UPSEQ0, priv_data->upseq0);
	tps65185_i2c_write(priv_data, REG_UPSEQ1, priv_data->upseq1);
	tps65185_i2c_write(priv_data, REG_DWNSEQ0, priv_data->dwnseq0);
	tps65185_i2c_write(priv_data, REG_DWNSEQ1, priv_data->dwnseq1);

	priv_data->shadow_en |= V3P3_EN_MASK;
	tps65185_i2c_write(priv_data, REG_ENABLE, priv_data->shadow_en);
	msleep(PAPYRUS_V3P3OFF_DELAY_MS);
	priv_data->shadow_en = (0x80 | 0x30 | 0x0F);
	tps65185_i2c_write(priv_data, REG_ENABLE, priv_data->shadow_en);

	tps65185_hw_power_ack(priv_data, 1);
	return 0;
}

static int tps65185_power_down(struct udevice *dev)
{
	struct tps65185_priv_data *priv_data = dev_get_priv(dev);

	priv_data->shadow_en = (0x40 | 0x20 | 0x0F);
	tps65185_i2c_write(priv_data, REG_ENABLE, priv_data->shadow_en);
	msleep(PAPYRUS_V3P3OFF_DELAY_MS);
	priv_data->shadow_en &= ~V3P3_EN_MASK;
	tps65185_i2c_write(priv_data, REG_ENABLE, priv_data->shadow_en);

	tps65185_hw_power_ack(priv_data, 0);

	return 0;
}

static int tps65185_temp_get(struct udevice *dev, u32 *temp)
{
	int ret;
	u8 read_val = 0;
	struct tps65185_priv_data *priv_data = dev_get_priv(dev);

	tps65185_i2c_write(priv_data, REG_TMST1, 0x80);
	tps65185_i2c_write(priv_data, REG_TMST1, 0x80);
	do {
		int retry_time = 100;

		ret = tps65185_i2c_read(priv_data, REG_TMST1, &read_val);
		if (ret < 0) {
			printf("read REG_TMST1 failed: %d\n", ret);
			return ret;
		}
		if (retry_time-- == 0) {
			printf("read REG_TMST1 retry 100 times\n");
			break;
		}
		debug("read_val = 0x%x\n", read_val);
	} while (((read_val & 0x20) == 0 || (read_val & 0x80)));

	mdelay(PAPYRUS_TEMP_READ_TIME_MS);
	ret = tps65185_i2c_read(priv_data, REG_TMST_VALUE, &read_val);
	if (ret) {
		printf("read REG_TMST_VALUE failed: %d\n", ret);
		return ret;
	}
	*temp = (u32)read_val;

	return 0;
}

static int tps65185_hw_init(struct udevice *dev)
{
	int ret;
	u8 rev_id = 0;
	struct tps65185_priv_data *priv_data = dev_get_priv(dev);

	dm_gpio_set_value(&priv_data->wake_up_gpio, 0);
	mdelay(PAPYRUS_SLEEP_MINIMUM_MS);
	dm_gpio_set_value(&priv_data->wake_up_gpio, 1);
	dm_gpio_set_value(&priv_data->pwr_up_gpio, 0);
	dm_gpio_set_value(&priv_data->vcom_gpio, 1);
	mdelay(PAPYRUS_EEPROM_DELAY_MS);
	ret = tps65185_i2c_read(priv_data, REG_REVID, &rev_id);
	if (ret) {
		printf("read revid failed: %d\n", ret);
		return ret;
	}

	if (rev_id > 0)
		printf("detected device with ID=%02x (TPS6518%dr%dp%d)\n",
		       rev_id, rev_id & 0xF, (rev_id & 0xC0) >> 6,
		       (rev_id & 0x30) >> 4);

	tps65185_i2c_write(priv_data, REG_ENABLE, priv_data->shadow_en);
	priv_data->rev_id = rev_id;
	printf("rev_id=%x\n", rev_id);
	return 0;
}

static void tps65185_init_arg(struct tps65185_priv_data *priv_data)
{
	priv_data->vadj = 0x03;

	priv_data->upseq0 = SEQ_VEE(0) | SEQ_VNEG(1)
				| SEQ_VPOS(2) | SEQ_VDD(3);
	priv_data->upseq1 = UDLY_3ms(0) | UDLY_3ms(1)
				| UDLY_3ms(2) | UDLY_3ms(3);

	priv_data->dwnseq0 = SEQ_VDD(0) | SEQ_VPOS(1)
				| SEQ_VNEG(2) | SEQ_VEE(3);
	priv_data->dwnseq1 = DDLY_6ms(0) | DDLY_6ms(1)
				| DDLY_6ms(2) | DDLY_6ms(3);

	priv_data->vcom1 = mv_to_vcom1_reg(1560);
	priv_data->vcom2 = mv_to_vcom2_reg(1560);
	priv_data->shadow_en = 0;
}

static int tps65185_probe(struct udevice *dev)
{
	int ret;
	struct tps65185_priv_data *tps65185_priv = dev_get_priv(dev);

	tps65185_priv->dev = dev;
	pmic_dev = dev;
	tps65185_init_arg(tps65185_priv);

	ret = gpio_request_by_name(dev, "wakeup-gpios", 0,
				   &tps65185_priv->wake_up_gpio, GPIOD_IS_OUT);
	if (ret) {
		printf("Cannot get wakeup_pin GPIO: %d\n", ret);
		return ret;
	}
	ret = gpio_request_by_name(dev, "powerup-gpios", 0,
				   &tps65185_priv->pwr_up_gpio, GPIOD_IS_OUT);
	if (ret) {
		printf("Cannot get pwr_up_gpio GPIO: %d\n", ret);
		return ret;
	}
	ret = gpio_request_by_name(dev, "vcomctl-gpios", 0,
				   &tps65185_priv->vcom_gpio, GPIOD_IS_OUT);
	if (ret) {
		printf("Cannot get vcom_gpio GPIO: %d\n", ret);
		return ret;
	}
	ret = gpio_request_by_name(dev, "poweren-gpios", 0,
				   &tps65185_priv->pwr_en_gpio, GPIOD_IS_OUT);
	if (!ret)
		dm_gpio_set_value(&tps65185_priv->pwr_en_gpio, 1);
	else
		printf("Cannot get pwren_pin GPIO: %d\n", ret);

	ret = tps65185_hw_init(dev);
	if (ret) {
		printf("Cannot init hardware for tps65185: %d\n", ret);
		return ret;
	}

	return 0;
}

const struct rk_ebc_pwr_ops tps65185_funcs = {
	.power_on = tps65185_power_on,
	.power_down = tps65185_power_down,
	.temp_get = tps65185_temp_get,
	.vcom_set = tps65185_set_vcom_value,
};

static const struct udevice_id ebc_power_of_match[] = {
	{ .compatible = "ti,tps65185" },
	{}
};

U_BOOT_DRIVER(tps65185_ebc_pwr) = {
	.name = "tps65185_ebc_pwr",
	.id = UCLASS_I2C_GENERIC,
	.of_match = ebc_power_of_match,
	.probe = tps65185_probe,
	.ops = &tps65185_funcs,
	.bind = dm_scan_fdt_dev,
	.priv_auto_alloc_size = sizeof(struct tps65185_priv_data),
};

