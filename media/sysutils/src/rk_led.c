/*
 * Copyright (c) 2022 rockchip
 *
 */

#include "rk_led.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "rk_sysfs.h"

#define GPIO_SYSFS_PATH	"/sys/class/leds"

int rk_led_set_mode(char *dev_path, bool blink, uint32_t brightness) {
    int ret;
    char file[64];

    if (access(GPIO_SYSFS_PATH, F_OK)) {
        printf("warning: /sys/class/leds is not exit\n");
        return -1;
    }

    if (!dev_path) {
        printf("rk_led_set_mode: dev_path is null\n");
        return -1;
    }

    ret = sprintf(file, "%s/%s", GPIO_SYSFS_PATH, dev_path);
    if (ret < 0)
        return ret;

    ret = write_sysfs_int("brightness", file, brightness);
    if (ret < 0) {
        printf("switch led fail\n");
        return -1;
    }
    if (brightness > 0) {
        const char *flag = blink ? "timer" : "none";
        ret = write_sysfs_string("trigger", file, flag);
        if (ret < 0) {
            printf("controll blink fail\n");
            return -1;
        }
        if (!blink) {
            ret = write_sysfs_int("brightness", file, brightness);
            if (ret < 0)
                printf("switch led fail\n");
        }
    }
    return 0;
}

