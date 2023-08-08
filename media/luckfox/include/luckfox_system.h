/*****************************************************************************
* | File        :   luckfox_system.h
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

#ifndef __LUCKFOX_SYSTEM_H__
#define __LUCKFOX_SYSTEM_H__

#include <stdbool.h>
#include <stdint.h>

#define MAX_INFO_LEN    64

typedef enum {
    SUSPEND_FREEZE = 0,
    SUSPEND_MEM,
} SUSPEND_TYPE;

int luckfox_chip_id_get(char *chipid);

int luckfox_vendor_read(int vendor_id, char *data, int size);
int luckfox_vendor_write(int vendor_id, const char *data, int size);

int luckfox_system_reboot(void);
int luckfox_system_suspend(SUSPEND_TYPE type);
int luckfox_system_shutdown(void);


#define VENDOR_READ(VENDOR_ID, DATA) luckfox_vendor_read(VENDOR_ID, DATA, sizeof(DATA) / sizeof(DATA[0]))
#define VENDOR_WRITE(VENDOR_ID, DATA) luckfox_vendor_write(VENDOR_ID, DATA, sizeof(DATA) / sizeof(DATA[0]))

#endif //__LUCKFOX_SYSTEM_H__

