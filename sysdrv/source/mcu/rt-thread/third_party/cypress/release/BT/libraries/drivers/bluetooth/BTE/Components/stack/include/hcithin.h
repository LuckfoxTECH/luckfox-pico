/****************************************************************************/
/*                                                                          */
/*  Name        hcithin.h                                                   */
/*                                                                          */
/*  Function    This files contains MACRO definitions for use with the thin */
/*              HCI implmentation. Assumes consistant byte ordering between */
/*              upper and lower stacks. Also assumes that 16 bit values are */
/*              16 bit aligned.                                             */
/*                                                                          */
/*                                                                          */
/*  Copyright (c) 1999, 2000, 2001, Widcomm Inc., All Rights Reserved.      */
/*  Widcomm Bluetooth Core. Proprietary and confidential.                   */
/*                                                                          */
/****************************************************************************/

#ifndef HCITHIN_H
#define HCITHIN_H

#include "data_types.h"

/* Routines to copy data into an HCI buffer and increment the buffer pointer */
#define UINT8_TO_BUFFER(p, u8)  { *(p)++ = (UINT8)(u8);}
#define UINT16_TO_BUFFER(p,u16) { *(UINT16 *)(p) = u16; p += 2; }
#define UINT32_TO_BUFFER(p,u32) { *(UINT32 *)(p) = u32; p += 4; }
#define BDADDR_TO_BUFFER(p, a)  { memcpy((p),(a), sizeof(BD_ADDR)); p += sizeof(BD_ADDR); }
#define ARRAY16_TO_BUFFER(p,a)  { memcpy((p),(a), 16); p += 16; }
#define LAP_TO_BUFFER(p, a)     { memcpy((p),(a), LAP_LEN); p+= LAP_LEN; }
#define DEVCLASS_TO_BUFFER(p,a) { memcpy((p),(a), DEV_CLASS_LEN); p+= DEV_CLASS_LEN; }
#define ARRAY_TO_BUFFER(p,a,l)  { memcpy((p),(a), (l)); p += l; }

/* Routines to copy data out of an HCI buffer and increment the buffer pointer */
#define BUFFER_TO_UINT8(u8, p)  { u8  = *(p)++;}
#define BUFFER_TO_UINT16(u16,p) { u16 = *((UINT16 *)(p)); p += 2; }
#define BUFFER_TO_UINT32(u32,p) { u32 = *((UINT32 *)(p)); p += 4; }
#define BUFFER_TO_BDADDR(a,p)   { memcpy((a),(p), sizeof(BD_ADDR)); p += sizeof(BD_ADDR); }
#define BUFFER_TO_ARRAY16(a,p)  { memcpy((a),(p), 16); p += 16; }
#define BUFFER_TO_LAP(a, p)     { memcpy((a),(p), LAP_LEN); p+= LAP_LEN; }
#define BUFFER_TO_DEVCLASS(a,p) { memcpy((a),(p), DEV_CLASS_LEN); p += DEV_CLASS_LEN; }

#endif /* HCITHIN_H */

