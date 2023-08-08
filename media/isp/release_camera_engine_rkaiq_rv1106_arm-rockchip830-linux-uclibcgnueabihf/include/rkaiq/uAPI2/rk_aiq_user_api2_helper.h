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

#ifndef __RK_AIQ_USER_API2_HELPER_H__
#define __RK_AIQ_USER_API2_HELPER_H__

#include "rk_aiq_user_api_sysctl.h"

typedef enum __RkAiqUapiOpMode {
  RKAIQUAPI_OPMODE_SET = 0,
  RKAIQUAPI_OPMODE_GET,
} RkAiqUapiOpMode_t;

#define RKAIQ_UAPI_NAME_MAX_LEN 64
#define JSON_PATCH_PATH "path"
#define JSON_PATCH_VALUE "value"

#define UAPIRPC_DEBUG

#define __RKAIQUAPI_SET(api_name, type_name)                                   \
  inline int __rkaiq_uapi_##api_name##_set(void *ctx, void *data) {            \
    rk_aiq_sys_ctx_t *__ctx = (rk_aiq_sys_ctx_t *)ctx;                         \
    type_name *__data = (type_name *)data;                                     \
    return rkaiq_uapi_##api_name##_set(__ctx, __data);                         \
  }

#define __RKAIQUAPI_GET(api_name, type_name)                                   \
  inline int __rkaiq_uapi_##api_name##_get(void *ctx, void *data) {            \
    rk_aiq_sys_ctx_t *__ctx = (rk_aiq_sys_ctx_t *)ctx;                         \
    type_name *__data = (type_name *)data;                                     \
    return rkaiq_uapi_##api_name##_get(__ctx, __data);                         \
  }

#define __RKAIQUAPI_CALLER(type_name)                                          \
  inline int __rkaiq_uapi_##type_name##_call(                                  \
      void *desc, void *sys_ctx, cJSON *cmd_js, cJSON **ret_js, int mode) {    \
    RkAiqUapiDesc_t *uapi_desc = (RkAiqUapiDesc_t *)desc;                      \
    rk_aiq_sys_ctx_t *aiq_ctx = (rk_aiq_sys_ctx_t *)sys_ctx;                   \
    char* js_str = NULL;                                                       \
    j2s_ctx ctx;                                                               \
    int ret = -1;                                                              \
    j2s_init(&ctx);                                                            \
    ctx.format_json = false;                                                   \
    ctx.manage_data = false;                                                   \
    type_name real_obj;                                                        \
                                                                               \
    if (mode == RKAIQUAPI_OPMODE_SET) {                                        \
      /* Get old json then apply change */                                     \
      cJSON *old_json = NULL;                                                  \
      ret = __rkaiq_uapi_##type_name##_call(desc, sys_ctx, cmd_js, &old_json,  \
                                            RKAIQUAPI_OPMODE_GET);             \
      if (ret || !old_json) {                                                  \
        XCAM_LOG_ERROR("sysctl for %s readback failed.", #type_name);          \
        return -1;                                                             \
      }                                                                        \
      ret = cJSONUtils_ApplyPatches(old_json, cmd_js);                         \
      if (0 != ret) {                                                          \
        XCAM_LOG_ERROR("%s apply patch failed %d!", __func__, ret);            \
        return -1;                                                             \
      }                                                                        \
      memset(&real_obj, 0, sizeof(type_name));                                 \
      ret = j2s_json_to_struct(&ctx, old_json, #type_name, &real_obj);         \
      j2s_deinit(&ctx);                                                        \
      if (ret) {                                                               \
        return -1;                                                             \
      }                                                                        \
      if (!uapi_desc->arg_set) {                                               \
        return -1;                                                             \
      }                                                                        \
      return uapi_desc->arg_set(aiq_ctx, &real_obj);                           \
    } else if (mode == RKAIQUAPI_OPMODE_GET) {                                 \
      if (!uapi_desc->arg_get) {                                               \
        return -1;                                                             \
      }                                                                        \
      uapi_desc->arg_get(aiq_ctx, &real_obj);                                  \
      *ret_js = j2s_struct_to_json(&ctx, #type_name, &real_obj);               \
      j2s_deinit(&ctx);                                                        \
      if (!*ret_js) {                                                          \
        XCAM_LOG_ERROR("create %s failed.", #type_name);                       \
        return -1;                                                             \
      }                                                                        \
    }                                                                          \
    return 0;                                                                  \
  }

#define __RKAIQUAPI_SET_FUNC(api_name, type_name) __rkaiq_uapi_##api_name##_set
#define __RKAIQUAPI_GET_FUNC(api_name, type_name) __rkaiq_uapi_##api_name##_get
#define __RKAIQUAPI_FUNC_CAST(__api) (RkAiqUapi)(__api)

typedef int (*RkAiqUapi)(void *, void *);
typedef int (*RkAiqUapiCaller)(void *, void *, void *, void **, int);

typedef struct __RkAiqUapiDesc {
  char arg_path[RKAIQ_UAPI_NAME_MAX_LEN];
  char arg_type[RKAIQ_UAPI_NAME_MAX_LEN];
  RkAiqUapi arg_set;
  RkAiqUapi arg_get;
  RkAiqUapiCaller uapi_caller;
} RkAiqUapiDesc_t;

#ifdef __cplusplus

#define __RKAIQUAPI_DESC_DEF(__arg_path, __arg_type, __arg_set, __arg_get)     \
  {                                                                            \
    __arg_path, #__arg_type, (RkAiqUapi)__arg_set, (RkAiqUapi)__arg_get,       \
        (RkAiqUapiCaller)__rkaiq_uapi_##__arg_type##_call,                     \
  }

#else

#define __RKAIQUAPI_DESC_DEF(__arg_path, __arg_type, __arg_set, __arg_get)     \
  {                                                                            \
    .arg_path = {__arg_path}, .arg_type = {#__arg_type},                       \
    .arg_set = (RkAiqUapi)__arg_set, .arg_get = (RkAiqUapi)__arg_get,          \
    .uapi_caller = (RkAiqUapiCaller)__rkaiq_uapi_##__arg_type##_call,          \
  }

#endif

#define __RKAIQUAPI_SET_WRAPPER(func, type_name)                               \
  int func##2(const rk_aiq_sys_ctx_t *sys_ctx, const type_name *ptr) {         \
    return func(sys_ctx, *ptr);                                                \
  }

#define __RKAIQUAPI_SET_WRAPPER_NAME(func) func##2

int rkaiq_uapi_unified_ctl(rk_aiq_sys_ctx_t *sys_ctx, const char *js_str,
                           char **ret_str, int op_mode);

#endif /*__RK_AIQ_USER_API2_HELPER_H__*/
