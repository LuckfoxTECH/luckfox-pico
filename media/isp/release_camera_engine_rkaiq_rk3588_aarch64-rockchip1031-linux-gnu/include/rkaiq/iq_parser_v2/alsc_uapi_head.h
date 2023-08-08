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

#ifndef __ALSC_UAPI_HEAD_H__
#define __ALSC_UAPI_HEAD_H__

#include "alsc/rk_aiq_types_alsc_hw.h"

typedef struct rk_aiq_lsc_table_s {
    // M4_ARRAY_DESC("lsc_sect_size_x", "u16", M4_SIZE(1,16), M4_RANGE(0,10000), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    uint16_t    lsc_sect_size_x[LSC_SIZE_TBL_SIZE];
    // M4_ARRAY_DESC("lsc_sect_size_y", "u16", M4_SIZE(1,16), M4_RANGE(0,10000), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    uint16_t    lsc_sect_size_y[LSC_SIZE_TBL_SIZE];
    // M4_ARRAY_DESC("r_data_tbl", "u16", M4_SIZE(17,17), M4_RANGE(0,4096), "0", M4_DIGIT(0),  M4_DYNAMIC(0))
    unsigned short r_data_tbl[LSC_DATA_TBL_SIZE];
    // M4_ARRAY_DESC("gr_data_tbl", "u16", M4_SIZE(17,17), M4_RANGE(0,4096), "0", M4_DIGIT(0),  M4_DYNAMIC(0))
    unsigned short gr_data_tbl[LSC_DATA_TBL_SIZE];
    // M4_ARRAY_DESC("gb_data_tbl", "u16", M4_SIZE(17,17), M4_RANGE(0,4096), "0", M4_DIGIT(0),  M4_DYNAMIC(0))
    unsigned short gb_data_tbl[LSC_DATA_TBL_SIZE];
    // M4_ARRAY_DESC("b_data_tbl", "u16", M4_SIZE(17,17), M4_RANGE(0,4096), "0", M4_DIGIT(0),  M4_DYNAMIC(0))
    unsigned short b_data_tbl[LSC_DATA_TBL_SIZE];
} rk_aiq_lsc_table_t;

#endif  /*__ALSC_UAPI_HEAD_H__*/
