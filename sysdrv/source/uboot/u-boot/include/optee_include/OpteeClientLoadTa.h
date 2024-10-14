/*
 * Copyright 2023, Rockchip Electronics Co., Ltd
 * hisping lin, <hisping.lin@rock-chips.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#ifndef _OPTEE_CLIENT_LOAD_TA_H_
#define _OPTEE_CLIENT_LOAD_TA_H_

#include <optee_include/tee_base_types.h>
#include <optee_include/tee_client_api.h>

#define TA_BINARY_FOUND 0
#define TA_BINARY_NOT_FOUND -2

struct userta_header {
	uint32_t magic;
	uint32_t img_ver;
	uint32_t ta_num;
    uint32_t reserve;
};

struct userta_item {
	uint8_t ta_uuid[16];
	uint32_t ta_offset;
	uint32_t ta_len;
	uint32_t ta_ver;
    uint32_t reserve;
};

int search_ta(void *uuid_octets, void *ta, size_t *ta_size);

#endif /*_OPTEE_CLIENT_LOAD_TA_H_*/
