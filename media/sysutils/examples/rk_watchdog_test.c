/*
 * Copyright (c) 2022 rockchip
 *
 */
#include "rk_watchdog.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

#define DEFAULT_PING_RATE 1

static int feeddog_quit;
static pthread_t watchdog_tid = 0;

static void *rk_feeddog_thread(void *arg) {
    int i = 0;

    while (feeddog_quit == 0) {
        rk_watchdog_refresh();
        printf(".%d",i);
        i++;
        if (i == 60)
            i = 0;
        sleep(DEFAULT_PING_RATE);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    int ret;
    bool is_quit = true;

    setbuf(stdout, NULL);
    ret = rk_watchdog_start(0);
    if (ret)
        printf("rk_watchdog_start fail!\n");

    feeddog_quit = 0;
    if (pthread_create(&watchdog_tid, NULL,
        rk_feeddog_thread, NULL)) {
        printf("rk_feeddog_thread create failed!");
        goto fail;
    }

    char cmd[64];
    printf("\n#Usage: input 'quit' to exit programe!\n");

    while (is_quit) {
        fgets(cmd, sizeof(cmd), stdin);
        if (strstr(cmd, "quit")) {
          printf("#Get 'quit' cmd!\n");
          break;
        }
        printf("alive\n");
        usleep(500000);
    }

    if (watchdog_tid) {
        feeddog_quit = 1;
        pthread_cancel(watchdog_tid);
        if (pthread_join(watchdog_tid, NULL)) {
            printf("rk_feeddog_thread join failed!");
            return -1;
        }
        watchdog_tid = 0;
    }

fail:
    ret = rk_watchdog_stop();
    return 0;
}

