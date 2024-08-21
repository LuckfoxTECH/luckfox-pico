/*
 * Copyright (C) 2022  Rockchip Electronics Co., Ltd.
 * Authors:
 *     YuQiaowei <cerf.yu@rock-chips.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_NDEBUG 0
#undef LOG_TAG
#define LOG_TAG "rga_copy_tile_demo"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <linux/stddef.h>

#include "RgaUtils.h"
#include "im2d.hpp"
#include "utils.h"

#define LOCAL_FILE_PATH "/data"

int main() {
    int ret = 0;
    int src_width, src_height, src_format;
    int dst_width, dst_height, dst_format;
    char *src_buf, *dst_buf, *tile_buf;
    int src_buf_size, dst_buf_size;

    rga_buffer_t src_img, dst_img, tile_img;
    rga_buffer_handle_t src_handle, dst_handle, tile_handle;

    memset(&src_img, 0, sizeof(src_img));
    memset(&dst_img, 0, sizeof(dst_img));

    src_width = 1280;
    src_height = 720;
    src_format = RK_FORMAT_YCbCr_420_SP;

    dst_width = 1280;
    dst_height = 720;
    dst_format = RK_FORMAT_YCbCr_420_SP;

    src_buf_size = src_width * src_height * get_bpp_from_format(src_format);
    dst_buf_size = dst_width * dst_height * get_bpp_from_format(dst_format);

    src_buf = (char *)malloc(src_buf_size);
    dst_buf = (char *)malloc(dst_buf_size);
    tile_buf = (char *)malloc(dst_buf_size);

    /* fill image data */
    if (0 != read_image_from_file(src_buf, LOCAL_FILE_PATH, src_width, src_height, src_format, 0)) {
        printf("src image read err\n");
        draw_YUV420(src_buf, src_width, src_height);
    }
    memset(dst_buf, 0x80, dst_buf_size);
    memset(tile_buf, 0x80, dst_buf_size);

    src_handle = importbuffer_virtualaddr(src_buf, src_buf_size);
    dst_handle = importbuffer_virtualaddr(dst_buf, dst_buf_size);
    tile_handle = importbuffer_virtualaddr(tile_buf, dst_buf_size);
    if (src_handle == 0 || dst_handle == 0) {
        printf("importbuffer failed!\n");
        goto release_buffer;
    }


    src_img = wrapbuffer_handle(src_handle, src_width, src_height, src_format);
    dst_img = wrapbuffer_handle(dst_handle, dst_width, dst_height, dst_format);
    tile_img = wrapbuffer_handle(tile_handle, dst_width, dst_height, dst_format);

    /*
     * Copy the src(raster) image to the tile image.
        --------------        --------------
        |            |        |            |
        |  src_image |   =>   | tile_image |
        |            |        |            |
        --------------        --------------
     */

    src_img.rd_mode = IM_RASTER_MODE;

    /* Select the required TILE mode. */
    tile_img.rd_mode = IM_TILE8x8_MODE;
    // tile_img.rd_mode = IM_TILE4x4_MODE;

    ret = imcopy(src_img, tile_img);
    if (ret == IM_STATUS_SUCCESS) {
        printf("%s raster -> tile8x8 running success!\n", LOG_TAG);
    } else {
        printf("%s raster -> tile8x8 running failed, %s\n", LOG_TAG, imStrError((IM_STATUS)ret));
        goto release_buffer;
    }

	printf("tile8x8 [0x%x, 0x%x, 0x%x, 0x%x]\n", tile_buf[0], tile_buf[1], tile_buf[2], tile_buf[3]);
    write_image_to_file(tile_buf, LOCAL_FILE_PATH, dst_width, dst_height, dst_format, 0);

    /*
     * Copy the tile image to the dst(raster) image.
        --------------        --------------
        |            |        |            |
        | tile_image |   =>   |  dst_image |
        |            |        |            |
        --------------        --------------
     */

    /* Select the required TILE mode. */
    tile_img.rd_mode = IM_TILE8x8_MODE;
    // tile_img.rd_mode = IM_TILE4x4_MODE;

    dst_img.rd_mode = IM_RASTER_MODE;

    ret = imcopy(tile_img, dst_img);
    if (ret == IM_STATUS_SUCCESS) {
        printf("%s tile8x8 -> raster running success!\n", LOG_TAG);
    } else {
        printf("%s tile8x8 -> raster running failed, %s\n", LOG_TAG, imStrError((IM_STATUS)ret));
        goto release_buffer;
    }

	printf("raster [0x%x, 0x%x, 0x%x, 0x%x]\n", dst_buf[0], dst_buf[1], dst_buf[2], dst_buf[3]);
    write_image_to_file(dst_buf, LOCAL_FILE_PATH, dst_width, dst_height, dst_format, 1);

release_buffer:
    if (src_handle)
        releasebuffer_handle(src_handle);
    if (dst_handle)
        releasebuffer_handle(dst_handle);
    if (tile_handle)
        releasebuffer_handle(tile_handle);

    if (src_buf)
        free(src_buf);
    if (dst_buf)
        free(dst_buf);
    if (tile_buf)
        free(tile_buf);

    return ret;
}
