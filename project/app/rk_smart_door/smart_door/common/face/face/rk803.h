/* SPDX-License-Identifier: ((GPL-2.0+ WITH Linux-syscall-note) OR MIT) */
/*
 * Copyright (C) 2021 Rockchip Electronics Co., Ltd.
 */

#ifndef _UAPI_RK803_H
#define _UAPI_RK803_H

#include <linux/types.h>

#define RK803_SET_GPIO1		_IOW('p',  1, int)
#define RK803_SET_GPIO2		_IOW('p',  2, int)
#define RK803_SET_CURENT1	_IOW('p',  3, int)
#define RK803_SET_CURENT2	_IOW('p',  4, int)
#define RK803_SET_STROBE	_IOW('p',  5, int)

enum SL_LED_CURRENT {
	LED_100MA = 0,
	LED_200MA,
	LED_300MA,
	LED_400MA,
	LED_500MA,
	LED_600MA,
	LED_700MA,
	LED_800MA,
	LED_900MA,
	LED_1000MA,
	LED_1100MA,
	LED_1200MA,
	LED_1300MA,
	LED_1400MA,
	LED_1500MA,
	LED_1600MA,
	LED_1700MA,
	LED_1800MA,
	LED_1900MA,
	LED_2000MA,
	LED_2100MA,
	LED_2200MA,
	LED_2300MA,
	LED_2400MA,
	LED_2500MA,
	LED_2600MA,
	LED_2700MA,
	LED_2800MA,
	LED_2900MA,
	LED_3000MA,
	LED_3100MA,
	LED_3200MA
};
#endif /* _UAPI_RK803_H */
