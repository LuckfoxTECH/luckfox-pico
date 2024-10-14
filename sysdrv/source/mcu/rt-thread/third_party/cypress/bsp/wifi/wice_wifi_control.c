/*
 * =====================================================================================
 *
 *       Filename:  wice_wifi_control.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  06/04/2020 04:33:35 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (),
 *   Organization:
 *
 * =====================================================================================
 */
#include "board_wifibt.h"

int wice_host_reg_on()
{
    rk_wifi_host_reg_on();

    return 1;
}

void wice_host_wake_register(void (*call_back)(void *))
{
    rk_wifi_host_wake_register(call_back);
}

void wice_host_wake_disable()
{
    rk_wifi_host_wake_disable();
}

void wice_host_wake_enable()
{
    rk_wifi_host_wake_enable();
}
