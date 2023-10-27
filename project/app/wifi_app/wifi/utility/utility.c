#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <paths.h>
#include <sys/wait.h>

#include "slog.h"
#include "utility.h"

#define DEBUG 1

static int system_fd_closexec(const char* command)
{
	int wait_val = 0;
	pid_t pid = -1;

	if (!command)
		return 1;

	if ((pid = vfork()) < 0)
		return -1;

	if (pid == 0) {
		int i = 0;
		int stdin_fd = fileno(stdin);
		int stdout_fd = fileno(stdout);
		int stderr_fd = fileno(stderr);
		long sc_open_max = sysconf(_SC_OPEN_MAX);
		if (sc_open_max < 0) {
			sc_open_max = 20000; /* enough? */
		}
		/* close all descriptors in child sysconf(_SC_OPEN_MAX) */
		for (i = 0; i < sc_open_max; i++) {
			if (i == stdin_fd || i == stdout_fd || i == stderr_fd)
				continue;
			close(i);
		}

		execl(_PATH_BSHELL, "sh", "-c", command, (char*)0);
		_exit(127);
	}

	while (waitpid(pid, &wait_val, 0) < 0) {
		if (errno != EINTR) {
			wait_val = -1;
			break;
		}
	}

	return wait_val;
}

int exec_command_system(const char *cmd)
{
	system(cmd);
#if 0
	pid_t status;

	status = system_fd_closexec(cmd);

	if (-1 == status) {
		pr_err("[system_exec_err] -1\n");
		return -1;
	} else {
		if (WIFEXITED(status)) {
			if (0 == WEXITSTATUS(status)) {
				return 0;
			} else {
				pr_err("[system_exec_err %s] -2\n", cmd);
				return -2;
			}
		} else {
			pr_err("[system_exec_err] -3\n");
			return -3;
		}
	}
#endif
	return 0;
}

void exec_command(char cmdline[], char recv_buff[], int len)
{
	if (DEBUG) pr_info("[BT_DEBUG] execute: %s\n", cmdline);

	FILE *stream = NULL;
	char *tmp_buff = recv_buff;

	memset(recv_buff, 0, len);

	if ((stream = popen(cmdline, "r")) != NULL) {
		while (fgets(tmp_buff, len, stream)) {
			//pr_info("tmp_buf[%d]: %s\n", strlen(tmp_buff), tmp_buff);
			tmp_buff += strlen(tmp_buff);
			len -= strlen(tmp_buff);
			if (len <= 1)
				break;
		}

		if (DEBUG) pr_info("[BT_DEBUG] execute_r: %s \n", recv_buff);
		pclose(stream);
	} else
		pr_err("[popen] error: %s\n", cmdline);
}

int test_pthread(pthread_t tid) /*pthread_kill的返回值：成功（0） 线程不存在（ESRCH） 信号不合法（EINVAL）*/
{
	int pthread_kill_err;
	pthread_kill_err = pthread_kill(tid, 0);

	if(pthread_kill_err == ESRCH)
		pr_info("ID 0x%x NOT EXIST OR EXIT\n", (unsigned int)tid);
	else if(pthread_kill_err == EINVAL)
		pr_info("SIGNAL ILL\n");
	else
		pr_info("ID 0x%x ALIVE\n", (unsigned int)tid);

	return pthread_kill_err;
}

int get_ps_pid(const char Name[])
{
	int len;
	char name[32] = {0};
	char cmdresult[256] = {0};
	char cmd[64] = {0};
	FILE *pFile = NULL;
	int  pid = 0;
	int retry_cnt = 3;

retry:
	memset(name, 0, 32);
	memset(cmdresult, 0, 256);
	memset(cmd, 0, 64);

	len = strlen(Name);
	strncpy(name,Name,len);
	name[31] ='\0';

//	sprintf(cmd, "pidof %s", name);
	sprintf(cmd, "busybox ps | grep %s | grep -v grep", name);

	pFile = popen(cmd, "r");
	if (pFile != NULL)  {
		while (fgets(cmdresult, sizeof(cmdresult), pFile)) {
			//pid = atoi(cmdresult);
			break;
		}
		pclose(pFile);
	}

	//if ((pid == 0) && (retry_cnt--))
	if ((cmdresult[0] == 0) && (retry_cnt--))
		goto retry;

	//return pid;
	return cmdresult[0];
}

int kill_task(char *name)
{
	char cmd[128] = {0};
	int exec_cnt = 3, retry_cnt = 10;

	if (!get_ps_pid(name))
		return 0;

	memset(cmd, 0, 128);
	sprintf(cmd, "killall %s", name);

	exec_command_system(cmd);

	if (get_ps_pid(name))
		msleep(600);

	if (get_ps_pid(name)) {
		memset(cmd, 0, 128);
		sprintf(cmd, "killall -9 %s", name);
		exec_command_system(cmd);
		msleep(300);
	}

	if (get_ps_pid(name)) {
		pr_info("%s: kill %s failure [%d]\n", __func__, name, get_ps_pid(name));
		return -1;
	} else {
		pr_info("%s: kill %s successful\n", __func__, name);
		return 0;
	}
}

int run_task(char *name, char *cmd)
{
	int exec_cnt = 3, retry_cnt = 6;

	while(exec_cnt) {
		if(!exec_command_system(cmd))
			break;
		exec_cnt--;
	}

	if(exec_cnt <= 0) {
		pr_info("%s: run %s failed\n", __func__, name);
		return -1;
	}
	msleep(100);

retry:
	if (!get_ps_pid(name) && (retry_cnt--)) {
		msleep(100);
		goto retry;
	}

	if (!get_ps_pid(name))
		return -1;
	else
		return 0;
}

int rk_addr2str(const char *addr, char *str)
{
	return sprintf(str, "%2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X",
			addr[5], addr[4], addr[3], addr[2], addr[1], addr[0]);
}
