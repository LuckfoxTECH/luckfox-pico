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

#ifndef _HW_SCAN_H_
#define _HW_SCAN_H_

#include <linux/workqueue.h>
#include <linux/delay.h>

#define SSV_SCAN_DECISION           1
#define SSV_SCAN_SUSPEND            2
#define SSV_SCAN_RESUME             3
#define SSV_SCAN_ABORT              4

void ssv6xxx_scan_complete(struct ssv_softc *sc, bool abort);
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 17, 0))    
int ssv6200_hw_scan(struct ieee80211_hw *hw, struct ieee80211_vif *vif,
                struct ieee80211_scan_request *hw_req);
#else
int ssv6200_hw_scan(struct ieee80211_hw *hw, struct ieee80211_vif *vif,
                struct cfg80211_scan_request *hw_req);
#endif
void ssv6200_cancel_hw_scan(struct ieee80211_hw *hw, struct ieee80211_vif *vif);
void ssv6xxx_scan_process(struct work_struct *work);
void ssv6xxx_scan_ignore_process(struct work_struct *work);
void ssv6xxx_cancel_hw_scan(struct ssv_softc *sc);

#endif // _HW_SCAN_H_
