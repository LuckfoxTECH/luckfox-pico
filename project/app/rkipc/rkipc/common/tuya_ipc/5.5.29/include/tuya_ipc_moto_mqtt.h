#ifndef _TUYA_MOTO_MQTT_H_
#define _TUYA_MOTO_MQTT_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "cJSON.h"
#include "tuya_cloud_com_defs.h"
#include "tuya_cloud_types.h"

#define PUBLIC
#define PRIVATE

#define TOPIC_LEN 64

#define MODULE_NAME_LEN 16
#define MAX_MODULE 2
typedef VOID (*ProcessMotoMsg)(IN cJSON *msg);
typedef struct {
	CHAR_T moduleName[MODULE_NAME_LEN];
	ProcessMotoMsg cb;
} MOTO_MODULE_CB_T;

typedef struct {
	PRIVATE
	BOOL_T inited;
	BOOL_T booked;
	CHAR_T devId[GW_ID_LEN + 1];

	// CHAR_T      motoId[MOTO_ID_LEN];
	CHAR_T bookTopic[TOPIC_LEN];

	PUBLIC
	OPERATE_RET (*BookTopic)(VOID);
	OPERATE_RET (*BookTopicForce)(VOID);
	OPERATE_RET(*PublishMqttMsg)
	(IN CONST CHAR_T *motoId, IN CONST UINT_T protocol, IN CONST BYTE_T *p_data);
} MOTO_MQTT_T;

MOTO_MQTT_T *GetMotoMqttInstance(VOID);

int tuya_ipc_moto_mqtt_init();

#ifdef __cplusplus
}
#endif

#endif
