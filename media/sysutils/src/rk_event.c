/*
 * Copyright (c) 2022 rockchip
 *
 */
#include "rk_event.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <linux/input.h>

#define DEV_PATH_MAX_LEN 128

typedef struct dev_info {
    int dev_fd;
    char *dev_path;
    struct dev_info *dev_next;
}DEV_INFO;

typedef struct event_manager_t{
    DEV_INFO *first;
    DEV_INFO *last;
    int dev_num;
}EVENT_MANAGER_T;

static int event_quit;
static pthread_t event_tid = 0;
static pthread_mutex_t event_mutex;
static int epoll_fd = -1;
struct epoll_event *evs = NULL, ev;
event_handler_t dev_event_handler = NULL;
static struct event_manager_t *event_manager = NULL;

int rk_event_register(char *dev_path) {
    int ret;
    if(event_tid) {
        printf("event_register must befor event_listen_start\n");
        return -1;
    }

    if (!dev_path) {
        printf("event_register fail,dev_path is null\n");
        return -1;
    }

    if(epoll_fd < 0) {
        pthread_mutex_init(&event_mutex, NULL);
        if (!event_manager) {
            event_manager = (struct event_manager_t *)malloc(sizeof(struct event_manager_t));
            if(!event_manager) {
                printf("malloc event_manager fail\n");
                return -1;
            } else {
                memset(event_manager, 0, sizeof(struct event_manager_t));
                event_manager->first = NULL;
                event_manager->last = NULL;
                event_manager->dev_num = 0;
            }
        }
        epoll_fd = epoll_create1(EPOLL_CLOEXEC);
        if (epoll_fd < 0) {
            printf("epoll create failed\n");
            return -1;
        }
    }

    if (event_manager->first) {
        DEV_INFO *tmp = event_manager->first;
        while (tmp != NULL) {
            if(strcmp(dev_path, tmp->dev_path) == 0) {
                printf("dev_path has been registered!\n");
                return -1;
            }
            tmp = tmp->dev_next;
        }
    }

    DEV_INFO *dev_elm = NULL;
    dev_elm = (DEV_INFO *)malloc(sizeof(DEV_INFO));
    if (!dev_elm) {
        printf("malloc dev_info struct fail\n");
        return -1;
    }
    memset(dev_elm, 0 ,sizeof(DEV_INFO));
    dev_elm->dev_path = NULL;
    dev_elm->dev_next = NULL;

    dev_elm->dev_path = (char *)malloc(DEV_PATH_MAX_LEN);
    if(!dev_elm->dev_path) {
        printf("save dev_path fail\n");
        goto fail;
    }
    memset(dev_elm->dev_path, 0, DEV_PATH_MAX_LEN);
    strcpy(dev_elm->dev_path, dev_path);

    dev_elm->dev_fd = open(dev_path, O_RDONLY | O_NONBLOCK);
    if (dev_elm->dev_fd < 0) {
        printf("failed open %s\n", dev_path);
        if (dev_elm->dev_path) {
            free(dev_elm->dev_path);
            dev_elm->dev_path = NULL;
        }
        goto fail;
    }

    ev.data.fd = dev_elm->dev_fd;
    ev.events = EPOLLIN | EPOLLET;
    ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, dev_elm->dev_fd, &ev);
    if(ret == -1) {
        printf("epoll_ctl fail errno: %s\n", strerror(errno));
        if (dev_elm->dev_path) {
            free(dev_elm->dev_path);
            dev_elm->dev_path = NULL;
        }
        close(dev_elm->dev_fd);
        goto fail;
    }

    if (event_manager->dev_num == 0) {
        if(!event_manager->first) {
            event_manager->first =
            event_manager->last =
            dev_elm;
            event_manager->dev_num++;
        }
    } else if(event_manager->dev_num > 0) {
        event_manager->last->dev_next = dev_elm;
        event_manager->dev_num++;
        event_manager->last = dev_elm;
    }

    return 0;

fail:
    if (dev_elm)
        free(dev_elm);
    return -1;
}

int rk_event_unregister(char *dev_path) {
    if (!dev_path) {
        printf("event_unregister fail,dev_path is null\n");
        return -1;
    }

    if (event_manager->first) {
        DEV_INFO *pre_tmp = NULL;
        DEV_INFO *tmp = event_manager->first;
        while (tmp != NULL) {
            if(strcmp(dev_path, tmp->dev_path) == 0) {
                pthread_mutex_lock(&event_mutex);
                ev.data.fd = tmp->dev_fd;
                ev.events = EPOLLIN;
                epoll_ctl(epoll_fd, EPOLL_CTL_DEL, tmp->dev_fd, &ev);
                close(tmp->dev_fd);
                if (event_manager->dev_num > 1) {
                    if(pre_tmp) {
                        if (tmp->dev_next == NULL) {
                            event_manager->last = pre_tmp;
                            event_manager->last->dev_next = NULL;
                        } else
                            pre_tmp->dev_next = tmp->dev_next;
                    }else
                       event_manager->first = tmp->dev_next;
                }
                if(tmp->dev_path)
                    free(tmp->dev_path);
                if(tmp)
                    free(tmp);
                event_manager->dev_num--;
                if(event_manager->dev_num == 0)
                    event_manager->first =
                    event_manager->last =
                    NULL;
                pthread_mutex_unlock(&event_mutex);
                return 0;
            }
            pre_tmp = tmp;
            tmp = tmp->dev_next;
        }
    }
    printf("dev_path isn't registered\n");
    return 0;
}

static int rk_event_read(struct epoll_event *ev) {
    struct input_event input_ev;
    while (read(ev->data.fd, &input_ev, sizeof(struct input_event)) > 0) {
            if(!input_ev.type && !input_ev.value && !input_ev.code)
                continue;
            if(dev_event_handler)
                dev_event_handler(&input_ev);
    }
    return 0;
}

static void *rk_event_recthread(void *arg) {
    int ret, i;

    while (event_quit == 0) {
        ret = epoll_wait(epoll_fd, evs, event_manager->dev_num, -1);
        for (i = 0; i < ret; i ++) {
            if (evs[i].events & EPOLLIN)
                rk_event_read(&evs[i]);
        }
    }
    return NULL;
}

int rk_event_listen_start(event_handler_t handler) {
    if (!handler) {
        printf("rk_event_listen_start: handler is null\n");
        return -1;
    }

    pthread_mutex_lock(&event_mutex);
    dev_event_handler = handler;
    if (event_manager->dev_num > 0 && evs == NULL) {
        evs = (struct epoll_event *)calloc(event_manager->dev_num, sizeof(struct epoll_event));
        if(!evs) {
            printf("calloc epoll_event array fail\n");
            return -1;
        }
    }

    event_quit = 0;
    if (pthread_create(&event_tid, NULL,
        rk_event_recthread, NULL)) {
        printf("rk_event_recthread create failed!");
        return -1;
    }
    pthread_mutex_unlock(&event_mutex);
    return 0;
}

int rk_event_listen_stop(void) {
    pthread_mutex_lock(&event_mutex);
    epoll_fd = -1;
    event_quit = 1;
    dev_event_handler = NULL;

    if(evs) {
        free(evs);
        evs = NULL;
    }
    
    if (event_manager->first) {
            DEV_INFO *tmp = event_manager->first;
            DEV_INFO *tmp_next = NULL;
            while (tmp != NULL) {
                tmp_next = tmp->dev_next;
                close(tmp->dev_fd);
                if(tmp->dev_path)
                    free(tmp->dev_path);
                if(tmp)
                    free(tmp);
                tmp = tmp_next;
                event_manager->dev_num--;
            }
    }
    if (event_manager) {
        free(event_manager);
        event_manager = NULL;
    }

    if (event_tid) {
        pthread_cancel(event_tid);
        if (pthread_join(event_tid, NULL)) {
            printf("rk_event_recthread join failed!");
            pthread_mutex_unlock(&event_mutex);
            return -1;
        }
        event_tid = 0;
    }
    pthread_mutex_unlock(&event_mutex);
    pthread_mutex_destroy(&event_mutex);
    return 0;
}

