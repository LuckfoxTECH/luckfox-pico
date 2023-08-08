/*********************************************************************************
 *Copyright(C),2017, 涂鸦科技 www.tuya.comm
 *FileName:    tuya_ipc_doorbell.h
 **********************************************************************************/

#ifndef _TUYA_IPC_DOORBELL_H
#define _TUYA_IPC_DOORBELL_H

#ifdef __cplusplus
extern "C" {
#endif

OPERATE_RET mqc_door_bell_press(IN TIME_T current_time, IN CHAR_T *suffix);

OPERATE_RET mqc_door_bell_press_generic(IN CHAR_T *etype, IN TIME_T current_time,
                                        IN CHAR_T *suffix);

OPERATE_RET mqc_door_bell_answer(CHAR_T *extra_data);

/***********************************************************
 *  Function: tuya_ipc_doorbell_event
 *  Description: A mandatory API to be implemented
 *  Input: CALLED by TUYA sdk
 *  Output: none
 *  Return:
 ***********************************************************/
VOID tuya_ipc_doorbell_event(char *action);

/***********************************************************
 *  Function: tuya_ipc_doorbell_init
 *  Description: init ac doorbell
 *  Input: none
 *  Output: none
 *  Return:
 ***********************************************************/
OPERATE_RET tuya_ipc_doorbell_init();

#ifdef __cplusplus
}
#endif

#endif /*_TUYA_IPC_DOORBELL_H*/
