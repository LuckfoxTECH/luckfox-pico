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
#include "ssv_rftool.h"
#include "ssv_debug.h"


extern int ssv_rftool_event(void *app_param, struct sk_buff *skb);

/* ssv_rftool_init must hook hci callback function
 * ex: ssv6xxx_hci_proc_rx_register()
 * */
int ssv_rftool_init(void **plat_srfc, struct ssv6xxx_hci_ops *hci_ops, void *hci_priv, struct device *dev)
{
    struct ssv_rftool_softc *srfc = NULL;

    srfc = kzalloc(sizeof(*srfc), GFP_KERNEL);
    if (srfc == NULL){
        SSV_LOG_DBG("Fail to alloc rftool_softc\n");
        return -ENOMEM;
    }
    
    srfc->hci_priv = hci_priv;
    srfc->hci_ops = hci_ops;
    srfc->dev = dev;
    init_completion(&srfc->ssv_rftool_cmd_done);
    
        ssv6xxx_hci_proc_rx_register((void *)hci_priv, ssv_rftool_event, (void *)srfc);

    *plat_srfc = srfc;
    return 0;
}
EXPORT_SYMBOL(ssv_rftool_init);

void ssv_rftool_deinit(void *plat_srfc)
{
    struct ssv_rftool_softc *srfc = (struct ssv_rftool_softc *)plat_srfc;
    
    ssv6xxx_hci_proc_rx_unregister((void *)srfc->hci_priv, ssv_rftool_event, NULL);
    
    kfree(srfc);
    plat_srfc = NULL;
    
    return;
}
EXPORT_SYMBOL(ssv_rftool_deinit);
