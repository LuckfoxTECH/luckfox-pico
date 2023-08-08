//#if BUILD_SIMULATOR
#include "sample_gfx.h"
#include "sample_file.h"
#include "sample_define.h"
#include "rk_comm_ive.h"

#if BUILD_SIMULATOR
#include <stdio.h>
#include <stdarg.h>
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;

int verify_current_frame = 0;

cv::VideoCapture cap;
cv::VideoWriter writer;
#endif

void gfx_show_img(const char* name, void* _img_data, int pic_width, int pic_height, int type, int delay){
#if BUILD_SIMULATOR
    if (type == IVE_IMAGE_TYPE_U8C1) {
        type = CV_8UC1;
    } else if (type == IVE_IMAGE_TYPE_U8C3_PACKAGE) {
        type = CV_8UC3;
    }
    Mat image(pic_height, pic_width, type, _img_data);
    cv::imshow(name, image);
    cv::waitKey(delay);
#endif
}

int gfx_query_frame(RK_U32* width, RK_U32* height, RK_U32* totalFrame, const char* fmt, ...) {
#if BUILD_SIMULATOR
    char file_name[256];
    va_list args;

    va_start(args, fmt);
    RK_VSPRINTF(file_name, RK_MAX_FILE_LEN, fmt, args);
    va_end(args);

    bool is_opened;

    if (strstr(file_name, "avi") != NULL ||
               strstr(file_name, "mp4") != NULL) {
        if (!cap.isOpened()) {
            is_opened = cap.open(file_name);
            if (!is_opened) {
                printf("open video(%s) failed!", file_name);
                return -1;
            }
        }

        verify_current_frame++;
        *width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
        *height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
        *totalFrame = cap.get(CV_CAP_PROP_FRAME_COUNT);
    }
#endif
    return 0;
}

int gfx_read_frame(void* data, int size, const char* fmt, ...) {
    int total_frames = 0;
#if BUILD_SIMULATOR
    char file_name[256];
    va_list args;

    va_start(args, fmt);
    RK_VSPRINTF(file_name, RK_MAX_FILE_LEN, fmt, args);
    va_end(args);

    bool is_opened;
    int file_len = size;

    if (strstr(file_name, "avi") != NULL ||
               strstr(file_name, "mp4") != NULL) {
        if (!cap.isOpened()) {
            is_opened = cap.open(file_name);
            if (!is_opened) {
                printf("open video(%s) failed!", file_name);
                return -1;
            }
        }

        verify_current_frame++;
        total_frames = cap.get(CV_CAP_PROP_FRAME_COUNT);
        if (verify_current_frame == total_frames) {
            verify_current_frame = 0;
            cap.release();
        }

        Mat frame;
        cap >> frame;
        if (frame.empty()) {
            printf("%s:capture video frame empty, exit directly.", __func__);
            return -1;
        } else {
            if (size == frame.cols * frame.rows) {
                cvtColor(frame, frame, CV_BGR2GRAY);
            }
            memcpy(data, frame.data, file_len);
        }
    }
#endif
    return total_frames;
}

int gfx_write_frame(void* data, int width, int height, int type, int flag, const char* fmt, ...) {
    int ret = 0;
#if BUILD_SIMULATOR
    char file_name[256];
    va_list args;

    if (data == NULL || fmt == NULL) {
        printf("invalid parameters!");
        return -1;
    }

    va_start(args, fmt);
    RK_VSPRINTF(file_name, RK_MAX_FILE_LEN, fmt, args);
    va_end(args);

    Mat mat;
    if (strstr(file_name, "avi") != NULL || strstr(file_name, "mp4") != NULL) {
        if (!writer.isOpened()) {
            int rate = 25;
            //const char *file = "../../output/gmm.mp4";
            remove(file_name);
            writer.open(file_name,
                        CV_FOURCC('M', 'J', 'P', 'G'),
                        //CV_FOURCC('M', 'P', '4', 'V'), CV_FOURCC('M', 'J', 'P', 'G'),//CV_FOURCC('D', 'I', 'V', '3'),
                        rate,
                        Size(width, height), true);
            if (!writer.isOpened()) {
                return -1;
            }
        }
        if (type == IVE_IMAGE_TYPE_U8C1) {
            mat = Mat(height, width, CV_8UC1, data);
            cvtColor(mat, mat, CV_GRAY2BGR);
        } else if (type == IVE_IMAGE_TYPE_U8C3_PACKAGE) {
            mat = Mat(height, width, CV_8UC3, data);
        }

        writer << mat;
        if (flag == 1) {
            writer.release();
        }
    }
#endif
    return ret;
}
//#endif

