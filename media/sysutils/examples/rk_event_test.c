/*
 * Copyright (c) 2022 rockchip
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "rk_event.h"
#include <string.h>
#include <unistd.h>
#include <linux/input.h>
#include <pthread.h>
#include <sys/time.h>

void main_event_handler_t(struct input_event *eventcb) {
    if (eventcb == NULL) {
        printf("eventcb is null\n");
        return;
    }
    printf("[eventval:sec:%ld, usec:%ld, type:%d, code:%d, value:%d]\n", eventcb->time.tv_sec,
    eventcb->time.tv_usec, eventcb->type, eventcb->code, eventcb->value);
}

int main() {
    bool is_quit = true;

    char str1[] = "/dev/input/event0";
    char str2[] = "/dev/input/event1";
    char str3[] = "/dev/input/event2";
    char str4[] = "/dev/input/event3";
    rk_event_register(str1);
    rk_event_register(str2);
    rk_event_register(str3);

    rk_event_unregister(str4);

    rk_event_listen_start(main_event_handler_t);

    char cmd[64];
    printf("\n#Usage: input 'quit' to exit programe!\n");

    while (is_quit) {
        fgets(cmd, sizeof(cmd), stdin);
        if (strstr(cmd, "quit")) {
          printf("#Get 'quit' cmd!\n");
          break;
        }
        usleep(500000);
    }
    rk_event_listen_stop();

    return 0;
}
