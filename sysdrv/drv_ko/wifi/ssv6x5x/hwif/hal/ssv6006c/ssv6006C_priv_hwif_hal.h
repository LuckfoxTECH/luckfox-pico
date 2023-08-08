/*
 * Copyright (c) 2015 iComm Semiconductor Ltd.
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

#ifndef _SSV6006_PRIV_LHAL_H_
#define _SSV6006_PRIV_LHAL_H_

// Macros for predefined register access.
#define REG32(_addr)    REG32_R(_addr)
#define REG32_R(_addr)   ({ u32 reg; ssv_hwif_read_reg(priv, _addr, &reg); reg;})
#define REG32_W(_addr, _value)   do { ssv_hwif_write_reg(priv, _addr, _value); } while (0)

#define LHAL_REG_SET_BITS(priv, _reg, _set, _clr)    \
({                                                  \
    int ret;                                        \
    u32 _regval;                                    \
    ret = ssv_hwif_read_reg(priv, _reg, &_regval);       \
    _regval &= ~(_clr);                             \
    _regval |= (_set);                              \
    if (ret == 0)                                   \
        ret = ssv_hwif_write_reg(priv, _reg, _regval);   \
    ret;                                            \
})

#define LHAL_CMD52_REG_SET_BITS(priv, _reg, _set, _clr)    \
({                                                  \
    int ret;                                        \
    u32 _regval;                                    \
    ret = ssv_hwif_cmd52_read_reg(priv, _reg, &_regval);       \
    _regval &= ~(_clr);                             \
    _regval |= (_set);                              \
    if (ret == 0)                                   \
        ret = ssv_hwif_cmd52_write_reg(priv, _reg, _regval);   \
    if (ret == 0)                                   \
    ret = ssv_hwif_cmd52_read_reg(priv, _reg, &_regval);       \
    ret;                                            \
})

static inline int ssv_hwif_read_reg (struct ssv6xxx_platform_data **priv, u32 addr, u32 *val)
{
    struct device *dev = container_of((void **)priv, struct device, platform_data);
    return (*priv)->ops->readreg(dev, addr, val);
}

static inline int ssv_hwif_write_reg (struct ssv6xxx_platform_data **priv, u32 addr, u32 val)
{
    struct device *dev = container_of((void **)priv, struct device, platform_data);
    return (*priv)->ops->writereg(dev, addr, val);
}

static inline int ssv_hwif_cmd52_read_reg (struct ssv6xxx_platform_data **priv, u32 addr, u32 *val)
{
    struct device *dev = container_of((void **)priv, struct device, platform_data);
    if (((*priv)->ops->property(dev) & SSV_HWIF_INTERFACE_MASK) == SSV_HWIF_INTERFACE_SDIO) {
        return (*priv)->ops->cmd52_read(dev, addr, val);
    } else {
        *val = 0;
        return -1;
    }
}

static inline int ssv_hwif_cmd52_write_reg (struct ssv6xxx_platform_data **priv, u32 addr, u32 val)
{
    struct device *dev = container_of((void **)priv, struct device, platform_data);
    if (((*priv)->ops->property(dev) & SSV_HWIF_INTERFACE_MASK) == SSV_HWIF_INTERFACE_SDIO) {
        return (*priv)->ops->cmd52_write(dev, addr, val);
    } else {
        return -1;
    }
}

#endif // _SSV6006_PRIV_LHAL_H_
