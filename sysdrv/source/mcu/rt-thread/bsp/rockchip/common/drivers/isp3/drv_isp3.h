/**
  * Copyright (c) 2022 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_isp3.h
  * @version V0.0.1
  * @date    6-July-2022
  * @brief
  *
  ******************************************************************************
  */
#ifndef __DRV_ISP_V3_H__
#define __DRV_ISP_V3_H__

/* Includes ------------------------------------------------------------------*/
#include "isp3.h"
#include "common.h"
#include "isp3_regs.h"
#include "isp3_reg_pre.h"
#include "isp3_external.h"
#include "../camera/camera.h"
#include "../../hal/lib/hal/inc/hal_base.h"
#include "../../../rv1106-mcu/drivers/config.h"
#include "drv_isp_module.h"

#if defined(RT_USING_ISP3)

/* Exported macro ------------------------------------------------------------*/
/* Rockchip ISP version information */
#define ISP_MAJOR_VERSION                   0L/* major version number */
#define ISP_MINOR_VERSION                   0L/* minor version number */
#define ISP_REVISE_REVISION                 1L/* revise version number */

/* Rockchip ISP version */
#define ISP_VERSION                         ((ISP_MAJOR_VERSION * 10000) + \
                                               (ISP_MINOR_VERSION * 100) + \
                                                ISP_REVISE_REVISION)
#define RK_ISP_DEVICE_NAME                  "isp_0"
#define RK_ISP_MAIN_SUBDEV_NAME             "sensor_0"
#define RK_ISP_ASSIST_SUBDEV_NAME           "sensor_1"
#define RK_ISP_ROW_BYTE_ALIGN                256

#define RK_ISP_BUF_NUM_MAX                  (32)
#define RK_ISP_BUF_NUM_MIN                  (2)
#define RK_ISP_BLOCK_NUM_MAX                (32)
#define RK_ISP_BLOCK_NUM_MIN                (2)
#define RK_ISP_DEVICE_NAME_SIZE             (12)

#define ISP_WIDOW_WIDTH_MIN                 (64)
#define ISP_WIDOW_WIDTH_MAX                 (8192)
#define ISP_WIDOW_HEIGHT_MIN                (64)
#define ISP_WIDOW_HEIGHT_MAX                (8192)
#define ISP_VIDEO_BUF_NUM_MAX               (32)
#define ISP_VIDEO_BUF_NUM_MIN               (3)
#define ISP_VIDEO_PLANE_NUM_MAX             (8)
#define ISP_BLOCK_NUM_MAX                   (ISP_VIDEO_BUF_NUM_MAX)
#define ISP_BLOCK_NUM_MIN                   (2)
#define ISP_RAW_STORED_BIT_WIDTH            (16U)
#define ISP_YUV_STORED_BIT_WIDTH            (8U)
#define ISP_IRQ_PREEM_PRIO                  (14)
#define ISP_IRQ_SUB_PRIO                    (0)

#define ISP_OUTPUT_422_420_SHIFT            (16U)
#define ISP_OUTPUT_422_420_MASK             (0x01 << ISP_OUTPUT_422_420_SHIFT)
#define ISP_OUTPUT_YUV_422                  (0x00 << ISP_OUTPUT_422_420_SHIFT)
#define ISP_OUTPUT_YUV_420                  (0x01 << ISP_OUTPUT_422_420_SHIFT)

#define ISP_OUTPUT_420_ORDER_SHIFT          (17U)
#define ISP_OUTPUT_420_ORDER_MASK           (0x01 << ISP_OUTPUT_420_ORDER_SHIFT)
#define ISP_OUTPUT_420_ORDER_EVEN           (0x00 << ISP_OUTPUT_420_ORDER_SHIFT)
#define ISP_OUTPUT_420_ORDER_ODD            (0x01 << ISP_OUTPUT_420_ORDER_SHIFT)

#define ISP_RAW_END_TYPE_SHIFT              (18U)
#define ISP_RAW_END_TYPE_MASK               (0x01 << ISP_RAW_END_TYPE_SHIFT)
#define ISP_RAW_END_TYPE_LITTLE             (0x00 << ISP_RAW_END_TYPE_SHIFT)
#define ISP_RAW_END_TYPE_BIG                (0x01 << ISP_RAW_END_TYPE_SHIFT)

#define ISP_UV_STORED_ORDER_SHIFT           (19U)
#define ISP_UV_STORED_ORDER_MASK            (0x01 << ISP_UV_STORED_ORDER_SHIFT)
#define ISP_UV_STORED_ORDER_UVUV            (0x00 << ISP_UV_STORED_ORDER_SHIFT)
#define ISP_UV_STORED_ORDER_VUVU            (0x01 << ISP_UV_STORED_ORDER_SHIFT)

#define MIPI_DROP_FRM                       BIT(3)

#define HDR_DMA_MAX                     3
#define HDR_DMA0                        0
#define HDR_DMA1                        1
#define HDR_DMA2                        2

/* Exported types ------------------------------------------------------------*/
typedef enum buf_state
{
    ISP_BUF_STATE_DEQUEUED = 0x01,
    ISP_BUF_STATE_PREPARING = 0x02,
    ISP_BUF_STATE_PREPARED = 0x04,
    ISP_BUF_STATE_QUEUED = 0x08,
    ISP_BUF_STATE_REQUEUEING = 0x10,
    ISP_BUF_STATE_ACTIVE = 0x20,
    ISP_BUF_STATE_DONE = 0x40,
    ISP_BUF_STATE_ERROR = 0x80,
} eISP_Bufstat;

typedef enum buf_list_type
{
    ISP_BUF_LIST_TYPE_MP = 0x0,
    ISP_BUF_LIST_TYPE_SP,
    ISP_BUF_LIST_TYPE_LF,
    ISP_BUF_LIST_TYPE_MF,
    ISP_BUF_LIST_TYPE_SF,
    ISP_BUF_LIST_TYPE_IDLE,
} eISP_Listtype;

typedef enum buf_mem_type
{
    ISP_BUF_MEM_TYPE_COMPACT = 0x0,
    ISP_BUF_MEM_TYPE_NORMAL,
} eISP_Bufmemtype;

struct isp_video_requestbuffers
{
    eISP_workMode work_mode;
    eISP_Bufmemtype type;
    uint8_t count;
};

struct isp_video_buf
{
    struct isp_videobuf_queue *buf_q;
    uint8_t *addr;
    uint32_t bufsize;
    rt_list_t queued_entry;
    rt_list_t done_entry;
    rt_list_t actived_entry;
    eISP_Bufstat state;
    uint32_t timestamp;
    uint8_t index;
    uint32_t frame_id;
    bool malloced_by_dma_large;
    eISP_Listtype listtype;
};

struct isp_videobuf_queue
{
    struct isp_video_buf bufs[ISP_VIDEO_BUF_NUM_MAX];
    rt_mutex_t qmutex_lock;
    rt_sem_t qsem;
    rt_mutex_t done_lock;
    rt_list_t queued_list;
    rt_list_t done_list;
    rt_list_t actived_list;
    rt_list_t lf_done_list;
    rt_list_t mf_done_list;
    rt_list_t sf_done_list;
    uint8_t num_buffers;
    uint8_t min_buffers_needed;
    uint8_t queued_count;
    uint8_t done_count;
    uint8_t streaming: 1;
    /* atomic_t owned_by_drv_count; */
    uint8_t owned_by_drv_count;
};

typedef enum isp_format_type
{
    ISP_DEVICE_FORMAT_YUV = 0x0,
    ISP_DEVICE_FORMAT_RGB,
    ISP_DEVICE_FORMAT_RAW, /* bayer */
    ISP_DEVICE_FORMAT_JPEG
} eISP_Devformat;

struct isp_input_fmt
{
    union
    {
        eISP_yuvInOrder yuv_input_order;
        eISP_rawWidth raw_input_width;
    } data;
    eISP_Devformat fmt_type;
    uint16_t mbus_code;
    uint32_t mipi_dt;
    uint8_t data_width;
};

struct isp_output_fmt
{
    uint32_t fourcc;
    uint32_t format;
    uint8_t cplanes;
    uint8_t mplanes;
    uint8_t uv_swap;
    uint8_t bpp[ISP_VIDEO_PLANE_NUM_MAX];
    eISP_Devformat fmt_type;
    uint32_t write_fmt_mp;
    uint32_t write_fmt_sp;
    uint8_t format_data_width;
};

/**
 * isp_input_info - cache the input parameters for setting ISP_DVP_FOR register
 */
struct isp_input_info
{
    struct rk_camera_info cam_info; /**< cam_info: cache the frame information about input camera,get from camera driver */
    struct isp_input_fmt input_fmt; /**< input_fmt: cache parameters based on media-bus matching for ISP_DVP_FOR register,get from cif driver */
};

/**
 * isp_output_info - cache the output parameters for setting ISP_DVP_FOR register
 */
struct isp_output_info
{
    struct rk_camera_pix_format pix_format; /** < pix_format:cache the frame format for capturing, is set by application */
    struct rk_camera_crop_rect crop;    /** < crop:cache crop information for capturing, is set by application */
    const struct isp_output_fmt *output_fmt;  /** < output_fmt:cache parameters based on fourcc matching for ISP_DVP_FOR register,get from cif driver */
    bool is_crop;   /** < is_crop:mask whether crop image or not,relies on crop field */
};

typedef enum plane_type
{
    ISP_PLANE_TYPE_MP = 0x01,
    ISP_PLANE_TYPE_SP = 0x02,
    ISP_PLANE_TYPE_RAW = 0x04,
} eISP_Planetype;

typedef enum color_component
{
    ISP_COLOR_Y = 0x0,
    ISP_COLOR_CBCR = 0x1,
    ISP_COLOR_MAX,
} eISP_Colorcomponent;


struct isp_plane_info
{
    eISP_Planetype plane_type;        /** < format plane info for data path */
    uint32_t plane_size[ISP_COLOR_MAX];     /** < plane_size[0]:for y field, plane_size[1]:for cbcr field */
};

struct isp_clock_info
{
    rk_clk_gate *isp_pclk;
    rk_clk_gate *isp_hclk;
    rk_clk_gate *isp_aclk;
    rk_clk_gate *isp0_vicap_clk;
};

struct vicap_clock_info
{
    rk_clk_gate *vicap_pclk;
    rk_clk_gate *vicap_hclk;
    rk_clk_gate *vicap_aclk;
    rk_clk_gate *vicap_dclk;
    rk_clk_gate *vicap_i0clk;
    rk_clk_gate *vicap_rx0pclk;
    rk_clk_gate *vicap_pclk_vepu;
};

struct csi2_clock_info
{
    rk_clk_gate *csi2_pclk;
    rk_clk_gate *csi2_rxbyteclkhs0;
};

struct dphy_clock_info
{
    rk_clk_gate *dphy_pclk;
};

typedef enum sw_ibuf_op_mode
{
    RK_ISP_IBUF_OP_MODE_NORMAL = 0x0,
    RK_ISP_IBUF_OP_MODE_USER = 0x3,
    RK_ISP_IBUF_OP_MODE_RAW_RDBACK_FRAME1 = 0x4,
    RK_ISP_IBUF_OP_MODE_HDR_RDBACK_FRAME2 = 0x5,
    RK_ISP_IBUF_OP_MODE_HDR_RDBACK_FRAME3 = 0x6,
    RK_ISP_IBUF_OP_MODE_HDR_FRAMX2_DDR = 0x8,
    RK_ISP_IBUF_OP_MODE_HDR_LINEX2_DDR = 0x9,
    RK_ISP_IBUF_OP_MODE_HDR_LINEX2_NODDR = 0xa,
    RK_ISP_IBUF_OP_MODE_HDR_FRAMEX3_DDR = 0xc,
    RK_ISP_IBUF_OP_MODE_HDR_LINEX3_DDR = 0xd,
} eISP_Ibuf_Opmode;

typedef enum tx2dma_route
{
    RK_ISP_TX2DMA_RAW0 = 0x0,
    RK_ISP_TX2DMA_RAW1,
    RK_ISP_TX2DMA_RAW2,
    RK_ISP_TX2DMA_RAW3,
} eISP_Tx2dma_Routeid;

typedef enum sw_csi_id
{
    RK_ISP_SW_CSI_ID0 = 0x0,
    RK_ISP_SW_CSI_ID1,
    RK_ISP_SW_CSI_ID2,
    RK_ISP_SW_CSI_ID3,
    RK_ISP_SW_CSI_ID4,
    RK_ISP_SW_CSI_ID5,
    RK_ISP_SW_CSI_ID6,
    RK_ISP_SW_CSI_ID7,
} eISP_Csi_Id;

typedef enum irq_index
{
    RK_ISP_IRQ_MMU_0 = 0x0,
    RK_ISP_IRQ_ISP,
    RK_ISP_IRQ_MI,
    RK_ISP_IRQ_MIPI,
    RK_ISP_IRQ_MAX,
} eISP_Irqindex;

struct rk_isp_irq
{
    int irq_num;
    char name[20];
    rt_isr_handler_t irq_handler;
};

struct heap_pool
{
    uint32_t start_addr;
    uint32_t end_addr;
    uint32_t total_size;
    uint32_t available_size;
    uint32_t cur_addr;
    uint32_t wasteful_size;
};

typedef enum rdbk_index
{
    RK_ISP_BUF_RDBK_L = 0x0,
    RK_ISP_BUF_RDBK_M,
    RK_ISP_BUF_RDBK_S,
    RK_ISP_BUF_RDBK_MAX,
} eISP_Buf_Rdbkindex;

#pragma pack(1)
struct dphy_desc
{
    bool csi_host_idx;
    int data_lanes;
    bool enable;
    char *isp_subdev_name;
};
#pragma pack()

#pragma pack(1)
struct dphy_board_desc
{
    bool enable;
    uint32_t reg;
    struct dphy_desc csi2_dphy1;
    struct dphy_desc csi2_dphy2;
};
#pragma pack()

#pragma pack(1)
struct csi2_board_desc
{
    bool csi_host_idx;
    bool enable;
    char *subdev_name;
    char *isp_subdev_name;
    uint32_t reg;
};
#pragma pack()

#pragma pack(1)
struct vicap_board_desc
{
    bool enable;
    uint32_t reg;
};
#pragma pack()

extern struct dphy_board_desc csi2_dphy_board;
extern struct csi2_board_desc csi2_host0_board;
extern struct csi2_board_desc csi2_host1_board;
extern struct vicap_board_desc vicap_board;

struct rk_isp_dev
{
    struct rk_isp_device parent;
    char name[RK_ISP_DEVICE_NAME_SIZE];
    struct VI_GRF_REG *top_grf;
    struct DPHYRX_REG *mipi_dphy;
    struct VICAP_REG *vicap;
    struct MIPI_CSI_DPHY_REG *dphy;
    struct CSI2HOST_REG *csihost;
    struct dphy_board_desc *csi2_dphy_board;
    struct csi2_board_desc *csi2_host0_board;
    struct csi2_board_desc *csi2_host1_board;
    struct vicap_board_desc *vicap_board;
    struct rk_isp_irq *irq_set;
    struct rk_camera_device *subdev;
    struct isp_videobuf_queue buf_queue;
    /* used to config mp yuv addr */
    struct isp_video_buf *buf_addr0;
    struct isp_video_buf *buf_addr1;
    struct isp_video_buf *buf_lf_addr0;
    struct isp_video_buf *buf_lf_addr1;
    struct isp_video_buf *buf_mf_addr0;
    struct isp_video_buf *buf_mf_addr1;
    struct isp_video_buf *buf_sf_addr0;
    struct isp_video_buf *buf_sf_addr1;
    struct isp_video_buf *buf_rdbk[RK_ISP_BUF_RDBK_MAX];
    eISP_Bufmemtype buf_mem_type;
    struct isp_input_info input;
    struct isp_output_info output;
    struct isp_clock_info isp_clk;
    struct vicap_clock_info vicap_clk;
    struct csi2_clock_info csi2_clk;
    struct dphy_clock_info dphy_clk;
    struct isp_plane_info plane_info;
#if ISP_WORK_QUEUE_ENABLE
    struct rt_workqueue *isp_workqueue;
    struct rk_isp_work isp_work;
#endif
    eISP_workMode work_mode;
    enum rk_camera_hdr_mode hdr_mode;
    uint32_t frm_id;
    rt_sem_t streamoff_sem;
    bool is_streamoff;
    struct heap_pool heap;
    struct rkisp_stats_buffer stats;
    struct rkisp_params_buffer params;
    rt_sem_t stats_sem;
    unsigned int frame_id;
};

/* Exported constants --------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
#endif

#endif
