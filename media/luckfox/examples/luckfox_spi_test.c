/*****************************************************************************
* | File        :   luckfox_spi_test.c
* | Author      :   luckfox team
* | Function    :   spi callback test
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

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> //exit()
#include <signal.h> //signal()
#include "luckfox_spi.h"

#define spidev0_path "/dev/spidev0.0"

/* SPI send and receive buffer */
static unsigned char tx_buffer[100] = "hello the world !";
static unsigned char rx_buffer[100];

static int fd; // File descriptor for SPI control pins

static uint32_t mode = SPI_MODE_2; // Used to save SPI mode
static uint8_t bits = 8;           // Number of bits for receive and send data
static uint32_t speed = 500000;    // Transfer speed

int main(int argc, char *argv[])
{
   int ret;
   uint16_t delay = 10;
   fd = luckfox_spi_init(spidev0_path, mode, bits, speed);
   if (fd == -1)
   {
      printf("spi_init error\n");
      exit(-1);
   }

   ret = luckfox_spi_transfer(fd, tx_buffer, rx_buffer, sizeof(tx_buffer), delay);
   if (ret == -1)
   {
      printf("transfer error...\n");
      exit(-1);
   }

   /* Print tx_buffer and rx_buffer*/
   printf("tx_buffer: \n %s\n ", tx_buffer);
   printf("rx_buffer: \n %s\n ", rx_buffer);

   luckfox_spi_deinit(fd);
   return 0;
}
