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

#ifndef __TX_TV_BARRAGE_H__
#define __TX_TV_BARRAGE_H__ 
#include "TXDeviceSDK.h"
CXX_EXTERN_BEGIN
enum tx_barrage_msg_element_type
{
    element_none = 0,
    element_text = 1,
    element_face = 2,
    element_image = 3,
    element_audio = 4,
};
typedef struct tag_tx_text_msg_element
{
    char * msg_text;
}tx_text_msg_element;
typedef struct tag_tx_face_msg_element
{
    unsigned int face_index;
}tx_face_msg_element;
typedef struct tag_tx_image_msg_element
{
    char * image_guid;
    int guid_length;
    char * image_url;
    char * image_thumb_url;
}tx_image_msg_element;
typedef struct tag_tx_audio_msg_element
{
    unsigned int voice_switch;
    char * audio_msg_url;
}tx_audio_msg_element;
typedef struct tag_tx_barrage_msg_element
{
    int msg_ele_type;
    void * msg_ele_point;
}tx_barrage_msg_element;
typedef struct tag_tx_barrage_msg
{
    unsigned long long group_id;
    unsigned long long from_target_id;
    unsigned int from_target_appid;
    unsigned int from_target_instid;
    char * group_name;
    char * from_nick;
    char * from_group_card;
    char * from_head_url;
    tx_barrage_msg_element * msg_ele_array;
    int msg_ele_count;
}tx_barrage_msg;
typedef struct tag_tx_barrage_notify
{
 void (*on_receive_barrage_msg)(tx_barrage_msg * pMsg);
}tx_barrage_notify;
SDK_API void tx_set_barrage_notify(tx_barrage_notify * notify);
CXX_EXTERN_END
#endif
