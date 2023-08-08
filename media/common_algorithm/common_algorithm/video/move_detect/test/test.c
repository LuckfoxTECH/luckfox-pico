#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "move_detection.h"

void rga_ds(unsigned char* pin, unsigned char* pout, int w, int h, int w_ds, int h_ds);
void down_sampling(unsigned char* im, unsigned char* img_down_sam, int height, int width);
void down_sampling1(unsigned char* im, unsigned char* img_down_sam, int height, int width);

void down_sampling(unsigned char* im, unsigned char* img_down_sam, int height, int width)
{
	int i, j;

	height = height / 3;
	width = width / 3;

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			img_down_sam[i*width + j] = (im[(i * 3 + 0)*width * 3 + j * 3 + 0] + im[(i * 3 + 0)*width * 3 + j * 3 + 1] + im[(i * 3 + 0)*width * 3 + j * 3 + 2] + im[(i * 3 + 1)*width * 3 + j * 3 + 0] + im[(i * 3 + 1)*width * 3 + j * 3 + 1] + im[(i * 3 + 1)*width * 3 + j * 3 + 2] + im[(i * 3 + 2)*width * 3 + j * 3 + 0] + im[(i * 3 + 2)*width * 3 + j * 3 + 1] + im[(i * 3 + 2)*width * 3 + j * 3 + 2]) / 9;
			//img_down_sam[i*width + j] = ( im[(i * 3 + 0)*width * 3 + j * 3 + 1] +
			//im[(i * 3 + 1)*width * 3 + j * 3 + 0] + im[(i * 3 + 1)*width * 3 + j * 3 + 1] + im[(i * 3 + 1)*width * 3 + j * 3 + 2] +
			//im[(i * 3 + 2)*width * 3 + j * 3 + 1]) / 5;
		}
	}
}
void down_sampling1(unsigned char* im, unsigned char* img_down_sam, int height, int width)
{
	int i, j;

	height = height / 2;
	width = width / 2;

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			img_down_sam[i*width + j] = im[(i * 2 + 1)*width * 2 + j * 2 + 1];
		}
	}
}

#define TIME_DEBUG(FUNC) {\
	struct timeval tv1, tv2;\
	gettimeofday(&tv1, NULL);\
	FUNC; \
	gettimeofday(&tv2, NULL);\
	printf("elapse %ld ms\n", (tv2.tv_sec - tv1.tv_sec) * 1000 + (tv2.tv_usec - tv1.tv_usec) / 1000);\
}

#define HW_MAIN 1


#if HW_MAIN

const char yuv_input[200] = "/data/314_1080p_10000kbps_h265_300frames.yuv";
const int yuv_w = 1920;
const int yuv_h = 1080;
int is_single_ref = 0;

int hw_simu_top()
{
	unsigned char datain[yuv_w * yuv_h];
	unsigned char datain_uv[yuv_w * yuv_h / 2];
	INFO_LIST info_list[4096];
	ROI_INFO roi_in;

	FILE *fidin = fopen(yuv_input, "rb");
	//FILE *fod_debug3 = fopen("info_list.txt", "wb");
	int sz;
	int frame;
	int i;
	int frame_size_y, frame_size_uv;
	ROI_INFO* p_roi_in = (ROI_INFO*)&roi_in;
	int width, height, width_ds, height_ds;
	struct md_ctx *md_ctx;
	void *img_down_sam;

	width = yuv_w;
	height = yuv_h;
	width_ds = yuv_w/3;
	height_ds = yuv_h/3;
	roi_in.flag = 1;
	roi_in.is_move = 0;
	roi_in.up_left[0] = 0;//y
	roi_in.up_left[1] = 0;//x
	roi_in.down_right[0] = yuv_h-1;//y
	roi_in.down_right[1] = yuv_w-1;//x

	fseek(fidin, 0L, SEEK_END);
	sz = ftell(fidin);
	fseek(fidin, 0L, SEEK_SET);
	frame = sz / (width * height * 3 / 2);
	frame_size_y = width*height;
	frame_size_uv = frame_size_y/2;

	md_ctx = move_detection_init(width, height, width_ds, height_ds, is_single_ref);
	img_down_sam = (unsigned char *)malloc(width_ds * height_ds);

	fread(datain, sizeof(unsigned char), frame_size_y, fidin);
	fread(datain_uv, sizeof(unsigned char), frame_size_uv, fidin);

	rga_ds(datain, img_down_sam, width, height, width_ds, height_ds);
	memset(info_list, 0, sizeof(info_list));
	move_detection(md_ctx, img_down_sam, p_roi_in, info_list);
	//fwrite(info_list, sizeof(INFO_LIST), 4096, fod_debug3);

	TIME_DEBUG(usleep(1000*1000));
	int retry_count = 100;
retry:
	printf("Test: is_single_ref %d, raw %d x %d, ds %d x %d\n", is_single_ref, width, height, width_ds, height_ds);
	for (i = 1; i < frame - 1; i++)
	{
		fread(datain, sizeof(unsigned char), frame_size_y, fidin);
		fread(datain_uv, sizeof(unsigned char), frame_size_uv, fidin);

		rga_ds(datain, img_down_sam, width, height, width_ds, height_ds);
		memset(info_list, 0, sizeof(info_list));
		TIME_DEBUG(move_detection(md_ctx, img_down_sam, p_roi_in, info_list));
		//fwrite(info_list, sizeof(INFO_LIST), 4096, fod_debug3);
	}
	if (retry_count-- > 0) {
		retry_count = 1000;
		fseek(fidin, 0L, SEEK_SET);
		goto retry;
	}

	move_detection_deinit(md_ctx);
	fclose(fidin);
	//fclose(fod_debug3);

	return 0;
}

#endif


void rga_ds(unsigned char* pin, unsigned char* pout, int w, int h, int w_ds, int h_ds)
{
	int i, j;

	if(3*w_ds != w)
		assert(0);
	if(3*h_ds != h)
		assert(0);

	for (i = 0; i < h_ds; i++)
	{
		for (j = 0; j < w_ds; j++)
		{
			pout[i*w_ds + j] = (pin[(i * 3 + 0)*w_ds * 3 + j * 3 + 0] + pin[(i * 3 + 0)*w_ds * 3 + j * 3 + 1] + pin[(i * 3 + 0)*w_ds * 3 + j * 3 + 2] +
				pin[(i * 3 + 1)*w_ds * 3 + j * 3 + 0] + pin[(i * 3 + 1)*w_ds * 3 + j * 3 + 1] + pin[(i * 3 + 1)*w_ds * 3 + j * 3 + 2] +
				pin[(i * 3 + 2)*w_ds * 3 + j * 3 + 0] + pin[(i * 3 + 2)*w_ds * 3 + j * 3 + 1] + pin[(i * 3 + 2)*w_ds * 3 + j * 3 + 2]) / 9;
		}
	}
}

#include<pthread.h>
void *thr_fn(void *arg)
{
	printf("new thread\n");
	hw_simu_top();
}

//C:\Users\rock\Desktop\ipc\1080\isp_test_2.yuv 1080 1920 isp_test_2.yuv C:\Users\rock\Desktop\rk/960x540\isp_test_2.yuv
int main(int argc, char** argv) {
	int threads = 1;
	pthread_t *tid;

	if (argc != 3) {
		printf("example: \n \
				test_md 0 2: test multi ref with 2 thread\n \
				test_md 1 2: test single ref with 2 thread\n");
		return -1;
	}
	is_single_ref = atoi(argv[1]);
	threads = atoi(argv[2]);
	threads = (threads < 1) ? 1 : threads;

	tid = malloc(sizeof(pthread_t) * threads);

	for (int i = 0; i < threads; i++) {
		int err = pthread_create(&tid[i], NULL, thr_fn, NULL);
		if (err != 0) {
			printf("can't create thread: %s\n", strerror(err));
			return -1;
		}
	}

	for (int i = 0; i < threads; i++)
		pthread_join(tid[i],NULL);

	free(tid);
	return 0;
}
