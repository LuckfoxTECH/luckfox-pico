/*
 * $ Copyright Cypress Semiconductor $
 */

/** @bluetooth_hfp.c
 *
 * This contains the relevant application code for the Bluetooth HFP profile.
 *
 */

#include "wiced.h"
#include "dual_a2dp_hfp_audio.h"
#include "hashtable.h"
#include "wiced_bt_types.h"
#include "wiced_bt_hfp_hf.h"
#include "bluetooth_dm.h"
#include "bluetooth_audio.h"
#include "wiced_bt_sco.h"
#include "wiced_tone.h"
#include "mem_pool.h"
#include "bluetooth_hfp.h"
#include "bluetooth_nv.h"
#include "wiced_bt_cfg.h"
#include "gki.h"
#include "wwd_debug.h"
#include "btm_api.h"
#include "wiced_bt_hfp_hf_int.h"

/******************************************************
 *                      Macros
 ******************************************************/
/* Parameter information for HCI_BRCM_WRITE_I2SPCM_INTF_PARAM */
#define HCI_BRCM_I2SPCM_PARAM_SIZE          4
#define HCI_BRCM_I2SPCM_I2S_DISABLE         0
#define HCI_BRCM_I2SPCM_I2S_ENABLE          1
#define HCI_BRCM_I2SPCM_IS_SLAVE            0
#define HCI_BRCM_I2SPCM_IS_MASTER           1
#define HCI_BRCM_I2SPCM_IS_DEFAULT_ROLE     2
#define HCI_BRCM_I2SPCM_SAMPLE_8K           0
#define HCI_BRCM_I2SPCM_SAMPLE_16K          1
#define HCI_BRCM_I2SPCM_SAMPLE_4K           2
#define HCI_BRCM_I2SPCM_SAMPLE_DEFAULT      3
#define HCI_BRCM_I2SPCM_CLOCK_128K          0
#define HCI_BRCM_I2SPCM_CLOCK_256K          1
#define HCI_BRCM_I2SPCM_CLOCK_512K          2
#define HCI_BRCM_I2SPCM_CLOCK_1024K         3
#define HCI_BRCM_I2SPCM_CLOCK_2048K         4
#define HCI_BRCM_I2SPCM_CLOCK_DEFAULT       5

#define BT_AUDIO_HFP_SUPPORTED_FEATURES  ( WICED_BT_HFP_HF_FEATURE_3WAY_CALLING | \
                                                                         WICED_BT_HFP_HF_FEATURE_CLIP_CAPABILITY | \
                                                                         WICED_BT_HFP_HF_FEATURE_REMOTE_VOLUME_CONTROL| \
                                                                         WICED_BT_HFP_HF_FEATURE_CODEC_NEGOTIATION | \
                                                                         WICED_BT_HFP_HF_FEATURE_HF_INDICATORS)

/* Scales the volume from the range (0-15) to (0-127). */
#define SCALE_TO_PLAYER_VOLUME( v ) ( (v==0)?0:(((60/7)*(v-1)) +7) )

/******************************************************
 *                    Constants
 ******************************************************/

#define BT_AUDIO_HFP_VOLUME_MIN     1
#define BT_AUDIO_HFP_VOLUME_MAX    15
#define BT_AUDIO_INVALID_SCO_INDEX 0xFFFF

#define BT_HFP_CONTEXT_MAX_LIMIT     2
/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

typedef struct
{
    wiced_bt_device_address_t               peer_bd_addr;
    wiced_bt_hfp_hf_connection_state_t      connection_status;
    uint16_t                                sco_index;
    int                                     call_active;
    int                                     call_held;
    wiced_bt_hfp_hf_callsetup_state_t       call_setup;
    wiced_bt_hfp_hf_inband_ring_state_t     inband_ring_status;
    uint8_t                                 mic_volume;
    uint8_t                                 spkr_volume;
    wiced_bool_t                            in_use_context;
} bluetooth_hfp_context_t;

/******************************************************
 *               Static Function Declarations
 ******************************************************/

static void bt_audio_hfp_init_service(void);
void bt_audio_hfp_connect_service(void);
void bt_audio_hfp_disconnect_service(void);
static void bt_audio_hfp_event_cb(wiced_bt_hfp_hf_event_t event, wiced_bt_hfp_hf_event_data_t *p_data);

static wiced_result_t bt_audio_hfp_volume_up(void);
static wiced_result_t bt_audio_hfp_volume_down(void);
static wiced_result_t bt_audio_hfp_multi_func_short_release(void);
static wiced_result_t bt_audio_hfp_multi_func_long_release(void);

static wiced_result_t bt_audio_hfp_button_event_handler(app_service_action_t action);
static wiced_result_t bt_audio_hfp_update_volume(uint8_t vol_level,  wiced_bt_hfp_hf_volume_type_t volume_type);
static int get_hfp_context(wiced_bt_device_address_t remote_addr);
static int set_hfp_context(wiced_bt_device_address_t remote_addr);

/******************************************************
 *               Static Function Declarations
 ******************************************************/
extern wiced_result_t bt_audio_configure_device(bt_audio_config_t *p_audio_config, bt_audio_device_type_t device_type, service_type_t service_type);

/******************************************************
 *               Variable Definitions
 ******************************************************/
extern bt_buffer_pool_handle_t  mem_pool_pcm;
bluetooth_hfp_context_t hfp_ctxt_data[BT_HFP_CONTEXT_MAX_LIMIT];
int hfp_context_index = 0;

static wiced_bool_t use_wbs;

/******************************************************
 *               Function Definitions
 ******************************************************/

/**
 * void wiced_add_hfp_service(void)
 *
 * initialize the hfp service
 */
void bt_audio_add_hfp_service(void)
{
    wiced_app_service_t cell;
    wiced_result_t result = WICED_ERROR;
    int i;

    cell.priority                   = SERVICE_BT_HFP_PRIORITY;
    cell.type                       = SERVICE_BT_HFP;
    cell.state                      = SERVICE_DISABLED;


    cell.init_service               = bt_audio_hfp_init_service;
    cell.connect_service            = bt_audio_hfp_connect_service;
    cell.disconnect_service         = bt_audio_hfp_disconnect_service;
    cell.deinit_service             = NULL;
    cell.allow_service              = NULL;
    cell.prevent_service            = NULL;

    for (i = 0; i < BT_HFP_CONTEXT_MAX_LIMIT; i++)
    {
        hfp_ctxt_data[i].call_active       = 0;
        hfp_ctxt_data[i].call_setup        = WICED_BT_HFP_HF_CALLSETUP_STATE_IDLE;
        hfp_ctxt_data[i].mic_volume        = 8;
        hfp_ctxt_data[i].spkr_volume       = 8;
        hfp_ctxt_data[i].sco_index         = BT_AUDIO_INVALID_SCO_INDEX;
        hfp_ctxt_data[i].connection_status = WICED_BT_HFP_HF_STATE_DISCONNECTED;
        hfp_ctxt_data[i].in_use_context    = WICED_FALSE;
    }
    cell.button_handler             = bt_audio_hfp_button_event_handler;

    result = wiced_add_entry(&cell);

    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_ERROR(("[BT-HFP] Failed to add Service entry{Service:%d Error:%d}\n", cell.type, result));
        /* TODO. May be we wish to send the result to the app_worker_thread as well */
        return;
    }

    WPRINT_APP_INFO(("[BT-HFP] Service Added\n"));
}

static wiced_result_t hf_setup_call()
{
    app_set_current_service(SERVICE_BT_HFP);
    app_set_service_state(SERVICE_BT_HFP, SERVICE_PLAYING_AUDIO);
    return WICED_SUCCESS;
}

static wiced_result_t hf_teardown_call()
{
    app_reset_current_service(SERVICE_BT_HFP);
    return WICED_SUCCESS;
}

static void bt_audio_hfp_callsetup_evt_handler(wiced_bt_hfp_hf_call_data_t *call_data)
{
    WPRINT_APP_INFO(("%s: call_data->setup_state = %d\n", __func__, call_data->setup_state));
    switch (call_data->setup_state)
    {
    case WICED_BT_HFP_HF_CALLSETUP_STATE_INCOMING:
        WPRINT_APP_INFO(("%s: Call(incoming) setting-up\n", __func__));
        hf_setup_call();
        break;

    case WICED_BT_HFP_HF_CALLSETUP_STATE_IDLE:
        if (call_data->active_call_present == 0)
        {
            if (hfp_ctxt_data[hfp_context_index].call_setup == WICED_BT_HFP_HF_CALLSETUP_STATE_INCOMING
                    || hfp_ctxt_data[hfp_context_index].call_setup == WICED_BT_HFP_HF_CALLSETUP_STATE_DIALING
                    || hfp_ctxt_data[hfp_context_index].call_setup == WICED_BT_HFP_HF_CALLSETUP_STATE_ALERTING)
            {
                WPRINT_APP_INFO(("[BT-HFP] Call: Inactive; Call-setup: DONE\n"));
                hf_teardown_call();
                break;
            }
            if (hfp_ctxt_data[hfp_context_index].call_setup == WICED_BT_HFP_HF_CALLSETUP_STATE_INCOMING)
            {
#if 0
                if (hfp_ctxt_data[hfp_context_index].inband_ring_status == WICED_BT_HFP_HF_INBAND_RING_ENABLED)
                    wiced_tone_stop(); //stop tone if being played out, if not, does nothing.
#endif
            }

            if (hfp_ctxt_data[hfp_context_index].call_active == 1)
            {
                WPRINT_APP_INFO(("[BT-HFP] Call Terminated\n"));
                hf_teardown_call();
                break;
            }
        }
        else if (call_data->active_call_present == 1)
        {
            WPRINT_APP_INFO(("[BT-HFP] Call: Active; Call-setup: DONE\n"));
        }
        break;

    case WICED_BT_HFP_HF_CALLSETUP_STATE_DIALING:
        WPRINT_APP_INFO(("[BT-HFP] Call(outgoing) setting-up\n"));
        hf_setup_call();
        break;

    case WICED_BT_HFP_HF_CALLSETUP_STATE_ALERTING:
        WPRINT_APP_INFO(("[BT-HFP] Remote(outgoing) ringing\n"));
        break;

    default:
        break;
    }

    hfp_ctxt_data[hfp_context_index].call_active = call_data->active_call_present;
    hfp_ctxt_data[hfp_context_index].call_setup = call_data->setup_state;
    hfp_ctxt_data[hfp_context_index].call_held = call_data->held_call_present;
}

static void bt_audio_hfp_event_cb(wiced_bt_hfp_hf_event_t event, wiced_bt_hfp_hf_event_data_t *p_data)
{
    wiced_app_service_t  *service = NULL;
    wiced_bt_dev_status_t status;

    WPRINT_APP_DEBUG(("%s:event %d\n", __func__, event));
    switch (event)
    {
    case WICED_BT_HFP_HF_CONNECTION_STATE_EVT:
        WPRINT_APP_INFO(("%s:CONNECTION STATE [%d] \n", __func__, p_data->conn_state));
        if (p_data->conn_state == WICED_BT_HFP_HF_STATE_CONNECTED)
        {
            hfp_context_index = set_hfp_context(p_data->remote_address);
            if (hfp_context_index < 0 && hfp_context_index >= BT_HFP_CONTEXT_MAX_LIMIT)
            {
                return;
            }
            memcpy(&hfp_ctxt_data[hfp_context_index].peer_bd_addr, p_data->remote_address, sizeof(wiced_bt_device_address_t));
            status = wiced_bt_sco_create_as_acceptor(&hfp_ctxt_data[hfp_context_index].sco_index);
            WPRINT_APP_INFO(("%s: status [%d] SCO INDEX [%d] \n", __func__, status, hfp_ctxt_data[hfp_context_index].sco_index));
        }
        else if (p_data->conn_state == WICED_BT_HFP_HF_STATE_SLC_CONNECTED)
        {
            hfp_context_index = get_hfp_context(p_data->remote_address);
            if (hfp_context_index < 0 && hfp_context_index >= BT_HFP_CONTEXT_MAX_LIMIT)
            {
                return;
            }
            service = wiced_get_entry(SERVICE_BT_HFP);
            service->state = SERVICE_ENABLED;
#if 0
            signal_for_event_semaphore(WICED_BT_CONNECT_HFP, WICED_TRUE);
#endif
        }
        else if (p_data->conn_state == WICED_BT_HFP_HF_STATE_DISCONNECTED)
        {
            hfp_context_index = get_hfp_context(p_data->remote_address);
            if (hfp_context_index < 0 && hfp_context_index >= BT_HFP_CONTEXT_MAX_LIMIT)
            {
                return;
            }
            memset(&hfp_ctxt_data[hfp_context_index].peer_bd_addr, 0, sizeof(wiced_bt_device_address_t));
            hfp_ctxt_data[hfp_context_index].in_use_context = WICED_FALSE;
            if (hfp_ctxt_data[hfp_context_index].sco_index != BT_AUDIO_INVALID_SCO_INDEX)
            {
                status = wiced_bt_sco_remove(hfp_ctxt_data[hfp_context_index].sco_index);
                hfp_ctxt_data[hfp_context_index].sco_index = BT_AUDIO_INVALID_SCO_INDEX;
                WPRINT_APP_INFO(("%s: remove sco status [%d] \n", __func__, status));
            }
            app_disable_service(SERVICE_BT_HFP);
#if 0
            signal_for_event_semaphore(WICED_BT_DISCONNECT_HFP, WICED_TRUE);
#endif
        }
        hfp_ctxt_data[hfp_context_index].connection_status = p_data->conn_state;
        break;

    case WICED_BT_HFP_HF_AG_FEATURE_SUPPORT_EVT:
        if (p_data->ag_feature_flags & WICED_BT_HFP_AG_FEATURE_INBAND_RING_TONE_CAPABILITY)
        {
            hfp_ctxt_data[hfp_context_index].inband_ring_status = WICED_BT_HFP_HF_INBAND_RING_ENABLED;
        }
        else
        {
            hfp_ctxt_data[hfp_context_index].inband_ring_status = WICED_BT_HFP_HF_INBAND_RING_DISABLED;
        }

        wiced_bt_hfp_hf_scb_t *p_scb = wiced_bt_hfp_hf_get_scb_by_handle(p_data->handle);
        if ((p_data->ag_feature_flags & WICED_BT_HFP_AG_FEATURE_CODEC_NEGOTIATION) &&
                (p_scb->feature_mask & WICED_BT_HFP_HF_FEATURE_CODEC_NEGOTIATION))
        {
            use_wbs = WICED_TRUE;
        }
        else
        {
            use_wbs = WICED_FALSE;
        }
        WPRINT_APP_INFO(("WICED_BT_HFP_HF_AG_FEATURE_SUPPORT_EVT  use_wbs = [%d] \n", use_wbs));
        break;

    case WICED_BT_HFP_HF_CALL_SETUP_EVT:
        hfp_context_index = get_hfp_context(p_data->remote_address);
        if (hfp_context_index < 0 && hfp_context_index >= BT_HFP_CONTEXT_MAX_LIMIT)
        {
            return;
        }
        bt_audio_hfp_callsetup_evt_handler(&p_data->call_data);
        break;

    case WICED_BT_HFP_HF_RING_EVT:
    {
        wiced_result_t ret;
        wiced_app_service_t  *service = get_app_current_service();

        WPRINT_APP_INFO(("%s: RING hfp_context_index:%x\n", __func__, hfp_context_index));
        if ((hfp_ctxt_data[hfp_context_index].inband_ring_status == WICED_BT_HFP_HF_INBAND_RING_DISABLED) &&
                (service != NULL && service->type == SERVICE_BT_HFP)
                && app_data.a2dp_media_state == WICED_FALSE) //TODO: this check is temporary fix, need to fix the underlying wiced_audio API to return error (now it crashes).
        {
#if 0
            ret = wiced_tone_play(NULL, 0, NULL, 0);
            if (ret != WICED_SUCCESS)
                WPRINT_APP_INFO(("%s: wiced_tone_play returned error = %d\n", __func__, ret));
#endif
        }
    }
    break;

    case WICED_BT_HFP_HF_INBAND_RING_STATE_EVT:
        hfp_ctxt_data[hfp_context_index].inband_ring_status = p_data->inband_ring;
        break;

    case WICED_BT_HFP_HF_CLIP_IND_EVT:
        WPRINT_APP_INFO(("%s: CLIP - number %s, type %d\n", __func__, p_data->clip.caller_num, p_data->clip.type));
        break;

    case WICED_BT_HFP_HF_VOLUME_CHANGE_EVT:
    {
        wiced_result_t result;
        WPRINT_APP_INFO(("%s: %s VOLUME - %d \n", __func__, (p_data->volume.type == 0) ? "SPK" : "MIC",  p_data->volume.level));
        result = bt_audio_hfp_update_volume(p_data->volume.level, p_data->volume.type);
        if (result == WICED_SUCCESS)
        {
            hfp_ctxt_data[hfp_context_index].spkr_volume = p_data->volume.level;
        }
    }
    break;

    default:
        //nothing to be done.
        break;
    }
}

static void bt_audio_sco_data_cback(uint16_t sco_index, BT_HDR *sco_data, uint8_t status)
{
    bt_audio_codec_data_t *pcm;
    uint16_t *in_buf;
    uint16_t *out_buf;
    int i, j, samples_count;

    if (status != 0)
    {
        //WPRINT_APP_INFO( ("[%s] status: %d\n", __func__, status) );
        //printf("[%s] status: %d\n", __func__, status);
        return;
    }

#ifdef USE_MEM_POOL
    pcm = bt_buffer_pool_allocate_buffer(mem_pool_pcm);
#else
    pcm = rt_malloc(sizeof(bt_audio_codec_data_t) + sco_data->len * 2);
#endif
    if (pcm == NULL)
    {
        return;
    }

    in_buf = (uint16_t *)((UINT8 *)(sco_data + 1) + sco_data->offset);

    pcm->offset = 0;
    pcm->length = sco_data->len * 2;
    out_buf = (uint16_t *)pcm->data;

    i = 0;
    j = 0;
    samples_count = sco_data->len / 2;
    do
    {
        out_buf[j++] = in_buf[i];
        out_buf[j++] = in_buf[i];
        i++;
    }
    while (i < samples_count);

    bt_audio_write_to_player_buffer(pcm);
}

void bt_audio_hfp_send_sco_data(bt_audio_codec_data_t *pcm)
{
    uint8_t *data = pcm->data + pcm->offset;
    if (hfp_ctxt_data[hfp_context_index].sco_index < 0 || hfp_ctxt_data[hfp_context_index].sco_index >= BT_AUDIO_INVALID_SCO_INDEX)
        hfp_ctxt_data[hfp_context_index].sco_index = 0;
    wiced_bt_sco_write_data(hfp_ctxt_data[hfp_context_index].sco_index, data, pcm->length);
}

static void bt_audio_hfp_init_service(void)
{
    wiced_result_t result;
    wiced_bt_hfp_hf_config_data_t p_config_data;
    extern const wiced_bt_cfg_buf_pool_t wiced_bt_audio_cfg_buf_pools[WICED_BT_CFG_NUM_BUF_POOLS];
    wiced_bt_cfg_buf_pool_t *sco_buf_pool;

    p_config_data.mic_volume        = hfp_ctxt_data[0].mic_volume;
    p_config_data.speaker_volume    = hfp_ctxt_data[0].spkr_volume;
    p_config_data.feature_mask      = BT_AUDIO_HFP_SUPPORTED_FEATURES;
#if 0
    p_config_data.num_server        = 2;
#else
    p_config_data.num_server        = 1;
#endif
    p_config_data.scn[0]            = HANDS_FREE_SCN;
    p_config_data.scn[1]            = HANDS_FREE_SCN;

    result = wiced_bt_hfp_hf_init(&p_config_data, bt_audio_hfp_event_cb);
    WPRINT_APP_INFO(("[%s] HF Profile init result = %d\n", __func__, result));

    result = wiced_bt_sco_set_data_callback(bt_audio_sco_data_cback);
    WPRINT_APP_INFO(("[%s] SCO data cb register result = %d\n", __func__, result));

    if (HCI_SCO_POOL_ID <=  WICED_BT_CFG_NUM_BUF_POOLS)
    {
        sco_buf_pool = (wiced_bt_cfg_buf_pool_t *)&wiced_bt_audio_cfg_buf_pools[ HCI_SCO_POOL_ID];
        result = wiced_bt_sco_set_buffer_pool(sco_buf_pool->buf_size, sco_buf_pool->buf_count);
        WPRINT_APP_INFO(("[%s] SCO setting buffer-pool result = %d\n", __func__, result));
    }
}

void bt_audio_hfp_connect_service(void)
{
    wiced_result_t result;
    bt_audio_paired_device_info_t out_device;

    if (bt_audio_nv_get_device_info_by_index(0, &out_device) != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("[HFP] Can't connect HFP service - no previously connected device exist\n"));
#if 0
        signal_for_event_semaphore(WICED_BT_CONNECT_HFP, WICED_FALSE);
#endif
        return;
    }
    result = wiced_bt_hfp_hf_connect(out_device.device_link.bd_addr);
    if (WICED_SUCCESS != result)
    {
        WPRINT_APP_INFO(("[HFP] Trying to connect - failed !\n"));
#if 0
        signal_for_event_semaphore(WICED_BT_CONNECT_HFP, WICED_FALSE);
#endif
        return;
    }
    return;
}

void bt_audio_hfp_disconnect_service(void)
{
    wiced_result_t result;

    if (hfp_ctxt_data[hfp_context_index].connection_status == WICED_BT_HFP_HF_STATE_DISCONNECTED)
    {
        WPRINT_APP_INFO(("[HFP] bt_audio_hfp_disconnect_service - not connected\n"));
        return;
    }

    result = wiced_bt_hfp_hf_disconnect(hfp_ctxt_data[hfp_context_index].peer_bd_addr);

    if (WICED_SUCCESS != result)
    {
        WPRINT_APP_INFO(("[HFP] HFP trying to disconnect - failed !\n"));
#if 0
        signal_for_event_semaphore(WICED_BT_DISCONNECT_HFP, WICED_FALSE);
#endif
    }

    return;
}

static wiced_result_t bt_audio_hfp_update_volume(uint8_t vol_level,  wiced_bt_hfp_hf_volume_type_t volume_type)
{
    wiced_result_t result = WICED_ERROR;

    if (vol_level > BT_AUDIO_HFP_VOLUME_MAX)
        return result;

    if (volume_type == WICED_BT_HFP_HF_SPEAKER)
    {
        uint8_t player_volume = SCALE_TO_PLAYER_VOLUME(vol_level);
        result = bt_audio_update_player_volume(player_volume);
    }
    //TODO support mic vol support update
    return result;
}

static wiced_result_t bt_audio_hfp_volume_up(void)
{
    wiced_result_t result;

    if (hfp_ctxt_data[hfp_context_index].spkr_volume == BT_AUDIO_HFP_VOLUME_MAX)
    {
        WPRINT_APP_INFO(("%s: Volume level already Maximum\n", __func__));
        return WICED_SUCCESS;
    }

    hfp_ctxt_data[hfp_context_index].spkr_volume++;

    result = bt_audio_hfp_update_volume(hfp_ctxt_data[hfp_context_index].spkr_volume, WICED_BT_HFP_HF_SPEAKER);
    if (result == WICED_SUCCESS)
    {
        wiced_bt_hfp_hf_notify_volume(hfp_ctxt_data[hfp_context_index].peer_bd_addr, WICED_BT_HFP_HF_SPEAKER, hfp_ctxt_data[hfp_context_index].spkr_volume);
    }

    return result;
}

static wiced_result_t bt_audio_hfp_volume_down(void)
{
    wiced_result_t result;

    if (hfp_ctxt_data[hfp_context_index].spkr_volume == BT_AUDIO_HFP_VOLUME_MIN)
    {
        WPRINT_APP_DEBUG(("%s: Volume level already Minimum\n", __func__));
        return WICED_SUCCESS;
    }

    hfp_ctxt_data[hfp_context_index].spkr_volume--;

    result = bt_audio_hfp_update_volume(hfp_ctxt_data[hfp_context_index].spkr_volume, WICED_BT_HFP_HF_SPEAKER);
    if (result == WICED_SUCCESS)
    {
        wiced_bt_hfp_hf_notify_volume(hfp_ctxt_data[hfp_context_index].peer_bd_addr, WICED_BT_HFP_HF_SPEAKER, hfp_ctxt_data[hfp_context_index].spkr_volume);
    }

    return result;
}

wiced_result_t bt_audio_hfp_dial_last_number(void)
{
    WPRINT_APP_INFO(("%s: Redial...\n", __func__));
    return wiced_bt_hfp_hf_perform_call_action(hfp_ctxt_data[hfp_context_index].peer_bd_addr, WICED_BT_HFP_HF_CALL_ACTION_DIAL, NULL);
}

static wiced_result_t bt_audio_hfp_multi_func_long_release(void)
{
    wiced_result_t ret = WICED_ERROR;
    int action = -1;

    switch (hfp_ctxt_data[hfp_context_index].call_setup)
    {
    //end an incoming call
    case WICED_BT_HFP_HF_CALLSETUP_STATE_INCOMING:
        action = WICED_BT_HFP_HF_CALL_ACTION_HANGUP;
        break;

    //reject a waiting call (UDUB)
    case WICED_BT_HFP_HF_CALLSETUP_STATE_WAITING:
        action = WICED_BT_HFP_HF_CALL_ACTION_HOLD_0;
        break;

    //no action defined
    case WICED_BT_HFP_HF_CALLSETUP_STATE_DIALING:
    case WICED_BT_HFP_HF_CALLSETUP_STATE_ALERTING: //fall through
        //nothing to do here
        break;

    default: //WICED_BT_HFP_HF_CALLSETUP_STATE_IDLE
        if (hfp_ctxt_data[hfp_context_index].call_active == 1 && hfp_ctxt_data[hfp_context_index].call_held == 1)
        {
            //place active call on hold and activate a held call
            action = WICED_BT_HFP_HF_CALL_ACTION_HOLD_2;
        }
        break;
    }
    if (action != -1)
        ret = wiced_bt_hfp_hf_perform_call_action(hfp_ctxt_data[hfp_context_index].peer_bd_addr, (wiced_bt_hfp_hf_call_action_t)action, NULL);

    return ret;
}

static wiced_result_t bt_audio_hfp_multi_func_short_release(void)
{
    wiced_result_t ret = WICED_ERROR;
    int action = -1;

    switch (hfp_ctxt_data[hfp_context_index].call_setup)
    {
    //answer incoming call
    case WICED_BT_HFP_HF_CALLSETUP_STATE_INCOMING:
        action = WICED_BT_HFP_HF_CALL_ACTION_ANSWER;
        break;

    //accept a waiting call by ending the active call
    case WICED_BT_HFP_HF_CALLSETUP_STATE_WAITING:
        action = WICED_BT_HFP_HF_CALL_ACTION_HOLD_1;
        break;

    //end an outgoing call being setup
    case WICED_BT_HFP_HF_CALLSETUP_STATE_DIALING:
    case WICED_BT_HFP_HF_CALLSETUP_STATE_ALERTING: //fall through
        action = WICED_BT_HFP_HF_CALL_ACTION_HANGUP;
        break;

    default: //WICED_BT_HFP_HF_CALLSETUP_STATE_IDLE
        if (hfp_ctxt_data[hfp_context_index].call_active == 1)
        {
            //end an active call or end an active call and accept a held call if present
            action = (hfp_ctxt_data[hfp_context_index].call_held == 0) ? WICED_BT_HFP_HF_CALL_ACTION_HANGUP : WICED_BT_HFP_HF_CALL_ACTION_HOLD_1;
        }
        break;
    }

    if (action != -1)
        ret = wiced_bt_hfp_hf_perform_call_action(hfp_ctxt_data[hfp_context_index].peer_bd_addr, (wiced_bt_hfp_hf_call_action_t)action, NULL);

    return ret;
}

static wiced_result_t bt_audio_hfp_button_event_handler(app_service_action_t action)
{
    wiced_result_t ret = WICED_ERROR;
    switch (action)
    {
    case ACTION_VOLUME_UP:
        ret = bt_audio_hfp_volume_up();
        break;
    case ACTION_VOLUME_DOWN:
        ret = bt_audio_hfp_volume_down();
        break;
    case ACTION_MULTI_FUNCTION_SHORT_RELEASE:
        ret = bt_audio_hfp_multi_func_short_release();
        break;
    case ACTION_MULTI_FUNCTION_LONG_RELEASE:
        ret = bt_audio_hfp_multi_func_long_release();
        break;
    case ACTION_PAUSE_PLAY:
    case ACTION_STOP:
    case ACTION_FORWARD:
    case ACTION_BACKWARD:
    case ACTION_FACTORY_RESET:
    case NO_ACTION:
    default:
        break;
    }
    return ret;
}

void bt_audio_hfp_sco_connection_evt_handler(wiced_bt_sco_connected_t *connected)
{
    WPRINT_APP_INFO(("%s: SCO Audio connected, sco_index = %d [in context sco index=%d]\n", __func__, connected->sco_index, hfp_ctxt_data[hfp_context_index].sco_index));
    bt_audio_start_loop(BT_AUDIO_DEVICE_RECORDER);
}

void bt_audio_hfp_sco_disconnection_evt_handler(wiced_bt_sco_disconnected_t *disconnected)
{
    wiced_bt_dev_status_t status;

    WPRINT_APP_INFO(("%s: SCO Audio disconnected, sco_index = %d\n", __func__, disconnected->sco_index));
    bt_audio_end_loop(BT_AUDIO_DEVICE_PLAYER | BT_AUDIO_DEVICE_RECORDER);
    if (hfp_ctxt_data[hfp_context_index].connection_status != WICED_BT_HFP_HF_STATE_DISCONNECTED)
    {
        status = wiced_bt_sco_create_as_acceptor(&hfp_ctxt_data[hfp_context_index].sco_index);
        WPRINT_APP_INFO(("%s: status [%d] SCO INDEX [%d] \n", __func__, status, hfp_ctxt_data[hfp_context_index].sco_index));
    }

    if (use_wbs)
    {
        BTM_SetWBSCodec(BTM_SCO_CODEC_CVSD);
        BTM_WriteVoiceSettings(BTM_VOICE_SETTING_CVSD);
    }
}

void bt_audio_hfp_sco_connection_request_evt_handler(wiced_bt_sco_connection_request_t *conn_request)
{
    wiced_app_service_t  *service = NULL;
    wiced_result_t result;
    /* FIXME: Remove the USE_WICED_HCI and get the sample rate based on the codec info sent under the event AT_BCS*/
    bt_audio_config_t audio_config =
    {
        .sample_rate = 16000,
        .bits_per_sample = 16,
        .channels = 2,
        .volume = BT_AUDIO_DEFAULT_VOLUME
    };

    if (use_wbs == TRUE)
    {
        audio_config.sample_rate  = 16000;
    }
    else
    {
        audio_config.sample_rate  = 8000;
    }

    WPRINT_APP_INFO(("%s: SCO Audio connection request, sco_index = %d\n", __func__, conn_request->sco_index));

    hfp_context_index = get_hfp_context(conn_request->bd_addr);
    if (hfp_context_index < 0 && hfp_context_index >= BT_HFP_CONTEXT_MAX_LIMIT)
    {
        return;
    }
#if 0
    if (hfp_ctxt_data[hfp_context_index].inband_ring_status == WICED_BT_HFP_HF_INBAND_RING_DISABLED)
        wiced_tone_stop(); //stop tone if being played out, if not, does nothing.
#endif

    service = get_app_current_service();
    /* if HFP service has already started or previous service has been stopped */
    if (service == NULL || service->type == SERVICE_BT_HFP)
    {
        /*TODO: service type should be HFP by now, test to confirm and remove if service == NULL */
        result = bt_audio_configure_device(&audio_config, BT_AUDIO_DEVICE_PLAYER, (service_type_t)service->type);
        if (result == WICED_SUCCESS)
        {
            result = bt_audio_configure_device(&audio_config, BT_AUDIO_DEVICE_RECORDER, (service_type_t)service->type);
            if (result != WICED_SUCCESS)
                bt_audio_end_loop(BT_AUDIO_DEVICE_PLAYER);
        }

        if (result == WICED_SUCCESS)
        {
            if (use_wbs == TRUE)
            {
                result = BTM_SetWBSCodec(BTM_SCO_CODEC_MSBC);
                WPRINT_APP_INFO(("[%s] BTM_SetWBSCodec result = %d\n", __func__, result));

                /* Specify PCM input for SBC codec in fw */
                BTM_ConfigI2SPCM(BTM_SCO_CODEC_MSBC,
                                 (UINT8)HCI_BRCM_I2SPCM_IS_DEFAULT_ROLE,
                                 (UINT8)HCI_BRCM_I2SPCM_SAMPLE_DEFAULT,
                                 (UINT8)HCI_BRCM_I2SPCM_CLOCK_DEFAULT);

                BTM_WriteVoiceSettings(BTM_VOICE_SETTING_TRANS);

                result = wiced_bt_sco_set_data_callback(bt_audio_sco_data_cback);
                WPRINT_APP_INFO(("[%s] SCO data cb register result = %d\n", __func__, result));

                wiced_bt_sco_accept_connection(conn_request->sco_index, WICED_BT_SCO_CONNECTION_ACCEPT, WICED_BT_SCO_ESCO_SETTING_ID_MSBC_T2);
            }
            else
            {
                result = BTM_SetWBSCodec(BTM_SCO_CODEC_CVSD);
                WPRINT_APP_INFO(("[%s] BTM_SetWBSCodec result = %d\n", __func__, result));

                /* Specify PCM input for SBC codec in fw */
                BTM_ConfigI2SPCM(BTM_SCO_CODEC_CVSD,
                                 (UINT8)HCI_BRCM_I2SPCM_IS_DEFAULT_ROLE,
                                 (UINT8)HCI_BRCM_I2SPCM_SAMPLE_DEFAULT,
                                 (UINT8)HCI_BRCM_I2SPCM_CLOCK_DEFAULT);

                BTM_WriteVoiceSettings(BTM_VOICE_SETTING_CVSD);

                result = wiced_bt_sco_set_data_callback(bt_audio_sco_data_cback);
                WPRINT_APP_INFO(("[%s] SCO data cb register result = %d\n", __func__, result));

                wiced_bt_sco_accept_connection(conn_request->sco_index, WICED_BT_SCO_CONNECTION_ACCEPT, WICED_BT_SCO_ESCO_SETTING_ID_CVSD);
            }
        }
        else
        {
            wiced_bt_sco_accept_connection(conn_request->sco_index, WICED_BT_SCO_CONNECTION_REJECT_RESOURCES, WICED_BT_SCO_ESCO_SETTING_ID_CVSD);
        }
    }
    //else??
}

void bt_audio_hfp_sco_connection_change_evt_handler(wiced_bt_sco_connection_change_t *conn_change)
{
    WPRINT_APP_INFO(("%s: SCO Audio connection changed, sco_index = %d, rx_pkt_len = %d, tx_pkt_len = %d\n", __func__,
                     conn_change->sco_index, conn_change->rx_pkt_len, conn_change->tx_pkt_len));
}

wiced_bt_hfp_hf_connection_state_t bt_audio_hfp_get_connection_state(void)
{
    return hfp_ctxt_data[hfp_context_index].connection_status;
}

static int get_hfp_context(wiced_bt_device_address_t remote_addr)
{
    int i = 0;

    while (i < BT_HFP_CONTEXT_MAX_LIMIT)
    {
        if ((hfp_ctxt_data[i].in_use_context == WICED_TRUE) &&
                (!memcmp(remote_addr, hfp_ctxt_data[i].peer_bd_addr, sizeof(wiced_bt_device_address_t))))
        {
            WPRINT_APP_INFO(("get_hfp_context Already present i:%d\n", i));
            return i;
        }
        i++;
    }
    return -1;
}

static int set_hfp_context(wiced_bt_device_address_t remote_addr)
{
    int i = 0;

    while (i < BT_HFP_CONTEXT_MAX_LIMIT)
    {
        WPRINT_APP_INFO(("hfp_ctxt_data[%d].in_use_context:%d \n", i, hfp_ctxt_data[i].in_use_context));
        if ((hfp_ctxt_data[i].in_use_context == WICED_TRUE) &&
                (!memcmp(remote_addr, hfp_ctxt_data[i].peer_bd_addr, sizeof(wiced_bt_device_address_t))))
        {
            WPRINT_APP_INFO(("set_hfp_context Already present i:%d\n", i));
            return i;
        }
        else if (hfp_ctxt_data[i].in_use_context == WICED_FALSE)
        {
            hfp_ctxt_data[i].in_use_context = WICED_TRUE;
            WPRINT_APP_INFO(("set_hfp_context New entry i:%d\n", i));
            return i;
        }
        i++;
    }
    return -1;
}

void bt_audio_hfp_clear_data(void)
{
    memset(hfp_ctxt_data, 0, sizeof(hfp_ctxt_data));
    hfp_context_index = 0;
}

