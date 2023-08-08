/*
 * Copyright (c) 2015 iComm-semi Ltd.
 *
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation, either version 3 of the License, or 
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
 * See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __TX_DECODE_ENGINE__H__
#define __TX_DECODE_ENGINE__H__ 
#include "TXSDKCommonDef.h"
CXX_EXTERN_BEGIN
enum RESULT
{
 ERROR_NULL = 0,
 ERROR_INITED = 1,
 ERROR_NO_MEMORY = 2,
 ERROR_CREATE_LOCK_FAIL = 3,
 ERROR_CREATE_THREAD_FAIL = 4,
};
#define MAX_SSID_LEN 128
#define MAX_PSWD_LEN 128
#define MAX_IP_LEN 16
typedef struct
{
 char sz_ssid[MAX_SSID_LEN];
 char sz_password[MAX_PSWD_LEN];
    char sz_ip[MAX_IP_LEN];
    unsigned short sh_port;
} tx_voicelink_param;
typedef void (*VL_FUNC_NOTIFY)(tx_voicelink_param* pparam);
SDK_API int tx_init_decoder(VL_FUNC_NOTIFY func, int samplerate);
SDK_API void tx_uninit_decoder();
SDK_API void tx_fill_audio(signed short* audio, int nlen);
CXX_EXTERN_END
#endif
