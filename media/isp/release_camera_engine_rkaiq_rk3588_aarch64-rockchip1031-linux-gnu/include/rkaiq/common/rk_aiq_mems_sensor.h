/*
 *  Copyright (c) 2021 Rockchip Corporation
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

#ifndef COMMON_RK_AIQ_SENSOR_H
#define COMMON_RK_AIQ_SENSOR_H

#include <stdint.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum mems_sensor_type_e {
    SENSOR_INVALID_TYPE = 0,
    SENSOR_GRYO_TYPE,
    SENSOR_ACCEL_TYPE,
    SENSOR_TEMP_TYPE,
    SENSOR_ALL_TYPE,
    SENSOR_MAX_TYPE,
} mems_sensor_type_t;

typedef enum mems_sensor_return_e {
    SENSOR_NO_ERR = 0,
    SENSOR_ERR_DEV_BUSY,
    SENSOR_ERR_INVALID_PARAM,
    SENSOR_ERR_MISMATCH_SENSOR,
    SENSOR_ERR_REACH_MAX,
    SENSOR_ERR_FAIL,
    SENSOR_ERR_TIMEOUT,
    SENSOR_ERR_NULL_PTR,
} mems_sensor_return_t;

typedef struct mems_sensor_capabilities_s {
    mems_sensor_type_t type;
    double* resolutions;
    uint8_t num_resolutions;
    double* scale;
    uint8_t num_scale;
    double* sample_rates;
    uint8_t num_sample_rates;
    bool is_data_valid;
} mems_sensor_capabilities_t;

typedef struct mems_sensor_config_s {
    double resolution;
    double scale;
    double sample_rate;
    uint32_t max_data_count;
    uint32_t buf_count;
} mems_sensor_config_t;

typedef struct sensor_vec_s {
    union {
        double v[3];
        struct {
            double x;
            double y;
            double z;
        };
    };
}sensor_vec_t;

typedef struct uncalib_event_s {
    union {
        double uncalib[3];
        struct {
            double x_uncalib;
            double y_uncalib;
            double z_uncalib;
        };
    };
    union {
        double bias[3];
        struct {
            double x_bias;
            double y_bias;
            double z_bias;
        };
    };
}uncalib_event_t;

typedef sensor_vec_t gyro_data_t;
typedef sensor_vec_t accel_data_t;
typedef int temperature_data_t;

typedef struct sensor_vec_all_s {
    gyro_data_t gyro;
    accel_data_t accel;
    temperature_data_t temperature;
}sensor_vec_all_t;


typedef struct mems_sensor_event_s {
    uint64_t id;
    uint64_t timestamp_us;
    union {
        double data[16];
        accel_data_t accel;
        gyro_data_t gyro;
        temperature_data_t temperature;
        sensor_vec_all_t all;

        uncalib_event_t uncalib_gyro;
        uncalib_event_t uncalib_accel;
    };
} mems_sensor_event_t;

typedef struct mems_sensor_list_s {
    int count;
    char** key_list;
} mems_sensor_list_t;

typedef void* mems_sensor_ctx_t;
typedef void* mems_sensor_handle_t;
typedef void* mems_sensor_data_t;

typedef mems_sensor_ctx_t (*rk_aiq_mems_sensor_createContext)();
typedef mems_sensor_return_t (*rk_aiq_mems_sensor_destroyContext)(mems_sensor_ctx_t ctx);
typedef mems_sensor_return_t (*rk_aiq_mems_sensor_getSensorList)(mems_sensor_ctx_t ctx,
                                                                 mems_sensor_type_t type,
                                                                 mems_sensor_list_t* list);
typedef mems_sensor_return_t (*rk_aiq_mems_sensor_releaseSensorList)(
    mems_sensor_list_t* sensors_list);
typedef mems_sensor_return_t (*rk_aiq_mems_sensor_getSensorCapabilities)(
    const mems_sensor_ctx_t ctx,const mems_sensor_type_t type, const char* sensor_key,
    mems_sensor_capabilities_t* caps);
typedef mems_sensor_return_t (*rk_aiq_mems_sensor_releaseSensorCapabilities)(
    mems_sensor_capabilities_t* caps);
typedef mems_sensor_return_t (*rk_aiq_mems_sensor_getConfig)(const mems_sensor_ctx_t ctx,
                                                             const mems_sensor_type_t type,
                                                             const char* sensor_key,
                                                             mems_sensor_config_t* cfg);
typedef mems_sensor_return_t (*rk_aiq_mems_sensor_setConfig)(const mems_sensor_ctx_t ctx,
                                                             const mems_sensor_type_t type,
                                                             const char* sensor_key,
                                                             mems_sensor_config_t cfg);
typedef mems_sensor_handle_t (*rk_aiq_mems_sensor_createHandle)(const mems_sensor_ctx_t ctx,
                                                                const mems_sensor_type_t type,
                                                                const char* sensor_key,
                                                                const uint32_t max_data_num,
                                                                const uint32_t buf_cnt);
typedef mems_sensor_return_t (*rk_aiq_mems_sensor_destroyHandle)(mems_sensor_handle_t handle);
typedef mems_sensor_data_t (*rk_aiq_mems_sensor_getData)(mems_sensor_handle_t handle,
                                                         size_t* num_samples);
typedef mems_sensor_data_t (*rk_aiq_mems_sensor_getLastNSamples)(mems_sensor_handle_t handle,
                                                                 size_t num_samples);
typedef mems_sensor_return_t (*rk_aiq_mems_sensor_releaseSamplesData)(mems_sensor_handle_t handle,
                                                                      mems_sensor_data_t data);

typedef struct rk_aiq_mems_sensor_intf_s {
    rk_aiq_mems_sensor_createContext createContext;
    rk_aiq_mems_sensor_destroyContext destroyContext;
    rk_aiq_mems_sensor_getSensorList getSensorList;
    rk_aiq_mems_sensor_releaseSensorList releaseSensorList;
    rk_aiq_mems_sensor_getSensorCapabilities getSensorCapabilities;
    rk_aiq_mems_sensor_releaseSensorCapabilities releaseSensorCapabilities;
    rk_aiq_mems_sensor_getConfig getConfig;
    rk_aiq_mems_sensor_setConfig setConfig;
    rk_aiq_mems_sensor_createHandle createHandle;
    rk_aiq_mems_sensor_destroyHandle destroyHandle;
    rk_aiq_mems_sensor_getData getData;
    rk_aiq_mems_sensor_getLastNSamples getLastNSamples;
    rk_aiq_mems_sensor_releaseSamplesData releaseSamplesData;
} rk_aiq_mems_sensor_intf_t;

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // COMMON_RK_AIQ_SENSOR_H
