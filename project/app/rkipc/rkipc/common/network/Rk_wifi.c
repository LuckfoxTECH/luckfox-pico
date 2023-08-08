// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "network.h"

#include "Hostapd.h"
#include "RK_encode.h"
#include "Rk_wifi.h"
#include "slog.h"
#include "utility.h"

static bool save_last_ap = false;
static int connecting_id = -1;

static RK_WIFI_RUNNING_State_e gstate = RK_WIFI_State_OFF;

static char retry_connect_cmd[128];

typedef struct {
	char ssid[SSID_BUF_LEN];
	char bssid[BSSID_BUF_LEN];
} RK_WIFI_CONNECT_INFO;

static RK_WIFI_CONNECT_INFO connect_info = {
    "",
    "",
};

typedef struct RK_WIFI_encode_gbk {
	char ori[128];
	char utf8[128];
	struct RK_WIFI_encode_gbk *next;
} RK_WIFI_encode_gbk_t;

// GBK编码路由链表
static RK_WIFI_encode_gbk_t *m_gbk_head = NULL;
// 无密码路由链表
static RK_WIFI_encode_gbk_t *m_nonpsk_head = NULL;

static RK_WIFI_encode_gbk_t *encode_gbk_insert(RK_WIFI_encode_gbk_t *head, const char *ori,
                                               const char *utf8) {
	if ((ori == NULL || strlen(ori) == 0) || (utf8 == NULL || strlen(utf8) == 0))
		return head;

	RK_WIFI_encode_gbk_t *gbk = (RK_WIFI_encode_gbk_t *)malloc(sizeof(RK_WIFI_encode_gbk_t));
	memset(gbk->ori, 0, sizeof(gbk->ori));
	memset(gbk->utf8, 0, sizeof(gbk->utf8));

	if (strlen(ori) >= sizeof(gbk->ori) || strlen(utf8) >= sizeof(gbk->utf8)) {
		free(gbk);
		return head;
	}

	strcat(gbk->ori, ori);
	strcat(gbk->utf8, utf8);

	gbk->next = head;
	head = gbk;

	return head;
}

static RK_WIFI_encode_gbk_t *encode_gbk_reset(RK_WIFI_encode_gbk_t *head) {
	RK_WIFI_encode_gbk_t *gbk, *gbk_next;

	gbk = head;
	while (gbk) {
		gbk_next = gbk->next;
		free(gbk);

		gbk = gbk_next;
	}
	head = NULL;

	return head;
}

static char *get_encode_gbk_ori(RK_WIFI_encode_gbk_t *head, const char *str, char *dst) {
	int is_gbk = 0;
	RK_WIFI_encode_gbk_t *gbk = head;

	while (gbk) {
		if (strcmp(gbk->utf8, str) == 0) {
			is_gbk = 1;
			strcpy(dst, gbk->ori);
			break;
		}
		gbk = gbk->next;
	}
	if (!is_gbk) {
		strcpy(dst, str);
	}

	return dst;
}

static char *get_encode_gbk_utf8(RK_WIFI_encode_gbk_t *head, const char *str, char *dst) {
	int is_gbk = 0;
	RK_WIFI_encode_gbk_t *gbk = head;

	while (gbk) {
		if (strcmp(gbk->ori, str) == 0) {
			is_gbk = 1;
			strcpy(dst, gbk->utf8);
			break;
		}
		gbk = gbk->next;
	}
	if (!is_gbk) {
		strcpy(dst, str);
	}

	return dst;
}

static int is_non_psk(const char *str) {
	RK_WIFI_encode_gbk_t *nonpsk = m_nonpsk_head;
	while (nonpsk) {
		if (strcmp(nonpsk->utf8, str) == 0) {
			return 1;
		}
		nonpsk = nonpsk->next;
	}

	return 0;
}

static RK_wifi_state_callback m_cb;
static volatile bool wifi_wrong_key = false;
static volatile bool wifi_cancel = false;
static volatile bool wifi_connect_lock = false;
static volatile bool wifi_is_exist = false;

static void format_wifiinfo(int flag, char *info);
static int get_pid(const char Name[]);
static void *RK_wifi_start_monitor(void *arg);

static char *wifi_state[] = {
    "RK_WIFI_State_IDLE",          "RK_WIFI_State_CONNECTING",
    "RK_WIFI_State_CONNECTFAILED", "RK_WIFI_State_CONNECTFAILED_WRONG_KEY",
    "RK_WIFI_State_CONNECTED",     "RK_WIFI_State_DISCONNECTED",
    "RK_WIFI_State_OPEN",          "RK_WIFI_State_OFF",
    "RK_WIFI_State_SCAN_RESULTS",  "RK_WIFI_State_DHCP_OK",
};

static void wifi_state_send(RK_WIFI_RUNNING_State_e state, RK_WIFI_INFO_Connection_s *info) {
	if (m_cb == NULL)
		return;

	if (state == RK_WIFI_State_CONNECTFAILED || state == RK_WIFI_State_CONNECTING) {
		RK_WIFI_INFO_Connection_s cndinfo;

		RK_wifi_running_getConnectionInfo(&cndinfo);
		strcpy(cndinfo.ssid, connect_info.ssid);
		strcpy(cndinfo.bssid, connect_info.bssid);
		cndinfo.ssid[SSID_BUF_LEN - 1] = '\0';
		cndinfo.bssid[BSSID_BUF_LEN - 1] = '\0';
		pr_info("[RKWIFI]: %s: %s, %s, %s, reason=%d\n", __func__, wifi_state[state], cndinfo.ssid,
		        cndinfo.bssid, cndinfo.reason);
		m_cb(state, &cndinfo);
	} else {
		if (info) {
			info->ssid[SSID_BUF_LEN - 1] = '\0';
			info->bssid[BSSID_BUF_LEN - 1] = '\0';
			pr_info("[RKWIFI]: %s: %s, %s, %s, reason=%d\n", __func__, wifi_state[state],
			        info->ssid, info->bssid, info->reason);
		}
		m_cb(state, info);
	}
}

static char *remove_escape_character(const char *buf, char *dst) {
	char buf_temp[strlen(buf) + 1];
	int i = 0;

	memset(buf_temp, 0, sizeof(buf_temp));
	while (*buf != '\0') {
		if (*buf == '\\' && (*(buf + 1) == '\\' || *(buf + 1) == '\"')) {
			dst[i] = *(buf + 1);
			buf = buf + 2;
		} else {
			dst[i] = *buf;
			buf++;
		}
		i++;
	}
	dst[i] = '\0';

	return dst;
}

static char *spec_char_convers(const char *buf, char *dst) {
	char buf_temp[strlen(buf) + 1];
	int i = 0;
	unsigned long con;

	memset(buf_temp, 0, sizeof(buf_temp));
	while (*buf != '\0') {
		if (*buf == '\\' && *(buf + 1) == 'x') {
			strcpy(buf_temp, buf);
			*buf_temp = '0';
			*(buf_temp + 4) = '\0';
			con = strtoul(buf_temp, NULL, 16);
			dst[i] = con;
			buf += 4;
		} else {
			dst[i] = *buf;
			buf++;
		}
		i++;
	}
	dst[i] = '\0';
	return dst;
}

int RK_wifi_register_callback(RK_wifi_state_callback cb) {
	m_cb = cb;
	return 1;
}

static int RK_wifi_search_with_bssid(const char *bssid) {
	RK_WIFI_SAVED_INFO wsi;

	RK_wifi_getSavedInfo(&wsi);
	for (int i = 0; i < wsi.count; i++) {
		if (strncmp(wsi.save_info[i].bssid, bssid, strlen(bssid)) == 0) {
			return wsi.save_info[i].id;
		}
	}

	return -1;
}

static int RK_wifi_search_with_ssid(const char *ssid) {
	RK_WIFI_SAVED_INFO wsi;
	int len;

	RK_wifi_getSavedInfo(&wsi);
	for (int i = 0; i < wsi.count; i++) {
		if ((strlen(wsi.save_info[i].ssid) > 64) || (strlen(ssid) > 64))
			pr_err("RK_wifi_search_with_ssid ssid error!!!\n");
		pr_err("RK_wifi_search_with_ssid save_info[%d].ssid: %s:%ld, ssid: %s:%ld \n", i,
		       wsi.save_info[i].ssid, strlen(wsi.save_info[i].ssid), ssid, strlen(ssid));
		if (strlen(wsi.save_info[i].ssid) > strlen(ssid))
			len = strlen(wsi.save_info[i].ssid);
		else
			len = strlen(ssid);
		if (strncmp(wsi.save_info[i].ssid, ssid, len) == 0) {
			return wsi.save_info[i].id;
		}
	}

	return -1;
}

static bool get_ssid_directly(char *flags, int row, int columns) {
	/* <=3: id ssid bssid
	   4: id ssid bssid flags*/
	if (columns <= 3 || (columns == 4 && strlen(flags) > 0))
		return true;

	return false;
}

static char *ltrim(char *str) {
	if (str == NULL || *str == '\0') {
		return str;
	}

	int len = 0;
	char *p = str;
	while (*p != '\0' && isspace(*p)) {
		++p;
		++len;
	}

	memmove(str, p, strlen(str) - len + 1);

	return str;
}

static char *rtrim(char *str) {
	if (str == NULL || *str == '\0') {
		return str;
	}

	int len = strlen(str);
	char *p = str + len - 1;
	while (p >= str && isspace(*p)) {
		*p = '\0';
		--p;
	}

	return str;
}

static char *RK_property_trim(char *str) {
	str = rtrim(str);
	str = ltrim(str);

	return str;
}

static int get_ssid_from_list_network(RK_WIFI_SAVED_INFO_s *info, int row, int columns) {
	int start = 0, end = 0;
	char cmd[128], str[128];
	char ssid[128], sname[128], utf8[128];
	char *bssid;

	if (!info)
		return -1;

	memset(info->ssid, 0, SSID_BUF_LEN);
	memset(cmd, 0, 128);
	memset(str, 0, 128);

	if (get_ssid_directly(info->state, row, columns)) {
		snprintf(cmd, sizeof(cmd), "wpa_cli -i wlan0 list_network | awk '{print $2}' | sed -n %dp",
		         row);
		exec_command(cmd, str, 128);
		pr_info("%s: row = %d, column(2) = %s\n", __func__, row, str);

		memset(sname, 0, sizeof(sname));
		memset(utf8, 0, sizeof(utf8));
		memset(ssid, 0, sizeof(ssid));

		str[strlen(str) - 1] = '\0';
		remove_escape_character(str, ssid);
		spec_char_convers(ssid, sname);
		get_encode_gbk_utf8(m_gbk_head, sname, utf8);
		pr_info("direct: convers str: %s, sname: %s, ori: %s\n", ssid, sname, utf8);
		strcpy(info->ssid, utf8);

		return 0;
	}

	snprintf(cmd, sizeof(cmd), "wpa_cli -i wlan0 list_network | sed -n %dp", row);
	exec_command(cmd, str, 128);
	pr_info("%s: row(%d) = %s\n", __func__, row, str);

	memset(cmd, 0, 128);
	sprintf(cmd, "%d", info->id);
	start = strlen(cmd);

	if ((bssid = strstr(str, info->bssid)) == NULL) {
		pr_err("%s: %s can't find bssid(%s)\n", __func__, str, info->bssid);
		return -1;
	}
	end = strlen(str) - strlen(bssid);

	memset(ssid, 0, 128);
	strncpy(ssid, str + start, end - start);
	RK_property_trim(ssid);

	memset(str, 0, 128);
	memset(sname, 0, 128);
	memset(utf8, 0, 128);
	remove_escape_character(ssid, str);
	spec_char_convers(str, sname);
	get_encode_gbk_utf8(m_gbk_head, sname, utf8);
	pr_info("ndirect convers str: %s, sname: %s, ori: %s\n", str, sname, utf8);
	strcpy(info->ssid, utf8);

	return 0;
}

int get_bssid_from_list_network(RK_WIFI_SAVED_INFO_s *info, int row, int columns) {
	char cmd[128], str[128];
	int bssid_column = columns;

	if (!info)
		return -1;

	memset(info->bssid, 0, BSSID_BUF_LEN);
	memset(cmd, 0, 128);
	memset(str, 0, 128);

	if (strlen(info->state) > 0)
		bssid_column = columns - 1;

	snprintf(cmd, sizeof(cmd), "wpa_cli -i wlan0 list_network | awk '{print $%d}' | sed -n %dp",
	         bssid_column, row);
	exec_command(cmd, str, 128);
	pr_info("%s: row = %d, column(%d) = %s\n", __func__, row, bssid_column, str);

	strcpy(info->bssid, str);
	return 0;
}

int get_id_from_list_network(int row) {
	int network_id;
	char cmd[128], str[128];

	memset(cmd, 0, 128);
	memset(str, 0, 128);
	snprintf(cmd, sizeof(cmd), "wpa_cli -i wlan0 list_network | awk '{print $1}' | sed -n %dp",
	         row);
	exec_command(cmd, str, 128);
	network_id = atoi(str);

	pr_info("%s: row = %d, network_id = %d\n", __func__, row, network_id);
	return network_id;
}

int get_columns_from_list_network(int row) {
	int columns;
	char cmd[128], str[128];

	memset(cmd, 0, 128);
	memset(str, 0, 128);

	snprintf(cmd, sizeof(cmd), "wpa_cli -i wlan0 list_network | awk '{print NF}' | sed -n %dp",
	         row);
	exec_command(cmd, str, 128);
	columns = atoi(str);

	pr_info("%s: row = %d, columns = %d\n", __func__, row, columns);
	return columns;
}

void get_flags_from_list_network(char *flags_buf, int buf_len, int row, int columns) {
	char cmd[128], str[128];

	memset(flags_buf, 0, buf_len);
	memset(cmd, 0, 128);
	memset(str, 0, 128);

	snprintf(cmd, sizeof(cmd), "wpa_cli -i wlan0 list_network | awk '{print $%d}' | sed -n %dp",
	         columns, row);
	exec_command(cmd, str, 128);
	if (!strncmp(str, "[CURRENT]", strlen("[CURRENT]")) ||
	    !strncmp(str, "[DISABLED]", strlen("[DISABLED]"))) {
		strcpy(flags_buf, str);
		pr_info("%s: row = %d, column(%d) = %s\n", __func__, row, columns, flags_buf);
	}
}

int RK_wifi_getSavedInfo(RK_WIFI_SAVED_INFO *pInfo) {
	int cnt, row, columns;
	char str[128];

	if (pInfo == NULL)
		return -1;

	memset(pInfo, 0, sizeof(RK_WIFI_SAVED_INFO));

	memset(str, 0, 128);
	exec_command("wpa_cli -i wlan0 list_network | wc -l", str, 128);
	cnt = atoi(str) - 1;
	pInfo->count = cnt;
	pr_info("wifi cnt: %d(%s)\n", cnt, str);

	if (cnt <= 0)
		return -1;

	for (int i = 0; i < cnt; i++) {
		row = i + 2;

		columns = get_columns_from_list_network(row);
		pInfo->save_info[i].id = get_id_from_list_network(row);
		get_flags_from_list_network(pInfo->save_info[i].state, STATE_BUF_LEN, row, columns);
		get_bssid_from_list_network(&pInfo->save_info[i], row, columns);
		get_ssid_from_list_network(&pInfo->save_info[i], row, columns);
	}

	for (int i = 0; i < cnt; i++) {
		pr_info("id: %d, name: %s, bssid: %s, state: %s\n", pInfo->save_info[i].id,
		        pInfo->save_info[i].ssid, pInfo->save_info[i].bssid, pInfo->save_info[i].state);
	}

	return 0;
}

int RK_wifi_running_getState(RK_WIFI_RUNNING_State_e *pState) {
	int ret = 0;
	char str[128];

	if (!pState)
		return -1;

	// check wpa is running first
	exec_command("pidof wpa_supplicant", str, 128);
	if (0 == strlen(str)) {
		*pState = RK_WIFI_State_IDLE;
		return ret;
	}

	// check whether wifi connected
	exec_command("wpa_cli -iwlan0 status | grep wpa_state | awk -F '=' '{printf $2}'", str, 128);
	if (0 == strncmp(str, "COMPLETED", 9)) {
		*pState = RK_WIFI_State_CONNECTED;
		return ret;
	} else {
		*pState = RK_WIFI_State_DISCONNECTED;
		return ret;
	}
}

int RK_wifi_running_getConnectionInfo(RK_WIFI_INFO_Connection_s *pInfo) {
	FILE *fp = NULL;
	char line[512];
	char *value;

	if (pInfo == NULL)
		return -1;

	if (remove("/tmp/status.tmp"))
		pr_err("remove /tmp/status.tmp failed!\n");
	exec_command_system("wpa_cli -iwlan0 status > /tmp/status.tmp");

	// check wpa is running first
	memset(line, 0, sizeof(line));
	exec_command("cat /tmp/status.tmp", line, 512);
	pr_info("status.tmp: %s\n", line);

	memset(line, 0, sizeof(line));
	exec_command("wpa_cli -iwlan0 status", line, 512);
	pr_info("wpa_cli status: %s\n", line);

	fp = fopen("/tmp/status.tmp", "r");
	if (!fp) {
		pr_err("fopen /tmp/status.tmp failed!\n");
		return -1;
	}

	memset(pInfo, 0, sizeof(RK_WIFI_INFO_Connection_s));
	memset(line, 0, sizeof(line));
	while (fgets(line, sizeof(line) - 1, fp)) {
		if (line[strlen(line) - 1] == '\n')
			line[strlen(line) - 1] = '\0';

		if (0 == strncmp(line, "bssid", 5)) {
			value = strchr(line, '=');
			if (value && strlen(value) > 0) {
				strncpy(pInfo->bssid, value + 1, sizeof(pInfo->bssid) - 1);
			}
		} else if (0 == strncmp(line, "freq", 4)) {
			value = strchr(line, '=');
			if (value && strlen(value) > 1) {
				pInfo->freq = atoi(value + 1);
			}
		} else if (0 == strncmp(line, "ssid", 4)) {
			value = strchr(line, '=');
			if (value && strlen(value) > 0) {
				char sname[128];
				char sname1[128];
				char utf8[128];
				memset(sname, 0, sizeof(sname));
				memset(sname1, 0, sizeof(sname));
				memset(utf8, 0, sizeof(utf8));
				spec_char_convers(value + 1, sname);
				remove_escape_character(sname, sname1);
				get_encode_gbk_utf8(m_gbk_head, sname1, utf8);
				pr_info("convers str: %s, sname: %s, ori: %s\n", value + 1, sname1, utf8);
				strcpy(pInfo->ssid, utf8);
			}
		} else if (0 == strncmp(line, "id", 2)) {
			value = strchr(line, '=');
			if (value && strlen(value) > 1) {
				pInfo->id = atoi(value + 1);
			}
		} else if (0 == strncmp(line, "mode", 4)) {
			value = strchr(line, '=');
			if (value && strlen(value) > 0) {
				strncpy(pInfo->mode, value + 1, sizeof(pInfo->mode) - 1);
			}
		} else if (0 == strncmp(line, "wpa_state", 9)) {
			value = strchr(line, '=');
			if (value && strlen(value) > 0) {
				strncpy(pInfo->wpa_state, value + 1, sizeof(pInfo->wpa_state) - 1);
			}
		} else if (0 == strncmp(line, "ip_address", 10)) {
			value = strchr(line, '=');
			if (value && strlen(value) > 0) {
				strncpy(pInfo->ip_address, value + 1, sizeof(pInfo->ip_address) - 1);
			}
		} else if (0 == strncmp(line, "address", 7)) {
			value = strchr(line, '=');
			if (value && strlen(value) > 0) {
				strncpy(pInfo->mac_address, value + 1, sizeof(pInfo->mac_address) - 1);
			}
		}
	}
	fclose(fp);
	return 0;
}

int RK_wifi_enable_ap(char *ssid, const char *psk, const char *ip) {
	return wifi_rtl_start_hostapd(ssid, psk, ip);
}

int RK_wifi_disable_ap() { return wifi_rtl_stop_hostapd(); }

static int is_wifi_enable() {
	int ret = 0;

	if (gstate == RK_WIFI_State_OPEN) {
		if (get_pid("wpa_supplicant") > 0)
			ret = 1;
		else
			pr_info("BUG: wifi open but wpa_supplicant noexist!!!\n");
	}

	return ret;
}

int RK_wifi_enable(int enable) {
	static pthread_t start_wifi_monitor_threadId = 0;

	pr_info("[RKWIFI] start_wpa_supplicant wpa_pid: %d, monitor_id: 0x%lx\n",
	        get_pid("wpa_supplicant"), start_wifi_monitor_threadId);

	pr_info("+++++ wifi version: %s +++++\n", RK_WIFI_VERSION);

	if (enable) {
		if (!is_wifi_enable()) {
			// exec_command_system("ifconfig wlan0 down");
			exec_command_system("ifconfig wlan0 up");
			exec_command_system("ifconfig wlan0 0.0.0.0");
			exec_command_system("killall dhcpcd");
			exec_command_system("killall dnsmasq");

			if (get_pid("wpa_supplicant") > 0) {
				// exec_command_system("killall udhcpc");
				exec_command_system("killall wpa_supplicant");
				usleep(600000);
			}
			exec_command_system("wpa_supplicant -B -i wlan0 -c /data/cfg/wpa_supplicant.conf -d");
			usleep(100000);
			// exec_command_system("udhcpc -i wlan0 -t 5 &");
			exec_command_system("dhcpcd wlan0 -AL -t 0 &");

			gstate = RK_WIFI_State_OPEN;
			wifi_state_send(gstate, NULL);

			pthread_create(&start_wifi_monitor_threadId, NULL, RK_wifi_start_monitor, NULL);
			pthread_detach(start_wifi_monitor_threadId);

			pr_info("RK_wifi_enable enable ok!\n");
		}
	} else {
		if (is_wifi_enable()) {
			exec_command_system("ifconfig wlan0 down");
			exec_command_system("killall wpa_supplicant");
			// exec_command_system("killall udhcpc");
			usleep(600000);
			if (start_wifi_monitor_threadId > 0) {
				pthread_cancel(start_wifi_monitor_threadId);
				pthread_join(start_wifi_monitor_threadId, NULL);
			}
			start_wifi_monitor_threadId = 0;

			gstate = RK_WIFI_State_OFF;
			wifi_state_send(gstate, NULL);
			pr_info("RK_wifi_enable disable ok!\n");
		}
	}

	return 0;
}

int RK_wifi_scan(void) {
	char str[32];

	memset(str, 0, sizeof(str));
	exec_command("wpa_cli -iwlan0 scan", str, 32);

	if (0 != strncmp(str, "OK", 2) && 0 != strncmp(str, "ok", 2)) {
		pr_info("scan error: %s\n", str);
		return -2;
	}

	return 0;
}

char *RK_wifi_scan_r(void) { return RK_wifi_scan_r_sec(0x1F); }

char *RK_wifi_scan_r_sec(const unsigned int cols) {
	char line[256];
	char item[384];
	char col[128];
	char *scan_r, *p_strtok;
	size_t size = 0, index = 0;
	static size_t UNIT_SIZE = 512;
	FILE *fp = NULL;
	int is_utf8;
	int is_nonpsk;

	if (!(cols & 0x1F)) {
		scan_r = (char *)malloc(3 * sizeof(char));
		memset(scan_r, 0, 3);
		strcpy(scan_r, "[]");
		return scan_r;
	}

	remove("/tmp/scan_r.tmp");
	exec_command_system("wpa_cli -iwlan0 scan_r > /tmp/scan_r.tmp");

	fp = fopen("/tmp/scan_r.tmp", "r");
	if (!fp)
		return NULL;

	memset(line, 0, sizeof(line));
	fgets(line, sizeof(line), fp);

	size += UNIT_SIZE;
	scan_r = (char *)malloc(size * sizeof(char));
	memset(scan_r, 0, size);
	strcat(scan_r, "[");

	m_gbk_head = encode_gbk_reset(m_gbk_head);
	m_nonpsk_head = encode_gbk_reset(m_nonpsk_head);
	while (fgets(line, sizeof(line) - 1, fp)) {
		index = 0;
		is_nonpsk = 0;
		p_strtok = strtok(line, "\t");
		memset(item, 0, sizeof(item));
		strcat(item, "{");
		while (p_strtok) {
			if (p_strtok[strlen(p_strtok) - 1] == '\n')
				p_strtok[strlen(p_strtok) - 1] = '\0';
			if ((cols & (1 << index)) || 3 == index) {
				memset(col, 0, sizeof(col));
				if (0 == index) {
					snprintf(col, sizeof(col), "\"bssid\":\"%s\",", p_strtok);
				} else if (1 == index) {
					snprintf(col, sizeof(col), "\"frequency\":%d,", atoi(p_strtok));
				} else if (2 == index) {
					snprintf(col, sizeof(col), "\"rssi\":%d,", atoi(p_strtok));
				} else if (3 == index) {
					if (cols & (1 << index)) {
						snprintf(col, sizeof(col), "\"flags\":\"%s\",", p_strtok);
					}
					if (!strstr(p_strtok, "WPA") && !strstr(p_strtok, "WEP")) {
						is_nonpsk = 1;
					}
				} else if (4 == index) {
					char utf8[strlen(p_strtok) + 1];
					memset(utf8, 0, sizeof(utf8));

					if (strlen(p_strtok) > 0) {
						char dst[strlen(p_strtok) + 1];
						memset(dst, 0, sizeof(dst));
						spec_char_convers(p_strtok, dst);

						// Strings that will send should retain escape characters
						// Strings whether GBK or UTF8 that need save local should remove
						// escape
						// characters
						// The ssid can't contains escape character while do connect
						is_utf8 = RK_encode_is_utf8(dst, strlen(dst));
						char utf8_noescape[sizeof(utf8)];
						char dst_noescape[sizeof(utf8)];
						memset(utf8_noescape, 0, sizeof(utf8_noescape));
						memset(dst_noescape, 0, sizeof(dst_noescape));
						if (!is_utf8) {
							RK_encode_gbk_to_utf8((unsigned char *)dst, strlen(dst),
							                      (unsigned char *)utf8);
							remove_escape_character(dst, dst_noescape);
							remove_escape_character(utf8, utf8_noescape);
							m_gbk_head = encode_gbk_insert(m_gbk_head, dst_noescape, utf8_noescape);

							// if convert gbk to utf8 failed, ignore it
							if (!RK_encode_is_utf8(utf8, strlen(utf8))) {
								continue;
							}
						} else {
							strcpy(utf8, dst);
							remove_escape_character(dst, dst_noescape);
							remove_escape_character(utf8, utf8_noescape);
						}

						// Decide whether encrypted or not
						if (is_nonpsk) {
							m_nonpsk_head =
							    encode_gbk_insert(m_nonpsk_head, dst_noescape, utf8_noescape);
						}
					}
					snprintf(col, sizeof(col), "\"ssid\":\"%s\",", utf8);
				}
				strcat(item, col);
			}
			p_strtok = strtok(NULL, "\t");
			index++;
		}
		if (item[strlen(item) - 1] == ',') {
			item[strlen(item) - 1] = '\0';
		}
		strcat(item, "},");

		if (size <= (strlen(scan_r) + strlen(item)) + 3) {
			size += UNIT_SIZE;
			scan_r = (char *)realloc(scan_r, sizeof(char) * size);
		}
		strcat(scan_r, item);
	}
	if (scan_r[strlen(scan_r) - 1] == ',') {
		scan_r[strlen(scan_r) - 1] = '\0';
	}
	strcat(scan_r, "]");
	fclose(fp);
	return scan_r;
}

static int add_network() {
	char ret[8];

	memset(ret, 0, sizeof(ret));
	exec_command("wpa_cli -iwlan0 add_network", ret, 8);

	if (0 == strlen(ret))
		return -1;

	return atoi(ret);
}

static int set_network(const int id, const char *ssid, const char *psk,
                       const RK_WIFI_CONNECTION_Encryp_e encryp) {
	char str[8];
	char cmd[128];
	char wifi_ssid[128];
	char wifi_psk[128];

	strcpy(wifi_ssid, ssid);
	strcpy(wifi_psk, psk);

	// 1. set network ssid
	memset(str, 0, sizeof(str));
	memset(cmd, 0, sizeof(cmd));
	format_wifiinfo(0, wifi_ssid);
	snprintf(cmd, sizeof(cmd), "wpa_cli -iwlan0 set_network %d ssid %s", id, wifi_ssid);
	exec_command(cmd, str, 8);
	if (0 == strlen(str) || 0 != strncmp(str, "OK", 2))
		return -1;

	// 2. set network psk
	memset(str, 0, sizeof(str));
	memset(cmd, 0, sizeof(cmd));
	if (strlen(psk) == 0 && encryp == NONE) {
		snprintf(cmd, sizeof(cmd), "wpa_cli -iwlan0 set_network %d key_mgmt NONE", id);
		exec_command(cmd, str, 8);
		if (0 == strlen(str) || 0 != strncmp(str, "OK", 2))
			return -2;
	} else if (encryp == WEP) {
		snprintf(cmd, sizeof(cmd), "wpa_cli -iwlan0 set_network %d key_mgmt NONE", id);
		exec_command(cmd, str, 8);
		if (0 == strlen(str) || 0 != strncmp(str, "OK", 2))
			return -41;

		memset(str, 0, sizeof(str));
		memset(cmd, 0, sizeof(cmd));
		snprintf(cmd, sizeof(cmd), "wpa_cli -iwlan0 set_network %d wep_key0 \\\"%s\\\"", id, psk);
		exec_command(cmd, str, 8);
		if (0 == strlen(str) || 0 != strncmp(str, "OK", 2))
			return -42;
	} else if (strlen(psk) || encryp == WPA) {
		format_wifiinfo(1, wifi_psk);
		snprintf(cmd, sizeof(cmd), "wpa_cli -iwlan0 set_network %d psk \\\"%s\\\"", id, wifi_psk);
		exec_command(cmd, str, 8);
		if (0 == strlen(str) || 0 != strncmp(str, "OK", 2))
			return -3;
	}

	return 0;
}

static int set_hide_network(const int id) {
	char str[8];
	char cmd[128];

	memset(str, 0, sizeof(str));
	memset(cmd, 0, sizeof(cmd));
	snprintf(cmd, sizeof(cmd), "wpa_cli -iwlan0 set_network %d scan_ssid %d", id, 1);
	exec_command(cmd, str, 8);

	if (0 == strlen(str) || 0 != strncmp(str, "OK", 2))
		return -1;

	return 0;
}

static int clear_bssid_network(const int id) {
	char str[8];
	char cmd[128];

	memset(str, 0, sizeof(str));
	memset(cmd, 0, sizeof(cmd));
	snprintf(cmd, sizeof(cmd), "wpa_cli -iwlan0 bssid %d 00:00:00:00:00:00", id);
	exec_command(cmd, str, 8);

	if (0 == strlen(str) || 0 != strncmp(str, "OK", 2)) {
		pr_err("clear_bssid_network fail!\n");
		return -1;
	}

	return 0;
}

static int select_network(const int id) {
	char str[8];
	char cmd[128];

	clear_bssid_network(id);

	memset(str, 0, sizeof(str));
	memset(cmd, 0, sizeof(cmd));
	memset(retry_connect_cmd, 0, sizeof(retry_connect_cmd));
	snprintf(cmd, sizeof(cmd), "wpa_cli -iwlan0 select_network %d", id);
	snprintf(retry_connect_cmd, sizeof(retry_connect_cmd), "wpa_cli -iwlan0 select_network %d", id);
	exec_command(cmd, str, 8);

	if (0 == strlen(str) || 0 != strncmp(str, "OK", 2))
		return -1;

	return 0;
}

static int enable_network(const int id) {
	char str[8];
	char cmd[128];

	memset(str, 0, sizeof(str));
	memset(cmd, 0, sizeof(cmd));
	snprintf(cmd, sizeof(cmd), "wpa_cli -iwlan0 enable_network %d", id);
	exec_command(cmd, str, 8);

	if (0 == strlen(str) || 0 != strncmp(str, "OK", 2))
		return -1;

	return 0;
}

#define WIFI_CONNECT_RETRY 50
static bool check_wifi_isconnected(void) {

	pr_info("check_wifi_isconnected\n");
	bool isWifiConnected = false;
	int connect_retry_count = WIFI_CONNECT_RETRY;
	RK_WIFI_INFO_Connection_s wifiinfo;
	int flag = 1;

	for (int i = 0; i < connect_retry_count; i++) {
		sleep(1);
		memset(&wifiinfo, 0, sizeof(RK_WIFI_INFO_Connection_s));
		RK_wifi_running_getConnectionInfo(&wifiinfo);
		if (strncmp(wifiinfo.wpa_state, "COMPLETED", 9) == 0) {
			if ((strlen(wifiinfo.ip_address) != 0) &&
			    strncmp(wifiinfo.ip_address, "127.0.0.1", 9) != 0) {
				isWifiConnected = true;
				pr_info("wifi is connected.\n");
				break;
			} else {
				if ((!(i % 30)) || flag) {
					flag = 0;
					// exec_command_system("killall udhcpc");
					// usleep(300000);
					// exec_command_system("udhcpc -i wlan0 -t 10 &");
					exec_command_system("killall dhcpcd");
					usleep(300000);
					exec_command_system("dhcpcd wlan0 -AL -t 0 &");
				}
			}
		}

		pr_info("Check wifi state with none state. try more %d/%d, \n", i + 1, WIFI_CONNECT_RETRY);

		if (wifi_wrong_key == true) {
			pr_info("check_wifi_isconnected is wifi_wrong_key break.\n");
			break;
		}

		if (wifi_cancel == true) {
			exec_command_system("wpa_cli flush");
			exec_command_system("wpa_cli reconfigure");
			exec_command_system("wpa_cli -iwlan0 disable_network all");
			wifi_cancel = false;
			break;
		}
	}

	if (!isWifiConnected)
		pr_info("wifi is not connected.\n");

	return isWifiConnected;
}

static void format_wifiinfo(int flag, char *info) {
	char temp[1024];
	int j = 0;

	if (flag == 0) {
		for (int i = 0; i < strlen(info); i++) {
			sprintf(temp + 2 * i, "%02x", info[i]);
		}
		temp[strlen(info) * 2] = '\0';
		strcpy(info, temp);
		pr_info("format_wifiinfo ssid: %s\n", info);
	} else if (flag == 1) {
		for (int i = 0; i < strlen(info); i++) {
			temp[j++] = '\\';
			temp[j++] = info[i];
		}
		temp[j] = '\0';
		strcpy(info, temp);
		pr_info("format_wifiinfo password: %s\n", info);
	}
}

static int set_priority_network(const int id, int priority) {
	char str[8];
	char cmd[128];

	memset(str, 0, sizeof(str));
	memset(cmd, 0, sizeof(cmd));
	snprintf(cmd, sizeof(cmd), "wpa_cli -iwlan0 set_network %d priority %d", id, priority);
	exec_command(cmd, str, 8);

	if (0 == strlen(str) || 0 != strncmp(str, "OK", 2))
		return -1;

	return 0;
}

static void set_network_highest_priority(const int id) {
	char str[128];
	int cnt;
	int network_id;
	char cmd[128];

	exec_command("wpa_cli -i wlan0 list_network | wc -l", str, 128);
	cnt = atoi(str) - 1;
	pr_info("wifi cnt: %d(%s)\n", cnt, str);

	for (int i = 0; i < cnt; i++) {
		snprintf(cmd, sizeof(cmd), "wpa_cli -i wlan0 list_network | awk '{print $1}' | sed -n %dp",
		         i + 2);
		exec_command(cmd, str, 128);
		network_id = atoi(str);

		if (network_id == id)
			set_priority_network(network_id, cnt);
		else
			set_priority_network(network_id, 1);
	}
}

static int save_configuration() {
	exec_command_system("wpa_cli -iwlan0 enable_network all");
	exec_command_system("wpa_cli -iwlan0 save_config");
	exec_command_system("sync");

	return 0;
}

static void check_ping_test() {
	char line[1024];
	int ping_retry = 10;
	int dns_retry;

	while (ping_retry--) {
		dns_retry = 5;
		memset(line, 0, sizeof(line));
		pr_info("check dns\n");
		while (dns_retry--) {
			exec_command("cat /etc/resolv.conf", line, 1024);
			if (strstr(line, "nameserver"))
				break;
			sleep(1);
		}
		pr_info("dns ok: %s\n", line);
		pr_info("to ping\n");
		sync();
		sleep(1);
		memset(line, 0, sizeof(line));
		exec_command("ping www.baidu.com -c 1", line, 1024);
		// usleep(100000);
		pr_info("line: %s\n", line);
		// if (strstr(line, "PING www.baidu.com") && strstr(line, "bytes from")) {
		// if (strstr(line, "PING 8.8.8.8") && strstr(line, "bytes from")) {
		if (strstr(line, "bytes from")) {
			pr_info("ping ok\n");
			break;
		}
		usleep(200000);
	}
}

static void wifi_connectfail_process(int id) {
	char str[128];
	char cmd1[128];
	char cmd2[128];

	memset(str, 0, sizeof(str));
	memset(cmd1, 0, sizeof(cmd1));

	snprintf(cmd1, sizeof(cmd1), "wpa_cli -iwlan0 disable_network %d", id);
	snprintf(cmd2, sizeof(cmd2), "wpa_cli -iwlan0 remove_network %d", id);

	if (wifi_is_exist == true) {
		exec_command(cmd1, str, 128);
	} else {
		exec_command(cmd2, str, 128);
	}
	// exec_command_system("wpa_cli flush");
	// exec_command_system("wpa_cli reconfigure");
	// exec_command_system("wpa_cli reconnect");
}

static void *wifi_connect_state_check(void *arg) {
	RK_WIFI_RUNNING_State_e state = -1;
	bool isconnected;
	// RK_WIFI_INFO_Connection_s cndinfo;
	int id = *((int *)arg);

	prctl(PR_SET_NAME, "wifi_connect_state_check");
	pr_info("[%s]\n", __func__);

	isconnected = check_wifi_isconnected();

	if (isconnected == 1) {
		// char cmd[128];
		// char str[8];
		RK_WIFI_SAVED_INFO wsi;

		// RK_wifi_running_getConnectionInfo(&cndinfo);
		// snprintf(cmd, sizeof(cmd), "wpa_cli -iwlan0 bssid %d %s", cndinfo.id,
		// cndinfo.bssid);
		// exec(cmd, str);
		// pr_info("set bssid: %s\n", cmd);

		RK_wifi_getSavedInfo(&wsi);

		save_configuration();
		check_ping_test();
		state = RK_WIFI_State_DHCP_OK;
	} else {
		if (wifi_wrong_key == false)
			state = RK_WIFI_State_CONNECTFAILED;
		wifi_connectfail_process(id);
	}

	wifi_is_exist = false;

	if (state < 0)
		return NULL;

	wifi_state_send(state, NULL);
	return NULL;
}

void save_connect_info(const char *ssid, const char *bssid) {
	int len;

	memset(&connect_info, 0, sizeof(RK_WIFI_CONNECT_INFO));

	if (ssid) {
		len = SSID_BUF_LEN > strlen(ssid) ? strlen(ssid) : SSID_BUF_LEN;
		strncpy(connect_info.ssid, ssid, len);
	}

	if (bssid) {
		len = BSSID_BUF_LEN > strlen(bssid) ? strlen(bssid) : BSSID_BUF_LEN;
		strncpy(connect_info.bssid, bssid, len);
	}
}

int RK_wifi_connect(const char *ssid, const char *psk) {
	int ret = 0;

	if (!ssid) {
		pr_err("%s: invalid ssid\n", __func__);
		return -1;
	}

	if (wifi_connect_lock == true) {
		pr_err("RK_wifi is connecting!\n");
		return -1;
	}

	wifi_connect_lock = true;
	ret = RK_wifi_connect1(ssid, psk, WPA, 0);
	wifi_connect_lock = false;

	return ret;
}

int RK_wifi_connect1(const char *ssid, const char *psk, const RK_WIFI_CONNECTION_Encryp_e encryp,
                     const int hide) {
	int id, ret;
	char ori[strlen(ssid) + 1];

	if (!ssid) {
		pr_err("%s: invalid ssid\n", __func__);
		return -1;
	}
	pr_info("[%s] ssid: %s, psk: %s\n", __func__, ssid, psk);

	save_connect_info(ssid, NULL);
	wifi_state_send(RK_WIFI_State_CONNECTING, NULL);

	exec_command_system("wpa_cli -iwlan0 disable_network all");
	wifi_wrong_key = false;
	usleep(100000);

	if (save_last_ap) {
		exec_command_system("cp /data/cfg/wpa_supplicant.conf /data/cfg/wpa_supplicant.conf.bak");
		exec_command_system("wpa_cli flush");
		sleep(100000);
		exec_command_system("wpa_cli save_config");
	}

	if ((id = RK_wifi_search_with_ssid(ssid)) < 0) {
		id = add_network();
		if (id < 0) {
			pr_err("add_network id: %d failed!\n", id);
			goto fail;
		}
	}

	connecting_id = id;

	memset(ori, 0, sizeof(ori));
	if ((psk == NULL) || is_non_psk(ssid)) {
		pr_info("%s: is none psk, ssid:\"%s\" ssid_len:%lu\n", __func__, ssid, strlen(ssid));
		get_encode_gbk_ori(m_nonpsk_head, ssid, ori);

		ret = set_network(id, ori, "", NONE);
		if (0 != ret) {
			pr_info("%s: set_network failed. ssid:\"%s\"\n", __func__, ssid);
			goto fail;
		}
	} else {
		pr_info("%s: ssid:\"%s\" ssid_len:%lu; psk:\"%s\", encryp: %d.\n", __func__, ssid,
		        strlen(ssid), psk, encryp);
		get_encode_gbk_ori(m_gbk_head, ssid, ori);

		ret = set_network(id, ori, psk, encryp);
		if (0 != ret) {
			pr_info("%s: set_network failed. ssid:\"%s\", psk:\"%s\"\n", __func__, ssid, psk);
			goto fail;
		}
	}
	pr_info("%s: ori:\"%s\" ori_len:%lu\n", __func__, ori, strlen(ori));

	set_network_highest_priority(id);

	ret = set_hide_network(id);
	if (0 != ret) {
		pr_err("%s: set_hide_network id: %d failed!\n", __func__, id);
		goto fail;
	}

	ret = select_network(id);
	if (0 != ret) {
		pr_err("%s: select_network id: %d failed!\n", __func__, id);
		goto fail;
	}

	ret = enable_network(id);
	if (0 != ret) {
		pr_err("%s: enable_network id: %d failed!\n", __func__, id);
		goto fail;
	}

	pthread_t pth;
	pthread_create(&pth, NULL, wifi_connect_state_check, &connecting_id);

	return 0;

fail:
	wifi_state_send(RK_WIFI_State_CONNECTFAILED, NULL);
	return -1;
}

int RK_wifi_forget_with_ssid(const char *ssid) {
	int id;
	char cmd[64];
	char str[8];

	pr_info("[%s]: ssid %s\n", __func__, ssid);

	if (!ssid) {
		pr_err("%s: ssid is null\n", __func__);
		return -1;
	}

	id = RK_wifi_search_with_ssid(ssid);
	if (id < 0) {
		pr_err("RK_wifi_forget_with_ssid [%s] not found!\n", ssid);
		return -1;
	}

	snprintf(cmd, sizeof(cmd), "wpa_cli -iwlan0 remove_network %d", id);
	exec_command(cmd, str, 8);
	if (0 == strlen(str) || 0 != strncmp(str, "OK", 2))
		return -1;

	exec_command_system("wpa_cli -iwlan0 save");

	return 0;
}

int RK_wifi_forget_with_bssid(const char *bssid) {
	int id;
	char cmd[64];
	char str[8];

	if (!bssid) {
		pr_err("%s: bssid is null\n", __func__);
		return -1;
	}

	if (strlen(bssid) < 17) {
		pr_err("%s: invalid bssid %s\n", __func__, bssid);
		return -1;
	}

	id = RK_wifi_search_with_bssid(bssid);
	if (id < 0) {
		pr_err("RK_wifi_forget_with_bssid %d not found!\n", id);
		return -1;
	}

	snprintf(cmd, sizeof(cmd), "wpa_cli -iwlan0 remove_network %d", id);
	exec_command(cmd, str, 8);

	if (0 == strlen(str) || 0 != strncmp(str, "OK", 2))
		return -1;

	exec_command_system("wpa_cli -iwlan0 save");
	exec_command_system("sync");

	return 0;
}

int RK_wifi_cancel(void) {
	int timeout = 8;

	if (wifi_connect_lock == false) {
		pr_info("wifi dont connecting!");
		return -1;
	}

	wifi_cancel = true;

	while (timeout--) {
		if (wifi_cancel == false)
			break;
		sleep(1);
	}

	if (wifi_cancel == false)
		return 0;
	else
		return -1;
}

int RK_wifi_connect_with_ssid(char *ssid) {
	int id, ret;

	wifi_wrong_key = false;
	wifi_is_exist = true;

	pr_err("%s: %s\n", __func__, ssid);

	if (!ssid) {
		pr_err("%s: ssid is null\n", __func__);
		goto fail;
	}

	id = RK_wifi_search_with_ssid(ssid);
	if (id < 0) {
		pr_err("RK_wifi_connect_with_bssid %d not found!\n", id);
		goto fail;
	}

	connecting_id = id;

	save_connect_info(NULL, ssid);
	wifi_state_send(RK_WIFI_State_CONNECTING, NULL);

	exec_command_system("wpa_cli -iwlan0 disable_network all");

	ret = select_network(id);
	if (0 != ret) {
		pr_err("select_network id: %d failed!\n", id);
		goto fail;
	}

	set_network_highest_priority(id);

	ret = enable_network(id);
	if (0 != ret) {
		pr_err("enable_network id: %d failed!\n", id);
		goto fail;
	}

	pthread_t pth;
	pthread_create(&pth, NULL, wifi_connect_state_check, &connecting_id);

	return 0;

fail:
	wifi_state_send(RK_WIFI_State_CONNECTFAILED, NULL);
	wifi_is_exist = false;
	return -1;
}

int RK_wifi_connect_with_bssid(const char *bssid) {
	int id, ret;

	wifi_wrong_key = false;

	pr_err("%s: %s\n", __func__, bssid);

	if (!bssid) {
		pr_err("%s: bssid is null\n", __func__);
		return -1;
	}

	if (strlen(bssid) < 17) {
		pr_err("%s: invalid bssid %s\n", __func__, bssid);
		return -1;
	}

	id = RK_wifi_search_with_bssid(bssid);
	if (id < 0) {
		pr_err("RK_wifi_connect_with_bssid %d not found!\n", id);
		return -1;
	}

	connecting_id = id;

	save_connect_info(NULL, bssid);
	wifi_state_send(RK_WIFI_State_CONNECTING, NULL);

	exec_command_system("wpa_cli -iwlan0 disable_network all");

	ret = select_network(id);
	if (0 != ret) {
		pr_err("select_network id: %d failed!\n", id);
		goto fail;
	}

	set_network_highest_priority(id);

	ret = enable_network(id);
	if (0 != ret) {
		pr_err("enable_network id: %d failed!\n", id);
		goto fail;
	}

	pthread_t pth;
	pthread_create(&pth, NULL, wifi_connect_state_check, &connecting_id);

	return 0;

fail:
	wifi_state_send(RK_WIFI_State_CONNECTFAILED, NULL);
	return -1;
}

int RK_wifi_disconnect_network(void) {
	exec_command_system("wpa_cli -iwlan0 disconnect");
	return 0;
}

int RK_wifi_restart_network(void) { return 0; }

int RK_wifi_set_hostname(const char *name) {
	if (!name) {
		pr_err("%s: name is null\n", __func__);
		return -1;
	}

	sethostname(name, strlen(name));
	return 0;
}

int RK_wifi_get_hostname(char *name, int len) {
	if (name)
		gethostname(name, len);

	return 0;
}

int RK_wifi_reset(void) {
	if (get_pid("wpa_supplicant")) {
		exec_command_system("wpa_cli -i wlan0 flush");
		exec_command_system("wpa_cli -i wlan0 save");
	} else {
		exec_command_system("rm /data/cfg/wpa_supplicant.conf");
		exec_command_system("cp /etc/wpa_supplicant.conf /data/cfg/wpa_supplicant.conf");
	}
	exec_command_system("sync");

	return 1;
}

int RK_wifi_recovery(void) {
	if (save_last_ap) {
		exec_command_system("cp /data/cfg/wpa_supplicant.conf.bak /data/cfg/wpa_supplicant.conf");
	}

	exec_command_system("wpa_cli -i wlan0 flush");
	exec_command_system("wpa_cli -i wlan0 reconfigure");
	exec_command_system("wpa_cli -i wlan0 reconnect");

	return 1;
}

int RK_wifi_get_mac(char *wifi_mac) {
	int sock_mac;
	struct ifreq ifr_mac;
	char mac_addr[18] = {0};

	if (!wifi_mac) {
		pr_err("%s: wifi_mac is null\n", __func__);
		return -1;
	}

	sock_mac = socket(AF_INET, SOCK_STREAM, 0);

	if (sock_mac == -1) {
		pr_info("create mac socket failed.");
		return -1;
	}

	memset(&ifr_mac, 0, sizeof(ifr_mac));
	strncpy(ifr_mac.ifr_name, "wlan0", sizeof(ifr_mac.ifr_name) - 1);

	if ((ioctl(sock_mac, SIOCGIFHWADDR, &ifr_mac)) < 0) {
		pr_info("Mac socket ioctl failed.");
		return -1;
	}

	sprintf(
	    mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X", (unsigned char)ifr_mac.ifr_hwaddr.sa_data[0],
	    (unsigned char)ifr_mac.ifr_hwaddr.sa_data[1], (unsigned char)ifr_mac.ifr_hwaddr.sa_data[2],
	    (unsigned char)ifr_mac.ifr_hwaddr.sa_data[3], (unsigned char)ifr_mac.ifr_hwaddr.sa_data[4],
	    (unsigned char)ifr_mac.ifr_hwaddr.sa_data[5]);

	close(sock_mac);
	strncpy(wifi_mac, mac_addr, 18);
	pr_info("the wifi mac : %s\r\n", wifi_mac);

	return 0;
}

int RK_wifi_has_config() {
	FILE *fd;
	fd = fopen("/data/cfg/wpa_supplicant.conf", "r");
	if (fd == NULL)
		return 0;

	fseek(fd, 0L, SEEK_END);
	int len = ftell(fd);
	char *buf = (char *)malloc(len + 1);
	fseek(fd, 0L, SEEK_SET);
	fread(buf, 1, len, fd);
	fclose(fd);

	buf[len] = '\0';
	if (strstr(buf, "network") && strstr(buf, "ssid")) {
		free(buf);
		return 1;
	}

	free(buf);
	return 0;
}

int RK_wifi_ping(char *address) {
	// return rk_ping(address);
	return 0;
}

#define EVENT_BUF_SIZE 1024
#define PROPERTY_VALUE_MAX 32
#define PROPERTY_KEY_MAX 32
#include <poll.h>
#include <wpa_ctrl.h>
static void wifi_close_sockets();
static const char WPA_EVENT_IGNORE[] = "CTRL-EVENT-IGNORE ";
static const char IFNAME[] = "IFNAME=";
static const char IFACE_DIR[] = "/var/run/wpa_supplicant";
#define WIFI_CHIP_TYPE_PATH "/sys/class/rkwifi/chip"
#define WIFI_DRIVER_INF "/sys/class/rkwifi/driver"
#define IFNAMELEN (sizeof(IFNAME) - 1)
static struct wpa_ctrl *ctrl_conn;
static struct wpa_ctrl *monitor_conn;
#define DBG_NETWORK 1

static int exit_sockets[2];
static char primary_iface[PROPERTY_VALUE_MAX] = "wlan0";

#define HOSTAPD "hostapd"
#define WPA_SUPPLICANT "wpa_supplicant"
#define DNSMASQ "dnsmasq"
#define SIMPLE_CONFIG "simple_config"
#define SMART_CONFIG "smart_config"
#define UDHCPC "udhcpc"

static int get_pid(const char Name[]) {
	char name[32] = {0};
	strcpy(name, Name);
	name[31] = '\0';
	char cmdresult[256] = {0};
	char cmd[64] = {0};
	FILE *pFile = NULL;
	int pid = 0;

	sprintf(cmd, "pidof %s", name);
	pFile = popen(cmd, "r");
	if (pFile != NULL) {
		while (fgets(cmdresult, sizeof(cmdresult), pFile)) {
			pid = atoi(cmdresult);
			break;
		}
		pclose(pFile);
	}
	return pid;
}

static void wifi_close_sockets() {
	if (ctrl_conn != NULL) {
		wpa_ctrl_close(ctrl_conn);
		ctrl_conn = NULL;
	}

	if (monitor_conn != NULL) {
		wpa_ctrl_close(monitor_conn);
		monitor_conn = NULL;
	}

	if (exit_sockets[0] >= 0) {
		close(exit_sockets[0]);
		exit_sockets[0] = -1;
	}

	if (exit_sockets[1] >= 0) {
		close(exit_sockets[1]);
		exit_sockets[1] = -1;
	}
}

static int str_starts_with(char *str, char *search_str) {
	if ((str == NULL) || (search_str == NULL))
		return 0;
	return (strstr(str, search_str) == str);
}

static void get_wifi_info_by_event(char *event, RK_WIFI_RUNNING_State_e state,
                                   RK_WIFI_INFO_Connection_s *info) {
	int len = 0;
	char buf[10];
	char *start_tag = NULL, *end_tag = NULL, *id_tag = NULL, *reason_tag = NULL;

	if (event == NULL)
		return;

	memset(info, 0, sizeof(RK_WIFI_INFO_Connection_s));
	RK_wifi_running_getConnectionInfo(info);

	switch (state) {
	case RK_WIFI_State_DISCONNECTED:
		start_tag = strstr(event, "bssid=");
		if (start_tag)
			strncpy(info->bssid, start_tag + strlen("bssid="), 17);

		// strncpy(info->ssid, connect_info.ssid, SSID_BUF_LEN);
		reason_tag = strstr(event, "reason=");
		if (reason_tag) {
			memset(buf, 0, sizeof(buf));
			strncpy(buf, reason_tag + strlen("reason="), 2);
			info->reason = atoi(buf);
		}

		break;

	case RK_WIFI_State_CONNECTFAILED_WRONG_KEY:
		start_tag = strstr(event, "ssid=\"");
		if (start_tag) {
			end_tag = strstr(event, "\" auth_failures");
			if (!end_tag) {
				pr_err("%s: don't find end tag\n", __func__);
				break;
			}
			len = strlen(start_tag) - strlen(end_tag) - strlen("ssid=\"");
			char value[128] = {0};
			char sname[128];
			char sname1[128];
			char utf8[128];
			memset(sname, 0, sizeof(sname));
			memset(sname1, 0, sizeof(sname));
			memset(utf8, 0, sizeof(utf8));
			strncpy(value, start_tag + strlen("ssid=\""), len);
			spec_char_convers(value, sname);
			remove_escape_character(sname, sname1);
			get_encode_gbk_utf8(m_gbk_head, sname1, utf8);
			pr_info("convers str: %s, sname: %s, ori: %s\n", value, sname1, utf8);
			strcpy(info->ssid, utf8);
		}

		id_tag = strstr(event, "id=");
		if (id_tag) {
			len = strlen(id_tag) - strlen("id=") - strlen(start_tag) - 1;
			memset(buf, 0, sizeof(buf));
			strncpy(buf, id_tag + strlen("id="), len);
			info->id = atoi(buf);
		}

		strcpy(info->bssid, connect_info.bssid);
		info->bssid[BSSID_BUF_LEN - 1] = '\0';
		break;
	default:
		break;
	}
}

static void get_valid_connect_info(RK_WIFI_INFO_Connection_s *info) {
	int count = 10;

	while (count--) {
		RK_wifi_running_getConnectionInfo(info);
		if (!info->id && !strlen(info->ssid) && !strlen(info->bssid) && !info->freq &&
		    !strlen(info->mode) && !strlen(info->ip_address) && !strlen(info->mac_address) &&
		    !strlen(info->wpa_state)) {
			pr_info("wait to get valid connect info\n");
			usleep(100000);
		} else {
			break;
		}
	}
}

static int dispatch_event(char *event) {
	RK_WIFI_INFO_Connection_s info;

	if (strstr(event, "CTRL-EVENT-BSS") || strstr(event, "CTRL-EVENT-TERMINATING"))
		return 0;

	pr_info("%s: %s\n", __func__, event);

	if (str_starts_with(event, (char *)WPA_EVENT_DISCONNECTED)) {
		pr_info("%s: wifi is disconnect\n", __FUNCTION__);
		exec_command_system("ip addr flush dev wlan0");
		get_wifi_info_by_event(event, RK_WIFI_State_DISCONNECTED, &info);
		wifi_state_send(RK_WIFI_State_DISCONNECTED, &info);
		exec_command_system("wpa_cli -i wlan0 reconnect");
	} else if (str_starts_with(event, (char *)WPA_EVENT_CONNECTED)) {
		pr_info("%s: wifi is connected\n", __func__);
		get_valid_connect_info(&info);
		wifi_state_send(RK_WIFI_State_CONNECTED, &info);
	} else if (str_starts_with(event, (char *)WPA_EVENT_SCAN_RESULTS)) {
		pr_info("%s: wifi event results\n", __func__);
		exec_command_system("echo 1 > /tmp/scan_r");
		wifi_state_send(RK_WIFI_State_SCAN_RESULTS, NULL);
	} else if (strstr(event, "reason=WRONG_KEY")) {
		wifi_wrong_key = true;
		pr_info("%s: wifi reason=WRONG_KEY \n", __func__);
		get_wifi_info_by_event(event, RK_WIFI_State_CONNECTFAILED_WRONG_KEY, &info);
		wifi_state_send(RK_WIFI_State_CONNECTFAILED_WRONG_KEY, &info);
	} else if (str_starts_with(event, (char *)WPA_EVENT_TERMINATING)) {
		pr_info("%s: wifi is WPA_EVENT_TERMINATING!\n", __func__);
		wifi_close_sockets();
		return -1;
	}

	return 0;
}

static int check_wpa_supplicant_state() {
	int wpa_supplicant_pid = 0;
	wpa_supplicant_pid = get_pid(WPA_SUPPLICANT);
	// pr_info("%s: wpa_supplicant_pid = %d\n",__FUNCTION__,wpa_supplicant_pid);
	if (wpa_supplicant_pid > 0) {
		return 1;
	}
	return 0;
}

static int wifi_ctrl_recv(char *reply, size_t *reply_len) {
	int res;
	int ctrlfd = wpa_ctrl_get_fd(monitor_conn);
	struct pollfd rfds[2];

	memset(rfds, 0, 2 * sizeof(struct pollfd));
	rfds[0].fd = ctrlfd;
	rfds[0].events |= POLLIN;
	rfds[1].fd = exit_sockets[1];
	rfds[1].events |= POLLIN;
	do {
		res = poll(rfds, 2, 30000);
		if (res < 0) {
			pr_info("Error poll = %d\n", res);
			return res;
		} else if (res == 0) {
			/* timed out, check if supplicant is activeor not .. */
			res = check_wpa_supplicant_state();
			if (res < 0)
				return -2;
		}
	} while (res == 0);

	if (rfds[0].revents & POLLIN) {
		return wpa_ctrl_recv(monitor_conn, reply, reply_len);
	}
	return -2;
}

static int wifi_wait_on_socket(char *buf, size_t buflen) {
	size_t nread = buflen - 1;
	int result;
	char *match, *match2;

	if (monitor_conn == NULL) {
		return snprintf(buf, buflen, "IFNAME=%s %s - connection closed", primary_iface,
		                WPA_EVENT_TERMINATING);
	}

	result = wifi_ctrl_recv(buf, &nread);

	/* Terminate reception on exit socket */
	if (result == -2) {
		return snprintf(buf, buflen, "IFNAME=%s %s - connection closed", primary_iface,
		                WPA_EVENT_TERMINATING);
	}

	if (result < 0) {
		// pr_info("wifi_ctrl_recv failed: %s\n", strerror(errno));
		// return snprintf(buf, buflen, "IFNAME=%s %s - recv error",
		//	primary_iface, WPA_EVENT_TERMINATING);
	}

	buf[nread] = '\0';

	/* Check for EOF on the socket */
	if (result == 0 && nread == 0) {
		/* Fabricate an event to pass up */
		pr_info("Received EOF on supplicant socket\n");
		return snprintf(buf, buflen, "IFNAME=%s %s - signal 0 received", primary_iface,
		                WPA_EVENT_TERMINATING);
	}

	if (strncmp(buf, IFNAME, IFNAMELEN) == 0) {
		match = strchr(buf, ' ');
		if (match != NULL) {
			if (match[1] == '<') {
				match2 = strchr(match + 2, '>');
				if (match2 != NULL) {
					nread -= (match2 - match);
					memmove(match + 1, match2 + 1, nread - (match - buf) + 1);
				}
			}
		} else {
			return snprintf(buf, buflen, "%s", WPA_EVENT_IGNORE);
		}
	} else if (buf[0] == '<') {
		match = strchr(buf, '>');
		if (match != NULL) {
			nread -= (match + 1 - buf);
			memmove(buf, match + 1, nread + 1);
			if (0)
				pr_info("supplicant generated event without interface - %s\n", buf);
		}
	} else {
		if (0)
			pr_info("supplicant generated event without interface and without "
			        "message level - %s\n",
			        buf);
	}

	return nread;
}

static int wifi_connect_on_socket_path(const char *path) {
	if (!check_wpa_supplicant_state()) {
		pr_info("%s: wpa_supplicant is not ready\n", __FUNCTION__);
		return -1;
	}

	ctrl_conn = wpa_ctrl_open(path);
	if (ctrl_conn == NULL) {
		pr_info("Unable to open connection to supplicant on \"%s\": %s\n", path, strerror(errno));
		return -1;
	}
	monitor_conn = wpa_ctrl_open(path);
	if (monitor_conn == NULL) {
		wpa_ctrl_close(ctrl_conn);
		ctrl_conn = NULL;
		return -1;
	}
	if (wpa_ctrl_attach(monitor_conn) != 0) {
		wpa_ctrl_close(monitor_conn);
		wpa_ctrl_close(ctrl_conn);
		ctrl_conn = monitor_conn = NULL;
		return -1;
	}

	if (socketpair(AF_UNIX, SOCK_STREAM, 0, exit_sockets) == -1) {
		wpa_ctrl_close(monitor_conn);
		wpa_ctrl_close(ctrl_conn);
		ctrl_conn = monitor_conn = NULL;
		return -1;
	}
	return 0;
}

/* Establishes the control and monitor socket connections on the interface */
static int wifi_connect_to_supplicant() {
	static char path[1024];
	int count = 10;

	pr_info("%s \n", __FUNCTION__);
	while (count-- > 0) {
		if (access(IFACE_DIR, F_OK) == 0)
			break;
		sleep(1);
	}

	snprintf(path, sizeof(path), "%s/%s", IFACE_DIR, primary_iface);

	return wifi_connect_on_socket_path(path);
}

static void *RK_wifi_start_monitor(void *arg) {
	char eventStr[EVENT_BUF_SIZE];
	int ret;

	prctl(PR_SET_NAME, "RK_wifi_start_monitor");

	if ((ret = wifi_connect_to_supplicant()) != 0) {
		pr_info("%s, connect to supplicant fail.\n", __FUNCTION__);
		return NULL;
	}

	for (;;) {
		memset(eventStr, 0, EVENT_BUF_SIZE);
		if (!wifi_wait_on_socket(eventStr, EVENT_BUF_SIZE))
			continue;

		if (dispatch_event(eventStr)) {
			pr_info("disconnecting from the supplicant, no more events\n");
			break;
		}
	}

	return NULL;
}
