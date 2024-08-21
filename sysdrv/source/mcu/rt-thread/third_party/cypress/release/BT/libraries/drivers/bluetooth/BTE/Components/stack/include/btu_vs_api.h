
/*****************************************************************************
**
**  Name:       btu_vs_api.h
**
**  Description:Interface to BTU VS Application Programming Interface
**
**  Copyright (c) 2000-2013, Broadcom Corp., All Rights Reserved.
**  Broadcom Bluetooth Core. Proprietary and confidential.
**
*****************************************************************************/

#ifndef BTU_VS_API_H
#define BTU_VS_API_H

#include "btm_api.h"
#include "l2c_api.h"

/* vendor specific event handler needed type and callback */
#if (BTM_PCM2_INCLUDED == TRUE)
/* Define pcm2_action           */
enum
{
    BTM_PCM2_ACT_NONE,
    BTM_PCM2_ACT_SENT_ARC,
    BTM_PCM2_READ_PARAM,
    BTM_PCM2_WRITE_PARAM,
};
typedef UINT8 tBTU_PCM2_ACTION;

#endif


typedef struct
{
    tBTM_SCO_DATA_CB                *p_sco_data_cb;
    tBTM_SCO_PCM_PARAM              *p_pcm_param;
    tBTM_SCO_ROUTE_TYPE             path;
    BOOLEAN                         err_data_rpt;
} tBTU_SCO_PATH;

typedef struct
{
    tBTM_SCO_CODEC_TYPE codec_type;
    UINT8           role;
    UINT8           sample_rate;
    UINT8           clock_rate;
} tBTU_PCM_PARAM;

typedef struct
{
    BD_ADDR     bd_addr;
    UINT8       priority;
    BOOLEAN     reset;
    UINT8       direction;  /* L2CAP_DIRECTION_DATA_SOURCE (0)
                             * or L2CAP_DIRECTION_DATA_SINK(1)
                             * L2CAP_DIRECTION_IGNORE (legacy) */
} tBTU_SET_PRI_PARAM;

/* set local address parameter */
typedef struct
{
    BD_ADDR     bd_addr;    /* new local address to set */
    void        *p_cback;   /* set address completion callback function */
} tBTU_SET_ADDR_PARAM;

/* set TX power parameters */
typedef struct
{
    BD_ADDR_PTR bd_addr_ptr;
    int         power;
} tBTU_SET_TX_POWER_PARAM;

typedef struct
{
    BD_ADDR     bd_addr;
} tBTU_TBFC_CONN_PARAM;

typedef struct
{
    UINT16      hci_handle;
} tBTU_TBFC_SUSPEND_PARAM;

typedef struct
{
    BD_ADDR     bd_addr;
    UINT16      len;
    UINT16      handle;
    UINT8       *p_msg;
} tBTU_TBFC_LE_NOTIFY_PARAM;

typedef struct
{
    BD_ADDR     remote_bda;
    UINT8       link_role;
    UINT8       link_state;
    UINT16      conn_handle;
    BT_HDR      *p_buf;
} tBTU_TBFC_TRIGGER_PARAM;

typedef struct
{
    UINT8    *p;
    UINT8   hci_evt_len;
} tBTU_H4IBSS;

typedef struct
{
    BD_ADDR     remote_bda;
    UINT8       op_code;
    UINT16      msg_len;
    UINT8       *p_data;
} tBTU_BURST_DATA_PARAM;

typedef struct
{
    UINT16  hci_handle;
} tBTU_READ_BPCS_FEAT;

/* requested topology state request */
typedef struct
{
    UINT16 request_state_mask;
} tBTU_TOPOLOGY_CHECK;

#endif  /* BTU_VS_API_H */

