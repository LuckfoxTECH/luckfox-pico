/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 */

#ifndef _HTTP_API_H_
#define _HTTP_API_H_

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

#ifdef GNU_GCC
#include <sys/socket.h>
#include <netdb.h>
//#include <netinet/in.h>
#else
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#endif

#include "http_dbg.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HTTP_PREPROCESSOR_FRAME_SIZE (512)
#define HTTP_CONTENT_TYPE_LENGTH 128
#define HTTP_TRANSFER_ENCODING_LENGTH 128
#define HTTP_COOKIE_LENGTH 128
#define HTTP_SOCKET_PORT 80
#define HTTP_HEADER_LENGTH 1024
#define HTTP_DOWNLOAD_FILE_NAME_LENGTH 1024
#define HTTP_HOST_ADDRESS_LENGTH 64
#define HTTP_CR_LF_LEN    2
#define HTTP_REDIRECT_URL_LENGTH 128


typedef enum http_status
{
    HTTP_STATUS_OK       = 0,
    HTTP_STATUS_ERR      = -1,
} http_status_t;

typedef enum http_mode
{
    HTTP_GET       = 0,
    HTTP_POST      = 1,
} http_mode_t;

typedef struct http_param
{
    char   pParam[HTTP_REDIRECT_URL_LENGTH];
    uint32_t  nLength;
} http_param_t;

struct http_res_header
{
    int status_code;//HTTP/1.1 '200' OK
    int http_mode;
    char content_type[HTTP_CONTENT_TYPE_LENGTH];//Content-Type: application/gzip
    char transfer_encoding[HTTP_TRANSFER_ENCODING_LENGTH];//Transfer-Encoding: chunked
    char cookie[HTTP_COOKIE_LENGTH];
    long content_length;//Content-Length: 11683079
    http_param_t httpRedirectURL; // Stores the redirection URL if we got a 301 or 303 return code
};

int is_chunked(struct http_res_header resp);
struct http_res_header parse_header(const char *response);
http_status_t http_open(char *url, char **param1, int *param2, int mode, char *cookie, int offset);
int http_socket_read(int socket, char *data, size_t data_len);
int http_socket_read_chunk(int socket, char *data, size_t data_len);
http_status_t http_close(int socket);

#ifdef __cplusplus
}
#endif

#endif /*_HTTP_API_H_ */
