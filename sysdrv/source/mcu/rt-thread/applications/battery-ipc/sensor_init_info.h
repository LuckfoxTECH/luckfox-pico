#ifndef __SENSOR_INIT_INFO_H__
#define __SENSOR_INIT_INFO_H__

#define	FLGA_GPIO_OUTPUT        (1<<25)
#define	FLGA_GPIO_INPUT         (0<<25)
#define	FLAG_GPIO_DEFAULT_HIGH  (1<<26)
#define	FLAG_GPIO_DEFAULT_LOW   (0<<26)

#define FIX2INT16(x)            (int)(x * (1 << 16))

struct gpio_info
{
	/*
	 * 例如：
	 *     GPIO2_D3 (2*32 + 3*8 + 3 = 91)
	 *     GPIO3_A2 (3*32 + 0*8 + 2 = 98)
	 */
	int gpio_index;
	/*
	 * bit25: 0 --> gpio is input
	 * bit25: 1 --> gpio is output
	 *
	 * bit26: 0 --> gpio default level is low
	 * bit26: 1 --> gpio default level is high
	 *
	 * bit0~bit3: gpio's iomux
	 */
	int gpio_func;

	/* PWM 通道号 */
	int pwm_channel;
	/* PWM 周期, 单位：纳秒*/
	int pwm_period;
	/* PWM 占空比 */
	int pwm_pulse;

	/* ADC 通道号 */
	int adc_channel;
	 /* 0: adc值越大环境越暗 1：adc值越大环境越亮 */
	int adc_direction;
};

struct sensor_init_cfg
{
	uint32_t head;
	int32_t len;
	uint32_t crc32;

	/**
	 * IRCUT
	 */
	struct gpio_info ircut_a;
	struct gpio_info ircut_b;

	/**
	 * IR_LED
	 */
	struct gpio_info led_ir_enable;
	struct gpio_info led_ir;

	/**
	 * White_LED
	 */
	struct gpio_info led_white_enable;
	struct gpio_info led_white;

	/* ADC */
	struct gpio_info gpio_adc_enable;
	struct gpio_info gpio_adc;

	/* Sensor resolution */
	int32_t cam_w;
	int32_t cam_h;

	/* ambient light sensor: 0: none; 1: analog; 2: digital*/
	int32_t als_type;
	/* 白天夜视切换的环境亮度阈值, typical: FIX2INT16(10.0)*/
	int32_t als_value;
};

#endif /* __SENSOR_INIT_INFO_H__ */
