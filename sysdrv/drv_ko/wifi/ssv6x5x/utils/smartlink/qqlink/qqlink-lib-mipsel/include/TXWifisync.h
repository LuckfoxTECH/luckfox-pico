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

#ifndef __TX_WIFI_SYNC_H__
#define __TX_WIFI_SYNC_H__ 
#include "TXSDKCommonDef.h"
CXX_EXTERN_BEGIN
enum wifisyncerror {
 QLERROR_INIT_SUCCESS = 0,
 QLERROR_MEMORY_ALLOC = 1,
 QLERROR_NOTIFY_NULL = 2,
 QLERROR_PARAM_KEY = 3,
 QLERROR_PARAM_KEY_LEN = 4,
 QLERROR_OPENSSL_LOAD_FAILED = 5,
 QLERROR_HOP_NULL = 6,
 QLERROR_SZIFNAME_INVALID = 7,
 QLERROR_WIFICHIP_NOTSUPPORT = 8,
 QLERROR_INIT_OTHER = 9,
};
enum fill80211relust {
 QLERROR_SUCCESS = 0,
 QLERROR_HOP = 1,
 QLERROR_LOCK = 2,
 QLERROR_OTHER = 3,
 QLERROR_DECRYPT_FAILED = 4,
 QLERROR_NEED_INIT = 5,
 QLERROR_VERSION = 6,
 QLERROR_START_FRAME = 7,
    QLERROR_BCAST_NOT_FRAME = 8,
    QLERROR_BCAST_CALC_C = 9,
    QLERROR_BCAST_ONE_DATA = 10,
    QLERROR_MCAST_NOT_FRAME = 11,
    QLERROR_MCAST_ONE_DATA = 12,
};
#define QLMAX_SSID_LEN 128
#define QLMAX_PSWD_LEN 128
#define QLMAX_IP_LEN 16
typedef struct
{
 char sz_ssid[QLMAX_SSID_LEN];
 char sz_password[QLMAX_PSWD_LEN];
    char sz_ip[QLMAX_IP_LEN];
    unsigned short sh_port;
} tx_wifi_sync_param;
typedef void (* FUNC_HOP)(int nchannel);
typedef void (* FUNC_NOTIFY)(tx_wifi_sync_param *pwifi_sync_param, void *puserdata);
SDK_API int init_wifi_sync(FUNC_NOTIFY fNotify, char *szSN, void *puserdata);
SDK_API int fill_80211_frame(const unsigned char *buff, int nlen, int npackoffset, int* pChannel);
SDK_API void wifi_sync_notify_hop(int channel);
SDK_API void destory_wifi_sync();
typedef enum {
 WCT_REALTEK8188,
 WCT_MTK7601,
} WifiChipType;
SDK_API int start_smartlink(
    FUNC_HOP fHopping,
    char* szifName,
    FUNC_NOTIFY fNotify,
    char* szSN,
 WifiChipType chipType,
    int hoppingTime,
    int snifferTime,
    void* pUserData
    );
SDK_API void stop_smartlink();
SDK_API int is_smartlink_running();
CXX_EXTERN_END
#endif
