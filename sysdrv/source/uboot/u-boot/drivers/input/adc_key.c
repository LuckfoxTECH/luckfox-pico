/*
 * (C) Copyright 2017 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <dm.h>
#include <key.h>

static int adc_key_ofdata_to_platdata(struct udevice *dev)
{
	struct dm_key_uclass_platdata *uc_key;
	int t, down_threshold = -1, up_threshold;
	int ret, num = 0, volt_margin = 150000;	/* will be div 2 */
	u32 voltage, chn[2];
	ofnode node;

	uc_key = dev_get_uclass_platdata(dev);
	if (!uc_key)
		return -ENXIO;

	uc_key->type = ADC_KEY;
	uc_key->name = dev_read_string(dev, "label");
	ret = dev_read_u32_array(dev_get_parent(dev),
				 "io-channels", chn, ARRAY_SIZE(chn));
	if (ret) {
		printf("%s: read 'io-channels' failed, ret=%d\n",
		       uc_key->name, ret);
		return -EINVAL;
	}

	up_threshold = dev_read_u32_default(dev_get_parent(dev),
				    "keyup-threshold-microvolt", -ENODATA);
	if (up_threshold < 0)
		return -ENODATA;

	uc_key->code = dev_read_u32_default(dev, "linux,code", -ENODATA);
	if (uc_key->code < 0)
		return -ENODATA;

	voltage = dev_read_u32_default(dev, "press-threshold-microvolt", -ENODATA);
	if (voltage < 0)
		return -ENODATA;

	dev_for_each_subnode(node, dev->parent) {
		ret = ofnode_read_s32(node, "press-threshold-microvolt", &t);
		if (ret)
			return ret;

		if (t > voltage && t < up_threshold)
			up_threshold = t;
		else if (t < voltage && t > down_threshold)
			down_threshold = t;
		num++;
	}

	/* although one node only, it doesn't mean only one key on hardware */
	if (num == 1) {
		down_threshold = voltage - volt_margin;
		up_threshold = voltage + volt_margin;
	}

	uc_key->in_volt = 1;
	uc_key->channel = chn[1];
	uc_key->center = voltage;
	/*
	 * Define the voltage range such that the button is only pressed
	 * when the voltage is closest to its own press-threshold-microvolt
	 */
	if (down_threshold < 0)
		uc_key->min = 0;
	else
		uc_key->min = down_threshold + (voltage - down_threshold) / 2;

	uc_key->max = voltage + (up_threshold - voltage) / 2;

	return 0;
}

U_BOOT_DRIVER(adc_key) = {
	.name   = "adc_key",
	.id     = UCLASS_KEY,
	.ofdata_to_platdata = adc_key_ofdata_to_platdata,
};

/* Key Bus */
static int adc_key_bus_bind(struct udevice *dev)
{
	return key_bind_children(dev, "adc_key");
}

static const struct udevice_id adc_key_bus_match[] = {
	{ .compatible = "adc-keys" },
	{ },
};

U_BOOT_DRIVER(adc_key_bus) = {
	.name	   = "adc_key_bus",
	.id	   = UCLASS_SIMPLE_BUS,
	.of_match  = adc_key_bus_match,
	.bind	   = adc_key_bus_bind,
};
