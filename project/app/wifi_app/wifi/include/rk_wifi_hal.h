#include "Rk_wifi.h"

struct Rk_wifi_driver_ops {
	const char *name;
	//注册状态回调
	int (*wifi_register_callback)(RK_wifi_state_callback cb);
	//获取当前WiFi运行状态
	int (*wifi_running_getState)(RK_WIFI_RUNNING_State_e* pState);
	//获取当前连接信息
	int (*wifi_running_getConnectionInfo)(RK_WIFI_INFO_Connection_s* pInfo);
	//打开WiFi
	int (*wifi_enable)(int enable);
	//扫描
	int (*wifi_scan)(void);
	//获取扫描结果
	char(*wifi_scan_r)(void);
	char(*wifi_scan_for_softap)(void);
	//发起连接
	int (*wifi_connect)(char* ssid, const char* psk);
	//连接WEP加密的WIFI
	int (*wifi_connect_wep)(char* ssid, const char* psk);
	//断开当前网络
	int (*wifi_disconnect_network)(void);
	//AP模块设置路由信息
	int (*wifi_set_hostname)(const char* name);
	int (*wifi_get_hostname)(char* name, int len);
	//获取本机WiFi的MAC地址
	int (*wifi_get_mac)(char *wifi_mac);
	int (*wifi_ping)(char *address);
	//WiFi恢复之前的配置,或重连之前的配置
	int (*wifi_recovery)(void);
	//移除连接过的WiFi
	int (*wifi_forget_with_ssid)(char *ssid);
	//取消配网
	int (*wifi_cancel)(void);
	//获取配置过的WiFi信息
	int (*wifi_getSavedInfo)(RK_WIFI_SAVED_INFO_s **pInfo, int *ap_cnt);
	//连接之前连过的WiFi
	int (*wifi_connect_with_ssid)(char* ssid);
	//WiFi恢复出厂设置
	int (*wifi_reset)(void);
	//获取AP的信号强度
	int (*wifi_get_connected_ap_rssi)(void);
	//获取COUNTRY CODE
	int (*wifi_get_ccode)(void);
	//配置COUNTRY CODE
	int (*wifi_set_ccode)(void);
	//wifi固件ota升级
	int (*wifi_ota)(char *path);

	/*
	 * 低功耗专用API
	 */
	int (*wifi_enter_sleep)(void);
	int (*wifi_exit_sleep)(void);
	int (*wifi_start_keepalive)(RK_WIFI_LOW_POWER_KEEPALIVE_s *cfgs);
	int (*wifi_stop_keepalive)(int port);
	int (*wifi_set_BeaconListenInterval)(int dtim);
	int (*wifi_set_pir)(int enable);
};

extern struct Rk_wifi_driver_ops hisi_driver_ops;
extern struct Rk_wifi_driver_ops atbm_driver_ops;
extern struct Rk_wifi_driver_ops wpa_driver_ops;
extern RK_wifi_state_callback m_wifi_cb;
