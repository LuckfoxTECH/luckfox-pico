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

#ifndef __TX_TV_SDK_H__
#define __TX_TV_SDK_H__ 
#include "TXSDKCommonDef.h"
typedef struct tag_tx_tv_notify
{
 void (*on_bind_complete)(unsigned long long ddwID, int error);
 void (*on_receive_video_push)(char * pBufReply, int nLenReply);
}tx_tv_notify;
SDK_API void tx_set_tv_notify(tx_tv_notify * notify);
typedef void (*on_receive_video_reply)(char * pBufReply, int nLenReply);
SDK_API void tx_send_video_request(int type, unsigned long long toDin, const char * pBuff, unsigned int uLen, on_receive_video_reply callback);
#ifdef __cplusplus
extern "C"
{
#endif
SDK_API unsigned long long tx_get_uin_by_tinyid(unsigned long long ddwID);
SDK_API unsigned long long tx_get_tinyid_by_uin(unsigned long long ddwID);
#ifdef __cplusplus
}
#endif
#endif
