/*
 * (C) Copyright 2017 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <dm.h>
#include <misc.h>
#include <time.h>
#include <asm/gpio.h>
#include <asm/io.h>
#include <asm/setup.h>
#include <asm/arch/uart.h>
#include <asm/arch/vendor.h>
#include <configs/gva_rk3229.h>

DECLARE_GLOBAL_DATA_PTR;

/* define serialno max length, the max length is 512 Bytes
 * The remaining bytes are used to ensure that the first 512 bytes
 * are valid when executing 'env_set("serial#", value)'.
 */
#define VENDOR_SN_MAX	513
/* These values are provided by the chip documentation */
#define RK3229_CPUID_OFF  0x7
#define RK3229_CPUID_LEN  0x10

/*
 * First obtain the serial number from vendor partition,
 * if vendor partition is not initialized, then generate
 * a default serial number according to CPU Id.
 */
int set_serialno(void)
{
	char serialno_str[VENDOR_SN_MAX];
	struct udevice *dev;
	u8 cpuid[RK3229_CPUID_LEN] = {0};
	u8 low[RK3229_CPUID_LEN/2], high[RK3229_CPUID_LEN/2];
	u64 serialno;
	int ret, i;

	/* Read serial number from vendor storage part */
	memset(serialno_str, 0, VENDOR_SN_MAX);
#ifdef CONFIG_VENDOR_STORAGE_API
	ret = vendor_storage_read(VENDOR_SN_ID, serialno_str, (VENDOR_SN_MAX-1));
	if (ret > 0) {
		env_set("serial#", serialno_str);
	} else {
#endif
#ifdef CONFIG_ROCKCHIP_EFUSE
		/* retrieve the device */
		ret = uclass_get_device_by_driver(UCLASS_MISC,
						  DM_GET_DRIVER(rockchip_efuse), &dev);
		if (ret) {
			printf("%s: could not find efuse device\n", __func__);
			return ret;
		}
		/* read the cpu_id range from the efuses */
		ret = misc_read(dev, RK3229_CPUID_OFF, &cpuid, sizeof(cpuid));
		if (ret) {
			printf("%s: reading cpuid from the efuses failed\n", __func__);
			return ret;
		}
#endif
		/* Generate the serial number based on CPU ID */
		for (i = 0; i < 8; i++) {
			low[i] = cpuid[1 + (i << 1)];
			high[i] = cpuid[i << 1];
		}
		serialno = crc32_no_comp(0, low, 8);
		serialno |= (u64)crc32_no_comp(serialno, high, 8) << 32;
		snprintf(serialno_str, sizeof(serialno_str), "%llx", serialno);

		env_set("serial#", serialno_str);
#ifdef CONFIG_VENDOR_STORAGE_API
	}
#endif
	return 0;
}

#ifdef CONFIG_MISC_INIT_R
int misc_init_r(void)
{
	set_serialno();

	return 0;
}
#endif

#ifdef CONFIG_SERIAL_TAG
void get_board_serial(struct tag_serialnr *serialnr)
{
	char *serial_string;
	u64 serial = 0;

	serial_string = env_get("serial#");

	if (serial_string)
		serial = simple_strtoull(serial_string, NULL, 16);

	serialnr->high = (u32)(serial >> 32);
	serialnr->low = (u32)(serial & 0xffffffff);
}
#endif

#define FASTBOOT_KEY_GPIO 43 /*GPIO1B3*/

int fastboot_key_pressed(void)
{
	gpio_request(FASTBOOT_KEY_GPIO, "fastboot_key");
	gpio_direction_input(FASTBOOT_KEY_GPIO);
	return !gpio_get_value(FASTBOOT_KEY_GPIO);
}

int rk_board_late_init(void)
{
	if (fastboot_key_pressed()) {
		printf("enter fastboot!\n");
		env_set("preboot", "setenv preboot; fastboot usb0");
	}

	return 0;
}
