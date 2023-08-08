/*****************************************************************************
* | File        :   luckfox_watchdog.c
* | Author      :   luckfox team
* | Function    :   watchdog
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
#include "luckfox_watchdog.h"
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

int luckfox_watchdog_stop(void) {
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

int luckfox_watchdog_start(int timeval) {
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

int luckfox_watchdog_refresh(void) {
    int ret, dummy;

    ret = ioctl(watchdog_fd, WDIOC_KEEPALIVE, &dummy);
    if (ret) {
        printf("luckfox_watchdog_refresh fail\n");
        return -1;
    }
    return 0;
}

