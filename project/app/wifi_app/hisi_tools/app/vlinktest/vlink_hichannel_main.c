/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: sample cli file.
 * Author: Hisilicon
 * Create: 2020-09-09
 */
/*****************************************************************************
  1 头文件包含
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

/*****************************************************************************
  2 宏定义、全局变量
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
  3 枚举、结构体定义
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
/*****************************************************************************
  4 函数实现
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

    vlink_hi_channel_cmd_reboot();
    usleep(100000);

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
    if (ret == -1) {
        sample_log_print("%s up error\n", SYSTEM_NETDEV_NAME);
        return HI_FAILURE;
    }
    sample_log_print("net device set ip success\n");

//route table
    (hi_void)memset_s(cmd, SYSTEM_CMD_SIZE, 0, SYSTEM_CMD_SIZE);
    (hi_void)snprintf_s(cmd, SYSTEM_CMD_SIZE, SYSTEM_CMD_SIZE - 1, "route del default dev %s", SYSTEM_NETDEV_NAME);
    ret = system(cmd);
    if (ret == -1) {
        sample_log_print("%s error\n", cmd);
    }    

    (hi_void)memset_s(cmd, SYSTEM_CMD_SIZE, 0, SYSTEM_CMD_SIZE);
    (hi_void)snprintf_s(cmd, SYSTEM_CMD_SIZE, SYSTEM_CMD_SIZE - 1, "route add default gw %s", gateway);
    ret = system(cmd);
    if (ret == -1) {
        sample_log_print("%s error\n", cmd);
	    return HI_FAILURE;
    } 
    sample_log_print("net device set route success\n");
//dns
    ret = system("echo -n > /etc/resolv.conf");
    if (ret == -1) {
        sample_log_print("%s error\n", cmd);
		return HI_FAILURE;
    } 

    (hi_void)memset_s(cmd, SYSTEM_CMD_SIZE, 0, SYSTEM_CMD_SIZE);
    (hi_void)snprintf_s(cmd, SYSTEM_CMD_SIZE, SYSTEM_CMD_SIZE - 1, "echo nameserver %s >> /etc/resolv.conf", dnsfirst);
    ret = system(cmd);
    if (ret == -1) {
        sample_log_print("%s error\n", cmd);
		return HI_FAILURE;
    } 

    (hi_void)memset_s(cmd, SYSTEM_CMD_SIZE, 0, SYSTEM_CMD_SIZE);
    (hi_void)snprintf_s(cmd, SYSTEM_CMD_SIZE, SYSTEM_CMD_SIZE - 1, "echo nameserver %s >> /etc/resolv.conf", dnssec);
    ret = system(cmd);
    if (ret == -1) {
        sample_log_print("%s error\n", cmd);
		return HI_FAILURE;
    }
    sample_log_print("net device set dns success\n");
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
    } else if (strncmp(pmsg, "reboot", 6) == 0) {
    vlink_hi_channel_cmd_reboot();
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

hi_s32 vlink_wifi_work_status_ret(hi_char *workstatus)
{
	hi_s32 ret;
/*
    VLINK_WIFI_NETWORK_NETCFG = 0,
    VLINK_WIFI_NETWORK_CONNECTING,
    VLINK_WIFI_NETWORK_CONNECTE_OK,
    VLINK_WIFI_NETWORK_DISCONNECTE,
    VLINK_WIFI_NETWORK_DHCP_OK,
    VLINK_WIFI_NETWORK_DHCP_ERR,	
    VLINK_WIFI_NETWORK_NOAP,
    VLINK_WIFI_NETWORK_PWD_ERR,
    VLINK_WIFI_NETWORK_OTH_ERR,
*/
        sample_log_print("vlink_wifi_work_status_ret======workstatus:%s\n", workstatus);

	switch(atoi(workstatus))
	{
		case 0:
			sample_log_print("vlink_wifi_work_status_ret======net config\n");
		break;
		case 1:
			sample_log_print("vlink_wifi_work_status_ret======net connecting\n");
		break;
		case 2:
			sample_log_print("vlink_wifi_work_status_ret======net connect ok\n");
		break;
		case 3:
			sample_log_print("vlink_wifi_work_status_ret======net disconnect\n");
		break;
		case 4:
			sample_log_print("vlink_wifi_work_status_ret======net dhcp ok\n");
		break;
		case 5:
			sample_log_print("vlink_wifi_work_status_ret======net dhcp error\n");
		break;
		case 6:
			sample_log_print("vlink_wifi_work_status_ret======net no ap\n");
		break;
		case 7:
			sample_log_print("vlink_wifi_work_status_ret======net password error\n");
		break;
		case 8:
			sample_log_print("vlink_wifi_work_status_ret======net other error\n");
		break;
	}

	return ret;
}

hi_s32 sample_send_event(unsigned char *msg_data, int len)
{
    struct sockaddr_in servaddr;

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(SOCK_EVENT_PORT);

    if (sendto(g_sample_link->eventfd, msg_data, len, MSG_DONTWAIT, (const struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
        sample_log_print("sendto error!fd:%d\n", g_sample_link->eventfd);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

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

			break;
			case 2:
			{
				hi_char macaddr[20] = {0};
				item = cJSON_GetObjectItem(root, "mac");
				if((NULL != item) && (NULL != item->valuestring) && (0 != strlen(item->valuestring)))
				{
					vlink_hi_channel_set_mac(item->valuestring);
				}

				sample_send_event(msg_data, len);
			}
			break;
			case 3:
			{
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

				sample_send_event(msg_data, len);
			}
			break;
			case 4:

			break;
			case 5:

			break;
			case 6:

			break;
			case 7:

			break;
			case 8:

			break;
			case 9:

			break;
			case 10:

			break;
			case 11://writeret
				item = cJSON_GetObjectItem(root, "ret");
				if((NULL != item) && (NULL != item->valuestring) && (0 != strlen(item->valuestring)))
				{
					vlink_wifi_ota_process_write_ret(item->valuestring);
				}
			break;
			case 12:

			break;
			case 13:
				item = cJSON_GetObjectItem(root, "ret");
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
			}
			break;
			case 22:
				sample_send_event(msg_data, len);
			break;
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

int main(int argc, char *argv[])
{
    hi_s32 ret;
    sample_unused(argc);
    sample_unused(argv);
    set_lo_ipaddr();

    signal(SIGINT, sample_terminate);
    signal(SIGTERM, sample_terminate);
    signal(SIGPWR, sample_power);

    g_sample_link = (sample_link_s *)malloc(sizeof(sample_link_s));
    if (g_sample_link == HI_NULL) {
        return -1;
    }

    (void)memset_s(g_sample_link, sizeof(sample_link_s), 0, sizeof(sample_link_s));
    pthread_mutex_init(&g_sample_link->mut, HI_NULL);
    pthread_cond_init(&g_sample_link->cond, HI_NULL);

    if (sample_wlan_init_up() != HI_SUCCESS) {
        sample_log_print("sample_wlan_init_up is fail\n");
    }

    if (hi_channel_init() != HI_SUCCESS) {
        sample_log_print("hi_channel_init is fail\n");
    }
    else
    {
        sample_log_print("hi_channel_init is ok\n");
    }

    hi_channel_register_rx_cb(sample_link_rec_cb);
    //vlink_hi_channel_cmd_getmac_info();
    //hi_channel_send_to_dev((hi_u8 *)host_cmd[HOST_CMD_GET_MAC], (hi_s32)strlen(host_cmd[HOST_CMD_GET_MAC]));
    //hi_channel_send_to_dev((hi_u8 *)host_cmd[HOST_CMD_GET_IP], (hi_s32)strlen(host_cmd[HOST_CMD_GET_IP]));

    if (sample_sock_create() != HI_SUCCESS) {
        sample_log_print("create sock is fail\n");
        goto link_out;
    }

    ret = pthread_create(&g_sample_link->sock_thread, HI_NULL, sample_sock_thread, HI_NULL);
    if (ret != HI_SUCCESS) {
        sample_log_print("create sock thread is fail\n");
        goto link_out;
    }

    vlink_hi_channel_cmd_getmac_info();

    /* main loop */
    while (!g_terminate) {
        sample_message_s message;
        pthread_mutex_lock(&g_sample_link->mut);
        while (sample_dequeue(&g_sample_link->cmd_queue, &message) != HI_SUCCESS) {
            pthread_cond_wait(&g_sample_link->cond, &g_sample_link->mut);
        }
        pthread_mutex_unlock(&g_sample_link->mut);
        sample_log_print("=====SAMPLE LOOP RECIEVE MSG-what[%d]:[%s] LEN:%d=====\n", message.what, message.obj, message.len);
        fflush(stdout);

/*
        switch (message.what) {
            case SAMPLE_CMD_HELP:
                sample_usage();
                break;
            case SAMPLE_CMD_EXIT:
                g_terminate = HI_TRUE;
                break;
            case SAMPLE_CMD_IOCTL:
                if (hi_channel_send_to_dev(message.obj, message.len) != HI_SUCCESS) {
                    SAMPLE_LOG_PRINT("sample_iwpriv_cmd send fail\n");
                } else {
                    SAMPLE_LOG_PRINT("sample_iwpriv_cmd send ok\n");
		}
                break;
            default:
                break;
        }
*/

    }
link_out:
    sample_cleanup();
    return 0;
}

