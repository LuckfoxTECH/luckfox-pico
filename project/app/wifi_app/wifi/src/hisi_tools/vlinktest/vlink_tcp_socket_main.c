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

#define MAXLINE 4096
#define TCP_PORT 20000

void* vlink_tcp_socket_thread(void* arg)
{
    int  listenfd, connfd;
    struct sockaddr_in  servaddr;
    char  buff[4096];
    int  n;

    if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("192.168.43.1");//htonl(INADDR_ANY);
    servaddr.sin_port = htons(TCP_PORT);

    if( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }

    if( listen(listenfd, 10) == -1){
        printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }

    printf("======waiting for client's request===ip[%s]=port[%d]==\n", "192.168.43.1", TCP_PORT);
    while(1){
        if( (connfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1){
            printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
            continue;
        }
        n = recv(connfd, buff, MAXLINE, 0);
        buff[n] = '\0';
        printf("recv msg from client: %s\n", buff);

    	if( send(connfd, buff, strlen(buff), 0) < 0){
        	printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
        	return 0;
    	}

        close(connfd);
    }
    close(listenfd);
    return 0;
}

int vlink_tcp_socket_start_info(void)
{
	pthread_t tcpserver;
	int rc1 = 0;
	rc1 = pthread_create(&tcpserver, NULL, vlink_tcp_socket_thread, NULL);
	if(rc1 != 0)
		printf("%s: %d\n",__func__, strerror(rc1));

	pthread_detach(tcpserver);
}



