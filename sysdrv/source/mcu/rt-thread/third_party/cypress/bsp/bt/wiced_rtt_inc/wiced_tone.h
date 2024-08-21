/*
 * $ Copyright Cypress Semiconductor $
 */

#ifndef WICED_TONE_H
#define WICED_TONE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "wiced_audio.h"

/**  \file    wiced_bt.h
        \brief  API header file for WICED Tone Player functionality accessed from application.
        Application can make use of Tone APIs for playing short tones stored on the device.
*/

/** \defgroup tone WICED TONE
*
* @{
*/

/** \brief Play tone
*
* This function starts playback of locally stored tone data.
*
* \param  data           pointer to tone data location.
* \param  length        length of data in bytes.
* \param  audio_info  audio configuration information, see @wiced_audio_config_t
*                               sample rate, bits per sample, number of channels and volume.
* \param  repeat       number of times the tone should be repeated, 0 - no repetition.
*                              > 0, repeat n times.
*
* \return    WICED_SUCCESS : on success;
*                WICED_ERROR   : if an error occurred
*/
wiced_result_t wiced_tone_play(uint8_t *data, uint32_t length, wiced_audio_config_t *audio_info, uint8_t repeat);


/** \brief Stop the tone currently being played
*
* This function stops the playback of tone, if currently active.
*
* \return    WICED_SUCCESS : on success;
*                WICED_ERROR   : if an error occurred
*/
wiced_result_t wiced_tone_stop(void);


/** @} */ // end of tone

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif //#ifndef WICED_TONE_H

