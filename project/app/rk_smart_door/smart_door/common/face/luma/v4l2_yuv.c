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

#include "v4l2_yuv.h"

#define FMT_NUM_PLANES	1

static enum v4l2_buf_type buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
static enum v4l2_memory mem_type = V4L2_MEMORY_MMAP;

//rkisp_mainpath
typedef unsigned int uint32_t;

static const char *fourcc_to_string (uint32_t fourcc)
{
	static char str[5];
	memset (str, 0, sizeof(str));
	memcpy (str, &fourcc, 4);
	return str;
}

typedef unsigned long int uintptr_t;
int _v4l2_yuv_init(struct v4l2_contex *ctx, const char *video_node,
		 int width, int height, int format, int buf_cnt)
{
	int video_fd = -1;
	struct v4l2_ctx_buffer *tmp_buffers = NULL;
	struct v4l2_buffer buf;
	struct v4l2_plane planes[FMT_NUM_PLANES];

	video_fd = open(video_node, O_RDWR /* required */ /*| O_NONBLOCK*/, 0);
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

	struct v4l2_selection selection;
	memset(&selection, 0, sizeof(selection));

	selection.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	selection.target = V4L2_SEL_TGT_CROP;
	selection.flags = 0;
	selection.r.left = 0;
	selection.r.top = 0;
	selection.r.width = width;
	selection.r.height = height;

	if (ioctl(video_fd, VIDIOC_S_SELECTION, &selection) < 0) {
		printf("yuv VIDIOC_S_SELECTION fail: %d, %s\n",
				errno, strerror(errno));
		goto error;
	}

	/* get capture format */
	struct v4l2_format fmt;
	memset(&fmt, 0, sizeof(fmt));
	fmt.type = buf_type;
	if (ioctl(video_fd, VIDIOC_G_FMT, &fmt) < 0) {
		printf("VIDIOC_G_FMT fail: %d, %s\n", errno, strerror(errno));
		goto error;
	}

	/* set capture format */
	fmt.type                = buf_type;
	fmt.fmt.pix.width       = width;
	fmt.fmt.pix.height      = height;
	fmt.fmt.pix.pixelformat = format;
	fmt.fmt.pix.field       = V4L2_FIELD_NONE;

	fmt.fmt.pix_mp.width = width;
	fmt.fmt.pix_mp.height = height;
	fmt.fmt.pix_mp.pixelformat = format;
	fmt.fmt.pix_mp.field = V4L2_FIELD_NONE;
	fmt.fmt.pix_mp.quantization = V4L2_QUANTIZATION_FULL_RANGE;
	fmt.fmt.pix_mp.plane_fmt[0].bytesperline = width;

	if (ioctl(video_fd, VIDIOC_S_FMT, &fmt) < 0) {
		printf("VIDIOC_S_FMT fail: %d, %s\n", errno, strerror(errno));
		goto error;
	}

#if 0
	if (ioctl(video_fd, VIDIOC_G_FMT, &fmt) < 0) {
		printf("VIDIOC_G_FMT fail: %d, %s\n", errno, strerror(errno));
		goto error;
	}

	printf("yuv get fmt:type:%d w:%d h:%d pixformat:%d "\
			"field:%d sizeimage:%d bytesperline:%d\n",
			fmt.type,
			fmt.fmt.pix.width,
			fmt.fmt.pix.height,
			fmt.fmt.pix.pixelformat,
			fmt.fmt.pix.field,
			fmt.fmt.pix.sizeimage,
			fmt.fmt.pix_mp.plane_fmt[0].bytesperline);
	printf("fmt: %uX%u\npixelformat: %s\n",
			fmt.fmt.pix.width,fmt.fmt.pix.height,
			fourcc_to_string(fmt.fmt.pix.pixelformat));
	printf("yuv: get mpfmt: w:%d h:%d pixformt:%d field:%d quantization:%d\n",
			fmt.fmt.pix_mp.width,
			fmt.fmt.pix_mp.height,
			fmt.fmt.pix_mp.pixelformat,
			fmt.fmt.pix_mp.field,
			fmt.fmt.pix_mp.quantization);

	/* get capture format */
	struct v4l2_crop crop;
	memset(&crop, 0, sizeof(crop));
	if (ioctl(video_fd, VIDIOC_G_CROP, &crop) < 0) {
		printf("VIDIOC_G_CROP fail: %d, %s\n", errno, strerror(errno));
		goto error;
	}

	printf("crop: (%u, %u)%uX%u\n",
		crop.c.left, crop.c.top, crop.c.width, crop.c.height);
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

void _v4l2_yuv_deinit(struct v4l2_contex *ctx)
{
	if (!ctx || !ctx->buffers)
		return;

	for (int i = 0; i < ctx->buffer_cnt; i++) {
		munmap((void*)ctx->buffers[i].start, ctx->buffers[i].length);
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

int v4l2_yuv_stream_on(struct v4l2_contex *ctx)
{
	return ioctl(ctx->video_fd, VIDIOC_STREAMON, &buf_type);
}

void v4l2_yuv_stream_off(struct v4l2_contex *ctx)
{
	ioctl(ctx->video_fd, VIDIOC_STREAMOFF, &buf_type);
}

void *v4l2_yuv_dequeue(struct v4l2_contex *ctx,
		int *index, size_t *len, int *dma_fd, int *sequence)
{
	int ret;
	struct v4l2_buffer buf;
	struct v4l2_plane planes[FMT_NUM_PLANES];

	if (!ctx || !ctx->buffers)
		return NULL;

	if (!index || !len || !dma_fd)
		return NULL;

	memset(planes, 0, sizeof(struct v4l2_plane));
	memset(&buf, 0, sizeof(buf));

	buf.type     = buf_type;
	buf.memory   = mem_type;
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

int v4l2_yuv_enqueue(void *param, int index)
{
	struct v4l2_contex *ctx = (struct v4l2_contex *)param;
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
	buf.m.planes = planes;
	buf.length   = FMT_NUM_PLANES;

	ret = ioctl(ctx->video_fd, VIDIOC_QBUF, &buf);

	return ret;
}

#define MAX_DUMP_FILE (10)
int dump_yuv(struct v4l2_contex *ctx, int index, char *buffer, int length)
{
	FILE *fp = (FILE *)ctx->fp;
	char name[128];
	snprintf(name, 128, "/tmp/dump/y_video%d_640x480.raw", ctx->video_fd);

	if (ctx->frame_cnt > MAX_DUMP_FILE)
		return -1;

	if (!fp) {
		fp = fopen(name, "wb+");
		if (!fp) {
			printf("create %s failed.\n", name);
			return -2;
		}
		ctx->fp = (void*)fp;
	}

	ctx->frame_cnt++;

	printf("%s get frame[%d], index:%d buffer:%p, length:%d\n",
	       name, ctx->frame_cnt, index, buffer, length);

	length = 640*480;
	fwrite(buffer, length, 1, fp);

	if (ctx->frame_cnt > MAX_DUMP_FILE) {
		fclose(fp);
		ctx->fp = NULL;
		printf("save %d frame raw data to %s success.\n",
				MAX_DUMP_FILE, name);
		return -1;
	}

	return 0;
}

void *v4l2_yuv_thread(void *arg)
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
		buffer = v4l2_yuv_dequeue(ctx, &index, &length, &dma_fd, &sequence);
		if (!buffer) {
			printf("%s VIDIOC_DQBUF fail\n", __func__);
			goto exit;
		}

		if (!ctx->running_flag)
			break;

		//dump_yuv(ctx, index, buffer, length);

		if (ctx->buff_cb) {
			ctx->buff_cb(ctx, buffer, index, dma_fd, sequence);
		} else {
			ret = v4l2_yuv_enqueue(ctx, index);
			if (ret) {
				printf("%s VIDIOC_QBUF fail\n", __func__);
				goto exit;
			}
		}
	}

exit:
	if (ret)
		pthread_exit("v4l2_yuv_capture_thread exit failed.");
	else
		pthread_exit("v4l2_yuv_capture_thread exit success.");
}

int v4l2_yuv_init(struct v4l2_contex *ctx,
		const char *video_node, int width, int height)
{
	printf("%s:%s\n", __func__, video_node);
	int res = 0;
	int ret = -1;
	void *buffer;
	size_t length;
	int buf_cnt = 2;
	int format  = V4L2_PIX_FMT_NV12;

	ret = _v4l2_yuv_init(ctx, video_node, width, height, format, buf_cnt);
	if (ret) {
		printf("_v4l2_yuv_init error! ret:%d\n", ret);
		goto exit;
	}

	ret = v4l2_yuv_stream_on(ctx);
	if (ret) {
		printf("v4l2_yuv_stream_on error!\n");
		goto exit;
	}

	ctx->enqueue_func = v4l2_yuv_enqueue;

	pthread_create(&ctx->thrd_tid, NULL, v4l2_yuv_thread, ctx);

exit:
	return ret;
};

int v4l2_yuv_deinit(struct v4l2_contex *ctx)
{
	int ret;
	void *thread_result;

	printf("%s\n", __func__);

	ctx->running_flag = 0;
	v4l2_yuv_stream_off(ctx);

	if (ctx->thrd_tid) {
		printf("wait ctx->thrd_tid thread join.\n");
		ret = pthread_join(ctx->thrd_tid, &thread_result);
		printf("thread joined, it returned %s\n",
				(char *)thread_result);
	}

	_v4l2_yuv_deinit(ctx);
	return 0;
}
