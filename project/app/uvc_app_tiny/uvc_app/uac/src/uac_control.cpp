/*
 * Copyright 2020 Rockchip Electronics Co. LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
//#ifdef __cplusplus

#include "uac_control.h"
#include "uac_control_factory.h"
#include "uac_log.h"
#ifdef UAC_MPI
#include "mpi_control_common.h"
#endif

int uac_app_log_level;

typedef struct _UacControls {
  int mode;
  UACControl *uac;
  pthread_mutex_t mutex;
} UacControls;

static UacControls *gUAControl = NULL;
int uac_control_create(int type) {
  int i = 0;
  char *ch = NULL;
  if (type == UAC_API_GRAPH) {
    ch = (char *)"graph";
  } else if (type == UAC_API_MPI) {
    ch = (char *)"mpi";
  }

  ALOGD("-------------uac use %s--------------\n", ch);
  if (!gUAControl) {
    uac_control_destory();
  }

  gUAControl = (UacControls *)calloc(UAC_STREAM_MAX, sizeof(UacControls));
  if (!gUAControl) {
    ALOGE("fail to malloc memory!\n");
    return -1;
  }

  memset(gUAControl, 0, sizeof(UacControls));
  for (i = 0; i < UAC_STREAM_MAX; i++) {
    gUAControl[i].mode = i;
    pthread_mutex_init(&gUAControl[i].mutex, NULL);

    gUAControl[i].uac = UacControlFactory::create((UacApiType)type, i);
    if (!gUAControl[i].uac) {
      uac_control_destory();
      return -1;
    }
  }

  return 0;
}

void uac_control_destory() {
  if (gUAControl == NULL)
    return;

  int i = 0;
  UACControl *uac = NULL;
  if (gUAControl) {
    for (i = 0; i < UAC_STREAM_MAX; i++) {
      uac = gUAControl[i].uac;
      if (uac)
        delete uac;
      gUAControl[i].uac = NULL;
      pthread_mutex_destroy(&gUAControl[i].mutex);
    }
  }

  free(gUAControl);
  gUAControl = NULL;
}

UacControls *getControlContext(int mode) { return &gUAControl[mode]; }

int uac_start(int mode) {
  int ret = 0;
  UacControls *uacs = getControlContext(mode);
  pthread_mutex_lock(&uacs->mutex);
  if (mode == uacs->mode) {
    ret = uacs->uac->uacStart();
  }
  pthread_mutex_unlock(&uacs->mutex);
  return ret;
}

void uac_stop(int mode) {
  UacControls *uacs = getControlContext(mode);
  pthread_mutex_lock(&uacs->mutex);
  if (mode == uacs->mode) {
    uacs->uac->uacStop();
  }
  pthread_mutex_unlock(&uacs->mutex);
}

void uac_set_sample_rate(int mode, int samplerate) {
  UacControls *uacs = getControlContext(mode);
  pthread_mutex_lock(&uacs->mutex);
  if (mode == uacs->mode) {
    uacs->uac->uacSetSampleRate(samplerate);
  }
  pthread_mutex_unlock(&uacs->mutex);
}

void uac_set_volume(int mode, int volume) {
  UacControls *uacs = getControlContext(mode);
  pthread_mutex_lock(&uacs->mutex);
  if (mode == uacs->mode) {
    uacs->uac->uacSetVolume(volume);
  }
  pthread_mutex_unlock(&uacs->mutex);
}

void uac_set_mute(int mode, int mute) {
  UacControls *uacs = getControlContext(mode);
  pthread_mutex_lock(&uacs->mutex);
  if (mode == uacs->mode) {
    uacs->uac->uacSetMute(mute);
  }
  pthread_mutex_unlock(&uacs->mutex);
}

void uac_set_ppm(int mode, int ppm) {
  UacControls *uacs = getControlContext(mode);
  pthread_mutex_lock(&uacs->mutex);
  if (mode == uacs->mode) {
    uacs->uac->uacSetPpm(ppm);
  }
  pthread_mutex_unlock(&uacs->mutex);
}
//#endif
