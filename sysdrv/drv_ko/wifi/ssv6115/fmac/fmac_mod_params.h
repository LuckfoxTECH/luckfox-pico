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
 * Declaration of module parameters
 */


#ifndef __FMAC_MOD_PARAMS_H__
#define __FMAC_MOD_PARAMS_H__


/*******************************************************************************
 *         Include Files
 ******************************************************************************/
#include <linux/version.h>

/*******************************************************************************
 *         Defines
 ******************************************************************************/


/*******************************************************************************
 *         Enumerations
 ******************************************************************************/


/*******************************************************************************
 *         Structures
 ******************************************************************************/
struct ssv_mod_params {
    bool ht_on;
    bool vht_on;
    int mcs_map;
    bool he_on;
    bool twt_on;
    bool ldpc_on;
    bool vht_stbc;
    int phy_cfg;
    int uapsd_timeout;
    bool ap_uapsd_on;
    bool sgi;
    bool use_2040;
    bool use_80;
    bool custregd;
    int nss;
    bool bfmee;
    bool mesh;
    bool mutx;
    unsigned int roc_dur_max;
    int listen_itv;
    bool listen_bcmc;
    int lp_clk_ppm;
    bool ps_on;
    int tx_lft;
    int amsdu_maxnb;
    int uapsd_queues;

//#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 20, 0)
    int he_mcs_map;
    bool he_ul_on;

};


/*******************************************************************************
 *         Variables
 ******************************************************************************/
extern struct ssv_mod_params ssv_mod_params;


/*******************************************************************************
 *         Functions
 ******************************************************************************/
struct ssv_softc;
int ssv_handle_dynparams(struct ssv_softc *sc, struct wiphy *wiphy);
void ssv_enable_wapi(struct ssv_softc *sc);
void ssv_enable_mfp(struct ssv_softc *sc);


#endif /* __FMAC_MOD_PARAMS_H__ */

