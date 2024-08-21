#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>

int fd;
static void signal_handler(int signum)
{
    int ret = 0;
    char data_buf[256];
    printf("Into signal_handler\n");
    ret = read(fd, data_buf, 256);
    if(ret < 0) {
        printf("Read kernel-data fail\n");
    } else {
        printf("Read kernel-data: %s\n", data_buf);
    }
}

int main(int argc, char *argv[])
{
    int flag;
    char *filename = "/dev/aic_cdev";
    fd = open(filename, O_RDWR);
    if(fd < 0) {
        printf("Can't open cdev-file %s\r\n", filename);
        return -1;
    }
    printf("Open cdev: [%s]\r\n", filename);

    signal(SIGIO, signal_handler);

    fcntl(fd, F_SETOWN, getpid());
    flag = fcntl(fd, F_GETFD);
    fcntl(fd, F_SETFL, flag|FASYNC);

    while(1)
       sleep(2);
    return 0;
}

