// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * serdes-i2c.c  --  display i2c for different serdes chips
 *
 * Copyright (c) 2023 Rockchip Electronics Co. Ltd.
 *
 * Author: luowei <lw@rock-chips.com>
 */

#include "core.h"

static int dm_i2c_reg_write_u8(struct udevice *dev, u8 reg, u8 val)
{
	int ret;
	u8 buf[2];
	struct i2c_msg msg;
	struct dm_i2c_chip *chip = dev_get_parent_platdata(dev);

	buf[0] = reg;
	buf[1] = val;
	msg.addr = chip->chip_addr;
	msg.flags = 0;
	msg.len = 2;
	msg.buf = buf;

	ret = dm_i2c_xfer(dev, &msg, 1);
	if (ret) {
		printf("dm i2c write failed: %d\n", ret);
		return ret;
	}

	return 0;
}

static uint8_t dm_i2c_reg_read_u8(struct udevice *dev, u8 reg)
{
	int ret;
	u8 data;
	struct dm_i2c_chip *chip = dev_get_parent_platdata(dev);
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

	ret = dm_i2c_xfer(dev, msg, 2);
	if (ret) {
		printf("dm i2c read failed: %d\n", ret);
		return ret;
	}

	return data;
}

static int dm_i2c_reg_clrset_u8(struct udevice *dev,
				u8 offset,
				u8 clr, u8 set)
{
	u8 val;

	val = dm_i2c_reg_read_u8(dev, offset);
	if (val < 0)
		return val;

	val &= ~clr;
	val |= set;

	return dm_i2c_reg_write_u8(dev, offset, val);
}

/**
 * serdes_reg_read: Read a single serdes register.
 *
 * @serdes: Device to read from.
 * @reg: Register to read.
 * @val: Date read from register.
 */
int serdes_reg_read(struct serdes *serdes,
		    unsigned int reg, unsigned int *val)
{
	unsigned int value;

	if (serdes->chip_data->reg_val_type == REG_8BIT_VAL_8IT)
		value = dm_i2c_reg_read_u8(serdes->dev, reg);
	else
		value = dm_i2c_reg_read(serdes->dev, reg);

	*val = value;
	SERDES_DBG_I2C("%s %s %s Read Reg%04x %04x\n",
		       __func__, serdes->dev->name,
		       serdes->dev->name, reg, *val);
	return 0;
}
EXPORT_SYMBOL_GPL(serdes_reg_read);

/**
 * serdes_reg_write: Write a single serdes register.
 *
 * @serdes: Device to write to.
 * @reg: Register to write to.
 * @val: Value to write.
 */
int serdes_reg_write(struct serdes *serdes, unsigned int reg,
		     unsigned int val)
{
	int ret = 0;

	SERDES_DBG_I2C("%s %s Write Reg%04x %04x) type=%d\n",
		       __func__, serdes->dev->name,
		       reg, val, serdes->chip_data->reg_val_type);
	if (serdes->chip_data->reg_val_type == REG_8BIT_VAL_8IT) {
		ret = dm_i2c_reg_write_u8(serdes->dev, reg, val);
		if (ret != 0)
			return ret;
	} else {
		ret = dm_i2c_reg_write(serdes->dev, reg, val);
		if (ret != 0)
			return ret;
	}

	return ret;
}
EXPORT_SYMBOL_GPL(serdes_reg_write);

/**
 * serdes_multi_reg_write: Write many serdes register.
 *
 * @serdes: Device to write to.
 * @regs: Registers to write to.
 * @num_regs: Number of registers to write.
 */
int serdes_multi_reg_write(struct serdes *serdes,
			   const struct reg_sequence *regs,
			   int num_regs)
{
	int i, ret = 0;

	SERDES_DBG_I2C("%s %s %s num=%d\n", __func__, serdes->dev->name,
		       serdes->chip_data->name, num_regs);

	for (i = 0; i < num_regs; i++) {
		ret = serdes_reg_write(serdes, regs[i].reg, regs[i].def);
		SERDES_DBG_I2C("serdes %s Write Reg%04x %04x ret=%d\n",
			       serdes->chip_data->name,
			       regs[i].reg, regs[i].def, ret);
	}

	return ret;
}
EXPORT_SYMBOL_GPL(serdes_multi_reg_write);

/**
 * serdes_set_bits: Set the value of a bitfield in a serdes register
 *
 * @serdes: Device to write to.
 * @reg: Register to write to.
 * @mask: Mask of bits to set.
 * @val: Value to set (unshifted)
 */
int serdes_set_bits(struct serdes *serdes, unsigned int reg,
		    unsigned int mask, unsigned int val)
{
	int ret = 0;

	SERDES_DBG_I2C("%s %s %s Write Reg%04x %04x) mask=%04x\n",
		       __func__,
		       serdes->dev->name,
		       serdes->dev->name, reg, val, mask);

	if (serdes->chip_data->reg_val_type == REG_8BIT_VAL_8IT)
		ret = dm_i2c_reg_clrset_u8(serdes->dev, reg, mask, val);
	else
		ret = dm_i2c_reg_clrset(serdes->dev, reg, mask, val);

	return ret;
}
EXPORT_SYMBOL_GPL(serdes_set_bits);

int serdes_i2c_set_sequence(struct serdes *serdes)
{
	int i, ret = 0;
	unsigned int def = 0;

	for (i = 0; i < serdes->serdes_init_seq->reg_seq_cnt; i++) {
		if (serdes->serdes_init_seq->reg_sequence[i].reg == 0xffff) {
			SERDES_DBG_MFD("%s: delay 0x%04x us\n", __func__,
				       serdes->serdes_init_seq->reg_sequence[i].def);
			udelay(serdes->serdes_init_seq->reg_sequence[i].def);
			continue;
		}

		ret = serdes_reg_write(serdes,
				       serdes->serdes_init_seq->reg_sequence[i].reg,
				       serdes->serdes_init_seq->reg_sequence[i].def);

		if (ret < 0) {
			SERDES_DBG_MFD("failed to write reg %04x, ret %d\n",
				       serdes->serdes_init_seq->reg_sequence[i].reg, ret);
			ret = serdes_reg_write(serdes,
					       serdes->serdes_init_seq->reg_sequence[i].reg,
					       serdes->serdes_init_seq->reg_sequence[i].def);
		}
		serdes_reg_read(serdes, serdes->serdes_init_seq->reg_sequence[i].reg, &def);
		if ((def != serdes->serdes_init_seq->reg_sequence[i].def) || (ret < 0)) {
			/*if read value != write value then write again*/
			printf("%s read %04x %04x != %04x\n", serdes->dev->name,
			       serdes->serdes_init_seq->reg_sequence[i].reg,
			       def, serdes->serdes_init_seq->reg_sequence[i].def);
			ret = serdes_reg_write(serdes,
					       serdes->serdes_init_seq->reg_sequence[i].reg,
					       serdes->serdes_init_seq->reg_sequence[i].def);
		}
	}

	SERDES_DBG_MFD("serdes %s sequence_init\n", serdes->dev->name);

	return ret;
}
EXPORT_SYMBOL_GPL(serdes_i2c_set_sequence);

int serdes_parse_init_seq(struct udevice *dev, const u16 *data,
			  int length, struct serdes_init_seq *seq)
{
	struct reg_sequence *reg_sequence;
	u16 *buf, *d;
	unsigned int i, cnt;
	int ret;

	if (!seq)
		return -EINVAL;

	buf = calloc(1, length);
	if (!buf)
		return -ENOMEM;

	memcpy(buf, data, length);

	d = buf;
	cnt = length / 4;
	seq->reg_seq_cnt = cnt;

	seq->reg_sequence = calloc(cnt, sizeof(struct reg_sequence));
	if (!seq->reg_sequence) {
		ret = -ENOMEM;
		goto free_buf;
	}

	for (i = 0; i < cnt; i++) {
		reg_sequence = &seq->reg_sequence[i];
		reg_sequence->reg = get_unaligned_be16(&d[0]);
		reg_sequence->def = get_unaligned_be16(&d[1]);
		d += 2;
	}

	return 0;

free_buf:
	free(buf);

	return ret;
}
EXPORT_SYMBOL_GPL(serdes_parse_init_seq);

int serdes_get_init_seq(struct serdes *serdes)
{
	const void *data = NULL;
	int len, err;

	data = dev_read_prop(serdes->dev, "serdes-init-sequence", &len);
	if (!data) {
		printf("failed to get serdes-init-sequence\n");
		return -EINVAL;
	}

	serdes->serdes_init_seq = calloc(1, sizeof(*serdes->serdes_init_seq));
	if (!serdes->serdes_init_seq)
		return -ENOMEM;

	err = serdes_parse_init_seq(serdes->dev,
				    data, len, serdes->serdes_init_seq);
	if (err) {
		printf("failed to parse serdes-init-sequence\n");
		goto free_init_seq;
	}

	return 0;

free_init_seq:
	free(serdes->serdes_init_seq);

	return err;
}
EXPORT_SYMBOL_GPL(serdes_get_init_seq);

int serdes_gpio_register(struct udevice *dev,
			 struct serdes *serdes)
{
	bool pre_reloc_only = !(gd->flags & GD_FLG_RELOC);
	struct uclass_driver *drv;
	int ret = -ENODEV;
	const char *name;
	ofnode subnode;
	struct udevice *subdev;
	struct udevice *gpio_dev;

	SERDES_DBG_MFD("%s node=%s\n",
		       __func__, ofnode_get_name(dev->node));

	/* Lookup GPIO driver */
	drv = lists_uclass_lookup(UCLASS_GPIO);
	if (!drv) {
		printf("Cannot find GPIO driver\n");
		return -ENOENT;
	}

	dev_for_each_subnode(subnode, dev) {
		if (pre_reloc_only &&
		    !ofnode_pre_reloc(subnode))
			continue;

		name = ofnode_get_name(subnode);
		if (!name)
			continue;

		if (strstr(name, "gpio")) {
			ret = device_bind_driver_to_node(dev,
							 "serdes-gpio", name,
							 subnode, &subdev);
			if (ret)
				return ret;

			ret = uclass_get_device_by_ofnode(UCLASS_GPIO,
							  subnode,
							  &gpio_dev);
			if (ret) {
				printf("%s failed to get gpio dev\n", __func__);
				return ret;
			}

			SERDES_DBG_MFD("%s select %s gpio_dev=%s\n",
				       __func__, name, gpio_dev->name);
			return 0;
		}
	}

	return ret;
}
EXPORT_SYMBOL_GPL(serdes_gpio_register);

int serdes_pinctrl_register(struct udevice *dev,
			    struct serdes *serdes)
{
	bool pre_reloc_only = !(gd->flags & GD_FLG_RELOC);
	struct uclass_driver *drv;
	int ret = -ENODEV;
	const char *name;
	ofnode subnode;
	struct udevice *subdev;
	struct udevice *pinctrl_dev;

	SERDES_DBG_MFD("%s node=%s\n",
		       __func__, ofnode_get_name(dev->node));

	/* Lookup PINCTRL driver */
	drv = lists_uclass_lookup(UCLASS_PINCTRL);
	if (!drv) {
		printf("Cannot find PINCTRL driver\n");
		return -ENOENT;
	}

	dev_for_each_subnode(subnode, dev) {
		if (pre_reloc_only &&
		    !ofnode_pre_reloc(subnode))
			continue;

		name = ofnode_get_name(subnode);
		if (!name)
			continue;

		if (strstr(name, "pinctrl")) {
			ret = device_bind_driver_to_node(dev,
							 "serdes-pinctrl", name,
							 subnode, &subdev);
			if (ret)
				return ret;

			ret = uclass_get_device_by_ofnode(UCLASS_PINCTRL,
							  subnode,
							  &pinctrl_dev);
			if (ret) {
				printf("%s failed to get pinctrl\n", __func__);
				return ret;
			}

			SERDES_DBG_MFD("%s select %s pinctrl_dev=%s\n",
				       __func__, name, pinctrl_dev->name);
			return 0;
		}
	}

	return ret;
}
EXPORT_SYMBOL_GPL(serdes_pinctrl_register);

static int serdes_i2c_init(struct serdes *serdes)
{
	int ret = 0;
	int i = 0;

	if (serdes->vpower_supply)
		regulator_set_enable(serdes->vpower_supply, true);

	if (dm_gpio_is_valid(&serdes->enable_gpio))
		dm_gpio_set_value(&serdes->enable_gpio, 1);

	mdelay(5);

	for (i = 0; i < 3; i++) {
		ret = serdes_i2c_set_sequence(serdes);
		if (!ret)
			break;
		mdelay(20);
	}

	SERDES_DBG_MFD("%s: %s %s\n", __func__, serdes->dev->name,
		       serdes->chip_data->name);

	return ret;
}

static int serdes_i2c_probe(struct udevice *dev)
{
	struct serdes *serdes = dev_get_priv(dev);
	int ret;

	ret = i2c_set_chip_offset_len(dev, 2);
	if (ret)
		return ret;

	serdes->dev = dev;
	serdes->chip_data = (struct serdes_chip_data *)dev_get_driver_data(dev);
	serdes->type = serdes->chip_data->serdes_type;

	SERDES_DBG_MFD("serdes %s %s probe start\n",
		       serdes->dev->name, serdes->chip_data->name);

	ret = uclass_get_device_by_phandle(UCLASS_REGULATOR, dev,
					   "vpower-supply",
					   &serdes->vpower_supply);
	if (ret && ret != -ENOENT)
		SERDES_DBG_MFD("%s: Cannot get power supply: %d\n",
			       __func__, ret);

	ret = gpio_request_by_name(dev, "enable-gpios", 0,
				   &serdes->enable_gpio, GPIOD_IS_OUT);
	if (ret)
		SERDES_DBG_MFD("%s: failed to get enable gpio: %d\n",
			       __func__, ret);

	ret = gpio_request_by_name(dev, "lock-gpios", 0,
				   &serdes->lock_gpio,
				   GPIOD_IS_IN);
	if (ret)
		SERDES_DBG_MFD("%s: failed to get lock gpio: %d\n",
			       __func__, ret);

	ret = gpio_request_by_name(dev, "err-gpios", 0,
				   &serdes->err_gpio,
				   GPIOD_IS_IN);
	if (ret)
		SERDES_DBG_MFD("%s: failed to err gpio: %d\n",
			       __func__, ret);

	ret = serdes_get_init_seq(serdes);
	if (ret)
		return ret;

	serdes_i2c_init(serdes);

	printf("%s %s successful, version %s\n",
	       __func__,
	       serdes->dev->name,
	       SERDES_UBOOT_DISPLAY_VERSION);

	return 0;
}

static const struct udevice_id serdes_of_match[] = {
#if IS_ENABLED(CONFIG_SERDES_DISPLAY_CHIP_NOVO_NCA9539)
	{ .compatible = "novo,nca9539", .data = (ulong)&serdes_nca9539_data },
#endif
	{ }
};

U_BOOT_DRIVER(serdes_misc) = {
	.name = "serdes-misc",
	.id = UCLASS_MISC,
	.of_match = serdes_of_match,
	.probe = serdes_i2c_probe,
	.priv_auto_alloc_size = sizeof(struct serdes),
};

int serdes_power_init(void)
{
	struct udevice *dev;
	int ret = 0;

	ret = uclass_get_device_by_driver(UCLASS_MISC,
					  DM_GET_DRIVER(serdes_misc),
					  &dev);
	if (ret)
		printf("%s failed to get misc device ret=%d\n", __func__, ret);

	return ret;
}
