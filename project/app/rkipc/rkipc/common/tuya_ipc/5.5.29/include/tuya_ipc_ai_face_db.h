#ifndef __TUYA_IPC_AI_FACE_DB_H__
#define __TUYA_IPC_AI_FACE_DB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_ipc_cloud_storage.h"

#define DB_FILE_PATH 128
#define DB_FILE_NAME 6

typedef struct {
	CHAR_T *face_image_add;
	INT_T face_id;

} AI_FACE_DB_DATA;

/**
 * \fn OPERATE_RET tuya_ipc_ai_face_db_init
 * \brief   user should set the path of face feature db saved locally,
 *          also set the lenth of face feature db
 * \return OPERATE_RET
 */

OPERATE_RET tuya_ipc_ai_face_db_init(CHAR_T *pPath, IN INT_T featureLen);

/**
 * \fn OPERATE_RET tuya_ipc_ai_face_db_add
 * \brief add face feature db ,send in face_id and data
 *
 * \return OPERATE_RET
 */

OPERATE_RET tuya_ipc_ai_face_db_add(IN CHAR_T *pFace_id, IN PVOID_T data);

/**
 * \fn OPERATE_RET tuya_ipc_ai_face_db_delete
 * \brief delete face feature db, send int face_id
 *
 * \return OPERATE_RET
 */

OPERATE_RET tuya_ipc_ai_face_db_delete(IN CHAR_T *face_id);

/**
 * \fn OPERATE_RET tuya_ipc_ai_face_db_read
 * \brief get face feature db, send in face_id
 *
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_ai_face_db_read(IN CHAR_T *pFace_id, OUT PVOID_T pData);

/**
 * \fn OPERATE_RET tuya_ipc_ai_face_get_db_path
 * \brief get the path of face feature db set in tuya_ipc_ai_face_db_init
 *
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_ai_face_get_db_path(IN CHAR_T *pPath);

#ifdef __cplusplus
}
#endif

#endif //__TUYA_IPC_AI_FACE_DB_H__
