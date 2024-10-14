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
#define LOG_TAG "rga_copy_splice_task_demo"

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
    int left_width, left_height, left_format;
    int right_width, right_height, right_format;
    int dst_width, dst_height, dst_format;
    char *left_buf, *right_buf, *dst_buf;
    int left_buf_size, right_buf_size, dst_buf_size;

    rga_buffer_t left_img, right_img, dst_img;
    im_rect left_rect, right_rect;
    rga_buffer_handle_t left_handle, right_handle, dst_handle;
    im_job_handle_t job_handle;

    memset(&left_img, 0, sizeof(left_img));
    memset(&right_img, 0, sizeof(right_img));
    memset(&dst_img, 0, sizeof(dst_img));
    memset(&left_rect, 0, sizeof(left_rect));
    memset(&right_rect, 0, sizeof(right_rect));

    left_width = 1280;
    left_height = 720;
    left_format = RK_FORMAT_RGBA_8888;

    right_width = 1280;
    right_height = 720;
    right_format = RK_FORMAT_RGBA_8888;

    dst_width = 2560;
    dst_height = 720;
    dst_format = RK_FORMAT_RGBA_8888;

    left_buf_size = left_width * left_height * get_bpp_from_format(left_format);
    right_buf_size = right_width * right_height * get_bpp_from_format(right_format);
    dst_buf_size = dst_width * dst_height * get_bpp_from_format(dst_format);

    left_buf = (char *)malloc(left_buf_size);
    right_buf = (char *)malloc(right_buf_size);
    dst_buf = (char *)malloc(dst_buf_size);

    /* fill image data */
    if (0 != read_image_from_file(left_buf, LOCAL_FILE_PATH, left_width, left_height, left_format, 0)) {
        printf("left image read err\n");
        memset(left_buf, 0xaa, left_buf_size);
    }
    if (0 != read_image_from_file(right_buf, LOCAL_FILE_PATH, right_width, right_height, right_format, 0)) {
        printf("right image read err\n");
        memset(left_buf, 0xbb, left_buf_size);
    }
    memset(dst_buf, 0x80, dst_buf_size);

    left_handle = importbuffer_virtualaddr(left_buf, left_buf_size);
    right_handle = importbuffer_virtualaddr(right_buf, left_buf_size);
    dst_handle = importbuffer_virtualaddr(dst_buf, dst_buf_size);
    if (left_handle == 0 || dst_handle == 0) {
        printf("importbuffer failed!\n");
        goto release_buffer;
    }

    left_img = wrapbuffer_handle(left_handle, left_width, left_height, left_format);
    right_img = wrapbuffer_handle(right_handle, right_width, right_height, right_format);
    dst_img = wrapbuffer_handle(dst_handle, dst_width, dst_height, dst_format);

    /*
     * Splicing the left and right images to output.
        ---------------------------
        |            |            |
        | left_rect  | right_rect |
        |            |            |
        ---------------------------
     */

    /*
     * 1). Create a job handle.
     */
    job_handle = imbeginJob();
    if (job_handle <= 0) {
        printf("job begin failed![%d], %s\n", job_handle, imStrError());
        goto release_buffer;
    }

    /*
     * 2). Add a task of copying the left image.
                                    dst_img
        --------------    ---------------------------
        |            |    |            |            |
        |  left_img  | => |  left_rect |            |
        |            |    |            |            |
        --------------    ---------------------------
     */
    left_rect.x = 0;
    left_rect.y = 0;
    left_rect.width = left_width;
    left_rect.height = left_height;

    ret = imcheck(left_img, dst_img, {}, left_rect);
    if (IM_STATUS_NOERROR != ret) {
        printf("%d, check error! %s", __LINE__, imStrError((IM_STATUS)ret));
        imcancelJob(job_handle);
        goto release_buffer;
    }

    ret = improcessTask(job_handle, left_img, dst_img, {}, {}, left_rect, {}, NULL, IM_SYNC);
    if (ret == IM_STATUS_SUCCESS) {
        printf("%s job[%d] add left task success!\n", LOG_TAG, job_handle);
    } else {
        printf("%s job[%d] add left task failed, %s\n", LOG_TAG, job_handle, imStrError((IM_STATUS)ret));
        imcancelJob(job_handle);
        goto release_buffer;
    }

    /*
     * 3). Add a task of copying the right image.
                                    dst_img
        --------------    ---------------------------
        |            |    |            |            |
        | right_img  | => |            | right_rect |
        |            |    |            |            |
        --------------    ---------------------------
     */
    right_rect.x = left_width;
    right_rect.y = 0;
    right_rect.width = right_width;
    right_rect.height = right_height;

    ret = imcheck(right_img, dst_img, {}, right_rect);
    if (IM_STATUS_NOERROR != ret) {
        printf("%d, check error! %s", __LINE__, imStrError((IM_STATUS)ret));
        imcancelJob(job_handle);
        goto release_buffer;
    }

    ret = improcessTask(job_handle, right_img, dst_img, {}, {}, right_rect, {}, NULL, IM_SYNC);
    if (ret == IM_STATUS_SUCCESS) {
        printf("%s job[%d] add right task success!\n", LOG_TAG, job_handle);
    } else {
        printf("%s job[%d] add right task failed, %s\n", LOG_TAG, job_handle, imStrError((IM_STATUS)ret));
        imcancelJob(job_handle);
        goto release_buffer;
    }

    /*
     * 4). Submit and wait for the job to complete.
     */
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
    if (left_handle)
        releasebuffer_handle(left_handle);
    if (right_handle)
        releasebuffer_handle(right_handle);
    if (dst_handle)
        releasebuffer_handle(dst_handle);

    if (left_buf)
        free(left_buf);
    if (right_buf)
        free(right_buf);
    if (dst_buf)
        free(dst_buf);

    return ret;
}
