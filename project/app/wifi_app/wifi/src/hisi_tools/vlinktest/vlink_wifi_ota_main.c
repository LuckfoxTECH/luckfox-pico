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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
 
#include <sys/un.h>  

#define SOCKET_DOMAIN "/tmp/wifiota.domain" 
#define VLINK_WIFI_OTA_PATH "/mnt/T31ZL/030/Hi3861L_demo_ota.bin"

#define MAXLINE 4096
#define TCP_PORT 20000



static int create_wifi_ota_server_socket(void)  
{  
	int listen_fd;   
	int ret;  

	struct sockaddr_un srv_addr;  
	listen_fd = socket(PF_LOCAL, SOCK_DGRAM, 0);  
	if(listen_fd < 0)  
	{  
		printf("cannot create communication socket\n");  
		return -1;  
	}   
	//set server addr_param  
	srv_addr.sun_family = AF_LOCAL;  
	strncpy(srv_addr.sun_path, SOCKET_DOMAIN, sizeof(srv_addr.sun_path) - 1);  
	unlink(SOCKET_DOMAIN);  
	//bind sockfd & addr  
	ret = bind(listen_fd, (struct sockaddr*)&srv_addr, sizeof(srv_addr));  
	if(ret == -1)  
	{  
		printf("cannot bind server socket\n");  
		close(listen_fd);  
		unlink(SOCKET_DOMAIN);  
		return -1;  
	}  

	return listen_fd;
} 


void vlink_wifi_ota_process_write_ret(char* result)
{
#define SENDSTRING "OK"
	int ret = atoi(result);	

	printf("vlink_wifi_ota_process_write_ret-[%d]-\n", ret); 
	if (ret == 0)
	{
		int fd = -1;
		struct sockaddr_un srv_addr;

		//create socket
		fd = socket(PF_LOCAL, SOCK_DGRAM, 0);
		if (fd < 0)
		{
			printf("cannot bind server socket\n"); 
			return;
		}

		srv_addr.sun_family = AF_LOCAL;  
		strcpy(srv_addr.sun_path, SOCKET_DOMAIN); 
		sendto(fd, SENDSTRING, strlen(SENDSTRING), 0, (struct sockaddr *)&srv_addr, sizeof(struct sockaddr_un));
	}

}


int getIndexOfSigns(char ch)
{
    if(ch >= '0' && ch <= '9')
    {
        return ch - '0';
    }
    if(ch >= 'A' && ch <='F') 
    {
        return ch - 'A' + 10;
    }
    if(ch >= 'a' && ch <= 'f')
    {
        return ch - 'a' + 10;
    }
    return -1;
}

int hexToDec(char *source)
{
    int sum = 0;
    int t = 1;
    int i, len;
 
    len = strlen(source);
    for(i=len-1; i>=0; i--)
    {
        sum += t * getIndexOfSigns(*(source + i));
        t *= 16;
    }  
 
    return sum;
}

void* vlink_wifi_ota_thread(void* arg)
{
#define BUFFSIZE 1450
	int listen_fd = create_wifi_ota_server_socket();  
	char recv_buf[5];  
	hi_uchar read_buf[BUFFSIZE]; 
	hi_uchar send_buf[SAMPLE_CMD_MAX_LEN]; 
	int send_len = 0;
	int file_end = 0;
	FILE *fpRead = NULL;
 
	printf("vlink_wifi_ota_thread------start-----------\n"); 
	if(listen_fd < 0)
	{
		printf("vlink_wifi_ota_thread--cannot creatServerSocket\n");
		unlink(SOCKET_DOMAIN);
		return NULL;  
	}   
	fpRead = fopen(VLINK_WIFI_OTA_PATH, "rb");
	fseek(fpRead, 0, SEEK_SET);
 
	while (1)
	{
		int num = 0; 
		memset(recv_buf, 0, 5);  
		num = recvfrom (listen_fd, recv_buf, 5, 0, NULL, NULL); 
		if (num > 0)
		{
			recv_buf[num] = '\0';
			if (strcmp(recv_buf, "OK") == 0)
			{
				memset(send_buf, 0, SAMPLE_CMD_MAX_LEN);
				send_buf[0] = CMD_SENDMSG_OTADATA;


				if (file_end == 0)
				{
					bzero (read_buf, BUFFSIZE);
					int count = fread(read_buf, sizeof (char), BUFFSIZE, fpRead);
					file_end = feof(fpRead);
					printf ("%d, %d\n", count, file_end);	
					send_len = count + 3;	
					memcpy_s(&send_buf[3], count, read_buf, count);	
				} else {
					send_len = 3;	
				}

				send_buf[1] = (send_len >> 8) & 0xFF;
				send_buf[2] = send_len & 0xFF;

				if (hi_channel_send_to_dev(send_buf, send_len) != HI_SUCCESS) {
					printf("vlink_hi_channel_cmd_startota--send fail\n");
				} else {
					printf("vlink_hi_channel_cmd_startota--send ok\n");
				} 			 
			}
		}		  
	}
	return NULL;
}

int vlink_wifi_ota_start_info(void)
{
	pthread_t wifiota;
	int rc1 = 0;
	rc1 = pthread_create(&wifiota, NULL, vlink_wifi_ota_thread, NULL);
	if(rc1 != 0)
		printf("%s: %d\n",__func__, strerror(rc1));

	pthread_detach(wifiota);
}



