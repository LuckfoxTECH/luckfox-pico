/*
 * (C) Copyright 2013
 * David Feng <fenghua@phytium.com.cn>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <linux/compiler.h>
#include <efi_loader.h>
#include <iomem.h>
#include <stacktrace.h>

DECLARE_GLOBAL_DATA_PTR;

#if !CONFIG_IS_ENABLED(IRQ)
int interrupt_init(void)
{
	return 0;
}

void enable_interrupts(void)
{
	return;
}

int disable_interrupts(void)
{
	return 0;
}
#endif

#define REG_BITS(val, shift, mask)	(((val) >> (shift)) & (mask))

#ifndef CONFIG_SPL_BUILD
void show_regs(struct pt_regs *regs)
{
	int el = current_el();
	int i;

	const char *esr_bits_ec[] = {
		[0]  = "an unknown reason",
		[1]  = "a WFI or WFE instruction",
		[3]  = "an MCR or MRC access",
		[4]  = "an MCRR or MRRC access",
		[5]  = "an MCR or MRC access",
		[6]  = "an LDC or STC access to CP14",
		[7]  = "an access to an Advanced SIMD or floating-point register, resulting from CPACR_EL1.FPEN or CPTR_ELx.TFP",
		[8]  = "an MCR or MRC access",
		[12] = "an MCRR or MRRC access",
		[14] = "an Illegal execution state, or a PC or SP alignment fault",
		[10] = "HVC or SVC instruction execution",
		[18] = "HVC or SVC instruction execution",
		[19] = "SMC instruction execution in AArch32 state",
		[21] = "HVC or SVC instruction execution",
		[22] = "HVC or SVC instruction execution",
		[23] = "SMC instruction execution in AArch64 state",
		[24] = "MSR, MRS, or System instruction execution in AArch64 state",
		[31] = "IMPLEMENTATION DEFINED exception to EL3",
		[32] = "an Instruction abort",
		[33] = "an Instruction abort",
		[34] = "an Illegal execution state, or a PC or SP alignment fault",
		[36] = "a Data abort, from lower exception level",
		[37] = "a Data abort, from current exception level",
		[38] = "an Illegal execution state, or a PC or SP alignment fault",
		[40] = "a trapped Floating-point exception",
		[44] = "a trapped Floating-point exception",
		[47] = "SError interrupt",
		[48] = "a Breakpoint or Vector Catch debug event",
		[49] = "a Breakpoint or Vector Catch debug event",
		[50] = "a Software Step debug event",
		[51] = "a Software Step debug event",
		[52] = "a Watchpoint debug event",
		[53] = "a Watchpoint debug event",
		[56] = "execution of a Software Breakpoint instructio",
	};

	printf("\n");

	/* PC/LR/SP ... */
	printf("* Reason:        Exception from %s\n", esr_bits_ec[REG_BITS(regs->esr, 26, 0x3f)]);
	if (gd->flags & GD_FLG_RELOC) {
		printf("* PC         =   %016lx\n", regs->elr - gd->reloc_off);
		printf("* LR         =   %016lx\n", regs->regs[30] - gd->reloc_off);
	} else {
		printf("* ELR(PC)    =   %016lx\n", regs->elr);
		printf("* LR         =   %016lx\n", regs->regs[30]);
	}
	printf("* SP         =   %016lx\n", regs->sp);
	printf("* ESR_EL%d    =   %016lx\n", el, regs->esr);
	printf("* Reloc Off  =   %016lx\n\n", gd->reloc_off);

	/* CPU */
	for (i = 0; i < 29; i += 2)
		printf("x%-2d: %016lx x%-2d: %016lx\n",
		       i, regs->regs[i], i+1, regs->regs[i+1]);
	printf("\n");

	/* SoC */
#ifdef CONFIG_ROCKCHIP_CRASH_DUMP
	iomem_show_by_compatible("-cru", 0, 0x400);
	iomem_show_by_compatible("-pmucru", 0, 0x400);
	iomem_show_by_compatible("-grf", 0, 0x400);
	iomem_show_by_compatible("-pmugrf", 0, 0x400);
#endif
	/* Call trace */
	dump_core_stack(regs);
}

#else
void show_regs(struct pt_regs *regs)
{
	int i;

	if (gd->flags & GD_FLG_RELOC) {
		printf("ELR:     %lx\n", regs->elr - gd->reloc_off);
		printf("LR:      %lx\n", regs->regs[30] - gd->reloc_off);
	} else {
		printf("ELR:     %lx\n", regs->elr);
		printf("LR:      %lx\n", regs->regs[30]);
	}

	printf("ESR:     %lx (ec=%ld)\n", regs->esr, REG_BITS(regs->esr, 26, 0x3f));

	for (i = 0; i < 29; i += 2)
		printf("x%-2d: %016lx x%-2d: %016lx\n",
		       i, regs->regs[i], i+1, regs->regs[i+1]);
	printf("\n");

	dump_core_stack(regs);
}
#endif

/*
 * do_bad_sync handles the impossible case in the Synchronous Abort vector.
 */
void do_bad_sync(struct pt_regs *pt_regs, unsigned int esr)
{
	efi_restore_gd();
	printf("Bad mode in \"Synchronous Abort\" handler, esr 0x%08x\n", esr);
	show_regs(pt_regs);
	panic("Resetting CPU ...\n");
}

/*
 * do_bad_irq handles the impossible case in the Irq vector.
 */
void do_bad_irq(struct pt_regs *pt_regs, unsigned int esr)
{
	efi_restore_gd();
	printf("Bad mode in \"Irq\" handler, esr 0x%08x\n", esr);
	show_regs(pt_regs);
	panic("Resetting CPU ...\n");
}

/*
 * do_bad_fiq handles the impossible case in the Fiq vector.
 */
void do_bad_fiq(struct pt_regs *pt_regs, unsigned int esr)
{
	efi_restore_gd();
	printf("Bad mode in \"Fiq\" handler, esr 0x%08x\n", esr);
	show_regs(pt_regs);
	panic("Resetting CPU ...\n");
}

/*
 * do_bad_error handles the impossible case in the Error vector.
 */
void do_bad_error(struct pt_regs *pt_regs, unsigned int esr)
{
	efi_restore_gd();
	printf("Bad mode in \"Error\" handler, esr 0x%08x\n", esr);
	show_regs(pt_regs);
	panic("Resetting CPU ...\n");
}

/*
 * do_sync handles the Synchronous Abort exception.
 */
void do_sync(struct pt_regs *pt_regs, unsigned int esr)
{
	efi_restore_gd();
	printf("\"Synchronous Abort\" handler, esr 0x%08x\n", esr);
	show_regs(pt_regs);
	panic("Resetting CPU ...\n");
}

#if !CONFIG_IS_ENABLED(IRQ)
/*
 * do_irq handles the Irq exception.
 */
void do_irq(struct pt_regs *pt_regs, unsigned int esr)
{
	efi_restore_gd();
	printf("\"Irq\" handler, esr 0x%08x\n", esr);
	show_regs(pt_regs);
	panic("Resetting CPU ...\n");
}
#endif

/*
 * do_fiq handles the Fiq exception.
 */
void do_fiq(struct pt_regs *pt_regs, unsigned int esr)
{
	efi_restore_gd();
	printf("\"Fiq\" handler, esr 0x%08x\n", esr);
	show_regs(pt_regs);
	panic("Resetting CPU ...\n");
}

/*
 * do_error handles the Error exception.
 * Errors are more likely to be processor specific,
 * it is defined with weak attribute and can be redefined
 * in processor specific code.
 */
void __weak do_error(struct pt_regs *pt_regs, unsigned int esr)
{
	efi_restore_gd();
	printf("\"Error\" handler, esr 0x%08x\n", esr);
	show_regs(pt_regs);
	panic("Resetting CPU ...\n");
}
