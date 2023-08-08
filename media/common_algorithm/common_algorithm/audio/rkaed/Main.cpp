#ifdef _MSC_VER
#include "audio/wave_reader.h"
#include "audio/wave_writer.h"
#include "skv/rkaudio_sed.h"
#else
#include "wave_reader.h"
#include "wave_writer.h"
#include "rkaudio_sed.h"
#endif


#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <assert.h>
#include <iostream>

#define IN_SIZE 256

double clk_sed;
static clock_t clk_start, clk_end;
int g_mem_cost;
typedef unsigned char SKV_BYTE;

int main(int argc, char* argv[])
{
	int ret = 0;
	double Total_time_s = 0.0, Total_time_e = 0.0;
	double Total_sample = 0.0;
	double Tmp_sample = 0.0;
	// 读取数据并处理
	clock_t startTime, endTime;
#if 1
	// 输入参数
	argc = 3;
#if ENABLE_8k
	argv[1] = (char*)"babycry_8k.wav";
	argv[2] = (char*)"babycry_8k_out.wav";
#else
	argv[1] = (char*)"total.wav";
	argv[2] = (char*)"total_out.wav";
#endif
#endif
	/* For Debug  */
	int aed_stat = 0;
	int out_size = 0, in_size = 0, res = 0;
	int bcd_stat = 0, bcd_stat_old = 0;
	if (argc < 3)
	{
		printf("Error: Wrong input parameters! A example is as following: \n");
		printf("fosafer_enh test_in.wav test_out.wav 0\n");
		exit(-1);
	}

	char* in_filename = argv[1];
	char* out_filename = argv[2];
	// for wave reader
	wave_reader* wr;
	wave_reader_error rerror;

	// for wave writer
	wave_writer* ww;
	wave_writer_error werror;
	wave_writer_format format;

	// 读取输入音频
	wr = wave_reader_open(in_filename, &rerror);
	if (!wr) {
		printf("rerror=%d\n", rerror);
		return -1;
	}

	int mSampleRate = wave_reader_get_sample_rate(wr);
	int mBitPerSample = wave_reader_get_sample_bits(wr);
	int mNumChannel = wave_reader_get_num_channels(wr);

	// 输入检查
	if (mNumChannel > 1) {
		printf("This algorithm is a single channel algorithm and will run on the first channel of data\n");
	}

	// 每次读取数据大小    
	int read_size = IN_SIZE * mNumChannel * mBitPerSample / 8;
	SKV_BYTE* in = (SKV_BYTE*)malloc(read_size * sizeof(SKV_BYTE));
	int out_res_num = 5;
	short* out = (short*)malloc(IN_SIZE * out_res_num * sizeof(short));
	// 输出音频格式设置
	format.num_channels = 5;
	format.sample_rate = mSampleRate;
	format.sample_bits = mBitPerSample;
	ww = wave_writer_open(out_filename, &format, &werror);
	// 输出音频建立失败
	if (!ww)
	{
		printf("werror=%d\n", werror);
		wave_reader_close(wr);
		return -1;
	}

	// 声音事件检测初始化
	RKAudioSedRes sed_res;
	RKAudioSedParam* sed_param = rkaudio_sed_param_init();
	void* st_sed = rkaudio_sed_init(mSampleRate, mBitPerSample, mNumChannel, sed_param);
	rkaudio_sed_param_destroy(sed_param);
	if (st_sed == NULL) {
		printf("Failed to create baby cry handle\n");
		return -1;
	}

	startTime = clock();
	int cnt = 0;
	while (0 < (res = wave_reader_get_samples(wr, IN_SIZE, in)))
	{
		in_size = res * (mBitPerSample / 8) * mNumChannel;
		cnt++;
		clk_start = clock();
		out_size = rkaudio_sed_process(st_sed, (short*)in, in_size / 2, &sed_res);
        if (out_size < 0)
			fprintf(stderr, "bcd process return error=%d\n", out_size);
        
		clk_end = clock();
		clk_sed += clk_end - clk_start;


		//printf("lsd=%d,snr=%d,bcd=%d,buz_res=%d,gbs_res=%d\n", sed_res.lsd_res, sed_res.snr_res, sed_res.bcd_res, sed_res.buz_res, sed_res.gbs_res);



		// 输出，测试用
		for (int i = 0; i < IN_SIZE; i++) {
			*(out + out_res_num * i)     = 10000 * sed_res.snr_res;
			*(out + out_res_num * i + 1) = 10000 * sed_res.lsd_res;
			*(out + out_res_num * i + 2) = 10000 * sed_res.bcd_res;
			*(out + out_res_num * i + 3) = 10000 * sed_res.buz_res;
			*(out + out_res_num * i + 4) = 10000 * sed_res.gbs_res;
		}
		wave_writer_put_samples(ww, IN_SIZE, out);
		Total_sample += in_size / 2 / mNumChannel;
		//if (cnt % 63 == 0)
		//	printf("cnt = %d\n", cnt);
	}
	endTime = clock();

	printf("Finished, speech_time = %f, cost_time = %f\n", \
		Total_sample / mSampleRate, (double)(endTime - startTime) / CLOCKS_PER_SEC);

	printf("sed = %f\n", clk_sed / CLOCKS_PER_SEC);
	wave_writer_close(ww, &werror);
	wave_reader_close(wr);

	free(in);
	free(out);

	// 释放
	if (st_sed)
		rkaudio_sed_destroy(st_sed);

	return 0;
}
