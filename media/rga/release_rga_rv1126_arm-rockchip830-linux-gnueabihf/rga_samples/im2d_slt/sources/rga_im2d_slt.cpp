/*
 * Copyright (C) 2021 Rockchip Electronics Co., Ltd.
 * Authors:
 *  Cerf Yu <cerf.yu@rock-chips.com>
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
#define LOG_TAG "rga_im2d_slt"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <memory.h>
#include <pthread.h>

#if LIBDRM
#include "drm_alloc.h"
#include "xf86drm.h"
#endif

#include "RockchipRga.h"
#include "im2d_api/im2d.hpp"
#include "RgaUtils.h"

#include "dma_alloc.h"
#include "drm_alloc.h"

#include "slt_config.h"

enum {
    FILL_BUFF  = 0,
    EMPTY_BUFF = 1
};

typedef struct private_data {
    int id;
    const char *name;
    int mode;
    unsigned int num;

    int width;
    int height;
    int format;

    int rd_mode;
    int core;
    int priority;

    int result;
} private_data_t;

static int get_buf_size_by_w_h_f(int w, int h, int f) {
    float bpp = get_bpp_from_format(f);
    int size = 0;

    size = (int)w * h * bpp;
    return size;
}

int read_image_from_path(void *buf, const char *path, int f, int sw, int sh, int index, int mode) {
    const char *inputFilePath = "%s/in%dw%d-h%d-%s.bin";
    const char *inputFbcFilePath = "%s/in%dw%d-h%d-%s-fbc.bin";
    char filePath[100];

    snprintf(filePath, 100, (mode == IM_FBC_MODE) ? inputFbcFilePath : inputFilePath,
             path, index, sw, sh, translate_format_str(f));

    FILE *file = fopen(filePath, "rb");
    if (!file) {
        fprintf(stderr, "Could not open %s\n", filePath);
        return -EINVAL;
    }

    if (mode == IM_FBC_MODE)
        fread(buf, get_buf_size_by_w_h_f(sw, sh, f) * 1.5, 1, file);
    else
        fread(buf, get_buf_size_by_w_h_f(sw, sh, f), 1, file);

    fclose(file);

    return 0;
}

static int write_image_to_path(void *buf, const char *path, int f, int sw, int sh, int index, int mode) {
    const char *outputFilePath = "%s/out%dw%d-h%d-%s.bin";
    const char *outputFbcFilePath = "%s/out%dw%d-h%d-%s-fbc.bin";
    char filePath[100];

    snprintf(filePath, 100, (mode == IM_FBC_MODE) ? outputFbcFilePath : outputFilePath,
             path, index, sw, sh, translate_format_str(f));

    FILE *file = fopen(filePath, "wb+");
    if (!file) {
        fprintf(stderr, "Could not open %s\n", filePath);
        return false;
    } else {
        fprintf(stderr, "open %s and write ok\n", filePath);
    }

    if (mode == IM_FBC_MODE)
        fwrite(buf, get_buf_size_by_w_h_f(sw, sh, f) * 1.5, 1, file);
    else
        fwrite(buf, get_buf_size_by_w_h_f(sw, sh, f), 1, file);

    fclose(file);

    return 0;
}

#if IM2D_SLT_GRAPHICBUFFER_EN
#include <ui/GraphicBuffer.h>

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

sp<GraphicBuffer> GraphicBuffer_Init(int width, int height,int format, bool use_rga2, bool cacheable) {
    uint64_t usage = 0;

    if (cacheable)
        usage |= GRALLOC_USAGE_SW_READ_OFTEN;

    if (use_rga2)
        usage |= RK_GRALLOC_USAGE_WITHIN_4G;

    sp<GraphicBuffer> gb(new GraphicBuffer(width, height, format, 0, usage));

    if (gb->initCheck()) {
        /*
         * The lower version of gralloc (gralloc-0.3) does not support 64-bit usage,
         * so it needs to be truncated externally to 32-bit. And don't need 4G usage.
         */
        printf("graphicbuffer re-alloc 32-bit usage\n");
        gb = sp<GraphicBuffer>(new GraphicBuffer(width, height, format, (int)usage));
        if (gb->initCheck()) {
            printf("GraphicBuffer check error : %s\n",strerror(errno));
            return NULL;
        }
    }

    return gb;
}

/* Write data to buffer or init buffer. */
int GraphicBuffer_Fill(sp<GraphicBuffer> gb, int width, int height,int format, int flag, int index, int mode) {
    int ret = 0;
       char* buf = NULL;
    size_t buf_size;
    ret = gb->lock(GRALLOC_USAGE_SW_READ_OFTEN, (void**)&buf);
    if (ret) {
        printf("lock buffer error : %s\n",strerror(errno));
        return -1;
    }

    buf_size = width * height * get_bpp_from_format(format);
    if (mode == IM_FBC_MODE)
        buf_size = buf_size * 1.5;

    if(flag) {
        memset(buf, index, buf_size);
    } else {
        ret = read_image_from_path(buf, IM2D_SLT_DEFAULT_INPUT_PATH, format, width, height, index, mode);
        if (ret != 0) {
            printf ("open file %s \n", "fault");
            memset(buf, index, buf_size);
            ret = -1;
            goto unlock;
        }
    }

unlock:
    ret = gb->unlock();
       if (ret) {
        printf("unlock buffer error : %s\n",strerror(errno));
        return -1;
    }

    return ret;
}
#endif

/******************************************************************************/
void *pthread_rga_run(void *args) {
    int ret = 0, time = 0;
    unsigned int num;
    int srcWidth,srcHeight,srcFormat;
    int dstWidth,dstHeight,dstFormat;

    char *src_va, *dst_va;
#if IM2D_SLT_DRM_BUFFER_EN
    struct drm_object drm_src, drm_dst;
#elif IM2D_SLT_GRAPHICBUFFER_EN
    sp<GraphicBuffer> src_buf;
    sp<GraphicBuffer> dst_buf;
#elif IM2D_SLT_RK_DMA_HEAP_EN
    int src_fd, dst_fd;
    rga_buffer_handle_t src_handle, dst_handle;
#endif

    rga_buffer_t src;
    rga_buffer_t dst;
    im_rect src_rect;
    im_rect dst_rect;

    private_data_t *data = (private_data_t *)args;

    num = data->num;

    srcWidth = data->width;
    srcHeight = data->height;
    srcFormat = data->format;

    dstWidth = data->width;
    dstHeight = data->height;
    dstFormat = data->format;

    memset(&src, 0x0, sizeof(src));
    memset(&dst, 0x0, sizeof(dst));
    memset(&src_rect, 0x0, sizeof(src_rect));
    memset(&dst_rect, 0x0, sizeof(dst_rect));

    do {
        time++;

#if IM2D_SLT_DRM_BUFFER_EN
        if (data->rd_mode != IM_FBC_MODE) {
            drm_src.drm_buf = (uint8_t *)drm_buf_alloc(srcWidth, srcHeight, get_bpp_from_format(srcFormat) * 8,
                                                       &drm_src.drm_buffer_fd, &drm_src.drm_buffer_handle,
                                                       &drm_src.actual_size, IM2D_SLT_BUFFER_PHY_EN ? ROCKCHIP_BO_CONTIG : 0);
            drm_dst.drm_buf = (uint8_t *)drm_buf_alloc(dstWidth, dstHeight, get_bpp_from_format(dstFormat) * 8,
                                                       &drm_dst.drm_buffer_fd, &drm_dst.drm_buffer_handle,
                                                       &drm_dst.actual_size, IM2D_SLT_BUFFER_PHY_EN ? ROCKCHIP_BO_CONTIG : 0);

            src_va = (char *)drm_src.drm_buf;
            dst_va = (char *)drm_dst.drm_buf;

            ret = read_image_from_path(src_va, IM2D_SLT_DEFAULT_INPUT_PATH,
                                       srcFormat, srcWidth, srcHeight, 0, data->rd_mode);
            if (ret != 0) {
                printf ("ID[%d] %s open file %s \n", data->id, data->name, "fault");
                goto NORMAL_ERR;
            }

            memset(dst_va, 0xff, dstWidth * dstHeight * get_bpp_from_format(dstFormat));
        } else {
            drm_src.drm_buf = (uint8_t *)drm_buf_alloc(srcWidth, srcHeight * 1.5, get_bpp_from_format(srcFormat) * 8,
                                                       &drm_src.drm_buffer_fd, &drm_src.drm_buffer_handle,
                                                       &drm_src.actual_size, IM2D_SLT_BUFFER_PHY_EN ? ROCKCHIP_BO_CONTIG : 0);
            drm_dst.drm_buf = (uint8_t *)drm_buf_alloc(dstWidth, dstHeight * 1.5, get_bpp_from_format(dstFormat) * 8,
                                                       &drm_dst.drm_buffer_fd, &drm_dst.drm_buffer_handle,
                                                       &drm_dst.actual_size, IM2D_SLT_BUFFER_PHY_EN ? ROCKCHIP_BO_CONTIG : 0);

            src_va = (char *)drm_src.drm_buf;
            dst_va = (char *)drm_dst.drm_buf;

            ret = read_image_from_path(src_va, IM2D_SLT_DEFAULT_INPUT_PATH,
                                       srcFormat, srcWidth, srcHeight, 0, data->rd_mode);
            if (ret != 0) {
                printf ("ID[%d] %s open file %s \n", data->id, data->name, "fault");
                goto NORMAL_ERR;
            }

            memset(dst_va, 0xff, dstWidth * dstHeight * 1.5 * get_bpp_from_format(dstFormat));
        }

        src = wrapbuffer_fd(drm_src.drm_buffer_fd, srcWidth, srcHeight, srcFormat);
        dst = wrapbuffer_fd(drm_dst.drm_buffer_fd, dstWidth, dstHeight, dstFormat);
        if (src.width == 0 || dst.width == 0) {
            printf("%s", imStrError());
            goto NORMAL_ERR;
        }
#elif IM2D_SLT_GRAPHICBUFFER_EN
        if (data->rd_mode == IM_FBC_MODE) {
            src_buf = GraphicBuffer_Init(srcWidth, srcHeight * 1.5, srcFormat,
                                         data->core == IM_SCHEDULER_RGA2_CORE0 ? true : false, IM2D_SLT_BUFFER_CACHEABLE);
            dst_buf = GraphicBuffer_Init(dstWidth, dstHeight * 1.5, dstFormat,
                                         data->core == IM_SCHEDULER_RGA2_CORE0 ? true : false, IM2D_SLT_BUFFER_CACHEABLE);
        } else {
            src_buf = GraphicBuffer_Init(srcWidth, srcHeight, srcFormat,
                                         data->core == IM_SCHEDULER_RGA2_CORE0 ? true : false, IM2D_SLT_BUFFER_CACHEABLE);
            dst_buf = GraphicBuffer_Init(dstWidth, dstHeight, dstFormat,
                                         data->core == IM_SCHEDULER_RGA2_CORE0 ? true : false, IM2D_SLT_BUFFER_CACHEABLE);
        }
        if (src_buf == NULL || dst_buf == NULL) {
            printf("GraphicBuff init error!\n");
            goto NORMAL_ERR;
        }

        if(-1 == GraphicBuffer_Fill(src_buf, srcWidth, srcHeight, srcFormat, FILL_BUFF, 0, data->rd_mode)) {
            printf("%s, src write Graphicbuffer error!\n", __FUNCTION__);
            goto NORMAL_ERR;
        }
        if(-1 == GraphicBuffer_Fill(dst_buf, dstWidth, dstHeight, dstFormat, EMPTY_BUFF, 0xff, data->rd_mode)) {
            printf("%s, dst write Graphicbuffer error!\n", __FUNCTION__);
            goto NORMAL_ERR;
        }

        src = wrapbuffer_GraphicBuffer(src_buf);
        src = wrapbuffer_fd(src.fd, srcWidth, srcHeight, srcFormat);
        dst = wrapbuffer_GraphicBuffer(dst_buf);
        dst = wrapbuffer_fd(dst.fd, dstWidth, dstHeight, dstFormat);
        /* If it is in fbc mode, because the height of the alloc memory
         * is modified, it needs to be corrected here */
        if (data->rd_mode == IM_FBC_MODE) {
            src.height  = srcHeight;
            src.hstride = srcHeight;
            dst.height  = dstHeight;
            dst.hstride = dstHeight;
        }
        if (src.width == 0 || dst.width == 0) {
            printf("%s", imStrError());
        }
#elif IM2D_SLT_RK_DMA_HEAP_EN
        ret = dma_buf_alloc(srcWidth, srcHeight, srcFormat, &src_fd, (void **)&src_va);
        if (ret < 0) {
            printf("ID[%d] %s alloc src dma_buf failed!\n", data->id, data->name);
            goto NORMAL_ERR;
        }

        ret = dma_buf_alloc(dstWidth, dstHeight, dstFormat, &dst_fd, (void **)&dst_va);
        if (ret < 0) {
            printf("ID[%d] %s alloc dst dma_buf failed!\n", data->id, data->name);
            goto NORMAL_ERR;
        }

        dma_sync_device_to_cpu(src_fd);
        dma_sync_device_to_cpu(dst_fd);

        ret = read_image_from_path(src_va, IM2D_SLT_DEFAULT_INPUT_PATH,
                                    srcFormat, srcWidth, srcHeight, 0, data->rd_mode);
        if (ret != 0) {
            printf ("ID[%d] %s open file %s \n", data->id, data->name, "fault");
            goto NORMAL_ERR;
        }

        memset(dst_va, 0xff, dstWidth * dstHeight * get_bpp_from_format(dstFormat));

        dma_sync_cpu_to_device(src_fd);
        dma_sync_cpu_to_device(dst_fd);

        src_handle = importbuffer_fd(src_fd, srcWidth, srcHeight, srcFormat);
        if (src_handle <= 0) {
            printf("ID[%d] %s import src dma_buf failed!\n", data->id, data->name);
            goto NORMAL_ERR;
        }
        dst_handle = importbuffer_fd(dst_fd, dstWidth, dstHeight, dstFormat);
        if (dst_handle <= 0) {
            printf("ID[%d] %s import dst dma_buf failed!\n", data->id, data->name);
            releasebuffer_handle(src_handle);
            goto NORMAL_ERR;
        }

        src = wrapbuffer_handle(src_handle, srcWidth, srcHeight, srcFormat);
        dst = wrapbuffer_handle(dst_handle, dstWidth, dstHeight, dstFormat);
        if (src.width == 0 || dst.width == 0) {
            printf("%s", imStrError());

            releasebuffer_handle(src_handle);
            releasebuffer_handle(dst_handle);
            goto NORMAL_ERR;
        }
#endif

        ret = imcheck(src, dst, src_rect, dst_rect);
        if (ret != IM_STATUS_NOERROR) {
            printf("ID[%d]: %s check %d time error! %s", data->id, data->name, time, imStrError((IM_STATUS)ret));

            goto NORMAL_ERR;
        }

        imconfig(IM_CONFIG_SCHEDULER_CORE, data->core);
        imconfig(IM_CONFIG_PRIORITY, data->priority);

        src.rd_mode = data->rd_mode;
        dst.rd_mode = data->rd_mode;

        ret = imcopy(src, dst);
        if (ret == IM_STATUS_SUCCESS) {
            printf("ID[%d]: %s imcopy %d time success!\n", data->id, data->name, time);

#if IM2D_SLT_GRAPHICBUFFER_EN
            ret = src_buf->lock(GRALLOC_USAGE_SW_READ_OFTEN, (void**)&src_va);
            if (ret) {
                printf("lock src_buf error : %s\n",strerror(errno));
                goto NORMAL_ERR;
            }

            ret = dst_buf->lock(GRALLOC_USAGE_SW_READ_OFTEN, (void**)&dst_va);
            if (ret) {
                printf("lock dst_buf error : %s\n",strerror(errno));
                goto NORMAL_ERR;
            }
#elif IM2D_SLT_RK_DMA_HEAP_EN
            dma_sync_device_to_cpu(src_fd);
            dma_sync_device_to_cpu(dst_fd);
#endif

            ret = memcmp(src_va, dst_va, dst.wstride * dst.hstride * get_bpp_from_format(dst.format));
            if (ret < 0) {
                printf("ID[%d]: %s check buffer %d time error!\n", data->id, data->name, time);
                printf("src: %x %x %x %x\n", (int)src_va[0], (int)src_va[1], (int)src_va[2], (int)src_va[3]);
                printf("dst: %x %x %x %x\n", (int)dst_va[0], (int)dst_va[1], (int)dst_va[2], (int)dst_va[3]);
                write_image_to_path(dst_va, IM2D_SLT_DEFAULT_OUTPUT_PATH,
                                    dst.format, dst.wstride, dst.hstride, data->id + 1, dst.rd_mode);
                write_image_to_path(src_va, IM2D_SLT_DEFAULT_OUTPUT_PATH,
                                    src.format, src.wstride, src.hstride, data->id + 2, src.rd_mode);

                goto CHECK_ERR;
            } else {
                printf("ID[%d]: %s check buffer %d time success!\n", data->id, data->name, time);
            }

#if IM2D_SLT_GRAPHICBUFFER_EN
            ret = src_buf->unlock();
            if (ret) {
                printf("unlock src_buf error : %s\n",strerror(errno));
                goto NORMAL_ERR;
            }

            ret = dst_buf->unlock();
            if (ret) {
                printf("unlock dst_buf error : %s\n",strerror(errno));
                goto NORMAL_ERR;
            }
#endif
        } else {
            printf("ID[%d]: %s run %d time error!, %s\n", data->id, data->name, time, imStrError((IM_STATUS)ret));
            goto NORMAL_ERR;
        }

#if IM2D_SLT_DRM_BUFFER_EN
        drm_buf_destroy(drm_src.drm_buffer_fd, drm_src.drm_buffer_handle,
                        drm_src.drm_buf, drm_src.actual_size);
        drm_buf_destroy(drm_dst.drm_buffer_fd, drm_dst.drm_buffer_handle,
                        drm_dst.drm_buf, drm_dst.actual_size);
#elif IM2D_SLT_RK_DMA_HEAP_EN
        releasebuffer_handle(src_handle);
        releasebuffer_handle(dst_handle);

        dma_buf_free(srcWidth, srcHeight, srcFormat, &src_fd, (void *)src_va);
        dma_buf_free(dstWidth, dstHeight, dstFormat, &dst_fd, (void *)dst_va);
#endif
    } while (data->mode && --num);

#if IM2D_SLT_THREAD_EN
    data->result = 0;
    pthread_exit(NULL);

NORMAL_ERR:
    data->result = -1;
    pthread_exit(NULL);
CHECK_ERR:
    data->result = -2;
    pthread_exit(NULL);
#else
    data->result = 0;
    return NULL;

NORMAL_ERR:
    data->result = -1;
    return NULL;
CHECK_ERR:
    data->result = -2;
    return NULL;
#endif
}

int main() {
    int pthread_num = 0;
    pthread_t tdSyncID[IM2D_SLT_THREAD_MAX];
    private_data_t data[IM2D_SLT_THREAD_MAX];

    memset(&data, 0x0, sizeof(private_data_t) * IM2D_SLT_THREAD_MAX);
    printf("-------------------------------------------------\n");

#if IM2D_SLT_TEST_RGA3_0_EN
    pthread_num++;
    data[pthread_num].id = pthread_num;
    data[pthread_num].name = "RGA3_core0";
    data[pthread_num].mode = IM2D_SLT_WHILE_EN;
    data[pthread_num].num = IM2D_SLT_WHILE_NUM;
    data[pthread_num].width = IM2D_SLT_DEFAULT_WIDTH;
    data[pthread_num].height = IM2D_SLT_DEFAULT_HEIGHT;
    data[pthread_num].format = IM2D_SLT_DEFAULT_FORMAT;
    data[pthread_num].rd_mode = IM_RASTER_MODE;
    data[pthread_num].core = IM_SCHEDULER_RGA3_CORE0;
    data[pthread_num].priority = 1;
#endif

#if IM2D_SLT_TEST_RGA3_1_EN
    pthread_num++;
    data[pthread_num].id = pthread_num;
    data[pthread_num].name = "RGA3_core1";
    data[pthread_num].mode = IM2D_SLT_WHILE_EN;
    data[pthread_num].num = IM2D_SLT_WHILE_NUM;
    data[pthread_num].width = IM2D_SLT_DEFAULT_WIDTH;
    data[pthread_num].height = IM2D_SLT_DEFAULT_HEIGHT;
    data[pthread_num].format = IM2D_SLT_DEFAULT_FORMAT;
    data[pthread_num].rd_mode = IM_RASTER_MODE;
    data[pthread_num].core = IM_SCHEDULER_RGA3_CORE1;
    data[pthread_num].priority = 1;
#endif

#if IM2D_SLT_TEST_RGA2_EN
    pthread_num++;
    data[pthread_num].id = pthread_num;
    data[pthread_num].name = "RGA2";
    data[pthread_num].mode = IM2D_SLT_WHILE_EN;
    data[pthread_num].num = IM2D_SLT_WHILE_NUM;
    data[pthread_num].width = IM2D_SLT_DEFAULT_WIDTH;
    data[pthread_num].height = IM2D_SLT_DEFAULT_HEIGHT;
    data[pthread_num].format = IM2D_SLT_DEFAULT_FORMAT;
    data[pthread_num].rd_mode = IM_RASTER_MODE;
    data[pthread_num].core = IM_SCHEDULER_RGA2_CORE0;
    data[pthread_num].priority = 1;
#endif

#if IM2D_SLT_TEST_RGA3_0_FBC_EN
    pthread_num++;
    data[pthread_num].id = pthread_num;
    data[pthread_num].name = "RGA3_core0_fbc";
    data[pthread_num].mode = IM2D_SLT_WHILE_EN;
    data[pthread_num].num = IM2D_SLT_WHILE_NUM;
    data[pthread_num].width = IM2D_SLT_DEFAULT_WIDTH;
    data[pthread_num].height = IM2D_SLT_DEFAULT_HEIGHT;
    data[pthread_num].format = IM2D_SLT_DEFAULT_FORMAT;
    data[pthread_num].rd_mode = IM_FBC_MODE;
    data[pthread_num].core = IM_SCHEDULER_RGA3_CORE0;
    data[pthread_num].priority = 1;
#endif

#if IM2D_SLT_TEST_RGA3_1_FBC_EN
    pthread_num++;
    data[pthread_num].id = pthread_num;
    data[pthread_num].name = "RGA3_core1_fbc";
    data[pthread_num].mode = IM2D_SLT_WHILE_EN;
    data[pthread_num].num = IM2D_SLT_WHILE_NUM;
    data[pthread_num].width = IM2D_SLT_DEFAULT_WIDTH;
    data[pthread_num].height = IM2D_SLT_DEFAULT_HEIGHT;
    data[pthread_num].format = IM2D_SLT_DEFAULT_FORMAT;
    data[pthread_num].rd_mode = IM_FBC_MODE;
    data[pthread_num].core = IM_SCHEDULER_RGA3_CORE1;
    data[pthread_num].priority = 1;
#endif

#if IM2D_SLT_THREAD_EN
    for (int i = 1; i <= pthread_num; i++) {
        pthread_create(&tdSyncID[i], NULL, pthread_rga_run, (void *)(&data[i]));
        printf("creat Sync pthread[0x%lx] = %d, id = %d\n", tdSyncID[i], i, data[i].id);
    }

    for (int i = 1; i <= pthread_num; i++) {
        pthread_join(tdSyncID[i], NULL);
        if (data[i].result < 0) {
            printf("ID[%d] case '%s' is faile!\n", data[i].id, data[i].name);
            printf("-------------------------------------------------\n");
            printf("im2d api slt fail!\n");
            return -1;
        }
    }

    printf("-------------------------------------------------\n");
    printf("im2d api slt success!\n");

    return 0;
#else
    (void)(tdSyncID);

    for (int i = 1; i <= pthread_num; i++) {
        pthread_rga_run((void *)(&data[i]));
        printf("ID[%d] %s run end!\n", data[i].id, data[i].name);
        if (data[i].result < 0) {
            printf("ID[%d] case '%s' is faile!\n", data[i].id, data[i].name);
            return -1;
        }
    }

    printf("-------------------------------------------------\n");
    printf("im2d api slt success!\n");

    return 0;
#endif
}

