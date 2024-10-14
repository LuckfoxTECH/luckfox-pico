/*
 * thumbnails_head.h
 *
 *  Copyright (c) 2021 Rockchip Electronics Co., Ltd.
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

#ifndef __CALIBDBV2_THUMBNAILS_HEAD_H__
#define __CALIBDBV2_THUMBNAILS_HEAD_H__

#include <rk_aiq_comm.h>

RKAIQ_BEGIN_DECLARE

typedef struct rkaiq_thumbnails_config_s {
    // M4_NUMBER_DESC("owner_cookies", "u32", M4_RANGE(0,65535), "1", M4_DIGIT(0))
    uint32_t owner_cookies;
    // M4_NUMBER_DESC("stream_type", "u32", M4_RANGE(0,65535), "1", M4_DIGIT(0))
    uint32_t stream_type;
    // image_source after these nodes, 1 node per bit
    // M4_NUMBER_DESC("after_nodes", "u32", M4_RANGE(0,65535), "1", M4_DIGIT(0))
    uint64_t after_nodes;
    // image_source after the node, 1 node per bit
    // M4_NUMBER_DESC("after_nodes", "u32", M4_RANGE(0,65535), "1", M4_DIGIT(0))
    uint64_t before_node;
    // fourcc code
    // M4_ARRAY_DESC("format", "u8", M4_SIZE(1,4), M4_RANGE(0,100), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    char format[4];
    // M4_NUMBER_DESC("width_intfactor", "u32", M4_RANGE(0,65535), "1", M4_DIGIT(0))
    uint32_t width_intfactor;
    // M4_NUMBER_DESC("height_intfactor", "u32", M4_RANGE(0,65535), "1", M4_DIGIT(0))
    uint32_t height_intfactor;
    // M4_NUMBER_DESC("buffer_count", "s32", M4_RANGE(-65535,65535), "1", M4_DIGIT(0))
    int32_t buffer_count;
} rkaiq_thumbnails_config_t;

typedef struct CalibDbV2_Thumbnails_Param_s {
    // pointers to thumbnail info array
    // which contains format/width/height/stride
    // M4_STRUCT_LIST_DESC("thumbnail_configs", M4_SIZE(1,4), "normal_ui_style")
    rkaiq_thumbnails_config_t* thumbnail_configs;
    // total thumbnails count
    uint32_t thumbnail_configs_len;
} CalibDbV2_Thumbnails_Param_t;

typedef struct CalibDbV2_Thumbnails_s {
	// M4_STRUCT_DESC("param", "normal_ui_style")
    CalibDbV2_Thumbnails_Param_t param;
} CalibDbV2_Thumbnails_t;

RKAIQ_END_DECLARE

#endif
