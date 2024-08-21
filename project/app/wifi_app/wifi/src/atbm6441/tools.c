// 高风险文件，出现相关问题需第一点考虑排查以下警告
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-sign"
#pragma GCC diagnostic ignored "-Wformat-overflow="
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#pragma GCC diagnostic ignored "-Wunused-const-variable"
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wtype-limits"

#include <arpa/inet.h>
#include <asm/ioctl.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#include "Rk_wifi.h"

#include "atbm_ioctl_ext.h"
#include "atbm_tool.h"
#include "tools.h"
#include "rk_wifi_hal.h"
#include "Rk_wifi.h"

#if defined(SUPPORT_EASY_ATBM)
#include "tools.h"
#endif

#define ATBM_POWER 122
#define ATBM_POWER_SET _IOW(ATBM_POWER, 0, char)

#if 1
#define DEBUG_PRINTF(fmt, args...) printf("[%s|%d]:" fmt, __FUNCTION__, __LINE__, ##args)
#else
// #define DEBUG_PRINTF(fmt, args...) printf(fmt, ##args)
#define DEBUG_PRINTF(fmt, args...)
#endif

#define ATBM_RESET_KEY_LONG_PRESS_EVENT "key press with long time"
#define ATBM_RESET_KEY_SHORT_PRESS_EVENT "key press with short time"
#define ATBM_START_KEEPALIVE_SUCCESS_EVENT "keepalive ok"
#define ATBM_START_KEEPALIVE_FAILED_EVENT "keepalive fail"
#define ATBM_BATTERY_LEVEL_EVENT "Per bat stat is "
#define ATBM_WAKEUP_REASON_EVENT "wake up reason is "
#define ATBM_PIR_DETECT_EVENT "after resume,pir detect cnt is "
#define ATBM_UTC_ERROR "sntp server error,pls retry later"
#define ATBM_UTC_SUCCESS "utc time is "

static key_long_press_event_func key_long_press_func_cb = NULL;                 /* 复位键长按事件处理函数 */
static key_short_press_event_func key_short_press_func_cb = NULL;               /* 门铃事件处理函数 */
static wakeup_event_func wakeup_event_func_cb = NULL;                           /* 唤醒原因事件处理函数 */
static battery_level_event_func battery_level_event_func_cb = NULL;			        /* 电池电量事件处理函数 */
static pir_detect_event_func pir_detect_event_func_cb = NULL;			              /* pir事件处理函数 */
static wifi_connect_result_event_func wifi_connect_result_event_func_cb = NULL;	/* WiFi连接结果事件处理函数 */

static struct ap_cfg ap_cfg_set;
static struct etf_cfg etf_cfg_set;
static struct powersave_mode power_save_set;
static struct tcp_filter_info tcp_filter_set;
static struct ipc_data_info ipc_data_set;
static struct fast_cfg_recv_info fast_cfg_recv_set;
static struct fast_cfg_send_info fast_cfg_send_set;

static char server_ipaddr[16];

static struct status_async status;

static struct connect_info networkinfo;
//保存WiFi全局状态
static RK_WIFI_INFO_Connection_s g_wifi_Info;


static struct transform ps_trans[PS_TYPE_MAX] = {
    {"NO_SLEEP", PS_TYPE_NO_SLEEP},
    {"MODEM_SLEEP", PS_TYPE_MODEM_SLEEP},
    {"LIGHT_SLEEP", PS_TYPE_LIGHT_SLEEP},
    {"DEEP_SLEEP", PS_TYPE_DEEP_SLEEP}};

static struct transform key_mgmt_trans[KEY_MGMT_MAX] = {
    {"NONE", KEY_MGMT_NONE},
    {"WEP", KEY_MGMT_WEP},
    {"WEP_SHARED", KEY_MGMT_WEP_SHARED},
    {"WPA", KEY_MGMT_WPA},
    {"WPA2", KEY_MGMT_WPA2}};

static struct transform country_trans[COUNTRY_MAX] = {
    {"CHINESE", COUNTRY_CHINESE}, {"USA", COUNTRY_USA},
    {"EUROPE", COUNTRY_EUROPE},   {"JAPAN", COUNTRY_JAPAN},
    {"CANADA", COUNTRY_CANADA},   {"AUSTRALIA", COUNTRY_AUSTRALIA},
    {"ISRAEL", COUNTRY_ISRAEL},   {"MEXICO", COUNTRY_MEXICO},
    {"FRANCE", COUNTRY_FRANCE}};

static struct transform rate_trans[RATE_INDEX_MAX] = {
    {"1M", RATE_INDEX_B_1M},       {"2M", RATE_INDEX_B_2M},
    {"5.5M", RATE_INDEX_B_5_5M},   {"11M", RATE_INDEX_B_11M},
    {"22M", RATE_INDEX_PBCC_22M},  {"33M", RATE_INDEX_PBCC_33M},
    {"6M", RATE_INDEX_A_6M},       {"9M", RATE_INDEX_A_9M},
    {"12M", RATE_INDEX_A_12M},     {"18M", RATE_INDEX_A_18M},
    {"24M", RATE_INDEX_A_24M},     {"36M", RATE_INDEX_A_36M},
    {"48M", RATE_INDEX_A_48M},     {"54M", RATE_INDEX_A_54M},
    {"6.5M", RATE_INDEX_N_6_5M},   {"13M", RATE_INDEX_N_13M},
    {"19.5M", RATE_INDEX_N_19_5M}, {"26M", RATE_INDEX_N_26M},
    {"39M", RATE_INDEX_N_39M},     {"52M", RATE_INDEX_N_52M},
    {"58.5M", RATE_INDEX_N_58_5M}, {"65M", RATE_INDEX_N_65M},
    {"6M", RATE_INDEX_N_MCS32_6M}};

static int rate_set[20] = {10,  20,  55, 110, 60,  90,  120, 180, 240, 360,
                           480, 540, 65, 130, 195, 260, 390, 520, 585, 650};

static int is_insmod_driver = 0;
static int g_fp = 0;
static sem_t sem;
static sem_t sem_status;
static sem_t sem_sock_sync;
static int thread_run = 0;
static int ip_set_auto = 1;
static int wifi_mode = 0; // STA=0 IP=1
static char ssid_set = 0;
static char key_set = 0;
static char key_id_set = 0;
static char key_mgmt_set = 0;
static char ap_ssid_set = 0;
static char ap_key_set = 0;
static char ap_key_id_set = 0;
static char ap_key_mgmt_set = 0;
static char driver_can_rmmod = 0;
static char rmmod_auto = 1;
static char driver_switch = 0;
static char ipc_server_set = 0;
static char ipc_port_set = 0;

char cmd_line[CMD_LINE_LEN];

pthread_cond_t atbm_sync_cond;
pthread_mutex_t atbm_sync_lock;
int atbm_sync_flag = 0;
long long utctime = 0;

void atbm_reboot_handle(int sig_num) {
  int ret;
  ret = fw_at("AT+LIGHT_SLEEP 1");
  if (ret != 0) {
    DEBUG_PRINTF("error setting AT+LIGHT_SLEEP 1,ret is %d !", ret);
  }
  exit(0);
}

void MAC_printf(char mac[6]) {
  DEBUG_PRINTF("%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3],
               mac[4], mac[5]);
}

int powersave_transform(char *ps_type) {
  if (NULL != ps_type) {
    int i = 0;
    for (i = 0; i < PS_TYPE_MAX; i++) {
      if (!strcmp(ps_trans[i].type, ps_type)) {
        return ps_trans[i].type_value;
      }
    }
  }

  DEBUG_PRINTF("Line:%d fail\n", __LINE__);
  return -1;
}

char *key_mgmt_transform2str(int key_mgmt) {
  int i = 0;

  for (i = 0; i < KEY_MGMT_MAX; i++) {
    if (key_mgmt == key_mgmt_trans[i].type_value) {
      return key_mgmt_trans[i].type;
    }
  }

  DEBUG_PRINTF("Line:%d fail\n", __LINE__);
  return NULL;
}

int key_mgmt_transform2int(char *key_type) {
  int i = 0;

  for (i = 0; i < KEY_MGMT_MAX; i++) {
    if (!strcmp(key_type, key_mgmt_trans[i].type)) {
      return key_mgmt_trans[i].type_value;
    }
  }

  DEBUG_PRINTF("Line:%d fail\n", __LINE__);
  return -1;
}

char *country_transform2str(int country_id) {
  int i = 0;

  for (i = 0; i < COUNTRY_MAX; i++) {
    if (country_id == country_trans[i].type_value) {
      return country_trans[i].type;
    }
  }

  DEBUG_PRINTF("Line:%d fail\n", __LINE__);
  return NULL;
}

int country_transform2int(char *country) {
  int i = 0;

  for (i = 0; i < COUNTRY_MAX; i++) {
    if (!strcmp(country, country_trans[i].type)) {
      return country_trans[i].type_value;
    }
  }

  DEBUG_PRINTF("Line:%d fail\n", __LINE__);
  return -1;
}

char *rate_transform2str(char rate) {
  int i = 0;

  for (i = 0; i < RATE_INDEX_MAX; i++) {
    if (rate == rate_trans[i].type_value) {
      return rate_trans[i].type;
    }
  }

  DEBUG_PRINTF("Line:%d fail\n", __LINE__);
  return NULL;
}

int wifi_param_check(struct connect_info *info, check_type type, char is_ap) {
  if (type >= CHECK_MAX) {
    DEBUG_PRINTF("Invalid param check type\n");
    goto err;
  }

  if (info == NULL) {
    DEBUG_PRINTF("param info is null\n");
    goto err;
  }

  switch (type) {
  case CHECK_SSID:
    if (info->ssidLength < 1 || info->ssidLength > 32) {
      DEBUG_PRINTF("Line:%d ssid len %d is invalid\n", __LINE__,
                   info->ssidLength);
      goto err;
    }
    break;
  case CHECK_KEY:
    if ((!is_ap && key_mgmt_set) || (is_ap && ap_key_mgmt_set)) {
      if ((info->keyMgmt == KEY_MGMT_WPA || info->keyMgmt == KEY_MGMT_WPA2) &&
          (info->keyLength < 8 || info->keyLength > 64)) {
        DEBUG_PRINTF("Line:%d key len %d is invalid, not match WPA or WPA2\n",
                     __LINE__, info->keyLength);
        goto err;
      } else if ((info->keyMgmt == KEY_MGMT_WEP ||
                  info->keyMgmt == KEY_MGMT_WEP_SHARED) &&
                 (info->keyLength != 5 && info->keyLength != 13 &&
                  info->keyLength != 10 && info->keyLength != 26)) {
        DEBUG_PRINTF("Line:%d key len %d is invalid, should be 5 or 13\n",
                     __LINE__, info->keyLength);
        goto err;
      }
    }
    break;
  case CHECK_KEY_ID:
    if ((!is_ap && key_mgmt_set) || (is_ap && ap_key_mgmt_set)) {
      if ((info->keyMgmt == KEY_MGMT_WEP ||
           info->keyMgmt == KEY_MGMT_WEP_SHARED) &&
          (info->keyId > 3)) {
        DEBUG_PRINTF("Line:%d key id %d err\n", __LINE__, info->keyId);
        goto err;
      }
    }
    break;
  case CHECK_KEY_MGMT:
    if ((!is_ap && key_id_set) || (is_ap && ap_key_id_set)) {
      if ((info->keyMgmt == KEY_MGMT_WEP ||
           info->keyMgmt == KEY_MGMT_WEP_SHARED) &&
          (info->keyId > 3)) {
        DEBUG_PRINTF("Line:%d key id %d not match key mgmt %s\n", __LINE__,
                     info->keyId, key_mgmt_transform2str(info->keyMgmt));
        goto err;
      }
    }
    if ((!is_ap && key_set) || (is_ap && ap_key_set)) {
      if ((info->keyMgmt == KEY_MGMT_WPA || info->keyMgmt == KEY_MGMT_WPA2) &&
          (info->keyLength < 8 || info->keyLength > 64)) {
        DEBUG_PRINTF("Line:%d key len %d not match %s\n", __LINE__,
                     info->keyLength, key_mgmt_transform2str(info->keyMgmt));
        goto err;
      } else if ((info->keyMgmt == KEY_MGMT_WEP ||
                  info->keyMgmt == KEY_MGMT_WEP_SHARED) &&
                 (info->keyLength != 5 && info->keyLength != 13 &&
                  info->keyLength != 10 && info->keyLength != 26)) {
        DEBUG_PRINTF("Line:%d key len %d not match %s\n", __LINE__,
                     info->keyLength, key_mgmt_transform2str(info->keyMgmt));
        goto err;
      }
    }
    break;
  default:
    DEBUG_PRINTF("Invalid param check type\n");
    goto err;
    break;
  }

  return 0;
err:
  return -1;
}
int set_ps_cmd(int fp, int argc, char *argv[]) {
  if (0 == argc) {
    DEBUG_PRINTF("set_ps variables:\n"
                 "  ps_level (power save level, refer: 0/1/2\n"
                 "  ps_type (power save mode, refer: "
                 "NO_SLEEP/MODEM_SLEEP/LIGHT_SLEEP/DEEP_SLEEP)\n");
    return 0;
  }
  if (argc < 4 || argc % 2) {
    DEBUG_PRINTF("Invalid SET_POWERSAVE command: needs at least 4 arguments "
                 "(ps_level<A1><A2>ps_type "
                 "set together), and arguments must be even number\n");
    goto err;
  } else {
    int arg_cnt = 0;
    int level_set = 0;
    int type_set = 0;
    int ret = 0;

    while (arg_cnt < argc) {
      if (!strcmp(argv[arg_cnt], "ps_level")) {
        arg_cnt++;
        int level = atoi(argv[arg_cnt]);
        if (level > 2 || level < 0) {
          DEBUG_PRINTF("Line:%d invalid ps_level %d\n", __LINE__, level);
          DEBUG_PRINTF("Line:%d no ps_level values configured.\n", __LINE__);
          goto err;
        } else {
          power_save_set.powerave_level = level;
          level_set = 1;
        }
      } else if (!strcmp(argv[arg_cnt], "ps_type")) {
        arg_cnt++;
        int ps_type = 0;
        ps_type = powersave_transform(argv[arg_cnt]);
        if (-1 == ps_type) {
          DEBUG_PRINTF("Line:%d invalid ps_type %s\n", __LINE__, argv[arg_cnt]);
          DEBUG_PRINTF("Line:%d no ps_type values configured.\n", __LINE__);
          goto err;
        } else {
          power_save_set.powersave_mode = ps_type;
          type_set = 1;
        }
      } else {
        DEBUG_PRINTF("Line:%d invalid command %s\n", __LINE__, argv[arg_cnt]);
        goto err;
      }

      arg_cnt++;
    }

    if (!level_set || !type_set) {
      DEBUG_PRINTF("Line:%d not set ps_level or ps_type\n", __LINE__);
      DEBUG_PRINTF("Line:%d no values configured.\n", __LINE__);
      goto err;
    }

    ret = ioctl(fp, ATBM_PS_SET, (unsigned int)(&power_save_set));
    if (ret) {
      goto err;
    }
  }

  return 0;
err:
  return -1;
}

int list_networks_cmd(int fp, int argc, char *argv[]) {
  DEBUG_PRINTF("ssid    /   key   / flags\n");
  DEBUG_PRINTF("%s\t%s", networkinfo.ssid, networkinfo.key);
  DEBUG_PRINTF("\t%s\n", key_mgmt_transform2str(networkinfo.keyMgmt));
  return 0;
}

int set_network_cmd(int fp, int argc, char *argv[]) {
  if (0 == argc) {
    DEBUG_PRINTF("set_network variables:\n"
                 "  ssid (network name, SSID)\n"
                 "  key (passphrase or password)\n"
                 "  key_mgmt (key management protocol, refer: "
                 "NONE/WEP/WEP_SHARED/WPA/WPA2)\n"
                 "  key_id (key identity)\n");
    return 0;
  }
  if (argc < 2) {
    DEBUG_PRINTF("Invalid SET_NETWORK command: needs at least 2 argument\n");
    goto err;
  } else {
    struct connect_info info_tmp;
    memcpy(&info_tmp, &networkinfo, sizeof(networkinfo));

    if (!strcmp(argv[0], "ssid")) {
      strcpy(info_tmp.ssid, argv[1]);
      info_tmp.ssidLength = strlen(argv[1]);
      if (wifi_param_check(&info_tmp, CHECK_SSID, 0)) {
        goto err;
      }
      memcpy(&networkinfo, &info_tmp, sizeof(networkinfo));
      ssid_set = 1;
    } else if (!strcmp(argv[0], "key_mgmt")) {
      int key_type = key_mgmt_transform2int(argv[1]);
      if (key_type < 0) {
        DEBUG_PRINTF("Line:%d invalid key_mgmt %s\n", __LINE__, argv[1]);
        DEBUG_PRINTF("Line:%d no new key_mgmt values configured.\n", __LINE__);
        goto err;
      }

      info_tmp.keyMgmt = key_type;
      if (wifi_param_check(&info_tmp, CHECK_KEY_MGMT, 0)) {
        goto err;
      }
      memcpy(&networkinfo, &info_tmp, sizeof(networkinfo));
      key_mgmt_set = 1;
    } else if (!strcmp(argv[0], "key_id")) {
      info_tmp.keyId = atoi(argv[1]);
      if (wifi_param_check(&info_tmp, CHECK_KEY_ID, 0)) {
        goto err;
      }
      memcpy(&networkinfo, &info_tmp, sizeof(networkinfo));
      key_id_set = 1;
    } else if (!strcmp(argv[0], "key")) {
      strcpy(info_tmp.key, argv[1]);
      info_tmp.keyLength = strlen(argv[1]);
      if (wifi_param_check(&info_tmp, CHECK_KEY, 0)) {
        goto err;
      }
      memcpy(&networkinfo, &info_tmp, sizeof(networkinfo));
      key_set = 1;
    } else {
      DEBUG_PRINTF("Line:%d invalid command %s\n", __LINE__, argv[0]);
      goto err;
    }
  }

  return 0;
err:
  return -1;
}

int atbm_enable_network(char *ssid, char *key)
{
  if (wifi_mode) {
    DEBUG_PRINTF("please switch wifi mode to STA.\n");
    goto err;
  } else if (ssid != NULL) {
    memset(&networkinfo, 0, sizeof(networkinfo));
    strcpy(networkinfo.ssid, ssid);
    networkinfo.ssidLength = strlen(ssid);
    strcpy(networkinfo.key, key);
    networkinfo.keyLength = strlen(key);
    networkinfo.keyMgmt = KEY_MGMT_WPA2;
    DEBUG_PRINTF("ssid: %s, key: %s.\n", networkinfo.ssid, networkinfo.key);
    memset(&status, 0, sizeof(status));
    if (0 != ioctl(g_fp, ATBM_CONNECT, (unsigned int)(&networkinfo))) {
      DEBUG_PRINTF("after ioctl\n\n");
      goto err;
    }
  }

  DEBUG_PRINTF("return 0\n\n");

  return 0;
err:
  DEBUG_PRINTF("return 1\n\n");
  return -1;
}

int enable_network_cmd(int fp, int argc, char *argv[]) {
  if (wifi_mode) {
    DEBUG_PRINTF("please switch wifi mode to STA.\n");
    goto err;
  } else if (argv[0]) {
    strcpy(networkinfo.ssid, argv[0]);
    networkinfo.ssidLength = strlen(argv[0]);
    strcpy(networkinfo.key, argv[1]);
    networkinfo.keyLength = strlen(argv[1]);
    networkinfo.keyMgmt = KEY_MGMT_WPA2;
    DEBUG_PRINTF("ssid: %s, key: %s.\n", networkinfo.ssid, networkinfo.key);
    memset(&status, 0, sizeof(status));
    if (0 != ioctl(g_fp, ATBM_CONNECT, (unsigned int)(&networkinfo))) {
      goto err;
    }
  } else {
    memset(&status, 0, sizeof(status));
    if (0 != ioctl(g_fp, ATBM_CONNECT, (unsigned int)(&networkinfo))) {
      goto err;
    }
  }

  return 0;
err:
  return -1;
}

int clear_network_cmd(int fp, int argc, char *argv[]) {
  memset(&networkinfo, 0, sizeof(networkinfo));
  ssid_set = 0;
  key_set = 0;
  key_id_set = 0;
  key_mgmt_set = 0;
  return 0;
}

int atbm_get_status(RK_WIFI_INFO_Connection_s *pInfo)
{
	int ret = 0;
	struct status_info status_info;
	struct in_addr addr;

	memset(&status_info, 0, sizeof(struct status_info));
	memset(pInfo, 0, sizeof(RK_WIFI_INFO_Connection_s));
	ret = ioctl(g_fp, ATBM_STATUS, (unsigned int)(&status_info));
	if (ret) {
		goto err;
	}

	wifi_mode = status_info.wifimode;
	DEBUG_PRINTF("[WIFI_MODE] %s\n", wifi_mode ? "AP" : "STA");
	if (status_info.wifimode) {
		DEBUG_PRINTF("please switch wifi mode to STA.\n");
		goto err;
	}

	if (status_info.bconnect) {
		//wpa_state
		if (pInfo)
			memcpy(pInfo->wpa_state, "CONNECTED", strlen("CONNECTED"));
		DEBUG_PRINTF("wpa_state=%s\n", pInfo->wpa_state);

		//ssid
		DEBUG_PRINTF("ssid=%s\n", status_info.con_event.ssid);
		if (pInfo)
			memcpy(pInfo->ssid, status_info.con_event.ssid, strlen(status_info.con_event.ssid));

		//ipaddr
		addr.s_addr = status_info.con_event.ipaddr;
		DEBUG_PRINTF("ip_address=%s\n", inet_ntoa(addr));
		if (pInfo)
			memcpy(pInfo->ip_address, inet_ntoa(addr), strlen(inet_ntoa(addr)));

		//bssid
		if (pInfo)
			sprintf(pInfo->bssid, "%02x:%02x:%02x:%02x:%02x:%02x",
				status_info.con_event.bssid[0],
				status_info.con_event.bssid[1],
				status_info.con_event.bssid[2],
				status_info.con_event.bssid[3],
				status_info.con_event.bssid[4],
				status_info.con_event.bssid[5]);
		DEBUG_PRINTF("ap_address=%s\n", pInfo->bssid);

		//ipmask
		addr.s_addr = status_info.con_event.ipmask;
		DEBUG_PRINTF("ip_mask=%s\n", inet_ntoa(addr));

		//dns
		addr.s_addr = status_info.con_event.dnsaddr[0];
		DEBUG_PRINTF("dns=%s\n", inet_ntoa(addr));
		if (pInfo) {
			memcpy(pInfo->dns, inet_ntoa(addr), strlen(inet_ntoa(addr)));
		}

		//gw
		addr.s_addr = status_info.con_event.gwaddr;
		DEBUG_PRINTF("gate_way=%s\n", inet_ntoa(addr));
		if (pInfo) {
			memcpy(pInfo->gw, inet_ntoa(addr), strlen(inet_ntoa(addr)));
		}
	} else {
		if (pInfo)
			memcpy(pInfo->wpa_state, "DISCONNECTED", strlen("DISCONNECTED"));
		DEBUG_PRINTF("wpa_state=INACTIVE\n");
	}

	return 1;
err:
	return -1;
}

static int atbm_init_status(void)
{
	int ret = 0;
	struct status_info status_info;
	struct in_addr addr;

	memset(&status_info, 0, sizeof(struct status_info));
	ret = ioctl(g_fp, ATBM_STATUS, (unsigned int)(&status_info));
	if (ret) {
		goto err;
	}

	wifi_mode = status_info.wifimode;
	DEBUG_PRINTF("[WIFI_MODE] %s\n", wifi_mode ? "AP" : "STA");
	if (status_info.wifimode) {
		DEBUG_PRINTF("please switch wifi mode to STA.\n");
		goto err;
	}

	if (status_info.bconnect) {
		memset(&g_wifi_Info, 0, sizeof(RK_WIFI_INFO_Connection_s));
		//wpa_state
		memcpy(g_wifi_Info.wpa_state, "CONNECTED", strlen("CONNECTED"));

		//ssid
		DEBUG_PRINTF("wpa_state=%s\n", g_wifi_Info.wpa_state);
		DEBUG_PRINTF("ssid=%s\n", status_info.con_event.ssid);
		memcpy(g_wifi_Info.ssid, status_info.con_event.ssid, strlen(status_info.con_event.ssid));

		//ipaddr
		addr.s_addr = status_info.con_event.ipaddr;
		DEBUG_PRINTF("ip_address=%s\n", inet_ntoa(addr));
		memcpy(g_wifi_Info.ip_address, inet_ntoa(addr), strlen(inet_ntoa(addr)));

		//bssid
		if (status_info.wifimode) {
			MAC_printf(status_info.macaddr);
		} else {
			MAC_printf(status_info.con_event.bssid);
		}
		sprintf(g_wifi_Info.bssid, "%02x:%02x:%02x:%02x:%02x:%02x",
			status_info.con_event.bssid[0],
			status_info.con_event.bssid[1],
			status_info.con_event.bssid[2],
			status_info.con_event.bssid[3],
			status_info.con_event.bssid[4],
			status_info.con_event.bssid[5]);
		DEBUG_PRINTF("ap_address=%s\n", g_wifi_Info.bssid);

		//gw_dns
		addr.s_addr = status_info.con_event.ipmask;
		DEBUG_PRINTF("\nip_mask=%s\n", inet_ntoa(addr));
		addr.s_addr = status_info.con_event.gwaddr;
		DEBUG_PRINTF("gate_way=%s\n", inet_ntoa(addr));
		memcpy(g_wifi_Info.gw, inet_ntoa(addr), strlen(inet_ntoa(addr)));
		addr.s_addr = status_info.con_event.dnsaddr[0];
		DEBUG_PRINTF("dns=%s\n", inet_ntoa(addr));
		memcpy(g_wifi_Info.dns, inet_ntoa(addr), strlen(inet_ntoa(addr)));

		//INIT STATE
		char cmd[64];
		struct in_addr ip_addr;
		ip_addr.s_addr = status_info.con_event.ipaddr;
		sprintf(cmd, "ifconfig wlan0 %s", inet_ntoa(ip_addr));
		DEBUG_PRINTF("cmd:%s\n", cmd);
		system(cmd);

		ip_addr.s_addr = status_info.con_event.gwaddr;
		sprintf(cmd, "echo \"nameserver %s\" > /etc/resolv.conf",
				inet_ntoa(ip_addr));
		DEBUG_PRINTF("cmd:%s\n", cmd);
		system(cmd);
		
		system("ifconfig wlan0 up");
		
		ip_addr.s_addr = status_info.con_event.gwaddr;
		sprintf(cmd, "route add default gw %s", inet_ntoa(ip_addr));
		DEBUG_PRINTF("cmd:%s\n", cmd);
		system(cmd);

		if (NULL != m_wifi_cb) {
			m_wifi_cb(RK_WIFI_State_CONNECTED, &g_wifi_Info);
			m_wifi_cb(RK_WIFI_State_DHCP_OK, &g_wifi_Info);
		}
	} else {
		memcpy(g_wifi_Info.wpa_state, "DISCONNECTED", strlen("DISCONNECTED"));
		DEBUG_PRINTF("wpa_state=INACTIVE\n");
		if (NULL != m_wifi_cb) {
			m_wifi_cb(RK_WIFI_State_DISCONNECTED, &g_wifi_Info);
		}
	}

	return 0;
err:
	return -1;
}

int get_status_send(int fp, int argc, char *argv[]) {
  int ret = 0;
  struct status_info status_info;
  struct in_addr addr;

  memset(&status_info, 0, sizeof(struct status_info));
  ret = ioctl(fp, ATBM_STATUS, (unsigned int)(&status_info));
  if (ret) {
    goto err;
  }

  DEBUG_PRINTF("wifi_mode=%s\n", status_info.wifimode ? "AP" : "STA");
  if (status_info.wifimode || status_info.bconnect) {
    DEBUG_PRINTF("wpa_state=ACTIVE\n");
    DEBUG_PRINTF("ssid=%s\n", status_info.con_event.ssid);
    addr.s_addr = status_info.con_event.ipaddr;
    DEBUG_PRINTF("ip_address=%s\n", inet_ntoa(addr));
    DEBUG_PRINTF("mac_address=");
    if (status_info.wifimode) {
      MAC_printf(status_info.macaddr);
    } else {
      MAC_printf(status_info.con_event.bssid);
    }
    addr.s_addr = status_info.con_event.ipmask;
    DEBUG_PRINTF("\nip_mask=%s\n", inet_ntoa(addr));
    addr.s_addr = status_info.con_event.gwaddr;
    DEBUG_PRINTF("gate_way=%s\n", inet_ntoa(addr));
  } else {
    DEBUG_PRINTF("wpa_state=INACTIVE\n");
  }

  return 0;
err:
  return -1;
}

/**
 * @brief 获取WiFi连接状态，sta模式的
 *
 * @return sta模式下已连接返回1，否则返回0，失败返回负数
 */
int atbm_clear_wifi_cfg(void)
{
  int ret = 0;
	ret = ioctl(g_fp, ATBM_CLEAR_WIFI_CFG, NULL);
	if (0 != ret) {
		DEBUG_PRINTF("clear wifi config fail\n");
		return -1;
	}

	system("ifconfig wlan0 0.0.0.0");
	DEBUG_PRINTF("clear wifi config success\n");
	return 0;
}

/**
 * @brief 获取WiFi连接状态，sta模式的
 *
 * @return sta模式下已连接返回1，否则返回0，失败返回负数
 */
int get_ipaddr_v4(char *ip_addr, int ip_addr_size) {
  if (NULL == ip_addr) {
    DEBUG_PRINTF("null prt...\n");
    return -1;
  }

  int ret = 0;
  struct status_info status_info;
  struct in_addr addr;

  memset(&status_info, 0, sizeof(struct status_info));
  ret = ioctl(g_fp, ATBM_STATUS, (unsigned int)(&status_info));
  if (ret) {
    return -1;
  }

  DEBUG_PRINTF("wifi_mode=%s\n", status_info.wifimode ? "AP" : "STA");
  if (status_info.wifimode || status_info.bconnect) {
    DEBUG_PRINTF("wpa_state=ACTIVE\n");
    addr.s_addr = status_info.con_event.ipaddr;
    DEBUG_PRINTF("ip_address=%s\n", inet_ntoa(addr));
    snprintf(ip_addr, ip_addr_size, "%s", inet_ntoa(addr));
  } else {
    DEBUG_PRINTF("wpa_state=INACTIVE\n");
  }

  return 0;
}

/**
 * @brief 获取WiFi连接状态，sta模式的
 *
 * @return sta模式下已连接返回1，否则返回0，失败返回负数
 */
int get_sta_model_status() {
  int ret = 0;
  struct status_info status_info;

  memset(&status_info, 0, sizeof(struct status_info));
  ret = ioctl(g_fp, ATBM_STATUS, (unsigned int)(&status_info));
  if (0 != ret) {
    DEBUG_PRINTF("failed when ioctl(%d)\n", ret);
    return -1;
  }

  DEBUG_PRINTF("wifi_mode = %s, connect status = %d\n",
               status_info.wifimode ? "AP" : "STA", status_info.bconnect);
  if (0 == status_info.wifimode && status_info.bconnect) {
    DEBUG_PRINTF("sta model and connected...\n");
    return 1;
  }

  return 0;
}

int set_ip_cmd(int fp, int argc, char *argv[]) {
  char cmd[30];
  struct in_addr ip_addr;

  if (ip_set_auto) {
    DEBUG_PRINTF("Line:%d can not set ip now, please change ip_auto mode\n",
                 __LINE__);
    goto err;
  }

  if (status.is_connected) {
    ip_addr.s_addr = status.event.ipaddr;
    sprintf(cmd, "ifconfig wlan0 %s", inet_ntoa(ip_addr));
    system(cmd);
    system("ifconfig wlan0 up");
  } else {
    DEBUG_PRINTF("Now, not connect any AP.\n");
  }
  return 0;
err:
  return -1;
}

int set_ip_auto_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;
  int value = 0;

  if (argc < 1) {
    DEBUG_PRINTF("Invalid SET_IP_AUTO command: needs at least 1 argument\n");
    goto err;
  } else {
    value = atoi(argv[0]);
    if (0 != value && 1 != value) {
      DEBUG_PRINTF("Line:%d invalid ip_auto value %d\n", __LINE__, value);
      DEBUG_PRINTF("Line:%d now, ip_auto mode is %d.\n", __LINE__, ip_set_auto);
      goto err;
    } else {
      DEBUG_PRINTF("set ip_set_auto form %d to %d\n", ip_set_auto, value);
      ip_set_auto = value;
    }
  }

  return 0;
err:
  return -1;
}

int list_filters_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;
  int i = 0;
  struct tcp_filter_info tcp_filter_get;

  ret = ioctl(fp, ATBM_GET_FILTER, (unsigned int)(&tcp_filter_get));
  if (!ret) {
    DEBUG_PRINTF("<TCP FILTER CONFIGURED>\n");
    DEBUG_PRINTF("SRC PORT:");
    for (i = 0; i < tcp_filter_get.src_cnt; i++) {
      DEBUG_PRINTF("%d ", ntohs(tcp_filter_get.src_tcpport[i]));
    }
    DEBUG_PRINTF("\n");
    DEBUG_PRINTF("DES PORT:");
    for (i = 0; i < tcp_filter_get.des_cnt; i++) {
      DEBUG_PRINTF("%d ", ntohs(tcp_filter_get.des_tcpport[i]));
    }
    DEBUG_PRINTF("\nICMP: %s\n",
                 (tcp_filter_get.Flags & FILTER_F_ICMP) ? "SET" : "NOT SET");
  } else {
    goto err;
  }

  return 0;
err:
  return -1;
}

int set_filter(char *cmd_0, char *cmd_1) {
  int ret = 0;

  int i = 0;
  int j = 0;
  int tcp_port = 0;

  if (!strcmp(cmd_0, "SRC")) {
    memset(tcp_filter_set.src_tcpport, 0, sizeof(tcp_filter_set.src_tcpport));
    tcp_filter_set.src_cnt = 0;

    tcp_port = atoi(cmd_1);
    for (j = 0; j < tcp_filter_set.src_cnt; j++) {
      if (htons(tcp_port) == tcp_filter_set.src_tcpport[j]) {
        DEBUG_PRINTF("Line:%d the same tcp_port %d. please reset tcp ports\n",
                     __LINE__, tcp_port);
        memset(tcp_filter_set.src_tcpport, 0,
               sizeof(tcp_filter_set.src_tcpport));
        tcp_filter_set.src_cnt = 0;
        goto err;
      }
    }
    tcp_filter_set.src_tcpport[tcp_filter_set.src_cnt] = htons(tcp_port);
    tcp_filter_set.src_cnt++;
  } else if (!strcmp(cmd_0, "DES")) {
    memset(tcp_filter_set.des_tcpport, 0, sizeof(tcp_filter_set.des_tcpport));
    tcp_filter_set.des_cnt = 0;

    tcp_port = atoi(cmd_1);
    for (j = 0; j < tcp_filter_set.des_cnt; j++) {
      if (htons(tcp_port) == tcp_filter_set.des_tcpport[j]) {
        DEBUG_PRINTF("Line:%d the same tcp_port %d. please reset tcp ports\n",
                     __LINE__, tcp_port);
        memset(tcp_filter_set.des_tcpport, 0,
               sizeof(tcp_filter_set.des_tcpport));
        tcp_filter_set.des_cnt = 0;
        goto err;
      }
    }
    tcp_filter_set.des_tcpport[tcp_filter_set.des_cnt] = htons(tcp_port);
    tcp_filter_set.des_cnt++;
  } else if (!strcmp(cmd_0, "ICMP")) {
    tcp_filter_set.Flags = 0;
    tcp_filter_set.Flags = atoi(cmd_1) ? FILTER_F_ICMP : 0;
  } else {
    DEBUG_PRINTF("Line:%d invalid command %s\n", __LINE__, cmd_0);
    goto err;
  }

  {
    int i = 0;
    DEBUG_PRINTF("Now, you set\nSRC (client):");
    for (i = 0; i < tcp_filter_set.src_cnt; i++) {
      DEBUG_PRINTF("%d ", ntohs(tcp_filter_set.src_tcpport[i]));
    }
    DEBUG_PRINTF("\nDES (server):");
    for (i = 0; i < tcp_filter_set.des_cnt; i++) {
      DEBUG_PRINTF("%d ", ntohs(tcp_filter_set.des_tcpport[i]));
    }
    DEBUG_PRINTF("\nICMP: %s\n",
                 (tcp_filter_set.Flags & FILTER_F_ICMP) ? "SET" : "NOT SET");
  }

  return 0;
err:
  return -1;
}

int set_filter_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;

  if (0 == argc) {
    DEBUG_PRINTF("set_filter variables:\n"
                 "  SRC (source tcp port, for client)\n"
                 "  DES (destination tcp port, for server)\n"
                 "  ICMP (configure ICMP filter, 1 or 0, default not set)\n"
                 "  example: set_filter SRC 1201 1316\n");
    return 0;
  } else {
    if (argc < 2 || argc > 5) {
      DEBUG_PRINTF("Invalid SET_FILTER command: needs at least 2 argument, max "
                   "5 arguments\n");
      goto err;
    } else {
      int i = 0;
      int j = 0;
      int tcp_port = 0;

      if (!strcmp(argv[0], "SRC")) {
        memset(tcp_filter_set.src_tcpport, 0,
               sizeof(tcp_filter_set.src_tcpport));
        tcp_filter_set.src_cnt = 0;

        for (i = 1; i < argc; i++) {
          tcp_port = atoi(argv[i]);
          for (j = 0; j < tcp_filter_set.src_cnt; j++) {
            if (htons(tcp_port) == tcp_filter_set.src_tcpport[j]) {
              DEBUG_PRINTF(
                  "Line:%d the same tcp_port %d. please reset tcp ports\n",
                  __LINE__, tcp_port);
              memset(tcp_filter_set.src_tcpport, 0,
                     sizeof(tcp_filter_set.src_tcpport));
              tcp_filter_set.src_cnt = 0;
              goto err;
            }
          }
          tcp_filter_set.src_tcpport[tcp_filter_set.src_cnt] = htons(tcp_port);
          tcp_filter_set.src_cnt++;
        }
      } else if (!strcmp(argv[0], "DES")) {
        memset(tcp_filter_set.des_tcpport, 0,
               sizeof(tcp_filter_set.des_tcpport));
        tcp_filter_set.des_cnt = 0;

        for (i = 1; i < argc; i++) {
          tcp_port = atoi(argv[i]);
          for (j = 0; j < tcp_filter_set.des_cnt; j++) {
            if (htons(tcp_port) == tcp_filter_set.des_tcpport[j]) {
              DEBUG_PRINTF(
                  "Line:%d the same tcp_port %d. please reset tcp ports\n",
                  __LINE__, tcp_port);
              memset(tcp_filter_set.des_tcpport, 0,
                     sizeof(tcp_filter_set.des_tcpport));
              tcp_filter_set.des_cnt = 0;
              goto err;
            }
          }
          tcp_filter_set.des_tcpport[tcp_filter_set.des_cnt] = htons(tcp_port);
          tcp_filter_set.des_cnt++;
        }
      } else if (!strcmp(argv[0], "ICMP")) {
        tcp_filter_set.Flags = 0;
        tcp_filter_set.Flags = atoi(argv[1]) ? FILTER_F_ICMP : 0;
      } else {
        DEBUG_PRINTF("Line:%d invalid command %s\n", __LINE__, argv[0]);
        goto err;
      }
    }
  }

  {
    int i = 0;
    DEBUG_PRINTF("Now, you set\nSRC (client):");
    for (i = 0; i < tcp_filter_set.src_cnt; i++) {
      DEBUG_PRINTF("%d ", ntohs(tcp_filter_set.src_tcpport[i]));
    }
    DEBUG_PRINTF("\nDES (server):");
    for (i = 0; i < tcp_filter_set.des_cnt; i++) {
      DEBUG_PRINTF("%d ", ntohs(tcp_filter_set.des_tcpport[i]));
    }
    DEBUG_PRINTF("\nICMP: %s\n",
                 (tcp_filter_set.Flags & FILTER_F_ICMP) ? "SET" : "NOT SET");
  }

  return 0;
err:
  return -1;
}

int enable_filter() {
  if (tcp_filter_set.des_cnt == 0 && tcp_filter_set.src_cnt == 0) {
    DEBUG_PRINTF("please set filter first.\n");
    return -1;
  }
  tcp_filter_set.Flags |= FILTER_F_ARP | FILTER_F_DNS;
  return ioctl(g_fp, ATBM_SET_FILTER, (unsigned int)(&tcp_filter_set));
}

int enable_filter_cmd(int fp, int argc, char *argv[]) {
  if (tcp_filter_set.des_cnt == 0 && tcp_filter_set.src_cnt == 0) {
    DEBUG_PRINTF("please set filter first.\n");
    return -1;
  }
  tcp_filter_set.Flags |= FILTER_F_ARP | FILTER_F_DNS;
  return ioctl(fp, ATBM_SET_FILTER, (unsigned int)(&tcp_filter_set));
}

int scan_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;

  if (wifi_mode) {
    DEBUG_PRINTF("please switch wifi mode to STA.\n");
    ret = -1;
  } else {
    ret = ioctl(fp, ATBM_SCAN, NULL);
  }

  return ret;
}

int atbm_scan(void)
{
	int ret = 0;

	if (wifi_mode) {
		DEBUG_PRINTF("please switch wifi mode to STA.\n");
		ret = -1;
	} else {
		ret = ioctl(g_fp, ATBM_SCAN, NULL);
	}

	return ret;
}

int scan_results_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;
  struct scan_result scan_info;
  int start = 1;

  if (wifi_mode) {
    DEBUG_PRINTF("please switch wifi mode to STA.\n");
    ret = -1;
  } else {
    memset(&scan_info, 0, sizeof(scan_info));
    scan_info.remain = 1;

    while (scan_info.remain) {
      ret = ioctl(fp, ATBM_SCAN_INFO, (unsigned int)(&scan_info));
      if (!ret) {
        int i = 0;
        DEBUG_PRINTF("\nget scan cnt: %d (%s)\n", scan_info.scan_cnt,
                     scan_info.remain ? "REMAIN" : "OVER");
        if (start) {
          DEBUG_PRINTF("bssid\t/\tlevel\t/\tchannel\t/\tssid\n");
          start = 0;
        }
        for (i = 0; i < scan_info.scan_cnt; i++) {
          MAC_printf(scan_info.scan_info[i].bssid);
          DEBUG_PRINTF("\t%d\t%d\t%s\n", scan_info.scan_info[i].rssi,
                       scan_info.scan_info[i].channel,
                       scan_info.scan_info[i].ssid);
        }
      } else {
        break;
      }
    }
  }

  return ret;
}

int atbm_scan_results(void)
{
  int ret = 0;
  struct scan_result scan_info;
  int start = 1;

  if (wifi_mode) {
    DEBUG_PRINTF("please switch wifi mode to STA.\n");
    ret = -1;
  } else {
    memset(&scan_info, 0, sizeof(scan_info));
    scan_info.remain = 1;

    while (scan_info.remain) {
      ret = ioctl(g_fp, ATBM_SCAN_INFO, (unsigned int)(&scan_info));
      if (!ret) {
        int i = 0;
        DEBUG_PRINTF("\nget scan cnt: %d (%s)\n", scan_info.scan_cnt,
                     scan_info.remain ? "REMAIN" : "OVER");
        if (start) {
          DEBUG_PRINTF("bssid\t/\tlevel\t/\tchannel\t/\tssid\n");
          start = 0;
        }
        for (i = 0; i < scan_info.scan_cnt; i++) {
          MAC_printf(scan_info.scan_info[i].bssid);
          DEBUG_PRINTF("\t%d\t%d\t%s\n", scan_info.scan_info[i].rssi,
                       scan_info.scan_info[i].channel,
                       scan_info.scan_info[i].ssid);
        }
      } else {
        break;
      }
    }
  }

  return ret;
}

int set_wifi_mode_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;
  char is_ap = 0;

  if (argc < 1) {
    DEBUG_PRINTF("Invalid SET_WIFI_MODE command: needs at least 1 argument\n");
    goto err;
  }

  if (!strcmp(argv[0], "AP")) {
    is_ap = 1;
  } else if (!strcmp(argv[0], "STA")) {
    is_ap = 0;
  } else {
    DEBUG_PRINTF("Line:%d invalid wifi mode %s\n", __LINE__, argv[0]);
    goto err;
  }

  ret = ioctl(fp, ATBM_WIFI_MODE, is_ap);
  if (ret) {
    goto err;
  }

  wifi_mode = is_ap;

  return 0;
err:
  return -1;
}

int clear_ap_cfg_cmd(int fp, int argc, char *argv[]) {
  memset(&ap_cfg_set, 0, sizeof(ap_cfg_set));
  ap_ssid_set = 0;
  ap_key_set = 0;
  ap_key_id_set = 0;
  ap_key_mgmt_set = 0;
  return 0;
}

int set_ap_cfg_cmd(int fp, int argc, char *argv[]) {
  if (0 == argc) {
    DEBUG_PRINTF(
        "set_ap_cfg variables:\n"
        "  ssid (network name, SSID)\n"
        "  key (passphrase or password)\n"
        "  key_mgmt (key management protocol, refer: NONE/WEP/WPA/WPA2)\n"
        "  key_id (key identity)\n");
    return 0;
  }
  if (argc < 2) {
    DEBUG_PRINTF("Invalid SET_AP_CFG command: needs at least 2 argument\n");
    goto err;
  } else {
    struct connect_info info_tmp;
    memcpy(&info_tmp, &ap_cfg_set.ap_parameter, sizeof(info_tmp));

    if (!strcmp(argv[0], "ssid")) {
      strcpy(info_tmp.ssid, argv[1]);
      info_tmp.ssidLength = strlen(argv[1]);
      if (wifi_param_check(&info_tmp, CHECK_SSID, 1)) {
        goto err;
      }
      memcpy(&ap_cfg_set.ap_parameter, &info_tmp, sizeof(info_tmp));
      ap_ssid_set = 1;
    } else if (!strcmp(argv[0], "key_mgmt")) {
      int key_type = key_mgmt_transform2int(argv[1]);
      if (key_type < 0 || key_type == 2) {
        DEBUG_PRINTF("Line:%d invalid key_mgmt %s\n", __LINE__, argv[1]);
        DEBUG_PRINTF("Line:%d no new key_mgmt values configured.\n", __LINE__);
        goto err;
      }

      info_tmp.keyMgmt = key_type;
      if (wifi_param_check(&info_tmp, CHECK_KEY_MGMT, 1)) {
        goto err;
      }
      memcpy(&ap_cfg_set.ap_parameter, &info_tmp, sizeof(info_tmp));
      ap_key_mgmt_set = 1;
    } else if (!strcmp(argv[0], "key_id")) {
      info_tmp.keyId = atoi(argv[1]);
      if (wifi_param_check(&info_tmp, CHECK_KEY_ID, 1)) {
        goto err;
      }
      memcpy(&ap_cfg_set.ap_parameter, &info_tmp, sizeof(info_tmp));
      ap_key_id_set = 1;
    } else if (!strcmp(argv[0], "key")) {
      strcpy(info_tmp.key, argv[1]);
      info_tmp.keyLength = strlen(argv[1]);
      if (wifi_param_check(&info_tmp, CHECK_KEY, 1)) {
        goto err;
      }
      memcpy(&ap_cfg_set.ap_parameter, &info_tmp, sizeof(info_tmp));
      ap_key_set = 1;
    } else {
      DEBUG_PRINTF("Line:%d invalid command %s\n", __LINE__, argv[0]);
      goto err;
    }
  }

  return 0;
err:
  return -1;
}

int enable_ap_cfg_cmd(int fp, int argc, char *argv[]) {
  int old_wifi_mode = wifi_mode;
  wifi_mode = 1;
  if (ioctl(fp, ATBM_AP_CFG, (unsigned int)(&ap_cfg_set)) == 0) {
    return 0;
  } else {
    wifi_mode = old_wifi_mode;
    return -1;
  }
}

int switch_channel_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;

  if (argc < 1) {
    DEBUG_PRINTF("Invalid SWITCH_CHANNEL command: needs at least 1 argument\n");
    goto err;
  } else {
    char channel_id = atoi(argv[0]);
    if (channel_id <= 0 || channel_id > 14) {
      DEBUG_PRINTF("Line:%d invalid channel_id %s\n", __LINE__, argv[0]);
      DEBUG_PRINTF("Line:%d no channel_id values configured.\n", __LINE__);
      goto err;
    }

    ret = ioctl(fp, ATBM_WIFI_CHANNEL, channel_id);
    if (ret) {
      goto err;
    }
  }

  return 0;
err:
  return -1;
}

int set_country_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;

  if (argc < 1) {
    DEBUG_PRINTF("Invalid SET_COUNTRY command: needs at least 1 argument\n");
    goto err;
  } else {
    char country_id = country_transform2int(argv[0]);
    if (country_id < 0) {
      DEBUG_PRINTF("Line:%d invalid country %s\n", __LINE__, argv[0]);
      DEBUG_PRINTF("Line:%d no country values configured.\n", __LINE__);
      goto err;
    }
    DEBUG_PRINTF("set country: %d\n", country_id);
    ret = ioctl(fp, ATBM_SET_COUNTRY, country_id);
    if (ret) {
      goto err;
    }
  }

  return 0;
err:
  return -1;
}

int get_country_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;
  char country_id = 0;

  ret = ioctl(fp, ATBM_GET_COUNTRY, &country_id);
  if (ret) {
    goto err;
  }

  if (country_id >= COUNTRY_MAX) {
    DEBUG_PRINTF("Line:%d get invalid country\n", __LINE__);
    goto err;
  }

  DEBUG_PRINTF("country: %s\n", country_transform2str(country_id));

  return 0;
err:
  return -1;
}

int get_stalist_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;
  int i = 0;
  struct stalist_info stalist;

  memset(&stalist, 0, sizeof(stalist));
  ret = ioctl(fp, ATBM_GET_STALIST, (unsigned int)(&stalist));
  if (ret) {
    goto err;
  }

  DEBUG_PRINTF("STA List:\n");
  for (i = 0; i < stalist.sta_cnt; i++) {
    MAC_printf(stalist.info[i].macAddr);
    DEBUG_PRINTF("\n");
  }
  DEBUG_PRINTF("\n");

  return 0;
err:
  return -1;
}

int smart_cfg_start_cmd(int fp, int argc, char *argv[]) {
  return ioctl(fp, ATBM_SMART_START, NULL);
}

int smart_cfg_stop_cmd(int fp, int argc, char *argv[]) {
  return ioctl(fp, ATBM_SMART_STOP, NULL);
}

int ap_touch_start_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;
  struct ap_touch_info ap_touch;

  if (argc < 1) {
    DEBUG_PRINTF("Invalid AP_TOUCH_START command: needs at least 1 argument\n");
    goto err;
  } else {
    memset(&ap_touch, 0, sizeof(ap_touch));
    if (!strcmp(argv[0], "AP_NOTIFY")) {
      ap_touch.is_no_notify = 0;
    } else {
      ap_touch.is_no_notify = 1;
    }

    ret = ioctl(fp, ATBM_APTOUCH_START, (unsigned int)(&ap_touch));
    if (ret) {
      goto err;
    }

    wifi_mode = 1;
  }

  return 0;
err:
  return -1;
}

int ap_touch_stop_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;

  ret = ioctl(fp, ATBM_APTOUCH_STOP, NULL);
  if (!ret) {
    wifi_mode = 0;
  }

  return ret;
}

int set_etf_cfg_cmd(int fp, int argc, char *argv[]) {
  if (0 == argc) {
    DEBUG_PRINTF("set_etf_cfg variables:\n"
                 "  channel (wifi channel 1~14)\n"
                 "  rate (transmit rate)\n"
                 "  len (packet len)\n"
                 "  40m (set 40m support, 0 or 1\n"
                 "  greenfield (set greenfield mode, 0 or 1)\n");
    return 0;
  }
  if (argc < 2) {
    DEBUG_PRINTF("Invalid SET_ETF_CFG command: needs at least 2 argument\n");
    goto err;
  } else {
    int value = atoi(argv[1]);

    if (!strcmp(argv[0], "channel")) {
      if (value <= 0 || value > 14) {
        DEBUG_PRINTF("Line:%d invalid channel %s\n", __LINE__, argv[1]);
        DEBUG_PRINTF("Line:%d no channel values configured.\n", __LINE__);
        goto err;
      }
      etf_cfg_set.channel = value;
    } else if (!strcmp(argv[0], "rate")) {
      int i = 0;
      for (i = 0; i < 20; i++) {
        if (value == rate_set[i]) {
          break;
        }
      }

      if (i == 20) {
        DEBUG_PRINTF("Line:%d invalid rate %s\n", __LINE__, argv[1]);
        DEBUG_PRINTF("Line:%d no rate values configured.\n", __LINE__);
        goto err;
      }
      etf_cfg_set.rate = value;
    } else if (!strcmp(argv[0], "len")) {
      if (value < 0) {
        DEBUG_PRINTF("Line:%d invalid len %s\n", __LINE__, argv[1]);
        DEBUG_PRINTF("Line:%d no len values configured.\n", __LINE__);
        goto err;
      }
      etf_cfg_set.len = value;
    } else if (!strcmp(argv[0], "40m")) {
      if (value != 0 && value != 1) {
        DEBUG_PRINTF("Line:%d invalid 40m %s\n", __LINE__, argv[1]);
        DEBUG_PRINTF("Line:%d no 40m values configured.\n", __LINE__);
      }
      etf_cfg_set.is40M = value;
    } else if (!strcmp(argv[0], "greenfield")) {
      if (value != 0 && value != 1) {
        DEBUG_PRINTF("Line:%d invalid greenfield %s\n", __LINE__, argv[1]);
        DEBUG_PRINTF("Line:%d no greenfield values configured.\n", __LINE__);
      }
      etf_cfg_set.greenfield = value;
    } else {
      DEBUG_PRINTF("Line:%d invalid command %s\n", __LINE__, argv[0]);
      goto err;
    }
  }

  return 0;
err:
  return -1;
}

int etf_start_tx_cmd(int fp, int argc, char *argv[]) {
  DEBUG_PRINTF("Current channel:%d  rate:%d  len:%d  is40M:%d  greenfield:%d\n",
               etf_cfg_set.channel, etf_cfg_set.rate, etf_cfg_set.len,
               etf_cfg_set.is40M, etf_cfg_set.greenfield);

  return ioctl(fp, ATBM_ETF_START_TX, (unsigned int)(&etf_cfg_set));
}

int etf_stop_tx_cmd(int fp, int argc, char *argv[]) {
  return ioctl(fp, ATBM_ETF_STOP_TX, NULL);
}

int etf_singletone_cmd(int fp, int argc, char *argv[]) {
  DEBUG_PRINTF("Current channel:%d\n", etf_cfg_set.channel);

  return ioctl(fp, ATBM_ETF_SINGLETONE, (unsigned int)(&etf_cfg_set));
}

int etf_start_rx_cmd(int fp, int argc, char *argv[]) {
  DEBUG_PRINTF("Current channel:%d  is40M:%d\n", etf_cfg_set.channel,
               etf_cfg_set.is40M);

  return ioctl(fp, ATBM_ETF_START_RX, (unsigned int)(&etf_cfg_set));
}

int etf_start_rx_no_drop_cmd(int fp, int argc, char *argv[]) {
  DEBUG_PRINTF("Current channel:%d  is40M:%d\n", etf_cfg_set.channel,
               etf_cfg_set.is40M);

  return ioctl(fp, ATBM_ETF_START_RX_NO_DROP, (unsigned int)(&etf_cfg_set));
}

int etf_stop_rx_cmd(int fp, int argc, char *argv[]) {
  return ioctl(fp, ATBM_ETF_STOP_RX, NULL);
}

int etf_reset_rx_cmd(int fp, int argc, char *argv[]) {
  return ioctl(fp, ATBM_ETF_RESET_RX, NULL);
}

int adaptive_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;
  struct adaptive_info adaptive;

  if (argc < 1) {
    DEBUG_PRINTF("Invalid ADAPTIVE command: needs at least 1 argument\n");
    goto err;
  } else {
    memset(&adaptive, 0, sizeof(adaptive));

    if (!strcmp(argv[0], "ON")) {
      adaptive.adaptive_flag = 1;
    } else if (!strcmp(argv[0], "OFF")) {
      adaptive.adaptive_flag = 0;
    } else {
      DEBUG_PRINTF("Line:%d invalid adaptive flag %s\n", __LINE__, argv[0]);
      goto err;
    }

    ret = ioctl(fp, ATBM_ADAPTIVE, (unsigned int)(&adaptive));
    if (ret) {
      goto err;
    }
  }

  return 0;
err:
  return -1;
}

int get_ver_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;
  struct version_info version;

  memset(&version, 0, sizeof(version));
  ret = ioctl(fp, ATBM_VERSION, (unsigned int)(&version));
  if (ret) {
    goto err;
  }

  DEBUG_PRINTF("[HW FW Version]\n %s\n", version.hw_ver);
  return 0;
err:
  return -1;
}

int get_sdk_ver_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;
  struct version_info version;

  memset(&version, 0, sizeof(version));
  ret = ioctl(fp, ATBM_SDK_VERSION, (unsigned int)(&version));
  if (ret) {
    goto err;
  }

  DEBUG_PRINTF("[SDK Version]\n %s\n", version.sdk_ver);
  return 0;
err:
  return -1;
}

int update_fw_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;
  char is_restart = 0;
  struct update_info update;

  if (argc < 1) {
    DEBUG_PRINTF("Invalid UPDATE_FW command: needs at least 1 argument\n");
    goto err;
  } else {
    FILE *file = NULL;
    int len = 0;
    int read_len = 0;
    int ret_size = 0;
    int first_write = 0;

    memset(&update, 0, sizeof(update));

    file = fopen(argv[0], "rb");
    if (NULL == file) {
      DEBUG_PRINTF("Line:%d invalid fw path %s\n", __LINE__, argv[0]);
      goto err;
    }

    fseek(file, 0, SEEK_END);
    len = ftell(file);
    fseek(file, 0, SEEK_SET);
    DEBUG_PRINTF("file:%s  size:%d bytes\n", argv[0], len);

    if (len <= 512) {
      DEBUG_PRINTF("Line:%d The file length is invalid\n", __LINE__);
      goto err;
    }

    ret = ioctl(fp, ATBM_UPDATE_FW, (unsigned int)(&update));
    if (ret) {
      DEBUG_PRINTF("Line:%d start update err\n", __LINE__);
      goto err;
    }

    update.start = 1;
    first_write = 1;

    while (!feof(file) && len > 0) {
      if (first_write) {
        read_len = 512; // write fw bin header
        first_write = 0;
      } else {
        if (len >= 512) {
          read_len = 512;
        } else {
          read_len = len;
        }
      }

      ret_size = fread(update.data, 1, read_len, file);
      if (ret_size != read_len) {
        DEBUG_PRINTF("Line:%d returned bytes not match\n", __LINE__);
        fclose(file);
        goto err;
      }

      update.len = read_len;
      len -= read_len;

      ret = ioctl(fp, ATBM_UPDATE_FW, (unsigned int)(&update));
      if (ret) {
        DEBUG_PRINTF("Line:%d write update err\n", __LINE__);
        fclose(file);
        goto err;
      }
    }

    fclose(file);

    update.start = 0;
    update.end = 1;
    ret = ioctl(fp, ATBM_UPDATE_FW, (unsigned int)(&update));
    if (ret) {
      DEBUG_PRINTF("Line:%d end update err:%d\n", __LINE__);
      fclose(file);
      goto err;
    }
  }

  return 0;
err:
  return -1;
}

int listen_itvl_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;

  if (wifi_mode) {
    DEBUG_PRINTF("please switch wifi mode to STA.\n");
    goto err;
  } else {
    if (argc < 1) {
      DEBUG_PRINTF("Invalid LISTEN_ITVL command: needs at least 1 argument\n");
      goto err;
    } else {
      char val = atoi(argv[0]);
      if (val <= 0 || val > 20) {
        DEBUG_PRINTF("Line:%d invalid interval %s\n", __LINE__, argv[0]);
        goto err;
      }

      ret = ioctl(fp, ATBM_LISTEN_ITVL, val);
      if (ret) {
        goto err;
      }
    }
  }

  return 0;
err:
  return -1;
}

int fw_print_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;

  if (argc < 1) {
    DEBUG_PRINTF("Invalid FW_PRINT command: needs at least 1 argument\n");
    goto err;
  } else {
    char val = atoi(argv[0]);
    ret = ioctl(fp, ATBM_FW_DBG_PRINT, val);
    if (ret) {
      goto err;
    }
  }

  return 0;
err:
  return -1;
}

int fw_at(char *cmd) {
  struct at_cmd_info at_cmd;

  memset(&at_cmd, 0, sizeof(at_cmd));
  memcpy(at_cmd.cmd, cmd, strlen(cmd) + 1);
  DEBUG_PRINTF("fw_at is %s\n", at_cmd.cmd);
  return ioctl(g_fp, ATBM_FW_AT_CMD, (unsigned int)(&at_cmd));
}

int fw_at_cmd(int fp, int argc, char *argv[]) {
  struct at_cmd_info at_cmd;

  if (argc != 1) {
    DEBUG_PRINTF("Line:%d FW_CMD invalid argument number\n", __LINE__);
    return -1;
  }

  memset(&at_cmd, 0, sizeof(at_cmd));
  memcpy(at_cmd.cmd, argv[0], strlen(argv[0]) + 1);
  return ioctl(fp, ATBM_FW_AT_CMD, (unsigned int)(&at_cmd));
}

int set_rmmod_auto_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;
  int value = 0;

  if (argc < 1) {
    DEBUG_PRINTF("Invalid SET_RMMOD_AUTO command: needs at least 1 argument\n");
    goto err;
  } else {
    value = atoi(argv[0]);
    if (0 != value && 1 != value) {
      DEBUG_PRINTF("Line:%d invalid rmmod_auto value %d\n", __LINE__, value);
      DEBUG_PRINTF("Line:%d now, rmmod_auto mode is %d.\n", __LINE__,
                   rmmod_auto);
      goto err;
    } else {
      rmmod_auto = value;
    }
  }

  return 0;
err:
  return -1;
}

int get_rate_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;
  struct rate_info rate;

  ret = ioctl(fp, ATBM_TX_RATE, &rate);
  if (ret) {
    goto err;
  }
  DEBUG_PRINTF("rate: %d\n", rate.rate);
  if (rate_transform2str(rate.rate) == NULL) {
    goto err;
  }

  DEBUG_PRINTF("current transmit rate: %s\n", rate_transform2str(rate.rate));

  return 0;
err:
  return -1;
}

int send_ipc_data_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;
  int i = 0;
  int find_data = 0;
  int data_len = 0;
  int send_cnt = 0;

  if (argc < 1) {
    DEBUG_PRINTF("Invalid SEND_IPC_DATA command: needs at least 1 argument\n");
    goto err;
  }

  for (i = 0; i < argc; i++) {
    if (!memcmp(argv[i], "server=", 7)) {
      if (strlen(argv[i]) > (7 + 15) || strlen(argv[i]) <= 7) {
        DEBUG_PRINTF("invalid server argument.\n");
        goto err;
      }
      strcpy(ipc_data_set.server, argv[i] + 7);
      ipc_server_set = 1;
    } else if (!memcmp(argv[i], "port=", 5)) {
      if (strlen(argv[i]) <= 5) {
        DEBUG_PRINTF("invalid port argument.\n");
        goto err;
      }
      ipc_data_set.port = atoi(argv[i] + 5);
      ipc_port_set = 1;
    } else if (!memcmp(argv[i], "cnt=", 4)) {
      if (strlen(argv[i]) <= 4) {
        DEBUG_PRINTF("invalid cnt argument.\n");
        goto err;
      }
      send_cnt = atoi(argv[i] + 4);
    } else if (!memcmp(argv[i], "data=", 5)) {
      if (strlen(argv[i]) <= 5) {
        DEBUG_PRINTF("invalid data argument.\n");
        goto err;
      }
      data_len = strlen(argv[i] + 5);
      if (data_len > 999) {
        DEBUG_PRINTF("invalid data length. data cnt:%d over max length 999\n",
                     data_len);
        goto err;
      }
      strcpy(ipc_data_set.data, argv[i] + 5);
      ipc_data_set.len = data_len;
      find_data = 1;
    }
  }

  if (!find_data) {
    DEBUG_PRINTF("ipc data err or not dound.\n");
    goto err;
  }

  if (!ipc_port_set || !ipc_server_set) {
    DEBUG_PRINTF("not set ipc server or port.\n");
    goto err;
  }

  if (send_cnt == 0) {
    send_cnt = 1;
  }

  while (send_cnt--) {
    ret = ioctl(fp, ATBM_IPC_DATA, (unsigned int)(&ipc_data_set));
    if (ret) {
      goto err;
    }
  }

  return 0;
err:
  return -1;
}

int atbm_get_rssi_cmd(void)
{
	int ret = 0;
	struct rssi_info rssi;

	ret = ioctl(g_fp, ATBM_RSSI, &rssi);
	if (ret) {
		goto err;
	}

	DEBUG_PRINTF("current rssi: %d\n", rssi.rssi);

	return rssi.rssi;;

err:
	return -1;
}

int get_rssi_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;
  struct rssi_info rssi;

  ret = ioctl(fp, ATBM_RSSI, &rssi);
  if (ret) {
    goto err;
  }

  DEBUG_PRINTF("current rssi: %d\n", rssi.rssi);
  return 0;
err:
  return -1;
}

int get_time_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;
  struct time_info_req time;
  static const char day_name[7][4] = {"Sun", "Mon", "Tue", "Wed",
                                      "Thu", "Fri", "Sat"};
  static const char mon_name[12][4] = {"Jan", "Feb", "Mar", "Apr",
                                       "May", "Jun", "Jul", "Aug",
                                       "Sep", "Oct", "Nov", "Dec"};

  ret = ioctl(fp, ATBM_TIME, &time);
  if (ret) {
    goto err;
  }

  DEBUG_PRINTF("Current time:\n%s %s %02d %02d:%02d:%02d %02d\n",
               day_name[time.time.tm_wday], mon_name[time.time.tm_mon],
               time.time.tm_mday, time.time.tm_hour, time.time.tm_min,
               time.time.tm_sec, 1900 + time.time.tm_year);

  return 0;
err:
  return -1;
}

int clear_wifi_cfg_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;

  ret = ioctl(fp, ATBM_CLEAR_WIFI_CFG, NULL);
  if (ret) {
    goto err;
  }

  return 0;
err:
  return -1;
}

int get_etf_rx_info_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;
  struct etf_rx_info_req rx_info;

  ret = ioctl(fp, ATBM_ETF_RX_INFO, &rx_info);
  if (ret) {
    goto err;
  }

  DEBUG_PRINTF(
      "current etf rx: success_cnt%d, fcs_err_cnt:%d, per:%d%%, rssi:%d\n",
      rx_info.rx_success_cnt, rx_info.fcs_err_cnt,
      rx_info.fcs_err_cnt * 100 /
          (rx_info.rx_success_cnt + rx_info.fcs_err_cnt),
      rx_info.rssi);

  return 0;
err:
  return -1;
}

int add_netpattern_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;
  struct netpattern_req netpattern_info;

  if (argc != 5) {
    DEBUG_PRINTF("Invalid ADD_NETPATTERN command: needs 5 argument\n");
    goto err;
  }

  memset(&netpattern_info, 0, sizeof(netpattern_info));

  if (atoi(argv[0]) < 0 || atoi(argv[0]) >= 4) {
    DEBUG_PRINTF("Invalid index\n");
    goto err;
  }
  netpattern_info.netpattern_index = atoi(argv[0]);
  netpattern_info.src_ipaddr = inet_addr(argv[1]);
  if (netpattern_info.src_ipaddr == 0xffffffff) {
    DEBUG_PRINTF("Invalid ip\n");
    goto err;
  }
  netpattern_info.src_port = atoi(argv[2]);
  if (strcmp(argv[3], "TCP") && strcmp(argv[3], "UDP")) {
    DEBUG_PRINTF("Invalid protocol %s, please refer: TCP/UDP\n", argv[3]);
    goto err;
  } else {
    netpattern_info.protocol = strcmp(argv[3], "TCP") == 0 ? 1 : 0;
  }
  netpattern_info.netpattern_len = strlen(argv[4]);
  if (netpattern_info.netpattern_len > 63) {
    netpattern_info.netpattern_len = 63;
  }
  memcpy(netpattern_info.netpattern_data, argv[4],
         netpattern_info.netpattern_len);
  netpattern_info.netpattern_data[netpattern_info.netpattern_len] = '\0';

  ret = ioctl(fp, ATBM_NETPATTERN_ADD, &netpattern_info);
  if (ret) {
    goto err;
  }

  return 0;
err:
  return -1;
}

int del_netpattern_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;
  struct netpattern_req netpattern_info;

  if (argc != 1) {
    DEBUG_PRINTF("Invalid DEL_NETPATTERN command: needs 1 argument\n");
    goto err;
  }

  if (atoi(argv[0]) < 0 || atoi(argv[0]) >= 4) {
    DEBUG_PRINTF("Invalid index\n");
    goto err;
  }
  netpattern_info.netpattern_index = atoi(argv[0]);

  ret = ioctl(fp, ATBM_NETPATTERN_DEL, &netpattern_info);
  if (ret) {
    goto err;
  }

  return 0;
err:
  return -1;
}

int add_conn_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;
  struct conn_param_req conn_info;

  if (argc != 6) {
    DEBUG_PRINTF("Invalid ADD_CONN command: needs 6 argument\n");
    goto err;
  }

  memset(&conn_info, 0, sizeof(conn_info));

  if (atoi(argv[0]) < 0 || atoi(argv[0]) >= 4) {
    DEBUG_PRINTF("Invalid index\n");
    goto err;
  }
  conn_info.sess_id = atoi(argv[0]);
  conn_info.dst_ip = inet_addr(argv[1]);
  if (conn_info.dst_ip == 0xffffffff) {
    DEBUG_PRINTF("Invalid ip\n");
    goto err;
  }
  conn_info.dst_port = atoi(argv[2]);
  conn_info.interval_time = atoi(argv[3]);
  if (strcmp(argv[4], "TCP") && strcmp(argv[4], "UDP")) {
    DEBUG_PRINTF("Invalid protocol, please refer: TCP/UDP\n");
    goto err;
  } else {
    conn_info.protocol = strcmp(argv[4], "TCP") == 0 ? 1 : 0;
  }
  if (strlen(argv[5]) <= 63) {
    strcpy(conn_info.tcp_payload, argv[5]);
  } else {
    memcpy(conn_info.tcp_payload, argv[5], 63);
    conn_info.tcp_payload[63] = '\0';
  }

  ret = ioctl(fp, ATBM_CONN_PARAM_ADD, &conn_info);
  if (ret) {
    goto err;
  }

  return 0;
err:
  return -1;
}

int del_conn_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;
  struct conn_param_req conn_info;

  if (argc != 1) {
    DEBUG_PRINTF("Invalid DEL_CONN command: needs 1 argument\n");
    goto err;
  }

  if (atoi(argv[0]) < 0 || atoi(argv[0]) >= 4) {
    DEBUG_PRINTF("Invalid index\n");
    goto err;
  }
  conn_info.sess_id = atoi(argv[0]);

  ret = ioctl(fp, ATBM_CONN_PARAM_DEL, &conn_info);
  if (ret) {
    goto err;
  }

  return 0;
err:
  return -1;
}

int conn_lose_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;
  struct conn_lose_req conn_lose_map;

  ret = ioctl(fp, ATBM_CONN_LOSE_MAP, &conn_lose_map);
  if (ret) {
    goto err;
  }

  DEBUG_PRINTF("conn lose map: 0x%x\n", conn_lose_map.lose_map);
  return 0;
err:
  return -1;
}

int conn_switch_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;
  struct conn_switch_req conn_switch;

  if (argc != 2) {
    DEBUG_PRINTF("Invalid CONN_SWITCH command: needs 2 argument\n");
    goto err;
  }

  if (atoi(argv[0]) < 0 || atoi(argv[0]) >= 4) {
    DEBUG_PRINTF("Invalid index\n");
    goto err;
  }
  conn_switch.sess_id = atoi(argv[0]);

  if (strcmp(argv[1], "ON") && strcmp(argv[1], "OFF")) {
    DEBUG_PRINTF("Invalid switch, please refer: ON/OFF\n");
    goto err;
  } else {
    conn_switch.start_enable = strcmp(argv[1], "ON") == 0 ? 1 : 0;
  }

  ret = ioctl(fp, ATBM_CONN_SWITCH, &conn_switch);
  if (ret) {
    goto err;
  }

  return 0;
err:
  return -1;
}

int add_wk_ssid_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;
  struct wakeup_ssid_req wk_ssid;

  if (argc != 1) {
    DEBUG_PRINTF("Invalid ADD_WK_SSID command: needs 1 argument\n");
    goto err;
  }

  if (strlen(argv[0]) > 32) {
    DEBUG_PRINTF("Invalid ssid\n");
    goto err;
  }

  wk_ssid.ssid_len = strlen(argv[0]);
  memcpy(wk_ssid.ssid, argv[0], wk_ssid.ssid_len);

  ret = ioctl(fp, ATBM_SET_WK_SSID, &wk_ssid);
  if (ret) {
    goto err;
  }

  return 0;
err:
  return -1;
}

int del_wk_ssid_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;
  struct wakeup_ssid_req wk_ssid;

  if (argc != 1) {
    DEBUG_PRINTF("Invalid DEL_WK_SSID command: needs 1 argument\n");
    goto err;
  }

  if (strlen(argv[0]) > 32) {
    DEBUG_PRINTF("Invalid ssid\n");
    goto err;
  }

  wk_ssid.ssid_len = strlen(argv[0]);
  memcpy(wk_ssid.ssid, argv[0], wk_ssid.ssid_len);

  ret = ioctl(fp, ATBM_CLR_WK_SSID, &wk_ssid);
  if (ret) {
    goto err;
  }

  return 0;
err:
  return -1;
}

int wkup_event_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;
  u32 wk_event = 0;

  if (argc != 1) {
    DEBUG_PRINTF("Invalid WKUP_EVENT command: needs 1 argument\n");
    goto err;
  }

  if (strcmp(argv[0], "TCP_PATTERN") == 0) {
    wk_event = WAKEUP_TCP_NETPATTERN;
  } else if (strcmp(argv[0], "UDP_PATTERN") == 0) {
    wk_event = WAKEUP_UDP_NETPATTERN;
  } else if (strcmp(argv[0], "WK_SSID") == 0) {
    wk_event = WAKEUP_WK_SSID;
  } else if (strcmp(argv[0], "MAGIC_PKT") == 0) {
    wk_event = WAKEUP_MAGIC_PKT;
  } else if (strcmp(argv[0], "NONE") == 0) {
    wk_event = 0;
  } else {
    DEBUG_PRINTF("Invalid wk event: %s\n", argv[0]);
    goto err;
  }

  ret = ioctl(fp, ATBM_WKUP_EVENT, wk_event);
  if (ret) {
    goto err;
  }

  return 0;
err:
  return -1;
}

int auto_reconn_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;
  struct auto_reconnect_req auto_reconn;

  if (argc != 1) {
    DEBUG_PRINTF("Invalid AUTO_RECONN command: needs 1 argument\n");
    goto err;
  }

  auto_reconn.auto_enable = atoi(argv[0]) == 0 ? 0 : 1;

  ret = ioctl(fp, ATBM_AUTO_RECONN, &auto_reconn);
  if (ret) {
    goto err;
  }

  return 0;
err:
  return -1;
}

int customer_cmd_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;
  struct customer_cmd_req customer_cmd;

  if (argc != 1 && argc != 2) {
    DEBUG_PRINTF("Invalid CUSTOMER_CMD command: needs 1 or 2 arguments\n");
    goto err;
  }

  customer_cmd.cmd_id = atoi(argv[0]);
  if (argc > 1) {
    // reserved
  }

  ret = ioctl(fp, ATBM_CUSTOMER_CMD, &customer_cmd);
  if (ret) {
    goto err;
  }

  return 0;
err:
  return -1;
}

int customer_cert_save_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;
  struct customer_cert_save_req req_info;

  if (argc != 2) {
    DEBUG_PRINTF("Invalid CUSTOMER_CERT command: needs 2 arguments\n");
    goto err;
  } else {
    FILE *file = NULL;
    int len = 0;
    int read_len = 0;
    int ret_size = 0;
    int first_write = 0;
    int cert_type = 0;

    if (strcmp(argv[0], "CERT") == 0) {
      cert_type = 0;
    } else if (strcmp(argv[0], "PRIV_KEY") == 0) {
      cert_type = 1;
    } else if (strcmp(argv[0], "ROOT_CA") == 0) {
      cert_type = 2;
    } else {
      DEBUG_PRINTF("Invalid cert_type: %s\n", argv[0]);
      goto err;
    }

    memset(&req_info, 0, sizeof(req_info));
    req_info.cert_type = cert_type;
    req_info.first = 1;

    file = fopen(argv[1], "rb");
    if (NULL == file) {
      DEBUG_PRINTF("Line:%d invalid cert path %s\n", __LINE__, argv[1]);
      goto err;
    }

    fseek(file, 0, SEEK_END);
    len = ftell(file);
    fseek(file, 0, SEEK_SET);
    DEBUG_PRINTF("file:%s  size:%d bytes\n", argv[1], len);

    if (len > 2048) {
      DEBUG_PRINTF("Line:%d The file length is invalid\n", __LINE__);
      goto err;
    }

    while (!feof(file) && len > 0) {
      if (len > 1024) {
        read_len = 1024;
        req_info.remain = 1;
      } else {
        read_len = len;
        req_info.remain = 0;
      }

      ret_size = fread(req_info.data, 1, read_len, file);
      if (ret_size != read_len) {
        DEBUG_PRINTF("Line:%d returned bytes not match\n", __LINE__);
        fclose(file);
        goto err;
      }

      req_info.length = read_len;
      len -= read_len;

      ret = ioctl(fp, ATBM_CUSTOMER_CERT, (unsigned int)&req_info);
      if (ret) {
        DEBUG_PRINTF("Line:%d write cert err\n", __LINE__);
        fclose(file);
        goto err;
      }

      if (req_info.first) {
        req_info.first = 0;
      }
    }

    fclose(file);
    DEBUG_PRINTF("save cert over.\n");
  }

  return 0;
err:
  return -1;
}

int start_check_alive_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;
  struct check_alive_req cmd;

  if (argc != 2) {
    DEBUG_PRINTF(
        "Invalid CHECK_ALIVE command: 2 arguments: period tmo_count\n");
    goto err;
  }

  cmd.period = atoi(argv[0]);
  cmd.tmo_cnt = atoi(argv[1]);

  ret = ioctl(fp, ATBM_CHECK_ALIVE, &cmd);
  if (ret) {
    goto err;
  }

  return 0;
err:
  return -1;
}

int direct_trans_buffer_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;
  int str_length = 0;
  struct direct_trans_buffer_req cmd;

  if (argc != 1) {
    DEBUG_PRINTF(
        "Invalid DIRECT_TRANS_BUFFER command: 1 argument: buffer data\n");
    goto err;
  }

  str_length = strlen(argv[0]);
  if (str_length > 1600) {
    DEBUG_PRINTF("str too long\n");
    goto err;
  }

  strcpy(cmd.trans, argv[0]);
  DEBUG_PRINTF("str: %s\n", argv[0]);

  ret = ioctl(fp, ATBM_DIRECT_BUFFER, &cmd);
  if (ret) {
    goto err;
  }
  DEBUG_PRINTF("get new str: %s\n", cmd.trans);

  return 0;
err:
  return -1;
}

int set_control_debug_cmd(int fp, int argc, char *argv[]) {
  int ret = 0;

  if (argc < 1) {
    DEBUG_PRINTF("Invalid CONTROL_DEBUG command: needs at least 1 argument\n");
    goto err;
  } else {
    int control_debug = atoi(argv[0]);
    DEBUG_PRINTF("new control debug: 0x%x\n", control_debug);

    ret = ioctl(fp, ATBM_CONTROL_DEBUG, control_debug);
    if (ret) {
      goto err;
    }
  }

  return 0;
err:
  return -1;
}

int force_reboot_cmd(int fp, int argc, char *argv[]) {
  return ioctl(fp, ATBM_FORCE_REBOOT, 0);
}

int fast_cfg_recv_cmd(int fp, int argc, char *argv[]) {
  int arg_cnt = 0;
  int enable = 0;
  int time = 0;

  if ((0 == argc) || (argc > 4) || (argc % 2)) {
    DEBUG_PRINTF("fast_cfg_recv variables:\n"
                 "  enable (enable or diable fast cfg recv, refer: 0/1)\n"
                 "  time (duration time fast cfg recv mode, refer: 0 ~ "
                 "65535(s) 0:always)\n");
    return -1;
  }

  memset(&fast_cfg_recv_set, 0, sizeof(fast_cfg_recv_set));

  while (arg_cnt < argc) {
    if (!strcmp(argv[arg_cnt], "enable")) {
      arg_cnt++;
      enable = atoi(argv[arg_cnt]);
      if ((enable < 0) || (enable > 1)) {
        DEBUG_PRINTF("invalid enable value(%d)\n", enable);
        return -1;
      }
      fast_cfg_recv_set.enable = enable;
    } else if (!strcmp(argv[arg_cnt], "time")) {
      arg_cnt++;
      time = atoi(argv[arg_cnt]);
      if ((time < 0) || (time > 65535)) {
        DEBUG_PRINTF("invalid time value(%d)\n", time);
        return -1;
      }
      fast_cfg_recv_set.time = time;
    }
    arg_cnt++;
  }

  if (ioctl(fp, ATBM_FAST_CFG_RECV, (unsigned int)(&fast_cfg_recv_set))) {
    return -1;
  }

  return 0;
}

int fast_cfg_send_cmd(int fp, int argc, char *argv[]) {
  int arg_cnt = 0;
  int enable = 0;
  int time = 0;
  int ssid_len;
  int pwd_len;

  if ((0 == argc) || (argc > 8) || (argc % 2)) {
    DEBUG_PRINTF("fast_cfg_recv variables:\n"
                 "  enable (enable or diable fast cfg recv, refer: 0/1)\n"
                 "  time (duration time fast cfg recv mode, refer: 0 ~ "
                 "65535(s) 0:always)\n"
                 "  ssid (fast_cfg objective ssid)\n"
                 "  pwd (fast_cfg objective password)\n");
    return -1;
  }

  memset(&fast_cfg_send_set, 0, sizeof(fast_cfg_send_set));

  while (arg_cnt < argc) {
    if (!strcmp(argv[arg_cnt], "enable")) {
      arg_cnt++;
      enable = atoi(argv[arg_cnt]);
      if ((enable < 0) || (enable > 1)) {
        DEBUG_PRINTF("invalid enable value(%d)\n", enable);
        return -1;
      }
      fast_cfg_send_set.enable = enable;
    } else if (!strcmp(argv[arg_cnt], "time")) {
      arg_cnt++;
      time = atoi(argv[arg_cnt]);
      if ((time < 0) || (time > 65535)) {
        DEBUG_PRINTF("invalid time value(%d)\n", time);
        return -1;
      }
      fast_cfg_send_set.time = time;
    } else if (!strcmp(argv[arg_cnt], "ssid")) {
      arg_cnt++;
      ssid_len = strlen(argv[arg_cnt]);
      if ((ssid_len < 0) || (ssid_len > 32)) {
        DEBUG_PRINTF("invalid ssid len value(%d)\n", ssid_len);
        return -1;
      }
      fast_cfg_send_set.ssid_len = ssid_len;
      if (ssid_len) {
        memcpy(fast_cfg_send_set.ssid, argv[arg_cnt], ssid_len);
      }
    } else if (!strcmp(argv[arg_cnt], "pwd")) {
      arg_cnt++;
      pwd_len = strlen(argv[arg_cnt]);
      if ((pwd_len < 0) || (pwd_len > 64)) {
        DEBUG_PRINTF("invalid pwd len value(%d)\n", pwd_len);
        return -1;
      }
      fast_cfg_send_set.password_len = pwd_len;
      if (pwd_len) {
        memcpy(fast_cfg_send_set.password, argv[arg_cnt], pwd_len);
      }
    }
    arg_cnt++;
  }

  if (ioctl(fp, ATBM_FAST_CFG_SEND, (unsigned int)(&fast_cfg_send_set))) {
    return -1;
  }

  return 0;
}

int atbm_set_port_filter(int port, int enable) {
  int ret = 0;
  char tmp_buf[16] = {'\0'};

  snprintf(tmp_buf, sizeof(tmp_buf), "%d", enable);
  ret = set_filter("ICMP", tmp_buf);
  DEBUG_PRINTF("set_filter ICMP ret: %d\n", ret);

  snprintf(tmp_buf, sizeof(tmp_buf), "%d", port);
  ret = set_filter("DES", tmp_buf); // 保活服务器端口
  DEBUG_PRINTF("set_filter DES ret: %d\n", ret);
  ret = set_filter("SRC", tmp_buf);
  DEBUG_PRINTF("set_filter SRC ret: %d\n", ret);

  ret = enable_filter();

  return ret;
}

void atbm_driver_go_sleep(void)
{
  if (is_insmod_driver) {
    char is_empty = 0;
    sem_wait(&sem);
    ioctl(g_fp, ATBM_PRE_RMMOD, 0);
    while (1) {
      ioctl(g_fp, ATBM_TX_EMPTY, &is_empty);
      if (is_empty) {
        break;
      }
      usleep(100 * 1000);
    }
    ioctl(g_fp, ATBM_FW_SLEEP, 0);
    enable_driver(0);
    sem_post(&sem);
    DEBUG_PRINTF("post sem...\n");
  }
}

int user_help(int fp, int argc, char *argv[]);

struct COMMAND_STR command[] = {
    {"set_ps", set_ps_cmd, "<variable> <value> = set power save variables"},
    {"list_networks", list_networks_cmd, "= list configured network"},
    {"set_network", set_network_cmd,
     "<variable> <value> = set network variables"},
    {"enable_network", enable_network_cmd, "= enable network"},
    {"clear_network", clear_network_cmd, "<= clear configured network"},
    {"status", get_status_send, "= get current connect status"},
    {"set_ip", set_ip_cmd,
     "= set ip got from enable_network command. auto set ip default"},
    {"set_ip_auto", set_ip_auto_cmd, "<is_auto> = set ip_autoset mode"},
    {"list_filters", list_filters_cmd, "= list enabled filter"},
    {"set_filter", set_filter_cmd,
     "<variable> <tcp ports ...> = set tcp filter ports"},
    {"enable_filter", enable_filter_cmd, "= enable filter set"},
    {"scan", scan_cmd, "= request new BSS scan"},
    {"scan_results", scan_results_cmd, "= get latest scan results"},
    {"wifi_mode", set_wifi_mode_cmd, "<wifi mode> = set wifi mode, AP or STA"},
    {"clear_ap_cfg", clear_ap_cfg_cmd, "= clear ap cfg"},
    {"set_ap_cfg", set_ap_cfg_cmd, "<variable> <value> = set ap cfg"},
    {"enable_ap_cfg", enable_ap_cfg_cmd, "= enable ap cfg"},
    {"switch_channel", switch_channel_cmd,
     "<channel id> = switch channel id 1~14"},
    {"set_country", set_country_cmd, "<country name> = set country"},
    {"get_country", get_country_cmd, "= get country"},
    {"get_stalist", get_stalist_cmd, "= get sta list"},
    {"smart_cfg_start", smart_cfg_start_cmd, "= start smart cfg mode"},
    {"smart_cfg_stop", smart_cfg_stop_cmd, "= stop smart cfg mode"},
    {"ap_touch_start", ap_touch_start_cmd,
     "<touch mode> = start ap touch with AP_NOTIFY or NO_NOTIFY, default "
     "NO_NOTIFY"},
    {"ap_touch_stop", ap_touch_stop_cmd, "= stop ap touch"},
    {"set_etf_cfg", set_etf_cfg_cmd, "<variable> <value> = set etf cfg"},
    {"etf_start_tx", etf_start_tx_cmd, "= start etf tx"},
    {"etf_stop_tx", etf_stop_tx_cmd, "= stop etf tx"},
    {"etf_singletone", etf_singletone_cmd, "= send sigletone"},
    {"etf_start_rx", etf_start_rx_cmd, "= start etf rx"},
    {"etf_start_rx_no_drop", etf_start_rx_no_drop_cmd,
     "= start etf rx without drop"},
    {"etf_stop_rx", etf_stop_rx_cmd, "= stop etf rx"},
    {"etf_reset_rx", etf_reset_rx_cmd, "= reset etf rx"},
    {"adaptive", adaptive_cmd, "<adaptive flag> = set adaptive ON or OFF"},
    {"get_ver", get_ver_cmd, "= get hw firmware verison"},
    {"get_sdk_ver", get_sdk_ver_cmd, "= get sdk version"},
    {"update_fw", update_fw_cmd, "<firmware path> = update firmware"},
    {"listen_itvl", listen_itvl_cmd,
     "<itvl> = set beacon listen interval unit:100ms"},
    {"fw_cmd", fw_at_cmd, "<at_cmd> = send fw at cmd"},
    {"fw_print", fw_print_cmd, "<enable> = set if enable fw debug print"},
    {"set_rmmod_auto", set_rmmod_auto_cmd,
     "<auto> = set rmmod auto mode 1 or 0"},
    {"get_rate", get_rate_cmd, "= get transmit rate"},
    {"send_ipc_data", send_ipc_data_cmd,
     "= send ipc data to server.\nserver=xxx.xxx.xxx.xxx port=xx [optional], "
     "data=xxxxxxxxxx "
     "[necessary], ipc data length max 999 bytes\n"},
    {"get_rssi", get_rssi_cmd, "= get rssi value"},
    {"get_time", get_time_cmd, "= get current time"},
    {"clear_wifi_cfg", clear_wifi_cfg_cmd, "= clear wifi config"},
    {"get_etf_rx_info", get_etf_rx_info_cmd,
     "= get current etf rx info, such as success count, fcs error count and "
     "rssi"},
    {"add_netpattern", add_netpattern_cmd,
     "<index> <ip> <port> <protocol> <pattern_data> = add netpattern info used "
     "to wkup wifi"},
    {"del_netpattern", del_netpattern_cmd,
     "<index> = del netpattern info [index]"},
    {"add_conn", add_conn_cmd,
     "<index> <ip> <port> <itvl> <protocol> <text> = set conn param info"},
    {"del_conn", del_conn_cmd, "<index> = del conn param info [index]"},
    {"conn_lose", conn_lose_cmd, "= get conn lose map"},
    {"conn_switch", conn_switch_cmd,
     "<index> ON/OFF = switch conn[index] status"},
    {"add_wk_ssid", add_wk_ssid_cmd,
     "<ssid> = add wkup ssid used to wakeup wifi"},
    {"del_wk_ssid", del_wk_ssid_cmd, "<ssid> = del wkup ssid set before"},
    {"wkup_event", wkup_event_cmd,
     "TCP_PATTERN/UDP_PATTERN/WK_SSID/MAGIC_PKT/NONE = set wakeup wifi event "
     "support, NONE clear "
     "all set"},
    {"auto_reconn", auto_reconn_cmd,
     "<enable> = set wifi auto reconnect when connect lost, refer:0/1"},
    {"customer_cmd", customer_cmd_cmd,
     "<cmd_id> <cmd_buff> = send customer command"},
    {"customer_cert", customer_cert_save_cmd,
     "<type> <file_path> = save cert info to 6441"},
    {"start_check_alive", start_check_alive_cmd,
     "<period> <tmo_count> = start 6441-host soft alive sync"},
    {"direct_trans", direct_trans_buffer_cmd, "<str> = direct data buffer"},
    {"control_debug", set_control_debug_cmd,
     "<debug_mask> = set control debug mask to limit debug message show"},
    {"force_reboot", force_reboot_cmd, "= control 6441 reboot"},
    {"fast_cfg_recv", fast_cfg_recv_cmd,
     "<variable> <value>= fast config mode recv"},
    {"fast_cfg_send", fast_cfg_send_cmd,
     "<variable> <value>= fast config mode send"},

    {"help", user_help, "= show usage help"},
};

int user_help(int fp, int argc, char *argv[]) {
  int i = 0;
  int cnt = sizeof(command) / sizeof(command[0]);

  for (i = 0; i < cnt; i++)
    DEBUG_PRINTF("%s\n%s\n\n", command[i].cmd, command[i].descript);

  return 0;
}

static int tokenize_cmd(char *cmd, char *argv[], int is_at_cmd) {
  char *pos;
  int argc = 0;

  pos = cmd;
  for (;;) {
    while (*pos == ' ')
      pos++;
    if (*pos == '\0')
      break;
    if (*pos == '"') {
      argv[argc] = pos + 1;
      char *pos2 = strrchr(pos, '"');
      if (pos2) {
        *pos2 = '\0';
        pos = pos2 + 1;
      }
    } else {
      argv[argc] = pos;
    }
    argc++;
    if (argc == MAX_ARGS)
      break;
    if (is_at_cmd) {
      if (argc > 1)
        break;
    }
    while (*pos != '\0' && *pos != ' ')
      pos++;
    if (*pos == ' ')
      *pos++ = '\0';
  }

  return argc;
}

int cmd_parse(int fp, char *arg) {
  int i = 0;
  int cnt = sizeof(command) / sizeof(command[0]);
  char *argv[MAX_ARGS];
  int argc;
  int ret = 0;

  if (!strncmp(arg, "fw_cmd ", strlen("fw_cmd "))) {
    argc = tokenize_cmd(arg, argv, 1);
    if (argc != 2) {
      DEBUG_PRINTF("FW_CMD err.\n");
      return -1;
    }
  } else {
    argc = tokenize_cmd(arg, argv, 0);
  }
  if (argc) {
    for (i = 0; i < cnt; i++) {
      if (!strcmp(command[i].cmd, argv[0])) {
        ret = command[i].cmd_send(fp, argc - 1, &argv[1]);
        if (ret) {
          DEBUG_PRINTF("FAIL\n");
        } else {
          DEBUG_PRINTF("OK\n");
        }
        return 0;
      }
    }
  }

  DEBUG_PRINTF("Line:%d not match command\n", __LINE__);
  return -1;
}

static int insmod_driver(int test_insmod)
{
	int ret = 0;
	int flags = 0;
	int ip_set = 0;
	struct status_info status_info;
	g_fp = -1;

	sem_wait(&sem_status);
	g_fp = open("/dev/atbm_ioctl", O_RDWR);
	while (g_fp < 0) {
		DEBUG_PRINTF("open /dev/atbm_ioctl fail, maybe prefetch bit is not cleared\n");
		g_fp = open("/dev/atbm_ioctl", O_RDWR);
		usleep(10 * 1000);
	}
	sem_post(&sem_status);
	is_insmod_driver = 1;
	driver_can_rmmod = 0;

	fcntl(g_fp, F_SETOWN, getpid());
	flags = fcntl(g_fp, F_GETFL);
	fcntl(g_fp, F_SETFL, flags | FASYNC);

#if 0
	memset(&status_info, 0, sizeof(struct status_info));
	ret = ioctl(g_fp, ATBM_STATUS, (unsigned int)(&status_info));
	if (ret) {
		DEBUG_PRINTF("get initial wifi status fail.\n");
		return -1;
	}
	memcpy(&status.event, &status_info.con_event, sizeof(status.event));
	wifi_mode = status_info.wifimode;
	DEBUG_PRINTF("[WIFI_MODE] %s\n", wifi_mode ? "AP" : "STA");

	if (wifi_mode) {
		ip_set = 1;
	} else {
		if (status_info.bconnect) {
			status.is_connected = 1;
			ip_set = 1;
			DEBUG_PRINTF("CTRL-EVENT-STATE-CONNECTED - Connection to ");
			MAC_printf(status.event.bssid);
			DEBUG_PRINTF(" completed done\n");
		}
		if (NULL != m_wifi_cb) {
			m_wifi_cb(RK_WIFI_State_CONNECTED, NULL);
		}
	}

	if (ip_set) {
		if (ip_set_auto) {
			char cmd[30];
			struct in_addr ip_addr;

			DEBUG_PRINTF("auto set ip...\n");
			ip_addr.s_addr = status.event.ipaddr;
			sprintf(cmd, "ifconfig wlan0 %s", inet_ntoa(ip_addr));
			system(cmd);
			system("ifconfig wlan0 up");
			if (NULL != m_wifi_cb) {
				m_wifi_cb(RK_WIFI_State_DHCP_OK, NULL);
			}
		}
	}
#endif
	driver_switch = 1;
	return 0;
}

int rmmod_driver(void)
{
  system("ifconfig wlan0 down");
  close(g_fp);
  g_fp = -1;
  system("rmmod atbm6041_wifi_sdio.ko");
  is_insmod_driver = 0;
  driver_can_rmmod = 0;
  return 0;
}

int enable_driver(int on) {
  if (is_insmod_driver == on) {
    return 0;
  }

  if (on) {
    return insmod_driver(0);
  } else {
    return rmmod_driver();
  }
}

void *get_power_status_func(void *arg) {
  int fp = (int)arg;
  int ret = 0;
  char data = 0;

  while (1) {
    ret = read(fp, &data, 1);
    if (ret < 0) {
      if (!thread_run) {
        break;
      }
      continue;
    }

    if (data) {
      sem_wait(&sem);
      enable_driver(data);
      sem_post(&sem);
      if (driver_switch) {
        raise(SIGIO);
        driver_switch = 0;
      }
    }
  }
  return NULL;
}

void *get_command_func(void *arg) {
  struct sockaddr_un ser_un;
  int socket_fd = 0;
  int connect_fd = 0;
  int ret = 0;
  char recall[] = "OK";

  socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (socket_fd <= 0) {
    DEBUG_PRINTF("open socket err\n");
    return NULL;
  }

  unlink(SER_SOCKET_PATH);

  memset(&ser_un, 0, sizeof(ser_un));
  ser_un.sun_family = AF_UNIX;
  strcpy(ser_un.sun_path, SER_SOCKET_PATH);
  ret = bind(socket_fd, (struct sockaddr *)&ser_un, sizeof(struct sockaddr_un));
  if (ret < 0) {
    DEBUG_PRINTF("bind err %s %s\n", SER_SOCKET_PATH, strerror(errno));
    return NULL;
  }

  ret = listen(socket_fd, 5);
  if (ret < 0) {
    DEBUG_PRINTF("listen err\n");
    return NULL;
  }

  while (1) {
    connect_fd = accept(socket_fd, NULL, NULL);
    if (connect_fd < 0) {
      continue;
    }

    memset(cmd_line, 0, sizeof(cmd_line));
    while (1) {
      ret = read(connect_fd, cmd_line, sizeof(cmd_line));
      if (ret > 0) {
        break;
      }
    }

    write(connect_fd, recall, strlen(recall) + 1);
    close(connect_fd);
    DEBUG_PRINTF("cmd_line: %s\n", cmd_line);
    sem_post(&sem_sock_sync);

    if (!strcmp(cmd_line, "quit")) {
      break;
    }
  }

  close(socket_fd);
  return NULL;
}

static atbm_con_msg(void)
{
	int reason;

	if (status.is_connected) {
		struct in_addr ip_addr;
		ip_addr.s_addr = status.event.ipaddr;
		if (wifi_mode) {
			DEBUG_PRINTF("AP Mode.\n");
		} else {
			DEBUG_PRINTF("CTRL-EVENT-STATE-CONNECTED - Connection to ");
			MAC_printf(status.event.bssid);
			DEBUG_PRINTF(" completed done\n");
			if (NULL != m_wifi_cb) {
				m_wifi_cb(RK_WIFI_State_CONNECTED, NULL);
			}
		}

		DEBUG_PRINTF("ip addr:%s\n", inet_ntoa(ip_addr));
		DEBUG_PRINTF("ip_set_auto:%d\n", ip_set_auto);

		if (ip_set_auto) {
			char cmd[64];
			struct in_addr ip_addr;
	
			ip_addr.s_addr = status.event.ipaddr;
			sprintf(cmd, "ifconfig wlan0 %s", inet_ntoa(ip_addr));
			DEBUG_PRINTF("cmd:%s\n", cmd);
			system(cmd);

			ip_addr.s_addr = status.event.gwaddr;
			sprintf(cmd, "echo \"nameserver %s\" > /etc/resolv.conf", inet_ntoa(ip_addr));
			DEBUG_PRINTF("cmd:%s\n", cmd);
			system(cmd);
	
			system("ifconfig wlan0 up");
	
			ip_addr.s_addr = status.event.gwaddr;
			sprintf(cmd, "route add default gw %s", inet_ntoa(ip_addr));
			DEBUG_PRINTF("cmd:%s\n", cmd);
			system(cmd);
			if (NULL != m_wifi_cb) {
				m_wifi_cb(RK_WIFI_State_DHCP_OK, NULL);
			}
		}
	
	} else {
		if (!wifi_mode) {
			DEBUG_PRINTF("CTRL-EVENT-STATE-DISCONNECTED\n");
		}
		if (NULL != m_wifi_cb) {
			m_wifi_cb(RK_WIFI_State_DISCONNECTED, NULL);
		}
		//system("ifconfig wlan0 down");
	}
}

static void atbm_rmmod_msg(void)
{
	if (status.driver_mode) {
		driver_can_rmmod = 0;
		DEBUG_PRINTF(">> linux driver can not rmmod ...\n");
	} else {
		driver_can_rmmod = 1;
		DEBUG_PRINTF(">> linux driver can rmmod ...\n");
		if (rmmod_auto && driver_can_rmmod) {
			atbm_driver_go_sleep();
		}
	}
}

static void atbm_scan_r_msg(void)
{
	DEBUG_PRINTF("Scan Completed...\n");
	DEBUG_PRINTF("CTRL-EVENT-SCAN-RESULTS\n");
}

static void atbm_wake_host_msg(void)
{
	RK_WIFI_RUNNING_State_e reason;

	switch (status.driver_mode) {
	case WAKEUP_IO:
		reason = RK_WIFI_WAKEUP_IO;
		DEBUG_PRINTF("[WAKEUP] IO wakeup\n");
		break;
	case WAKEUP_NETWORK:
		reason = RK_WIFI_WAKEUP_NETWORK;
		DEBUG_PRINTF("[WAKEUP] NETWORK wakeup\n");
		break;
	case WAKEUP_CONNECT:
		reason = RK_WIFI_WAKEUP_CONNECT;
		DEBUG_PRINTF("[WAKEUP] AP RECONNECT wakeup\n");
		break;
	case WAKEUP_KEEPALIVE:
		reason = RK_WIFI_WAKEUP_KEEPALIVE;
		DEBUG_PRINTF("[WAKEUP] KEEPALIVE wakeup\n");
		break;
	case WAKEUP_DETECT_PIR:
		reason = RK_WIFI_WAKEUP_DETECT_PIR;
		DEBUG_PRINTF("[WAKEUP] PIR DETECT wakeup\n");
		break;
	default:
		DEBUG_PRINTF("[WAKEUP] UNKNOWN wakeup\n");
		break;
	}

	/* 回调唤醒原因 */
	if (NULL != m_wifi_cb) {
		m_wifi_cb(reason, NULL);
	}
}

static void atbm_disconnect_msg(void)
{
	RK_WIFI_RUNNING_State_e reason;

	switch (status.driver_mode) {
	case DISCONN_BSS_LOST:
		reason = RK_WIFI_State_DISCONNECTED_LOST_BEACON;
		DEBUG_PRINTF("[DISCONN] BSS lost\n");
		break;
	case DISCONN_HOST_DONE:
		reason = RK_WIFI_State_DISCONNECTED_BY_HOST;
		DEBUG_PRINTF("[DISCONN] Host call disconnect\n");
		break;
	case DISCONN_AP_DEAUTH:
		reason = RK_WIFI_State_DISCONNECTED_BY_ROUTER;
		DEBUG_PRINTF("[DISCONN] AP deauth\n");
		break;
	default:
		reason = RK_WIFI_State_DISCONNECTED;
		DEBUG_PRINTF("[DISCONN] UNKNOWN disconn\n");
		break;
	}

	/* 回调连接掉线原因 */
	if (NULL != m_wifi_cb) {
		m_wifi_cb(reason, NULL);
	}
}

static void atbm_connect_fail_msg(void)
{
	RK_WIFI_RUNNING_State_e reason;

	switch (status.driver_mode) {
	case CONN_NOT_FOUND_SSID:
		reason = RK_WIFI_State_CONNECTFAILED_NOT_FOUND;
		DEBUG_PRINTF("[CONN] Not found SSID\n");
		break;
	case CONN_JOIN_FAIL:
		reason = RK_WIFI_State_CONNECTFAILED;
		DEBUG_PRINTF("[CONN] Join auth fail\n");
		break;
	case CONN_NOT_GET_IP:
		reason = RK_WIFI_State_DHCP_ERR;
		DEBUG_PRINTF("[CONN] Not get ip by DHCP\n");
		break;
	default:
		reason = RK_WIFI_State_UNKNOW_ERR;
		DEBUG_PRINTF("[CONN] UNKNOWN conn\n");
		break;
	}
	
	/* 回调连接错误原因 */
	if (NULL != m_wifi_cb) {
		m_wifi_cb(reason, NULL);
	}
}

static void atbm_customer_msg(void)
{
	RK_WIFI_RUNNING_State_e reason;
	RK_WIFI_INFO_Connection_s wifi_info;

	wifi_info = g_wifi_Info;

	memset(&wifi_info.lp_data, 0, 32);
	DEBUG_PRINTF("status.event_buffer: %s\n", status.event_buffer);

	reason = RK_WIFI_State_LP_EVENT_TYPE;

	if (0 == strncmp(ATBM_RESET_KEY_LONG_PRESS_EVENT, status.event_buffer,
			 strlen(status.event_buffer))) {
		wifi_info.lp_event_type = RK_WIFI_LP_LONG_PRESS;
	}

	if (0 == strncmp(ATBM_RESET_KEY_SHORT_PRESS_EVENT, status.event_buffer,
					 strlen(status.event_buffer))) {
		wifi_info.lp_event_type = RK_WIFI_LP_SHORT_PRESS;
	} 

	if (0 == strncmp(ATBM_START_KEEPALIVE_SUCCESS_EVENT,
						status.event_buffer,
						strlen(status.event_buffer))) {
		DEBUG_PRINTF("congratulations, start keepalive success!\n");
		wifi_info.lp_event_type = RK_WIFI_LP_KEEPALIVE_OK;
		/*
		int ret = 0;
		ret = fw_at("AT+WIFI_LISTEN_ITVL 10");
		DEBUG_PRINTF("fw_at AT+WIFI_LISTEN_ITVL ret is %d\n", ret);
		ret = fw_at("AT+LIGHT_SLEEP 1");
		DEBUG_PRINTF("fw_at AT+LIGHT_SLEEP ret is %d\n", ret);
		atbm_driver_go_sleep();
		DEBUG_PRINTF("atbm_driver_go_sleep\n");
		*/
	}

	if (0 == strncmp(ATBM_START_KEEPALIVE_FAILED_EVENT,
					 status.event_buffer,
					 strlen(status.event_buffer))) {
		DEBUG_PRINTF("sorry, start keepalive failed...\n");
		// TODO:起保活服务失败时的处理
		wifi_info.lp_event_type = RK_WIFI_LP_KEEPALIVE_FAILED;
	}

	if (NULL != strstr(status.event_buffer, ATBM_BATTERY_LEVEL_EVENT)) {

		/* 电池电量解析，上报 */
		char *battery_level_start = NULL;
		int battery_level_value = 0;
		battery_level_start = status.event_buffer + strlen(ATBM_BATTERY_LEVEL_EVENT);
		battery_level_value = atoi(battery_level_start);
		DEBUG_PRINTF("get battery level:%s, value: %d\n", battery_level_start, atoi(battery_level_start));


		wifi_info.lp_event_type = RK_WIFI_LP_BAT_LEVEL;
		wifi_info.lp_data[0] = battery_level_value;
	}

	if (NULL != strstr(status.event_buffer, ATBM_WAKEUP_REASON_EVENT)) {
		/* 唤醒原因解析，上报 */
		char *wakeup_reason_start = NULL;
		int wakeup_reason_value = 0;
		wakeup_reason_start = status.event_buffer + strlen(ATBM_WAKEUP_REASON_EVENT);
		wakeup_reason_value = atoi(wakeup_reason_start);
		DEBUG_PRINTF("get wake up reason:%s, value: %d\n", wakeup_reason_start, atoi(wakeup_reason_start));

		wifi_info.lp_event_type = RK_WIFI_LP_WAKEUP_REASON;
		wifi_info.lp_data[0] = wakeup_reason_value;
	}

	if (NULL != strstr(status.event_buffer, ATBM_PIR_DETECT_EVENT)) {
		reason = RK_WIFI_LP_PIR_DETECT;
		/* pir事件解析，上报 */
		char *pir_detect_start = NULL;
		int pir_detect_value = 0;
		pir_detect_start = status.event_buffer + strlen(ATBM_PIR_DETECT_EVENT);
		pir_detect_value = atoi(pir_detect_start);
		DEBUG_PRINTF("get pir detect:%s, value: %d\n", pir_detect_start, atoi(pir_detect_start));
		wifi_info.lp_event_type = RK_WIFI_LP_PIR_DETECT;
		wifi_info.lp_data[0] = pir_detect_value;
	}

	if (NULL != strstr(status.event_buffer, ATBM_UTC_SUCCESS)) {
		/* UTC时间解析解析，上报 */
		pthread_mutex_lock(&atbm_sync_lock);
		char *utc_start = NULL;
		utc_start = status.event_buffer + strlen(ATBM_UTC_SUCCESS);
		utctime = atoi(utc_start);
		DEBUG_PRINTF("utc time is %lld \n",utctime);
		atbm_sync_flag = 1;
		pthread_cond_signal(&atbm_sync_cond);
		pthread_mutex_unlock(&atbm_sync_lock);
	}
	if (NULL != strstr(status.event_buffer, ATBM_UTC_ERROR)) {
	/* UTC时间解析解析，上报 */
		pthread_mutex_lock(&atbm_sync_lock);
		DEBUG_PRINTF("get utc failed pls retry! \n");
		utctime = -1;
		pthread_cond_signal(&atbm_sync_cond);
		pthread_mutex_unlock(&atbm_sync_lock);
	}


	/* 回调连接错误原因 */
	if (NULL != m_wifi_cb) {
		m_wifi_cb(reason, &wifi_info);
	}
}

void ioctl_msg_func(int sig_num)
{
	int len = 0;
	DEBUG_PRINTF("----msg enter-------\n");
  
	sem_wait(&sem_status);
	while (1) {
		DEBUG_PRINTF("read ... ...\n");
		len = read(g_fp, &status, sizeof(status));

		if (len < (int)(sizeof(status))) {
			DEBUG_PRINTF("read(read_len:%d) connect stat error, err:(%d, %s)\n", len,
			           errno, strerror(errno));
			break;
		} 
		DEBUG_PRINTF("recv type: %d, read_len: %d, len_check_ok: %d\n", status.type, len, len == sizeof(status));

		/* 0: connect msg;
		 * 1: rmmod;
		 * 2: scan complet,
		 * 3: wakeup host reason,
		 * 4: disconnect reason,
		 * 5: connect fail reason,
		 * 6: customer event
		 */
		if (status.type == 0) {
			atbm_con_msg();
		} else if (status.type == 1) {
			atbm_rmmod_msg();
		} else if (status.type == 2) {
			atbm_scan_r_msg();
		} else if (status.type == 3) {
			atbm_wake_host_msg();
		} else if (status.type == 4) {
			atbm_disconnect_msg();
		} else if (status.type == 5) {
			atbm_connect_fail_msg();
		} else if (status.type == 6) {
			atbm_customer_msg();
		}

		if (status.list_empty) {
			break;
		}
	}

	sem_post(&sem_status);
	DEBUG_PRINTF("----msg exit-------\n");
}

int atbm_init()
{
	memset(&status, 0, sizeof(status));
	memset(&networkinfo, 0, sizeof(networkinfo));
	memset(&ap_cfg_set, 0, sizeof(ap_cfg_set));
	memset(&etf_cfg_set, 0, sizeof(etf_cfg_set));
	memset(&power_save_set, 0, sizeof(power_save_set));
	memset(&tcp_filter_set, 0, sizeof(tcp_filter_set));
	memset(&ipc_data_set, 0, sizeof(ipc_data_set));
	memset(&fast_cfg_recv_set, 0, sizeof(fast_cfg_recv_set));
	memset(&fast_cfg_send_set, 0, sizeof(fast_cfg_send_set));

	ssid_set = 0;
	key_set = 0;
	key_id_set = 0;
	key_mgmt_set = 0;
	ap_ssid_set = 0;
	ap_key_set = 0;
	ap_key_id_set = 0;
	ap_key_mgmt_set = 0;
	ipc_server_set = 0;
	ipc_port_set = 0;

	DEBUG_PRINTF("*** Welcome to use atbm_iot_supplicant ***\n\n");
	DEBUG_PRINTF("VER: %s.\n\n", PRIV_VERSION);

	sem_init(&sem, 0, 1);
	sem_init(&sem_status, 0, 1);
	sem_init(&sem_sock_sync, 0, 0);

	signal(SIGIO, ioctl_msg_func);
	signal(SIGTERM, atbm_reboot_handle);
	/* test if driver has insmod */
	insmod_driver(1);

	thread_run = 1;

	DEBUG_PRINTF("%s init finish\n", __FUNCTION__);

	if (NULL != m_wifi_cb) {
		m_wifi_cb(RK_WIFI_State_OPEN, NULL);
	}

	atbm_init_status();

	return 0;
}

int atbm_deinit() {

  //DEBUG_PRINTF("to make atbm driver go sleep...\n");
  //atbm_driver_go_sleep();
  //DEBUG_PRINTF("make driver go sleep finish...\n");

  thread_run = 0;
  sem_destroy(&sem);
  sem_destroy(&sem_status);
  sem_destroy(&sem_sock_sync);

  if (g_fp != -1) {
    close(g_fp);
  }

  DEBUG_PRINTF("%s deinit finish\n", __FUNCTION__);
  return 0;
}

/**
 * @brief 注册复位键长按事件处理函数
 *
 * @param long_press_func_ptr 复位键长按事件处理函数
 * @return int
 */
int key_long_press_func_cb_register(key_long_press_event_func long_press_func_ptr) {
  DEBUG_PRINTF("to register cb(%p)\n", long_press_func_ptr);
  key_long_press_func_cb = long_press_func_ptr;
  return 0;
}

/**
 * @brief 注册复位键长按事件处理函数
 *
 * @param short_press_func_ptr 复位键长按事件处理函数
 * @return int
 */
int key_short_press_func_cb_register(key_short_press_event_func short_press_func_ptr) {
  DEBUG_PRINTF("to register cb(%p)\n", short_press_func_ptr);
  key_short_press_func_cb = short_press_func_ptr;
  return 0;
}

/**
 * @brief 注册唤醒原因事件处理函数
 *
 * @param wakeup_event_func_ptr 唤醒原因事件处理函数
 * @return int
 */
int wakeup_event_func_cb_register(wakeup_event_func wakeup_event_func_ptr) {
  DEBUG_PRINTF("to register cb(%p)\n", wakeup_event_func_ptr);
  wakeup_event_func_cb = wakeup_event_func_ptr;
  return 0;
}

/**
 * @brief 注册电池电量事件处理函数
 * 
 * @param battery_level_event_func_ptr 电池电量事件处理函数
 * @return int 
 */
int battery_level_event_func_cb_register(battery_level_event_func battery_level_event_func_ptr)
{
	DEBUG_PRINTF("to register cb(%p)\n", battery_level_event_func_ptr);
	battery_level_event_func_cb = battery_level_event_func_ptr;
	return 0;
}

/**
 * @brief 注册pir事件处理函数
 * 
 * @param pir_detect_event_func_ptr pir事件处理函数
 * @return int 
 */
int pir_detect_event_func_cb_register(pir_detect_event_func pir_detect_event_func_ptr)
{
	DEBUG_PRINTF("to register cb(%p)\n", pir_detect_event_func_ptr);
	pir_detect_event_func_cb = pir_detect_event_func_ptr;
	return 0;
}

/**
 * @brief 注册wifi连接结果事件处理函数
 * 
 * @param wifi_connect_result_event_func_ptr wifi连接结果事件处理函数
 * @return int 
 */
int wifi_connect_result_event_cb_register(wifi_connect_result_event_func wifi_connect_result_event_func_ptr)
{
	DEBUG_PRINTF("to register cb(%p)\n", wifi_connect_result_event_func_ptr);
	wifi_connect_result_event_func_cb = wifi_connect_result_event_func_ptr;
	return 0;
}

int wifi_fw_ota(char *fw)
{
  return update_fw_cmd(g_fp, 1, &fw);
}

int wifi_get_utctime(unsigned long long *utc)
{
	fw_at("AT+UTC_GET");
	while (atbm_sync_flag == 0)
	{
	pthread_cond_wait(&atbm_sync_cond,&atbm_sync_lock);
	}
	atbm_sync_flag = 0;
	if (utctime == -1) {
		return 0;
	} else {
		*utc = utctime;
	}
	printf("get utc time is %lld \n",*utc);
	return 1;
}

#pragma GCC diagnostic pop
