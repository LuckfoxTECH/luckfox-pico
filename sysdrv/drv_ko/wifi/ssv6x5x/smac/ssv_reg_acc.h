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

#ifndef _SSV_REG_ACC_H_
#define _SSV_REG_ACC_H_

/**
* Macros for ssv6200 register read/write access on Linux platform.
* @ SMAC_REG_WRITE() : write 4-byte value into hardware register.
* @ SMAC_REG_READ()  : read 4-byte value from hardware register.
* @ SMAC_REG_SET_BITS: set the specified bits to a value.
* @ SMAC_REG_CONFIRM:
* 
*/
#define SMAC_REG_WRITE(_s, _r, _v) \
        ({ \
            _s->hci.hci_ops->hci_write_word(_s->hci.hci_ctrl, _r,_v); \
        })
#define SMAC_REG_READ(_s, _r, _v)                   \
        ({ \
            _s->hci.hci_ops->hci_read_word(_s->hci.hci_ctrl, _r, _v); \
        })
#ifdef CONFIG_USB_EP0_RW_REGISTER
#define SMAC_REG_MCU_WRITE(_s, _r, _v) \
        ({ \
            typeof(_s) __s = _s; \
            __s->hci.hci_ops->hci_mcu_write_word(__s->hci.hci_ctrl, _r,_v); \
        })
#define SMAC_REG_MCU_READ(_s, _r, _v)                   \
        ({ \
            typeof(_s) __s = _s; \
            __s->hci.hci_ops->hci_mcu_read_word(__s->hci.hci_ctrl, _r, _v); \
        })
#endif

#ifndef __x86_64
#define SMAC_RF_REG_READ(_s, _r, _v)   SMAC_REG_READ(_s, _r, _v) 
#else
#define SMAC_RF_REG_READ(_s, _r, _v)                   \
        ({ \
            _s->hci.hci_ops->hci_read_word(_s->hci.hci_ctrl, _r, _v); \
            _s->hci.hci_ops->hci_read_word(_s->hci.hci_ctrl, _r, _v); \
        })
#endif

#define SMAC_BURST_REG_READ(_s, _r, _v, _n)                   \
        ({ \
            _s->hci.hci_ops->hci_burst_read_word(_s->hci.hci_ctrl, _r, _v, _n); \
        })
#define SMAC_BURST_REG_WRITE(_s, _r, _v, _n)                   \
        ({ \
            _s->hci.hci_ops->hci_burst_write_word(_s->hci.hci_ctrl, _r, _v, _n); \
        })
#define SMAC_LOAD_FW(_s,_addr, _data, _size)                      \
        ({ \
            _s->hci.hci_ops->hci_load_fw(_s->hci.hci_ctrl,_addr, _data, _size); \
        })
#define SMAC_IFC_RESET(_s)                      \
        ({ \
            _s->hci.hci_ops->hci_interface_reset(_s->hci.hci_ctrl); \
        })
#define SMAC_SYSPLF_RESET(_s, _r, _v)                      \
        ({ \
            _s->hci.hci_ops->hci_sysplf_reset(_s->hci.hci_ctrl, _r, _v); \
        })
#define SMAC_REG_CONFIRM(_s, _r, _v)                \
{                                                   \
    u32 _regval;                                    \
    SMAC_REG_READ(_s, _r, &_regval);                \
    if (_regval != (_v)) {                          \
        printk("ERROR!!Please check interface!\n"); \
        printk("[0x%08x]: 0x%08x!=0x%08x\n",        \
        (_r), (_v), _regval);                       \
        printk("SOS!SOS!\n"); 						\
        return -1;                                  \
    }                                               \
}    
#define SMAC_REG_SET_BITS(_sh, _reg, _set, _clr)    \
({                                                  \
    int ret;                                        \
    u32 _regval;                                    \
    ret = SMAC_REG_READ(_sh, _reg, &_regval);       \
    _regval &= ~(_clr);                             \
    _regval |= (_set);                              \
    if (ret == 0)                                   \
        ret = SMAC_REG_WRITE(_sh, _reg, _regval);   \
    ret;                                            \
})

#ifndef __x86_64
#define SMAC_RF_REG_SET_BITS(_sh, _reg, _set, _clr) SMAC_REG_SET_BITS(_sh, _reg, _set, _clr) 
#else           
#define SMAC_RF_REG_SET_BITS(_sh, _reg, _set, _clr) \
({                                                  \
    int ret;                                        \
    u32 _regval;                                    \
    ret = SMAC_REG_READ(_sh, _reg, &_regval);       \
    ret = SMAC_REG_READ(_sh, _reg, &_regval);       \
    _regval &= ~(_clr);                             \
    _regval |= (_set);                              \
    if (ret == 0)                                   \
        ret = SMAC_REG_WRITE(_sh, _reg, _regval);   \
    ret;                                            \
})
#endif

#endif // _SSV_REG_ACC_H_

