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

#include "rkadk_struct2ini.h"
#include "rkadk_common.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

RKADK_S32 RKADK_IniLoad(char *iniFile, dictionary *pstIni) {
  RKADK_CHECK_POINTER(iniFile, RKADK_FAILURE);

  if (access(iniFile, F_OK)) {
    RKADK_LOGD("%s don't exist, create an empty file", iniFile);
    FILE *file = fopen(iniFile, "w+");
    if (file) {
      fclose(file);
    } else {
      RKADK_LOGE("%s creation failed", iniFile);
      return RKADK_FAILURE;
    }
  }

  pstIni = iniparser_load(iniFile);
  if (pstIni == NULL) {
    RKADK_LOGE("can't parse file: %s", iniFile);
    return RKADK_FAILURE;
  }

  return RKADK_SUCCESS;
}

void RKADK_IniFree(dictionary *pstIni) {
  RKADK_CHECK_POINTER_N(pstIni);
  iniparser_freedict(pstIni);
}

RKADK_S32 RKADK_Ini2Struct(char *iniFile, void *structAddr,
                           RKADK_SI_CONFIG_MAP_S *mapTable, int mapTableSize) {
  char sectionKey[SI_MAX_SEARCH_STRING] = {0};
  int searchLen = 0;

  RKADK_CHECK_POINTER(iniFile, RKADK_FAILURE);
  RKADK_CHECK_POINTER(structAddr, RKADK_FAILURE);
  RKADK_CHECK_POINTER(mapTable, RKADK_FAILURE);

  if (mapTableSize <= 0) {
    RKADK_LOGE("invalid mapTableSize[%d]", mapTableSize);
    return RKADK_FAILURE;
  }

  dictionary *ini = iniparser_load(iniFile);
  if (ini == NULL) {
    RKADK_LOGE("can't parse file: %s", iniFile);
    return RKADK_FAILURE;
  }

  for (int i = 0; i < mapTableSize; i++) {
    searchLen =
        strlen(mapTable[i].structName) + strlen(mapTable[i].structMember);
    if (searchLen >= (SI_MAX_SEARCH_STRING - 1)) {
      RKADK_LOGE("searchLen[%d] is too long", searchLen);
      return RKADK_FAILURE;
    }

    memset(sectionKey, 0, sizeof(sectionKey));
    strcpy(sectionKey, mapTable[i].structName);
    strcat(sectionKey, ":");
    strcat(sectionKey, mapTable[i].structMember);

    if (mapTable[i].keyVlaueType == int_e) {
      int keyInt = iniparser_getint(ini, sectionKey, -1);
      if (keyInt == -1 && strcmp(mapTable[i].structMember, "first_frame_qp")
        && strcmp(mapTable[i].structMember, "max_qp")
        && strcmp(mapTable[i].structMember, "min_qp")
        && strcmp(mapTable[i].structMember, "frame_min_qp")
        && strcmp(mapTable[i].structMember, "i_min_qp")
        && strcmp(mapTable[i].structMember, "i_frame_min_qp")) {
#ifdef RKADK_DUMP_CONFIG
        RKADK_LOGE("int [%s]: not exist", sectionKey);
#endif
        return RKADK_PARAM_NOT_EXIST;
      } else {
        *(int *)((char *)structAddr + mapTable[i].offset) = keyInt;
      }
    } else if (mapTable[i].keyVlaueType == string_e) {
      char *keyString = (char *)iniparser_getstring(ini, sectionKey, NULL);
      if (keyString) {
        memset((char *)structAddr + mapTable[i].offset, 0,
               mapTable[i].stringLength);
        if (strlen(keyString)) {
          if (strlen(keyString) <= mapTable[i].stringLength) {
            memcpy((char *)structAddr + mapTable[i].offset, keyString,
                   strlen(keyString));
          } else {
            memcpy((char *)structAddr + mapTable[i].offset, keyString,
                   mapTable[i].stringLength - 1);
          }
        }
      } else {
#ifdef RKADK_DUMP_CONFIG
        RKADK_LOGE("string [%s]: not exist", sectionKey);
#endif
        return RKADK_PARAM_NOT_EXIST;
      }
    } else if (mapTable[i].keyVlaueType == double_e) {
      double keyDouble = iniparser_getdouble(ini, sectionKey, -1.0);
      const double EPS = 1e-6;

      if (fabs(keyDouble - (-1.0)) > EPS) {
        *(double *)((char *)structAddr + mapTable[i].offset) = keyDouble;
      } else {
#ifdef RKADK_DUMP_CONFIG
        RKADK_LOGE("double [%s]: not exist", sectionKey);
#endif
        return RKADK_PARAM_NOT_EXIST;
      }
    } else if (mapTable[i].keyVlaueType == bool_e) {
      int keyBool = iniparser_getboolean(ini, sectionKey, -1);
      if (keyBool != -1) {
        if (keyBool)
          *(bool *)((char *)structAddr + mapTable[i].offset) = true;
        else
          *(bool *)((char *)structAddr + mapTable[i].offset) = false;
      } else {
#ifdef RKADK_DUMP_CONFIG
        RKADK_LOGE("bool [%s] not exist", sectionKey);
#endif
        return RKADK_PARAM_NOT_EXIST;
      }
    }
  }

  iniparser_freedict(ini);
  return RKADK_SUCCESS;
}

RKADK_S32 RKADK_Struct2Ini(char *iniFile, void *structAddr,
                           RKADK_SI_CONFIG_MAP_S *mapTable, int mapTableSize) {
  char temp[SI_CONFIG_MAP_STR_LENGTH_MAX] = {0};
  char sectionKey[SI_MAX_SEARCH_STRING] = {0};

  RKADK_CHECK_POINTER(iniFile, RKADK_FAILURE);
  RKADK_CHECK_POINTER(structAddr, RKADK_FAILURE);
  RKADK_CHECK_POINTER(mapTable, RKADK_FAILURE);

  if (mapTableSize <= 0) {
    RKADK_LOGE("invalid mapTableSize[%d]", mapTableSize);
    return RKADK_FAILURE;
  }

  if (access(iniFile, F_OK)) {
    RKADK_LOGD("%s don't exist, create", iniFile);
    FILE *file = fopen(iniFile, "w+");
    if (file) {
      fclose(file);
    } else {
      RKADK_LOGE("%s creation failed", iniFile);
      return RKADK_FAILURE;
    }
  }

  dictionary *ini = iniparser_load(iniFile);
  if (ini == NULL) {
    RKADK_LOGE("can't parse file: %s", iniFile);
    return RKADK_FAILURE;
  }

  if (iniparser_find_entry(ini, mapTable[0].structName) == 0) {
    RKADK_LOGI("section name[%s] no exist, so create", mapTable[0].structName);
    iniparser_set(ini, mapTable[0].structName, NULL);
  }

  for (int i = 0; i < mapTableSize; i++) {
    memset(sectionKey, 0, sizeof(sectionKey));
    memset(temp, 0, sizeof(temp));
    strcpy(sectionKey, mapTable[i].structName);
    strcat(sectionKey, ":");
    strcat(sectionKey, mapTable[i].structMember);

    if (mapTable[i].keyVlaueType == int_e) {
      sprintf(temp, "%d", *(int *)((char *)structAddr + mapTable[i].offset));
    } else if (mapTable[i].keyVlaueType == string_e) {
      if (mapTable[i].stringLength >= (SI_CONFIG_MAP_STR_LENGTH_MAX - 1))
        RKADK_LOGE(
            "mapTable[%d].string_length > SI_CONFIG_MAP_STR_LENGTH_MAX(%d)", i,
            SI_CONFIG_MAP_STR_LENGTH_MAX);
      else
        sprintf(temp, "%s", (char *)structAddr + mapTable[i].offset);
    } else if (mapTable[i].keyVlaueType == double_e) {
      sprintf(temp, "%.7f",
              *(double *)((char *)structAddr + mapTable[i].offset));
    } else if (mapTable[i].keyVlaueType == bool_e) {
      if (*((bool *)structAddr + mapTable[i].offset))
        sprintf(temp, "%s", "TRUE");
      else
        sprintf(temp, "%s", "FALSE");
    } else
      continue;
    iniparser_set(ini, sectionKey, temp);
  }

  FILE *fp = fopen(iniFile, "w");
  if (fp == NULL) {
    RKADK_LOGE("fopen file: %s fail!", iniFile);
    iniparser_freedict(ini);
    return RKADK_FAILURE;
  }

  iniparser_dump_ini(ini, fp);
  iniparser_freedict(ini);
  fflush(fp);
  fclose(fp);
  return RKADK_SUCCESS;
}
