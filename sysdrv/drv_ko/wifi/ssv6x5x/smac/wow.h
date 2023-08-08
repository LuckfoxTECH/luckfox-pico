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

#ifndef _WOW_H_
#define _WOW_H_

#define SSV6XXX_WOW_MAX_NUM_PATTERN         16
#define SSV6XXX_WOW_MAX_PATTERN_SIZE        256

#define SSV6XXX_WOW_ANY_PATTERN_EN          0x01
#define SSV6XXX_WOW_MAGIC_PATTERN_EN        0x02

#ifdef CONFIG_PM
int ssv6xxx_trigger_pmu(struct ssv_softc *sc, u8 triggers, u32 host_ipv4_addr);
int ssv6xxx_suspend (struct ieee80211_hw *hw, struct cfg80211_wowlan *wowlan);
int ssv6xxx_resume (struct ieee80211_hw *hw);
#endif
void ssv6xxx_attach_wow(struct ssv_softc *sc);

#endif // _WOW_H_
