/*****************************************************************************
* | File        :   luckfox_gpio.c
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

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "luckfox_sysfs.h"
#include "luckfox_gpio.h"

#define GPIO_SYSFS_PATH	"/sys/class/gpio"

int luckfox_gpio_export(uint32_t gpio)
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

int luckfox_gpio_unexport(uint32_t gpio)
{
    return write_sysfs_int("unexport", GPIO_SYSFS_PATH, gpio);
}

int luckfox_gpio_set_direction(uint32_t gpio, enum gpio_direction input)
{
    int ret;
    char path[64];
    const char *direction = input ? "in" : "out";

    ret = sprintf(path, "%s/gpio%d", GPIO_SYSFS_PATH, gpio);
    if (ret < 0)
        return ret;

    return write_sysfs_string_and_verify("direction", path, direction);
}

int luckfox_gpio_get_direction(uint32_t gpio)
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

int luckfox_gpio_export_direction(uint32_t gpio, enum gpio_direction input)
{
    int ret;

    ret = luckfox_gpio_export(gpio);
    if (ret < 0)
        goto out;

    ret = luckfox_gpio_set_direction(gpio, input);
    if (ret < 0)
        luckfox_gpio_unexport(gpio);
out:
    return ret;
}

int luckfox_gpio_set_value(uint32_t gpio, int value)
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

int luckfox_gpio_get_value(uint32_t gpio)
{
    int ret;
    char path[64];

    ret = sprintf(path, "%s/gpio%d", GPIO_SYSFS_PATH, gpio);
    if (ret < 0)
        return ret;

    return read_sysfs_posint("value", path);
}
