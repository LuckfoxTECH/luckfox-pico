/*
 * Copyright (c) 2022 rockchip
 *
 */

#ifndef __RK_MOTOR_H__
#define __RK_MOTOR_H__

#include <linux/proc_fs.h>
#include <linux/miscdevice.h>
#include <linux/hrtimer.h>

#define MAX_GPIO_NUM 4
#define HORIZONTAL_MAX_STEPS 4000
#define VERTICAL_MAX_STEPS 2000
/* motor speed */
#define MOTOR_MAX_SPEED	1800000      /*us*/
#define MOTOR_MIN_SPEED	9000000

/* ioctl cmd */
#define MOTOR_STOP          _IOW('M', 0, long)
#define MOTOR_RESET         _IOW('M', 1, long)
#define MOTOR_MOVE          _IOW('M', 2, long)
#define MOTOR_GET_STATUS    _IOW('M', 3, long)
#define MOTOR_SPEED         _IOW('M', 4, long)
#define MOTOR_GOBACK        _IOW('M', 5, long)
#define MOTOR_CRUISE        _IOW('M', 6, long)
#define MOTOR_SET_STATUS    _IOW('M', 7, long)
#define MOTOR_RESET_NOTCALI _IOW('M', 8, long)

#endif //__RK_MOTOR_H__

