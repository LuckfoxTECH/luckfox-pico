/*
 * (C) Copyright 2000-2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * Misc boot support
 */
#include <common.h>
#include <command.h>
#include <net.h>
#include <asm/io.h>
#include <asm/arch/boot_mode.h>

#ifdef CONFIG_CMD_GO

/* Allow ports to override the default behavior */
__attribute__((weak))
unsigned long do_go_exec(ulong (*entry)(int, char * const []), int argc,
				 char * const argv[])
{
#ifdef CONFIG_CPU_V7
	ulong addr = (ulong)entry | 1;
	entry = (void *)addr;
#endif
	return entry (argc, argv);
}

static int do_go(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	ulong	addr, rc;
	int     rcode = 0;

	if (argc < 2)
		return CMD_RET_USAGE;

	addr = simple_strtoul(argv[1], NULL, 16);

	printf ("## Starting application at 0x%08lX ...\n", addr);

	/*
	 * pass address parameter as argv[0] (aka command name),
	 * and all remaining args
	 */
	rc = do_go_exec ((void *)addr, argc - 1, argv + 1);
	if (rc != 0) rcode = 1;

	printf ("## Application terminated, rc = 0x%lX\n", rc);
	return rcode;
}

/* -------------------------------------------------------------------- */

U_BOOT_CMD(
	go, CONFIG_SYS_MAXARGS, 1,	do_go,
	"start application at address 'addr'",
	"addr [arg ...]\n    - start application at address 'addr'\n"
	"      passing 'arg' as arguments"
);
#endif

static int do_reboot_brom(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	writel(BOOT_BROM_DOWNLOAD, CONFIG_ROCKCHIP_BOOT_MODE_REG);
	do_reset(NULL, 0, 0, NULL);

	return 0;
}

U_BOOT_CMD_ALWAYS(
	rbrom, 1, 0,	do_reboot_brom,
	"Perform RESET of the CPU",
	""
);

U_BOOT_CMD(
	reset, 2, 0,    do_reset,
	"Perform RESET of the CPU",
	""
);

U_BOOT_CMD(
        reboot, 2, 0,    do_reset,
        "Perform RESET of the CPU, alias of 'reset'",
        ""
);

#ifdef CONFIG_CMD_POWEROFF
U_BOOT_CMD(
	poweroff, 1, 0,	do_poweroff,
	"Perform POWEROFF of the device",
	""
);
#endif
