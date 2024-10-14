/*
 * Copyright (c) 2022 rockchip
 *
 */
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
