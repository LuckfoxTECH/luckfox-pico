/*****************************************************************************
**
**  Name:           utfc.h
**
**  Description:    UTF conversion utilities.
**
**  Copyright (c) 2003-2004, WIDCOMM Inc., All Rights Reserved.
**  WIDCOMM Bluetooth Core. Proprietary and confidential.
**
*****************************************************************************/
#ifndef UTFC_H
#define UTFC_H

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
**
** Function         utfc_16_to_8
**
** Description      Convert a UTF-16 array to a null-terminated UTF-8 string.
**                  Illegal characters are skipped.
**
** Returns          Length of UTF-8 string in bytes.
**
*******************************************************************************/
extern UINT16 utfc_16_to_8(UINT8 *p_utf8, UINT16 utf8_len, UINT16 *p_utf16, UINT16 utf16_len);

/*******************************************************************************
**
** Function         utfc_8_to_16
**
** Description      Convert a null-terminated UTF-8 string to a UTF-16 array.
**                  Illegal characters are skipped.  The UTF-16 array is
**                  appended with a zero (null) character.
**
** Returns          Length of UTF-16 array including null character.
**
*******************************************************************************/
extern UINT16 utfc_8_to_16(UINT16 *p_utf16, UINT16 utf16_len, UINT8 *p_utf8);

#ifdef __cplusplus
}
#endif

#endif /* UTFC_H */
