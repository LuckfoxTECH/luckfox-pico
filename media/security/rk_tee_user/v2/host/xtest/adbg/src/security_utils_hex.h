/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 */

#ifndef INCLUSION_GUARD_UTILS_HEX_H
#define INCLUSION_GUARD_UTILS_HEX_H
/*************************************************************************
* 1. Includes
*************************************************************************/

/*************************************************************************
* 2. Types, constants and external variables
*************************************************************************/

#if 0 /* Should format into a buffer instead */
/**
 **************************************************************************
 * Prints data hexadecimally to stdout
 *
 * Each line of binary data is printed as 66 characters on the format
 * shown below:
 * XX:XX:XX:XX XX:XX:XX:XX XX:XX:XX:XX XX:XX:XX:XX   xxxxxxxxxxxxxxxx
 *
 * @param [in] Prefix_p      String printed first each line, may be NULL
 *                           to indicate the empty string
 * @param [in] Postfix_p     String printed last each line, may be NULL
 *                           to indicate the empty string
 * @param [in] BufferName_p  Name of the buffer, if != NULL the name of the
 *                           buffer is printed together with the length of
 *                           the buffer before the data is printed
 * @param [in] Buffer_p      Pointer to a buffer holding the data
 * @param [in] BufferLength  Length of the buffer above
 *
 * @sigbased No
 * @waitmode Wait mode
 **************************************************************************/
void SecUtil_HexPrintBuffer(const char *Prefix_p, const char *Postfix_p,
			    const char *BufferName_p, const void *Buffer_p,
			    size_t BufferLength);
#endif

#if 0
/**
 **************************************************************************
 * Prints data hexadecimally in a format suitable for initialization of
 * an array in c.
 *
 * 0xXX,0xXX,0xXX,0xXX,0xXX,0xXX,0xXX,0xXX,0xXX,0xXX,0xXX,0xXX,0xXX,0xXX,
 * 0xXX,0xXX,
 *
 * @param [in] Prefix_p      String printed first each line, may be NULL
 *                           to indicate the empty string
 * @param [in] Postfix_p     String printed last each line, may be NULL
 *                           to indicate the empty string
 * @param [in] Buffer_p      Pointer to a buffer holding the data
 * @param [in] BufferLength  Length of the buffer above
 *
 * @sigbased No
 * @waitmode Wait mode
 **************************************************************************/
void SecUtil_CHexPrintBuffer(const char *Prefix_p, const char *Postfix_p,
			     const void *Buffer_p, size_t BufferLength);
#endif

/**************************************************************************
* Specifies how hexadecial data should be formated
*
* @param BytePrefix_p           ASCII string prefix for each hexadecimal
*                               byte printed (for example "0x"). If NULL
*                               nothing is inserted in front of a byte.
* @param ByteSeparator_p        ASCII string to insert between each printed
*                               hexadecimal byte. If NULL a ":" is inserted
*                               between each hexadecial byte.
* @param GroupSeparator_p       ASCII string to insert instead of
*                               ByteSeparator_p between each group
*                               of printed hexadecimal bytes. If NULL " "
*                               is used as group separator.
**************************************************************************/
typedef struct {
	const char *BytePrefix_p;
	const char *ByteSeparator_p;
	const char *GroupSeparator_p;
} SecUtil_HexFormat_t;

/*************************************************************************
* 3. Functions
*************************************************************************/

/****************************************************************************
 * Formats data hexadecimally into an NULL terminated ASCII string in
 * the format below.
 *
 * 0xXX:0xXX:0xXX:0xXX 0xXX:0xXX:0xXX:0xXX
 * 0xXX:0xXX:0xXX:0xXX 0xXX:0xXX:0xXX:0xXX
 *
 * The colons and spaces in the result can be cusomized with ByteSeparator_p
 * and GroupSeparator_p respectivly.
 *
 * @param [in]     Buffer_p          Pointer to a buffer holding the data
 * @param [in]     BufferLength      Length of the buffer above
 * @param [in]     HexFormat_p       How the data should be formatted
 * @param [in, out] Destination_p     Output ASCII buffer
 * @param [in]     DestinationLength Length of output buffer
 *
 * @returns  The size of a needed DestinationLength, if greater than supplied
 *           DestinationLength the result in Destination_p is truncated but
 *           still NULL terminated.
 * @sigbased No
 * @waitmode Wait mode
 **************************************************************************/

size_t SecUtil_BufferToHex(const void *const Buffer_p, size_t BufferLength,
			   const SecUtil_HexFormat_t *const HexFormat_p,
			   char *const Destination_p,
			   const size_t DestinationLength);

#endif /*INCLUSION_GUARD_UTILS_HEX_H*/
