#include "rk_system.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
    //chipid
    int ret;
    char chipid[MAX_INFO_LEN] = {0};
    ret = rk_chip_id_get(chipid);
    if (ret)
        printf("rk_chip_id_get fail!\n");
    else
        printf("chipid : %s", chipid);

    //serial number
    char write[256] = "rockchip";
    char read[256] = {0};
    if (VENDOR_WRITE(0, write))
        printf("VENDOR_WRITE fail\n");
    if(VENDOR_READ(0, read))
        printf("VENDOR_READ fail\n");
    else
        printf("Vendor read data : %s\n", read);
    ret = rk_system_suspend(SUSPEND_FREEZE);
    return 0;
}

