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
#define LOG_TAG "rga_alpha_colorkey_demo"

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
    char *fg_buf, *bg_buf;
    int fg_buf_size, bg_buf_size;

    int usage = 0;
    rga_buffer_t fg_img, bg_img;
    rga_buffer_handle_t fg_handle, bg_handle;
    im_colorkey_range range;

    memset(&fg_img, 0, sizeof(fg_img));
    memset(&bg_img, 0, sizeof(bg_img));

    fg_width = 1280;
    fg_height = 720;
    fg_format = RK_FORMAT_RGBA_8888;

    bg_width = 1280;
    bg_height = 720;
    bg_format = RK_FORMAT_RGBA_8888;

    fg_buf_size = fg_width * fg_height * get_bpp_from_format(fg_format);
    bg_buf_size = bg_width * bg_height * get_bpp_from_format(bg_format);

    fg_buf = (char *)malloc(fg_buf_size);
    bg_buf = (char *)malloc(bg_buf_size);

    /* fill image data */
    if (0 != read_image_from_file(fg_buf, LOCAL_FILE_PATH, fg_width, fg_height, fg_format, 0)) {
        printf("foreground image read err\n");
        memset(fg_buf, 0xaa, fg_buf_size);
    }
    if (0 != read_image_from_file(bg_buf, LOCAL_FILE_PATH, bg_width, bg_height, bg_format, 1)) {
        printf("background image read err\n");
        memset(bg_buf, 0x66, bg_buf_size);
    }

    /* Fill color on foreground image. */
    for (int i = 0; i < fg_height; i++) {
        for (int j = 0; j < fg_width/8; j++) {
            fg_buf[(i*fg_width*4) + j*4 + 0] = 0x11;   //R
            fg_buf[(i*fg_width*4) + j*4 + 1] = 0x11;   //G
            fg_buf[(i*fg_width*4) + j*4 + 2] = 0x11;   //B
            fg_buf[(i*fg_width*4) + j*4 + 3] = 0xff;   //A
        }
        for (int j = fg_width/8; j < fg_width/4; j++) {
            fg_buf[(i*fg_width*4) + j*4 + 0] = 0x66;
            fg_buf[(i*fg_width*4) + j*4 + 1] = 0x66;
            fg_buf[(i*fg_width*4) + j*4 + 2] = 0x66;
            fg_buf[(i*fg_width*4) + j*4 + 3] = 0xff;
        }
        for (int j = fg_width/4; j < fg_width/8*3; j++) {
            fg_buf[(i*fg_width*4) + j*4 + 0] = 0xaa;
            fg_buf[(i*fg_width*4) + j*4 + 1] = 0xaa;
            fg_buf[(i*fg_width*4) + j*4 + 2] = 0xaa;
            fg_buf[(i*fg_width*4) + j*4 + 3] = 0xff;
        }
        for (int j = fg_width/8*3; j < fg_width/2; j++) {
            fg_buf[(i*fg_width*4) + j*4 + 0] = 0xff;
            fg_buf[(i*fg_width*4) + j*4 + 1] = 0xff;
            fg_buf[(i*fg_width*4) + j*4 + 2] = 0xff;
            fg_buf[(i*fg_width*4) + j*4 + 3] = 0xff;
        }
    }

    fg_handle = importbuffer_virtualaddr(fg_buf, fg_buf_size);
    bg_handle = importbuffer_virtualaddr(bg_buf, bg_buf_size);
    if (fg_handle == 0 || bg_handle == 0) {
        printf("importbuffer failed!\n");
        goto release_buffer;
    }

    fg_img = wrapbuffer_handle(fg_handle, fg_width, fg_height, fg_format);
    bg_img = wrapbuffer_handle(bg_handle, bg_width, bg_height, bg_format);

    /*
     * Overlays the background image after removing a specified
     * range of colors in the foreground image.
        --------------        --------------      --------------
        |            |        |            |      |            |
        |   fg_img   |    +   |   bg_img   |  =>  | fg over bg |
        |            |        |            |      |            |
        --------------        --------------      --------------
     */

    ret = imcheck(fg_img, bg_img, {}, {});
    if (IM_STATUS_NOERROR != ret) {
        printf("%d, check error! %s", __LINE__, imStrError((IM_STATUS)ret));
        return -1;
    }

    range.min = 0xFF666666;     //ABGR
    range.max = 0xFFaaaaaa;

    ret = imcolorkey(fg_img, bg_img, range, IM_ALPHA_COLORKEY_NORMAL);
    printf("%s .... %s\n", LOG_TAG, imStrError(ret));
    if (ret != IM_STATUS_SUCCESS)
        goto release_buffer;

    write_image_to_file(bg_buf, LOCAL_FILE_PATH, bg_width, bg_height, bg_format, 0);

release_buffer:
    if (fg_handle)
        releasebuffer_handle(fg_handle);
    if (bg_handle)
        releasebuffer_handle(bg_handle);

    if (fg_buf)
        free(fg_buf);
    if (bg_buf)
        free(bg_buf);

    return ret;
}
