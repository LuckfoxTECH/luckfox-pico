/*****************************************************************************
* | File        :   luckfox_system.c
* | Author      :   luckfox team
* | Function    :   system
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
#include "luckfox_system.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdbool.h>

#define POWER_STATE_PATH "/sys/power/state"

#define VENDOR_MAX_SIZE		1024
#define VENDOR_REQ_TAG		0x56524551
#define VENDOR_READ_IO		_IOW('v', 0x01, unsigned int)
#define VENDOR_WRITE_IO		_IOW('v', 0x02, unsigned int)

typedef		unsigned short	    uint16;
typedef		unsigned int	    uint32;
typedef		unsigned char	    uint8;

struct luckfox_vendor_req {
    uint32 tag;
    uint16 id;
    uint16 len;
    uint8 data[1024];
};


int luckfox_chip_id_get(char *chipid) {
    char cpuinfo[256] = {0};

    FILE *cpuinfo_fp = NULL;
    cpuinfo_fp = fopen("/proc/cpuinfo", "r");
    if(NULL == cpuinfo_fp) {
        printf("Failed to open /proc/cpuinfo\n");
        return -1;
    }

    while(!feof(cpuinfo_fp)) {
        memset(cpuinfo, 0, sizeof(cpuinfo));
        fgets(cpuinfo,sizeof(cpuinfo) - 1, cpuinfo_fp);
        if (strstr(cpuinfo, "Serial") != NULL) {
            char *p = strchr(cpuinfo, ':');
            if (p) {
                if (chipid)
                    strncpy(chipid, p + 2*sizeof(char), MAX_INFO_LEN);
                fclose(cpuinfo_fp);
                return 0;
            }
        }
    }
    printf("Get chipid fail\n");
    fclose(cpuinfo_fp);
    return -1;
}

int luckfox_vendor_read(int vendor_id, char *data, int size)
{
    int ret ;
    uint8 p_buf[sizeof(struct luckfox_vendor_req)]; /* malloc req buffer or used extern buffer */
    struct luckfox_vendor_req *req;

    req = (struct luckfox_vendor_req *)p_buf;
    memset(p_buf, 0, sizeof(struct luckfox_vendor_req));
    int sys_fd = open("/dev/vendor_storage", O_RDONLY);
    if(sys_fd < 0){
        fprintf(stderr, "vendor_storage open fail\n");
        return -1;
    }

    req->tag = VENDOR_REQ_TAG;
    req->id = vendor_id;
    req->len = VENDOR_MAX_SIZE;

    ret = ioctl(sys_fd, VENDOR_READ_IO, req);
    close(sys_fd);
    if (ret) {
        fprintf(stderr, "vendor read error %d\n", ret);
        return -1;
    }

    if ( size < req->len ) {
        fprintf(stderr, "vendor storage: param size is lower then read size %d\n", strlen((const char *)req->data) );
        return -1;
    }

    memcpy(data, req->data, req->len);
    return 0;
}

int luckfox_vendor_write(int vendor_id, const char *data, int size)
{
    int ret ;
    uint8 p_buf[sizeof(struct luckfox_vendor_req)]; /* malloc req buffer or used extern buffer */
    struct luckfox_vendor_req *req;

    if (size > VENDOR_MAX_SIZE) {
        fprintf(stderr, "vendor storage input data overflow\n");
        return -1;
    }

    req = (struct luckfox_vendor_req *)p_buf;
    int sys_fd = open("/dev/vendor_storage",O_RDWR,0);
    if (sys_fd < 0) {
        fprintf(stderr, "vendor storage open fail\n");
        return -1;
    }

    req->tag = VENDOR_REQ_TAG;
    req->id = vendor_id;

    req->len = size;
    memcpy(req->data, data, req->len);

    ret = ioctl(sys_fd, VENDOR_WRITE_IO, req);
    close(sys_fd);
    if (ret) {
        fprintf(stderr, "vendor write error\n");
        return -1;
    }

    return 0;
}


int luckfox_system_reboot(void) {
    int ret;
    ret = system("reboot");
    if (ret) {
        printf("reboot fail\n");
        return -1;
    }
    return 0;
}
int luckfox_system_suspend(SUSPEND_TYPE type) {
    int ret = 0;
    FILE  *sysfsfp = NULL;

    sysfsfp = fopen(POWER_STATE_PATH, "w");
    if (!sysfsfp) {
        ret = -errno;
        fprintf(stderr, "Could not open %s\n", POWER_STATE_PATH);
        return -1;
    }

    switch(type) {
        case SUSPEND_FREEZE: {
            ret = fprintf(sysfsfp, "%s", "freeze");
            if (ret < 0) {
                printf("write freeze fail\n");
                fclose(sysfsfp);
                return -1;
            }
            break;
        }
        case SUSPEND_MEM: {
            FILE  *mem_fd = NULL;
            mem_fd = fopen("/sys/power/mem_sleep", "w");
            if (!mem_fd) {
                fprintf(stderr, "Could not open %s\n", "/sys/power/mem_sleep");
                fclose(sysfsfp);
                return -1;
            }
            ret = fprintf(mem_fd, "%s", "s2idle");
            if (ret < 0) {
                printf("write s2idle fail\n");
                fclose(mem_fd);
                fclose(sysfsfp);
                return -1;
            }
            ret = fprintf(sysfsfp, "%s", "mem");
            if (ret < 0) {
                printf("write mem fail\n");
                fclose(mem_fd);
                fclose(sysfsfp);
                return -1;
            }
            fclose(mem_fd);
            break;
        }
        default:
            printf("input SUSPEND_TYPE error\n");
    }
    if (fclose(sysfsfp)) {
        ret = -errno;
        return -1;
    }
    return 0;
}
int luckfox_system_shutdown(void) {
    int ret;
    ret = system("halt");
    if (ret) {
        printf("halt fail\n");
        return -1;
    }
    return 0;
}
