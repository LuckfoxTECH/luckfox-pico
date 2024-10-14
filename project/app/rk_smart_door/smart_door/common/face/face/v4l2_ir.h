#ifndef __V4L2_IR_H__
#define __V4L2_IR_H__

#include <stddef.h>
#include <pthread.h>
#include <stdbool.h>

struct ir_buffer {
	void *start;
	size_t length;
	int export_fd;
	int sequence;
};

typedef int (*capture_callback)(unsigned char *buffer, int length, int dma_fd, bool is_flood);

struct v4l2_ir_contex {
	char *mdev_path_cif_ir;
	char *mdev_path_isp_flood;
	char *mdev_path_isp_pro;
	int video_fd;
	int planes;
	int buffer_cnt;
	struct ir_buffer *buffers;
	capture_callback cb;
	void *cb_data;

	int width;
	int height;
	void *data;
	int rk803_fd;
};

int v4l2_ir_init(struct v4l2_ir_contex *ctx, const char *video_node,
		 int width, int height, int format, int buf_cnt);
void v4l2_ir_deinit(struct v4l2_ir_contex *ctx);
int v4l2_ir_stream_on(struct v4l2_ir_contex *ctx);
void v4l2_ir_stream_off(struct v4l2_ir_contex *ctx);
void *v4l2_ir_dequeue(struct v4l2_ir_contex *ctx, struct v4l2_plane *planes, int *index, size_t *len, int *dma_fd, int *sequence);
int v4l2_ir_enqueue(void *param, int index);

#endif
