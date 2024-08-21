/*
 * Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-04-17     Jair Wu      First version
 *
 */

#include "record_opus.h"
#include "drv_dsp.h"

#ifdef RT_USING_EXT_OPUS_ENCODER
#define DBG_SECTION_NAME    "OPUS"
#define DBG_LEVEL           DBG_INFO
#include "rtdbg.h"

struct record_opus
{
    bool has_post;
    encode_input_t input;
    encode_output_t output;
    encode_post_t post;
    void *userdata;
};
typedef struct record_opus *record_opus_handle_t;

static opus_enc_t *g_Opus;
static struct audio_server_data_share *g_share_data = NULL;

int record_opus_init(struct record_encoder *self, record_encoder_cfg_t *cfg)
{
    record_opus_handle_t opus = (record_opus_handle_t) rt_calloc(1, sizeof(*opus));
    RT_ASSERT(opus != NULL);
    struct audio_config opus_cfg = *(struct audio_config *) self->userdata;
    struct opus_info *info;
    char **comments;
    int ret = 0;
    if (!opus)
        return -RT_ERROR;
    opus->has_post = 0;
    opus->input = cfg->input;
    opus->output = cfg->output;
    opus->post = cfg->post;
    opus->userdata = cfg->userdata;

    opus_cfg.bitrate = 128000;
    info = rkdsp_malloc(sizeof(struct opus_info));
    RT_ASSERT(info != NULL);
    info->max_packets = 20;
    info->comment_num = 2;

    comments = rkdsp_malloc((sizeof(char *) * info->comment_num));
    RT_ASSERT(comments != NULL);
    comments[0] = rkdsp_malloc(128);
    RT_ASSERT(comments[0] != NULL);
    memset(comments[0], 0x0, 128);
    snprintf(comments[0], 128, "ARTIST=RK");

    comments[1] = rkdsp_malloc(128);
    RT_ASSERT(comments[1] != NULL);
    memset(comments[1], 0x0, 128);
    snprintf(comments[1], 128, "SN=test012345678901");

    info->comments = (uint32_t)comments;
    g_share_data = rkdsp_malloc(sizeof(struct audio_server_data_share));
    RT_ASSERT(g_share_data != NULL);
    g_share_data->dat[0] = (uint32_t)&opus_cfg;
    g_share_data->dat[1] = (uint32_t)info;
    ret = AudioOpusEncodeOpen((uint32_t)g_share_data);
    rkdsp_free(info);
    for (int i = 0; i < info->comment_num; i++)
        rkdsp_free(comments[i]);
    rkdsp_free(comments);
    if (ret < 0)
        return -RT_ERROR;
    self->userdata = (void *)opus;
    g_Opus = (opus_enc_t *)g_share_data->dat[0];
    LOG_I("opus init SUCCESS out %p", g_Opus);

    return RT_EOK;
}

record_encoder_error_t record_opus_process(struct record_encoder *self)
{
    record_opus_handle_t opus = (record_opus_handle_t)self->userdata;
    int32_t num_enc_bytes;
    int write_bytes = 0;
    int ret = 0;
    int read_size = g_Opus->frame_size * g_Opus->channels * sizeof(short);
    LOG_D("Opus encode process");

    if (g_Opus->header_size)
    {
        opus->output(opus->userdata, (char *)g_Opus->_buffer_header, g_Opus->header_size);
        g_Opus->header_size = 0;
    }
    while (1)
    {
        int read_bytes = opus->input(opus->userdata, (char *)g_Opus->_buffer_in, read_size);
        if (read_bytes == 0)
        {
FINISH_RETRY:
            AudioOpusEncodeFinish();
            num_enc_bytes = g_share_data->dat[1];
            if (num_enc_bytes > 0)
            {
                write_bytes = opus->output(opus->userdata, (char *)g_Opus->_buffer_out, num_enc_bytes);
                goto FINISH_RETRY;
            }
            LOG_I("opus->input finish");
            return RECORD_ENCODER_SUCCESS;
        }
        else if (read_bytes == -1)
        {
            LOG_E("opus->input failed");
            return RECORD_ENCODER_INPUT_ERROR;
        }

        ret = AudioOpusEncode();
        if (ret < 0)
        {
            return RECORD_ENCODER_ENCODE_ERROR;
        }
        num_enc_bytes = g_share_data->dat[1];
        if (num_enc_bytes < 0)
        {
            LOG_E("OPUS encode return %ld bytes", num_enc_bytes);
            return RECORD_ENCODER_ENCODE_ERROR;
        }
        LOG_D("enc bytes %d", num_enc_bytes);
        if (num_enc_bytes == 0)
            continue;
        write_bytes = opus->output(opus->userdata, (char *)g_Opus->_buffer_out, num_enc_bytes);
        if (write_bytes == -1)
        {
            LOG_E("opus->output failed");
            return RECORD_ENCODER_OUTPUT_ERROR;
        }
    }
    return 0;
}

bool record_opus_get_post_state(struct record_encoder *self)
{
    record_opus_handle_t opus = (record_opus_handle_t) self->userdata;
    return opus->has_post;
}

void record_opus_destroy(struct record_encoder *self)
{
    record_opus_handle_t opus = (record_opus_handle_t) self->userdata;
    if (opus)
        rt_free(opus);
    if (g_share_data)
    {
        rkdsp_free(g_share_data);
        g_share_data = NULL;
    }
    AudioOpusEncodeClose();
}

#endif  // RT_USING_EXT_OPUS_ENCODER
