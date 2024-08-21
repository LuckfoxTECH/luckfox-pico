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
#define LOG_TAG "rga_allocator_cma_demo"

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

#include "utils.h"
#include "dma_alloc.h"

#define LOCAL_FILE_PATH "/data"

int main(void) {
    int ret = 0;
    int64_t ts;
    int src_width, src_height, src_format;
    int dst_width, dst_height, dst_format;
    int src_buf_size, dst_buf_size;
    char *src_buf, *dst_buf;
    int src_dma_fd, dst_dma_fd;
    rga_buffer_t src = {};
    rga_buffer_t dst = {};
    im_rect src_rect = {};
    im_rect dst_rect = {};
    rga_buffer_handle_t src_handle, dst_handle;

    src_width = 1280;
    src_height = 720;
    src_format = RK_FORMAT_RGBA_8888;

    dst_width = 1280;
    dst_height = 720;
    dst_format = RK_FORMAT_RGBA_8888;

    src_buf_size = src_width * src_height * get_bpp_from_format(src_format);
    dst_buf_size = dst_width * dst_height * get_bpp_from_format(dst_format);

    /* Allocate dma_buf from CMA, return dma_fd and virtual address */
    ret = dma_buf_alloc(RV1106_CMA_HEAP_PATH, src_buf_size, &src_dma_fd, (void **)&src_buf);
    if (ret < 0) {
        printf("alloc src CMA buffer failed!\n");
        return -1;
    }

    ret = dma_buf_alloc(RV1106_CMA_HEAP_PATH, dst_buf_size, &dst_dma_fd, (void **)&dst_buf);
    if (ret < 0) {
        printf("alloc dst CMA buffer failed!\n");
        dma_buf_free(src_buf_size, &src_dma_fd, src_buf);
        return -1;
    }

    ret = read_image_from_file(src_buf, LOCAL_FILE_PATH, src_width, src_height, src_format, 0);
    if (ret < 0) {
        printf ("open file %s so memset!\n", "fault");
        draw_rgba((char *)src_buf, src_width, src_height);
    }
    memset(dst_buf, 0x33, dst_buf_size);

    /*
     * Import the allocated dma_fd into RGA by calling
     * importbuffer_fd, and use the returned buffer_handle
     * to call RGA to process the image.
     */
    src_handle = importbuffer_fd(src_dma_fd, src_buf_size);
    dst_handle = importbuffer_fd(dst_dma_fd, dst_buf_size);
    if (src_handle == 0 || dst_handle == 0) {
        printf("import dma_fd error!\n");
        ret = -1;
        goto free_buf;
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

    printf("output [0x%x, 0x%x, 0x%x, 0x%x]\n", dst_buf[0], dst_buf[1], dst_buf[2], dst_buf[3]);
    write_image_to_file(dst_buf, LOCAL_FILE_PATH, dst_width, dst_height, dst_format, 0);

release_buffer:
    if (src_handle > 0)
        releasebuffer_handle(src_handle);
    if (dst_handle > 0)
        releasebuffer_handle(dst_handle);

free_buf:
    dma_buf_free(src_buf_size, &src_dma_fd, src_buf);
    dma_buf_free(dst_buf_size, &dst_dma_fd, dst_buf);

    return 0;
}
