/**
  * Copyright (c) 2020 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#ifndef _OTA_FILE_H_
#define _OTA_FILE_H_

#include "ota.h"

#ifdef __cplusplus
extern "C" {
#endif

#if OTA_OPT_PROTOCOL_FILE
rt_bool_t  ota_is_exist_fw_file(char *path);
ota_status ota_update_file_init(void *url);
ota_status ota_update_file_get(uint8_t *buf, uint32_t buf_size, uint32_t *recv_size, uint8_t *eof_flag);
ota_status ota_update_file_deinit(void);
ota_status ota_data_check(void *url);
ota_status ota_dsp_version_cmp(void);


#endif

#ifdef __cplusplus
}
#endif

#endif /* _OTA_FILE_H_ */
