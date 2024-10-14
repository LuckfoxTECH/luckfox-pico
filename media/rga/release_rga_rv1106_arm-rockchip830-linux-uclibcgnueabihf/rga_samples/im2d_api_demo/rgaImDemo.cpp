/*
 * Copyright (C) 2020 Rockchip Electronics Co., Ltd.
 * Authors:
 *  PutinLee <putin.lee@rock-chips.com>
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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <memory.h>
#include <unistd.h>
#include <sys/time.h>

#include "im2d.hpp"
#include "RockchipRga.h"
#include "RgaUtils.h"
#include "args.h"

#include "utils.h"
#include "dma_alloc.h"

#define LOCAL_FILE_PATH "/data"

#define DEFAULT_WIDTH               1280
#define DEFAULT_HEIGHT              720
#define DEFAULT_SCALE_UP_WIDTH      1920
#define DEFAULT_SCALE_UP_HEIGHT     1080
#define DEFAULT_SCALE_DOWN_WIDTH    720
#define DEFAULT_SCALE_DOWN_HEIGHT   480

#define DEFAULT_RGBA_FORMAT         RK_FORMAT_RGBA_8888
#define DEFAULT_YUV_FORMAT          RK_FORMAT_YCbCr_420_SP

static int file_exists(const char* file_name) {
    FILE* file = fopen(file_name, "r");
    if (file != NULL) {
        fclose(file);
        return true;
    }

    return false;
}

int main(int argc, char*  argv[]) {
    struct timeval start, end;
    long usec1;

    int ret = 0;
    int while_time = 0;
    int mode;
    int parm_data[MODE_MAX] = {0};

    IM_USAGE rotate;
    IM_USAGE flip;
    im_color_t color;

    im_rect src_rect;
    im_rect dst_rect;
    rga_buffer_t src;
    rga_buffer_t dst;
    rga_buffer_handle_t src_handle;
    rga_buffer_handle_t dst_handle;

    int src_width, src_height, src_format;
    int dst_width, dst_height, dst_format;
    int src_buf_size, dst_buf_size;

    char* src_buf = NULL;
    char* dst_buf = NULL;
    int src_dma_fd, dst_dma_fd;

    const char *heap_name = NULL;
    int use_dma_heap = false;
    int flush_cache = false;

    memset(&src_rect, 0, sizeof(src_rect));
    memset(&dst_rect, 0, sizeof(dst_rect));
    memset(&src, 0, sizeof(src));
    memset(&dst, 0, sizeof(dst));

    mode = readArguments(argc, argv, parm_data);
    if(MODE_NONE == mode) {
        printf("%s, Unknow RGA mode\n", __FUNCTION__);
        return -1;
    }

    printf("RGA demo mode = 0x%x\n", mode);
    if (mode == MODE_QUERYSTRING) {
        printf("\n%s\n", querystring((IM_INFORMATION)parm_data[MODE_QUERYSTRING]));

        return 0;
    }

    if (mode == MODE_HELP) {
        return 0;
    }

    if (mode & WHILE_FLAG) {
        /* Remove flag of MODE_WHILE. */
        mode &= ~WHILE_FLAG;

        while_time = parm_data[MODE_WHILE];
    }

    src_width = DEFAULT_WIDTH;
    src_height = DEFAULT_HEIGHT;
    src_format = DEFAULT_RGBA_FORMAT;

    switch (mode) {
        case MODE_RESIZE:
            switch(parm_data[MODE_RESIZE]) {
                case IM_UP_SCALE:
                    dst_width = DEFAULT_SCALE_UP_WIDTH;
                    dst_height = DEFAULT_SCALE_UP_HEIGHT;
                    break;
                case IM_DOWN_SCALE:
                    dst_width = DEFAULT_SCALE_DOWN_WIDTH;
                    dst_height = DEFAULT_SCALE_DOWN_HEIGHT;
                    break;
                default:
                    printf("Please configure the correct scaling mode.\n");
                    return -1;
            }

            dst_format = DEFAULT_RGBA_FORMAT;

            break;

        case MODE_CVTCOLOR:
            dst_width = DEFAULT_WIDTH;
            dst_height = DEFAULT_HEIGHT;
            dst_format = DEFAULT_YUV_FORMAT;

            break;

        default:
            dst_width = DEFAULT_WIDTH;
            dst_height = DEFAULT_HEIGHT;
            dst_format = DEFAULT_RGBA_FORMAT;

            break;
    }

    src_buf_size = src_width * src_height * get_bpp_from_format(src_format);
    dst_buf_size = dst_width * dst_height * get_bpp_from_format(dst_format);

#if USE_RV1106_CMA
    heap_name = RV1106_CMA_HEAP_PATH;
    flush_cache = true;
#else
    heap_name = DMA_HEAP_DMA32_UNCACHED_PATH;
#endif

    /********** Get parameters **********/
    use_dma_heap = file_exists(heap_name);
    if (use_dma_heap) {
        printf("alloc buffer from \"%s\".\n", heap_name);
        ret = dma_buf_alloc(heap_name, src_buf_size, &src_dma_fd, (void **)&src_buf);
        if (ret < 0) {
            printf("alloc src %s failed!\n", heap_name);
            return -1;
        }

        ret = dma_buf_alloc(heap_name, dst_buf_size, &dst_dma_fd, (void **)&dst_buf);
        if (ret < 0) {
            printf("alloc dst %s failed!\n", heap_name);
            dma_buf_free(src_buf_size, &src_dma_fd, src_buf);
            goto free_buffer;
        }

        src_handle = importbuffer_fd(src_dma_fd, src_buf_size);
        if (src_handle == 0) {
            printf("Failed to import CMA buffer fd for src channel! %s\n", imStrError());
            goto free_buffer;
        }
        dst_handle = importbuffer_fd(dst_dma_fd, dst_buf_size);
        if (dst_handle == 0) {
            printf("Failed to import CMA buffer fd for dst channel! %s\n", imStrError());
            goto free_buffer;
        }
    } else {
        printf("\"%s\" does not exist, so use malloc.\n", heap_name);

        src_buf = (char*)malloc(src_buf_size);
        dst_buf = (char*)malloc(dst_buf_size);
        if (src_buf == NULL || dst_buf == NULL) {
            printf("malloc buffer failed!\n");
            goto free_buffer;
        }

        src_handle = importbuffer_virtualaddr(src_buf, src_buf_size);
        if (src_handle == 0) {
            printf("Failed to import virtualaddr for src channel! %s\n", imStrError());
            ret = -1;
            goto free_buffer;
        }
        dst_handle = importbuffer_virtualaddr(dst_buf, dst_buf_size);
        if (dst_handle == 0) {
            printf("Failed to import virtualaddr for dst channel! %s\n", imStrError());
            ret = -1;
            goto free_buffer;
        }
    }

    /* invalid CPU cache */
    if (flush_cache && src_dma_fd && dst_dma_fd) {
        dma_sync_device_to_cpu(src_dma_fd);
        dma_sync_device_to_cpu(dst_dma_fd);
    }

    ret = read_image_from_file(src_buf, LOCAL_FILE_PATH, src_width, src_height, src_format, 0);
    if (ret != 0) {
        printf ("can not open file for src channel, memset to 0x%x.\n", 0x11);
        memset(src_buf, 0x11, src_buf_size);
    }

    if(mode == MODE_FILL) {
        ret = read_image_from_file(dst_buf, LOCAL_FILE_PATH, dst_width, dst_height, dst_format, 0);
        if (ret != 0) {
            printf ("can not open file for dst channel, memset to 0x%x.\n", 0x22);
            memset(dst_buf, 0x22, dst_buf_size);
        }
    } else if (mode == MODE_BLEND) {
        ret = read_image_from_file(dst_buf, LOCAL_FILE_PATH, dst_width, dst_height, dst_format, 1);
        if (ret != 0) {
            printf ("can not open file for dst channel, memset to 0x%x.\n", 0x33);
            memset(dst_buf, 0x33, dst_buf_size);
        }
    } else {
        memset(dst_buf, 0x00, dst_buf_size);
    }

    /* clear CPU cache */
    if (flush_cache && src_dma_fd && dst_dma_fd) {
        dma_sync_cpu_to_device(src_dma_fd);
        dma_sync_cpu_to_device(dst_dma_fd);
    }

    src = wrapbuffer_handle(src_handle, src_width, src_height, src_format);
    dst = wrapbuffer_handle(dst_handle, dst_width, dst_height, dst_format);
    if(src.width == 0 || dst.width == 0) {
        printf("%s, %s", __FUNCTION__, imStrError());
        ret = -1;
        goto release_buffer;
    }

    do {
        if (while_time) {
            static int while_num = 1;
            printf("This is %d time in the loop\n", while_num);

            while_num++;
            while_time--;
        }
        /********** Execution function according to mode **********/
        switch(mode) {
            case MODE_COPY :      //rgaImDemo --copy
                ret = imcheck(src, dst, src_rect, dst_rect);
                if (ret != IM_STATUS_NOERROR) {
                    printf("%d, check error! %s\n", __LINE__, imStrError((IM_STATUS)ret));
                    goto release_buffer;
                }

                gettimeofday(&start, NULL);

                ret = imcopy(src, dst);
                if (ret != IM_STATUS_SUCCESS) {
                    printf("%d, imcopy failed! %s\n", __LINE__, imStrError((IM_STATUS)ret));
                    goto release_buffer;
                }

                gettimeofday(&end, NULL);
                usec1 = 1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
                printf("copying .... cost time %ld us\n", usec1);

                break;

            case MODE_RESIZE :    //rgaImDemo --resize=up/down
                ret = imcheck(src, dst, src_rect, dst_rect);
                if (ret != IM_STATUS_NOERROR) {
                    printf("%d, check error! %s\n", __LINE__, imStrError((IM_STATUS)ret));
                    goto release_buffer;
                }

                gettimeofday(&start, NULL);

                ret = imresize(src, dst);
                if (ret != IM_STATUS_SUCCESS) {
                    printf("%d, imresize failed! %s\n", __LINE__, imStrError((IM_STATUS)ret));
                    goto release_buffer;
                }

                gettimeofday(&end, NULL);
                usec1 = 1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
                printf("resizing .... cost time %ld us\n", usec1);

                break;

            case MODE_CROP :      //rgaImDemo --crop
                src_rect.x      = 100;
                src_rect.y      = 100;
                src_rect.width  = 300;
                src_rect.height = 300;

                ret = imcheck(src, dst, src_rect, dst_rect, IM_CROP);
                if (ret != IM_STATUS_NOERROR) {
                    printf("%d, check error! %s\n", __LINE__, imStrError((IM_STATUS)ret));
                    goto release_buffer;
                }

                gettimeofday(&start, NULL);

                ret = imcrop(src, dst, src_rect);
                if (ret != IM_STATUS_SUCCESS) {
                    printf("%d, imcrop failed! %s\n", __LINE__, imStrError((IM_STATUS)ret));
                    goto release_buffer;
                }

                gettimeofday(&end, NULL);
                usec1 = 1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
                printf("cropping .... cost time %ld us\n", usec1);

                break;

            case MODE_ROTATE :    //rgaImDemo --rotate=90/180/270
                rotate = (IM_USAGE)parm_data[MODE_ROTATE];

                if (IM_HAL_TRANSFORM_ROT_90 ==  rotate || IM_HAL_TRANSFORM_ROT_270 == rotate) {
                    dst.width   = src.height;
                    dst.height  = src.width;
                    dst.wstride = src.hstride;
                    dst.hstride = src.wstride;
                }

                ret = imcheck(src, dst, src_rect, dst_rect, rotate);
                if (ret != IM_STATUS_NOERROR) {
                    printf("%d, check error! %s\n", __LINE__, imStrError((IM_STATUS)ret));
                    goto release_buffer;
                }

                gettimeofday(&start, NULL);

                ret = imrotate(src, dst, rotate);
                if (ret != IM_STATUS_SUCCESS) {
                    printf("%d, imrotate failed! %s\n", __LINE__, imStrError((IM_STATUS)ret));
                    goto release_buffer;
                }

                gettimeofday(&end, NULL);
                usec1 = 1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
                printf("rotating .... cost time %ld us\n", usec1);

                break;

            case MODE_FLIP :      //rgaImDemo --flip=H/V
                flip = (IM_USAGE)parm_data[MODE_FLIP];

                ret = imcheck(src, dst, src_rect, dst_rect);
                if (ret != IM_STATUS_NOERROR) {
                    printf("%d, check error! %s\n", __LINE__, imStrError((IM_STATUS)ret));
                    goto release_buffer;
                }

                gettimeofday(&start, NULL);

                ret = imflip(src, dst, flip);
                if (ret != IM_STATUS_SUCCESS) {
                    printf("%d, imflip failed! %s\n", __LINE__, imStrError((IM_STATUS)ret));
                    goto release_buffer;
                }

                gettimeofday(&end, NULL);
                usec1 = 1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
                printf("flipping .... cost time %ld us\n", usec1);

                break;

            case MODE_TRANSLATE : //rgaImDemo --translate
                src_rect.x = 300;
                src_rect.y = 300;

                ret = imcheck(src, dst, src_rect, dst_rect);
                if (ret != IM_STATUS_NOERROR) {
                    printf("%d, check error! %s\n", __LINE__, imStrError((IM_STATUS)ret));
                    goto release_buffer;
                }

                gettimeofday(&start, NULL);

                ret = imtranslate(src, dst, src_rect.x, src_rect.y);
                if (ret != IM_STATUS_SUCCESS) {
                    printf("%d, imtranslate failed! %s\n", __LINE__, imStrError((IM_STATUS)ret));
                    goto release_buffer;
                }

                gettimeofday(&end, NULL);
                usec1 = 1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
                printf("translating .... cost time %ld us\n", usec1);

                break;

            case MODE_BLEND :     //rgaImDemo --blend
                ret = imcheck(src, dst, src_rect, dst_rect);
                if (ret != IM_STATUS_NOERROR) {
                    printf("%d, check error! %s\n", __LINE__, imStrError((IM_STATUS)ret));
                    goto release_buffer;
                }

                gettimeofday(&start, NULL);

                ret = imblend(src, dst, parm_data[MODE_FLIP] | IM_ALPHA_BLEND_PRE_MUL);
                if (ret != IM_STATUS_SUCCESS) {
                    printf("%d, imblend failed! %s\n", __LINE__, imStrError((IM_STATUS)ret));
                    goto release_buffer;
                }

                gettimeofday(&end, NULL);
                usec1 = 1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
                printf("blending .... cost time %ld us\n", usec1);

                break;

            case MODE_CVTCOLOR :  //rgaImDemo --cvtcolor
                ret = imcheck(src, dst, src_rect, dst_rect);
                if (ret != IM_STATUS_NOERROR) {
                    printf("%d, check error! %s\n", __LINE__, imStrError((IM_STATUS)ret));
                    goto release_buffer;
                }

                gettimeofday(&start, NULL);

                ret = imcvtcolor(src, dst, src.format, dst.format);
                if (ret != IM_STATUS_SUCCESS) {
                    printf("%d, imcvtcolor failed! %s\n", __LINE__, imStrError((IM_STATUS)ret));
                    goto release_buffer;
                }

                gettimeofday(&end, NULL);
                usec1 = 1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
                printf("cvtcolor .... cost time %ld us\n", usec1);

                break;

            case MODE_FILL :      //rgaImDemo --fill=blue/green/red
                color.value = parm_data[MODE_FILL];

                dst_rect.x      = 100;
                dst_rect.y      = 100;
                dst_rect.width  = 300;
                dst_rect.height = 300;

                ret = imcheck(src, dst, src_rect, dst_rect, IM_COLOR_FILL);
                if (ret != IM_STATUS_NOERROR) {
                    printf("%d, check error! %s\n", __LINE__, imStrError((IM_STATUS)ret));
                    goto release_buffer;
                }

                gettimeofday(&start, NULL);

                ret = imfill(dst, dst_rect, color.value);
                if (ret != IM_STATUS_SUCCESS) {
                    printf("%d, imfill failed! %s\n", __LINE__, imStrError((IM_STATUS)ret));
                    goto release_buffer;
                }

                gettimeofday(&end, NULL);
                usec1 = 1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
                printf("filling .... cost time %ld us\n", usec1);

                break;

            case MODE_NONE :
                printf("%s, Unknown mode\n", __FUNCTION__);

                break;

            default :
                printf("%s, Invalid mode\n", __FUNCTION__);

                break;
        }

        if (while_time) {
            /* 200ms */
            usleep(200000);
        }
    } while(while_time);

    /* invalid CPU cache */
    if (flush_cache && src_dma_fd && dst_dma_fd) {
        dma_sync_device_to_cpu(src_dma_fd);
        dma_sync_device_to_cpu(dst_dma_fd);
    }

    /********** output buf data to file **********/
    write_image_to_file(dst_buf, LOCAL_FILE_PATH, dst_width, dst_height, dst_format, 0);

release_buffer:
    /********** release rga buffer handle **********/
    releasebuffer_handle(src_handle);
    releasebuffer_handle(dst_handle);

free_buffer:
    if (use_dma_heap) {
        if (src_buf != NULL)
            dma_buf_free(src_buf_size, &src_dma_fd, src_buf);
        if (dst_buf != NULL)
            dma_buf_free(dst_buf_size, &dst_dma_fd, dst_buf);
    } else {
        if (src_buf != NULL) {
            free(src_buf);
            src_buf = NULL;
        }

        if (dst_buf != NULL) {
            free(dst_buf);
            dst_buf = NULL;
        }
    }

    return ret;
}

