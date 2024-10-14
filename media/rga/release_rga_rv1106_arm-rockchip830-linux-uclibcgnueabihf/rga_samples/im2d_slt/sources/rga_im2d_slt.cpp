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

#include "im2d.hpp"
#include "RgaUtils.h"

#include "utils.h"
#include "dma_alloc.h"
#include "drm_alloc.h"

#include "slt_config.h"
#include "crc.h"

enum {
    FILL_BUFF  = 0,
    EMPTY_BUFF = 1
};

typedef struct private_data {
    int id;
    const char *name;
    const char *dma_heap_name;
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

static int file_exists(const char* file_name) {
    FILE* file = fopen(file_name, "r");
    if (file != NULL) {
        fclose(file);
        return true;
    }

    return false;
}

enum ERR_NUM {
    slt_error           = -1,
    slt_check_error     = -2,
    slt_rga_error       = -3,
};

struct rga_image_info {
    rga_buffer_t img;
    char *buf;
    int buf_size;
};

typedef int (*rga_slt_case) (private_data_t *, int, struct rga_image_info &, struct rga_image_info &, struct rga_image_info &);

int rga_raster_test(private_data_t *data, int time,
                    struct rga_image_info &src_img,
                    struct rga_image_info &tmp_img,
                    struct rga_image_info &dst_img) {
    int ret;
    int case_index;
    int usage = 0;
    int ori_format;
    unsigned int result_crc = 0;
    unsigned int *crc_golden_table = NULL;

    rga_buffer_t src, dst, tmp;
    char *src_buf, *dst_buf, *tmp_buf;
    int src_buf_size, dst_buf_size, tmp_buf_size;
    im_rect src_rect, tmp_rect, dst_rect;

    memset(&src_rect, 0, sizeof(src_rect));
    memset(&tmp_rect, 0, sizeof(tmp_rect));
    memset(&dst_rect, 0, sizeof(dst_rect));

    src = src_img.img;
    src_buf = src_img.buf;
    src_buf_size = src_img.buf_size;

    tmp = tmp_img.img;
    tmp_buf = tmp_img.buf;
    tmp_buf_size = tmp_img.buf_size;

    dst = dst_img.img;
    dst_buf = dst_img.buf;
    dst_buf_size = dst_img.buf_size;

    if (!IM2D_SLT_GENERATE_CRC) {
        crc_golden_table = read_crcdata_from_file(data->name);
        if (crc_golden_table == NULL) {
            printf("cannot read crc golden table!\n");
            return slt_error;
        }
    }

    {
        /* case: bypass + src-CSC */
        case_index = 0;

        ret = imcvtcolor(src, dst, RK_FORMAT_YCbCr_420_SP, dst.format);
        if (ret != IM_STATUS_SUCCESS) {
            printf("ID[%d]: %s copy %d time running failed! %s\n", data->id, data->name, time, imStrError(ret));
            return slt_rga_error;
        }

        result_crc = crc32(0xffffffff, (unsigned char *)dst_buf, dst_buf_size);
        if(IM2D_SLT_GENERATE_CRC) {
            save_crcdata_to_file(result_crc, data->name, case_index);
        } else {
            if (!crc_check(case_index, result_crc, crc_golden_table))
                goto CHECK_ERROR;
        }

        /* case: 3-channel blend + rotate-180 + H_V mirror + scale-up + dst-CSC */
        case_index++;

        src_rect.x = 100;
        src_rect.y = 100;
        src_rect.width = 480;
        src_rect.height = 320;

        dst_rect.x = 100;
        dst_rect.y = 100;
        dst_rect.width = 720;
        dst_rect.height = 540;

        ori_format = dst.format;
        dst.format = RK_FORMAT_YCbCr_420_SP;

        usage = IM_SYNC | IM_ALPHA_BLEND_SRC_OVER | IM_ALPHA_BLEND_PRE_MUL | IM_HAL_TRANSFORM_ROT_180 | IM_HAL_TRANSFORM_FLIP_H_V;

        ret = improcess(src, dst, tmp, src_rect, dst_rect, dst_rect, usage);
        if (ret != IM_STATUS_SUCCESS) {
            printf("ID[%d]: %s process %d time running failed! %s\n", data->id, data->name, time, imStrError(ret));
            return slt_rga_error;
        }

        result_crc = crc32(0xffffffff, (unsigned char *)dst_buf, dst_buf_size);
        if(IM2D_SLT_GENERATE_CRC) {
            save_crcdata_to_file(result_crc, data->name, case_index);
        } else {
            if (!crc_check(case_index, result_crc, crc_golden_table))
                goto CHECK_ERROR;
        }

        dst.format = ori_format;

        /* case: rotate-90 + H_V mirror + scale-down */
        case_index++;

        dst_rect.x = 100;
        dst_rect.y = 100;
        dst_rect.width = 480;
        dst_rect.height = 320;

        usage = IM_SYNC | IM_HAL_TRANSFORM_ROT_90 | IM_HAL_TRANSFORM_FLIP_H_V;

        ret = improcess(src, dst, {}, {}, dst_rect, {}, usage);
        if (ret != IM_STATUS_SUCCESS) {
            printf("ID[%d]: %s process %d time running failed! %s\n", data->id, data->name, time, imStrError(ret));
            return slt_rga_error;
        }

        result_crc = crc32(0xffffffff, (unsigned char *)dst_buf, dst_buf_size);
        if(IM2D_SLT_GENERATE_CRC) {
            save_crcdata_to_file(result_crc, data->name, case_index);
        } else {
            if (!crc_check(case_index, result_crc, crc_golden_table))
                goto CHECK_ERROR;
        }

        if (data->core == IM_SCHEDULER_RGA2_CORE0 ||
            data->core == IM_SCHEDULER_RGA2_CORE1) {
            /* case: color fill */
            case_index++;

            dst_rect.x = 100;
            dst_rect.y = 100;
            dst_rect.width = 720;
            dst_rect.height = 540;

            ret = imfill(dst, dst_rect, 0xffaabbcc);
            if (ret != IM_STATUS_SUCCESS) {
                printf("ID[%d]: %s fill %d time running failed! %s\n", data->id, data->name, time, imStrError(ret));
                return slt_rga_error;
            }

            result_crc = crc32(0xffffffff, (unsigned char *)dst_buf, dst_buf_size);
            if(IM2D_SLT_GENERATE_CRC) {
                save_crcdata_to_file(result_crc, data->name, case_index);
            } else {
                if (!crc_check(case_index, result_crc, crc_golden_table))
                    goto CHECK_ERROR;
            }
        }
    }

    if (IM2D_SLT_GENERATE_CRC)
        printf("ID[%d]: %s genrate CRC golden %s/%s_%s.txt\n",
               data->id, data->name,
               IM2D_SLT_DEFAULT_GOLDEN_PATH, IM2D_SLT_GENERATE_CRC_GOLDEN_PREFIX, data->name);

    return 0;

CHECK_ERROR:
    printf("ID[%d] loop[%d]: %s case[%d] check-CRC failed! result = %#x, golden = %#x\n",
           data->id, time, data->name, case_index,
           result_crc, crc_golden_table[case_index]);

    return slt_check_error;
}

int rga_special_test(private_data_t *data, int time,
                     struct rga_image_info &src_img,
                     struct rga_image_info &tmp_img,
                     struct rga_image_info &dst_img) {
    int ret;
    int case_index;
    unsigned int result_crc = 0;
    unsigned int *crc_golden_table = NULL;

    rga_buffer_t src, dst, tmp;
    char *src_buf, *dst_buf, *tmp_buf;
    int src_buf_size, dst_buf_size, tmp_buf_size;
    im_rect src_rect, tmp_rect, dst_rect;

    memset(&src_rect, 0, sizeof(src_rect));
    memset(&tmp_rect, 0, sizeof(tmp_rect));
    memset(&dst_rect, 0, sizeof(dst_rect));

    src = src_img.img;
    src_buf = src_img.buf;
    src_buf_size = src_img.buf_size;

    tmp = tmp_img.img;
    tmp_buf = tmp_img.buf;
    tmp_buf_size = tmp_img.buf_size;

    dst = dst_img.img;
    dst_buf = dst_img.buf;
    dst_buf_size = dst_img.buf_size;

    if (!IM2D_SLT_GENERATE_CRC) {
        crc_golden_table = read_crcdata_from_file(data->name);
        if (crc_golden_table == NULL) {
            printf("cannot read crc golden table!\n");
            return slt_error;
        }
    }

    {
        /* case: in */
        case_index = 0;

        ret = imcopy(src, tmp);
        if (ret != IM_STATUS_SUCCESS) {
            printf("ID[%d]: %s input %d time running failed! %s\n", data->id, data->name, time, imStrError(ret));
            return slt_rga_error;
        }

        result_crc = crc32(0xffffffff, (unsigned char *)dst_buf, dst_buf_size);
        if(IM2D_SLT_GENERATE_CRC) {
            save_crcdata_to_file(result_crc, data->name, case_index);
        } else {
            if (!crc_check(case_index, result_crc, crc_golden_table))
                goto CHECK_ERROR;
        }

        /* case: out */
        if (!(data->rd_mode == IM_AFBC32x8_MODE ||
                data->rd_mode == IM_RKFBC64x4_MODE)) {
            case_index++;

            ret = imcopy(tmp, dst);
            if (ret != IM_STATUS_SUCCESS) {
                printf("ID[%d]: %s output %d time running failed! %s\n", data->id, data->name, time, imStrError(ret));
                return slt_rga_error;
            }

            result_crc = crc32(0xffffffff, (unsigned char *)dst_buf, dst_buf_size);
            if(IM2D_SLT_GENERATE_CRC) {
                save_crcdata_to_file(result_crc, data->name, case_index);
            } else {
                if (!crc_check(case_index, result_crc, crc_golden_table))
                    goto CHECK_ERROR;
            }
        }
    }

    if (IM2D_SLT_GENERATE_CRC)
        printf("ID[%d]: %s genrate CRC golden %s/%s_%s.txt\n",
               data->id, data->name,
               IM2D_SLT_DEFAULT_GOLDEN_PATH, IM2D_SLT_GENERATE_CRC_GOLDEN_PREFIX, data->name);

    return 0;

CHECK_ERROR:
    printf("ID[%d] loop[%d]: %s case[%d] check-CRC failed! result = %#x, golden = %#x\n",
           data->id, time, data->name, case_index,
           result_crc, crc_golden_table[case_index]);

    return slt_check_error;
}

int rga_perf_test(private_data_t *data, int time,
                  struct rga_image_info &src_img,
                  struct rga_image_info &tmp_img,
                  struct rga_image_info &dst_img) {
    int ret;

    rga_buffer_t src, dst, tmp;
    char *src_buf, *dst_buf, *tmp_buf;
    int src_buf_size, dst_buf_size, tmp_buf_size;
    im_rect src_rect, tmp_rect, dst_rect;

    memset(&src_rect, 0, sizeof(src_rect));
    memset(&tmp_rect, 0, sizeof(tmp_rect));
    memset(&dst_rect, 0, sizeof(dst_rect));

    src = src_img.img;
    src_buf = src_img.buf;
    src_buf_size = src_img.buf_size;

    tmp = tmp_img.img;
    tmp_buf = tmp_img.buf;
    tmp_buf_size = tmp_img.buf_size;

    dst = dst_img.img;
    dst_buf = dst_img.buf;
    dst_buf_size = dst_img.buf_size;

    {
        ret = imcopy(src, dst);
        if (ret != IM_STATUS_SUCCESS) {
            printf("ID[%d]: %s input %d time running failed! %s\n", data->id, data->name, time, imStrError(ret));
            return slt_rga_error;
        }
    }

    return 0;
}

/******************************************************************************/
static int rga_run(void *args, rga_slt_case running_case) {
    int ret = 0, time = 0;
    int use_dma_heap;
    int fbc_en = false;
    bool tile4x4_en = false;
    unsigned int num;
    int src_width, src_height, src_format;
    int dst_width, dst_height, dst_format;

    int src_buf_size, dst_buf_size, tmp_buf_size;
    char *src_buf = NULL, *dst_buf = NULL, *tmp_buf = NULL;
    int src_dma_fd = -1, dst_dma_fd = -1, tmp_dma_fd = -1;
    rga_buffer_handle_t src_handle = 0, dst_handle = 0, tmp_handle = 0;

    rga_buffer_t src;
    rga_buffer_t tmp;
    rga_buffer_t dst;
    im_rect src_rect;
    im_rect dst_rect;

    struct rga_image_info src_img;
    struct rga_image_info tmp_img;
    struct rga_image_info dst_img;

    private_data_t *data = (private_data_t *)args;

    num = data->num;

    src_width = data->width;
    src_height = data->height;
    src_format = data->format;

    dst_width = data->width;
    dst_height = data->height;
    dst_format = data->format;

    memset(&src, 0x0, sizeof(src));
    memset(&dst, 0x0, sizeof(dst));
    memset(&tmp, 0x0, sizeof(tmp));
    memset(&src_rect, 0x0, sizeof(src_rect));
    memset(&dst_rect, 0x0, sizeof(dst_rect));

    if (data->rd_mode == IM_AFBC16x16_MODE ||
        data->rd_mode == IM_AFBC32x8_MODE ||
        data->rd_mode == IM_RKFBC64x4_MODE)
        fbc_en = true;

    if (data->rd_mode == IM_TILE4x4_MODE)
        tile4x4_en = true;

    src_buf_size = src_width * src_height * get_bpp_from_format(src_format);
    dst_buf_size = dst_width * dst_height * get_bpp_from_format(dst_format);
    if (fbc_en) {
        src_buf_size = src_buf_size * 1.5;
        dst_buf_size = dst_buf_size * 1.5;
    }
    tmp_buf_size = src_buf_size;

    use_dma_heap = (data->dma_heap_name != NULL) && file_exists(data->dma_heap_name);
    if (use_dma_heap) {
        ret = dma_buf_alloc(data->dma_heap_name, src_buf_size, &src_dma_fd, (void **)&src_buf);
        if (ret < 0) {
            printf("alloc src dma_heap buffer failed!\n");
            goto RELEASE_BUFFER;
        }

        ret = dma_buf_alloc(data->dma_heap_name, tmp_buf_size, &tmp_dma_fd, (void **)&tmp_buf);
        if (ret < 0) {
            printf("alloc tmp dma_heap buffer failed!\n");
            goto RELEASE_BUFFER;
        }

        ret = dma_buf_alloc(data->dma_heap_name, dst_buf_size, &dst_dma_fd, (void **)&dst_buf);
        if (ret < 0) {
            printf("alloc dst dma_heap buffer failed!\n");
            goto RELEASE_BUFFER;
        }

        src_handle = importbuffer_fd(src_dma_fd, src_buf_size);
        if (src_handle <= 0) {
            printf("ID[%d] %s import src dma_buf failed!\n", data->id, data->name);
            goto RELEASE_BUFFER;
        }

        tmp_handle = importbuffer_fd(tmp_dma_fd, tmp_buf_size);
        if (tmp_handle <= 0) {
            printf("ID[%d] %s import tmp dma_buf failed!\n", data->id, data->name);
            goto RELEASE_BUFFER;
        }

        dst_handle = importbuffer_fd(dst_dma_fd, dst_buf_size);
        if (dst_handle <= 0) {
            printf("ID[%d] %s import dst dma_buf failed!\n", data->id, data->name);
            goto RELEASE_BUFFER;
        }
    } else {
        src_buf = (char *)malloc(src_buf_size);
        tmp_buf = (char *)malloc(tmp_buf_size);
        dst_buf = (char *)malloc(dst_buf_size);
        if (src_buf == NULL || tmp_buf == NULL || dst_buf == NULL) {
            printf("malloc fault!\n");
            goto RELEASE_BUFFER;
        }

        src_handle = importbuffer_virtualaddr(src_buf, src_buf_size);
        if (src_handle <= 0) {
            printf("ID[%d] %s import src virt_addr failed!\n", data->id, data->name);
            goto RELEASE_BUFFER;
        }

        tmp_handle = importbuffer_virtualaddr(tmp_buf, tmp_buf_size);
        if (tmp_handle <= 0) {
            printf("ID[%d] %s import tmp virt_addr failed!\n", data->id, data->name);
            goto RELEASE_BUFFER;
        }

        dst_handle = importbuffer_virtualaddr(dst_buf, dst_buf_size);
        if (dst_handle <= 0) {
            printf("ID[%d] %s import dst virt_addr failed!\n", data->id, data->name);
            goto RELEASE_BUFFER;
        }
    }

    if (fbc_en) {
        ret = read_image_from_fbc_file(src_buf, IM2D_SLT_DEFAULT_INPUT_PATH,
                                       src_width, src_height, src_format, 0);
        if (ret < 0)
            goto RELEASE_BUFFER;
    } else {
        ret = read_image_from_file(src_buf, IM2D_SLT_DEFAULT_INPUT_PATH,
                                   src_width, src_height, src_format, 0);
        if (ret < 0)
            goto RELEASE_BUFFER;
    }
    memset(tmp_buf, 0x22, tmp_buf_size);
    memset(dst_buf, 0x33, dst_buf_size);

    src = wrapbuffer_handle(src_handle, src_width, src_height, src_format);
    tmp = wrapbuffer_handle(tmp_handle, src_width, src_height, src_format);
    dst = wrapbuffer_handle(dst_handle, dst_width, dst_height, dst_format);
    if (src.width == 0 || tmp.width == 0 || dst.width == 0) {
        printf("warpbuffer failed, %s\n", imStrError());
        goto RELEASE_BUFFER;
    }

    src.rd_mode = data->rd_mode;
    dst.rd_mode = data->rd_mode;

    if (data->core != IM_SCHEDULER_DEFAULT)
        imconfig(IM_CONFIG_SCHEDULER_CORE, data->core);
    imconfig(IM_CONFIG_PRIORITY, data->priority);

    src_img.img = src;
    src_img.buf = src_buf;
    src_img.buf_size = src_buf_size;

    tmp_img.img = tmp;
    tmp_img.buf = tmp_buf;
    tmp_img.buf_size = tmp_buf_size;

    dst_img.img = dst;
    dst_img.buf = dst_buf;
    dst_img.buf_size = dst_buf_size;

    do {
        time++;

        ret = running_case(data, time,
                           src_img, tmp_img, dst_img);
        switch (ret) {
            case slt_error:
                goto RELEASE_BUFFER;
            case slt_check_error:
                goto CHECK_FAILED;
            case slt_rga_error:
                goto RUNNING_FAILED;
        }
    } while (data->mode && --num);

    printf("ID[%d]: %s running success!\n", data->id, data->name);

    ret = 0;
    goto RELEASE_BUFFER;

CHECK_FAILED:
RUNNING_FAILED:
    printf("src: %#x %#x %#x %#x\n", (int)src_buf[0], (int)src_buf[1], (int)src_buf[2], (int)src_buf[3]);
    printf("tmp: %#x %#x %#x %#x\n", (int)tmp_buf[0], (int)tmp_buf[1], (int)tmp_buf[2], (int)tmp_buf[3]);
    printf("dst: %#x %#x %#x %#x\n", (int)dst_buf[0], (int)dst_buf[1], (int)dst_buf[2], (int)dst_buf[3]);
    if (fbc_en) {
        write_image_to_fbc_file(src_buf, IM2D_SLT_DEFAULT_OUTPUT_PATH,
                                src.wstride, src.hstride, src.format, data->id * 10 + 1);
        write_image_to_file(tmp_buf, IM2D_SLT_DEFAULT_OUTPUT_PATH,
                            tmp.wstride, tmp.hstride, tmp.format, data->id * 10 + 2);
        write_image_to_fbc_file(dst_buf, IM2D_SLT_DEFAULT_OUTPUT_PATH,
                                dst.wstride, dst.hstride, dst.format, data->id * 10 + 3);
    } else {
        write_image_to_file(src_buf, IM2D_SLT_DEFAULT_OUTPUT_PATH,
                            src.wstride, src.hstride, src.format, data->id * 10 + 1);
        write_image_to_file(tmp_buf, IM2D_SLT_DEFAULT_OUTPUT_PATH,
                            tmp.wstride, tmp.hstride, tmp.format, data->id * 10 + 2);
        write_image_to_file(dst_buf, IM2D_SLT_DEFAULT_OUTPUT_PATH,
                            dst.wstride, dst.hstride, dst.format, data->id * 10 + 3);
    }

RELEASE_BUFFER:
    if (src_handle)
        releasebuffer_handle(src_handle);
    if (tmp_handle)
        releasebuffer_handle(tmp_handle);
    if (dst_handle)
        releasebuffer_handle(dst_handle);

    if (use_dma_heap) {
        if (src_dma_fd > 0 && src_buf != NULL)
            dma_buf_free(src_buf_size, &src_dma_fd, (void *)src_buf);
        if (tmp_dma_fd > 0 && tmp_buf != NULL)
            dma_buf_free(tmp_buf_size, &tmp_dma_fd, (void *)tmp_buf);
        if (dst_dma_fd > 0 && dst_buf != NULL)
            dma_buf_free(dst_buf_size, &dst_dma_fd, (void *)dst_buf);
    } else {
        printf("yqw, --debug, ID[%d], start free\n", data->id);
        if (src_buf != NULL)
            free(src_buf);
        if (tmp_buf != NULL)
            free(tmp_buf);
        if (dst_buf != NULL)
            free(dst_buf);
        printf("yqw, --debug, ID[%d], end free\n", data->id);
    }

    return ret;
}

void *pthread_rga_raster_func(void *args) {
    private_data_t *data = (private_data_t *)args;

    data->result = rga_run(args, rga_raster_test);

#if IM2D_SLT_THREAD_EN
    pthread_exit(NULL);
#else
    return NULL;
#endif
}

void *pthread_rga_special_func(void *args) {
    private_data_t *data = (private_data_t *)args;

    data->result = rga_run(args, rga_special_test);

#if IM2D_SLT_THREAD_EN
    pthread_exit(NULL);
#else
    return NULL;
#endif
}

void *pthread_rga_perf_func(void *args) {
    private_data_t *data = (private_data_t *)args;

    data->result = rga_run(args, rga_perf_test);

#if IM2D_SLT_THREAD_EN
    pthread_exit(NULL);
#else
    return NULL;
#endif
}

static int run_test(int pthread_num, private_data_t *data, void *(*test_func)(void *)) {
#if IM2D_SLT_THREAD_EN
    pthread_t tdSyncID[IM2D_SLT_THREAD_MAX];

    for (int i = 1; i <= pthread_num; i++) {
        pthread_create(&tdSyncID[i], NULL, test_func, (void *)(&data[i]));
        printf("creat Sync pthread[0x%lx] = %d, id = %d\n", tdSyncID[i], i, data[i].id);
    }

    for (int i = 1; i <= pthread_num; i++) {
        pthread_join(tdSyncID[i], NULL);
        if (data[i].result < 0) {
            printf("ID[%d] case '%s' is faile!\n", data[i].id, data[i].name);
            return -1;
        }
    }
#else
    for (int i = 1; i <= pthread_num; i++) {
        test_func((void *)(&data[i]));
        printf("ID[%d] %s run end!\n", data[i].id, data[i].name);
        if (data[i].result < 0) {
            printf("ID[%d] case '%s' is faile!\n", data[i].id, data[i].name);
            return -1;
        }
    }
#endif

    return 0;
}

int main() {
    int pthread_num = 0;
    private_data_t data[IM2D_SLT_THREAD_MAX];

    memset(&data, 0x0, sizeof(private_data_t) * IM2D_SLT_THREAD_MAX);
    printf("-------------------------------------------------\n");

#if IM2D_SLT_TEST_RGA3_0_EN
    pthread_num++;
    data[pthread_num].id = pthread_num;
    data[pthread_num].name = "RGA3_core0";
    data[pthread_num].dma_heap_name = IM2D_SLT_DMA_HEAP_PATH;
    data[pthread_num].mode = false;
    data[pthread_num].num = 0;
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
    data[pthread_num].dma_heap_name = IM2D_SLT_DMA_HEAP_PATH;
    data[pthread_num].mode = false;
    data[pthread_num].num = 0;
    data[pthread_num].width = IM2D_SLT_DEFAULT_WIDTH;
    data[pthread_num].height = IM2D_SLT_DEFAULT_HEIGHT;
    data[pthread_num].format = IM2D_SLT_DEFAULT_FORMAT;
    data[pthread_num].rd_mode = IM_RASTER_MODE;
    data[pthread_num].core = IM_SCHEDULER_RGA3_CORE1;
    data[pthread_num].priority = 1;
#endif

#if IM2D_SLT_TEST_RGA2_0_EN
    pthread_num++;
    data[pthread_num].id = pthread_num;
    data[pthread_num].name = "RGA2_core0";
    data[pthread_num].dma_heap_name = IM2D_SLT_DMA_HEAP_PATH;
    data[pthread_num].mode = false;
    data[pthread_num].num = 0;
    data[pthread_num].width = IM2D_SLT_DEFAULT_WIDTH;
    data[pthread_num].height = IM2D_SLT_DEFAULT_HEIGHT;
    data[pthread_num].format = IM2D_SLT_DEFAULT_FORMAT;
    data[pthread_num].rd_mode = IM_RASTER_MODE;
    data[pthread_num].core = IM_SCHEDULER_RGA2_CORE0;
    data[pthread_num].priority = 1;
#endif

#if IM2D_SLT_TEST_RGA2_1_EN
    pthread_num++;
    data[pthread_num].id = pthread_num;
    data[pthread_num].name = "RGA2_core1";
    data[pthread_num].dma_heap_name = IM2D_SLT_DMA_HEAP_PATH;
    data[pthread_num].mode = false;
    data[pthread_num].num = 0;
    data[pthread_num].width = IM2D_SLT_DEFAULT_WIDTH;
    data[pthread_num].height = IM2D_SLT_DEFAULT_HEIGHT;
    data[pthread_num].format = IM2D_SLT_DEFAULT_FORMAT;
    data[pthread_num].rd_mode = IM_RASTER_MODE;
    data[pthread_num].core = IM_SCHEDULER_RGA2_CORE1;
    data[pthread_num].priority = 1;
#endif

    if (run_test(pthread_num, data, pthread_rga_raster_func) < 0) {
        printf("-------------------------------------------------\n");
        printf("RGA raster-test fail!\n");
        return -1;
    }

    printf("-------------------------------------------------\n");
    printf("RGA raster-test success!\n");

#if IM2D_SLT_TEST_SPECIAL_EN
    memset(&data, 0x0, sizeof(private_data_t) * IM2D_SLT_THREAD_MAX);
    printf("-------------------------------------------------\n");

    pthread_num = 0;

#if IM2D_SLT_TEST_RGA3_0_FBC_EN
    pthread_num++;
    data[pthread_num].id = pthread_num;
    data[pthread_num].name = "RGA3_core0_fbc";
    data[pthread_num].dma_heap_name = IM2D_SLT_DMA_HEAP_PATH;
    data[pthread_num].mode = false;
    data[pthread_num].num = 0;
    data[pthread_num].width = IM2D_SLT_DEFAULT_WIDTH;
    data[pthread_num].height = IM2D_SLT_DEFAULT_HEIGHT;
    data[pthread_num].format = IM2D_SLT_DEFAULT_FORMAT;
    data[pthread_num].rd_mode = IM_AFBC16x16_MODE;
    data[pthread_num].core = IM_SCHEDULER_RGA3_CORE0;
    data[pthread_num].priority = 1;
#endif

#if IM2D_SLT_TEST_RGA3_1_FBC_EN
    pthread_num++;
    data[pthread_num].id = pthread_num;
    data[pthread_num].name = "RGA3_core1_fbc";
    data[pthread_num].dma_heap_name = IM2D_SLT_DMA_HEAP_PATH;
    data[pthread_num].mode = false;
    data[pthread_num].num = 0;
    data[pthread_num].width = IM2D_SLT_DEFAULT_WIDTH;
    data[pthread_num].height = IM2D_SLT_DEFAULT_HEIGHT;
    data[pthread_num].format = IM2D_SLT_DEFAULT_FORMAT;
    data[pthread_num].rd_mode = IM_AFBC16x16_MODE;
    data[pthread_num].core = IM_SCHEDULER_RGA3_CORE1;
    data[pthread_num].priority = 1;
#endif

#if IM2D_SLT_TEST_RGA2_0_TILE_EN
    pthread_num++;
    data[pthread_num].id = pthread_num;
    data[pthread_num].name = "RGA2_core0_tile4x4";
    data[pthread_num].dma_heap_name = IM2D_SLT_DMA_HEAP_PATH;
    data[pthread_num].mode = false;
    data[pthread_num].num = 0;
    data[pthread_num].width = IM2D_SLT_DEFAULT_WIDTH;
    data[pthread_num].height = IM2D_SLT_DEFAULT_HEIGHT;
    data[pthread_num].format = RK_FORMAT_YCbCr_420_SP;
    data[pthread_num].rd_mode = IM_TILE4x4_MODE;
    data[pthread_num].core = IM_SCHEDULER_RGA2_CORE0;
    data[pthread_num].priority = 1;
#endif

#if IM2D_SLT_TEST_RGA2_0_AFBC32x8_EN
    pthread_num++;
    data[pthread_num].id = pthread_num;
    data[pthread_num].name = "RGA2_core0_afbc32x8";
    data[pthread_num].dma_heap_name = IM2D_SLT_DMA_HEAP_PATH;
    data[pthread_num].mode = false;
    data[pthread_num].num = 0;
    data[pthread_num].width = 320;
    data[pthread_num].height = 240;
    data[pthread_num].format = RK_FORMAT_RGBA_8888;
    data[pthread_num].rd_mode = IM_AFBC32x8_MODE;
    data[pthread_num].core = IM_SCHEDULER_RGA2_CORE0;
    data[pthread_num].priority = 1;
#endif

#if IM2D_SLT_TEST_RGA2_0_RKFBC64x4_EN
    pthread_num++;
    data[pthread_num].id = pthread_num;
    data[pthread_num].name = "RGA2_core0_rkfbc64x4";
    data[pthread_num].dma_heap_name = IM2D_SLT_DMA_HEAP_PATH;
    data[pthread_num].mode = false;
    data[pthread_num].num = 0;
    data[pthread_num].width = 320;
    data[pthread_num].height = 240;
    data[pthread_num].format = RK_FORMAT_YCbCr_420_SP;
    data[pthread_num].rd_mode = IM_RKFBC64x4_MODE;
    data[pthread_num].core = IM_SCHEDULER_RGA2_CORE0;
    data[pthread_num].priority = 1;
#endif

#if IM2D_SLT_TEST_RGA2_1_TILE_EN
    pthread_num++;
    data[pthread_num].id = pthread_num;
    data[pthread_num].name = "RGA2_core1_tile4x4";
    data[pthread_num].dma_heap_name = IM2D_SLT_DMA_HEAP_PATH;
    data[pthread_num].mode = false;
    data[pthread_num].num = 0;
    data[pthread_num].width = IM2D_SLT_DEFAULT_WIDTH;
    data[pthread_num].height = IM2D_SLT_DEFAULT_HEIGHT;
    data[pthread_num].format = RK_FORMAT_YCbCr_420_SP;
    data[pthread_num].rd_mode = IM_TILE4x4_MODE;
    data[pthread_num].core = IM_SCHEDULER_RGA2_CORE1;
    data[pthread_num].priority = 1;
#endif

#if IM2D_SLT_TEST_RGA2_1_AFBC32x8_EN
    pthread_num++;
    data[pthread_num].id = pthread_num;
    data[pthread_num].name = "RGA2_core1_afbc32x8";
    data[pthread_num].dma_heap_name = IM2D_SLT_DMA_HEAP_PATH;
    data[pthread_num].mode = false;
    data[pthread_num].num = 0;
    data[pthread_num].width = 320;
    data[pthread_num].height = 240;
    data[pthread_num].format = RK_FORMAT_RGBA_8888;
    data[pthread_num].rd_mode = IM_AFBC32x8_MODE;
    data[pthread_num].core = IM_SCHEDULER_RGA2_CORE0;
    data[pthread_num].priority = 1;
#endif

#if IM2D_SLT_TEST_RGA2_1_RKFBC64x4_EN
    pthread_num++;
    data[pthread_num].id = pthread_num;
    data[pthread_num].name = "RGA2_core1_rkfbc64x4";
    data[pthread_num].dma_heap_name = IM2D_SLT_DMA_HEAP_PATH;
    data[pthread_num].mode = false;
    data[pthread_num].num = 0;
    data[pthread_num].width = 320;
    data[pthread_num].height = 240;
    data[pthread_num].format = RK_FORMAT_YCbCr_420_SP;
    data[pthread_num].rd_mode = IM_RKFBC64x4_MODE;
    data[pthread_num].core = IM_SCHEDULER_RGA2_CORE0;
    data[pthread_num].priority = 1;
#endif

    if (run_test(pthread_num, data, pthread_rga_special_func) < 0) {
        printf("-------------------------------------------------\n");
        printf("RGA special-test fail!\n");
        return -1;
    }

    printf("-------------------------------------------------\n");
    printf("RGA special-test success!\n");
#endif

#if IM2D_SLT_TEST_PERF_EN
    memset(&data, 0x0, sizeof(private_data_t) * IM2D_SLT_THREAD_MAX);
    printf("-------------------------------------------------\n");

    pthread_num = 0;

    for (pthread_num = 1; pthread_num < IM2D_SLT_THREAD_MAX; pthread_num++) {
        data[pthread_num].id = pthread_num;
        data[pthread_num].name = "perf_test";
        data[pthread_num].dma_heap_name = IM2D_SLT_DMA_HEAP_PATH;
        data[pthread_num].mode = IM2D_SLT_WHILE_EN;
        data[pthread_num].num = IM2D_SLT_WHILE_NUM;
        data[pthread_num].width = IM2D_SLT_DEFAULT_WIDTH;
        data[pthread_num].height = IM2D_SLT_DEFAULT_HEIGHT;
        data[pthread_num].format = IM2D_SLT_DEFAULT_FORMAT;
        data[pthread_num].rd_mode = IM_RASTER_MODE;
        data[pthread_num].core = IM_SCHEDULER_DEFAULT;
        data[pthread_num].priority = 1;
    }

    if (run_test(pthread_num - 1, data, pthread_rga_perf_func) < 0) {
        printf("-------------------------------------------------\n");
        printf("RGA perf-test fail!\n");
        return -1;
    }

    printf("-------------------------------------------------\n");
    printf("RGA perf-test success!\n");
#endif

    printf("-------------------------------------------------\n");

    return 0;
}
