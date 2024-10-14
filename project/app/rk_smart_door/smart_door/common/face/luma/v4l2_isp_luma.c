// Copyright 2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdbool.h>

#include "param/rkisp32-config.h"
#include "v4l2_isp_luma.h"

#if SC035_VGA_SENSOR
#define TARGET_LUMA (128 << 2)
#define AE_ROTATE (1)
#define PRO_ISP_PROC_ENABLE (1)
#define MAX_EXP     (12.0)
#define PRO_EXP     (4.0)
#define DEFAULT_EXP (5.0)

#elif SC132GS_SENSOR
#define TARGET_LUMA (100 << 2)
#define AE_ROTATE (0)
#define PRO_ISP_PROC_ENABLE (0)
#define MAX_EXP     (20.0)
#define PRO_EXP     (7.0)
#define DEFAULT_EXP (5.0)

#elif SC2355_SENSOR
#define TARGET_LUMA (128 << 2)
#define AE_ROTATE (1)
#define PRO_ISP_PROC_ENABLE (0)
#define MAX_EXP     (30.0)
#define PRO_EXP     (12.0)
#define DEFAULT_EXP (8.0)

#else
#error "error sensor type"
#endif

#define MIN_EXP     (0.5)

#define SET_RATIO   (0.9)
#define TOLERANCE   (3)
#define STROBE_DELAY_MS (0.3)
#define STABLE_DIFF_MS  (1.0)


//#define DEBUG_TIME

#define RECORD_MAX (5)
struct exp_record_s {
	unsigned char index;
	int sequence[RECORD_MAX];
	float exposure[RECORD_MAX];
	float exposure_last;
	pthread_mutex_t mut;
	sensor_set_exp_cb sensor_cb;
	int sensor_fd;
	bool stable;
};

static struct exp_record_s luma_record;

int DefaultWeight[ISP32_RAWAEBIG_MEAN_NUM] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 6, 6, 6, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 6, 6, 6, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 4, 6, 8, 6, 4, 0, 0, 0, 0, 0,
	1, 1, 1, 2, 6, 6, 8,10, 8, 6, 6, 2, 1, 1, 1,
	1, 1, 1, 4, 6, 8, 8,12, 8, 8, 6, 4, 1, 1, 1,
	1, 1, 2, 4, 6, 8, 8,10, 8, 8, 6, 4, 2, 1, 1,
	2, 2, 2, 4, 6, 6, 8, 8, 8, 6, 6, 4, 2, 2, 2,
	2, 2, 2, 6, 6, 6, 8, 8, 8, 6, 6, 6, 2, 2, 2,
	2, 2, 2, 6, 6, 6, 8, 8, 8, 6, 6, 6, 2, 2, 2
};

static int *g_ae_weight = NULL;

static struct isp_pip_ctx_s *g_pip_ctx_ir = NULL;
static struct isp_pip_ctx_s *g_pip_ctx_pro = NULL;

static struct isp_pip_ctx_s *v4l2_isp_pip_init(const char *media_dev_path,
		int width, int height, buff_callback buff_cb_yuv, void *cb_data,
		buff_callback buff_cb_stat, bool is_flood)
{
	printf("%s w:%d h:%d\n", __func__, width, height);

	struct media_path_s *isp_info =
		(struct media_path_s *)malloc(sizeof(struct media_path_s));

	memset(isp_info, 0, sizeof(struct media_path_s));

	struct isp_pip_ctx_s *pip_ctx =
		(struct isp_pip_ctx_s *)malloc(sizeof(struct isp_pip_ctx_s));

	memset(pip_ctx, 0, sizeof(struct isp_pip_ctx_s));

	v4l2_media_get_isp_info(media_dev_path, isp_info);

	v4l2_rawrd_init(&pip_ctx->rawrd_ctx,
			isp_info->rawrd2_s_path, width, height);

	v4l2_isp_init(&pip_ctx->isp_ctx, isp_info->isp_dev_path, width, height);

	pip_ctx->stats_ctx.buff_cb = buff_cb_stat;
	v4l2_stats_init(&pip_ctx->stats_ctx,
			isp_info->stats_path, width, height);

	v4l2_param_init(&pip_ctx->param_ctx,
			isp_info->input_params_path, width, height);

	pip_ctx->yuv_ctx.buff_cb = buff_cb_yuv;
	pip_ctx->yuv_ctx.cb_data = cb_data;
	pip_ctx->yuv_ctx.is_flood = is_flood;
	v4l2_yuv_init(&pip_ctx->yuv_ctx, isp_info->main_path, width, height);

	free(isp_info);

	return pip_ctx;
}

/* link isp before all video stream on */
int v4l2_isp_luma_link_init(char *mdev_path_isp_flood, char*mdev_path_isp_pro)
{
	v4l2_media_link_rawrd(mdev_path_isp_flood, true);

	v4l2_media_link_rawrd(mdev_path_isp_pro, true);

	v4l2_media_copy_isp_entity(mdev_path_isp_flood, mdev_path_isp_pro);
}

int v4l2_isp_luma_init(struct isp_luma_s *param)
{
	int width = param->width;
	int height = param->height;
	buff_callback buff_cb_yuv = param->yuv_buf_cb;
	void *cb_data = param->yuv_buf_cb_data;
	printf("%s w:%d h:%d\n", __func__, width, height);

	g_pip_ctx_ir = v4l2_isp_pip_init(param->mdev_path_isp_flood,
			width, height, buff_cb_yuv,
			cb_data, v4l2_isp_luma_cal_new_exp, true);

#if PRO_ISP_PROC_ENABLE
	g_pip_ctx_pro = v4l2_isp_pip_init(param->mdev_path_isp_pro,
			width, height, buff_cb_yuv,
			cb_data, v4l2_isp_luma_cal_new_exp, false);
#endif

	memset(&luma_record, 0, sizeof(luma_record));

	pthread_mutex_init(&luma_record.mut, NULL);

	luma_record.exposure_last = DEFAULT_EXP;
	luma_record.sensor_cb = param->sensor_cb;
	luma_record.sensor_fd = param->sensor_fd;

	g_ae_weight = (int*)malloc(sizeof(DefaultWeight));

#if AE_ROTATE
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {
			g_ae_weight[i * 15 + j] = DefaultWeight[j * 15 + i];
		}
	}
#else
	memcpy(g_ae_weight, DefaultWeight, sizeof(DefaultWeight));
#endif
}

int v4l2_isp_pip_deinit(struct isp_pip_ctx_s *pip_ctx)
{
	if (pip_ctx == NULL) {
		//printf("error:pip_ctx null\n");
		return 0;
	}

	v4l2_stats_deinit(&pip_ctx->stats_ctx);

	v4l2_yuv_deinit(&pip_ctx->yuv_ctx);

	v4l2_param_deinit(&pip_ctx->param_ctx);

	v4l2_isp_deinit(&pip_ctx->isp_ctx);

	v4l2_rawrd_deinit(&pip_ctx->rawrd_ctx);

	return 0;
}

int v4l2_isp_luma_deinit(void)
{
	v4l2_isp_pip_deinit(g_pip_ctx_ir);

	v4l2_isp_pip_deinit(g_pip_ctx_pro);

	pthread_mutex_destroy(&luma_record.mut);

	memset(&luma_record, 0, sizeof(luma_record));

	if (g_ae_weight) {
		free(g_ae_weight);
		g_ae_weight = NULL;
	}

	if (g_pip_ctx_ir) {
		free(g_pip_ctx_ir);
		g_pip_ctx_ir = NULL;
	}

	if (g_pip_ctx_pro) {
		free(g_pip_ctx_pro);
		g_pip_ctx_pro = NULL;
	}
}

int v4l2_isp_luma_trigger(int sequence)
{
	struct isp_pip_ctx_s *pip_ctx = g_pip_ctx_ir;
	if (pip_ctx == NULL) {
		printf("error:pip_ctx null\n");
		return 0;
	}

	v4l2_isp_trigger(&pip_ctx->isp_ctx, sequence);
	return 0;
}

int v4l2_isp_luma_enqueue(int fd, void *param)
{
	struct isp_pip_ctx_s *pip_ctx = g_pip_ctx_ir;
	if (pip_ctx == NULL) {
		printf("error:pip_ctx null\n");
		return 0;
	}

	return v4l2_rawrd_enqueue(&pip_ctx->rawrd_ctx, fd, param);
}
int v4l2_isp_luma_dequeue(void)
{
	struct isp_pip_ctx_s *pip_ctx = g_pip_ctx_ir;
	if (pip_ctx == NULL) {
		//printf("error:pip_ctx null\n");
		return 0;
	}

	return v4l2_rawrd_dequeue(&pip_ctx->rawrd_ctx);
}

int v4l2_isp_luma_proc(bool is_flood, int fd, void *param, int sequence)
{
	struct isp_pip_ctx_s *pip_ctx;

	if (is_flood) {
		pip_ctx = g_pip_ctx_ir;
	} else {
		pip_ctx = g_pip_ctx_pro;
	}

	if (pip_ctx == NULL) {
		//printf("error:pip_ctx null\n");
		return 0;
	}

#ifdef DEBUG_TIME
	struct timeval time1, time2;
	float use_time;
	gettimeofday(&time1, NULL);
#endif

	v4l2_rawrd_enqueue(&pip_ctx->rawrd_ctx, fd, param);
	v4l2_isp_trigger(&pip_ctx->isp_ctx, sequence);
	v4l2_rawrd_dequeue(&pip_ctx->rawrd_ctx);

#ifdef DEBUG_TIME
	gettimeofday(&time2, NULL);
	use_time = (time2.tv_sec - time1.tv_sec) * 1000 +
		(float)(time2.tv_usec - time1.tv_usec) / 1000;
	printf("%s %s: use_time %f ms\n", __func__,
			is_flood?"flood":"pro  ", use_time);
#endif

	return 0;
}

float v4l2_isp_luma_get_record_exp(int sequence)
{
	sequence -= 2;
	if (sequence < 0)
		return DEFAULT_EXP;

	float exposure = 0;

	struct exp_record_s *record = &luma_record;

	pthread_mutex_lock(&record->mut);
	unsigned char index = record->index;
	for (int i = 0; i < RECORD_MAX; i++) {
		unsigned char pos = index + RECORD_MAX - 1 - i;
		pos %= RECORD_MAX;
		if (record->sequence[pos] == sequence) {
			exposure = record->exposure[pos];
			break;
		}
	}
	pthread_mutex_unlock(&record->mut);

	return exposure;
}

void v4l2_isp_luma_cal_exp(void *param,
		void *buffer, int index, int dma_fd, int sequence)
{
	struct rkisp32_isp_stat_buffer *stats =
		(struct rkisp32_isp_stat_buffer *)buffer;

	struct exp_record_s *record = &luma_record;
	float exposure = v4l2_isp_luma_get_record_exp(stats->frame_id);

	if ((exposure == 0) || (exposure != record->exposure_last)) {
		printf("skip: exposure:%f last:%f\n",
				exposure, record->exposure_last);
		return;
	}

	float luma;
	unsigned int luma_sum = 0;
	unsigned int weight = 0;
	for (int i = 0; i < ISP32_RAWAEBIG_MEAN_NUM; i++) {
		luma_sum += stats->params.rawae1_0.data[i].channelg_xy *
			g_ae_weight[i];
		weight += g_ae_weight[i];
	}
	luma = (float)luma_sum / weight;

	if (luma > (TARGET_LUMA - TOLERANCE) &&
			luma < (TARGET_LUMA + TOLERANCE)) {
		pthread_mutex_lock(&record->mut);
		record->stable = true;
		pthread_mutex_unlock(&record->mut);
		return;
	}

	float exp_target = exposure * TARGET_LUMA / luma;
	exp_target = exp_target * SET_RATIO + (1.0 - SET_RATIO) * exposure;
	if (exp_target > MAX_EXP)
		exp_target = MAX_EXP;
	if (exp_target < MIN_EXP)
		exp_target = MIN_EXP;

#if 0
	if (record->exposure_last != exp_target)
		printf("stats id:%d, exp:%f luma:%f target:%f\n",
				stats->frame_id, exposure, luma, exp_target);
#endif

#if 1
	if (abs(record->exposure_last - exp_target) > 3.0)
		printf("target:%f\n", exp_target);
#endif

	pthread_mutex_lock(&record->mut);
	if (abs(record->exposure_last - exp_target) < STABLE_DIFF_MS)
		record->stable = true;
	else
		record->stable = false;

#if 0
	if (record->stable == false)
		printf("target:%f %s\n", exp_target, record->stable?"stable":"");
#endif

	record->exposure_last = exp_target;
	pthread_mutex_unlock(&record->mut);
}

void v4l2_isp_luma_cal_new_exp(void *param,
		void *buffer, int index, int dma_fd, int sequence)
{
	bool is_flood = !(sequence % 2);
	float ir_exp = PRO_EXP;

	if (is_flood) {
		v4l2_isp_luma_cal_exp(param, buffer, index, dma_fd, sequence);

		ir_exp = v4l2_isp_luma_update_exp(sequence);
	}

	struct exp_record_s *record = &luma_record;
	if (record->sensor_cb)
		record->sensor_cb(record->sensor_fd, ir_exp);
}

float v4l2_isp_luma_update_exp(int sequence)
{
	float exposure;
	struct exp_record_s *record = &luma_record;

	pthread_mutex_lock(&record->mut);

	unsigned char index = record->index;
	exposure = record->exposure_last;
	record->sequence[index] = sequence;
	record->exposure[index] = exposure;
	record->index++;
	record->index %= RECORD_MAX;

	pthread_mutex_unlock(&record->mut);

	return exposure + STROBE_DELAY_MS;
}

void v4l2_isp_luma_clr_stable(void)
{
	struct exp_record_s *record = &luma_record;

	pthread_mutex_lock(&record->mut);
	record->stable = false;
	pthread_mutex_unlock(&record->mut);
}

bool v4l2_isp_luma_get_stable(void)
{
	struct exp_record_s *record = &luma_record;
	bool stable;

	pthread_mutex_lock(&record->mut);
	stable = record->stable;
	pthread_mutex_unlock(&record->mut);

	return stable;
}
