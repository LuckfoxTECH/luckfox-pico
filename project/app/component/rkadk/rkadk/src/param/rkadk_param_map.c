/*
 * Copyright (c) 2021 Rockchip, Inc. All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "rkadk_param_map.h"

static RKADK_MAP_TABLE_CFG_S g_sensorMapTableList[RKADK_PARAM_MAP_BUTT] = {
    {sizeof(g_stSensorCfgMapTable) / sizeof(RKADK_SI_CONFIG_MAP_S),
     g_stSensorCfgMapTable},
    {sizeof(g_stRecCfgMapTable) / sizeof(RKADK_SI_CONFIG_MAP_S),
     g_stRecCfgMapTable},
    {sizeof(g_stRecTimeCfgMapTable_0) / sizeof(RKADK_SI_CONFIG_MAP_S),
     g_stRecTimeCfgMapTable_0},
    {sizeof(g_stRecCfgMapTable_0) / sizeof(RKADK_SI_CONFIG_MAP_S),
     g_stRecCfgMapTable_0},
    {sizeof(g_stRecParamMapTable_0) / sizeof(RKADK_SI_CONFIG_MAP_S),
     g_stRecParamMapTable_0},
    {sizeof(g_stRecTimeCfgMapTable_1) / sizeof(RKADK_SI_CONFIG_MAP_S),
     g_stRecTimeCfgMapTable_1},
    {sizeof(g_stRecCfgMapTable_1) / sizeof(RKADK_SI_CONFIG_MAP_S),
     g_stRecCfgMapTable_1},
    {sizeof(g_stRecParamMapTable_1) / sizeof(RKADK_SI_CONFIG_MAP_S),
     g_stRecParamMapTable_1},
    {sizeof(g_stPreviewCfgMapTable) / sizeof(RKADK_SI_CONFIG_MAP_S),
     g_stPreviewCfgMapTable},
    {sizeof(g_stPreviewParamMapTable) / sizeof(RKADK_SI_CONFIG_MAP_S),
     g_stPreviewParamMapTable},
    {sizeof(g_stLiveCfgMapTable) / sizeof(RKADK_SI_CONFIG_MAP_S),
     g_stLiveCfgMapTable},
    {sizeof(g_stLiveParamMapTable) / sizeof(RKADK_SI_CONFIG_MAP_S),
     g_stLiveParamMapTable},
    {sizeof(g_stPhotoCfgMapTable) / sizeof(RKADK_SI_CONFIG_MAP_S),
     g_stPhotoCfgMapTable},
    {sizeof(g_stViCfgMapTable_0) / sizeof(RKADK_SI_CONFIG_MAP_S),
     g_stViCfgMapTable_0},
    {sizeof(g_stViCfgMapTable_1) / sizeof(RKADK_SI_CONFIG_MAP_S),
     g_stViCfgMapTable_1},
    {sizeof(g_stViCfgMapTable_2) / sizeof(RKADK_SI_CONFIG_MAP_S),
     g_stViCfgMapTable_2},
    {sizeof(g_stViCfgMapTable_3) / sizeof(RKADK_SI_CONFIG_MAP_S),
     g_stViCfgMapTable_3},
    {sizeof(g_stDispCfgMapTable) / sizeof(RKADK_SI_CONFIG_MAP_S),
     g_stDispCfgMapTable},
    {sizeof(g_stThumbCfgMapTable) / sizeof(RKADK_SI_CONFIG_MAP_S),
     g_stThumbCfgMapTable}};

RKADK_MAP_TABLE_CFG_S *
RKADK_PARAM_GetMapTable(RKADK_U32 u32Camid, RKADK_PARAM_MAP_TYPE_E eMapTable) {
  RKADK_CHECK_CAMERAID(u32Camid, NULL);

  if (eMapTable >= RKADK_PARAM_MAP_BUTT) {
    RKADK_LOGE("invaild u32Camid[%d] eMapTable: %d", u32Camid, eMapTable);
    return NULL;
  }

  return &(g_sensorMapTableList[eMapTable]);
}
