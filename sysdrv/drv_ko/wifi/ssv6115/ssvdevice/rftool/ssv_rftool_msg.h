/*
 * Copyright (c) 2021 iComm-semi Ltd.
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
 * TX function declarations
 */


#ifndef __SSV_RFTOOL_MSG_H__
#define __SSV_RFTOOL_MSG_H__


/*******************************************************************************
 *         Include Files
 ******************************************************************************/
#include <linux/version.h>
#include "ssv_rftool.h"
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
//void ssv_rftool_api_send(struct ssv_nimble_softc *snc, char *buf, u16 buflen);
int ssv_rftool_event(void *app_param, struct sk_buff *skb);

#endif /* __SSV_RFTOOL_MSG_H__ */

