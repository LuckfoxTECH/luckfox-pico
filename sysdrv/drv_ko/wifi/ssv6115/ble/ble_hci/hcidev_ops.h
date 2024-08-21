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

/*
 * Common defines and declarations for host driver and firmware for all
 * platforms.
 */


#ifndef __HCIDEV_OPS_H__
#define __HCIDEV_OPS_H__


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
int ssv_ble_hci_dev_register(struct ssv_ble_softc *sbsc);
void ssv_ble_hci_dev_unregister(struct ssv_ble_softc *sbsc);


#endif /* __HCIDEV_OPS_H__ */

