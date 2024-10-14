#ifndef __v4l2_isp_H__
#define __v4l2_isp_H__

#include "v4l2_ctx.h"

int v4l2_isp_init(struct v4l2_contex *ctx, const char *video_node, int width, int height);
void v4l2_isp_deinit(struct v4l2_contex *ctx);
void v4l2_isp_trigger(struct v4l2_contex *ctx, int sequence);
#endif
