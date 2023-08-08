#ifndef _MOTOR_PRESET_OPERATION_H_
#define _MOTOR_PRESET_OPERATION_H_
#include "tuya_cloud_types.h"

#define MAX_PRESET_NUM 6

typedef struct {
	INT_T pan;
	INT_T tilt;
	INT_T zoom;
} S_PRESET_PTZ;

typedef struct {
	CHAR_T id[32];    // id in server
	CHAR_T name[32];  // preset point name
	INT_T mpId;       // index ID
	S_PRESET_PTZ ptz; // ptz for preset position
} S_PRESET_POSITION;

typedef struct {
	INT_T num;
	S_PRESET_POSITION position[MAX_PRESET_NUM];
} S_PRESET_CFG;

/**
 * \fn OPERATE_RET tuya_ipc_preset_add(S_PRESET_POSITION* preset_pos)
 * \brief add one preset point
 * \param[in] preset_pos
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_preset_add(S_PRESET_POSITION *preset_pos);

/**
 * \fn OPERATE_RET tuya_ipc_preset_del(IN CHAR_T* preset_id)
 * \brief delete one preset point.
 * \param[in] preset_id is the "devId field of the JSON of DP TUYA_DP_PRESET_SET"
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_preset_del(IN CHAR_T *preset_id);

/**
 * \fn OPERATE_RET tuya_ipc_preset_get(S_PRESET_CFG *preset_cfg)
 * \brief get all preset positions stored in tuya cloud.
 * \param[in out] preset_cfg
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_preset_get(S_PRESET_CFG *preset_cfg);

/**
 * \fn OPERATE_RET tuya_ipc_preset_add_pic(CHAR_T *addr, UINT_T size)
 * \brief upload a snapshot for current preset position
 * \param[in] addr/size: address and size of the picture to be uploaded
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_preset_add_pic(CHAR_T *addr, UINT_T size);

#endif
