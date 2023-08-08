#include "rk_time.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int ret;
    struct tm *input_time = NULL;
    struct tm *output_time = NULL;
    input_time = (struct tm *)malloc(sizeof(struct tm));
    if (!input_time) {
        printf("Malloc input struct tm fail!\n");
        return -1;
    }

    output_time = (struct tm *)malloc(sizeof(struct tm));
    if (!output_time) {
        printf("Malloc output struct tm fail!\n");
        return -1;
    }
    memset(input_time, 0, sizeof(struct tm));
    memset(output_time, 0, sizeof(struct tm));

    ret = rk_system_get_time(output_time);
    if (ret)
        printf("rk_system_get_time fail!\n");
    else
        printf("current time : %04d-%02d-%02d %02d:%02d:%02d\n",
            output_time->tm_year + 1900,output_time->tm_mon + 1, output_time->tm_mday,
            output_time->tm_hour, output_time->tm_min, output_time->tm_sec);

    input_time->tm_year = 2020;
    input_time->tm_mon = 2;
    input_time->tm_mday = 18;
    input_time->tm_hour = 9;
    input_time->tm_min = 0;
    input_time->tm_sec = 0;

    ret = rk_system_set_time(input_time);
    if (ret)
        printf("rk_system_set_time fail!\n");
    printf("SET TIME=%04d-%02d-%02d,%02d:%02d:%02d\n",input_time->tm_year, input_time->tm_mon,
        input_time->tm_mday, input_time->tm_hour, input_time->tm_min, input_time->tm_sec);


    memset(input_time, 0, sizeof(struct tm));
    memset(output_time, 0, sizeof(struct tm));
    input_time->tm_hour = 9;
    input_time->tm_min = 2;
    input_time->tm_sec = 0;
    ret = rk_system_set_alarm(input_time);
    if (ret) {
        printf("rk_system_set_alarm fail\n");
        return -1;
    }
    ret = rk_system_get_alarm(output_time);
    if (ret) {
        printf("rk_system_get_alarm fail\n");
        return -1;
    } else
        printf("alarm time : %02d:%02d:%02d\n", output_time->tm_hour,output_time->tm_min, output_time->tm_sec);
    ret = rk_system_enable_alarm();
    if (ret) {
        printf("rk_system_enable_alarm fail\n");
        return -1;
    }
    //ret = rk_system_disable_alarm();
    //if (ret)
        //printf("rk_system_disable_alarm fail\n");
    ret = rk_system_wait_alarm(500);
    if (ret)
        printf("rk_system_wait_alarm fail\n");
    if (input_time)
        free(input_time);
    if (output_time)
        free(output_time);
    return 0;
}

