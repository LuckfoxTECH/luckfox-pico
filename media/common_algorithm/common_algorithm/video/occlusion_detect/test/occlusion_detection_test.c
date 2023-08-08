#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <unistd.h>

#include "../include/occlusion_detection.h"

int main(int argc, char** argv) {
	unsigned char *datain = NULL;
	int ret = 0;
	int frame_cnt = 0;
	int frame_size = 0;
	int width, height;
	od_ctx od_ctx = NULL;
	OD_ROI_INFO roi_in[10] = {0};
    const char *yuv_input = NULL;

	width = 1920;
	height = 1080;
	yuv_input = "/data/1080p.nv12";

	FILE *infile = fopen(yuv_input, "rb");
	if (!infile) {
		printf("ERROR: invalid input path:%s\n", yuv_input);
		return -1;
	}

	frame_size = width * height * 3 / 2;
	datain = (unsigned char *)malloc(frame_size);
	if (!datain) {
		printf("ERROR: malloc failed! size:%d\n", frame_size);
		return -1;
	}

	od_ctx = occlusion_detection_init(width, height);
	if (!od_ctx) {
		printf("ERROR: occlusion_detection_init failed!\n");
		return -1;
	}

	while (1) {
		ret = fread(datain, 1, frame_size, infile);
		if (ret != frame_size) {
			printf("INFO: Get end of file!\n");
			break;
		}

		roi_in[0].flag = 1;
		roi_in[0].up_left[0] = 0; // y
		roi_in[0].up_left[1] = 0; // x
		roi_in[0].down_right[0] = height; // y
		roi_in[0].down_right[1] = width; // x
		roi_in[0].occlusion = 0;

		ret = occlusion_detection(od_ctx, datain, roi_in, 1);
		if (ret) {
			printf("ERROR: od_detection error!\n");
			break;
		}

		printf("[OD RESULT]: frame-%03d occlusion:%d\n", frame_cnt++, roi_in[0].occlusion);
		usleep(33333);
	}

	occlusion_detection_deinit(od_ctx);
	fclose(infile);

	return 0;
}
