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

#ifndef __RKADK_STRUCT_2_INI_H__
#define __RKADK_STRUCT_2_INI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "iniparser/iniparser.h"
#include "rkadk_common.h"

#define OFFSET(struct, member) ((char *)&((struct *)0)->member - (char *)0)
#define SIZEOF(struct, member) sizeof(((struct *)0)->member)
#define DEFINE_MAP(variable, structName, type, member)                         \
  {                                                                            \
    #variable, #member, type, SIZEOF(struct structName, member),               \
                                     OFFSET(struct structName, member)         \
  }

#define SI_CONFIG_MAP_STR_LENGTH_MAX 200
#define SI_MAX_SEARCH_STRING 200

enum si_data_type_e { string_e = 0, bool_e, int_e, double_e };

typedef struct {
  const char *structName;
  const char *structMember;
  enum si_data_type_e keyVlaueType;
  RKADK_U32 stringLength;
  RKADK_U32 offset;
} RKADK_SI_CONFIG_MAP_S;

RKADK_S32 RKADK_IniLoad(char *iniFile, dictionary *pstIni);

void RKADK_IniFree(dictionary *pstIni);

RKADK_S32 RKADK_Ini2Struct(char *iniFile, void *structAddr,
                           RKADK_SI_CONFIG_MAP_S *mapTable, int mapTableSize);
RKADK_S32 RKADK_Struct2Ini(char *iniFile, void *structAddr,
                           RKADK_SI_CONFIG_MAP_S *mapTable, int mapTableSize);

#ifdef __cplusplus
}
#endif
#endif
