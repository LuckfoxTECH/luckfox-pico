/*
 * Copyright (c) 2022 rockchip
 *
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "rk_sysfs.h"
#include "rk_gpio.h"

#define GPIO_SYSFS_PATH	"/sys/class/gpio"

int rk_gpio_export(uint32_t gpio)
{
    int ret;
    char file[64];

    ret = sprintf(file, "%s/gpio%d", GPIO_SYSFS_PATH, gpio);
    if (ret < 0)
        return ret;
    if (!access(file, F_OK)) {
        printf("warning: gpio %d, file (%s) has exist\n", gpio, file);
        return 0;
    }
    return write_sysfs_int("export", GPIO_SYSFS_PATH, gpio);
}

int rk_gpio_unexport(uint32_t gpio)
{
    return write_sysfs_int("unexport", GPIO_SYSFS_PATH, gpio);
}

int rk_gpio_set_direction(uint32_t gpio, enum gpio_direction input)
{
    int ret;
    char path[64];
    const char *direction = input ? "in" : "out";

    ret = sprintf(path, "%s/gpio%d", GPIO_SYSFS_PATH, gpio);
    if (ret < 0)
        return ret;

    return write_sysfs_string_and_verify("direction", path, direction);
}

int rk_gpio_get_direction(uint32_t gpio)
{
    int ret;
    char path[64];
    char string[15];

    ret = sprintf(path, "%s/gpio%d", GPIO_SYSFS_PATH, gpio);
    if (ret < 0)
        return ret;

    ret = read_sysfs_string("direction", path, string);
    if (ret < 0)
        return ret;

    return (memcmp(string, "out", sizeof("out"))) ?
        GPIO_DIRECTION_INPUT : GPIO_DIRECTION_OUTPUT;
}

int rk_gpio_export_direction(uint32_t gpio, enum gpio_direction input)
{
    int ret;

    ret = rk_gpio_export(gpio);
    if (ret < 0)
        goto out;

    ret = rk_gpio_set_direction(gpio, input);
    if (ret < 0)
        rk_gpio_unexport(gpio);
out:
    return ret;
}

int rk_gpio_set_value(uint32_t gpio, int value)
{
    int ret;
    char path[64] = {0};
    char direction[8] = {0};

    ret = sprintf(path, "%s/gpio%d", GPIO_SYSFS_PATH, gpio);
    if (ret < 0)
        return ret;

    ret = read_sysfs_string("direction", path, direction);
    if (ret < 0)
        return ret;

    if (strcmp(direction, "out") != 0)
        return -EINVAL;

    return write_sysfs_int_and_verify("value", path, value);
}

int rk_gpio_get_value(uint32_t gpio)
{
    int ret;
    char path[64];

    ret = sprintf(path, "%s/gpio%d", GPIO_SYSFS_PATH, gpio);
    if (ret < 0)
        return ret;

    return read_sysfs_posint("value", path);
}
