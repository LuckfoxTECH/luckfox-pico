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
#define LOG_TAG "rga_allocator_graphicbuffer_demo"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstddef>
#include <cmath>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "im2d.hpp"
#include "RgaUtils.h"

#include "utils.h"

/*
 *   In order to be compatible with different android versions,
 * some gralloc usage is defined here.
 *   The correct usage should be to refer to the corresponding header file:
 *   Android 12 and above: #include "hardware/gralloc_rockchip.h"
 *   Android 11 and below: #include "hardware/gralloc.h"
 */
#define GRALLOC_USAGE_PRIVATE_11                (1ULL << 56)
#define RK_GRALLOC_USAGE_WITHIN_4G              GRALLOC_USAGE_PRIVATE_11
#define RK_GRALLOC_USAGE_RGA_ACCESS             RK_GRALLOC_USAGE_WITHIN_4G

#define LOCAL_FILE_PATH "/data"

using namespace android;

int main(void) {
    int ret = 0;
    int64_t ts;
    int src_width, src_height, src_format;
    int dst_width, dst_height, dst_format;
    int src_buf_size, dst_buf_size;
    char *src_buf, *dst_buf;
    rga_buffer_t src = {};
    rga_buffer_t dst = {};
    im_rect src_rect = {};
    im_rect dst_rect = {};
    rga_buffer_handle_t src_handle, dst_handle;
    uint64_t src_gb_flags = 0, dst_gb_flags = 0;

    src_width = 1280;
    src_height = 720;
    src_format = HAL_PIXEL_FORMAT_RGBA_8888;

    dst_width = 1280;
    dst_height = 720;
    dst_format = HAL_PIXEL_FORMAT_RGBA_8888;

    src_buf_size = src_width * src_height * get_bpp_from_format(src_format);
    dst_buf_size = dst_width * dst_height * get_bpp_from_format(dst_format);

    /* allocate GraphicBuffer */
    src_gb_flags |= GRALLOC_USAGE_SW_WRITE_OFTEN | GRALLOC_USAGE_SW_READ_OFTEN;
    dst_gb_flags |= GRALLOC_USAGE_SW_WRITE_OFTEN | GRALLOC_USAGE_SW_READ_OFTEN;

    /* for CORE_RGA2 */
    src_gb_flags |= RK_GRALLOC_USAGE_WITHIN_4G;
    dst_gb_flags |= RK_GRALLOC_USAGE_WITHIN_4G;

    sp<GraphicBuffer> src_gb(new GraphicBuffer(src_width, src_height, src_format, 0, src_gb_flags));
    if (src_gb->initCheck()) {
        printf("src GraphicBuffer check error : %s\n",strerror(errno));
        return -1;
    }
    sp<GraphicBuffer> dst_gb(new GraphicBuffer(dst_width, dst_height, dst_format, 0, dst_gb_flags));
    if (dst_gb->initCheck()) {
        printf("dst GraphicBuffer check error : %s\n",strerror(errno));
        return -1;
    }

    ret = src_gb->lock(GRALLOC_USAGE_SW_WRITE_OFTEN, (void **)&src_buf);
    if (ret) {
        printf("lock buffer error : %s\n",strerror(errno));
        return -1;
    }

    ret = read_image_from_file(src_buf, LOCAL_FILE_PATH, src_width, src_height, src_format, 0);
    if (ret < 0) {
        printf ("open file %s so memset!\n", "fault");
        draw_rgba((char *)src_buf, src_width, src_height);
    }

    ret = src_gb->unlock();
	if (ret) {
        printf("unlock buffer error : %s\n",strerror(errno));
        return -1;
    }

    ret = dst_gb->lock(GRALLOC_USAGE_SW_WRITE_OFTEN, (void **)&dst_buf);
    if (ret) {
        printf("lock buffer error : %s\n",strerror(errno));
        return -1;
    }

    memset(dst_buf, 0x33, dst_buf_size);

    ret = dst_gb->unlock();
	if (ret) {
        printf("unlock buffer error : %s\n",strerror(errno));
        return -1;
    }

    /*
     * Import the allocated GraphicBuffer into RGA by calling
     * importbuffer_GraphicBuffer, and use the returned buffer_handle
     * to call RGA to process the image.
     */
    src_handle = importbuffer_GraphicBuffer(src_gb);
    dst_handle = importbuffer_GraphicBuffer(dst_gb);
    if (src_handle == 0 || dst_handle == 0) {
        printf("import GraphicBuffer error!\n");
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

    ret = dst_gb->lock(GRALLOC_USAGE_SW_READ_OFTEN, (void **)&dst_buf);
    if (ret) {
        printf("lock buffer error : %s\n",strerror(errno));
        return -1;
    }

    printf("output [0x%x, 0x%x, 0x%x, 0x%x]\n", dst_buf[0], dst_buf[1], dst_buf[2], dst_buf[3]);
    write_image_to_file(dst_buf, LOCAL_FILE_PATH, dst_width, dst_height, dst_format, 0);

    ret = dst_gb->unlock();
	if (ret) {
        printf("unlock buffer error : %s\n",strerror(errno));
        return -1;
    }

release_buffer:
    if (src_handle > 0)
        releasebuffer_handle(src_handle);
    if (dst_handle > 0)
        releasebuffer_handle(dst_handle);

free_buf:
    src_gb = NULL;
    dst_gb = NULL;

    return 0;
}
