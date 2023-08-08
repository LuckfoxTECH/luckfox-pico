#ifndef _TUYA_IPC_SKILL_H_
#define _TUYA_IPC_SKILL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_cloud_error_code.h"
#include "tuya_cloud_types.h"

#define SKILL_INFO_LEN 512

typedef enum {
	TUYA_IPC_SKILL_CLOUDSTG,
	TUYA_IPC_SKILL_WEBRTC,
	TUYA_IPC_SKILL_LOWPOWER,
	TUYA_IPC_SKILL_AIDETECT,
	TUYA_IPC_SKILL_LOCALSTG,
	TUYA_IPC_SKILL_CLOUDGW,
	TUYA_IPC_SKILL_CLOUDGW_INFO,
	TUYA_IPC_SKILL_DOORBELLSTG,
	TUYA_IPC_SKILL_P2P,
	TUYA_IPC_SKILL_PX,
} TUYA_IPC_SKILL_E;

typedef union {
	INT_T value;
	CHAR_T info[SKILL_INFO_LEN];
} tuya_ipc_skill_param_u;

OPERATE_RET tuya_ipc_skill_enable(TUYA_IPC_SKILL_E skill, tuya_ipc_skill_param_u *param);

#ifdef __cplusplus
}
#endif

#endif /*_TUYA_IPC_SKILL_H_*/