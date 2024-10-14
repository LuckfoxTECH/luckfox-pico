/*
 * Copyright 2020 Rockchip Electronics Co. LTD
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
#ifndef SRC_RT_MEDIA_AV_FILER_INCLUDE_RTAIDECTRESULTS_H_
#define SRC_RT_MEDIA_AV_FILER_INCLUDE_RTAIDECTRESULTS_H_

#include "rt_type.h"
#include "RTMediaBuffer.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define RT_AI_MAX_LEN 16

typedef enum _RTAIDetectType {
    RT_AI_DETECT_FACE,
    RT_AI_DETECT_FACELANDMARK,
    RT_AI_DETECT_BODY,
    RT_AI_DETECT_HAND,
    RT_AI_DETECT_FINGER,
    // add more
} RTAIDetectType;

#define VENDOR_RK_ROCKX   "rockx"
#define VENDOR_SENSETIME  "st_asteria"

typedef struct _RTAIDetectResults {
    /*
     * the name of vendor
     */
    char vendor[RT_AI_MAX_LEN];

    /*
     * the version of vendor's ai lib
     */
    char version[RT_AI_MAX_LEN];

    /*
     * describe of private data
     */
    char describe[RT_AI_MAX_LEN];

    /*
     * the sizeof of private data
     */
    INT32 privSize;

    /*
     * the point of private data
     */
    void *privData;
} RTAIDetectResults;

/*
 * create an ai detect struct
 */
RTAIDetectResults* createAIDetectResults();

/*
 * destory an ai detect struct
 * the user must release the privData by themself
 */
void  destroyAIDetectResults(RTAIDetectResults* result);

void* getAIDetectResults(RTMediaBuffer* buffer);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // SRC_RT_MEDIA_AV_FILER_INCLUDE_RTAIDECTRESULTS_H_

