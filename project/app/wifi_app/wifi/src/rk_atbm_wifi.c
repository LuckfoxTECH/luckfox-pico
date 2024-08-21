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
#include "tools.h"
#include "slog.h"

//获取当前连接信息
int atbm_wifi_running_getConnectionInfo(RK_WIFI_INFO_Connection_s *pInfo)
{
	int ret = atbm_get_status(pInfo);

	if (ret < 0)
		ret = false;
	else
		ret = true;

	return ret;
}

//打开WiFi
int atbm_wifi_enable(int enable)
{
	int ret;

	if (enable)
		ret = atbm_init();
	else
		ret = atbm_deinit();

	if (ret < 0)
		ret = false;
	else
		ret = true;

	return ret;
}

//扫描
int atbm_wifi_scan(void)
{
	int ret = atbm_scan();

	if (ret < 0)
		ret = false;
	else
		ret = true;

	return ret;
}

//获取扫描结果
char *atbm_wifi_scan_r(void)
{
	atbm_scan_results();

	return NULL;
}

//发起连接
int atbm_wifi_connect(char *ssid, const char *psk)
{
	int ret = atbm_enable_network(ssid, psk);

	if (ret < 0)
		ret = false;
	else
		ret = true;

	return ret;
}

//连接WEP加密的WIFI
int atbm_wifi_connect_wep(char* ssid, const char* psk)
{
	int ret;

	if (ret < 0)
		ret = false;
	else
		ret = true;

	return ret;
}

//断开当前网络
int atbm_wifi_disconnect_network(void)
{
	int ret = atbm_clear_wifi_cfg();

	if (ret < 0)
		ret = false;
	else
		ret = true;

	return ret;
}

//获取本机WiFi的MAC地址
int atbm_wifi_get_mac(char *wifi_mac)
{
	RK_WIFI_INFO_Connection_s pInfo;

	atbm_get_status(&pInfo);
	strcpy(wifi_mac, &pInfo.mac_address);

	return true;
}

//WiFi恢复之前的配置,或重连之前的配置
int atbm_wifi_recovery(void)
{
	int ret = atbm_clear_wifi_cfg();

	if (ret < 0)
		ret = false;
	else
		ret = true;

	return ret;
}

//取消配网
int atbm_wifi_cancel(void)
{
	return atbm_wifi_recovery();
}

//获取配置过的WiFi信息
int atbm_wifi_getSavedInfo(RK_WIFI_SAVED_INFO_s **pInfo, int *ap_cnt)
{
	int ret;

	if (ret < 0)
		ret = false;
	else
		ret = true;

	return ret;
}

//WiFi恢复出厂设置
int atbm_wifi_reset(void)
{
	int ret;

	ret = fw_at("AT+FLASH_CONFIG_RESET");
	pr_info("fw_at AT+FLASH_CONFIG_RESET ret is %d\n", ret);

	if (ret < 0)
		ret = false;
	else
		ret = true;

	return ret;
}

//获取AP的信号强度
int atbm_wifi_get_connected_ap_rssi(void)
{
	int ret;

	ret = atbm_get_rssi_cmd();

	if (ret < 0)
		ret = false;
	else
		ret = true;

	return ret;
}

//获取COUNTRY CODE
int atbm_wifi_get_ccode(void)
{
	int ret;

	if (ret < 0)
		ret = false;
	else
		ret = true;

	return ret;
}

//配置COUNTRY CODE
int atbm_wifi_set_ccode(void)
{
	int ret;

	if (ret < 0)
		ret = false;
	else
		ret = true;

	return ret;
}

/*
 * 低功耗专用API
 */
int atbm_wifi_enter_sleep(void)
{
	int ret = 0;

	ret = fw_at("AT+LIGHT_SLEEP 1");
	pr_info("fw_at AT+LIGHT_SLEEP ret is %d\n", ret);
	sleep(1);
	atbm_driver_go_sleep();
	pr_info("driver_go_sleep\n");

	if (ret < 0)
		ret = false;
	else
		ret = true;

	return ret;
}

int atbm_wifi_exit_sleep(void)
{
	int ret;

	ret = fw_at("AT+LIGHT_SLEEP 0");
	pr_info("fw_at AT+LIGHT_SLEEP ret is %d\n", ret);

	if (ret < 0)
		ret = false;
	else
		ret = true;

	return ret;
}

int atbm_wifi_start_keepalive(RK_WIFI_LOW_POWER_KEEPALIVE_s *cfg)
{
	int ret = 0;
	char buf_at_cmd[1024] = {0};
	char buf_server_ip[32] = {'\0'};

	ret = fw_at("AT+WIFI_LISTEN_ITVL 10");
	pr_info("fw_at AT+WIFI_LISTEN_ITVL 10 ret is %d\n", ret);

	snprintf(buf_at_cmd, sizeof(buf_at_cmd),
			 "AT+WIFI_HEART_PKT TEXT \"keepalive\" DEVID \"%s\" KEY \"%s\" PERIOD %s SERVER %s PORT %s",
			 cfg->device_id,
			 cfg->key,
			 cfg->period,
			 cfg->domain,
			 cfg->port);

	pr_info("buf_at_cmd:%s\n", buf_at_cmd);

	/* WiFi端需要处理休眠服务器的信息 */
	ret = atbm_set_port_filter(cfg->port, 0);
	pr_info("do_set_port_filter: %d\n", ret);

	/* 发送保活服务器信息给WiFi端 */
	ret = fw_at(buf_at_cmd);
	pr_info("do_at_cmd_send_ret: %d\n", ret);

	if (ret < 0)
		ret = false;
	else
		ret = true;

	return ret;
}

int atbm_wifi_stop_keepalive(int port)
{
	int ret = 0;

	ret = fw_at("AT+WIFI_LISTEN_ITVL 1");
	pr_info("fw_at AT+WIFI_LISTEN_ITVL1 ret is %d\n", ret);

	usleep(500 * 1000);

	ret = fw_at("AT+LIGHT_SLEEP 0");
	pr_info("fw_at AT+LIGHT_SLEEP ret is %d\n", ret);

	usleep(500 * 1000);

	ret = fw_at("AT+DEL_KEEPALIVE");
	pr_info("fw_at AT+DEL_KEEPALIVE ret is %d\n", ret);

	usleep(500 * 1000);

	/* WiFi端不需要处理休眠服务器的信息 */
	ret = atbm_set_port_filter(port, 1);
	pr_info("do_set_port_filter: %d\n", ret);

	if (ret < 0)
		ret = false;
	else
		ret = true;

	return ret;
}

int atbm_wifi_set_BeaconListenInterval(int dtim)
{
	int ret;

	ret = fw_at("AT+WIFI_LISTEN_ITVL 10");
	pr_info("fw_at AT+WIFI_LISTEN_ITVL ret is %d\n", ret);

	if (ret < 0)
		ret = false;
	else
		ret = true;

	return ret;
}

int atbm_wifi_ota(char *fw)
{
	int ret = wifi_fw_ota(fw);

	if (ret < 0)
		ret = false;
	else
		ret = true;

	return ret;
}

int atbm_wifi_get_batlevel()
{
	int ret = fw_at("AT+BAT_STATE");

	if (ret < 0)
		ret = false;
	else
		ret = true;

	return ret;
}

int atbm_wifi_pir_set(int enable)
{
	int ret;
	if (enable == 1)
		ret = fw_at("AT+PIR_ENABLE");
	else
		ret = fw_at("AT+PIR_DISABLE");

	if (ret < 0)
		ret = false;
	else
		ret = true;

	return ret;
}

int atbm_get_utc(unsigned long long *ret)
{
	return wifi_get_utctime(ret);
}

struct Rk_wifi_driver_ops atbm_driver_ops = {
	.name = "atbm iot",
	.wifi_enable = atbm_wifi_enable,
	.wifi_disconnect_network = atbm_wifi_disconnect_network,
	.wifi_connect = atbm_wifi_connect,
	.wifi_running_getConnectionInfo = atbm_wifi_running_getConnectionInfo,
	.wifi_cancel = atbm_wifi_cancel,
	.wifi_connect_wep = atbm_wifi_connect_wep,
	.wifi_recovery = atbm_wifi_recovery,
	.wifi_getSavedInfo = atbm_wifi_getSavedInfo,
	.wifi_reset = atbm_wifi_reset,
	.wifi_scan = atbm_wifi_scan,
	.wifi_scan_r = atbm_wifi_scan_r,
	.wifi_start_keepalive = atbm_wifi_start_keepalive,
	.wifi_stop_keepalive = atbm_wifi_stop_keepalive,
	.wifi_set_BeaconListenInterval = atbm_wifi_set_BeaconListenInterval,
	.wifi_ota = atbm_wifi_ota,
	.wifi_get_batlevel = atbm_wifi_get_batlevel,
	.wifi_set_pir = atbm_wifi_pir_set,
	.wifi_get_utc = atbm_get_utc,
};
