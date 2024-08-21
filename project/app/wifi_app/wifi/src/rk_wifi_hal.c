#include <stddef.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h> // Add the missing header file for usleep function
#include <string.h>
#include <stdio.h>

#include "utility.h"
#include "Rk_wifi.h"
#include "rk_wifi_hal.h"
#include "slog.h"

struct Rk_wifi_driver_ops *rk_hal_ops = NULL;
RK_wifi_state_callback m_wifi_cb;

//注册状态回调
int RK_wifi_register_callback(RK_wifi_state_callback cb)
{
	m_wifi_cb = cb;
	return true;
}

//打开WiFi
int RK_wifi_enable(int enable)
{
	char buf[64];
	int retry = 200; //200 * 10ms = 2s
	int ret = true;
	pr_info("enter");

	if (rk_hal_ops == NULL) {
		#ifdef ATBM6441
		exec_command("cat /sys/bus/sdio/devices/*/uevent | grep \"007A\:6011\"", buf, 64);
		if (buf[0]) {
			//The atbm603x/6441 have some vid:pid
			exec_command("lsmod | grep atbm6041_wifi_sdio", buf, 64);
			if (buf[0])
				rk_hal_ops = &atbm_driver_ops;
			printf("The wifi is atbm6441\n");
		}
		#endif

		#ifdef HI3861L
		exec_command("cat /sys/bus/sdio/devices/*/uevent | grep \"0296\:5347\"", buf, 64);
		if (buf[0]) {
			rk_hal_ops = &hisi_driver_ops;
			printf("The wifi is hi3861l\n");
		}
		#endif

		#ifdef WPA_WIFI
		//default
		if (!rk_hal_ops) {			
			printf("The wifi is wpa wifi.\n");
			rk_hal_ops = &wpa_driver_ops;
		}
		#endif
	}

	if (!rk_hal_ops) {
		printf("The wifi is unknow!!!.\n");
		return false;
	}

retry:
	exec_command("ifconfig -a | grep wlan0", buf, 64);
	if ((buf[0] == 0) && (retry--)) {
		//printf("waiting wlan0 for ready...\n");
		usleep(10 * 1000);//10ms
		goto retry;
	}

	if (buf[0] == 0) {
		printf("The wifi open failed as not wlan0\n");
		return false;
	}

	if (rk_hal_ops->wifi_enable)
		ret = rk_hal_ops->wifi_enable(enable);

	pr_info("exit");
	return ret;
}

//获取当前连接信息
int RK_wifi_running_getConnectionInfo(RK_WIFI_INFO_Connection_s *pInfo)
{
	memset(pInfo, 0, sizeof(RK_WIFI_INFO_Connection_s));
	if (rk_hal_ops->wifi_running_getConnectionInfo)
		return rk_hal_ops->wifi_running_getConnectionInfo(pInfo);

	return 0;
}

//扫描
int RK_wifi_scan(void)
{
	if (rk_hal_ops->wifi_scan)
		return rk_hal_ops->wifi_scan();

	return 0;
}

//获取扫描结果
char *RK_wifi_scan_r(void)
{
	if (rk_hal_ops->wifi_scan_r)
		return rk_hal_ops->wifi_scan_r();

	return 0;
}

//发起连接
int RK_wifi_connect(char* ssid, const char* psk)
{
	if (rk_hal_ops->wifi_connect)
		return rk_hal_ops->wifi_connect(ssid, psk);

	return 0;
}

//连接WEP加密的WIFI
int RK_wifi_connect_wep(char* ssid, const char* psk)
{
	if (rk_hal_ops->wifi_connect_wep)
		return rk_hal_ops->wifi_connect_wep(ssid, psk);

	return 0;
}

//断开当前网络
int RK_wifi_disconnect_network(void)
{
	if (rk_hal_ops->wifi_disconnect_network)
		return rk_hal_ops->wifi_disconnect_network();

	return 0;
}

//获取本机WiFi的MAC地址
int RK_wifi_get_mac(char *wifi_mac)
{
	if (rk_hal_ops->wifi_get_mac)
		return rk_hal_ops->wifi_get_mac(wifi_mac);

	return 0;
}

//WiFi恢复之前的配置,或重连之前的配置
int RK_wifi_recovery(void)
{
	if (rk_hal_ops->wifi_recovery)
		return rk_hal_ops->wifi_recovery();

	return 0;
}

//移除指定的连接过的WiFi
int RK_wifi_forget_with_ssid(char *ssid)
{
	if (rk_hal_ops->wifi_forget_with_ssid)
		return rk_hal_ops->wifi_forget_with_ssid(ssid);

	return 0;
}

//取消当前正在进行的配网
int RK_wifi_cancel(void)
{
	if (rk_hal_ops->wifi_cancel)
		return rk_hal_ops->wifi_cancel();

	return 0;
}

//获取配置过的WiFi信息
int RK_wifi_getSavedInfo(RK_WIFI_SAVED_INFO_s **pInfo, int *ap_cnt)
{
	if (rk_hal_ops->wifi_getSavedInfo)
		return rk_hal_ops->wifi_getSavedInfo(pInfo, ap_cnt);

	return 0;
}

//连接指定的之前连过的WiFi
int RK_wifi_connect_with_ssid(char *ssid)
{
	if (rk_hal_ops->wifi_connect_with_ssid)
		return rk_hal_ops->wifi_connect_with_ssid(ssid);

	return 0;
}

//WiFi恢复出厂设置
int RK_wifi_reset(void)
{
	if (rk_hal_ops->wifi_reset)
		return rk_hal_ops->wifi_reset();

	return 0;
}

//获取连到AP的信号强度
int RK_wifi_get_connected_ap_rssi(void)
{
	if (rk_hal_ops->wifi_get_connected_ap_rssi)
		return rk_hal_ops->wifi_get_connected_ap_rssi();

	return 0;
}

//获取当前的COUNTRY CODE
int RK_wifi_get_ccode(void)
{
	if (rk_hal_ops->wifi_get_ccode)
		return rk_hal_ops->wifi_get_ccode();

	return 0;
}

//配置COUNTRY CODE
int RK_wifi_set_ccode(void)
{
	if (rk_hal_ops->wifi_set_ccode)
		return rk_hal_ops->wifi_set_ccode();

	return 0;
}

/*
 * 低功耗专用API
 */
//WiFi进入低功耗模式
int RK_wifi_enter_sleep(int argc, char *argv[])
{
	if (rk_hal_ops->wifi_enter_sleep)
		return rk_hal_ops->wifi_enter_sleep();

	return 0;
}

//WiFi退出低功耗模式
int RK_wifi_exit_sleep(void)
{
	if (rk_hal_ops->wifi_exit_sleep)
		return rk_hal_ops->wifi_exit_sleep();

	return 0;
}

//启动跟服务器的保活连接
int RK_wifi_start_keepalive(RK_WIFI_LOW_POWER_KEEPALIVE_s *cfgs)
{
	if (rk_hal_ops->wifi_start_keepalive)
		return rk_hal_ops->wifi_start_keepalive(cfgs);

	return 0;
}

//停止
int RK_wifi_stop_keepalive(int port)
{
	if (rk_hal_ops->wifi_stop_keepalive)
		return rk_hal_ops->wifi_stop_keepalive(port);

	return 0;
}

//配置WiFi的listenIerval
int RK_wifi_set_BeaconListenInterval(int dtim)
{
	if (rk_hal_ops->wifi_set_BeaconListenInterval)
		return rk_hal_ops->wifi_set_BeaconListenInterval(dtim);

	return 0;
}

//特殊:Rockchip配网demo使用
char *RK_wifi_scan_for_softap(void)
{
	if (rk_hal_ops->wifi_scan_for_softap)
		return rk_hal_ops->wifi_scan_for_softap();

	return 0;
}

int RK_wifi_set_pir(int enable)
{
	if (rk_hal_ops->wifi_set_pir)
		return rk_hal_ops->wifi_set_pir(enable);

	return false;
}

int RK_wifi_get_wkreason()
{
	if (rk_hal_ops->wifi_get_wkreason)
		return rk_hal_ops->wifi_get_wkreason();

	printf("not init hal ops !\n");

	return 0;
}

int RK_wifi_get_batlevel()
{
	if(rk_hal_ops->wifi_get_batlevel)
		return rk_hal_ops->wifi_get_batlevel();
	printf("not init hal ops !\n");

	return 0;
}

int RK_wifi_get_utc(unsigned long long *utc)
{
	if(rk_hal_ops->wifi_get_utc)
		return rk_hal_ops->wifi_get_utc(utc);
	printf("not init hal ops!\n");

	return 0;
}

int Rk_wifi_ota(char* path)
{
	printf("get in RK wifi ota\n");

	if (rk_hal_ops->wifi_ota)
		return rk_hal_ops->wifi_ota(path);
	else
		printf("not init wifi ota ops !\n");

	return 0;
}

int RK_wifi_reboot(void)
{
	if(rk_hal_ops->wifi_reboot)
		return rk_hal_ops->wifi_reboot();
	printf("not init hal ops!\n");

	return 0;
}

int RK_get_pirstate(unsigned int *pir_ret)
{
	if(rk_hal_ops->wifi_get_pir)
		return rk_hal_ops->wifi_get_pir(pir_ret);
	printf("not init hal ops!\n");

	return 0;
}
