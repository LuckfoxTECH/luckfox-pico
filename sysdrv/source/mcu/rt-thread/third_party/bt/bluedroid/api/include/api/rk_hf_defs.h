// Copyright 2018 Fuzhou Rockchip Electronics Co., Ltd
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef __RK_HF_DEFS_H__
#define __RK_HF_DEFS_H__

#include "rk_bt_defs.h"

#ifdef __cplusplus
extern "C" {
#endif


/// Bluetooth HFP audio volume control target
typedef enum
{
    RK_HF_VOLUME_CONTROL_TARGET_SPK = 0,             /*!< speaker */
    RK_HF_VOLUME_CONTROL_TARGET_MIC,                 /*!< microphone */
} rk_hf_volume_control_target_t;

/// +CIND roaming status indicator values
typedef enum
{
    RK_HF_ROAMING_STATUS_INACTIVE = 0,               /*!< roaming is not active */
    RK_HF_ROAMING_STATUS_ACTIVE,                     /*!< a roaming is active */
} rk_hf_roaming_status_t;

/// +CIND call status indicator values
typedef enum
{
    RK_HF_CALL_STATUS_NO_CALLS = 0,                  /*!< no call in progress  */
    RK_HF_CALL_STATUS_CALL_IN_PROGRESS = 1,          /*!< call is present(active or held) */
} rk_hf_call_status_t;

/// +CIND call setup status indicator values
typedef enum
{
    RK_HF_CALL_SETUP_STATUS_NONE = 0,                /*!< no call setup in progress */
    RK_HF_CALL_SETUP_STATUS_INCOMING = 1,            /*!< incoming call setup in progress */
    RK_HF_CALL_SETUP_STATUS_OUTGOING_DIALING = 2,    /*!< outgoing call setup in dialing state */
    RK_HF_CALL_SETUP_STATUS_OUTGOING_ALERTING = 3,   /*!< outgoing call setup in alerting state */
} rk_hf_call_setup_status_t;

/// +CIND call held indicator values
typedef enum
{
    RK_HF_CALL_HELD_STATUS_NONE = 0,                 /*!< no calls held */
    RK_HF_CALL_HELD_STATUS_HELD_AND_ACTIVE = 1,      /*!< both active and held call */
    RK_HF_CALL_HELD_STATUS_HELD = 2,                 /*!< call on hold, no active call*/
} rk_hf_call_held_status_t;

/// +CIND network service availability status
typedef enum
{
    RK_HF_SERVICE_AVAILABILITY_STATUS_UNAVAILABLE = 0, /*!< service not available */
    RK_HF_SERVICE_AVAILABILITY_STATUS_AVAILABLE,       /*!< service available */
} rk_hf_service_availability_status_t;

/// +CLCC status of the call
typedef enum
{
    RK_HF_CURRENT_CALL_STATUS_ACTIVE = 0,            /*!< active */
    RK_HF_CURRENT_CALL_STATUS_HELD = 1,              /*!< held */
    RK_HF_CURRENT_CALL_STATUS_DIALING = 2,           /*!< dialing (outgoing calls only) */
    RK_HF_CURRENT_CALL_STATUS_ALERTING = 3,          /*!< alerting (outgoing calls only) */
    RK_HF_CURRENT_CALL_STATUS_INCOMING = 4,          /*!< incoming (incoming calls only) */
    RK_HF_CURRENT_CALL_STATUS_WAITING = 5,           /*!< waiting (incoming calls only) */
    RK_HF_CURRENT_CALL_STATUS_HELD_BY_RRK_HOLD = 6, /*!< call held by response and hold */
} rk_hf_current_call_status_t;

/// +CLCC direction of the call
typedef enum
{
    RK_HF_CURRENT_CALL_DIRECTION_OUTGOING = 0,       /*!< outgoing */
    RK_HF_CURRENT_CALL_DIRECTION_INCOMING = 1,       /*!< incoming */
} rk_hf_current_call_direction_t;

/// +CLCC multi-party call flag
typedef enum
{
    RK_HF_CURRENT_CALL_MPTY_TYPE_SINGLE = 0,         /*!< not a member of a multi-party call */
    RK_HF_CURRENT_CALL_MPTY_TYPE_MULTI = 1,          /*!< member of a multi-party call */
} rk_hf_current_call_mpty_type_t;

/// +CLCC call mode
typedef enum
{
    RK_HF_CURRENT_CALL_MODE_VOICE = 0,
    RK_HF_CURRENT_CALL_MODE_DATA = 1,
    RK_HF_CURRENT_CALL_MODE_FAX = 2,
} rk_hf_current_call_mode_t;

/// +CLCC address type
typedef enum
{
    RK_HF_CALL_ADDR_TYPE_UNKNOWN = 0x81,            /*!< unkown address type */
    RK_HF_CALL_ADDR_TYPE_INTERNATIONAL = 0x91,      /*!< international address */
} rk_hf_call_addr_type_t;

/// +CNUM service type of the phone number
typedef enum
{
    RK_HF_SUBSCRIBER_SERVICE_TYPE_UNKNOWN = 0,      /*!< unknown */
    RK_HF_SUBSCRIBER_SERVICE_TYPE_VOICE,            /*!< voice service */
    RK_HF_SUBSCRIBER_SERVICE_TYPE_FAX,              /*!< fax service */
} rk_hf_subscriber_service_type_t;

/// +BTRH response and hold result code
typedef enum
{
    RK_HF_BTRH_STATUS_HELD = 0,       /*!< incoming call is put on held in AG */
    RK_HF_BTRH_STATUS_ACCEPTED,       /*!< held incoming call is accepted in AG */
    RK_HF_BTRH_STATUS_REJECTED,       /*!< held incoming call is rejected in AG */
} rk_hf_btrh_status_t;

/// AT+BTRH response and hold action code
typedef enum
{
    RK_HF_BTRH_CMD_HOLD = 0,          /*!< put the incoming call on hold */
    RK_HF_BTRH_CMD_ACCEPT = 1,        /*!< accept a held incoming call */
    RK_HF_BTRH_CMD_REJECT = 2,        /*!< reject a held incoming call */
} rk_hf_btrh_cmd_t;

/// response indication codes for AT commands
typedef enum
{
    RK_HF_AT_RESPONSE_CODE_OK = 0,         /*!< acknowledges execution of a command line */
    RK_HF_AT_RESPONSE_CODE_ERR,            /*!< command not accepted */
    RK_HF_AT_RESPONSE_CODE_NO_CARRIER,     /*!< connection terminated */
    RK_HF_AT_RESPONSE_CODE_BUSY,           /*!< busy signal detected */
    RK_HF_AT_RESPONSE_CODE_NO_ANSWER,      /*!< connection completion timeout */
    RK_HF_AT_RESPONSE_CODE_DELAYED,        /*!< delayed */
    RK_HF_AT_RESPONSE_CODE_BLACKLISTED,    /*!< blacklisted */
    RK_HF_AT_RESPONSE_CODE_CME,            /*!< CME error */
} rk_hf_at_response_code_t;

/// voice recognition state
typedef enum
{
    RK_HF_VR_STATE_DISABLED = 0,           /*!< voice recognition disabled */
    RK_HF_VR_STATE_ENABLED,                /*!< voice recognition enabled */
} rk_hf_vr_state_t;

/// AT+CHLD command values
typedef enum
{
    RK_HF_CHLD_TYPE_REL = 0,               /*!< <0>, Terminate all held or set UDUB("busy") to a waiting call */
    RK_HF_CHLD_TYPE_REL_ACC,               /*!< <1>, Terminate all active calls and accepts a waiting/held call */
    RK_HF_CHLD_TYPE_HOLD_ACC,              /*!< <2>, Hold all active calls and accepts a waiting/held call */
    RK_HF_CHLD_TYPE_MERGE,                 /*!< <3>, Add all held calls to a conference */
    RK_HF_CHLD_TYPE_MERGE_DETACH,          /*!< <4>, connect the two calls and disconnects the subscriber from both calls */
    RK_HF_CHLD_TYPE_REL_X,                 /*!< <1x>, releases specified calls only */
    RK_HF_CHLD_TYPE_PRIV_X,                /*!< <2x>, request private consultation mode with specified call */
} rk_hf_chld_type_t;

/// Extended Audio Gateway Error Result Code Response
typedef enum
{
    RK_HF_CME_AG_FAILURE = 0,                    /*!< ag failure */
    RK_HF_CME_NO_CONNECTION_TO_PHONE = 1,        /*!< no connection to phone */
    RK_HF_CME_OPERATION_NOT_ALLOWED = 3,         /*!< operation not allowed */
    RK_HF_CME_OPERATION_NOT_SUPPORTED = 4,       /*!< operation not supported */
    RK_HF_CME_PH_SIM_PIN_REQUIRED = 5,           /*!< PH-SIM PIN Required */
    RK_HF_CME_SIM_NOT_INSERTED = 10,             /*!< SIM not inserted */
    RK_HF_CME_SIM_PIN_REQUIRED = 11,             /*!< SIM PIN required */
    RK_HF_CME_SIM_PUK_REQUIRED = 12,             /*!< SIM PUK required */
    RK_HF_CME_SIM_FAILURE = 13,                  /*!< SIM failure */
    RK_HF_CME_SIM_BUSY = 14,                     /*!< SIM busy */
    RK_HF_CME_INCORRECT_PASSWORD = 16,           /*!< incorrect password */
    RK_HF_CME_SIM_PIN2_REQUIRED = 17,            /*!< SIM PIN2 required */
    RK_HF_CME_SIM_PUK2_REQUIRED = 18,            /*!< SIM PUK2 required */
    RK_HF_CME_MEMEORY_FULL = 20,                 /*!< memory full */
    RK_HF_CME_INVALID_INDEX = 21,                /*!< invalid index */
    RK_HF_CME_MEMEORY_FAILURE = 23,              /*!< memory failure */
    RK_HF_CME_TEXT_STRING_TOO_LONG = 24,         /*!< test string too long */
    RK_HF_CME_INVALID_CHARACTERS_IN_TEXT_STRING = 25,  /*!< invalid characters in text string */
    RK_HF_CME_DIAL_STRING_TOO_LONG = 26,         /*!< dial string too long*/
    RK_HF_CME_INVALID_CHARACTERS_IN_DIAL_STRING = 27,  /*!< invalid characters in dial string */
    RK_HF_CME_NO_NETWORK_SERVICE = 30,           /*!< no network service */
    RK_HF_CME_NETWORK_TIMEOUT = 31,              /*!< network timeout */
    RK_HF_CME_NETWORK_NOT_ALLOWED = 32,          /*!< network not allowed --emergency calls only */
} rk_hf_cme_err_t;

#ifdef __cplusplus
}
#endif

#endif /* __RK_HF_DEFS_H__ */
