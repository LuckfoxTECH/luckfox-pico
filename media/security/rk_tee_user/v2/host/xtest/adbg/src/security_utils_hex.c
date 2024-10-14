// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 */

/*************************************************************************
* 1. Includes
*************************************************************************/
#include <sys/param.h>
#include <stdint.h>
#include <stdint.h>
#include <string.h>

#include "security_utils_hex.h"

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif


/*************************************************************************
* 2. Definition of external constants and variables
*************************************************************************/

/*************************************************************************
* 3. File scope types, constants and variables
*************************************************************************/
static const char SecUtil_HexChars[] = {
	'0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};
/*************************************************************************
* 4. Declaration of file local functions
*************************************************************************/

/*************************************************************************
* 5. Definition of external functions
*************************************************************************/
#if 0
void SecBase_HexPrintBuffer(
	const char *Prefix_p,
	const char *Postfix_p,
	const char *BufferName_p,
	const void *Buffer_p,
	size_t BufferLength
	)
{
	char LineBuf[67];
	size_t LinePos = 0;
	const uint8 *Data_p = Buffer_p;
	size_t n;
	char EmptyString[] = "";

	if (Prefix_p == NULL)
		Prefix_p = EmptyString;
	if (Postfix_p == NULL)
		Postfix_p = EmptyString;

	if (BufferName_p != NULL) {
		DELAY_MS(5);
		printf("%s%s: Size: %d%s", Prefix_p, BufferName_p, BufferLength,
		       Postfix_p);
	}

	/* Initialize the line buffer for a new line */
	SEC_MEM_SET(LineBuf, ' ', sizeof(LineBuf));
	LineBuf[sizeof(LineBuf) - 1] = '\0';
	LinePos = 0;
	for (n = 0; n < BufferLength; n++) {
		char Separator;
		char PrintableChar;

		if (((n + 1) % 4) == 0)
			Separator = ' ';
		else
			Separator = ':';

		if (isprint(Data_p[n]))
			PrintableChar = (char)Data_p[n];
		else
			PrintableChar = '.';

		LineBuf[LinePos * 3 + 0] = SecBase_HexChars[Data_p[n] >> 4];
		LineBuf[LinePos * 3 + 1] = SecBase_HexChars[Data_p[n] & 0x0F];
		LineBuf[LinePos * 3 + 2] = Separator;
		LineBuf[50 + LinePos] = PrintableChar;

		LinePos++;
		if (LinePos == 16) {
			DELAY_MS(5);
			printf("%s%s%s", Prefix_p, LineBuf, Postfix_p);
			/* Initialize the line buffer for a new line */
			SEC_MEM_SET(LineBuf, ' ', sizeof(LineBuf));
			LineBuf[sizeof(LineBuf) - 1] = '\0';
			LinePos = 0;
		}
	}
	if (LinePos != 0) {
		DELAY_MS(5);
		printf("%s%s%s", Prefix_p, LineBuf, Postfix_p);
	}
	(void)fflush(stdout);
}
#endif

#if 0
void SecUtil_CHexPrintBuffer(
	const char *Prefix_p,
	const char *Postfix,
	const void *Buffer_p,
	size_t BufferLength
	)
{
	char LineBuf[84];
	const uint8 *Data_p = Buffer_p;
	size_t n;
	char EmptyString[] = "";
	static const SecUtil_HexFormat_t HexFormat = { "0x", ", ", ", " };

	if (Prefix_p == NULL)
		Prefix_p = EmptyString;
	if (Postfix == NULL)
		Postfix = EmptyString;

	for (n = 0; n < BufferLength; n += 16) {
		(void)SecUtil_BufferToHex(Data_p + n, MIN(16, BufferLength - n),
					  &HexFormat,
					  LineBuf, sizeof(LineBuf));
		DELAY_MS(5);
		printf("%s%s%s", Prefix_p, LineBuf, Postfix);
	}
}
#endif

size_t SecUtil_BufferToHex(
	const void *const Buffer_p,
	size_t BufferLength,
	const SecUtil_HexFormat_t *const HexFormat_p,
	char *const Destination_p,
	const size_t DestinationLength
	)
{
	const uint8_t *Data_p = Buffer_p;
	size_t UsedDestLength = 0;
	size_t n = 0;
	const char *ByteSeparator_p = NULL;
	const char *GroupSeparator_p = NULL;
	const char *BytePrefix_p = NULL;
	size_t BytePrefixLength = 0;

	if (DestinationLength > 1)
		Destination_p[0] = '\0';

	UsedDestLength = 1;

	if (HexFormat_p != NULL) {
		BytePrefix_p = HexFormat_p->BytePrefix_p;
		ByteSeparator_p = HexFormat_p->ByteSeparator_p;
		GroupSeparator_p = HexFormat_p->GroupSeparator_p;
	}

	if (BytePrefix_p == NULL)
		BytePrefix_p = "";

	BytePrefixLength = strlen(BytePrefix_p);

	if (ByteSeparator_p == NULL)
		ByteSeparator_p = ":";

	if (GroupSeparator_p == NULL)
		GroupSeparator_p = " ";

	/*
	 * This for loop is unnecessarily complicated due to
	 * the absense of both snprintf and strlcat
	 */
	for (n = 0; n < BufferLength; n++) {
		const char *Separator_p = NULL;
		size_t SeparatorLength = 0;

		/* Establish separator for this byte and the next */
		if (n == BufferLength - 1)
			Separator_p = "";
		else if ((n + 1) % 4 == 0)
			Separator_p = GroupSeparator_p;
		else
			Separator_p = ByteSeparator_p;

		SeparatorLength = strlen(Separator_p);

		/* Insert the Byte prefix */
		if (UsedDestLength < DestinationLength) {
			size_t CopyLength = 0;

			CopyLength = MIN(BytePrefixLength,
					 DestinationLength - UsedDestLength);
			memcpy(Destination_p + UsedDestLength - 1, BytePrefix_p,
			       CopyLength);
			Destination_p[UsedDestLength - 1 + CopyLength] = '\0';
		}
		UsedDestLength += BytePrefixLength;

		/* Insert the first nibble of the ASCII hexadecimal byte */
		if (UsedDestLength < DestinationLength) {
			Destination_p[UsedDestLength -
				      1] = SecUtil_HexChars[Data_p[n] >> 4];
			Destination_p[UsedDestLength] = '\0';
		}
		UsedDestLength++;

		/* Insert the second nibble of the ASCII hexadecimal byte */
		if (UsedDestLength < DestinationLength) {
			Destination_p[UsedDestLength -
				      1] = SecUtil_HexChars[Data_p[n] & 0x0F];
			Destination_p[UsedDestLength] = '\0';
		}
		UsedDestLength++;

		/* Insert the separator */
		if (UsedDestLength < DestinationLength) {
			size_t CopyLength = 0;

			CopyLength = MIN(SeparatorLength,
					 DestinationLength - UsedDestLength);
			memcpy(Destination_p + UsedDestLength - 1, Separator_p,
			       CopyLength);
			Destination_p[UsedDestLength - 1 + CopyLength] = '\0';
		}
		UsedDestLength += SeparatorLength;
	}
	return UsedDestLength;
}

/*************************************************************************
* 6. Definition of internal functions
*************************************************************************/
