#include <ctype.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <errno.h>
#include <sys/prctl.h>
#include <sys/time.h>
#include <stdbool.h>

#include "rk_wifi_hal.h"
#include "Rk_wifi.h"
#include "slog.h"

extern unsigned char g_hi_wifi_info_lock;
extern RK_WIFI_INFO_Connection_s g_hi_wifi_info;

//获取当前WiFi运行状态
int hisi_running_getState(RK_WIFI_RUNNING_State_e* pState)
{
	return true;
}

//获取当前的连接信息
int hisi_running_getConnectionInfo(RK_WIFI_INFO_Connection_s* pInfo)
{
	memcpy(pInfo, &g_hi_wifi_info, sizeof(RK_WIFI_INFO_Connection_s));
	return true;
}

int hisi_connect1(char* ssid, const char* psk, const RK_WIFI_CONNECTION_Encryp_e encryp, const int hide)
{
	return true;
}

//打开WiFi
int hisi_enable(int enable)
{
	int ret = 0;
	if (enable)
	{
		ret = hisi_main();
	}
	else
	{
		ret = hisi_deinit();
	}

	if (ret < 0)
		ret = false;
	else
		ret = true;

	return ret;
}

//扫描
int hisi_scan(void)
{
	return true;
}

//获取扫描结果
char *hisi_scan_r(void)
{
	return NULL;
}

//发起连接
int hisi_connect(char *ssid, const char *psk)
{
	char sendmsg[120] = {0};
	int msglen = 0;
	int ret;

	vlink_hi_channel_cmd_netcfg_info(ssid, psk, sendmsg, &msglen);
	ret = hi_channel_send_to_dev(sendmsg, msglen);

	if (ret < 0)
		ret = false;
	else
		ret = true;

	return ret;
}

//连接WEP加密的WIFI
int hisi_connect_wep(char* ssid, const char* psk)
{
	return true;
}

//断开当前网络
int hisi_disconnect_network(void)
{
	int ret = vlink_hi_channel_cmd_sta_disconnect();

	if (ret < 0)
		ret = false;
	else
		ret = true;

	return ret;
}

//AP模块设置路由信息
int hisi_set_hostname(const char* name)
{
	return true;
}

int hisi_get_hostname(char* name, int len)
{
	return true;
}

//获取本机WiFi的MAC地址
int hisi_get_mac(char *wifi_mac)
{
	//todo linux ioctl
	return true;
}

int hisi_ping(char *address)
{
	return true;
}

//WiFi恢复之前的配置,或重连之前的配置
int hisi_recovery(void)
{
	return true;
}

//移除连接过的WiFi
int hisi_forget_with_ssid(char *ssid)
{
	return true;
}

//取消配网
int hisi_cancel(void)
{
	return true;
}

//获取配置过的WiFi信息
int hisi_getSavedInfo(RK_WIFI_SAVED_INFO_s **pInfo, int *ap_cnt)
{
	return true;
}

//连接之前连过的WiFi
int hisi_connect_with_ssid(char* ssid)
{
	return true;
}

//WiFi恢复出厂设置
int hisi_reset(void)
{
	vlink_hi_channel_cmd_reset();
	return true;
}

//获取AP的信号强度
int hisi_get_connected_ap_rssi(void)
{
	return true;
}

//获取COUNTRY CODE
int hisi_get_ccode(void)
{
	return true;
}

//配置COUNTRY CODE
int hisi_set_ccode(void)
{
	return true;
}

char *hisi_scan_for_softap(void)
{
	return true;
}

/*
 * 低功耗专用API
 */
int hisi_enter_sleep(void)
{
	vlink_hi_channel_cmd_deepsleep_info();
	return true;
}

int hisi_exit_sleep(void)
{
	vlink_hi_channel_cmd_exit_deepsleep();
	return true;
}

int hisi_start_keepalive(RK_WIFI_LOW_POWER_KEEPALIVE_s *cfg)
{
	vlink_hi_channel_cmd_setfilter_info("wifi");
	sleep(1);
	char port[16];
	char period[16];
	snprintf(port,sizeof(port),"%d",cfg->port);
	snprintf(period,sizeof(period),"%d",cfg->period);
	vlink_hi_channel_cmd_keeplive_info(cfg->domain,port, period,cfg->device_id,cfg->key);
	return true;
}

int hisi_stop_keepalive(int port)
{
	vlink_hi_channel_cmd_setfilter_info("camera");

	return true;
}

int hisi_set_BeaconListenInterval(int dtim)
{
	return true;
}

int hisi_set_pir(int enable)
{
	vlink_hi_channel_cmd_setpir_info(enable);
	return true;
}

int hisi_wifi_ota(char *path)
{
	return vlink_hi_channel_cmd_ota(path);
}

int hisi_get_batlevel(int type)
{
	return vlink_hi_channel_cmd_lpevent(RK_WIFI_LP_BAT_LEVEL);
}

int hisi_get_wkreason(int type)
{
	return vlink_hi_channel_cmd_lpevent(RK_WIFI_LP_WAKEUP_REASON);
}

int hisi_get_utc(unsigned long long * ret)
{
	return vlink_hi_channel_cmd_utc(ret);
}


int hisi_wifi_reboot(void)
{
	return vlink_hi_channel_cmd_reboot();
}

int hisi_wifi_get_pir(unsigned int *pir_ret)
{
	return vlink_hi_channel_cmd_getpir_info(pir_ret);
}

struct Rk_wifi_driver_ops hisi_driver_ops = {
	.name = "hichannel iot",
	.wifi_enable = hisi_enable,
	.wifi_disconnect_network = hisi_disconnect_network,
	.wifi_connect = hisi_connect,
	.wifi_running_getConnectionInfo = hisi_running_getConnectionInfo,
	.wifi_enter_sleep = hisi_enter_sleep,
	.wifi_start_keepalive = hisi_start_keepalive,
	.wifi_stop_keepalive = hisi_stop_keepalive,
	.wifi_set_BeaconListenInterval = hisi_set_BeaconListenInterval,
	.wifi_set_pir = hisi_set_pir,
	.wifi_ota = hisi_wifi_ota,
	.wifi_get_batlevel = hisi_get_batlevel,
	.wifi_get_wkreason = hisi_get_wkreason,
	.wifi_get_utc = hisi_get_utc,
	.wifi_reset = hisi_reset,
	.wifi_reboot = hisi_wifi_reboot,
	.wifi_get_pir = hisi_wifi_get_pir,
};
