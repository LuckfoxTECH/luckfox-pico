/*
 * Miscellaneous useful definitions
 *
 * ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is the MPEG TS, PS and ES tools.
 *
 * The Initial Developer of the Original Code is Amino Communications Ltd.
 * Portions created by the Initial Developer are Copyright (C) 2008
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Amino Communications Ltd, Swavesey, Cambridge UK
 *
 * ***** END LICENSE BLOCK *****
 */

#ifndef _misc_defns
#define _misc_defns

#include "tswrite_defns.h"
#include "video_defns.h"

// Some (internal) functions find it convenient to have a union of the
// possible output streams. Rather than duplicate the definition of these,
// we put them here...
union _writer {
	FILE *es_output;       // output to an ES file
	TS_writer_p ts_output; // output via a TS writer
};
typedef union _writer WRITER;

// In the programs that handle command lines, it's useful to have a simple
// macro for checking the presence of subsidiary arguments.
// Assumes that argc and argv have their normal names.
#define CHECKARG(program, argno)                                                                   \
	if ((argno) + 1 == argc) {                                                                     \
		fprintf(stderr, "### %s: missing argument to %s\n", program, argv[(argno)]);               \
		return 1;                                                                                  \
	}

// A simple macro to return a bit from a bitfield, for use in printf()
#define ON(byt, msk) ((byt & msk) ? 1 : 0)

#endif // _misc_defns

// Local Variables:
// tab-width: 8
// indent-tabs-mode: nil
// c-basic-offset: 2
// End:
// vim: set tabstop=8 shiftwidth=2 expandtab:
