/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    rk_audio.c
  * @author  sugar
  * @version V0.1
  * @date    2019/7/10
  * @brief   AUDIO Driver
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>

#ifdef RT_USING_AUDIO

#include "hal_base.h"
#include "rk_audio.h"

#ifdef RK_AUDIO_DEBUG
#define RK_AUDIO_DBG(...)   rt_kprintf(__VA_ARGS__)
#else
#define RK_AUDIO_DBG(...)
#endif

/********************* Private MACRO Definition ******************************/

#define RK_AUDIO_MCLKFS_DEFAULT 256

/********************* Private Structure Definition **************************/

static rt_list_t s_audio_dai_list = RT_LIST_OBJECT_INIT(s_audio_dai_list);
static rt_list_t s_audio_codec_list = RT_LIST_OBJECT_INIT(s_audio_codec_list);

static struct rt_mutex dai_lock;
static struct rt_mutex codec_lock;

/********************* Private Function Definition ***************************/

static void audio_stream_set_state(struct audio_stream *as, audio_stream_state_t state)
{
    rt_mutex_take(&as->lock, RT_WAITING_FOREVER);
    as->state = state;
    rt_mutex_release(&as->lock);
}

static rt_err_t audio_wait_for_avail(struct audio_stream *as)
{
    struct audio_pcm *pcm = as->pcm;
    rt_err_t ret = RT_EOK;

    rt_mutex_take(&as->lock, RT_WAITING_FOREVER);
    pcm->waiting = RT_TRUE;
    rt_mutex_release(&as->lock);
    rt_sem_take(pcm->wait, RT_WAITING_FOREVER);

    if (as->state != AUDIO_STREAM_STATE_RUNNING)
        ret = -RT_ERROR;

    return ret;
}

static rt_err_t rk_audio_set_clk(struct audio_stream *as, uint32_t samplerate)
{
    struct audio_card *card = as->card;
    struct audio_dai *dai = card->dai;
    struct audio_codec *codec = card->codec;
    eAUDIO_streamType stream = as->stream;
    uint16_t mclkfs = RK_AUDIO_MCLKFS_DEFAULT;
    rt_err_t ret = RT_EOK;
    uint32_t freq;

    if (card->desc.mclkfs)
        mclkfs = card->desc.mclkfs;

    freq = samplerate * mclkfs;
    if (dai->ops->set_clk)
    {
        ret = dai->ops->set_clk(dai, stream, freq);
        if (ret)
            return ret;
    }

    if (codec && codec->ops->set_clk)
    {
        ret = codec->ops->set_clk(codec, stream, freq);
        if (ret)
            return ret;
    }

    return ret;
}

static rt_err_t rk_audio_config(struct audio_stream *as, struct AUDIO_PARAMS *params)
{
    struct audio_card *card = as->card;
    struct audio_dai *dai = card->dai;
    struct audio_dai *vad_dai = card->vad_dai;
    struct audio_codec *codec = card->codec;
    struct audio_pcm *pcm = as->pcm;
    eAUDIO_streamType stream = as->stream;
    rt_err_t ret = RT_EOK;

#ifndef RT_USB_AUDIO_PLL_COMPENSATION
    rk_audio_set_clk(as, params->sampleRate);
#endif

    if (vad_dai)
    {
        ret = vad_dai->ops->config(vad_dai, stream, params);
        if (ret)
            return ret;
    }

    ret = pcm->ops->config(pcm, stream, params);
    if (ret)
        return ret;

    ret = dai->ops->config(dai, stream, params);
    if (ret)
        return ret;

    if (codec)
    {
        ret = codec->ops->config(codec, stream, params);
        if (ret)
            return ret;
    }

#ifdef RT_USING_DRIVER_AUDIO_PCM_PLUGIN
    /* Note: need to after rk_pcm_config() where has been prepared pcm params */
    ret = pcm->plugin_ops->hw_params(pcm, stream, params);
    if (ret)
        return ret;
#endif

    audio_stream_set_state(as, AUDIO_STREAM_STATE_PREPARED);

    return ret;
}

static rt_err_t rk_audio_start(struct audio_stream *as)
{
    struct audio_card *card = as->card;
    struct audio_dai *dai = card->dai;
    struct audio_dai *vad_dai = card->vad_dai;
    struct audio_codec *codec = card->codec;
    struct audio_pcm *pcm = as->pcm;
    eAUDIO_streamType stream = as->stream;
    rt_err_t ret = RT_EOK;

    if (vad_dai)
    {
        ret = vad_dai->ops->start(vad_dai, stream);
        if (ret)
            return ret;
    }

    ret = pcm->ops->start(pcm);
    if (ret)
        return ret;

    ret = dai->ops->start(dai, stream);
    if (ret)
        return ret;

    if (codec)
    {
        ret = codec->ops->start(codec, stream);
        if (ret)
            return ret;
    }

    audio_stream_set_state(as, AUDIO_STREAM_STATE_RUNNING);

    return ret;
}

static rt_err_t _rk_audio_stop(struct audio_stream *as, audio_stream_state_t state)
{
    struct audio_card *card = as->card;
    struct audio_dai *dai = card->dai;
    struct audio_dai *vad_dai = card->vad_dai;
    struct audio_codec *codec = card->codec;
    struct audio_pcm *pcm = as->pcm;
    eAUDIO_streamType stream = as->stream;
    rt_err_t ret = RT_EOK;

    if (vad_dai)
    {
        ret = vad_dai->ops->stop(vad_dai, stream);
        if (ret)
            return ret;
    }

    ret = pcm->ops->stop(pcm);
    if (ret)
        return ret;

    ret = dai->ops->stop(dai, stream);
    if (ret)
        return ret;

    if (codec)
    {
        ret = codec->ops->stop(codec, stream);
        if (ret)
            return ret;
    }

    rt_mutex_take(&as->lock, RT_WAITING_FOREVER);
    as->state = state;
    pcm->waiting = RT_FALSE;
    rt_mutex_release(&as->lock);

    rt_memset(&pcm->status, 0x0, sizeof(struct audio_buf_status));
    rt_sem_control(pcm->wait, RT_IPC_CMD_RESET, 0);

    return ret;
}

static rt_err_t rk_audio_stop(struct audio_stream *as)
{
    return _rk_audio_stop(as, AUDIO_STREAM_STATE_STOP);
}

static rt_err_t rk_audio_xrun(struct audio_stream *as)
{
    return _rk_audio_stop(as, AUDIO_STREAM_STATE_XRUN);
}

static rt_err_t rk_audio_init(rt_device_t dev)
{
    struct audio_stream *as = dev->user_data;
    struct audio_card *card = as->card;
    struct audio_card_desc *desc = &card->desc;
    struct audio_dai *dai = card->dai;
    struct audio_dai *vad_dai = card->vad_dai;
    struct audio_codec *codec = card->codec;
    struct AUDIO_INIT_CONFIG cfg = { 0 };
    rt_err_t ret = RT_EOK;

    cfg.master = !desc->codec_master;
    cfg.clkInvert = desc->clk_invert;
    cfg.format = desc->format;
    cfg.trcmMode = desc->trcm_mode;
    cfg.pdmMode = desc->pdm_mode;
    cfg.rxMap = desc->rxMap;
    cfg.txMap = desc->txMap;

    if (vad_dai)
    {
        ret = vad_dai->ops->init(vad_dai, &cfg);
        if (ret)
            return ret;
    }

    ret = dai->ops->init(dai, &cfg);
    if (ret)
        return ret;

    if (codec)
    {
        cfg.master = desc->codec_master;
        ret = codec->ops->init(codec, &cfg);
        if (ret)
            return ret;
    }

    return ret;
}

static rt_err_t rk_audio_deinit(rt_device_t dev)
{
    struct audio_stream *as = dev->user_data;
    struct audio_card *card = as->card;
    struct audio_dai *dai = card->dai;
    struct audio_dai *vad_dai = card->vad_dai;
    struct audio_codec *codec = card->codec;
    rt_err_t ret = RT_EOK;

    if (vad_dai)
    {
        ret = vad_dai->ops->deinit(vad_dai);
        if (ret)
            return ret;
    }

    ret = dai->ops->deinit(dai);
    if (ret)
        return ret;

    if (codec)
    {
        ret = codec->ops->deinit(codec);
        if (ret)
            return ret;
    }

    return ret;
}

static rt_err_t rk_audio_set_gain(struct audio_stream *as,
                                  struct AUDIO_DB_CONFIG *db_config)
{
    struct audio_codec *codec = as->card->codec;
    rt_err_t ret = RT_EOK;

    if (codec && codec->ops->set_gain)
        ret = codec->ops->set_gain(codec, as->stream, db_config);

    return ret;
}

static rt_err_t rk_audio_get_gain(struct audio_stream *as,
                                  struct AUDIO_DB_CONFIG *db_config)
{
    struct audio_codec *codec = as->card->codec;
    rt_err_t ret = RT_EOK;

    if (codec && codec->ops->get_gain)
        ret = codec->ops->get_gain(codec, as->stream, db_config);

    return ret;
}

static rt_err_t rk_audio_get_gain_info(struct audio_stream *as, struct AUDIO_GAIN_INFO *gainInfo)
{
    struct audio_codec *codec = as->card->codec;
    rt_err_t ret = RT_EOK;

    if (codec && codec->ops->get_gaininfo)
        ret = codec->ops->get_gaininfo(codec, gainInfo);

    return ret;
}

/********************* Public Function Definition ***************************/

/**
 * @brief  Register a Dai into audio system.
 * @param  dai: the handle of dai.
 * @return rt_err_t
 */
rt_err_t rk_audio_register_dai(struct audio_dai *dai)
{
    struct audio_dai *idai;

    if (!dai)
        return -RT_EINVAL;

    rt_mutex_take(&dai_lock, RT_WAITING_FOREVER);
    rt_list_for_each_entry(idai, &s_audio_dai_list, list)
    {
        if (idai->id == dai->id)
        {
            rt_mutex_release(&dai_lock);
            return -RT_ERROR;
        }
    }

    rt_list_insert_after(&s_audio_dai_list, &dai->list);
    rt_mutex_release(&dai_lock);

    return RT_EOK;
}

/**
 * @brief  Unregister a Dai from audio system.
 * @param  dai: the handle of dai.
 * @return void
 */
void rk_audio_unregister_dai(struct audio_dai *dai)
{
    rt_mutex_take(&dai_lock, RT_WAITING_FOREVER);
    rt_list_remove(&dai->list);
    rt_mutex_release(&dai_lock);
}

/**
 * @brief  Register a Codec into audio system.
 * @param  codec: the handle of codec.
 * @return rt_err_t
 */
rt_err_t rk_audio_register_codec(struct audio_codec *codec)
{
    struct audio_codec *icodec;

    if (!codec)
        return -RT_EINVAL;

    rt_mutex_take(&codec_lock, RT_WAITING_FOREVER);
    rt_list_for_each_entry(icodec, &s_audio_codec_list, list)
    {
        if (icodec->id == codec->id)
        {
            rt_mutex_release(&codec_lock);
            return -RT_ERROR;
        }
    }

    rt_list_insert_after(&s_audio_codec_list, &codec->list);
    rt_mutex_release(&codec_lock);

    return RT_EOK;
}

/**
 * @brief  Unregister a Codec from audio system.
 * @param  codec: the handle of codec.
 * @return void
 */
void rk_audio_unregister_codec(struct audio_codec *codec)
{
    rt_mutex_take(&codec_lock, RT_WAITING_FOREVER);
    rt_list_remove(&codec->list);
    rt_mutex_release(&codec_lock);
}

/**
 * @brief  Find dai via the given id(dai controller phy base).
 * @param  id: dai id.
 * @return audio_dai
 */
struct audio_dai *rk_audio_find_dai(uint32_t id)
{
    struct audio_dai *dai = NULL;

    rt_mutex_take(&dai_lock, RT_WAITING_FOREVER);
    rt_list_for_each_entry(dai, &s_audio_dai_list, list)
    {
        if (dai->id == id)
        {
            rt_mutex_release(&dai_lock);
            return dai;
        }
    }
    rt_mutex_release(&dai_lock);

    return NULL;
}

/**
 * @brief  Find codec via the given id.
 * @param  id: codec id.
 * @return audio_codec
 */
struct audio_codec *rk_audio_find_codec(uint32_t id)
{
    struct audio_codec *codec = NULL;

    rt_mutex_take(&codec_lock, RT_WAITING_FOREVER);
    rt_list_for_each_entry(codec, &s_audio_codec_list, list)
    {
        if (codec->id == id)
        {
            rt_mutex_release(&codec_lock);
            return codec;
        }
    }
    rt_mutex_release(&codec_lock);

    return NULL;
}

struct audio_stream *rk_audio_new_stream(struct audio_card *card,
        eAUDIO_streamType stream)
{
    struct audio_stream *as = rt_calloc(1, sizeof(*as));
    rt_err_t ret;

    if (!as)
        return RT_NULL;

#ifdef RT_USING_MULTI_PCM
    if (card->desc.multi_dais)
        as->pcm = rk_audio_new_mpcm(card);
    else
#endif
        as->pcm = rk_audio_new_pcm(card);
    if (!as->pcm)
    {
        rt_free(as);
        return RT_NULL;
    }
    as->pcm->as = as;
    as->card = card;
    as->stream = stream;

    ret = rt_mutex_init(&as->lock, card->desc.name, RT_IPC_FLAG_FIFO);
    RT_ASSERT(ret == RT_EOK);

    return as;
}

void rk_audio_free_stream(struct audio_stream *as)
{
#ifdef RT_USING_MULTI_PCM
    if (as->card->desc.multi_dais)
        rk_audio_free_mpcm(as->pcm);
    else
#endif
        rk_audio_free_pcm(as->pcm);

    rt_free(as);
}

struct audio_card *rk_audio_new_card(const struct audio_card_desc *acd)
{
    struct audio_card *ac = rt_calloc(1, sizeof(*ac));
    eAUDIO_streamType astream;
    int i;

    if (!ac)
        return RT_NULL;

    ac->dai = rk_audio_find_dai((uint32_t)acd->dai);
    if (!ac->dai)
    {
        rt_kprintf("can't find dai: %p\n", acd->dai);
        goto err;
    }

    ac->dai->card = ac;
    ac->desc = *acd;

    if (acd->playback)
    {
        astream = AUDIO_STREAM_PLAYBACK;
        ac->stream[astream] = rk_audio_new_stream(ac, astream);
        if (!ac->stream[astream])
        {
            rt_kprintf("new playback stream failed\n");
            goto err_sub;
        }
    }

    if (acd->capture)
    {
        astream = AUDIO_STREAM_CAPTURE;
        ac->stream[astream] = rk_audio_new_stream(ac, astream);
        if (!ac->stream[astream])
        {
            rt_kprintf("new capture stream failed\n");
            goto err_sub;
        }
    }

    if (acd->vad)
    {
        ac->vad_dai = rk_audio_find_dai((uint32_t)acd->vad);
        if (!ac->vad_dai)
            goto err_sub;
        ac->vad_dai->card = ac;
    }

    if (acd->codec)
    {
        ac->codec = rk_audio_find_codec((uint32_t)acd->codec);
        if (!ac->codec)
        {
            rt_kprintf("can't find codec: %p\n", acd->codec);
            goto err_sub;
        }
        ac->codec->card = ac;
    }

    return ac;

err_sub:
    for (i = 0; i < AUDIO_STREAM_NUM; i++)
    {
        if (ac->stream[i])
            rk_audio_free_stream(ac->stream[i]);
    }

err:
    rt_free(ac);

    return RT_NULL;
}

void rk_audio_free_card(struct audio_card *card)
{
    int i;

    for (i = 0; i < AUDIO_STREAM_NUM; i++)
    {
        if (card->stream[i])
            rk_audio_free_stream(card->stream[i]);
    }

    rt_free(card);
}

void rk_audio_stream_update(struct audio_stream *as)
{
    struct audio_pcm *pcm = as->pcm;
    audio_pcm_uframes_t ptr = pcm->status.hw_ptr;
    audio_pcm_uframes_t avail;

    RT_ASSERT(pcm);

    ptr += pcm->abuf.period_size;
    if (ptr >= pcm->boundary)
        ptr -= pcm->boundary;

    pcm->status.hw_ptr = ptr;

    if (as->stream == AUDIO_STREAM_PLAYBACK)
        avail = audio_pcm_playback_avail(pcm);
    else
        avail = audio_pcm_capture_avail(pcm);

    if (avail >= pcm->abuf.buf_size)
    {
        rt_kprintf("[0x%08x] stream %d: xrun, avail: %lu!\n",
                   HAL_GetTick(), as->stream, avail);
        rk_audio_xrun(pcm->as);
    }
    else if (pcm->waiting)
    {
        rt_mutex_take(&as->lock, RT_WAITING_FOREVER);
        pcm->waiting = RT_FALSE;
        rt_mutex_release(&as->lock);
        rt_sem_release(pcm->wait);
    }
}

rt_err_t rk_audio_open(rt_device_t dev, rt_uint16_t oflag)
{
    if ((oflag & RT_DEVICE_OFLAG_RDONLY) && !(dev->flag & RT_DEVICE_FLAG_RDONLY))
        return -RT_EIO;

    if ((oflag & RT_DEVICE_OFLAG_WRONLY) && !(dev->flag & RT_DEVICE_FLAG_WRONLY))
        return -RT_EIO;

    return rk_audio_init(dev);
}

rt_err_t rk_audio_close(rt_device_t dev)
{
    return rk_audio_deinit(dev);
}

rt_size_t rk_audio_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
    struct audio_stream *as = dev->user_data;
    struct audio_pcm *pcm = as->pcm;
    audio_pcm_uframes_t avail, appl_ptr, appl_ofs;
    uint8_t *hwbuf;
    rt_err_t ret = RT_EOK;

    if (!size)
        return 0;

    switch (as->state)
    {
    case AUDIO_STREAM_STATE_PREPARED:
    case AUDIO_STREAM_STATE_XRUN:
        rk_audio_start(as);
        break;
    case AUDIO_STREAM_STATE_RUNNING:
        break;
    default:
        ret = -RT_ERROR;
        goto err;
    }

    avail = audio_pcm_capture_avail(pcm);
    RK_AUDIO_DBG("[0x%08x] %s: %ld frames, %ld bytes, avail: %ld, sem: %d\n",
                 HAL_GetTick(), __func__, size, frames_to_bytes(pcm, size),
                 avail, pcm->wait->value);

    if (avail < size)
    {
        ret = audio_wait_for_avail(as);
        if (ret != RT_EOK)
            goto err;
    }

    appl_ptr = pcm->status.appl_ptr;
    appl_ofs = appl_ptr % pcm->abuf.buf_size;

    hwbuf = pcm->abuf.buf + frames_to_bytes(pcm, appl_ofs);

    rt_memcpy(buffer, hwbuf, frames_to_bytes(pcm, size));

    appl_ptr += size;
    if (appl_ptr >= pcm->boundary)
        appl_ptr -= pcm->boundary;
    pcm->status.appl_ptr = appl_ptr;

    return size;
err:
    rt_set_errno(ret);
    return 0;
}

rt_size_t rk_audio_write(rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size)
{
    struct audio_stream *as = dev->user_data;
    struct audio_pcm *pcm = as->pcm;
    audio_pcm_uframes_t avail, appl_ptr, appl_ofs;
    uint8_t *hwbuf;
    rt_err_t ret = RT_EOK;

    if (!size)
        return 0;

    avail = audio_pcm_playback_avail(pcm);
    RK_AUDIO_DBG("[0x%08x] %s: %ld frames, %ld bytes, avail: %ld, sem: %d\n",
                 HAL_GetTick(), __func__, size, frames_to_bytes(pcm, size),
                 avail, pcm->wait->value);

    if (avail < size)
    {
        ret = audio_wait_for_avail(as);
        if (ret != RT_EOK)
            goto err;
    }

    appl_ptr = pcm->status.appl_ptr;
    appl_ofs = appl_ptr % pcm->abuf.buf_size;

    hwbuf = pcm->abuf.buf + frames_to_bytes(pcm, appl_ofs);

    rt_memcpy(hwbuf, buffer, frames_to_bytes(pcm, size));

    appl_ptr += size;
    if (appl_ptr >= pcm->boundary)
        appl_ptr -= pcm->boundary;
    pcm->status.appl_ptr = appl_ptr;

    switch (as->state)
    {
    case AUDIO_STREAM_STATE_PREPARED:
    case AUDIO_STREAM_STATE_XRUN:
        rk_audio_start(as);
        break;
    case AUDIO_STREAM_STATE_RUNNING:
        break;
    default:
        ret = -RT_ERROR;
        goto err;
    }

    return size;

err:
    rt_set_errno(ret);
    return 0;
}

rt_err_t rk_audio_control(rt_device_t dev, int cmd, void *args)
{
    struct audio_stream *as = dev->user_data;
    rt_err_t ret = RT_EOK;

    switch (cmd)
    {
    case RK_AUDIO_CTL_HW_PARAMS:
        ret = rk_audio_config(as, (struct AUDIO_PARAMS *)args);
        break;
    case RK_AUDIO_CTL_START:
        ret = rk_audio_start(as);
        break;
    case RK_AUDIO_CTL_STOP:
        ret = rk_audio_stop(as);
        break;
    case RK_AUDIO_CTL_SET_GAIN:
        ret = rk_audio_set_gain(as, args);
        break;
    case RK_AUDIO_CTL_GET_GAIN:
        ret = rk_audio_get_gain(as, args);
        break;
    case RK_AUDIO_CTL_GET_GAIN_INFO:
        ret = rk_audio_get_gain_info(as, args);
        break;
    case RK_AUDIO_CTL_PCM_PREPARE:
        ret = as->pcm->ops->init(as->pcm, as->stream, (struct audio_buf *)args);
        break;
    case RK_AUDIO_CTL_PCM_RELEASE:
        ret = as->pcm->ops->deinit(as->pcm);
        break;
    default:
        ret = -RT_ERROR;
#ifdef RT_USING_DRIVER_AUDIO_PCM_PLUGIN
        if (cmd >= RK_AUDIO_CTL_PLUGIN_FIRST &&
                cmd <= RK_AUDIO_CTL_PLUGIN_LAST)
            ret = snd_pcm_plugin_controls(as->pcm, cmd, args);
#endif
        break;
    }

    return ret;
}

#ifdef RT_USING_DRIVER_AUDIO_PCM_PLUGIN
static rt_size_t rk_audio_plugin_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
    struct audio_stream *as = dev->user_data;
    struct audio_pcm *pcm = as->pcm;

    return snd_pcm_plugin_read(pcm, buffer, size);
}

static rt_size_t rk_audio_plugin_write(rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size)
{
    struct audio_stream *as = dev->user_data;
    struct audio_pcm *pcm = as->pcm;

    return snd_pcm_plugin_write(pcm, buffer, size);
}

rt_err_t rk_audio_hw_start(struct audio_stream *as)
{
    return rk_audio_start(as);
}

rt_err_t rk_audio_hw_stop(struct audio_stream *as)
{
    return rk_audio_stop(as);
}

rt_err_t rk_audio_hw_wait_for_avail(struct audio_stream *as)
{
    return audio_wait_for_avail(as);
}
#endif

#ifdef RT_USING_DEVICE_OPS
static const struct rt_device_ops rk_audio_ops =
{
    .open = rk_audio_open,
    .close = rk_audio_close,
#ifdef RT_USING_DRIVER_AUDIO_PCM_PLUGIN
    .read = rk_audio_plugin_read,
    .write = rk_audio_plugin_write,
#else
    .read = rk_audio_read,
    .write = rk_audio_write,
#endif
    .control = rk_audio_control,
};
#endif

rt_err_t rk_audio_register(struct audio_card *card, const char *name)
{
    struct audio_stream *as;
    char pcm_name[RT_NAME_MAX];
    rt_err_t ret = -RT_ERROR;
    int i;

    RT_ASSERT(card);

    for (i = 0; i < AUDIO_STREAM_NUM; i++)
    {
        as = card->stream[i];
        if (as)
        {
            bool is_play = (as->stream == AUDIO_STREAM_PLAYBACK);
            char suffix = is_play ? 'p' : 'c';
            rt_uint32_t sflag = is_play ? RT_DEVICE_FLAG_WRONLY : RT_DEVICE_FLAG_RDONLY;

            as->dev.type = RT_Device_Class_Sound;
#ifdef RT_USING_DEVICE_OPS
            as->dev.ops = &rk_audio_ops;
#else
            as->dev.init = rk_audio_init;
            as->dev.open = rk_audio_open;
            as->dev.close = rk_audio_close;
#ifdef RT_USING_DRIVER_AUDIO_PCM_PLUGIN
            as->dev.read = rk_audio_plugin_read;
            as->dev.write = rk_audio_plugin_write;
#else
            as->dev.read = rk_audio_read;
            as->dev.write = rk_audio_write;
#endif /* RT_USING_DRIVER_AUDIO_PCM_PLUGIN */
            as->dev.control = rk_audio_control;
#endif /* RT_USING_DEVICE_OPS */
            as->dev.user_data = as;
            rt_snprintf(pcm_name, RT_NAME_MAX, "%s%c", name, suffix);
            ret = rt_device_register(&as->dev, pcm_name, sflag);
        }
    }

    return ret;
}

rt_err_t rk_audio_register_by_card_desc(const struct audio_card_desc *desc)
{
    struct audio_card *card;
    rt_err_t ret;

    card = rk_audio_new_card(desc);
    if (!card)
        return -RT_ERROR;

    ret = rk_audio_register(card, desc->name);
    if (ret != RT_EOK)
        rk_audio_free_card(card);

    return ret;
}

int rk_audio_core_init(void)
{
    rt_mutex_init(&dai_lock, "dai_lock", RT_IPC_FLAG_FIFO);
    rt_mutex_init(&codec_lock, "codec_lock", RT_IPC_FLAG_FIFO);

    return 0;
}

INIT_PREV_EXPORT(rk_audio_core_init);
#endif
