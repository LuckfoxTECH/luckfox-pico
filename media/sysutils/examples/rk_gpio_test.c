/*
 * Copyright (c) 2022 rockchip
 *
 */

#include <stdio.h>
#include <unistd.h>
#include "rk_gpio.h"

int main(int argc, char *argv[])
{
    uint32_t in_pin = GPIO(RK_GPIO4, RK_PB6);
    uint32_t out_pin = GPIO(RK_GPIO1, RK_PA6);

    rk_gpio_export_direction(in_pin, GPIO_DIRECTION_INPUT);
    rk_gpio_export_direction(out_pin, GPIO_DIRECTION_OUTPUT);
    rk_gpio_set_value(out_pin, 0);

    while (1) {
        usleep(20000);
        rk_gpio_set_value(out_pin, rk_gpio_get_value(in_pin));
    }

    rk_gpio_unexport(in_pin);
    rk_gpio_unexport(out_pin);

    return 0;
}
