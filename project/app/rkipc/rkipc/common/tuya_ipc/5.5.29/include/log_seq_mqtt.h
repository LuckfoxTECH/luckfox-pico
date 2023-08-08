/**
 * @file log_seq_mqtt.h
 * @brief provide mqtt log sequence
 * @version 0.1
 * @date 2020-03-27
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __LOG_SEQ_MQTT_H
#define __LOG_SEQ_MQTT_H

#include "log_seq.h"

// define log sequence object is "mqtt", include this head file can recorde log sequence to "mqtt"
// module
#define LOGSEQ_OBJ "mqtt"

/**
 * @brief define mqtt detail log sequence information
 *        user can easy know bug location from this sequence, which include type, value, location
 */
typedef enum {
	// generic
	LOGSEQ_MQTT_TIME = 0, // type:string, value:system time, location:NULL
	LOGSEQ_MQTT_RSSI = 9, // type:string, value:RSSI signal strength, location:__mqtt_log_seq_err

	// DNS
	LOGSEQ_MQTT_DNS_RESV = 10, // type:error code, value:DNS parse, location:__mq_ctrl_task

	// connect
	LOGSEQ_MQTT_SOCK_CONN = 20, // type:error code, value:soket connect, location:__mq_ctrl_task
	LOGSEQ_MQTT_CONN = 21, // type:system error code, value:mqtt connect, location:__mq_ctrl_task
	LOGSEQ_MQTT_CONN_ACK =
	    22, // type:system error code, value:mqtt conect acknowleage, location:mqtt_connect

	// subscirbe
	LOGSEQ_MQTT_SUB =
	    30, // type:system error code, value:mqtt subscription, location:__mq_ctrl_task
	LOGSEQ_MQTT_SUB_ACK =
	    31, // type:system error code, value:mqtt subscription acknowleage, location:mqtt_subscribe

	// publish
	LOGSEQ_MQTT_PUB = 40, // type:system error code, value:mqtt publish, location:mqtt_publish_async
	LOGSEQ_MQTT_PUB_ACK =
	    41, // type:system error code, value:mqtt publish acknowleage, location:mqtt_publish
	LOGSEQ_MQTT_PUB_ACK_CLOSE = 42, // type:string, value:mqtt failed to publish acknowleage 3 times
	                                // then closed, location:LOGSEQ_MQTT_PUB_ACK_CLOSE

	// process logic
	LOGSEQ_MQTT_RECV = 50, // type:system error code, value:receive mqtt, location:__mq_ctrl_task
	LOGSEQ_MQTT_LOGIC_PROC =
	    51, // type:error code, value:mqtt logic process, location:__mq_ctrl_task
	LOGSEQ_MQTT_CONN_RECV =
	    52, // type:error code, value:mqtt connect receive, location:__mq_ctrl_task

	// ping
	LOGSEQ_MQTT_PING =
	    60, // type:system error code, value:mqtt ping failed, location:__alive_tm_msg_cb
	LOGSEQ_MQTT_PING_TIMEOUT = 61, // type:string, value:time of recorde，location:__resp_tm_cb
} MQTT_LOG_SEQ_E;

#endif
