/*
 * Copyright (c) 2022 rockchip
 *
 */
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include "rk_adc.h"
#include "iio.h"
#include "rk_sysfs.h"

int rk_adc_get_devnum(const char *name)
{
    return iio_find_type_by_name(name, IIO_TYPE_DEVICE);
}

int rk_adc_get_value(uint32_t dev_num, uint32_t channel)
{
    int ret;
    char path[64], channel_name[32];

    ret = sprintf(path, "%s/%s%d",
        IIO_SYSFS_PATH, IIO_TYPE_DEVICE, dev_num);
    if (ret < 0)
        return ret;

    ret = sprintf(channel_name, "in_voltage%d_raw", channel);
    if (ret < 0)
        return ret;

    return read_sysfs_posint(channel_name, path);
}

