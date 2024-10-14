#ifndef __RK_WIFI_H__
#define __RK_WIFI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* 编程规范
 * return 返回值：TRUE/FALSE -ERRCODE
 */

#define RK_WIFI_VERSION "V2.1"

#define RK_WIFI_SAVED_INFO_MAX		10
#define SSID_BUF_LEN				64
#define BSSID_BUF_LEN				20
#define STATE_BUF_LEN				20

#define RK_WIFI_ETHER_ADDR_LEN		6
#define RK_WIFI_BSSID_LEN			6
#define RK_WIFI_SSID_LENGTH_MAX 	64
#define RK_WIFI_IPV4_ADDR_LEN		4

//国家码
typedef enum RK_WIFI_COUNTRY_CODE {
	RK_WIFI_COUNTRY_CODE_CHINESE,
	RK_WIFI_COUNTRY_CODE_USA,
	RK_WIFI_COUNTRY_CODE_EUROPE,
	RK_WIFI_COUNTRY_CODE_JAPAN,
	RK_WIFI_COUNTRY_CODE_CANADA,
	RK_WIFI_COUNTRY_CODE_AUSTRALIA,
	RK_WIFI_COUNTRY_CODE_ISRAEL,
	RK_WIFI_COUNTRY_CODE_MEXICO,
	RK_WIFI_COUNTRY_CODE_FRANCE,
	RK_WIFI_COUNTRY_CODE_MAX,
} RK_WIFI_COUNTRY_CODE;

/* WIFI STATE   WIFI运行状态 */
typedef enum {
	RK_WIFI_State_IDLE = 0,
	/* 连接中 */
	RK_WIFI_State_CONNECTING,

	/* 连接失败*/
	RK_WIFI_State_CONNECTFAILED,

	/* 连接失败，扫描不到配置的AP*/
	RK_WIFI_State_CONNECTFAILED_NOT_FOUND,

	/* 连接失败，密码错误 */
	RK_WIFI_State_CONNECTFAILED_WRONG_KEY,

	/* 认证成功或失败，很少用到 */
	RK_WIFI_State_AUTH_SUCCESS,
	RK_WIFI_State_AUTH_FAILED,	

	/* 连接成功 */
	RK_WIFI_State_CONNECTED,

	/* WiFi断开 */
	RK_WIFI_State_DISCONNECTED,

	/* WiFi断开，应用层主动断开 */
	RK_WIFI_State_DISCONNECTED_BY_HOST,

	/* WiFi断开，被路由器踢掉 */
	RK_WIFI_State_DISCONNECTED_BY_ROUTER,

	/* WiFi断开，由于beacon丢失 */
	RK_WIFI_State_DISCONNECTED_LOST_BEACON,

	/* WiFi打开成功 */
	RK_WIFI_State_OPEN,

	/* WiFi打开失败 */
	RK_WIFI_State_OPEN_FAIL,

	/* WiFi关闭成功 */
	RK_WIFI_State_OFF,

	/* WiFi发起扫描 */
	RK_WIFI_State_SCAN,

	/* WiFi扫描完成 */
	RK_WIFI_State_SCAN_RESULTS,

	/* WiFi无法获取IP */
	RK_WIFI_State_DHCP_ERR,

	/* WiFi获取到IP */
	RK_WIFI_State_DHCP_OK,

	/* WiFi未配置 */
	RK_WIFI_State_NOT_CFG_NETWORK,

	/* 
	 * 低功耗专用，表示是低功耗WiFi发来的事件
	 * only for lower power wifi
	 */
	RK_WIFI_State_LP_EVENT_TYPE,

	/* WiFi未知错误 */
	RK_WIFI_State_UNKNOW_ERR,
} RK_WIFI_RUNNING_State_e;

/* 
 * RK_WIFI_State_LP_EVENT_TYPE
 * 低功耗专用，表示是低功耗WiFi发来的事件
 * 下面是具体事件
 * only for lower power wifi
 */
typedef enum {
	RK_WIFI_LP_LONG_PRESS,			//长按
	RK_WIFI_LP_SHORT_PRESS,			//短按
	RK_WIFI_LP_DOORBELL,			//门铃
	RK_WIFI_LP_KEEPALIVE_OK,		//保活成功
	RK_WIFI_LP_KEEPALIVE_FAILED,		//保活失败
	RK_WIFI_LP_BAT_LEVEL,			//电池电量
	RK_WIFI_LP_PIR_DETECT,			//PIR触发
	RK_WIFI_LP_WAKEUP_REASON,		//唤醒reason
	RK_WIFI_LP_ANTIDISASSEMBLY,		//防拆除
	RK_WIFI_LP_END,
} RK_WIFI_LP_Event_Type_e;

typedef enum {
	RK_WIFI_WAKEUP_IO,				// hardware io
	RK_WIFI_WAKEUP_NETWORK,			// keepalive wakeup netpattern
	RK_WIFI_WAKEUP_CONNECT,			// ap reconnect fail timeout
	RK_WIFI_WAKEUP_KEEPALIVE,		// keepalive resend timeout
	RK_WIFI_WAKEUP_TCP_NETPATTERN,	// tcp netpattern match
	RK_WIFI_WAKEUP_UDP_NETPATTERN,	// udp netpattern match
	RK_WIFI_WAKEUP_WK_SSID,			// wakeup ssid match
	RK_WIFI_WAKEUP_MAGIC_PKT,		// wakeup magic packet match
 	RK_WIFI_WAKEUP_DETECT_PIR,		// wakeup by pir detect
} RK_WIFI_LP_Wakeup_Reason_e;

/* 加密类型 */
typedef enum {
	NONE = 0,
	WPA,
	WEP,
	WPA3
} RK_WIFI_CONNECTION_Encryp_e;

/*
 * WiFi反馈的运行信息，一般通过两种方式获取
 * 1. 打开WiFi时注册的callback，被动反馈
 * 2. 通过RK_wifi_running_getConnectionInfo主动获取
 */
typedef struct {
	/* 配置WiFi的索引，一般只用于传统WiFi */
	int id;
	/* 连接到AP的MAC地址 */
	char bssid[BSSID_BUF_LEN];
	/* 连接到AP的名字 */
	char ssid[SSID_BUF_LEN];
	/* 连接到AP的频率 */
	int freq;
	char channel[6];
	/* AP/STA */
	char mode[20];
	/* 连接状态 */
	char wpa_state[20];

	/* 获取到的IP地址 */
	char ip_address[20];
	/* 获取到的网关 */
	char gw[20];
	/* 获取到的网关 */
	char dns[20];

	/* 本机的MAC地址 */
	char mac_address[20];

	/* WiFi断开原因，如果有 */
	int reason;

	/* WIFI固件版本信息 */
	char version[10];

	/* only for lower power wifi
	 * 低功耗WiFi的EVENT的具体类型
	 */
	RK_WIFI_LP_Event_Type_e lp_event_type;

	/* 具体类型的数据，比如电池电量等 */
	int lp_data[32];
} RK_WIFI_INFO_Connection_s;

typedef struct {
	RK_WIFI_INFO_Connection_s wifi_con_info;
	//下面只针对IPC低功耗WiFi
	RK_WIFI_RUNNING_State_e type;
	char data[64];
} RK_WIFI_INFO_s;

/* 配置的WiFi信息，一般只用于传统WiFi，传统WiFi可以配置多个AP */
typedef struct {
	int id;
	char bssid[BSSID_BUF_LEN];
	char ssid[SSID_BUF_LEN];
	char state[STATE_BUF_LEN];
} RK_WIFI_SAVED_INFO_s;

/* 跟服务器或云端保活的基本信息，仅用于IPC低功耗WiFi */
typedef struct {
	char domain[64];		//IP地址，如果是域名需解析成功具体的地址
	char port[4];			//远端端口号
	char device_id[128];	//设备ID
	char key[128];			//加密KEY
	char period[4];			//保活包交互间隔
	char wake_str[64];		//唤醒匹配的字符串
} RK_WIFI_LOW_POWER_KEEPALIVE_s;

/*
 * 注意：
 * return: 返回大于等于0成功，小于0失败
 */

/* 状态信息回调函数定义 */
typedef int(*RK_wifi_state_callback)(RK_WIFI_RUNNING_State_e state, RK_WIFI_INFO_Connection_s *info);

//注册状态回调
int RK_wifi_register_callback(RK_wifi_state_callback cb);

//获取当前连接信息
int RK_wifi_running_getConnectionInfo(RK_WIFI_INFO_Connection_s* pInfo);

//WiFi使能或失能，1 使能，0 失能
int RK_wifi_enable(int enable);

//扫描
int RK_wifi_scan(void);

//获取扫描结果，一般是再回调中得到RESULTS事件通知
char* RK_wifi_scan_r(void);

//发起连接
int RK_wifi_connect(char* ssid, const char* psk);

//连接WEP加密的WIFI
int RK_wifi_connect_wep(char* ssid, const char* psk);

//断开当前网络
int RK_wifi_disconnect_network(void);

//WiFi恢复之前的配置,或重连之前的配置
int RK_wifi_recovery(void);

//移除连接过的WiFi
int RK_wifi_forget_with_ssid(char *ssid);

//取消正在进行连网
int RK_wifi_cancel(void);

//获取配置过的WiFi信息
int RK_wifi_getSavedInfo(RK_WIFI_SAVED_INFO_s **pInfo, int *ap_cnt);

//连接之前连过的WiFi
int RK_wifi_connect_with_ssid(char* ssid);

//WiFi恢复出厂设置
int RK_wifi_reset(void);

//获取AP的信号强度
int RK_wifi_get_connected_ap_rssi(void);

//获取COUNTRY CODE
int RK_wifi_get_ccode(void);

//配置COUNTRY CODE
int RK_wifi_set_ccode(void);

//传输ota固件
int Rk_wifi_ota(char *path);

//wifi重启
int RK_wifi_reboot(void);

//获取平台PIR电平值：1为触发PIR，0为无触发
int RK_get_pirstate(unsigned int *pir_ret);

/*
 * 低功耗专用API
 */
//进入低功耗
int RK_wifi_enter_sleep(int argc, char *argv[]);
//退出低功耗
int RK_wifi_exit_sleep(void);
//启动保活
int RK_wifi_start_keepalive(RK_WIFI_LOW_POWER_KEEPALIVE_s *cfg);
//取消之前的保活
int RK_wifi_stop_keepalive(int port);
//配置路由器的本身的监听间隔
int RK_wifi_set_BeaconListenInterval(int dtim);

//特殊: Rockchip配网demo使用，目前仅用于传统WiFi
char *RK_wifi_scan_for_softap(void);
//获取电量，结果通过低功耗事件回调返回
int RK_wifi_get_batlevel();
//获取唤醒信息，结果通过低功耗事件回调返回
int RK_wifi_get_wkreason();
//同步阻塞获取utc时间
int RK_wifi_get_utc(unsigned long long *utc);

//配置PIR
int RK_wifi_set_pir(int enable);

#ifdef __cplusplus
}
#endif

#endif
