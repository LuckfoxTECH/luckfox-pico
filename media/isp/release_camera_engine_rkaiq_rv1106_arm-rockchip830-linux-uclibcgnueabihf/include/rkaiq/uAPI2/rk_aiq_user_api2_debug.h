
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

#ifndef RK_AIQ_USER_API_DEBUG_H
#define RK_AIQ_USER_API_DEBUG_H

#include "rk_aiq.h"

typedef struct rk_aiq_sys_ctx_s rk_aiq_sys_ctx_t;

RKAIQ_BEGIN_DECLARE

XCamReturn
rk_aiq_uapi2_debug_captureRawYuvSync(const rk_aiq_sys_ctx_t* ctx, capture_raw_t type);

/*!
 * \brief capture raw Image synchronously
 *
 * \param[in] ctx             the context returned by \ref rk_aiq_uapi_sysctl_init
 * \param[in] type            the type of capturing raw images
 * \param[in] count           the count of capturing raw images
 * \param[in] capture_dir     Raw's top-level storage directory
 * \param[out] output_dir     Raw's storage directory
 * \return return 0 if success
 */
XCamReturn
rk_aiq_uapi2_debug_captureRawSync(const rk_aiq_sys_ctx_t* ctx, capture_raw_t type,
                                 int count, const char* capture_dir, char* output_dir);

XCamReturn
rk_aiq_uapi2_debug_captureRawNotify(const rk_aiq_sys_ctx_t* ctx);

RKAIQ_END_DECLARE

#endif
