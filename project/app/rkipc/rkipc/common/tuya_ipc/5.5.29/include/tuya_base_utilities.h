/**
 * @file tuya_base_utilities.h
 * @brief tuya utilities module
 * @version 1.0
 * @date 2019-10-30
 *
 * @copyright Copyright (c) tuya.inc 2019
 *
 */
#ifndef _TUYA_BASE_UTILITIES_H
#define _TUYA_BASE_UTILITIES_H

#include "cJSON.h"
#include "hashmap.h"
#include "mem_pool.h"
#include "mix_method.h"
#include "ty_cJSON.h"
#include "uni_hlist.h"
#include "uni_network.h"
#include "uni_pointer.h"
#include "uni_queue.h"
#include "uni_rfc.h"
#include "uni_slist.h"
#include "uni_thread.h"
#include "uni_time.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief utilites initialization
 *
 * @return OPRT_OK on success, others on failed, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_base_utilities_init(VOID);

#ifdef __cplusplus
}
#endif

#endif
