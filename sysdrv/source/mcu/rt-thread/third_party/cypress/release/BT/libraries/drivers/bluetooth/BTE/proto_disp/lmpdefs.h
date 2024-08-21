/*****************************************************************************/
/*                                                                           */
/*  Name        lmpdefs.h                                                    */
/*  $Header: /BG_00.15.01.RVI/stack/include/lmpdefs.h 5     11/03/00 3:48p Ravindras $                                                               */
/*                                                                           */
/*  Function    this file contains definitions for teh LMP unit              */
/*                                                                           */
/*  Date        Modification                                                 */
/*  ------------------------                                                 */
/*  02/19/99    Victor  Create                                               */
/*  07/31/99    Victor  Modifications made for BT specification V1.0a        */
/*  01/01/00    Victor  Modifications made for BT specification V1.0b        */
/*                                                                           */
/*  Copyright (c) 1999, 2000, Widcomm Inc., All Rights Reserved.             */
/*  Widcomm Bluetooth Core. Proprietary and confidential.                    */
/*                                                                           */
/*****************************************************************************/

#ifndef LMPDEFS_H
#define LMPDEFS_H

#include "hcimsgs.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef LMP_TEST
#define LMP_PACKET_DESC_MAXLEN  1024
#endif

/* Define parameters of the LMP version protocol */
#define LMP_VERSION_NUMBER      0x01    /* Version number for BT spec 1.1   */
#define LMP_COMPID              0x0011  /* Widcomm's LMP Comp Id is 17 (decimal) */
#define LMP_SUBVERSION_NUMBER   0x0001  /* Current implementation version   */

/*
** Define packet size
*/
#define DM1_PACKET_SIZE  17
#define DH1_PACKET_SIZE  27
#define DM3_PACKET_SIZE  121
#define DH3_PACKET_SIZE  183
#define DM5_PACKET_SIZE  224
#define DH5_PACKET_SIZE  339
#define AUX1_PACKET_SIZE 29
#define HV1_PACKET_SIZE  10
#define HV2_PACKET_SIZE  20
#define HV3_PACKET_SIZE  30
#define DV_PACKET_SIZE   9
#define EDR2_DH1_PACKET_SIZE 54
#define EDR2_DH3_PACKET_SIZE 367
#define EDR2_DH5_PACKET_SIZE 679
#define EDR3_DH1_PACKET_SIZE 83
#define EDR3_DH3_PACKET_SIZE 552
#define EDR3_DH5_PACKET_SIZE 1021

/*
** SCO slot period definitions
*/
#define LMP_T_SCO_HV1   2
#define LMP_T_SCO_HV2   4
#define LMP_T_SCO_HV3   6

/*
** SCO packet types
*/
#define LMP_SCO_PKT_HV1         0
#define LMP_SCO_PKT_HV2         1
#define LMP_SCO_PKT_HV3         2
#define LMP_SCO_PKT_DV          3

/*
** SCO air modes
*/
#define LMP_SCO_U_LAW           0
#define LMP_SCO_A_LAW           1
#define LMP_SCO_CVSD            2
#define LMP_SCO_TRANSPNT        3
#define LMP_SCO_ANY             0xFF

/*
** Paging class definitions
*/
#define LMP_PAGING_CLASS_R0     0
#define LMP_PAGING_CLASS_R1     1
#define LMP_PAGING_CLASS_R2     2


#define MAX_SCO_CONNECTIONS     3

/* Default Poll Inteval */
#define LMP_DEF_POLL_INTERVAL   40

/*
** Frame definitions
*/
#define L_CH_L2CAP_UNDEFINED        0x00
#define L_CH_L2CAP_CONTINUATION     0x01
#define L_CH_L2CAP_START            0x02
#define L_CH_LM_FRAME               0x03
#define LMP_FLOW                    0x04

#define LMP_TRANS_MASTER            0
#define LMP_TRANS_SLAVE             1

#define LMP_NAME_REQ                1
#define LMP_NAME_REQ_LEN                2
#define LMP_NAME_RES                2
#define LMP_NAME_RES_LEN                17
#define LMP_ACCEPTED                3
#define LMP_ACCEPTED_LEN                2
#define LMP_NOT_ACCEPTED            4
#define LMP_NOT_ACCEPTED_LEN            3
#define LMP_CLKOFFSET_REQ           5
#define LMP_CLKOFFSET_REQ_LEN           1
#define LMP_CLKOFFSET_RES           6
#define LMP_CLKOFFSET_RES_LEN           3
#define LMP_DETACH                  7
#define LMP_DETACH_LEN                  2
#define LMP_IN_RAND                 8
#define LMP_IN_RAND_LEN                 17
#define LMP_COMBINATION_KEY         9
#define LMP_COMBINATION_KEY_LEN         17
#define LMP_UNIT_KEY                10
#define LMP_UNIT_KEY_LEN                17
#define LMP_AU_RAND                 11
#define LMP_AU_RAND_LEN                 17
#define LMP_SRES                    12
#define LMP_SRES_LEN                    5
#define LMP_TEMP_RAND               13
#define LMP_TEMP_RAND_LEN               17
#define LMP_TEMP_KEY                14
#define LMP_TEMP_KEY_LEN                17
#define LMP_ENCRPT_MODE_REQ         15
#define LMP_ENCRPT_MODE_REQ_LEN         2
#define LMP_ENCRPT_KEY_SIZE_REQ     16
#define LMP_ENCRPT_KEY_SIZE_REQ_LEN     2
#define LMP_START_ENCRPT_REQ        17
#define LMP_START_ENCRPT_REQ_LEN        17
#define LMP_STOP_ENCRPT_REQ         18
#define LMP_STOP_ENCRPT_REQ_LEN         1
#define LMP_SWITCH_REQ              19

#ifdef ERRATA_1190
#define LMP_SWITCH_REQ_LEN              5
#else
#define LMP_SWITCH_REQ_LEN              1
#endif

#define LMP_HOLD                    20

#ifdef ERRATA_1188
#define LMP_HOLD_LEN                    7
#else
#define LMP_HOLD_LEN                    3
#endif

#define LMP_HOLD_REQ                21

#ifdef ERRATA_1188
#define LMP_HOLD_REQ_LEN                7
#else
#define LMP_HOLD_REQ_LEN                3
#endif

#define LMP_SNIFF                   22
#define LMP_SNIFF_LEN                   10
#define LMP_SNIFF_REQ               23
#define LMP_SNIFF_REQ_LEN               10
#define LMP_UNSNIFF_REQ             24
#define LMP_UNSNIFF_REQ_LEN             1
#define LMP_PARK_REQ                25
#define LMP_PARK_REQ_LEN                17
#define LMP_PARK                    26
#define LMP_PARK_LEN                    17
#define LMP_SET_BRDCST_SCAN_WINDOW  27
#define LMP_SET_BRDCST_SCAN_WINDOW_LEN  6
#define LMP_MODIFY_BEACON           28
#define LMP_MODIFY_BEACON_LEN           13
#define LMP_UNPARK_BD_ADDR_REQ      29
#define LMP_UNPARK_BD_ADDR_REQ_LEN      9 /* No Db no second am_addr */
#define LMP_UNPARK_PM_ADDR_REQ      30
#define LMP_UNPARK_PM_ADDR_REQ_LEN      4 /* No Db no second am_addr */
#define LMP_INCR_POWER_REQ          31
#define LMP_INCR_POWER_REQ_LEN          2
#define LMP_DECR_POWER_REQ          32
#define LMP_DECR_POWER_REQ_LEN          2
#define LMP_MAX_POWER               33
#define LMP_MAX_POWER_LEN               1
#define LMP_MIN_POWER               34
#define LMP_MIN_POWER_LEN               1
#define LMP_AUTO_RATE               35
#define LMP_AUTO_RATE_LEN               1
#define LMP_PREFERRED_RATE          36
#define LMP_PREFERRED_RATE_LEN          2
#define LMP_VERSION_REQ             37
#define LMP_VERSION_REQ_LEN             6
#define LMP_VERSION_RES             38
#define LMP_VERSION_RES_LEN             6
#define LMP_FEATURES_REQ            39
#define LMP_FEATURES_REQ_LEN            9
#define LMP_FEATURES_RES            40
#define LMP_FEATURES_RES_LEN            9
#define LMP_QOS                     41
#define LMP_QOS_LEN                     4
#define LMP_QOS_REQ                 42
#define LMP_QOS_REQ_LEN                 4
#define LMP_SCO_LINK_REQ            43
#define LMP_SCO_LINK_REQ_LEN            7
#define LMP_REMOVE_SCO_LINK_REQ     44
#define LMP_REMOVE_SCO_LINK_REQ_LEN     3
#define LMP_MAX_SLOTS               45
#define LMP_MAX_SLOTS_LEN               2
#define LMP_MAX_SLOTS_REQ           46
#define LMP_MAX_SLOTS_REQ_LEN           2
#define LMP_TIMING_ACCURACY_REQ     47
#define LMP_TIMING_ACCURACY_REQ_LEN     1
#define LMP_TIMING_ACCURACY_RES     48
#define LMP_TIMING_ACCURACY_RES_LEN     3
#define LMP_SETUP_COMPLETE          49
#define LMP_SETUP_COMPLETE_LEN          1
#define LMP_USE_PERMANENT_KEY       50
#define LMP_USE_PERMANENT_KEY_LEN       1
#define LMP_HOST_CONNECTION_REQ     51
#define LMP_HOST_CONNECTION_REQ_LEN     1
#define LMP_SLOT_OFFSET             52
#define LMP_SLOT_OFFSET_LEN             9
#define LMP_PAGE_MODE_REQ           53
#define LMP_PAGE_MODE_REQ_LEN           3
#define LMP_PAGE_SCAN_MODE_REQ      54
#define LMP_PAGE_SCAN_MODE_REQ_LEN      3
#define LMP_SUPERVISION_TIMEOUT     55
#define LMP_SUPERVISION_TIMEOUT_LEN     3
#define LMP_TEST_ACTIVATE           56
#define LMP_TEST_ACTIVATE_LEN           1
#define LMP_TEST_CONTROL            57
#define LMP_TEST_CONTROL_LEN            10

#define LMP_MAX_OPCODE  57

#define LMP_MAX_FRAME_LEN               (17 + 2)

#define LMP_FEATURES_LEN                8
#define LMP_RAND_LEN                    16
#define LMP_AUTH_RES_LEN                3
#define LMP_KEY_LEN                     16

/*
**  Features encoding
*/
#define LMP_FEATURE_3_SLOT_PACKETS_MASK 0x01
#define LMP_FEATURE_3_SLOT_PACKETS_OFF  0
#define LMP_3_SLOT_PACKETS_SUPPORTED(x) ((x)[LMP_FEATURE_3_SLOT_PACKETS_OFF] & LMP_FEATURE_3_SLOT_PACKETS_MASK)

#define LMP_FEATURE_5_SLOT_PACKETS_MASK 0x02
#define LMP_FEATURE_5_SLOT_PACKETS_OFF  0
#define LMP_5_SLOT_PACKETS_SUPPORTED(x) ((x)[LMP_FEATURE_5_SLOT_PACKETS_OFF] & LMP_FEATURE_5_SLOT_PACKETS_MASK)

#define LMP_FEATURE_ENCRYPTION_MASK     0x04
#define LMP_FEATURE_ENCRYPTION_OFF      0
#define LMP_ENCRYPTION_SUPPORTED(x)     ((x)[LMP_FEATURE_ENCRYPTION_OFF] & LMP_FEATURE_ENCRYPTION_MASK)

#define LMP_FEATURE_SLOT_OFFSET_MASK    0x08
#define LMP_FEATURE_SLOT_OFFSET_OFF     0
#define LMP_SLOT_OFFSET_SUPPORTED(x)    ((x)[LMP_FEATURE_SLOT_OFFSET_OFF] & LMP_FEATURE_SLOT_OFFSET_MASK)

#define LMP_FEATURE_TIMING_ACC_MASK     0x10
#define LMP_FEATURE_TIMING_ACC_OFF      0
#define LMP_TIMING_ACC_SUPPORTED(x)     ((x)[LMP_FEATURE_TIMING_ACC_OFF] & LMP_FEATURE_TIMING_ACC_MASK)

#define LMP_FEATURE_SWITCH_MASK         0x20
#define LMP_FEATURE_SWITCH_OFF          0
#define LMP_SWITCH_SUPPORTED(x)         ((x)[LMP_FEATURE_SWITCH_OFF] & LMP_FEATURE_SWITCH_MASK)

#define LMP_FEATURE_HOLD_MODE_MASK      0x40
#define LMP_FEATURE_HOLD_MODE_OFF       0
#define LMP_HOLD_MODE_SUPPORTED(x)      ((x)[LMP_FEATURE_HOLD_MODE_OFF] & LMP_FEATURE_HOLD_MODE_MASK)

#define LMP_FEATURE_SNIFF_MODE_MASK     0x80
#define LMP_FEATURE_SNIFF_MODE_OFF      0
#define LMP_SNIFF_MODE_SUPPORTED(x)      ((x)[LMP_FEATURE_SNIFF_MODE_OFF] & LMP_FEATURE_SNIFF_MODE_MASK)

#define LMP_FEATURE_PARK_MODE_MASK      0x01
#define LMP_FEATURE_PARK_MODE_OFF       1
#define LMP_PARK_MODE_SUPPORTED(x)      ((x)[LMP_FEATURE_PARK_MODE_OFF] & LMP_FEATURE_PARK_MODE_MASK)

#define LMP_FEATURE_RSSI_MASK           0x02
#define LMP_FEATURE_RSSI_OFF            1
#define LMP_RSSI_SUPPORTED(x)           ((x)[LMP_FEATURE_RSSI_OFF] & LMP_FEATURE_RSSI_MASK)

#define LMP_FEATURE_CQM_DATA_RATE_MASK  0x04
#define LMP_FEATURE_CQM_DATA_RATE_OFF   1
#define LMP_CQM_DATA_RATE_SUPPORTED(x)  ((x)[LMP_FEATURE_CQM_DATA_RATE_OFF] & LMP_FEATURE_CQM_DATA_RATE_MASK)

#define LMP_FEATURE_SCO_LINK_MASK       0x08
#define LMP_FEATURE_SCO_LINK_OFF        1
#define LMP_SCO_LINK_SUPPORTED(x)       ((x)[LMP_FEATURE_SCO_LINK_OFF] & LMP_FEATURE_SCO_LINK_MASK)

#define LMP_FEATURE_HV2_PACKETS_MASK    0x10
#define LMP_FEATURE_HV2_PACKETS_OFF     1
#define LMP_HV2_PACKETS_SUPPORTED(x)    ((x)[LMP_FEATURE_HV2_PACKETS_OFF] & LMP_FEATURE_HV2_PACKETS_MASK)

#define LMP_FEATURE_HV3_PACKETS_MASK    0x20
#define LMP_FEATURE_HV3_PACKETS_OFF     1
#define LMP_HV3_PACKETS_SUPPORTED(x)    ((x)[LMP_FEATURE_HV3_PACKETS_OFF] & LMP_FEATURE_HV3_PACKETS_MASK)

#define LMP_FEATURE_U_LAW_MASK          0x40
#define LMP_FEATURE_U_LAW_OFF           1
#define LMP_U_LAW_SUPPORTED(x)          ((x)[LMP_FEATURE_U_LAW_OFF] & LMP_FEATURE_U_LAW_MASK)

#define LMP_FEATURE_A_LAW_MASK          0x80
#define LMP_FEATURE_A_LAW_OFF           1
#define LMP_A_LAW_SUPPORTED(x)          ((x)[LMP_FEATURE_A_LAW_OFF] & LMP_FEATURE_A_LAW_MASK)

#define LMP_FEATURE_CVSD_MASK           0x01
#define LMP_FEATURE_CVSD_OFF            2
#define LMP_CVSD_SUPPORTED(x)           ((x)[LMP_FEATURE_CVSD_OFF] & LMP_FEATURE_CVSD_MASK)

#define LMP_FEATURE_TRANSPNT_MASK       0x08
#define LMP_FEATURE_TRANSPNT_OFF        2
#define LMP_TRANSPNT_SUPPORTED(x)       ((x)[LMP_FEATURE_TRANSPNT_OFF] & LMP_FEATURE_TRANSPNT_MASK)

#define LMP_FEATURE_PAGING_SCHEME_MASK  0x02
#define LMP_FEATURE_PAGING_SCHEME_OFF   2
#define LMP_PAGING_SCHEME_SUPPORTED(x) ((x)[LMP_FEATURE_PAGING_SCHEME_OFF] & LMP_FEATURE_PAGING_SCHEME_MASK)

#define LMP_FEATURE_POWER_CTRL_MASK     0x04
#define LMP_FEATURE_POWER_CTRL_OFF      2
#define LMP_POWER_CTRL_SUPPORTED(x)     ((x)[LMP_FEATURE_POWER_CTRL_OFF] & LMP_FEATURE_POWER_CTRL_MASK)

#define LMP_FEATURE_FLOW_CTRL_LAG_MASK  0x70
#define LMP_FEATURE_FLOW_CTRL_LAG_OFF   2

/* Definitions for data rates */
#define LMP_DATA_RATE_DM                0
#define LMP_DATA_RATE_DH                1

/* Definitions for not_accepted reason */
#define LMP_REASON_NO_ERROR                             0
#define LMP_REASON_AUTH_FAILURE                         0x05
#define LMP_REASON_KEY_MISSING                          0x06
#define LMP_REASON_MAX_SCO                              0x0A
#define LMP_REASON_HOST_REJ_RESOURCES                   0x0B
#define LMP_REASON_HOST_REJ_SECURITY                    0x0C
#define LMP_REASON_HOST_REJ_PERSONAL                    0x0D
#define LMP_REASON_HOST_TIMEOUT                         0x10
#define LMP_REASON_REM_USER_ENDED                       0x13
#define LMP_REASON_REM_RESOURCES                        0x14
#define LMP_REASON_REM_POWER_OFF                        0x15
#define LMP_REASON_LOCAL_HOST_TERM                      0x16
#define LMP_REASON_REPEATED_ATTEMPTS                    0x17
#define LMP_REASON_PAIRING_NOT_ALLOWED                  0x18
#define LMP_REASON_UNKNOWN_PDU                          0x19
#define LMP_REASON_UNSUPPORTED_FEAT                     0x1A
#define LMP_REASON_SCO_OFFSET_REJ                       0x1B
#define LMP_REASON_SCO_INTERVAL_REJ                     0x1C
#define LMP_REASON_SCO_AIR_MODE_REJ                     0x1D
#define LMP_REASON_INVALID_PARAMETERS                   0x1E
#define LMP_REASON_UNSPECIFIED                          0x1F
#define LMP_REASON_UNSUPPORTED_PARM                     0x20
#define LMP_REASON_SWITCH_NOT_ALLOWED                   0x21
#define LMP_REASON_TRANS_COLLISION                      0x23
#define LMP_REASON_PDU_NOT_ALLOWED                      0x24
#define LMP_REASON_ENCRY_MODE_NOT_ACCEPTABLE            0x25
#define LMP_REASON_UNIT_KEY_USED                        0x26
#define LMP_REASON_QOS_NOT_SUPPORTED                    0x27
#define LMP_REASON_INSTANT_PASSED                       0x28
#define LMP_REASON_PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED  0x29

#define LMP_REASON_MAX_REASON                           0x29

#define LMP_MAX_RMT_NAME_FRAGMENT_SIZE  14

#define LMP_TEST_SCENARIO_EXIT          0xFF


#define LMP_DEFAULT_DRIFT               250
#define LMP_DEFAULT_JITTER              10

/*
** Define logicac struct used for sending and decoding LMP frames
*/
typedef struct
{
    BOOLEAN trans_slave;
    UINT8   opcode;
    union
    {
        UINT8  accepted_opcode;
        UINT8  rand_number[LMP_RAND_LEN];
        UINT16 clock_offset;
        UINT8  detach_reason;
        UINT8  *p_random;
        UINT32 sres;
        UINT8  key_size;
        UINT8  encryption_mode;
        UINT8  *p_features;
#ifndef ERRATA_1188
        UINT16 hold_time;
#endif
        UINT8  max_slots;

#ifdef ERRATA_1190
        UINT32 sw_instant;
#endif

#ifdef ERRATA_1188
        struct
        {
            UINT16 hold_time;
            UINT32 hold_instant;
        } hold;
#endif

        struct
        {
            UINT8  timing_control_flags;
            UINT16 db;
            UINT16 tb;
            UINT8  nb;
            UINT8  deltab;
            UINT8  daccess;
            UINT8  taccess;
            UINT8  nacc_slots;
            UINT8  npoll;
            UINT8  maccess;
            UINT8  acc_scheme;
        } beacon;
        UINT8  name_offset;
        struct
        {
            UINT8 offset;
            UINT8 length;
            UINT8 *p_fragment;
        } name;
        struct
        {
            UINT8 opcode;
            UINT8 reason;
        } not_accepted;
        struct
        {
            UINT8 scheme;
            UINT8 scheme_settings;
        } paging;
        struct
        {
            UINT8  timing_control_flags;
            UINT16 db;
            UINT16 tb;
            UINT8  nb;
            UINT8  deltab;
            UINT8  pm_addr;
            UINT8  ar_addr;
            UINT8  nb_sleep;
            UINT8  db_sleep;
            UINT8  daccess;
            UINT8  taccess;
            UINT8  nacc_slots;
            UINT8  npoll;
            UINT8  maccess;
            UINT8  acc_scheme;
        } beacon_power;
        UINT8 data_rate;
        struct
        {
            UINT16 poll_interval;
            UINT8  nbc;
        } qos;
        struct
        {
            UINT8  handle;
            UINT8  reason;
        } remove_sco_link_req;
        struct
        {
            UINT8  handle;
            UINT8  timing_control_flags;
            UINT8  d_sco;
            UINT8  t_sco;
            UINT8  sco_packet;
            UINT8  air_mode;
        } sco_link_req;
        struct
        {
            UINT8  timing_control_flags;
            UINT16 db;
            UINT16 broadcast_scan_window;
        } set_broadcast_scan_window;
        struct
        {
            UINT16 offset;
            UINT8  *p_bd_addr;
        } slot_offset;
        struct
        {
            UINT8  timing_control_flags;
            UINT16 d_sniff;
            UINT16 t_sniff;
            UINT16 attempt;
            UINT16 timeout;
        } sniff;
        UINT8  auth_res[LMP_AUTH_RES_LEN];
        UINT16 supervision_timeout;
        UINT8  *p_key;
        struct
        {
            UINT8 drift;
            UINT8 jitter;
        } timing_accuracy;
        struct
        {
            UINT8  timing_control_flags;
            UINT8  pm_addr;
            UINT16 db;
            UINT16 d_beacon;
            UINT16 t_beacon;
        } park;
        struct
        {
            UINT8  timing_control_flags;
            UINT16 db;
            UINT8  am_addr1;
            UINT8  am_addr2;
            UINT8  bd_addr1[BD_ADDR_LEN];
            UINT8  bd_addr2[BD_ADDR_LEN];
        } unpark_bd_addr;
        struct
        {
            UINT8  timing_control_flags;
            UINT16 db;
            UINT8  am_addr1;
            UINT8  am_addr2;
            UINT8  pm_addr1;
            UINT8  pm_addr2;
        } unpark_pm_addr;
        struct
        {
            UINT8  version_nr;
            UINT16 comp_id;
            UINT16 sub_version_nr;
        } version;
        struct
        {
            UINT8  scenario;
            UINT8  hopping_mode;
            UINT8  tx_freq;
            UINT8  rx_freq;
            UINT8  power_ctrl_mode;
            UINT8  poll_period;
            UINT8  packet_type;
            UINT16 length;
        } test_ctrl;
    } params;
} LMP_FRAME;

#ifdef __cplusplus
} /*extern "C" */
#endif
#endif
