/*****************************************************************************
* | File        :   luckfox_pwm.c
* | Author      :   luckfox team
* | Function    :   Pulse width modulation
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
#include "luckfox_pwm.h"
#include "luckfox_sysfs.h"

#define PWM_SYSFS_PATH "/sys/class/pwm"

int luckfox_pwm_export(uint32_t pwm)
{
    char path[64];
    char file[64];
    int ret;

    ret = sprintf(path, "%s/pwmchip%d", PWM_SYSFS_PATH, pwm);
    if (ret < 0)
        return ret;
    ret = sprintf(file, "%s/pwmchip%d/pwm0", PWM_SYSFS_PATH, pwm);
    if (ret < 0)
        return ret;

    if (!access(file, F_OK)) {
        printf("warning: pwm %d, file (%s) has exist\n", pwm, file);
        return 0;
    }
    return write_sysfs_int("export", path, 0);
}

int luckfox_pwm_unexport(uint32_t pwm)
{
    char path[64];
    int ret;

    ret = sprintf(path, "%s/pwmchip%d", PWM_SYSFS_PATH, pwm);
    if (ret < 0)
        return ret;
    return write_sysfs_int("unexport", path, 0);
}

int luckfox_pwm_set_duty(uint32_t pwm, uint32_t duty)
{
    char path[64];
    int ret;

    ret = sprintf(path, "%s/pwmchip%d/pwm0", PWM_SYSFS_PATH, pwm);
    if (ret < 0)
        return ret;
    return write_sysfs_int("duty_cycle", path, duty);
}

int luckfox_pwm_get_duty(uint32_t pwm)
{
    char path[64];
    int ret;

    ret = sprintf(path, "%s/pwmchip%d/pwm0", PWM_SYSFS_PATH, pwm);
    if (ret < 0)
        return ret;
    return read_sysfs_posint("duty_cycle", path);
}

int luckfox_pwm_set_period(uint32_t pwm, uint32_t period)
{
    char path[64];
    int ret;

    ret = sprintf(path, "%s/pwmchip%d/pwm0", PWM_SYSFS_PATH, pwm);
    if (ret < 0)
        return ret;
    return write_sysfs_int("period", path, period);
}

int luckfox_pwm_get_period(uint32_t pwm)
{
    char path[64];
    int ret;

    ret = sprintf(path, "%s/pwmchip%d/pwm0", PWM_SYSFS_PATH, pwm);
    if (ret < 0)
        return ret;
    return read_sysfs_posint("period", path);
}

int luckfox_pwm_set_polarity(uint32_t pwm, enum pwm_polarity polarity)
{
    char path[64];
    int ret;

    if (polarity != PWM_POLARITY_INVERSED &&
        polarity != PWM_POLARITY_NORMAL)
        return -EINVAL;

    ret = sprintf(path, "%s/pwmchip%d/pwm0", PWM_SYSFS_PATH, pwm);
    if (ret < 0)
        return ret;

    return write_sysfs_string_and_verify("polarity", path,
                        (polarity == PWM_POLARITY_NORMAL) ?
                        "normal" : "inversed");
}

int luckfox_pwm_get_polarity(uint32_t pwm)
{
    char path[64];
    char string[15];
    int ret;

    ret = sprintf(path, "%s/pwmchip%d/pwm0", PWM_SYSFS_PATH, pwm);
    if (ret < 0)
        return ret;

    ret = read_sysfs_string("polarity", path, string);
    if (ret < 0)
        return ret;

    return (memcmp(string, "inversed", sizeof("inversed"))) ?
        PWM_POLARITY_NORMAL : PWM_POLARITY_INVERSED;
}

int luckfox_pwm_set_enable(uint32_t pwm, bool enabled)
{
    char path[64];
    int ret;

    ret = sprintf(path, "%s/pwmchip%d/pwm0", PWM_SYSFS_PATH, pwm);
    if (ret < 0)
        return ret;

    return write_sysfs_int_and_verify("enable", path, (uint32_t)enabled);
}

int luckfox_pwm_get_enable(uint32_t pwm)
{
    char path[64];
    int ret;

    ret = sprintf(path, "%s/pwmchip%d/pwm0", PWM_SYSFS_PATH, pwm);
    if (ret < 0)
        return ret;
    return read_sysfs_posint("enable", path);
}

int luckfox_pwm_init(uint32_t pwm, uint32_t period, uint32_t duty,
            enum pwm_polarity polarity)
{
    int ret;
    char path[64];

    if (polarity != PWM_POLARITY_INVERSED &&
        polarity != PWM_POLARITY_NORMAL)
        return -EINVAL;

    ret = sprintf(path, "%s/pwmchip%d/pwm0", PWM_SYSFS_PATH, pwm);
    if (ret < 0)
        return ret;

    ret = luckfox_pwm_export(pwm);
    if (ret < 0)
        return ret;

    ret = write_sysfs_int("period", path, period);
    if (ret < 0)
        goto out;

    ret = write_sysfs_int("duty_cycle", path, duty);
    if (ret < 0)
        goto out;

    ret = write_sysfs_string_and_verify("polarity", path,
                        (polarity == PWM_POLARITY_NORMAL) ?
                        "normal" : "inversed");
    if (ret < 0)
        goto out;
    return 0;
out:
    luckfox_pwm_unexport(pwm);
    return ret;
}

int luckfox_pwm_deinit(uint32_t pwm) {
    int ret;

    ret = luckfox_pwm_unexport(pwm);
    if (ret < 0) {
        printf("luckfox_pwm_unexport fail\n");
        return ret;
    }
    return 0;
}


