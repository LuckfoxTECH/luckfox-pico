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
#ifndef SAMPLE_FILE_H_
#define SAMPLE_FILE_H_
#include "rk_comm_ive.h"
#include <stdio.h>

#if defined WIN32 || defined _WIN32 || defined WINCE
#define RK_VSPRINTF(buf, cnt, fmt, ...) vsprintf_s(buf, cnt, fmt, ##__VA_ARGS__);
#else
#define RK_VSPRINTF(buf, cnt, fmt, ...) vsprintf(buf, fmt, ##__VA_ARGS__);
#endif

#if defined WIN32 || defined _WIN32 || defined WINCE
#define RK_SPRINTF(buf, cnt, fmt, ...) sprintf_s(buf, cnt, fmt, ##__VA_ARGS__);
#else
#define RK_SPRINTF(buf, cnt, fmt, ...) sprintf(buf, fmt, ##__VA_ARGS__);
#endif

RK_VOID RK_STRCPY(RK_CHAR *dest, size_t size, const RK_CHAR *src);
RK_VOID* RK_Memset(RK_VOID *ptr, RK_S32 value, size_t num);
RK_S32 RK_Memcpy(void *dest, void* src, size_t size);
RK_S32 RK_ReadFileSize(RK_CHAR *filename, FILE **fp_out);
RK_S32 RK_ReadFile(RK_CHAR *filename, IVE_SRC_IMAGE_S *pstImage, FILE **fp_out);
RK_S32 RK_WriteFile(RK_CHAR *filename, void *p_buf, RK_U32 size, FILE **fp_out);

#endif