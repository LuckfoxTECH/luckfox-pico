
/*
 *  Copyright (c) 2019 Rockchip Corporation
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

#ifndef _RK_AIQ_UTILS_H_
#define _RK_AIQ_UTILS_H_

#include "RkAiqCalibDbTypes.h"

RKAIQ_BEGIN_DECLARE

/*!
 * \brief convert exposure reg value (time & gain) to real value;
 *
 * \param[out] realTime
 * \param[out] realGain
 *
 * \return return 0 if success
 */
XCamReturn rk_aiq_AeReg2RealValue
(
    const CalibDb_Sensor_Para_t* stSensorInfo,
    float   PixelPeriodsPerLine,
    float   LinePeriodsPerField,
    float   PixelClockFreqMHZ,
    uint32_t sensorRegTime,
    uint32_t sensorRegGain,
    float* realTime,
    float* realGain
);

RKAIQ_END_DECLARE

#endif
