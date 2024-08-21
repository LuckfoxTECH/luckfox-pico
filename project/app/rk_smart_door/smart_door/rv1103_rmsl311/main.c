// Copyright 2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// #include "smart_door.h"
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "face_recognition.h"
#include "rk_debug.h"
#include "uart.h"
#include "v4l2_isp_luma.h"

#define PRINT(fmt, ...) \
  do{ \
    printf("[%s:%d] "fmt, __func__, __LINE__, ##__VA_ARGS__); \
    printf("\n"); \
  } while(0)

int main(int argc, char *argv[]) {
    FILE *fp = NULL;
    const char *ini_file = "/tmp/smart_door.ini";
    char *face_sensor_name = "m00_b_sc035gs 4-0030";
    char *face_mdev_path_cif_ir = "/dev/media0";
    char *face_mdev_path_isp_flood = "/dev/media1";
    char *face_mdev_path_isp_pro = "/dev/media2";
    char cmd_line[20];
    pthread_t face_thread = 0;
    struct face_recogniton_arg face_arg;

    face_arg.sensor_name = face_sensor_name;
    face_arg.mdev_path_cif_ir = face_mdev_path_cif_ir;
    face_arg.mdev_path_isp_flood = face_mdev_path_isp_flood;
    face_arg.mdev_path_isp_pro = face_mdev_path_isp_pro;

    /* link vir isp before all video stream on */
    v4l2_isp_luma_link_init(face_mdev_path_isp_flood, face_mdev_path_isp_pro);

    if ((fp = fopen(ini_file, "w+")) == NULL) {
        PRINT("open ini file failed!");
        goto exit;
    }

    while(1) {
        memset(cmd_line, 0, sizeof(cmd_line));
        fseek(fp, 0, SEEK_SET);
        fgets(cmd_line, 20, fp);
        truncate(ini_file, 0);

        if (strstr(cmd_line, "face_start")) {
            if (face_thread)
                continue;

            PRINT("smart_door_face_recogniton_start running");
            pthread_create(&face_thread, 0, (void *)smart_door_face_recogniton_start, &face_arg);
        } else if (strstr(cmd_line, "face_stop")) {
            if (!face_thread)
                continue;

            PRINT("smart_door_face_recogniton stop!");
            smart_door_face_recogniton_stop();
            pthread_join(face_thread, NULL);
            face_thread = 0;
        } else if (strstr(cmd_line, "uart")) {
            PRINT("smart_door_uart running");
            smart_door_uart();
        } else if (strstr(cmd_line, "exit")) {
            PRINT("smart_door exit!");
            remove(ini_file);
            break;
        } else {
            usleep(1000000llu);
        }
    }

exit:
    if (fp) {
        fclose(fp);
        fp = NULL;
    }

    return RK_SUCCESS;
}

