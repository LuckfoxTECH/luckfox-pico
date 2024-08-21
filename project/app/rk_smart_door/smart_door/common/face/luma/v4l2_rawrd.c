#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <pthread.h>
#include <linux/videodev2.h>

#include "param/rkisp32-config.h"
#include "v4l2_rawrd.h"

#define FMT_NUM_PLANES	1

static enum v4l2_buf_type buf_type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
static enum v4l2_memory mem_type = V4L2_MEMORY_DMABUF;
typedef unsigned int uint32_t;

static const char *fourcc_to_string (uint32_t fourcc)
{
	static char str[5];
	memset (str, 0, sizeof(str));
	memcpy (str, &fourcc, 4);
	return str;
}

int _v4l2_rawrd_init(struct v4l2_contex *ctx, const char *video_node,
		 int width, int height, int format, int buf_cnt)
{
	int video_fd = -1;
	struct v4l2_ctx_buffer *tmp_buffers = NULL;
	struct v4l2_buffer buf;
	struct v4l2_plane planes[FMT_NUM_PLANES];

	video_fd = open(video_node, O_RDWR | O_CLOEXEC, 0);
	if (video_fd < 0) {
		printf("Cannot open %s: %d, %s\n", video_node, errno, strerror(errno));
		goto error;
	}
	printf("%s, video_fd is %d\n", video_node, video_fd);

	struct v4l2_capability cap;
	//memset(ctx, 0x00, sizeof(*ctx));
	/* get */
	if (ioctl(video_fd, VIDIOC_QUERYCAP, &cap) == -1) {
		printf("VIDIOC_QUERYCAP fail: %d, %s\n", errno, strerror(errno));
		goto error;
	}

	if (cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)
		buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	else if (cap.capabilities & V4L2_CAP_VIDEO_CAPTURE_MPLANE)
		buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	else if (cap.capabilities & V4L2_CAP_VIDEO_OUTPUT)
		buf_type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
	else if (cap.capabilities & V4L2_CAP_VIDEO_OUTPUT_MPLANE)
		buf_type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
	else if (cap.capabilities & V4L2_CAP_META_CAPTURE)
		buf_type = V4L2_BUF_TYPE_META_CAPTURE;
	else if (cap.capabilities & V4L2_CAP_META_OUTPUT)
		buf_type = V4L2_BUF_TYPE_META_OUTPUT;
	else {
		printf("@%s: unsupported buffer type.", __FUNCTION__);
		return -1;
	}

#if 0
	printf("------------------------------\n");
	printf("driver:       '%s'\n", cap.driver);
	printf("card:         '%s'\n", cap.card);
	printf("bus_info:     '%s'\n", cap.bus_info);
	printf("version:      %x\n", cap.version);
	printf("capabilities: %x\n", cap.capabilities);
	printf("device caps:  %x\n", cap.device_caps);
	printf("buffer type   %d\n", buf_type);
	printf("------------------------------\n");
#endif

	/* get capture format */
	struct v4l2_format fmt;
	memset(&fmt, 0, sizeof(fmt));
	fmt.type = buf_type;
	if (ioctl(video_fd, VIDIOC_G_FMT, &fmt) < 0) {
		printf("VIDIOC_G_FMT fail: %d, %s\n", errno, strerror(errno));
		goto error;
	}

	int mem_mode = CSI_MEM_WORD_LITTLE_ALIGN;
	if (ioctl(video_fd, RKISP_CMD_SET_CSI_MEMORY_MODE, &mem_mode) < 0) {
		printf("RKISP_CMD_SET_CSI_MEMORY_MODE fail: %d, %s\n",
				errno, strerror(errno));
		goto error;
	}

	/* set capture format */
	fmt.type                = buf_type;
	fmt.fmt.pix.width       = width;
	fmt.fmt.pix.height      = height;
	fmt.fmt.pix.pixelformat = format;
	fmt.fmt.pix.field       = V4L2_FIELD_NONE;

	if (ioctl(video_fd, VIDIOC_S_FMT, &fmt) < 0) {
		printf("VIDIOC_S_FMT fail: %d, %s\n", errno, strerror(errno));
		goto error;
	}

	if (ioctl(video_fd, VIDIOC_G_FMT, &fmt) < 0) {
		printf("VIDIOC_G_FMT fail: %d, %s\n", errno, strerror(errno));
		goto error;
	}

#if 0
	printf("rawrd get fmt:type:%d w:%d h:%d pixformat:%d field:%d sizeimage:%d\n",
			fmt.type,
			fmt.fmt.pix.width,
			fmt.fmt.pix.height,
			fmt.fmt.pix.pixelformat,
			fmt.fmt.pix.field,
			fmt.fmt.pix.sizeimage);
	printf("rawrd fmt: %uX%u\npixelformat:   %s\n",
			fmt.fmt.pix.width,fmt.fmt.pix.height, fourcc_to_string(fmt.fmt.pix.pixelformat));

	printf("raw: get mpfmt: w:%d h:%d pixformt:%d  field:%d quantization:%d\n",
			fmt.fmt.pix_mp.width,
			fmt.fmt.pix_mp.height,
			fmt.fmt.pix_mp.pixelformat,
			fmt.fmt.pix_mp.field,
			fmt.fmt.pix_mp.quantization);
#endif

	/* request v4l2_ctx_buffer */
	struct v4l2_requestbuffers req;

	memset(&req, 0, sizeof(req));
	req.count  = buf_cnt;
	req.type   = buf_type;
	req.memory = mem_type;

	if (ioctl(video_fd, VIDIOC_REQBUFS, &req) == -1) {
		printf("VIDIOC_REQBUFS fail: %d, %s\n", errno, strerror(errno));
		goto error;
	}

	/* alloc buffer */
	tmp_buffers = (struct v4l2_ctx_buffer *)calloc(req.count,
			sizeof(struct v4l2_ctx_buffer));
	if (!tmp_buffers) {
		printf("alloc v4l2_ctx_buffer error\n");
		goto error;
	}

	for (int numBufs = 0; numBufs < req.count; numBufs++) {
		tmp_buffers[numBufs].length = fmt.fmt.pix.sizeimage;;
		tmp_buffers[numBufs].bytesused = fmt.fmt.pix.sizeimage;;
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

void _v4l2_rawrd_deinit(struct v4l2_contex *ctx)
{
	if (!ctx || !ctx->buffers)
		return;

	if (ctx->buffers) {
		free(ctx->buffers);
	}

	if (ctx->video_fd >= 0)
		close(ctx->video_fd);

	memset(ctx, 0x00, sizeof(*ctx));
}

int v4l2_rawrd_stream_on(struct v4l2_contex *ctx)
{
	return ioctl(ctx->video_fd, VIDIOC_STREAMON, &buf_type);
}

void v4l2_rawrd_stream_off(struct v4l2_contex *ctx)
{
	ioctl(ctx->video_fd, VIDIOC_STREAMOFF, &buf_type);
}

int v4l2_rawrd_dequeue(struct v4l2_contex *ctx)
{
	int ret;
	struct v4l2_buffer buf;
	struct v4l2_plane planes[FMT_NUM_PLANES];

	if (!ctx)
		return -1;

	memset(planes, 0, sizeof(planes));
	memset(&buf, 0, sizeof(buf));

	buf.type     = buf_type;
	buf.memory   = mem_type;
	buf.index    = 0;
	buf.m.planes = planes;
	buf.length   = FMT_NUM_PLANES;

	ret = ioctl(ctx->video_fd, VIDIOC_DQBUF, &buf);
	if (ret) {
		printf("%s dq error, ret:%d\n", __func__, ret);
		return -1;
	}

	int fd = buf.m.planes[0].m.fd;
	return 0;
}

int v4l2_rawrd_enqueue(struct v4l2_contex *ctx, int fd, void *param)
{
	struct v4l2_plane *planes = (struct v4l2_plane *)param;
	int ret;
	struct v4l2_buffer buf;

	if (!ctx || !ctx->buffers)
		return -1;

	memset(planes, 0, sizeof(struct v4l2_plane));
	memset(&buf, 0, sizeof(buf));

	buf.type     = buf_type;
	buf.memory   = mem_type;
	buf.index    = 0;
	buf.length   = FMT_NUM_PLANES;
	buf.m.planes = planes;
	buf.m.planes[0].m.fd = fd;
	buf.m.planes[0].length = ctx->buffers[buf.index].length;
	buf.m.planes[0].bytesused = ctx->buffers[buf.index].bytesused;

	ret = ioctl(ctx->video_fd, VIDIOC_QBUF, &buf);
	return ret;
}

int v4l2_rawrd_init(struct v4l2_contex *ctx,
		const char *video_node, int width, int height)
{
	printf("%s:%s\n", __func__, video_node);
	int res = 0;
	int ret = -1;
	int index, dma_fd;
	void *buffer;
	size_t length;
	int buf_cnt = 1;
	int format  = V4L2_PIX_FMT_Y10;

	ret = _v4l2_rawrd_init(ctx, video_node, width, height, format, buf_cnt);
	if (ret) {
		printf("_v4l2_rawrd_init error! ret:%d\n", ret);
		return ret;
	}

	ret = v4l2_rawrd_stream_on(ctx);
	if (ret) {
		printf("v4l2_rawrd_stream_on error!\n");
	}
	return ret;
}

int v4l2_rawrd_deinit(struct v4l2_contex *ctx)
{
	printf("%s, exit.\n", __func__);

	v4l2_rawrd_stream_off(ctx);

	_v4l2_rawrd_deinit(ctx);

	return 0;
}
