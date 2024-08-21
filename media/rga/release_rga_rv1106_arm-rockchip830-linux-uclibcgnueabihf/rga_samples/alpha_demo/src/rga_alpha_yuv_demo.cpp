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
#define LOG_TAG "rga_alpha_rgba_yuv_demo"

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

#include "utils.h"

#define LOCAL_FILE_PATH "/data"

int main() {
    int ret = 0;
    int fg_width, fg_height, fg_format;
    int bg_width, bg_height, bg_format;
    int output_width, output_height, output_format;
    char *fg_buf, *bg_buf, *output_buf;
    int fg_buf_size, bg_buf_size, output_buf_size;

    int usage = 0;
    rga_buffer_t fg_img, bg_img, output_img;
    im_rect fg_rect, bg_rect, output_rect;
    rga_buffer_handle_t fg_handle, bg_handle, output_handle;

    memset(&fg_img, 0, sizeof(fg_img));
    memset(&bg_img, 0, sizeof(bg_img));
    memset(&output_img, 0, sizeof(output_img));
    memset(&fg_rect, 0, sizeof(fg_rect));
    memset(&bg_rect, 0, sizeof(bg_rect));
    memset(&output_rect, 0, sizeof(output_rect));

    fg_width = 1920;
    fg_height = 1080;
    fg_format = RK_FORMAT_YCbCr_420_SP;

    bg_width = 1280;
    bg_height = 720;
    bg_format = RK_FORMAT_RGBA_8888;

    output_width = 1920;
    output_height = 1080;
    output_format = RK_FORMAT_YCbCr_420_SP;

    fg_buf_size = fg_width * fg_height * get_bpp_from_format(fg_format);
    bg_buf_size = bg_width * bg_height * get_bpp_from_format(bg_format);
    output_buf_size = output_width * output_height * get_bpp_from_format(output_format);

    fg_buf = (char *)malloc(fg_buf_size);
    bg_buf = (char *)malloc(bg_buf_size);
    output_buf = (char *)malloc(output_buf_size);

    /* fill image data */
    if (0 != read_image_from_file(fg_buf, LOCAL_FILE_PATH, fg_width, fg_height, fg_format, 0)) {
        printf("foreground image read err\n");
        memset(fg_buf, 0xaa, fg_buf_size);
    }
    if (0 != read_image_from_file(bg_buf, LOCAL_FILE_PATH, bg_width, bg_height, bg_format, 0)) {
        printf("background image read err\n");
        memset(bg_buf, 0x66, bg_buf_size);
    }
    memset(output_buf, 0x80, output_buf_size);

    fg_handle = importbuffer_virtualaddr(fg_buf, fg_buf_size);
    bg_handle = importbuffer_virtualaddr(bg_buf, bg_buf_size);
    output_handle = importbuffer_virtualaddr(output_buf, output_buf_size);
    if (fg_handle == 0 || bg_handle == 0 || output_handle == 0) {
        printf("importbuffer failed!\n");
        goto release_buffer;
    }

    fg_img = wrapbuffer_handle(fg_handle, fg_width, fg_height, fg_format);
    bg_img = wrapbuffer_handle(bg_handle, bg_width, bg_height, bg_format);
    output_img = wrapbuffer_handle(output_handle, output_width, output_height, output_format);

    /*
     * Configure the blended rectangular area here.
     *      Here is intercepted from the foreground image (100, 200) as the starting point,
     *  and a rectangle with the same resolution as the background image is blended with
     *  the background image, and finally output to the output layer where (100, 200) is
     *  the starting point.
     *      fg_img => src_channel
     *      bg_img => src1_channel
     *      output_img => dst_channel
        ---------------------------        --------------      ---------------------------
        |         fg_img          |        |   bg_img/  |      |       output_img        |
        |     --------------      |        |   bg_rect  |      |     --------------      |
        |     |            |      |        |            |      |     |            |      |
        |     |   fg_rect  |      |    +   --------------  =>  |     | output_rect|      |
        |     |            |      |                            |     |(bg over fg)|      |
        |     --------------      |                            |     --------------      |
        |                         |                            |                         |
        ---------------------------                            ---------------------------
     */

    fg_rect.x = 100;
    fg_rect.y = 200;
    fg_rect.width = bg_width;
    fg_rect.height = bg_height;

    bg_rect.x = 0;
    bg_rect.y = 0;
    bg_rect.width = fg_rect.width;
    bg_rect.height = fg_rect.height;

    output_rect.x = fg_rect.x;
    output_rect.y = fg_rect.y;
    output_rect.width = fg_rect.width;
    output_rect.height = fg_rect.height;

    ret = imcheck_composite(fg_img, output_img, bg_img, fg_rect, output_rect, bg_rect);
    if (IM_STATUS_NOERROR != ret) {
        printf("%d, check error! %s", __LINE__, imStrError((IM_STATUS)ret));
        return -1;
    }

    usage = IM_SYNC | IM_ALPHA_BLEND_DST_OVER | IM_ALPHA_BLEND_PRE_MUL;

    ret = improcess(fg_img, output_img, bg_img, fg_rect, output_rect, bg_rect, -1, NULL, NULL, usage);
    if (ret == IM_STATUS_SUCCESS) {
        printf("%s running success!\n", LOG_TAG);
    } else {
        printf("%s running failed, %s\n", LOG_TAG, imStrError((IM_STATUS)ret));
        goto release_buffer;
    }

    write_image_to_file(output_buf, LOCAL_FILE_PATH, output_width, output_height, output_format, 0);

release_buffer:
    if (fg_handle)
        releasebuffer_handle(fg_handle);
    if (bg_handle)
        releasebuffer_handle(bg_handle);
    if (output_handle)
        releasebuffer_handle(output_handle);

    if (fg_buf)
        free(fg_buf);
    if (bg_buf)
        free(bg_buf);
    if (output_buf)
        free(output_buf);

    return ret;
}
