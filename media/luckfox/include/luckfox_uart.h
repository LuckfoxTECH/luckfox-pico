/*****************************************************************************
* | File        :   luckfox_uart.h
* | Author      :   luckfox team
* | Function    :   Read and write /dev/UART,  hardware UART
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
#ifndef __LUCKFOX_UART_
#define __LUCKFOX_UART_

#include <stdint.h>


#define LUCKFOX_UART_DEBUG 0
#if LUCKFOX_UART_DEBUG
#define LUCKFOX_UART_DEBUG(__info,...) printf("Debug: " __info,##__VA_ARGS__)
#else
#define LUCKFOX_UART_DEBUG(__info,...)
#endif

/**
 * Define UART attribute
**/
typedef struct UARTStruct {
    int fd; //
} HARDWARE_UART;

void luckfox_uart_begin(char *UART_device);
void luckfox_uart_end(void);
void luckfox_uart_setBaudrate(uint32_t Baudrate);

uint8_t luckfox_uart_write(const char * buf, uint32_t len);
uint8_t luckfox_uart_read(char* buf, uint32_t len);
uint8_t luckfox_uart_writeByte(uint8_t buf);
uint8_t luckfox_uart_readByte(void);

int luckfox_uart_Set(int databits, int stopbits, int parity);

#endif