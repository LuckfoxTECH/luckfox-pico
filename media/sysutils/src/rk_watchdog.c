/*
 * Copyright (c) 2022 rockchip
 *
 */
#include "rk_watchdog.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <getopt.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <pthread.h>
#include <linux/watchdog.h>

static int watchdog_fd = 0;
const char v = 'V';

int rk_watchdog_stop(void) {
    if (watchdog_fd > 0) {
        int ret,flags;

        ret = write(watchdog_fd, &v, 1);
        if (ret < 0)
            printf("Stopping watchdog ticks failed (%d)...\n", errno);

        flags = WDIOS_DISABLECARD;
        ret = ioctl(watchdog_fd, WDIOC_SETOPTIONS, &flags);
        if (!ret)
            printf("Watchdog card disabled.\n");
        else
            printf("WDIOS_DISABLECARD error '%s'\n", strerror(errno));
        close(watchdog_fd);
        watchdog_fd = 0;
    }
    return 0;
}

int rk_watchdog_start(int timeval) {
    int flags;
    int ret,timeout = timeval;

    if (!watchdog_fd) {
        watchdog_fd = open("/dev/watchdog", O_WRONLY);
        if (watchdog_fd == -1) {
           if (errno == ENOENT)
               printf("Watchdog device not enabled.\n");
           else if (errno == EACCES)
               printf("Run watchdog as root.\n");
           else
               printf("Watchdog device open failed %s\n",
                   strerror(errno));
           return -1;
        }

    }
    flags = WDIOS_ENABLECARD;
    ret = ioctl(watchdog_fd, WDIOC_SETOPTIONS, &flags);
    if (!ret) {
        printf("Watchdog card enabled.\n");
    } else {
        printf("WDIOS_ENABLECARD error '%s'\n", strerror(errno));
        goto fail;
    }
    if (timeout > 0) {
        ret = ioctl(watchdog_fd, WDIOC_SETTIMEOUT, &timeout);
        if (!ret)
           printf("Watchdog timeout set to %u seconds.\n", timeout);
        else
           printf("WDIOC_SETTIMEOUT error '%s'\n", strerror(errno));
    }
    return 0;

fail:
    ret = write(watchdog_fd, &v, 1);
    if (ret < 0)
        printf("Stopping watchdog ticks failed (%d)...\n", errno);
    close(watchdog_fd);
    watchdog_fd = 0;
    return -1;
}

int rk_watchdog_refresh(void) {
    int ret, dummy;

    ret = ioctl(watchdog_fd, WDIOC_KEEPALIVE, &dummy);
    if (ret) {
        printf("rk_watchdog_refresh fail\n");
        return -1;
    }
    return 0;
}

