#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>

#include "rk_aiq_mems_sensor.h"
#include "rkiio.h"

//#define IIO_TEST

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


static mems_sensor_ctx_t rkiio_aiq_sensor_ctx_create() {
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
    return (mems_sensor_ctx_t)ctx;
}

static mems_sensor_return_t rkiio_aiq_sensor_ctx_destroy(mems_sensor_ctx_t ctx) {
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
    return SENSOR_NO_ERR;
}

static mems_sensor_return_t rkiio_aiq_sensors_list_get(mems_sensor_ctx_t ctx,
                                                       mems_sensor_type_t sensor_type,
                                                       mems_sensor_list_t* sensors_list) {
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

static mems_sensor_return_t rkiio_aiq_sensors_list_release(mems_sensor_list_t* sensors_list) {
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
                                                     char* sensor_key,
                                                     mems_sensor_capabilities_t* caps) {
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
            return SENSOR_ERR_MISMATCH_SENSOR;
    }
    caps->is_data_valid = true;
    return SENSOR_NO_ERR;
}

static mems_sensor_return_t rkiio_aiq_sensor_cap_release(mems_sensor_capabilities_t* caps) {
    if (caps->sample_rates) rkiio_sampling_rate_cap_release(caps->sample_rates);
    return SENSOR_NO_ERR;
}

// only support sampling rate
static mems_sensor_return_t rkiio_aiq_sensor_config_get(mems_sensor_ctx_t ctx,
                                                        mems_sensor_type_t sensor_type,
                                                        char* sensor_key,
                                                        mems_sensor_config_t* cfg) {
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
            ret =
                rkiio_sampling_rate_get(sensor_key, RKIIO_SENSOR_ANGLVEL_TYPE, &(cfg->sample_rate));
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
                                                        char* sensor_key,
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
            return SENSOR_ERR_MISMATCH_SENSOR;
    }
    if (ret) {
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

static mems_sensor_handle_t rkiio_aiq_sensor_handle_create(mems_sensor_ctx_t ctx,
                                                           mems_sensor_type_t sensor_type,
                                                           char* sensor_key, uint32_t max_data_num,
                                                           uint32_t buf_cnt) {
    if (!g_init_flag || !ctx || !max_data_num || !buf_cnt) {
        return NULL;
    }
    rkiio_sensor_handle_t* sensor_handle =
        (rkiio_sensor_handle_t*)malloc(sizeof(rkiio_sensor_handle_t));
    memset(sensor_handle, 0, sizeof(rkiio_sensor_handle_t));
    sensor_handle->sensor_type = sensor_type;
    sensor_handle->buf_cnt     = buf_cnt;

    sensor_handle->handle    = rkiio_data_handle_create(sensor_key, 0, max_data_num);
    uint32_t real_max_data_num = 0;
    rkiio_data_handle_max_fifo_num_get(sensor_handle->handle, &real_max_data_num);
    sensor_handle->max_data_num =
        (max_data_num > real_max_data_num) ? max_data_num : real_max_data_num;

    pthread_mutex_init(&(sensor_handle->fifo_buf_mutex), NULL);
    sensor_handle->fifo_data_buf =
        (RKIIO_FIFO_DATA)malloc(sizeof(rkiio_data_0_t) * sensor_handle->max_data_num);

    sensor_handle->buf_pool =
        (rkiio_sensor_handle_buf_t*)malloc(sizeof(rkiio_sensor_handle_buf_t) * buf_cnt);
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
    return (mems_sensor_handle_t)sensor_handle;
}

static mems_sensor_return_t rkiio_aiq_sensor_handle_destroy(mems_sensor_handle_t handle) {
    if (!handle) {
        return SENSOR_ERR_NULL_PTR;
    }
    rkiio_sensor_handle_t* sensor_handle = (rkiio_sensor_handle_t*)handle;
    pthread_mutex_lock(&(sensor_handle->fifo_buf_mutex));
    rkiio_data_handle_destroy(sensor_handle->handle);
    if (sensor_handle->fifo_data_buf) free(sensor_handle->fifo_data_buf);
    if (sensor_handle->buf_pool) {
        for (uint32_t i = 0; i < sensor_handle->buf_cnt; i++) {
            if (sensor_handle->buf_pool[i].buf) free(sensor_handle->buf_pool[i].buf);
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

static uint32_t _rkiio_aiq_get_vaild_buf(rkiio_sensor_handle_t* handle) {
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

static mems_sensor_data_t _rkiio_aiq_getData_gyro(rkiio_sensor_handle_t* handle, uint64_t data_num) {
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

static mems_sensor_data_t _rkiio_aiq_getData_accel(rkiio_sensor_handle_t* handle, uint64_t data_num) {
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

static mems_sensor_data_t _rkiio_aiq_getData_temp(rkiio_sensor_handle_t* handle, uint64_t data_num) {
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

static mems_sensor_data_t _rkiio_aiq_getData_all(rkiio_sensor_handle_t* handle, uint64_t data_num) {
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

static mems_sensor_data_t rkiio_aiq_getData(mems_sensor_handle_t handle, size_t* num_samples) {
    if (!handle) {
        return NULL;
    }
    int32_t ret                           = 0;
    rkiio_sensor_handle_t* sensor_handle = (rkiio_sensor_handle_t*)handle;
    pthread_mutex_lock(&(sensor_handle->fifo_buf_mutex));
    uint64_t data_num = 0;
    ret = rkiio_data_get_all(sensor_handle->handle, sensor_handle->fifo_data_buf, &data_num);
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
    rkiio_sensor_handle_t* sensor_handle = (rkiio_sensor_handle_t*)handle;
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

static mems_sensor_return_t rkiio_aiq_sensor_data_release(mems_sensor_handle_t sensor_handle,
                                                          mems_sensor_data_t data) {
    if (!data || !sensor_handle) {
        return SENSOR_ERR_NULL_PTR;
    }
    rkiio_sensor_handle_t* handle = (rkiio_sensor_handle_t*)sensor_handle;
    uint32_t idx                    = _rkiio_aiq_buf_id_get(data, handle->max_data_num);
    pthread_mutex_lock(&(handle->buf_pool[idx].buf_mutex));
    handle->buf_pool[idx].is_valid = true;
    pthread_mutex_unlock(&(handle->buf_pool[idx].buf_mutex));
    return SENSOR_NO_ERR;
}

#ifndef IIO_TEST
rk_aiq_mems_sensor_intf_t g_rkiio_aiq_api = {
    .createContext             = rkiio_aiq_sensor_ctx_create,
    .destroyContext            = rkiio_aiq_sensor_ctx_destroy,
    .getSensorList             = rkiio_aiq_sensors_list_get,
    .releaseSensorList         = rkiio_aiq_sensors_list_release,
    .getSensorCapabilities     = rkiio_aiq_sensor_cap_get,
    .releaseSensorCapabilities = rkiio_aiq_sensor_cap_release,
    .getConfig                 = rkiio_aiq_sensor_config_get,
    .setConfig                 = rkiio_aiq_sensor_config_set,
    .createHandle              = rkiio_aiq_sensor_handle_create,
    .destroyHandle             = rkiio_aiq_sensor_handle_destroy,
    .getData                   = rkiio_aiq_getData,
    .getLastNSamples           = rkiio_aiq_getLastNSamples,
    .releaseSamplesData        = rkiio_aiq_sensor_data_release,
};
#endif

#ifdef IIO_TEST

static int g_quit              = 0;  // only for test
static FILE* save_file;

static void sigterm_handler(int sig) {
    fprintf(stderr, "signal %d\n", sig);
    g_quit = 1;
}

static void virtual_aiq_register(rk_aiq_mems_sensor_intf_t api) {
    int32_t ret;
    mems_sensor_ctx_t ctx = api.createContext();
    mems_sensor_list_t sensors_list;
    mems_sensor_type_t type = SENSOR_ALL_TYPE;
    memset(&sensors_list, 0, sizeof(mems_sensor_list_t));
    api.getSensorList(ctx, type, &sensors_list);
    if (sensors_list.count) {
        for (int32_t i = 0; i < sensors_list.count; i++) {
            printf("%d: %s\n", i, sensors_list.key_list[i]);
        }
    } else {
        printf("get no dev\n");
    }
    if (sensors_list.count) {
        mems_sensor_capabilities_t caps;
        memset(&caps, 0, sizeof(mems_sensor_capabilities_t));
        ret = api.getSensorCapabilities(ctx, type, sensors_list.key_list[0], &caps);
        if (ret) {
            printf("rkiio_aiq_sensor_cap_get fail\n");
            goto api_exit;
        } else {
            for (int32_t i = 0; i < caps.num_sample_rates; i++) {
                printf("rkaiq_sensor_capabilities: %d:sample_cap: %f\n", i, caps.sample_rates[i]);
            }
        }
        // cap set demo
        if (caps.num_sample_rates) {
            mems_sensor_config_t sensor_cfg;
            memset(&sensor_cfg, 0, sizeof(mems_sensor_config_t));
            ret = api.getConfig(ctx, type, sensors_list.key_list[0], &sensor_cfg);
            if (ret) {
                printf("rkiio_aiq_sensor_config_get fail\n");
            } else {
                printf("rkiio_aiq_sensor_config_get: %f\n", sensor_cfg.sample_rate);
            }
            sensor_cfg.sample_rate = caps.sample_rates[0];
            ret = api.setConfig(ctx, SENSOR_GRYO_TYPE, sensors_list.key_list[0], sensor_cfg);
            if (ret) {
                printf("rkiio_aiq_sensor_config_set fail\n");
            } else {
                printf("rkiio_aiq_sensor_config_set success\n");
            }
        }
        // private method set all sampling
        rkiio_sampling_rate_set_all(sensors_list.key_list[0], "1000");
        api.releaseSensorCapabilities(&caps);

        // start sampling
        uint64_t sampling_cnt         = 0;
        mems_sensor_handle_t handle = api.createHandle(ctx, type, sensors_list.key_list[0], 100, 3);
        if (!handle) {
            printf("rkiio_aiq_sensor_handle_create fail\n");
            goto api_exit;
        }
        while (!g_quit) {
            mems_sensor_data_t data;
            size_t num_samples = 0;
            if (1) {
                data = api.getData(handle, &num_samples);
                printf("getdata: %zu\n", num_samples);
            } else {
                num_samples = 50;
                data        = api.getLastNSamples(handle, num_samples);
                printf("getLastNSamples: %zu\n", num_samples);
            }
            // only for test
            if (type == SENSOR_GRYO_TYPE) {
                mems_sensor_event_t* real_data = (mems_sensor_event_t*)data;
                for (uint32_t j = 0; j < num_samples; j++) {
                    printf("gyro data(%" PRIu64 "): x: %f, y: %f, z: %f: t: %" PRIu64 "\n", real_data[j].id,
                           real_data[j].gyro.x, real_data[j].gyro.y, real_data[j].gyro.z,
                           real_data[j].timestamp_us);
                }
            } else if (type == SENSOR_ALL_TYPE) {
                mems_sensor_event_t* real_data = (mems_sensor_event_t*)data;
                for (uint32_t j = 0; j < num_samples; j++) {
                    char w_data[1024] = {0};
                    sprintf(w_data,
                            "gyro data(%" PRIu64 "): gx: %f, gy: %f, gz: %f: , ax: %f, ay: %f, az: %f, "
                            "temp: %d, time: %" PRIu64 "\n",
                            real_data[j].id, real_data[j].all.gyro.x, real_data[j].all.gyro.y,
                            real_data[j].all.gyro.z, real_data[j].all.accel.x,
                            real_data[j].all.accel.y, real_data[j].all.accel.z,
                            real_data[j].all.temperature, real_data[j].timestamp_us);
                    printf("%s\n", w_data);
                    /*                   if (save_file) {*/
                    /*fwrite(w_data, sizeof(char), strlen(w_data), save_file);*/
                    /*                   }*/
                }
            } else {
                printf("only support printf gyro\n");
            }
            api.releaseSamplesData(handle, data);
            sampling_cnt++;
            data = NULL;
            // g_quit = 1;
            printf("===============================split=======================================");
            usleep(50000);
        }
        api.destroyHandle(handle);
    }
api_exit:
    api.releaseSensorList(&sensors_list);
    api.destroyContext(ctx);
}

int main() {
    save_file = fopen("/data/test.txt", "w");
    rk_aiq_mems_sensor_intf_t api;
    api.createContext             = rkiio_aiq_sensor_ctx_create;
    api.destroyContext            = rkiio_aiq_sensor_ctx_destroy;
    api.getSensorList             = rkiio_aiq_sensors_list_get;
    api.releaseSensorList         = rkiio_aiq_sensors_list_release;
    api.getSensorCapabilities     = rkiio_aiq_sensor_cap_get;
    api.releaseSensorCapabilities = rkiio_aiq_sensor_cap_release;
    api.getConfig                 = rkiio_aiq_sensor_config_get;
    api.setConfig                 = rkiio_aiq_sensor_config_set;
    api.createHandle              = rkiio_aiq_sensor_handle_create;
    api.destroyHandle             = rkiio_aiq_sensor_handle_destroy;
    api.getData                   = rkiio_aiq_getData;
    api.getLastNSamples           = rkiio_aiq_getLastNSamples;
    api.releaseSamplesData        = rkiio_aiq_sensor_data_release;
    signal(SIGINT, sigterm_handler);
    signal(SIGTERM, sigterm_handler);
    virtual_aiq_register(api);
    if (save_file) fclose(save_file);
    printf(">>>>quit\n");
    //     mems_sensor_ctx_t ctx = rkiio_aiq_sensor_ctx_create();
    //     mems_sensor_list_t sensors_list;
    //     int32_t ret = 0;
    //     mems_sensor_type_t type = SENSOR_GRYO_TYPE;
    //     signal(SIGINT, sigterm_handler);
    //     memset(&sensors_list, 0, sizeof(mems_sensor_list_t));
    //     rkiio_aiq_sensors_list_get(ctx, type, &sensors_list);
    //     if (sensors_list.count) {
    //         for (int32_t i = 0; i < sensors_list.count; i++) {
    //             printf("%d: %s\n", i, sensors_list.key_list[i]);
    //         }
    //     } else {
    //         printf("get no dev\n");
    //     }
    //     if (sensors_list.count) {
    //         mems_sensor_capabilities_t caps;
    //         memset(&caps, 0, sizeof(mems_sensor_capabilities_t));
    //         ret = rkiio_aiq_sensor_cap_get(ctx, type, sensors_list.key_list[0], &caps);
    //         if (ret) {
    //             printf("rkiio_aiq_sensor_cap_get fail\n");
    //             goto main_exit;
    //         } else {
    //             for (int32_t i = 0; i < caps.num_sample_rates; i++) {
    //                 printf("rkaiq_sensor_capabilities: %d:sample_cap: %f\n", i,
    //                 caps.sample_rates[i]);
    //             }
    //         }
    //         // cap set demo
    //         if (caps.num_sample_rates) {
    //             mems_sensor_config_t sensor_cfg;
    //             memset(&sensor_cfg, 0, sizeof(mems_sensor_config_t));
    //             rkiio_aiq_sensor_config_get(ctx, type, sensors_list.key_list[0], &sensor_cfg);
    //             if (ret) {
    //                 printf("rkiio_aiq_sensor_config_get fail\n");
    //             } else {
    //                 printf("rkiio_aiq_sensor_config_get: %f\n", sensor_cfg.sample_rate);
    //             }
    //             sensor_cfg.sample_rate = caps.sample_rates[0];
    //             rkiio_aiq_sensor_config_set(ctx, SENSOR_GRYO_TYPE, sensors_list.key_list[0],
    //             sensor_cfg); if (ret) {
    //                 printf("rkiio_aiq_sensor_config_set fail\n");
    //             } else {
    //                 printf("rkiio_aiq_sensor_config_set success\n");
    //             }
    //         }
    //         // private method set all sampling
    //         rkiio_sampling_rate_set_all(sensors_list.key_list[0], "1000");
    //         rkiio_aiq_sensor_cap_release(&caps);

    //         // start sampling
    //         uint64_t sampling_cnt = 0;
    //         mems_sensor_handle_t handle = rkiio_aiq_sensor_handle_create(ctx, type,
    //         sensors_list.key_list[0], 100, 3); if (!handle) {
    //             printf("rkiio_aiq_sensor_handle_create fail\n");
    //             goto main_exit;
    //         }
    //         while (!g_quit) {
    //             mems_sensor_data_t data;
    //             size_t num_samples = 0;
    //             if (0) {
    //                 data = rkiio_aiq_getData(handle, &num_samples);
    //                 printf("getdata: %d\n", num_samples);
    //             } else {
    //                 num_samples = 50;
    //                 data = rkiio_aiq_getLastNSamples(handle, num_samples);
    //                 printf("getLastNSamples: %d\n", num_samples);
    //             }
    //             // only for test
    //             if (type == SENSOR_GRYO_TYPE) {
    //                 mems_sensor_event_t *real_data = (mems_sensor_event_t *)data;
    //                 for (uint32_t j = 0; j < num_samples; j++) {
    //                     printf("gyro data(%llu): x: %f, y: %f, z: %f: t: %llu\n",
    //                     real_data[j].id, real_data[j].gyro.x, real_data[j].gyro.y,
    //                     real_data[j].gyro.z, real_data[j].timestamp_us);
    //                 }
    //             } else {
    //                 printf("only support printf gyro\n");
    //             }
    //             rkiio_aiq_sensor_data_release(handle, data);
    //             sampling_cnt++;
    //             data = NULL;
    //             // g_quit = 1;
    //         }
    //         rkiio_aiq_sensor_handle_destroy(handle);
    //     }
    // main_exit:
    //     rkiio_aiq_sensors_list_release(&sensors_list);
    //     rkiio_aiq_sensor_ctx_destroy(ctx);
    return 0;
}
#endif
