/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-05-20     chd.ma   the first version
 */

#include <rtthread.h>

#include "uart_api.h"

#if defined (RT_USING_POSIX)
#include <dfs_posix.h>
#include <dfs_select.h>
#if defined (RT_USING_POSIX_TERMIOS)
#include <termios.h>
#else
#error "TERMIOS need RT_USING_POSIX_TERMIOS"
#endif
#else
#error "POSIX poll/select need RT_USING_POSIX"
#endif


#ifdef RT_USING_POSIX_TERMIOS

#define JOINT(x,y) x##y
#define B(x) JOINT(B,x)
#define Default_baud_rate   115200
#define Default_parity      'n'
#define BUFFER_SIZE         64

struct termios_test_s
{
    int baud_rate;
    const char *dev;
};

//static struct termios_test_s term_param;

static int _check_baud_rate(int baud_rate)
{
#define BAUD_RATE(x) \
    {\
        if(x==baud_rate) \
        {\
            return JOINT(B,x);\
        }\
    }
    BAUD_RATE(110);
    BAUD_RATE(200);
    BAUD_RATE(300);
    BAUD_RATE(600);
    BAUD_RATE(1200);
    BAUD_RATE(1800);
    BAUD_RATE(2400);
    BAUD_RATE(4800);
    BAUD_RATE(9600);
    BAUD_RATE(19200);
    BAUD_RATE(38400);
    BAUD_RATE(57600);
    BAUD_RATE(115200);
    BAUD_RATE(230400);
    BAUD_RATE(460800);
    BAUD_RATE(921600);
    BAUD_RATE(1500000);
    BAUD_RATE(2000000);
    BAUD_RATE(3000000);
    BAUD_RATE(4000000);
    rt_kprintf("%d is not support,use default %d value.\n", baud_rate, Default_baud_rate);
    return B(Default_baud_rate);
}

int open_comm(const char *name)
{
    int fd;
    fd = open(name, O_RDWR | O_NOCTTY | O_NONBLOCK, 0);

    if (fd == -1)
    {
        rt_kprintf("Open %s fail.\n", name);
        return -1;
    }
    else
    {
        rt_kprintf("Open %s success,fd:%d\n", name, fd);
    }

    return fd;
}

void close_comm(int fd)
{
    if (fd != -1)
    {
        close(fd);
    }
}

void config_comm(int fd, int speed_baud_rate, char parity, int data_bits, int stop_bits)
{
    int valid_baud_rate = 0;
    struct termios new_tc;

    memset(&new_tc, 0x00, sizeof(struct termios));

    valid_baud_rate = _check_baud_rate(speed_baud_rate);

    new_tc.c_cflag |= (CLOCAL | CREAD);   /* Enable in default. */

    /*
    *Set baud rate. e.g B115200 is 115200 bauds.
    */
    cfsetispeed(&new_tc, valid_baud_rate);  /* input speed */
    cfsetospeed(&new_tc, valid_baud_rate);  /* output speed */

    /*
    *Set parity.
    */
    switch (parity)
    {
    case 'n':
    case 'N':
        new_tc.c_cflag &= ~PARENB;  /* Disable parity. */
        break;
    case 'o':
    case 'O':
        new_tc.c_cflag |= PARENB;   /* Enable parity. */
        new_tc.c_cflag |= PARODD;   /* Odd parity. */
        break;
    case 'e':
    case 'E':
        new_tc.c_cflag |= PARENB;   /* Enable parity. */
        new_tc.c_cflag &= ~PARODD;  /* even parity. */
        break;
    }

    /*
    *Set data bits.
    */
    new_tc.c_cflag &= ~CSIZE;
    switch (data_bits)
    {
    case 5:
        break;
    case 6:
        break;
    case 7:
        new_tc.c_cflag |= CS7;
        break;
    case 8:
        new_tc.c_cflag |= CS8;
        break;
    }

    /*
    *Set stop bits.
    */
    (stop_bits == 2) ? (new_tc.c_cflag |= CSTOPB) : (new_tc.c_cflag &= ~ CSTOPB);

    tcflush(fd, TCIFLUSH);
    /* new_tc.c_cc[VTIME] = 0; */
    /* new_tc.c_cc[VMIN] = 1; */

    /* Enable auto flow control */
    new_tc.c_cflag |= CRTSCTS;

    if (tcsetattr(fd, TCSANOW, &new_tc) != 0)
    {
        rt_kprintf("Set port config fail!\n");
    }
}

extern uint8_t g_readThreadAlive;
int recv_comm(int fd, unsigned char *buffer, rt_size_t size, struct timeval *timeout)
{
    struct timeval t;
    int ret = 0;
    rt_size_t drv_recved = 0;
    int recved = 0, need = size;
    int timeout_cnt = 0;
    unsigned char *c = RT_NULL;
    fd_set readSet;

    RT_ASSERT(RT_NULL != buffer);

    if (fd == -1)
    {
        return -1;
    }

    t.tv_sec = 0;
    t.tv_usec = 100000;

    if (RT_NULL == timeout)
    {
        /* Wait forever approximate, it's a large time. */
        timeout_cnt = 0xffffffff;
    }
    else
    {
        timeout_cnt = (timeout->tv_sec * 1000 * 1000 + timeout->tv_usec) / (t.tv_sec * 1000 * 1000 + t.tv_usec);
    }

    while (1)
    {
        FD_ZERO(&readSet);
        FD_SET(fd, &readSet);
        ret = select(fd + 1, &readSet, RT_NULL, RT_NULL, &t);
        if (g_readThreadAlive == 0)
        {
            rt_kprintf("exit select ret = %d\n", ret);
            return 0;
        }

        if (ret < 0)
        {
            rt_kprintf("select error %d\n", ret);
            break;
        }
        else if (ret == 0)
        {
            /* timeout */
            timeout_cnt--;

            if (timeout_cnt == 0)
            {
                rt_kprintf("need %d data in timeout %d ms,but only %d recved.\n",
                           size,
                           timeout->tv_sec * 1000 + timeout->tv_usec / 1000,
                           recved);

                recved = 0;

                break;
            }
        }
        else
        {
            if (FD_ISSET(fd, &readSet))
            {
                c = &buffer[size - need];
                ioctl(fd, FIONREAD, &drv_recved);
                //rt_kprintf("drv_recved=%d,need=%d\n",drv_recved,need);
                /* check poll and ioctl */
                RT_ASSERT(drv_recved != 0);

                drv_recved = (drv_recved > need ? need : drv_recved);
                recved = read(fd, c, drv_recved);
                if (recved != drv_recved)
                {
                    rt_kprintf("fatal error %s(%d).\n", __FUNCTION__, __LINE__);
                    RT_ASSERT(0);
                    recved = 0;
                    break;
                }

                need -= recved;

                if (need)
                {
                    continue;
                }
                else if (need == 0)
                {
                    recved = size;
                    break;
                }
                else
                {
                    rt_kprintf("fatal error %s(%d).\n", __FUNCTION__, __LINE__);
                    RT_ASSERT(0);
                }

            }
        }
    }

    return recved;
}

rt_size_t available_count_comm(int fd);
void dump_uart_buffer(int fd)
{
    uint8_t buf[1];
    rt_size_t count = available_count_comm(fd);

    rt_kprintf("\n****** available count %d ******\n", count);

    for (int i = 0; i < count; i++)
    {
        recv_comm(fd, buf, 1, 0);
        rt_kprintf("%02x ", buf[0] & 0x0ff);
    }
}

rt_size_t available_count_comm(int fd)
{
    rt_size_t count;
    ioctl(fd, FIONREAD, &count);
    return count;
}

int send_comm(int fd, const unsigned char *buffer, rt_size_t size)
{
    RT_ASSERT(RT_NULL != buffer);

    if (fd == -1)
    {
        return -1;
    }

    /* serial framework does not support poll out now */
    write(fd, buffer, size);

    return 0;
}

int flush_comm(int fd)
{
    if (fd == -1)
    {
        return -1;
    }
    tcflush(fd, TCIFLUSH);
    return 0;
}

//void set_comm_flow_control(int fd, int enable)
//{
//    struct termios new_tc;
//    memset(&new_tc, 0x00, sizeof(struct termios));
//
//    if (tcgetattr(fd, &new_tc) != 0)
//    {
//        rt_kprintf("get termios config fail!\n");
//    }
//
//    if (enable)
//        new_tc.c_cflag |= CRTSCTS;
//    else
//        new_tc.c_cflag &= ~CRTSCTS;
//
//    if (tcsetattr(fd, TCSANOW, &new_tc) != 0)
//    {
//        rt_kprintf("Set termios config fail!\n");
//    }
//}

#else
rt_size_t serial_write(rt_device_t dev, const void *buffer, rt_size_t size)
{
    rt_size_t res;
    if (dev == RT_NULL)
        return 0;

    res = rt_device_write(dev, 0, buffer, size);
    return res;
}

rt_size_t serial_read(rt_device_t dev, void *buffer, rt_size_t size)
{
    rt_size_t res;
    if (dev == RT_NULL)
        return 0;

    res = rt_device_read(dev, 0, buffer, size);
}
#endif

