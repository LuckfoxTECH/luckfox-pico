/*
 * Copyright 2022 Rockchip Electronics Co. LTD
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
#ifndef SRC_INCLUDE_UAC_COMMON_DEF_H_
#define SRC_INCLUDE_UAC_COMMON_DEF_H_
#ifdef __cplusplus

#include <dirent.h>
#include <errno.h>
#include <math.h>
#include <pthread.h>
#include <pwd.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define OPT_SAMPLE_RATE "opt_samaple_rate"
#define OPT_CHANNELS "opt_channel"
#define OPT_VOLUME "opt_volume"
#define OPT_MUTE "opt_mute"
#define OPT_CONFIGS "opt_configs"
#define OPT_PPM "opt_ppm"

#define OPT_SET_ALSA_CAPTURE "set_capture_config"
#define OPT_SET_RESAMPLE "set_resample_config"
#define OPT_SET_VOLUME "set_volume_config"
#define OPT_SET_CONFIG "set_config"
#define OPT_SET_PPM "set_ppm"

#define ARRAY_ELEMS(a) (sizeof(a) / sizeof((a)[0]))

#define GET_ENTRY_VALUE(INPUT1, INPUT2, MAP, KEY1, KEY2, VALUE)                \
  do {                                                                         \
    for (size_t i = 0; i < ARRAY_ELEMS(MAP); i++) {                            \
      if (INPUT1 == MAP[i].KEY1 && INPUT2 == MAP[i].KEY2)                      \
        return MAP[i].VALUE;                                                   \
    }                                                                          \
  } while (0)

enum UacStreamType {
  // our device record datas from usb, pc/remote->our device
  UAC_STREAM_RECORD = 0,
  // play datas to usb, our device->pc/remote
  UAC_STREAM_PLAYBACK,
  UAC_STREAM_MAX
};

typedef struct _UacAudioConfig {
  int samplerate;
  union {
    int intVol;
    float floatVol;
  };
  int mute;
  int ppm;
} UacAudioConfig;

uint64_t getRelativeTimeMs();
uint64_t getRelativeTimeUs();
#endif
#endif // SRC_INCLUDE_UAC_COMMON_DEF_H_
