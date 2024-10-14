#ifndef __v4l2_isp_luma_H__
#define __v4l2_isp_luma_H__

#include "v4l2_rawrd.h"
#include "v4l2_param.h"
#include "v4l2_link.h"
#include "v4l2_stats.h"
#include "v4l2_yuv.h"
#include "v4l2_isp_subdev.h"

typedef int (*sensor_set_exp_cb)(int fd, double time_ms);

struct isp_luma_s {
	char *mdev_path_isp_flood;
	char *mdev_path_isp_pro;

	int width;
	int height;

	buff_callback yuv_buf_cb;
	void *yuv_buf_cb_data;

	sensor_set_exp_cb sensor_cb;
	int sensor_fd;
};

struct isp_pip_ctx_s {
	struct v4l2_contex stats_ctx;
	struct v4l2_contex yuv_ctx;
	struct v4l2_contex param_ctx;
	struct v4l2_contex isp_ctx;
	struct v4l2_contex rawrd_ctx;
};

int v4l2_isp_luma_link_init(char *mdev_path_isp_flood, char*mdev_path_isp_pro);
int v4l2_isp_luma_init(struct isp_luma_s *param);
int v4l2_isp_luma_deinit(void);
int v4l2_isp_luma_proc(bool is_flood, int fd, void *param, int sequence);
void v4l2_isp_luma_cal_new_exp(void *param,
		void *buffer, int index, int dma_fd, int sequence);
float v4l2_isp_luma_update_exp(int sequence);
void v4l2_isp_luma_clr_stable(void);

bool v4l2_isp_luma_get_stable(void);
#endif
