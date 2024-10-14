/*
 * Copyright (c) 2022 Rockchip Electronics Co. Ltd.
 */

#ifndef __RKCRYPTO_RANDOM_H__
#define __RKCRYPTO_RANDOM_H__

#include <stdint.h>
#include "rkcrypto_common.h"

RK_RES rk_get_random(uint8_t *data, uint32_t len);

#endif
