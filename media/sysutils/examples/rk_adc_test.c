/*
 * Copyright (c) 2022 rockchip
 *
 */
#include <stdio.h>
#include "rk_adc.h"

int main(int argc, char *argv[])
{
    printf("rk_adc_get_devnum %d\n", rk_adc_get_devnum("ff5e0000.saradc"));
    printf("rk_adc_get_value %d, %d, %d, %d, %d\n",
        rk_adc_get_value(rk_adc_get_devnum("ff5e0000.saradc"), 0),
        rk_adc_get_value(rk_adc_get_devnum("ff5e0000.saradc"), 1),
        rk_adc_get_value(rk_adc_get_devnum("ff5e0000.saradc"), 2),
        rk_adc_get_value(rk_adc_get_devnum("ff5e0000.saradc"), 3),
        rk_adc_get_value(rk_adc_get_devnum("ff5e0000.saradc"), 4));
    return 0;
}

