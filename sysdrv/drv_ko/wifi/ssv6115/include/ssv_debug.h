/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/
#ifndef __SSV_DEBUG_H__
#define __SSV_DEBUG_H__

#include "ssv_cfg.h"

/* driver log level*/
enum {
	SSV_DRIVER_LOG_LEVEL_NONE = 0,
	SSV_DRIVER_LOG_LEVEL_ERR = 1,
	SSV_DRIVER_LOG_LEVEL_INFO = 2,
	SSV_DRIVER_LOG_LEVEL_DEBUG = 3
};

// #define DRIVER_PREFIX "SSV: "


#define SSV_LOG_INFO_DUMP(_TitleString, _HexData, _HexDataLen) do {} while (0)
#define SSV_LOG_DBG_DUMP(_TitleString, _HexData, _HexDataLen) do {} while (0)

#ifdef SSV_NO_LOG

#undef SSV_LOG_ERR
#define SSV_LOG_ERR(fmt, arg...)
#undef SSV_LOG_INFO
#define SSV_LOG_INFO(fmt, arg...)
#undef SSV_LOG_DBG
#define SSV_LOG_DBG(fmt, arg...) 
#undef SSV_LOG_DBG_ONE_LINE
#define SSV_LOG_DBG_ONE_LINE(fmt, arg...)

#else /*ifdef SSV_NO_LOG*/

#ifdef SSV_DEBUG_LOG

extern struct ssv6xxx_cfg ssv_cfg;

#undef SSV_LOG_ERR
#define SSV_LOG_ERR(fmt, arg...)     \
	do {\
		if (SSV_DRIVER_LOG_LEVEL_ERR <= ssv_cfg.dbg_level) {\
			printk(KERN_ERR fmt, ##arg);\
		} \
	} while (0)

#undef SSV_LOG_INFO
#define SSV_LOG_INFO(fmt, arg...)     \
	do {\
		if (SSV_DRIVER_LOG_LEVEL_INFO <= ssv_cfg.dbg_level) {\
			printk(KERN_ERR fmt, ##arg);\
		} \
	} while (0)

#undef SSV_LOG_DBG
#define SSV_LOG_DBG(fmt, arg...)     \
	do {\
		if (SSV_DRIVER_LOG_LEVEL_DEBUG <= ssv_cfg.dbg_level) {\
			printk(KERN_ERR fmt, ##arg);\
		} \
	} while (0)

#undef SSV_LOG_DBG_ONE_LINE
#define SSV_LOG_DBG_ONE_LINE(fmt, arg...)     \
	do {\
		if (SSV_DRIVER_LOG_LEVEL_DEBUG <= ssv_cfg.dbg_level) {\
			printk(KERN_CONT fmt, ##arg);\
		} \
	} while (0)

#ifdef SSV_DEBUG_LOG_DATA_DUMP
#undef SSV_LOG_DBG_DUMP
#define SSV_LOG_DBG_DUMP(_TitleString, _HexData, _HexDataLen)			\
	if (SSV_DRIVER_LOG_LEVEL_DEBUG <= ssv_cfg.dbg_level) { \
		int __i = 0;								\
		u8	*ptr = (u8 *)_HexData;				\
		printk(KERN_ERR _TitleString "\n");		\
		for (__i = 0; __i<(int)_HexDataLen; __i++)				\
		{			\
			if (((__i) % 16) == 0)	printk(KERN_CONT "0x%p : ", &ptr[__i]);	\
			printk(KERN_CONT "%02X%s", ptr[__i], (((__i + 1) % 4) == 0) ? "  " : " ");	\
			if (((__i + 1) % 16) == 0)	printk(KERN_ERR "\n");			\
		}								\
		printk(KERN_ERR "\n");							\
	}
#endif /* SSV_LOG_DATA_DUMP */

#else //#ifdef SSV_DEBUG_LOG

#undef SSV_LOG_ERR
#define SSV_LOG_ERR(fmt, arg...)	printk(KERN_ERR fmt, ##arg)
#undef SSV_LOG_INFO
#define SSV_LOG_INFO(fmt, arg...)	printk(KERN_INFO fmt, ##arg)
#undef SSV_LOG_DBG
#define SSV_LOG_DBG(fmt, arg...)	printk(KERN_ERR fmt, ##arg)
#undef SSV_LOG_DBG_ONE_LINE
#define SSV_LOG_DBG_ONE_LINE(fmt, arg...) printk(KERN_CONT fmt, ##arg)

#endif /* SSV_DEBUG_LOG */
#endif /* SSV_NO_LOG */


#endif /* __SSV_DEBUG_H__ */
