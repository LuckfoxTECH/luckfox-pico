/*
 * Copyright (c) 2022 rockchip
 *
 */

#ifndef __RK_MOTOR_H__
#define __RK_MOTOR_H__

#include <stdint.h>
#include <stdbool.h>

struct motor_reset_data {
    unsigned int x_max_steps;
    unsigned int y_max_steps;
    unsigned int reset_x;
    unsigned int reset_y;
};

/* motor speed (us)
    max :900000
    min :1800000
*/
struct motors_input_speed{
    int input_x_speed;
    int input_y_speed;
};

struct motors_init_data {
    struct motor_reset_data motor_data;
    struct motors_input_speed motor_speed;
};

struct motors_input_steps{
    int input_x_steps;
    int input_y_steps;
};

enum motor_status {
    MOTOR_IS_STOP,
    MOTOR_IS_RUNNING,
};

struct motor_message {
    int x_cur_steps;
    int y_cur_steps;
    enum motor_status x_cur_status;
    enum motor_status y_cur_status;
    int x_cur_speed;
    int y_cur_speed;
};

int rk_motor_init(struct motors_init_data *data);
int rk_motor_deinit(void);
int rk_motor_move(struct motors_input_steps input_steps);
int rk_motor_stop(void);
int rk_motor_get_status(struct motor_message *message);
int rk_motor_speed(struct motors_input_speed input_speed);
int rk_motor_reset(struct motor_reset_data reset_data);
int rk_motor_goback(void);

#endif //__RK_MOTOR_H__

