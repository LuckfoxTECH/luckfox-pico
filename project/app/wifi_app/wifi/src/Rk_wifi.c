// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
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
#include "RK_encode.h"
#include "slog.h"
#include "utility.h"

static bool save_last_ap = false;
static int connecting_id = -1;
static volatile bool wifi_onoff_flag = -1;

static RK_WIFI_RUNNING_State_e gstate = RK_WIFI_State_OFF;

static char retry_connect_cmd[128];

typedef struct {
	char ssid[SSID_BUF_LEN];
	char bssid[BSSID_BUF_LEN];
} RK_WIFI_CONNECT_INFO;

static RK_WIFI_CONNECT_INFO connect_info =
{
	"",
	"",
};

typedef struct RK_WIFI_encode_gbk {
	char ori[128];
	char utf8[128];
	struct RK_WIFI_encode_gbk *next;
} RK_WIFI_encode_gbk_t;

static int _RK_wifi_getSavedInfo(RK_WIFI_SAVED_INFO_s **pInfo, int *ap_cnt);
static int _RK_wifi_running_getConnectionInfo(RK_WIFI_INFO_Connection_s* pInfo);

// GBK编码路由链表
static RK_WIFI_encode_gbk_t *m_gbk_head = NULL;
// 无密码路由链表
static RK_WIFI_encode_gbk_t *m_nonpsk_head = NULL;

static RK_WIFI_encode_gbk_t* encode_gbk_insert(RK_WIFI_encode_gbk_t *head, const char *ori, const char *utf8)
{
	if ((ori == NULL || strlen(ori) == 0) || (utf8 == NULL || strlen(utf8) == 0))
		return head;

	RK_WIFI_encode_gbk_t *gbk = (RK_WIFI_encode_gbk_t*) malloc(sizeof(RK_WIFI_encode_gbk_t));
	memset(gbk->ori, 0, sizeof(gbk->ori));
	memset(gbk->utf8, 0, sizeof(gbk->utf8));

	if (strlen(ori) >= sizeof(gbk->ori) || strlen(utf8) >= sizeof(gbk->utf8)) {
		free(gbk);
		return head;
	}

	strcat(gbk->ori, ori);
	strcat(gbk->utf8, utf8);
	pr_info("encode_gbk_insert gbk->ori: %s, gbk->utf8: %s\n", gbk->ori, gbk->utf8);

	gbk->next = head;
	head = gbk;

	return head;
}

static RK_WIFI_encode_gbk_t* encode_gbk_reset(RK_WIFI_encode_gbk_t *head)
{
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

static char* get_encode_gbk_ori(RK_WIFI_encode_gbk_t* head, const char* str, char* dst)
{
	int is_gbk = 0;
	RK_WIFI_encode_gbk_t *gbk = head;

	while (gbk) {
		pr_info("get_encode_gbk_ori gbk->ori: %s, gbk->utf8: %s, str: %s\n", gbk->ori, gbk->utf8, str);
		if (strcmp(gbk->utf8, str) == 0) {
			is_gbk = 1;
			strncpy(dst, gbk->ori, strlen(gbk->ori));
			break;
		}
		gbk = gbk->next;
	}
	if (!is_gbk) {
		strncpy(dst, str, strlen(str));
	}

	return dst;
}

static char* get_encode_gbk_utf8(RK_WIFI_encode_gbk_t* head, const char* str, char* dst)
{
	int is_gbk = 0;
	RK_WIFI_encode_gbk_t *gbk = head;

	while (gbk) {
		pr_info("get_encode_gbk_utf8 gbk->ori: %s, gbk->utf8: %s, str: %s\n", gbk->ori, gbk->utf8, str);
		if (strcmp(gbk->ori, str) == 0) {
			is_gbk = 1;
			strncpy(dst, gbk->utf8, strlen(gbk->utf8));
			break;
		}
		gbk = gbk->next;
	}
	if (!is_gbk) {
		strncpy(dst, str, strlen(str));
	}

	return dst;
}

static int is_non_psk(const char* str)
{
	RK_WIFI_encode_gbk_t *nonpsk = m_nonpsk_head;
	while (nonpsk) {
		if (strcmp(nonpsk->utf8, str) == 0) {
			return true;
		}
		nonpsk = nonpsk->next;
	}

	return false;
}

static int priority = 0;
static volatile bool wifi_wrong_key = false;
static volatile bool wifi_cancel = false;
static volatile bool wifi_connect_lock = false;
static volatile bool wifi_is_exist = false;
static volatile bool wpa_exit = false;
static volatile bool is_wep = false;
static int wep_id = -1;

static void format_wifiinfo(int flag, char *info);
static void *RK_wifi_start_monitor(void *arg);

static char* wifi_state[] = {
	"RK_WIFI_State_IDLE",
	"RK_WIFI_State_CONNECTING",
	"RK_WIFI_State_CONNECTFAILED",
	"RK_WIFI_State_CONNECTFAILED_WRONG_KEY",
	"RK_WIFI_State_CONNECTED",
	"RK_WIFI_State_DISCONNECTED",
	"RK_WIFI_State_OPEN",
	"RK_WIFI_State_OFF",
	"RK_WIFI_State_SCAN_RESULTS",
	"RK_WIFI_State_DHCP_OK",
};

static void wifi_state_send(RK_WIFI_RUNNING_State_e state, RK_WIFI_INFO_Connection_s *info)
{
	if (m_wifi_cb == NULL)
		return;

	//pr_info("[RKWIFI]: %s: [%d]: %s\n", __func__, state, wifi_state[state]);

	if (state == RK_WIFI_State_CONNECTFAILED || state ==RK_WIFI_State_CONNECTING) {
		RK_WIFI_INFO_Connection_s cndinfo;

		_RK_wifi_running_getConnectionInfo(&cndinfo);
		strncpy(cndinfo.ssid, connect_info.ssid, SSID_BUF_LEN);
		strncpy(cndinfo.bssid, connect_info.bssid, BSSID_BUF_LEN);
		cndinfo.ssid[SSID_BUF_LEN - 1] = '\0';
		cndinfo.bssid[BSSID_BUF_LEN - 1] = '\0';
		//pr_info("[RKWIFI]: %s: %s, %s, %s, reason=%d\n", __func__, wifi_state[state], cndinfo.ssid, cndinfo.bssid, cndinfo.reason);
		m_wifi_cb(state, &cndinfo);
	} else {
		if (info) {
			info->ssid[SSID_BUF_LEN - 1] = '\0';
			info->bssid[BSSID_BUF_LEN - 1] = '\0';
			//pr_info("[RKWIFI]: %s: %s, %s, %s, reason=%d\n", __func__, wifi_state[state], info->ssid, info->bssid, info->reason);
		}
		m_wifi_cb(state, info);
	}
}


static char *remove_escape_character(const char *buf, char *dst)
{
	char buf_temp[strlen(buf) + 1];
	int i = 0;

	memset(buf_temp, 0, sizeof(buf_temp));
	while(*buf != '\0') {
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

static char *spec_char_convers(const char *buf, char *dst)
{
	char buf_temp[strlen(buf) + 1];
	int i = 0;
	unsigned long con;

	memset(buf_temp, 0, sizeof(buf_temp));
	while(*buf != '\0') {
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

static int wpa_wifi_search_with_ssid(char *ssid)
{
	RK_WIFI_SAVED_INFO_s *wsi = NULL;
	int id, len, ap_cnt = 0;
	pr_info("%s: enter\n", __func__);

	_RK_wifi_getSavedInfo(&wsi, &ap_cnt);
	if (ap_cnt <= 0)
		return -EINVAL;

	for (int i = 0; i < ap_cnt; i++) {
		if ((strlen(wsi[i].ssid) > 64) || (strlen(ssid) > 64))
			pr_err("RK_wifi_search_with_ssid ssid error!!!\n");
		pr_err("RK_wifi_search_with_ssid save_info[%d].ssid: [%s], ssid: [%s] [%d:%d:%d]\n",
				i, wsi[i].ssid, ssid, strlen(wsi[i].ssid), strlen(ssid), ap_cnt);
		if (strlen(wsi[i].ssid) > strlen(ssid))
			len = strlen(wsi[i].ssid);
		else
			len = strlen(ssid);
		if (strncmp(wsi[i].ssid, ssid, len) == 0) {
			id = wsi[i].id;
			return id;
		}
	}

	if (wsi)
		free(wsi);

	return -EINVAL;
}

static bool get_ssid_directly(char *flags, int row, int columns)
{
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
		while (p >= str  && isspace(*p)) {
				*p = '\0';
				--p;
		}

		return str;
}

static char *RK_property_trim(char *str)
{
		str = rtrim(str);
		str = ltrim(str);

		return str;
}

static int get_ssid_from_list_network(RK_WIFI_SAVED_INFO_s *info, int row, int columns)
{
	int start = 0, end = 0;
	char cmd[256], str[256];
	char ssid[256], sname[256], utf8[256];
	char *bssid;
	pr_info("%s: enter\n", __func__);

	if (!info)
		return false;

	memset(info->ssid, 0, SSID_BUF_LEN);
	memset(cmd, 0, sizeof(cmd));
	memset(str, 0, sizeof(str));

	if (get_ssid_directly(info->state, row, columns)) {
		snprintf(cmd, sizeof(cmd), "cat /tmp/save_info.txt | awk '{print $2}' | sed -n %dp", row);
		exec_command(cmd, str, 256);
		pr_info("%s: row = %d, column(2) = %s\n", __func__, row, str);
		pr_info("%s: check len str: %d\n", __func__, strlen(str));

		memset(sname, 0, sizeof(sname));
		memset(utf8, 0, sizeof(utf8));
		memset(ssid, 0, sizeof(ssid));

		str[strlen(str)-1] = '\0';
		remove_escape_character(str, ssid);
		spec_char_convers(ssid, sname);
		get_encode_gbk_utf8(m_gbk_head, sname, utf8);
		pr_info("direct: convers str: %s, sname: %s, ori: %s\n", ssid, sname, utf8);
		strncpy(info->ssid, utf8, strlen(utf8));

		return true;
	}

	snprintf(cmd, sizeof(cmd), "cat /tmp/save_info.txt | sed -n %dp", row);
	exec_command(cmd, str, 256);
	pr_info("%s: row(%d) = %s\n", __func__, row, str);

	memset(cmd, 0, sizeof(cmd));
	sprintf(cmd, "%d", info->id);
	start = strlen(cmd);

	if ((bssid = strstr(str, info->bssid)) == NULL) {
		pr_err("%s: %s can't find bssid(%s)\n", __func__, str, info->bssid);
		return false;
	}
	end = strlen(str) - strlen(bssid);

	memset(ssid, 0, sizeof(ssid));
	strncpy(ssid, str + start, end - start);
	RK_property_trim(ssid);

	memset(str, 0, sizeof(str));
	memset(sname, 0, sizeof(sname));
	memset(utf8, 0, sizeof(utf8));
	remove_escape_character(ssid, str);
	spec_char_convers(str, sname);
	get_encode_gbk_utf8(m_gbk_head, sname, utf8);
	pr_info("ndirect convers str: %s, sname: %s, ori: %s\n", str, sname, utf8);
	strncpy(info->ssid, utf8, strlen(utf8));

	return true;
}

int get_bssid_from_list_network(RK_WIFI_SAVED_INFO_s *info, int row, int columns)
{
	char cmd[128], str[128];
	int bssid_column = columns;
	pr_info("%s: enter\n", __func__);

	if (!info)
		return false;

	memset(info->bssid, 0, BSSID_BUF_LEN);
	memset(cmd, 0, 128);
	memset(str, 0, 128);

	if (strlen(info->state) > 0)
		bssid_column = columns - 1;

	snprintf(cmd, sizeof(cmd), "cat /tmp/save_info.txt | awk '{print $%d}' | sed -n %dp", bssid_column, row);
	exec_command(cmd, str, 128);
	pr_info("%s: row = %d, column(%d) = %s\n", __func__, row, bssid_column, str);

	strncpy(info->bssid, str, ((strlen(str) - 1) > BSSID_BUF_LEN) ? BSSID_BUF_LEN : (strlen(str) - 1));
	pr_info("%s: bssid: %s\n", __func__, info->bssid);

	return true;
}

int get_id_from_list_network(int row)
{
	int network_id;
	char cmd[128], str[128];

	memset(cmd, 0, 128);
	memset(str, 0, 128);
	snprintf(cmd, sizeof(cmd), "cat /tmp/save_info.txt | awk '{print $1}' | sed -n %dp", row);
	exec_command(cmd, str, 128);
	network_id = atoi(str);

	pr_info("%s: row = %d, network_id = %d\n", __func__, row, network_id);
	return network_id;
}

int get_columns_from_list_network(int row)
{
	int columns;
	char cmd[128], str[128];

	memset(cmd, 0, 128);
	memset(str, 0, 128);

	snprintf(cmd, sizeof(cmd), "cat /tmp/save_info.txt | awk '{print NF}' | sed -n %dp", row);
	exec_command(cmd, str, 128);
	columns = atoi(str);

	pr_info("%s: row = %d, columns = %d\n", __func__, row, columns);
	return columns;
}

void get_flags_from_list_network(char *flags_buf, int buf_len, int row, int columns)
{
	char cmd[128], str[128];

	memset(flags_buf, 0, buf_len);
	memset(cmd, 0, 128);
	memset(str, 0, 128);
	pr_info("%s: enter\n", __func__);

	snprintf(cmd, sizeof(cmd), "cat /tmp/save_info.txt | awk '{print $%d}' | sed -n %dp", columns, row);
	exec_command(cmd, str, 128);
	if (!strncmp(str, "[CURRENT]", strlen("[CURRENT]"))
				|| !strncmp(str, "[DISABLED]", strlen("[DISABLED]"))
				|| !strncmp(str, "[TEMP-DISABLED]", strlen("[TEMP-DISABLED]"))) {
		strncpy(flags_buf, str, ((strlen(str) - 1) > buf_len) ? buf_len : (strlen(str) - 1));
		pr_info("%s: row = %d, column(%d) = %s\n", __func__, row, columns, flags_buf);
	}
}

int wpa_wifi_getSavedInfo(RK_WIFI_SAVED_INFO_s **pInfo, int *ap_cnt)
{
	FILE *fp = NULL;
	int cnt, row, columns;
	char str[128];
	RK_WIFI_SAVED_INFO_s *pInfo_in;
	pr_info("%s: enter\n", __func__);

	exec_command_system("rm -rf /tmp/save_info.txt");
	usleep(1 * 1000);
	exec_command_system("wpa_cli -i wlan0 list_network > /tmp/save_info.txt");
	usleep(1 * 1000);

	memset(str, 0, 128);
	exec_command("cat /tmp/save_info.txt | wc -l", str, 128);
	cnt = atoi(str) - 1;

	pr_info("ap cnt: %p, &cnt: %p\n", ap_cnt, &cnt);

	if (cnt <= 0)
		return false;

	*ap_cnt = cnt;

	pInfo_in = (RK_WIFI_SAVED_INFO_s *)malloc(sizeof(RK_WIFI_SAVED_INFO_s) * cnt);
	pr_info("wifi cnt: %d(%s)(%p)\n", cnt, str, pInfo_in);

	for (int i = 0; i < cnt; i++) {
		row = i+2;

		columns = get_columns_from_list_network(row);
		pInfo_in[i].id = get_id_from_list_network(row);
		get_flags_from_list_network(pInfo_in[i].state, STATE_BUF_LEN, row, columns);
		get_bssid_from_list_network(&pInfo_in[i], row, columns);
		get_ssid_from_list_network(&pInfo_in[i], row, columns);
	}

	for (int i = 0; i < cnt; i++) {
		pr_info("id: %d, name: %s, bssid: %s, state: %s\n", pInfo_in[i].id, pInfo_in[i].ssid, pInfo_in[i].bssid,
					pInfo_in[i].state);
	}

	*pInfo = pInfo_in;

	return true;
}

static int _get_ssid_from_list_network(RK_WIFI_SAVED_INFO_s *info, int row, int columns)
{
	int start = 0, end = 0;
	char cmd[256], str[256];
	char ssid[256], sname[256], utf8[256];
	char *bssid;
	pr_info("%s: enter\n", __func__);

	if (!info)
		return false;

	memset(info->ssid, 0, SSID_BUF_LEN);
	memset(cmd, 0, sizeof(cmd));
	memset(str, 0, sizeof(str));

	if (get_ssid_directly(info->state, row, columns)) {
		snprintf(cmd, sizeof(cmd), "cat /tmp/save_network.txt | awk '{print $2}' | sed -n %dp", row);
		exec_command(cmd, str, 256);
		pr_info("%s: row = %d, column(2) = %s\n", __func__, row, str);
		pr_info("%s: check len str: %d\n", __func__, strlen(str));

		memset(sname, 0, sizeof(sname));
		memset(utf8, 0, sizeof(utf8));
		memset(ssid, 0, sizeof(ssid));

		str[strlen(str)-1] = '\0';
		remove_escape_character(str, ssid);
		spec_char_convers(ssid, sname);
		get_encode_gbk_utf8(m_gbk_head, sname, utf8);
		pr_info("direct: convers str: %s, sname: %s, ori: %s\n", ssid, sname, utf8);
		strncpy(info->ssid, utf8, strlen(utf8));

		return true;
	}

	snprintf(cmd, sizeof(cmd), "cat /tmp/save_network.txt | sed -n %dp", row);
	exec_command(cmd, str, 256);
	pr_info("%s: row(%d) = %s\n", __func__, row, str);

	memset(cmd, 0, sizeof(cmd));
	sprintf(cmd, "%d", info->id);
	start = strlen(cmd);

	if ((bssid = strstr(str, info->bssid)) == NULL) {
		pr_err("%s: %s can't find bssid(%s)\n", __func__, str, info->bssid);
		return false;
	}
	end = strlen(str) - strlen(bssid);

	memset(ssid, 0, sizeof(ssid));
	strncpy(ssid, str + start, end - start);
	RK_property_trim(ssid);

	memset(str, 0, sizeof(str));
	memset(sname, 0, sizeof(sname));
	memset(utf8, 0, sizeof(utf8));
	remove_escape_character(ssid, str);
	spec_char_convers(str, sname);
	get_encode_gbk_utf8(m_gbk_head, sname, utf8);
	pr_info("ndirect convers str: %s, sname: %s, ori: %s\n", str, sname, utf8);
	strncpy(info->ssid, utf8, strlen(utf8));

	return true;
}

int _get_bssid_from_list_network(RK_WIFI_SAVED_INFO_s *info, int row, int columns)
{
	char cmd[128], str[128];
	int bssid_column = columns;
	pr_info("%s: enter\n", __func__);

	if (!info)
		return false;

	memset(info->bssid, 0, BSSID_BUF_LEN);
	memset(cmd, 0, 128);
	memset(str, 0, 128);

	if (strlen(info->state) > 0)
		bssid_column = columns - 1;

	snprintf(cmd, sizeof(cmd), "cat /tmp/save_network.txt | awk '{print $%d}' | sed -n %dp", bssid_column, row);
	exec_command(cmd, str, 128);
	pr_info("%s: row = %d, column(%d) = %s\n", __func__, row, bssid_column, str);

	strncpy(info->bssid, str, ((strlen(str) - 1) > BSSID_BUF_LEN) ? BSSID_BUF_LEN : (strlen(str) - 1));
	return true;
}

int _get_id_from_list_network(int row)
{
	int network_id;
	char cmd[128], str[128];

	memset(cmd, 0, 128);
	memset(str, 0, 128);
	snprintf(cmd, sizeof(cmd), "cat /tmp/save_network.txt | awk '{print $1}' | sed -n %dp", row);
	exec_command(cmd, str, 128);
	network_id = atoi(str);

	pr_info("%s: row = %d, network_id = %d\n", __func__, row, network_id);
	return network_id;
}

int _get_columns_from_list_network(int row)
{
	int columns;
	char cmd[128], str[128];

	memset(cmd, 0, 128);
	memset(str, 0, 128);

	snprintf(cmd, sizeof(cmd), "cat /tmp/save_network.txt | awk '{print NF}' | sed -n %dp", row);
	exec_command(cmd, str, 128);
	columns = atoi(str);

	pr_info("%s: row = %d, columns = %d\n", __func__, row, columns);
	return columns;
}

void _get_flags_from_list_network(char *flags_buf, int buf_len, int row, int columns)
{
	char cmd[128], str[128];

	memset(flags_buf, 0, buf_len);
	memset(cmd, 0, 128);
	memset(str, 0, 128);
	pr_info("%s: enter\n", __func__);

	snprintf(cmd, sizeof(cmd), "cat /tmp/save_network.txt | awk '{print $%d}' | sed -n %dp", columns, row);
	exec_command(cmd, str, 128);
	if (!strncmp(str, "[CURRENT]", strlen("[CURRENT]"))
				|| !strncmp(str, "[DISABLED]", strlen("[DISABLED]"))
				|| !strncmp(str, "[TEMP-DISABLED]", strlen("[TEMP-DISABLED]"))) {
		strncpy(flags_buf, str, ((strlen(str) - 1) > buf_len) ? buf_len : (strlen(str) - 1));
		pr_info("%s: row = %d, column(%d) = %s\n", __func__, row, columns, flags_buf);
	}
}

static int _RK_wifi_getSavedInfo(RK_WIFI_SAVED_INFO_s **pInfo, int *ap_cnt)
{
	FILE *fp = NULL;
	int cnt, row, columns;
	char str[128];
	RK_WIFI_SAVED_INFO_s *pInfo_in;
	pr_info("%s: enter\n", __func__);

	exec_command_system("rm /tmp/save_network.txt");
	usleep(1 * 1000);
	exec_command_system("wpa_cli -i wlan0 list_network > /tmp/save_network.txt");
	usleep(3 * 1000);

	memset(str, 0, 128);
	exec_command("cat /tmp/save_network.txt | wc -l", str, 128);
	cnt = atoi(str) - 1;

	pr_info("ap cnt: %p, &cnt: %p\n", ap_cnt, &cnt);

	if (cnt <= 0)
		return false;

	*ap_cnt = cnt;

	pInfo_in = (RK_WIFI_SAVED_INFO_s *)malloc(sizeof(RK_WIFI_SAVED_INFO_s) * cnt);
	pr_info("wifi cnt: %d(%s)(%p)\n", cnt, str, pInfo_in);

	for (int i = 0; i < cnt; i++) {
		row = i+2;

		columns = _get_columns_from_list_network(row);
		pInfo_in[i].id = _get_id_from_list_network(row);
		_get_flags_from_list_network(pInfo_in[i].state, STATE_BUF_LEN, row, columns);
		_get_bssid_from_list_network(&pInfo_in[i], row, columns);
		_get_ssid_from_list_network(&pInfo_in[i], row, columns);
	}

	for (int i = 0; i < cnt; i++) {
		pr_info("id: %d, name: %s, bssid: %s, state: %s\n", pInfo_in[i].id, pInfo_in[i].ssid, pInfo_in[i].bssid,
					pInfo_in[i].state);
	}

	*pInfo = pInfo_in;

	return true;
}

static int _RK_wifi_running_getConnectionInfo(RK_WIFI_INFO_Connection_s* pInfo)
{
	FILE *fp = NULL;
	char line[512];
	char *value, *orgs;
	pr_info("%s: enter\n", __func__);

	if (pInfo == NULL)
		return false;

	if (remove("/tmp/wifi_status.tmp"))
		pr_err("remove /tmp/wifi_status.tmp failed!\n");
	exec_command_system("wpa_cli -iwlan0 status > /tmp/wifi_status.tmp");

	// check wpa is running first
	memset(line, 0, sizeof(line));
	exec_command("cat /tmp/wifi_status.tmp", line, 512);
	pr_info("wifi_status.tmp: %s\n", line);

	memset(line, 0, sizeof(line));
	exec_command("wpa_cli -iwlan0 status", line, 512);
	pr_info("wpa_cli status: %s\n", line);

	fp = fopen("/tmp/wifi_status.tmp", "r");
	if (!fp) {
		pr_err("fopen /tmp/status.tmp failed!\n");
		return false;
	}

	memset(pInfo, 0, sizeof(RK_WIFI_INFO_Connection_s));
	memset(line, 0, sizeof(line));
	while (fgets(line, sizeof(line) - 1, fp)) {
		if (line[strlen(line) - 1] == '\n')
			line[strlen(line) - 1] = '\0';

		if (0 == strncmp(line, "bssid", 5)) {
			value = strchr(line, '=');
			if (value && strlen(value) > 0) {
				strncpy(pInfo->bssid, value + 1, sizeof(pInfo->bssid));
			}
		} else if (0 == strncmp(line, "freq", 4)) {
			value = strchr(line, '=');
			if (value && strlen(value) > 1) {
				pInfo->freq = atoi(value + 1);
			}
		} else if (0 == strncmp(line, "ssid", 4)) {
			value = strchr(line, '=');
			pr_info("%s: check len value: %d\n", __func__, strlen(value));
			if (value && strlen(value) > 0) {
				char sname[256];
				char sname1[256];
				char utf8[256];
				memset(sname, 0, sizeof(sname));
				memset(sname1, 0, sizeof(sname));
				memset(utf8, 0, sizeof(utf8));
				spec_char_convers(value + 1, sname);
				remove_escape_character(sname, sname1);
				get_encode_gbk_utf8(m_gbk_head, sname1, utf8);
				orgs = value + 1;
				pr_info("[convers str: %s, sname: %s, ori: %s]\n", value + 1, sname1, utf8);
				pr_info("%d:%d\n", strlen(orgs), strlen(utf8));
				strncpy(pInfo->ssid,  utf8, strlen(utf8));
			}
		} else if (0 == strncmp(line, "id", 2)) {
			value = strchr(line, '=');
			if (value && strlen(value) > 1) {
				pInfo->id = atoi(value + 1);
			}
		} else if (0 == strncmp(line, "mode", 4)) {
			value = strchr(line, '=');
			if (value && strlen(value) > 0) {
				strncpy(pInfo->mode, value + 1, sizeof(pInfo->mode));
			}
		} else if (0 == strncmp(line, "wpa_state", 9)) {
			value = strchr(line, '=');
			if (value && strlen(value) > 0) {
				strncpy(pInfo->wpa_state, value + 1, sizeof(pInfo->wpa_state));
			}
		} else if (0 == strncmp(line, "ip_address", 10)) {
			value = strchr(line, '=');
			if (value && strlen(value) > 0) {
				strncpy(pInfo->ip_address, value + 1, sizeof(pInfo->ip_address));
			}
		} else if (0 == strncmp(line, "address", 7)) {
			value = strchr(line, '=');
			if (value && strlen(value) > 0) {
				strncpy(pInfo->mac_address, value + 1, sizeof(pInfo->mac_address));
			}
		}
	}
	fclose(fp);
	return true;
}

int wpa_wifi_running_getConnectionInfo(RK_WIFI_INFO_Connection_s* pInfo)
{
	FILE *fp = NULL;
	char line[512];
	char *value, *orgs;
	pr_info("%s: enter\n", __func__);

	if (pInfo == NULL)
		return false;
	pr_info("%s: %d\n", __func__, __LINE__);

	if (remove("/tmp/status.tmp"))
		pr_err("remove /tmp/status.tmp failed!\n");
	exec_command_system("wpa_cli -iwlan0 status > /tmp/status.tmp");
	pr_info("%s: %d\n", __func__, __LINE__);

	// check wpa is running first
	memset(line, 0, sizeof(line));
	exec_command("cat /tmp/status.tmp", line, 512);
	pr_info("status.tmp: %s\n", line);
	pr_info("%s: %d\n", __func__, __LINE__);

	memset(line, 0, sizeof(line));
	exec_command("wpa_cli -iwlan0 status", line, 512);
	pr_info("wpa_cli status: %s\n", line);
	pr_info("%s: %d\n", __func__, __LINE__);

	fp = fopen("/tmp/status.tmp", "r");
	if (!fp) {
		pr_err("fopen /tmp/status.tmp failed!\n");
		return false;
	}
	pr_info("%s: %d\n", __func__, __LINE__);

	memset(pInfo, 0, sizeof(RK_WIFI_INFO_Connection_s));
	memset(line, 0, sizeof(line));
	while (fgets(line, sizeof(line) - 1, fp)) {
		if (line[strlen(line) - 1] == '\n')
			line[strlen(line) - 1] = '\0';

		if (0 == strncmp(line, "bssid", 5)) {
			value = strchr(line, '=');
			if (value && strlen(value) > 0) {
				strncpy(pInfo->bssid, value + 1, sizeof(pInfo->bssid));
			}
		} else if (0 == strncmp(line, "freq", 4)) {
			value = strchr(line, '=');
			if (value && strlen(value) > 1) {
				pInfo->freq = atoi(value + 1);
			}
		} else if (0 == strncmp(line, "ssid", 4)) {
			value = strchr(line, '=');
			pr_info("%s: check len value: %d\n", __func__, strlen(value));
			if (value && strlen(value) > 0) {
				char sname[256];
				char sname1[256];
				char utf8[256];
				memset(sname, 0, sizeof(sname));
				memset(sname1, 0, sizeof(sname));
				memset(utf8, 0, sizeof(utf8));
				spec_char_convers(value + 1, sname);
				remove_escape_character(sname, sname1);
				get_encode_gbk_utf8(m_gbk_head, sname1, utf8);
				orgs = value + 1;
				pr_info("[convers str: %s, sname: %s, ori: %s]\n", value + 1, sname1, utf8);
				pr_info("%d:%d\n", strlen(orgs), strlen(utf8));
				strncpy(pInfo->ssid,  utf8, strlen(utf8));
			}
		} else if (0 == strncmp(line, "id", 2)) {
			value = strchr(line, '=');
			if (value && strlen(value) > 1) {
				pInfo->id = atoi(value + 1);
			}
		} else if (0 == strncmp(line, "mode", 4)) {
			value = strchr(line, '=');
			if (value && strlen(value) > 0) {
				strncpy(pInfo->mode, value + 1, sizeof(pInfo->mode));
			}
		} else if (0 == strncmp(line, "wpa_state", 9)) {
			value = strchr(line, '=');
			if (value && strlen(value) > 0) {
				strncpy(pInfo->wpa_state, value + 1, sizeof(pInfo->wpa_state));
			}
		} else if (0 == strncmp(line, "ip_address", 10)) {
			value = strchr(line, '=');
			if (value && strlen(value) > 0) {
				strncpy(pInfo->ip_address, value + 1, sizeof(pInfo->ip_address));
			}
		} else if (0 == strncmp(line, "address", 7)) {
			value = strchr(line, '=');
			if (value && strlen(value) > 0) {
				strncpy(pInfo->mac_address, value + 1, sizeof(pInfo->mac_address));
			}
		}
	}
	pr_info("%s: %d\n", __func__, __LINE__);
	fclose(fp);
	return 1;
}

static int is_wifi_enable()
{
	int ret = false;

	if (gstate == RK_WIFI_State_OPEN) {
		if (get_ps_pid("wpa_supplicant") > 0)
			ret = true;
		else
			pr_info("BUG: wifi open but wpa_supplicant noexist!!!\n");
	}

	return ret;
}

static void wifi_close_sockets();
int wpa_wifi_enable(int enable)
{
	static pthread_t start_wifi_monitor_threadId = 0;
	int wpa_exit_cnt = 50;
	int s;

	pr_info("[RKWIFI] start_wpa_supplicant wpa_pid: %d, monitor_id: 0x%lx\n",
			get_ps_pid("wpa_supplicant"), start_wifi_monitor_threadId);

	pr_info("+++++ wifi version: V1.352 [%s]+++++\n", enable ? "open":"close");

	if (enable) {
		if (!is_wifi_enable()) {
			wifi_onoff_flag = true;
			//exec_command_system("ifconfig wlan0 down");
			exec_command_system("ifconfig wlan0 up");
			exec_command_system("ifconfig wlan0 0.0.0.0");
			exec_command_system("killall dhcpcd");
			if (get_ps_pid("wpa_supplicant") > 0) {
				exec_command_system("killall -15 wpa_supplicant");
				usleep(600000);
			}
			if (get_ps_pid("wpa_supplicant") > 0) {
				exec_command_system("killall -9 wpa_supplicant");
				usleep(600000);
			}
			usleep(200000);

			//check wpa conf
			if (access("/data/cfg/wpa_supplicant.conf", F_OK) == 0) {
				exec_command_system("wpa_supplicant -B -i wlan0 -c /data/cfg/wpa_supplicant.conf -d");
			} else {
				if (access("/etc/wpa_supplicant.conf", F_OK) == 0) {
					//check /data write?
					if (exec_command_system("cp /etc/wpa_supplicant.conf /data/wpa_supplicant.conf") == 0)
						exec_command_system("wpa_supplicant -B -i wlan0 -c /data/wpa_supplicant.conf -d");
					else
						exec_command_system("wpa_supplicant -B -i wlan0 -c /etc/wpa_supplicant.conf -d");
				} else {
					pr_info("Don't found wpa_supplicant.conf so RK_wifi_enable failed!\n");
					gstate = RK_WIFI_State_OFF;
					wifi_onoff_flag = false;
					wifi_state_send(gstate, NULL);
				}
			}

			usleep(500000);
			//exec_command_system("udhcpc -i wlan0 -t 5 &");
			exec_command_system("dhcpcd wlan0 -AL -t 0 &");

			gstate = RK_WIFI_State_OPEN;
			wifi_state_send(gstate, NULL);

			s = pthread_create(&start_wifi_monitor_threadId, NULL, RK_wifi_start_monitor, NULL);
			if (s != 0) {
				errno = s;
				perror("RK_wifi_enable enable failed!");
				pr_info("RK_wifi_enable failed!\n");
				gstate = RK_WIFI_State_OFF;
				wifi_onoff_flag = false;
				wifi_state_send(gstate, NULL);
				return false;
			}
			pthread_detach(start_wifi_monitor_threadId);

			pr_info("RK_wifi_enable enable ok!\n");
		}
	} else {
		if (is_wifi_enable()) {
			exec_command_system("wpa_cli -i wlan0 enable_network all");
			exec_command_system("wpa_cli -i wlan0 save_config");
			wifi_onoff_flag = false;
			usleep(200000);
			//wifi_close_sockets();
			while ((wpa_exit == false) && (wpa_exit_cnt--)) {
				usleep(200000);
				pr_info("rkwifi turning off... wpa_exit: %d, cnt: %d\n", wpa_exit, wpa_exit_cnt);
			}
			pr_info("rkwifi turning off... wpa_exit: %d, cnt: %d\n", wpa_exit, wpa_exit_cnt);
			exec_command_system("ifconfig wlan0 down");
			usleep(300000);
			exec_command_system("killall -15 wpa_supplicant");
			usleep(300000);
			exec_command_system("killall dhcpcd");
			//exec_command_system("killall udhcpc");
			usleep(200000);
			if (start_wifi_monitor_threadId > 0) {
				//pthread_cancel(start_wifi_monitor_threadId);
				//pthread_join(start_wifi_monitor_threadId, NULL);
			}
			start_wifi_monitor_threadId = 0;

			gstate = RK_WIFI_State_OFF;
			wifi_state_send(gstate, NULL);
			if (get_ps_pid("wpa_supplicant") > 0) {
				exec_command_system("killall -9 wpa_supplicant");
				usleep(200000);
			}
			pr_info("RK_wifi_enable disable ok!\n");
		}
	}

	return true;
}

int wpa_wifi_scan(void)
{
	int ret;
	char str[32];
	pr_info("%s: enter\n", __func__);

	memset(str, 0, sizeof(str));
	exec_command("wpa_cli -iwlan0 scan", str, 32);

	if (0 != strncmp(str, "OK", 2) &&  0 != strncmp(str, "ok", 2)) {
		pr_info("scan error: %s\n", str);
		return -2;
	}

	return true;
}

static char* wpa_wifi_scan_softap(const unsigned int cols)
{
	char line[256], utf[256];
	char item[384];
	char col[128];
	char *scan_r, *p_strtok;
	size_t size = 0, index = 0;
	static size_t UNIT_SIZE = 512;
	FILE *fp = NULL;
	int is_utf8;
	int is_nonpsk;
	pr_info("%s: enter\n", __func__);

	if (!(cols & 0x1F)) {
		scan_r = (char*) malloc(3 * sizeof(char));
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
	scan_r = (char*) malloc(size * sizeof(char));
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
					snprintf(col, sizeof(col), "\"signalLevel\":%d,", atoi(p_strtok));
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
						// Strings whether GBK or UTF8 that need save local should remove escape characters
						// The ssid can't contains escape character while do connect
						is_utf8 = RK_encode_is_utf8(dst, strlen(dst));
						char utf8_noescape[sizeof(utf8)];
						char dst_noescape[sizeof(utf8)];
						memset(utf8_noescape, 0, sizeof(utf8_noescape));
						memset(dst_noescape, 0, sizeof(dst_noescape));
						if (!is_utf8) {
							RK_encode_gbk_to_utf8(dst, strlen(dst), utf8);
							remove_escape_character(dst, dst_noescape);
							remove_escape_character(utf8, utf8_noescape);
							m_gbk_head = encode_gbk_insert(m_gbk_head, dst_noescape, utf8_noescape);

							// if convert gbk to utf8 failed, ignore it
							if (!RK_encode_is_utf8(utf8, strlen(utf8))) {
								continue;
							}
						} else {
							strncpy(utf8, dst, strlen(dst));
							remove_escape_character(dst, dst_noescape);
							remove_escape_character(utf8, utf8_noescape);
						}

						// Decide whether encrypted or not
						if (is_nonpsk) {
							m_nonpsk_head = encode_gbk_insert(m_nonpsk_head, dst_noescape, utf8_noescape);
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
			scan_r = (char*) realloc(scan_r, sizeof(char) * size);
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

char *wpa_wifi_scan_for_softap(void)
{
	return wpa_wifi_scan_softap(0x1F);
}

static char *wpa_wifi_scan_r_sec(const unsigned int cols)
{
	char line[256], utf[256];
	char item[384];
	char col[128];
	char *scan_r, *p_strtok;
	size_t size = 0, index = 0;
	static size_t UNIT_SIZE = 512;
	FILE *fp = NULL;
	int is_utf8;
	int is_nonpsk;
	pr_info("%s: enter\n", __func__);

	if (!(cols & 0x1F)) {
		scan_r = (char*) malloc(3 * sizeof(char));
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
	scan_r = (char*) malloc(size * sizeof(char));
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
						// Strings whether GBK or UTF8 that need save local should remove escape characters
						// The ssid can't contains escape character while do connect
						is_utf8 = RK_encode_is_utf8(dst, strlen(dst));
						char utf8_noescape[sizeof(utf8)];
						char dst_noescape[sizeof(utf8)];
						memset(utf8_noescape, 0, sizeof(utf8_noescape));
						memset(dst_noescape, 0, sizeof(dst_noescape));
						if (!is_utf8) {
							RK_encode_gbk_to_utf8(dst, strlen(dst), utf8);
							remove_escape_character(dst, dst_noescape);
							remove_escape_character(utf8, utf8_noescape);
							m_gbk_head = encode_gbk_insert(m_gbk_head, dst_noescape, utf8_noescape);

							// if convert gbk to utf8 failed, ignore it
							if (!RK_encode_is_utf8(utf8, strlen(utf8))) {
								continue;
							}
						} else {
							strncpy(utf8, dst, strlen(dst));
							remove_escape_character(dst, dst_noescape);
							remove_escape_character(utf8, utf8_noescape);
						}

						// Decide whether encrypted or not
						if (is_nonpsk) {
							m_nonpsk_head = encode_gbk_insert(m_nonpsk_head, dst_noescape, utf8_noescape);
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
			scan_r = (char*) realloc(scan_r, sizeof(char) * size);
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

char* wpa_wifi_scan_r(void)
{
	return wpa_wifi_scan_r_sec(0x1F);
}

static int add_network()
{
	char ret[8];

	memset(ret, 0, sizeof(ret));
	exec_command("wpa_cli -iwlan0 add_network", ret, 8);

	if (0 == strlen(ret))
		return -EINVAL;

	return atoi(ret);
}

static int set_network(const int id, const char* ssid, const char* psk, const RK_WIFI_CONNECTION_Encryp_e encryp)
{
	int ret;
	char str[8];
	char cmd[512];
	char wifi_ssid[512];
	char wifi_psk[512];

	strcpy(wifi_ssid, ssid);
	strcpy(wifi_psk, psk);

	// 1. set network ssid
	memset(str, 0, sizeof(str));
	memset(cmd, 0, sizeof(cmd));
	format_wifiinfo(0, wifi_ssid);
	snprintf(cmd, sizeof(cmd), "wpa_cli -iwlan0 set_network %d ssid %s", id, wifi_ssid);
	exec_command(cmd, str, 8);
	if (0 == strlen(str) || 0 != strncmp(str, "OK", 2))
		return false;

	// 2. set network psk
	memset(str, 0, sizeof(str));
	memset(cmd, 0, sizeof(cmd));
	if (strlen(psk) == 0 && encryp == NONE) {
		snprintf(cmd, sizeof(cmd), "wpa_cli -iwlan0 set_network %d key_mgmt NONE", id);
		exec_command(cmd, str, 8);
		if (0 == strlen(str) || 0 != strncmp(str, "OK", 2))
			return false;
	}  else if (encryp == WEP) {
		snprintf(cmd, sizeof(cmd), "wpa_cli -iwlan0 set_network %d key_mgmt NONE", id);
		exec_command(cmd, str, 8);
		if (0 == strlen(str) || 0 != strncmp(str, "OK", 2))
			return false;

		memset(str, 0, sizeof(str));
		memset(cmd, 0, sizeof(cmd));
		wep_id = id;

		if ((sizeof(psk) == 5) || (sizeof(psk) == 13) || (sizeof(psk) == 16))
			snprintf(cmd, sizeof(cmd), "wpa_cli -iwlan0 set_network %d wep_key0 \\\"%s\\\"", id, psk);
		else
			snprintf(cmd, sizeof(cmd), "wpa_cli -iwlan0 set_network %d wep_key0 %s", id, psk);
		exec_command(cmd, str, 8);
		if (0 == strlen(str) || 0 != strncmp(str, "OK", 2))
			return false;

		memset(str, 0, sizeof(str));
		memset(cmd, 0, sizeof(cmd));
		snprintf(cmd, sizeof(cmd), "wpa_cli -iwlan0 set_network %d auth_alg SHARED", id);
		exec_command(cmd, str, 8);
		if (0 == strlen(str) || 0 != strncmp(str, "OK", 2))
			return false;
	} else if (strlen(psk) || encryp == WPA) {
		format_wifiinfo(1, wifi_psk);
		snprintf(cmd, sizeof(cmd), "wpa_cli -iwlan0 set_network %d psk \\\"%s\\\"", id, wifi_psk);
		exec_command(cmd, str, 8);
		if (0 == strlen(str) || 0 != strncmp(str, "OK", 2))
			return false;
	}

	return true;
}

static int set_hide_network(const int id)
{
	int ret;
	char str[8];
	char cmd[128];

	memset(str, 0, sizeof(str));
	memset(cmd, 0, sizeof(cmd));
	snprintf(cmd, sizeof(cmd), "wpa_cli -iwlan0 set_network %d scan_ssid %d", id, 1);
	exec_command(cmd, str, 8);

	if (0 == strlen(str) || 0 != strncmp(str, "OK", 2))
		return false;

	return true;
}

static int clear_bssid_network(const int id)
{
	int ret;
	char str[8];
	char cmd[128];

	memset(str, 0, sizeof(str));
	memset(cmd, 0, sizeof(cmd));
	snprintf(cmd, sizeof(cmd), "wpa_cli -iwlan0 bssid %d 00:00:00:00:00:00", id);
	exec_command(cmd, str, 8);

	if (0 == strlen(str) || 0 != strncmp(str, "OK", 2)) {
		pr_err("clear_bssid_network fail!\n");
		return false;
	}

	return true;
}

static int select_network(const int id)
{
	int ret;
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
		return false;

	return true;
}

static int enable_network(const int id)
{
	int ret;
	char str[8];
	char cmd[128];

	memset(str, 0, sizeof(str));
	memset(cmd, 0, sizeof(cmd));
	snprintf(cmd, sizeof(cmd), "wpa_cli -iwlan0 enable_network %d", id);
	exec_command(cmd, str, 8);

	if (0 == strlen(str) || 0 != strncmp(str, "OK", 2))
		return false;

	return true;
}

static void rkwifi_check_ip(void)
{
	int i;
	RK_WIFI_INFO_Connection_s wifiinfo;
	RK_WIFI_RUNNING_State_e state = -1;
	pr_info("rkwifi_check_ip.\n");

	for (int i = 0; i < 30; i++) {
		usleep(500 * 1000);
		memset(&wifiinfo, 0, sizeof(RK_WIFI_INFO_Connection_s));
		_RK_wifi_running_getConnectionInfo(&wifiinfo);
		if (strncmp(wifiinfo.wpa_state, "COMPLETED", 9) == 0) {
			if ((strlen(wifiinfo.ip_address) != 0) &&
				strncmp(wifiinfo.ip_address, "127.0.0.1", 9) != 0) {
				pr_info("wifi got ip.\n");
				state = RK_WIFI_State_DHCP_OK;
				wifi_state_send(state, &wifiinfo);
				break;
			}
		}

		if (i > 13) {
			exec_command_system("killall dhcpcd");
			usleep(100000);
			exec_command_system("dhcpcd wlan0 -AL -t 0 &");
		}
	}

	pr_info("rkwifi_check_ip exit.\n");
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
		_RK_wifi_running_getConnectionInfo(&wifiinfo);
		if (strncmp(wifiinfo.wpa_state, "COMPLETED", 9) == 0) {
			if ((strlen(wifiinfo.ip_address) != 0) &&
				strncmp(wifiinfo.ip_address, "127.0.0.1", 9) != 0) {
				isWifiConnected = true;
				pr_info("wifi is connected.\n");
				break;
			} else {
				if ((!(i%30)) || flag) {
					flag = 0;
					//exec_command_system("killall udhcpc");
					//usleep(300000);
					//exec_command_system("udhcpc -i wlan0 -t 10 &");
					exec_command_system("killall dhcpcd");
					usleep(300000);
					exec_command_system("dhcpcd wlan0 -AL -t 0 &");
				}
			}
		} else if (i == 20) {
			pr_info("check_wifi_isconnected is still not COMPLETED! break \n");
			break;
		}

		pr_info("Check wifi state with none state. try more %d/%d, \n", i + 1, WIFI_CONNECT_RETRY);

		if (wifi_onoff_flag == false) {
			pr_info("check_wifi_isconnected is wifi_onoff_flag false break.\n");
			break;
		}

		if (wifi_wrong_key == true) {
			pr_info("check_wifi_isconnected is wifi_wrong_key break.\n");
			break;
		}

		if (wpa_exit == true) {
			pr_info("check_wifi_isconnected is wpa_exit break.\n");
			break;
		}

		if (wifi_cancel == true) {
			pr_info("check_wifi_isconnected wifi_cancel be called so break.\n");
			//exec_command_system("wpa_cli flush");
			//exec_command_system("wpa_cli reconfigure");
			//exec_command_system("wpa_cli -iwlan0 disable_network all");
			//wifi_cancel = false;
			break;
		}
	}

	if (!isWifiConnected)
		pr_info("wifi is not connected.\n");

	return isWifiConnected;
}

static void format_wifiinfo(int flag, char *info)
{
	char temp[1024];
	int j = 0;

	if (flag == 0) {
		for (int i = 0; i < strlen(info); i++) {
			sprintf(temp+2*i, "%02x", info[i]);
		}
		temp[strlen(info)*2] = '\0';
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

static int set_priority_network(const int id, int priority)
{
	int ret;
	char str[8];
	char cmd[128];

	memset(str, 0, sizeof(str));
	memset(cmd, 0, sizeof(cmd));
	snprintf(cmd, sizeof(cmd), "wpa_cli -iwlan0 set_network %d priority %d", id, priority);
	exec_command(cmd, str, 8);

	if (0 == strlen(str) || 0 != strncmp(str, "OK", 2))
		return false;

	return true;
}

static void set_network_highest_priority(const int id)
{
	char str[128];
	int cnt;
	int network_id;
	char cmd[128];

	exec_command("wpa_cli -i wlan0 list_network | wc -l", str, 128);
	cnt = atoi(str) - 1;
	pr_info("wifi cnt: %d(%s)\n", cnt, str);

	for (int i = 0; i < cnt; i++) {
		snprintf(cmd, sizeof(cmd), "wpa_cli -i wlan0 list_network | awk '{print $1}' | sed -n %dp", i+2);
		exec_command(cmd, str, 128);
		network_id = atoi(str);

		if (network_id == id)
			set_priority_network(network_id, cnt);
		else
			set_priority_network(network_id, 1);
	}
}

static int disable_no_connected_ssid(void)
{

	RK_WIFI_INFO_Connection_s info;
	char str[128];
	int cnt;
	int network_id;
	char cmd[128];

	pr_info("### %s ###\n", __func__);
	if (_RK_wifi_running_getConnectionInfo(&info) == false)
		return false;

	memset(str, 0, 128);
	exec_command("wpa_cli -i wlan0 list_network | wc -l", str, 128);
	cnt = atoi(str) - 1;
	pr_info("save_configuration wifi cnt: %d(%s)\n", cnt, str);

	for (int i = 0; i < cnt; i++) {
		memset(cmd, 0, 128);
		snprintf(cmd, sizeof(cmd), "wpa_cli -i wlan0 list_network | awk '{print $1}' | sed -n %dp", i+2);
		exec_command(cmd, str, 128);
		network_id = atoi(str);

		if (network_id != info.id) {
			memset(cmd, 0, 128);
			snprintf(cmd, sizeof(cmd), "wpa_cli -i wlan0 disable_network %d", network_id);
			exec_command_system(cmd);
		}
	}

	return true;
}

static int save_configuration()
{
	//exec_command_system("wpa_cli -iwlan0 enable_network all");
	exec_command_system("wpa_cli -iwlan0 save_config");
	exec_command_system("sync");

	return true;
}

static void check_ping_test()
{
	char line[1024];
	int ping_retry = 1;
	int dns_retry = 5;

	memset(line, 0, sizeof(line));
	pr_info("check dns\n");
	while (dns_retry--) {
		exec_command("cat /etc/resolv.conf", line, 1024);
		if (strstr(line, "nameserver"))
			break;
		sleep(1);
	}
	pr_info("dns ok: %s\n", line);

#if 0
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
		//usleep(100000);
		pr_info("line: %s\n", line);
		//if (strstr(line, "PING www.baidu.com") && strstr(line, "bytes from")) {
		//if (strstr(line, "PING 8.8.8.8") && strstr(line, "bytes from")) {
		if (strstr(line, "bytes from")) {
			pr_info("ping ok\n");
			break;
		}
		usleep(200000);
	}
#endif
}

static void wifi_connectfail_process(int id)
{
	char str[128];
	char cmd1[128];
	char cmd2[128];

	pr_info("### %s ###\n", __func__);

	memset(str, 0, sizeof(str));
	memset(cmd1, 0, sizeof(cmd1));

	snprintf(cmd1, sizeof(cmd1), "wpa_cli -iwlan0 disable_network %d", id);
	snprintf(cmd2, sizeof(cmd2), "wpa_cli -iwlan0 remove_network %d", id);

	if (wifi_is_exist == true) {
		exec_command(cmd1, str, 128);
	} else {
		exec_command(cmd2, str, 128);
	}

	exec_command_system("wpa_cli -i wlan0 save_config");
	//exec_command_system("wpa_cli flush");
	//exec_command_system("wpa_cli reconfigure");
	//exec_command_system("wpa_cli reconnect");
}

static void* wifi_connect_state_check(void *arg)
{
	RK_WIFI_RUNNING_State_e state = 0;
	bool isconnected;
	RK_WIFI_INFO_Connection_s cndinfo;
	int id = *((int *)arg);

	prctl(PR_SET_NAME,"wifi_connect_state_check");
	pr_info("[%s]\n", __func__);

	isconnected = check_wifi_isconnected();
	pr_info("[RKWIFI]: %s: %d : %d\n", __func__, isconnected, wifi_wrong_key);

	if (isconnected == true) {
		char cmd[128];
		char str[8];
		RK_WIFI_SAVED_INFO_s *wsi = NULL;
		int ap_cnt = 0;

		_RK_wifi_getSavedInfo(&wsi, &ap_cnt);
		if (wsi != NULL)
			free(wsi);

		save_configuration();
		check_ping_test();
		state = RK_WIFI_State_DHCP_OK;
	} else {
		if (wifi_wrong_key == false)
			state = RK_WIFI_State_CONNECTFAILED;
		wifi_connectfail_process(id);
	}

	if (state == 0)
		goto end;

	wifi_state_send(state, NULL);

end:
	wifi_is_exist = false;
	wifi_connect_lock = false;
	is_wep = false;
	wifi_cancel = false;
	pr_info("[%s exit]\n", __func__);

	if (isconnected == false) {
		exec_command_system("wpa_cli -i wlan0 enable_network all");
		exec_command_system("wpa_cli -i wlan0 reconnect");
	}

	return NULL;
}

void save_connect_info(char* ssid, char *bssid)
{
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

int wpa_wifi_connect_wep(char* ssid, const char* psk)
{
	int ret = 0;

	if (!ssid) {
		pr_err("%s: invalid ssid\n", __func__);
		return false;
	}

	if (wifi_connect_lock == true) {
		pr_err("RK_wifi_connect to %s but prev connecting! now return\n", ssid);
		return false;
	}

	wifi_connect_lock = true;
	is_wep = true;
	ret = wpa_wifi_connect1(ssid, psk, WEP, 0);

	return ret;
}

int wpa_wifi_connect(char* ssid, const char* psk)
{
	int ret = 0;

	if (!ssid) {
		pr_err("%s: invalid ssid\n", __func__);
		return false;
	}

	if (wifi_connect_lock == true) {
		pr_err("RK_wifi_connect to %s but prev connecting! now return\n", ssid);
		return false;
	}

	wifi_connect_lock = true;
	ret = wpa_wifi_connect1(ssid, psk, WPA, 0);

	return ret;
}

int wpa_wifi_connect1(char* ssid, const char* psk, const RK_WIFI_CONNECTION_Encryp_e encryp, const int hide)
{
	int id, ret;
	char ori[strlen(ssid) + 1];

	if (!ssid) {
		pr_err("%s: invalid ssid\n", __func__);
		return false;
	}
	pr_info("[%s] ssid: %s, psk: %s\n", __func__, ssid, psk);

	save_connect_info(ssid, NULL);
	wifi_state_send(RK_WIFI_State_CONNECTING, NULL);

	exec_command_system("wpa_cli -iwlan0 disable_network all");
	wifi_wrong_key = false;
	usleep(60000);

	if (save_last_ap) {
		exec_command_system("cp /data/cfg/wpa_supplicant.conf /data/cfg/wpa_supplicant.conf.bak");
		exec_command_system("wpa_cli flush");
		sleep(100000);
		exec_command_system("wpa_cli save_config");
	}

	if (wpa_wifi_search_with_ssid(ssid) == -EINVAL) {
		id = add_network();
		if (id == -EINVAL) {
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
		if (true != ret) {
			pr_info("%s: set_network failed. ssid:\"%s\"\n", __func__, ssid);
			goto fail;
		}
	} else {
		pr_info("%s: ssid:\"%s\" ssid_len:%lu; psk:\"%s\", encryp: %d.\n", __func__, ssid, strlen(ssid), psk, encryp);
		get_encode_gbk_ori(m_gbk_head, ssid, ori);

		ret = set_network(id, ori, psk, encryp);
		if (true != ret) {
			pr_info("%s: set_network failed. ssid:\"%s\", psk:\"%s\"\n", __func__, ssid, psk);
			goto fail;
		}
	}
	pr_info("%s: ori:\"%s\" ori_len:%lu\n", __func__, ori, strlen(ori));

	set_network_highest_priority(id);

	ret = set_hide_network(id);
	if (true != ret) {
		pr_err("%s: set_hide_network id: %d failed!\n", __func__, id);
		goto fail;
	}

	ret = select_network(id);
	if (true != ret) {
		pr_err("%s: select_network id: %d failed!\n", __func__, id);
		goto fail;
	}

	ret = enable_network(id);
	if (true != ret) {
		pr_err("%s: enable_network id: %d failed!\n", __func__, id);
		goto fail;
	}

	pthread_t pth;
	pthread_create(&pth, NULL, wifi_connect_state_check, &connecting_id);
	pthread_detach(pth);

	return true;

fail:
	wifi_state_send(RK_WIFI_State_CONNECTFAILED, NULL);
	wifi_connect_lock = false;
	is_wep = false;
	return false;
}

int wpa_wifi_forget_with_ssid(char *ssid)
{
	int id, ret;
	char cmd[64];
	char str[8];

	pr_info("[%s]: ssid %s\n", __func__, ssid);

	if (!ssid) {
		pr_err("%s: ssid is null\n", __func__);
		return false;
	}

	id = wpa_wifi_search_with_ssid(ssid);
	if (id == -EINVAL) {
		pr_err("RK_wifi_forget_with_ssid [%s] not found!\n", ssid);
		return false;
	}

	snprintf(cmd, sizeof(cmd), "wpa_cli -iwlan0 remove_network %d", id);
	exec_command(cmd, str, 8);
	if (0 == strlen(str) || 0 != strncmp(str, "OK", 2))
		return false;

	exec_command_system("wpa_cli -iwlan0 save");

	return true;
}

int wpa_wifi_cancel(void)
{
	int timeout = 8;

	if (wifi_connect_lock == false) {
		pr_info("wifi dont connecting!");
		return false;
	}

	wifi_cancel = true;

	while (timeout--) {
		if (wifi_cancel == false)
			break;
		sleep(1);
	}

	if (wifi_cancel == false)
		return true;
	else
		return false;
}

int wpa_wifi_connect_with_ssid(char *ssid)
{
	int id, ret;

	if (wifi_connect_lock == true) {
		pr_err("RK_wifi_connect_with_ssid to %s but prev connecting! now return\n", ssid);
		return false;
	}

	wifi_connect_lock = true;
	wifi_wrong_key = false;
	wifi_is_exist = true;

	pr_err("%s: %s\n", __func__, ssid);

	if (!ssid) {
		pr_err("%s: ssid is null\n", __func__);
		goto fail;
	}

	id = wpa_wifi_search_with_ssid(ssid);
	if (id == -EINVAL) {
		pr_err("wpa_wifi_connect_with_bssid %d not found!\n", id);
		goto fail;
	}

	connecting_id = id;

	save_connect_info(NULL, ssid);
	wifi_state_send(RK_WIFI_State_CONNECTING, NULL);

	exec_command_system("wpa_cli -iwlan0 disable_network all");

	ret = select_network(id);
	if (true != ret) {
		pr_err("select_network id: %d failed!\n", id);
		goto fail;
	}

	set_network_highest_priority(id);

	ret = enable_network(id);
	if (true != ret) {
		pr_err("enable_network id: %d failed!\n", id);
		goto fail;
	}

	pthread_t pth;
	pthread_create(&pth, NULL, wifi_connect_state_check, &connecting_id);
	pthread_detach(pth);

	return true;

fail:
	wifi_state_send(RK_WIFI_State_CONNECTFAILED, NULL);
	wifi_is_exist = false;
	wifi_connect_lock = false;
	return false;
}

int wpa_wifi_disconnect_network(void)
{
	exec_command_system("wpa_cli -iwlan0 disconnect");
	return true;
}

int wpa_wifi_restart_network(void)
{
	return true;
}

int wpa_wifi_reset(void)
{
	if (get_ps_pid("wpa_supplicant")) {
		exec_command_system("wpa_cli -i wlan0 flush");
		exec_command_system("wpa_cli -i wlan0 save");
	} else {
		exec_command_system("rm /data/cfg/wpa_supplicant.conf");
		exec_command_system("cp /etc/wpa_supplicant.conf /data/cfg/wpa_supplicant.conf");
	}
	exec_command_system("sync");

	return true;
}

int wpa_wifi_recovery(void)
{
	if (save_last_ap) {
		exec_command_system("cp /data/cfg/wpa_supplicant.conf.bak /data/cfg/wpa_supplicant.conf");
	}

	exec_command_system("wpa_cli -i wlan0 flush");
	exec_command_system("wpa_cli -i wlan0 reconfigure");
	exec_command_system("wpa_cli -i wlan0 reconnect");

	return true;
}

int wpa_wifi_get_mac(char *wifi_mac)
{
	int sock_mac;
	struct ifreq ifr_mac;
	char mac_addr[18] = {0};

	if (!wifi_mac) {
		pr_err("%s: wifi_mac is null\n", __func__);
		return false;
	}

	sock_mac = socket(AF_INET, SOCK_STREAM, 0);

	if (sock_mac == -1) {
		pr_info("create mac socket failed.");
		return false;
	}

	memset(&ifr_mac, 0, sizeof(ifr_mac));
	strncpy(ifr_mac.ifr_name, "wlan0", sizeof(ifr_mac.ifr_name) - 1);

	if ((ioctl(sock_mac, SIOCGIFHWADDR, &ifr_mac)) < 0) {
		pr_info("Mac socket ioctl failed.");
		return false;
	}

	sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
			(unsigned char)ifr_mac.ifr_hwaddr.sa_data[0],
			(unsigned char)ifr_mac.ifr_hwaddr.sa_data[1],
			(unsigned char)ifr_mac.ifr_hwaddr.sa_data[2],
			(unsigned char)ifr_mac.ifr_hwaddr.sa_data[3],
			(unsigned char)ifr_mac.ifr_hwaddr.sa_data[4],
			(unsigned char)ifr_mac.ifr_hwaddr.sa_data[5]);

	close(sock_mac);
	strncpy(wifi_mac, mac_addr, 18);
	pr_info("the wifi mac : %s\r\n", wifi_mac);

	return true;
}

int wpa_wifi_ping(char *address)
{
	//return wpa_ping(address);
	return true;
}

#define EVENT_BUF_SIZE 1024
#define PROPERTY_VALUE_MAX 32
#define PROPERTY_KEY_MAX 32
#include <poll.h>
//#include <wpa_ctrl.h>
#include "wpa_ctrl.h"

static const char WPA_EVENT_IGNORE[]	= "CTRL-EVENT-IGNORE ";
static const char IFNAME[]				= "IFNAME=";
static const char IFACE_DIR[]			= "/var/run/wpa_supplicant";
#define IFNAMELEN						(sizeof(IFNAME) - 1)
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

static void wifi_close_sockets()
{
	pr_info("########### %s ###############\n", __func__);
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

static int str_starts_with(char * str, char * search_str)
{
	if ((str == NULL) || (search_str == NULL))
		return false;
	return (strstr(str, search_str) == str);
}

static void get_wifi_info_by_event(char *event, RK_WIFI_RUNNING_State_e state, RK_WIFI_INFO_Connection_s *info)
{
	int len = 0;
	char buf[10];
	char *start_tag = NULL, *end_tag = NULL, *id_tag = NULL, *reason_tag = NULL;

	if (event == NULL)
		return;

	memset(info, 0, sizeof(RK_WIFI_INFO_Connection_s));
	_RK_wifi_running_getConnectionInfo(info);

	switch (state) {
	case RK_WIFI_State_DISCONNECTED:
		start_tag = strstr(event, "bssid=");
		if (start_tag)
			strncpy(info->bssid, start_tag + strlen("bssid="), 17);

		//strncpy(info->ssid, connect_info.ssid, SSID_BUF_LEN);
		reason_tag =  strstr(event, "reason=");
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
			char value[256] = {0};
			char sname[256];
			char sname1[256];
			char utf8[256];
			memset(sname, 0, sizeof(sname));
			memset(sname1, 0, sizeof(sname));
			memset(utf8, 0, sizeof(utf8));
			strncpy(value, start_tag + strlen("ssid=\""), len);
			pr_info("%s: check len value: %d\n", __func__, strlen(value));
			spec_char_convers(value, sname);
			remove_escape_character(sname, sname1);
			get_encode_gbk_utf8(m_gbk_head, sname1, utf8);
			pr_info("convers str: %s, sname: %s, ori: %s\n", value, sname1, utf8);
			strncpy(info->ssid,  utf8, strlen(utf8));
		}

		id_tag =  strstr(event, "id=");
		if (id_tag) {
			len = strlen(id_tag) - strlen("id=") - strlen(start_tag) - 1;
			memset(buf, 0, sizeof(buf));
			strncpy(buf, id_tag + strlen("id="), len);
			info->id = atoi(buf);

			//memset(cmd, 0, 128);
			//snprintf(cmd, sizeof(cmd), "wpa_cli -i wlan0 remove_network %d", info->id);
			//exec_command_system(cmd);
			//exec_command_system("wpa_cli -i wlan0 save_config");
		}

		strncpy(info->bssid, connect_info.bssid, BSSID_BUF_LEN);
		break;
	}
}

static void get_valid_connect_info(RK_WIFI_INFO_Connection_s *info)
{
	int count = 10;

	while(count--) {
		_RK_wifi_running_getConnectionInfo(info);
		if(!info->id && !strlen(info->ssid)
				&& !strlen(info->bssid) && !info->freq
				&& !strlen(info->mode) && !strlen(info->ip_address)
				&& !strlen(info->mac_address) && !strlen(info->wpa_state)) {
			pr_info("wait to get valid connect info\n");
			usleep(100000);
		} else {
			break;
		}
	}
}

static int dispatch_event(char* event)
{
	RK_WIFI_INFO_Connection_s info;

	if (strstr(event, "CTRL-EVENT-BSS") || strstr(event, "CTRL-EVENT-TERMINATING"))
		return false;

	pr_info("%s: %s\n", __func__, event);

	if (str_starts_with(event, (char *)WPA_EVENT_DISCONNECTED)) {
		pr_info("%s: wifi is disconnect\n", __FUNCTION__);
		exec_command_system("ip addr flush dev wlan0");
		get_wifi_info_by_event(event, RK_WIFI_State_DISCONNECTED, &info);
		wifi_state_send(RK_WIFI_State_DISCONNECTED, &info);
		if (wifi_connect_lock == false) {
			exec_command_system("wpa_cli -i wlan0 enable_network all");
			exec_command_system("wpa_cli -i wlan0 reconnect");
		}
	} else if (str_starts_with(event, (char *)WPA_EVENT_CONNECTED)) {
		pr_info("%s: wifi is connected\n", __func__);
		get_valid_connect_info(&info);
		wifi_state_send(RK_WIFI_State_CONNECTED, &info);
		disable_no_connected_ssid();
		if (wifi_connect_lock == false) {
			rkwifi_check_ip();
		}
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
		return false;
	}

	if (is_wep && strstr(event, "CTRL-EVENT-ASSOC-REJECT") && strstr(event, "status_code=13")) {
		char cmd[128];
		memset(cmd, 0, 128);
		snprintf(cmd, sizeof(cmd), "wpa_cli -i wlan0 set_network %d auth_alg OPEN", wep_id);
		exec_command_system(cmd);
		memset(cmd, 0, 128);
		snprintf(cmd, sizeof(cmd), "wpa_cli -i wlan0 select_network %d", wep_id);
		exec_command_system(cmd);
	}

	return true;
}

static int check_wpa_supplicant_state() {
	int count = 5;
	int wpa_supplicant_pid = 0;
	wpa_supplicant_pid = get_ps_pid(WPA_SUPPLICANT);
	//pr_info("%s: wpa_supplicant_pid = %d\n",__FUNCTION__,wpa_supplicant_pid);
	if(wpa_supplicant_pid > 0) {
		return true;
	}

	pr_info("%s: wpa abort exit!!!\n",__FUNCTION__);
	return false;
}

static int wifi_ctrl_recv(char *reply, size_t *reply_len)
{
	int res;
	int ctrlfd = wpa_ctrl_get_fd(monitor_conn);
	struct pollfd rfds[2];

	memset(rfds, 0, 2 * sizeof(struct pollfd));
	rfds[0].fd = ctrlfd;
	rfds[0].events |= POLLIN;
	rfds[1].fd = exit_sockets[1];
	rfds[1].events |= (POLLIN|POLLERR);
	do {
		res = poll(rfds, 2, 1000);
		if (res < 0) {
			pr_info("Error poll = %d\n", res);
			return res;
		} else if (res == 0) {
			/* timed out, check if supplicant is activeor not .. */
			res = check_wpa_supplicant_state();
			if (res == false)
				return false;
		}

		if (wifi_onoff_flag == false) {
			pr_info("------  exit wifi_ctrl_recv ------------\n");
			wpa_exit = true;
			return false;
		}
	} while (res == 0);

	if (rfds[0].revents & POLLIN) {
		return wpa_ctrl_recv(monitor_conn, reply, reply_len);
	}

	if (rfds[1].revents) {
		pr_info("------    0x%x ignore------------\n", rfds[1].revents);
		//wpa_exit = true;
		//return -3;
	}

	return false;
}

static int wifi_wait_on_socket(char *buf, size_t buflen)
{
	size_t nread = buflen - 1;
	int result;
	char *match, *match2;

	if (monitor_conn == NULL) {
		return snprintf(buf, buflen, "IFNAME=%s %s - connection closed",
			primary_iface, WPA_EVENT_TERMINATING);
	}

	result = wifi_ctrl_recv(buf, &nread);

	/* Terminate reception on exit socket */
	if (result <= 0) {
		return snprintf(buf, buflen, "IFNAME=%s %s - connection closed",
			primary_iface, WPA_EVENT_TERMINATING);

		//pr_info("wifi_ctrl_recv failed: %s\n", strerror(errno));
		//return snprintf(buf, buflen, "IFNAME=%s %s - recv error",
		//	primary_iface, WPA_EVENT_TERMINATING);
	}

	buf[nread] = '\0';

	/* Check for EOF on the socket */
	if (result == 0 && nread == 0) {
		/* Fabricate an event to pass up */
		pr_info("Received EOF on supplicant socket\n");
		return snprintf(buf, buflen, "IFNAME=%s %s - signal 0 received",
			primary_iface, WPA_EVENT_TERMINATING);
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
			pr_info("supplicant generated event without interface and without message level - %s\n", buf);
	}

	return nread;
}

static int wifi_connect_on_socket_path(const char *path)
{
	char supp_status[PROPERTY_VALUE_MAX] = {'\0'};

	if(!check_wpa_supplicant_state()) {
		pr_info("%s: wpa_supplicant is not ready\n",__FUNCTION__);
		return false;
	}

	ctrl_conn = wpa_ctrl_open(path);
	if (ctrl_conn == NULL) {
		pr_info("Unable to open connection to supplicant on \"%s\": %s\n",
		path, strerror(errno));
		return false;
	}
	monitor_conn = wpa_ctrl_open(path);
	if (monitor_conn == NULL) {
		wpa_ctrl_close(ctrl_conn);
		ctrl_conn = NULL;
		return false;
	}
	if (wpa_ctrl_attach(monitor_conn) != 0) {
		wpa_ctrl_close(monitor_conn);
		wpa_ctrl_close(ctrl_conn);
		ctrl_conn = monitor_conn = NULL;
		return false;
	}

	if (socketpair(AF_UNIX, SOCK_STREAM, 0, exit_sockets) == -1) {
		wpa_ctrl_close(monitor_conn);
		wpa_ctrl_close(ctrl_conn);
		ctrl_conn = monitor_conn = NULL;
		return false;
	}
	return true;
}

/* Establishes the control and monitor socket connections on the interface */
static int wifi_connect_to_supplicant()
{
	static char path[1024];
	int count = 10;

	pr_info("%s \n", __FUNCTION__);
	while(count-- > 0) {
		if (access(IFACE_DIR, F_OK) == 0)
			break;
		sleep(1);
	}

	snprintf(path, sizeof(path), "%s/%s", IFACE_DIR, primary_iface);

	return wifi_connect_on_socket_path(path);
}

static void *RK_wifi_start_monitor(void *arg)
{
	char eventStr[EVENT_BUF_SIZE];
	int ret;
	pr_info("------ enter RK_wifi_start_monitor ------$\n");

	prctl(PR_SET_NAME,"RK_wifi_start_monitor");
	wpa_exit = false;

	if (wifi_connect_to_supplicant() == false) {
		pr_info("%s, connect to supplicant fail.\n", __FUNCTION__);
		return;
	}

	for (;;) {
		if (wpa_exit == true) {
			pr_info("------ wpa_exit for() ------\n");
			break;
		}

		memset(eventStr, 0, EVENT_BUF_SIZE);
		if (!wifi_wait_on_socket(eventStr, EVENT_BUF_SIZE))
			continue;

		if (dispatch_event(eventStr) == false) {
			pr_info("disconnecting from the supplicant, no more events\n");
			break;
		}
	}
	wifi_close_sockets();
	pr_info("------ exit RK_wifi_start_monitor ------$\n");
}

struct Rk_wifi_driver_ops wpa_driver_ops = {
	.name = "wpa_supplicant standard",
	.wifi_enable = wpa_wifi_enable,
	.wifi_disconnect_network = wpa_wifi_disconnect_network,
	.wifi_connect = wpa_wifi_connect,
	.wifi_running_getConnectionInfo = wpa_wifi_running_getConnectionInfo,
	.wifi_cancel = wpa_wifi_cancel,
	.wifi_connect_wep = wpa_wifi_connect_wep,
	.wifi_forget_with_ssid = wpa_wifi_forget_with_ssid,
	.wifi_recovery = wpa_wifi_recovery,
	.wifi_getSavedInfo = wpa_wifi_getSavedInfo,
	.wifi_reset = wpa_wifi_reset,
	.wifi_scan = wpa_wifi_scan,
	.wifi_scan_r = wpa_wifi_scan_r,
	.wifi_connect_with_ssid = wpa_wifi_connect_with_ssid,
	.wifi_scan_for_softap = wpa_wifi_scan_softap,
};
