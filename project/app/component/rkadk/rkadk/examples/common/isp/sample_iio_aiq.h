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

#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <rk_aiq_mems_sensor.h>
#include "rkiio.h"
#include "rkadk_log.h"

mems_sensor_ctx_t rkiio_aiq_sensor_ctx_create();
mems_sensor_return_t rkiio_aiq_sensor_ctx_destroy(mems_sensor_ctx_t ctx);

mems_sensor_return_t rkiio_aiq_sensors_list_get(mems_sensor_ctx_t ctx,
                                                mems_sensor_type_t sensor_type,
                                                mems_sensor_list_t* sensors_list);

mems_sensor_return_t rkiio_aiq_sensors_list_release(mems_sensor_list_t* sensors_list);

mems_sensor_return_t rkiio_aiq_sensor_cap_get(mems_sensor_ctx_t ctx,
                                              mems_sensor_type_t sensor_type,
                                              char* sensor_key,
                                              mems_sensor_capabilities_t* caps);

mems_sensor_return_t rkiio_aiq_sensor_cap_release(mems_sensor_capabilities_t* caps);

mems_sensor_return_t rkiio_aiq_sensor_config_get(mems_sensor_ctx_t ctx,
                                                 mems_sensor_type_t sensor_type,
                                                 char* sensor_key,
                                                 mems_sensor_config_t* cfg);

mems_sensor_return_t rkiio_aiq_sensor_config_set(mems_sensor_ctx_t ctx,
                                                 mems_sensor_type_t sensor_type,
                                                 char* sensor_key,
                                                 mems_sensor_config_t cfg);

mems_sensor_handle_t rkiio_aiq_sensor_handle_create(mems_sensor_ctx_t ctx,
                                                    mems_sensor_type_t sensor_type,
                                                    char* sensor_key, uint32_t max_data_num,
                                                    uint32_t buf_cnt);

mems_sensor_return_t rkiio_aiq_sensor_handle_destroy(mems_sensor_handle_t handle);

mems_sensor_data_t rkiio_aiq_getData(mems_sensor_handle_t handle, size_t* num_samples);

mems_sensor_data_t rkiio_aiq_getLastNSamples(mems_sensor_handle_t handle,
                                             size_t num_samples);

mems_sensor_return_t rkiio_aiq_sensor_data_release(mems_sensor_handle_t sensor_handle,
                                                   mems_sensor_data_t data);

#endif
