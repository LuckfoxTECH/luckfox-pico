/*
 * Copyright (c) 2022 rockchip
 *
 */
#include "rk_motor.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv)
{
    int ret;
    struct motors_init_data *pinit_data = NULL;
    struct motors_input_steps input_steps;
    struct motor_message *get_motor_message = NULL;
    struct motors_input_speed input_speed;
    struct motor_reset_data reset_data;

    pinit_data = (struct motors_init_data *)malloc(sizeof(struct motors_init_data));
    if(pinit_data == NULL) {
        printf("malloc motor init data fail\n");
        return -1;
    }
    memset(pinit_data, 0, sizeof(struct motors_init_data));
    pinit_data->motor_data.x_max_steps = 4000;
    pinit_data->motor_data.y_max_steps = 4000;
    pinit_data->motor_data.reset_x = 2000;
    pinit_data->motor_data.reset_y = 2000;
    pinit_data->motor_speed.input_x_speed = 1800000;
    pinit_data->motor_speed.input_y_speed = 1800000;
    ret = rk_motor_init(pinit_data);
    if(ret) {
        printf("rk_motor_init fail\n");
        goto fail;
    }
    get_motor_message = (struct motor_message *)malloc(sizeof(struct motor_message));
    if (get_motor_message == NULL) {
        printf("get_motor_message fail\n");
        goto fail;
    }
    sleep(2);
    memset(get_motor_message , 0, sizeof(struct motor_message));
    ret = rk_motor_get_status(get_motor_message);
    if (ret){
        printf("test motor get status fail\n");
        goto fail;
    }
    printf("x_current_steps=%d\n", get_motor_message->x_cur_steps);
    printf("x_cur_status=%d\n", get_motor_message->x_cur_status);
    printf("x_cur_speed=%d\n", get_motor_message->x_cur_speed);
    printf("y_current_steps=%d\n", get_motor_message->y_cur_steps);
    printf("y_cur_status=%d\n", get_motor_message->y_cur_status);
    printf("y_cur_speed=%d\n", get_motor_message->y_cur_speed);
    input_steps.input_x_steps = 2000;
    input_steps.input_y_steps = 2000;
    ret = rk_motor_move(input_steps);
    if(ret) {
        printf("motor test move motor fail\n");
    }
    sleep(2);
    ret = rk_motor_stop();
    if(ret) {
        printf("test motor stop fail\n");
    }
    sleep(2);
    input_speed.input_x_speed = 900000;
    input_speed.input_y_speed = 900000;
    ret = rk_motor_speed(input_speed);
    if(ret){
        printf("test motor speed fail\n");
    }
    memset(get_motor_message , 0, sizeof(struct motor_message));
    ret = rk_motor_get_status(get_motor_message);
    if (ret){
        printf("test motor get status fail\n");
        goto fail;
    }
    printf("x_current_steps=%d\n", get_motor_message->x_cur_steps);
    printf("x_cur_status=%d\n", get_motor_message->x_cur_status);
    printf("x_cur_speed=%d\n", get_motor_message->x_cur_speed);
    printf("y_current_steps=%d\n", get_motor_message->y_cur_steps);
    printf("y_cur_status=%d\n", get_motor_message->y_cur_status);
    printf("y_cur_speed=%d\n", get_motor_message->y_cur_speed);

    reset_data.x_max_steps = 4000;
    reset_data.y_max_steps = 4000;
    reset_data.reset_x = 2000;
    reset_data.reset_y = 2000;
    ret = rk_motor_reset(reset_data);
    if(ret){
        printf("test motor reset fail\n");
    }
    sleep(5);
    input_steps.input_x_steps = -1000;
    input_steps.input_y_steps = -1000;
    ret = rk_motor_move(input_steps);
    if(ret) {
        printf("motor test move motor fail\n");
    }
    sleep(3);
    ret = rk_motor_goback();
    if(ret) {
        printf("motor test goback fail\n");
    }
    sleep(2);
fail:
    ret = rk_motor_deinit();
    if(pinit_data)
        free(pinit_data);
    if(get_motor_message)
        free(get_motor_message);
    return 0;
}

