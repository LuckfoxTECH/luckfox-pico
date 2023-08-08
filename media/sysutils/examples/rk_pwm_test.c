/*
 * Copyright (c) 2022 rockchip
 *
 */

/*
 * Test in sweeperrobort board,
 * switch left-moto-lift button,
 * the left moto should be started when we push the button
 */
#include <stdio.h>
#include <unistd.h>
#include "rk_gpio.h"
#include "rk_pwm.h"

#define PWM_USE	2

int main(int argc, char *argv[])
{
    int ret, store;
    bool is_quit = true;
    uint32_t in_pin = GPIO(RK_GPIO4, RK_PB6);
    uint32_t out_pin = GPIO(RK_GPIO1, RK_PA6);

    rk_gpio_export_direction(in_pin, GPIO_DIRECTION_INPUT);
    rk_gpio_export_direction(out_pin, GPIO_DIRECTION_OUTPUT);

    rk_gpio_set_value(out_pin, 0);
    rk_pwm_init(PWM_USE, 62500, 31250, PWM_POLARITY_NORMAL);

    char cmd[64];
    printf("\n#Usage: input 'quit' to exit programe!\n");

    while (is_quit) {
        fgets(cmd, sizeof(cmd), stdin);
        usleep(20000);
        ret = rk_gpio_get_value(in_pin);
        if (ret != store) {
            store = ret;
            rk_gpio_set_value(out_pin, ret);
            rk_pwm_set_enable(PWM_USE, (bool)ret);

            printf( "[TEST_INFO]\n"
                "-- in_gpio: in %d, value %d\n"
                "-- out_gpio: in %d, value %d\n"
                "-- pwm: period %d, duty %d, polartiy %d, enable %d\n",
                rk_gpio_get_direction(in_pin), rk_gpio_get_value(in_pin),
                rk_gpio_get_direction(out_pin), rk_gpio_get_value(out_pin),
                rk_pwm_get_period(PWM_USE), rk_pwm_get_duty(PWM_USE),
                rk_pwm_get_polarity(PWM_USE), rk_pwm_get_enable(PWM_USE));
        }
    }
    rk_gpio_unexport(in_pin);
    rk_gpio_unexport(out_pin);
    rk_pwm_deinit(PWM_USE);
    return 0;
}

