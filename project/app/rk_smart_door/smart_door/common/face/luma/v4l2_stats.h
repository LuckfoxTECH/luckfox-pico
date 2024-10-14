#ifndef __v4l2_stats_H__
#define __v4l2_stats_H__

#include "v4l2_ctx.h"

int v4l2_stats_stream_on(struct v4l2_contex *ctx);
void v4l2_stats_stream_off(struct v4l2_contex *ctx);


int v4l2_stats_enqueue(struct v4l2_contex *ctx, int index);
void *v4l2_stats_dequeue(struct v4l2_contex *ctx,
		int *index, size_t *len, int *sequence);

void *v4l2_stats_thread(void *arg);
int v4l2_stats_thread_init(void *arg);

int v4l2_stats_init(struct v4l2_contex *ctx,
		const char *video_node,
		int width, int height);

void v4l2_stats_deinit(struct v4l2_contex *ctx);
#endif
