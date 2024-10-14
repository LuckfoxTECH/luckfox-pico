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
#define LOG_TAG "rga_async_demo"

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
    int tmp_width, tmp_height, tmp_format;
    int dst_width, dst_height, dst_format;
    char *src_buf, *tmp_buf, *dst_buf;
    int src_buf_size, tmp_buf_size, dst_buf_size;
    int acquire_fence_fd, release_fence_fd;

    rga_buffer_t src_img, tmp_img, dst_img;
    rga_buffer_handle_t src_handle, tmp_handle, dst_handle;

    memset(&src_img, 0, sizeof(src_img));
    memset(&tmp_img, 0, sizeof(tmp_img));
    memset(&dst_img, 0, sizeof(dst_img));

    src_width = 1280;
    src_height = 720;
    src_format = RK_FORMAT_RGBA_8888;

    tmp_width = 1280;
    tmp_height = 720;
    tmp_format = RK_FORMAT_RGBA_8888;

    dst_width = 1280;
    dst_height = 720;
    dst_format = RK_FORMAT_RGBA_8888;

    src_buf_size = src_width * src_height * get_bpp_from_format(src_format);
    tmp_buf_size = tmp_width * tmp_height * get_bpp_from_format(tmp_format);
    dst_buf_size = dst_width * dst_height * get_bpp_from_format(dst_format);

    src_buf = (char *)malloc(src_buf_size);
    tmp_buf = (char *)malloc(tmp_buf_size);
    dst_buf = (char *)malloc(dst_buf_size);

    /* fill image data */
    if (0 != read_image_from_file(src_buf, LOCAL_FILE_PATH, src_width, src_height, src_format, 0)) {
        printf("src image image read err\n");
        draw_rgba(src_buf, src_width, src_height);
    }
    memset(tmp_buf, 0x40, tmp_buf_size);
    memset(dst_buf, 0x80, dst_buf_size);

    src_handle = importbuffer_virtualaddr(src_buf, src_buf_size);
    tmp_handle = importbuffer_virtualaddr(tmp_buf, tmp_buf_size);
    dst_handle = importbuffer_virtualaddr(dst_buf, dst_buf_size);
    if (src_handle == 0 || tmp_handle == 0 || dst_handle == 0) {
        printf("importbuffer failed!\n");
        goto release_buffer;
    }

    src_img = wrapbuffer_handle(src_handle, src_width, src_height, src_format);
    tmp_img = wrapbuffer_handle(tmp_handle, tmp_width, tmp_height, tmp_format);
    dst_img = wrapbuffer_handle(dst_handle, dst_width, dst_height, dst_format);

    /*
     * The image will be asynchronously copied from src_image to tmp, and
     * rely on fence to copy src_image to tmp_image and then copy to dst_image.
        --------------        --------------        --------------
        |            |        |            |        |            |
        |  src_image |   =>   |  tmp_image |   =>   |  dst_image |
        |            |        |            |        |            |
        --------------        --------------        --------------
     */

    /*
     * (1). Copy src_image to tmp_image and get release_fence.
     */
    ret = imcheck(src_img, tmp_img, {}, {});
    if (IM_STATUS_NOERROR != ret) {
        printf("%d, check error! %s", __LINE__, imStrError((IM_STATUS)ret));
        goto release_buffer;
    }

    release_fence_fd = -1;
    ret = imcopy(src_img, tmp_img, 0, &release_fence_fd);
    if (ret == IM_STATUS_SUCCESS) {
        printf("%s src->tmp running success!\n", LOG_TAG);
    } else {
        printf("%s src->tmp running failed, %s\n", LOG_TAG, imStrError((IM_STATUS)ret));
        goto release_buffer;
    }

    if (release_fence_fd < 0) {
        printf("get release_fence_fd failed!\n");
        goto release_buffer;
    }

    /*
     * (2). Copy tmp_image to dst_image, use the release_fence obtained in
     * operation 1 as acquire_fence, and get the release_fence of this task.
     */
    acquire_fence_fd = release_fence_fd;
    release_fence_fd = -1;

    ret = imcheck(tmp_img, dst_img, {}, {});
    if (IM_STATUS_NOERROR != ret) {
        printf("%d, check error! %s", __LINE__, imStrError((IM_STATUS)ret));
        goto release_buffer;
    }

    ret = improcess(tmp_img, dst_img, {}, {}, {}, {}, acquire_fence_fd, &release_fence_fd, NULL, IM_ASYNC);
    if (ret == IM_STATUS_SUCCESS) {
        printf("%s tmp->dst running success!\n", LOG_TAG);
    } else {
        printf("%s tmp->dst running failed, %s\n", LOG_TAG, imStrError((IM_STATUS)ret));
        goto release_buffer;
    }

    if (release_fence_fd < 0) {
        printf("get release_fence_fd failed!\n");
        goto release_buffer;
    }

    /*
     * (3). Will wait for the release_fence of the last operation, when it is
     * signed it indicates that all tasks are completed.
     */
    ret = imsync(release_fence_fd);
    if (ret == IM_STATUS_SUCCESS) {
        printf("%s waiting success!\n", LOG_TAG);
    } else {
        printf("%s waiting failed, %s\n", LOG_TAG, imStrError((IM_STATUS)ret));
        goto release_buffer;
    }

    printf("output [0x%x, 0x%x, 0x%x, 0x%x]\n", dst_buf[0], dst_buf[1], dst_buf[2], dst_buf[3]);
    write_image_to_file(dst_buf, LOCAL_FILE_PATH, dst_width, dst_height, dst_format, 0);

release_buffer:
    if (src_handle)
        releasebuffer_handle(src_handle);
    if (tmp_handle)
        releasebuffer_handle(tmp_handle);
    if (dst_handle)
        releasebuffer_handle(dst_handle);

    if (src_buf)
        free(src_buf);
    if (tmp_buf)
        free(tmp_buf);
    if (dst_buf)
        free(dst_buf);

    return ret;
}
