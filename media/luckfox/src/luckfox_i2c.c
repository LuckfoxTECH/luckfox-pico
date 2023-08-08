/*****************************************************************************
* | File        :   luckfox_i2c.c
* | Author      :   luckfox team
* | Function    :   i2c
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

#include "luckfox_i2c.h"

int luckfox_i2c_init(char *dev)
{
    int fd = open(dev, O_RDWR);
    if (fd < 0)
    {
        printf("fail to open %s \r\n", dev);
        exit(1);
    }
    return fd;
}
int luckfox_i2c_deinit(int fd)
{
    close(fd);
}

int luckfox_i2c_write(int fd, uint8_t addr, uint8_t reg, uint8_t val)
{
    // int retries;
    uint8_t data[2];

    data[0] = reg;
    data[1] = val;

    ioctl(fd, I2C_TENBIT, 0);

    if (ioctl(fd, I2C_SLAVE, addr) < 0)
    {
        printf("fail to set i2c device slave address!\n");
        close(fd);
        return -1;
    }

    ioctl(fd, I2C_RETRIES, 5);

    if (write(fd, data, 2) == 2)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int luckfox_i2c_read(int fd, uint8_t addr, uint8_t reg, uint8_t *val)
{
    // int retries;
    ioctl(fd, I2C_TENBIT, 0);
    if (ioctl(fd, I2C_SLAVE, addr) < 0)
    {
        printf("fail to set i2c device slave address!\n");
        close(fd);
        return -1;
    }
    ioctl(fd, I2C_RETRIES, 5);

    if (write(fd, &reg, 1) == 1)
    {
        if (read(fd, val, 1) == 1)
        {
            return 0;
        }
    }
    else
    {
        return -1;
    }
}

int luckfox_i2c_i2cdetect(int fd, uint8_t addr)
{
    ioctl(fd, I2C_TENBIT, 0);
    if (ioctl(fd, I2C_SLAVE, addr) < 0)
    {
        printf("fail to set i2c device slave address!\n");
        return -1;
    }  
    return 0;
}


