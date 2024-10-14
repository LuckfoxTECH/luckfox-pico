/*****************************************************************************/
/*                                                                           */
/*  Name:          hcidext.c                                                 */
/*                                                                           */
/*  Description:   External definitions for HCI Data unit                    */
/*                                                                           */
/*  Date        Modification                                                 */
/*  ------------------------                                                 */
/*  08/01/99    Victor  Create                                               */
/*                                                                           */
/*  Copyright (c) 1999-2004, WIDCOMM Inc., All Rights Reserved.              */
/*  WIDCOMM Bluetooth Core.  Interface is based on the TI Software           */
/*  Acrhitechture.                                                           */
/*                                                                           */
/*****************************************************************************/
#ifndef HCIDEXT_H
#define HCIDEXT_H

extern void    hcid_init(void);
extern void    hcid_reset(void);
extern void    hcid_update_timer(void);
extern UINT8   hcid_create_acl_pipe(UINT8 am_addr, UINT16 handle, UINT16 packet_type,
                                    UINT8 loopback_mode);
extern void    hcid_remove_acl_pipe(UINT8 conn_id);
extern void    hcid_set_rexmit_tout(UINT8 conn_id, UINT16 tout);
extern UINT16  hcid_get_rexmit_tout(UINT8 conn_id);
extern BOOLEAN hcid_suspend(int am_addr);
extern void    hcid_resume(int conn_id);
extern void    hcid_change_packet_types(int conn_id, UINT16 new_pkt_types);
extern void    hcid_flush(int conn_id);
extern void    hcid_set_to_host_flow_control(UINT8 value);
extern void    hcid_set_host_buffer_size(UINT16 acl_len, UINT16 sco_len,
        UINT16 acl_num, UINT16 sco_num);
extern void    hcid_host_packets_done(UINT8 conn_id, UINT16 num_pkts);
extern void    hcid_process_dn_acl_data(int conn_id, BT_HDR *p_buf);
extern void    hcid_process_tx_complete(BT_HDR *p_flow);
extern void    hcid_process_up_acl_data(BT_HDR *p_flow);
extern UINT32  hcid_num_rx_bytes(UINT8 conn_id);
extern UINT32  hcid_num_tx_bytes(UINT8 conn_id);
extern void    hcid_set_data_rate(UINT8 conn_id, UINT8 data_rate);
extern UINT16  hcid_read_failed_contact_counter(UINT8 conn_id);
extern void    hcid_reset_failed_contact_counter(UINT8 conn_id);
extern void    hcid_process_dn_sco_data(SCOCB *p_sco, BT_HDR *p_buf);

#endif
