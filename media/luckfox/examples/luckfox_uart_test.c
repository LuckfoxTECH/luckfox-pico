/*****************************************************************************
* | File        :   luckfox_uart_test.c
* | Author      :   luckfox team
* | Function    :   uart callback test
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
#include <math.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h> //signal()
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h> //exit()
#include <string.h>
#include "luckfox_uart.h"

int uart_fd = 0;
char w_data[] = "Waveshare Hello World";
char r_data[256];

void Delay_ms(uint32_t xms)
{
  uint32_t i;
  for (i = 0; i < xms; i++)
  {
    usleep(1000);
  }
}

void Handler(int signo)
{
  // SYSFS_UART_End(uart_fd);
  printf("\r\nHandler:exit\r\n");
  exit(0);
}

int main(int argc, char *argv[])
{
  int cnt = 0;
  int spn = 0;
  char dev[15];
  // Exception handling:ctrl + c
  signal(SIGINT, Handler);

  // need input serial port number
  if (argc != 2)
  {
    printf("please input serial port number!\r\n");
    printf("example: luckfox_uart_test -3\r\n");
    exit(1);
  }

  // get serial port number
  sscanf(argv[1], "%d", &spn);
  spn = fabs(spn);
  if (spn < 3 || spn > 4)
  {
    printf("error: bad serial port number\r\n");
    exit(1);
  }
  else
  {
    sprintf(dev, "/dev/ttyS%d", spn);
  }

  // init serial port
  luckfox_uart_begin(dev);
  luckfox_uart_write(w_data, strlen(w_data));
  Delay_ms(5);
  // callback
  while (1)
  {

    memset(r_data, '\0', 256);
    for (cnt = 0; cnt < 256; cnt++)
    {
      r_data[cnt] = luckfox_uart_readByte();

      if (r_data[cnt] == '\n')
      {
        r_data[cnt + 1] = '\0';
        break;
      }
    }
    printf("%s\r\n", r_data);
    luckfox_uart_write(r_data, cnt + 1);
    Delay_ms(5); // Waiting to send
  }

  return 0;
}
