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
#define LOG_TAG "rga_resize_uv_downsampling_demo"

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

#define USE_DMA_HEAP 1

#if USE_DMA_HEAP
#include "dma_alloc.h"
#endif

#define LOCAL_FILE_PATH "/data"

/*
 *   The current sample code is suitable for chips whose RGA hardware does not
 * support average downsampling, and uses Y400 format for UV downsampling.
 */

static IM_STATUS local_downsampling_imcvtcolor(rga_buffer_t &orig_src, rga_buffer_t &orig_dst,
                                               int src_fmt, int dst_fmt) {
    int ret = 0;
    rga_buffer_t src_img, dst_img;
    im_rect src_rect, dst_rect;

    memset(&src_img, 0, sizeof(src_img));
    memset(&dst_img, 0, sizeof(dst_img));
    memset(&src_rect, 0, sizeof(src_rect));
    memset(&dst_rect, 0, sizeof(dst_rect));

    src_img = wrapbuffer_handle(orig_src.handle,
                                orig_src.wstride, orig_src.hstride * get_bpp_from_format(orig_src.format),
                                RK_FORMAT_YCbCr_400);
    dst_img = wrapbuffer_handle(orig_dst.handle,
                                orig_dst.wstride, orig_dst.hstride * get_bpp_from_format(orig_dst.format),
                                RK_FORMAT_YCbCr_400);

    if (src_img.handle != dst_img.handle) {
        src_rect.x = 0;
        src_rect.y = 0;
        src_rect.width = orig_src.wstride;
        src_rect.height = orig_src.hstride;

        dst_rect.x = 0;
        dst_rect.y = 0;
        dst_rect.width = orig_dst.wstride;
        dst_rect.height = orig_dst.hstride;

        ret = improcess(src_img, dst_img, {}, src_rect, dst_rect, {}, IM_SYNC);
        if (ret == IM_STATUS_SUCCESS) {
            printf("%s Y channel running success!\n", LOG_TAG);
        } else {
            printf("%s Y channel running failed!\n", LOG_TAG);
            return (IM_STATUS)ret;
        }
    }

    /*
     * Scale uv data.
        --------------    --------------
        |            |    |            |
        |   y_data   |    |   y_data   |
        |            |    |            |
        |            |    |            |
        --------------    --------------
        |            |    |            |
        | uv422_data | => | uv420_data |
        |            |    --------------
        |            |
        --------------
     */

    src_rect.x = 0;
    src_rect.y = orig_src.hstride;
    src_rect.width = orig_src.wstride;
    src_rect.height = orig_src.hstride;

    dst_rect.x = 0;
    dst_rect.y = orig_dst.hstride;
    dst_rect.width = orig_dst.wstride;
    dst_rect.height = orig_dst.hstride / 2;

    ret = improcess(src_img, dst_img, {}, src_rect, dst_rect, {}, IM_SYNC);
    if (ret == IM_STATUS_SUCCESS) {
        printf("%s UV channel running success!\n", LOG_TAG);
    } else {
        printf("%s UV channel running failed!\n", LOG_TAG);
        return (IM_STATUS)ret;
    }

    return IM_STATUS_SUCCESS;
}

int main() {
    int ret = 0;
    int src_width, src_height, src_format;
    int dst_width, dst_height, dst_format;
    int src_dma_fd, dst_dma_fd;
    char *src_buf, *dst_buf;
    int src_buf_size, dst_buf_size;

    rga_buffer_t src_img, dst_img;
    im_rect src_rect, dst_rect;
    rga_buffer_handle_t src_handle, dst_handle;

    memset(&src_img, 0, sizeof(src_img));
    memset(&dst_img, 0, sizeof(dst_img));
    memset(&src_rect, 0, sizeof(src_rect));
    memset(&dst_rect, 0, sizeof(dst_rect));

    src_width = 1280;
    src_height = 720;
    src_format = RK_FORMAT_YCbCr_422_SP;

    dst_width = 1280;
    dst_height = 720;
    dst_format = RK_FORMAT_YCbCr_420_SP;

    src_buf_size = src_width * src_height * get_bpp_from_format(src_format);
    dst_buf_size = dst_width * dst_height * get_bpp_from_format(dst_format);

#if USE_DMA_HEAP
    ret = dma_buf_alloc(DMA_HEAP_DMA32_UNCACHED_PATH, src_buf_size, &src_dma_fd, (void **)&src_buf);
    if (ret < 0) {
        printf("alloc src dma32_heap buffer failed!\n");
        return -1;
    }

    ret = dma_buf_alloc(DMA_HEAP_DMA32_UNCACHED_PATH, dst_buf_size, &dst_dma_fd, (void **)&dst_buf);
    if (ret < 0) {
        printf("alloc dst dma32_heap buffer failed!\n");
        return -1;
    }
#else
    (void)(src_dma_fd);
    (void)(dst_dma_fd);

    src_buf = (char *)malloc(src_buf_size);
    dst_buf = (char *)malloc(dst_buf_size);
    if (src_buf == NULL || dst_buf == NULL) {
        printf("malloc failed!\n");
        return -1;
    }
#endif

    /* fill image data */
    if (0 != read_image_from_file(src_buf, LOCAL_FILE_PATH, src_width, src_height, src_format, 0)) {
        printf("src image read err\n");
        memset(src_buf, 0xaa, src_buf_size);
    }
    memset(dst_buf, 0x80, dst_buf_size);

    src_handle = importbuffer_virtualaddr(src_buf, src_buf_size);
    dst_handle = importbuffer_virtualaddr(dst_buf, dst_buf_size);
    if (src_handle == 0 || dst_handle == 0) {
        printf("importbuffer failed!\n");
        goto release_buffer;
    }

    src_img = wrapbuffer_handle(src_handle, src_width, src_height, src_format);
    dst_img = wrapbuffer_handle(dst_handle, dst_width, dst_height, dst_format);

    ret = imcheck(src_img, dst_img, {}, {});
    if (IM_STATUS_NOERROR != ret) {
        printf("%d, check error! %s", __LINE__, imStrError((IM_STATUS)ret));
        return -1;
    }

    ret = imcvtcolor(src_img, dst_img, RK_FORMAT_YCbCr_422_SP, RK_FORMAT_YCbCr_420_SP);
    if (ret == IM_STATUS_SUCCESS) {
        printf("%s RGA cvtcolor running success!\n", LOG_TAG);
    } else {
        printf("%s RGA cvtcolor running failed, %s\n", LOG_TAG, imStrError((IM_STATUS)ret));
        goto release_buffer;
    }

    printf("RGA output [0x%x, 0x%x, 0x%x, 0x%x]\n", dst_buf[0], dst_buf[1], dst_buf[2], dst_buf[3]);
    write_image_to_file(dst_buf, LOCAL_FILE_PATH, dst_width, dst_height, dst_format, 0);

    ret = local_downsampling_imcvtcolor(src_img, dst_img, RK_FORMAT_YCbCr_422_SP, RK_FORMAT_YCbCr_420_SP);
    if (ret == IM_STATUS_SUCCESS) {
        printf("%s local downsampling cvtcolor running success!\n", LOG_TAG);
    } else {
        printf("%s local downsampling cvtcolor running failed, %s\n", LOG_TAG, imStrError((IM_STATUS)ret));
        goto release_buffer;
    }

    printf("local output [0x%x, 0x%x, 0x%x, 0x%x]\n", src_buf[0], src_buf[1], src_buf[2], src_buf[3]);
    write_image_to_file(dst_buf, LOCAL_FILE_PATH, dst_width, dst_height, dst_format, 1);

release_buffer:
    if (src_handle)
        releasebuffer_handle(src_handle);
    if (dst_handle)
        releasebuffer_handle(dst_handle);

#if !USE_DMA_HEAP
    if(src_buf)
        free(src_buf);
    if (dst_buf)
        free(dst_buf);
#endif

    return ret;
}
