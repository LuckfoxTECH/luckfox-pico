/*
 * Copyright (c) 2022 rockchip
 *
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "rk_pwm.h"
#include "rk_sysfs.h"

#define PWM_SYSFS_PATH "/sys/class/pwm"

int rk_pwm_export(uint32_t pwm)
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

int rk_pwm_unexport(uint32_t pwm)
{
    char path[64];
    int ret;

    ret = sprintf(path, "%s/pwmchip%d", PWM_SYSFS_PATH, pwm);
    if (ret < 0)
        return ret;
    return write_sysfs_int("unexport", path, 0);
}

int rk_pwm_set_duty(uint32_t pwm, uint32_t duty)
{
    char path[64];
    int ret;

    ret = sprintf(path, "%s/pwmchip%d/pwm0", PWM_SYSFS_PATH, pwm);
    if (ret < 0)
        return ret;
    return write_sysfs_int("duty_cycle", path, duty);
}

int rk_pwm_get_duty(uint32_t pwm)
{
    char path[64];
    int ret;

    ret = sprintf(path, "%s/pwmchip%d/pwm0", PWM_SYSFS_PATH, pwm);
    if (ret < 0)
        return ret;
    return read_sysfs_posint("duty_cycle", path);
}

int rk_pwm_set_period(uint32_t pwm, uint32_t period)
{
    char path[64];
    int ret;

    ret = sprintf(path, "%s/pwmchip%d/pwm0", PWM_SYSFS_PATH, pwm);
    if (ret < 0)
        return ret;
    return write_sysfs_int("period", path, period);
}

int rk_pwm_get_period(uint32_t pwm)
{
    char path[64];
    int ret;

    ret = sprintf(path, "%s/pwmchip%d/pwm0", PWM_SYSFS_PATH, pwm);
    if (ret < 0)
        return ret;
    return read_sysfs_posint("period", path);
}

int rk_pwm_set_polarity(uint32_t pwm, enum pwm_polarity polarity)
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

int rk_pwm_get_polarity(uint32_t pwm)
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

int rk_pwm_set_enable(uint32_t pwm, bool enabled)
{
    char path[64];
    int ret;

    ret = sprintf(path, "%s/pwmchip%d/pwm0", PWM_SYSFS_PATH, pwm);
    if (ret < 0)
        return ret;

    return write_sysfs_int_and_verify("enable", path, (uint32_t)enabled);
}

int rk_pwm_get_enable(uint32_t pwm)
{
    char path[64];
    int ret;

    ret = sprintf(path, "%s/pwmchip%d/pwm0", PWM_SYSFS_PATH, pwm);
    if (ret < 0)
        return ret;
    return read_sysfs_posint("enable", path);
}

int rk_pwm_init(uint32_t pwm, uint32_t period, uint32_t duty,
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

    ret = rk_pwm_export(pwm);
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
    rk_pwm_unexport(pwm);
    return ret;
}

int rk_pwm_deinit(uint32_t pwm) {
    int ret;

    ret = rk_pwm_unexport(pwm);
    if (ret < 0) {
        printf("rk_pwm_unexport fail\n");
        return ret;
    }
    return 0;
}


