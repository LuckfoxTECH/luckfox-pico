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
#ifndef __BLE_HCI_MSG_H__
#define __BLE_HCI_MSG_H__
    
    
/*******************************************************************************
 *         Include Files
 ******************************************************************************/
#include "ble_hci.h"

/*******************************************************************************
 *         Defines
 ******************************************************************************/

/*******************************************************************************
 *         Enumerations
 ******************************************************************************/

/*******************************************************************************
 *         Structures
 ******************************************************************************/


/*******************************************************************************
 *         Variables
 ******************************************************************************/


/*******************************************************************************
 *         Functions
 ******************************************************************************/

void ssv_ble_hci_send_bleinit(struct ssv_ble_softc *sbsc, u8 reset_only);
void ssv_ble_hci_send_packet(struct ssv_ble_softc *sbsc, char *buf, u16 buflen);
int  ssv_ble_hci_event(void *app_param, struct sk_buff *skb);


#endif /* __NIMBLE_MSG_H__ */

