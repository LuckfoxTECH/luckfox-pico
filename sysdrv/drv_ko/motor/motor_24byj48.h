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

enum motor_status {
	MOTOR_IS_STOP,
	MOTOR_IS_RUNNING,
};

struct motor_reset_data {
	unsigned int x_max_steps;
	unsigned int y_max_steps;
	unsigned int reset_x;
	unsigned int reset_y;
};

enum motor_cnt {
	HORIZONTAL_MOTOR,
	VERTICAL_MOTOR,
	HAS_MOTOR_CNT,
};

struct motor_device_attribute {
	char motor_direction;
	int max_steps;
	int input_move_steps;
	int real_move_steps;
	int cur_steps;
	int motor_speed;
	int reset_point;
	ktime_t  m_kt;
	unsigned int timer_count;
	int motor_gpio[MAX_GPIO_NUM];
	enum motor_status motor_current_status;
};

struct motor_message {
	int x_cur_steps;
	int y_cur_steps;
	enum motor_status x_cur_status;
	enum motor_status y_cur_status;
	int x_cur_speed;
	int y_cur_speed;
};

struct motors_input_steps {
	int input_x_steps;
	int input_y_steps;
};

struct motors_input_speed {
	int input_x_speed;
	int input_y_speed;
};

struct motors_init_data {
	struct motor_reset_data motor_data;
	struct motors_input_speed motor_speed;
};

struct motor_device {
	struct proc_dir_entry *proc;
	struct miscdevice misc_dev;
	struct device *dev;
	struct mutex dev_mutex;
	spinlock_t slock;
	int motor_open_flag;
	struct hrtimer horizontal_motor_timer;
	struct hrtimer vertical_motor_timer;
	struct motor_device_attribute motors[HAS_MOTOR_CNT];
};


#endif //__RK_MOTOR_H__

