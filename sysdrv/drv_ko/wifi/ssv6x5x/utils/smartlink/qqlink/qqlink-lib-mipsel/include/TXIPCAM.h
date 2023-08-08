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

#ifndef __IPCAMERA_H__
#define __IPCAMERA_H__ 
#include "TXSDKCommonDef.h"
#include "TXDeviceSDK.h"
CXX_EXTERN_BEGIN
enum definition {
 def_low = 1,
 def_middle = 2,
 def_high = 3,
};
enum rotate_direction {
 rotate_direction_left = 1,
 rotate_direction_right = 2,
 rotate_direction_up = 3,
 rotate_direction_down = 4,
};
enum rotate_degree {
 rotate_degree_h_min = 0,
 rotate_degree_h_max = 360,
 rotate_degree_v_min = 0,
 rotate_degree_v_max = 180,
};
typedef struct _tx_ipcamera_notify {
 int (*on_set_definition)(int definition, char *cur_definition, int cur_definition_length);
 int (*on_control_rotate)(int rotate_direction, int rotate_degree);
} tx_ipcamera_notify;
SDK_API int tx_ipcamera_set_callback(tx_ipcamera_notify *notify);
typedef struct _tx_history_video_range
{
 unsigned int start_time;
 unsigned int end_time;
} tx_history_video_range;
typedef struct _tx_history_video_notify
{
 void (*on_fetch_history_video)(unsigned int last_time, int max_count, int *count, tx_history_video_range * range_list);
 void (*on_play_history_video)(unsigned int play_time);
} tx_history_video_notify;
SDK_API void tx_init_history_video_notify(tx_history_video_notify *notify);
CXX_EXTERN_END
#endif
