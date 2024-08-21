/*
 * (C) Copyright 2020 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <adc.h>
#include <div64.h>
#include <fdtdec.h>
#include <dm/uclass.h>

DECLARE_GLOBAL_DATA_PTR;

static int adc_raw_to_mV(struct udevice *dev, unsigned int raw, int *mV)
{
	unsigned int data_mask;
	int ret, vref = 1800000;
	u64 raw64 = raw;

	ret = adc_data_mask(dev, &data_mask);
	if (ret)
		return ret;

	raw64 *= vref;
	do_div(raw64, data_mask);
	*mV = raw64;

	return 0;
}

int key_read(int code)
{
	const void *fdt_blob = gd->fdt_blob;
	struct udevice *dev;
	int adc_node, offset;
	int t, down_threshold = -1, up_threshold;
	int ret, num = 0, volt_margin = 150000;	/* will be div 2 */
	int mV, cd, voltage = -1;
	int min, max;
	u32 chn[2], adc;

	ret = uclass_get_device_by_name(UCLASS_ADC, "saradc", &dev);
	if (ret)
		ret = uclass_get_device_by_name(UCLASS_ADC, "adc", &dev);
	if (ret) {
		debug("No saradc device, ret=%d\n", ret);
		return 0;
	}

	adc_node = fdt_node_offset_by_compatible(fdt_blob, 0, "adc-keys");
	if (adc_node < 0) {
		debug("No 'adc-keys' node, ret=%d\n", adc_node);
		return 0;
	}

	ret = fdtdec_get_int_array(fdt_blob, adc_node, "io-channels",
				   chn, ARRAY_SIZE(chn));
	if (ret) {
		debug("Can't read 'io-channels', ret=%d\n", ret);
		return 0;
	}

	up_threshold = fdtdec_get_int(fdt_blob, adc_node,
				      "keyup-threshold-microvolt", -ENODATA);
	if (up_threshold < 0) {
		debug("Can't read 'keyup-threshold-microvolt'\n");
		return 0;
	}

	/* find the expected key-code */
	for (offset = fdt_first_subnode(fdt_blob, adc_node);
	     offset >= 0;
	     offset = fdt_next_subnode(fdt_blob, offset)) {
		cd = fdtdec_get_int(fdt_blob, offset, "linux,code", -ENODATA);
		if (cd < 0) {
			debug("Can't read 'linux,code', ret=%d\n", cd);
			return 0;
		}

		if (cd == code) {
			voltage = fdtdec_get_int(fdt_blob, offset,
					"press-threshold-microvolt", -ENODATA);
			if (voltage < 0) {
				debug("Can't read 'press-threshold-microvolt'\n");
				return 0;
			}
			break;
		}
	}

	if (voltage < 0)
		return 0;

	for (offset = fdt_first_subnode(fdt_blob, adc_node);
	     offset >= 0;
	     offset = fdt_next_subnode(fdt_blob, offset)) {
		t = fdtdec_get_int(fdt_blob, offset,
				   "press-threshold-microvolt", -ENODATA);
		if (t < 0) {
			debug("Can't read 'press-threshold-microvolt'\n");
			return 0;
		}

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

	/*
	 * Define the voltage range such that the button is only pressed
	 * when the voltage is closest to its own press-threshold-microvolt
	 */
	if (down_threshold < 0)
		min = 0;
	else
		min = down_threshold + (voltage - down_threshold) / 2;

	max = voltage + (up_threshold - voltage) / 2;

	/* now, read key status */
	ret = adc_channel_single_shot("saradc", chn[1], &adc);
	if (ret)
		ret = adc_channel_single_shot("adc", chn[1], &adc);
	if (ret) {
		debug("Failed to read adc%d, ret=%d\n", chn[1], ret);
		return 0;
	}

	ret = adc_raw_to_mV(dev, adc, &mV);
	if (ret) {
		debug("Failed to convert adc to mV, ret=%d\n", ret);
		return 0;
	}

	debug("key[%d] <%d, %d, %d>: adc=%d -> mV=%d\n",
	      code, min, voltage, max, adc, mV);

	return (mV <= max && mV >= min);
}

