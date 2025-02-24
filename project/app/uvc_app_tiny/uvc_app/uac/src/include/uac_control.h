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
#ifndef SRC_INCLUDE_UAC_CONTROL_H_
#define SRC_INCLUDE_UAC_CONTROL_H_

#include "uac_common_def.h"

enum UacApiType { UAC_API_MPI = 0, UAC_API_GRAPH = 1, UAC_API_MAX };

#ifdef __cplusplus
class UACControl {
public:
  UACControl() {}
  virtual ~UACControl() {}

public:
  virtual int uacStart() = 0;
  virtual void uacStop() = 0;
  virtual void uacSetSampleRate(int sampleRate) = 0;
  virtual void uacSetVolume(int volume) = 0;
  virtual void uacSetMute(int mute) = 0;
  virtual void uacSetPpm(int ppm) = 0;
};
#endif
#ifdef __cplusplus
extern "C" {
#endif

int uac_start(int mode);
void uac_stop(int mode);
void uac_set_sample_rate(int mode, int samplerate);
void uac_set_volume(int mode, int volume);
void uac_set_mute(int mode, int mute);
void uac_set_ppm(int mode, int ppm);

int uac_control_create(int type);
void uac_control_destory();
#ifdef __cplusplus
}
#endif

#endif // SRC_INCLUDE_UAC_CONTROL_H_
