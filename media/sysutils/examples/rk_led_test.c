/*
 * Copyright (c) 2022 rockchip
 *
 */

#include <stdio.h>
#include <unistd.h>
#include "rk_led.h"

int main(int argc, char *argv[])
{
    rk_led_set_mode("white", true, 255);

    return 0;
}

