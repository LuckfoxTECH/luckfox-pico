/*
 * Copyright (c) 2022 iComm-semi Ltd.
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

/**
 * @file hwif.c
 * @brief hardware io interface functions.
 */

#include <linux/version.h>

#include "hwif.h"
#include "ipc_msg.h"
#include "ssv_debug.h"

static struct st_ipc_mem_layout ipc_mem_layout;
int ssv_configure_ipc_mem(struct device *child, struct ssv6xxx_hwif_ops *hwif_ops)
{
    //setting ipc config
    if(sizeof(struct st_ipc_mem_layout) <= SSV_IPC_MEM_SIZE) 
    {
        int i;
        u32 *pvalue = 0;

        memset(&ipc_mem_layout, 0, sizeof(struct st_ipc_mem_layout));
        
        ipc_mem_layout.pattern = 0x565353;
        pvalue =(u32*)(&ipc_mem_layout);

        //assign to ipc memory
        for(i=0; i<sizeof(struct st_ipc_mem_layout); i+=4)
        {
            u32 addr = 0x0850FE00+i;
            hwif_ops->writereg(child, addr, *pvalue);
            //SSV_LOG_DBG("write 0x%x as 0x%x\n", addr, *pvalue );
            pvalue++;
        }
    }
    else
    {
        SSV_LOG_ERR("ipc mem size out of size(%d)\n", SSV_IPC_MEM_SIZE);
        BUG_ON(1);
        return -1;
    }
	
	return 0;
}
