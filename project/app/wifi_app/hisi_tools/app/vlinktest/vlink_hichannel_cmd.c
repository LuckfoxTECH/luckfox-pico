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
            sample_log_print("vlink_hi_channel_cmd_setfilter_info--send fail\n");
        } else {
            sample_log_print("vlink_hi_channel_cmd_setfilter_info--send ok\n");
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
            sample_log_print("vlink_hi_channel_cmd_keeplive_info--send fail\n");
        } else {
            sample_log_print("vlink_hi_channel_cmd_keeplive_info--send ok\n");
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
            sample_log_print("vlink_hi_channel_cmd_deepsleep_info--send fail\n");
        } else {
            sample_log_print("vlink_hi_channel_cmd_deepsleep_info--send ok\n");
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
            sample_log_print("vlink_hi_channel_cmd_getrssi_info--send fail\n");
        } else {
            sample_log_print("vlink_hi_channel_cmd_getrssi_info--send ok\n");
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
            sample_log_print("vlink_hi_channel_cmd_getversion_info--send fail\n");
        } else {
            sample_log_print("vlink_hi_channel_cmd_getversion_info--send ok\n");
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
            sample_log_print("vlink_hi_channel_cmd_getwakecode_info--send fail\n");
        } else {
            sample_log_print("vlink_hi_channel_cmd_getwakecode_info--send ok\n");
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
            sample_log_print("vlink_hi_channel_cmd_factoryreset_info--send fail\n");
        } else {
            sample_log_print("vlink_hi_channel_cmd_factoryreset_info--send ok\n");
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
            sample_log_print("vlink_hi_channel_cmd_getpir_info--send fail\n");
        } else {
            sample_log_print("vlink_hi_channel_cmd_getpir_info--send ok\n");
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

	if (enable == 1) {
		cJSON_AddNumberToObject(pJsonRoot, "enable", 1);
		cJSON_AddNumberToObject(pJsonRoot, "time", 15);
		cJSON_AddNumberToObject(pJsonRoot, "length", 0);
	} else {
		cJSON_AddNumberToObject(pJsonRoot, "enable", 0);
	}

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
            sample_log_print("vlink_hi_channel_cmd_setpir_info--send fail\n");
        } else {
            sample_log_print("vlink_hi_channel_cmd_setpir_info--send ok\n");
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

hi_u32 vlink_hi_channel_cmd_reboot(hi_void)
{
    hi_u32 datalen = 0;
    hi_u32 sendmsg_len = 0;
    //cmd 1-byte, len 2-bytes, data 380
    unsigned char sendcmdbuf[VLINK_SEND_MSG_MAX_LEN] = {0};

    //memset(sendcmdbuf, 0, VLINK_SEND_MSG_MAX_LEN);

    sendcmdbuf[0] = 61;
    //sendcmdbuf[1] = (datalen >> 8) & 0xFF;
    //sendcmdbuf[2] = datalen & 0xFF;

    sendmsg_len = 1;

    sample_log_print("vlink_hi_channel_cmd_reboot-------sendcmdbuf-[%02X]-\n", sendcmdbuf[0]);

    if (hi_channel_send_to_dev(sendcmdbuf, sendmsg_len) != HI_SUCCESS) {
        sample_log_print("vlink_hi_channel_cmd_reboot--send fail\n");
    } else {
        sample_log_print("vlink_hi_channel_cmd_reboot--send ok\n");
    }

    return 0;
}
