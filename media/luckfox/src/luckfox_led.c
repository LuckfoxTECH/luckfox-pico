/*****************************************************************************
* | File        :   luckfox_led.c
* | Author      :   luckfox team
* | Function    :   led
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
#include "luckfox_led.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "luckfox_sysfs.h"

#define GPIO_SYSFS_PATH	"/sys/class/leds"

int luckfox_led_set_mode(char *dev_path, bool blink, uint32_t brightness) {
    int ret;
    char file[64];

    if (access(GPIO_SYSFS_PATH, F_OK)) {
        printf("warning: /sys/class/leds is not exit\n");
        return -1;
    }

    if (!dev_path) {
        printf("luckfox_led_set_mode: dev_path is null\n");
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

