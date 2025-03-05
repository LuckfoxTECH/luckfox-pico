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
#ifndef SRC_INCLUDE_UAC_CONTROL_MPI_H_
#define SRC_INCLUDE_UAC_CONTROL_MPI_H_
#ifdef __cplusplus
#include "uac_control.h"

class UACControlMpi : public UACControl {
public:
  UACControlMpi(int mode);
  virtual ~UACControlMpi();

public:
  virtual int uacStart();
  virtual void uacStop();
  virtual void uacSetSampleRate(int sampleRate);
  virtual void uacSetVolume(int volume);
  virtual void uacSetMute(int mute);
  virtual void uacSetPpm(int ppm);

protected:
  int startAi();
  int startVqe();
  int startAo();
  void streamBind();
  int stopAi();
  int stopVqe();
  int stopAo();
  void streamUnBind();

private:
  void *mCtx;
};
#endif
#endif // SRC_INCLUDE_UAC_CONTROL_MPI_H_
