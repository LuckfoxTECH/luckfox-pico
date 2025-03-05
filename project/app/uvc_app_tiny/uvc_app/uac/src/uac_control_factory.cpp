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
//#ifdef __cplusplus

#include "uac_control_factory.h"
#include "uac_control_graph.h"
#include "uac_control_mpi.h"
#include "uac_log.h"

UACControl *createUacGraph(int mode) {
  UACControl *uac = NULL;
#ifdef UAC_GRAPH
  uac = new UACControlGraph(mode);
#endif
  return uac;
}

UACControl *createUacMpi(int mode) {
  UACControl *uac = NULL;
#ifdef UAC_MPI
  uac = new UACControlMpi(mode);
#endif
  return uac;
}

UACControl *UacControlFactory::create(UacApiType type, int mode) {
  UACControl *uac = NULL;
  switch (type) {
  case UAC_API_MPI:
    uac = createUacMpi(mode);
    break;
  case UAC_API_GRAPH:
    uac = createUacGraph(mode);
    break;
  default:
    ALOGD("unkown UacApiType(%d), please check!\n", type);
    break;
  }

  return uac;
}
//#endif