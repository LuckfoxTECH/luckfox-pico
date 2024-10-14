/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    wice_rtt_interface.h
  * @author  aaron sun
  * @version V0.1
  * @date    25-JUN-2019
  * @brief   wice for rtt wlan struct interface
  *
  ******************************************************************************
  */
#ifndef __WICE_RTT_INTERFACE_H
#define __WICE_RTT_INTERFACE_H

typedef struct {
    short             signal_strength;
    unsigned char     security_key[64];     /* target AP's password */
    unsigned char     security_key_length;  /* length of password */
    unsigned char     ap_ssid_len;              /* target AP's name */
    unsigned char     ap_ssid_value[32];
    unsigned char     ap_bssid[6];             /* target AP's mac address */
    unsigned int      ap_security_type;
    unsigned char     random;
} WIFI_AP;

void wice_create(void);
void wice_delete(void);
int rk_wifi_smartconfig(void);
int rk_smartconfig_stop(void);
WIFI_AP * rk_wifi_smartconfig_get(void);
void mfgcmd(char *pstr);
#endif
