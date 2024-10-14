/*
 * Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-04-17     Jair Wu      First version
 *
 */

#include "opus_parser.h"

#ifdef RT_USING_EXT_OPUS_DECODER

#undef DEBUG
#define DEBUG               0
#define OGG_BUFFER_IN       2048
#define FILE_SIZE_LIMIT     (2147483648UL)

uint32_t opus_get_duration(char *filename)
{
    FILE *fd;
    struct stat buf;
    uint32_t file_length;
    uint64_t pos = 0;
    uint64_t pos_h = 0;
    char *buffer_in;
    uint32_t read_bytes;
    ogg_sync_state oy;
    ogg_page og;
    int ret;

    if (stat(filename, &buf) == 0)
        file_length = buf.st_size;
    else
        file_length = 0;
    if (file_length > FILE_SIZE_LIMIT)
    {
        rt_kprintf("File too large %lu\n", file_length);
        return 0;
    }

    fd = fopen(filename, "rb");
    if (!fd)
    {
        rt_kprintf("[%s]open failed\n", filename);
        return 0;
    }

    if (file_length > 255 * 255 * 4)
    {
        /* A simple way to get total time of large file */
#if DEBUG
        rt_kprintf("Large file %lu, Skip to %lu\n", file_length, file_length - (255 * 255 * 2));
#endif
        fseek(fd, file_length - (255 * 255 * 2), SEEK_SET);
    }

    ogg_sync_init(&oy);
    while (1)
    {
        buffer_in = ogg_sync_buffer(&oy, OGG_BUFFER_IN);
        read_bytes = fread(buffer_in, 1, OGG_BUFFER_IN, fd);
        if (read_bytes <= 0)
            break;
        ogg_sync_wrote(&oy, read_bytes);
        ret = ogg_sync_pageout(&oy, &og);
        if (ret != 1)
        {
            /* need more data for page */
            continue;
        }
        /* Actually it is 64 bit */
        pos = *(uint32_t *)(og.header + 6);
        pos_h = *(uint32_t *)(og.header + 10);
        pos |= pos_h << 32;
    }
    ogg_sync_clear(&oy);
    fclose(fd);
    /* duration = pos / samplerate, samplerate is always 48k in opus */
#if DEBUG
    /*
     * @attention
     * Need to define RT_PRINTF_LONGLONG in src/kservice.c
     * */
    rt_kprintf("total length %llu, duration %llums.\n", pos, pos / 48);
#endif

    return pos / 48;
}

#endif  // RT_USING_LIB_OGG
