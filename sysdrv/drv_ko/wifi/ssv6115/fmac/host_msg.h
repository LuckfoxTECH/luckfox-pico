#ifndef __HOST_MSG_H__
#define __HOST_MSG_H__


/*******************************************************************************
 *         Include Files
 ******************************************************************************/

/*******************************************************************************
 *         Defines
 ******************************************************************************/


/*******************************************************************************
 *         Enumerations
 ******************************************************************************///if change must sync to hci.h in FW

typedef enum en_host_priv_msg_type
{
    E_HOST_PRIV_MSG_TYPE_RX_REORD_CREATE = 0x1000,   ///< 0x1000, Create AMPDU RX reordering
    E_HOST_PRIV_MSG_TYPE_RX_REORD_DEL,               ///< 0x1001, Delete AMPDU RX reordering
    E_HOST_PRIV_MSG_TYPE_RX_REORD_TIMEOUT,           ///< 0x1002, Timeout for AMPDU RX reordering
    E_HOST_PRIV_MSG_TYPE_PROBE_CLIENT,               ///< 0x1003, polling client
    E_HOST_PRIV_MSG_TYPE_CHECK_FW_STATUS,              ///< 0x1004, polling client
    E_HOST_PRIV_MSG_TYPE_SEND_KEEP_ALIVE,              ///< 0x1005, sending keep alive frame
    E_HOST_PRIV_MSG_TYPE_END = 0xFFFF,
}EN_HOST_PRIV_MSG_TYPE;

/*******************************************************************************
 *         Structures
 ******************************************************************************/

/*******************************************************************************
 *         Variables
 ******************************************************************************/


/*******************************************************************************
 *         Funcations
 ******************************************************************************/
#endif /* __HOST_MSG_H__ */
