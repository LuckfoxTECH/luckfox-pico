/*
 * Copyright (c) 2013 The Chromium OS Authors.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <initcall.h>
#include <efi.h>

DECLARE_GLOBAL_DATA_PTR;

#define TICKS_TO_US(ticks)	((ticks) / (COUNTER_FREQUENCY / 1000000))
#define US_TO_MS(ticks)		((ticks) / 1000)
#define US_TO_US(ticks)		((ticks) % 1000)

#ifdef DEBUG
static inline void call_get_ticks(ulong *ticks) { *ticks = get_ticks(); }
#else
static inline void call_get_ticks(ulong *ticks) { }
#endif

int initcall_run_list(const init_fnc_t init_sequence[])
{
	const init_fnc_t *init_fnc_ptr;
	ulong start = 0, end = 0, sum = 0;

	if (!gd->sys_start_tick)
		gd->sys_start_tick = get_ticks();

	for (init_fnc_ptr = init_sequence; *init_fnc_ptr; ++init_fnc_ptr) {
		unsigned long reloc_ofs = 0;
		int ret;

		if (gd->flags & GD_FLG_RELOC)
			reloc_ofs = gd->reloc_off;
#ifdef CONFIG_EFI_APP
		reloc_ofs = (unsigned long)image_base;
#endif
		debug("initcall: %p", (char *)*init_fnc_ptr - reloc_ofs);
		if (gd->flags & GD_FLG_RELOC)
			debug(" (relocated to %p)\n", (char *)*init_fnc_ptr);
		else
			debug("\n");
		call_get_ticks(&start);
		ret = (*init_fnc_ptr)();
		call_get_ticks(&end);

		if (start != end) {
			sum = TICKS_TO_US(end - gd->sys_start_tick);
			debug("\t\t\t\t\t\t\t\t#%8ld us #%4ld.%3ld ms\n",
			      TICKS_TO_US(end - start), US_TO_MS(sum), US_TO_US(sum));
		}
		if (ret) {
			printf("initcall sequence %p failed at call %p (err=%d)\n",
			       init_sequence,
			       (char *)*init_fnc_ptr - reloc_ofs, ret);
			return -1;
		}
	}
	return 0;
}
