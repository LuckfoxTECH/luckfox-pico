/*
 * Copyright (C) 2020 Rockchip Electronics Co., Ltd.
 * author: Zhihua Wang, hogan.wang@rock-chips.com
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL), available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <rga/RgaApi.h>
#include "display.h"
#include "rkdrm_display.h"
//#include "rkfacial.h"
#include "rga_control.h"
#include <pthread.h>

#define BUF_COUNT 3
#define USE_NV12

struct display {
    int fmt;
    int width;
    int height;
    int plane_type;
    struct drm_dev dev;
    struct drm_buf buf[BUF_COUNT];
    int buf_cnt;
    int rga_fmt;
};

struct display g_disp;

struct window {
    bo_t bo;
    int fd;
    int width;
    int height;
    int x;
    int y;
    int fmt;
};

struct window win0, win1, win2;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_t g_th;

static int drm_display_init(struct display *disp)
{
    int ret;
    if (drmInit(&disp->dev)) {
        fprintf(stderr, "drmInit: Failed\n");
        return -1;
    }

    for (int i = 0; i < disp->buf_cnt; i++) {
        ret = drmGetBuffer(disp->dev.drm_fd, disp->width, disp->height, disp->fmt, &disp->buf[i]);
        if (ret) {
            fprintf(stderr, "Alloc drm buffer failed, %d\n", i);
            return -1;
        }
    }

    return 0;
}

static void display_compose(void *srcp, int srcw, int srch, int fmt, int x, int y)
{
    rga_info_t src, dst;
    memset(&src, 0, sizeof(rga_info_t));
    src.fd = -1;
    src.virAddr = srcp;
    src.mmuFlag = 1;
    rga_set_rect(&src.rect, 0, 0, srcw, srch, srcw, srch, fmt);
    memset(&dst, 0, sizeof(rga_info_t));
    dst.fd = -1;
    dst.virAddr = win0.bo.ptr;
    dst.mmuFlag = 1;
    rga_set_rect(&dst.rect, x, y, srcw, srch, win0.width, win0.height, fmt);
    if (c_RkRgaBlit(&src, &dst, NULL))
        printf("%s: rga fail\n", __func__);
}

static void *display_win_thread(void *arg)
{
    while (1) {
        pthread_mutex_lock(&lock);
        display_compose(win1.bo.ptr, win1.width, win1.height, win1.fmt, win1.x, win1.y);
        display_compose(win2.bo.ptr, win2.width, win2.height, win2.fmt, win2.x, win2.y);
        pthread_mutex_unlock(&lock);
        display_commit(win0.bo.ptr, win0.fd, win0.fmt, win0.width, win0.height, 0);
        usleep(10000);
    }
    pthread_exit(NULL);
}

int display_init(int width, int height)
{
    int ret;
#ifdef USE_NV12
    g_disp.fmt = DRM_FORMAT_NV12;
    g_disp.rga_fmt = RK_FORMAT_YCbCr_420_SP;
#endif
#ifdef USE_RGB888
    g_disp.fmt = DRM_FORMAT_BGR888;
    g_disp.rga_fmt = RK_FORMAT_RGB_888;
#endif
    g_disp.width = width;
    g_disp.height = height;
    g_disp.plane_type = DRM_PLANE_TYPE_OVERLAY;
    g_disp.buf_cnt = BUF_COUNT;
    ret = drm_display_init(&g_disp);
    if (ret)
        return ret;

    win0.width = 720;
    win0.height = 1280;
    rga_control_buffer_init(&win0.bo, &win0.fd, win0.width, win0.height, 12);
    win1.width = 720;
    win1.height = 640;
    rga_control_buffer_init(&win1.bo, &win1.fd, win1.width, win1.height, 12);
    win2.width = 720;
    win2.height = 640;
    win2.x = 0;
    win2.y = 640;
    rga_control_buffer_init(&win2.bo, &win2.fd, win2.width, win2.height, 12);

    if (pthread_create(&g_th, NULL, display_win_thread, NULL)) {
        printf("%s create fail!\n", __func__);
        return -1;
    }

    return 0;
}


static void drm_display_exit(struct display *disp)
{
    drmDeinit(&disp->dev);
    for (int i = 0; i < disp->buf_cnt; i++)
        drmPutBuffer(disp->dev.drm_fd, &disp->buf[i]);

    rga_control_buffer_deinit(&win0.bo, win0.fd);
    rga_control_buffer_deinit(&win1.bo, win1.fd);
    rga_control_buffer_deinit(&win2.bo, win2.fd);
}

void display_exit(void)
{
    drm_display_exit(&g_disp);
}

void drm_commit(struct display *disp, int num, void *ptr, int fd, int fmt, int w, int h, int rotation)
{
    int ret;
    rga_info_t src, dst;
    char *map = disp->buf[num].map;
    int dst_w = disp->width;
    int dst_h = disp->height;
    int dst_fmt = disp->rga_fmt;

    memset(&src, 0, sizeof(rga_info_t));
    src.fd = -1;
    src.virAddr = ptr;
    src.mmuFlag = 1;
    src.rotation = rotation;
    rga_set_rect(&src.rect, 0, 0, w, h, w, h, fmt);
    memset(&dst, 0, sizeof(rga_info_t));
    dst.fd = -1;
    dst.virAddr = map;
    dst.mmuFlag = 1;
    rga_set_rect(&dst.rect, 0, 0, dst_w, dst_h, dst_w, dst_h, dst_fmt);
    if (c_RkRgaBlit(&src, &dst, NULL)) {
        printf("%s: rga fail\n", __func__);
        return;
    }

    ret = drmCommit(&disp->buf[num], disp->width, disp->height, 0, 0, &disp->dev, disp->plane_type);
    if (ret) {
        fprintf(stderr, "display commit error, ret = %d\n", ret);
    }
}

void display_commit(void *ptr, int fd, int fmt, int w, int h, int rotation)
{
    static int num = 0;

    drm_commit(&g_disp, num, ptr, fd, fmt, w, h, rotation);
    num = (num + 1) % BUF_COUNT;
}

void display_switch(enum display_video_type type)
{
#if 0
    set_rgb_display(NULL);
    set_ir_display(NULL);
    set_usb_display(NULL);
    if (type == DISPLAY_VIDEO_RGB)
        set_rgb_display(display_commit);
    else if (type == DISPLAY_VIDEO_IR)
        set_ir_display(display_commit);
    else if (type == DISPLAY_VIDEO_USB)
        set_usb_display(display_commit);
#endif
}

void display_win(void *srcp, int fmt, int srcw, int srch, int rotation, void *dstp, int dstw, int dsth)
{
    rga_info_t src, dst;
    memset(&src, 0, sizeof(rga_info_t));
    src.fd = -1;
    src.virAddr = srcp;
    src.mmuFlag = 1;
    src.rotation = rotation;
    rga_set_rect(&src.rect, 0, 0, srcw, srch, srcw, srch, fmt);
    memset(&dst, 0, sizeof(rga_info_t));
    dst.fd = -1;
    dst.virAddr = dstp;
    dst.mmuFlag = 1;
    rga_set_rect(&dst.rect, 0, 0, dstw, dsth, dstw, dsth, fmt);
    if (c_RkRgaBlit(&src, &dst, NULL))
        printf("%s: rga fail\n", __func__);
}

void display_win1(void *ptr, int fd, int fmt, int w, int h, int rotation)
{
    pthread_mutex_lock(&lock);
    win0.fmt = fmt;
    win1.fmt = fmt;
    display_win(ptr, fmt, w, h, rotation, win1.bo.ptr, win1.width, win1.height);
    pthread_mutex_unlock(&lock);
}

void display_win2(void *ptr, int fd, int fmt, int w, int h, int rotation)
{
    pthread_mutex_lock(&lock);
    win0.fmt = fmt;
    win2.fmt = fmt;
    display_win(ptr, fmt, w, h, rotation, win2.bo.ptr, win2.width, win2.height);
    pthread_mutex_unlock(&lock);
}

void display_get_resolution(int *width, int *height)
{
    *width = g_disp.width;
    *height = g_disp.height;
}
