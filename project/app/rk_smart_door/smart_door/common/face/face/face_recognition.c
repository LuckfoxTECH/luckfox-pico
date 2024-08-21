#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>
#include <poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <errno.h>

#include "rk803.h"
#include "v4l2_ir.h"
#include "face_recognition.h"
#include "unlock_algo.h"

#include "uart.h"
#include "uart_parser.h"
#include "mcu_inf.h"
#include "interface.h"
#include "server.h"

#include "v4l2_isp_luma.h"

#if SC035_VGA_SENSOR
#define SENSOR_WIDTH  640
#define SENSOR_HEIGHT 480
#define SENSOR_VTS_30FPS 0x4e2
#define SENSOR_FPS    30
#define SENSOR_VTS    (30 * SENSOR_VTS_30FPS / SENSOR_FPS)
#define SENSOR_GAIN   1
#define SENSOR_GAIN_RATIO (0x10)
#define ALGO_USE_RAW_BUFF (0)
#define ROLLING_SLAVE_MODE (0)
#define IR_AE_ENABLE       (1)
#define DEFAULT_EXP        (5.0)
#define PRO_EXP            (4.0)

#elif SC132GS_SENSOR

#define SENSOR_WIDTH  1080
#define SENSOR_HEIGHT 1280
#define SENSOR_FPS    20
#define SENSOR_VTS_30FPS    0x960
#define SENSOR_VTS    (30 * SENSOR_VTS_30FPS / SENSOR_FPS)
#define SENSOR_GAIN   2
#define SENSOR_GAIN_RATIO (0x20)
#define ALGO_USE_RAW_BUFF (1)
#define ROLLING_SLAVE_MODE (0)
#define IR_AE_ENABLE       (1)
#define DEFAULT_EXP        (5.0)
#define PRO_EXP            (7.0)

#elif SC2355_SENSOR
#define FULL_SIZE_MODE (0)//sync unlock_algo.cpp
#if FULL_SIZE_MODE
#define SENSOR_WIDTH  1600
#define SENSOR_HEIGHT 1200
#else
#define SENSOR_WIDTH  800
#define SENSOR_HEIGHT 600
#endif
#define DEFAULT_EXP (12.0)
#define SENSOR_FPS    20
#define SENSOR_VTS_30FPS 0x4e2
#define SENSOR_VTS    (30 * SENSOR_VTS_30FPS / SENSOR_FPS)
#define SENSOR_GAIN   1
#define SENSOR_GAIN_RATIO (1)
#define ALGO_USE_RAW_BUFF (1)
//rolling sensor slave mode
#define ROLLING_SLAVE_MODE (1)
#define IR_AE_ENABLE       (0)
#define PRO_EXP            (12.0)

#else
#error "error sensor type"
#endif


#define ALGO_STREAM_OFF_MODE (1)
#if IR_AE_ENABLE
#define ALGO_STABLE_CHECK    (1)
#else
#define ALGO_STABLE_CHECK    (0)
#endif

#define AUTO_START_VERIFY (0)

enum RK803_IR_TYPE {
	RK803_IR_OFF,
	RK803_IR_FLOOD,
	RK803_IR_PROJECTOR,
};

static int g_runnig_flag = 1;
static int g_demo_mode = 0;

int rk803_init(void)
{
	return open("/dev/rk803", O_RDWR | O_CLOEXEC, 0);
}

int rk803_set_ir(int fd, enum RK803_IR_TYPE type)
{
	int ret;
	int gpio1 = 0, gpio2 = 0;

	switch (type) {
	case RK803_IR_FLOOD:
		gpio1 = 1;
		gpio2 = 0;
		break;
	case RK803_IR_PROJECTOR:
		gpio1 = 0;
		gpio2 = 1;
		break;
	default:
		gpio1 = 0;
		gpio2 = 0;
		break;
	}

	ret  = ioctl(fd, RK803_SET_GPIO1, gpio1);
	ret |= ioctl(fd, RK803_SET_GPIO2, gpio2);

	return 0;
}

int rk803_set_strobe(int fd, int enable)
{
	printf("set strobe%d\n", enable);
	return ioctl(fd, RK803_SET_STROBE, enable);
}

void rk803_reset_strobe(int fd)
{
	printf("manual reset strobe/vsync\n");
	rk803_set_strobe(fd, 0);
	usleep(2*1000);
	rk803_set_strobe(fd, 1);
}

int rk803_set_flood_current(int fd, enum SL_LED_CURRENT current)
{
	return ioctl(fd, RK803_SET_CURENT1, current);
}

int rk803_set_pro_current(int fd, enum SL_LED_CURRENT current)
{
	return ioctl(fd, RK803_SET_CURENT2, current);
}

void rk803_deinit(int fd)
{
	rk803_set_ir(fd, RK803_IR_OFF);

	close(fd);
}

uint16_t sensor_time_ms_to_reg(double time_ms)
{
	return (uint16_t)(time_ms * 30 * SENSOR_VTS_30FPS / 1000);
}

int sensor_init(const char *sensor_node)
{
	int fd = open(sensor_node, O_RDWR | O_CLOEXEC, 0);
	if (fd < 0) {
		printf("open sensor error!\n");
		return fd;
	}

	struct v4l2_control ctrl;
	/* set vblank to 30 fps */
	memset(&ctrl, 0, sizeof(ctrl));
	ctrl.id = V4L2_CID_VBLANK;
	ctrl.value = SENSOR_VTS - SENSOR_HEIGHT;
	ioctl(fd, VIDIOC_S_CTRL, &ctrl);

	/* set gain */
	memset(&ctrl, 0, sizeof(ctrl));
	ctrl.id = V4L2_CID_ANALOGUE_GAIN;
	ctrl.value = SENSOR_GAIN * SENSOR_GAIN_RATIO;
	ioctl(fd, VIDIOC_S_CTRL, &ctrl);

	//printf("set default %s ms\n", DEFAULT_EXP);
	memset(&ctrl, 0, sizeof(ctrl));
	ctrl.id = V4L2_CID_EXPOSURE;
	ctrl.value = sensor_time_ms_to_reg(DEFAULT_EXP);
	ioctl(fd, VIDIOC_S_CTRL, &ctrl);

	return fd;
}


#define RKMODULE_SET_QUICK_STREAM   \
	_IOW('V', BASE_VIDIOC_PRIVATE + 10, __u32)

int sensor_quick_stream(int fd, unsigned int on_off)
{
	return ioctl(fd, RKMODULE_SET_QUICK_STREAM, &on_off);
}

void sensor_deinit(int fd)
{
	close(fd);
}

int sensor_set_expose(int fd, double time_ms)
{
	int ret;
	struct v4l2_control ctrl;
	printf("set exp:%f\n", time_ms);

	memset(&ctrl, 0, sizeof(ctrl));
	ctrl.id = V4L2_CID_EXPOSURE;
	ctrl.value = sensor_time_ms_to_reg(time_ms);
	return ioctl(fd, VIDIOC_S_CTRL, &ctrl);
}

int poll_event(int fd, int timeout_msec, int stop_fd)
{
	int num_fds = (stop_fd == -1) ? 1 : 2;
	struct pollfd poll_fds[2];
	int ret = 0;

	if (fd <= 0) {
		printf("poll fd error\n");
		return -1;
	}

	memset(poll_fds, 0, sizeof(poll_fds));
	poll_fds[0].fd = fd;
	poll_fds[0].events = (POLLPRI | POLLIN | POLLOUT | POLLERR | POLLNVAL | POLLHUP);

	if (stop_fd != -1) {
		poll_fds[1].fd = stop_fd;
		poll_fds[1].events = POLLPRI | POLLIN | POLLOUT;
		poll_fds[1].revents = 0;
	}

	ret = poll(poll_fds, num_fds, timeout_msec);

	if (stop_fd != -1) {
		if ((poll_fds[1].revents & POLLIN) || (poll_fds[1].revents & POLLPRI)) {
			printf("Poll returning from flush\n");
			return -1;
		}
	}

	if (ret > 0) {
		if (poll_fds[0].revents & (POLLERR | POLLNVAL | POLLHUP)) {
			printf("v4l2 dev/subdev polled error\n");
			return -1;
		}
	} else {
		//printf("poll fail, ret = %d\n", ret);
		return -1;
	}

	return 0;
}

void destroy_stop_fd(int event_stop_fd[2])
{
	if (event_stop_fd[1] != -1 || event_stop_fd[0] != -1) {
		close(event_stop_fd[0]);
		close(event_stop_fd[1]);
		event_stop_fd[0] = -1;
		event_stop_fd[1] = -1;
	}
}

int create_stop_fd(int event_stop_fd[2])
{
	int status;
	int ret = 0;

	status = pipe(event_stop_fd);
	if (status < 0) {
		printf("Failed to create event poll stop pipe: %s\n", strerror(errno));
		ret = -1;
		goto exit_error;
	}

	/**
	 * make the reading end of the pipe non blocking.
	 * This helps during flush to read any information left there without
	 * blocking
	 */
	status = fcntl(event_stop_fd[0], F_SETFL, O_NONBLOCK);
	if (status < 0) {
		printf("Fail to set stats poll stop pipe flag: %s\n", strerror(errno));
		ret = -1;
		goto exit_error;
	}

	return 0;

exit_error:
	destroy_stop_fd(event_stop_fd);

	return ret;
}

#include "mcu_inf.h"

struct algo_buffer {
	bool valid;
	int index;
	int dma_fd;
	void *buffer;
	int sequence;
};

struct algo_ctx_t {
	int width;
	int height;

	struct algo_buffer flood_buff;
	struct algo_buffer pro_buff;
	bool buffer_ready;

	pthread_mutex_t mut;
	pthread_cond_t cond;

	struct mcu_cmd_s mcu_cmd;
	int sensor_fd;
	void *ir_ctx;

	char sensor_stream_on;
};

static int algo_sensor_stream_on(void *param)
{
	struct algo_ctx_t *algo = (struct algo_ctx_t *)param;
	struct v4l2_ir_contex *ir_ctx = (struct v4l2_ir_contex *)algo->ir_ctx;

	v4l2_isp_luma_clr_stable();

	sensor_quick_stream(algo->sensor_fd, 1);

#if ROLLING_SLAVE_MODE
	rk803_set_strobe(ir_ctx->rk803_fd, 1);
	algo->sensor_stream_on = 1;

	printf("start exp, enable strobe\n");
#endif
	return 0;
}

void face_stop_func()
{
	system("echo face_stop > /tmp/smart_door.ini");
}

static struct algo_ctx_t *algo_buffer_check_init(int w, int h)
{
	struct algo_ctx_t *algo =
		(struct algo_ctx_t *)malloc(sizeof(struct algo_ctx_t));
	if (!algo) {
		printf("%s malloc error\n", __func__);
		return NULL;
	}

	memset(algo, 0, sizeof(struct algo_ctx_t));
	algo->width = w;
	algo->height = h;

	pthread_mutex_init(&algo->mut, NULL);
	pthread_cond_init(&algo->cond, NULL);

	algo->mcu_cmd.algo_exit_cb = face_stop_func;

#if ALGO_STREAM_OFF_MODE
	algo->mcu_cmd.algo_enable_cb = algo_sensor_stream_on;
	algo->mcu_cmd.cb_data = (void*)algo;
#endif

	return algo;
}

static void algo_buffer_check_deinit(struct algo_ctx_t *algo)
{
	pthread_mutex_destroy(&algo->mut);
	pthread_cond_destroy(&algo->cond);
	free(algo);
}

static void algo_buffer_check(void *param, void *buffer,
		int index, int dma_fd, int sequence)
{
	struct v4l2_contex *ctx = (struct v4l2_contex *)param;
	struct algo_ctx_t *algo = ctx->cb_data;
#if ALGO_USE_RAW_BUFF
	struct v4l2_ir_contex *device_ctx = (struct v4l2_ir_contex *)algo->ir_ctx;
	v4l2_ctx_enqueue enqueue = ctx->enqueue_func;
#else
	struct v4l2_contex *device_ctx = (struct v4l2_contex *)ctx;
	v4l2_ctx_enqueue enqueue = v4l2_yuv_enqueue;
#endif
	int ret = 0;

	if (enqueue == NULL) {
		printf("enqueue func null\n");
		return;
	}

	if (g_runnig_flag == 0) {
		ret = enqueue(device_ctx, index);
		return;
	}

	if (algo->buffer_ready || algo->mcu_cmd.algo_enable == false) {
		ret = enqueue(device_ctx, index);
		if (ret)
			printf("%s VIDIOC_QBUF fail\n", __func__);
		return;
	}

#if ALGO_STABLE_CHECK
	if (ctx->is_flood) {
		if (v4l2_isp_luma_get_stable() == false) {

			printf("skip:algo ir exp no stable\n");
			ret = enqueue(device_ctx, index);
			if (ret)
				printf("%s VIDIOC_QBUF fail\n", __func__);
			return;
		}

		printf("algo ir exp stable\n");
	}
#endif

	struct algo_buffer *buff;
	if (ctx->is_flood)
		buff = &algo->flood_buff;
	else
		buff = &algo->pro_buff;

	if (buff->valid == true) {
		ret = enqueue(device_ctx, buff->index);
		if (ret) {
			printf("%s VIDIOC_QBUF fail\n", __func__);
			return;
		}
		buff->valid == false;
	}

	buff->valid    = true;
	buff->index    = index;
	buff->dma_fd   = dma_fd;
	buff->buffer   = buffer;
	buff->sequence = sequence;
	printf("seq:%d index:%d buff:0x%x fd:%d %s\n",
			buff->sequence,
			buff->index,
			buff->buffer,
			buff->dma_fd,
			ctx->is_flood?"flood":"pro");

	if (algo->flood_buff.valid && algo->pro_buff.valid &&
			(algo->flood_buff.sequence == (algo->pro_buff.sequence + 1))) {
		pthread_mutex_lock(&algo->mut);
		algo->buffer_ready = true;
		pthread_mutex_unlock(&algo->mut);
		pthread_cond_signal(&algo->cond);

#if ALGO_STREAM_OFF_MODE
		/* close sensor stream */
		sensor_quick_stream(algo->sensor_fd, 0);
		algo->sensor_stream_on = 0;
#endif
	}
}

static void *v4l2_ir_capture_thread(void *arg)
{
	struct v4l2_ir_contex *ctx = arg;
	capture_callback callback = ctx->cb;
	int width   = ctx->width;
	int height  = ctx->height;
	struct v4l2_contex *ctx_tmp;
	struct algo_ctx_t *algo = (struct algo_ctx_t *)ctx->cb_data;

	int res;
	int ret = -1;
	int index, dma_fd;
	void *buffer;
	size_t length;
	char video_node[32];
	int buf_cnt = 4;
	int format  = V4L2_PIX_FMT_Y10;//V4L2_PIX_FMT_SRGGB10;
	int sequence = 0;

#if IR_AE_ENABLE
	struct isp_luma_s isp_luma;
	isp_luma.mdev_path_isp_flood = ctx->mdev_path_isp_flood;
	isp_luma.mdev_path_isp_pro = ctx->mdev_path_isp_pro;
	isp_luma.width = width;
	isp_luma.height = height;
#if (!ALGO_USE_RAW_BUFF)
	isp_luma.yuv_buf_cb = algo_buffer_check;
	isp_luma.yuv_buf_cb_data = ctx->cb_data;
#endif

	isp_luma.sensor_cb = sensor_set_expose;
	isp_luma.sensor_fd = algo->sensor_fd;
	v4l2_isp_luma_init(&isp_luma);
#endif

	memset(video_node, 0, sizeof(video_node));
	v4l2_media_get_entity_node(ctx->mdev_path_cif_ir,
			"stream_cif_mipi_id0", video_node);
	ret = v4l2_ir_init(ctx, video_node, width, height, format, buf_cnt);
	if (ret) {
		printf("v4l2_ir_init error!\n");
		goto exit;
	}

#if !AUTO_START_VERIFY
	rk803_set_ir(ctx->rk803_fd, RK803_IR_OFF);
	algo->sensor_stream_on = 0;
#else
#if ROLLING_SLAVE_MODE
	rk803_set_ir(ctx->rk803_fd, RK803_IR_PROJECTOR);
#else
	rk803_set_ir(ctx->rk803_fd, RK803_IR_FLOOD);
#endif
#endif

	ret = v4l2_ir_stream_on(ctx);
	if (ret) {
		printf("v4l2_ir_init error!\n");
		goto exit;
	}
	ctx_tmp = (struct v4l2_contex *)malloc(sizeof(struct v4l2_contex));
	memset(ctx_tmp, 0, sizeof(struct v4l2_contex));
	ctx_tmp->enqueue_func = v4l2_ir_enqueue;
	ctx_tmp->cb_data = ctx->cb_data;

#if AUTO_START_VERIFY
	algo->sensor_stream_on = 1;
	intf_set_module_face_verify(1);
#else
	sensor_quick_stream(algo->sensor_fd, 0);
#if ROLLING_SLAVE_MODE
	rk803_set_ir(ctx->rk803_fd, RK803_IR_PROJECTOR);
#else
	rk803_set_ir(ctx->rk803_fd, RK803_IR_FLOOD);
#endif
#endif

	while (g_runnig_flag) {

#if ROLLING_SLAVE_MODE
		if (algo->sensor_stream_on) {
			usleep(1*1000);
			//start exp
			rk803_set_strobe(ctx->rk803_fd, 1);
			sensor_quick_stream(algo->sensor_fd, 1);
		}
#endif
		struct v4l2_plane planes[1];
		buffer = v4l2_ir_dequeue(ctx, planes, &index, &length, &dma_fd, &sequence);
		if (!buffer) {
			printf("%s VIDIOC_DQBUF fail\n", __func__);
			goto exit;
		}

		bool is_flood = sequence % 2;
#if (!ROLLING_SLAVE_MODE)
		is_flood = !is_flood;
#endif
		printf("dq seq:%d %s\n", sequence, is_flood?"flood":"pro");

#if ROLLING_SLAVE_MODE
		rk803_set_strobe(ctx->rk803_fd, 0);
		sensor_quick_stream(algo->sensor_fd, 0);

		if (is_flood) {
			int time = read_test_val("/tmp/pro");
			if (time == 0)
				time = 12;
			printf("set next pro %d ms\n", time);
			sensor_set_expose(algo->sensor_fd, time); //pro time
			rk803_set_ir(ctx->rk803_fd, RK803_IR_PROJECTOR);
		} else {
			float time = unlock_cal_target_exp(buffer, dma_fd, width, height);
			int fix_time = read_test_val("/tmp/ir");
			if (fix_time)
				time = fix_time;

			printf("set next ir %f ms\n", time);
			sensor_set_expose(algo->sensor_fd, time);
			rk803_set_ir(ctx->rk803_fd, RK803_IR_FLOOD);
		}
#endif

#if 0
		res = callback(buffer, length, dma_fd, is_flood);
		if (res) {
			printf("callback terminate.\n");
			ret = (res == -1) ? 0 : res;
			goto exit;
		}
#endif

		if (g_runnig_flag == 0) {
			v4l2_ir_enqueue(ctx, index);
			goto exit;
		}

#if IR_AE_ENABLE
		v4l2_isp_luma_proc(is_flood, dma_fd, planes, sequence);
#endif

#if ALGO_USE_RAW_BUFF
		ctx_tmp->is_flood = is_flood;
		algo_buffer_check(ctx_tmp, buffer, index, dma_fd, sequence);
#else
		ret = v4l2_ir_enqueue(ctx, index);
		if (ret) {
			printf("%s VIDIOC_QBUF fail\n", __func__);
			goto exit;
		}
#endif
	}

exit:
	g_runnig_flag = 0;

	if (ctx_tmp)
		free(ctx_tmp);

	if (ret)
		pthread_exit("v4l2_ir_capture_thread exit failed.");
	else
		pthread_exit("v4l2_ir_capture_thread exit success.");
}

static int v4l2_ir_capture_deinit(struct v4l2_ir_contex *ctx)
{
	int ret = 0;

	printf("%s, runnig stop.\n", __func__);

	v4l2_ir_deinit(ctx);

#if IR_AE_ENABLE
	v4l2_isp_luma_deinit();
#endif

	printf("%s, exit.\n", __func__);

	return ret;
}

struct algo_result_type {
	unlock_algo_res res;
	char str[25];
};

struct algo_result_type result_str[] = {
	{UNLOCK_RESULT_DETECT_FACE_OK, "face ok"},
	{UNLOCK_RESULT_NO_FACE,        "no face"},
	{UNLOCK_RESULT_BIG_ANGLE,      "big angle"},
	{UNLOCK_RESULT_IR_FAIL,        "ir fail"},
	{UNLOCK_RESULT_DEPTH_FAIL,     "depth fail"},
	{UNLOCK_RESULT_TOO_CLOSE,      "too close"},
	{UNLOCK_RESULT_TOO_FAR,        "too far"},
	{UNLOCK_RESULT_TOO_LEFT,       "too left"},
	{UNLOCK_RESULT_TOO_RIGHT,      "too right"},
	{UNLOCK_RESULT_TOO_HIGH,       "too high"},
	{UNLOCK_RESULT_TOO_LOW,        "too low"},
	{UNLOCK_RESULT_NO_ID,          "no id"},
	{UNLOCK_RESULT_COMPARE_FACE_OK,"compare face ok"},
	{UNLOCK_RESULT_SAVE_FACE_OK,   "save face ok"},
	{UNLOCK_RESULT_SAVE_FACE_FAIL, "save face fail"},
	{UNLOCK_RESULT_NULL,           "result null"},
	{UNLOCK_RESULT_FAS_OK,         "fas ok"},
	{UNLOCK_RESULT_FACE_EXIST,     "face exist"},
	{UNLOCK_RESULT_REGISTER_FULL,  "register full"},
	{UNLOCK_RESULT_IR_OK,          "ir ok"},
	{UNLOCK_RESULT_DEPTH_OK,       "dp ok"},
	{UNLOCK_RESULT_DIR_ERROR,       "dir error"},
};

static void *do_algo_thread(void *arg)
{
	struct algo_ctx_t *ctx =(struct algo_ctx_t *)arg;

	int width   = ctx->width;
	int height  = ctx->height;
	int res;
	int usr_id = -1;

	unlock_algo_init(width, height);

	if (g_demo_mode)
		intf_set_module_face_verify(1);

	while (g_runnig_flag) {
		pthread_mutex_lock(&ctx->mut);
		if (ctx->buffer_ready != true)
			pthread_cond_wait(&ctx->cond, &ctx->mut);
		pthread_mutex_unlock(&ctx->mut);

		if (ctx->buffer_ready != true || ctx->mcu_cmd.algo_enable != true)
			continue;

		struct algo_buffer *pro_buff = &ctx->pro_buff;
		struct algo_buffer *flood_buff = &ctx->flood_buff;

		printf("pro: seq:%d index:%d buff:0x%x fd:%d\n",
			pro_buff->sequence,
			pro_buff->index,
			pro_buff->buffer,
			pro_buff->dma_fd);
		printf("flood: seq:%d index:%d buff:0x%x fd:%d\n",
			flood_buff->sequence,
			flood_buff->index,
			flood_buff->buffer,
			flood_buff->dma_fd);

		printf("unlock algo start\n");

		struct timeval time1, time2;
		float use_time;
		gettimeofday(&time1, NULL);

		uint8_t enroll_mode = ctx->mcu_cmd.enroll_mode;
		uint8_t dir = ctx->mcu_cmd.enroll_dir;

#if ALGO_USE_RAW_BUFF
		res = unlock_algo_process(flood_buff->buffer, pro_buff->buffer,
				flood_buff->dma_fd, pro_buff->dma_fd, &usr_id, enroll_mode, dir);
#else
		res = unlock_algo_yuv_process(flood_buff->buffer, pro_buff->buffer,
				flood_buff->dma_fd, pro_buff->dma_fd, &usr_id, enroll_mode, dir);
#endif

		gettimeofday(&time2, NULL);
		use_time = (time2.tv_sec - time1.tv_sec) * 1000
			+ (float)(time2.tv_usec - time1.tv_usec) / 1000;

		printf("******** unlock algo end, time:%f res:%d :%s *******\n",
				use_time, res, result_str[res].str);

		int face_state = FACE_STATE_NOFACE;
		int result = -1;
		switch (res) {
			/* face state */
			case UNLOCK_RESULT_NO_FACE:
				face_state = FACE_STATE_NOFACE;
				result = MR_ABORTED;
				break;
			case UNLOCK_RESULT_TOO_CLOSE:
				face_state = FACE_STATE_TOOCLOSE;
				result = MR_ABORTED;
				break;
			case UNLOCK_RESULT_TOO_FAR:
				face_state = FACE_STATE_TOOFAR;
				result = MR_ABORTED;
				break;
			case UNLOCK_RESULT_BIG_ANGLE:
				face_state = FACE_STATE_DIRECTION_ERROR;
				break;
#if (SC035_VGA_SENSOR || SC2355_SENSOR)//use mirror
			case UNLOCK_RESULT_TOO_LEFT:
				face_state = FACE_STATE_TOORIGHT;
				break;
			case UNLOCK_RESULT_TOO_RIGHT:
				face_state = FACE_STATE_TOOLEFT;
				break;
#else
			case UNLOCK_RESULT_TOO_LEFT:
				face_state = FACE_STATE_TOOLEFT;
				break;
			case UNLOCK_RESULT_TOO_RIGHT:
				face_state = FACE_STATE_TOORIGHT;
				break;
#endif
			case UNLOCK_RESULT_TOO_HIGH:
				face_state = FACE_STATE_TOOUP;
				break;
			case UNLOCK_RESULT_TOO_LOW:
				face_state = FACE_STATE_TOODOWN;
				break;
			/* live and recognize */
			case UNLOCK_RESULT_IR_FAIL:
				face_state = FACE_STATE_NORMAL;
				result = MR_FAILED4_LIVENESSCHECK;
				break;
			case UNLOCK_RESULT_DEPTH_FAIL:
				face_state = FACE_STATE_NORMAL;
				result = MR_FAILED4_LIVENESSCHECK;
				break;
			case UNLOCK_RESULT_NO_ID:
				face_state = FACE_STATE_NORMAL;
				result = MR_FAILED4_UNKNOWNUSER;
				break;
			case UNLOCK_RESULT_SAVE_FACE_OK:
			case UNLOCK_RESULT_COMPARE_FACE_OK:
			case UNLOCK_RESULT_FACE_EXIST:
				face_state = FACE_STATE_NORMAL;
				result = MR_SUCCESS;
				break;
			default:
				result = -1;
				break;
		}

		struct timeval time_now;
		gettimeofday(&time_now, NULL);
		if ((result == -1) && (ctx->mcu_cmd.verify_timeout <
					(time_now.tv_sec + time_now.tv_usec / 1000000)))
			result = MR_FAILED4_TIMEOUT;

		/* set face state */
		s_note_data_face face_res;
		memset(&face_res, 0, sizeof(face_res));
		face_res.state = face_state;
		intf_module_send_face_state(face_res.state);
		sys_send_facestate_result_msg(&face_res);

		if (result != -1) {
			if (enroll_mode) {
				sys_send_enroll_result_msg(
						enroll_mode,
						result, usr_id, dir);
				intf_module_send_enroll_result(result, usr_id);
			} else {
				sys_send_verify_result_msg(result, usr_id);
				intf_module_send_verify_result(result, usr_id);
			}

			if (g_demo_mode)
				intf_set_module_face_verify(1);
		} else {
#if ALGO_STREAM_OFF_MODE
			algo_sensor_stream_on(ctx);
#endif
		}

		ctx->buffer_ready = false;
	}

	unlock_algo_deinit();

exit:
	g_runnig_flag = 0;

	pthread_exit("do_algo_thread exit.");
}

static int ir_capture_callback(unsigned char *buffer, int length, int dma_fd, bool is_flood)
{
	static int frame_cnt;
	static FILE *fp = NULL;
	int max_frame = 10;
	const char *tmp_file = "/tmp/Y10_1080_1280.raw";

	if (frame_cnt >= max_frame)
		return 0;

	if (!fp) {
		fp = fopen(tmp_file, "w+");
		if (!fp) {
			printf("create %s failed.\n", tmp_file);
			return -2;
		}
	}

	frame_cnt++;

	printf("get frame[%d], buffer = %p, dma_fd = %d, length = %d, type: %s\n",
	       frame_cnt, buffer, dma_fd, length, is_flood ? "flood" : "projector");

	fwrite(buffer, length, 1, fp);

	if (frame_cnt >= max_frame) {
		//g_runnig_flag = 0;
		fclose(fp);
		fp = NULL;
		//frame_cnt = 0;
		printf("save %d frame raw data to %s success.\n", max_frame, tmp_file);
		return -1;
	}

	return 0;
}

int smart_door_face_recogniton_start(struct face_recogniton_arg *arg)
{
	int ret;
	int event_stop_fd[2];
	int rk803_fd = -1, csi2_fd = -1, sensor_fd = -1;
	char ir_csi2_node[32];
	char ir_sensor_node[32];

	struct v4l2_event_subscription sub;
	struct v4l2_event event;
	pthread_t capture_tid = 0;
	pthread_t algo_tid = 0;
	void *thread_result;

	struct timeval time1, time2;
	int use_time;
	gettimeofday(&time1, NULL);

	/* detect time */
	int timeout_second = 2;

	g_runnig_flag = 1;

	rk803_fd = rk803_init();
	if (rk803_fd < 0) {
		printf("rk803_init error!\n");
		goto exit;
	}

	rk803_set_flood_current(rk803_fd, LED_1000MA);
	rk803_set_pro_current(rk803_fd, LED_1100MA);

	memset(ir_sensor_node, 0, sizeof(ir_sensor_node));
	v4l2_media_get_entity_node(arg->mdev_path_cif_ir, arg->sensor_name, ir_sensor_node);
	sensor_fd = sensor_init(ir_sensor_node);
	if (sensor_fd < 0) {
		printf("sensor_init error!\n");
		goto exit;
	}

	memset(ir_csi2_node, 0, sizeof(ir_csi2_node));
	v4l2_media_get_entity_node(arg->mdev_path_cif_ir, "rockchip-mipi-csi2", ir_csi2_node);
	csi2_fd = open(ir_csi2_node, O_RDWR);
	if (csi2_fd <= 0) {
		printf("open %s failed\n", ir_csi2_node);
		goto exit;
	}

	memset(&sub, 0x00, sizeof(sub));

	sub.type = V4L2_EVENT_FRAME_SYNC;

	ret = ioctl(csi2_fd, VIDIOC_SUBSCRIBE_EVENT, &sub);
	if (ret) {
		printf("csi2 VIDIOC_SUBSCRIBE_EVENT error.\n");
		goto exit;
	}

	create_stop_fd(event_stop_fd);

	struct v4l2_ir_contex ctx;
	memset(&ctx, 0, sizeof(ctx));
	ctx.cb     = ir_capture_callback;
	ctx.width  = SENSOR_WIDTH;
	ctx.height = SENSOR_HEIGHT;
	ctx.mdev_path_cif_ir    = arg->mdev_path_cif_ir;
	ctx.mdev_path_isp_flood = arg->mdev_path_isp_flood;
	ctx.mdev_path_isp_pro   = arg->mdev_path_isp_pro;
	ctx.rk803_fd = rk803_fd;

	struct algo_ctx_t *algo =
		algo_buffer_check_init(ctx.width, ctx.height);
	ctx.cb_data = algo;
	algo->sensor_fd = sensor_fd;
	algo->ir_ctx = (void*)&ctx;

	pthread_create(&capture_tid, NULL, v4l2_ir_capture_thread, &ctx);

	pthread_create(&algo_tid, NULL, do_algo_thread, algo);

	smart_door_uart_init(cmd_uart_parser);

	cmd_uart_init(smart_door_uart_sendbyte);

	cmd_uart_set_del_usr_cb(unlock_algo_clear_face);

	cmd_uart_send_nid_ready();

	mcu_inf_set_cmd_data(&algo->mcu_cmd);

	face_server_init();

	intf_module_ready();

	while (g_runnig_flag) {

		if (g_demo_mode) {
			gettimeofday(&time2, NULL);
			use_time = (time2.tv_sec - time1.tv_sec)
				+ (int)(time2.tv_usec - time1.tv_usec) / 1000000;
			if (use_time > timeout_second) {
				printf("detect timeout\n");
				g_runnig_flag = 0;
			}
		}

		/* expected: event_stop_fd[0].revents == POLLPRI */
		ret = poll_event(csi2_fd, 300, event_stop_fd[0]);
		if (ret) {
			//printf("poll_event error.");

#if ROLLING_SLAVE_MODE
			if (algo->sensor_stream_on)
				rk803_reset_strobe(rk803_fd);
#endif

			continue;
		}

		/* Data ready, FD_ISSET(0, &fds) will be true. */
		ioctl(csi2_fd, VIDIOC_DQEVENT, &event);

		if (event.type != V4L2_EVENT_FRAME_SYNC) {
			printf("ERROR: unknown event.type=0x%x\n", event.type);
			continue;
		}

		//printf("event.sequence = %d\n", event.sequence);

#if (ROLLING_SLAVE_MODE == 0)
		if (event.sequence % 2 == 0) {
#if (IR_AE_ENABLE == 0)
			sensor_set_expose(sensor_fd, 12.0);
#endif
			rk803_set_ir(rk803_fd, RK803_IR_PROJECTOR);
		} else {
#if SC132GS_SENSOR //luma proc do not set pro exp
			sensor_set_expose(sensor_fd, PRO_EXP);
#endif
			rk803_set_ir(rk803_fd, RK803_IR_FLOOD);
		}
#endif
	}

exit:
	destroy_stop_fd(event_stop_fd);

	if (csi2_fd > 0)
		close(csi2_fd);

	v4l2_ir_stream_off(&ctx);

	if (capture_tid) {
		printf("wait capture thread join.\n");
		ret = pthread_join(capture_tid, &thread_result);

		printf("thread joined, it returned %s\n", (char *)thread_result);
	}

	if (algo_tid) {
		pthread_cond_signal(&algo->cond);

		printf("wait algo_tid thread join.\n");
		ret = pthread_join(algo_tid, &thread_result);

		printf("thread joined, it returned %s\n", (char *)thread_result);
	}

	v4l2_ir_capture_deinit(&ctx);

	rk803_deinit(rk803_fd);
	sensor_deinit(sensor_fd);

	face_server_deinit();
	smart_door_uart_deinit();
	cmd_uart_deinit();
	algo_buffer_check_deinit(algo);

	return ret;
}

void smart_door_face_recogniton_stop(void)
{
	g_runnig_flag = 0;
}

int smart_door_face_auto_test(struct face_recogniton_arg *arg)
{
	printf("%s\n", __func__);

	g_demo_mode = 1;

	while (1) {
		//detect time (second)
		smart_door_face_recogniton_start(arg);

		//sleep time (second)
		sleep(3);
	}
}
