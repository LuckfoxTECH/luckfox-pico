/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-10     Chris Zhong      First version
*/

#ifndef LV_DEMO_CONF_H
#define LV_DEMO_CONF_H

/*redefine these demos*/
#undef LV_USE_DEMO_BENCHMARK
#undef LV_USE_DEMO_MUSIC
#undef LV_DEMO_MUSIC_SQUARE
#undef LV_DEMO_MUSIC_LANDSCAPE
#undef LV_DEMO_MUSIC_ROUND
#undef LV_DEMO_MUSIC_LARGE
#undef LV_DEMO_MUSIC_AUTO_PLAY

#ifdef RT_LV_DEMO_BENCHMARK
#define LV_USE_DEMO_BENCHMARK   1
#else
#define LV_USE_DEMO_BENCHMARK   0
#endif

#ifdef RT_LV_DEMO_MUSIC
#define LV_USE_DEMO_MUSIC         1
#else
#define LV_USE_DEMO_MUSIC         0
#endif

#define LV_DEMO_MUSIC_SQUARE      0
#define LV_DEMO_MUSIC_LANDSCAPE   0
#define LV_DEMO_MUSIC_ROUND       0
#define LV_DEMO_MUSIC_LARGE       0
#define LV_DEMO_MUSIC_AUTO_PLAY   1

#endif /*LV_DEMO_CONF_H*/
