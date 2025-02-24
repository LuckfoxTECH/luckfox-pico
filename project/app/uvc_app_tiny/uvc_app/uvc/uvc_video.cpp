/*
 * Copyright (C) 2019 Rockchip Electronics Co., Ltd.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL), available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "uvc_video.h"
#include "uvc-gadget.h"
#include "uvc_log.h"

#include <fcntl.h>
#include <list>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/prctl.h>
#if UVC_DYNAMIC_DEBUG_FPS
struct uvc_debug_info_def uvc_debug_info;
#endif

struct uvc_buffer_list {
  std::list<struct uvc_buffer *> buffer_list;
  pthread_mutex_t mutex;
};

struct video_uvc {
  struct uvc_buffer_list write;
  struct uvc_buffer_list read;
  struct uvc_buffer_list all;
  struct uvc_buffer_list cache;
  // struct uvc_buffer *cache;
  pthread_t id;
  bool run;
  int video_id;
};

static std::list<struct uvc_video *> lst_v;
static pthread_mutex_t mtx_v = PTHREAD_MUTEX_INITIALIZER;

static struct uvc_buffer *uvc_buffer_create(int width, int height,
                                            struct uvc_video *v) {
  struct uvc_buffer *buffer = NULL;

  buffer = (struct uvc_buffer *)calloc(1, sizeof(struct uvc_buffer));
  if (!buffer)
    return NULL;
  buffer->width = width;
  buffer->height = height;
  buffer->total_size = buffer->width * buffer->height * 2;
  buffer->size = 0; // buffer->total_size;
  buffer->video_id = v->id;

  LOG_INFO("uvc buffer = %p, total_size=%d\n", buffer, buffer->total_size);

  return buffer;
}

static void uvc_buffer_push_back(struct uvc_buffer_list *uvc_buffer,
                                 struct uvc_buffer *buffer) {
  pthread_mutex_lock(&uvc_buffer->mutex);
  uvc_buffer->buffer_list.push_back(buffer);
  pthread_mutex_unlock(&uvc_buffer->mutex);
}

static struct uvc_buffer *
uvc_buffer_pop_front(struct uvc_buffer_list *uvc_buffer) {
  struct uvc_buffer *buffer = NULL;

  pthread_mutex_lock(&uvc_buffer->mutex);
  if (!uvc_buffer->buffer_list.empty()) {
    buffer = uvc_buffer->buffer_list.front();
    uvc_buffer->buffer_list.pop_front();
  }
  pthread_mutex_unlock(&uvc_buffer->mutex);
  return buffer;
}

static struct uvc_buffer *
uvc_buffer_pop_assign_cache(struct uvc_buffer_list *uvc_buffer_l,
                            struct uvc_buffer *assign_buffer) {
  struct uvc_buffer *buffer = NULL;

  pthread_mutex_lock(&uvc_buffer_l->mutex);
  for (std::list<struct uvc_buffer *>::iterator i =
           uvc_buffer_l->buffer_list.begin();
       i != uvc_buffer_l->buffer_list.end(); ++i) {
    buffer = *i;
    if (assign_buffer->fd == buffer->fd &&
        assign_buffer->frame == buffer->frame) {
      uvc_buffer_l->buffer_list.erase(i);
      break;
    }
  }
  LOG_DEBUG(
      "debug info!uvc_buffer_pop_assign_cache=fd:%d frame:%p uvc_buf:%p\n",
      assign_buffer->fd, assign_buffer->frame, buffer);
  pthread_mutex_unlock(&uvc_buffer_l->mutex);
  return buffer;
}

static struct uvc_buffer *uvc_buffer_front(struct uvc_buffer_list *uvc_buffer) {
  struct uvc_buffer *buffer = NULL;

  pthread_mutex_lock(&uvc_buffer->mutex);
  if (!uvc_buffer->buffer_list.empty())
    buffer = uvc_buffer->buffer_list.front();
  else //{printf("buff null\n");
    buffer = NULL;
  //     }
  pthread_mutex_unlock(&uvc_buffer->mutex);
  return buffer;
}

static void _uvc_mpi_buffer_release(uvc_device *dev, struct uvc_buffer *buffer,
                                    bool need_lock) {
  if (buffer->frame) {
    LOG_DEBUG(
        "debug info!uvc_mpi_buffer_destroy fd:%d index:%d buf:%p->mb=%p\n",
        buffer->fd, buffer->index, buffer, buffer->frame);
    if (need_lock)
      uvc_control_release_frame(dev, buffer->frame);
    else
      uvc_control_release_frame_nonlock(dev, buffer->frame);
  }
}

static void uvc_mpi_buffer_destroy(struct uvc_video *v,
                                   struct uvc_buffer_list *uvc_buffer) {
  struct uvc_buffer *buffer = NULL;

  pthread_mutex_lock(&uvc_buffer->mutex);
  while (!uvc_buffer->buffer_list.empty()) {
    buffer = uvc_buffer->buffer_list.front();
    _uvc_mpi_buffer_release(v->dev, buffer, false);
    uvc_buffer->buffer_list.pop_front();
  }
  pthread_mutex_unlock(&uvc_buffer->mutex);
}

static void uvc_buffer_destroy(struct uvc_video *v,
                               struct uvc_buffer_list *uvc_buffer) {
  struct uvc_buffer *buffer = NULL;

  pthread_mutex_lock(&uvc_buffer->mutex);
  while (!uvc_buffer->buffer_list.empty()) {
    buffer = uvc_buffer->buffer_list.front();
    LOG_INFO("free uvc buffer = %p\n", buffer);
    free(buffer);
    uvc_buffer->buffer_list.pop_front();
  }
  pthread_mutex_unlock(&uvc_buffer->mutex);
}

static void uvc_buffer_clear(struct uvc_buffer_list *uvc_buffer) {
  pthread_mutex_lock(&uvc_buffer->mutex);
  uvc_buffer->buffer_list.clear();
  pthread_mutex_unlock(&uvc_buffer->mutex);
}

static void *uvc_gadget_pthread(void *arg) {
  struct uvc_function_config *fc = (struct uvc_function_config *)arg;
  int *id = (int *)arg;

  prctl(PR_SET_NAME, "uvc_gadget_pthread", 0, 0, 0);

  uvc_gadget_main(fc);
  uvc_set_user_run_state(true, fc->video);

  pthread_exit(NULL);
}

int uvc_gadget_pthread_create(struct uvc_function_config *fc) {
  pthread_t *pid = NULL;
  int id = fc->video;
  uvc_memset_uvc_user(id);
  if ((pid = uvc_video_get_uvc_pid(id))) {
    if (pthread_create(pid, NULL, uvc_gadget_pthread, fc)) {
      LOG_ERROR("create uvc_gadget_pthread fail!\n");
      return -1;
    }
  }
  return 0;
}

static int _uvc_video_id_check(int id) {
  int ret = 0;

  if (!lst_v.empty()) {
    for (std::list<struct uvc_video *>::iterator i = lst_v.begin();
         i != lst_v.end(); ++i) {
      struct uvc_video *l = *i;
      if (id == l->id) {
        ret = -1;
        break;
      }
    }
  }

  return ret;
}

int uvc_video_id_check(int id) {
  int ret = 0;

  pthread_mutex_lock(&mtx_v);
  ret = _uvc_video_id_check(id);
  pthread_mutex_unlock(&mtx_v);

  return ret;
}

int uvc_video_id_add(struct uvc_function_config *fc) {
  int ret = 0;
  int id = fc->video;

  LOG_INFO("add uvc video id: %d\n", id);

  pthread_mutex_lock(&mtx_v);
  if (!_uvc_video_id_check(id)) {
    struct uvc_video *v =
        (struct uvc_video *)calloc(1, sizeof(struct uvc_video));
    if (v) {
      v->id = id;
      lst_v.push_back(v);
      pthread_mutex_unlock(&mtx_v);
      uvc_gadget_pthread_create(fc);
      pthread_mutex_lock(&mtx_v);
      pthread_mutex_init(&v->buffer_mutex, NULL);
      pthread_mutex_init(&v->user_mutex, NULL);
      ret = 0;
    } else {
      LOG_ERROR("%s: %d: memory alloc fail.\n", __func__, __LINE__);
      ret = -1;
    }
  } else {
    LOG_WARN("%s: %d: %d already exist.\n", __func__, __LINE__, id);
    ret = -1;
  }
  pthread_mutex_unlock(&mtx_v);

  return ret;
}

void uvc_video_id_remove(int id) {
  pthread_mutex_lock(&mtx_v);
  if (_uvc_video_id_check(id)) {
    for (std::list<struct uvc_video *>::iterator i = lst_v.begin();
         i != lst_v.end(); ++i) {
      struct uvc_video *l = *i;
      if (id == l->id) {
        pthread_mutex_destroy(&l->buffer_mutex);
        pthread_mutex_destroy(&l->user_mutex);
        free(l);
        lst_v.erase(i);
        break;
      }
    }
  }
  pthread_mutex_unlock(&mtx_v);
}

int uvc_video_id_get(unsigned int seq) {
  int ret = -1;

  pthread_mutex_lock(&mtx_v);
  if (!lst_v.empty()) {
    unsigned int cnt = 0;
    for (std::list<struct uvc_video *>::iterator i = lst_v.begin();
         i != lst_v.end(); ++i) {
      if (cnt++ == seq) {
        struct uvc_video *l = *i;
        ret = l->id;
        break;
      }
    }
  }
  pthread_mutex_unlock(&mtx_v);

  return ret;
}

static void uvc_gadget_pthread_exit(int id);

static int uvc_video_id_exit(int id) {
  if (uvc_video_id_check(id)) {
    uvc_gadget_pthread_exit(id);
    uvc_video_id_remove(id);
    return 0;
  }

  return -1;
}

static int _uvc_video_id_exit_all() {
  int ret = -1;

  pthread_mutex_lock(&mtx_v);
  if (!lst_v.empty()) {
    struct uvc_video *l = lst_v.front();
    pthread_mutex_unlock(&mtx_v);
    uvc_video_id_exit(l->id);
    pthread_mutex_lock(&mtx_v);
    ret = 0;
  }
  pthread_mutex_unlock(&mtx_v);

  return ret;
}

void uvc_video_id_exit_all() {
  while (!_uvc_video_id_exit_all())
    continue;
}

static void _uvc_video_set_uvc_process(struct uvc_video *v, bool state) {
  v->uvc_process = state;
}

void uvc_video_set_uvc_process(int id, bool state) {
  pthread_mutex_lock(&mtx_v);
  if (_uvc_video_id_check(id)) {
    for (std::list<struct uvc_video *>::iterator i = lst_v.begin();
         i != lst_v.end(); ++i) {
      struct uvc_video *l = *i;
      if (id == l->id) {
        _uvc_video_set_uvc_process(l, state);
        break;
      }
    }
  }
  pthread_mutex_unlock(&mtx_v);
}

static bool _uvc_video_get_uvc_process(struct uvc_video *v) {
  return v->uvc_process;
}

bool uvc_video_get_uvc_process(int id) {
  bool state = false;

  pthread_mutex_lock(&mtx_v);
  if (_uvc_video_id_check(id)) {
    for (std::list<struct uvc_video *>::iterator i = lst_v.begin();
         i != lst_v.end(); ++i) {
      struct uvc_video *l = *i;
      if (id == l->id) {
        state = _uvc_video_get_uvc_process(l);
        break;
      }
    }
  }
  pthread_mutex_unlock(&mtx_v);

  return state;
}

pthread_t *uvc_video_get_uvc_pid(int id) {
  pthread_t *tid = NULL;

  pthread_mutex_lock(&mtx_v);
  if (_uvc_video_id_check(id)) {
    for (std::list<struct uvc_video *>::iterator i = lst_v.begin();
         i != lst_v.end(); ++i) {
      struct uvc_video *l = *i;
      if (id == l->id) {
        tid = &l->uvc_pid;
        break;
      }
    }
  }
  pthread_mutex_unlock(&mtx_v);

  return tid;
}

void uvc_video_join_uvc_pid(int id) {
  pthread_mutex_lock(&mtx_v);
  if (_uvc_video_id_check(id)) {
    for (std::list<struct uvc_video *>::iterator i = lst_v.begin();
         i != lst_v.end(); ++i) {
      struct uvc_video *l = *i;
      if (id == l->id) {
        if (l->uvc_pid) {
          pthread_mutex_unlock(&mtx_v);
          pthread_join(l->uvc_pid, NULL);
          l->uvc_pid = 0;
          pthread_mutex_lock(&mtx_v);
        }
      }
    }
  }
  pthread_mutex_unlock(&mtx_v);
}

static void uvc_gadget_pthread_exit(int id) {
  while (!uvc_get_user_run_state(id))
    pthread_yield();
  uvc_set_user_run_state(false, id);
  uvc_video_join_uvc_pid(id);
}

static void _uvc_get_user_resolution(struct uvc_video *v, int *width,
                                     int *height);

static int _uvc_buffer_init(struct uvc_video *v) {
  int i = 0;
  int ret = 0;
  struct uvc_buffer *buffer = NULL;
  int width, height;

  _uvc_get_user_resolution(v, &width, &height);

  pthread_mutex_lock(&v->buffer_mutex);

  v->uvc = new video_uvc();
  if (!v->uvc) {
    ret = -1;
    goto exit;
  }
  v->uvc->id = 0;
  v->uvc->video_id = v->id;
  v->uvc->run = 1;
  pthread_mutex_init(&v->uvc->write.mutex, NULL);
  pthread_mutex_init(&v->uvc->read.mutex, NULL);
  pthread_mutex_init(&v->uvc->all.mutex, NULL);
  pthread_mutex_init(&v->uvc->cache.mutex, NULL);

  uvc_buffer_clear(&v->uvc->write);
  uvc_buffer_clear(&v->uvc->read);
  uvc_buffer_clear(&v->uvc->all);
  uvc_buffer_clear(&v->uvc->cache);

  LOG_INFO("uvc_buf_cnt = %d\n", v->uvc_buf_cnt);
  for (i = 0; i < v->uvc_buf_cnt; i++) {
    buffer = uvc_buffer_create(width, height, v);
    if (!buffer) {
      ret = -1;
      goto exit;
    }
    uvc_buffer_push_back(&v->uvc->write, buffer);
    uvc_buffer_push_back(&v->uvc->all, buffer);
  }
  _uvc_video_set_uvc_process(v, true);
  v->can_exit = true;

exit:
  pthread_mutex_unlock(&v->buffer_mutex);
  return ret;
}

int uvc_buffer_init(struct uvc_device *dev) {
  int ret = -1;
  int id = dev->video_id;
  pthread_mutex_lock(&mtx_v);
  if (_uvc_video_id_check(id)) {
    for (std::list<struct uvc_video *>::iterator i = lst_v.begin();
         i != lst_v.end(); ++i) {
      struct uvc_video *l = *i;
      if (id == l->id) {
        l->dev = dev;
        l->uvc_buf_cnt = dev->nbufs;
        ret = _uvc_buffer_init(l);
        break;
      }
    }
  }
  pthread_mutex_unlock(&mtx_v);

  return ret;
}

static void _uvc_buffer_deinit(struct uvc_video *v) {
  pthread_mutex_lock(&v->buffer_mutex);
  if (v->uvc) {
    v->uvc->run = 0;
    _uvc_video_set_uvc_process(v, false);
    LOG_INFO("_uvc_buffer_deinit read\n");
    uvc_mpi_buffer_destroy(
        v, &v->uvc->read); // when close uvc or mpi get ok, will push to read.
    LOG_INFO("_uvc_buffer_deinit cache\n");
    uvc_mpi_buffer_destroy(v, &v->uvc->cache); // at uvc driver buf
    LOG_INFO("_uvc_buffer_deinit all\n");
    uvc_buffer_destroy(v, &v->uvc->all);
    pthread_mutex_destroy(&v->uvc->write.mutex);
    pthread_mutex_destroy(&v->uvc->read.mutex);
    pthread_mutex_destroy(&v->uvc->all.mutex);
    pthread_mutex_destroy(&v->uvc->cache.mutex);
    delete v->uvc;
    v->uvc = NULL;
  }
  pthread_mutex_unlock(&v->buffer_mutex);
}

void uvc_buffer_deinit(int id) {
  struct uvc_video *l;
  if (_uvc_video_id_check(id)) {
    for (std::list<struct uvc_video *>::iterator i = lst_v.begin();
         i != lst_v.end(); ++i) {
      l = *i;
      if (id == l->id) {
        break;
      }
    }
  }
  int wait_cnt = 0;
  while (l->uvc_process && !l->can_exit) {
    wait_cnt++;
    usleep(1000);
    if (wait_cnt > 30) {
      LOG_INFO("uvc_buffer_deinit timeout 30ms,force exit!\n");
      break;
    }
  }

  // pthread_mutex_lock(&mtx_v);
  _uvc_buffer_deinit(l);
  // pthread_mutex_unlock(&mtx_v);
}

static bool _uvc_buffer_write_enable(struct uvc_video *v) {
  bool ret = false;

  if (pthread_mutex_trylock(&v->buffer_mutex))
    return ret;
  if (v->uvc && uvc_buffer_front(&v->uvc->write))
    ret = true;
  pthread_mutex_unlock(&v->buffer_mutex);
  return ret;
}

bool uvc_buffer_write_enable(int id) {
  bool ret = false;

  pthread_mutex_lock(&mtx_v);
  if (_uvc_video_id_check(id)) {
    for (std::list<struct uvc_video *>::iterator i = lst_v.begin();
         i != lst_v.end(); ++i) {
      struct uvc_video *l = *i;
      if (id == l->id) {
        ret = _uvc_buffer_write_enable(l);
        break;
      }
    }
  }
  pthread_mutex_unlock(&mtx_v);

  return ret;
}

static bool _uvc_buffer_read_enable(struct uvc_video *v) {
  bool ret = false;

  if (pthread_mutex_trylock(&v->buffer_mutex))
    return ret;
  if (v->uvc && uvc_buffer_front(&v->uvc->read))
    ret = true;
  pthread_mutex_unlock(&v->buffer_mutex);
  return ret;
}

bool uvc_buffer_read_enable(int id) {
  bool ret = false;

  pthread_mutex_lock(&mtx_v);
  if (_uvc_video_id_check(id)) {
    for (std::list<struct uvc_video *>::iterator i = lst_v.begin();
         i != lst_v.end(); ++i) {
      struct uvc_video *l = *i;
      if (id == l->id) {
        ret = _uvc_buffer_read_enable(l);
        break;
      }
    }
  }
  pthread_mutex_unlock(&mtx_v);

  return ret;
}

struct uvc_buffer *uvc_buffer_write_get(int id) {
  struct uvc_buffer *buffer = NULL;
  pthread_mutex_lock(&mtx_v);
  if (_uvc_video_id_check(id)) {
    for (std::list<struct uvc_video *>::iterator i = lst_v.begin();
         i != lst_v.end(); ++i) {
      struct uvc_video *l = *i;
      if (id == l->id) {
        l->can_exit = false;
        pthread_mutex_lock(&l->buffer_mutex);
        buffer = uvc_buffer_pop_front(&l->uvc->write);
        pthread_mutex_unlock(&l->buffer_mutex);
        break;
      }
    }
  }
  pthread_mutex_unlock(&mtx_v);
  return buffer;
}

void uvc_buffer_write_set(int id, struct uvc_buffer *buf) {
  pthread_mutex_lock(&mtx_v);
  if (_uvc_video_id_check(id)) {
    for (std::list<struct uvc_video *>::iterator i = lst_v.begin();
         i != lst_v.end(); ++i) {
      struct uvc_video *l = *i;
      if (id == l->id) {
        l->can_exit = true;
        pthread_mutex_lock(&l->buffer_mutex);
        uvc_buffer_push_back(&l->uvc->write, buf);
        pthread_mutex_unlock(&l->buffer_mutex);
        break;
      }
    }
  }

  pthread_mutex_unlock(&mtx_v);
}

void uvc_buffer_read_set(int id, struct uvc_buffer *buf) {
  pthread_mutex_lock(&mtx_v);
  if (_uvc_video_id_check(id)) {
    for (std::list<struct uvc_video *>::iterator i = lst_v.begin();
         i != lst_v.end(); ++i) {
      struct uvc_video *l = *i;
      if (id == l->id) {
        l->can_exit = true;
        pthread_mutex_lock(&l->buffer_mutex);
        uvc_buffer_push_back(&l->uvc->read, buf);
        pthread_mutex_unlock(&l->buffer_mutex);
        break;
      }
    }
  }

  pthread_mutex_unlock(&mtx_v);
}

void uvc_buffer_cache_set(int id, struct uvc_buffer *buf) {
  pthread_mutex_lock(&mtx_v);
  if (_uvc_video_id_check(id)) {
    for (std::list<struct uvc_video *>::iterator i = lst_v.begin();
         i != lst_v.end(); ++i) {
      struct uvc_video *l = *i;
      if (id == l->id) {
        pthread_mutex_lock(&l->buffer_mutex);
        LOG_DEBUG("set cache mb:%p\n", buf->frame);
        uvc_buffer_push_back(&l->uvc->cache, buf);
        pthread_mutex_unlock(&l->buffer_mutex);
        break;
      }
    }
  }

  pthread_mutex_unlock(&mtx_v);
}

static void _uvc_set_user_resolution(struct uvc_video *v, int width,
                                     int height) {
  pthread_mutex_lock(&v->user_mutex);
  v->uvc_user.width = width;
  v->uvc_user.height = height;
  LOG_DEBUG("uvc_user.width = %u, uvc_user.height = %u\n", v->uvc_user.width,
            v->uvc_user.height);
  pthread_mutex_unlock(&v->user_mutex);
}

void uvc_set_user_resolution(int width, int height, int id) {
  pthread_mutex_lock(&mtx_v);
  if (_uvc_video_id_check(id)) {
    for (std::list<struct uvc_video *>::iterator i = lst_v.begin();
         i != lst_v.end(); ++i) {
      struct uvc_video *l = *i;
      if (id == l->id) {
        _uvc_set_user_resolution(l, width, height);
        break;
      }
    }
  }
  pthread_mutex_unlock(&mtx_v);
}

static void _uvc_get_user_resolution(struct uvc_video *v, int *width,
                                     int *height) {
  pthread_mutex_lock(&v->user_mutex);
  *width = v->uvc_user.width;
  *height = v->uvc_user.height;
  pthread_mutex_unlock(&v->user_mutex);
}

void uvc_get_user_resolution(int *width, int *height, int id) {
  pthread_mutex_lock(&mtx_v);
  if (_uvc_video_id_check(id)) {
    for (std::list<struct uvc_video *>::iterator i = lst_v.begin();
         i != lst_v.end(); ++i) {
      struct uvc_video *l = *i;
      if (id == l->id) {
        _uvc_get_user_resolution(l, width, height);
        break;
      }
    }
  }
  pthread_mutex_unlock(&mtx_v);
}

static bool _uvc_get_user_run_state(struct uvc_video *v) {
  bool ret;

  pthread_mutex_lock(&v->user_mutex);
  ret = v->uvc_user.run;
  pthread_mutex_unlock(&v->user_mutex);

  return ret;
}

bool uvc_get_user_run_state(int id) {
  bool state = false;

  pthread_mutex_lock(&mtx_v);
  if (_uvc_video_id_check(id)) {
    for (std::list<struct uvc_video *>::iterator i = lst_v.begin();
         i != lst_v.end(); ++i) {
      struct uvc_video *l = *i;
      if (id == l->id) {
        state = _uvc_get_user_run_state(l);
        break;
      }
    }
  }
  pthread_mutex_unlock(&mtx_v);

  return state;
}

int uvc_video_qbuf_index(struct uvc_device *dev, struct uvc_buffer *send_buf,
                         int index, int len) {
  unsigned int i = 0;
  int ret = 0;

  if (index == 0) {
    uvc_video_reqbufs(dev, dev->nbufs);
    uvc_video_stream(dev, 1);
    dev->vbuf_info = (struct v4l2_buffer_info *)calloc(
        dev->nbufs, sizeof(struct v4l2_buffer_info));
  } else { // check if init qbuf ever
    for (i = 0; i < dev->nbufs; i++) {
      if (send_buf->fd == dev->vbuf_info[i].fd &&
          send_buf->frame == dev->vbuf_info[i].frame) {
        LOG_DEBUG("debug info!index:%d find allocated i:%d fd:%d fram:%p\n",
                  index, i, send_buf->fd, send_buf->frame);
        ret = 1; // find
        break;
      }
    }
  }
  if (ret == 0) {
    if (index == dev->nbufs) {
      LOG_ERROR("had qbuf:%d exceed max uvc buf:%d\n", index, dev->nbufs);
      ret = -1; // err, full buflist.
    } else {
      dev->vbuf_info[index].index = send_buf->index;
      dev->vbuf_info[index].fd = send_buf->fd;
      dev->vbuf_info[index].frame = send_buf->frame;
      if (dev->io == IO_METHOD_USERPTR)
        dev->vbuf_info[index].userptr = (unsigned long)dev->dummy_buf[send_buf->index].start;

      LOG_INFO("debug info! save index:%d send index:%d i:%d send_buf_fd:%d "
               "send_buf_frame:%p "
               "total:%d size:%d\n",
               index, send_buf->index, i, dev->vbuf_info[index].fd,
               dev->vbuf_info[index].frame, send_buf->total_size,
               send_buf->size);
    }
  }

  return ret;
}

static void _uvc_set_user_run_state(struct uvc_video *v, bool state) {
  pthread_mutex_lock(&v->user_mutex);
  v->uvc_user.run = state;
  pthread_mutex_unlock(&v->user_mutex);
}

void uvc_set_user_run_state(bool state, int id) {
  pthread_mutex_lock(&mtx_v);
  if (_uvc_video_id_check(id)) {
    for (std::list<struct uvc_video *>::iterator i = lst_v.begin();
         i != lst_v.end(); ++i) {
      struct uvc_video *l = *i;
      if (id == l->id) {
        _uvc_set_user_run_state(l, state);
        break;
      }
    }
  }
  pthread_mutex_unlock(&mtx_v);
}

static void _uvc_set_user_fcc(struct uvc_video *v, unsigned int fcc) {
  v->uvc_user.fcc = fcc;
}

void uvc_set_user_fcc(unsigned int fcc, int id) {
  pthread_mutex_lock(&mtx_v);
  if (_uvc_video_id_check(id)) {
    for (std::list<struct uvc_video *>::iterator i = lst_v.begin();
         i != lst_v.end(); ++i) {
      struct uvc_video *l = *i;
      if (id == l->id) {
        _uvc_set_user_fcc(l, fcc);
        break;
      }
    }
  }
  pthread_mutex_unlock(&mtx_v);
}

static unsigned int _uvc_get_user_fcc(struct uvc_video *v) {
  return v->uvc_user.fcc;
}

unsigned int uvc_get_user_fcc(int id) {
  unsigned int fcc = 0;

  pthread_mutex_lock(&mtx_v);
  if (_uvc_video_id_check(id)) {
    for (std::list<struct uvc_video *>::iterator i = lst_v.begin();
         i != lst_v.end(); ++i) {
      struct uvc_video *l = *i;
      if (id == l->id) {
        fcc = _uvc_get_user_fcc(l);
        break;
      }
    }
  }
  pthread_mutex_unlock(&mtx_v);

  return fcc;
}

static void _uvc_memset_uvc_user(struct uvc_video *v) {
  memset(&v->uvc_user, 0, sizeof(struct uvc_user));
}

void uvc_memset_uvc_user(int id) {
  pthread_mutex_lock(&mtx_v);
  if (_uvc_video_id_check(id)) {
    for (std::list<struct uvc_video *>::iterator i = lst_v.begin();
         i != lst_v.end(); ++i) {
      struct uvc_video *l = *i;
      if (id == l->id) {
        _uvc_memset_uvc_user(l);
        break;
      }
    }
  }
  pthread_mutex_unlock(&mtx_v);
}

static bool _uvc_buffer_check(struct uvc_video *v, struct uvc_buffer *buffer) {
  int width = 0, height = 0;

  _uvc_get_user_resolution(v, &width, &height);
  if (buffer->width == width && buffer->height == height)
    return true;
  else
    return false;
}

static void uvc_delay_time_calcu_before_get(struct uvc_device *dev,
                                            struct uvc_video *v) {
#if UVC_DYNAMIC_DEBUG_USE_TIME
  if (!access(UVC_DYNAMIC_DEBUG_USE_TIME_CHECK, 0)) {
    int32_t use_time_us, now_time_us;
    struct timespec now_tm = {0, 0};
    clock_gettime(CLOCK_MONOTONIC, &now_tm);
    now_time_us = now_tm.tv_sec * 1000000LL + now_tm.tv_nsec / 1000; // us
    use_time_us =
        now_time_us -
        v->last_pts; // usb send ok and calcu the latency time use last pts
    LOG_INFO(
        "isp->mpp->usb_ready->usb_send_ok seq:%d latency time:%d us, %d ms\n",
        v->last_seq, use_time_us, use_time_us / 1000);
  }

  if (dev->usb_state == USB_STATE_FIRST_GET_READY) {
    int32_t use_time_us;
    struct timespec now_tm = {0, 0};
    clock_gettime(CLOCK_MONOTONIC, &now_tm);
    dev->usb_state = USB_STATE_FIRST_GET_OK;
    dev->first_usb_get_ready_pts =
        now_tm.tv_sec * 1000000LL + now_tm.tv_nsec / 1000;
    use_time_us = dev->first_usb_get_ready_pts - dev->stream_on_pts;
    LOG_INFO("steamon->get_ready latency time:%d us, %d ms\n", use_time_us,
             use_time_us / 1000);
    use_time_us = dev->first_usb_get_ready_pts - dev->first_cmd_pts;
    LOG_INFO("first_cmd->steamon->get_ready latency time:%d ms\n",
             use_time_us / 1000);
  } else if (dev->usb_state == USB_STATE_FIRST_SEND_OK) {
    struct timespec now_tm = {0, 0};
    int32_t use_time_us;
    clock_gettime(CLOCK_MONOTONIC, &now_tm);
    dev->first_usb_send_ok_pts =
        now_tm.tv_sec * 1000000LL + now_tm.tv_nsec / 1000;
    dev->usb_state = USB_STATE_NORMAL_RUN;
    use_time_us = dev->first_usb_send_ok_pts - dev->first_cmd_pts;
    LOG_INFO(
        "first_cmd->steamon->get_ready->get_ok->send_ok latency time:%d ms\n",
        use_time_us / 1000);
    use_time_us = dev->first_usb_send_ok_pts - dev->stream_on_pts;
    LOG_INFO("steamon->get_ready->get_ok->send_ok latency time:%d us, %d ms\n",
             use_time_us, use_time_us / 1000);
    use_time_us = dev->first_usb_send_ok_pts - v->last_pts;
    LOG_INFO(
        "isp->mpp->usb_ready->usb_send_ok seq:%d latency time:%d us, %d ms\n",
        v->last_seq, use_time_us, use_time_us / 1000);
  }
#endif
}

static void uvc_delay_time_calcu_after_get(struct uvc_device *dev,
                                           struct uvc_video *v,
                                           struct uvc_buffer *buffer) {
#if UVC_DYNAMIC_DEBUG_USE_TIME
  if (dev->usb_state == USB_STATE_FIRST_GET_OK) {
    struct timespec now_tm = {0, 0};
    int32_t use_time_us;
    clock_gettime(CLOCK_MONOTONIC, &now_tm);
    dev->first_usb_get_ok_pts =
        now_tm.tv_sec * 1000000LL + now_tm.tv_nsec / 1000;
    dev->usb_state = USB_STATE_FIRST_SEND_OK;
    use_time_us = dev->first_usb_get_ok_pts - dev->stream_on_pts;
    LOG_INFO("steamon->get_ready->get_ok seq:%d time:%d us, %d ms\n",
             buffer->seq, use_time_us, use_time_us / 1000);
  }

  v->last_pts = buffer->pts;
  v->now_pts = buffer->pts;
  v->last_seq = buffer->seq;
  if (!access(UVC_DYNAMIC_DEBUG_USE_TIME_CHECK, 0)) {
    int32_t use_time_us, now_time_us;
    struct timespec now_tm = {0, 0};
    clock_gettime(CLOCK_MONOTONIC, &now_tm);
    now_time_us = now_tm.tv_sec * 1000000LL + now_tm.tv_nsec / 1000; // us
    use_time_us = now_time_us - v->now_pts;
    LOG_INFO("isp->mpp->usb_ready seq:%d latency time:%d us, %d ms\n",
             buffer->seq, use_time_us, use_time_us / 1000);
  }
#endif
}

struct uvc_buffer *uvc_get_enc_data(struct uvc_device *dev, struct uvc_video *v,
                                    bool init) {
  struct uvc_buffer *buffer = NULL;
  int ret = 0;
  v->idle_cnt = 0;
  int time_out = 60;
  if (init || !dev->get_buf_count) // first frm for small timeout
    time_out = 30;
  while (!(buffer = uvc_buffer_front(&v->uvc->read)) &&
         _uvc_get_user_run_state(v)) {
    pthread_mutex_unlock(&mtx_v);
    usleep(1000);
    v->idle_cnt++;
    pthread_mutex_lock(&mtx_v);
    if (v->idle_cnt > time_out) {
      if (!(buffer = uvc_buffer_front(&v->uvc->read)) &&
          _uvc_get_user_run_state(v)) // onece more check it.
      {
        if (init == false) {
          if (v->drop_frame_cnt == 0) {
            LOG_INFO("fill buf timeout %d ms\n", time_out);
          }
        } else {
          if (v->drop_frame_cnt == 0) {
            LOG_INFO("init:%d,fill buf timeout %d ms buffer:%p\n", init,
                     time_out, buffer);
          }
        }
        v->drop_frame_cnt++;
        if (v->drop_frame_cnt > 200) {
          if (access("/tmp/uvc_camera_no_buf", 0)) {
            LOG_INFO("it's already 200 frames buf no to get from CAMERA,tell "
                     "watchdog now\n");
            system("touch /tmp/uvc_camera_no_buf &");
          }
          v->drop_frame_cnt = 0;
        }
      }
      break;
    }
  }
  if (buffer) { // init &&
    ret = uvc_video_qbuf_index(dev, buffer, dev->get_buf_count, buffer->size);
    if (!ret) {
      dev->get_buf_count += 1;
      LOG_INFO("new qbuf index:%d\n", dev->get_buf_count);
    } else if (ret == -1) {
      uvc_buffer_pop_front(&v->uvc->read);
      _uvc_mpi_buffer_release(dev, buffer, true);
      uvc_buffer_push_back(&v->uvc->write, buffer);
      LOG_INFO("abandon buffer-mb:%p\n", buffer->frame);
      goto FAIL;
    }
  }

  if (buffer)
    uvc_delay_time_calcu_after_get(dev, v, buffer);
  return buffer;
FAIL:
  return NULL;
}

static struct uvc_buffer *_uvc_user_fill_buffer(struct uvc_video *v,
                                                struct uvc_device *dev,
                                                struct v4l2_buffer *buf) {
  struct uvc_buffer *buffer = NULL;
  uvc_delay_time_calcu_before_get(dev, v);

  buffer = uvc_get_enc_data(dev, v, false);
  if (buffer) {
    switch (dev->io) {
    case IO_METHOD_DMA_BUFF:
      buf->index = buffer->index;
      buf->sequence = buffer->seq;
      buf->m.fd = buffer->fd;
      buf->reserved2 = buffer->offset;
      break;
    case IO_METHOD_USERPTR:
      buf->index = buffer->index;
      buf->sequence = buffer->seq;
      buf->m.userptr = (unsigned long)dev->dummy_buf[buffer->index].start;
      memset(dev->dummy_buf[buffer->index].start, 0, dev->dummy_buf[buffer->index].length);
      memcpy(dev->dummy_buf[buffer->index].start, buffer->buffer, buffer->size);
      buf->length = dev->dummy_buf[buffer->index].length;
      break;
    default:
      LOG_ERROR("no support this io method:%d\n", dev->io);
      break;
    }

    if (_uvc_get_user_run_state(v) && _uvc_video_get_uvc_process(v)) {
      // buf->bytesused = 550*1024;//for test
      buf->bytesused = buffer->size;
      v->drop_frame_cnt = 0;
      LOG_DEBUG("debug info! i:%d fd:%d buf->bytesused=%d\n", buffer->index,
                buf->m.fd, buf->bytesused);
    } else {
      LOG_WARN("_uvc_user_fill_buffer no go here \n");
      buf->bytesused = 0;
    }
    buffer = uvc_buffer_pop_front(&v->uvc->read);
  } else {
    buf->bytesused = 0;
    buf->m.fd = 0;
  }

  return buffer;
}

struct uvc_buffer *uvc_user_fill_buffer(struct uvc_device *dev,
                                        struct v4l2_buffer *buf, int id) {
  struct uvc_buffer *buffer = NULL;
  pthread_mutex_lock(&mtx_v);
  if (_uvc_video_id_check(id)) {
    for (std::list<struct uvc_video *>::iterator i = lst_v.begin();
         i != lst_v.end(); ++i) {
      struct uvc_video *l = *i;
      if (id == l->id) {
        buffer = _uvc_user_fill_buffer(l, dev, buf);
        break;
      }
    }
  }
  pthread_mutex_unlock(&mtx_v);

  return buffer;
}

static void _uvc_user_release_cache_buffer(struct uvc_video *v,
                                           struct uvc_device *dev,
                                           struct v4l2_buffer *buf) {
  struct uvc_buffer buffer;
  bool matching = false;

  for (int i = 0; i < dev->nbufs; i++) {
     if (dev->io == IO_METHOD_DMA_BUFF &&
        dev->vbuf_info[i].fd == buf->m.fd &&
        dev->vbuf_info[i].index == buf->index) {
      buffer.frame = dev->vbuf_info[i].frame;
      buffer.fd = dev->vbuf_info[i].fd;
      matching = true;
      // LOG_ERROR("find matching:id:%d fd:%d\n", i, buf->m.fd);
      break;
	  } else if (dev->io == IO_METHOD_USERPTR &&
               dev->vbuf_info[i].userptr == buf->m.userptr &&
               dev->vbuf_info[i].index == buf->index) {
      buffer.frame = dev->vbuf_info[i].frame;
      buffer.fd = dev->vbuf_info[i].fd;
      matching = true;
      break;
    }
  }
  if (matching) {
    uvc_buffer_pop_assign_cache(&v->uvc->cache, &buffer);
    _uvc_mpi_buffer_release(dev, &buffer, true);
  } else {
    LOG_ERROR("not find matching: fd:%d index:%d\n", buf->m.fd, buf->index);
  }
}

// uvc driver return the buf
void uvc_user_release_cache_buffer(struct uvc_device *dev,
                                   struct v4l2_buffer *buf, int id) {
  pthread_mutex_lock(&mtx_v);
  if (_uvc_video_id_check(id)) {
    for (std::list<struct uvc_video *>::iterator i = lst_v.begin();
         i != lst_v.end(); ++i) {
      struct uvc_video *l = *i;
      if (id == l->id) {
        _uvc_user_release_cache_buffer(l, dev, buf);
        break;
      }
    }
  }
  pthread_mutex_unlock(&mtx_v);
}
