#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include <Rk_softap.h>
#include <Rk_wifi.h>

#define DBG 1

#if DBG
#define DEBUG_INFO(M, ...) printf("Hostapd %d: " M, __LINE__, ##__VA_ARGS__)
#else
#define DEBUG_INFO(M, ...) do {} while (0)
#endif
#define DEBUG_ERR(M, ...) printf("Hostapd %d: " M, __LINE__, ##__VA_ARGS__)

static void exec_command(char cmdline[], char recv_buff[], int len)
{
	printf("[BT_DEBUG] execute: %s\n", cmdline);

	FILE *stream = NULL;
	char *tmp_buff = recv_buff;

	memset(recv_buff, 0, len);

	if ((stream = popen(cmdline, "r")) != NULL) {
		while (fgets(tmp_buff, len, stream)) {
			//pr_info("tmp_buf[%d]: %s\n", strlen(tmp_buff), tmp_buff);
			tmp_buff += strlen(tmp_buff);
			len -= strlen(tmp_buff);
			if (len <= 1)
				break;
		}

		printf("[BT_DEBUG] execute_r: %s \n", recv_buff);
		pclose(stream);
	} else
		printf("[popen] error: %s\n", cmdline);
}

static const int console_run(const char *cmdline)
{
	int ret;

	DEBUG_INFO("cmdline = %s\n", cmdline);

	ret = system(cmdline);
	if (ret < 0) {
		DEBUG_ERR("Running cmdline failed: %s\n", cmdline);
		return 0;
	}

	return 1;
}

static int create_hostapd_file(char *ap, char *ssid, char *psk)
{
	FILE* fp;
	char cmdline[256] = {0};
	static char HOSTAPD_CONF_DIR[] = "/data/hostapd.conf";

	fp = fopen(HOSTAPD_CONF_DIR, "wt+");
	if (NULL == fp)
		return -1;

	sprintf(cmdline, "interface=%s\n", ap);
	fputs(cmdline, fp);
	fputs("ctrl_interface=/var/run/hostapd\n", fp);
	fputs("driver=nl80211\n", fp);
	fputs("ssid=", fp);
	fputs(ssid, fp);
	fputs("\n", fp);
	fputs("channel=6\n", fp);
	fputs("hw_mode=g\n", fp);
	fputs("ieee80211n=1\n", fp);
	fputs("ignore_broadcast_ssid=0\n", fp);

	if (psk != NULL && 0 != strcmp(psk, "")) {
		fputs("auth_algs=1\n", fp);
		fputs("wpa=3\n", fp);
		fputs("wpa_passphrase=", fp);
		fputs(psk, fp);
		fputs("\n", fp);
		fputs("wpa_key_mgmt=WPA-PSK\n", fp);
		fputs("wpa_pairwise=TKIP\n", fp);
		fputs("rsn_pairwise=CCMP", fp);
	}

	fclose(fp);
	return 0;
}

static int creat_dnsmasq_file()
{
	FILE* fp;
	static char DNSMASQ_CONF_DIR[] = "/data/dnsmasq.conf";
	static char SOFTAP_INTERFACE_STATIC_IP[] = "10.201.126.1";

	system("rm /data/dnsmasq.conf -rf");

	fp = fopen(DNSMASQ_CONF_DIR, "wt+");
	if (NULL == fp)
		return 0;

	fputs("user=root\n", fp);
	fputs("listen-address=", fp);
	fputs(SOFTAP_INTERFACE_STATIC_IP, fp);
	fputs("\n", fp);
	fputs("dhcp-range=10.201.126.50,10.201.126.150\n", fp);
	fputs("server=/google/8.8.8.8\n", fp);
	fclose(fp);
	return 1;
}

static int start_hostapd(char *ap, char *ssid, char *psk, char *ip)
{
	char cmdline[256] = {0};
	static char DNSMASQ_CONF_DIR[] = "/data/dnsmasq.conf";
	static char HOSTAPD_CONF_DIR[] = "/data/hostapd.conf";
	create_hostapd_file(ap, ssid, psk);

	sprintf(cmdline, "ifconfig %s up", ap);
	console_run(cmdline);
	sleep(1);
	sprintf(cmdline, "ifconfig %s 10.201.126.1 netmask 255.255.255.0", ap);
	console_run(cmdline);
	//sprintf(cmdline, "route add default gw %s %s", ip, ap);
	//console_run(cmdline);

	creat_dnsmasq_file();
	memset(cmdline, 0, sizeof(cmdline));
	sprintf(cmdline, "dnsmasq -C %s --interface=%s", DNSMASQ_CONF_DIR, ap);
	console_run(cmdline);

	memset(cmdline, 0, sizeof(cmdline));
	sprintf(cmdline, "hostapd %s &", HOSTAPD_CONF_DIR);
	console_run(cmdline);

	int time = 100;
	while (time-- > 0 && access("/var/run/hostapd", F_OK)) {
		usleep(100 * 1000);
	}
	return 0;
}

int wifi_start_hostapd(char *ssid, char *psk, char *ip)
{
	char wifi_type[128];
	char ap[64];

	console_run("killall dnsmasq");
	console_run("killall hostapd");
	console_run("killall udhcpc");

	exec_command("ifconfig -a | grep p2p0", wifi_type, 128);
	if (strstr(wifi_type, "p2p0")) {
		strcpy(ap, "p2p0");
		console_run("ifconfig p2p0 up");
	} else {
		strcpy(ap, "wlan1");
		console_run("ifconfig wlan1 up");
	}

	return start_hostapd(ap, ssid, psk, ip);
}

int wifi_stop_hostapd(void)
{
	char wifi_type[128];

	console_run("killall hostapd");
	exec_command("ifconfig -a | grep p2p0", wifi_type, 128);
	if (strstr(wifi_type, "p2p0")) {
		console_run("ifconfig p2p0 down");
	} else {
		console_run("killall dnsmasq");
		console_run("ifconfig wlan1 down");
	}

	int time = 100;
	while (time-- > 0 && !access("/var/run/hostapd", F_OK)) {
		usleep(10 * 1000);
	}

	return 0;
}

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <unistd.h>
#include <pthread.h>

typedef struct {
	RK_SOFTAP_STATE_CALLBACK callback;
	RK_SOFTAP_SERVER_TYPE server_type;
} RkSoftAp;

#define REQUEST_WIFI_LIST					"/provision/wifiListInfo"
#define REQUEST_WIFI_SET_UP					"/provision/wifiSetup"
#define REQUEST_IS_WIFI_CONNECTED			"/provision/wifiState"
#define REQUEST_POST_CONNECT_RESULT			"/provision/connectResult"

#define MSG_BUFF_LEN 8888
static char HTTP_RESPOSE_MESSAGE[] = "HTTP/1.1 200 OK\r\nContent-Type:text/html\r\nContent-Length:%d\r\n\r\n%s";

RkSoftAp m_softap;

#define SOCKET_PORT 8443

pthread_t m_thread;
bool m_isConnecting = false;
static int fd_server = -1;
static int m_port;

static RK_SOFTAP_STATE_CALLBACK m_cb = NULL;
static RK_SOFTAP_STATE m_state = RK_SOFTAP_STATE_IDLE;

static void sendState(RK_SOFTAP_STATE state, const char* data) {
	if(m_cb != NULL)
		m_cb(state, data);
}

static int initSocket(const unsigned int port)
{
	int ret, fd_socket, val = 1;
	struct sockaddr_in server_addr;

	/* create a socket */
	fd_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (fd_socket < 0) {
		printf("%s: create socket failed\n", __func__);
		return -1;
	}

	/* set socket non-blocking */
	//int flags;
	//flags = fcntl(fd_socket, F_GETFL, 0);
	//fcntl(sock, F_SETFL, flags | O_NONBLOCK);

	ret = setsockopt(fd_socket, SOL_SOCKET, SO_REUSEADDR, (void *)&val, sizeof(int));
	if (ret < 0) {
		printf("%s: setsockopt failed, ret: %d\n", __func__, ret);
		return -2;
	}

	/* initialize server address */
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port);

	/* bind with the local file */
	ret = bind(fd_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (ret < 0) {
		printf("%s: bind failed, ret: %d\n", __func__, ret);
		close(fd_socket);
		return -3;
	}

	/* listen */
	ret = listen(fd_socket, 1);
	if (ret < 0) {
		printf("%s: listen failed, ret: %d\n", __func__, ret);
		close(fd_socket);
		return -4;
	}

	return fd_socket;
}

static int sendWifiList(const int fd, const char* buf)
{
	char msg[MSG_BUFF_LEN] = {0};
	char send_msg[MSG_BUFF_LEN] = {0};
	size_t i, size = 0;
	char *wifilist;

scan_retry:
	printf("[RK] RK_wifi_scan ...\n");
	RK_wifi_scan();
	usleep(800000);

	printf("[RK] RK_wifi_scan_r_sec ...\n");
	wifilist = RK_wifi_scan_for_softap();
	printf("[RK] RK_wifi_scan_r_sec end wifilist: %p\n", wifilist);

	if (wifilist == NULL)
		goto scan_retry;
	if (wifilist && (strlen(wifilist) < 3)) {
		free(wifilist);
		goto scan_retry;
	}

	strcpy(msg, "{\"type\":\"WifiList\", \"content\":");
	strcat(msg, wifilist);
	strcat(msg, "}");

	snprintf(send_msg, sizeof(msg), HTTP_RESPOSE_MESSAGE, strlen(msg), msg);

	printf("scan_r: %s\n", send_msg);

	if (send(fd, send_msg, sizeof(send_msg), 0) < 0) {
		return false;
	}

	free(wifilist);
	return true;
}

static int isWifiConnected(const int fd, const char* buf)
{
	char msg[MSG_BUFF_LEN] = {0};
	bool isConn = 0;

	RK_WIFI_INFO_Connection_s info;

	if (!RK_wifi_running_getConnectionInfo(&info)) {
		if (strncmp(info.wpa_state, "COMPLETED", 9) == 0)
			isConn = 1;
	}

	memset(msg, 0, sizeof(msg));
	snprintf(msg, sizeof(msg), HTTP_RESPOSE_MESSAGE, 1, isConn ? "1" : "0");

	m_isConnecting = false;

	if (send(fd, msg, sizeof(msg), 0) < 0) {
		return false;
	}

	return true;
}

static char softap_ssid[128];
static char softap_psk[128];

static bool wifiSetup(const int fd, const char* buf)
{
	char msg[MSG_BUFF_LEN] = {0};
	char *str1, *str2;
	char tmp[128];
	int ret;

	printf("enter %s\n", __func__);

	memset(msg, 0, sizeof(msg));
	snprintf(msg, sizeof(msg), HTTP_RESPOSE_MESSAGE, 0, "");
	if (send(fd, msg, sizeof(msg), 0) < 0) {
		return false;
	}

	if (m_isConnecting == true)
		return false;

	memset(softap_ssid, 0, 128);
	memset(softap_psk, 0, 128);

	//{"ssid":"YHX","pwd":"rk123456789"}
	str1 = strstr(buf, "\"ssid\"");
	str2 = strstr(buf, "\"\,\"pwd\"");
	strncpy(softap_ssid, str1 + 8, str2 - str1 - 8);

	str1 = strstr(buf, "\"pwd\"");
	str2 = strstr(str1, "\"\}");
	strncpy(softap_psk, str1 + 7, str2 - str1 - 7);

	printf("do connect ssid:\"%s\", psk:\"%s\", isConnecting:%d\n", softap_ssid, softap_psk, m_isConnecting);

	sendState(RK_SOFTAP_STATE_CONNECTTING, NULL);
	m_state = RK_SOFTAP_STATE_CONNECTTING;

	system("wpa_cli -i wlan0 remove_network all");

	ret = RK_wifi_connect(softap_ssid, softap_psk);
	if (ret < 0) {
		sendState(RK_SOFTAP_STATE_FAIL, NULL);
		m_state = RK_SOFTAP_STATE_FAIL;
		m_isConnecting = false;
		return false;
	}

	m_isConnecting = true;

	printf("exit %s\n", __func__);
	return true;
}

static bool doConnectResult(const int fd, const char* buf)
{
	char msg[MSG_BUFF_LEN] = {0};
	char result[2];
	char *str;

	memset(msg, 0, sizeof(msg));
	snprintf(msg, sizeof(msg), HTTP_RESPOSE_MESSAGE, 0, "");
	if (send(fd, msg, sizeof(msg), 0) < 0) {
		return false;
	}

	//{"result":"0"}
	str = strstr(buf, "\"result\"");
	strncpy(result, str + 10, 1);
	printf("%s: %s", __func__, result);

	if (strncmp(result, "1", 1) == 0) { // connect success, disable ap
		sendState(RK_SOFTAP_STATE_SUCCESS, NULL);
		m_state = RK_SOFTAP_STATE_SUCCESS;
		wifi_stop_hostapd();
	} else {
		sendState(RK_SOFTAP_STATE_FAIL, NULL);
		m_state = RK_SOFTAP_STATE_FAIL;
	}

	return true;
}

static void handleRequest(const int fd_client)
{
	size_t n;
	char buf[2048] = {0};

	n = recv(fd_client, buf, sizeof(buf), 0);
	if (n <= 0) {
		close(fd_client);
		return;
	}
	buf[n] = '\0';
	printf("TcpServer recv buf:\n[%s]\n", buf);

	if (strstr(buf, REQUEST_WIFI_LIST)) {
		sendWifiList(fd_client, buf);
	} else if (strstr(buf, REQUEST_WIFI_SET_UP)) {
		wifiSetup(fd_client, buf);
	} else if (strstr(buf, REQUEST_IS_WIFI_CONNECTED)) {
		isWifiConnected(fd_client, buf);
	} else if (strstr(buf, REQUEST_POST_CONNECT_RESULT)) {
		doConnectResult(fd_client, buf);
	}

	close(fd_client);
}

static void *threadAccept(void *arg)
{
	int fd_client, port;
	struct sockaddr_in addr_client;
	socklen_t len_addr_client;
	len_addr_client = sizeof(addr_client);

	prctl(PR_SET_NAME, "threadAccept");

	port = *(int *)arg;

	printf("threadAccept port = %d\n", port);
	fd_server = initSocket(port);
	if (fd_server < 0) {
		printf("TcpServer::threadAccept init tcp socket port %d fail. error:%d\n", port, fd_server);
		goto end;
	}

	/* Accept connection all time */
	while (1) {
		printf("accept...\n");
		fd_client = accept(fd_server, (struct sockaddr *)&addr_client, &len_addr_client);
		printf("accept fd_client = %d\n", fd_client);
		if (fd_client < 0)
			goto end;

		handleRequest(fd_client);

		if (m_state == RK_SOFTAP_STATE_SUCCESS)
			break;
	}

end:
	printf("Exit Tcp accept thread\n");

	shutdown(fd_server, SHUT_RDWR);
	close(fd_server);
	fd_server = -1;

	return NULL;
}

static int startTcpServer(void)
{
	int ret;

	m_port = SOCKET_PORT;
	m_state = RK_SOFTAP_STATE_IDLE;
	printf("startTcpServer m_port = %d\n", m_port);
	ret = pthread_create(&m_thread, NULL, threadAccept, &m_port);

	return ret;
}

static int stopTcpServer()
{
	if (m_thread <= 0)
		return 0;

	if (fd_server >= 0) {
		shutdown(fd_server, SHUT_RDWR);
		close(fd_server);
		fd_server = -1;
	}

	if (0 != pthread_join(m_thread, NULL)) {
		printf("stopTcpServer fail\n");
		return -1;
	}

	m_thread = 0;
	printf("stopTcpServer success\n");
	return 0;
}

int RK_softap_start(char *name, RK_SOFTAP_SERVER_TYPE server_type)
{
	//RK_wifi_enable(1);
	wifi_start_hostapd(name, NULL, NULL);
	//startTcpServer();

	return 0;
}

int RK_softap_stop(void)
{
	//RK_wifi_enable(1);
	wifi_stop_hostapd();
	//stopTcpServer();
	return 0;
}


int RK_softap_register_callback(RK_SOFTAP_STATE_CALLBACK cb) {
	m_cb = cb;
	return 0;
}

int RK_softap_getState(RK_SOFTAP_STATE *pState)
{
	return m_state;
}
