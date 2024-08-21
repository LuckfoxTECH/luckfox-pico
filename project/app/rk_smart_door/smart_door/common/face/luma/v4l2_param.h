#ifndef __v4l2_param_H__
#define __v4l2_param_H__

#include "v4l2_ctx.h"

int v4l2_param_stream_on(struct v4l2_contex *ctx);
void v4l2_param_stream_off(struct v4l2_contex *ctx);

int v4l2_param_enqueue(struct v4l2_contex *ctx, int fd);
int v4l2_param_dequeue(struct v4l2_contex *ctx);

int v4l2_param_init(struct v4l2_contex *ctx,
		const char *video_node, int width, int height);
void v4l2_param_deinit(struct v4l2_contex *ctx);
#endif
