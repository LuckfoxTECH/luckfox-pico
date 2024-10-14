#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define CFU_SLICE_LEN          52U

struct cfu_slice
{
    unsigned int address;
    unsigned char length;
    char payload[CFU_SLICE_LEN];
} __attribute__((__packed__));

void usage(void)
{
    printf("cfu_fw_gen addr name\n");
    exit(-1);
}

int main(int argc, char **argv)
{
    int addr;
    char *origin, *name;
    int origin_fd, cfu_fd;
    int reads, writes, slice_len;
    struct cfu_slice slice = {0, 0, {0}};
    int seq = 0;

    if (argc != 4)
        usage();

    origin = argv[1];
    addr = atoi(argv[2]);
    name = argv[3];

    origin_fd = open(origin, O_RDONLY);
    cfu_fd = open(name, O_CREAT | O_WRONLY | O_TRUNC, 0664);
    if (origin_fd < 0 || cfu_fd < 0)
    {
        printf("can not open the file: %s\n", strerror(errno));
        return -1;
    }

    while (1)
    {
        reads = read(origin_fd, slice.payload, CFU_SLICE_LEN);
        slice.address = addr + seq * CFU_SLICE_LEN;
        slice.length = (unsigned char)reads;
        slice_len = slice.length + 5; // 5 = address + len

        writes = write(cfu_fd, (const void *)&slice, slice_len);
        if (writes != slice_len)
        {
            printf("write cfu bin failed: %s\n", strerror(errno));
            return -1;
        }

        if (reads < CFU_SLICE_LEN)
        {
            break;
        }
        seq++;
    }

    close(origin_fd);
    close(cfu_fd);
    printf("Generating %s success\n", name);
}
