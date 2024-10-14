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
#include <linux/version.h>
#include <linux/kthread.h>
#include <linux/inetdevice.h>
#include <linux/of.h>

#include "hci/ssv_hci.h"
#include "nimble_msg.h"
#include "nimble.h"
#include "ssv_debug.h"

/* ssv_nimble_init must hook hci callback function
 * ex: ssv6xxx_hci_proc_rx_register()
 * */
int ssv_nimble_init(void **plat_sbc, struct ssv6xxx_hci_ops *hci_ops, void *hci_priv, struct device *dev)
{
    struct ssv_nimble_softc *snc = NULL;

    snc = kzalloc(sizeof(*snc), GFP_KERNEL);
    if (snc == NULL){
        SSV_LOG_DBG("Fail to alloc ble_softc\n");
        return -ENOMEM;
    }
    
    snc->hci_priv = hci_priv;
    snc->hci_ops = hci_ops;
    snc->dev = dev;

    ssv6xxx_hci_proc_rx_register((void *)hci_priv, ssv_nimble_event, (void *)snc);

    *plat_sbc = snc;
    return 0;
}
EXPORT_SYMBOL(ssv_nimble_init);

void ssv_nimble_deinit(void *plat_sbc)
{
    struct ssv_nimble_softc *snc = (struct ssv_nimble_softc *)plat_sbc;
    
    ssv6xxx_hci_proc_rx_unregister((void *)snc->hci_priv, ssv_nimble_event, NULL);
    
    kfree(snc);
    plat_sbc = NULL;
    
    return;
}
EXPORT_SYMBOL(ssv_nimble_deinit);
