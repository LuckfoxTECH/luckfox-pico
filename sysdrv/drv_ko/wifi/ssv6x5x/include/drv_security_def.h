/*
 * Driver interface definition
 * Copyright (c) 2003-2010, Jouni Malinen <j@w1.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 *
 * This file defines a driver interface used by both %wpa_supplicant and
 * hostapd. The first part of the file defines data structures used in various
 * driver operations. This is followed by the struct wpa_driver_ops that each
 * driver wrapper will beed to define with callback functions for requesting
 * driver operations. After this, there are definitions for driver event
 * reporting with wpa_supplicant_event() and some convenience helper functions
 * that can be used to report events.
 */

#ifndef _DRV_SECURITY_DEF_H_
#define _DRV_SECURITY_DEF_H_

/**
* enum sec_type_en (sec_type) - Security type setting.
*
* @ SEC_DISABLE: no security
* @ SEC_WEP40: WEP-40
* @ SEC_WEP104: WEP-104
* @ SEC_TKIP: TKIP
* @ SEC_CCMP: CCMP
*/
typedef enum sec_type_en {
    SECURITY_NONE,
    SECURITY_WEP40,
    SECURITY_WEP104,
    SECURITY_TKIP,
    SECURITY_CCMP,
    SECURITY_SMS4, 
    SECURITY_CIPHER_INVALID
} sec_type;

enum SEC_ALG {
	SEC_ALG_NONE,
	SEC_ALG_WEP,
	SEC_ALG_TKIP,
	SEC_ALG_CCMP,
	SEC_ALG_IGTK,
	SEC_ALG_PMK
};

enum SSV6XXX_WSID_SEC
{
    SSV6XXX_WSID_SEC_NONE = 0,
    SSV6XXX_WSID_SEC_PAIRWISE = 1<<0,
    SSV6XXX_WSID_SEC_GROUP = 1<<1,
    //SSV6XXX_WSID_SEC_MAX = 1<<2
};

#endif
