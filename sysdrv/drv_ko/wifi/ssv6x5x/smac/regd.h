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

enum ssv_country_name {
    SSV_CTRY_DEFAULT = 0, 
    SSV_CTRY_CN,
    SSV_CTRY_JP,
    SSV_CTRY_HK,
    SSV_CTRY_TW,
    SSV_CTRY_US,
    SSV_CTRY_MAX,
};

#define MAX_SSV_REG_CHNL_RULES      5
struct ssv_reg_chnl_rule {
    u32 start_freq_khz;
    u32 end_freq_khz;
    u32 flags;
};

struct ssv_regdomain_chnl_plan {
    char country_name[3];
    enum ssv_country_name country_enum;
    u16 n_reg_rules;
    struct ssv_reg_chnl_rule reg_chnl_rules[MAX_SSV_REG_CHNL_RULES];
};

int ssv6xxx_update_hw_channel(struct ssv_softc *sc);
