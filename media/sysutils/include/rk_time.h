/*
 * Copyright (c) 2022 rockchip
 *
 */

#ifndef __RK_TIME_H__
#define __RK_TIME_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>

int rk_system_get_time(struct tm *time);
int rk_system_set_time(struct tm *time);

int rk_system_get_alarm(struct tm *time);
int rk_system_set_alarm(struct tm *time);
int rk_system_enable_alarm(void);
int rk_system_disable_alarm(void);
int rk_system_wait_alarm(uint32_t timeout);

#endif //__RK_TIME_H__

