/*
 * $ Copyright Cypress Semiconductor $
 */
#include "wiced.h"
#include "wiced_rtos.h"
#include "bluetooth_audio.h"
#include "wiced_codec_sbc_params.h"
#include "wwd_debug.h"

#ifdef USE_MEM_POOL
#include "mem_pool.h"
#endif

/*****************************************************************************
**
**  Name:           bluetooth_audio_decoder.c
**
**  Description:    Implements interfacing functions to invoke decoders.
**
*****************************************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/
unsigned short bt_audio_decoder_alloc_output_buffer(int16_t **buffer, uint16_t length);
unsigned short bt_audio_decoder_read_encoded_data(uint8_t *data, uint16_t length);
unsigned short bt_audio_decoder_write_decoded_data(int16_t *data, uint16_t length);

/******************************************************
 *                      Macros
 ******************************************************/
#define BT_AUDIO_INIT_CODEC_CBS { \
                                                               .alloc_output_buffer_fp = bt_audio_decoder_alloc_output_buffer,   \
                                                               .read_encoded_data_fp = bt_audio_decoder_read_encoded_data,  \
                                                               .write_decoded_data_fp = bt_audio_decoder_write_decoded_data  \
                                                       }

/******************************************************
 *                    Constants
 ******************************************************/
#define BT_AUDIO_DECODER_QUEUE_MAX_SIZE   20

/******************************************************
 *               Variables Definitions
 ******************************************************/
bt_audio_decoder_context_t decoder;
static bt_audio_codec_data_t *decoder_input_buffer = NULL;
#ifdef USE_MEM_POOL
extern bt_buffer_pool_handle_t  mem_pool;
extern bt_buffer_pool_handle_t  mem_pool_pcm;
#endif

/******************************************************
*               Function Definitions
******************************************************/

wiced_result_t bt_audio_decoder_context_init(void)
{
    wiced_result_t result;
    wiced_codec_data_transfer_api_t decoder_cbs = BT_AUDIO_INIT_CODEC_CBS;

    decoder.wiced_sbc_if = wiced_get_codec_by_type(WICED_CODEC_SBC);
    wiced_assert("SBC decoder interface not available!", decoder.wiced_sbc_if != NULL);
    if (decoder.wiced_sbc_if == NULL)
    {
        WPRINT_APP_INFO(("bt_audio_context_init: get decoder i/f \n"));
        return WICED_ERROR;
    }
    decoder.wiced_sbc_if->init(&decoder_cbs, NULL);
    result = wiced_rtos_init_event_flags(&decoder.events);
    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("bt_audio_decoder_context_init: event flags creation result=%d\n", result));
        return result;
    }

    result = wiced_rtos_init_queue(&decoder.queue, "CODEC_QUEUE", sizeof(bt_audio_codec_data_t *), BT_AUDIO_DECODER_QUEUE_MAX_SIZE);
    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("bt_audio_decoder_context_init: queue creation result=%d\n", result));
    }
    return result;
}

wiced_result_t bt_audio_decoder_context_deinit(void)
{
    wiced_result_t result;
    bt_audio_codec_data_t *decoder_buffer = NULL;
    result = wiced_rtos_set_event_flags(&decoder.events, BT_AUDIO_CODEC_EVENT_DEINIT);
    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("wiced_rtos_set_event_flags: result=%d\n", result));
        return result;
    }

    rt_thread_mdelay(1000);

    do
    {
        WPRINT_APP_INFO(("wiced_rtos_pop_from_queue decoder_buffer\n"));
        result = wiced_rtos_pop_from_queue(&decoder.queue, &decoder_buffer, 1);
        if (result == WICED_SUCCESS)
        {
            rt_free(decoder_buffer);
            decoder_buffer = NULL;
        }
        else
        {
            WPRINT_APP_INFO(("wiced_rtos_pop_from_queue break\n"));
        }
    }
    while (result == WICED_SUCCESS);

    result = wiced_rtos_deinit_event_flags(&decoder.events);
    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("wiced_rtos_deinit_event_flags result=%d\n", result));
        return result;
    }

    result = wiced_rtos_deinit_queue(&decoder.queue);
    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("wiced_rtos_deinit_queue: result=%d\n", result));
        return result;
    }

    decoder.wiced_sbc_if = wiced_get_codec_by_type(WICED_CODEC_SBC);
    wiced_assert("SBC decoder interface not available!", decoder.wiced_sbc_if != NULL);
    if (decoder.wiced_sbc_if == NULL)
    {
        WPRINT_APP_INFO(("wiced_sbc_if NULL \n"));
        return result;
    }
    decoder.wiced_sbc_if->close();

    memset(&decoder, 0, sizeof(bt_audio_decoder_context_t));
    return result;
}

wiced_result_t bt_audio_write_to_decoder_queue(bt_audio_codec_data_t *audio)
{
    wiced_result_t result;
    result = wiced_rtos_push_to_queue(&decoder.queue, &audio, WICED_NO_WAIT);
    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("%s: push to queue failed, freeing buffer\n", __func__));
#ifdef USE_MEM_POOL
        bt_buffer_pool_free_buffer(audio);
#else
        rt_free(audio);
#endif
        return result;
    }
    result = wiced_rtos_set_event_flags(&decoder.events, BT_AUDIO_CODEC_EVENT_DECODE);
    return result;
}


wiced_result_t bt_audio_reset_decoder_config(void)
{
    if (decoder_input_buffer != NULL)
    {
#ifdef USE_MEM_POOL
        bt_buffer_pool_free_buffer(decoder_input_buffer);
#else
        rt_free(decoder_input_buffer);
#endif
        decoder_input_buffer = NULL;
    }
    return WICED_SUCCESS;
}


wiced_result_t bt_audio_configure_decoder(wiced_bt_a2dp_codec_info_t *decoder_config)
{
    wiced_result_t result = WICED_ERROR;

    if (decoder_config->codec_id == WICED_BT_A2DP_SINK_CODEC_SBC)
    {
        decoder.wiced_sbc_if->set_configuration((void *) &decoder_config->cie.sbc);
        result = WICED_SUCCESS;
    }
    return result;
}

unsigned short bt_audio_decoder_alloc_output_buffer(int16_t **buffer, uint16_t length)
{
    bt_audio_codec_data_t *pcm;

#ifdef USE_MEM_POOL
    pcm = bt_buffer_pool_allocate_buffer(mem_pool_pcm);
#else
    pcm = rt_malloc(sizeof(bt_audio_codec_data_t) + length);
#endif
    if (pcm == NULL)
    {
        return 0;
    }

    pcm->length = length;
    pcm->offset = 0;
    *buffer = (int16_t *)pcm->data;

    return length;
}

unsigned short bt_audio_decoder_read_encoded_data(uint8_t *data, uint16_t length)
{
    wiced_result_t result;
    uint16_t out_length = 0;

    if (decoder_input_buffer == NULL)
    {
        result = wiced_rtos_pop_from_queue(&decoder.queue, &decoder_input_buffer, WICED_NEVER_TIMEOUT);
        //wiced_assert(("bt_audio_decoder_task: decoder event wait returned error= %d\n", result), (result == WICED_SUCCESS));
        RT_ASSERT(result == WICED_SUCCESS);
        if (result != WICED_SUCCESS)
        {
            return 0;
        }
    }

    out_length = MIN(decoder_input_buffer->length, length);
    //WPRINT_APP_INFO( ("%s: available length %d, length being copied %d, start offset %d, \n", __func__, decoder_input_buffer->length, out_length, audio->offset) );
    /* Read Data from the input stream */
    memcpy(data, (decoder_input_buffer->data + decoder_input_buffer->offset), out_length);

    /* update buffer pointer */
    decoder_input_buffer->length -= out_length;
    decoder_input_buffer->offset += out_length;

    if (decoder_input_buffer->length == 0)
    {
#ifdef USE_MEM_POOL
        bt_buffer_pool_free_buffer(decoder_input_buffer);
#else
        rt_free(decoder_input_buffer);
#endif
        decoder_input_buffer = NULL;
    }
    //WPRINT_APP_INFO( ("%s: returned length %d\n", __func__, out_length) );
    return (out_length);
}

unsigned short bt_audio_decoder_write_decoded_data(int16_t *data, uint16_t length)
{
    bt_audio_codec_data_t *pcm = (bt_audio_codec_data_t *)(((uint8_t *)data) - sizeof(bt_audio_codec_data_t));

    if (length == 0)
    {
        //WPRINT_APP_INFO( ("%s: length = %d \n", __func__, length) );

        if (data != NULL)
        {
#ifdef USE_MEM_POOL
            bt_buffer_pool_free_buffer(pcm);
#else
            rt_free(pcm);
#endif
        }
        return 1;
    }
    pcm->offset = 0;
    pcm->length = length * sizeof(uint16_t);
    bt_audio_write_to_player_buffer(pcm);
    //WPRINT_APP_INFO( ("%s\n", __func__) );
    return 1;
}

void bt_audio_decoder_task(uint32_t arg)
{
    wiced_result_t result;
    uint32_t flags_set = 0;

    WPRINT_APP_INFO(("bt_audio_decoder_task: Starting decoder task\n"));

    while ((result = wiced_rtos_wait_for_event_flags(&decoder.events, BT_AUDIO_CODEC_EVENT_ALL, &flags_set, WICED_TRUE, WAIT_FOR_ANY_EVENT, WICED_WAIT_FOREVER)) == WICED_SUCCESS)
    {
        if ((flags_set & BT_AUDIO_CODEC_EVENT_DEINIT) == BT_AUDIO_CODEC_EVENT_DEINIT)
        {
            WPRINT_APP_INFO(("exit bt_audio_decoder_task \n"));
            break;
        }

        if ((flags_set & BT_AUDIO_CODEC_EVENT_DECODE) != BT_AUDIO_CODEC_EVENT_DECODE)
        {
            continue;
        }

        while (wiced_rtos_is_queue_empty(&decoder.queue) != WICED_SUCCESS)
        {
            wiced_assert("bt_audio_decoder_task: decoder not configured\n", (decoder.wiced_sbc_if->configured != 0));
            if (decoder.wiced_sbc_if->configured == 0)
            {
                WPRINT_APP_INFO(("bt_audio_decoder_task: decoder not configured\n"));
                break;
            }

            //decode
            result = decoder.wiced_sbc_if->decode();
        }
    }

    //wiced_assert(("bt_audio_decoder_task: decoder event wait returned error= %d\n", result), (result == WICED_SUCCESS));
    RT_ASSERT(result == WICED_SUCCESS);
    WPRINT_APP_INFO(("bt_audio_decoder_task: decoder event wait returned error= %d\n", result));

}

