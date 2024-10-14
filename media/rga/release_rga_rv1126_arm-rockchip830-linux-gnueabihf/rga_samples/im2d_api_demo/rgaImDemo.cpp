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

#ifdef ANDROID
#include <ui/GraphicBuffer.h>
#if defined(ANDROID_12) || defined(USE_HARDWARE_ROCKCHIP)
#include <hardware/hardware_rockchip.h>
#endif
#endif

#include "im2d.hpp"
#include "RockchipRga.h"
#include "RgaUtils.h"
#include "args.h"

#define ERROR               -1

/********** SrcInfo set **********/
#define SRC_WIDTH  1280
#define SRC_HEIGHT 720

#ifdef ANDROID
#define SRC_FORMAT HAL_PIXEL_FORMAT_RGBA_8888
#endif
#ifdef LINUX
#define SRC_FORMAT RK_FORMAT_RGBA_8888
#endif
/********** DstInfo set **********/
#define DST_WIDTH  1280
#define DST_HEIGHT 720

#ifdef ANDROID
#define DST_FORMAT HAL_PIXEL_FORMAT_RGBA_8888
#endif
#ifdef LINUX
#define DST_FORMAT RK_FORMAT_RGBA_8888
#endif

struct timeval start, end;
long usec1;

#ifdef ANDROID
enum {
    FILL_BUFF  = 0,
    EMPTY_BUFF = 1
};

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

sp<GraphicBuffer> GraphicBuffer_Init(int width, int height,int format) {
    uint64_t usage = 0;

    /* cacheable */
    // usage |= GRALLOC_USAGE_SW_READ_OFTEN;
    usage |= RK_GRALLOC_USAGE_WITHIN_4G;

    sp<GraphicBuffer> gb(new GraphicBuffer(width, height, format, 0, usage));

    if (gb->initCheck()) {
        printf("GraphicBuffer check error : %s\n",strerror(errno));
        return NULL;
    } else
        printf("GraphicBuffer check %s \n","ok");

    return gb;
}

/********** write data to buffer or init buffer**********/
int GraphicBuffer_Fill(sp<GraphicBuffer> gb, int flag, int index) {
    int ret;
    char* buf = NULL;
    ret = gb->lock(GRALLOC_USAGE_SW_WRITE_OFTEN, (void**)&buf);

    if (ret) {
        printf("lock buffer error : %s\n",strerror(errno));
        return ERROR;
    } else
        printf("lock buffer %s \n","ok");

    if(flag)
        memset(buf,0x00,gb->getPixelFormat()*gb->getWidth()*gb->getHeight());
    else {
        ret = get_buf_from_file(buf, gb->getPixelFormat(), gb->getWidth(), gb->getHeight(), index);
        if (!ret)
            printf("open file %s \n", "ok");
        else {
            printf ("open file %s \n", "fault");
            return ERROR;
        }
    }

    ret = gb->unlock();
    if (ret) {
        printf("unlock buffer error : %s\n",strerror(errno));
        return ERROR;
    } else
        printf("unlock buffer %s \n","ok");

    return 0;
}

#if USE_AHARDWAREBUFFER
int AHardwareBuffer_Init(int width, int height, int format, AHardwareBuffer** outBuffer) {
    sp<GraphicBuffer> gbuffer;
    gbuffer = GraphicBuffer_Init(width, height, format);
    if(gbuffer == NULL) {
        return ERROR;
    }

    *outBuffer = reinterpret_cast<AHardwareBuffer*>(gbuffer.get());
    // Ensure the buffer doesn't get destroyed when the sp<> goes away.
    AHardwareBuffer_acquire(*outBuffer);
    printf("AHardwareBuffer init ok!\n");
    return 0;
}

int AHardwareBuffer_Fill(AHardwareBuffer** buffer, int flag, int index) {
    sp<GraphicBuffer> gbuffer;

    gbuffer = reinterpret_cast<GraphicBuffer*>(*buffer);

    if(ERROR == GraphicBuffer_Fill(gbuffer, flag, index)) {
        printf("%s, write Graphicbuffer error!\n", __FUNCTION__);
        return ERROR;
    }

    *buffer = reinterpret_cast<AHardwareBuffer*>(gbuffer.get());
    // Ensure the buffer doesn't get destroyed when the sp<> goes away.

    AHardwareBuffer_acquire(*buffer);
    printf("AHardwareBuffer %s ok!\n", flag==0?"fill":"empty");
    return 0;
}

void AHardwareBuffer_Deinit(AHardwareBuffer* buffer) {
    AHardwareBuffer_release(buffer);
}

#endif
#endif

int main(int argc, char*  argv[]) {
    int ret = 0, while_time = 0;
    int parm_data[MODE_MAX] = {0};

    int               COLOR;
    IM_USAGE          ROTATE;
    IM_USAGE          FLIP;

    int               MODE;
    IM_INFORMATION    IM_INFO;
    IM_STATUS         STATUS;

    im_rect         src_rect;
    im_rect         dst_rect;
    rga_buffer_t     src;
    rga_buffer_t     dst;
    rga_buffer_handle_t src_handle;
    rga_buffer_handle_t dst_handle;

#ifdef ANDROID
#if USE_AHARDWAREBUFFER
    AHardwareBuffer* src_buf = nullptr;
    AHardwareBuffer* dst_buf = nullptr;
#else
    sp<GraphicBuffer> src_buf;
    sp<GraphicBuffer> dst_buf;
#endif
#endif

#ifdef LINUX
    char* src_buf = NULL;
    char* dst_buf = NULL;
#endif

    memset(&src_rect, 0, sizeof(src_rect));
    memset(&dst_rect, 0, sizeof(dst_rect));
    memset(&src, 0, sizeof(src));
    memset(&dst, 0, sizeof(dst));

    MODE = readArguments(argc, argv, parm_data);
    if (MODE & WHILE_FLAG) {
        /* Remove flag of MODE_WHILE. */
        MODE &= ~WHILE_FLAG;

        while_time = parm_data[MODE_WHILE];
    }
    printf("MODE = %x\n", MODE);
    if(MODE_NONE == MODE) {
        printf("%s, Unknow RGA mode\n", __FUNCTION__);
        return ERROR;
    }

    /********** Get parameters **********/
    if(MODE != MODE_QUERYSTRING) {
#ifdef ANDROID
#if USE_AHARDWAREBUFFER
        if(ERROR == AHardwareBuffer_Init(SRC_WIDTH, SRC_HEIGHT, SRC_FORMAT, &src_buf)) {
            printf("AHardwareBuffer init error!\n");
            return ERROR;
        }
        if(ERROR == AHardwareBuffer_Init(DST_WIDTH, DST_HEIGHT, DST_FORMAT, &dst_buf)) {
            printf("AHardwareBuffer init error!\n");
            return ERROR;
        }

        if(ERROR == AHardwareBuffer_Fill(&src_buf, FILL_BUFF, 0)) {
            printf("%s, write AHardwareBuffer error!\n", __FUNCTION__);
            return -1;
        }
        if(MODE == MODE_BLEND || MODE == MODE_FILL) {
            if(ERROR == AHardwareBuffer_Fill(&dst_buf, FILL_BUFF, 1)) {
                printf("%s, write AHardwareBuffer error!\n", __FUNCTION__);
                return ERROR;
            }
        } else {
            if(ERROR == AHardwareBuffer_Fill(&dst_buf, EMPTY_BUFF, 1)) {
                printf("%s, write AHardwareBuffer error!\n", __FUNCTION__);
                return ERROR;
            }
        }

        src_handle = importbuffer_AHardwareBuffer(src_buf);
        if (src_handle <= 0) {
            printf("Failed to import AHardwareBuffer for src channel!\n");
            return ERROR;
        }
        dst_handle = importbuffer_AHardwareBuffer(dst_buf);
        if (dst_handle <= 0) {
            printf("Failed to import AHardwareBuffer for dst channel!\n");
            return ERROR;
        }
#else
        src_buf = GraphicBuffer_Init(SRC_WIDTH, SRC_HEIGHT, SRC_FORMAT);
        dst_buf = GraphicBuffer_Init(DST_WIDTH, DST_HEIGHT, DST_FORMAT);
        if (src_buf == NULL || dst_buf == NULL) {
            printf("GraphicBuff init error!\n");
            return ERROR;
        }

        if(ERROR == GraphicBuffer_Fill(src_buf, FILL_BUFF, 0)) {
            printf("%s, write Graphicbuffer error!\n", __FUNCTION__);
            return -1;
        }
        if(MODE == MODE_BLEND || MODE == MODE_FILL) {
            if(ERROR == GraphicBuffer_Fill(dst_buf, FILL_BUFF, 1)) {
                printf("%s, write Graphicbuffer error!\n", __FUNCTION__);
                return ERROR;
            }
        } else {
            if(ERROR == GraphicBuffer_Fill(dst_buf, EMPTY_BUFF, 1)) {
                printf("%s, write Graphicbuffer error!\n", __FUNCTION__);
                return ERROR;
            }
        }

        src_handle = importbuffer_GraphicBuffer(src_buf);
        if (src_handle <= 0) {
            printf("Failed to import GraphicBuffer for src channel!\n");
            return ERROR;
        }
        dst_handle = importbuffer_GraphicBuffer(dst_buf);
        if (dst_handle <= 0) {
            printf("Failed to import GraphicBuffer for dst channel!\n");
            return ERROR;
        }
#endif
#elif LINUX
        src_buf = (char*)malloc(SRC_WIDTH*SRC_HEIGHT*get_bpp_from_format(SRC_FORMAT));
        dst_buf = (char*)malloc(DST_WIDTH*DST_HEIGHT*get_bpp_from_format(DST_FORMAT));

        ret = get_buf_from_file(src_buf, SRC_FORMAT, SRC_WIDTH, SRC_HEIGHT, 0);
        if (!ret)
            printf("open file\n");
        else
            printf ("can not open file\n");

        if(MODE == MODE_BLEND || MODE == MODE_FILL) {
            ret = get_buf_from_file(dst_buf, DST_FORMAT, DST_WIDTH, DST_HEIGHT, 1);
            if (!ret)
                printf("open file\n");
            else
                printf ("can not open file\n");
        } else {
            memset(dst_buf,0x00,DST_WIDTH*DST_HEIGHT*get_bpp_from_format(DST_FORMAT));
        }

        src_handle = importbuffer_virtualaddr(src_buf, SRC_WIDTH, SRC_HEIGHT, SRC_FORMAT);
        if (src_handle <= 0) {
            printf("Failed to import virtualaddr for src channel!\n");
            return ERROR;
        }
        dst_handle = importbuffer_virtualaddr(dst_buf, DST_WIDTH, DST_HEIGHT, DST_FORMAT);
        if (dst_handle <= 0) {
            printf("Failed to import virtualaddr for dst channel!\n");
            return ERROR;
        }
#endif

        src = wrapbuffer_handle(src_handle, SRC_WIDTH, SRC_HEIGHT, SRC_FORMAT);
        dst = wrapbuffer_handle(dst_handle, DST_WIDTH, DST_HEIGHT, DST_FORMAT);
        if(src.width == 0 || dst.width == 0) {
            printf("%s, %s", __FUNCTION__, imStrError());
            return ERROR;
        }
    }

    do {
        if (while_time) {
            static int while_num = 1;
            printf("This is %d time in the loop\n", while_num);

            while_num++;
            while_time--;
        }
        /********** Execution function according to mode **********/
        switch(MODE) {
            case MODE_QUERYSTRING :

                IM_INFO = (IM_INFORMATION)parm_data[MODE_QUERYSTRING];
                printf("\n%s\n", querystring(IM_INFO));

                break;

            case MODE_COPY :      //rgaImDemo --copy

                ret = imcheck(src, dst, src_rect, dst_rect);
                if (IM_STATUS_NOERROR != ret) {
                    printf("%d, check error! %s\n", __LINE__, imStrError((IM_STATUS)ret));
                    return -1;
                }
                gettimeofday(&start, NULL);

                STATUS = imcopy(src, dst);

                gettimeofday(&end, NULL);
                usec1 = 1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
                printf("copying .... cost time %ld us, %s\n", usec1, imStrError(STATUS));

                break;

            case MODE_RESIZE :    //rgaImDemo --resize=up/down
                releasebuffer_handle(dst_handle);
                dst_handle = -1;

                switch(parm_data[MODE_RESIZE]) {
                    case IM_UP_SCALE :
#ifdef ANDROID
#if USE_AHARDWAREBUFFER
                        if(ERROR == AHardwareBuffer_Init(1920, 1080, DST_FORMAT, &dst_buf)) {
                            printf("AHardwareBuffer init error!\n");
                            return ERROR;
                        }

                        if(ERROR == AHardwareBuffer_Fill(&dst_buf, EMPTY_BUFF, 0)) {
                            printf("%s, write AHardwareBuffer error!\n", __FUNCTION__);
                            return ERROR;
                        }

                        dst_handle = importbuffer_AHardwareBuffer(dst_buf);
                        if (dst_handle <= 0) {
                            printf("Failed to import AHardwareBuffer for dst channel!\n");
                            return ERROR;
                        }
#else
                        dst_buf = GraphicBuffer_Init(1920, 1080, DST_FORMAT);
                        if (dst_buf == NULL) {
                            printf("dst GraphicBuff init error!\n");
                            return ERROR;
                        }
                        if(ERROR == GraphicBuffer_Fill(dst_buf, EMPTY_BUFF, 1)) {
                            printf("%s, write Graphicbuffer error!\n", __FUNCTION__);
                            return ERROR;
                        }

                        dst_handle = importbuffer_GraphicBuffer(dst_buf);
                        if (dst_handle <= 0) {
                            printf("Failed to import GraphicBuffer for dst channel!\n");
                            return ERROR;
                        }
#endif
#elif LINUX
                        if (dst_buf != NULL) {
                            free(dst_buf);
                            dst_buf = NULL;
                        }
                        dst_buf = (char*)malloc(1920*1080*get_bpp_from_format(DST_FORMAT));

                        memset(dst_buf,0x00,1920*1080*get_bpp_from_format(DST_FORMAT));

                        dst_handle = importbuffer_virtualaddr(dst_buf, 1920, 1080, DST_FORMAT);
                        if (dst_handle <= 0) {
                            printf("Failed to import virtualaddr for dst channel!\n");
                            return ERROR;
                        }
#endif
                        dst = wrapbuffer_handle(dst_handle, 1920, 1080, DST_FORMAT);
                        if(dst.width == 0) {
                            printf("%s, %s\n", __FUNCTION__, imStrError());
                            return ERROR;
                        }

                        break;
                    case IM_DOWN_SCALE :

#ifdef ANDROID
#if USE_AHARDWAREBUFFER
                        if(ERROR == AHardwareBuffer_Init(720, 480, DST_FORMAT, &dst_buf)) {
                            printf("AHardwareBuffer init error!\n");
                            return ERROR;
                        }

                        if(ERROR == AHardwareBuffer_Fill(&dst_buf, EMPTY_BUFF, 0)) {
                            printf("%s, write AHardwareBuffer error!\n", __FUNCTION__);
                            return ERROR;
                        }

                        dst_handle = importbuffer_AHardwareBuffer(dst_buf);
                        if (dst_handle <= 0) {
                            printf("Failed to import AHardwareBuffer for dst channel!\n");
                            return ERROR;
                        }
#else
                        dst_buf = GraphicBuffer_Init(720, 480, DST_FORMAT);
                        if (dst_buf == NULL) {
                            printf("dst GraphicBuff init error!\n");
                            return ERROR;
                        }
                        if(ERROR == GraphicBuffer_Fill(dst_buf, EMPTY_BUFF, 1)) {
                            printf("%s, write Graphicbuffer error!\n", __FUNCTION__);
                            return ERROR;
                        }

                        dst_handle = importbuffer_GraphicBuffer(dst_buf);
                        if (dst_handle <= 0) {
                            printf("Failed to import GraphicBuffer for dst channel!\n");
                            return ERROR;
                        }
#endif
#elif LINUX
                        if (dst_buf != NULL) {
                            free(dst_buf);
                            dst_buf = NULL;
                        }
                        dst_buf = (char*)malloc(720*480*get_bpp_from_format(DST_FORMAT));

                        memset(dst_buf,0x00,720*480*get_bpp_from_format(DST_FORMAT));

                        dst_handle = importbuffer_virtualaddr(dst_buf, 720, 480, DST_FORMAT);
                        if (dst_handle <= 0) {
                            printf("Failed to import virtualaddr for dst channel!\n");
                            return ERROR;
                        }
#endif
                        dst = wrapbuffer_handle(dst_handle, 720, 480, DST_FORMAT);
                        if(dst.width == 0) {
                            printf("%s, %s\n", __FUNCTION__, imStrError());
                            return ERROR;
                        }
                        break;
                }

                ret = imcheck(src, dst, src_rect, dst_rect);
                if (IM_STATUS_NOERROR != ret) {
                    printf("%d, check error! %s\n", __LINE__, imStrError((IM_STATUS)ret));
                    return -1;
                }

                gettimeofday(&start, NULL);

                STATUS = imresize(src, dst);

                gettimeofday(&end, NULL);
                usec1 = 1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
                printf("resizing .... cost time %ld us, %s\n", usec1, imStrError(STATUS));

                break;

            case MODE_CROP :      //rgaImDemo --crop

                src_rect.x      = 100;
                src_rect.y      = 100;
                src_rect.width  = 300;
                src_rect.height = 300;

                ret = imcheck(src, dst, src_rect, dst_rect, IM_CROP);
                if (IM_STATUS_NOERROR != ret) {
                    printf("%d, check error! %s\n", __LINE__, imStrError((IM_STATUS)ret));
                    return -1;
                }

                gettimeofday(&start, NULL);

                STATUS = imcrop(src, dst, src_rect);

                gettimeofday(&end, NULL);
                usec1 = 1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
                printf("cropping .... cost time %ld us, %s\n", usec1, imStrError(STATUS));

                break;

            case MODE_ROTATE :    //rgaImDemo --rotate=90/180/270

                ROTATE = (IM_USAGE)parm_data[MODE_ROTATE];

                if (IM_HAL_TRANSFORM_ROT_90 ==  ROTATE || IM_HAL_TRANSFORM_ROT_270 == ROTATE) {
                    dst.width   = src.height;
                    dst.height  = src.width;
                    dst.wstride = src.hstride;
                    dst.hstride = src.wstride;
                }

                ret = imcheck(src, dst, src_rect, dst_rect, ROTATE);
                if (IM_STATUS_NOERROR != ret) {
                    printf("%d, check error! %s\n", __LINE__, imStrError((IM_STATUS)ret));
                    return -1;
                }

                gettimeofday(&start, NULL);

                STATUS = imrotate(src, dst, ROTATE);

                gettimeofday(&end, NULL);
                usec1 = 1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
                printf("rotating .... cost time %ld us, %s\n", usec1, imStrError(STATUS));

                break;

            case MODE_FLIP :      //rgaImDemo --flip=H/V

                FLIP = (IM_USAGE)parm_data[MODE_FLIP];

                ret = imcheck(src, dst, src_rect, dst_rect);
                if (IM_STATUS_NOERROR != ret) {
                    printf("%d, check error! %s\n", __LINE__, imStrError((IM_STATUS)ret));
                    return -1;
                }

                gettimeofday(&start, NULL);

                STATUS = imflip(src, dst, FLIP);

                gettimeofday(&end, NULL);
                usec1 = 1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
                printf("flipping .... cost time %ld us, %s\n", usec1, imStrError(STATUS));

                break;

            case MODE_TRANSLATE : //rgaImDemo --translate

                src_rect.x = 300;
                src_rect.y = 300;

                ret = imcheck(src, dst, src_rect, dst_rect);
                if (IM_STATUS_NOERROR != ret) {
                    printf("%d, check error! %s\n", __LINE__, imStrError((IM_STATUS)ret));
                    return -1;
                }

                gettimeofday(&start, NULL);

                STATUS = imtranslate(src, dst, src_rect.x, src_rect.y);

                gettimeofday(&end, NULL);
                usec1 = 1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
                printf("translating .... cost time %ld us, %s\n", usec1, imStrError(STATUS));

                break;

            case MODE_BLEND :     //rgaImDemo --blend

                ret = imcheck(src, dst, src_rect, dst_rect);
                if (IM_STATUS_NOERROR != ret) {
                    printf("%d, check error! %s\n", __LINE__, imStrError((IM_STATUS)ret));
                    return -1;
                }

                gettimeofday(&start, NULL);

                STATUS = imblend(src, dst);

                gettimeofday(&end, NULL);
                usec1 = 1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
                printf("blending .... cost time %ld us, %s\n", usec1, imStrError(STATUS));

                break;

            case MODE_CVTCOLOR :  //rgaImDemo --cvtcolor

#ifdef ANDROID
                src.format = HAL_PIXEL_FORMAT_RGBA_8888;
                dst.format = HAL_PIXEL_FORMAT_YCrCb_NV12;
#endif
#ifdef LINUX
                src.format = RK_FORMAT_RGBA_8888;
                dst.format = RK_FORMAT_YCbCr_420_SP;
#endif

                ret = imcheck(src, dst, src_rect, dst_rect);
                if (IM_STATUS_NOERROR != ret) {
                    printf("%d, check error! %s\n", __LINE__, imStrError((IM_STATUS)ret));
                    return -1;
                }

                gettimeofday(&start, NULL);

                STATUS = imcvtcolor(src, dst, src.format, dst.format);

                gettimeofday(&end, NULL);
                usec1 = 1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
                printf("cvtcolor .... cost time %ld us, %s\n", usec1, imStrError(STATUS));

                break;

            case MODE_FILL :      //rgaImDemo --fill=blue/green/red

                COLOR = parm_data[MODE_FILL];

                dst_rect.x      = 100;
                dst_rect.y      = 100;
                dst_rect.width  = 300;
                dst_rect.height = 300;

                ret = imcheck(src, dst, src_rect, dst_rect, IM_COLOR_FILL);
                if (IM_STATUS_NOERROR != ret) {
                    printf("%d, check error! %s\n", __LINE__, imStrError((IM_STATUS)ret));
                    return -1;
                }

                gettimeofday(&start, NULL);

                STATUS = imfill(dst, dst_rect, COLOR);

                gettimeofday(&end, NULL);
                usec1 = 1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
                printf("filling .... cost time %ld us, %s\n", usec1, imStrError(STATUS));

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
    }while(while_time);

    /********** release rga buffer handle **********/
    releasebuffer_handle(src_handle);
    releasebuffer_handle(dst_handle);

    /********** output buf data to file **********/
#ifdef ANDROID
    char* outbuf = NULL;
#if USE_AHARDWAREBUFFER
    sp<GraphicBuffer> gbuffer = reinterpret_cast<GraphicBuffer*>(dst_buf);
    if (gbuffer != NULL) {
        ret = gbuffer->lock(GRALLOC_USAGE_SW_WRITE_OFTEN | GRALLOC_USAGE_SW_READ_OFTEN, (void**)&outbuf);
        if (ret != 0) {
            printf("%s, %d, lock buffer failed!\n", __FUNCTION__, __LINE__);
            return -1;
        }
        output_buf_data_to_file(outbuf, dst.format, dst.wstride, dst.hstride, 0);
        ret = gbuffer->unlock();
        if (ret != 0) {
            printf("%s, %d, unlock buffer failed!\n", __FUNCTION__, __LINE__);
            return -1;
        }
    }

    AHardwareBuffer_Deinit(src_buf);
    AHardwareBuffer_Deinit(dst_buf);
#else
    if (dst_buf != NULL) {
        ret = dst_buf->lock(GRALLOC_USAGE_SW_WRITE_OFTEN | GRALLOC_USAGE_SW_READ_OFTEN, (void**)&outbuf);
        if (ret != 0) {
            printf("%s, %d, lock buffer failed!\n", __FUNCTION__, __LINE__);
            return -1;
        }
        output_buf_data_to_file(outbuf, dst.format, dst.wstride, dst.hstride, 0);
        ret = dst_buf->unlock();
        if (ret != 0) {
            printf("%s, %d, unlock buffer failed!\n", __FUNCTION__, __LINE__);
            return -1;
        }
    }
#endif
#endif

#ifdef LINUX
    if (src_buf != NULL) {
        free(src_buf);
        src_buf = NULL;
    }

    if (dst_buf != NULL) {
        output_buf_data_to_file(dst_buf, dst.format, dst.wstride, dst.hstride, 0);
        free(dst_buf);
        dst_buf = NULL;
    }
#endif

    return 0;
}

