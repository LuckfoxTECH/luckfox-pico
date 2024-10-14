/*
 *  Copyright (C) 2019, Fuzhou Rockchip Electronics Co., Ltd.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of Fuzhou Rockchip Electronics Co., Ltd. nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _WLAN_API_H_
#define _WLAN_API_H_

#include <stdint.h>
#include "sysinfo.h"
#include "net/wlan/wlan_defs.h"
//#include "net/wlan/ethernetif.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef int (*wlan_cb_func)(uint32_t param0);

int wlan_set_state_callback(wlan_cb_func cb);
int wlan_set_mac_addr(uint8_t *mac_addr, int mac_len);

struct netif *wlan_create(enum wlan_mode mode);
int wlan_delete(struct netif *nif);
/* STA */
int wlan_sta_set(uint8_t *ssid, uint8_t ssid_len, uint8_t *psk);
int wlan_sta_enable(void);
int wlan_sta_disable(void);
int wlan_sta_scan_once(void);
int wlan_sta_scan_result(wlan_sta_scan_results_t *results);
rt_bool_t wlan_is_connected(void);
int wlan_sta_connect(void);
int wlan_sta_disconnect(void);
int wlan_sta_state(wlan_sta_states_t *state);

/* softAP */
int wlan_ap_set(uint8_t *ssid, uint8_t ssid_len, uint8_t *psk);
int wlan_ap_set_config(wlan_ap_config_t *config);
int wlan_ap_get_config(wlan_ap_config_t *config);

int wlan_ap_enable(void);
int wlan_ap_reload(void);
int wlan_ap_disable(void);

int wlan_ap_sta_num(int *num);
int wlan_ap_sta_info(wlan_ap_stas_t *stas);
#ifdef __cplusplus
}
#endif
#endif