#ifndef __v4l2_yuv_H__
#define __v4l2_yuv_H__

#include "v4l2_ctx.h"

int v4l2_yuv_stream_on(struct v4l2_contex *ctx);
void v4l2_yuv_stream_off(struct v4l2_contex *ctx);


int v4l2_yuv_enqueue(void *param, int index);
void *v4l2_yuv_dequeue(struct v4l2_contex *ctx,
		int *index, size_t *len, int *dma_fd, int *sequence);

void *v4l2_yuv_thread(void *arg);
int v4l2_yuv_thread_init(void *arg);
int v4l2_yuv_init(struct v4l2_contex *ctx,
		const char *video_node, int width, int height);
int v4l2_yuv_deinit(struct v4l2_contex *ctx);
#endif
