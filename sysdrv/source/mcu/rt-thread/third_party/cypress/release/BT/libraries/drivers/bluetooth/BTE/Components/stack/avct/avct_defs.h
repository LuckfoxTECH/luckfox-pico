/*****************************************************************************
**
**  Name:           avct_defs.h
**
**  Description:    This contains constants definitions and other information
**                  from the AVCTP specification.  This file is intended for
**                  use internal to AVCT only.
**
**
**  Copyright (c) 2003-2004, WIDCOMM Inc., All Rights Reserved.
**  WIDCOMM Bluetooth Core. Proprietary and confidential.
**
*****************************************************************************/
#ifndef AVCT_DEFS_H
#define AVCT_DEFS_H

/*****************************************************************************
** constants
*****************************************************************************/

/* packet type */
#define AVCT_PKT_TYPE_SINGLE        0       /* single packet */
#define AVCT_PKT_TYPE_START         1       /* start packet */
#define AVCT_PKT_TYPE_CONT          2       /* continue packet */
#define AVCT_PKT_TYPE_END           3       /* end packet */

/* header lengths for different packet types */
#define AVCT_HDR_LEN_SINGLE         3
#define AVCT_HDR_LEN_START          4
#define AVCT_HDR_LEN_CONT           1
#define AVCT_HDR_LEN_END            1

/* invalid cr+ipid value */
#define AVCT_CR_IPID_INVALID        1

/*****************************************************************************
** message parsing and building macros
*****************************************************************************/

#define AVCT_BLD_HDR(p, label, type, cr_ipid) \
    *(p)++ = ((label) << 4) | ((type) << 2) | (cr_ipid);

#define AVCT_PRS_HDR(p, label, type, cr_ipid) \
    label = *(p) >> 4; \
    type = (*(p) >> 2) & 3; \
    cr_ipid = *(p)++ & 3;

#define AVCT_PRS_LAB_CR(p, label, cr_ipid) \
    label = *(p) >> 4; \
    cr_ipid = *(p)++ & 3;

#define AVCT_PRS_PKT_TYPE(p, type) \
    type = (*(p) >> 2) & 3;

#endif /* AVCT_DEFS_H */
