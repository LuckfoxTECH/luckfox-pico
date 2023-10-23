#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>

#define RK_CMD_LINE_LEN 1600
#define CLI_SOCKET_PATH "/tmp/rkserver_socket"

int rkwifi_cli(int argc, char *argv[])
{
	char command[RK_CMD_LINE_LEN];
	struct sockaddr_un ser_un;
	int tmp_argc = 1;
	int socket_fd = 0;
	int ret = 0;
	int i = 0;

	memset(command, 0, sizeof(command));
	sprintf(command, "%s", argv[tmp_argc++]);

	while (tmp_argc < argc)
	{
		sprintf(command, "%s %s", command, argv[tmp_argc++]);
	}

	socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (socket_fd <= 0)
	{
		printf("open socket err\n");
		return -1;
	}

	memset(&ser_un, 0, sizeof(ser_un));
	ser_un.sun_family = AF_UNIX;
	strcpy(ser_un.sun_path, CLI_SOCKET_PATH);

	ret = connect(socket_fd, (struct sockaddr *)&ser_un, sizeof(struct sockaddr_un));
	if(ret < 0)
	{
		printf("connect err\n");
		return -1; 
	}

	printf("rkwifi cmd: %s\n", command);
	write(socket_fd, command, strlen(command)+1);
	read(socket_fd, command, sizeof(command));
	if (strcmp(command, "OK"))
	{
		printf("send cmd err\n");
		return -1;
	} else
		printf("%s\n", command);

	return 0;
}
