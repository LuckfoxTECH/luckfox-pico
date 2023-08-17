/*****************************************************************************
* | File        :   luckfox_adc_test.c
* | Author      :   luckfox team
* | Function    :   2ch adc read test
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
#include "luckfox_adc.h"

void Delay_ms(uint32_t xms)
{
  uint32_t i;
  for (i = 0; i < xms; i++)
  {
    usleep(1000);
  }
}

int main(int argc, char *argv[])
{
  int adc_read_value[2];
  float adc_voltage[2];
  while (1)
  {
    printf("***********ADC READ TEST***********\r\n");
    for (int cnt = 0; cnt < 2; cnt++)
    {
      adc_read_value[cnt] = luckfox_adc_get_value(luckfox_adc_get_devnum("ff3c0000.saradc"), cnt);
      adc_voltage[cnt] = adc_read_value[cnt] * 1.8 / 1024;
      printf("ADC %d:raw data = %d,voltage = %.2f \r\n", cnt, adc_read_value[cnt], adc_voltage[cnt]);
    }
    Delay_ms(1000);
  }
  return 0;
}
