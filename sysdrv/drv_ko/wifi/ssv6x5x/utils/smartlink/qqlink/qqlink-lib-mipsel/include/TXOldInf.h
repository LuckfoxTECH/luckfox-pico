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

#ifndef __TX_OLD_INTERFACE_H__
#define __TX_OLD_INTERFACE_H__ 
#include "TXSDKCommonDef.h"
#include "TXDataPoint.h"
CXX_EXTERN_BEGIN
typedef struct tag_tx_ccmsg_inst_info
{
    unsigned long long target_id;
    unsigned int appid;
    unsigned int instid;
    unsigned int platform;
    unsigned int open_appid;
    unsigned int productid;
    unsigned int sso_bid;
    char * guid;
    int guid_len;
}tx_ccmsg_inst_info;
typedef void (*on_send_cc_data_point_ret)(unsigned int cookie, unsigned long long to_client, int err_code);
SDK_API int tx_send_cc_data_point(unsigned long long to_client, unsigned int id, char * value, unsigned int * cookie, on_send_cc_data_point_ret ret_callback);
SDK_API int tx_send_cc_data_points(unsigned long long to_client, tx_data_point * data_points, int data_points_count, unsigned int * cookie, on_send_cc_data_point_ret ret_callback);
CXX_EXTERN_END
#endif
