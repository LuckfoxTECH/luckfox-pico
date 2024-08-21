/**
  * Copyright (c) 2023 Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"

#ifdef RT_USING_AUPIPE_ECNR

#include "rtdef.h"
#include "cJSON.h"
#include "rtdebug.h"

#define RT_LOGD(format, ...)                            \
do {                                                    \
    fprintf(stderr, "[RKSKVD]:" format, ##__VA_ARGS__); \
    fprintf(stderr, "\n");                              \
} while (0)

#define RT_LOG(format, ...)                            \
do {                                                   \
    fprintf(stderr, "[RKSKV]:" format, ##__VA_ARGS__); \
} while (0)

static SkvAecParam *parseSkvAecConfigs(void *parser, const char *group, int *model)
{
    const char *node = "aec";
    cJSON *child0, *child1;
    char *val_str;
    double val_dbl;

    if ((parser == RT_NULL) || (group == RT_NULL) || (model == RT_NULL))
        return RT_NULL;

    SkvAecParam *aec = (SkvAecParam *)rt_malloc(sizeof(SkvAecParam));
    RT_ASSERT(aec != RT_NULL);

    RT_LOG("==== JSON AEC Params ====\n");

    /* skv_configs/ */
    child0 = cJSON_GetObjectItem((const cJSON *)parser, group);
    RT_ASSERT(child0 != RT_NULL);

    /* skv_configs/aec */
    child0 = cJSON_GetObjectItem(child0, node);
    RT_ASSERT(child0 != RT_NULL);

    /* skv_configs/aec/status */
    child1 = cJSON_GetObjectItem(child0, "status");
    if (child1)
    {
        val_str = cJSON_GetStringValue(child1);
        RT_ASSERT(val_str != RT_NULL);

        RT_LOG("status: %s\n", val_str);

        if (!strcasecmp("enable", val_str))
        {
            *model |= RT_SKV_AEC;
        }
    }
    else
    {
        RT_LOG("NOT FILL status\n");
    }

    /* skv_configs/aec/pos */
    child1 = cJSON_GetObjectItem(child0, "pos");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        aec->pos = (int)val_dbl;
        RT_LOG("pos: %d, %f\n", aec->pos, val_dbl);
    }
    else
    {
        RT_LOG("NOT FILL pos\n");
    }

    /* skv_configs/aec/drop_ref_channel */
    child1 = cJSON_GetObjectItem(child0, "drop_ref_channel");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        aec->drop_ref_channel = (int)val_dbl;
        RT_LOG("drop_ref_channel: %d, %f\n", aec->drop_ref_channel, val_dbl);
    }
    else
    {
        RT_LOG("NOT FILL drop_ref_channel\n");
    }

    /* skv_configs/aec/aec_mode_en */
    child1 = cJSON_GetObjectItem(child0, "aec_mode_en");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        aec->aec_mode_en = (int)val_dbl;
        RT_LOG("aec_mode_en: %d, %f\n", aec->aec_mode_en, val_dbl);
    }
    else
    {
        RT_LOG("NOT FILL aec_mode_en\n");
    }

    /* skv_configs/aec/delay_len */
    child1 = cJSON_GetObjectItem(child0, "delay_len");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        aec->delay_len = (int)val_dbl;
        RT_LOG("delay_len: %d, %f\n", aec->delay_len, val_dbl);
    }
    else
    {
        RT_LOG("NOT FILL delay_len\n");
    }

    /* skv_configs/aec/look_ahead */
    child1 = cJSON_GetObjectItem(child0, "look_ahead");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        aec->look_ahead = (int)val_dbl;
        RT_LOG("look_ahead: %d, %f\n", aec->look_ahead, val_dbl);
    }
    else
    {
        RT_LOG("NOT FILL look_ahead\n");
    }

    /* skv_configs/aec/mic_chn_map */
    child1 = cJSON_GetObjectItem(child0, "mic_chn_map");
    if (child1)
    {
        int temp[16];
        int chns;

        val_str = cJSON_GetStringValue(child1);
        RT_ASSERT(val_str != RT_NULL);
        RT_LOG("mic_chn_map: %s\n", val_str);

        sscanf(val_str, "%d chn;map:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", &chns,
               &temp[0], &temp[1], &temp[2], &temp[3], &temp[4], &temp[5], &temp[6], &temp[7],
               &temp[8], &temp[9], &temp[10], &temp[11], &temp[12], &temp[13], &temp[14], &temp[15]);

        if (chns > 0)
        {
            RT_ASSERT(chns <= 16);
            aec->aec_mode_en |= AEC_EN_CHN_MAP;
            aec->mic_chns_map = (short *)rt_malloc(chns * sizeof(short));
            RT_ASSERT(aec->mic_chns_map != RT_NULL);

            // RT_LOG("mic_chn_map:%s", val_str);
            for (int i = 0 ; i < chns; i++)
            {
                aec->mic_chns_map[i] = temp[i];
                // RT_LOG("mic_chn_map[%d]: %d", i, aec->mic_chns_map[i]);
            }
        }
    }
    else
    {
        RT_LOG("NOT FILL mic_chn_map\n");
    }

    return aec;
}

static SkvBeamFormParam *parseSkvBfConfigs(void *parser, const char *group, int *model)
{
    const char *node = "bf";
    cJSON *child0, *child1;
    char *val_str;
    double val_dbl;

    if ((parser == RT_NULL) || (group == RT_NULL) || (model == RT_NULL))
        return RT_NULL;

    SkvBeamFormParam *bf = (SkvBeamFormParam *)rt_malloc(sizeof(SkvBeamFormParam));
    RT_ASSERT(bf != RT_NULL);
    rt_memset(bf, 0, sizeof(SkvBeamFormParam));

    RT_LOG("==== JSON Beamform Params ====\n");

    /* skv_configs/ */
    child0 = cJSON_GetObjectItem((const cJSON *)parser, group);
    RT_ASSERT(child0 != RT_NULL);

    /* skv_configs/bf */
    child0 = cJSON_GetObjectItem(child0, node);
    RT_ASSERT(child0 != RT_NULL);

    /* skv_configs/bf/status */
    child1 = cJSON_GetObjectItem(child0, "status");
    if (child1)
    {
        val_str = cJSON_GetStringValue(child1);
        RT_ASSERT(val_str != RT_NULL);

        RT_LOG("status: %s\n", val_str);

        if (!strcasecmp("enable", val_str))
        {
            *model |= RT_SKV_BF;
        }
    }
    else
    {
        RT_LOG("NOT FILL status\n");
    }

    /* skv_configs/bf/drop_ref_channel */
    child1 = cJSON_GetObjectItem(child0, "drop_ref_channel");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        bf->drop_ref_channel = (int)val_dbl;
        RT_LOG("drop_ref_channel: %d, %f\n", bf->drop_ref_channel, val_dbl);
    }
    else
    {
        RT_LOG("NOT FILL drop_ref_channel\n");
    }

    /* skv_configs/bf/num_ref_channel */
    child1 = cJSON_GetObjectItem(child0, "num_ref_channel");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        bf->num_ref_channel = (int)val_dbl;
        RT_LOG("num_ref_channel: %d, %f\n", bf->num_ref_channel, val_dbl);
    }
    else
    {
        RT_LOG("NOT FILL num_ref_channel\n");
    }

    /* skv_configs/bf/ref_pos */
    child1 = cJSON_GetObjectItem(child0, "ref_pos");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        bf->ref_pos = (int)val_dbl;
        RT_LOG("ref_pos: %d, %f\n", bf->ref_pos, val_dbl);
    }
    else
    {
        RT_LOG("NOT FILL ref_pos\n");
    }

    /* skv_configs/bf/targ */
    child1 = cJSON_GetObjectItem(child0, "targ");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        bf->targ = (int)val_dbl;
        RT_LOG("targ: %d, %f\n", bf->targ, val_dbl);
    }
    else
    {
        RT_LOG("NOT FILL targ\n");
    }

    return bf;
}

static void parseSkvFastAecConfigs(void *parser, const char *group, int *mode)
{
    const char *node = "fast_aec";
    cJSON *child0, *child1;
    char *val_str;

    if ((parser == RT_NULL) || (group == RT_NULL) || (mode == RT_NULL))
        return;

    RT_LOG("==== JSON FAST_AEC Params ====\n");

    /* skv_configs/ */
    child0 = cJSON_GetObjectItem((const cJSON *)parser, group);
    RT_ASSERT(child0 != RT_NULL);

    /* skv_configs/fast_aec */
    child0 = cJSON_GetObjectItem(child0, node);
    RT_ASSERT(child0 != RT_NULL);

    /* skv_configs/fast_aec/status */
    child1 = cJSON_GetObjectItem(child0, "status");
    if (child1)
    {
        val_str = cJSON_GetStringValue(child1);
        RT_ASSERT(val_str != RT_NULL);

        RT_LOG("status: %s\n", val_str);

        if (!strcasecmp("enable", val_str))
        {
            *mode |= BF_EN_FAST_AEC;
        }
    }
    else
    {
        RT_LOG("NOT FILL status\n");
    }
}

static void parseSkvWakeUpConfigs(void *parser, const char *group, int *mode)
{
    const char *node = "wakeup";
    cJSON *child0, *child1;
    char *val_str;

    if ((parser == RT_NULL) || (group == RT_NULL) || (mode == RT_NULL))
        return;

    RT_LOG("==== JSON WAKEUP Params ====\n");

    /* skv_configs/ */
    child0 = cJSON_GetObjectItem((const cJSON *)parser, group);
    RT_ASSERT(child0 != RT_NULL);

    /* skv_configs/fast_aec */
    child0 = cJSON_GetObjectItem(child0, node);
    RT_ASSERT(child0 != RT_NULL);

    /* skv_configs/fast_aec/status */
    child1 = cJSON_GetObjectItem(child0, "status");
    if (child1)
    {
        val_str = cJSON_GetStringValue(child1);
        RT_ASSERT(val_str != RT_NULL);

        RT_LOG("status: %s\n", val_str);

        if (!strcasecmp("enable", val_str))
        {
            *mode |= BF_EN_WAKEUP;
        }
    }
    else
    {
        RT_LOG("NOT FILL status\n");
    }
}

static SkvAesParam *parseSkvAesConfigs(void *parser, const char *group, int *mode)
{
    const char *node = "aes";
    cJSON *child0, *child1;
    char *val_str;
    double val_dbl;

    if ((parser == RT_NULL) || (group == RT_NULL) || (mode == RT_NULL))
        return RT_NULL;

    SkvAesParam *aes = (SkvAesParam *)rt_malloc(sizeof(SkvAesParam));
    RT_ASSERT(aes != RT_NULL);

    RT_LOG("==== JSON AES Params ====\n");

    /* skv_configs/ */
    child0 = cJSON_GetObjectItem((const cJSON *)parser, group);
    RT_ASSERT(child0 != RT_NULL);

    /* skv_configs/aes */
    child0 = cJSON_GetObjectItem(child0, node);
    RT_ASSERT(child0 != RT_NULL);

    /* skv_configs/aes/status */
    child1 = cJSON_GetObjectItem(child0, "status");
    if (child1)
    {
        val_str = cJSON_GetStringValue(child1);
        RT_ASSERT(val_str != RT_NULL);

        RT_LOG("status: %s\n", val_str);

        if (!strcasecmp("enable", val_str))
        {
            *mode |= BF_EN_AES;
            RT_LOG("AES: enable\n");
        }
    }
    else
    {
        RT_LOG("NOT FILL status\n");
    }

    /* skv_configs/aes/beta_up */
    child1 = cJSON_GetObjectItem(child0, "beta_up");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        aes->beta_up = val_dbl;
        RT_LOG("AES: beta_up=%f\n", (double)aes->beta_up);
    }
    else
    {
        RT_LOG("NOT FILL beta_up\n");
    }

    /* skv_configs/aes/beta_down */
    child1 = cJSON_GetObjectItem(child0, "beta_down");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        aes->beta_down = val_dbl;
        RT_LOG("AES: beta_down=%f\n", (double)aes->beta_down);
    }
    else
    {
        RT_LOG("NOT FILL beta_down\n");
    }

    return aes;
}

static void parseSkvGscConfigs(void *parser, const char *group, int *mode)
{
    const char *node = "gsc";
    cJSON *child0, *child1;
    char *val_str;
    double val_dbl;

    if ((parser == RT_NULL) || (group == RT_NULL) || (mode == RT_NULL))
        return;

    RT_LOG("==== JSON GSC Params ====\n");

    /* skv_configs/ */
    child0 = cJSON_GetObjectItem((const cJSON *)parser, group);
    RT_ASSERT(child0 != RT_NULL);

    /* skv_configs/gsc */
    child0 = cJSON_GetObjectItem(child0, node);
    RT_ASSERT(child0 != RT_NULL);

    /* skv_configs/gsc/status */
    child1 = cJSON_GetObjectItem(child0, "status");
    if (child1)
    {
        val_str = cJSON_GetStringValue(child1);
        RT_ASSERT(val_str != RT_NULL);

        RT_LOG("status: %s\n", val_str);

        if (!strcasecmp("enable", val_str))
        {
            *mode |= BF_EN_GSC;
            RT_LOG("GSC: enable\n");
        }
    }
    else
    {
        RT_LOG("NOT FILL status\n");
    }

    /* skv_configs/gsc/look_ahead */
    child1 = cJSON_GetObjectItem(child0, "method");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        if ((int)val_dbl >= 1)
        {
            *mode |= BF_GSC_METHOD;
            RT_LOG("method: BF_GSC_METHOD\n");
        }
    }
    else
    {
        RT_LOG("NOT FILL method\n");
    }
}

static SkvAgcParam *parseSkvAgcConfigs(void *parser, const char *group, int *mode)
{
    const char *node = "agc";
    cJSON *child0, *child1;
    char *val_str;
    double val_dbl;

    if ((parser == RT_NULL) || (group == RT_NULL) || (mode == RT_NULL))
        return RT_NULL;

    SkvAgcParam *agc = (SkvAgcParam *)rt_malloc(sizeof(SkvAgcParam));
    RT_ASSERT(agc != RT_NULL);

    RT_LOG("==== JSON AGC Params ====\n");

    /* skv_configs/ */
    child0 = cJSON_GetObjectItem((const cJSON *)parser, group);
    RT_ASSERT(child0 != RT_NULL);

    /* skv_configs/agc */
    child0 = cJSON_GetObjectItem(child0, node);
    RT_ASSERT(child0 != RT_NULL);

    /* skv_configs/agc/status */
    child1 = cJSON_GetObjectItem(child0, "status");
    if (child1)
    {
        val_str = cJSON_GetStringValue(child1);
        RT_ASSERT(val_str != RT_NULL);

        RT_LOG("status: %s\n", val_str);

        if (!strcasecmp("enable", val_str))
        {
            *mode |= BF_EN_AGC;
        }
    }
    else
    {
        RT_LOG("NOT FILL status\n");
    }

    /* skv_configs/agc/attack_time */
    child1 = cJSON_GetObjectItem(child0, "attack_time");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        agc->attack_time = val_dbl;
        RT_LOG("AGC: attack_time=%f\n", (double)agc->attack_time);
    }
    else
    {
        RT_LOG("NOT FILL attack_time\n");
    }

    /* skv_configs/agc/release_time */
    child1 = cJSON_GetObjectItem(child0, "release_time");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        agc->release_time = val_dbl;
        RT_LOG("AGC: release_time=%f\n", (double)agc->release_time);
    }
    else
    {
        RT_LOG("NOT FILL release_time\n");
    }

    /* skv_configs/agc/attenuate_time */
    child1 = cJSON_GetObjectItem(child0, "attenuate_time");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        agc->attenuate_time = val_dbl;
        RT_LOG("AGC: attenuate_time=%f\n", (double)agc->attenuate_time);
    }
    else
    {
        RT_LOG("NOT FILL attenuate_time\n");
    }

    /* skv_configs/agc/max_gain */
    child1 = cJSON_GetObjectItem(child0, "max_gain");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        agc->max_gain = val_dbl;
        RT_LOG("AGC: max_gain=%f\n", (double)agc->max_gain);
    }
    else
    {
        RT_LOG("NOT FILL max_gain\n");
    }

    /* skv_configs/agc/max_peak */
    child1 = cJSON_GetObjectItem(child0, "max_peak");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        agc->max_peak = val_dbl;
        RT_LOG("AGC: max_peak=%f\n", (double)agc->max_peak);
    }
    else
    {
        RT_LOG("NOT FILL max_peak\n");
    }

    /* skv_configs/agc/fRth0 */
    child1 = cJSON_GetObjectItem(child0, "fRth0");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        agc->fRth0 = val_dbl;
        RT_LOG("AGC: fRth0=%f\n", (double)agc->fRth0);
    }
    else
    {
        RT_LOG("NOT FILL fRth0\n");
    }

    /* skv_configs/agc/fRth1 */
    child1 = cJSON_GetObjectItem(child0, "fRth1");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        agc->fRth1 = val_dbl;
        RT_LOG("AGC: fRth1=%f\n", (double)agc->fRth1);
    }
    else
    {
        RT_LOG("NOT FILL fRth1\n");
    }

    /* skv_configs/agc/fRth2 */
    child1 = cJSON_GetObjectItem(child0, "fRth2");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        agc->fRth2 = val_dbl;
        RT_LOG("AGC: fRth2=%f\n", (double)agc->fRth2);
    }
    else
    {
        RT_LOG("NOT FILL fRth2\n");
    }

    /* skv_configs/agc/fRk0 */
    child1 = cJSON_GetObjectItem(child0, "fRk0");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        agc->fRk0 = val_dbl;
        RT_LOG("AGC: fRk0=%f\n", (double)agc->fRk0);
    }
    else
    {
        RT_LOG("NOT FILL fRk0\n");
    }

    /* skv_configs/agc/fRk1 */
    child1 = cJSON_GetObjectItem(child0, "fRk1");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        agc->fRk1 = val_dbl;
        RT_LOG("AGC: fRk1=%f\n", (double)agc->fRk1);
    }
    else
    {
        RT_LOG("NOT FILL fRk1\n");
    }

    /* skv_configs/agc/fRk2 */
    child1 = cJSON_GetObjectItem(child0, "fRk2");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        agc->fRk2 = val_dbl;
        RT_LOG("AGC: fRk2=%f\n", (double)agc->fRk2);
    }
    else
    {
        RT_LOG("NOT FILL fRk2\n");
    }

    /* skv_configs/agc/fLineGainDb */
    child1 = cJSON_GetObjectItem(child0, "fLineGainDb");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        agc->fLineGainDb = val_dbl;
        RT_LOG("AGC: fLineGainDb=%f\n", (double)agc->fLineGainDb);
    }
    else
    {
        RT_LOG("NOT FILL fLineGainDb\n");
    }

    /* skv_configs/agc/swSmL0 */
    child1 = cJSON_GetObjectItem(child0, "swSmL0");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        agc->swSmL0 = val_dbl;
        RT_LOG("AGC: swSmL0=%f\n", (double)agc->swSmL0);
    }
    else
    {
        RT_LOG("NOT FILL swSmL0\n");
    }

    /* skv_configs/agc/swSmL1 */
    child1 = cJSON_GetObjectItem(child0, "swSmL1");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        agc->swSmL1 = val_dbl;
        RT_LOG("AGC: swSmL1=%f\n", (double)agc->swSmL1);
    }
    else
    {
        RT_LOG("NOT FILL swSmL1\n");
    }

    /* skv_configs/agc/swSmL2 */
    child1 = cJSON_GetObjectItem(child0, "swSmL2");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        agc->swSmL2 = val_dbl;
        RT_LOG("AGC: swSmL2=%f\n", (double)agc->swSmL2);
    }
    else
    {
        RT_LOG("NOT FILL swSmL2\n");
    }

    return agc;
}

static SkvAnrParam *parseSkvAnrConfigs(void *parser, const char *group, int *mode)
{
    const char *node = "anr";
    cJSON *child0, *child1;
    char *val_str;
    double val_dbl;

    if ((parser == RT_NULL) || (group == RT_NULL) || (mode == RT_NULL))
        return RT_NULL;

    SkvAnrParam *anr = (SkvAnrParam *)rt_malloc(sizeof(SkvAnrParam));
    RT_ASSERT(anr != RT_NULL);

    RT_LOG("==== JSON ANR Params ====\n");

    /* skv_configs/ */
    child0 = cJSON_GetObjectItem((const cJSON *)parser, group);
    RT_ASSERT(child0 != RT_NULL);

    /* skv_configs/anr */
    child0 = cJSON_GetObjectItem(child0, node);
    RT_ASSERT(child0 != RT_NULL);

    /* skv_configs/anr/status */
    child1 = cJSON_GetObjectItem(child0, "status");
    if (child1)
    {
        val_str = cJSON_GetStringValue(child1);
        RT_ASSERT(val_str != RT_NULL);

        RT_LOG("status: %s\n", val_str);

        if (!strcasecmp("enable", val_str))
        {
            *mode |= BF_EN_ANR;
        }
    }
    else
    {
        RT_LOG("NOT FILL status\n");
    }

    /* skv_configs/anr/noiseFactor */
    child1 = cJSON_GetObjectItem(child0, "noiseFactor");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        anr->noiseFactor = val_dbl;
        RT_LOG("ANR: noiseFactor=%f\n", (double)anr->noiseFactor);
    }
    else
    {
        RT_LOG("NOT FILL noiseFactor\n");
    }

    /* skv_configs/anr/psiMin */
    child1 = cJSON_GetObjectItem(child0, "psiMin");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        anr->psiMin = val_dbl;
        RT_LOG("ANR: psiMin=%f\n", (double)anr->psiMin);
    }
    else
    {
        RT_LOG("NOT FILL psiMin\n");
    }

    /* skv_configs/anr/psiMax */
    child1 = cJSON_GetObjectItem(child0, "psiMax");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        anr->psiMax = val_dbl;
        RT_LOG("ANR: psiMax=%f\n", (double)anr->psiMax);
    }
    else
    {
        RT_LOG("NOT FILL psiMax\n");
    }

    /* skv_configs/anr/fGmin */
    child1 = cJSON_GetObjectItem(child0, "fGmin");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        anr->fGmin = val_dbl;
        RT_LOG("ANR: fGmin=%f\n", (double)anr->fGmin);
    }
    else
    {
        RT_LOG("NOT FILL fGmin\n");
    }

    /* skv_configs/anr/swU */
    child1 = cJSON_GetObjectItem(child0, "swU");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        anr->swU = val_dbl;
        RT_LOG("ANR: swU=%f\n", (double)anr->swU);
    }
    else
    {
        RT_LOG("NOT FILL swU\n");
    }

    return anr;
}

static SkvNlpParam *parseSkvNlpConfigs(void *parser, const char *group, int *mode)
{
    const char *node = "nlp";
    cJSON *child0, *child1;
    char *val_str;

    if ((parser == RT_NULL) || (group == RT_NULL) || (mode == RT_NULL))
        return RT_NULL;

    SkvNlpParam *nlp = (SkvNlpParam *)rt_malloc(sizeof(SkvNlpParam));
    RT_ASSERT(nlp != RT_NULL);

    RT_LOG("==== JSON NLP Params ====\n");

    /* skv_configs/ */
    child0 = cJSON_GetObjectItem((const cJSON *)parser, group);
    RT_ASSERT(child0 != RT_NULL);

    /* skv_configs/nlp */
    child0 = cJSON_GetObjectItem(child0, node);
    RT_ASSERT(child0 != RT_NULL);

    /* skv_configs/nlp/status */
    child1 = cJSON_GetObjectItem(child0, "status");
    if (child1)
    {
        val_str = cJSON_GetStringValue(child1);
        RT_ASSERT(val_str != RT_NULL);

        RT_LOG("status: %s\n", val_str);

        if (!strcasecmp("enable", val_str))
        {
            *mode |= BF_EN_NLP;
        }
    }
    else
    {
        RT_LOG("NOT FILL status\n");
    }

    /* skv_configs/nlp/band_pass_thd */
    child1 = cJSON_GetObjectItem(child0, "band_pass_thd");
    if (child1)
    {
        int temp[8];

        val_str = cJSON_GetStringValue(child1);
        RT_ASSERT(val_str != RT_NULL);

        RT_LOG("mic_chn_map: %s\n", val_str);

        sscanf(val_str, "%d,%d,%d,%d,%d,%d,%d,%d", &temp[0], &temp[1], &temp[2], &temp[3],
               &temp[4], &temp[5], &temp[6], &temp[7]);
        for (int i = 0 ; i < 8; i++)
        {
            nlp->nlp16k[i][0] = temp[i];
            RT_LOG("nlp16k[%d][0]: %d\n", i, nlp->nlp16k[i][0]);
        }
    }
    else
    {
        RT_LOG("NOT FILL band_pass_thd\n");
    }

    /* skv_configs/nlp/super_est_factor */
    child1 = cJSON_GetObjectItem(child0, "super_est_factor");
    if (child1)
    {
        int temp[8];
        val_str = cJSON_GetStringValue(child1);
        RT_ASSERT(val_str != RT_NULL);

        RT_LOG("mic_chn_map: %s\n", val_str);

        sscanf(val_str, "%d,%d,%d,%d,%d,%d,%d,%d", &temp[0], &temp[1], &temp[2], &temp[3],
               &temp[4], &temp[5], &temp[6], &temp[7]);

        for (int i = 0 ; i < 8; i++)
        {
            nlp->nlp16k[i][1] = temp[i];
            RT_LOG("nlp16k[%d][1]: %d\n", i, nlp->nlp16k[i][1]);
        }
    }
    else
    {
        RT_LOG("NOT FILL super_est_factor\n");
    }

    return nlp;
}

static SkvDereverbParam *parseSkvDereverbConfigs(void *parser, const char *group, int *mode)
{
    const char *node = "dereverb";
    cJSON *child0, *child1;
    char *val_str;
    double val_dbl;

    if ((parser == RT_NULL) || (group == RT_NULL) || (mode == RT_NULL))
        return RT_NULL;

    SkvDereverbParam *dereverb = (SkvDereverbParam *)rt_malloc(sizeof(SkvDereverbParam));
    RT_ASSERT(dereverb != RT_NULL);

    RT_LOG("==== JSON Dereverb Params ====\n");

    /* skv_configs */
    child0 = cJSON_GetObjectItem((const cJSON *)parser, group);
    RT_ASSERT(child0 != RT_NULL);

    /* skv_configs/dereverb */
    child0 = cJSON_GetObjectItem(child0, node);
    RT_ASSERT(child0 != RT_NULL);

    /* skv_configs/dereverb/status */
    child1 = cJSON_GetObjectItem(child0, "status");
    if (child1)
    {
        val_str = cJSON_GetStringValue(child1);
        RT_ASSERT(val_str != RT_NULL);

        RT_LOG("status: %s\n", val_str);

        if (!strcasecmp("enable", val_str))
        {
            *mode |= BF_EN_DEREV;
        }
    }
    else
    {
        RT_LOG("NOT FILL status\n");
    }

    /* skv_configs/dereverb/rlsLg */
    child1 = cJSON_GetObjectItem(child0, "rlsLg");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        dereverb->rlsLg = val_dbl;
        RT_LOG("Dereverb: rlsLg=%f\n", (double)dereverb->rlsLg);
    }
    else
    {
        RT_LOG("NOT FILL rlsLg\n");
    }

    /* skv_configs/dereverb/curveLg */
    child1 = cJSON_GetObjectItem(child0, "curveLg");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        dereverb->curveLg = val_dbl;
        RT_LOG("Dereverb: curveLg=%f\n", (double)dereverb->curveLg);
    }
    else
    {
        RT_LOG("NOT FILL curveLg\n");
    }

    /* skv_configs/dereverb/delay */
    child1 = cJSON_GetObjectItem(child0, "delay");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        dereverb->delay = val_dbl;
        RT_LOG("Dereverb: delay=%f\n", (double)dereverb->delay);
    }
    else
    {
        RT_LOG("NOT FILL delay\n");
    }

    /* skv_configs/dereverb/forgetting */
    child1 = cJSON_GetObjectItem(child0, "forgetting");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        dereverb->forgetting = val_dbl;
        RT_LOG("Dereverb: forgetting=%f\n", (double)dereverb->forgetting);
    }
    else
    {
        RT_LOG("NOT FILL forgetting\n");
    }

    /* skv_configs/dereverb/t60 */
    child1 = cJSON_GetObjectItem(child0, "t60");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        dereverb->t60 = val_dbl;
        RT_LOG("Dereverb: t60=%f\n", (double)dereverb->t60);
    }
    else
    {
        RT_LOG("NOT FILL t60\n");
    }

    /* skv_configs/dereverb/coCoeff */
    child1 = cJSON_GetObjectItem(child0, "coCoeff");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        dereverb->coCoeff = val_dbl;
        RT_LOG("Dereverb: coCoeff=%f\n", (double)dereverb->coCoeff);
    }
    else
    {
        RT_LOG("NOT FILL coCoeff\n");
    }

    return dereverb;
}

static SkvCngParam *parseSkvCngConfigs(void *parser, const char *group, int *mode)
{
    const char *node = "cng";
    cJSON *child0, *child1;
    char *val_str;
    double val_dbl;

    if ((parser == RT_NULL) || (group == RT_NULL) || (mode == RT_NULL))
        return RT_NULL;

    SkvCngParam *cng = (SkvCngParam *)rt_malloc(sizeof(SkvCngParam));
    RT_ASSERT(cng != RT_NULL);

    /* skv_configs */
    child0 = cJSON_GetObjectItem((const cJSON *)parser, group);
    RT_ASSERT(child0 != RT_NULL);

    /* skv_configs/cng */
    child0 = cJSON_GetObjectItem(child0, node);
    RT_ASSERT(child0 != RT_NULL);

    RT_LOG("==== JSON CNG Params ====\n");

    /* skv_configs/cng/status */
    child1 = cJSON_GetObjectItem(child0, "status");
    if (child1)
    {
        val_str = cJSON_GetStringValue(child1);
        RT_ASSERT(val_str != RT_NULL);

        RT_LOG("status: %s\n", val_str);

        if (!strcasecmp("enable", val_str))
        {
            *mode |= BF_EN_CNG;
        }
    }
    else
    {
        RT_LOG("NOT FILL status\n");
    }

    /* skv_configs/cng/fGain */
    child1 = cJSON_GetObjectItem(child0, "fGain");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        cng->fGain = val_dbl;
        RT_LOG("CNG: fGain=%f\n", (double)cng->fGain);
    }
    else
    {
        RT_LOG("NOT FILL fGain\n");
    }

    /* skv_configs/cng/fMpy */
    child1 = cJSON_GetObjectItem(child0, "fMpy");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        cng->fMpy = val_dbl;
        RT_LOG("CNG: fMpy=%f\n", (double)cng->fMpy);
    }
    else
    {
        RT_LOG("NOT FILL fMpy\n");
    }

    /* skv_configs/cng/fSmoothAlpha */
    child1 = cJSON_GetObjectItem(child0, "fSmoothAlpha");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        cng->fSmoothAlpha = val_dbl;
        RT_LOG("CNG: fSmoothAlpha=%f\n", (double)cng->fSmoothAlpha);
    }
    else
    {
        RT_LOG("NOT FILL fSmoothAlpha\n");
    }

    /* skv_configs/cng/fSpeechGain */
    child1 = cJSON_GetObjectItem(child0, "fSpeechGain");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        cng->fSpeechGain = val_dbl;
        RT_LOG("CNG: fSpeechGain=%f\n", (double)cng->fSpeechGain);
    }
    else
    {
        RT_LOG("NOT FILL fSpeechGain\n");
    }

    return cng;
}

static SkvDtdParam *parseSkvDtdConfigs(void *parser, const char *group, int *mode)
{
    const char *node = "dtd";
    cJSON *child0, *child1;
    char *val_str;
    double val_dbl;

    if ((parser == RT_NULL) || (group == RT_NULL) || (mode == RT_NULL))
        return RT_NULL;

    SkvDtdParam *dtd = (SkvDtdParam *)rt_malloc(sizeof(SkvDtdParam));
    RT_ASSERT(dtd != RT_NULL);

    /* skv_configs */
    child0 = cJSON_GetObjectItem((const cJSON *)parser, group);
    RT_ASSERT(child0 != RT_NULL);

    /* skv_configs/dtd */
    child0 = cJSON_GetObjectItem(child0, node);
    RT_ASSERT(child0 != RT_NULL);

    RT_LOG("==== JSON DTD Params ====\n");

    /* skv_configs/dtd/status */
    child1 = cJSON_GetObjectItem(child0, "status");
    if (child1)
    {
        val_str = cJSON_GetStringValue(child1);
        RT_ASSERT(val_str != RT_NULL);

        RT_LOG("status: %s\n", val_str);

        if (!strcasecmp("enable", val_str))
        {
            *mode |= BF_EN_STDT;
        }
    }
    else
    {
        RT_LOG("NOT FILL status\n");
    }

    /* skv_configs/dtd/ksiThd_high */
    child1 = cJSON_GetObjectItem(child0, "ksiThd_high");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        dtd->ksiThd_high = val_dbl;
        RT_LOG("DTD: ksiThd_high=%f\n", (double)dtd->ksiThd_high);
    }
    else
    {
        RT_LOG("NOT FILL ksiThd_high\n");
    }

    /* skv_configs/dtd/ksiThd_low */
    child1 = cJSON_GetObjectItem(child0, "ksiThd_low");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        dtd->ksiThd_low = val_dbl;
        RT_LOG("DTD: ksiThd_low=%f\n", (double)dtd->ksiThd_low);
    }
    else
    {
        RT_LOG("NOT FILL ksiThd_low\n");
    }

    return dtd;
}

static SkvEqParam *parseSkvEqConfigs(void *parser, const char *group, int *mode)
{
    const char *node = "eq";
    cJSON *child0, *child1;
    char *val_str;
    double val_dbl;

    if ((parser == RT_NULL) || (group == RT_NULL) || (mode == RT_NULL))
        return RT_NULL;

    SkvEqParam *eq = (SkvEqParam *)rt_malloc(sizeof(SkvEqParam));
    RT_ASSERT(eq != RT_NULL);

    RT_LOG("==== JSON EQ Params ====\n");

    /* skv_configs/ */
    child0 = cJSON_GetObjectItem((const cJSON *)parser, group);
    RT_ASSERT(child0 != RT_NULL);

    /* skv_configs/eq */
    child0 = cJSON_GetObjectItem(child0, node);
    RT_ASSERT(child0 != RT_NULL);

    /* skv_configs/eq/status */
    child1 = cJSON_GetObjectItem(child0, "status");
    if (child1)
    {
        val_str = cJSON_GetStringValue(child1);
        RT_ASSERT(val_str != RT_NULL);

        RT_LOG("status: %s\n", val_str);

        if (!strcasecmp("enable", val_str))
        {
            *mode |= BF_EN_NLP;
        }
    }
    else
    {
        RT_LOG("NOT FILL status\n");
    }

    /* skv_configs/eq/shwParaLen */
    child1 = cJSON_GetObjectItem(child0, "shwParaLen");
    if (child1)
    {
        val_dbl = cJSON_GetNumberValue(child1);
        eq->shwParaLen = val_dbl;
        RT_LOG("EQ: shwParaLen=%f\n", (double)eq->shwParaLen);
    }
    else
    {
        RT_LOG("NOT FILL shwParaLen\n");
    }

    /* skv_configs/eq/pfCoeff_0 */
    child1 = cJSON_GetObjectItem(child0, "pfCoeff_0");
    if (child1)
    {
        int temp[13];

        val_str = cJSON_GetStringValue(child1);
        RT_ASSERT(val_str != RT_NULL);

        RT_LOG("EqPara_16k: %s\n", val_str);

        sscanf(val_str, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", &temp[0], &temp[1], &temp[2],
               &temp[3], &temp[4], &temp[5], &temp[6], &temp[7],
               &temp[8], &temp[9], &temp[10], &temp[11], &temp[12]);
        for (int i = 0 ; i < 13; i++)
        {
            eq->pfCoeff[0][i] = temp[i];
            RT_LOG("pfCoeff[0][%d]: %d\n", i, eq->pfCoeff[0][i]);
        }
    }
    else
    {
        RT_LOG("NOT FILL pfCoeff_1\n");
    }

    /* skv_configs/eq/pfCoeff_1 */
    child1 = cJSON_GetObjectItem(child0, "pfCoeff_1");
    if (child1)
    {
        int temp[13];

        val_str = cJSON_GetStringValue(child1);
        RT_ASSERT(val_str != RT_NULL);

        RT_LOG("EqPara_16k: %s\n", val_str);

        sscanf(val_str, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", &temp[0], &temp[1], &temp[2],
               &temp[3], &temp[4], &temp[5], &temp[6], &temp[7],
               &temp[8], &temp[9], &temp[10], &temp[11], &temp[12]);
        for (int i = 0 ; i < 13; i++)
        {
            eq->pfCoeff[1][i] = temp[i];
            RT_LOG("pfCoeff[1][%d]: %d\n", i, eq->pfCoeff[1][i]);
        }
    }
    else
    {
        RT_LOG("NOT FILL pfCoeff_1\n");
    }

    /* skv_configs/eq/pfCoeff_2 */
    child1 = cJSON_GetObjectItem(child0, "pfCoeff_2");
    if (child1)
    {
        int temp[13];

        val_str = cJSON_GetStringValue(child1);
        RT_ASSERT(val_str != RT_NULL);

        RT_LOG("EqPara_16k: %s\n", val_str);

        sscanf(val_str, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", &temp[0], &temp[1], &temp[2],
               &temp[3], &temp[4], &temp[5], &temp[6], &temp[7],
               &temp[8], &temp[9], &temp[10], &temp[11], &temp[12]);
        for (int i = 0 ; i < 13; i++)
        {
            eq->pfCoeff[2][i] = temp[i];
            RT_LOG("pfCoeff[2][%d]: %d\n", i, eq->pfCoeff[2][i]);
        }
    }
    else
    {
        RT_LOG("NOT FILL pfCoeff_2\n");
    }

    /* skv_configs/eq/pfCoeff_3 */
    child1 = cJSON_GetObjectItem(child0, "pfCoeff_3");
    if (child1)
    {
        int temp[13];

        val_str = cJSON_GetStringValue(child1);
        RT_ASSERT(val_str != RT_NULL);

        RT_LOG("EqPara_16k: %s\n", val_str);

        sscanf(val_str, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", &temp[0], &temp[1], &temp[2],
               &temp[3], &temp[4], &temp[5], &temp[6], &temp[7],
               &temp[8], &temp[9], &temp[10], &temp[11], &temp[12]);
        for (int i = 0 ; i < 13; i++)
        {
            eq->pfCoeff[3][i] = temp[i];
            RT_LOG("pfCoeff[3][%d]: %d\n", i, eq->pfCoeff[3][i]);
        }
    }
    else
    {
        RT_LOG("NOT FILL pfCoeff_3\n");
    }

    /* skv_configs/eq/pfCoeff_4 */
    child1 = cJSON_GetObjectItem(child0, "pfCoeff_4");
    if (child1)
    {
        int temp[13];

        val_str = cJSON_GetStringValue(child1);
        RT_ASSERT(val_str != RT_NULL);

        RT_LOG("EqPara_16k: %s\n", val_str);

        sscanf(val_str, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", &temp[0], &temp[1], &temp[2],
               &temp[3], &temp[4], &temp[5], &temp[6], &temp[7],
               &temp[8], &temp[9], &temp[10], &temp[11], &temp[12]);
        for (int i = 0 ; i < 13; i++)
        {
            eq->pfCoeff[4][i] = temp[i];
            RT_LOG("pfCoeff[4][%d]: %d\n", i, eq->pfCoeff[4][i]);
        }
    }
    else
    {
        RT_LOG("NOT FILL pfCoeff_4\n");
    }

    return eq;
}

static void dumpSkvAecConfig(SkvAecParam *param)
{
    if (param == RT_NULL)
        return;

    RT_LOGD("********************* AEC Configs dump ******************");
    RT_LOGD("pos = %d, drop_ref_channel = %d", param->pos, param->drop_ref_channel);
    RT_LOGD("aec_mode = 0x%x, delay_len = %d, look_ahead = %d", param->aec_mode_en,
            param->delay_len, param->look_ahead);
    RT_LOGD("********************** End ******************************");
}

static void dumpSkvBeamFormConfig(SkvBeamFormParam *param)
{
    if (param == RT_NULL)
        return;

    RT_LOGD("****************** BeamForm Configs dump ******************");
    RT_LOGD("model_en = 0x%x, ref_pos = %d", param->model_en, param->ref_pos);
    RT_LOGD("targ = %d", param->targ);
    RT_LOGD("num_ref_channel = %d, drop_ref_channel = %d",
            param->num_ref_channel, param->drop_ref_channel);

    if ((param->model_en & BF_EN_DEREV) && param->dereverb)
    {
        RT_LOGD("dereverb:");
        SkvDereverbParam *dere = param->dereverb;
        RT_LOGD("   rlsLg = %d, curveLg = %d, delay = %d",
                dere->rlsLg, dere->curveLg, dere->delay);
        RT_LOGD("   forgetting = %f, t60 = %f, coCoeff = %f",
                dere->forgetting, dere->t60, dere->coCoeff);
    }

    if ((param->model_en & BF_EN_NLP) && param->nlp)
    {
        RT_LOGD("nlp:");
        RT_LOGD("   BandPassThd   SuperEstFactor");
        SkvNlpParam *nlp = param->nlp;
        for (int i = 0; i < 8; i++)
        {
            RT_LOGD("        %2d             %2d", nlp->nlp16k[i][0], nlp->nlp16k[i][1]);
        }
    }

    if ((param->model_en & BF_EN_AES) && param->aes)
    {
        SkvAesParam *aes = param->aes;
        RT_LOGD("aes:");
        RT_LOGD("   beta_up = %f, beta_down = %f", aes->beta_up, aes->beta_down);
    }

    if ((param->model_en & BF_EN_ANR) && param->anr)
    {
        SkvAnrParam *anr = param->anr;
        RT_LOGD("anr:");
        RT_LOGD("   noiseFactor = %f, swU = %d", anr->noiseFactor,
                anr->swU);
        RT_LOGD("   psiMin = %f, psiMax = %f, fGmin = %f", anr->psiMin,
                anr->psiMax, anr->fGmin);
    }

    if ((param->model_en & BF_EN_AGC) && param->agc)
    {
        SkvAgcParam *agc = param->agc;
        RT_LOGD("agc:");
        RT_LOGD("   attack_time = %f, release_time = %f, attenuate_time = %f",
                agc->attack_time, agc->release_time, agc->attenuate_time);
        RT_LOGD("   max_gain = %f, max_peak = %f", agc->max_gain, agc->max_peak);

        RT_LOGD("   fLineGainDb = %f", agc->fLineGainDb);
        RT_LOGD("   fRth0 = %f, fRth1 = %f, fRth2 = %f", agc->fRth0, agc->fRth1, agc->fRth2);
        RT_LOGD("   fRk0 = %f, fRk1 = %f, fRk1 = %f", agc->fRk0, agc->fRk1, agc->fRk2);
        RT_LOGD("   swSmL0 = %d, swSmL1 = %d, swSmL2 = %d", agc->swSmL0, agc->swSmL1, agc->swSmL2);
    }

    if ((param->model_en & BF_EN_CNG) && param->cng)
    {
        SkvCngParam *cng = param->cng;
        RT_LOGD("cng:");
        RT_LOGD("   fGain = %f, fMpy = %f", cng->fGain, cng->fMpy);
        RT_LOGD("   fSmoothAlpha = %f, fSpeechGain = %f", cng->fSmoothAlpha, cng->fSpeechGain);
    }

    if ((param->model_en & BF_EN_STDT) && param->dtd)
    {
        SkvDtdParam *dtd = param->dtd;
        RT_LOGD("stdt:");
        RT_LOGD("   ksiThd_high = %f, ksiThd_low = %f", dtd->ksiThd_high, dtd->ksiThd_low);
    }
    RT_LOGD("********************** End ***************************");
}

static void destroySkvAnrConfig(SkvAnrParam **config)
{
    rt_free(*config);
    *config = RT_NULL;
}

static void destroySkvAgcConfig(SkvAgcParam **config)
{
    rt_free(*config);
    *config = RT_NULL;
}

static void destroySkvAecConfig(SkvAecParam **config)
{
    SkvAecParam *aec = *config;
    if (aec != RT_NULL)
        rt_free(aec->mic_chns_map);

    rt_free(*config);
    *config = RT_NULL;
}

static void destroySkvNlpConfig(SkvNlpParam **config)
{
    rt_free(*config);
    *config = RT_NULL;
}

static void destroySkvDereverbConfig(SkvDereverbParam **config)
{
    rt_free(*config);
    *config = RT_NULL;
}

static void destroySkvCngConfig(SkvCngParam **config)
{
    rt_free(*config);
    *config = RT_NULL;
}

static void destroySkvDtdConfig(SkvDtdParam **config)
{
    rt_free(*config);
    *config = RT_NULL;
}

static void destroySkvAesConfig(SkvAesParam **config)
{
    rt_free(*config);
    *config = RT_NULL;
}

static void destroySkvEqConfig(SkvEqParam **config)
{
    rt_free(*config);
    *config = RT_NULL;
}

static void destroySkvBeamFormConfig(SkvBeamFormParam **config)
{
    SkvBeamFormParam *param = *config;

    if (param != RT_NULL)
    {
        destroySkvDereverbConfig(&param->dereverb);
        destroySkvNlpConfig(&param->nlp);
        destroySkvAnrConfig(&param->anr);
        destroySkvAgcConfig(&param->agc);
        destroySkvCngConfig(&param->cng);
        destroySkvDtdConfig(&param->dtd);
        destroySkvAesConfig(&param->aes);
        destroySkvEqConfig((&param->eq));
        rt_free(param);
        *config = RT_NULL;
    }
}

static void destroySkvConfig(RTSkvParam **config)
{
    RTSkvParam *param = *config;
    if (param == RT_NULL)
        return;

    destroySkvAecConfig(&param->aec);
    destroySkvBeamFormConfig(&param->bf);
    rt_free(param);
    *config = RT_NULL;
}

RTSkvParam *parse_encr_configs(char *file_path)
{
    char *buf;
    size_t size;
    cJSON *root = NULL;
    const char *group = "skv_configs";

    FILE *fd = fopen(file_path, "rb");
    if (!fd)
    {
        printf("%s open fail!\n", file_path);
        return -1;
    }
    fseek(fd, 0, SEEK_END);
    size = ftell(fd);
    fseek(fd, 0, SEEK_SET);
    buf = malloc(size);
    if (!buf)
    {
        fclose(fd);
        return -1;
    }
    fread(buf, 1, size, fd);
    fclose(fd);

    root = cJSON_Parse(buf);
    RT_ASSERT(root != RT_NULL);

    RTSkvParam *skv = (RTSkvParam *)rt_malloc(sizeof(RTSkvParam));
    RT_ASSERT(skv != RT_NULL);
    rt_memset(skv, 0, sizeof(RTSkvParam));

    skv->aec = parseSkvAecConfigs(root, group, &skv->model);
    skv->bf = parseSkvBfConfigs(root, group, &skv->model);

    parseSkvFastAecConfigs(root, group, &skv->bf->model_en);
    parseSkvWakeUpConfigs(root, group, &skv->bf->model_en);
    parseSkvGscConfigs(root, group, &skv->bf->model_en);

    skv->bf->agc = parseSkvAgcConfigs(root, group, &skv->bf->model_en);
    skv->bf->anr = parseSkvAnrConfigs(root, group, &skv->bf->model_en);
    skv->bf->nlp = parseSkvNlpConfigs(root, group, &skv->bf->model_en);
    skv->bf->dereverb = parseSkvDereverbConfigs(root, group, &skv->bf->model_en);
    skv->bf->cng = parseSkvCngConfigs(root, group, &skv->bf->model_en);
    skv->bf->dtd = parseSkvDtdConfigs(root, group, &skv->bf->model_en);
    skv->bf->aes = parseSkvAesConfigs(root, group, &skv->bf->model_en);
    skv->bf->eq = parseSkvEqConfigs(root, group, &skv->bf->model_en);

    if (skv->model & RT_SKV_AEC)
    {
        dumpSkvAecConfig(skv->aec);
    }

    if (skv->model & RT_SKV_BF)
    {
        dumpSkvBeamFormConfig(skv->bf);
    }

    return skv;
}

#endif
