/*****************************************************************************
* | File        :   luckfox_gpio.h
* | Author      :   luckfox team
* | Function    :   gpio 
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2023-08-01
* | Info        :   Basic version
*
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#ifndef _GPIO_H_
#define _GPIO_H_

#include <stdbool.h>
#include <stdint.h>

#define GPIO0	0
#define GPIO1	1
#define GPIO2	2
#define GPIO3	3
#define GPIO4	4
#define GPIO6	6

#define PA0		0
#define PA1		1
#define PA2		2
#define PA3		3
#define PA4		4
#define PA5		5
#define PA6		6
#define PA7		7
#define PB0		8
#define PB1		9
#define PB2		10
#define PB3		11
#define PB4		12
#define PB5		13
#define PB6		14
#define PB7		15
#define PC0		16
#define PC1		17
#define PC2		18
#define PC3		19
#define PC4		20
#define PC5		21
#define PC6		22
#define PC7		23
#define PD0		24
#define PD1		25
#define PD2		26
#define PD3		27
#define PD4		28
#define PD5		29
#define PD6		30
#define PD7		31

#define GPIO(bank, pin)	((((bank) * 32) + (pin)))

/**********************************************/
#define LUCKFOX_PICO_MAX_GPIO0      GPIO(GPIO1,PB2)
#define LUCKFOX_PICO_MAX_GPIO1      GPIO(GPIO1,PB3)

#define LUCKFOX_PICO_MAX_GPIO2      GPIO(GPIO1,PC7)
#define LUCKFOX_PICO_MAX_GPIO3      GPIO(GPIO1,PC6)
#define LUCKFOX_PICO_MAX_GPIO4      GPIO(GPIO1,PC5)
#define LUCKFOX_PICO_MAX_GPIO5      GPIO(GPIO1,PC4)

#define LUCKFOX_PICO_MAX_GPIO6      GPIO(GPIO1,PD2)
#define LUCKFOX_PICO_MAX_GPIO7      GPIO(GPIO1,PD3)
#define LUCKFOX_PICO_MAX_GPIO8      GPIO(GPIO2,PB1)
#define LUCKFOX_PICO_MAX_GPIO9      GPIO(GPIO1,PC0)

#define LUCKFOX_PICO_MAX_GPIO10     GPIO(GPIO1,PC1)
#define LUCKFOX_PICO_MAX_GPIO11     GPIO(GPIO1,PC2)
#define LUCKFOX_PICO_MAX_GPIO12     GPIO(GPIO1,PC3)
#define LUCKFOX_PICO_MAX_GPIO13     GPIO(GPIO2,PB0)

#define LUCKFOX_PICO_MAX_GPIO14     GPIO(GPIO1,PD0)
#define LUCKFOX_PICO_MAX_GPIO15     GPIO(GPIO1,PD1)

/**********************************************/
#define LUCKFOX_PICO_MAX_GPIO28     GPIO(GPIO2,PA7)

#define LUCKFOX_PICO_MAX_GPIO27     GPIO(GPIO4,PC1)
#define LUCKFOX_PICO_MAX_GPIO26     GPIO(GPIO4,PC0)

#define LUCKFOX_PICO_MAX_GPIO22     GPIO(GPIO2,PA6)

#define LUCKFOX_PICO_MAX_GPIO21     GPIO(GPIO2,PA3)
#define LUCKFOX_PICO_MAX_GPIO20     GPIO(GPIO2,PA2)
#define LUCKFOX_PICO_MAX_GPIO19     GPIO(GPIO2,PA1)
#define LUCKFOX_PICO_MAX_GPIO18     GPIO(GPIO2,PA0)

#define LUCKFOX_PICO_MAX_GPIO17     GPIO(GPIO2,PA5)
#define LUCKFOX_PICO_MAX_GPIO16     GPIO(GPIO2,PA4)



/**********************************************/
#define LUCKFOX_PICO_PLUS_GPIO0      GPIO(GPIO1,PB2)
#define LUCKFOX_PICO_PLUS_GPIO1      GPIO(GPIO1,PB3)

#define LUCKFOX_PICO_PLUS_GPIO2      GPIO(GPIO1,PC7)
#define LUCKFOX_PICO_PLUS_GPIO3      GPIO(GPIO1,PC6)
#define LUCKFOX_PICO_PLUS_GPIO4      GPIO(GPIO1,PC5)
#define LUCKFOX_PICO_PLUS_GPIO5      GPIO(GPIO1,PC4)

#define LUCKFOX_PICO_PLUS_GPIO6      GPIO(GPIO1,PD2)
#define LUCKFOX_PICO_PLUS_GPIO7      GPIO(GPIO1,PD3)
#define LUCKFOX_PICO_PLUS_GPIO8      GPIO(GPIO1,PA2)
#define LUCKFOX_PICO_PLUS_GPIO9      GPIO(GPIO1,PC0)

#define LUCKFOX_PICO_PLUS_GPIO10     GPIO(GPIO1,PC1)
#define LUCKFOX_PICO_PLUS_GPIO11     GPIO(GPIO1,PC2)
#define LUCKFOX_PICO_PLUS_GPIO12     GPIO(GPIO1,PC3)
#define LUCKFOX_PICO_PLUS_GPIO13     GPIO(GPIO0,PA4)

#define LUCKFOX_PICO_PLUS_GPIO14     GPIO(GPIO1,PD0)
#define LUCKFOX_PICO_PLUS_GPIO15     GPIO(GPIO1,PD1)

/**********************************************/
#define LUCKFOX_PICO_PLUS_GPIO27     GPIO(GPIO4,PC1)
#define LUCKFOX_PICO_PLUS_GPIO26     GPIO(GPIO4,PC0)

#define LUCKFOX_PICO_PLUS_GPIO22     GPIO(GPIO3,PA1)

#define LUCKFOX_PICO_PLUS_GPIO21     GPIO(GPIO3,PA3)
#define LUCKFOX_PICO_PLUS_GPIO20     GPIO(GPIO3,PA2)
#define LUCKFOX_PICO_PLUS_GPIO19     GPIO(GPIO3,PA4)
#define LUCKFOX_PICO_PLUS_GPIO18     GPIO(GPIO3,PA5)

#define LUCKFOX_PICO_PLUS_GPIO17     GPIO(GPIO3,PA7)
#define LUCKFOX_PICO_PLUS_GPIO16     GPIO(GPIO3,PA6)


/**********************************************/
#define LUCKFOX_PICO_GPIO0      GPIO(GPIO1,PB2)
#define LUCKFOX_PICO_GPIO1      GPIO(GPIO1,PB3)

#define LUCKFOX_PICO_GPIO2      GPIO(GPIO1,PC7)
#define LUCKFOX_PICO_GPIO3      GPIO(GPIO1,PC6)
#define LUCKFOX_PICO_GPIO4      GPIO(GPIO1,PC5)
#define LUCKFOX_PICO_GPIO5      GPIO(GPIO1,PC4)

#define LUCKFOX_PICO_GPIO6      GPIO(GPIO1,PD2)
#define LUCKFOX_PICO_GPIO7      GPIO(GPIO1,PD3)
#define LUCKFOX_PICO_GPIO8      GPIO(GPIO1,PA2)
#define LUCKFOX_PICO_GPIO9      GPIO(GPIO1,PC0)

#define LUCKFOX_PICO_GPIO10     GPIO(GPIO1,PC1)
#define LUCKFOX_PICO_GPIO11     GPIO(GPIO1,PC2)
#define LUCKFOX_PICO_GPIO12     GPIO(GPIO1,PC3)
#define LUCKFOX_PICO_GPIO13     GPIO(GPIO0,PA4)

#define LUCKFOX_PICO_GPIO14     GPIO(GPIO1,PD0)
#define LUCKFOX_PICO_GPIO15     GPIO(GPIO1,PD1)

/**********************************************/
#define LUCKFOX_PICO_GPIO27     GPIO(GPIO4,PC1)
#define LUCKFOX_PICO_GPIO26     GPIO(GPIO4,PC0)

#define LUCKFOX_PICO_GPIO21     GPIO(GPIO4,PA4)
#define LUCKFOX_PICO_GPIO20     GPIO(GPIO4,PA3)
#define LUCKFOX_PICO_GPIO19     GPIO(GPIO4,PA2)
#define LUCKFOX_PICO_GPIO18     GPIO(GPIO4,PA6)

#define LUCKFOX_PICO_GPIO17     GPIO(GPIO4,PB0)
#define LUCKFOX_PICO_GPIO16     GPIO(GPIO4,PB1)

enum gpio_direction
{
    GPIO_DIRECTION_OUTPUT = 0,
    GPIO_DIRECTION_INPUT,
};

int luckfox_gpio_export(uint32_t gpio);
int luckfox_gpio_unexport(uint32_t gpio);
int luckfox_gpio_set_direction(uint32_t gpio, enum gpio_direction input);
int luckfox_gpio_get_direction(uint32_t gpio);
int luckfox_gpio_export_direction(uint32_t gpio, enum gpio_direction input);
int luckfox_gpio_set_value(uint32_t gpio, int value);
int luckfox_gpio_get_value(uint32_t gpio);

#endif
