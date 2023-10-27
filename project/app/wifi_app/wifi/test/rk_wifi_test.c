#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/prctl.h>
#include <sys/time.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "Rk_wifi.h"
#include "Rk_softap.h"

static volatile int wifi_onff_cnt = 0;
static pthread_t rkwifi_init_thread = 0;
static RK_WIFI_RUNNING_State_e g_wifi_state = RK_WIFI_State_OFF;

static void printf_wifi_link_info(RK_WIFI_INFO_Connection_s *info)
{
	if (!info)
		return;

	printf("[===================================]\n");
	printf("	id: %d\n", info->id);
	printf("	bssid: %s\n", info->bssid);
	printf("	ssid: %s\n", info->ssid);
	printf("	freq: %d\n", info->freq);
	printf("	channel: %s\n", info->channel);
	printf("	mode: %s\n", info->mode);
	printf("	wpa_state: %s\n", info->wpa_state);
	printf("	ip_address: %s\n", info->ip_address);
	printf("	mac_address: %s\n", info->mac_address);
	printf("	gw_address: %s\n", info->gw);
	printf("	dns_address: %s\n", info->dns);
	printf("[===================================]\n");
}

/* SOFTAP CONFIG TEST */
static int rk_wifi_softap_state_callback(RK_SOFTAP_STATE state, const char* data)
{
	switch (state) {
	case RK_SOFTAP_STATE_CONNECTTING:
		printf("RK_SOFTAP_STATE_CONNECTTING\n");
		break;
	case RK_SOFTAP_STATE_DISCONNECT:
		printf("RK_SOFTAP_STATE_DISCONNECT\n");
		break;
	case RK_SOFTAP_STATE_FAIL:
		printf("RK_SOFTAP_STATE_FAIL\n");
		break;
	case RK_SOFTAP_STATE_SUCCESS:
		printf("RK_SOFTAP_STATE_SUCCESS\n");
		break;
	default:
		break;
	}

	return true;
}

void rk_wifi_softap_start(int argc, char *argv[])
{
	printf("%s enter\n", __func__);
	RK_softap_register_callback(rk_wifi_softap_state_callback);
	if (0 != RK_softap_start("Rockchip-SoftAp", RK_SOFTAP_TCP_SERVER)) {
		return;
	}
}

void rk_wifi_softap_stop(int argc, char *argv[])
{
	RK_softap_stop();
}
/* SOFTAP TEST END */

static int rk_wifi_state_callback(RK_WIFI_RUNNING_State_e state, RK_WIFI_INFO_Connection_s *info)
{
	printf("%s state: %d\n", __func__, state);
	printf_wifi_link_info(info);

	g_wifi_state = state;
	if (state == RK_WIFI_State_CONNECTED) {
		printf("RK_WIFI_State_CONNECTED\n");
	} else if (state == RK_WIFI_State_CONNECTFAILED) {
		printf("RK_WIFI_State_CONNECTFAILED\n");
	} else if (state == RK_WIFI_State_CONNECTFAILED_WRONG_KEY) {
		printf("RK_WIFI_State_CONNECTFAILED_WRONG_KEY\n");
	} else if (state == RK_WIFI_State_OPEN) {
		printf("RK_WIFI_State_OPEN\n");
	} else if (state == RK_WIFI_State_OFF) {
		printf("RK_WIFI_State_OFF\n");
	} else if (state == RK_WIFI_State_DISCONNECTED) {
		printf("RK_WIFI_State_DISCONNECTED\n");
	} else if (state == RK_WIFI_State_SCAN_RESULTS) {
		char *scan_r;
		printf("RK_WIFI_State_SCAN_RESULTS\n");
		scan_r = RK_wifi_scan_r();
		free(scan_r);
	} else if (state == RK_WIFI_State_LP_EVENT_TYPE) {
		if (info->lp_event_type == RK_WIFI_LP_BAT_LEVEL) {
			printf("RK_WIFI_MCU_BAT_LEVEL: %d\n", info->lp_data[0]);
		} else if (info->lp_event_type == RK_WIFI_LP_PIR_DETECT) {
			printf("RK_WIFI_MCU_PIR_DETECT: %d\n", info->lp_data[0]);
		} else if (info->lp_event_type == RK_WIFI_LP_LONG_PRESS) {
			printf("RK_WIFI_MCU_LONG_PRESS_KEY\n");
		} else if (info->lp_event_type == RK_WIFI_LP_SHORT_PRESS) {
			printf("RK_WIFI_MCU_SHORT_PRESS_KEY\n");
		} else if (info->lp_event_type == RK_WIFI_LP_KEEPALIVE_OK) {
			printf("RK_WIFI_MCU_KEEPALIVE_OK\n");
		} else if (info->lp_event_type == RK_WIFI_LP_KEEPALIVE_FAILED) {
			printf("RK_WIFI_MCU_KEEPALIVE_FAILED\n");
		} else if (info->lp_event_type == RK_WIFI_LP_WAKEUP_REASON) {
			printf("RK_WIFI_LP_WAKEUP_REASON: %d\n", info->lp_data[0]);
		}
	}

	return true;
}

int rk_wifi_open(void)
{
	pthread_t thread;
	int timeout = 200; //200 * 10ms == 2s
	RK_WIFI_INFO_Connection_s Info;

	printf("%s\n", __func__);

	RK_wifi_register_callback(rk_wifi_state_callback);
	if (!RK_wifi_enable(1))
		goto fail;

	while ((timeout--) && (g_wifi_state == RK_WIFI_State_OFF)) {
		usleep(10 * 1000);
		printf("%s: TURNING ON ...\n", __func__);
	}

	if (g_wifi_state == RK_WIFI_State_OFF)
		goto fail;

	if (RK_wifi_running_getConnectionInfo(&Info))
		printf_wifi_link_info(&Info);

	return true;

fail:
	printf("%s: RK_wifi_enable fail\n", __func__);
	return false;
}

void rk_wifi_close(int argc, char *argv[])
{
	if (!RK_wifi_enable(0))
		printf("RK_wifi_enable 0 fail!\n");
}

//9 input fish1:rk12345678
void rk_wifi_connect(int argc, char *argv[])
{
	printf("%s: ssid: %s, psk: %s\n", __func__, argv[0], argv[1]);
	if (!RK_wifi_connect(argv[0], argv[1]))
		printf("rk_wifi_connect fail!\n");

	return;
}

void rk_wifi_scan(int argc, char *argv[])
{
	if (!RK_wifi_scan())
		printf("RK_wifi_scan fail!\n");
}

void rk_wifi_getSavedInfo(int argc, char *argv[])
{
	RK_WIFI_SAVED_INFO_s *wsi;
	int ap_cnt = 0;

	RK_wifi_getSavedInfo(&wsi, &ap_cnt);

	for (int i = 0; i < ap_cnt; i++) {
		printf("id: %d, name: %s, bssid: %s, state: %s\n",
				wsi[i].id, wsi[i].ssid,
				wsi[i].bssid, wsi[i].state);
	}

	if (wsi != NULL)
		free(wsi);
}

void rk_wifi_getConnectionInfo(int argc, char *argv[])
{
	RK_WIFI_INFO_Connection_s info;

	if (RK_wifi_running_getConnectionInfo(&info))
		printf_wifi_link_info(&info);
}

void rk_wifi_connect_with_ssid(int argc, char *argv[])
{
	if (!RK_wifi_connect_with_ssid(argv[0]))
		printf("RK_wifi_connect_with_ssid fail!\n");
}

void rk_wifi_stop_keepalive(int argc, char *argv[])
{
	if (!RK_wifi_stop_keepalive(866))
		printf("RK_wifi_stop_keepalive fail!\n");
}

void rk_wifi_reset(int argc, char *argv[])
{
	if (!RK_wifi_reset())
		printf("RK_wifi_reset fail!\n");
}

void rk_wifi_cancel(int argc, char *argv[])
{
	if (!RK_wifi_cancel())
		printf("RK_wifi_cancel fail!\n");
}

void rk_wifi_forget_with_ssid(int argc, char *argv[])
{
	if (!RK_wifi_forget_with_ssid(argv[0])) {
		printf("rk_wifi_forget_with_ssid fail!\n");
	}
}

void rk_wifi_disconnect(int argc, char *argv[])
{
	RK_wifi_disconnect_network();
}

void rk_wifi_set_pir(int argc, char *argv[])
{
	int enable = atoi(argv[0]);

	printf("%s: %s\n", __func__, enable ? "enable" : "disable");
	if (RK_wifi_set_pir(enable) < 0)
		printf("rk_wifi_set_pir fail!\n");

	return;
}

void rk_wifi_set_keepalive(int argc, char *argv[])
{
	RK_WIFI_LOW_POWER_KEEPALIVE_s kp;
	printf("argv: %s:%s:%s\n", argv[0], argv[1], argv[2]);

	memset(&kp, 0, sizeof(RK_WIFI_LOW_POWER_KEEPALIVE_s));
	strncpy(kp.domain, argv[0], strlen(argv[0]));
	strncpy(kp.port, argv[1], strlen(argv[1]));
	strncpy(kp.period, argv[2], strlen(argv[2]));

	printf("domain:%s, port:%s, period:%s\n",
			 kp.domain, kp.port, kp.period);
	if (RK_wifi_start_keepalive(&kp) < 0)
		printf("RK_wifi_start_keepalive fail!\n");

	return;
}

/* WIFI ON/OFF LOOP TEST */
void *wifi_test_onff_thread(void *arg)
{
	while (1) {
		//open
		RK_wifi_register_callback(rk_wifi_state_callback);
		if (!RK_wifi_enable(1))
			printf("RK_wifi_enable 1 fail!\n");

		while (g_wifi_state == RK_WIFI_State_OPEN) {
			sleep(1);
			printf("%s: RKWIFI TURNING ON ...\n", __func__);
		}

		//scan
		RK_wifi_scan();
		sleep(1);

		//close
		printf("%s: RKWIFI DEINIT\n", __func__);
		if (!RK_wifi_enable(0))
			printf("RK_wifi_enable 0 fail!\n");

		while (g_wifi_state == RK_WIFI_State_OFF) {
			sleep(1);
			printf("%s: RKWIFI TURNING OFF ...\n", __func__);
		}
		printf("%s: RKWIFI TURN ONOFF CNT: [%d] \n", __func__, wifi_onff_cnt++);
	}
}

void rk_wifi_openoff_test(int argc, char *argv[])
{
	printf("%s: ", __func__);

	if (rkwifi_init_thread) {
		printf("rkwifi_init_thread already exist\n");
		return;
	}

	if (pthread_create(&rkwifi_init_thread, NULL, wifi_test_onff_thread, NULL)) {
		printf("Createrkwifi_init_thread failed\n");
		return;
	}
}

void rk_wifi_ota(int argc,char *argv[])
{
	if (argc < 1) {
		printf("no path,pls enter correct path \n");
		return;
	}
	Rk_wifi_ota(argv[0]);
}

#define CLI_SOCKET_PATH "/tmp/rkserver_socket"
#define RK_CMD_LINE_LEN 1600
#define MAX_ARGS 10

typedef struct {
	const char *cmd;
	const char *desc;
	int (*exec)(int argc, char *argv[]);
} command_t;

static command_t socket_cmd_table[] = {
	{"close", "rkwifi_server close", rk_wifi_close},
	{"scan", "rkwifi_server scan", rk_wifi_scan},
	{"connect", "rkwifi_server connect ssid password", rk_wifi_connect},
	{"disconnect", "rkwifi_server disconnect", rk_wifi_disconnect},
	{"getinfo", "rkwifi_server getinfo", rk_wifi_getConnectionInfo},
	{"deepsleep", "rkwifi_server deepsleep [ONLY LOW POWER]", RK_wifi_enter_sleep},
	{"reconnect", "rkwifi_server reconnect ssid", rk_wifi_connect_with_ssid},
	{"forget", "rkwifi_server forget ssid", rk_wifi_forget_with_ssid},
	{"getSavedInfo", "rkwifi_server getSavedInfo", rk_wifi_getSavedInfo},
	{"cancel", "rkwifi_server cancel", rk_wifi_cancel},
	{"reset", "rkwifi_server reset", rk_wifi_reset},
	{"onoff_test", "rkwifi_server onoff_test", rk_wifi_openoff_test},
	{"ap_cfg", "rkwifi_server ap_cfg", rk_wifi_softap_start},
	{"ap_cfg_clr", "rkwifi_server ap_cfg_clr", rk_wifi_softap_stop},
	{"setpir", "rkwifi_server setpir <0/1> = disable/enable PIR", rk_wifi_set_pir},
	{"ota","rkwifi_server ota", rk_wifi_ota},
	{"start_kp", "rkwifi_server start_kp <ip:port:expire>", rk_wifi_set_keepalive},
	{"stop_kp", "rkwifi_server stop_kp", rk_wifi_stop_keepalive},
};

static int rk_tokenize_cmd(char *cmd, char *argv[], int is_at_cmd)
{
	char *pos;
	int argc = 0;
	printf("rk_tokenize_cmd %s\n", cmd);

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
		while (*pos != '\0' && *pos != ' ')
			pos++;
		if (*pos == ' ')
			*pos++ = '\0';
	}

	return argc;
}

static int rk_cmd_parse(char *arg)
{
	int i = 0;
	int cnt = sizeof(socket_cmd_table) / sizeof(socket_cmd_table[0]);
	char *argv[MAX_ARGS];
	int argc;
	int ret = 0;
	printf("rk_cmd_parse %s\n", arg);

	argc = rk_tokenize_cmd(arg, argv, 0);
	printf("[%s %d]: %s %s\n", arg, argc, argv[0], argv[1]);
	if (argc) {
		for (i = 0; i < cnt; i++) {
			if (!strcmp(socket_cmd_table[i].cmd, argv[0])) {
				ret = socket_cmd_table[i].exec(argc - 1, &argv[1]);
				if (ret == false) {
					printf("FAIL\n");
				} else {
					printf("OK\n");
				}
				break;
			}
		}
	}

	return true;
}

void *rkwifi_server_command_func(void *arg)
{
	struct sockaddr_un ser_un;
	int socket_fd = 0;
	int connect_fd = 0;
	int ret = 0;
	char recall[] = "OK";
	char cmd_line[RK_CMD_LINE_LEN];

	socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (socket_fd <= 0) {
		printf("open socket err\n");
		return NULL;
	}

	unlink(CLI_SOCKET_PATH);

	memset(&ser_un, 0, sizeof(ser_un));
	ser_un.sun_family = AF_UNIX;
	strcpy(ser_un.sun_path, CLI_SOCKET_PATH);
	ret = bind(socket_fd, (struct sockaddr *)&ser_un, sizeof(struct sockaddr_un));
	if (ret < 0) {
		printf("bind err %s %s\n", CLI_SOCKET_PATH, strerror(errno));
		return NULL;
	}

	ret = listen(socket_fd, 5);
	if (ret < 0) {
		printf("listen err\n");
		return NULL;
	}

	while (1) {
		printf("accept ...\n");
		connect_fd = accept(socket_fd, NULL, NULL);
			if (connect_fd < 0) {
			continue;
		}

		memset(cmd_line, 0, sizeof(cmd_line));
		ret = read(connect_fd, cmd_line, sizeof(cmd_line));
		if (ret <= 0) {
			printf("cmd_line error!!!\n");
		}

		write(connect_fd, recall, strlen(recall) + 1);
		close(connect_fd);
		printf("cmd_line: %s [%d]\n", cmd_line, strlen(cmd_line));

		if (!strncmp(cmd_line, "quit", strlen("quit"))) {
			break;
		}

		rk_cmd_parse(cmd_line);
	}

	close(socket_fd);
	return NULL;
}

static void show_help(void) {
	unsigned int i;
	printf("[Usage]:\n");
	for (i = 0; i < sizeof(socket_cmd_table)/sizeof(command_t); i++)
		printf("\t\"%s\"\n", socket_cmd_table[i].desc);
}

extern int rkwifi_cli(int argc, char *argv[]);
int main(int argc, char *argv[])
{
	int i, item_cnt;
	pthread_t sock_tid;

	if (argc <= 1) {
		show_help();
		return true;
	}

	if (!strncmp("-h", argv[1], sizeof("-h"))) {
		show_help();
		return true;
	}

	if (!strncmp("start", argv[1], sizeof("start"))) {
		printf("rkwifi_server: start");
		if (!rk_wifi_open())
			return false;

		pthread_create(&sock_tid, NULL, rkwifi_server_command_func, NULL);

		while (1)
			sleep(1);
	}

	if (argc >= 2)
		return rkwifi_cli(argc, argv);


	return true;
}
