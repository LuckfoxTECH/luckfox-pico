/* Copyright (c) Rockchip Electronics Co. Ltd. */

#ifndef __FILE_MSG_H__
#define __FILE_MSG_H__

typedef enum _FILE_NOTIFY{
	FILE_NEW = 0, /* Recording now */
	FILE_END, /* Record end */
	FILE_OVERLONG, /* File size overlong */
	OPEN_FAILED,
	WRITE_FAILED,
	WRITE_SLOW,
	CARD_NONEXIST,
} FILE_NOTIFY;

typedef void (*FILE_EVENT_CALLBACK)(int cmd, void *msg0, void *msg1);

void file_msg_reg_callback(FILE_EVENT_CALLBACK call);
void file_msg_notify(FILE_NOTIFY notify, const char *filename, int arg);
#endif
