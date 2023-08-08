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

#ifndef AIRKISS_H_
#define AIRKISS_H_ 
#ifdef __cplusplus
extern "C" {
#endif
#ifndef AIRKISS_ENABLE_CRYPT
#define AIRKISS_ENABLE_CRYPT 1
#endif
typedef void* (*airkiss_memset_fn) (void* ptr, int value, unsigned int num);
typedef void* (*airkiss_memcpy_fn) (void* dst, const void* src, unsigned int num);
typedef int (*airkiss_memcmp_fn) (const void* ptr1, const void* ptr2, unsigned int num);
typedef int (*airkiss_printf_fn) (const char* format, ...);
typedef struct
{
 airkiss_memset_fn memset;
 airkiss_memcpy_fn memcpy;
 airkiss_memcmp_fn memcmp;
 airkiss_printf_fn printf;
} airkiss_config_t;
typedef struct
{
 int dummyap[26];
 int dummy[32];
} airkiss_context_t;
typedef struct
{
 char* pwd;
 char* ssid;
 unsigned char pwd_length;
 unsigned char ssid_length;
 unsigned char random;
 unsigned char reserved;
} airkiss_result_t;
typedef enum
{
 AIRKISS_STATUS_CONTINUE = 0,
 AIRKISS_STATUS_CHANNEL_LOCKED = 1,
 AIRKISS_STATUS_COMPLETE = 2
} airkiss_status_t;
#if AIRKISS_ENABLE_CRYPT
int airkiss_set_key(airkiss_context_t* context, const unsigned char* key, unsigned int length);
#endif
const char* airkiss_version(void);
int airkiss_init(airkiss_context_t* context, const airkiss_config_t* config);
int airkiss_recv(airkiss_context_t* context, const void* frame, unsigned short length);
int airkiss_get_result(airkiss_context_t* context, airkiss_result_t* result);
int airkiss_change_channel(airkiss_context_t* context);
typedef enum
{
 AIRKISS_LAN_ERR_OVERFLOW = -5,
 AIRKISS_LAN_ERR_CMD = -4,
 AIRKISS_LAN_ERR_PAKE = -3,
 AIRKISS_LAN_ERR_PARA = -2,
 AIRKISS_LAN_ERR_PKG = -1,
 AIRKISS_LAN_CONTINUE = 0,
 AIRKISS_LAN_SSDP_REQ = 1,
 AIRKISS_LAN_PAKE_READY = 2
} airkiss_lan_ret_t;
typedef enum
{
 AIRKISS_LAN_SSDP_REQ_CMD = 0x1,
 AIRKISS_LAN_SSDP_RESP_CMD = 0x1001,
 AIRKISS_LAN_SSDP_NOTIFY_CMD = 0x1002
} airkiss_lan_cmdid_t;
int airkiss_lan_recv(const void* body, unsigned short length, const airkiss_config_t* config);
int airkiss_lan_pack(airkiss_lan_cmdid_t ak_lan_cmdid, void* appid, void* deviceid, void* _datain, unsigned short inlength, void* _dataout, unsigned short* outlength, const airkiss_config_t* config);
#ifdef __cplusplus
}
#endif
#endif
