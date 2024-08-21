/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2018-2020, Linaro Limited
 */

#ifndef LIBCKTEEC_CK_DEBUG_H
#define LIBCKTEEC_CK_DEBUG_H

#include <pkcs11.h>

/* Return a pointer to a string buffer of "CKR_xxx\0" return value ID */
const char *ckr2str(CK_RV id);

/* ckm2str - Return string buffer of "CKM_xxx\0" for a mechanism ID */
const char *ckm2str(CK_MECHANISM_TYPE id);

/* slot_ckf2str - Return string buffer of "CKF_xxx\0" for a slot flag */
const char *slot_ckf2str(CK_ULONG flag);

/* token_ckf2str - Return string buffer "CKF_xxx\0" for a token flag */
const char *token_ckf2str(CK_ULONG flag);

/* mecha_ckf2str - Return string buffer "CKF_xxx\0" for a mechanism flag */
const char *mecha_ckf2str(CK_ULONG flag);

/* session_ckf2str - Return string buffer "CKF_xxx\0" for a session flag */
const char *session_ckf2str(CK_ULONG flag);

/* session_cks2str - Return string buffer "CKS_xxx\0" for a session state */
const char *session_cks2str(CK_ULONG flag);

/* Return a pointer to a string buffer of "CKA_xxx\0" attribute ID */
const char *cka2str(CK_ATTRIBUTE_TYPE id);

/* Return a pointer to a string buffer of "CKO_xxx\0" object class ID */
const char *cko2str(CK_OBJECT_CLASS id);

/* Return a pointer to a string buffer of "CKK_xxx\0" key type ID */
const char *ckk2str(CK_KEY_TYPE id);

#endif /*LIBCKTEEC_CK_DEBUG_H*/
