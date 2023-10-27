#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <Rk_socket_app.h>

#define LOG_TAG "RK_SOCKET_APP"
#define LOG_DEBUG_LEVEL (1)
#define LOG_ERROR_FLAG (4)
#define LOG_WARING_FLAG (3)
#define LOG_INFO_FLAG (2)
#define LOG_DEBUG_FLAG (1)

#define LOG_PRINTF(level, format, ...) \
        do { \
                if (level > LOG_DEBUG_LEVEL) { \
                        printf("[%s]: " format, LOG_TAG, ##__VA_ARGS__); \
                } \
        } while(0)

#define log_info(format, ...) LOG_PRINTF(LOG_INFO_FLAG, format, ##__VA_ARGS__)
#define log_dbg(format, ...) LOG_PRINTF(LOG_DEBUG_FLAG, format, ##__VA_ARGS__)
#define log_warn(format, ...) LOG_PRINTF(LOG_WARING_FLAG, format, ##__VA_ARGS__)
#define log_err(format, ...) LOG_PRINTF(LOG_ERROR_FLAG, format, ##__VA_ARGS__)

int RK_socket_server_setup(struct rk_socket_app *app_socket)
{
    int buflen = 1024*1024; //1M, default 8K
    socklen_t optlen = 0;

    unlink (app_socket->sock_path);
    if ((app_socket->server_sockfd = socket (AF_UNIX, SOCK_STREAM, 0)) < 0) {
        log_err("fail to create socket\n");
        perror("socket");
        return -1;
    }

    app_socket->server_address.sun_family = AF_UNIX;
    strcpy (app_socket->server_address.sun_path, app_socket->sock_path);
    app_socket->server_len = sizeof (app_socket->server_address);
    app_socket->client_len = sizeof (app_socket->client_address);

    if ((bind (app_socket->server_sockfd, (struct sockaddr *)&app_socket->server_address, app_socket->server_len)) < 0) {
        log_err("fail to bind socket\n");
        perror("bind");
        return -1;

    }

    setsockopt(app_socket->server_sockfd, SOL_SOCKET, SO_RCVBUF, (void*)&buflen, sizeof(int));
    getsockopt(app_socket->server_sockfd, SOL_SOCKET, SO_RCVBUF, (void*)&buflen, &optlen);
    log_info("%s: SO_RCVBUF buflen=%d, optlen=%d\n", __func__, buflen, optlen);

    if (listen (app_socket->server_sockfd, 10) < 0) {
        log_err("fail to listen socket\n");
        perror("listen");
        return -1;
    }
    log_dbg ("Server is ready for client connect...\n");

    return 0;
}

int RK_socke_server_accpet(struct rk_socket_app *app_socket) {
    app_socket->client_sockfd = accept (app_socket->server_sockfd, (struct sockaddr *)&app_socket->server_address, (socklen_t *)&app_socket->client_len);
    if (app_socket->client_sockfd == -1) {
        log_err("fail to accept socket\n");
        perror ("accept");
        return -1;
    }
    return 0;
}

void RK_socket_server_teardown(struct rk_socket_app *app_socket) {
    unlink (app_socket->sock_path);

    if(app_socket->server_sockfd) {
        close(app_socket->server_sockfd);
        app_socket->server_sockfd = 0;
    }

    if(app_socket->client_sockfd) {
        close(app_socket->client_sockfd);
        app_socket->client_sockfd = 0;
    }
}

int RK_socket_client_setup(char *socket_path) {
    struct sockaddr_un address;
    int sockfd, len;
    int buflen = 1024*1024; //1M, default 8K
    socklen_t optlen = 0;

    if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        log_err("%s: can not creat socket\n", __func__);
        return -1;
    }

    address.sun_family = AF_UNIX;
    strcpy (address.sun_path, socket_path);
    len = sizeof (address);

    setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, (void*)&buflen, sizeof(int));
    getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, (void*)&buflen, &optlen);
    log_info("%s: SO_SNDBUF buflen=%d, optlen=%d\n", __func__, buflen, optlen);

    if (connect (sockfd, (struct sockaddr *)&address, len) == -1) {
        log_err("%s: can not connect to socket\n", __func__);;
        return -1;
    }

    return sockfd;
}

void RK_socket_client_teardown(int sockfd) {
    if(sockfd) {
        close(sockfd);
        sockfd = 0;
    }
}

int RK_socket_send(int sockfd, char *msg, int len) {
    int bytes;
    if (sockfd < 0) {
        log_err("%s: invalid sockfd\n",__func__);
        return -1;
    }
    if ((bytes = send(sockfd, msg, len, 0)) < 0) {
        log_err("fail to send\n");
        perror ("send");
    }
    return bytes;
}

int RK_socket_recieve(int sockfd, char *msg, int len) {
    int bytes;

    if (sockfd < 0) {
        log_err("%s: invalid sockfd\n",__func__);
        return -1;
    }

    if ((bytes = recv(sockfd, msg, len, 0)) < 0)
    {
        log_err("fail to recv\n");
        perror ("recv");
    }
    return bytes;
}

int RK_socket_udp_send(char *socket_path, char *msg, int len) {
	int bytes = 0;
	int sockfd;
	struct sockaddr_un serverAddr;

	sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		printf("FUNC:%s create sockfd failed!\n", __func__);
		return 0;
	}

	serverAddr.sun_family = AF_UNIX;
	strcpy(serverAddr.sun_path, socket_path);

	bytes = sendto(sockfd, msg, len, MSG_DONTWAIT, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
	if(bytes < 0)
		log_err("bytes: %d, errno: %s\n", bytes, strerror(errno));

	close(sockfd);
	return bytes;
}
