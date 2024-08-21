#ifndef __v4l2_rawrd_H__
#define __v4l2_rawrd_H__

#include "v4l2_ctx.h"

int v4l2_rawrd_enqueue(struct v4l2_contex *ctx, int fd, void *param);
int v4l2_rawrd_dequeue(struct v4l2_contex *ctx);

int v4l2_rawrd_init(struct v4l2_contex *ctx,
		const char *video_node, int width, int height);
int v4l2_rawrd_deinit(struct v4l2_contex *ctx);

#endif
