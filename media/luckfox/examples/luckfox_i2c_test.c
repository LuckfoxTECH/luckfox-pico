/*****************************************************************************
* | File        :   luckfox_i2c_test.c
* | Author      :   luckfox team
* | Function    :   i2c scanf test
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
#include "luckfox_i2c.h"

int fd = 0;

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
  // System Exit
  printf("\r\nHandler:exit\r\n");
  luckfox_i2c_deinit(fd);

  exit(0);
}

int main(int argc, char *argv[])
{
  signal(SIGINT, Handler);
  fd = luckfox_i2c_init("/dev/i2c-3");
  printf("i2cdetect addr : ");
  for (int x = 0; x < 0x7f; x++)
  {
    if (luckfox_i2c_write(fd, x, 0, 0) == 0)
    {
      printf("0x%x,", x);
    }
  }
  printf("\r\n");
  return 0;
}
