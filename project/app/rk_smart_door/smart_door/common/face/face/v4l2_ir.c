#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev2.h>

#include "param/rkcif-config.h"
#include "v4l2_ir.h"

#define FMT_NUM_PLANES	1

int v4l2_ir_init(struct v4l2_ir_contex *ctx, const char *video_node,
		 int width, int height, int format, int buf_cnt)
{
	int video_fd = -1;
	struct ir_buffer *tmp_buffers = NULL;
	struct v4l2_buffer buf;
	struct v4l2_plane planes[FMT_NUM_PLANES];
	enum v4l2_buf_type buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;

	//memset(ctx, 0x00, sizeof(ctx));

	video_fd = open(video_node, O_RDWR /* required */ /*| O_NONBLOCK*/, 0);
	if (video_fd < 0) {
		printf("Cannot open %s: %d, %s\n", video_node, errno, strerror(errno));
		goto error;
	}

	printf("%s, video_fd is %d\n", video_node, video_fd);

	/* set capture format */
	struct v4l2_format fmt;

	memset(&fmt, 0, sizeof(fmt));
	fmt.type                = buf_type;
	fmt.fmt.pix.width       = width;
	fmt.fmt.pix.height      = height;
	fmt.fmt.pix.pixelformat = format;
	fmt.fmt.pix.field       = V4L2_FIELD_NONE;

	int mem_mode = CSI_LVDS_MEM_WORD_LOW_ALIGN;
	if (ioctl(video_fd, RKCIF_CMD_SET_CSI_MEMORY_MODE, &mem_mode) < 0) {
		printf("RKCIF_CMD_SET_CSI_MEMORY_MODE fail: %d, %s\n",
				errno, strerror(errno));
		goto error;
	}

	if (ioctl(video_fd, VIDIOC_S_FMT, &fmt) == -1) {
		printf("VIDIOC_S_FMT fail: %d, %s\n", errno, strerror(errno));
		goto error;
	}

	/* request ir_buffer */
	struct v4l2_requestbuffers req;

	memset(&req, 0, sizeof(req));
	req.count  = buf_cnt;
	req.type   = buf_type;
	req.memory = V4L2_MEMORY_MMAP;

	if (ioctl(video_fd, VIDIOC_REQBUFS, &req) == -1) {
		printf("VIDIOC_REQBUFS fail: %d, %s\n", errno, strerror(errno));
		goto error;
	}

	/* alloc ir_buffer */
	tmp_buffers = (struct ir_buffer *)calloc(req.count, sizeof(struct ir_buffer));
	if (!tmp_buffers) {
		printf("alloc ir_buffer error\n");
		goto error;
	}

	for (int numBufs = 0; numBufs < req.count; numBufs++) {
		memset(&buf, 0, sizeof(buf));
		buf.type   = buf_type;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index  = numBufs;
		buf.m.planes = planes;
		buf.length = FMT_NUM_PLANES;

		// query ir_buffer
		if (ioctl(video_fd, VIDIOC_QUERYBUF, &buf) == -1) {
			printf("VIDIOC_QUERYBUF fail\n");
			goto error;
		}

		tmp_buffers[numBufs].length = buf.m.planes[0].length;
		tmp_buffers[numBufs].start =
			mmap(NULL, buf.m.planes[0].length,
			     PROT_READ | PROT_WRITE, MAP_SHARED, video_fd,
			     buf.m.planes[0].m.mem_offset);

		if (tmp_buffers[numBufs].start == MAP_FAILED) {
			printf("start == MAP_FAILED\n");
			goto error;
		}

		// export buf dma fd
		struct v4l2_exportbuffer expbuf;

		memset(&expbuf, 0, sizeof(expbuf));
		expbuf.type = buf_type;
		expbuf.index = numBufs;
		expbuf.flags = O_CLOEXEC;

		if (ioctl(video_fd, VIDIOC_EXPBUF, &expbuf) < 0) {
			printf("get dma buf failed\n");
			goto error;
		}

		tmp_buffers[numBufs].export_fd = expbuf.fd;

		// qbuffer
		if (ioctl(video_fd, VIDIOC_QBUF, &buf) == -1) {
			printf("VIDIOC_QBUF fail\n");
			goto error;
		}
	}

	ctx->video_fd   = video_fd;
	ctx->planes     = FMT_NUM_PLANES;
	ctx->buffers    = tmp_buffers;
	ctx->buffer_cnt = buf_cnt;

	return 0;
error:
	if (tmp_buffers)
		free(tmp_buffers);

	if (video_fd >= 0)
		close(video_fd);

	ctx->video_fd   = -1;
	ctx->buffers    = NULL;
	ctx->buffer_cnt = 0;

	return -1;
}

void v4l2_ir_deinit(struct v4l2_ir_contex *ctx)
{
	if (!ctx || !ctx->buffers)
		return;

	if (ctx->buffers) {
		for (int i = 0; i < ctx->buffer_cnt; i++) {
			munmap(ctx->buffers[i].start, ctx->buffers[i].length);
			close(ctx->buffers[i].export_fd);
		}

		free(ctx->buffers);
	}

	if (ctx->video_fd >= 0)
		close(ctx->video_fd);

	//memset(ctx, 0x00, sizeof(*ctx));
}

int v4l2_ir_stream_on(struct v4l2_ir_contex *ctx)
{
	enum v4l2_buf_type buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;

	return ioctl(ctx->video_fd, VIDIOC_STREAMON, &buf_type);
}

void v4l2_ir_stream_off(struct v4l2_ir_contex *ctx)
{
	enum v4l2_buf_type buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;

	ioctl(ctx->video_fd, VIDIOC_STREAMOFF, &buf_type);
}

void *v4l2_ir_dequeue(struct v4l2_ir_contex *ctx,
		struct v4l2_plane *planes, int *index, size_t *len, int *dma_fd, int *sequence)
{
	int ret;
	struct v4l2_buffer buf;

	if (!ctx || !ctx->buffers)
		return NULL;

	if (!index || !len || !dma_fd)
		return NULL;

	memset(planes, 0, sizeof(struct v4l2_plane));
	memset(&buf, 0, sizeof(buf));

	buf.type     = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	buf.memory   = V4L2_MEMORY_MMAP;
	buf.index    = 0;
	buf.m.planes = planes;
	buf.length   = FMT_NUM_PLANES;

	ret = ioctl(ctx->video_fd, VIDIOC_DQBUF, &buf);
	if (ret)
		return NULL;

	*index  = buf.index;
	*len    = buf.m.planes[0].bytesused;
	*dma_fd = ctx->buffers[buf.index].export_fd;
	*sequence = buf.sequence;

	return ctx->buffers[buf.index].start;
}

int v4l2_ir_enqueue(void *param, int index)
{
	struct v4l2_ir_contex *ctx = (struct v4l2_ir_contex *)param;
	int ret;
	struct v4l2_buffer buf;
	enum v4l2_buf_type buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	struct v4l2_plane planes[FMT_NUM_PLANES];

	if (!ctx || !ctx->buffers)
		return -1;

	memset(planes, 0, sizeof(planes));
	memset(&buf, 0, sizeof(buf));

	buf.type     = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	buf.memory   = V4L2_MEMORY_MMAP;
	buf.index    = index;
	buf.m.planes = planes;
	buf.length   = FMT_NUM_PLANES;

	ret = ioctl(ctx->video_fd, VIDIOC_QBUF, &buf);

	return ret;
}

