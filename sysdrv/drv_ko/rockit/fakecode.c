// SPDX-License-Identifier: GPL-2.0
// Copyright (c) 2023 Fuzhou Rockchip Electronics Co., Ltd
#include "linux/list.h"

struct rk_tb_client {
	struct list_head node;
	void *data;
	void (*cb)(void *data);
};

bool rk_tb_mcu_is_done(void) { return true; }
EXPORT_SYMBOL_GPL(rk_tb_mcu_is_done);

int rk_tb_client_register_cb(struct rk_tb_client *client) { return 0; }
EXPORT_SYMBOL_GPL(rk_tb_client_register_cb);

int rk_tb_client_register_cb_head(struct rk_tb_client *client) { return 0; }
EXPORT_SYMBOL_GPL(rk_tb_client_register_cb_head);
