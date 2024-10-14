/*
 * Copyright (c) 2022 rockchip
 *
 */

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "iio.h"
#include "rk_sysfs.h"

static int calc_digits(int num)
{
    int count = 0;

    while (num != 0) {
        num /= 10;
        count++;
    }

    return count;
}

int iio_find_type_by_name(const char *name, const char *type)
{
    const struct dirent *ent;
    int number, numstrlen, ret;

    FILE *namefp;
    DIR *dp;
    char thisname[IIO_MAX_NAME_LENGTH];
    char *filename;

    dp = opendir(IIO_SYSFS_PATH);
    if (!dp) {
        fprintf(stderr, "No industrialio devices available\n");
        return -ENODEV;
    }

    while (ent = readdir(dp), ent) {
        if (strcmp(ent->d_name, ".") != 0 &&
            strcmp(ent->d_name, "..") != 0 &&
            strlen(ent->d_name) > strlen(type) &&
            strncmp(ent->d_name, type, strlen(type)) == 0) {
            errno = 0;
            ret = sscanf(ent->d_name + strlen(type), "%d", &number);
            if (ret < 0) {
                ret = -errno;
                fprintf(stderr,
                    "failed to read element number\n");
                goto error_close_dir;
            } else if (ret != 1) {
                ret = -EIO;
                fprintf(stderr,
                    "failed to match element number\n");
                goto error_close_dir;
            }

            numstrlen = calc_digits(number);
            /* verify the next character is not a colon */
            if (strncmp(ent->d_name + strlen(type) + numstrlen,
                ":", 1) != 0) {
                filename = (char *)malloc(strlen(IIO_SYSFS_PATH)
                        + strlen(type) + numstrlen + 6);
                if (!filename) {
                    ret = -ENOMEM;
                    goto error_close_dir;
                }

                ret = sprintf(filename, "%s%s%d/name",
                          IIO_SYSFS_PATH, type, number);
                if (ret < 0) {
                    free(filename);
                    goto error_close_dir;
                }

                namefp = fopen(filename, "r");
                if (!namefp) {
                    free(filename);
                    continue;
                }

                free(filename);
                errno = 0;
                if (fscanf(namefp, "%s", thisname) != 1) {
                    ret = errno ? -errno : -ENODATA;
                    goto error_close_dir;
                }

                if (fclose(namefp)) {
                    ret = -errno;
                    goto error_close_dir;
                }

                if (strcmp(name, thisname) == 0) {
                    if (closedir(dp) == -1)
                        return -errno;

                    return number;
                }
            }
        }
    }
    if (closedir(dp) == -1)
    	return -errno;

    return -ENODEV;

error_close_dir:
    if (closedir(dp) == -1)
        perror("find_type_by_name(): Failed to close directory");

    return ret;
}
