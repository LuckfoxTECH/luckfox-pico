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

#ifndef _SSV6006_PRIV_H_
#define _SSV6006_PRIV_H_

#define COMMON_FOR_SMAC
// Macros for predefined register access.
#define REG32(_addr)    REG32_R(_addr)
#define REG32_W(_addr, _value)   do { SMAC_REG_WRITE(sh, _addr, _value); } while (0)

static void inline print_null(const char *fmt, ...)
{
}
// Macros for turismo common header
#define MSLEEP(_val)        msleep(_val)
#define MDELAY(_val)        mdelay(_val)
#define UDELAY(_val)        udelay(_val)
//#define PRINT               print_null
#define PRINT               printk
#define PRINT_ERR           printk
#define PRINT_INFO          printk
//--
void ssv_attach_ssv6006_common(struct ssv_hal_ops *hal_ops);
void ssv_attach_ssv6051_phy(struct ssv_hal_ops *hal_ops);
void ssv_attach_ssv6051_cabrioA_BBRF(struct ssv_hal_ops *hal_ops);
void ssv_attach_ssv6006_phy(struct ssv_hal_ops *hal_ops);
void ssv_attach_ssv6006c_phy(struct ssv_hal_ops *hal_ops);
void ssv_attach_ssv6006c_mac(struct ssv_hal_ops *hal_ops);
void ssv_attach_ssv6006_mac(struct ssv_hal_ops *hal_ops);
void ssv_attach_ssv6006_cabrioA_BBRF(struct ssv_hal_ops *hal_ops);
void ssv_attach_ssv6006_geminiA_BBRF(struct ssv_hal_ops *hal_ops);
void ssv_attach_ssv6006_turismoA_BBRF(struct ssv_hal_ops *hal_ops);
void ssv_attach_ssv6006_turismoB_BBRF(struct ssv_hal_ops *hal_ops);
void ssv_attach_ssv6006_turismoC_BBRF(struct ssv_hal_ops *hal_ops);
int ssv6006_get_pa_band(int ch);
void ssv6006c_reset_mib_mac(struct ssv_hw *sh);
void ssv6006c_set_external_lna(struct ssv_hw *sh, bool val);
void ssv6006c_set_external_pa(struct ssv_hw *sh, bool val);
void ssv6006c_set_extenal_PA(struct ssv_hw *sh, bool val);
void ssv6006c_update_edcca(struct ssv_hw *sh, bool val);

#endif // _SSV6006_PRIV_H_
