// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <net/if.h>

#include <arpa/inet.h>
#include <asm/types.h>
#include <ctype.h>
#include <errno.h>
#include <linux/ethtool.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <linux/sockios.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/prctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define true 1
#define false 0

#define ETH_DOWN 1
#define ETH_UNPLUGGED 2

typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef signed short int __int16_t;
typedef unsigned short int __uint16_t;
typedef signed int __int32_t;
typedef unsigned int __uint32_t;

typedef __uint8_t uint8_t;
typedef __uint16_t uint16_t;
typedef __uint32_t uint32_t;
// typedef __uint64_t uint64_t;

typedef signed char int8_t;
typedef unsigned char u_int8_t;
typedef short int16_t;
typedef unsigned short u_int16_t;
typedef int int32_t;
typedef unsigned int u_int32_t;

typedef int (*rk_network_cb)(int status);

typedef struct Ether_info {
	char interface[16];
	char method[16];
	char ip[32];
	char netmask[32];
	char gateway[32];
	char dns[32];
	char dns1[32];
	char mac[48];
	int support_ipv4; // 1:support
	int v4_is_dhcp;
	int link_type; // 0:ethernet 1:vlan
} Ether_info_t;

struct route_info {
	struct in_addr dstAddr;
	struct in_addr srcAddr;
	struct in_addr gateWay;
	char ifName[IF_NAMESIZE];
};

/**
 * @brief 设置IPv4
 *
 * @param[in] interface  eth0
 * @param[in] method	 dhcp or static
 * @param[in] address	 192.168.1.100
 * @param[in] gateway	 192.168.1.1
 * @param[in] Ether_info link_tye = 0
 * @return int
 */
int rk_network_ipv4_set(char *interface, char *method, char *address, char *netmask, char *gateway);

/**
 * @brief 获取IPv4配置
 *
 * @param[in] interface
 * @param[in] method
 * @param[in] address
 * @param[in] gateway
 * @return int 0:获取成功 -1：获取失败
 */
int rk_network_ipv4_get(char *interface, char *method, char *address, char *netmask, char *gateway);

/**
 * @brief 取得当前有线网卡使用的dns
 *
 * @param[in] ethernet_v4_dns
 * @param[in] ethernet_v4_dns2
 *
 * @return
 * count:读取到的dns的数量，count=1是为lo的默认dns,count为2，3时为eth0的两个dns
 */
int rk_network_dns_get(char *ethernet_v4_dns, char *ethernet_v4_dns2);

/**
 * @brief 设置当前有线网卡使用的dns
 *
 * @param[in] ifname 要设置的网卡的名称
 * @param[in] dns
 * @param[in] dns2
 *
 * @return int
 */
int rk_network_dns_set(char *dns, char *dns2);

/**
 * @brief 获取设备MAC地址
 *
 * @param[in] ifname
 * @param[in] mac
 *
 * @return int 0:获取成功 -1：获取失败
 */
int rk_network_get_mac(const char *ifname, char *mac);

/**
 * @brief 设置设备MAC地址
 *
 * @param[in] ifname
 * @param[in] mac
 * @return void
 */
void rk_network_set_mac(const char *ifname, const char *mac);

/**
 * @brief 获取网卡速率
 *
 * @param[in] ifname
 * @param[in] speed
 * @param[in] duplex	1：全双工 0：半双工 ff:unknown
 * @param[in] autoneg	1:on 0:off
 * @return int 0:获取成功 -1：获取失败
 */
int rk_network_nicspeed_get(const char *ifname, int *speed, int *duplex, int *autoneg);

/**
 * @brief 设置网卡速率
 *
 * @param[in] ifname
 * @param[in] speed
 * @param[in] duplex	1：全双工 0：半双工 ff:unknown
 * @param[in] autoneg	1:on 0:off
 * @return int 0:设置成功 -1：获取失败
 */
int rk_network_nicspeed_set(const char *ifname, int speed, int duplex, int autoneg);

/**
 * @brief 获取网卡支持速率
 *
 * @param[in] ifname
 * @return int 0:获取成功 -1：获取失败
 */
int rk_network_nicspeed_support_get(const char *ifname, char *nic_supported);

/**
 * @brief 开关以太网
 *
 * @param[in] Ether_info
 * @param[in] which
 * @param[in] powerswitch  1：开 0：关
 * @return int 0:执行成功
 */
int rk_ethernet_power_set(const char *ifname, int powerswitch);

/**
 * @brief 取得当前网卡状态
 * @param[in] ifname
 * @return 0：nic not found 1:down 2:linked 3:unplugged -1:error
 */
int rk_nic_state_get(const char *ifname);

void rk_network_init(rk_network_cb func);

void rk_network_deinit();

/* ---------------------------------- wifi ----------------------------------*/
/**
 * @brief 获取当前wifi状态
 * @return int 0:开启 1:关闭
 */
int rk_wifi_power_get(int *on);

/**
 * @brief 开关wifi
 * @param[in] on
 * @return int 0:执行成功
 */
int rk_wifi_power_set(int on);

/**
 * @brief 立即扫描wifi
 * @return int 0:执行成功
 */
int rk_wifi_scan_wifi();

/**
 * @brief 获取wifi列表
 * @param[in] wifi_list
 * @return int 0:执行成功
 */
int rk_wifi_get_list(char **wifi_list);

/**
 * @brief 连接wifi
 * @param[in] ssid
 * @param[in] psk
 * @return int 0:执行成功
 */
int rk_wifi_connect_with_ssid(const char *ssid, const char *psk);

/**
 * @brief 删除wifi
 * @param[in] ssid
 * @return int 0:执行成功
 */
int rk_wifi_forget_with_ssid(const char *ssid);
// int RK_wifi_getSavedInfo(RK_WIFI_SAVED_INFO *pSaveInfo); //获取配置过的WiFi
// int RK_wifi_connect_with_ssid(char *ssid);               // 已经连过的
#endif
