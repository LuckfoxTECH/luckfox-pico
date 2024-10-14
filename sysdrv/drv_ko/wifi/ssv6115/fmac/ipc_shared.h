/**
 ****************************************************************************************
 *
 * @file ipc_shared.h
 *
 * @brief Shared data between both IPC modules.
 *
 * Copyright (C) BouffaloLab 2017-2018
 *
 ****************************************************************************************
 */

#ifndef _IPC_SHARED_H_
#define _IPC_SHARED_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "ipc_compat.h"
#include "lmac_types.h"
#include "lmac_mac.h"

/*
 * DEFINES AND MACROS
 ****************************************************************************************
 */
#define CO_BIT(pos) (1U<<(pos))

#define IPC_TXQUEUE_CNT     NX_TXQ_CNT
#define NX_TXDESC_CNT0      8
#define NX_TXDESC_CNT1      64
#define NX_TXDESC_CNT2      64
#define NX_TXDESC_CNT3      8
#define NX_TXDESC_CNT4      8

/*
 * Number of Host buffers available for Data Rx handling (through DMA)
 */
#define IPC_RXBUF_CNT       128

/*
 * Number of shared descriptors available for Data RX handling
 */
#define IPC_RXDESC_CNT      128

/*
 * RX Data buffers size (in bytes)
 */
#define IPC_RXBUF_SIZE 5120

/*
 * Number of Host buffers available for Emb->App MSGs sending (through DMA)
 */
#define IPC_MSGE2A_BUF_CNT       64
/*
 * Number of Host buffers available for Debug Messages sending (through DMA)
 */
#define IPC_DBGBUF_CNT       32

/*
 * Length used in MSGs structures
 */
#define IPC_A2E_MSG_BUF_SIZE    127 // size in 4-byte words
#define IPC_E2A_MSG_PARAM_SIZE   256 // size in 4-byte words
/*
 * Debug messages buffers size (in bytes)
 */
#define IPC_DBG_PARAM_SIZE       256

/*
 * Define used for Rx hostbuf validity.
 * This value should appear only when hostbuf was used for a Reception.
 */
#define RX_DMA_OVER_PATTERN 0xAAAAAA00

/*
 * Define used for MSG buffers validity.
 * This value will be written only when a MSG buffer is used for sending from Emb to App.
 */
#define IPC_MSGE2A_VALID_PATTERN 0xADDEDE2A

/*
 * Define used for Debug messages buffers validity.
 * This value will be written only when a DBG buffer is used for sending from Emb to App.
 */
#define IPC_DBG_VALID_PATTERN 0x000CACA0

/*
 *  Length of the receive vectors, in bytes
 */
#define DMA_HDR_PHYVECT_LEN    36

/*
 * Maximum number of payload addresses and lengths present in the descriptor
 */
#define NX_TX_PAYLOAD_MAX      6

/*
 ****************************************************************************************
 */
// c.f LMAC/src/tx/tx_swdesc.h
/// Descriptor filled by the Host
struct hostdesc
{
    /// Pointer to packet payload
    u32_l packet_addr;
    /// Size of the payload
    u16_l packet_len;

    /// Address of the status descriptor in host memory (used for confirmation upload)
 //   u32_l host_hdr_pads;
    /// Destination Address
    struct mac_addr eth_dest_addr;
    /// Source Address
    struct mac_addr eth_src_addr;
    /// Ethernet Type
    u16_l ethertype;
    /// Buffer containing the PN to be used for this packet
    u16_l pn[4];
    /// Sequence Number used for transmission of this MPDU
    u16_l sn;
    /// Timestamp of first transmission of this MPDU
    u16_l timestamp;
    /// Packet TID (0xFF if not a QoS frame)
    u8_l tid;
    /// Interface Id
    u8_l vif_idx;
    /// Station Id (0xFF if station is unknown)
    u8_l staid;
    /// TX flags
    u16_l flags;

    u16_l sw_ack;
    u16_l sw_seq;

};

struct txdesc_api
{
	/// add templately
    //u32_l ready;
    /// Information provided by Host
    struct hostdesc host;
};


struct txdesc_host
{
    u32_l ready;

    /// API of the embedded part
    struct txdesc_api api;
};

/// Comes from ipc_dma.h
/// Element in the pool of TX DMA bridge descriptors.
struct dma_desc
{
    /** Application subsystem address which is used as source address for DMA payload
      * transfer*/
    u32_l            src;
    /** Points to the start of the embedded data buffer associated with this descriptor.
     *  This address acts as the destination address for the DMA payload transfer*/
    u32_l            dest;
    /// Complete length of the buffer in memory
    u16_l            length;
    /// Control word for the DMA engine (e.g. for interrupt generation)
    u16_l            ctrl;
    /// Pointer to the next element of the chained list
    u32_l            next;
};

// Comes from la.h
/// Length of the configuration data of a logic analyzer
#define LA_CONF_LEN          10

/// Structure containing the configuration data of a logic analyzer
struct la_conf_tag
{
    u32_l conf[LA_CONF_LEN];
    u32_l trace_len;
    u32_l diag_conf;
};

/// Size of a logic analyzer memory
#define LA_MEM_LEN       (1024 * 1024)

/// Type of errors
enum
{
    /// Recoverable error, not requiring any action from Upper MAC
    DBG_ERROR_RECOVERABLE = 0,
    /// Fatal error, requiring Upper MAC to reset Lower MAC and HW and restart operation
    DBG_ERROR_FATAL
};

/// Maximum length of the SW diag trace
#define DBG_SW_DIAG_MAX_LEN   1024

/// Maximum length of the error trace
#define DBG_ERROR_TRACE_SIZE  256

/// Number of MAC diagnostic port banks
#define DBG_DIAGS_MAC_MAX     48

/// Number of PHY diagnostic port banks
#define DBG_DIAGS_PHY_MAX     32

/// Maximum size of the RX header descriptor information in the debug dump
#define DBG_RHD_MEM_LEN      (5 * 1024)

/// Maximum size of the RX buffer descriptor information in the debug dump
#define DBG_RBD_MEM_LEN      (5 * 1024)

/// Maximum size of the TX header descriptor information in the debug dump
#define DBG_THD_MEM_LEN      (10 * 1024)

/// Debug information forwarded to host when an error occurs
struct dbg_debug_info_tag
{
    /// Type of error (0: recoverable, 1: fatal)
    u32_l error_type;
    /// Pointer to the first RX Header Descriptor chained to the MAC HW
    u32_l rhd;
    /// Size of the RX header descriptor buffer
    u32_l rhd_len;
    /// Pointer to the first RX Buffer Descriptor chained to the MAC HW
    u32_l rbd;
    /// Size of the RX buffer descriptor buffer
    u32_l rbd_len;
    /// Pointer to the first TX Header Descriptors chained to the MAC HW
    u32_l thd[NX_TXQ_CNT];
    /// Size of the TX header descriptor buffer
    u32_l thd_len[NX_TXQ_CNT];
    /// MAC HW diag configuration
    u32_l hw_diag;
    /// Error message
    u32_l error[DBG_ERROR_TRACE_SIZE/4];
    /// SW diag configuration length
    u32_l sw_diag_len;
    /// SW diag configuration
    u32_l sw_diag[DBG_SW_DIAG_MAX_LEN/4];
    /// PHY channel information
    struct phy_channel_info chan_info;
    /// Embedded LA configuration
    struct la_conf_tag la_conf;
    /// MAC diagnostic port state
    u16_l diags_mac[DBG_DIAGS_MAC_MAX];
    /// PHY diagnostic port state
    u16_l diags_phy[DBG_DIAGS_PHY_MAX];
    /// MAC HW RX Header descriptor pointer
    u32_l rhd_hw_ptr;
    /// MAC HW RX Buffer descriptor pointer
    u32_l rbd_hw_ptr;
};

/// Full debug dump that is forwarded to host in case of error
struct dbg_debug_dump_tag
{
    /// Debug information
    struct dbg_debug_info_tag dbg_info;

    /// RX header descriptor memory
    u8_l rhd_mem[DBG_RHD_MEM_LEN];

    /// RX buffer descriptor memory
    u8_l rbd_mem[DBG_RBD_MEM_LEN];

    /// TX header descriptor memory
    u8_l thd_mem[NX_TXQ_CNT][DBG_THD_MEM_LEN];

    /// Logic analyzer memory
    u8_l la_mem[LA_MEM_LEN];
};

struct rxdesc_tag
{
    /// Host Buffer Address
    u32_l host_id;
    /// Length
    u32_l frame_len;
    /// Status
    u8_l status;
};


/**
 ****************************************************************************************
 *  @defgroup IPC_MISC IPC Misc
 *  @ingroup IPC
 *  @brief IPC miscellaneous functions
 ****************************************************************************************
 */

/// Message structure for MSGs from Emb to App
struct ipc_e2a_msg
{
    u16_l id;                ///< Message id.
    u16_l dummy_dest_id;                ///<
    u16_l dummy_src_id;                ///<
    u16_l param_len;         ///< Parameter embedded struct length.
    //u32_l param[IPC_E2A_MSG_PARAM_SIZE];  ///< Parameter embedded struct. Must be word-aligned.
    u32_l param[];  ///< Parameter embedded struct. Must be word-aligned.
};

/// Message structure for Debug messages from Emb to App
struct ipc_dbg_msg
{
    u32_l string[IPC_DBG_PARAM_SIZE/4]; ///< Debug string
    u32_l pattern;                    ///< Used to stamp a valid buffer
};

/// Message structure for MSGs from App to Emb.
/// Actually a sub-structure will be used when filling the messages.
struct ipc_a2e_msg
{
    u32_l dummy_word;                // used to cope with kernel message structure
    u32_l msg[IPC_A2E_MSG_BUF_SIZE]; // body of the msg
};

#endif // _IPC_SHARED_H_

