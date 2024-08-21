/*
 * Copyright (c) 2024 Rockchip, Inc. All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
#ifdef ENABLE_EIS

#include "sample_iio_aiq.h"

typedef struct rkiio_sensor_ctx_s {
  uint64_t ctx_id;
} rkiio_sensor_ctx_t;

typedef struct rkiio_sensor_handle_buf_s {
  pthread_mutex_t buf_mutex;
  bool is_valid;
  void* buf;
} rkiio_sensor_handle_buf_t;

typedef struct rkiio_sensor_handle_ctx_s {
  mems_sensor_type_t sensor_type;
  RKIIO_DATA_HANDLE handle;
  uint32_t max_data_num;
  uint32_t buf_cnt;
  rkiio_sensor_handle_buf_t* buf_pool;
  pthread_mutex_t fifo_buf_mutex;
  RKIIO_FIFO_DATA fifo_data_buf;
} rkiio_sensor_handle_t;

static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
static bool g_init_flag      = false;
static uint32_t g_sensor_cnt     = 0;
static uint32_t g_handle_cnt     = 0;

mems_sensor_ctx_t rkiio_aiq_sensor_ctx_create() {
  pthread_mutex_lock(&g_mutex);
  g_sensor_cnt++;
  if (!g_init_flag) {
    rkiio_sys_init();
    rkiio_timestamp_type_set_all(RKIIO_TIMESTAMP_MONOTONIC);
  }
  g_init_flag = true;
  pthread_mutex_unlock(&g_mutex);
  rkiio_sensor_ctx_t* ctx = (rkiio_sensor_ctx_t*)malloc(sizeof(rkiio_sensor_ctx_t));
  memset(ctx, 0, sizeof(rkiio_sensor_ctx_t));

  RKADK_LOGD("create ctx done: %p", ctx);
  return (mems_sensor_ctx_t)ctx;
}

mems_sensor_return_t rkiio_aiq_sensor_ctx_destroy(mems_sensor_ctx_t ctx) {
  if (!ctx) {
    return SENSOR_ERR_NULL_PTR;
  }
  rkiio_sensor_ctx_t* rkiio_ctx = (rkiio_sensor_ctx_t*)ctx;
  free(rkiio_ctx);
  pthread_mutex_lock(&g_mutex);
  g_sensor_cnt--;
  if (!g_sensor_cnt) {
    rkiio_sys_uninit();
    g_init_flag = false;
  }
  pthread_mutex_unlock(&g_mutex);
  RKADK_LOGD("destroy ctx done");
  return SENSOR_NO_ERR;
}

mems_sensor_return_t rkiio_aiq_sensors_list_get(mems_sensor_ctx_t ctx,
                                                mems_sensor_type_t sensor_type,
                                                mems_sensor_list_t *sensors_list) {
  if (!g_init_flag) {
    RKADK_LOGE("No init");
    return SENSOR_ERR_FAIL;
  }

  if (!sensors_list || !ctx) {
    RKADK_LOGE("sensors_list or ctx is null");
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
      RKADK_LOGE("unknow sensor_type: %d", sensor_type);
      return SENSOR_ERR_MISMATCH_SENSOR;
  }

  if (sensors_list->count) {
    RKADK_LOGD("get sensor list success, count: %d", sensors_list->count);
    return SENSOR_NO_ERR;
  }

  RKADK_LOGE("mismatch sensor");
  return SENSOR_ERR_MISMATCH_SENSOR;
}

mems_sensor_return_t rkiio_aiq_sensors_list_release(mems_sensor_list_t *sensors_list) {
  if (!sensors_list) {
  RKADK_LOGE("sensors_list is null");
  return SENSOR_ERR_NULL_PTR;
  }

  int32_t ret = rkiio_dev_list_destroy(sensors_list->key_list, sensors_list->count);
  if (ret) {
    RKADK_LOGE("rkiio_dev_list_destroy failed[%d]", ret);
    return SENSOR_ERR_FAIL;
  }

  return SENSOR_NO_ERR;
}

mems_sensor_return_t rkiio_aiq_sensor_cap_get(mems_sensor_ctx_t ctx,
                                              mems_sensor_type_t sensor_type,
                                              char *sensor_key,
                                              mems_sensor_capabilities_t *caps) {
  if (!g_init_flag) {
    RKADK_LOGE("No init");
    return SENSOR_ERR_FAIL;
  }

  if (!ctx) {
    RKADK_LOGE("ctx is null");
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
      rkiio_sampling_rate_cap_get(sensor_key, RKIIO_SENSOR_ACCEL_TYPE, &(caps->sample_rates),
                                  &(caps->num_sample_rates));
      break;
    case SENSOR_TEMP_TYPE:
      rkiio_sampling_rate_cap_get(sensor_key, RKIIO_SENSOR_TEMP_TYPE, &(caps->sample_rates),
                                  &(caps->num_sample_rates));
      break;
    case SENSOR_ALL_TYPE:
      rkiio_sampling_rate_cap_get(sensor_key, RKIIO_SENSOR_ALL_TYPE, &(caps->sample_rates),
                                  &(caps->num_sample_rates));
      break;
    default:
      RKADK_LOGE("mismatch sensor");
      return SENSOR_ERR_MISMATCH_SENSOR;
  }

  caps->is_data_valid = true;
  return SENSOR_NO_ERR;
}

mems_sensor_return_t rkiio_aiq_sensor_cap_release(mems_sensor_capabilities_t *caps) {
  if (caps->sample_rates) rkiio_sampling_rate_cap_release(caps->sample_rates);
  return SENSOR_NO_ERR;
}

// only support sampling rate
mems_sensor_return_t rkiio_aiq_sensor_config_get(mems_sensor_ctx_t ctx,
                                                 mems_sensor_type_t sensor_type,
                                                 char *sensor_key,
                                                 mems_sensor_config_t *cfg) {
  int32_t ret = 0;

  if (!g_init_flag) {
    RKADK_LOGE("No init");
    return SENSOR_ERR_FAIL;
  }

  if (!cfg || !ctx) {
    RKADK_LOGE("cfg or ctx is null");
    return SENSOR_ERR_NULL_PTR;
  }

  if (g_handle_cnt) {
    RKADK_LOGE("dev busy, g_handle_cnt: %d", g_handle_cnt);
    return SENSOR_ERR_DEV_BUSY;
  }

  memset(cfg, 0, sizeof(mems_sensor_config_t));
  switch (sensor_type) {
    case SENSOR_GRYO_TYPE:
      ret = rkiio_sampling_rate_get(sensor_key, RKIIO_SENSOR_ANGLVEL_TYPE, &(cfg->sample_rate));
      break;
    case SENSOR_ACCEL_TYPE:
      ret = rkiio_sampling_rate_get(sensor_key, RKIIO_SENSOR_ACCEL_TYPE, &(cfg->sample_rate));
      break;
    case SENSOR_TEMP_TYPE:
      ret = rkiio_sampling_rate_get(sensor_key, RKIIO_SENSOR_TEMP_TYPE, &(cfg->sample_rate));
      break;
    case SENSOR_ALL_TYPE:
      ret = rkiio_sampling_rate_get(sensor_key, RKIIO_SENSOR_ALL_TYPE, &(cfg->sample_rate));
      break;
    default:
      RKADK_LOGE("mismatch sensor");
      return SENSOR_ERR_MISMATCH_SENSOR;
  }

  if (ret) {
    cfg->sample_rate = 0.0f;
    RKADK_LOGE("rkiio_sampling_rate_get failed[%d]", ret);
    return SENSOR_ERR_FAIL;
  }

  return SENSOR_NO_ERR;
}

mems_sensor_return_t rkiio_aiq_sensor_config_set(mems_sensor_ctx_t ctx,
                                                 mems_sensor_type_t sensor_type,
                                                 char *sensor_key,
                                                 mems_sensor_config_t cfg) {
  int32_t ret = 0;

  if (!g_init_flag) {
    RKADK_LOGE("No init");
    return SENSOR_ERR_FAIL;
  }

  if (!ctx) {
    RKADK_LOGE("cfg or ctx is null");
    return SENSOR_ERR_NULL_PTR;
  }

  if (g_handle_cnt) {
    RKADK_LOGE("dev busy, g_handle_cnt: %d", g_handle_cnt);
    return SENSOR_ERR_DEV_BUSY;
  }

  switch (sensor_type) {
    case SENSOR_GRYO_TYPE:
      ret = rkiio_sampling_rate_set(sensor_key, RKIIO_SENSOR_ANGLVEL_TYPE, cfg.sample_rate);
      break;
    case SENSOR_ACCEL_TYPE:
      ret = rkiio_sampling_rate_set(sensor_key, RKIIO_SENSOR_ACCEL_TYPE, cfg.sample_rate);
      break;
    case SENSOR_TEMP_TYPE:
      ret = rkiio_sampling_rate_set(sensor_key, RKIIO_SENSOR_TEMP_TYPE, cfg.sample_rate);
      break;
    case SENSOR_ALL_TYPE:
      ret = rkiio_sampling_rate_set(sensor_key, RKIIO_SENSOR_ALL_TYPE, cfg.sample_rate);
      break;
    default:
      RKADK_LOGE("mismatch sensor");
      return SENSOR_ERR_MISMATCH_SENSOR;
  }

  if (ret) {
    RKADK_LOGE("rkiio_sampling_rate_set failed[%d]", ret);
    return SENSOR_ERR_FAIL;
  }

  return SENSOR_NO_ERR;
}

static uint32_t _rkiio_aiq_buf_id_get(void* buf, uint32_t max_data_num) {
  uint8_t* data            = (uint8_t*)buf;
  uint64_t idx             = sizeof(mems_sensor_event_t) * max_data_num;
  int32_t* reserverd_data = (int32_t*)&(data[idx]);
  return *reserverd_data;
}

static void _rkiio_aiq_buf_id_set(void* buf, uint32_t max_data_num, uint32_t id) {
  uint8_t* data            = (uint8_t*)buf;
  uint64_t idx             = sizeof(mems_sensor_event_t) * max_data_num;
  int32_t* reserverd_data = (int32_t*)&(data[idx]);
  *reserverd_data        = id;
}

mems_sensor_handle_t rkiio_aiq_sensor_handle_create(mems_sensor_ctx_t ctx,
                                                    mems_sensor_type_t sensor_type,
                                                    char *sensor_key, uint32_t max_data_num,
                                                    uint32_t buf_cnt) {
  if (!g_init_flag) {
    RKADK_LOGE("No init");
    return NULL;
  }

  if (!ctx || !max_data_num || !buf_cnt) {
    RKADK_LOGE("Invalid parameter, max_data_num: %d, buf_cnt: %d", max_data_num, buf_cnt);
    return NULL;
  }

  rkiio_sensor_handle_t *sensor_handle = (rkiio_sensor_handle_t*)malloc(sizeof(rkiio_sensor_handle_t));
  memset(sensor_handle, 0, sizeof(rkiio_sensor_handle_t));
  sensor_handle->sensor_type = sensor_type;
  sensor_handle->buf_cnt = buf_cnt;

  sensor_handle->handle = rkiio_data_handle_create(sensor_key, 0, max_data_num);
  uint32_t real_max_data_num = 0;
  rkiio_data_handle_max_fifo_num_get(sensor_handle->handle, &real_max_data_num);
  sensor_handle->max_data_num = (max_data_num > real_max_data_num) ? max_data_num : real_max_data_num;

  pthread_mutex_init(&(sensor_handle->fifo_buf_mutex), NULL);
  sensor_handle->fifo_data_buf = (RKIIO_FIFO_DATA)malloc(sizeof(rkiio_data_0_t) * sensor_handle->max_data_num);

  sensor_handle->buf_pool = (rkiio_sensor_handle_buf_t*)malloc(sizeof(rkiio_sensor_handle_buf_t) * buf_cnt);
  memset(sensor_handle->buf_pool, 0, sizeof(rkiio_sensor_handle_buf_t) * buf_cnt);
  for (uint32_t i = 0; i < buf_cnt; i++) {
    pthread_mutex_init(&(sensor_handle->buf_pool[i].buf_mutex), NULL);
    sensor_handle->buf_pool[i].is_valid = true;
    sensor_handle->buf_pool[i].buf      = (void*)malloc(
        sizeof(mems_sensor_event_t) * sensor_handle->max_data_num + sizeof(uint32_t));
    memset(sensor_handle->buf_pool[i].buf, 0,
           sizeof(mems_sensor_event_t) * sensor_handle->max_data_num + sizeof(uint32_t));
    _rkiio_aiq_buf_id_set(sensor_handle->buf_pool[i].buf, sensor_handle->max_data_num, i);
  }
  pthread_mutex_lock(&g_mutex);
  g_handle_cnt++;
  pthread_mutex_unlock(&g_mutex);

  RKADK_LOGD("create handle done, g_handle_cnt: %d", g_handle_cnt);
  return (mems_sensor_handle_t)sensor_handle;
}

mems_sensor_return_t rkiio_aiq_sensor_handle_destroy(mems_sensor_handle_t handle) {
  if (!handle) {
    return SENSOR_ERR_NULL_PTR;
  }

  rkiio_sensor_handle_t* sensor_handle = (rkiio_sensor_handle_t*)handle;
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
  RKADK_LOGD("destroy handle done, g_handle_cnt: %d", g_handle_cnt);
  return SENSOR_NO_ERR;
}

static uint32_t _rkiio_aiq_get_vaild_buf(rkiio_sensor_handle_t *handle) {
  while (1) {
    for (uint32_t i = 0; i < handle->buf_cnt; i++) {
      pthread_mutex_lock(&(handle->buf_pool[i].buf_mutex));
      // printf("%s: id: %u, valid: %d\n", __FUNCTION__, i, handle->buf_pool[i].is_valid);
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

static mems_sensor_data_t _rkiio_aiq_getData_gyro(rkiio_sensor_handle_t *handle, uint64_t data_num) {
  rkiio_data_0_t* fifo       = (rkiio_data_0_t*)handle->fifo_data_buf;
  uint32_t valid_id            = _rkiio_aiq_get_vaild_buf(handle);
  mems_sensor_event_t* event = (mems_sensor_event_t*)(handle->buf_pool[valid_id].buf);

  memset(event, 0, sizeof(mems_sensor_event_t) * data_num);
  for (uint64_t i = 0; i < data_num; i++) {
    memcpy(&(event[i].gyro), &(fifo[i].anglvel), sizeof(xyz_data_t));
    event[i].timestamp_us = fifo[i].timestamp;
    event[i].id           = fifo[i].id;
    // printf("%llu:, x: %f, y: %f, z: %f, t: %llu, %llu:, x: %f, y: %f, z: %f, t: %llu\n",
    //     fifo[i].id, fifo[i].anglvel.x, fifo[i].anglvel.y, fifo[i].anglvel.z,
    //     fifo[i].timestamp, event[i].id, event[i].gyro.x, event[i].gyro.y, event[i].gyro.z,
    //     event[i].timestamp_us);
  }
  return (mems_sensor_data_t)event;
}

static mems_sensor_data_t _rkiio_aiq_getData_accel(rkiio_sensor_handle_t *handle, uint64_t data_num) {
  rkiio_data_0_t* fifo       = (rkiio_data_0_t*)handle->fifo_data_buf;
  uint32_t valid_id            = _rkiio_aiq_get_vaild_buf(handle);
  mems_sensor_event_t* event = (mems_sensor_event_t*)(handle->buf_pool[valid_id].buf);

  memset(event, 0, sizeof(mems_sensor_event_t) * data_num);
  for (uint64_t i = 0; i < data_num; i++) {
    memcpy(&(event[i].accel), &(fifo[i].accel), sizeof(xyz_data_t));
    event[i].timestamp_us = fifo[i].timestamp;
    event[i].id           = fifo[i].id;
  }
  return (mems_sensor_data_t)event;
}

static mems_sensor_data_t _rkiio_aiq_getData_temp(rkiio_sensor_handle_t *handle, uint64_t data_num) {
  rkiio_data_0_t* fifo       = (rkiio_data_0_t*)handle->fifo_data_buf;
  uint32_t valid_id            = _rkiio_aiq_get_vaild_buf(handle);
  mems_sensor_event_t* event = (mems_sensor_event_t*)(handle->buf_pool[valid_id].buf);

  memset(event, 0, sizeof(mems_sensor_event_t) * data_num);
  for (uint64_t i = 0; i < data_num; i++) {
    // temp alter to s32?
    event[i].temperature  = fifo[i].temp;
    event[i].timestamp_us = fifo[i].timestamp;
    event[i].id           = fifo[i].id;
  }
  return (mems_sensor_data_t)event;
}

static mems_sensor_data_t _rkiio_aiq_getData_all(rkiio_sensor_handle_t *handle, uint64_t data_num) {
  rkiio_data_0_t* fifo       = (rkiio_data_0_t*)handle->fifo_data_buf;
  uint32_t valid_id            = _rkiio_aiq_get_vaild_buf(handle);
  mems_sensor_event_t* event = (mems_sensor_event_t*)(handle->buf_pool[valid_id].buf);

  memset(event, 0, sizeof(mems_sensor_event_t) * data_num);
  for (uint64_t i = 0; i < data_num; i++) {
    memcpy(&(event[i].all.gyro), &(fifo[i].anglvel), sizeof(xyz_data_t));
    memcpy(&(event[i].all.accel), &(fifo[i].accel), sizeof(xyz_data_t));
    // temp alter to s32?
    event[i].all.temperature = fifo[i].temp;
    event[i].timestamp_us    = fifo[i].timestamp;
    event[i].id              = fifo[i].id;
  }
  return (mems_sensor_data_t)event;
}

mems_sensor_data_t rkiio_aiq_getData(mems_sensor_handle_t handle, size_t *num_samples) {
  int32_t ret  = 0;
  uint64_t data_num = 0;
  mems_sensor_data_t data = NULL;
  rkiio_sensor_handle_t *sensor_handle;

  if (!handle) {
    RKADK_LOGE("handle is null");
    return NULL;
  }

  sensor_handle = (rkiio_sensor_handle_t*)handle;
  pthread_mutex_lock(&(sensor_handle->fifo_buf_mutex));

  ret = rkiio_data_get_all(sensor_handle->handle, sensor_handle->fifo_data_buf, &data_num);
  if (ret || !data_num) {
    *num_samples = 0;
    pthread_mutex_unlock(&(sensor_handle->fifo_buf_mutex));
    RKADK_LOGE("rkiio_data_get_all failed[%d], data_num[%lld]", ret, data_num);
    return NULL;
  }

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
      RKADK_LOGE("unknown sensor_type[%d]", sensor_handle->sensor_type);
      data = NULL;
  }

  *num_samples = data ? (size_t)data_num : 0;
  pthread_mutex_unlock(&(sensor_handle->fifo_buf_mutex));
  return data;
}

mems_sensor_data_t rkiio_aiq_getLastNSamples(mems_sensor_handle_t handle,
                                             size_t num_samples) {
  int32_t ret = 0;
  mems_sensor_data_t data = NULL;
  rkiio_sensor_handle_t *sensor_handle;
  uint64_t data_num = (uint64_t)num_samples;

  if (!handle || !num_samples) {
    RKADK_LOGE("Invalid parameter, num_samples: %d", num_samples);
    return NULL;
  }

  sensor_handle = (rkiio_sensor_handle_t*)handle;
  if ((uint32_t)num_samples > sensor_handle->max_data_num) {
    RKADK_LOGE("num_samples[%d] > max_data_num[%d]", num_samples, sensor_handle->max_data_num);
    return NULL;
  }

  pthread_mutex_lock(&(sensor_handle->fifo_buf_mutex));
  ret = rkiio_data_read(sensor_handle->handle, sensor_handle->fifo_data_buf, data_num);
  if (ret) {
    pthread_mutex_unlock(&(sensor_handle->fifo_buf_mutex));
    RKADK_LOGE("rkiio_data_read failed[%d]", ret);
    return NULL;
  }

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
      RKADK_LOGE("unknown sensor_type[%d]", sensor_handle->sensor_type);
      data = NULL;
  }

  pthread_mutex_unlock(&(sensor_handle->fifo_buf_mutex));
  return data;
}

mems_sensor_return_t rkiio_aiq_sensor_data_release(mems_sensor_handle_t sensor_handle,
                                                   mems_sensor_data_t data) {
  if (!data || !sensor_handle) {
    RKADK_LOGE("Invalid parameter");
    return SENSOR_ERR_NULL_PTR;
  }

  rkiio_sensor_handle_t* handle = (rkiio_sensor_handle_t*)sensor_handle;
  uint32_t idx                    = _rkiio_aiq_buf_id_get(data, handle->max_data_num);
  pthread_mutex_lock(&(handle->buf_pool[idx].buf_mutex));
  handle->buf_pool[idx].is_valid = true;
  pthread_mutex_unlock(&(handle->buf_pool[idx].buf_mutex));
  return SENSOR_NO_ERR;
}

#endif
