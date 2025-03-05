/*
 * Copyright 2022 Rockchip Electronics Co. LTD
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
//#ifdef __cplusplus
#include "uac_common_def.h"

uint64_t getRelativeTimeMs() {
  return getRelativeTimeUs() / 1000; /* milliseconds */
}

uint64_t getRelativeTimeUs() {
  struct timespec time = {0, 0};
  clock_gettime(CLOCK_MONOTONIC, &time);
  return (uint64_t)time.tv_sec * 1000000LL +
         (uint64_t)time.tv_nsec / 1000; /* microseconds */
}
//#endif