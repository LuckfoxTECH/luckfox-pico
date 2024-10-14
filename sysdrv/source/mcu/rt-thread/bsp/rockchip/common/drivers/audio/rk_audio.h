/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  *
  * Change Logs:
  * Date           Author       Notes
  * 2019-07-10     sugar      first implementation.
  */

#ifndef __RK_AUDIO_H__
#define __RK_AUDIO_H__

#include "hal_audio.h"

#ifdef RT_USING_DRIVER_AUDIO_PCM_PLUGIN
#include "pcm/pcm_types.h"
#endif

#define RK_AUDIO_CTL(a) (0x1000 + a)

/** cmd for audio stream */
#define RK_AUDIO_CTL_HW_PARAMS     RK_AUDIO_CTL(1)
#define RK_AUDIO_CTL_STATUS        RK_AUDIO_CTL(2)
#define RK_AUDIO_CTL_SYNC_PTR      RK_AUDIO_CTL(3)
#define RK_AUDIO_CTL_PREPARE       RK_AUDIO_CTL(4)
#define RK_AUDIO_CTL_RESET         RK_AUDIO_CTL(5)
#define RK_AUDIO_CTL_START         RK_AUDIO_CTL(6)
#define RK_AUDIO_CTL_STOP          RK_AUDIO_CTL(7)
#define RK_AUDIO_CTL_DRAIN         RK_AUDIO_CTL(8)
#define RK_AUDIO_CTL_PAUSE         RK_AUDIO_CTL(9)
#define RK_AUDIO_CTL_RESUME        RK_AUDIO_CTL(10)
#define RK_AUDIO_CTL_WRITEI_FRAMES RK_AUDIO_CTL(11)
#define RK_AUDIO_CTL_READI_FRAMES  RK_AUDIO_CTL(12)
#define RK_AUDIO_CTL_SET_GAIN      RK_AUDIO_CTL(13)
#define RK_AUDIO_CTL_GET_GAIN      RK_AUDIO_CTL(14)
#define RK_AUDIO_CTL_GET_GAIN_INFO RK_AUDIO_CTL(15)
/** cmd for vad*/
#define RK_AUDIO_CTL_VAD_SET_BUFFER   RK_AUDIO_CTL(20)
#define RK_AUDIO_CTL_VAD_SET_PERIOD   RK_AUDIO_CTL(21)
#define RK_AUDIO_CTL_VAD_EN_BUSMODE   RK_AUDIO_CTL(22)
#define RK_AUDIO_CTL_VAD_DIS_BUSMODE  RK_AUDIO_CTL(23)

/** cmd for pcm buffer */
#define RK_AUDIO_CTL_PCM_PREPARE       RK_AUDIO_CTL(30)
#define RK_AUDIO_CTL_PCM_RELEASE       RK_AUDIO_CTL(31)
#define RK_AUDIO_CTL_PCM_BUFFER_STATUS RK_AUDIO_CTL(32)
#define RK_AUDIO_CTL_PCM_START         RK_AUDIO_CTL(33)
#define RK_AUDIO_CTL_PCM_STOP          RK_AUDIO_CTL(34)
/** cmd for audio path switch */
#define RK_AUDIO_CTL_PATH_VAD    RK_AUDIO_CTL(40)
#define RK_AUDIO_CTL_PATH_DMA    RK_AUDIO_CTL(41)
#define RK_AUDIO_CTL_PATH_BYPASS RK_AUDIO_CTL(42)
/** cmd for audio plugins */
#define RK_AUDIO_CTL_PLUGIN_PREPARE     RK_AUDIO_CTL(100)
#define RK_AUDIO_CTL_PLUGIN_RELEASE     RK_AUDIO_CTL(101)
#define RK_AUDIO_CTL_PLUGIN_HW          RK_AUDIO_CTL(102)
#define RK_AUDIO_CTL_PLUGIN_SET_SOFTVOL RK_AUDIO_CTL(103)
#define RK_AUDIO_CTL_PLUGIN_GET_SOFTVOL RK_AUDIO_CTL(104)
#define RK_AUDIO_CTL_PLUGIN_FIRST       RK_AUDIO_CTL_PLUGIN_PREPARE
#define RK_AUDIO_CTL_PLUGIN_LAST        RK_AUDIO_CTL_PLUGIN_GET_SOFTVOL

/** audio stream num: playback and capture */
#define AUDIO_STREAM_NUM 2
#define MAX_DAIS 4

typedef enum
{
    AUDIO_STREAM_STATE_OPEN,
    AUDIO_STREAM_STATE_SETUP,
    AUDIO_STREAM_STATE_PREPARED,
    AUDIO_STREAM_STATE_RUNNING,
    AUDIO_STREAM_STATE_XRUN,
    AUDIO_STREAM_STATE_STOP,
} audio_stream_state_t;

typedef uint32_t audio_pcm_uframes_t;
typedef int32_t audio_pcm_sframes_t;

/**
 * struct audio_buf - audio buf struct.
 */
struct audio_buf
{
    uint8_t *buf; /**< audio buf */
    audio_pcm_uframes_t buf_size; /**< buf size in uframes */
    audio_pcm_uframes_t period_size; /**< period size in uframes */
};

/**
 * struct audio_buf_status - audio buf status.
 */
struct audio_buf_status
{
    audio_pcm_uframes_t hw_ptr; /**< audio hw ptr in uframes */
    audio_pcm_uframes_t appl_ptr; /**< audio appl ptr in uframes */
};

struct audio_dai;

/**
 * struct audio_dai_ops - audio dai ops.
 */
struct audio_dai_ops
{
    rt_err_t (*init)(struct audio_dai *dai, struct AUDIO_INIT_CONFIG *config);
    rt_err_t (*deinit)(struct audio_dai *dai);
    rt_err_t (*set_clk)(struct audio_dai *dai, eAUDIO_streamType stream, uint32_t freq);
    rt_err_t (*config)(struct audio_dai *dai, eAUDIO_streamType stream, struct AUDIO_PARAMS *params);
    rt_err_t (*start)(struct audio_dai *dai, eAUDIO_streamType stream);
    rt_err_t (*stop)(struct audio_dai *dai, eAUDIO_streamType stream);
};

/**
 * struct audio_dai - audio dai struct.
 */
struct audio_dai
{
    rt_list_t list; /**< list node */
    uint32_t id; /**< assign from dai base(such as i2s) as the unique id. */
    struct audio_card *card; /**< parent card */
    const struct AUDIO_DMA_DATA *dmaData[AUDIO_STREAM_NUM]; /**< dma data */
    const struct audio_dai_ops *ops; /**< audio ops function */
};

struct audio_pcm;

/**
 * struct audio_dai_ops - audio dai ops.
 */
struct audio_pcm_ops
{
    rt_err_t (*init)(struct audio_pcm *pcm, eAUDIO_streamType stream, struct audio_buf *buf);
    rt_err_t (*deinit)(struct audio_pcm *pcm);
    rt_err_t (*config)(struct audio_pcm *pcm, eAUDIO_streamType stream, struct AUDIO_PARAMS *params);
    rt_err_t (*start)(struct audio_pcm *pcm);
    rt_err_t (*stop)(struct audio_pcm *pcm);
};

struct audio_card;

/** PCM type */
enum _snd_pcm_type
{
    /** Hardware level PCM */
    SND_PCM_TYPE_HW = 0,
    /** Soft volume plugin */
    SND_PCM_TYPE_SOFTVOL,
    SND_PCM_TYPE_LAST = SND_PCM_TYPE_SOFTVOL,
};

/** PCM type */
typedef enum _snd_pcm_type snd_pcm_type_t;

/**
 * struct audio_pcm - audio pcm struct.
 */
struct audio_pcm
{
    struct audio_card *card; /**< the parent audio card */
    struct audio_stream *as; /**< the parent audio stream */
    struct AUDIO_PARAMS params; /**< audio params */
    struct audio_buf abuf; /**< buf info */
    struct audio_buf_status status; /**< buf status */
    uint32_t boundary; /**< buf boundary */
    rt_sem_t wait; /**< wait sem for avail buf */
    rt_bool_t waiting; /**< indicate waiting for sem release */
    const struct audio_pcm_ops *ops; /**< audio ops function */
#ifdef RT_USING_DRIVER_AUDIO_PCM_PLUGIN
    audio_pcm_uframes_t start_threshold; /**< start threshold */
    audio_pcm_uframes_t stop_threshold; /**< stop threshold */
    snd_pcm_type_t type;
    snd_pcm_format_t format;
    snd_pcm_channel_area_t *running_areas;
    snd_pcm_t *fast_op_arg; /**fast_op_arg pcm */
    void *private_data;
    char name[SND_NAME_MAX];
    const snd_pcm_plugin_ops_t *plugin_ops;
    const snd_pcm_fast_ops_t *fast_ops;
#endif
};

struct audio_codec;

/**
 * struct audio_codec_ops - audio codec ops.
 */
struct audio_codec_ops
{
    rt_err_t (*init)(struct audio_codec *codec, struct AUDIO_INIT_CONFIG *config);
    rt_err_t (*deinit)(struct audio_codec *codec);
    rt_err_t (*set_clk)(struct audio_codec *codec, eAUDIO_streamType stream, uint32_t freq);
    rt_err_t (*config)(struct audio_codec *codec, eAUDIO_streamType stream, struct AUDIO_PARAMS *params);
    rt_err_t (*start)(struct audio_codec *codec, eAUDIO_streamType stream);
    rt_err_t (*stop)(struct audio_codec *codec, eAUDIO_streamType stream);
    rt_err_t (*set_gain)(struct audio_codec *codec, eAUDIO_streamType stream, struct AUDIO_DB_CONFIG *dBConfig);
    rt_err_t (*get_gain)(struct audio_codec *codec, eAUDIO_streamType stream, struct AUDIO_DB_CONFIG *dBConfig);
    rt_err_t (*get_gaininfo)(struct audio_codec *codec, struct AUDIO_GAIN_INFO *info);
};

/**
 * struct audio_codec - audio codec struct.
 */
struct audio_codec
{
    rt_list_t list; /**< list node */
    uint32_t id; /**< assign from codee base as the unique id. */
    struct audio_card *card; /**< parent card */
    const struct audio_codec_ops *ops; /**< audio ops function */
};

/**
 * struct audio_mdais_desc - audio multi dais desc.
 */
struct audio_mdais_desc
{
    void *dais[MAX_DAIS]; /**< dai instance array */
    uint8_t capture_mapping[MAX_DAIS]; /**< capture channel mapping */
    uint8_t playback_mapping[MAX_DAIS]; /**< playback channel mapping */
};

/**
 * struct audio_card_desc - audio card desc.
 */
struct audio_card_desc
{
    char name[RT_NAME_MAX];
    void *dai; /**< dai instance: I2Sx, I2STDMx, PDMx... */
    void *vad; /**< vad instance */
    void *codec; /**< codec instance */
    bool codec_master; /**< indicate codec is master */
    bool clk_invert; /**< indicate clk is invert */
    bool playback; /**< indicate support playback */
    bool capture; /**< indicate support capture */
    bool multi_dais; /**< indicate support for multi dais */
    eAUDIO_fmtType format; /**< indicate xfer format */
    eTRCM_modeType trcm_mode; /**< indicate trcm mode just for i2stdm */
    ePDM_mode pdm_mode; /**< indicate pdm mode */
    uint16_t mclkfs; /**< mclk fs */
    uint16_t txMap; /**< route mapping of PATHx to SDOx, 4 bits per path.
                      *  |15:12|11:8|7:4|3:0|-->|p3|p2|p1|p0|
                      *  each path can choose one sdo as its sink.
                      *  e.g.
                      *  txMap = 0x3210: p3->sdo3, p2->sdo2, p1->sdo1, p0->sdo0
                      *  txMap = 0x3012: p3->sdo3, p2->sdo0, p1->sdo1, p0->sdo2
                      *  txMap = 0x0: ignored
                      */
    uint16_t rxMap; /**< route mapping of SDIx to PATHx, 4 bits per path.
                      *  |15:12|11:8|7:4|3:0|-->|p3|p2|p1|p0|
                      *  each path can choose one sdi as its source.
                      *  e.g.
                      *  rxMap = 0x3210: p3<-sdi3, p2<-sdi2, p1<-sdi1, p0<-sdi0
                      *  rxMap = 0x3012: p3<-sdi3, p2<-sdi0, p1<-sdi1, p0<-sdi2
                      *  rxMap = 0x0: ignored
                      */
};

struct audio_card;

struct audio_stream
{
    struct rt_device dev;

    struct audio_card *card;
    struct audio_pcm *pcm;
    struct rt_mutex lock;
    eAUDIO_streamType stream;
    audio_stream_state_t state;
};

/**
 * struct audio_card - audio card struct.
 */
struct audio_card
{
    struct audio_dai *dai; /**< audio dai device */
    struct audio_dai *vad_dai; /**< vad dai device, optional */
    struct audio_codec *codec; /**< audio codec device */
    struct audio_stream *stream[AUDIO_STREAM_NUM];
    struct audio_card_desc desc; /**< audio card desc */
};

rt_inline uint32_t frame_bits(struct audio_pcm *pcm)
{
    return (pcm->params.sampleBits * pcm->params.channels);
}

rt_inline uint32_t bytes_to_samples(struct audio_pcm *pcm, uint32_t size)
{
    return size * 8 / pcm->params.sampleBits;
}

rt_inline audio_pcm_sframes_t bytes_to_frames(struct audio_pcm *pcm, uint32_t size)
{
    return size * 8 / frame_bits(pcm);
}

rt_inline uint32_t samples_to_bytes(struct audio_pcm *pcm, uint32_t size)
{
    return size * pcm->params.sampleBits / 8;
}

rt_inline uint32_t frames_to_bytes(struct audio_pcm *pcm, audio_pcm_uframes_t size)
{
    return size * frame_bits(pcm) / 8;
}

rt_inline rt_size_t audio_pcm_buffer_bytes(struct audio_pcm *pcm)
{
    return frames_to_bytes(pcm, pcm->abuf.buf_size);
}

rt_inline rt_size_t audio_pcm_period_bytes(struct audio_pcm *pcm)
{
    return frames_to_bytes(pcm, pcm->abuf.period_size);
}

rt_inline audio_pcm_uframes_t audio_pcm_playback_avail(struct audio_pcm *pcm)
{
    audio_pcm_sframes_t avail;

    avail = pcm->status.hw_ptr + pcm->abuf.buf_size - pcm->status.appl_ptr;
    if (avail < 0)
        avail += pcm->boundary;
    else if ((audio_pcm_uframes_t) avail >= pcm->boundary)
        avail -= pcm->boundary;

    return avail;
}

rt_inline audio_pcm_uframes_t audio_pcm_capture_avail(struct audio_pcm *pcm)
{
    audio_pcm_sframes_t avail;

    avail = pcm->status.hw_ptr - pcm->status.appl_ptr;
    if (avail < 0)
        avail += pcm->boundary;

    return avail;
}

rt_inline audio_pcm_sframes_t audio_pcm_playback_hw_avail(struct audio_pcm *pcm)
{
    return pcm->abuf.buf_size - audio_pcm_playback_avail(pcm);
}

rt_inline audio_pcm_sframes_t audio_pcm_capture_hw_avail(struct audio_pcm *pcm)
{
    return pcm->abuf.buf_size - audio_pcm_capture_avail(pcm);
}

#ifdef RT_USING_DRIVER_AUDIO_PCM_PLUGIN
rt_inline void audio_pcm_appl_backward(struct audio_pcm *pcm, audio_pcm_uframes_t frames)
{
    audio_pcm_sframes_t appl_ptr = pcm->status.appl_ptr;
    appl_ptr -= frames;
    if (appl_ptr < 0)
        appl_ptr += pcm->boundary;
    pcm->status.appl_ptr = appl_ptr;
}

rt_inline void audio_pcm_appl_forward(struct audio_pcm *pcm, audio_pcm_uframes_t frames)
{
    audio_pcm_uframes_t appl_ptr = pcm->status.appl_ptr;
    appl_ptr += frames;
    if (appl_ptr >= pcm->boundary)
        appl_ptr -= pcm->boundary;
    pcm->status.appl_ptr = appl_ptr;
}

rt_inline void audio_pcm_hw_backward(struct audio_pcm *pcm, audio_pcm_uframes_t frames)
{
    audio_pcm_sframes_t hw_ptr = pcm->status.hw_ptr;
    hw_ptr -= frames;
    if (hw_ptr < 0)
        hw_ptr += pcm->boundary;
    pcm->status.hw_ptr = hw_ptr;
}

rt_inline void audio_pcm_hw_forward(struct audio_pcm *pcm, audio_pcm_uframes_t frames)
{
    audio_pcm_uframes_t hw_ptr = pcm->status.hw_ptr;
    hw_ptr += frames;
    if (hw_ptr >= pcm->boundary)
        hw_ptr -= pcm->boundary;
    pcm->status.hw_ptr = hw_ptr;
}

/**
 * audio_stream_get_state - Get the audio stream state
 * @pcm: Audio stream instance
 */
rt_inline audio_stream_state_t audio_stream_get_state(struct audio_stream *as)
{
    return as->state;
}

rt_err_t rk_audio_hw_start(struct audio_stream *as);
rt_err_t rk_audio_hw_stop(struct audio_stream *as);
rt_err_t rk_audio_hw_wait_for_avail(struct audio_stream *as);
#endif

rt_err_t rk_audio_register_dai(struct audio_dai *dai);
void rk_audio_unregister_dai(struct audio_dai *dai);
rt_err_t rk_audio_register_codec(struct audio_codec *codec);
void rk_audio_unregister_codec(struct audio_codec *codec);
struct audio_dai *rk_audio_find_dai(uint32_t id);
struct audio_codec *rk_audio_find_codec(uint32_t id);
struct audio_stream *rk_audio_new_stream(struct audio_card *card, eAUDIO_streamType stream);
void rk_audio_free_stream(struct audio_stream *as);
void rk_audio_stream_update(struct audio_stream *as);
struct audio_pcm *rk_audio_new_pcm(struct audio_card *card);
void rk_audio_free_pcm(struct audio_pcm *pcm);
#ifdef RT_USING_MULTI_PCM
struct audio_pcm *rk_audio_new_mpcm(struct audio_card *card);
void rk_audio_free_mpcm(struct audio_pcm *pcm);
#endif
struct audio_card *rk_audio_new_card(const struct audio_card_desc *acd);
void rk_audio_free_card(struct audio_card *card);
rt_err_t rk_audio_register(struct audio_card *card, const char *name);
rt_err_t rk_audio_register_by_card_desc(const struct audio_card_desc *desc);

#endif /* __RK_AUDIO_H__ */
