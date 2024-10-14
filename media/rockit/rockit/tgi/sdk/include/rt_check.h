/*
 * Copyright 2018 Rockchip Electronics Co. LTD
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
 * author: rimon.xu@rock-chips.com
 *   date: 20181102
 */

#ifndef INCLUDE_RT_BASE_RT_CHECK_H_
#define INCLUDE_RT_BASE_RT_CHECK_H_

#define CHECK_OP(name, op, actual, exp) \
    do { \
        if (actual op exp) { \
            /* TODO */ \
        } else { \
            RT_LOGE("%s(%d): %s failed. actual %d vs exp %d\n", \
                     __FUNCTION__, __LINE__, name, actual, exp); \
            goto __FAILED; \
        } \
    } while (0)

#define RET_CHECK_OP(name, op, actual, exp) \
    do { \
        if (actual op exp) { \
            /* TODO */ \
        } else { \
            return RT_ERR_UNKNOWN; \
        } \
    } while (0)

#define RET_CHECK_FUNC(func, type, value) \
    do { \
        type ret = func; \
        if (ret != value) \
            return ret; \
    } while (0)

#define RET_CHECK_IS_NULL(ptr, ret) \
    do { \
        if (ptr == RT_NULL) { \
            return ret; \
        } \
    } while (0)

#define CHECK_EQ(actual, exp) CHECK_OP("CHECK_EQ", ==, actual, exp)
#define CHECK_UE(actual, exp) CHECK_OP("CHECK_UE", !=, actual, exp)
#define CHECK_LE(actual, exp) CHECK_OP("CHECK_LE", <=, actual, exp)
#define CHECK_LT(actual, exp) CHECK_OP("CHECK_LT", < , actual, exp)
#define CHECK_GE(actual, exp) CHECK_OP("CHECK_GE", >=, actual, exp)
#define CHECK_GT(actual, exp) CHECK_OP("CHECK_GT", > , actual, exp)

#define RET_CHECK_EQ(actual, exp) RET_CHECK_OP("CHECK_EQ", ==, actual, exp)
#define RET_CHECK_UE(actual, exp) RET_CHECK_OP("CHECK_UE", !=, actual, exp)
#define RET_CHECK_LE(actual, exp) RET_CHECK_OP("CHECK_LE", <=, actual, exp)
#define RET_CHECK_LT(actual, exp) RET_CHECK_OP("CHECK_LT", < , actual, exp)
#define RET_CHECK_GE(actual, exp) RET_CHECK_OP("CHECK_GE", >=, actual, exp)
#define RET_CHECK_GT(actual, exp) RET_CHECK_OP("CHECK_GT", > , actual, exp)

#define FUNC_CHECK(ret) CHECK_OP("FUNC_CHECK", ==, ret, RT_OK)

#define CHECK_IS_NULL(value) CHECK_OP("CHECK_IS_NULL", !=, value, RT_NULL)

#endif  // INCLUDE_RT_BASE_RT_CHECK_H_
