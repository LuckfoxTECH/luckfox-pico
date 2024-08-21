/*
 * Copyright 2023 Rockchip Electronics Co. LTD
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
 *
 */
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>
#include <time.h>
#include <unistd.h>

#include "rk_aiq.h"
#include "rk_aiq_mems_sensor.h"
#include "rkiio.h"
#include "rtsp_demo.h"
#include "sample_comm.h"
#include "uAPI/rk_aiq_user_api_sysctl.h"

#define BUFFER_SIZE 255
#define VI_CHN_MAX 3
#define VENC_CHN_MAX 4

typedef struct _rkThreadStatus {
	RK_BOOL bIfMainThreadQuit;
} ThreadStatus;

/* global param */
ThreadStatus *gPThreadStatus = RK_NULL;
RK_S32 g_exit_result = RK_SUCCESS;
pthread_mutex_t g_rtsp_mutex = {0};
RK_BOOL g_rtsp_ifenbale = RK_FALSE;
rtsp_demo_handle g_rtsplive = RK_NULL;
static rtsp_session_handle g_rtsp_session[4] = {0};

typedef struct _rkMpiCtx {
	SAMPLE_VI_CTX_S vi;
	SAMPLE_VENC_CTX_S venc;
} SAMPLE_MPI_CTX_S;

static void program_handle_error(const char *func, RK_U32 line) {
	RK_LOGE("func: <%s> line: <%d> error exit!", func, line);
	g_exit_result = RK_FAILURE;
	gPThreadStatus->bIfMainThreadQuit = RK_TRUE;
}

static void program_normal_exit(const char *func, RK_U32 line) {
	RK_LOGE("func: <%s> line: <%d> normal exit!", func, line);
	gPThreadStatus->bIfMainThreadQuit = RK_TRUE;
}

static void sigterm_handler(int sig) {
	fprintf(stderr, "signal %d\n", sig);
	program_normal_exit(__func__, __LINE__);
}

static RK_CHAR optstr[] = "?::a::d:c:f:w:h:e:o:I:l:m:";
static const struct option long_options[] = {
    {"aiq", optional_argument, NULL, 'a'},
    {"device_name", required_argument, NULL, 'd'},
    {"chn_id", required_argument, NULL, 'c'},
    {"pixel_format", optional_argument, NULL, 'f'},
    {"width", required_argument, NULL, 'w'},
    {"height", required_argument, NULL, 'h'},
    {"eis_mode", required_argument, NULL, 'e'},
    {"output_path", required_argument, NULL, 'o'},
    {"camid", required_argument, NULL, 'I'},
    {"loop_count", required_argument, NULL, 'l'},
    {"hdr_mode", required_argument, NULL, 'h' + 'm'},
    {"help", optional_argument, NULL, '?'},
    {NULL, 0, NULL, 0},
};

static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
static bool g_init_flag = false;
static uint32_t g_sensor_cnt = 0;
static uint32_t g_handle_cnt = 0;

typedef struct rkiio_sensor_ctx_s {
	uint64_t ctx_id;
} rkiio_sensor_ctx_t;

typedef struct rkiio_sensor_handle_buf_s {
	pthread_mutex_t buf_mutex;
	bool is_valid;
	void *buf;
} rkiio_sensor_handle_buf_t;

typedef struct rkiio_sensor_handle_ctx_s {
	mems_sensor_type_t sensor_type;
	RKIIO_DATA_HANDLE handle;
	uint32_t max_data_num;
	uint32_t buf_cnt;
	rkiio_sensor_handle_buf_t *buf_pool;
	pthread_mutex_t fifo_buf_mutex;
	RKIIO_FIFO_DATA fifo_data_buf;
} rkiio_sensor_handle_t;

static mems_sensor_ctx_t rkiio_aiq_sensor_ctx_create() {
	pthread_mutex_lock(&g_mutex);
	g_sensor_cnt++;
	if (!g_init_flag) {
		rkiio_sys_init();
		rkiio_timestamp_type_set_all(RKIIO_TIMESTAMP_MONOTONIC);
	}
	g_init_flag = true;
	pthread_mutex_unlock(&g_mutex);
	rkiio_sensor_ctx_t *ctx = (rkiio_sensor_ctx_t *)malloc(sizeof(rkiio_sensor_ctx_t));
	memset(ctx, 0, sizeof(rkiio_sensor_ctx_t));
	return (mems_sensor_ctx_t)ctx;
}

static mems_sensor_return_t rkiio_aiq_sensor_ctx_destroy(mems_sensor_ctx_t ctx) {
	if (!ctx) {
		return SENSOR_ERR_NULL_PTR;
	}
	rkiio_sensor_ctx_t *rkiio_ctx = (rkiio_sensor_ctx_t *)ctx;
	free(rkiio_ctx);
	pthread_mutex_lock(&g_mutex);
	g_sensor_cnt--;
	if (!g_sensor_cnt) {
		rkiio_sys_uninit();
		g_init_flag = false;
	}
	pthread_mutex_unlock(&g_mutex);
	return SENSOR_NO_ERR;
}

static mems_sensor_return_t rkiio_aiq_sensors_list_get(mems_sensor_ctx_t ctx,
                                                       mems_sensor_type_t sensor_type,
                                                       mems_sensor_list_t *sensors_list) {
	if (!g_init_flag) {
		return SENSOR_ERR_FAIL;
	}
	if (!sensors_list || !ctx) {
		return SENSOR_ERR_NULL_PTR;
	}
	switch (sensor_type) {
	case SENSOR_GRYO_TYPE:
		sensors_list->key_list = rkiio_anglvel_dev_list_get(&sensors_list->count);
		break;
	case SENSOR_ACCEL_TYPE:
		sensors_list->key_list = rkiio_accel_dev_list_get(&sensors_list->count);
		break;
	case SENSOR_TEMP_TYPE:
		sensors_list->key_list = rkiio_temp_dev_list_get(&sensors_list->count);
		break;
	case SENSOR_ALL_TYPE:
		sensors_list->key_list = rkiio_all_dev_list_get(&sensors_list->count);
		break;
	default:
		return SENSOR_ERR_MISMATCH_SENSOR;
	}
	if (sensors_list->count) {
		return SENSOR_NO_ERR;
	}
	return SENSOR_ERR_MISMATCH_SENSOR;
}

static mems_sensor_return_t
rkiio_aiq_sensors_list_release(mems_sensor_list_t *sensors_list) {
	if (!sensors_list) {
		return SENSOR_ERR_NULL_PTR;
	}
	int32_t ret = rkiio_dev_list_destroy(sensors_list->key_list, sensors_list->count);
	if (ret) {
		return SENSOR_ERR_FAIL;
	}
	return SENSOR_NO_ERR;
}

static mems_sensor_return_t rkiio_aiq_sensor_cap_get(mems_sensor_ctx_t ctx,
                                                     mems_sensor_type_t sensor_type,
                                                     char *sensor_key,
                                                     mems_sensor_capabilities_t *caps) {
	if (!g_init_flag) {
		return SENSOR_ERR_FAIL;
	}
	if (!ctx) {
		return SENSOR_ERR_NULL_PTR;
	}
	memset(caps, 0, sizeof(mems_sensor_capabilities_t));
	caps->type = sensor_type;
	switch (sensor_type) {
	case SENSOR_GRYO_TYPE:
		rkiio_sampling_rate_cap_get(sensor_key, RKIIO_SENSOR_ANGLVEL_TYPE,
		                            &(caps->sample_rates), &(caps->num_sample_rates));
		break;
	case SENSOR_ACCEL_TYPE:
		rkiio_sampling_rate_cap_get(sensor_key, RKIIO_SENSOR_ACCEL_TYPE,
		                            &(caps->sample_rates), &(caps->num_sample_rates));
		break;
	case SENSOR_TEMP_TYPE:
		rkiio_sampling_rate_cap_get(sensor_key, RKIIO_SENSOR_TEMP_TYPE,
		                            &(caps->sample_rates), &(caps->num_sample_rates));
		break;
	case SENSOR_ALL_TYPE:
		rkiio_sampling_rate_cap_get(sensor_key, RKIIO_SENSOR_ALL_TYPE,
		                            &(caps->sample_rates), &(caps->num_sample_rates));
		break;
	default:
		return SENSOR_ERR_MISMATCH_SENSOR;
	}
	caps->is_data_valid = true;
	return SENSOR_NO_ERR;
}

static mems_sensor_return_t
rkiio_aiq_sensor_cap_release(mems_sensor_capabilities_t *caps) {
	if (caps->sample_rates)
		rkiio_sampling_rate_cap_release(caps->sample_rates);
	return SENSOR_NO_ERR;
}

// only support sampling rate
static mems_sensor_return_t rkiio_aiq_sensor_config_get(mems_sensor_ctx_t ctx,
                                                        mems_sensor_type_t sensor_type,
                                                        char *sensor_key,
                                                        mems_sensor_config_t *cfg) {
	if (!g_init_flag) {
		return SENSOR_ERR_FAIL;
	}
	if (!cfg || !ctx) {
		return SENSOR_ERR_NULL_PTR;
	}
	if (g_handle_cnt) {
		return SENSOR_ERR_DEV_BUSY;
	}
	memset(cfg, 0, sizeof(mems_sensor_config_t));
	int32_t ret = 0;
	switch (sensor_type) {
	case SENSOR_GRYO_TYPE:
		ret = rkiio_sampling_rate_get(sensor_key, RKIIO_SENSOR_ANGLVEL_TYPE,
		                              &(cfg->sample_rate));
		break;
	case SENSOR_ACCEL_TYPE:
		ret = rkiio_sampling_rate_get(sensor_key, RKIIO_SENSOR_ACCEL_TYPE,
		                              &(cfg->sample_rate));
		break;
	case SENSOR_TEMP_TYPE:
		ret = rkiio_sampling_rate_get(sensor_key, RKIIO_SENSOR_TEMP_TYPE,
		                              &(cfg->sample_rate));
		break;
	case SENSOR_ALL_TYPE:
		ret = rkiio_sampling_rate_get(sensor_key, RKIIO_SENSOR_ALL_TYPE,
		                              &(cfg->sample_rate));
		break;
	default:
		return SENSOR_ERR_MISMATCH_SENSOR;
	}
	if (ret) {
		cfg->sample_rate = 0.0f;
		return SENSOR_ERR_FAIL;
	}
	return SENSOR_NO_ERR;
}

static mems_sensor_return_t rkiio_aiq_sensor_config_set(mems_sensor_ctx_t ctx,
                                                        mems_sensor_type_t sensor_type,
                                                        char *sensor_key,
                                                        mems_sensor_config_t cfg) {
	if (!g_init_flag) {
		return SENSOR_ERR_FAIL;
	}
	if (!ctx) {
		return SENSOR_ERR_NULL_PTR;
	}
	if (g_handle_cnt) {
		return SENSOR_ERR_DEV_BUSY;
	}
	int32_t ret = 0;
	switch (sensor_type) {
	case SENSOR_GRYO_TYPE:
		ret = rkiio_sampling_rate_set(sensor_key, RKIIO_SENSOR_ANGLVEL_TYPE,
		                              cfg.sample_rate);
		break;
	case SENSOR_ACCEL_TYPE:
		ret =
		    rkiio_sampling_rate_set(sensor_key, RKIIO_SENSOR_ACCEL_TYPE, cfg.sample_rate);
		break;
	case SENSOR_TEMP_TYPE:
		ret =
		    rkiio_sampling_rate_set(sensor_key, RKIIO_SENSOR_TEMP_TYPE, cfg.sample_rate);
		break;
	case SENSOR_ALL_TYPE:
		ret = rkiio_sampling_rate_set(sensor_key, RKIIO_SENSOR_ALL_TYPE, cfg.sample_rate);
		break;
	default:
		return SENSOR_ERR_MISMATCH_SENSOR;
	}
	if (ret) {
		return SENSOR_ERR_FAIL;
	}
	return SENSOR_NO_ERR;
}

static uint32_t _rkiio_aiq_buf_id_get(void *buf, uint32_t max_data_num) {
	uint8_t *data = (uint8_t *)buf;
	uint64_t idx = sizeof(mems_sensor_event_t) * max_data_num;
	int32_t *reserverd_data = (int32_t *)&(data[idx]);
	return *reserverd_data;
}

static void _rkiio_aiq_buf_id_set(void *buf, uint32_t max_data_num, uint32_t id) {
	uint8_t *data = (uint8_t *)buf;
	uint64_t idx = sizeof(mems_sensor_event_t) * max_data_num;
	int32_t *reserverd_data = (int32_t *)&(data[idx]);
	*reserverd_data = id;
}

static mems_sensor_handle_t rkiio_aiq_sensor_handle_create(mems_sensor_ctx_t ctx,
                                                           mems_sensor_type_t sensor_type,
                                                           char *sensor_key,
                                                           uint32_t max_data_num,
                                                           uint32_t buf_cnt) {
	if (!g_init_flag || !ctx || !max_data_num || !buf_cnt) {
		return NULL;
	}
	rkiio_sensor_handle_t *sensor_handle =
	    (rkiio_sensor_handle_t *)malloc(sizeof(rkiio_sensor_handle_t));
	memset(sensor_handle, 0, sizeof(rkiio_sensor_handle_t));
	sensor_handle->sensor_type = sensor_type;
	sensor_handle->buf_cnt = buf_cnt;

	sensor_handle->handle = rkiio_data_handle_create(sensor_key, 0, max_data_num);
	uint32_t real_max_data_num = 0;
	rkiio_data_handle_max_fifo_num_get(sensor_handle->handle, &real_max_data_num);
	sensor_handle->max_data_num =
	    (max_data_num > real_max_data_num) ? max_data_num : real_max_data_num;

	pthread_mutex_init(&(sensor_handle->fifo_buf_mutex), NULL);
	sensor_handle->fifo_data_buf =
	    (RKIIO_FIFO_DATA)malloc(sizeof(rkiio_data_0_t) * sensor_handle->max_data_num);

	sensor_handle->buf_pool =
	    (rkiio_sensor_handle_buf_t *)malloc(sizeof(rkiio_sensor_handle_buf_t) * buf_cnt);
	memset(sensor_handle->buf_pool, 0, sizeof(rkiio_sensor_handle_buf_t) * buf_cnt);
	for (uint32_t i = 0; i < buf_cnt; i++) {
		pthread_mutex_init(&(sensor_handle->buf_pool[i].buf_mutex), NULL);
		sensor_handle->buf_pool[i].is_valid = true;
		sensor_handle->buf_pool[i].buf = (void *)malloc(
		    sizeof(mems_sensor_event_t) * sensor_handle->max_data_num + sizeof(uint32_t));
		memset(sensor_handle->buf_pool[i].buf, 0,
		       sizeof(mems_sensor_event_t) * sensor_handle->max_data_num +
		           sizeof(uint32_t));
		_rkiio_aiq_buf_id_set(sensor_handle->buf_pool[i].buf, sensor_handle->max_data_num,
		                      i);
	}
	pthread_mutex_lock(&g_mutex);
	g_handle_cnt++;
	pthread_mutex_unlock(&g_mutex);
	return (mems_sensor_handle_t)sensor_handle;
}

static mems_sensor_return_t rkiio_aiq_sensor_handle_destroy(mems_sensor_handle_t handle) {
	if (!handle) {
		return SENSOR_ERR_NULL_PTR;
	}
	rkiio_sensor_handle_t *sensor_handle = (rkiio_sensor_handle_t *)handle;
	pthread_mutex_lock(&(sensor_handle->fifo_buf_mutex));
	rkiio_data_handle_destroy(sensor_handle->handle);
	if (sensor_handle->fifo_data_buf)
		free(sensor_handle->fifo_data_buf);
	if (sensor_handle->buf_pool) {
		for (uint32_t i = 0; i < sensor_handle->buf_cnt; i++) {
			if (sensor_handle->buf_pool[i].buf)
				free(sensor_handle->buf_pool[i].buf);
		}
		free(sensor_handle->buf_pool);
	}
	pthread_mutex_unlock(&(sensor_handle->fifo_buf_mutex));
	free(sensor_handle);
	pthread_mutex_lock(&g_mutex);
	g_handle_cnt--;
	pthread_mutex_unlock(&g_mutex);
	return SENSOR_NO_ERR;
}

static uint32_t _rkiio_aiq_get_vaild_buf(rkiio_sensor_handle_t *handle) {
	while (1) {
		for (uint32_t i = 0; i < handle->buf_cnt; i++) {
			pthread_mutex_lock(&(handle->buf_pool[i].buf_mutex));
			// printf("%s: id: %u, valid: %d\n", __FUNCTION__, i,
			// handle->buf_pool[i].is_valid);
			if (handle->buf_pool[i].is_valid) {
				handle->buf_pool[i].is_valid = false;
				pthread_mutex_unlock(&(handle->buf_pool[i].buf_mutex));
				return i;
			}
			pthread_mutex_unlock(&(handle->buf_pool[i].buf_mutex));
		}
		usleep(100000);
	}
}

static mems_sensor_data_t _rkiio_aiq_getData_gyro(rkiio_sensor_handle_t *handle,
                                                  uint64_t data_num) {
	rkiio_data_0_t *fifo = (rkiio_data_0_t *)handle->fifo_data_buf;
	uint32_t valid_id = _rkiio_aiq_get_vaild_buf(handle);
	mems_sensor_event_t *event = (mems_sensor_event_t *)(handle->buf_pool[valid_id].buf);
	memset(event, 0, sizeof(mems_sensor_event_t) * data_num);
	for (uint64_t i = 0; i < data_num; i++) {
		memcpy(&(event[i].gyro), &(fifo[i].anglvel), sizeof(xyz_data_t));
		event[i].timestamp_us = fifo[i].timestamp;
		event[i].id = fifo[i].id;
		// printf("%llu:, x: %f, y: %f, z: %f, t: %llu, %llu:, x: %f, y: %f, z: %f, t:
		// %llu\n",
		//     fifo[i].id, fifo[i].anglvel.x, fifo[i].anglvel.y, fifo[i].anglvel.z,
		//     fifo[i].timestamp, event[i].id, event[i].gyro.x, event[i].gyro.y,
		//     event[i].gyro.z, event[i].timestamp_us);
	}
	return (mems_sensor_data_t)event;
}

static mems_sensor_data_t _rkiio_aiq_getData_accel(rkiio_sensor_handle_t *handle,
                                                   uint64_t data_num) {
	rkiio_data_0_t *fifo = (rkiio_data_0_t *)handle->fifo_data_buf;
	uint32_t valid_id = _rkiio_aiq_get_vaild_buf(handle);
	mems_sensor_event_t *event = (mems_sensor_event_t *)(handle->buf_pool[valid_id].buf);
	memset(event, 0, sizeof(mems_sensor_event_t) * data_num);
	for (uint64_t i = 0; i < data_num; i++) {
		memcpy(&(event[i].accel), &(fifo[i].accel), sizeof(xyz_data_t));
		event[i].timestamp_us = fifo[i].timestamp;
		event[i].id = fifo[i].id;
	}
	return (mems_sensor_data_t)event;
}

static mems_sensor_data_t _rkiio_aiq_getData_temp(rkiio_sensor_handle_t *handle,
                                                  uint64_t data_num) {
	rkiio_data_0_t *fifo = (rkiio_data_0_t *)handle->fifo_data_buf;
	uint32_t valid_id = _rkiio_aiq_get_vaild_buf(handle);
	mems_sensor_event_t *event = (mems_sensor_event_t *)(handle->buf_pool[valid_id].buf);
	memset(event, 0, sizeof(mems_sensor_event_t) * data_num);
	for (uint64_t i = 0; i < data_num; i++) {
		// temp alter to s32?
		event[i].temperature = fifo[i].temp;
		event[i].timestamp_us = fifo[i].timestamp;
		event[i].id = fifo[i].id;
	}
	return (mems_sensor_data_t)event;
}

static mems_sensor_data_t _rkiio_aiq_getData_all(rkiio_sensor_handle_t *handle,
                                                 uint64_t data_num) {
	rkiio_data_0_t *fifo = (rkiio_data_0_t *)handle->fifo_data_buf;
	uint32_t valid_id = _rkiio_aiq_get_vaild_buf(handle);
	mems_sensor_event_t *event = (mems_sensor_event_t *)(handle->buf_pool[valid_id].buf);
	memset(event, 0, sizeof(mems_sensor_event_t) * data_num);
	for (uint64_t i = 0; i < data_num; i++) {
		memcpy(&(event[i].all.gyro), &(fifo[i].anglvel), sizeof(xyz_data_t));
		memcpy(&(event[i].all.accel), &(fifo[i].accel), sizeof(xyz_data_t));
		// temp alter to s32?
		event[i].all.temperature = fifo[i].temp;
		event[i].timestamp_us = fifo[i].timestamp;
		event[i].id = fifo[i].id;
	}
	return (mems_sensor_data_t)event;
}

static mems_sensor_data_t rkiio_aiq_getData(mems_sensor_handle_t handle,
                                            size_t *num_samples) {
	if (!handle) {
		return NULL;
	}
	int32_t ret = 0;
	rkiio_sensor_handle_t *sensor_handle = (rkiio_sensor_handle_t *)handle;
	pthread_mutex_lock(&(sensor_handle->fifo_buf_mutex));
	uint64_t data_num = 0;
	ret = rkiio_data_get_all(sensor_handle->handle, sensor_handle->fifo_data_buf,
	                         &data_num);
	if (ret || !data_num) {
		*num_samples = 0;
		pthread_mutex_unlock(&(sensor_handle->fifo_buf_mutex));
		return NULL;
	}
	mems_sensor_data_t data = NULL;
	switch (sensor_handle->sensor_type) {
	case SENSOR_GRYO_TYPE:
		data = _rkiio_aiq_getData_gyro(sensor_handle, data_num);
		break;
	case SENSOR_ACCEL_TYPE:
		data = _rkiio_aiq_getData_accel(sensor_handle, data_num);
		break;
	case SENSOR_TEMP_TYPE:
		data = _rkiio_aiq_getData_temp(sensor_handle, data_num);
		break;
	case SENSOR_ALL_TYPE:
		data = _rkiio_aiq_getData_all(sensor_handle, data_num);
		break;
	default:
		data = NULL;
	}
	*num_samples = data ? (size_t)data_num : 0;
	pthread_mutex_unlock(&(sensor_handle->fifo_buf_mutex));
	return data;
}

static mems_sensor_data_t rkiio_aiq_getLastNSamples(mems_sensor_handle_t handle,
                                                    size_t num_samples) {
	if (!handle || !num_samples) {
		return NULL;
	}
	rkiio_sensor_handle_t *sensor_handle = (rkiio_sensor_handle_t *)handle;
	if ((uint32_t)num_samples > sensor_handle->max_data_num) {
		printf("%s: num_samples > max_data_num\n", __FUNCTION__);
		return NULL;
	}
	int32_t ret = 0;
	pthread_mutex_lock(&(sensor_handle->fifo_buf_mutex));
	uint64_t data_num = (uint64_t)num_samples;
	ret = rkiio_data_read(sensor_handle->handle, sensor_handle->fifo_data_buf, data_num);
	if (ret) {
		pthread_mutex_unlock(&(sensor_handle->fifo_buf_mutex));
		return NULL;
	}
	mems_sensor_data_t data = NULL;
	switch (sensor_handle->sensor_type) {
	case SENSOR_GRYO_TYPE:
		data = _rkiio_aiq_getData_gyro(sensor_handle, data_num);
		break;
	case SENSOR_ACCEL_TYPE:
		data = _rkiio_aiq_getData_accel(sensor_handle, data_num);
		break;
	case SENSOR_TEMP_TYPE:
		data = _rkiio_aiq_getData_temp(sensor_handle, data_num);
		break;
	case SENSOR_ALL_TYPE:
		data = _rkiio_aiq_getData_all(sensor_handle, data_num);
		break;
	default:
		data = NULL;
	}
	pthread_mutex_unlock(&(sensor_handle->fifo_buf_mutex));
	return data;
}

static mems_sensor_return_t
rkiio_aiq_sensor_data_release(mems_sensor_handle_t sensor_handle,
                              mems_sensor_data_t data) {
	if (!data || !sensor_handle) {
		return SENSOR_ERR_NULL_PTR;
	}
	rkiio_sensor_handle_t *handle = (rkiio_sensor_handle_t *)sensor_handle;
	uint32_t idx = _rkiio_aiq_buf_id_get(data, handle->max_data_num);
	pthread_mutex_lock(&(handle->buf_pool[idx].buf_mutex));
	handle->buf_pool[idx].is_valid = true;
	pthread_mutex_unlock(&(handle->buf_pool[idx].buf_mutex));
	return SENSOR_NO_ERR;
}

/******************************************************************************
 * function : show usage
 ******************************************************************************/
static void print_usage(const RK_CHAR *name) {
	printf("usage example:\n");
	printf("\t%s -w 1296 -h 972 -a /etc/iqfiles/  -e 1\n", name);
#ifdef RKAIQ
	printf("\t-a | --aiq: enable aiq with dirpath provided, eg:-a /etc/iqfiles/, "
	       "set dirpath empty to using path by default, without this option aiq "
	       "should run in other application\n");
#endif
	printf("\t-d | --device_name: set pcDeviceName, eg: /dev/video0 Default "
	       "NULL\n");
	printf("\t-c | --chn_id: channel id, default: 1\n");
	printf("\t-f | --pixel_format: camera Format, Default nv12, "
	       "Value:nv12,nv16,uyvy,yuyv.\n");
	printf("\t-w | --width: camera with, Default 1296\n");
	printf("\t-h | --height: camera height, Default 972\n");
	printf("\t-e | --eis_mode: set EIS(Electric Image Stabilization) mode, "
	       "0: close, 1: enable eis, Default: 1\n");
	printf("\t-o | --output_path: vi output file path, Default NULL\n");
	printf("\t-I | --camid: camera ctx id, Default 0\n");
	printf("\t-l | --loop_count: loop count, Default -1\n");
	printf("\t--hdr_mode: set hdr mode, 0: normal 1: HDR2, 2: HDR3, Default: 0\n");
}

static void *venc_get_stream(void *pArgs) {

	SAMPLE_VENC_CTX_S *ctx = (SAMPLE_VENC_CTX_S *)pArgs;
	RK_S32 s32Ret = RK_FAILURE;
	FILE *fp = RK_NULL;
	RK_S32 s32fd = 0;
	RK_S32 loopCount = 0;
	RK_VOID *pData = RK_NULL;
	RK_CHAR name[BUFFER_SIZE] = {0};
	sprintf(name, "venc_%d_get_stream", ctx->s32ChnId);
	prctl(PR_SET_NAME, name);

	if (ctx->dstFilePath) {
		memset(name, 0, BUFFER_SIZE);
		snprintf(name, sizeof(name), "/%s/venc_%d.bin", ctx->dstFilePath, ctx->s32ChnId);
		fp = fopen(name, "wb");
		if (fp == RK_NULL) {
			RK_LOGE("chn %d can't open %s file !\n", ctx->s32ChnId, ctx->dstFilePath);
			program_handle_error(__func__, __LINE__);
			return RK_NULL;
		}
		s32fd = fileno(fp);
	}
	while (!gPThreadStatus->bIfMainThreadQuit) {
		s32Ret = SAMPLE_COMM_VENC_GetStream(ctx, &pData);
		if (s32Ret == RK_SUCCESS) {

			if (ctx->s32loopCount > 0) {
				if (loopCount >= ctx->s32loopCount) {
					SAMPLE_COMM_VENC_ReleaseStream(ctx);
					program_normal_exit(__func__, __LINE__);
					break;
				}
			}

			if (fp && !gPThreadStatus->bIfMainThreadQuit) {
				fwrite(pData, 1, ctx->stFrame.pstPack->u32Len, fp);
				fflush(fp);
			}
			if (g_rtsp_ifenbale) {
				pthread_mutex_lock(&g_rtsp_mutex);
				rtsp_tx_video(g_rtsp_session[ctx->s32ChnId], pData,
				              ctx->stFrame.pstPack->u32Len, ctx->stFrame.pstPack->u64PTS);
				rtsp_do_event(g_rtsplive);
				pthread_mutex_unlock(&g_rtsp_mutex);
			} else {
				RK_LOGD("venc %d get_stream count: %d", ctx->s32ChnId, loopCount);
			}

			RK_LOGD("venc %d get_stream count: %d", ctx->s32ChnId, loopCount);

			SAMPLE_COMM_VENC_ReleaseStream(ctx);
			loopCount++;
		}
	}

	if (fp) {
		fsync(s32fd);
		fclose(fp);
	}

	RK_LOGE("venc_get_stream chnid:%d exit", ctx->s32ChnId);
	return RK_NULL;
}

static RK_S32 rtsp_init(CODEC_TYPE_E enCodecType) {
	RK_S32 i = 0;
	g_rtsplive = create_rtsp_demo(554);
	RK_CHAR rtspAddr[BUFFER_SIZE] = {0};

	for (i = 0; i < VENC_CHN_MAX; i++) {

		sprintf(rtspAddr, "/live/%d", i);
		g_rtsp_session[i] = rtsp_new_session(g_rtsplive, rtspAddr);
		if (enCodecType == RK_CODEC_TYPE_H264) {
			rtsp_set_video(g_rtsp_session[i], RTSP_CODEC_ID_VIDEO_H264, RK_NULL, 0);
		} else if (enCodecType == RK_CODEC_TYPE_H265) {
			rtsp_set_video(g_rtsp_session[i], RTSP_CODEC_ID_VIDEO_H265, RK_NULL, 0);
		} else {
			RK_LOGE("not support other type\n");
			g_rtsp_ifenbale = RK_FALSE;
			return RK_SUCCESS;
		}
		rtsp_sync_video_ts(g_rtsp_session[i], rtsp_get_reltime(), rtsp_get_ntptime());
		RK_LOGE("rtsp <%s> init success", rtspAddr);
	}
	g_rtsp_ifenbale = RK_TRUE;
	return RK_SUCCESS;
}

static RK_S32 rtsp_deinit(void) {
	if (g_rtsplive)
		rtsp_del_demo(g_rtsplive);
	return RK_SUCCESS;
}

static RK_S32 global_param_init(void) {

	gPThreadStatus = (ThreadStatus *)malloc(sizeof(ThreadStatus));
	if (!gPThreadStatus) {
		printf("malloc for gPThreadStatus failure\n");
		goto __global_init_fail;
	}
	memset(gPThreadStatus, 0, sizeof(ThreadStatus));

	if (RK_SUCCESS != pthread_mutex_init(&g_rtsp_mutex, RK_NULL)) {
		RK_LOGE("pthread_mutex_init failure");
		goto __global_init_fail;
	}

	return RK_SUCCESS;

__global_init_fail:
	if (gPThreadStatus) {
		free(gPThreadStatus);
		gPThreadStatus = RK_NULL;
	}
	return RK_FAILURE;
}

static RK_S32 global_param_deinit(void) {

	if (gPThreadStatus) {
		free(gPThreadStatus);
		gPThreadStatus = RK_NULL;
	}

	pthread_mutex_destroy(&g_rtsp_mutex);

	return RK_SUCCESS;
}

/******************************************************************************
 * function    : main()
 * Description : main
 ******************************************************************************/
int main(int argc, char *argv[]) {
	rk_aiq_mems_sensor_intf_t eis_api = {
	    .createContext = rkiio_aiq_sensor_ctx_create,
	    .destroyContext = rkiio_aiq_sensor_ctx_destroy,
	    .getSensorList = rkiio_aiq_sensors_list_get,
	    .releaseSensorList = rkiio_aiq_sensors_list_release,
	    .getSensorCapabilities = rkiio_aiq_sensor_cap_get,
	    .releaseSensorCapabilities = rkiio_aiq_sensor_cap_release,
	    .getConfig = rkiio_aiq_sensor_config_get,
	    .setConfig = rkiio_aiq_sensor_config_set,
	    .createHandle = rkiio_aiq_sensor_handle_create,
	    .destroyHandle = rkiio_aiq_sensor_handle_destroy,
	    .getData = rkiio_aiq_getData,
	    .getLastNSamples = rkiio_aiq_getLastNSamples,
	    .releaseSamplesData = rkiio_aiq_sensor_data_release,
	};
	SAMPLE_MPI_CTX_S *ctx = RK_NULL;
	RK_S32 s32Ret = RK_FAILURE;
	RK_U32 u32ViWidth = 1296;
	RK_U32 u32ViHeight = 972;
	RK_CHAR *pOutPath = NULL;
	RK_CHAR *pDeviceName = NULL;
	RK_S32 s32CamId = 0;
	RK_S32 s32ChnId = 1;
	RK_S32 s32loopCnt = -1;
	RK_U32 u32VencFps = 25;
	RK_U32 u32BitRate = 2 * 1024;
	RK_U32 u32ViBuffCnt = 2;
	RK_U32 u32VencBuffSize = 0;
	RK_U32 EIS_Mode = 1;
	CODEC_TYPE_E enCodecType = RK_CODEC_TYPE_H264;
	VENC_RC_MODE_E enRcMode = VENC_RC_MODE_H264CBR;
	MPP_CHN_S stSrcChn, stDestChn;
	PIXEL_FORMAT_E PixelFormat = RK_FMT_YUV420SP;
	COMPRESS_MODE_E CompressMode = COMPRESS_MODE_NONE;
	rk_aiq_working_mode_t hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;
	pthread_t venc_thread_id;

	if (argc < 2) {
		print_usage(argv[0]);
		return 0;
	}

	ctx = (SAMPLE_MPI_CTX_S *)(malloc(sizeof(SAMPLE_MPI_CTX_S)));
	memset(ctx, 0, sizeof(SAMPLE_MPI_CTX_S));

	s32Ret = global_param_init();
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("global_param_init failure");
		g_exit_result = RK_FAILURE;
		goto __PARAM_INIT_FAILED;
	}

	signal(SIGINT, sigterm_handler);
	signal(SIGTERM, sigterm_handler);

#ifdef RKAIQ
	RK_BOOL bMultictx = RK_FALSE;
#endif
	int c;
	char *iq_file_dir = NULL;
	while ((c = getopt_long(argc, argv, optstr, long_options, NULL)) != -1) {
		const char *tmp_optarg = optarg;
		switch (c) {
		case 'a':
			if (!optarg && NULL != argv[optind] && '-' != argv[optind][0]) {
				tmp_optarg = argv[optind++];
			}
			if (tmp_optarg) {
				iq_file_dir = (char *)tmp_optarg;
			} else {
				iq_file_dir = NULL;
			}
			break;
		case 'd':
			pDeviceName = optarg;
			break;
		case 'c':
			s32ChnId = atoi(optarg);
			break;
		case 'f':
			if (!strcmp(optarg, "nv12")) {
				PixelFormat = RK_FMT_YUV420SP;
			} else if (!strcmp(optarg, "nv16")) {
				PixelFormat = RK_FMT_YUV422SP;
			} else if (!strcmp(optarg, "uyvy")) {
				PixelFormat = RK_FMT_YUV422_UYVY;
			} else if (!strcmp(optarg, "yuyv")) {
				PixelFormat = RK_FMT_YUV422_YUYV;
			}
#if defined(RV1106)
			else if (!strcmp(optarg, "rgb565")) {
				PixelFormat = RK_FMT_RGB565;
				s32ChnId = 1;
			} else if (!strcmp(optarg, "xbgr8888")) {
				PixelFormat = RK_FMT_XBGR8888;
				s32ChnId = 1;
			}
#endif
			else {
				RK_LOGE("this pixel_format is not supported in the sample");
				print_usage(argv[0]);
				goto __FAILED2;
			}
			break;
		case 'w':
			u32ViWidth = atoi(optarg);
			break;
		case 'h':
			u32ViHeight = atoi(optarg);
			break;
		case 'e':
			EIS_Mode = atoi(optarg);
			break;
		case 'I':
			s32CamId = atoi(optarg);
			break;
		case 'l':
			s32loopCnt = atoi(optarg);
			break;
		case 'o':
			pOutPath = optarg;
			break;
		case 'h' + 'm':
			if (atoi(optarg) == 0) {
				hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;
			} else if (atoi(optarg) == 1) {
				hdr_mode = RK_AIQ_WORKING_MODE_ISP_HDR2;
			} else if (atoi(optarg) == 2) {
				hdr_mode = RK_AIQ_WORKING_MODE_ISP_HDR3;
			} else {
				RK_LOGE("input hdr_mode is not support(error)");
				print_usage(argv[0]);
				goto __FAILED2;
			}
			break;
		case '?':
		default:
			print_usage(argv[0]);
			return 0;
		}
	}

	printf("#CameraIdx: %d\n", s32CamId);
	printf("#pDeviceName: %s\n", pDeviceName);
	printf("#Output Path: %s\n", pOutPath);
	printf("#IQ Path: %s\n", iq_file_dir);
	printf("#EIS_MODE: %d\n", EIS_Mode);
	if (iq_file_dir) {
#ifdef RKAIQ
		printf("#Rkaiq XML DirPath: %s\n", iq_file_dir);
		printf("#bMultictx: %d\n\n", bMultictx);

		SAMPLE_COMM_ISP_Init(s32CamId, hdr_mode, bMultictx, iq_file_dir);
		if (EIS_Mode == 1) {
			SAMPLE_COMM_ISP_RegMemsSensorIntf(s32CamId, &eis_api);
		}
		SAMPLE_COMM_ISP_Run(s32CamId);
#endif
	}

	if (RK_MPI_SYS_Init() != RK_SUCCESS) {
		RK_LOGE("RK_MPI_SYS_Init failure");
		g_exit_result = RK_FAILURE;
		goto __FAILED;
	}

	s32Ret = rtsp_init(enCodecType);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("rtsp_init failure");
		g_exit_result = RK_FAILURE;
		goto __FAILED;
	}
	// Init VI
	ctx->vi.u32Width = u32ViWidth;
	ctx->vi.u32Height = u32ViHeight;
	ctx->vi.s32DevId = s32CamId;
	ctx->vi.u32PipeId = ctx->vi.s32DevId;
	ctx->vi.s32ChnId = s32ChnId;
	ctx->vi.stChnAttr.stIspOpt.stMaxSize.u32Width = u32ViWidth;
	ctx->vi.stChnAttr.stIspOpt.stMaxSize.u32Height = u32ViHeight;
	ctx->vi.stChnAttr.stIspOpt.u32BufCount = 2;
	ctx->vi.stChnAttr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
	ctx->vi.stChnAttr.u32Depth = 1;
	ctx->vi.stChnAttr.enPixelFormat = PixelFormat;
	ctx->vi.stChnAttr.enCompressMode = CompressMode;
	ctx->vi.stChnAttr.stFrameRate.s32SrcFrameRate = -1;
	ctx->vi.stChnAttr.stFrameRate.s32DstFrameRate = -1;
	ctx->vi.dstFilePath = pOutPath;
	ctx->vi.s32loopCount = s32loopCnt;
	ctx->vi.u32BufferLine = ctx->vi.u32Height;
	if (pDeviceName) {
		strcpy(ctx->vi.stChnAttr.stIspOpt.aEntityName, pDeviceName);
	}
	s32Ret = SAMPLE_COMM_VI_CreateChn(&ctx->vi);
	if (s32Ret != RK_SUCCESS) {
		g_exit_result = RK_FAILURE;
		RK_LOGE("SAMPLE_COMM_VI_CreateChn failure:%d", s32Ret);
		goto __FAILED;
	}

	/* Init VENC*/
	ctx->venc.s32ChnId = 0;
	ctx->venc.u32Width = u32ViWidth;
	ctx->venc.u32Height = u32ViHeight;
	ctx->venc.u32Fps = u32VencFps;
	ctx->venc.u32Gop = 50;
	ctx->venc.u32BitRate = u32BitRate;
	ctx->venc.enCodecType = enCodecType;
	ctx->venc.enRcMode = enRcMode;
	ctx->venc.getStreamCbFunc = venc_get_stream;
	ctx->venc.s32loopCount = s32loopCnt;
	ctx->venc.dstFilePath = pOutPath;
	ctx->venc.u32BufferLine = ctx->venc.u32Height;
	if (u32VencBuffSize) {
		ctx->venc.u32BuffSize = u32VencBuffSize;
	} else {
		ctx->venc.u32BuffSize = u32ViWidth * u32ViHeight / 2;
	}
	if (RK_CODEC_TYPE_H264 != enCodecType) {
		ctx->venc.stChnAttr.stVencAttr.u32Profile = 0;
	} else {
		ctx->venc.stChnAttr.stVencAttr.u32Profile = 100;
	}
	s32Ret = SAMPLE_COMM_VENC_CreateChn(&ctx->venc);
	if (s32Ret == RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_VENC_CreateChn success:%d", s32Ret);
	}

	/* VI bind VENC */
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = ctx->vi.s32DevId;
	stSrcChn.s32ChnId = ctx->vi.s32ChnId;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = ctx->venc.s32ChnId;
	s32Ret = SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);
	if (s32Ret == RK_SUCCESS) {
		RK_LOGE("VI and VENC bind success:%X", s32Ret);
	} else {
		program_handle_error(__func__, __LINE__);
	}

	pthread_create(&venc_thread_id, 0, venc_get_stream, (void *)&ctx->venc);

	printf("%s initial finish\n", __func__);

	while (!gPThreadStatus->bIfMainThreadQuit) {
		sleep(1);
	}

	printf("%s exit!\n", __func__);

	pthread_join(ctx->venc.getStreamThread, RK_NULL);

	/* VI unbind VENC and destroy venc*/
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = ctx->vi.s32DevId;
	stSrcChn.s32ChnId = ctx->vi.s32ChnId;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = ctx->venc.s32ChnId;
	s32Ret = SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("VI and VENC bind failure:%X", s32Ret);
		g_exit_result = RK_FAILURE;
	}
	SAMPLE_COMM_VENC_DestroyChn(&ctx->venc);
	s32Ret = SAMPLE_COMM_VI_DestroyChn(&ctx->vi);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VI_DisableChn failure:%X", s32Ret);
		g_exit_result = RK_FAILURE;
	}
	rtsp_deinit();

__FAILED:
	RK_MPI_SYS_Exit();
	if (iq_file_dir) {
#ifdef RKAIQ
		SAMPLE_COMM_ISP_Stop(s32CamId);
#endif
	}
__FAILED2:
	global_param_deinit();

	if (ctx) {
		free(ctx);
		ctx = RK_NULL;
	}
__PARAM_INIT_FAILED:
	return g_exit_result;
}

#ifdef __cplusplus
#if __cplusplus
#endif
#endif /* End of #ifdef __cplusplus */
