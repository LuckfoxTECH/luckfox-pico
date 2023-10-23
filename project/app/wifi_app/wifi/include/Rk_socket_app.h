#ifndef SOCKET_APP_H
#define SOCKET_APP_H

#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <sys/stat.h>

#ifdef __cplusplus
extern "C" {
#endif

struct rk_socket_app {
	int server_sockfd;
	int client_sockfd;
	int server_len;
	int client_len;
	struct sockaddr_un server_address;
	struct sockaddr_un client_address;
	char sock_path[512];
};

/*server api*/
int RK_socket_server_setup(struct rk_socket_app *app_socket);
int RK_socke_server_accpet(struct rk_socket_app *app_socket);
void RK_socket_server_teardown(struct rk_socket_app *app_socket);

/*client api*/
int RK_socket_client_setup(char *socket_path);
void RK_socket_client_teardown(int sockfd);

/*common api*/
int RK_socket_send(int sockfd, char *msg, int len);
int RK_socket_recieve(int sockfd, char *msg, int len);

/*simple socket client, just send msg*/
int RK_socket_udp_send(char *socket_path, char *msg, int len);

#ifdef __cplusplus
}
#endif

#endif
