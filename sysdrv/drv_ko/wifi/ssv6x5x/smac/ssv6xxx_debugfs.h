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

#ifndef __SSV6XXX_DBGFS_H__
#define __SSV6XXX_DBGFS_H__
// Device debugfs
int ssv6xxx_init_debugfs (struct ssv_softc *sc, const char *name);
void ssv6xxx_deinit_debugfs (struct ssv_softc *sc);
// virtual interface debugfs
int ssv6xxx_debugfs_remove_interface(struct ssv_softc *sc, struct ieee80211_vif *vif);
int ssv6xxx_debugfs_add_interface(struct ssv_softc *sc, struct ieee80211_vif *vif);
// Station debugfs
int ssv6xxx_debugfs_remove_sta(struct ssv_softc *sc, struct ssv_sta_info *sta);
int ssv6xxx_debugfs_add_sta(struct ssv_softc *sc, struct ssv_sta_info *sta);
#endif // __SSV6XXX_DBGFS_H__
