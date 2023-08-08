#include "tuya_iot_config.h"
#include <arpa/inet.h>
#include <net/if.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "tuya_ipc_api.h"
#include "tuya_ipc_qrcode_proc.h"
#include "wifi_hwl.h"

#define NET_DEV "eth0"
#define WLAN_DEV "wlan0"

static void exec_cmd(char *pCmd) {
	printf("Exec Cmd:%s \r\n", pCmd);
	system(pCmd);
}

static OPERATE_RET hwl_get_local_ip_info(char *interface, OUT NW_IP_S *ip) {
	char tmp[256];

	memset(tmp, 0, sizeof(tmp));
	snprintf(tmp, sizeof(tmp), "ifconfig %s", interface);
	FILE *pp = popen(tmp, "r");
	if (pp == NULL) {
		return OPRT_COM_ERROR;
	}

	memset(tmp, 0, sizeof(tmp));
	while (fgets(tmp, sizeof(tmp), pp) != NULL) {
		char *pIPStart = strstr(tmp, "inet addr:");
		if (pIPStart != NULL) { /* It's all a line containing IP GW mask that jumps
			                       out directly  */
			break;
		}
	}

	/* finding ip  */
	char *pIPStart = strstr(tmp, "inet addr:");
	if (pIPStart != NULL)
		sscanf(pIPStart + strlen("inet addr:"), "%15s", ip->ip);

	/* finding gw  */
	char *pGWStart = strstr(tmp, "broadcast ");
	if (pGWStart != NULL)
		sscanf(pGWStart + strlen("broadcast "), "%s", ip->gw);

	/* finding mask */
	char *pMASKStart = strstr(tmp, "netmask ");
	if (pMASKStart != NULL)
		sscanf(pMASKStart + strlen("netmask "), "%s", ip->mask);

	pclose(pp);

	return OPRT_OK;
}

/***********************************************************
 *  Function: hwl_wf_all_ap_scan
 *  Desc:     scan current environment and obtain all the ap
 *            infos in current environment
 *  Output:   ap_ary: current ap info array
 *  Output:   num   : the num of ar_ary
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_wf_all_ap_scan(OUT AP_IF_S **ap_ary, OUT UINT_T *num) {
	printf("+++%s\n", __FUNCTION__);
	return 0;
}

/***********************************************************
 *  Function: hwl_wf_assign_ap_scan
 *  Desc:     scan current environment and obtain the specific
 *            ap info.
 *  Input:    ssid: the specific ssid
 *  Output:   ap  : the ap info
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_wf_assign_ap_scan(IN CONST CHAR_T *ssid, OUT AP_IF_S **ap) {
	printf("+++%s\n", __FUNCTION__);
	return 0;
}

/***********************************************************
 *  Function: hwl_wf_release_ap
 *  Desc:     release the memory malloced in <hwl_wf_all_ap_scan>
 *            and <hwl_wf_assign_ap_scan> if needed. tuya-sdk
 *            will call this function when the ap info is no use.
 *  Input:    ap: the ap info
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_wf_release_ap(IN AP_IF_S *ap) {
	printf("+++%s\n", __FUNCTION__);
	return 0;
}

/***********************************************************
 *  Function: hwl_wf_set_cur_channel
 *  Desc:     set wifi interface work channel
 *  Input:    chan: the channel to set
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
static int s_curr_channel = 1;
OPERATE_RET hwl_wf_set_cur_channel(IN CONST BYTE_T chan) {
	printf("+++%s\n", __FUNCTION__);
	// char tmpCmd[100] = {0};
	// snprintf(tmpCmd, 100, "iwconfig %s channel %d", WLAN_DEV, chan);
	// exec_cmd(tmpCmd);
	// s_curr_channel = chan;

	printf("WIFI Set Channel:%d \r\n", chan);
	return 0;
}

/***********************************************************
 *  Function: hwl_wf_get_cur_channel
 *  Desc:     get wifi interface work channel
 *  Input:    chan: the channel wifi works
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_wf_get_cur_channel(OUT BYTE_T *chan) {
	printf("+++%s\n", __FUNCTION__);
	return 0;
}

/***********************************************************
 *  Function: hwl_wf_sniffer_set
 *  Desc:     enable / disable wifi sniffer mode.
 *            if wifi sniffer mode is enabled, wifi recv from
 *            packages from the air, and user shoud send these
 *            packages to tuya-sdk with callback <cb>.
 *  Input:    en: enable or disable
 *  Input:    cb: notify callback
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
static volatile SNIFFER_CALLBACK s_pSnifferCall = NULL;
static volatile BOOL_T s_enable_sniffer = FALSE;

OPERATE_RET hwl_wf_sniffer_set(IN CONST BOOL_T en, IN CONST SNIFFER_CALLBACK cb) {
	printf("+++%s\n", __FUNCTION__);
	// 模拟发送按键？ 然后再取得结果？
	// tuya_ipc_direct_connect(
	//     "{\"p\":\"12345678\",\"s\":\"TP-LINK_YYLX\",\"t\":\"AY8jzbUCg8WWB3\"}",
	//     TUYA_IPC_DIRECT_CONNECT_QRCODE);
	// printf("after tuya_ipc_direct_connect\n");
	return 0;
}

/***********************************************************
 *  Function: hwl_wf_get_ip
 *  Desc:     get wifi ip info.when wifi works in
 *            ap+station mode, wifi has two ips.
 *  Input:    wf: wifi function type
 *  Output:   ip: the ip addr info
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_wf_get_ip(IN CONST WF_IF_E wf, OUT NW_IP_S *ip) {
	printf("+++%s, wf is %d\n", __FUNCTION__, wf);
	if (NULL == ip) {
		return OPRT_INVALID_PARM;
	}

	if (wf == WF_AP) { /* Simple Processing in AP Mode */
		memcpy(ip->ip, "192.168.0.1", strlen("192.168.0.1"));
		memcpy(ip->gw, "192.168.0.1", strlen("192.168.0.1"));
		memcpy(ip->mask, "255.255.255.0", strlen("255.255.255.0"));
	}

	if (wf == WF_STATION) {
		// get the ip of ethernet
		hwl_get_local_ip_info(NET_DEV, ip);

		NW_IP_S tmp;
		memset(&tmp, 0, sizeof(NW_IP_S));
		// get the ip of wifi
		hwl_get_local_ip_info(WLAN_DEV, &tmp);
		if (strlen(tmp.ip)) {
			// if wifi ip exist, replace ip
			memcpy(ip, &tmp, sizeof(NW_IP_S));
		}
	}

	printf("WIFI[%d] Get IP:%s\r\n", wf, ip->ip);
	return OPRT_OK;
}

/***********************************************************
 *  Function: hwl_wf_set_ip
 *  Desc:     set wifi ip info.when wifi works in
 *            ap+station mode, wifi has two ips.
 *  Input:    wf: wifi function type
 *  Input:    ip: the ip addr info
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_wf_set_ip(IN CONST WF_IF_E wf, IN CONST NW_IP_S *ip) {
	printf("+++%s\n", __FUNCTION__);
	return 0;
}

/***********************************************************
 *  Function: hwl_wf_get_mac
 *  Desc:     get wifi mac info.when wifi works in
 *            ap+station mode, wifi has two macs.
 *  Input:    wf: wifi function type
 *  Output:   mac: the mac info
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_wf_get_mac(IN CONST WF_IF_E wf, OUT NW_MAC_S *mac) {
	printf("+++%s\n", __FUNCTION__);
	if (NULL == mac) {
		return OPRT_INVALID_PARM;
	}

	FILE *pp = popen("ifconfig " WLAN_DEV, "r");
	if (pp == NULL) {
		return OPRT_COM_ERROR;
	}

	char tmp[256];
	memset(tmp, 0, sizeof(tmp));
	while (fgets(tmp, sizeof(tmp), pp) != NULL) {
		char *pMACStart = strstr(tmp, "HWaddr ");
		if (pMACStart != NULL) {
			int x1, x2, x3, x4, x5, x6;
			sscanf(pMACStart + strlen("HWaddr "), "%x:%x:%x:%x:%x:%x", &x1, &x2, &x3, &x4, &x5,
			       &x6);
			mac->mac[0] = x1 & 0xFF;
			mac->mac[1] = x2 & 0xFF;
			mac->mac[2] = x3 & 0xFF;
			mac->mac[3] = x4 & 0xFF;
			mac->mac[4] = x5 & 0xFF;
			mac->mac[5] = x6 & 0xFF;

			break;
		}
	}
	pclose(pp);

	printf("WIFI Get MAC %02X-%02X-%02X-%02X-%02X-%02X\r\n", mac->mac[0], mac->mac[1], mac->mac[2],
	       mac->mac[3], mac->mac[4], mac->mac[5]);

	return OPRT_OK;
	return 0;
}

/***********************************************************
 *  Function: hwl_wf_set_mac
 *  Desc:     set wifi mac info.when wifi works in
 *            ap+station mode, wifi has two macs.
 *  Input:    wf: wifi function type
 *  Input:    mac: the mac info
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_wf_set_mac(IN CONST WF_IF_E wf, IN CONST NW_MAC_S *mac) {
	printf("+++%s\n", __FUNCTION__);
	return 0;
}

/***********************************************************
 *  Function: hwl_wf_wk_mode_set
 *  Desc:     set wifi work mode
 *  Input:    mode: wifi work mode
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_wf_wk_mode_set(IN CONST WF_WK_MD_E mode) {
	printf("+++%s, mode is %d\n", __FUNCTION__, mode);
	// char tmpCmd[100] = {0};

	// snprintf(tmpCmd, 100, "ifconfig %s up", WLAN_DEV);
	// exec_cmd(tmpCmd);

	// switch (mode) {
	// case WWM_LOWPOWER: {
	//   // Linux system does not care about low power
	//   break;
	// }
	// case WWM_SNIFFER: {
	//   snprintf(tmpCmd, 100, "ifconfig %s down", WLAN_DEV);
	//   exec_cmd(tmpCmd);
	//   snprintf(tmpCmd, 100, "iwconfig %s mode Monitor", WLAN_DEV);
	//   exec_cmd(tmpCmd);
	//   snprintf(tmpCmd, 100, "ifconfig %s up", WLAN_DEV);
	//   exec_cmd(tmpCmd);
	//   break;
	// }
	// case WWM_STATION: {
	//   snprintf(tmpCmd, 100, "ifconfig %s down", WLAN_DEV);
	//   exec_cmd(tmpCmd);
	//   snprintf(tmpCmd, 100, "iwconfig %s mode Managed", WLAN_DEV);
	//   exec_cmd(tmpCmd);
	//   snprintf(tmpCmd, 100, "ifconfig %s up", WLAN_DEV);
	//   exec_cmd(tmpCmd);
	//   break;
	// }
	// case WWM_SOFTAP: {
	//   snprintf(tmpCmd, 100, "ifconfig %s down", WLAN_DEV);
	//   exec_cmd(tmpCmd);
	//   snprintf(tmpCmd, 100, "iwconfig %s mode Master", WLAN_DEV);
	//   exec_cmd(tmpCmd);
	//   snprintf(tmpCmd, 100, "ifconfig %s up", WLAN_DEV);
	//   exec_cmd(tmpCmd);
	//   break;
	// }
	// case WWM_STATIONAP: { // reserved
	//   break;
	// }
	// default: { break; }
	// }
	// printf("WIFI Set Mode %d\r\n", mode);

	return 0;
}

/***********************************************************
 *  Function: hwl_wf_wk_mode_get
 *  Desc:     get wifi work mode
 *  Output:   mode: wifi work mode
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_wf_wk_mode_get(OUT WF_WK_MD_E *mode) {
	printf("+++%s\n", __FUNCTION__);
	if (NULL == mode) {
		return OPRT_INVALID_PARM;
	}

	// FILE *pp = popen("iwconfig " WLAN_DEV, "r");
	// if (pp == NULL) {
	//   printf("WIFI Get Mode Fail. Force Set Station \r\n");
	//   *mode = WWM_STATION;
	//   return OPRT_OK;
	// }

	// char scan_mode[10] = {0};
	// char tmp[256] = {0};
	// while (fgets(tmp, sizeof(tmp), pp) != NULL) {
	//   char *pModeStart = strstr(tmp, "Mode:");
	//   if (pModeStart != NULL) {
	//     int x1, x2, x3, x4, x5, x6;
	//     sscanf(pModeStart + strlen("Mode:"), "%s ", scan_mode);
	//     break;
	//   }
	// }
	// pclose(pp);

	*mode = WWM_STATION;
	// if (strncasecmp(scan_mode, "Managed", strlen("Managed")) == 0) {
	//   *mode = WWM_STATION;
	// }

	// if (strncasecmp(scan_mode, "Master", strlen("Master")) == 0) {
	//   *mode = WWM_SOFTAP;
	// }

	// if (strncasecmp(scan_mode, "Monitor", strlen("Monitor")) == 0) {
	//   *mode = WWM_SNIFFER;
	// }
	// printf("WIFI Get Mode [%s] %d\r\n", scan_mode, *mode);

	return OPRT_OK;
}

/***********************************************************
 *  Function: hwl_wf_station_connect
 *  Desc:     connect wifi with ssid and passwd.
 *  Input:    ssid && passwd
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_wf_station_connect(IN CONST CHAR_T *ssid, IN CONST CHAR_T *passwd) {
	printf("+++%s\n", __FUNCTION__);
	return 0;
}

/***********************************************************
 *  Function: hwl_wf_station_disconnect
 *  Desc:     disconnect wifi from connect ap
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_wf_station_disconnect(VOID) {
	printf("+++%s\n", __FUNCTION__);
	printf("STA Disconn AP\r\n");
	// Reserved
	return 0;
}

/***********************************************************
 *  Function: hwl_wf_station_get_conn_ap_rssi
 *  Desc:     get wifi connect rssi
 *  Output:   rssi: the return rssi.
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_wf_station_get_conn_ap_rssi(OUT SCHAR_T *rssi) {
	printf("+++%s\n", __FUNCTION__);
	return 0;
}

/***********************************************************
 *  Function: hwl_wf_station_stat_get
 *  Desc:     get wifi station work status
 *  Output:   stat: the wifi station work status
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_wf_station_stat_get(OUT WF_STATION_STAT_E *stat) {
	printf("+++%s\n", __FUNCTION__);
	if (NULL == stat) {
		return OPRT_INVALID_PARM;
	}
	*stat = WSS_GOT_IP; // Be sure to return in real time
	// Reserved
	return OPRT_OK;
	return 0;
}

/***********************************************************
 *  Function: hwl_wf_ap_start
 *  Desc:     start a soft ap
 *  Input:    cfg: the soft ap config
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_wf_ap_start(IN CONST WF_AP_CFG_IF_S *cfg) {
	printf("+++%s\n", __FUNCTION__);
	if (NULL == cfg)
		return OPRT_INVALID_PARM;

	printf("Start AP SSID:%s \r\n", cfg->ssid);
	return 0;
}

/***********************************************************
 *  Function: hwl_wf_ap_stop
 *  Desc:     stop a soft ap
 *  Return:   OPRT_OK: success  Other: fail
 ***********************************************************/
OPERATE_RET hwl_wf_ap_stop(VOID) {
	printf("+++%s\n", __FUNCTION__);
	return 0;
}

OPERATE_RET hwl_wf_set_country_code(IN CONST CHAR_T *p_country_code) {
	printf("+++%s\n", __FUNCTION__);
	return 0;
}

/***********************************************************
 *  Function: hwl_wf_network_mode
 *  Desc:
 *  Output:  mode 0:wifi     1:ethernet
 *  Return:   OPRT_OK: success   Other: fail
 ***********************************************************/
OPERATE_RET hwl_wf_network_mode(OUT WF_NETWORK_MODE_E *mode) {
	printf("+++%s\n", __FUNCTION__);
	return 0;
}

VOID tuya_ipc_get_snapshot_cb(char *pjbuf, unsigned int *size) {
	printf("+++%s\n", __FUNCTION__);
	// get_motion_snapshot(pjbuf,size);
}
