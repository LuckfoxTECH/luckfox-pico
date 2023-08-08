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

#ifndef __TX_DATA_POINT_H__
#define __TX_DATA_POINT_H__ 
#include "TXSDKCommonDef.h"
#include "TXMsg.h"
CXX_EXTERN_BEGIN
typedef struct tag_tx_data_point
{
    unsigned int id;
    char * value;
    unsigned int seq;
    unsigned int ret_code;
}tx_data_point;
typedef struct tag_tx_data_point_notify
{
    void (*on_receive_data_point)(unsigned long long from_client, tx_data_point * data_points, int data_points_count);
} tx_data_point_notify;
SDK_API int tx_init_data_point(const tx_data_point_notify *notify);
typedef void (*on_report_data_point_ret)(unsigned int cookie, int err_code);
SDK_API int tx_report_data_point(unsigned int id, char * value, unsigned int * cookie, on_report_data_point_ret ret_callback);
SDK_API int tx_report_data_points(tx_data_point * data_points, int data_points_count, unsigned int * cookie, on_report_data_point_ret ret_callback);
typedef void (*on_ack_data_point_ret)(unsigned int cookie, unsigned long long from_client, int err_code);
SDK_API int tx_ack_data_point(unsigned long long from_client, unsigned int id, char * value, unsigned int seq, unsigned int ret_code, unsigned int * cookie, on_ack_data_point_ret ret_callback);
SDK_API int tx_ack_data_points(unsigned long long from_client, tx_data_point * data_points, int data_points_count, unsigned int * cookie, on_ack_data_point_ret ret_callback);
CXX_EXTERN_END
#endif
