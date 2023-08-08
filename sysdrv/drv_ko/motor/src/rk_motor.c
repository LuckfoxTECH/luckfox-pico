/*
 * Copyright (c) 2022 rockchip
 *
 */

#include "rk_motor.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>


/* ioctl cmd */
#define MOTOR_STOP          _IOW('M',0,long)
#define MOTOR_RESET         _IOW('M',1,long)
#define MOTOR_MOVE          _IOW('M',2,long)
#define MOTOR_GET_STATUS    _IOW('M',3,long)
#define MOTOR_SPEED         _IOW('M',4,long)
#define MOTOR_GOBACK        _IOW('M',5,long)
#define MOTOR_CRUISE        _IOW('M',6,long)
#define MOTOR_SET_STATUS    _IOW('M',7,long)

#define MOTOR_DEV "/dev/motor"
static int motor_fd = -1;

int rk_motor_goback(void) {
    int ret;

    if(motor_fd !=-1) {
        ret = ioctl(motor_fd, MOTOR_GOBACK, 0);
        if(ret) {
            printf("rk_motor_goback fail\n");
            return -1;
        }
    } else {
        printf("motor uninitialized\n");
        return -1;
    }
    return 0;
}

int rk_motor_reset(struct motor_reset_data data) {
    int ret;
    struct motor_reset_data reset_data;

    if(motor_fd != -1){
        reset_data.x_max_steps = data.x_max_steps;
        reset_data.y_max_steps = data.y_max_steps;
        reset_data.reset_x = data.reset_x;
        reset_data.reset_y = data.reset_y;
        ret =ioctl(motor_fd, MOTOR_RESET, (unsigned long)&reset_data);
        if(ret){
            printf("rk_motor_reset fail\n");
            return -1;
        }
    } else {
        printf("motor uninitialized\n");
        return -1;
    }
    return 0;
}

/* motor speed (us)
    max :900000
    min :1800000
*/
int rk_motor_speed(struct motors_input_speed input_speed) {
    int ret;
    struct motors_input_speed motors_speed;

    if(motor_fd !=-1) {
        motors_speed.input_x_speed = input_speed.input_x_speed;
        motors_speed.input_y_speed = input_speed.input_y_speed;
        ret = ioctl(motor_fd, MOTOR_SPEED, (unsigned long)&motors_speed);
        if(ret) {
            printf("rk_motor_speed fail\n");
            return -1;
        }
    } else {
        printf("motor uninitialized\n");
        return -1;
    }
    return 0;
}

int rk_motor_get_status(struct motor_message *pmessage) {
    int ret;
    struct motor_message get_motor_message;

    if(!pmessage) {
        printf("rk_motor_get_status pmessage is NULL");
        return -1;
    }

    if(motor_fd !=-1) {
        ret = ioctl(motor_fd, MOTOR_GET_STATUS, (unsigned long)&get_motor_message);
        if(ret) {
            printf("rk_motor_get_status fail\n");
            return -1;
        }
    }  else {
        printf("motor uninitialized\n");
        return -1;
    }
    pmessage->x_cur_steps = get_motor_message.x_cur_steps;
    pmessage->y_cur_steps = get_motor_message.y_cur_steps;
    pmessage->x_cur_status = get_motor_message.x_cur_status;
    pmessage->y_cur_status = get_motor_message.y_cur_status;
    pmessage->x_cur_speed = get_motor_message.x_cur_speed;
    pmessage->y_cur_speed = get_motor_message.y_cur_speed;

    return 0;
}

int rk_motor_stop(void) {
    int ret;

    if(motor_fd !=-1) {
        ret = ioctl(motor_fd, MOTOR_STOP, 0);
        if(ret) {
            printf("rk_motor_stop fail\n");
            return -1;
        }
    } else {
        printf("motor uninitialized\n");
        return -1;
    }
    return 0;
}

int rk_motor_move(struct motors_input_steps input_steps) {
    int ret;

    struct motors_input_steps pinput_steps;
    pinput_steps.input_x_steps = input_steps.input_x_steps;
    pinput_steps.input_y_steps = input_steps.input_y_steps;
    if(motor_fd !=-1) {
        ret = ioctl(motor_fd, MOTOR_MOVE, (unsigned long)&pinput_steps);
        if (ret) {
            printf("rk_motor_move fail\n");
            return -1;
        }
    } else {
        printf("motor uninitialized\n");
        return -1;
    }
    return 0;
}

int rk_motor_init(struct motors_init_data *data) {
    int ret;

    struct motors_init_data init_data;

    motor_fd = open(MOTOR_DEV, O_RDWR);
    if (motor_fd == -1) {
        perror("open motor dev fail\n");
        return -1;
    }
    if (data !=NULL) {
        init_data.motor_data.x_max_steps = data->motor_data.x_max_steps;
        init_data.motor_data.y_max_steps = data->motor_data.y_max_steps;
        init_data.motor_data.reset_x = data->motor_data.reset_x;
        init_data.motor_data.reset_y = data->motor_data.reset_y;
        init_data.motor_speed.input_x_speed = data->motor_speed.input_x_speed;
        init_data.motor_speed.input_y_speed = data->motor_speed.input_y_speed;
        ret = ioctl(motor_fd, MOTOR_SET_STATUS, (unsigned long)&init_data);
        if(ret) {
            printf("rk_motor_init set status fail\n");
            return -1;
        }
    }
    return 0;
}

int rk_motor_deinit(void) {
    if(motor_fd !=-1) {
        close(motor_fd);
        motor_fd = -1;
    }
    return 0;
}

