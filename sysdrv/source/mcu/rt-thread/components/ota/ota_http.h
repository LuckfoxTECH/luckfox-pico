/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 */

#ifndef _OTA_HTTP_H_
#define _OTA_HTTP_H_

#include "ota.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    char *target;
    const char *tag;
    int frame_size;
    char *type;
    long file_size;
} ota_http_cfg_t;

struct ota_http_opts_t
{
    const char *type;
    int (*init)(struct ota_http_opts_t *self, ota_http_cfg_t *cfg);
    int (*read)(struct ota_http_opts_t *self, char *data, size_t data_len);
    void (*destroy)(struct ota_http_opts_t *self);
    void *userdata;
};

typedef struct ota_http_opts_t ota_http_opts;

typedef struct
{
    char url[256];
    ota_http_opts http_opts;
} ota_http_parameters;


#if OTA_OPT_PROTOCOL_HTTP

int http_opts_init(struct ota_http_opts_t *self, ota_http_cfg_t *cfg);
int http_opts_read(struct ota_http_opts_t *self, char *data,
                   size_t data_len);
void http_opts_destroy(struct ota_http_opts_t *self);

ota_status ota_update_http_init(void *url);
ota_status ota_update_http_get(uint8_t *buf, uint32_t buf_size, uint32_t *recv_size, uint8_t *eof_flag);
ota_status ota_fw_head_http_get(uint8_t *buf, uint32_t buf_size, uint32_t *recv_size, uint8_t *eof_flag);
int ota_start_download_task(void *arg);
void ota_exit_download_task(void);

#endif

#define DEFAULT_HTTP_OPTS { \
            .type = "http", \
            .init = http_opts_init, \
            .read = http_opts_read, \
            .destroy = http_opts_destroy \
        }

#ifdef __cplusplus
}
#endif

#endif /* _OTA_HTTP_H_ */


