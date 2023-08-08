/*
 * Copyright (c) 2022 rockchip
 *
 */
#ifndef _GPIO_H_
#define _GPIO_H_

#include <stdbool.h>
#include <stdint.h>

#define RK_GPIO0	0
#define RK_GPIO1	1
#define RK_GPIO2	2
#define RK_GPIO3	3
#define RK_GPIO4	4
#define RK_GPIO6	6

#define RK_PA0		0
#define RK_PA1		1
#define RK_PA2		2
#define RK_PA3		3
#define RK_PA4		4
#define RK_PA5		5
#define RK_PA6		6
#define RK_PA7		7
#define RK_PB0		8
#define RK_PB1		9
#define RK_PB2		10
#define RK_PB3		11
#define RK_PB4		12
#define RK_PB5		13
#define RK_PB6		14
#define RK_PB7		15
#define RK_PC0		16
#define RK_PC1		17
#define RK_PC2		18
#define RK_PC3		19
#define RK_PC4		20
#define RK_PC5		21
#define RK_PC6		22
#define RK_PC7		23
#define RK_PD0		24
#define RK_PD1		25
#define RK_PD2		26
#define RK_PD3		27
#define RK_PD4		28
#define RK_PD5		29
#define RK_PD6		30
#define RK_PD7		31

#define GPIO(bank, pin)	((((bank) * 32) + (pin)))

enum gpio_direction
{
    GPIO_DIRECTION_OUTPUT = 0,
    GPIO_DIRECTION_INPUT,
};

int rk_gpio_export(uint32_t gpio);
int rk_gpio_unexport(uint32_t gpio);
int rk_gpio_set_direction(uint32_t gpio, enum gpio_direction input);
int rk_gpio_get_direction(uint32_t gpio);
int rk_gpio_export_direction(uint32_t gpio, enum gpio_direction input);
int rk_gpio_set_value(uint32_t gpio, int value);
int rk_gpio_get_value(uint32_t gpio);

#endif
