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
 * IPC utility function declarations
 */


#ifndef __FMAC_UTILS_H__
#define __FMAC_UTILS_H__


/*******************************************************************************
 *         Include Files
 ******************************************************************************/
#include <linux/dma-mapping.h>
#include <linux/skbuff.h>

#include "lmac_msg.h"
#include "ipc_host.h"
#include "fmac_cmds.h"


/*******************************************************************************
 *         Defines
 ******************************************************************************/
/**
 ****************************************************************************************
 * @brief This function sets a value of a bit field inside an array of bits,
 * represented as an array of bytes.
 * @param[in] array Array of bits
 * @param[in] lsb Position of the LSB of the field inside the array of bits
 * @param[in] width Width of the field
 * @param[in] val Value to be set
 ****************************************************************************************
 */
__INLINE void _ssv_co_val_set(u8 array[], int lsb, int width, u8 val)
{
    int msb = lsb + width - 1;
    int l_byte_idx = lsb/8;
    int m_byte_idx = msb/8;

    if (m_byte_idx == l_byte_idx)
    {
        u8 mask = CO_BIT(width) - 1;
        int shift = lsb % 8;
        array[l_byte_idx] &= ~(mask << shift);
        array[l_byte_idx] |= (val & mask) << shift;
    }
    else
    {
        u8 l_bits_cnt = m_byte_idx * 8 - lsb;
        u8 l_mask = CO_BIT(l_bits_cnt) - 1;
        u8 m_mask = CO_BIT(width - l_bits_cnt) - 1;
        int l_shift = lsb % 8;
        array[l_byte_idx] &= ~(l_mask << l_shift);
        array[m_byte_idx] &= ~m_mask;
        array[l_byte_idx] |= (val & l_mask) << l_shift;
        array[m_byte_idx] |= (val >> l_bits_cnt) & m_mask;
    }
}

/**
 ****************************************************************************************
 * @brief This function sets a specific bit position inside an array of bits, represented
 * as an array of bytes.
 * @param[in] array Array of bits
 * @param[in] pos Bit position to be set
 ****************************************************************************************
 */
__INLINE void _ssv_co_bit_set(u8 array[], u8 pos)
{
    array[pos / 8] |= CO_BIT(pos % 8);
}


/*******************************************************************************
 *         Enumerations
 ******************************************************************************/
enum ssv_dev_flag {
    SSV_DEV_RESTARTING,
    SSV_DEV_STACK_RESTARTING,
    SSV_DEV_STARTED,
};


/*******************************************************************************
 *         Structures
 ******************************************************************************/
struct ssv_dbginfo {
    struct mutex mutex;
    struct dbg_debug_dump_tag *buf;
    dma_addr_t dma_addr;
    int bufsz;
};

/*
 * Structure used to store information regarding Debug msg buffers in the driver
 */
struct ssv_dbg_elem {
    struct ipc_dbg_msg *dbgbuf_ptr;
    dma_addr_t dma_addr;
};


/*******************************************************************************
 *         Variables
 ******************************************************************************/


/*******************************************************************************
 *         Functions
 ******************************************************************************/
/**
 ******************************************************************************
 * @brief Initialize IPC interface.
 *
 * This function initializes IPC interface by registering callbacks, setting
 * shared memory area and calling IPC Init function.
 *
 * This function should be called only once during driver's lifetime.
 *
 * @param[in]   sc              Pointer to main structure storing all the
 *                              relevant information
 * @param[in]   ipc_shared_mem
 *
 ******************************************************************************
 */
int ssv_ipc_init(struct ssv_softc *sc);

/**
 ******************************************************************************
 * @brief Release IPC interface.
 *
 * @param[in]   sc              Pointer to main structure storing all the relevant
 *                              information
 *
 ******************************************************************************
 */
void ssv_ipc_deinit(struct ssv_softc *sc);

/**
 ******************************************************************************
 * @brief Function called upon DBG_ERROR_IND message reception.
 * This function triggers the UMH script call that will indicate to the user
 * space the error that occurred and stored the debug dump. Once the UMH script
 *  is executed, the ssv_umh_done function has to be called.
 *
 * @param[in]   sc      Pointer to main structure storing all the relevant
 *                        information
 *
 ******************************************************************************
 */
void ssv_error_ind(struct ssv_softc *sc);

void ssv_chanctx_link(struct ssv_vif *vif, u8 idx,
                        struct cfg80211_chan_def *chandef);
void ssv_chanctx_unlink(struct ssv_vif *vif);
int  ssv_chanctx_valid(struct ssv_softc *sc, u8 idx);

struct ssv_sta *ssv_get_sta(struct ssv_softc *sc, const u8 *mac_addr);

void ssv_rxtput_calculation(struct ssv_softc *sc, u32 len, u32 seq_no);

void ssv_hex_dump(unsigned char *data, int len);
int ssv_xmit_deauth_frame(struct ssv_softc *sc, struct ssv_vif *vif, struct ssv_sta *sta, 
        u8 *da, u8 *sa, u8 *bssid, u16 reason);
struct sk_buff *ssv_build_nulldata_frame(u8 *da, u8 *sa, u8 *bssid, bool qos, bool fromds);
int ssv_fmac_hci_tx(struct ssv_softc *sc, struct sk_buff *skb, int txqid, 
        bool force_trigger,u32 tx_flags);
enum data_frame_types ssv_get_data_frame_type(struct sk_buff *skb);
#ifdef CONFIG_SSV_CHANNEL_FOLLOW
void ssv_channel_switch_check(struct ssv_softc *sc, u16 freq, enum nl80211_channel_type chann_type);
#endif /*CONFIG_SSV_CHANNEL_FOLLOW */
void ssv6xxx_fw_reset_send_deauth_check(struct ssv_softc *sc, struct ssv_vif *vif);
#endif /* __FMAC_UTILS_H__ */
