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
#define LOG_TAG "rga_alpha_osd_demo"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstddef>
#include <cmath>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "RgaUtils.h"
#include "im2d.hpp"
#include "dma_alloc.h"
#include "utils.h"

#define LOCAL_FILE_PATH "/data"

int main() {
    int ret = 0;
    int block_witdh, block_height;
    int fg_width, fg_height, fg_format;
    int bg_width, bg_height, bg_format;
    char *fg_buf, *bg_buf;
    int fg_dma_fd, bg_dma_fd;
    int fg_buf_size, bg_buf_size;
    int block_size, block_count;

    int usage = 0;
    rga_buffer_t fg_img, bg_img;
    im_rect bg_rect;
    rga_buffer_handle_t fg_handle, bg_handle;
    im_osd_t osd_config;

    memset(&fg_img, 0, sizeof(fg_img));
    memset(&bg_img, 0, sizeof(bg_img));
    memset(&bg_rect, 0, sizeof(bg_rect));
    memset(&osd_config, 0, sizeof(osd_config));

    block_witdh = 64;
    block_height = 96;
    block_count = 6;

    fg_width = block_witdh;
    fg_height = block_height * block_count;
    fg_format = RK_FORMAT_RGBA_8888;

    bg_width = 1280;
    bg_height = 720;
    bg_format = RK_FORMAT_RGBA_8888;

    block_size = block_witdh * block_height * get_bpp_from_format(fg_format);
    fg_buf_size = fg_width * fg_height * get_bpp_from_format(fg_format);
    bg_buf_size = bg_width * bg_height * get_bpp_from_format(bg_format);

    /* Allocate dma_buf from CMA, return dma_fd and virtual address */
    ret = dma_buf_alloc(DMA_HEAP_UNCACHE_PATH, fg_buf_size, &fg_dma_fd, (void **)&fg_buf);
    if (ret < 0) {
        printf("alloc fg CMA buffer failed!\n");
        return -1;
    }

    ret = dma_buf_alloc(DMA_HEAP_UNCACHE_PATH, bg_buf_size, &bg_dma_fd, (void **)&bg_buf);
    if (ret < 0) {
        printf("alloc bg CMA buffer failed!\n");
        dma_buf_free(fg_buf_size, &fg_dma_fd, fg_buf);
        return -1;
    }

    /* fill image data */
    for (int i = 0; (i < block_count) && ((block_height * i) < fg_height); i++) {
        if (0 != read_image_from_file(fg_buf + i * block_size, LOCAL_FILE_PATH, block_witdh, block_height, fg_format, 0)) {
            printf("block image read err\n");
            memset(fg_buf, 0xaa, fg_buf_size);
        }
    }
    output_buf_data_to_file(fg_buf, fg_format, fg_width, fg_height, 0);

    if (0 != read_image_from_file(bg_buf, LOCAL_FILE_PATH, bg_width, bg_height, bg_format, 0)) {
        printf("background image read err\n");
        memset(bg_buf, 0x66, bg_buf_size);
    }

    fg_handle = importbuffer_fd(fg_dma_fd, fg_buf_size);
    bg_handle = importbuffer_fd(bg_dma_fd, bg_buf_size);
    if (fg_handle == 0 || bg_handle == 0) {
        printf("importbuffer failed!\n");
        goto release_buffer;
    }

    fg_img = wrapbuffer_handle(fg_handle, fg_width, fg_height, fg_format);
    bg_img = wrapbuffer_handle(bg_handle, bg_width, bg_height, bg_format);

    /*
     * Overlay multiple blocks on the background image and guide the color of
     * the blocks according to the external inversion flag.
        ----     ---------------------    ---------------------
        |  |     |                   |    | ----              |
        ----     |                   |    | |  |              |
        |  |     |                   |    | ----              |
        ----  +  |                   | => | |  |              |
        |  |     |                   |    | ----              |
        ----     |                   |    | |  |              |
                 |                   |    | ----              |
                 ---------------------    ---------------------

     */

    bg_rect.x = 100;
    bg_rect.y = 100;
    bg_rect.width = fg_width;
    bg_rect.height = fg_height;

    osd_config.osd_mode = IM_OSD_MODE_STATISTICS | IM_OSD_MODE_AUTO_INVERT;

    osd_config.block_parm.width_mode = IM_OSD_BLOCK_MODE_NORMAL;
    osd_config.block_parm.width = block_height;
    osd_config.block_parm.block_count = block_count;
    osd_config.block_parm.background_config = IM_OSD_BACKGROUND_DEFAULT_BRIGHT;
    osd_config.block_parm.direction = IM_OSD_MODE_VERTICAL;
    osd_config.block_parm.color_mode = IM_OSD_COLOR_PIXEL;

    osd_config.invert_config.invert_channel = IM_OSD_INVERT_CHANNEL_COLOR;
    osd_config.invert_config.flags_mode = IM_OSD_FLAGS_EXTERNAL;
    osd_config.invert_config.invert_flags = 0x000000000000002a;
    osd_config.invert_config.flags_index = 1;
    osd_config.invert_config.threash = 40;
    osd_config.invert_config.invert_mode = IM_OSD_INVERT_USE_SWAP;

    ret = imcheck(fg_img, bg_img, {}, bg_rect);
    if (IM_STATUS_NOERROR != ret) {
        printf("%d, check error! %s", __LINE__, imStrError((IM_STATUS)ret));
        return -1;
    }

    ret = imosd(fg_img, bg_img, bg_rect, &osd_config);
    printf("%s .... %s\n", LOG_TAG, imStrError(ret));
    if (ret != IM_STATUS_SUCCESS)
        goto release_buffer;

    write_image_to_file(bg_buf, LOCAL_FILE_PATH, bg_width, bg_height, bg_format, 0);

release_buffer:
    if (fg_handle)
        releasebuffer_handle(fg_handle);
    if (bg_handle)
        releasebuffer_handle(bg_handle);

    dma_buf_free(fg_buf_size, &fg_dma_fd, fg_buf);
    dma_buf_free(bg_buf_size, &bg_dma_fd, bg_buf);

    return ret;
}
