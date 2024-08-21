/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_VOP_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup VOP
 *  @{
 */
#ifndef _HAL_MATH_H_
#define _HAL_MATH_H_

/** @defgroup VOP_Private_Definition Private Definition
 *  @{
 */

static const int SinTable[] =
{
    0x00000000, 0x023be165, 0x04779632, 0x06b2f1d2, 0x08edc7b6, 0x0b27eb5c,
    0x0d61304d, 0x0f996a26, 0x11d06c96, 0x14060b67, 0x163a1a7d, 0x186c6ddd,
    0x1a9cd9ac, 0x1ccb3236, 0x1ef74bf2, 0x2120fb82, 0x234815ba, 0x256c6f9e,
    0x278dde6e, 0x29ac379f, 0x2bc750e8, 0x2ddf003f, 0x2ff31bdd, 0x32037a44,
    0x340ff241, 0x36185aee, 0x381c8bb5, 0x3a1c5c56, 0x3c17a4e7, 0x3e0e3ddb,
    0x3fffffff, 0x41ecc483, 0x43d464fa, 0x45b6bb5d, 0x4793a20f, 0x496af3e1,
    0x4b3c8c11, 0x4d084650, 0x4ecdfec6, 0x508d9210, 0x5246dd48, 0x53f9be04,
    0x55a6125a, 0x574bb8e5, 0x58ea90c2, 0x5a827999, 0x5c135399, 0x5d9cff82,
    0x5f1f5ea0, 0x609a52d1, 0x620dbe8a, 0x637984d3, 0x64dd894f, 0x6639b039,
    0x678dde6d, 0x68d9f963, 0x6a1de735, 0x6b598ea1, 0x6c8cd70a, 0x6db7a879,
    0x6ed9eba0, 0x6ff389de, 0x71046d3c, 0x720c8074, 0x730baeec, 0x7401e4bf,
    0x74ef0ebb, 0x75d31a5f, 0x76adf5e5, 0x777f903b, 0x7847d908, 0x7906c0af,
    0x79bc384c, 0x7a6831b8, 0x7b0a9f8c, 0x7ba3751c, 0x7c32a67c, 0x7cb82884,
    0x7d33f0c8, 0x7da5f5a3, 0x7e0e2e31, 0x7e6c924f, 0x7ec11aa3, 0x7f0bc095,
    0x7f4c7e52, 0x7f834ecf, 0x7fb02dc4, 0x7fd317b3, 0x7fec09e1, 0x7ffb025e,
    0x7fffffff
};

/**
 * _HAL_Sin() returns the sin of an angle in angles
 *
 * @angles: angle, in angles, from 0 to 360.
 *
 * The returned value ranges from -0x7fffffff to +0x7fffffff.
 */
static int _HAL_Sin(int angles)
{
    int ret;

    if (angles > 180) {
        ret = -SinTable[angles - 180];
    } else if (angles > 90) {
        ret = SinTable[180 - angles];
    } else {
        ret = SinTable[angles];
    }

    return ret;
}

/**
 * HAL_Sin() returns the sin of an angle in angles
 *
 * @angles: angle, in angles. The angle can be positive or negative
 *
 * The returned value ranges from -0x7fffffff to +0x7fffffff.
 */
static inline int HAL_Sin(int angles)
{
    angles = (angles % 360 + 360) % 360;

    return _HAL_Sin(angles);
}

/* HAL_Cos(x) = HAL_Sin(x + 90 angles) */
#define HAL_Cos(v) HAL_Sin((v) + 90)

/** @} */

#endif

/** @} */

/** @} */

#endif
