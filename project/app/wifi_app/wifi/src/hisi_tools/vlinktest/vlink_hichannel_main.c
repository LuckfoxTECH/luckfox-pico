/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: sample cli file.
 * Author: Hisilicon
 * Create: 2020-09-09
 */
/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/sockios.h>
#include <linux/if.h>
#include <linux/wireless.h>

#include "securec.h"
#include "hi_base.h"
#include "hichannel_host.h"
#include "hichannel_host_comm.h"

#include "cJSON.h"

#include "Rk_wifi.h"
#include "rk_wifi_hal.h"
#include "utility.h"

/*****************************************************************************
  2 �궨�塢ȫ�ֱ���
*****************************************************************************/
static hi_s32 sample_exit_cmd_process(hi_void *wdata, hi_char *param, hi_u32 len, hi_void *pmsg);
static hi_s32 sample_help_cmd_process(hi_void *wdata, hi_char *param, hi_u32 len, hi_void *pmsg);

static const sample_cmd_entry_stru  g_sample_cmd[] = {
    {"help", sample_help_cmd_process},
    {"exit", sample_exit_cmd_process},
};

#define MAX_CMD_LEN 20
#define MAX_IPV4_LEN 16
#define WIFI_MAC_LEN 6
#define SLEEP_TIME 10
#define SAMPLE_CMD_NUM (sizeof(g_sample_cmd) / sizeof(g_sample_cmd[0]))
/*****************************************************************************
  3 ö�١��ṹ�嶨��
*****************************************************************************/
typedef enum {
    /* commands */
    SAMPLE_CMD_IOCTL,
    SAMPLE_CMD_HELP,
    SAMPLE_CMD_EXIT,
    SAMPLE_CMD_BUTT,
} sample_cmd_e;

typedef enum {
    HOST_CMD_GET_MAC,
    HOST_CMD_GET_IP,
    HOST_CMD_SET_FILTER,
    HOST_CMD_TBTT,
}host_cmd_e;

/* command/event information */
typedef struct {
    sample_cmd_e what;
    hi_u32 len;
    hi_u8 obj[CMD_MAX_LEN];
} sample_message_s;

struct snode {
    sample_message_s message;
    struct snode *next;
};

struct squeue {
    struct snode *front;
    struct snode *rear;
};

typedef struct {
    pthread_mutex_t mut;
    pthread_cond_t cond;
    struct squeue cmd_queue;
    pthread_t sock_thread;
    hi_s32 sockfd;
    hi_s32 eventfd;
} sample_link_s;

static hi_bool g_terminate = HI_FALSE;
static sample_link_s *g_sample_link = HI_NULL;
static hi_char host_cmd[][MAX_CMD_LEN] = {"cmd_get_mac","cmd_get_ip","cmd_set_filter", "cmd_set_netcfg", "cmd_set_keeplive", "cmd_set_standby", "cmd_set_deepsleep"};
static hi_u8 g_netsta = 0;

extern hi_bool ota_trans_ret;
extern hi_u32 hi_ota_sync_flag;
extern pthread_cond_t hi_ota_sync_cond;
extern pthread_mutex_t hi_ota_sync_lock;

extern hi_u32 hi_sync_flag;
extern hi_bool ota_trans_ret;
extern pthread_cond_t hi_sync_cond;
extern pthread_mutex_t hi_sync_lock;
extern hi_u64 hi_utc;
extern hi_u32 ifgetutc;
extern hi_u32 pir_state;

//rockchip
//内部保存WiFi信息一个副本
RK_WIFI_INFO_Connection_s g_hi_wifi_info;
unsigned char g_hi_wifi_state = 0;

//由于海思平台的命令都是异步操作，导致一个问题是有些命令比如get status这些
//同步命令无法使用，临时解决办法在rx_cb里面加个变量做同步
unsigned char g_hi_wifi_info_lock = 0;

//由于二次唤醒后重新加载驱动，如果WiFi一直连着，则不会主动反馈连接的成功的CONNECTED EVENT
//需要第一次主动去获取WiFi的状态，加个初始化编译，第一次主动上报状态
static unsigned char g_hi_wifi_init = 0;

/*****************************************************************************
  4 ����ʵ��
*****************************************************************************/
static hi_void sample_usage(hi_void)
{
    printf("\nUsage:\n");
    printf("\tsample_cli  quit          quit sample_ap\n");
    printf("\tsample_cli  help          show this message\n");
    printf("\tsample_cli  netcfg           send ssid pwd\n");
    printf("\tsample_cli  keeplive         keeplive\n");
    printf("\tsample_cli  standby          stanby\n");
    printf("\tsample_cli  deepsleep        wifi deep sleep\n");
}

hi_s32 sample_str_cmd_process(hi_void *wdata, hi_char *param, hi_u32 len, hi_void *pmsg)
{
    sample_unused(wdata);
    sample_message_s *msg = (sample_message_s *)pmsg;
    msg->what = SAMPLE_CMD_IOCTL;
    msg->len = len;
    (hi_void)memcpy_s(msg->obj, CMD_MAX_LEN, param, len);
    msg->obj[CMD_MAX_LEN - 1] = '\0';
    return HI_SUCCESS;
}

hi_s32 sample_help_cmd_process(hi_void *wdata, hi_char *param, hi_u32 len, hi_void *pmsg)
{
    sample_unused(wdata);
    sample_unused(param);
    sample_unused(len);
    sample_message_s *msg = (sample_message_s *)pmsg;
    msg->what = SAMPLE_CMD_HELP;
    return HI_SUCCESS;
}

static hi_s32 sample_exit_cmd_process(hi_void *wdata, hi_char *param, hi_u32 len, hi_void *pmsg)
{
    sample_unused(wdata);
    sample_unused(param);
    sample_unused(len);
    sample_message_s *msg = (sample_message_s *)pmsg;
    msg->what = SAMPLE_CMD_EXIT;
    return HI_SUCCESS;
}

static hi_void sample_cleanup(hi_void)
{
	sample_log_print("sample_cleanup enter\n");

	if (g_sample_link == NULL)
		return;

	if (g_sample_link->sock_thread) {
        pthread_cancel(g_sample_link->sock_thread);
        pthread_join(g_sample_link->sock_thread, HI_NULL);
    }

    pthread_mutex_destroy(&g_sample_link->mut);
    pthread_cond_destroy(&g_sample_link->cond);

    if (g_sample_link->sockfd != -1) {
        close(g_sample_link->sockfd);
    }

    if (g_sample_link->eventfd != -1) {
        close(g_sample_link->eventfd);
    }

    if (g_sample_link != HI_NULL) {
        free(g_sample_link);
        g_sample_link = HI_NULL;
    }
}

static hi_void sample_terminate(hi_s32 sig)
{
	sample_log_print("sample_cleanup enter\n");
	vlink_hi_channel_cmd_reboot();
	usleep(100000);
	sample_unused(sig);
	sample_cleanup();
	g_terminate = HI_TRUE;
	_exit(0);
}

static hi_void sample_deinit(hi_s32 sig)
{
    sample_unused(sig);
    sample_cleanup();
    g_terminate = HI_TRUE;
    _exit(0);
}

static hi_void sample_power(hi_s32 sig)
{
    sample_unused(sig);
}

static hi_s32 sample_wlan_init_up(hi_void)
{
    hi_s32 ret;
    hi_char cmd[SYSTEM_CMD_SIZE] = {0};

    (hi_void)memset_s(cmd, SYSTEM_CMD_SIZE, 0, SYSTEM_CMD_SIZE);
    if (snprintf_s(cmd, SYSTEM_CMD_SIZE, SYSTEM_CMD_SIZE - 1, "ifconfig %s up", SYSTEM_NETDEV_NAME) == -1) {
        sample_log_print("snprintf_s fail\n");
        return HI_FAILURE;
    }
    ret = system(cmd);
    if (ret == -1) {
        sample_log_print("%s up error\n", SYSTEM_NETDEV_NAME);
        return HI_FAILURE;
    }

    sample_log_print("net device up success\n");
    return HI_SUCCESS;
}

static hi_s32 sample_wlan_init_mac(hi_u8 *mac_addr, hi_u8 len)
{
    hi_s32 ret;
    hi_char cmd[SYSTEM_CMD_SIZE] = {0};

    if (len != WIFI_MAC_LEN) {
        return HI_FAILURE;
    }

    (hi_void)memset_s(cmd, SYSTEM_CMD_SIZE, 0, SYSTEM_CMD_SIZE);
    if (snprintf_s(cmd, SYSTEM_CMD_SIZE, SYSTEM_CMD_SIZE - 1, "ifconfig %s down", SYSTEM_NETDEV_NAME) == -1) {
        sample_log_print("snprintf_s fail\n");
        return HI_FAILURE;
    }
    ret = system(cmd);
    if (ret == -1) {
        sample_log_print("%s down error\n", SYSTEM_NETDEV_NAME);
        return HI_FAILURE;
    }

    (hi_void)memset_s(cmd, SYSTEM_CMD_SIZE, 0, SYSTEM_CMD_SIZE);
#if 0
    if (snprintf_s(cmd, SYSTEM_CMD_SIZE, SYSTEM_CMD_SIZE - 1, "ifconfig %s hw ether %x:%x:%x:%x:%x:%x",
        SYSTEM_NETDEV_NAME, mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]) == -1) {
#else
    if (snprintf_s(cmd, SYSTEM_CMD_SIZE, SYSTEM_CMD_SIZE - 1, "ifconfig %s hw ether %s", SYSTEM_NETDEV_NAME, mac_addr) == -1) {
#endif
        sample_log_print("snprintf_s fail\n");
        return HI_FAILURE;
    }
    ret = system(cmd);
    if (ret == -1) {
        sample_log_print("%s set mac error\n", SYSTEM_NETDEV_NAME);
        return HI_FAILURE;
    }

    (hi_void)memset_s(cmd, SYSTEM_CMD_SIZE, 0, SYSTEM_CMD_SIZE);
    if (snprintf_s(cmd, SYSTEM_CMD_SIZE, SYSTEM_CMD_SIZE - 1, "ifconfig %s up", SYSTEM_NETDEV_NAME) == -1) {
        sample_log_print("snprintf_s fail\n");
        return HI_FAILURE;
    }
    ret = system(cmd);
    if (ret == -1) {
        sample_log_print("%s up error\n", SYSTEM_NETDEV_NAME);
        return HI_FAILURE;
    }

    sample_log_print("net device set mac success\n");

    return HI_SUCCESS;
}

static hi_s32 sample_wlan_init_ip(hi_u8 *ip_addr, hi_u8 *netmask, hi_u8 *gateway, hi_u8 *dnsfirst, hi_u8 *dnssec, hi_u8 len)
{
    hi_s32 ret;
    hi_char cmd[SYSTEM_CMD_SIZE] = {0};

    sample_unused(len);
    if ((ip_addr[0] == 0) && (ip_addr[1] == 0) && (ip_addr[2] == 0) && (ip_addr[3] == 0)) { /* 1 2 3 ipaddr */
        return HI_SUCCESS;
    }

    (hi_void)memset_s(cmd, SYSTEM_CMD_SIZE, 0, SYSTEM_CMD_SIZE);
    if (snprintf_s(cmd, SYSTEM_CMD_SIZE, SYSTEM_CMD_SIZE - 1, "ifconfig %s down", SYSTEM_NETDEV_NAME) == -1) {
        sample_log_print("snprintf_s fail\n");
        return HI_FAILURE;
    }
    ret = system(cmd);
    if (ret == -1) {
        sample_log_print("%s down error\n", SYSTEM_NETDEV_NAME);
        return HI_FAILURE;
    }

    (hi_void)memset_s(cmd, SYSTEM_CMD_SIZE, 0, SYSTEM_CMD_SIZE);
#if 0
    if (snprintf_s(cmd, SYSTEM_CMD_SIZE, SYSTEM_CMD_SIZE - 1,
        "ifconfig %s %d.%d.%d.%d netmask %d.%d.%d.%d", SYSTEM_NETDEV_NAME, ip_addr[0], ip_addr[1], ip_addr[2], ip_addr[3]
        , ip_addr[4], ip_addr[5], ip_addr[6], ip_addr[7]) == -1) {
        sample_log_print("snprintf_s fail\n");
        return HI_FAILURE;
    }
#else
    (hi_void)snprintf_s(cmd, SYSTEM_CMD_SIZE, SYSTEM_CMD_SIZE - 1, "ifconfig %s %s netmask %s", SYSTEM_NETDEV_NAME, ip_addr, netmask);
#endif
    ret = system(cmd);
    pr_info("cmd: %s\n", cmd);

    if (ret == -1) {
        sample_log_print("%s set ip error\n", SYSTEM_NETDEV_NAME);
        return HI_FAILURE;
    }

    (hi_void)memset_s(cmd, SYSTEM_CMD_SIZE, 0, SYSTEM_CMD_SIZE);
    if (snprintf_s(cmd, SYSTEM_CMD_SIZE, SYSTEM_CMD_SIZE - 1, "ifconfig %s up", SYSTEM_NETDEV_NAME) == -1) {
        sample_log_print("snprintf_s fail\n");
        return HI_FAILURE;
    }
    ret = system(cmd);
    pr_info("cmd: %s\n", cmd);
    if (ret == -1) {
        sample_log_print("%s up error\n", SYSTEM_NETDEV_NAME);
        return HI_FAILURE;
    }
    sample_log_print("net device set ip success\n");

//route table
    (hi_void)memset_s(cmd, SYSTEM_CMD_SIZE, 0, SYSTEM_CMD_SIZE);
    (hi_void)snprintf_s(cmd, SYSTEM_CMD_SIZE, SYSTEM_CMD_SIZE - 1, "route | grep default && route del default dev %s", SYSTEM_NETDEV_NAME);
    ret = system(cmd);
    pr_info("cmd: %s\n", cmd);
    if (ret == -1) {
        sample_log_print("%s error\n", cmd);
    }    

    (hi_void)memset_s(cmd, SYSTEM_CMD_SIZE, 0, SYSTEM_CMD_SIZE);
    (hi_void)snprintf_s(cmd, SYSTEM_CMD_SIZE, SYSTEM_CMD_SIZE - 1, "route add default gw %s", gateway);
    ret = system(cmd);
    pr_info("cmd: %s\n", cmd);
    if (ret == -1) {
        sample_log_print("%s error\n", cmd);
	    return HI_FAILURE;
    } 
    sample_log_print("net device set route success\n");
//dns
    ret = system("echo -n > /etc/resolv.conf");
    pr_info("cmd: %s\n", cmd);
    if (ret == -1) {
        sample_log_print("%s error\n", cmd);
		return HI_FAILURE;
    } 

    (hi_void)memset_s(cmd, SYSTEM_CMD_SIZE, 0, SYSTEM_CMD_SIZE);
    (hi_void)snprintf_s(cmd, SYSTEM_CMD_SIZE, SYSTEM_CMD_SIZE - 1, "echo nameserver %s >> /etc/resolv.conf", dnsfirst);
    ret = system(cmd);
    pr_info("cmd: %s\n", cmd);
    if (ret == -1) {
        sample_log_print("%s error\n", cmd);
		return HI_FAILURE;
    } 

    (hi_void)memset_s(cmd, SYSTEM_CMD_SIZE, 0, SYSTEM_CMD_SIZE);
    (hi_void)snprintf_s(cmd, SYSTEM_CMD_SIZE, SYSTEM_CMD_SIZE - 1, "echo nameserver %s >> /etc/resolv.conf", dnssec);
    ret = system(cmd);
    pr_info("cmd: %s\n", cmd);
    if (ret == -1) {
        sample_log_print("%s error\n", cmd);
		return HI_FAILURE;
    }
    pr_info("net device set dns success\n");
    return HI_SUCCESS;
}

static hi_s32 sample_wlan_flush_ip(hi_void)
{
    hi_s32 ret;
    hi_char cmd[SYSTEM_CMD_SIZE] = {0};

    (hi_void)memset_s(cmd, SYSTEM_CMD_SIZE, 0, SYSTEM_CMD_SIZE);
    if (snprintf_s(cmd, SYSTEM_CMD_SIZE, SYSTEM_CMD_SIZE - 1, "ifconfig %s 0.0.0.0", SYSTEM_NETDEV_NAME) == -1) {
        sample_log_print("snprintf_s fail\n");
        return HI_FAILURE;
    }
    ret = system(cmd);
    if (ret != 0) {
        sample_log_print("%s flush error\n", SYSTEM_NETDEV_NAME);
        return HI_FAILURE;
    }
	sample_log_print("net device flush ip success\n");
}

static void *wlan_start_ota(void *arg)
{
	vlink_hi_channel_cmd_ota("/data/wifi_ota/Hi3861L_tdemo_allinone.bin");
}

static hi_s32 sample_wlan_ota_check(char *version)
{
	hi_s32 ret;
	char ver[10];
	hi_u32 ver_old = 0, ver_new = 0;
	static pthread_t start_wifi_ota = 0;

	if (access("/data/wifi_ota/VERSION",0))
		return 0;

	exec_command("cat /data/wifi_ota/VERSION", ver, 10);
	ver_new = atoi(ver);
	ver_old = atoi(version);

	pr_info("firmware ver_old: %d, ver_new: %d", ver_old, ver_new);
	if ((ver_old != 0) && (ver_old > ver_new))
		return 0;

	pthread_create(&start_wifi_ota, NULL, wlan_start_ota, NULL);
	pthread_detach(start_wifi_ota);

	return 0;
}

static hi_void set_lo_ipaddr(hi_void)
{
    hi_s32 results;
    hi_char cmd[SYSTEM_CMD_SIZE] = {0}; /* system Temporary variables */
    hi_char *spawn_args[] = {"ifconfig", "lo", "127.0.0.1", HI_NULL};

    results = sprintf_s(cmd, sizeof(cmd), "%s %s %s", spawn_args[0], /* spawn_args[0]:ifconfig */
                        spawn_args[1], spawn_args[2]); /* spawn_args[1]:lo,spawn_args[2]:ipaddr */
    if (results < EOK) {
        sample_log_print("SAMPLE_STA: set lo ipaddr sprintf_s err!\n");
        return;
    }

    system(cmd);
}

static hi_s32 sample_sock_create(hi_void)
{
    if (g_sample_link == HI_NULL) {
        return HI_FAILURE;
    }

    g_sample_link->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (g_sample_link->sockfd == -1) {
        sample_log_print("error:%s", strerror(errno));
        return HI_FAILURE;
    }

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SOCK_PORT);

    if (bind(g_sample_link->sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
        sample_log_print("error:%s", strerror(errno));
        return HI_FAILURE;
    }

    g_sample_link->eventfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (g_sample_link->eventfd == -1) {
        sample_log_print("error:%s", strerror(errno));
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static hi_s32 sample_enqueue(struct squeue *pqueue, const sample_message_s *element)
{
    struct snode *pnew = HI_NULL;

    if (pqueue == HI_NULL || element == HI_NULL) {
        return -1;
    }
    /* Create a new node */
    pnew = malloc(sizeof(struct snode));
    if (pnew == HI_NULL) {
        return -1;
    }

    pnew->message = *element;
    pnew->next = HI_NULL;

    if (pqueue->rear == HI_NULL) {
        /* queue is empty, set front and rear points to new node */
        pqueue->front = pqueue->rear = pnew;
    } else {
        /* queue is not empty, set rear points to the new node */
        pqueue->rear = pqueue->rear->next = pnew;
    }

    return HI_SUCCESS;
}

static hi_s32 sample_dequeue(struct squeue *pqueue, sample_message_s *element)
{
    struct snode *p = HI_NULL;

    if (pqueue == HI_NULL || element == HI_NULL) {
        return HI_FAILURE;
    }

    if (pqueue->front == HI_NULL) {
        return HI_FAILURE;
    }

    *element = pqueue->front->message;
    p = pqueue->front;
    pqueue->front = p->next;
    /* if the queue is empty, set rear = NULL */
    if (pqueue->front == HI_NULL) {
        pqueue->rear = HI_NULL;
    }

    free(p);
    return HI_SUCCESS;
}

hi_s32 get_netcfg_info(char *ssid, char *password)
{
	FILE *fp;
	int ret;

	fp = fopen("/tmp/vlink_main.conf", "r");
	if (!fp)
		return HI_FAILURE;
	ret = fscanf(fp, "%s", ssid);
	if (ret <= 0)
		goto failure;
	ret = fscanf(fp, "%s", password);
	if (ret <= 0)
		goto failure;

	fclose(fp);
	return HI_SUCCESS;

failure:
	fclose(fp);
	return HI_FAILURE;
}

hi_s32 vlink_sample_str_cmd_process(hi_void *pmsg)
{
    sample_log_print("vlink_sample_str_cmd_process====pmsg=[%s]==\n", pmsg);
    if (strncmp(pmsg, "netcfg", 6) == 0) {
	char sendmsg[120] = {0};
	int msglen = 0;
	char ssid[33] = {0};
	char password[65] = {0};
	if (get_netcfg_info(ssid, password) == HI_SUCCESS) {
	vlink_hi_channel_cmd_netcfg_info(ssid, password, sendmsg, &msglen);
	} else {
#if 1
	vlink_hi_channel_cmd_netcfg_info("VLink", "Vlink8888", sendmsg, &msglen);
#else
	vlink_hi_channel_cmd_netcfg_info("TP-LINK_AA04", "1427021981", sendmsg, &msglen);
#endif
	//sample_log_print("vlink_sample_str_cmd_process====sendmsg=[%s]==\n", sendmsg);
	}
	hi_channel_send_to_dev(sendmsg, msglen);
    } else if (strncmp(pmsg, "getip", 5) == 0) {
	vlink_hi_channel_cmd_getip_info();
    } else if (strncmp(pmsg, "getmac", 6) == 0) {
	vlink_hi_channel_cmd_getmac_info();
    } else if (strncmp(pmsg, "keeplive", 8) == 0) {
	vlink_hi_channel_cmd_keeplive_info("192.168.3.116", "9000", "10");
    } else if (strncmp(pmsg, "filterwifi", 10) == 0) {
	vlink_hi_channel_cmd_setfilter_info("wifi");
    } else if (strncmp(pmsg, "filtercam", 9) == 0) {
	vlink_hi_channel_cmd_setfilter_info("camera");
    } else if (strncmp(pmsg, "deepsleep", 9) == 0) {
	vlink_hi_channel_cmd_deepsleep_info();
    } else if (strncmp(pmsg, "startap", 7) == 0) {
	vlink_hi_channel_cmd_startap_info();
    } else if (strncmp(pmsg, "tcpstart", 8) == 0) {
	vlink_tcp_socket_start_info();
    } else if (strncmp(pmsg, "startota", 8) == 0) {
	vlink_hi_channel_cmd_startota();
    } else if (strncmp(pmsg, "getrssi", 7) == 0) {
	vlink_hi_channel_cmd_getrssi_info();
    } else if (strncmp(pmsg, "getver", 6) == 0) {
	vlink_hi_channel_cmd_getversion_info();
    } else if (strncmp(pmsg, "wakecode", 8) == 0) {
	vlink_hi_channel_cmd_getwakecode_info();
    } else if (strncmp(pmsg, "factory", 7) == 0) {
	vlink_hi_channel_cmd_factoryreset_info();
    } else if (strncmp(pmsg, "pirset", 6) == 0) {
	hi_u8 enable = 0;
	sscanf_s(pmsg, "%*s %d", &enable);
	vlink_hi_channel_cmd_setpir_info(enable);
    } else if (strncmp(pmsg, "pirget", 6) == 0) {
	vlink_hi_channel_cmd_getpir_info();
    } else if (strncmp(pmsg, "tyset", 5) == 0) {
	vlink_hi_channel_cmd_settuya_info();
    } else if (strncmp(pmsg, "getall", 6) == 0) {
	vlink_hi_channel_cmd_getall_info();
    } else if (strncmp(pmsg, "pirclr", 6) == 0) {
	vlink_hi_channel_cmd_clrpir_info();
    }

    return HI_SUCCESS;
}


static hi_void *sample_sock_thread(hi_void *args)
{
    hi_char link_buf[SOCK_BUF_MAX];
    ssize_t recvbytes = 0;
    sample_message_s message;
    struct sockaddr_in clientaddr;
    socklen_t addrlen = sizeof(clientaddr);
    sample_unused(args);

    while (1) {
        (hi_void)memset_s(link_buf, sizeof(link_buf), 0, sizeof(link_buf));
        (hi_void)memset_s(&clientaddr, sizeof(struct sockaddr_in), 0, addrlen);

        recvbytes = recvfrom(g_sample_link->sockfd, link_buf, sizeof(link_buf), 0, (struct sockaddr *)&clientaddr, &addrlen);
        if (recvbytes < 0) {
            if (errno == EINTR) {
                usleep(SLEEP_TIME);
                continue;
            } else {
                sample_log_print("recvfrom error! fd:%d\n", g_sample_link->sockfd);
                return HI_NULL;
            }
        }
#if 0
        if (sample_sock_cmd_entry(g_sample_link, link_buf, recvbytes, (hi_void *)&message) != HI_SUCCESS) {
            sample_log_print("sample_str_cmd_process entry\n");
            sample_str_cmd_process(g_sample_link, link_buf, recvbytes, (hi_void *)&message);
        }
        if (vlink_sample_str_cmd_process(link_buf) != HI_SUCCESS) {
            sample_log_print("vlink_sample_str_cmd_process error\n");
        }
		sample_str_cmd_process(g_sample_link, link_buf, recvbytes, (hi_void *)&message);
#endif
        sample_log_print("sample_sock_thread====recvfrom string=[%s]==\n", link_buf);

        if (vlink_sample_str_cmd_process(link_buf) != HI_SUCCESS) {
            sample_log_print("vlink_sample_str_cmd_process error\n");
        }

	//sample_str_cmd_process(g_sample_link, link_buf, recvbytes, (hi_void *)&message);

        if (sendto(g_sample_link->sockfd, "OK", strlen("OK"), MSG_DONTWAIT, (const struct sockaddr *)&clientaddr,
                   addrlen) == -1) {
            sample_log_print("sendto error!fd:%d\n", g_sample_link->sockfd);
        }

        pthread_mutex_lock(&g_sample_link->mut);
        if (sample_enqueue(&g_sample_link->cmd_queue, &message) == HI_SUCCESS) {
            pthread_cond_signal(&g_sample_link->cond);
        }
        pthread_mutex_unlock(&g_sample_link->mut);
    }
}

#if 0
void sample_link_rec_cb(unsigned char *msg_data, int len)
{
    hi_s32 ret;
    hi_s32 index;

    if ((len == 0) || (msg_data == HI_NULL)) {
        return;
    }

    index = msg_data[0];
    sample_log_print("hisi_link_rec:%d\n", index);
    if (index == HOST_CMD_GET_MAC) {
        ret = sample_wlan_init_mac(&msg_data[1], WIFI_MAC_LEN);
        if (ret != HI_SUCCESS) {
            sample_log_print("sample_wlan_init_mac is fail\n");
        }
    } else if (index == HOST_CMD_GET_IP) {
        ret = sample_wlan_init_ip(&msg_data[1], len - 1);
        if (ret != HI_SUCCESS) {
            sample_log_print("sample_wlan_init_ip is fail\n");
        }
    } else {
        sample_log_print("hisi_link_rec is fail\n");
    }
}
#else
hi_s32 vlink_hi_channel_set_mac(hi_char *macaddr)
{
	hi_s32 ret;
	//hi_u8 mactmp[6] = {0};

	//sscanf(macaddr, "%02X:%02X:%02X:%02X:%02X:%02X", mactmp[0], mactmp[1], mactmp[2], mactmp[3], mactmp[4], mactmp[5]);
	
        sample_log_print("vlink_hi_channel_set_mac======mac_addr:%s\n", macaddr);
        ret = sample_wlan_init_mac(macaddr, WIFI_MAC_LEN);
        if (ret != HI_SUCCESS) {
            sample_log_print("sample_wlan_init_mac is fail\n");
        }

	return ret;
}

/*
 * VLINK_WIFI_NETWORK_NETCFG = 0,
 * VLINK_WIFI_NETWORK_CONNECTING,
 * VLINK_WIFI_NETWORK_CONNECTE_OK,
 * VLINK_WIFI_NETWORK_DISCONNECTE,
 * VLINK_WIFI_NETWORK_DHCP_OK,
 * VLINK_WIFI_NETWORK_DHCP_ERR,	
 * VLINK_WIFI_NETWORK_NOAP,
 * VLINK_WIFI_NETWORK_PWD_ERR,
 * VLINK_WIFI_NETWORK_OTH_ERR,
 */
hi_s32 vlink_wifi_work_status_ret(hi_char *workstatus)
{
	hi_s32 ret;

	if (NULL == m_wifi_cb) {
		return 0;
	}

	sample_log_print("vlink_wifi_work_status_ret======workstatus:%s\n", workstatus);

	switch(atoi(workstatus))
	{
	case 0:
		sample_log_print("vlink_wifi_work_status_ret======net config\n");
		break;
	case 1:
		sample_log_print("vlink_wifi_work_status_ret======net connecting\n");
		m_wifi_cb(RK_WIFI_State_CONNECTING, NULL);
		break;
	case 2:
		//HI_WIFI_EVT_CONNECTED
		sample_log_print("vlink_wifi_work_status_ret======net connect ok\n");
		m_wifi_cb(RK_WIFI_State_CONNECTED, NULL);
		//g_hi_wifi_state = 1;
		break;
	case 3:
		//HI_WIFI_EVT_DISCONNECTED
		sample_log_print("vlink_wifi_work_status_ret======net disconnect\n");
		sample_wlan_flush_ip();
		m_wifi_cb(RK_WIFI_State_DISCONNECTED, NULL);
		memset(&g_hi_wifi_info.wpa_state, 0, 20);
		strcpy(&g_hi_wifi_info.wpa_state, "DISCONNECTED");
		//g_hi_wifi_state = 0;
		break;
	case 4:
		sample_log_print("vlink_wifi_work_status_ret======net dhcp ok\n");
		m_wifi_cb(RK_WIFI_State_DHCP_OK, NULL);
		break;
	case 5:
		sample_log_print("vlink_wifi_work_status_ret======net dhcp error\n");
		m_wifi_cb(RK_WIFI_State_DHCP_ERR, NULL);
		break;
	case 6:
		//HI_WIFI_EVT_STA_FCON_NO_NETWORK
		sample_log_print("vlink_wifi_work_status_ret======net no ap\n");
		m_wifi_cb(RK_WIFI_State_CONNECTFAILED_NOT_FOUND, NULL);
		break;
	case 7:
		sample_log_print("vlink_wifi_work_status_ret======net password error\n");
		m_wifi_cb(RK_WIFI_State_CONNECTFAILED_WRONG_KEY, NULL);
		break;
	case 8:
		sample_log_print("vlink_wifi_work_status_ret======net other error\n");
		m_wifi_cb(RK_WIFI_State_CONNECTFAILED, NULL);
		break;
	}

	return ret;
}

hi_s32 sample_send_event(unsigned char *msg_data, int len)
{
    struct sockaddr_in servaddr;

	if (g_sample_link == NULL)
		return HI_SUCCESS;

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(SOCK_EVENT_PORT);

    if (sendto(g_sample_link->eventfd, msg_data, len, MSG_DONTWAIT, (const struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
        sample_log_print("sendto error!fd:%d\n", g_sample_link->eventfd);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/*
 * sample_link_rec_cb 函数case对应的含义
 * #define VLINK_WIFI_CMD_SENDMSG_NETCFG		"1"
 * #define VLINK_WIFI_CMD_SENDMSG_GETMAC		"2"
 * #define VLINK_WIFI_CMD_SENDMSG_GETIP			"3"
 * #define VLINK_WIFI_CMD_SENDMSG_SETFILTER		"4"
 * #define VLINK_WIFI_CMD_SENDMSG_KEEPLIVE		"5"
 * #define VLINK_WIFI_CMD_SENDMSG_STANDBY		"6"
 * #define VLINK_WIFI_CMD_SENDMSG_DEEPSLEEP		"7"
 * #define VLINK_WIFI_CMD_SENDMSG_STARTAP		"8"
 * #define VLINK_WIFI_CMD_SENDMSG_STARTOTA		"9"
 * #define VLINK_WIFI_CMD_SENDMSG_OTADATA		"10"
 * #define VLINK_WIFI_CMD_SENDMSG_OTAWRITERET	"11"
 * #define VLINK_WIFI_CMD_SENDMSG_OTARET		"12"
 * #define VLINK_WIFI_CMD_SENDMSG_REASON		"13"
 * #define VLINK_WIFI_CMD_SENDMSG_GETINFO		"88"
 */
void sample_link_rec_cb(unsigned char *msg_data, int len)
{
	hi_s32 ret;
	hi_s32 index;

	cJSON* item;
	sample_log_print("sample_link_rec_cb:--start--\n");
	if ((len == 0) || (msg_data == HI_NULL)) {
		return;
	}
	sample_log_print("sample_link_rec_cb:msg_data-[%s]-[%d]--\n", msg_data, len);
	cJSON* root = cJSON_Parse(msg_data); 
	if(NULL == root)
	{
		sample_log_print("sample_link_rec_cb:parseJson---Parse fail\n");
		return;
	}

	item = cJSON_GetObjectItem(root, "cmd");
	if(NULL != item)
	{
		hi_u8 cmdval = atoi(item->valuestring);
		switch(cmdval)
		{
		case 1:
			//VLINK_WIFI_CMD_SENDMSG_NETCFG
			break;
		case 2:
		{
			//VLINK_WIFI_CMD_SENDMSG_GETMAC
			hi_char macaddr[20] = {0};
			item = cJSON_GetObjectItem(root, "mac");
			if((NULL != item) && (NULL != item->valuestring) && (0 != strlen(item->valuestring)))
			{
				vlink_hi_channel_set_mac(item->valuestring);
			}

			sample_send_event(msg_data, len);
			break;
		}
		case 3:
		{
			//VLINK_WIFI_CMD_SENDMSG_GETIP
			hi_char ipaddr[20] = {0};
			hi_char nwaddr[20] = {0};
			hi_char gwaddr[20] = {0};
			hi_char dnsfaddr[20] = {0};
			hi_char dnssaddr[20] = {0};

			item = cJSON_GetObjectItem(root, "ip");
			if((NULL != item) && (NULL != item->valuestring) && (0 != strlen(item->valuestring)))
			{
				memcpy_s(ipaddr, 20, item->valuestring, strlen(item->valuestring));
				if(strcmp(ipaddr, "0.0.0.0") != 0)
				{
					g_netsta = 4;
				}
			}
			item = cJSON_GetObjectItem(root, "nm");
			if((NULL != item) && (NULL != item->valuestring) && (0 != strlen(item->valuestring)))
			{
				memcpy_s(nwaddr, 20, item->valuestring, strlen(item->valuestring));
			}
			item = cJSON_GetObjectItem(root, "gw");
			if((NULL != item) && (NULL != item->valuestring) && (0 != strlen(item->valuestring)))
			{
				memcpy_s(gwaddr, 20, item->valuestring, strlen(item->valuestring));
			}
			item = cJSON_GetObjectItem(root, "dns1");
			if((NULL != item) && (NULL != item->valuestring) && (0 != strlen(item->valuestring)))
			{
				memcpy_s(dnsfaddr, 20, item->valuestring, strlen(item->valuestring));
			}
			item = cJSON_GetObjectItem(root, "dns2");
			if((NULL != item) && (NULL != item->valuestring) && (0 != strlen(item->valuestring)))
			{
				memcpy_s(dnssaddr, 20, item->valuestring, strlen(item->valuestring));
			}

			sample_wlan_init_ip(ipaddr, nwaddr, gwaddr, dnsfaddr, dnssaddr, 0);
			if (m_wifi_cb)
				m_wifi_cb(RK_WIFI_State_DHCP_OK, NULL);

			sample_send_event(msg_data, len);
			break;
		}
		case 4:
			//VLINK_WIFI_CMD_SENDMSG_SETFILTER
			break;
		case 5:
			//VLINK_WIFI_CMD_SENDMSG_KEEPLIVE
			break;
		case 6:
			//VLINK_WIFI_CMD_SENDMSG_STANDBY
			break;
		case 7:
			//VLINK_WIFI_CMD_SENDMSG_DEEPSLEEP
			break;
		case 8:
			//VLINK_WIFI_CMD_SENDMSG_STARTAP
		break;
		case 9:
			//VLINK_WIFI_CMD_SENDMSG_STARTOTA
		break;
		case 10:
			//VLINK_WIFI_CMD_SENDMSG_OTADATA
		break;
		case 11:
			//VLINK_WIFI_CMD_SENDMSG_OTAWRITERET
			item = cJSON_GetObjectItem(root, "ret");
			if((NULL != item) && (NULL != item->valuestring) && (0 != strlen(item->valuestring)))
			{
				vlink_wifi_ota_process_write_ret(item->valuestring);
			}
			break;
		case 12:
			//VLINK_WIFI_CMD_SENDMSG_OTARET
			break;
		case 13:
			//VLINK_WIFI_CMD_SENDMSG_REASON
			item = cJSON_GetObjectItem(root, "reason");
			if((NULL != item) && (NULL != item->valuestring) && (0 != strlen(item->valuestring)))
			{
				vlink_wifi_work_status_ret(item->valuestring);
			}
		break;
		case 14:
			item = cJSON_GetObjectItem(root, "rssi");
			if((NULL != item) && (NULL != item->valuestring) && (0 != strlen(item->valuestring)))
			{
				sample_log_print(":rssi-[%s]--\n", item->valuestring);
			}
		break;
		case 15:
			item = cJSON_GetObjectItem(root, "ver");
			if((NULL != item) && (NULL != item->valuestring) && (0 != strlen(item->valuestring)))
			{
				sample_log_print(":ver-[%s]--\n", item->valuestring);
			}
			item = cJSON_GetObjectItem(root, "ker");
			if((NULL != item) && (NULL != item->valuestring) && (0 != strlen(item->valuestring)))
			{
				sample_log_print(":ker-[%s]--\n", item->valuestring);
			}

			sample_send_event(msg_data, len);
		break;
		case 16:
			item = cJSON_GetObjectItem(root, "code");
			if((NULL != item) && (NULL != item->valuestring) && (0 != strlen(item->valuestring)))
			{
				sample_log_print(":code-[%s]--\n", item->valuestring);
			}
		break;
		case 17:

		break;
		case 18:

		break;
		case 19:
			item = cJSON_GetObjectItem(root, "enable");
			if((NULL != item) && (NULL != item->valuestring) && (0 != strlen(item->valuestring)))
			{
				sample_log_print(":pir enable-[%s]--\n", item->valuestring);
			}
		break;
		case 20:
			sample_send_event(msg_data, len);
			break;
		case 21:
		{
			hi_u8 netsta;

			item = cJSON_GetObjectItem(root, "netsta");
			if((NULL != item))
			{
				sample_log_print(":netsta-[%d]--\n", item->valueint);
				netsta = item->valueint;
				if((g_netsta != netsta) && (netsta == 6))
				{
					sample_wlan_flush_ip();
				}
				g_netsta = netsta;
			}
			break;
		}
		case 22:
			sample_send_event(msg_data, len);
			break;
		case 50:
		{
			item = cJSON_GetObjectItem(root,"event-type");
			int type = atoi(item->valuestring);
			RK_WIFI_INFO_Connection_s info;
			info.lp_event_type = type;
			if (info.lp_event_type == RK_WIFI_LP_BAT_LEVEL) {
				item = cJSON_GetObjectItem(root,"bat");
				info.lp_data[0] = atoi(item->valuestring); 
			} else if (info.lp_event_type == RK_WIFI_LP_WAKEUP_REASON) {
				item = cJSON_GetObjectItem(root,"wakeup-reason");
				info.lp_data[0] = atoi(item->valuestring);
			}
			printf("info.lp-event-type is %d \n",type);
			printf("get event type ! \n");
			m_wifi_cb(RK_WIFI_State_LP_EVENT_TYPE,&info);
			break;
		}

		case 51:
		{
			pthread_mutex_lock(&hi_sync_lock);
			item = cJSON_GetObjectItem(root,"ret");
			ifgetutc = atoi(item->valuestring);
			hi_sync_flag = 1;
			if (ifgetutc == 1) {
				item = cJSON_GetObjectItem(root,"utc");
				hi_utc = atoll(item->valuestring);
			}
			pthread_cond_signal(&hi_sync_cond);
			pthread_mutex_unlock(&hi_sync_lock);
			break;
		}

		case 52:
		{
			item = cJSON_GetObjectItem(root,"failed");
			if (NULL !=  item) {
				ota_trans_ret = HI_FAILURE;
			}
			pthread_mutex_lock(&hi_ota_sync_lock);
			hi_ota_sync_flag = 1;
			pthread_cond_signal(&hi_ota_sync_cond);
			pthread_mutex_unlock(&hi_ota_sync_lock);
			break;
		}

		case 53:
		{
			pthread_mutex_lock(&hi_sync_lock);
			item = cJSON_GetObjectItem(root,"pir-val");
			pir_state = atoi(item->valuestring);
			hi_sync_flag = 1;
			pthread_cond_signal(&hi_sync_cond);
			pthread_mutex_unlock(&hi_sync_lock);
			break;
		}

		case 88:
		{
			//VLINK_WIFI_CMD_SENDMSG_GETINFO
			hi_char ipaddr[20] = {0};
			hi_char nwaddr[20] = {0};
			hi_char gwaddr[20] = {0};
			hi_char dnsfaddr[20] = {0};
			hi_char dnssaddr[20] = {0};

			RK_WIFI_INFO_Connection_s hi_wifi_info;
			memset(&hi_wifi_info, 0, sizeof(RK_WIFI_INFO_Connection_s));

			item = cJSON_GetObjectItem(root, "version");
			if ((NULL != item) && (NULL != item->valuestring) && (0 != strlen(item->valuestring)))
			{
				memcpy_s(hi_wifi_info.version, 10, item->valuestring, strlen(item->valuestring));
			}

			item = cJSON_GetObjectItem(root, "mac");
			if ((NULL != item) && (NULL != item->valuestring) && (0 != strlen(item->valuestring)))
			{
				memcpy_s(hi_wifi_info.mac_address, 20, item->valuestring, strlen(item->valuestring));
			}
			item = cJSON_GetObjectItem(root, "ssid");
			if ((NULL != item) && (NULL != item->valuestring) && (0 != strlen(item->valuestring))) {
				memcpy_s(hi_wifi_info.ssid, 64, item->valuestring, strlen(item->valuestring));
			}
			item = cJSON_GetObjectItem(root, "bssid");
			if ((NULL != item) && (NULL != item->valuestring) && (0 != strlen(item->valuestring))) {
				memcpy_s(hi_wifi_info.bssid, 20, item->valuestring, strlen(item->valuestring));
			}
			item = cJSON_GetObjectItem(root, "channel");
			if ((NULL != item) && (NULL != item->valuestring) && (0 != strlen(item->valuestring))) {
				memcpy_s(hi_wifi_info.channel, 6, item->valuestring, strlen(item->valuestring));
			}
			item = cJSON_GetObjectItem(root, "status");
			if ((NULL != item) && (NULL != item->valuestring) && (0 != strlen(item->valuestring))) {
				memcpy_s(hi_wifi_info.wpa_state, 20, item->valuestring, strlen(item->valuestring));
			}

			item = cJSON_GetObjectItem(root, "ip");
			if((NULL != item) && (NULL != item->valuestring) && (0 != strlen(item->valuestring)))
			{
				memcpy_s(hi_wifi_info.ip_address, 20, item->valuestring, strlen(item->valuestring));
				memcpy_s(ipaddr, 20, item->valuestring, strlen(item->valuestring));
				if(strcmp(hi_wifi_info.ip_address, "0.0.0.0") != 0)
				{
					g_netsta = 4;
				}
			}
			item = cJSON_GetObjectItem(root, "nm");
			if((NULL != item) && (NULL != item->valuestring) && (0 != strlen(item->valuestring)))
			{
				memcpy_s(nwaddr, 20, item->valuestring, strlen(item->valuestring));
			}
			item = cJSON_GetObjectItem(root, "gw");
			if((NULL != item) && (NULL != item->valuestring) && (0 != strlen(item->valuestring)))
			{
				memcpy_s(hi_wifi_info.gw, 20, item->valuestring, strlen(item->valuestring));
				memcpy_s(gwaddr, 20, item->valuestring, strlen(item->valuestring));
			}
			item = cJSON_GetObjectItem(root, "dns1");
			if((NULL != item) && (NULL != item->valuestring) && (0 != strlen(item->valuestring)))
			{
				memcpy_s(hi_wifi_info.dns, 20, item->valuestring, strlen(item->valuestring));
				memcpy_s(dnsfaddr, 20, item->valuestring, strlen(item->valuestring));
			}
			item = cJSON_GetObjectItem(root, "dns2");
			if ((NULL != item) && (NULL != item->valuestring) && (0 != strlen(item->valuestring)))
			{
				memcpy_s(dnssaddr, 20, item->valuestring, strlen(item->valuestring));
			}

			sample_send_event(msg_data, len);

			if (g_hi_wifi_info.mac_address[0] == 0)
				vlink_hi_channel_set_mac(hi_wifi_info.mac_address);

			pr_info("stats old: %s, new: %s\n", g_hi_wifi_info.wpa_state, hi_wifi_info.wpa_state);
			if (strcmp(g_hi_wifi_info.wpa_state, hi_wifi_info.wpa_state)) {
				sample_wlan_init_ip(ipaddr, nwaddr, gwaddr, dnsfaddr, dnssaddr, 0);

				if (m_wifi_cb) {
					if (!strncmp(hi_wifi_info.wpa_state, "CONNECTED", strlen("CONNECTED"))) {
						m_wifi_cb(RK_WIFI_State_CONNECTED, &hi_wifi_info);
						m_wifi_cb(RK_WIFI_State_DHCP_OK, &hi_wifi_info);
					} else {
						m_wifi_cb(RK_WIFI_State_DISCONNECTED, NULL);
					}
				}
			}
			g_hi_wifi_info = hi_wifi_info;
			sample_wlan_ota_check(g_hi_wifi_info.version);
			break;
		}
		default: //
			break;
		}

	} else {
		sample_log_print("sample_link_rec_cb:parseJson-cmd--Parse fail\n");
		cJSON_Delete(root);
		return;
	}	

	cJSON_Delete(root);
	return;
}
#endif

int hisi_deinit()
{
	if (g_sample_link == NULL)
		return 0;

	if (g_sample_link->sock_thread) {
		pthread_cancel(g_sample_link->sock_thread);
		pthread_join(g_sample_link->sock_thread, HI_NULL);
	}

	pthread_mutex_destroy(&g_sample_link->mut);
	pthread_cond_destroy(&g_sample_link->cond);

	if (g_sample_link->sockfd != -1) {
		close(g_sample_link->sockfd);
	}

	if (g_sample_link->eventfd != -1) {
		close(g_sample_link->eventfd);
	}

	if (g_sample_link != HI_NULL) {
		free(g_sample_link);
		g_sample_link = HI_NULL;
	}
	g_terminate = HI_TRUE;
	return 0;
}

int hisi_main(void)
{
	hi_s32 ret;
	pr_info("enter");

	//set_lo_ipaddr();

	set_lo_ipaddr();
	pr_info("set lo ipaddy \n");
	if (pthread_cond_init(&hi_sync_cond,NULL)!=0 || pthread_mutex_init(&hi_sync_lock,NULL)) {
		sample_log_print("get stat flag init failed \n");
		return -1;
	}
	//signal(SIGINT, sample_terminate);
	//signal(SIGTERM, sample_terminate);
	//signal(SIGPWR, sample_power);
	if (sample_wlan_init_up() != HI_SUCCESS) {
		sample_log_print("sample_wlan_init_up is fail\n");
	}
	pr_info("set wifi init up \n");
	if (hi_channel_init() != HI_SUCCESS) {
		sample_log_print("hi_channel_init is fail!\n");
	}
	else
	{
		sample_log_print("hi_channel_init is ok!\n");
	}
	pr_info("set hichannel init up \n");
	hi_channel_register_rx_cb(sample_link_rec_cb);

	memset(&g_hi_wifi_info, 0, sizeof(RK_WIFI_INFO_Connection_s));
	strcpy(g_hi_wifi_info.wpa_state, "DISCONNECTED");
	vlink_hi_channel_cmd_get_info();

	if (m_wifi_cb) {
		sample_log_print("hi_channel_init open ok\n");
		m_wifi_cb(RK_WIFI_State_OPEN, NULL);
	}

	pr_info("exit");
	return 0;
}
