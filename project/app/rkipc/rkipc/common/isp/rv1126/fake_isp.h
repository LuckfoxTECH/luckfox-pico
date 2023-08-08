// Copyright 2021 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <linux/videodev2.h>

#include <rk_aiq_user_api_ahdr.h>
#include <rk_aiq_user_api_sysctl.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_FAKE_FRAMES_NUM 5

#define RKISP_CMD_GET_SHARED_BUF _IOR('V', BASE_VIDIOC_PRIVATE + 2, struct rkisp_thunderboot_resmem)

#define RKISP_CMD_GET_SHM_BUFFD _IOWR('V', BASE_VIDIOC_PRIVATE + 6, struct rkisp_thunderboot_shmem)

#define RKISP_CMD_FREE_SHARED_BUF _IO('V', BASE_VIDIOC_PRIVATE + 3)

struct rkisp_fastboot_resmem_head {
	uint16_t enable;
	uint16_t complete;
	uint16_t frm_total;
	uint16_t hdr_mode;
	uint16_t width;
	uint16_t height;
	uint32_t bus_fmt;
} __attribute__((packed));

struct rkisp_thunderboot_resmem {
	uint32_t resmem_padr;
	uint32_t resmem_size;
} __attribute__((packed));

struct rkisp_thunderboot_shmem {
	uint32_t shm_start;
	uint32_t shm_size;
	int32_t shm_fd;
} __attribute__((packed));

struct _raw_data {
	uint16_t flag;
	uint32_t size;
	uint32_t ddr_haddr;
	uint32_t ddr_laddr;
	uint32_t raw_size;
} __attribute__((packed));

struct rkipc_raw_format {
	uint16_t flag;
	uint32_t size;
	uint16_t vesrion;
	char sensor[32];
	char scene[32];
	uint32_t frame_no;
	uint16_t width;
	uint16_t height;
	uint8_t bitw;
	uint8_t bayer;
	uint8_t hdr_frame_cnt;
	uint8_t frame_type;
	uint8_t buf_type;
	uint16_t row_len;
	uint16_t valid_row_len;
	char remark[32];
} __attribute__((packed));

struct _frame_inf {
	uint16_t flag;
	uint32_t size;
	uint16_t vesrion;
	float normal_exp;
	float normal_gain;
	float hdr_exp_l;
	float hdr_exp_m;
	float hdr_exp_s;
	float hdr_gain_l;
	float hdr_gain_m;
	float hdr_gain_s;
	float awg_rgain;
	float awg_bgain;
	char awg_ill[16];
} __attribute__((packed));

struct mcu_rkaiq_rkraw {
	struct _raw_data rawdata;
	uint8_t *frame_base;
	int raw_buf_fd;
	struct rkipc_raw_format rawfmt;
	struct _frame_inf finfo;
} __attribute__((packed));

rk_aiq_sys_ctx_t *aiq_fake_init(rk_aiq_working_mode_t mode, const char *iq_dir,
                                rk_aiq_raw_prop_t prop, void (*callback)(void *));
void aiq_fake_exit(rk_aiq_sys_ctx_t *ctx);
void alloc_rkraws(uint8_t **rkraws);
void free_rkraws(uint8_t **rkraws);
bool free_rkisp_reserve_mem(const char *subdev);
void make_rkraws(struct mcu_rkaiq_rkraw *mcu_rkraws, uint8_t **rkraws);
bool parse_mcu_rkraws(const char *subdev, struct mcu_rkaiq_rkraw *mcu_rkraws);

#ifdef __cplusplus
}
#endif
