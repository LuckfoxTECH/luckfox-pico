/****************************************************************************
**
**  Name        gki_inet.h
**
**  Function    This file contains macros and interfaces for host-to-network
**              conversions used in Internet operations.
**
**
**  Copyright (c) 2002-2004, WIDCOMM Inc., All Rights Reserved.
**  Proprietary and confidential.
**
*****************************************************************************/
#ifndef GKI_INET_H
#define GKI_INET_H

#include "data_types.h"

#define htons   ntohs
#define htonl   ntohl

#define htonets nettohs
#define htonetl nettohl

#if BIG_ENDIAN == TRUE
#define ntohs(n) (n)
#define ntohl(n) (n)
#define ntoh6(n) (n)

#define nettohs(n) (n)
#define nettohl(n) (n)
#else
extern UINT16 ntohs(UINT16 n);
extern UINT32 ntohl(UINT32 n);
extern UINT8 *ntoh6(UINT8 *p);

#define nettohs(n) ((UINT16)((((n) << 8) & 0xff00) | (((n) >> 8) & 0x00ff)))
#define nettohl(n) ((((n) & 0x000000ff) << 24) | (((n) << 8) & 0x00ff0000) | \
                   (((n) >> 8) & 0x0000ff00) | (((n) >> 24) & 0x000000ff))
#endif

#endif /* GKI_INET_H */

