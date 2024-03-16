/*****************************************************************************
* | File        :   luckfox_gpio_test.c
* | Author      :   luckfox team
* | Function    :   gpio output test
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

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h> //exit()
#include "luckfox_gpio.h"

int GPIO_BEGIN_PIN, GPIO_END_PIN;
int *TEST_PIN;

int PICO_MAX_GPIOS[] = {
    LUCKFOX_PICO_MAX_GPIO0,
    LUCKFOX_PICO_MAX_GPIO1,
    LUCKFOX_PICO_MAX_GPIO2,
    LUCKFOX_PICO_MAX_GPIO3,
    LUCKFOX_PICO_MAX_GPIO4,
    LUCKFOX_PICO_MAX_GPIO5,
    LUCKFOX_PICO_MAX_GPIO6,
    LUCKFOX_PICO_MAX_GPIO7,
    LUCKFOX_PICO_MAX_GPIO8,
    LUCKFOX_PICO_MAX_GPIO9,
    LUCKFOX_PICO_MAX_GPIO10,
    LUCKFOX_PICO_MAX_GPIO11,
    LUCKFOX_PICO_MAX_GPIO12,
    LUCKFOX_PICO_MAX_GPIO13,
    LUCKFOX_PICO_MAX_GPIO14,
    LUCKFOX_PICO_MAX_GPIO15,
    LUCKFOX_PICO_MAX_GPIO16,
    LUCKFOX_PICO_MAX_GPIO17,
    LUCKFOX_PICO_MAX_GPIO18,
    LUCKFOX_PICO_MAX_GPIO19,
    LUCKFOX_PICO_MAX_GPIO20,
    LUCKFOX_PICO_MAX_GPIO21,
    LUCKFOX_PICO_MAX_GPIO22,
    /*23-25*/
    LUCKFOX_PICO_MAX_GPIO26,
    LUCKFOX_PICO_MAX_GPIO27,
    LUCKFOX_PICO_MAX_GPIO28,

};

int PICO_PLUS_GPIOS[] = {
    LUCKFOX_PICO_PLUS_GPIO0,
    LUCKFOX_PICO_PLUS_GPIO1,
    LUCKFOX_PICO_PLUS_GPIO2,
    LUCKFOX_PICO_PLUS_GPIO3,
    LUCKFOX_PICO_PLUS_GPIO4,
    LUCKFOX_PICO_PLUS_GPIO5,
    LUCKFOX_PICO_PLUS_GPIO6,
    LUCKFOX_PICO_PLUS_GPIO7,
    LUCKFOX_PICO_PLUS_GPIO8,
    LUCKFOX_PICO_PLUS_GPIO9,
    LUCKFOX_PICO_PLUS_GPIO10,
    LUCKFOX_PICO_PLUS_GPIO11,
    LUCKFOX_PICO_PLUS_GPIO12,
    LUCKFOX_PICO_PLUS_GPIO13,
    LUCKFOX_PICO_PLUS_GPIO14,
    LUCKFOX_PICO_PLUS_GPIO15,
    LUCKFOX_PICO_PLUS_GPIO16,
    LUCKFOX_PICO_PLUS_GPIO17,
    LUCKFOX_PICO_PLUS_GPIO18,
    LUCKFOX_PICO_PLUS_GPIO19,
    LUCKFOX_PICO_PLUS_GPIO20,
    LUCKFOX_PICO_PLUS_GPIO21,
    LUCKFOX_PICO_PLUS_GPIO22,
    /*23-25*/
    LUCKFOX_PICO_PLUS_GPIO26,
    LUCKFOX_PICO_PLUS_GPIO27,
};
int PICO_GPIOS[] = {
    LUCKFOX_PICO_GPIO0,
    LUCKFOX_PICO_GPIO1,
    LUCKFOX_PICO_GPIO2,
    LUCKFOX_PICO_GPIO3,
    LUCKFOX_PICO_GPIO4,
    LUCKFOX_PICO_GPIO5,
    LUCKFOX_PICO_GPIO6,
    LUCKFOX_PICO_GPIO7,
    LUCKFOX_PICO_GPIO8,
    LUCKFOX_PICO_GPIO9,
    LUCKFOX_PICO_GPIO10,
    LUCKFOX_PICO_GPIO11,
    LUCKFOX_PICO_GPIO12,
    LUCKFOX_PICO_GPIO13,
    LUCKFOX_PICO_GPIO14,
    LUCKFOX_PICO_GPIO15,
    LUCKFOX_PICO_GPIO16,
    LUCKFOX_PICO_GPIO17,
    LUCKFOX_PICO_GPIO18,
    LUCKFOX_PICO_GPIO19,
    LUCKFOX_PICO_GPIO20,
    LUCKFOX_PICO_GPIO21,
    LUCKFOX_PICO_GPIO26,
    LUCKFOX_PICO_GPIO27,

};

int PICO_ULTRA_GPIOS[] = {
    LUCKFOX_PICO_ULTRA_GPIO0,
    LUCKFOX_PICO_ULTRA_GPIO1,
    LUCKFOX_PICO_ULTRA_GPIO2,
    LUCKFOX_PICO_ULTRA_GPIO3,
    LUCKFOX_PICO_ULTRA_GPIO4,
    LUCKFOX_PICO_ULTRA_GPIO5,
    LUCKFOX_PICO_ULTRA_GPIO6,
    LUCKFOX_PICO_ULTRA_GPIO7,
    LUCKFOX_PICO_ULTRA_GPIO8,
    LUCKFOX_PICO_ULTRA_GPIO9,
    LUCKFOX_PICO_ULTRA_GPIO10,
    LUCKFOX_PICO_ULTRA_GPIO11,
    LUCKFOX_PICO_ULTRA_GPIO12,
    LUCKFOX_PICO_ULTRA_GPIO13,
    LUCKFOX_PICO_ULTRA_GPIO14,
    LUCKFOX_PICO_ULTRA_GPIO15,
    LUCKFOX_PICO_ULTRA_GPIO16,
    LUCKFOX_PICO_ULTRA_GPIO17,
    LUCKFOX_PICO_ULTRA_GPIO18,
    LUCKFOX_PICO_ULTRA_GPIO19,
    LUCKFOX_PICO_ULTRA_GPIO20,
    LUCKFOX_PICO_ULTRA_GPIO21,
    LUCKFOX_PICO_ULTRA_GPIO22,
    LUCKFOX_PICO_ULTRA_GPIO23,
    LUCKFOX_PICO_ULTRA_GPIO24,
    LUCKFOX_PICO_ULTRA_GPIO25,
    LUCKFOX_PICO_ULTRA_GPIO26,
    LUCKFOX_PICO_ULTRA_GPIO27,
    LUCKFOX_PICO_ULTRA_GPIO28,
    LUCKFOX_PICO_ULTRA_GPIO29,
};

void Delay_ms(uint32_t xms)
{
    uint32_t i;
    for (i = 0; i < xms; i++)
    {
        usleep(1000);
    }
}

void Handler(int signo)
{
    // System Exit
    printf("\r\nHandler:exit\r\n");
    for (int x = GPIO_BEGIN_PIN; x <= GPIO_END_PIN; x++)
    {
        luckfox_gpio_set_value(TEST_PIN[x], 0);
        luckfox_gpio_unexport(TEST_PIN[x]);
    }
    exit(0);
}

int main(int argc, char *argv[])
{
    char input_char = '0';
    int delay_time = 500;
    signal(SIGINT, Handler);
    printf("-----------------------------\r\n");
    printf("----------GPIO TEST----------\r\n");
    printf("-----------------------------\r\n");
    printf("Please select your test borad\r\n");
    printf("* 1. LUCKFOX PICO\r\n");
    printf("* 2. LUCKFOX PICO PLUS\r\n");
    printf("* 3. LUCKFOX PICO MAX\r\n");
    printf("* 4. LUCKFOX PICO MAX\r\n");
    printf("-----------------------------\r\n");
    while (1)
    {
        printf("Which would you like? :");
        input_char = getchar();
        if (input_char >= '1' && input_char <= '4')
        {
            break;
        }
    }

    if (input_char == '1')
    {
        GPIO_BEGIN_PIN = 16;
        GPIO_END_PIN = 21;
        TEST_PIN = PICO_GPIOS;
    }
    else if (input_char == '2')
    {
        GPIO_BEGIN_PIN = 16;
        GPIO_END_PIN = 22;
        TEST_PIN = PICO_PLUS_GPIOS;
    }
    else if (input_char == '3')
    {
        GPIO_BEGIN_PIN = 16;
        GPIO_END_PIN = 22;
        TEST_PIN = PICO_MAX_GPIOS;
    }
    else if (input_char == '4')
    {
        GPIO_BEGIN_PIN = 2;
        GPIO_END_PIN = 27;
        TEST_PIN = PICO_ULTRA_GPIOS;
    }
    else
    {
        exit(0);
    }
    printf("-----------------------------\r\n");
    printf("Please select your test borad\r\n");
    printf("* 1. Default part(GP%d-GP%d)\r\n", GPIO_BEGIN_PIN, GPIO_END_PIN);
    printf("* 2. ALL(GP2-GP%d)\r\n", GPIO_END_PIN);
    printf("-----------------------------\r\n");
    while (1)
    {
        printf("Which would you like? :");
        input_char = getchar();
        if (input_char >= '1' && input_char <= '2')
        {
            break;
        }
    }

    if (input_char == '1')
    {
        ;
    }
    else if (input_char == '2')
    {
        GPIO_BEGIN_PIN = 2;
        delay_time = 250;
    }
    else
    {
        exit(0);
    }

    for (int x = GPIO_BEGIN_PIN; x <= GPIO_END_PIN; x++)
    {
        luckfox_gpio_export_direction(TEST_PIN[x], GPIO_DIRECTION_OUTPUT);
        luckfox_gpio_set_value(TEST_PIN[x], 0);
    }
    while (1)
    {

        for (int x = GPIO_BEGIN_PIN; x <= GPIO_END_PIN; x++)
        {
            luckfox_gpio_set_value(TEST_PIN[x], 1);
            printf("GP%d : GPIO%d set \r\n", x, TEST_PIN[x]);
            Delay_ms(delay_time);
        }
        for (int x = GPIO_BEGIN_PIN; x <= GPIO_END_PIN; x++)
        {
            luckfox_gpio_set_value(TEST_PIN[x], 0);
            printf("GP%d : GPIO%d reset \r\n", x, TEST_PIN[x]);
            Delay_ms(delay_time);
        }
    }

    return 0;
}
