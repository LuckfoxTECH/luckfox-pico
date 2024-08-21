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
#define LOG_TAG "rga_fill_rectangle_task_array_demo"

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
    int dst_width, dst_height, dst_format;
    int dst_buf_size;
    char *dst_buf;
    int dst_dma_fd;
    rga_buffer_t dst = {};
    im_rect green_rect[4] = {};
    im_rect red_rect[4] = {};
    rga_buffer_handle_t dst_handle;
    im_job_handle_t job_handle;

    dst_width = 1920;
    dst_height = 1080;
    dst_format = RK_FORMAT_RGBA_8888;

    dst_buf_size = dst_width * dst_height * get_bpp_from_format(dst_format);

    /*
     * Allocate dma_buf within 4G from dma32_heap,
     * return dma_fd and virtual address.
     * ColorFill can only be used on buffers within 4G.
     */
    ret = dma_buf_alloc(DMA_HEAP_DMA32_UNCACHED_PATH, dst_buf_size, &dst_dma_fd, (void **)&dst_buf);
    if (ret < 0) {
        printf("alloc dma32_heap buffer failed!\n");
        return -1;
    }

    memset(dst_buf, 0x33, dst_buf_size);

    /*
     * Import the allocated dma_fd into RGA by calling
     * importbuffer_fd, and use the returned buffer_handle
     * to call RGA to process the image.
     */
    dst_handle = importbuffer_fd(dst_dma_fd, dst_buf_size);
    if (dst_handle == 0) {
        printf("import dma_fd error!\n");
        ret = -1;
        goto free_buf;
    }

    dst = wrapbuffer_handle(dst_handle, dst_width, dst_height, dst_format);

    /*
     * Fill one rectangle array with a green solid rectangle, and fill another
     * rectangle array with a red border of thickness 2.
           dst_image
        --------------
        | -------    |
        | |   --|--  |
        | ----|-- |  |
        |     -----  |
        --------------
     */

    /* Create a job handle. */
    job_handle = imbeginJob();
    if (job_handle <= 0) {
        printf("job begin failed![%d], %s\n", job_handle, imStrError());
        goto release_buffer;
    }

    /* Add a task that fills an array of green rectangle borders */
    green_rect[0] = {0, 0, 100, 100};
    green_rect[1] = {50, 50, 150, 150};
    green_rect[2] = {100, 100, 200, 200};
    green_rect[3] = {150, 150, 250, 250};

    for (int i = 0; i < 4; i++) {
        ret = imcheck({}, dst, {}, green_rect[i], IM_COLOR_FILL);
        if (IM_STATUS_NOERROR != ret) {
            printf("%d, check error! %s", __LINE__, imStrError((IM_STATUS)ret));
            imcancelJob(job_handle);
            goto release_buffer;
        }
    }

    ret = imrectangleTaskArray(job_handle, dst, green_rect, 4, 0xff00ff00, -1);
    if (ret == IM_STATUS_SUCCESS) {
        printf("%s job[%d] add fill task array success!\n", LOG_TAG, job_handle);
    } else {
        printf("%s job[%d] add fill task array failed, %s\n", LOG_TAG, job_handle, imStrError((IM_STATUS)ret));
        imcancelJob(job_handle);
        goto release_buffer;
    }

    /* Add a task that fills an array of green rectangle borders */
    red_rect[0] = {200, 200, 300, 300};
    red_rect[1] = {250, 250, 350, 350};
    red_rect[2] = {300, 300, 400, 400};
    red_rect[3] = {350, 350, 450, 450};

    for (int i = 0; i < 4; i++) {
        ret = imcheck({}, dst, {}, red_rect[i], IM_COLOR_FILL);
        if (IM_STATUS_NOERROR != ret) {
            printf("%d, check error! %s", __LINE__, imStrError((IM_STATUS)ret));
            imcancelJob(job_handle);
            goto release_buffer;
        }
    }

    ret = imrectangleTaskArray(job_handle, dst, red_rect, 4, 0xff0000ff, 2);
    if (ret == IM_STATUS_SUCCESS) {
        printf("%s job[%d] add fill task array success!\n", LOG_TAG, job_handle);
    } else {
        printf("%s job[%d] add fill task array failed, %s\n", LOG_TAG, job_handle, imStrError((IM_STATUS)ret));
        imcancelJob(job_handle);
        goto release_buffer;
    }

    /* Submit and wait for the job to complete. */
    ret = imendJob(job_handle);
    if (ret == IM_STATUS_SUCCESS) {
        printf("%s job[%d] running success!\n", LOG_TAG, job_handle);
    } else {
        printf("%s job[%d] running failed, %s\n", LOG_TAG, job_handle, imStrError((IM_STATUS)ret));
        goto release_buffer;
    }

    printf("output [0x%x, 0x%x, 0x%x, 0x%x]\n", dst_buf[0], dst_buf[1], dst_buf[2], dst_buf[3]);
    write_image_to_file(dst_buf, LOCAL_FILE_PATH, dst_width, dst_height, dst_format, 0);

release_buffer:
    if (dst_handle > 0)
        releasebuffer_handle(dst_handle);

free_buf:
    dma_buf_free(dst_buf_size, &dst_dma_fd, dst_buf);

    return 0;
}
