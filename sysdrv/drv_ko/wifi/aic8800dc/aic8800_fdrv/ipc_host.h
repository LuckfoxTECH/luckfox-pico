/**
 ******************************************************************************
 *
 * @file ipc_host.h
 *
 * @brief IPC module.
 *
 * Copyright (C) RivieraWaves 2011-2019
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

/**
 ******************************************************************************
 * @brief This structure is used to initialize the MAC SW
 *
 * The WLAN device driver provides functions call-back with this structure
 ******************************************************************************
 */
struct ipc_host_cb_tag {
	/// WLAN driver call-back function: send_data_cfm
	int (*send_data_cfm)(void *pthis, void *host_id);

	/// WLAN driver call-back function: recv_data_ind
	uint8_t (*recv_data_ind)(void *pthis, void *host_id);

	/// WLAN driver call-back function: recv_radar_ind
	uint8_t (*recv_radar_ind)(void *pthis, void *host_id);

	/// WLAN driver call-back function: recv_unsup_rx_vec_ind
	uint8_t (*recv_unsup_rx_vec_ind)(void *pthis, void *host_id);

	/// WLAN driver call-back function: recv_msg_ind
	uint8_t (*recv_msg_ind)(void *pthis, void *host_id);

	/// WLAN driver call-back function: recv_msgack_ind
	uint8_t (*recv_msgack_ind)(void *pthis, void *host_id);

	/// WLAN driver call-back function: recv_dbg_ind
	uint8_t (*recv_dbg_ind)(void *pthis, void *host_id);

	/// WLAN driver call-back function: prim_tbtt_ind
	void (*prim_tbtt_ind)(void *pthis);

	/// WLAN driver call-back function: sec_tbtt_ind
	void (*sec_tbtt_ind)(void *pthis);

};

/*
 * Struct used to store information about host buffers (DMA Address and local pointer)
 */
struct ipc_hostbuf {
	void    *hostid;     ///< ptr to hostbuf client (ipc_host client) structure
	uint32_t dma_addr;   ///< ptr to real hostbuf dma address
};

/// Definition of the IPC Host environment structure.
struct ipc_host_env_tag {
	/// Structure containing the callback pointers
	struct ipc_host_cb_tag cb;

	/// Pointer to the shared environment
	struct ipc_shared_env_tag *shared;

	#ifdef CONFIG_RWNX_FULLMAC
	// Array used to store the descriptor addresses
	struct ipc_hostbuf ipc_host_rxdesc_array[IPC_RXDESC_CNT];
	// Index of the host RX descriptor array (ipc_shared environment)
	uint8_t ipc_host_rxdesc_idx;
	/// Store the number of RX Descriptors
	uint8_t rxdesc_nb;
	#endif //(CONFIG_RWNX_FULLMAC)

	/// Fields for Data Rx handling
	// Index used for ipc_host_rxbuf_array to point to current buffer
	uint8_t ipc_host_rxbuf_idx;
	// Store the number of Rx Data buffers
	uint32_t rx_bufnb;
	// Store the size of the Rx Data buffers
	uint32_t rx_bufsz;

	/// Fields for Radar events handling
	// Global array used to store the hostid and hostbuf addresses
	struct ipc_hostbuf ipc_host_radarbuf_array[IPC_RADARBUF_CNT];
	// Index used for ipc_host_rxbuf_array to point to current buffer
	uint8_t ipc_host_radarbuf_idx;
	// Store the number of radar event buffers
	uint32_t radar_bufnb;
	// Store the size of the radar event buffers
	uint32_t radar_bufsz;

	///Fields for Unsupported frame handling
	// Global array used to store the hostid and hostbuf addresses
	struct ipc_hostbuf ipc_host_unsuprxvecbuf_array[IPC_UNSUPRXVECBUF_CNT];
	// Index used for ipc_host_unsuprxvecbuf_array to point to current buffer
	uint8_t ipc_host_unsuprxvecbuf_idx;
	// Store the number of unsupported rx vector buffers
	uint32_t unsuprxvec_bufnb;
	// Store the size of unsupported rx vector buffers
	uint32_t unsuprxvec_bufsz;

	// Index used that points to the first free TX desc
	uint32_t txdesc_free_idx[IPC_TXQUEUE_CNT][CONFIG_USER_MAX];
	// Index used that points to the first used TX desc
	uint32_t txdesc_used_idx[IPC_TXQUEUE_CNT][CONFIG_USER_MAX];
	// Array storing the currently pushed host ids for the BK queue
	void *tx_host_id0[CONFIG_USER_MAX][NX_TXDESC_CNT0];
	// Array storing the currently pushed host ids for the BE queue
	void *tx_host_id1[CONFIG_USER_MAX][NX_TXDESC_CNT1];
	// Array storing the currently pushed host ids for the VI queue
	void *tx_host_id2[CONFIG_USER_MAX][NX_TXDESC_CNT2];
	// Array storing the currently pushed host ids for the VO queue
	void *tx_host_id3[CONFIG_USER_MAX][NX_TXDESC_CNT3];
	#if NX_TXQ_CNT == 5
	// Array storing the currently pushed host ids for the BCN queue
	void *tx_host_id4[1][NX_TXDESC_CNT4];
	#endif
	// Pointer to the different host ids arrays, per IPC queue
	void **tx_host_id[IPC_TXQUEUE_CNT][CONFIG_USER_MAX];
	// Pointer to the different TX descriptor arrays, per IPC queue
	volatile struct txdesc_host *txdesc[IPC_TXQUEUE_CNT][CONFIG_USER_MAX];

	/// Fields for Emb->App MSGs handling
	// Global array used to store the hostid and hostbuf addresses for msg/ind
	struct ipc_hostbuf ipc_host_msgbuf_array[IPC_MSGE2A_BUF_CNT];
	// Index of the MSG E2A buffers array to point to current buffer
	uint8_t ipc_host_msge2a_idx;
	// Store the number of E2A MSG buffers
	uint32_t ipc_e2amsg_bufnb;
	// Store the size of the E2A MSG buffers
	uint32_t ipc_e2amsg_bufsz;

	/// E2A ACKs of A2E MSGs
	uint8_t msga2e_cnt;
	void *msga2e_hostid;

	/// Fields for Debug MSGs handling
	// Global array used to store the hostid and hostbuf addresses for Debug messages
	struct ipc_hostbuf ipc_host_dbgbuf_array[IPC_DBGBUF_CNT];
	// Index of the Debug messages buffers array to point to current buffer
	uint8_t ipc_host_dbg_idx;
	// Store the number of Debug messages buffers
	uint32_t ipc_dbg_bufnb;
	// Store the size of the Debug messages buffers
	uint32_t ipc_dbg_bufsz;

	/// Pointer to the attached object (used in callbacks and register accesses)
	void *pthis;
};

extern const int nx_txdesc_cnt[];
extern const int nx_txuser_cnt[];

#endif // _IPC_HOST_H_
