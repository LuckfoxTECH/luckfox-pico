/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    termios_test.c
  * @author
  * @version V0.1
  * @date    10-Jun-2019
  * @brief   test uart send and receive
  *
  ******************************************************************************
  */

#include <rtthread.h>

#ifdef RT_USING_COMMON_TEST_UART

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>
#include <dfs_posix.h>
#include <dfs_select.h>
#include <termios.h>

#define JOINT(x, y) x##y
#define B(x) JOINT(B,x)
#define Default_baud_rate   115200
#define Default_parity      'n'
#define BUFFER_SIZE         64

struct termios_test_s
{
    int baud_rate;
    const char *dev;
};

static struct termios_test_s term_param;

static int _check_baud_rate(int baud_rate)
{
#define BAUD_RATE(x) \
    {\
        if(x==baud_rate) \
        {\
            rt_kprintf("%d baud rate\n",baud_rate);\
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

static int open_comm(const char *name)
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

static void close_comm(int fd)
{
    if (fd != -1)
    {
        close(fd);
    }
}

static void config_comm(int fd, int speed_baud_rate, char parity,
                        int data_bits, int stop_bits)
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
    (stop_bits == 2) ? (new_tc.c_cflag |= CSTOPB) : (new_tc.c_cflag &= ~CSTOPB);

    tcflush(fd, TCIFLUSH);
    /* new_tc.c_cc[VTIME] = 0; */
    /* new_tc.c_cc[VMIN] = 1; */

    /* Enable auto flow control */
    /* new_tc.c_cflag |= CRTSCTS; */

    if (tcsetattr(fd, TCSANOW, &new_tc) != 0)
    {
        rt_kprintf("Set port config fail!\n");
    }
}

static int recv_comm(int fd, unsigned char *buffer, rt_size_t size,
                     struct timeval *timeout)
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

static int send_comm(int fd, const unsigned char *buffer, rt_size_t size)
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

static int flush_comm(int fd)
{
    if (fd == -1)
    {
        return -1;
    }
    tcflush(fd, TCIFLUSH);
    return 0;
}

static void termios_loopback_entry(void *p)
{
    int len = 0, i = 0;
    int fd = -1;
    unsigned char *pBuf = RT_NULL;
    struct termios_test_s *pTerm = (struct termios_test_s *)p;

    if ((fd = open_comm(pTerm->dev)) == -1)
    {
        rt_kprintf("Check the device name...\n");
        return;
    }

    pBuf = (unsigned char *)rt_malloc(BUFFER_SIZE);
    RT_ASSERT(pBuf != RT_NULL);

    memset(pBuf, 0x55, BUFFER_SIZE);

    config_comm(fd, pTerm->baud_rate, Default_parity, 8, 1);

    flush_comm(fd);

    len = BUFFER_SIZE;
    rt_kprintf("send: 64 bytes 0x55..\n");
    send_comm(fd, pBuf, len);

    memset(pBuf, 0x0, BUFFER_SIZE);
    rt_kprintf("Block recv 64 bytes.\n");
    /* Block recv 64 bytes */
    len = recv_comm(fd, pBuf, len, RT_NULL);

    rt_kprintf("Recv:\n");
    for (i = 0; i < len; i++)
    {
        if ((i != 0) && (i % 8 == 0))
            rt_kprintf("\n");
        rt_kprintf("0x%02x ", pBuf[i]);
    }

    rt_kprintf("Termios test exit.\n");
    close_comm(fd);

    rt_free(pBuf);
    pBuf = RT_NULL;
}

static void termios_test_entry(void *p)
{
    int len = 0;
    int fd = -1;
    unsigned char *pBuf = RT_NULL;
    struct termios_test_s *pTerm = (struct termios_test_s *)p;

    if ((fd = open_comm(pTerm->dev)) == -1)
    {
        rt_kprintf("Check the device name...\n");
        return;
    }

    pBuf = (unsigned char *)rt_malloc(BUFFER_SIZE);
    RT_ASSERT(pBuf != RT_NULL);

    memset(pBuf, 0x00, BUFFER_SIZE);

    config_comm(fd, pTerm->baud_rate, Default_parity, 8, 1);

    flush_comm(fd);

    rt_kprintf("Block recv 10 bytes.\n");
    /* Block recv 10 bytes */
    len = recv_comm(fd, pBuf, 10, RT_NULL);

    rt_kprintf("Recv:%s\n", pBuf);

    send_comm(fd, pBuf, len);
    rt_kprintf("Termios test exit.\n");

    close_comm(fd);

    rt_free(pBuf);
    pBuf = RT_NULL;
}

static void termios_send_entry(void *p)
{
    int len = BUFFER_SIZE * 2;
    int fd = -1;
    unsigned char *pBuf = RT_NULL;
    struct termios_test_s *pTerm = (struct termios_test_s *)p;

    if ((fd = open_comm(pTerm->dev)) == -1)
    {
        rt_kprintf("Check the device name...\n");
        return;
    }

    pBuf = (unsigned char *)rt_malloc(len);
    RT_ASSERT(pBuf != RT_NULL);

    memset(pBuf, 0x55, len);

    config_comm(fd, pTerm->baud_rate, Default_parity, 8, 1);

    flush_comm(fd);

    rt_kprintf("send 0x55 in 128 times\n");
    send_comm(fd, pBuf, len);
    rt_kprintf("send finish\n");

    close_comm(fd);

    rt_free(pBuf);
    pBuf = RT_NULL;
}

static void termios_recv_entry(void *p)
{
    unsigned int len = BUFFER_SIZE, count = 0, total_err = 0;
    int fd = -1;
    int i = 0, err = 0;
    unsigned char *pBuf = RT_NULL;
    struct termios_test_s *pTerm = (struct termios_test_s *)p;

    if ((fd = open_comm(pTerm->dev)) == -1)
    {
        rt_kprintf("Check the device name...\n");
        return;
    }

    pBuf = (unsigned char *)rt_malloc(len);
    RT_ASSERT(pBuf != RT_NULL);

    config_comm(fd, pTerm->baud_rate, Default_parity, 8, 1);
    flush_comm(fd);

    while (1)
    {
        err = 0;
        memset(pBuf, 0x00, len);
        len = recv_comm(fd, pBuf, len, RT_NULL);
        count += len;

        for (i = 0; i < len; i++)
        {
            if (pBuf[i] != 0x55)
            {
                rt_kprintf("Recv:pBuf[%d] == %02x, it should be 0x55\n", i, pBuf[i]);
                err++;
            }
        }
        total_err += err;
        rt_kprintf("Recv:%d bytes, err:%d total:%d, total_err:%d\n", len, err, count, total_err);
    }

    close_comm(fd);

    rt_free(pBuf);
    pBuf = RT_NULL;
}

static int termios_test(int argc, char **argv)
{
    char *cmd = argv[1];
    rt_thread_t tid = RT_NULL;

    if (argc < 2)
    {
        rt_kprintf("Please input device name...\n");
        return -1;
    }

    term_param.dev = argv[2];
    term_param.baud_rate = ((argc >= 3) ? atoi(argv[3]) : Default_baud_rate);

    if (*cmd == 's')
        tid = rt_thread_create("termtest",
                               termios_send_entry, (void *)&term_param,
                               512, RT_THREAD_PRIORITY_MAX / 3, 20);
    else if (*cmd == 'r')
        tid = rt_thread_create("termtest",
                               termios_recv_entry, (void *)&term_param,
                               768, RT_THREAD_PRIORITY_MAX / 3, 20);
    else if (*cmd == 't')
        tid = rt_thread_create("termtest",
                               termios_test_entry, (void *)&term_param,
                               512, RT_THREAD_PRIORITY_MAX / 3, 20);
    else if (*cmd == 'l')
        tid = rt_thread_create("termtest",
                               termios_loopback_entry, (void *)&term_param,
                               512, RT_THREAD_PRIORITY_MAX / 3, 20);

    if (tid != RT_NULL)
        rt_thread_startup(tid);

    return 0;
}

#ifdef RT_USING_FINSH
#include <finsh.h>
#ifdef FINSH_USING_MSH
MSH_CMD_EXPORT_ALIAS(termios_test, termtest, e.g: termtest r / dev / uart4 115200);
#endif /* FINSH_USING_MSH */
#endif /* RT_USING_FINSH */

#endif /* RT_USING_COMMON_TEST_UART */
