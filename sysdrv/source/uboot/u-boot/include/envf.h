/*
 * Copyright (c) 2024 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>

/**
 * Get specific key-value pairs from env partition
 *
 * @param desc		block description
 * @param name		which key want to get from env
 * @return key-value pairs if ok, else NULL
 */
char *envf_get(struct blk_desc *desc, const char *name);
