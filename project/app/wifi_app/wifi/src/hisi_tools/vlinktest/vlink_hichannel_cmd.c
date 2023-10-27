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

#define VLINK_SEND_MSG_MAX_LEN		(SYSTEM_CMD_SIZE - 4)
#define VLINK_SEND_MSG_HEAD_LEN		3

hi_u32 hi_ota_sync_flag = 0;
hi_bool ota_trans_ret = HI_TRUE;
pthread_cond_t hi_ota_sync_cond;
pthread_mutex_t hi_ota_sync_lock;

hi_u32 vlink_hi_channel_cmd_netcfg_info(hi_char* ssid, hi_char* key, hi_char* sendmsg, hi_u32 *sendmsg_len)
{
	cJSON * pJsonRoot = NULL;
	cJSON *pJson = NULL;
	hi_u32 datalen = 0;
	//cmd 1-byte, len 2-bytes, data 380
	unsigned char sendcmdbuf[VLINK_SEND_MSG_MAX_LEN] = {0};

	pJsonRoot = cJSON_CreateObject();

	//cJSON_AddStringToObject(pJsonRoot, "cmd", CMD_SENDMSG_NETCFG);
	cJSON_AddStringToObject(pJsonRoot, "ssid", ssid);
	cJSON_AddStringToObject(pJsonRoot, "key", key);

	pJson = cJSON_Print(pJsonRoot);

	sample_log_print("vlink_hi_channel_netcfg_info-----------pJson[%s]-len[%d]--\n", pJson, strlen(pJson));
	datalen = strlen(pJson);

	*sendmsg_len = datalen + VLINK_SEND_MSG_HEAD_LEN;

	//memset(sendcmdbuf, 0, VLINK_SEND_MSG_MAX_LEN);

	sendcmdbuf[0] = CMD_SENDMSG_NETCFG;
	sendcmdbuf[1] = (datalen >> 8) & 0xFF;
	sendcmdbuf[2] = datalen & 0xFF;	

	sample_log_print("vlink_hi_channel_netcfg_info-------sendcmdbuf-[%02X]-[%02X]-[%02X]-\n", sendcmdbuf[0], sendcmdbuf[1], sendcmdbuf[2]);

	memcpy_s(&sendcmdbuf[VLINK_SEND_MSG_HEAD_LEN], datalen, pJson, datalen);
	free(pJson);
	cJSON_Delete(pJsonRoot);

	memcpy_s(sendmsg, *sendmsg_len, sendcmdbuf, *sendmsg_len);

	return 0;
}

hi_u32 vlink_hi_channel_cmd_getmac_info(hi_void)
{
	hi_u32 datalen = 0;
	hi_u32 sendmsg_len = 0;
	//cmd 1-byte, len 2-bytes, data 380
	unsigned char sendcmdbuf[VLINK_SEND_MSG_MAX_LEN] = {0};

	//memset(sendcmdbuf, 0, VLINK_SEND_MSG_MAX_LEN);

	sendcmdbuf[0] = CMD_SENDMSG_GETMAC;
	sendcmdbuf[1] = (datalen >> 8) & 0xFF;
	sendcmdbuf[2] = datalen & 0xFF;	

	sendmsg_len = datalen + VLINK_SEND_MSG_HEAD_LEN;

	sample_log_print("vlink_hi_channel_cmd_getmac_info-------sendcmdbuf-[%02X]-[%02X]-[%02X]-\n", sendcmdbuf[0], sendcmdbuf[1], sendcmdbuf[2]);

	if (hi_channel_send_to_dev(sendcmdbuf, sendmsg_len) != HI_SUCCESS) {
		sample_log_print("vlink_hi_channel_cmd_getmac_info--send fail\n");
	} else {
		sample_log_print("vlink_hi_channel_cmd_getmac_info--send ok\n");
	}

	return 0;
}

hi_u32 vlink_hi_channel_cmd_get_info(hi_void)
{
	hi_u32 datalen = 0;
	hi_u32 sendmsg_len = 0;
	//cmd 1-byte, len 2-bytes, data 380
	unsigned char sendcmdbuf[VLINK_SEND_MSG_MAX_LEN] = {0};

	//memset(sendcmdbuf, 0, VLINK_SEND_MSG_MAX_LEN);

	sendcmdbuf[0] = CMD_SENDMSG_GET_INFO;
	sendcmdbuf[1] = (datalen >> 8) & 0xFF;
	sendcmdbuf[2] = datalen & 0xFF;	

	sendmsg_len = datalen + VLINK_SEND_MSG_HEAD_LEN;

	sample_log_print("vlink_hi_channel_cmd_get_info-------sendcmdbuf-[%02X]-[%02X]-[%02X]-\n", sendcmdbuf[0], sendcmdbuf[1], sendcmdbuf[2]);

	if (hi_channel_send_to_dev(sendcmdbuf, sendmsg_len) != HI_SUCCESS) {
		pr_info("vlink_hi_channel_cmd_get_info--send fail\n");
	} else {
		pr_info("vlink_hi_channel_cmd_get_info--send ok\n");
	}

	return 0;
}

hi_u32 vlink_hi_channel_cmd_getip_info(hi_void)
{
	hi_u32 datalen = 0;
	hi_u32 sendmsg_len = 0;
	//cmd 1-byte, len 2-bytes, data 380
	unsigned char sendcmdbuf[VLINK_SEND_MSG_MAX_LEN] = {0};

	//memset(sendcmdbuf, 0, VLINK_SEND_MSG_MAX_LEN);

	sendcmdbuf[0] = CMD_SENDMSG_GETIP;
	sendcmdbuf[1] = (datalen >> 8) & 0xFF;
	sendcmdbuf[2] = datalen & 0xFF;	

	sendmsg_len = datalen + VLINK_SEND_MSG_HEAD_LEN;

	sample_log_print("vlink_hi_channel_cmd_getip_info-------sendcmdbuf-[%02X]-[%02X]-[%02X]-\n", sendcmdbuf[0], sendcmdbuf[1], sendcmdbuf[2]);

        if (hi_channel_send_to_dev(sendcmdbuf, sendmsg_len) != HI_SUCCESS) {
            sample_log_print("vlink_hi_channel_cmd_getip_info--send fail\n");
        } else {
            sample_log_print("vlink_hi_channel_cmd_getip_info--send ok\n");
	}

	return 0;
}

hi_u32 vlink_hi_channel_cmd_setfilter_info(hi_char* device)
{
	hi_u32 datalen = 0;
	hi_u32 sendmsg_len = 0;
	cJSON * pJsonRoot = NULL;
	cJSON *pJson = NULL;
	//cmd 1-byte, len 2-bytes, data 380
	unsigned char sendcmdbuf[VLINK_SEND_MSG_MAX_LEN] = {0};

	pJsonRoot = cJSON_CreateObject();

	cJSON_AddStringToObject(pJsonRoot, "device", device);
	pJson = cJSON_Print(pJsonRoot);

	sample_log_print("vlink_hi_channel_cmd_setfilter_info-------pJson[%s]---\n", pJson);

	datalen = strlen(pJson);

	sendmsg_len = datalen + VLINK_SEND_MSG_HEAD_LEN;

	//memset(sendcmdbuf, 0, VLINK_SEND_MSG_MAX_LEN);

	sendcmdbuf[0] = CMD_SENDMSG_SETFILTER;
	sendcmdbuf[1] = (datalen >> 8) & 0xFF;
	sendcmdbuf[2] = datalen & 0xFF;	

	sample_log_print("vlink_hi_channel_cmd_setfilter_info-------sendcmdbuf-[%02X]-[%02X]-[%02X]-\n", sendcmdbuf[0], sendcmdbuf[1], sendcmdbuf[2]);

	memcpy_s(&sendcmdbuf[VLINK_SEND_MSG_HEAD_LEN], datalen, pJson, datalen);
	free(pJson);
	cJSON_Delete(pJsonRoot);

	//memcpy_s(sendmsg, sendmsg_len, sendcmdbuf, sendmsg_len);

        if (hi_channel_send_to_dev(sendcmdbuf, sendmsg_len) != HI_SUCCESS) {
            pr_info("vlink_hi_channel_cmd_setfilter_info--send fail\n");
        } else {
            pr_info("vlink_hi_channel_cmd_setfilter_info--send ok\n");
	}
	return 0;
}

hi_u32 vlink_hi_channel_cmd_keeplive_info(hi_char* serverip, hi_char* port, hi_char* expire)
{
	hi_u32 datalen = 0;
	hi_u32 sendmsg_len = 0;
	cJSON * pJsonRoot = NULL;
	cJSON *pJson = NULL;
	//cmd 1-byte, len 2-bytes, data 380
	unsigned char sendcmdbuf[VLINK_SEND_MSG_MAX_LEN] = {0};

	pJsonRoot = cJSON_CreateObject();

	cJSON_AddStringToObject(pJsonRoot, "ip", serverip);
	cJSON_AddStringToObject(pJsonRoot, "port", port);
	cJSON_AddStringToObject(pJsonRoot, "expire", expire);
	pJson = cJSON_Print(pJsonRoot);

	sample_log_print("vlink_hi_channel_cmd_keeplive_info-------pJson[%s]---\n", pJson);

	datalen = strlen(pJson);

	sendmsg_len = datalen + VLINK_SEND_MSG_HEAD_LEN;

	//memset(sendcmdbuf, 0, VLINK_SEND_MSG_MAX_LEN);

	sendcmdbuf[0] = CMD_SENDMSG_KEEPLIVE;
	sendcmdbuf[1] = (datalen >> 8) & 0xFF;
	sendcmdbuf[2] = datalen & 0xFF;	

	sample_log_print("vlink_hi_channel_cmd_keeplive_info-------sendcmdbuf-[%02X]-[%02X]-[%02X]-\n", sendcmdbuf[0], sendcmdbuf[1], sendcmdbuf[2]);

	memcpy_s(&sendcmdbuf[VLINK_SEND_MSG_HEAD_LEN], datalen, pJson, datalen);
	free(pJson);
	cJSON_Delete(pJsonRoot);

	//memcpy_s(sendmsg, sendmsg_len, sendcmdbuf, sendmsg_len);

        if (hi_channel_send_to_dev(sendcmdbuf, sendmsg_len) != HI_SUCCESS) {
            pr_info("vlink_hi_channel_cmd_keeplive_info--send fail\n");
        } else {
            pr_info("vlink_hi_channel_cmd_keeplive_info--send ok\n");
	}

	return 0;
}

hi_u32 vlink_hi_channel_cmd_reboot(hi_void)
{
    hi_u32 datalen = 0;
    hi_u32 sendmsg_len = 0;
    //cmd 1-byte, len 2-bytes, data 380
    unsigned char sendcmdbuf[VLINK_SEND_MSG_MAX_LEN] = {0};

    sendcmdbuf[0] = 61;
    sendmsg_len = 1;

    sample_log_print("vlink_hi_channel_cmd_reboot-------sendcmdbuf-[%02X]-\n", sendcmdbuf[0]);

    if (hi_channel_send_to_dev(sendcmdbuf, sendmsg_len) != HI_SUCCESS) {
        pr_info("vlink_hi_channel_cmd_reboot--send fail\n");
    } else {
        pr_info("vlink_hi_channel_cmd_reboot--send ok\n");
    }

    return 0;
}

hi_u32 vlink_hi_channel_cmd_sta_disconnect(hi_void)
{
    hi_u32 datalen = 0;
    hi_u32 sendmsg_len = 0;
    //cmd 1-byte, len 2-bytes, data 380
    unsigned char sendcmdbuf[VLINK_SEND_MSG_MAX_LEN] = {0};

    sendcmdbuf[0] = 20;
    sendmsg_len = 1;

    sample_log_print("vlink_hi_channel_cmd_sta_disconnect-------sendcmdbuf-[%02X]-\n", sendcmdbuf[0]);

    if (hi_channel_send_to_dev(sendcmdbuf, sendmsg_len) != HI_SUCCESS) {
        pr_info("vlink_hi_channel_cmd_sta_disconnect--send fail\n");
    } else {
        pr_info("vlink_hi_channel_cmd_sta_disconnect--send ok\n");
    }

    return 0;
}

hi_u32 vlink_hi_channel_cmd_deepsleep_info(hi_void)
{
	hi_u32 datalen = 0;
	hi_u32 sendmsg_len = 0;
	//cmd 1-byte, len 2-bytes, data 380
	unsigned char sendcmdbuf[VLINK_SEND_MSG_MAX_LEN] = {0};

	//memset(sendcmdbuf, 0, VLINK_SEND_MSG_MAX_LEN);

	sendcmdbuf[0] = CMD_SENDMSG_DEEPSLEEP;
	sendcmdbuf[1] = (datalen >> 8) & 0xFF;
	sendcmdbuf[2] = datalen & 0xFF;	
	sendmsg_len = datalen + VLINK_SEND_MSG_HEAD_LEN;

	sample_log_print("vlink_hi_channel_cmd_deepsleep_info-------sendcmdbuf-[%02X]-[%02X]-[%02X]-\n", sendcmdbuf[0], sendcmdbuf[1], sendcmdbuf[2]);

        if (hi_channel_send_to_dev(sendcmdbuf, sendmsg_len) != HI_SUCCESS) {
            pr_info("vlink_hi_channel_cmd_deepsleep_info--send fail\n");
        } else {
            pr_info("vlink_hi_channel_cmd_deepsleep_info--send ok\n");
	}

	return 0;
}

hi_u32 vlink_hi_channel_cmd_startap_info(hi_void)
{
	hi_u32 datalen = 0;
	hi_u32 sendmsg_len = 0;
	//cmd 1-byte, len 2-bytes, data 380
	unsigned char sendcmdbuf[VLINK_SEND_MSG_MAX_LEN] = {0};

	//memset(sendcmdbuf, 0, VLINK_SEND_MSG_MAX_LEN);

	sendcmdbuf[0] = CMD_SENDMSG_STARTAP;
	sendcmdbuf[1] = (datalen >> 8) & 0xFF;
	sendcmdbuf[2] = datalen & 0xFF;	
	sendmsg_len = datalen + VLINK_SEND_MSG_HEAD_LEN;

	sample_log_print("vlink_hi_channel_cmd_startap_info-------sendcmdbuf-[%02X]-[%02X]-[%02X]-\n", sendcmdbuf[0], sendcmdbuf[1], sendcmdbuf[2]);

        if (hi_channel_send_to_dev(sendcmdbuf, sendmsg_len) != HI_SUCCESS) {
            sample_log_print("vlink_hi_channel_cmd_startap_info--send fail\n");
        } else {
            sample_log_print("vlink_hi_channel_cmd_startap_info--send ok\n");
	}

	return 0;
}

hi_u32 vlink_hi_channel_cmd_startota(hi_void)
{
	hi_u32 datalen = 0;
	hi_u32 sendmsg_len = 0;
	//cmd 1-byte, len 2-bytes, data 380
	unsigned char sendcmdbuf[VLINK_SEND_MSG_MAX_LEN] = {0};

	//memset(sendcmdbuf, 0, VLINK_SEND_MSG_MAX_LEN);
	vlink_wifi_ota_start_info();

	sendcmdbuf[0] = CMD_SENDMSG_STARTOTA;
	sendcmdbuf[1] = (datalen >> 8) & 0xFF;
	sendcmdbuf[2] = datalen & 0xFF;	
	sendmsg_len = datalen + VLINK_SEND_MSG_HEAD_LEN;

	sample_log_print("vlink_hi_channel_cmd_startota-------sendcmdbuf-[%02X]-[%02X]-[%02X]-\n", sendcmdbuf[0], sendcmdbuf[1], sendcmdbuf[2]);

	sleep(2);

        if (hi_channel_send_to_dev(sendcmdbuf, sendmsg_len) != HI_SUCCESS) {
            sample_log_print("vlink_hi_channel_cmd_startota--send fail\n");
        } else {
            sample_log_print("vlink_hi_channel_cmd_startota--send ok\n");
	}

	return 0;
}


hi_u32 vlink_hi_channel_cmd_getrssi_info(hi_void)
{

	hi_u32 datalen = 0;
	hi_u32 sendmsg_len = 0;
	//cmd 1-byte, len 2-bytes, data 380
	unsigned char sendcmdbuf[VLINK_SEND_MSG_MAX_LEN] = {0};

	//memset(sendcmdbuf, 0, VLINK_SEND_MSG_MAX_LEN);

	sendcmdbuf[0] = CMD_SENDMSG_GET_RSSI;
	sendcmdbuf[1] = (datalen >> 8) & 0xFF;
	sendcmdbuf[2] = datalen & 0xFF;	

	sendmsg_len = datalen + VLINK_SEND_MSG_HEAD_LEN;

	sample_log_print("vlink_hi_channel_cmd_getrssi_info-------sendcmdbuf-[%02X]-[%02X]-[%02X]-\n", sendcmdbuf[0], sendcmdbuf[1], sendcmdbuf[2]);

        if (hi_channel_send_to_dev(sendcmdbuf, sendmsg_len) != HI_SUCCESS) {
            pr_info("vlink_hi_channel_cmd_getrssi_info--send fail\n");
        } else {
            pr_info("vlink_hi_channel_cmd_getrssi_info--send ok\n");
	}
	return 0;
}

hi_u32 vlink_hi_channel_cmd_getversion_info(hi_void)
{

	hi_u32 datalen = 0;
	hi_u32 sendmsg_len = 0;
	//cmd 1-byte, len 2-bytes, data 380
	unsigned char sendcmdbuf[VLINK_SEND_MSG_MAX_LEN] = {0};

	sendcmdbuf[0] = CMD_SENDMSG_GET_VERSION;
	sendcmdbuf[1] = (datalen >> 8) & 0xFF;
	sendcmdbuf[2] = datalen & 0xFF;	

	sendmsg_len = datalen + VLINK_SEND_MSG_HEAD_LEN;

	sample_log_print("vlink_hi_channel_cmd_getversion_info-------sendcmdbuf-[%02X]-[%02X]-[%02X]-\n", sendcmdbuf[0], sendcmdbuf[1], sendcmdbuf[2]);

        if (hi_channel_send_to_dev(sendcmdbuf, sendmsg_len) != HI_SUCCESS) {
            pr_info("vlink_hi_channel_cmd_getversion_info--send fail\n");
        } else {
            pr_info("vlink_hi_channel_cmd_getversion_info--send ok\n");
	}
	return 0;
}

hi_u32 vlink_hi_channel_cmd_getwakecode_info(hi_void)
{

	hi_u32 datalen = 0;
	hi_u32 sendmsg_len = 0;
	//cmd 1-byte, len 2-bytes, data 380
	unsigned char sendcmdbuf[VLINK_SEND_MSG_MAX_LEN] = {0};

	sendcmdbuf[0] = CMD_SENDMSG_GET_WAKECODE;
	sendcmdbuf[1] = (datalen >> 8) & 0xFF;
	sendcmdbuf[2] = datalen & 0xFF;	

	sendmsg_len = datalen + VLINK_SEND_MSG_HEAD_LEN;

	sample_log_print("vlink_hi_channel_cmd_getwakecode_info-------sendcmdbuf-[%02X]-[%02X]-[%02X]-\n", sendcmdbuf[0], sendcmdbuf[1], sendcmdbuf[2]);

        if (hi_channel_send_to_dev(sendcmdbuf, sendmsg_len) != HI_SUCCESS) {
            pr_info("vlink_hi_channel_cmd_getwakecode_info--send fail\n");
        } else {
            pr_info("vlink_hi_channel_cmd_getwakecode_info--send ok\n");
	}
	return 0;
}

hi_u32 vlink_hi_channel_cmd_factoryreset_info(hi_void)
{

	hi_u32 datalen = 0;
	hi_u32 sendmsg_len = 0;
	//cmd 1-byte, len 2-bytes, data 380
	unsigned char sendcmdbuf[VLINK_SEND_MSG_MAX_LEN] = {0};

	sendcmdbuf[0] = CMD_SENDMSG_FACTORY_RESET;
	sendcmdbuf[1] = (datalen >> 8) & 0xFF;
	sendcmdbuf[2] = datalen & 0xFF;	

	sendmsg_len = datalen + VLINK_SEND_MSG_HEAD_LEN;

	sample_log_print("vlink_hi_channel_cmd_factoryreset_info-------sendcmdbuf-[%02X]-[%02X]-[%02X]-\n", sendcmdbuf[0], sendcmdbuf[1], sendcmdbuf[2]);

        if (hi_channel_send_to_dev(sendcmdbuf, sendmsg_len) != HI_SUCCESS) {
            pr_info("vlink_hi_channel_cmd_factoryreset_info--send fail\n");
        } else {
            pr_info("vlink_hi_channel_cmd_factoryreset_info--send ok\n");
	}
	return 0;
}

hi_u32 vlink_hi_channel_cmd_getpir_info(hi_void)
{

	hi_u32 datalen = 0;
	hi_u32 sendmsg_len = 0;
	//cmd 1-byte, len 2-bytes, data 380
	unsigned char sendcmdbuf[VLINK_SEND_MSG_MAX_LEN] = {0};

	sendcmdbuf[0] = CMD_SENDMSG_PIR_GET;
	sendcmdbuf[1] = (datalen >> 8) & 0xFF;
	sendcmdbuf[2] = datalen & 0xFF;	

	sendmsg_len = datalen + VLINK_SEND_MSG_HEAD_LEN;

	sample_log_print("vlink_hi_channel_cmd_getpir_info-------sendcmdbuf-[%02X]-[%02X]-[%02X]-\n", sendcmdbuf[0], sendcmdbuf[1], sendcmdbuf[2]);

        if (hi_channel_send_to_dev(sendcmdbuf, sendmsg_len) != HI_SUCCESS) {
            pr_info("vlink_hi_channel_cmd_getpir_info--send fail\n");
        } else {
            pr_info("vlink_hi_channel_cmd_getpir_info--send ok\n");
	}
	return 0;
}

hi_u32 vlink_hi_channel_cmd_setpir_info(hi_u8 enable)
{

	hi_u32 datalen = 0;
	hi_u32 sendmsg_len = 0;
	cJSON * pJsonRoot = NULL;
	cJSON *pJson = NULL;
	//cmd 1-byte, len 2-bytes, data 380
	unsigned char sendcmdbuf[VLINK_SEND_MSG_MAX_LEN] = {0};

	pJsonRoot = cJSON_CreateObject();

	if (enable == 1)
		cJSON_AddNumberToObject(pJsonRoot, "enable", 1);
	else
		cJSON_AddNumberToObject(pJsonRoot, "enable", 0);
	cJSON_AddNumberToObject(pJsonRoot, "time", 15);
	cJSON_AddNumberToObject(pJsonRoot, "length", 0);

	pJson = cJSON_Print(pJsonRoot);

	sample_log_print("vlink_hi_channel_cmd_setpir_info-------pJson[%s]---\n", pJson);

	datalen = strlen(pJson);

	sendmsg_len = datalen + VLINK_SEND_MSG_HEAD_LEN;

	//memset(sendcmdbuf, 0, VLINK_SEND_MSG_MAX_LEN);

	sendcmdbuf[0] = CMD_SENDMSG_PIR_SET;
	sendcmdbuf[1] = (datalen >> 8) & 0xFF;
	sendcmdbuf[2] = datalen & 0xFF;	

	sample_log_print("vlink_hi_channel_cmd_setpir_info-------sendcmdbuf-[%02X]-[%02X]-[%02X]-\n", sendcmdbuf[0], sendcmdbuf[1], sendcmdbuf[2]);

	memcpy_s(&sendcmdbuf[VLINK_SEND_MSG_HEAD_LEN], datalen, pJson, datalen);
	free(pJson);
	cJSON_Delete(pJsonRoot);

	//memcpy_s(sendmsg, sendmsg_len, sendcmdbuf, sendmsg_len);

        if (hi_channel_send_to_dev(sendcmdbuf, sendmsg_len) != HI_SUCCESS) {
            pr_info("vlink_hi_channel_cmd_setpir_info--send fail\n");
        } else {
            pr_info("vlink_hi_channel_cmd_setpir_info--send ok\n");
	}
	return 0;
}


hi_u32 vlink_hi_channel_cmd_settuya_info(hi_void)
{
#define TUYA_IP			"42.192.35.108"
#define TUYA_PORT		443

#define TUYA_DID		"6c1d4426c02c8f4753umwk"
#define TUYA_KEY		"19d72549f0a336ce"

	hi_u32 datalen = 0;
	hi_u32 sendmsg_len = 0;
	cJSON * pJsonRoot = NULL;
	cJSON *pJson = NULL;
	//cmd 1-byte, len 2-bytes, data 380
	unsigned char sendcmdbuf[VLINK_SEND_MSG_MAX_LEN] = {0};

	pJsonRoot = cJSON_CreateObject();

	cJSON_AddStringToObject(pJsonRoot, "tyip", TUYA_IP);
	cJSON_AddNumberToObject(pJsonRoot, "typort", TUYA_PORT);
	cJSON_AddStringToObject(pJsonRoot, "tydid", TUYA_DID);
	cJSON_AddStringToObject(pJsonRoot, "tykey", TUYA_KEY);

	pJson = cJSON_Print(pJsonRoot);

	sample_log_print("vlink_hi_channel_cmd_setpir_info-------pJson[%s]---\n", pJson);

	datalen = strlen(pJson);

	sendmsg_len = datalen + VLINK_SEND_MSG_HEAD_LEN;

	//memset(sendcmdbuf, 0, VLINK_SEND_MSG_MAX_LEN);

	sendcmdbuf[0] = CMD_SENDMSG_TUYA_SET;
	sendcmdbuf[1] = (datalen >> 8) & 0xFF;
	sendcmdbuf[2] = datalen & 0xFF;	

	sample_log_print("vlink_hi_channel_cmd_setpir_info-------sendcmdbuf-[%02X]-[%02X]-[%02X]-\n", sendcmdbuf[0], sendcmdbuf[1], sendcmdbuf[2]);

	memcpy_s(&sendcmdbuf[VLINK_SEND_MSG_HEAD_LEN], datalen, pJson, datalen);
	free(pJson);
	cJSON_Delete(pJsonRoot);

        if (hi_channel_send_to_dev(sendcmdbuf, sendmsg_len) != HI_SUCCESS) {
            sample_log_print("vlink_hi_channel_cmd_setpir_info--send fail\n");
        } else {
            sample_log_print("vlink_hi_channel_cmd_setpir_info--send ok\n");
	}
	return 0;
}

hi_u32 vlink_hi_channel_cmd_getall_info(hi_void)
{
    hi_u32 datalen = 0;
    hi_u32 sendmsg_len = 0;
    //cmd 1-byte, len 2-bytes, data 380
    unsigned char sendcmdbuf[VLINK_SEND_MSG_MAX_LEN] = {0};

    //memset(sendcmdbuf, 0, VLINK_SEND_MSG_MAX_LEN);

    sendcmdbuf[0] = CMD_SENDMSG_GETALL;
    sendcmdbuf[1] = (datalen >> 8) & 0xFF;
    sendcmdbuf[2] = datalen & 0xFF;

    sendmsg_len = datalen + VLINK_SEND_MSG_HEAD_LEN;

    sample_log_print("vlink_hi_channel_cmd_getall_info-------sendcmdbuf-[%02X]-[%02X]-[%02X]-\n", sendcmdbuf[0], sendcmdbuf[1], sendcmdbuf[2]);

    if (hi_channel_send_to_dev(sendcmdbuf, sendmsg_len) != HI_SUCCESS) {
        sample_log_print("vlink_hi_channel_cmd_getall_info--send fail\n");
    } else {
        sample_log_print("vlink_hi_channel_cmd_getall_info--send ok\n");
    }

    return 0;
}

hi_u32 vlink_hi_channel_cmd_clrpir_info(hi_void)
{
    hi_u32 datalen = 0;
    hi_u32 sendmsg_len = 0;
    //cmd 1-byte, len 2-bytes, data 380
    unsigned char sendcmdbuf[VLINK_SEND_MSG_MAX_LEN] = {0};

    //memset(sendcmdbuf, 0, VLINK_SEND_MSG_MAX_LEN);

    sendcmdbuf[0] = CMD_SENDMSG_PIR_CLR;
    sendcmdbuf[1] = (datalen >> 8) & 0xFF;
    sendcmdbuf[2] = datalen & 0xFF;

    sendmsg_len = datalen + VLINK_SEND_MSG_HEAD_LEN;

    sample_log_print("vlink_hi_channel_cmd_clrpir_info-------sendcmdbuf-[%02X]-[%02X]-[%02X]-\n", sendcmdbuf[0], sendcmdbuf[1], sendcmdbuf[2]);

    if (hi_channel_send_to_dev(sendcmdbuf, sendmsg_len) != HI_SUCCESS) {
        sample_log_print("vlink_hi_channel_cmd_clrpir_info--send fail\n");
    } else {
        sample_log_print("vlink_hi_channel_cmd_clrpir_info--send ok\n");
    }

    return 0;
}

hi_u32 vlink_hi_channel_cmd_ota(hi_char * path)
{
	pr_info("start ota ... ... \n");
	FILE *fp = NULL;
	unsigned char * sendbuf;
	hi_u32 datalen = 0;
	hi_u32 times = 0;
	if (pthread_cond_init(&hi_ota_sync_cond,NULL)!=0 || pthread_mutex_init(&hi_ota_sync_lock,NULL)) {
		sample_log_print("get ota sync mutex or condition init failed \n");
		return 0;
	}
	sendbuf = malloc(MAX_SEND_DATA_SIZE);
	if (sendbuf == NULL) {
		printf("failed to alloc mem for ota \n");
		free(sendbuf);
		return 0;
	}
	fp = fopen(path,"r");
	if ( fp == NULL ) {
		sample_log_print("failed to open %s \n",path);
		free(sendbuf);
		return 0;
	}
	sendbuf[0] = CMD_SENDMSG_OTADATA;
	datalen=fread(&sendbuf[3],1,MAX_SEND_DATA_SIZE-100,fp);
	sample_log_print("read datalen is %d \n",datalen);
	sendbuf[1] = (datalen >> 8) & 0xFF;
	sendbuf[2] = datalen & 0xFF;
	if (hi_channel_send_to_dev(sendbuf, datalen+3) != HI_SUCCESS) {
		sample_log_print("vlink_hi_channel_cmd_ota--send fail\n");
	} else {
		sample_log_print("vlink_hi_channel_cmd_ota--send ok\n");
	}

	while (!feof(fp)) {
		if ( ota_trans_ret == HI_FAILURE ) {
			sample_log_print("ota failed ! \n");
			break;
		}
		memset(sendbuf,0,MAX_SEND_DATA_SIZE);
		datalen=fread(&sendbuf[3],1,MAX_SEND_DATA_SIZE-100,fp);
		sample_log_print("time is %d read datalen is %d \n",times,datalen);
		sendbuf[0] = CMD_SENDMSG_OTAFW;
		sendbuf[1] = (datalen >> 8) & 0xFF;
		sendbuf[2] = datalen & 0xFF;
		times++;
		if (hi_channel_send_to_dev(sendbuf, datalen+3) != HI_SUCCESS) {
			sample_log_print("vlink_hi_channel_cmd_ota--send fail\n");
			ota_trans_ret = HI_FAILURE;
			break;
		}
		while (hi_ota_sync_flag==0){
			pthread_cond_wait(&hi_ota_sync_cond,&hi_ota_sync_lock);
		}
		pthread_mutex_unlock(&hi_ota_sync_lock);
		hi_ota_sync_flag = 0;
	}
	if (ota_trans_ret == HI_TRUE) {
		memset(sendbuf,0,MAX_SEND_DATA_SIZE);
		sendbuf[0] = CMD_SENDMSG_OTAFWOK;
		sendbuf[1] = 0;
		sendbuf[2] = 0;
		if (hi_channel_send_to_dev(sendbuf, 3) != HI_SUCCESS) {
			sample_log_print("vlink_hi_channel_cmd_otafwok_info--send fail\n");
		} else {
			sample_log_print("vlink_hi_channel_cmd_otafwok_info--send ok\n");
		}
	}
	free(sendbuf);
	sleep(10);//wait for wifi ota process
	return 1;

}
