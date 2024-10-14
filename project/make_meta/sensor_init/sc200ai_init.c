#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "sensor_define.h"
#include "sensor_init_info.h"

int main(int argc, char *argv[])
{
	if (argc == 1) {
		printf("usage: xxx_init <output file name>");
	}

	char *file_path = argv[1];
	int fd = open(file_path, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd < 0) {
		printf("create %s failed: %s\n", file_path, strerror(errno));
		return -1;
	}

	struct sensor_init_cfg init_data = {
		.head                         = 0,
		.len                          = 0,
		.crc32                        = 0,
		.cam_w = 1920,
		.cam_h = 1080,
		.ircut_a = {
			.gpio_index = GPIO1_D1,
			.gpio_func = IOMUX_FUNC0 | FLGA_GPIO_OUTPUT | FLAG_GPIO_DEFAULT_HIGH,
		},
		.ircut_b = {
			.gpio_index = GPIO1_D3,
			.gpio_func = IOMUX_FUNC0 | FLGA_GPIO_OUTPUT | FLAG_GPIO_DEFAULT_LOW,
		},
		.led_ir = {
			.gpio_index = GPIO1_D0,
			.gpio_func = IOMUX_FUNC3 | FLGA_GPIO_OUTPUT | FLAG_GPIO_DEFAULT_LOW,
			/* PWM3_IR_M2 */
			.pwm_channel = 3,
			.pwm_period = 5000,
			.pwm_pulse = 0,
		},
		.led_ir_enable = {
			.gpio_index = GPIO_INVALID,
			.gpio_func = IOMUX_FUNC0 | FLGA_GPIO_OUTPUT,
		},
		.led_white = {
			.gpio_index = GPIO1_D2,
			.gpio_func = IOMUX_FUNC6 | FLGA_GPIO_OUTPUT | FLAG_GPIO_DEFAULT_LOW,
			/* PWM0_M1 */
			.pwm_channel = 0,
			.pwm_period = 5000,
			.pwm_pulse = 0,
		},
		.led_white_enable = {
			.gpio_index = GPIO_INVALID,
			.gpio_func = IOMUX_FUNC0 | FLGA_GPIO_OUTPUT,
		},
		.gpio_adc_enable = {
			.gpio_index = GPIO_INVALID,
			.gpio_func = IOMUX_FUNC0 | FLGA_GPIO_OUTPUT,
		},
		.gpio_adc = {
			.gpio_index = GPIO4_C1,
			.gpio_func = IOMUX_FUNC1 | FLGA_GPIO_INPUT,
			/* SARADC_IN1 */
			.adc_channel = 1,
			.adc_direction = 0,
		},
		.als_type = ALS_TYPE_DIGITAL,
		.als_value = FIX2INT16(3.0),
	};

	int len;
	len = write(fd, &init_data, sizeof(struct sensor_init_cfg));

	close(fd);

	if (len < 0) {
		printf("write error:%s\n", strerror(errno));
		return -1;
	}

	printf("make sensor init setting file [%s] len=%d\n", file_path, len);

	return 0;
}
