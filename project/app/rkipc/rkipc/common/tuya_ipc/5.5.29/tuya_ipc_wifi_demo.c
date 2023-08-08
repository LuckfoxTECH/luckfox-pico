/*********************************************************************************
 *Copyright(C),2015-2020,  www.tuya.comm
 *FileName: tuya_linux_wifi_demo.c
 *
 * File Description:
 * 1. WIFI operation API
 *
 * Developer work：
 * 1. Connect to WIFI based on SSID and PASSWD.
 * 2. Grab the image and identify the QR code.
 * 3. Specific WIFI chip adaptation.
 *
 **********************************************************************************/
#include "tuya_ipc.h"

#if defined(WIFI_GW) && (WIFI_GW == 1)

#define WLAN_DEV "wlan0"

/*
Interface that the user needs to implement：
tuya_hal_wifi_station_connect
__tuya_linux_get_snap_qrcode
Other interfaces can be used directly in theory, if a user has their own implementation, they can be
replaced.
*/
static BOOL_T sniffer_set_done = FALSE;
OPERATE_RET tuya_adapter_wifi_station_connect(IN CONST CHAR_T *ssid, IN CONST CHAR_T *passwd) {
	return 0;
	if (sniffer_set_done) {
		sniffer_set_done = FALSE;
		// IPC_APP_Notify_LED_Sound_Status_CB(IPC_REV_WIFI_CFG);
		usleep(1000 * 1000);
	}

	// IPC_APP_Notify_LED_Sound_Status_CB(IPC_CONNECTING_WIFI);

	if (NULL == ssid) {
		// get bind info from ethernet network
		printf("get bind info ...\n");
	} else {
		// get bind info from ap / wifi-smart / qrcode
		printf("get wifi info ...\n");
	}

	// TODO
	// Add a blocking operation for the wifi connection here.

	sleep(2);

	return OPRT_OK;
}

#define MAX_AP_SEARCH 20
static void exec_cmd(char *pCmd) {
	printf("Exec Cmd:%s \r\n", pCmd);
	system(pCmd);
}

/*
wlp3s0    Scan completed :
          Cell 01 - Address: 94:77:2B:0A:96:10
                    Channel:1
                    Frequency:2.412 GHz (Channel 1)
。。。。。。。
。。。。。。。

*/
OPERATE_RET tuya_adapter_wifi_all_ap_scan(OUT AP_IF_S **ap_ary, OUT UINT_T *num) {
	if (NULL == ap_ary || NULL == num) {
		return OPRT_INVALID_PARM;
	}

	static AP_IF_S s_aps[MAX_AP_SEARCH];

	memset(s_aps, 0, sizeof(s_aps));
	*ap_ary = s_aps;
	*num = 0;

#ifdef __HuaweiLite__
	// todo
	// ap used Liteos system scan to implement
#else
	FILE *pp = popen("iwlist " WLAN_DEV " scan", "r");
	if (pp == NULL) {
		printf("popen fails\r\n");
		return OPRT_COM_ERROR;
	}

	char tmp[256] = {0};
	memset(tmp, 0, sizeof(tmp));

	int recordIdx = -1;
	while (fgets(tmp, sizeof(tmp), pp) != NULL) {
		/* First look for BSSID as a benchmark */
		char *pBSSIDStart = strstr(tmp, " - Address: ");
		if (pBSSIDStart != NULL) {
			recordIdx++;
			if (recordIdx >= MAX_AP_SEARCH) {
				printf(" Reach Max Record \r\n");
				recordIdx--;
				break;
			}

			BYTE_T *pTmp = s_aps[recordIdx].bssid;
			int x1, x2, x3, x4, x5, x6;

			sscanf(pBSSIDStart + strlen(" - Address: "), "%x:%x:%x:%x:%x:%x", &x1, &x2, &x3, &x4,
			       &x5, &x6);
			pTmp[0] = x1 & 0xFF;
			pTmp[1] = x2 & 0xFF;
			pTmp[2] = x3 & 0xFF;
			pTmp[3] = x4 & 0xFF;
			pTmp[4] = x5 & 0xFF;
			pTmp[5] = x6 & 0xFF;

			goto ReadNext;
		} else {
			if (recordIdx < 0) { /* find the first BSSID to continue reading */
				goto ReadNext;
			}
		}

		{
			/* find the signal  */
			char *pSIGNALStart = strstr(tmp, "Quality=");
			if (pSIGNALStart != NULL) {
				int x = 0;
				int y = 0;
				sscanf(pSIGNALStart + strlen("Quality="), "%d/%d ", &x, &y);
				s_aps[recordIdx].rssi = x * 100 / (y + 1);
				goto ReadNext;
			}
		}

		{
			/* find the channel	*/
			char *pCHANNELStart = strstr(tmp, "(Channel ");
			if (pCHANNELStart != NULL) {
				int x = 0;
				sscanf(pCHANNELStart + strlen("(Channel "), "%d)", &x);
				s_aps[recordIdx].channel = x;
				goto ReadNext;
			}
		}

		{
			/* find the ssid  */
			char *pSSIDStart = strstr(tmp, "ESSID:\"");
			if (pSSIDStart != NULL) {
				sscanf(pSSIDStart + strlen("ESSID:\""), "%s", s_aps[recordIdx].ssid);
				s_aps[recordIdx].s_len = strlen(s_aps[recordIdx].ssid);
				if (s_aps[recordIdx].s_len != 0) {
					s_aps[recordIdx].ssid[s_aps[recordIdx].s_len - 1] = 0;
					s_aps[recordIdx].s_len--;
				}
				goto ReadNext;
			}
		}

	ReadNext:
		memset(tmp, 0, sizeof(tmp));
	}

	pclose(pp);
	*num = recordIdx + 1;

	printf("WIFI Scan AP Begin\r\n");
	int index = 0;
	for (index = 0; index < *num; index++) {
		printf("index:%d bssid:%02X-%02X-%02X-%02X-%02X-%02X RSSI:%d SSID:%s\r\n", index,
		       s_aps[index].bssid[0], s_aps[index].bssid[1], s_aps[index].bssid[2],
		       s_aps[index].bssid[3], s_aps[index].bssid[4], s_aps[index].bssid[5],
		       s_aps[index].rssi, s_aps[index].ssid);
	}
#endif
	printf("WIFI Scan AP End\r\n");

	return OPRT_OK;
}

OPERATE_RET tuya_adapter_wifi_assign_ap_scan(IN CONST CHAR_T *ssid, OUT AP_IF_S **ap) {
	if (NULL == ssid || NULL == ap) {
		return OPRT_INVALID_PARM;
	}

	/*
   scan all ap and search
	*/
	AP_IF_S *pTotalAp = NULL;
	UINT_T tatalNum = 0;
	int index = 0;
	// tuya_hal_wifi_all_ap_scan(&pTotalAp, &tatalNum);

	*ap = NULL;

	for (index = 0; index < tatalNum; index++) {
		if (memcmp(pTotalAp[index].ssid, ssid, pTotalAp[index].s_len) == 0) {
			*ap = pTotalAp + index;
			break;
		}
	}

	return OPRT_OK;
}

OPERATE_RET tuya_adapter_wifi_release_ap(IN AP_IF_S *ap) { // Static variables, no need to free
	return OPRT_OK;
}

static int s_curr_channel = 1;
OPERATE_RET tuya_adapter_wifi_set_cur_channel(IN CONST BYTE_T chan) {
	// 	char tmpCmd[100] = {0};
	// 	snprintf(tmpCmd, 100, "iwconfig %s channel %d", WLAN_DEV, chan);
	// 	exec_cmd(tmpCmd);
	// 	s_curr_channel = chan;

	// 	printf("WIFI Set Channel:%d \r\n", chan);

	// #ifdef WIFI_CHIP_7601
	// 	tuya_linux_wf_wk_mode_set(WWM_SNIFFER);
	// #endif

	return OPRT_OK;
}

/*
wlp3s0    13 channels in total; available frequencies :
          Channel 01 : 2.412 GHz
。。。。。。
。。。。。。
          Channel 13 : 2.472 GHz
          Current Frequency:2.452 GHz (Channel 9)
*/
OPERATE_RET tuya_adapter_wifi_get_cur_channel(OUT BYTE_T *chan) {
	if (NULL == chan) {
		return OPRT_INVALID_PARM;
	}

	FILE *pp = popen("iwlist " WLAN_DEV " channel", "r");

	if (pp == NULL) {
		return OPRT_COM_ERROR;
	}

	char tmp[128] = {0};
	memset(tmp, 0, sizeof(tmp));
	while (fgets(tmp, sizeof(tmp), pp) != NULL) {
		char *pIPStart = strstr(tmp, " (Channel ");
		if (pIPStart != NULL) {
			break;
		}
	}

	/* find the channel	*/
	char *pCHANNELStart = strstr(tmp, " (Channel ");
	if (pCHANNELStart != NULL) {
		int x = 0;
		sscanf(pCHANNELStart + strlen(" (Channel "), "%d", &x);
		*chan = x;
	} else {
		*chan = s_curr_channel;
	}

	pclose(pp);

	printf("WIFI Get Curr Channel:%d \r\n", *chan);

	return OPRT_OK;
}

#pragma pack(1)
/*
http://www.radiotap.org/
*/
typedef struct {
	/**
	 * @it_version: radiotap version, always 0
	 */
	BYTE_T it_version;

	/**
	 * @it_pad: padding (or alignment)
	 */
	BYTE_T it_pad;

	/**
	 * @it_len: overall radiotap header length
	 */
	USHORT_T it_len;

	/**
	 * @it_present: (first) present word
	 */
	UINT_T it_present;
} ieee80211_radiotap_header;
#pragma pack()

static volatile SNIFFER_CALLBACK s_pSnifferCall = NULL;
static volatile BOOL_T s_enable_sniffer = FALSE;

static void *func_Sniffer(void *pReserved) {
	printf("Sniffer Thread Create\r\n");

	//     int sock = socket(PF_PACKET, SOCK_RAW, htons(0x03));//ETH_P_ALL
	//     if(sock < 0)
	//     {
	//         printf("Sniffer Socket Alloc Fails %d \r\n", sock);
	//         perror("Sniffer Socket Alloc");
	//         return (void *)0;
	//     }

	//     {/* Force binding to wlan0, can be considered to remove */
	//         struct ifreq ifr;
	//         memset(&ifr, 0x00, sizeof(ifr));
	//         strncpy(ifr.ifr_name, WLAN_DEV , sizeof(ifr.ifr_name)-1);
	//         setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, (char *)&ifr, sizeof(ifr));
	//     }

	//     #define MAX_REV_BUFFER 512
	//     BYTE_T rev_buffer[MAX_REV_BUFFER];

	//     int skipLen = 26;/* Radiotap default length is 26 */

	//     while((s_pSnifferCall != NULL) && (TRUE == s_enable_sniffer))
	//     {
	//         int rev_num = recvfrom(sock, rev_buffer, MAX_REV_BUFFER, 0, NULL, NULL);
	//         ieee80211_radiotap_header *pHeader = (ieee80211_radiotap_header *)rev_buffer;
	//         skipLen = pHeader->it_len;

	// #ifdef WIFI_CHIP_7601
	//         skipLen = 144;
	// #endif
	//         if(skipLen >= MAX_REV_BUFFER)
	//         {/* Package is lost directly when its length greater than maximum*/
	//             continue;
	//         }

	//         if(0)
	//         {
	//             printf("skipLen:%d ", skipLen);
	//             int index = 0;
	//             for(index = 0; index < 180; index++)
	//             {
	//                 printf("%02X-", rev_buffer[index]);
	//             }
	//             printf("\r\n");
	//         }
	//         if(rev_num > skipLen)
	//         {
	//             s_pSnifferCall(rev_buffer + skipLen, rev_num - skipLen,2);
	//         }
	//     }

	//      s_pSnifferCall = NULL;

	//     close(sock);

	printf("Sniffer Proc Finish\r\n");
	return (void *)0;
}

static pthread_t sniffer_thId; // ID of capture thread

// Prevent duplication calling
int tuya_adapter_wifi_sniffer_set(const bool en, const SNIFFER_CALLBACK cb)
// OPERATE_RET tuya_hal_wifi_sniffer_set(IN CONST BOOL_T en,IN CONST SNIFFER_CALLBACK cb)
{
	if (en == s_enable_sniffer) {
		printf("Already in status %d\r\n", en);
		return OPRT_OK;
	}
	s_enable_sniffer = en;
	if (en == TRUE) {
		// IPC_APP_Notify_LED_Sound_Status_CB(IPC_START_WIFI_CFG);

		printf("Enable Sniffer\r\n");
		// tuya_hal_wifi_set_work_mode(WWM_SNIFFER);

		s_pSnifferCall = cb;
		pthread_create(&sniffer_thId, NULL, func_Sniffer, NULL);

		printf("Enable Qrcode \r\n");
	} else {
		printf("Disable Sniffer\r\n");
		pthread_join(sniffer_thId, NULL);

		// tuya_hal_wifi_set_work_mode(WWM_STATION);

		printf("Disable Qrcode\r\n");

		sniffer_set_done = TRUE;
	}

	return OPRT_OK;
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
		if (pIPStart != NULL) { /* It's all a line containing IP GW mask that jumps out directly  */
			break;
		}
	}

	{
		/* finding ip  */
		char *pIPStart = strstr(tmp, "inet addr:");
		if (pIPStart != NULL) {
			sscanf(pIPStart + strlen("inet addr:"), "%15s", ip->ip);
		}
	}

	{
		/* finding gw  */
		char *pGWStart = strstr(tmp, "broadcast ");
		if (pGWStart != NULL) {
			sscanf(pGWStart + strlen("broadcast "), "%s", ip->gw);
		}
	}

	{
		/* finding mask */
		char *pMASKStart = strstr(tmp, "netmask ");
		if (pMASKStart != NULL) {
			sscanf(pMASKStart + strlen("netmask "), "%s", ip->mask);
		}
	}

	pclose(pp);

	return OPRT_OK;
}

// if ethernet is used , replace ethernet name
#define NET_DEV "eth0"

/*
wlan0	  Link encap:Ethernet  HWaddr 08:57:00:88:5c:16
          inet addr:192.168.22.3  Bcast:192.168.23.255	Mask:255.255.252.0
。。。。。
。。。。
*/
OPERATE_RET tuya_adapter_wifi_get_ip(IN CONST WF_IF_E wf, OUT NW_IP_S *ip) {
	if (NULL == ip) {
		return OPRT_INVALID_PARM;
	}

	if (wf == WF_AP) { /* Simple Processing in AP Mode */
		memcpy(ip->ip, "192.168.0.1", strlen("192.168.0.1"));
		memcpy(ip->gw, "192.168.0.1", strlen("192.168.0.1"));
		memcpy(ip->mask, "255.255.255.0", strlen("255.255.255.0"));
	}

	if (wf == WF_STATION) {
		// // get the ip of ethernet
		// hwl_get_local_ip_info(NET_DEV, ip);

		NW_IP_S tmp;
		memset(&tmp, 0, sizeof(NW_IP_S));
		// get the ip of wifi
		hwl_get_local_ip_info(WLAN_DEV, &tmp);
		if (strlen(tmp.ip)) {
			// replace ip
			memcpy(ip, &tmp, sizeof(NW_IP_S));
		}
	}

	// printf("WIFI[%d] Get IP:%s\r\n", wf, ip->ip);
	return OPRT_OK;
}

/*
wlp3s0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 192.168.3.14  netmask 255.255.255.0  broadcast 192.168.3.255
        ether a4:02:b9:b1:99:50  txqueuelen 1000  (Ethernet)
。。。。。。
。。。。。。
*/
OPERATE_RET tuya_adapter_wifi_get_mac(IN CONST WF_IF_E wf, INOUT NW_MAC_S *mac) {
	if (NULL == mac) {
		return OPRT_INVALID_PARM;
	}

#ifdef __HuaweiLite__
	// todo
	// Implementing MAC acquisition of liteos system
#else
	FILE *pp = popen("ifconfig " WLAN_DEV, "r");
	if (pp == NULL) {
		return OPRT_COM_ERROR;
	}

	char tmp[256];
	memset(tmp, 0, sizeof(tmp));
	while (fgets(tmp, sizeof(tmp), pp) != NULL) {
		char *pMACStart = strstr(tmp, "ether ");
		if (pMACStart != NULL) {
			int x1, x2, x3, x4, x5, x6;
			sscanf(pMACStart + strlen("ether "), "%x:%x:%x:%x:%x:%x", &x1, &x2, &x3, &x4, &x5, &x6);
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
#endif

	printf("WIFI Get MAC %02X-%02X-%02X-%02X-%02X-%02X\r\n", mac->mac[0], mac->mac[1], mac->mac[2],
	       mac->mac[3], mac->mac[4], mac->mac[5]);

	return OPRT_OK;
}

OPERATE_RET tuya_adapter_wifi_set_mac(IN CONST WF_IF_E wf, IN CONST NW_MAC_S *mac) {
	if (NULL == mac) {
		return OPRT_INVALID_PARM;
	}
	printf("WIFI Set MAC\r\n");
	// reserved
	return OPRT_OK;
}

OPERATE_RET tuya_adapter_wifi_set_work_mode(IN CONST WF_WK_MD_E mode) {
	// char tmpCmd[100] = {0};

	// snprintf(tmpCmd, 100, "ifconfig %s up", WLAN_DEV);
	// exec_cmd(tmpCmd);

	// 	switch (mode) {
	// 	case WWM_LOWPOWER: {
	// 		// Linux system does not care about low power
	// 		break;
	// 	}
	// 	case WWM_SNIFFER: {
	// #ifndef WIFI_CHIP_7601
	// 		snprintf(tmpCmd, 100, "ifconfig %s down", WLAN_DEV);
	// 		exec_cmd(tmpCmd);
	// #endif
	// 		snprintf(tmpCmd, 100, "iwconfig %s mode Monitor", WLAN_DEV);
	// 		exec_cmd(tmpCmd);
	// #ifndef WIFI_CHIP_7601
	// 		snprintf(tmpCmd, 100, "ifconfig %s up", WLAN_DEV);
	// 		exec_cmd(tmpCmd);
	// #endif
	// 		break;
	// 	}
	// 	case WWM_STATION: {
	// #ifndef WIFI_CHIP_7601
	// 		snprintf(tmpCmd, 100, "ifconfig %s down", WLAN_DEV);
	// 		exec_cmd(tmpCmd);
	// #endif
	// 		snprintf(tmpCmd, 100, "iwconfig %s mode Managed", WLAN_DEV);
	// 		exec_cmd(tmpCmd);
	// #ifndef WIFI_CHIP_7601
	// 		snprintf(tmpCmd, 100, "ifconfig %s up", WLAN_DEV);
	// 		exec_cmd(tmpCmd);
	// #endif
	// 		break;
	// 	}
	// 	case WWM_SOFTAP: {
	// #ifndef WIFI_CHIP_7601
	// 		snprintf(tmpCmd, 100, "ifconfig %s down", WLAN_DEV);
	// 		exec_cmd(tmpCmd);
	// #endif
	// 		snprintf(tmpCmd, 100, "iwconfig %s mode Master", WLAN_DEV);
	// 		exec_cmd(tmpCmd);
	// #ifndef WIFI_CHIP_7601
	// 		snprintf(tmpCmd, 100, "ifconfig %s up", WLAN_DEV);
	// 		exec_cmd(tmpCmd);
	// #endif
	// 		break;
	// 	}
	// 	case WWM_STATIONAP: { // reserved
	// 		break;
	// 	}
	// 	default: { break; }
	// 	}
	printf("WIFI Set Mode %d\r\n", mode);

	return OPRT_OK;
}

/*
wlp3s0    IEEE 802.11  ESSID:"H"
          Mode:Managed  Frequency:2.452 GHz  Access Point: 58:7F:66:04:73:A8
          Bit Rate=130 Mb/s   Tx-Power=22 dBm
。。。。。。
。。。。。。
*/
OPERATE_RET tuya_adapter_wifi_get_work_mode(OUT WF_WK_MD_E *mode) {
	// 	if (NULL == mode) {
	// 		return OPRT_INVALID_PARM;
	// 	}

	// #ifdef __HuaweiLite__
	// 	// todo liteos system
	// 	// Implementation of mode acquisition
	// 	char scan_mode[10] = {0};
	// #else
	// 	FILE *pp = popen("iwconfig " WLAN_DEV, "r");
	// 	if (pp == NULL) {
	// 		printf("WIFI Get Mode Fail. Force Set Station \r\n");
	// 		*mode = WWM_STATION;
	// 		return OPRT_OK;
	// 	}

	char scan_mode[10] = {0};
	// 	char tmp[256] = {0};
	// 	while (fgets(tmp, sizeof(tmp), pp) != NULL) {
	// 		char *pModeStart = strstr(tmp, "Mode:");
	// 		if (pModeStart != NULL) {
	// 			int x1, x2, x3, x4, x5, x6;
	// 			sscanf(pModeStart + strlen("Mode:"), "%s ", scan_mode);
	// 			break;
	// 		}
	// 	}
	// 	pclose(pp);
	// #endif

	*mode = WWM_STATION;
	if (strncasecmp(scan_mode, "Managed", strlen("Managed")) == 0) {
		*mode = WWM_STATION;
	}

	if (strncasecmp(scan_mode, "Master", strlen("Master")) == 0) {
		*mode = WWM_SOFTAP;
	}

	if (strncasecmp(scan_mode, "Monitor", strlen("Monitor")) == 0) {
		*mode = WWM_SNIFFER;
	}
	//    printf("WIFI Get Mode [%s] %d\r\n", scan_mode, *mode);

	return OPRT_OK;
}

/***********************************************************
 *  Function: wf_station_disconnect
 *  Input: none
 *  Output: none
 *  Return: OPERATE_RET
 ***********************************************************/
OPERATE_RET tuya_adapter_wifi_station_disconnect(VOID) {
	printf("STA Disconn AP\r\n");
	// Reserved

	return OPRT_OK;
}

/*
wlp3s0    IEEE 802.11  ESSID:"HUAWEI-1234"
。。。。。。
          Link Quality=70/70  Signal level=-6 dBm
。。。。。。
*/
OPERATE_RET tuya_adapter_wifi_station_get_conn_ap_rssi(OUT SCHAR_T *rssi) {
	// 	if (NULL == rssi) {
	// 		return OPRT_INVALID_PARM;
	// 	}
	// 	*rssi = 0;

	// #ifdef __HuaweiLite__
	// 	// todo
	// 	// liteos system
	// 	// Implementation of RSSI acquisition
	// #else
	// 	FILE *pp = popen("iwconfig " WLAN_DEV, "r");
	// 	if (pp == NULL) {
	// 		return OPRT_COM_ERROR;
	// 	}

	// 	char tmp[128] = {0};
	// 	while (fgets(tmp, sizeof(tmp), pp) != NULL) {
	// 		/* find signal  */
	// 		char *pSIGNALStart = strstr(tmp, "Quality=");
	// 		if (pSIGNALStart != NULL) {
	// 			int x = 0;
	// 			int y = 0;
	// 			sscanf(pSIGNALStart + strlen("Quality="), "%d/%d", &x, &y);
	// 			*rssi = x * 100 / (y + 1);
	// 			break;
	// 		}
	// 	}
	// 	pclose(pp);
	// #endif
	printf("Get Conn AP RSSI:%d\r\n", *rssi);

	return OPRT_OK;
}

// note：High frequency interface
OPERATE_RET tuya_adapter_wifi_station_get_status(OUT WF_STATION_STAT_E *stat) {
	if (NULL == stat) {
		return OPRT_INVALID_PARM;
	}
	*stat = WSS_GOT_IP; // Be sure to return in real time
	// Reserved
	return OPRT_OK;
}

OPERATE_RET tuya_adapter_wifi_ap_start(IN CONST WF_AP_CFG_IF_S *cfg) {
	if (NULL == cfg) {
		return OPRT_INVALID_PARM;
	}

	printf("Start AP SSID:%s \r\n", cfg->ssid);
	// Reserved
	return OPRT_OK;
}

OPERATE_RET tuya_adapter_wifi_ap_stop(VOID) {
	printf("Stop AP \r\n");
	// Reserved
	return OPRT_OK;
}

OPERATE_RET tuya_adapter_wifi_set_country_code(IN CONST CHAR_T *p_country_code) {
	printf("Set Country Code:%s \r\n", p_country_code);

	return OPRT_OK;
}

int tuya_adapter_wifi_send_mgnt(const uint8_t *buf, const uint32_t len) {
	// use to send pro pack
	return 0;
}

OPERATE_RET tuya_adapter_wifi_register_recv_mgnt_callback(CONST BOOL_T enable,
                                                          CONST WIFI_REV_MGNT_CB recv_cb) {
	return 0;
}
OPERATE_RET tuya_adapter_wifi_set_lp_mode(CONST BOOL_T en, CONST UCHAR_T dtim) { return 0; }
BOOL_T tuya_adapter_wifi_rf_calibrated(VOID_T) { return 1; }
OPERATE_RET tuya_adapter_wifi_station_get_err_stat(WF_STATION_STAT_E *stat) { return 0; }
OPERATE_RET tuya_adapter_wifi_get_bssid(UCHAR_T *mac) { return 0; }
OPERATE_RET
tuya_adapter_wifi_fast_station_connect_v2(CONST FAST_WF_CONNECTED_AP_INFO_V2_S *fast_ap_info) {
	return 0;
}
OPERATE_RET
tuya_adapter_wifi_get_connected_ap_info_v2(FAST_WF_CONNECTED_AP_INFO_V2_S **fast_ap_info) {
	return 0;
}
#endif
