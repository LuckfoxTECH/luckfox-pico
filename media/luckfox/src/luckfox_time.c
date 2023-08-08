/*****************************************************************************
* | File        :   luckfox_time.c
* | Author      :   luckfox team
* | Function    :   RTC 
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
#include "luckfox_time.h"
#include <string.h>
#include <errno.h>
#include <linux/rtc.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>

#define __set_time(_tv)		settimeofday(_tv, NULL)


int luckfox_system_get_time(struct tm *time) {
    int retval,fd;
    struct rtc_time rtc_tm;

    fd=open("/dev/rtc", O_RDONLY);
    if(fd == -1) {
        perror("error open /dev/rtc");
        return -1;
    }

    retval=ioctl(fd, RTC_RD_TIME, &rtc_tm);
    if(retval == -1) {
        perror("error RTC_RD_TIME ioctl");
        close(fd);
        return -1;
    }
    if (!time) {
        printf("time is NULL\n");
        close(fd);
        return -1;
    }
    time->tm_sec = rtc_tm.tm_sec;
    time->tm_min = rtc_tm.tm_min;
    time->tm_hour = rtc_tm.tm_hour;
    time->tm_mday = rtc_tm.tm_mday;
    time->tm_mon = rtc_tm.tm_mon;
    time->tm_year = rtc_tm.tm_year;
    close(fd);
    return 0;
}

int luckfox_system_set_time(struct tm *time) {

    int fd, ret;
    struct rtc_time mytime;
    struct tm time_sys;
    struct timeval tt;

    if (!time) {
        printf("time is NULL\n");
        return -1;
    }

    fd = open("/dev/rtc",O_RDWR);
    if(fd == -1) {
        perror("error open /dev/rtc");
        return -1;
    }

    mytime.tm_sec  = time->tm_sec ;
    mytime.tm_min  = time->tm_min ;
    mytime.tm_hour = time->tm_hour ;
    mytime.tm_mday = time->tm_mday ;
    mytime.tm_mon  = time->tm_mon -1;
    mytime.tm_year = time->tm_year - 1900;

    if (mytime.tm_year	<= 0) {
        printf("Year input wrong\n");
        return -1;
    };
    if ( (mytime.tm_mon <= 0) || (mytime.tm_mon >= 13) ) {
        printf("Month input wrong\n");
        return -1;
    };
    if ( (mytime.tm_mday <= 0) || (mytime.tm_mday >= 32) ) {
        printf("Day input wrong\n");
        return -1;
    };
    if ( (mytime.tm_hour < 0) || (mytime.tm_hour >= 24) ) {
        printf("Hour input wrong\n");
        return -1;
    };
    if ( (mytime.tm_min < 0) || (mytime.tm_min >= 60) ) {
        printf("Minute input wrong\n");
        return -1;
    };
    if ( (mytime.tm_sec < 0) || (mytime.tm_sec >= 60) ) {
        printf("Second input wrong\n");
        return -1;
    };

    time_sys.tm_sec  = time->tm_sec ;
    time_sys.tm_min  = time->tm_min ;
    time_sys.tm_hour = time->tm_hour ;
    time_sys.tm_mday = time->tm_mday ;
    time_sys.tm_mon  = time->tm_mon -1;
    time_sys.tm_year = time->tm_year - 1900;
    time_sys.tm_isdst = -1;
    tt.tv_sec = mktime(&time_sys);
    tt.tv_usec = 0;

    ret = ioctl(fd, RTC_SET_TIME, &mytime);
    if(ret == -1) {
        perror("error RTC_SET_TIME ioctl");
        close(fd);
        return -1;
    }

    ret =  settimeofday(&tt, NULL);
    if (ret) {
        if(errno == EPERM)
            printf("permission denied\n");
        else if (errno == EINVAL)
            printf("Parameter setting error\n");
    }

    close(fd);
    return 0;
}

int luckfox_system_get_alarm(struct tm *time) {
    int retval,fd;
    struct rtc_time rtc_tm;

    if (!time) {
        printf("time is NULL\n");
        return -1;
    }
    fd=open("/dev/rtc", O_RDONLY);
    if(fd == -1) {
        perror("error open /dev/rtc");
        return -1;
    }

    retval=ioctl(fd, RTC_ALM_READ, &rtc_tm);
    if(retval == -1) {
        perror("error RTC_ALM_READ ioctl");
        close(fd);
        return -1;
    }
    time->tm_sec = rtc_tm.tm_sec;
    time->tm_min = rtc_tm.tm_min;
    time->tm_hour = rtc_tm.tm_hour;
    close(fd);
    return 0;
}
int luckfox_system_set_alarm(struct tm *time) {
    int retval,fd;
    struct rtc_time rtc_tm;

    if (!time) {
        printf("time is NULL\n");
        return -1;
    }
    if ( (time->tm_hour < 0) || (time->tm_hour >= 24) ) {
        printf("Hour input wrong\n");
        return -1;
    };
    if ( (time->tm_min < 0) || (time->tm_min >= 60) ) {
        printf("Minute input wrong\n");
        return -1;
    };
    if ( (time->tm_sec < 0) || (time->tm_sec >= 60) ) {
        printf("Second input wrong\n");
        return -1;
    };
    rtc_tm.tm_sec  = time->tm_sec ;
    rtc_tm.tm_min  = time->tm_min ;
    rtc_tm.tm_hour = time->tm_hour ;
    fd = open("/dev/rtc", O_WRONLY);
    if(fd == -1) {
        perror("error open /dev/rtc");
        return -1;
    }

    retval = ioctl(fd, RTC_ALM_SET, &rtc_tm);
    if(retval == -1) {
        perror("error RTC_ALM_SET ioctl");
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}
int luckfox_system_enable_alarm(void) {
    int retval,fd;

    fd = open("/dev/rtc",O_RDWR);
    if(fd == -1) {
        perror("error open /dev/rtc");
        return -1;
    }
    retval = ioctl(fd, RTC_AIE_ON, 0);
    if(retval == -1) {
        perror("error RTC_AIE_ON ioctl");
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}
int luckfox_system_disable_alarm(void) {
    int retval,fd;

    fd = open("/dev/rtc",O_RDWR);
    if(fd == -1) {
        perror("error open /dev/rtc");
        return -1;
    }
    retval = ioctl(fd, RTC_AIE_OFF, 0);
    if(retval == -1) {
        perror("error RTC_AIE_OFF ioctl");
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}
int luckfox_system_wait_alarm(uint32_t wait_seconds) {
    int ret,fd;
    fd_set read_fdset;
    struct timeval timeout;

    fd = open("/dev/rtc",O_RDWR);
    if(fd == -1) {
        perror("error open /dev/rtc");
        return -1;
    }

    FD_ZERO(&read_fdset);
    FD_SET(fd,&read_fdset);

    if (wait_seconds <= 0) {
        printf("timeout setting error\n");
        goto fail;
    }
    timeout.tv_sec=wait_seconds;
    timeout.tv_usec=0;

    ret = select(fd+1,&read_fdset,NULL,NULL,&timeout);
    if (ret < 0) {
        if (errno==EINTR) {
            printf("EINTR occur\n");
        }
        goto fail;
    } else if(ret==0) {
        errno=ETIMEDOUT;
        printf("alarm timeout\n");
        goto fail;
    } else if(ret==1) {
        printf("alarm rang\n");
    }

    close(fd);
    return 0;

fail:
    close(fd);
    return -1;
}
