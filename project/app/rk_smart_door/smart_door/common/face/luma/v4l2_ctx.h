#ifndef __v4l2_CTX_H__
#define __v4l2_CTX_H__

#include <stddef.h>
#include <pthread.h>
#include <stdbool.h>

#define DEBUG printf("%s %d\n", __func__, __LINE__);

struct v4l2_ctx_buffer {
	size_t length;
	size_t bytesused;
	void *start;
	int export_fd;
	unsigned long userptr;
};

typedef void (*buff_callback)(
		void *ctx_ptr, void *buffer, int index,
		int dma_fd, int sequence);

typedef int (*v4l2_ctx_enqueue)(void *param, int index);

struct v4l2_contex {
	int video_fd;
	int planes;
	int buffer_cnt;
	struct v4l2_ctx_buffer *buffers;
	buff_callback buff_cb;
	void *cb_data;

	int running_flag;
	pthread_t thrd_tid;
	int frame_cnt;
	void *fp;
	bool is_flood;

	v4l2_ctx_enqueue enqueue_func;
};

#endif
