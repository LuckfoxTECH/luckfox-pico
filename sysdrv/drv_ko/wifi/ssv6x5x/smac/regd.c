/*
 * Copyright (c) 2015 iComm-semi Ltd.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <linux/module.h>
#include <linux/nl80211.h>
#include <linux/version.h>

#include <ssv6200.h>
#include <linux_80211.h>
#include "regd.h"
#include "dev.h"

MODULE_AUTHOR("iComm-semi, Ltd");
MODULE_DESCRIPTION("Support for SSV6xxx wireless LAN cards.");
MODULE_SUPPORTED_DEVICE("SSV6xxx 802.11n WLAN cards");
MODULE_LICENSE("Dual BSD/GPL");

#define CHAN2G(_freq, _idx)  {      \
    .band = INDEX_80211_BAND_2GHZ,    \
    .center_freq = (_freq),         \
    .hw_value = (_idx),             \
    .max_power = 20,                \
}

#define CHAN5G(_freq, _idx)  {      \
    .band = INDEX_80211_BAND_5GHZ,    \
    .center_freq = (_freq),         \
    .hw_value = (_idx),             \
    .max_power = 20,                \
}

#define SHPCHECK(__hw_rate, __flags) \
    ((__flags & IEEE80211_RATE_SHORT_PREAMBLE) ? (__hw_rate +3 ) : 0)


#define RATE(_bitrate, _hw_rate, _flags) {      \
    .bitrate        = (_bitrate),               \
    .flags          = (_flags),                 \
    .hw_value       = (_hw_rate),               \
    .hw_value_short = SHPCHECK(_hw_rate,_flags) \
}

/**
* Note that this table maybe modified at run-time. We shall make a copy of 
* this table before using it.
*/
static struct ieee80211_channel ssv6200_2ghz_chantable[] =
{   /* BAND-2G (pa_band 0)*/
    CHAN2G(2412, 1), /* Channel 1 */
    CHAN2G(2417, 2), /* Channel 2 */
    CHAN2G(2422, 3), /* Channel 3 */
    CHAN2G(2427, 4), /* Channel 4 */
    CHAN2G(2432, 5), /* Channel 5 */
    CHAN2G(2437, 6), /* Channel 6 */
    CHAN2G(2442, 7), /* Channel 7 */
    CHAN2G(2447, 8), /* Channel 8 */
    CHAN2G(2452, 9), /* Channel 9 */
    CHAN2G(2457, 10), /* Channel 10 */
    CHAN2G(2462, 11), /* Channel 11 */
    CHAN2G(2467, 12), /* Channel 12 */
    CHAN2G(2472, 13), /* Channel 13 */
    CHAN2G(2484, 14), /* Channel 14 */
};

static struct ieee80211_channel ssv6200_5ghz_chantable[] = {
	/*BAND_5100 (pa_band 1)*/
	CHAN5G(5180, 36), /* Channel 36 */
	/*BAND_5500 */
	CHAN5G(5200, 40), /* Channel 40 */
	CHAN5G(5220, 44), /* Channel 44 */
	CHAN5G(5240, 48), /* Channel 48 */
	CHAN5G(5260, 52), /* Channel 52 */
	CHAN5G(5280, 56), /* Channel 56 */
	CHAN5G(5300, 60), /* Channel 60 */
	CHAN5G(5320, 64), /* Channel 64 */
	/*BAND_5700 (pa_band 2)*/
	CHAN5G(5500, 100), /* Channel 100 */
	CHAN5G(5520, 104), /* Channel 104 */
	CHAN5G(5540, 108), /* Channel 108 */
	CHAN5G(5560, 112), /* Channel 112 */
	CHAN5G(5580, 116), /* Channel 116 */
	CHAN5G(5600, 120), /* Channel 120 */
	CHAN5G(5620, 124), /* Channel 124 */
	CHAN5G(5640, 128), /* Channel 128 */
	CHAN5G(5660, 132), /* Channel 132 */
	CHAN5G(5680, 136), /* Channel 136 */
	/* _BAND_5900 (pa_band3)*/	
	CHAN5G(5700, 140), /* Channel 140 */
	CHAN5G(5720, 144), /* Channel 144 */
	CHAN5G(5745, 149), /* Channel 149 */
	CHAN5G(5765, 153), /* Channel 153 */
	CHAN5G(5785, 157), /* Channel 157 */
	CHAN5G(5805, 161), /* Channel 161 */
	CHAN5G(5825, 165), /* Channel 165 */
};

static struct ieee80211_rate ssv6200_legacy_rates[] =
{
    RATE(10,  0x00, 0),
    RATE(20,  0x01, 0),
    RATE(55,  0x02, IEEE80211_RATE_SHORT_PREAMBLE),
    RATE(110, 0x03, IEEE80211_RATE_SHORT_PREAMBLE),
    RATE(60,  0x07, 0),
    RATE(90,  0x08, 0),
    RATE(120, 0x09, 0),
    RATE(180, 0x0a, 0),
    RATE(240, 0x0b, 0),
    RATE(360, 0x0c, 0),
    RATE(480, 0x0d, 0),
    RATE(540, 0x0e, 0),
};

#define REGDOM_RULE(_start, _end, _flag)    \
{                               \
    .start_freq_khz = (_start),      \
    .end_freq_khz = (_end),          \
    .flags = (_flag),          \
}

static struct ssv_regdomain_chnl_plan ssv_regdom_table[] = {
    {
        .country_name = "00",
        .country_enum = SSV_CTRY_DEFAULT, 
        .n_reg_rules = 2, 
        .reg_chnl_rules = {
            REGDOM_RULE(2402, 2494, 0),
            REGDOM_RULE(5170, 5835, 0),
        }
    },
    {
        .country_name = "CN",
        .country_enum = SSV_CTRY_CN, 
        .n_reg_rules = 4, 
        .reg_chnl_rules = {
            REGDOM_RULE(2402, 2482, 0),
            REGDOM_RULE(5170, 5250, 0),
            REGDOM_RULE(5250, 5330, 0),
            REGDOM_RULE(5735, 5835, 0),
        }
    },
    {
        .country_name = "JP",
        .country_enum = SSV_CTRY_JP, 
        .n_reg_rules = 5, 
        .reg_chnl_rules = {
            REGDOM_RULE(2402, 2494, 0),
            REGDOM_RULE(5030, 5090, 0),
            REGDOM_RULE(5170, 5250, 0),
            REGDOM_RULE(5250, 5330, 0),
            REGDOM_RULE(5490, 5710, 0),
        }
    },
    {
        .country_name = "HK",
        .country_enum = SSV_CTRY_HK, 
        .n_reg_rules = 5, 
        .reg_chnl_rules = {
            REGDOM_RULE(2402, 2482, 0),
            REGDOM_RULE(5170, 5250, 0),
            REGDOM_RULE(5250, 5330, 0),
            REGDOM_RULE(5490, 5710, 0),
            REGDOM_RULE(5735, 5835, 0),
        }
    },
    {
        .country_name = "TW",
        .country_enum = SSV_CTRY_TW, 
        .n_reg_rules = 5, 
        .reg_chnl_rules = {
            REGDOM_RULE(2402, 2472, 0),
            REGDOM_RULE(5270, 5330, 0),
            REGDOM_RULE(5490, 5590, 0),
            REGDOM_RULE(5650, 5710, 0),
            REGDOM_RULE(5735, 5835, 0),
        }
    },
    {
        .country_name = "US",
        .country_enum = SSV_CTRY_US, 
        .n_reg_rules = 4, 
        .reg_chnl_rules = {
            REGDOM_RULE(2402, 2472, 0),
            REGDOM_RULE(5170, 5250, 0),
            REGDOM_RULE(5250, 5330, 0),
            REGDOM_RULE(5735, 5835, 0),
        }
    },

};

static void ssv6xxx_update_regdom_channel(char *ctry)
{
    struct ssv_regdomain_chnl_plan *regd;
    int i = 0, chan_rule_idx = 0, ctry_idx = 0;
    u16 start_freq, end_freq;
    u32 flags;
   
    if ((ctry == NULL) || !strcmp(ctry, ""))
        return;

    for (ctry_idx = 0; ctry_idx < SSV_CTRY_MAX; ctry_idx++) {
        if (!strncmp(ssv_regdom_table[ctry_idx].country_name, ctry, 2))
            break;
    }
    
    if (ctry_idx == SSV_CTRY_MAX) {
        printk("error country code %s\n", ctry);
        return;
    }
    
    regd = &ssv_regdom_table[ctry_idx];
    //update 2.4g channel
    for (i = 0; i < ARRAY_SIZE(ssv6200_2ghz_chantable); i++) {
        ssv6200_2ghz_chantable[i].flags |= IEEE80211_CHAN_DISABLED;
    
        for (chan_rule_idx = 0; chan_rule_idx < regd->n_reg_rules; chan_rule_idx++) {
            start_freq = regd->reg_chnl_rules[chan_rule_idx].start_freq_khz;
            end_freq = regd->reg_chnl_rules[chan_rule_idx].end_freq_khz;
            flags = regd->reg_chnl_rules[chan_rule_idx].flags;
        
            if ((start_freq >= 2402) && (end_freq <= 2494)) {//2.4g rule

                if ((ssv6200_2ghz_chantable[i].center_freq >= start_freq) && 
                    (ssv6200_2ghz_chantable[i].center_freq <= end_freq)) {
                
                    ssv6200_2ghz_chantable[i].flags &= ~IEEE80211_CHAN_DISABLED;
                    ssv6200_2ghz_chantable[i].flags |= flags;
                    break;
                }
            }
        }
        //if (ssv6200_2ghz_chantable[i].flags & IEEE80211_CHAN_DISABLED)
        //    printk("chan %d, disable\n", ssv6200_2ghz_chantable[i].hw_value);
    }
    
    //update 5g channel
    for (i = 0; i < ARRAY_SIZE(ssv6200_5ghz_chantable); i++) {
        ssv6200_5ghz_chantable[i].flags |= IEEE80211_CHAN_DISABLED;
         
        for (chan_rule_idx = 0; chan_rule_idx < regd->n_reg_rules; chan_rule_idx++) {
            start_freq = regd->reg_chnl_rules[chan_rule_idx].start_freq_khz;
            end_freq = regd->reg_chnl_rules[chan_rule_idx].end_freq_khz;
            flags = regd->reg_chnl_rules[chan_rule_idx].flags;
            
            if ((start_freq >= 5170) && (end_freq <= 5835)) {//5g rule
                
                if ((ssv6200_5ghz_chantable[i].center_freq >= start_freq) && 
                    (ssv6200_5ghz_chantable[i].center_freq <= end_freq)) {
                
                    ssv6200_5ghz_chantable[i].flags &= ~IEEE80211_CHAN_DISABLED;
                    ssv6200_5ghz_chantable[i].flags |= flags;
                    break;
                }
            }
        }
        //if (ssv6200_5ghz_chantable[i].flags & IEEE80211_CHAN_DISABLED)
        //    printk("chan %d, disable\n", ssv6200_5ghz_chantable[i].hw_value);
    }
}

int ssv6xxx_update_hw_channel(struct ssv_softc *sc)
{
    void *channels;
    
    ssv6xxx_update_regdom_channel(sc->sh->cfg.ctry); 
    /* Initialize channels & rates */
	if (sc->sh->cfg.hw_caps & SSV6200_HW_CAP_2GHZ) {
        /**
                * Make a copy of the channel table before using it because it maybe
                * modified at run-time. Be aware that the duplicate one shall be freed
                * at driver deinit time.
                */
        channels = kmemdup(ssv6200_2ghz_chantable, sizeof(ssv6200_2ghz_chantable), GFP_KERNEL);
		if (!channels) {
		    return -1;
        }

		sc->sbands[INDEX_80211_BAND_2GHZ].channels = channels;
		sc->sbands[INDEX_80211_BAND_2GHZ].band = INDEX_80211_BAND_2GHZ;
		sc->sbands[INDEX_80211_BAND_2GHZ].n_channels = ARRAY_SIZE(ssv6200_2ghz_chantable); 
        sc->sbands[INDEX_80211_BAND_2GHZ].bitrates = ssv6200_legacy_rates;
		sc->sbands[INDEX_80211_BAND_2GHZ].n_bitrates = ARRAY_SIZE(ssv6200_legacy_rates);
	}
	
    if (sc->sh->cfg.hw_caps & SSV6200_HW_CAP_5GHZ) {
        /**
                * Make a copy of the channel table before using it because it maybe
                * modified at run-time. Be aware that the duplicate one shall be freed
                * at driver deinit time.
                */
        channels = kmemdup(ssv6200_5ghz_chantable, sizeof(ssv6200_5ghz_chantable), GFP_KERNEL);
		if (!channels) {
            channels = sc->sbands[INDEX_80211_BAND_2GHZ].channels;
            kfree(channels);
			return -1;
        }
        
		sc->sbands[INDEX_80211_BAND_5GHZ].channels = channels;
		sc->sbands[INDEX_80211_BAND_5GHZ].band = INDEX_80211_BAND_5GHZ;
		sc->sbands[INDEX_80211_BAND_5GHZ].n_channels = ARRAY_SIZE(ssv6200_5ghz_chantable);
		sc->sbands[INDEX_80211_BAND_5GHZ].bitrates = ssv6200_legacy_rates + 4;
		sc->sbands[INDEX_80211_BAND_5GHZ].n_bitrates = ARRAY_SIZE(ssv6200_legacy_rates) - 4;
	}

    return 0;
}
