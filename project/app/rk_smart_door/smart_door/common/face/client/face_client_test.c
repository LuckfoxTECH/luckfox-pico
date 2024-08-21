#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>

#include "face_client.h"

static int read_test_val(char *path)
{
	int fd_node;
	char str[10] = {0};
	int value = 0;

	fd_node = open(path, O_RDONLY);
	if (fd_node < 0) {
		return 0;
	}

	if (lseek(fd_node, 0, SEEK_SET) != -1) {
		read(fd_node, str, sizeof(str) - 1);
		value = atoi(str);
	}

	close(fd_node);

	return value;
}

int test_node(void)
{
#define NODE_NAME "/tmp/client"
	int ret = 0;
	int test_mode = read_test_val(NODE_NAME);
	if (test_mode == 1)
		set_module_face_verify(3);
	else if (test_mode == 2)
		set_module_face_enroll(3);
	else if (test_mode == 3)
		set_module_clr_all_face();
	else if (test_mode == 4)
		set_module_clr_face_id(0);
	else if (test_mode == 5)
		set_module_clr_face_id(1);
	else if (test_mode == 6)
		set_module_exit_face();
	else if (test_mode == 10)
		ret = -1;

	if (test_mode)
		system("rm "NODE_NAME);

	return ret;
}

void module_ready_cb(void)
{
	printf("%s\n", __func__);
}

void verity_result_cb(int result, int usr_id)
{
	printf("%s result:%d id:%d\n", __func__, result, usr_id);
}

void enroll_result_cb(int result, int usr_id)
{
	printf("%s result:%d id:%d\n", __func__, result, usr_id);
}

void face_state_cb(int result)
{
	printf("%s result:%d\n", __func__, result);
}

int main()
{
	face_client_init();

	set_module_ready_cb(module_ready_cb);
	set_verify_result_cb(verity_result_cb);
	set_enroll_result_cb(enroll_result_cb);
	set_face_state_cb(face_state_cb);

	while (1) {
		int ret = test_node();
		if (ret)
			break;
		sleep(1);
	}

	face_client_deinit();
}
