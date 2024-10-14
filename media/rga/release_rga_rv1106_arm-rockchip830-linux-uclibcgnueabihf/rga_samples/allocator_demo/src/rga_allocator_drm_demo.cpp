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
#define LOG_TAG "rga_allocator_drm_demo"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstddef>
#include <cmath>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "im2d.h"
#include "RgaUtils.h"

#include "drm_alloc.h"
#include "utils.h"

#define LOCAL_FILE_PATH "/data"

class DrmObject {
public:
    int drm_buffer_fd;
    int drm_buffer_handle;
    size_t actual_size;
    uint8_t * drm_buf;
};

int main(void) {
    int ret = 0;
    int64_t ts;
    int src_width, src_height, src_format;
    int dst_width, dst_height, dst_format;
    int src_alloc_flags = 0, dst_alloc_flags = 0;
    rga_buffer_t src = {};
    rga_buffer_t dst = {};
    im_rect src_rect = {};
    im_rect dst_rect = {};
    DrmObject drm_src;
    DrmObject drm_dst;
    rga_buffer_handle_t src_handle, dst_handle;

    src_width = 1280;
    src_height = 720;
    src_format = RK_FORMAT_RGBA_8888;

    dst_width = 1280;
    dst_height = 720;
    dst_format = RK_FORMAT_RGBA_8888;

    /* Allocate drm_buf, return dma_fd and virtual address. */
    drm_src.drm_buf = (uint8_t *)drm_buf_alloc(src_width, src_height,
                                               get_bpp_from_format(src_format) * 8,
                                               &drm_src.drm_buffer_fd,
                                               &drm_src.drm_buffer_handle,
                                               &drm_src.actual_size,
                                               src_alloc_flags);

    drm_dst.drm_buf = (uint8_t *)drm_buf_alloc(dst_width, dst_height,
                                               get_bpp_from_format(dst_format) * 8,
                                               &drm_dst.drm_buffer_fd,
                                               &drm_dst.drm_buffer_handle,
                                               &drm_dst.actual_size,
                                               dst_alloc_flags);
    if (drm_src.drm_buf == NULL || drm_dst.drm_buf == NULL) {
        printf("alloc drm buffer failed!\n");
        return -1;
    }

    ret = read_image_from_file(drm_src.drm_buf, LOCAL_FILE_PATH, src_width, src_height, src_format, 0);
    if (ret < 0) {
        printf ("open file %s so memset!\n", "fault");
        draw_rgba((char *)drm_src.drm_buf, src_width, src_height);
    }
    memset(drm_dst.drm_buf, 0x33, drm_dst.actual_size);

    /*
     * Import the allocated dma_fd into RGA by calling
     * importbuffer_fd, and use the returned buffer_handle
     * to call RGA to process the image.
     */
    src_handle = importbuffer_fd(drm_src.drm_buffer_fd, drm_src.actual_size);
    dst_handle = importbuffer_fd(drm_dst.drm_buffer_fd, drm_dst.actual_size);
    if (src_handle == 0 || dst_handle == 0) {
        printf("import drm fd error!\n");
        ret = -1;
        goto drm_buf_destroy;
    }

    src = wrapbuffer_handle(src_handle, src_width, src_height, src_format);
    dst = wrapbuffer_handle(dst_handle, dst_width, dst_height, dst_format);

    ret = imcheck(src, dst, src_rect, dst_rect);
    if (IM_STATUS_NOERROR != ret) {
        printf("%d, check error! %s", __LINE__, imStrError((IM_STATUS)ret));
        goto release_buffer;
    }

    ts = get_cur_us();

    ret = imcopy(src, dst);
    if (ret == IM_STATUS_SUCCESS) {
        printf("%s running success! cost %ld us\n", LOG_TAG, get_cur_us() - ts);
    } else {
        printf("%s running failed, %s\n", LOG_TAG, imStrError((IM_STATUS)ret));
        goto release_buffer;
    }

    printf("output [0x%x, 0x%x, 0x%x, 0x%x]\n", drm_dst.drm_buf[0], drm_dst.drm_buf[1], drm_dst.drm_buf[2], drm_dst.drm_buf[3]);
    write_image_to_file(drm_dst.drm_buf, LOCAL_FILE_PATH, dst_width, dst_height, dst_format, 0);

release_buffer:
    if (src_handle > 0)
        releasebuffer_handle(src_handle);
    if (dst_handle > 0)
        releasebuffer_handle(dst_handle);

drm_buf_destroy:
    drm_buf_destroy(drm_src.drm_buffer_fd, drm_src.drm_buffer_handle, drm_src.drm_buf, drm_src.actual_size);
    drm_buf_destroy(drm_dst.drm_buffer_fd, drm_dst.drm_buffer_handle, drm_dst.drm_buf, drm_dst.actual_size);

    return 0;
}
