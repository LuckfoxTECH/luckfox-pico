// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//#include <DeviceIo/DeviceIo.h>
#include "Hostapd.h"
#include <Rk_softap.h>
#include <error.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

//#define true 1
//#define false 0
//#define DBG true

#if DBG
#define DEBUG_INFO(M, ...) printf("Hostapd %d: " M, __LINE__, ##__VA_ARGS__)
#else
#define DEBUG_INFO(M, ...)                                                                         \
	do {                                                                                           \
	} while (0)
#endif
#define DEBUG_ERR(M, ...) printf("Hostapd %d: " M, __LINE__, ##__VA_ARGS__)

const bool console_run(const char *cmdline) {
	DEBUG_INFO("cmdline = %s\n", cmdline);
	int ret;
	ret = system(cmdline);
	if (ret < 0) {
		DEBUG_ERR("Running cmdline failed: %s\n", cmdline);
		return false;
	}
	return true;
}

int check_wifi_chip_type_string(char *type) {
	int ret, fd;
	char buf[64];
	static char WIFI_CHIP_TYPE_PATH[] = "/sys/class/rkwifi/chip";

	ret = 0;
	memset(buf, 0, 64);
	fd = open(WIFI_CHIP_TYPE_PATH, O_RDONLY);
	if (fd < 0) {
		ret = fd;
		goto end;
	}

	if (0 == read(fd, buf, 32)) {
		ret = -1;
		goto end;
	}
	strcpy(type, buf);

end:
	if (fd >= 0)
		close(fd);

	return ret;
}

int _create_hostapd_file(const char *ap, const char *ssid, const char *psk) {
	FILE *fp;
	char cmdline[256] = {0};
	static char HOSTAPD_CONF_DIR[] = "/userdata/bin/hostapd.conf";

	fp = fopen(HOSTAPD_CONF_DIR, "wt+");
	if (NULL == fp)
		return -1;

	sprintf(cmdline, "interface=%s\n", ap);
	fputs(cmdline, fp);
	fputs("ctrl_interface=/var/run/hostapd\n", fp);
	fputs("driver=nl80211\n", fp);
	fputs("ssid=", fp);
	fputs(ssid, fp);
	fputs("\n", fp);
	fputs("channel=6\n", fp);
	fputs("hw_mode=g\n", fp);
	fputs("ieee80211n=1\n", fp);
	fputs("ignore_broadcast_ssid=0\n", fp);

	if (psk != NULL && 0 != strcmp(psk, "")) {
		fputs("auth_algs=1\n", fp);
		fputs("wpa=3\n", fp);
		fputs("wpa_passphrase=", fp);
		fputs(psk, fp);
		fputs("\n", fp);
		fputs("wpa_key_mgmt=WPA-PSK\n", fp);
		fputs("wpa_pairwise=TKIP\n", fp);
		fputs("rsn_pairwise=CCMP", fp);
	}

	fclose(fp);
	return 0;
}

bool _creat_dnsmasq_file() {
	FILE *fp;
	static char DNSMASQ_CONF_DIR[] = "/userdata/bin/dnsmasq.conf";
	static char SOFTAP_INTERFACE_STATIC_IP[] = "10.201.126.1";
	fp = fopen(DNSMASQ_CONF_DIR, "wt+");
	if (NULL == fp)
		return false;

	fputs("user=root\n", fp);
	fputs("listen-address=", fp);
	fputs(SOFTAP_INTERFACE_STATIC_IP, fp);
	fputs("\n", fp);
	fputs("dhcp-range=10.201.126.50,10.201.126.150\n", fp);
	fputs("server=/google/8.8.8.8\n", fp);
	fclose(fp);
	return true;
}

int _wifi_rtl_start_hostapd(const char *ap, const char *ssid, const char *psk, const char *ip) {
	char cmdline[256] = {0};
	static char DNSMASQ_CONF_DIR[] = "/userdata/bin/dnsmasq.conf";
	static char HOSTAPD_CONF_DIR[] = "/userdata/bin/hostapd.conf";
	_create_hostapd_file(ap, ssid, psk);

	sprintf(cmdline, "ifconfig %s up", ap);
	console_run(cmdline);
	sprintf(cmdline, "ifconfig %s 10.201.126.1 netmask 255.255.255.0", ap);
	console_run(cmdline);
	sprintf(cmdline, "route add default gw %s %s", ip, ap);
	console_run(cmdline);

	// _creat_dnsmasq_file();
	memset(cmdline, 0, sizeof(cmdline));
	sprintf(cmdline, "dnsmasq -C %s --interface=%s", DNSMASQ_CONF_DIR, ap);
	console_run(cmdline);

	memset(cmdline, 0, sizeof(cmdline));
	sprintf(cmdline, "hostapd %s &", HOSTAPD_CONF_DIR);
	console_run(cmdline);

	int time = 100;
	while (time-- > 0 && access("/var/run/hostapd", F_OK)) {
		usleep(100 * 1000);
	}
	return 0;
}

int wifi_rtl_start_hostapd(const char *ssid, const char *psk, const char *ip) {
	char wifi_type[64];
	char ap[64];

	check_wifi_chip_type_string(wifi_type);
	DEBUG_INFO("wifi type: %s\n", wifi_type);
	console_run("killall dnsmasq");
	console_run("killall hostapd");
	// console_run("killall udhcpc");

	if (!strncmp(wifi_type, "RTL", 3)) {
		strcpy(ap, "p2p0");
		console_run("ifconfig p2p0 down");
		console_run("rm -rf /userdata/bin/p2p0");
	} else {
		strcpy(ap, "wlan1");
		console_run("ifconfig wlan1 down");
		console_run("rm -rf /userdata/bin/wlan1");
		console_run("iw dev wlan1 del");
		console_run("ifconfig wlan0 up");

		if (!strncmp(wifi_type, "AP6181", 6)) {
			console_run("iw dev wlan0 interface add wlan1 type __ap");
		} else {
			console_run("iw phy0 interface add wlan1 type managed");
		}
	}

	return _wifi_rtl_start_hostapd(ap, ssid, psk, ip);
}

int wifi_rtl_stop_hostapd() {
	char wifi_type[64];

	console_run("killall hostapd");
	check_wifi_chip_type_string(wifi_type);
	if (!strncmp(wifi_type, "RTL", 3)) {
		console_run("ifconfig p2p0 down");
	} else {
		console_run("killall dnsmasq");
		console_run("ifconfig wlan1 down");
	}

	int time = 100;
	while (time-- > 0 && !access("/var/run/hostapd", F_OK)) {
		usleep(10 * 1000);
	}

	return 0;
}

typedef struct {
	RK_SOFTAP_STATE_CALLBACK callback;
	RK_SOFTAP_SERVER_TYPE server_type;
} RkSoftAp;

RkSoftAp m_softap;

int RK_softap_register_callback(RK_SOFTAP_STATE_CALLBACK cb) {
	m_softap.callback = cb;
	return 0;
}

int RK_softap_start(char *name, RK_SOFTAP_SERVER_TYPE server_type) { return 0; }

int RK_softap_stop(void) { return 0; }

int RK_softap_getState(RK_SOFTAP_STATE *pState) { return 0; }
