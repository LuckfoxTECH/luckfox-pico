/**
 ******************************************************************************
 *
 * @file ipc_host.h
 *
 * @brief IPC module.
 *
 * Copyright (C) BouffaloLab 2017-2018
 *
 ******************************************************************************
 */
 #ifndef _IPC_HOST_H_
 #define _IPC_HOST_H_
 
 /*
  * INCLUDE FILES
  ******************************************************************************
  */
 #include "ipc_shared.h"
 #ifndef __KERNEL__
 #include "arch.h"
 #else
 #include "ipc_compat.h"
 #endif
 
//  #include "fmac_txq.h"
 #include "hal_desc.h"
 
 /**
  ******************************************************************************
  * @brief This structure is used to initialize the MAC SW
  *
  * The WLAN device driver provides functions call-back with this structure
  ******************************************************************************
  */
 struct ipc_host_cb_tag
 {
     /// WLAN driver call-back function: recv_msg_ind
     uint8_t (*recv_msg_ind)(void *pthis, void *host_id);
 
     /// WLAN driver call-back function: recv_msgack_ind
     uint8_t (*recv_msgack_ind)(void *pthis, void *host_id);
 };

 /// Definition of the IPC Host environment structure.
 struct ipc_host_env_tag
 {
     /// Structure containing the callback pointers
     struct ipc_host_cb_tag cb;
 
     /// E2A ACKs of A2E MSGs
     uint8_t msga2e_cnt;
     void *msga2e_hostid;
 
     /// Pointer to the attached object (used in callbacks and register accesses)
     void *pthis;
 };
 
 /**
  ******************************************************************************
  * @brief Initialize the IPC running on the Application CPU.
  *
  * This function:
  *   - initializes the IPC software environments
  *   - enables the interrupts in the IPC block
  *
  * @param[in]   env   Pointer to the IPC host environment
  *
  * @warning Since this function resets the IPC Shared memory, it must be called
  * before the LMAC FW is launched because LMAC sets some init values in IPC
  * Shared memory at boot.
  *
  ******************************************************************************
  */
 void ssv_ipc_host_init(struct ipc_host_env_tag *env,
                   struct ipc_host_cb_tag *cb,
                   void *pthis);
 
 #endif // _IPC_HOST_H_
 