// Copyright 2020-2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RKIPC_ROCKIVA_H__
#define __RKIPC_ROCKIVA_H__

#include "rockiva/rockiva_ba_api.h"

#define MAX_RKNN_LIST_NUM 10

typedef struct node {
	long long timeval;
	RockIvaBaResult ba_result;
	struct node *next;
} Node;

typedef struct my_stack {
	int size;
	Node *top;
} rknn_list;

#ifdef __cplusplus
extern "C" {
#endif

int rkipc_rockiva_init();
int rkipc_rockiva_deinit();
int rkipc_rockiva_write_rgb888_frame(uint16_t width, uint16_t height, uint32_t frame_id,
                                     unsigned char *buffer);
int rkipc_rockiva_write_rgb888_frame_by_fd(uint16_t width, uint16_t height, uint32_t frame_id,
                                           int32_t fd);
int rkipc_rockiva_write_nv12_frame_by_fd(uint16_t width, uint16_t height, uint32_t frame_id,
                                         int32_t fd);
int rkipc_rockiva_write_nv12_frame_by_phy_addr(uint16_t width, uint16_t height, uint32_t frame_id,
                                               uint8_t *phy_addr);
int rkipc_rknn_object_get(RockIvaBaResult *ba_result);
#ifdef __cplusplus
}
#endif
#endif
