// Copyright 2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <errno.h>
#include <stddef.h>
#include <unistd.h>
#include <pthread.h>

#include "interface.h"
#include "server.h"

static int server_sockfd = -1;
static int client_sockfd = -1;
static int g_runnig_flag;
static pthread_t server_tid = 0;

#define SERVER_NAME "@face_server_socket"

#define MAX_CMD_LEN (256)

void server_write_cmd(char *cmd, int len)
{
	if (client_sockfd != -1)
		write(client_sockfd, cmd, len);
}

static void *server_thread(void *arg)
{
	socklen_t server_len, client_len;
	struct sockaddr_un server_addr;
	struct sockaddr_un client_addr;
	char cmd[MAX_CMD_LEN];
	int nread;
	int maxfd;
	int ret = 0;
	fd_set rdset, cur_reset;
	struct timeval tv;

	printf("%s start\n", __func__);

	//delete the old server socket
	unlink(SERVER_NAME);

	//create socket
	server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

	//name the socket
	server_addr.sun_family = AF_UNIX;
	strcpy(server_addr.sun_path, SERVER_NAME);
	server_addr.sun_path[0]=0;
	server_len = strlen(SERVER_NAME)  + offsetof(struct sockaddr_un, sun_path);

	bind(server_sockfd, (struct sockaddr *)&server_addr, server_len);

	//listen the server
	listen(server_sockfd, 5);
	client_sockfd = -1;
	client_len = sizeof(client_addr);

	g_runnig_flag = 1;

	FD_ZERO(&rdset);
	FD_SET(server_sockfd, &rdset);
	maxfd = server_sockfd;
	memset(&tv, 0x00, sizeof(tv));
	tv.tv_usec  = 100000;

	while(g_runnig_flag) {

		cur_reset = rdset;
		int fd_result = select(maxfd + 1, &cur_reset, NULL, NULL, &tv);
		if(fd_result < 0) {
			printf("select err");
			int ret = -EINVAL;
			goto exit;
		} else if(fd_result == 0) {
			usleep(10000);
			continue;
		} else  {
			if(FD_ISSET(server_sockfd, &cur_reset)) {
				client_sockfd = accept(server_sockfd,
						(struct sockaddr*)&client_addr, &client_len);
				if (client_sockfd < 0)
					continue;

				printf("connect client success, fd:%d\n", client_sockfd);
				maxfd = maxfd > client_sockfd ? maxfd : client_sockfd;
				FD_SET(client_sockfd, &rdset);
			}

			if(client_sockfd != -1 &&
					FD_ISSET(client_sockfd, &cur_reset)) {

				//read data from client socket
				memset(cmd, 0, sizeof(cmd));
				nread = read(client_sockfd, cmd, MAX_CMD_LEN);

				if (nread == 0) { //client disconnected
					printf("client %d disconnected\n",client_sockfd);
					FD_CLR(client_sockfd, &rdset);
					close(client_sockfd);
					client_sockfd = -1;
				} else {
					intf_cmd_parser(cmd, nread);
				}
			}
		}
		usleep(1000);
	}

exit:
	if (client_sockfd != -1) {
		close(client_sockfd);
		client_sockfd = -1;
	}

	unlink(SERVER_NAME);

	if (server_sockfd != -1) {
		close(server_sockfd);
		server_sockfd = -1;
	}

	pthread_exit("server_thread exit.");
}

int face_server_init(void)
{
	pthread_create(&server_tid, NULL, server_thread, NULL);
}

int face_server_deinit(void)
{
	int ret = 0;
	void *thread_result;
	g_runnig_flag = 0;

	if (server_tid) {
		printf("wait server thread join.\n");
		ret = pthread_join(server_tid, &thread_result);

		printf("thread joined, it returned %s\n",
				(char *)thread_result);
	}

	return ret;
}


