/*****************************************************************************
* | File        :   ii0.h
* | Author      :   luckfox team
* | Function    :   iio
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
#ifndef _IIO_H_
#define _IIO_H_

#include <linux/iio/types.h>

#define IIO_MAX_NAME_LENGTH	30
#define IIO_SYSFS_PATH		"/sys/bus/iio/devices/"
#define IIO_TYPE_DEVICE		"iio:device"

/**
 * iio_find_type_by_name() - function to match top level types by name
 * @name: top level type instance name
 * @type: the type of top level instance being searched
 *
 * Returns the device number of a matched IIO device on success, otherwise a
 * negative error code.
 * Typical types this is used for are device and trigger.
 **/
int iio_find_type_by_name(const char *name, const char *type);

#endif
