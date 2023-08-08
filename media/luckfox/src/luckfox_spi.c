/*****************************************************************************
* | File        :   luckfox_spi.c
* | Author      :   luckfox team
* | Function    :   spi
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

#include "luckfox_spi.h"
uint32_t spi_max_speed = 500000;
uint8_t spi_bits = 8;
int luckfox_spi_init(char *dev, uint32_t mode, uint8_t bits, uint32_t speed)
{
   int ret;
   int fd;
   spi_max_speed = speed;
   spi_bits = bits;
   fd = open(dev, O_RDWR);
   if (fd < 0)
   {
      printf("open %s error......\r\n",dev);
      return -1;
   }

   // Set SPI mode
   ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
   if (ret == -1)
   {
      printf("SPI_IOC_RD_MODE error......\n ");
      goto fd_close;
   }
   ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
   if (ret == -1)
   {
      printf("SPI_IOC_WR_MODE error......\n ");
      goto fd_close;
   }

   // Set the length of SPI communication
   ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
   if (ret == -1)
   {
      printf("SPI_IOC_RD_BITS_PER_WORD error......\n ");
      goto fd_close;
   }
   ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
   if (ret == -1)
   {
      printf("SPI_IOC_WR_BITS_PER_WORD error......\n ");
      goto fd_close;
   }

   // Set the maximum working frequency of SPI
   ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
   if (ret == -1)
   {
      printf("SPI_IOC_WR_MAX_SPEED_HZ error......\n ");
      goto fd_close;
   }
   ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
   if (ret == -1)
   {
      printf("SPI_IOC_RD_MAX_SPEED_HZ error......\n ");
      goto fd_close;
   }

   printf("spi mode: 0x%x\n", mode);
   printf("bits per word: %d\n", bits);
   printf("max speed: %d Hz (%d KHz)\n", speed, speed / 1000);

   return fd;

fd_close:

   close(fd);
   return -1;
}
int luckfox_spi_deinit(int fd)
{
   close(fd);
}
int luckfox_spi_transfer(int fd, uint8_t const *tx, uint8_t const *rx, size_t len, uint16_t delay)
{
   int ret;

   struct spi_ioc_transfer tr = {
       .tx_buf = (unsigned long)tx,
       .rx_buf = (unsigned long)rx,
       .len = len,
       .delay_usecs = delay,
       .speed_hz = spi_max_speed,
       .bits_per_word = spi_bits,
   };

   ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
   if (ret == -1)
   {
      return -1;
   }

   return 0;
}
