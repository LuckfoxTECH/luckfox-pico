/*****************************************************************************
* | File        :   luckfox_adc.c
* | Author      :   luckfox team
* | Function    :   adc
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
#include <sys/types.h>
#include "luckfox_adc.h"
#include "iio.h"
#include "luckfox_sysfs.h"

int luckfox_adc_get_devnum(const char *name)
{
    return iio_find_type_by_name(name, IIO_TYPE_DEVICE);
}

int luckfox_adc_get_value(uint32_t dev_num, uint32_t channel)
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

