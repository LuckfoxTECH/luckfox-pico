/*
 * Copyright (c) 2022 rockchip
 *
 */

#ifndef __RK_SYSTEM_H__
#define __RK_SYSTEM_H__

#include <stdbool.h>
#include <stdint.h>

#define MAX_INFO_LEN    64

typedef enum {
    SUSPEND_FREEZE = 0,
    SUSPEND_MEM,
} SUSPEND_TYPE;

int rk_chip_id_get(char *chipid);

int rk_vendor_read(int vendor_id, char *data, int size);
int rk_vendor_write(int vendor_id, const char *data, int size);

int rk_system_reboot(void);
int rk_system_suspend(SUSPEND_TYPE type);
int rk_system_shutdown(void);


#define VENDOR_READ(VENDOR_ID, DATA) rk_vendor_read(VENDOR_ID, DATA, sizeof(DATA) / sizeof(DATA[0]))
#define VENDOR_WRITE(VENDOR_ID, DATA) rk_vendor_write(VENDOR_ID, DATA, sizeof(DATA) / sizeof(DATA[0]))

#endif //__RK_SYSTEM_H__

