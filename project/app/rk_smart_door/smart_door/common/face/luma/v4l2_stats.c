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
#include "v4l2_stats.h"

#define FMT_NUM_PLANES	1

static enum v4l2_buf_type buf_type = V4L2_BUF_TYPE_META_CAPTURE;
static enum v4l2_memory mem_type = V4L2_MEMORY_MMAP;

//statistics
typedef unsigned long int uintptr_t;
int _v4l2_stats_init(struct v4l2_contex *ctx, const char *video_node,
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
		memset(&buf, 0, sizeof(buf));
		buf.type   = buf_type;
		buf.memory = mem_type;
		buf.index  = numBufs;

		// query
		if (ioctl(video_fd, VIDIOC_QUERYBUF, &buf) == -1) {
			printf("VIDIOC_QUERYBUF fail\n");
			goto error;
		}

		void *pointer = MAP_FAILED;
		pointer = mmap (0, buf.length,
				PROT_READ | PROT_WRITE, MAP_SHARED, video_fd, buf.m.offset);
		if (pointer == MAP_FAILED) {
			printf("userptr == MAP_FAILED\n");
			goto error;
		}

		buf.m.userptr = (uintptr_t)pointer;

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
		tmp_buffers[numBufs].userptr = buf.m.userptr;
		tmp_buffers[numBufs].length = buf.length;

		//qbuffer
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

void _v4l2_stats_deinit(struct v4l2_contex *ctx)
{
	if (!ctx || !ctx->buffers)
		return;

	for (int i = 0; i < ctx->buffer_cnt; i++) {
		munmap((void*)ctx->buffers[i].userptr, ctx->buffers[i].length);
		if (ctx->buffers[i].export_fd)
			close(ctx->buffers[i].export_fd);
	}

	if (ctx->buffers) {
		free(ctx->buffers);
	}

	if (ctx->video_fd >= 0)
		close(ctx->video_fd);

	memset(ctx, 0x00, sizeof(*ctx));
}

int v4l2_stats_stream_on(struct v4l2_contex *ctx)
{
	return ioctl(ctx->video_fd, VIDIOC_STREAMON, &buf_type);
}

void v4l2_stats_stream_off(struct v4l2_contex *ctx)
{
	ioctl(ctx->video_fd, VIDIOC_STREAMOFF, &buf_type);
}

void *v4l2_stats_dequeue(struct v4l2_contex *ctx,
		int *index, size_t *len, int *sequence)
{
	int ret;
	struct v4l2_buffer buf;
	struct v4l2_plane planes[FMT_NUM_PLANES];

	if (!ctx || !ctx->buffers)
		return NULL;

	if (!index || !len)
		return NULL;

	memset(planes, 0, sizeof(struct v4l2_plane));
	memset(&buf, 0, sizeof(buf));

	buf.type     = buf_type;
	buf.memory   = mem_type;

	ret = ioctl(ctx->video_fd, VIDIOC_DQBUF, &buf);
	if (ret)
		return NULL;

	*index  = buf.index;
	*sequence = buf.sequence;

	return (void*)ctx->buffers[buf.index].userptr;
}

int v4l2_stats_enqueue(struct v4l2_contex *ctx, int index)
{
	int ret;
	struct v4l2_buffer buf;
	struct v4l2_plane planes[FMT_NUM_PLANES];

	if (!ctx || !ctx->buffers)
		return -1;

	memset(planes, 0, sizeof(planes));
	memset(&buf, 0, sizeof(buf));

	buf.type     = buf_type;
	buf.memory   = mem_type;
	buf.index    = index;

	ret = ioctl(ctx->video_fd, VIDIOC_QBUF, &buf);

	return ret;
}

int v4l2_stats_init(struct v4l2_contex *ctx,
		const char *video_node,
		int width, int height)
{
	printf("%s:%s\n", __func__, video_node);
	int res = 0;
	int ret = -1;
	void *buffer;
	size_t length;
	int buf_cnt = 4;
	int format  = V4L2_PIX_FMT_Y10;//V4L2_PIX_FMT_SRGGB10;

	ret = _v4l2_stats_init(ctx, video_node, width, height, format, buf_cnt);
	if (ret) {
		printf("_v4l2_stats_init error! ret:%d\n", ret);
		goto exit;
	}

	ret = v4l2_stats_stream_on(ctx);
	if (ret) {
		printf("v4l2_stats_stream_on error!\n");
		goto exit;
	}

	pthread_create(&ctx->thrd_tid, NULL, v4l2_stats_thread, ctx);

exit:
	return ret;
};

void v4l2_stats_print(void *buff)
{
	struct rkisp32_isp_stat_buffer *stats =
		(struct rkisp32_isp_stat_buffer *)buff;

	printf(">>> stats id:%d, luma:\n", stats->frame_id);
	for (int i = 0; i < 15; i++) {
		int index = i * 15 ;
		printf("[%d - %d]: ", index, index + 15);
		for (int j = 0; j < 15; j++)
			printf("%d ", stats->params.rawae1_0.data[index + j].channelg_xy);
		printf("\n");
	}
}

void *v4l2_stats_thread(void *arg)
{
	printf("%s\n", __func__);
	int res = 0;
	int ret = -1;
	int index, dma_fd;
	void *buffer;
	size_t length;
	struct v4l2_contex *ctx = (struct v4l2_contex *)arg;
	int sequence;

	ctx->running_flag = 1;
	while (ctx->running_flag) {
		buffer = v4l2_stats_dequeue(ctx, &index, &length, &sequence);
		if (!buffer) {
			printf("%s VIDIOC_DQBUF fail\n", __func__);
			goto exit;
		}

		if (!ctx->running_flag)
			break;

		//v4l2_stats_print(buffer);
		if (ctx->buff_cb)
			ctx->buff_cb(ctx, buffer, index, dma_fd, sequence);

		ret = v4l2_stats_enqueue(ctx, index);
		if (ret) {
			printf("%s VIDIOC_QBUF fail\n", __func__);
			goto exit;
		}
	}

exit:
	if (ret)
		pthread_exit("v4l2_stats_capture_thread exit failed.");
	else
		pthread_exit("v4l2_stats_capture_thread exit success.");
}

void v4l2_stats_deinit(struct v4l2_contex *ctx)
{
	void *thread_result;
	int ret;
	ctx->running_flag = 0;

	v4l2_stats_stream_off(ctx);

	if (ctx->thrd_tid) {
		printf("wait ctx->thrd_tid thread join.\n");
		ret = pthread_join(ctx->thrd_tid, &thread_result);

		printf("thread joined, it returned %s\n", (char *)thread_result);
	}

	_v4l2_stats_deinit(ctx);

	printf("%s, exit.\n", __func__);
}
