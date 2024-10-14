/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_vicap.h
  * @version V0.0.1
  * @brief   video capture unit driver for rk2108
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-03-29     ISP Team      first implementation
  *
  ******************************************************************************
  */

#ifndef __DRV_VICAP_H__
#define __DRV_VICAP_H__

/* Includes ------------------------------------------------------------------*/
#if defined(__RT_THREAD__)
#include "adapter/adapter.h"
#elif defined(__RK_OS__)
#include "driver/adapter.h"
#endif

#ifdef VICAP_MODULE_COMPILED
#if defined(__RT_THREAD__)
#include "vicap.h"
#include "camera/camera.h"
#elif defined(__RK_OS__)
#include "driver/vicap.h"
#include "driver/camera.h"
#endif

/* Exported macro ------------------------------------------------------------*/
/* Rockchip VICAP version information */
#define VICAP_MAJOR_VERSION                   0L/* major version number */
#define VICAP_MINOR_VERSION                   0L/* minor version number */
#define VICAP_REVISE_REVISION                 1L/* revise version number */

/* Rockchip VICAP version */
#define VICAP_VERSION                         ((VICAP_MAJOR_VERSION * 10000) + \
                                               (VICAP_MINOR_VERSION * 100) + \
                                                VICAP_REVISE_REVISION)
#define RK_VICAP_DEVICE_NAME                  "vicap_0"
#define RK_VICAP_SUBDEV_NAME                  "sensor_0"

#define RK_VICAP_BUF_NUM_MAX                  (32)
#define RK_VICAP_BUF_NUM_MIN                  (2)
#define RK_VICAP_BLOCK_NUM_MAX                (32)
#define RK_VICAP_BLOCK_NUM_MIN                (2)
#define RK_VICAP_DEVICE_NAME_SIZE             (12)

#define VICAP_WIDOW_WIDTH_MIN                 (64)
#define VICAP_WIDOW_WIDTH_MAX                 (8192)
#define VICAP_WIDOW_HEIGHT_MIN                (64)
#define VICAP_WIDOW_HEIGHT_MAX                (8192)
#define VICAP_VIDEO_BUF_NUM_MAX               (32)
#define VICAP_VIDEO_BUF_NUM_MIN               (3)
#define VICAP_VIDEO_PLANE_NUM_MAX             (8)
#define VICAP_BLOCK_NUM_MAX                   (VICAP_VIDEO_BUF_NUM_MAX)
#define VICAP_BLOCK_NUM_MIN                   (2)
#define VICAP_RAW_STORED_BIT_WIDTH            (16U)
#define VICAP_YUV_STORED_BIT_WIDTH            (8U)
#define VICAP_IRQ_PREEM_PRIO                  (14)
#define VICAP_IRQ_SUB_PRIO                    (0)

#define VICAP_OUTPUT_422_420_SHIFT            (16U)
#define VICAP_OUTPUT_422_420_MASK             (0x01 << VICAP_OUTPUT_422_420_SHIFT)
#define VICAP_OUTPUT_YUV_422                  (0x00 << VICAP_OUTPUT_422_420_SHIFT)
#define VICAP_OUTPUT_YUV_420                  (0x01 << VICAP_OUTPUT_422_420_SHIFT)

#define VICAP_OUTPUT_420_ORDER_SHIFT          (17U)
#define VICAP_OUTPUT_420_ORDER_MASK           (0x01 << VICAP_OUTPUT_420_ORDER_SHIFT)
#define VICAP_OUTPUT_420_ORDER_EVEN           (0x00 << VICAP_OUTPUT_420_ORDER_SHIFT)
#define VICAP_OUTPUT_420_ORDER_ODD            (0x01 << VICAP_OUTPUT_420_ORDER_SHIFT)

#define VICAP_RAW_END_TYPE_SHIFT              (18U)
#define VICAP_RAW_END_TYPE_MASK               (0x01 << VICAP_RAW_END_TYPE_SHIFT)
#define VICAP_RAW_END_TYPE_LITTLE             (0x00 << VICAP_RAW_END_TYPE_SHIFT)
#define VICAP_RAW_END_TYPE_BIG                (0x01 << VICAP_RAW_END_TYPE_SHIFT)

#define VICAP_UV_STORED_ORDER_SHIFT           (19U)
#define VICAP_UV_STORED_ORDER_MASK            (0x01 << VICAP_UV_STORED_ORDER_SHIFT)
#define VICAP_UV_STORED_ORDER_UVUV            (0x00 << VICAP_UV_STORED_ORDER_SHIFT)
#define VICAP_UV_STORED_ORDER_VUVU            (0x01 << VICAP_UV_STORED_ORDER_SHIFT)

/* Exported types ------------------------------------------------------------*/
typedef enum buf_state
{
    VICAP_BUF_STATE_DEQUEUED = 0x01,
    VICAP_BUF_STATE_PREPARING = 0x02,
    VICAP_BUF_STATE_PREPARED = 0x04,
    VICAP_BUF_STATE_QUEUED = 0x08,
    VICAP_BUF_STATE_REQUEUEING = 0x10,
    VICAP_BUF_STATE_ACTIVE = 0x20,
    VICAP_BUF_STATE_DONE = 0x40,
    VICAP_BUF_STATE_ERROR = 0x80,
} eVICAP_Bufstat;

struct vicap_video_requestbuffers
{
    eVICAP_workMode work_mode;
    uint8_t count;
};

struct vicap_id_num
{
    uint32_t frame_id;
    uint32_t block_id;
};

struct vicap_video_buf
{
    struct vicap_videobuf_queue *buf_q;
    uint8_t *addr;
    uint32_t bufsize;
    rk_list_node queued_entry;
    rk_list_node done_entry;
    rk_list_node actived_entry;
    eVICAP_Bufstat state;
    uint32_t timestamp;
    uint8_t index;
    struct vicap_id_num id;
    bool malloced_by_dma_large;
};

struct vicap_videobuf_queue
{
    struct vicap_video_buf *bufs[VICAP_VIDEO_BUF_NUM_MAX];
    rk_mutex_t qmutex_lock;
    rk_semaphore_t qsem;
    rk_mutex_t done_lock;
    rk_queue_list queued_list;
    rk_queue_list done_list;
    rk_queue_list actived_list;
    uint8_t num_buffers;
    uint8_t min_buffers_needed;
    uint8_t queued_count;
    uint8_t done_count;
    uint8_t streaming: 1;
    /* atomic_t owned_by_drv_count; */
    uint8_t owned_by_drv_count;
};

typedef enum vicap_format_type
{
    VICAP_DEVICE_FORMAT_YUV = 0x0,
    VICAP_DEVICE_FORMAT_RAW,
    VICAP_DEVICE_FORMAT_JPEG
} eVICAP_Devformat;

struct vicap_input_fmt
{
    union
    {
        eVICAP_yuvInOrder yuv_input_order;
        eVICAP_rawWidth raw_input_width;
    } data;
    eVICAP_Devformat fmt_type;
    uint16_t mbus_code;
};

struct vicap_output_fmt
{
    uint32_t fourcc;
    uint32_t format;
    uint8_t cplanes;
    uint8_t mplanes;
    uint8_t bpp[VICAP_VIDEO_PLANE_NUM_MAX];
};

/**
 * vicap_input_info - cache the input parameters for setting VICAP_DVP_FOR register
 */
struct vicap_input_info
{
    struct rk_camera_info cam_info; /**< cam_info: cache the frame information about input camera,get from camera driver */
    struct vicap_input_fmt input_fmt; /**< input_fmt: cache parameters based on media-bus matching for VICAP_DVP_FOR register,get from cif driver */
};

/**
 * vicap_output_info - cache the output parameters for setting VICAP_DVP_FOR register
 */
struct vicap_output_info
{
    struct rk_camera_pix_format pix_format; /** < pix_format:cache the frame format for capturing, is set by application */
    struct rk_camera_crop_rect crop;    /** < crop:cache crop information for capturing, is set by application */
    const struct vicap_output_fmt *output_fmt;  /** < output_fmt:cache parameters based on fourcc matching for VICAP_DVP_FOR register,get from cif driver */
    bool is_crop;   /** < is_crop:mask whether crop image or not,relies on crop field */
};

struct vicap_block_info
{
    uint32_t block_num;
    uint32_t lines_per_block;
    uint32_t Y_len_per_block;
};

struct vicap_clock_info
{
    rk_clk_gate *vicap_pclk;
    rk_clk_gate *vicap_hclk;
    rk_clk_gate *vicap_aclk;
};

struct rk_vicap_dev
{
    struct rk_vicap_device parent;
    char name[RK_VICAP_DEVICE_NAME_SIZE];
    struct VICAP_REG *vicap_reg;
    struct CRU_REG *cru_reg;
    struct vicap_clock_info vicap_clk;
    struct rk_camera_device *subdev;

    struct vicap_videobuf_queue buf_queue;
    /* used to config block0/frame0 yuv addr */
    struct vicap_video_buf *buf_addr0;
    /* used to config block1/frame1 yuv addr */
    struct vicap_video_buf *buf_addr1;

    struct vicap_input_info input;
    struct vicap_output_info output;
    struct vicap_block_info block_info;

    int irq;
    NVIC_IRQHandler irqhandler;
#if VICAP_WORK_QUEUE_ENABLE
    struct rt_workqueue *vicap_workqueue;
    struct rk_vicap_work vicap_work;
#endif
    eVICAP_workMode work_mode;
    rk_semaphore_t streamoff_sem;
    bool is_streamoff;

};

/* Exported constants --------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
#endif

#endif
