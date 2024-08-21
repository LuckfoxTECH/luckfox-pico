// Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef __RK_LOG_INTERNAL_H__
#define __RK_LOG_INTERNAL_H__

//these functions do not check level versus RK_LOCAL_LEVEL, this should be done in rk_log.h
void rk_log_buffer_hex_internal(const char *tag, const void *buffer, uint16_t buff_len, rk_log_level_t level);
void rk_log_buffer_char_internal(const char *tag, const void *buffer, uint16_t buff_len, rk_log_level_t level);
void rk_log_buffer_hexdump_internal(const char *tag, const void *buffer, uint16_t buff_len, rk_log_level_t log_level);

#endif

