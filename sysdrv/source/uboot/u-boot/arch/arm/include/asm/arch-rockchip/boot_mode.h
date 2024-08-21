#ifndef __REBOOT_MODE_H
#define __REBOOT_MODE_H

/* high 24 bits is tag, low 8 bits is type */
#define REBOOT_FLAG		0x5242C300
/* cold boot */
#define BOOT_COLD		0
/* normal boot */
#define BOOT_NORMAL		(REBOOT_FLAG + 0)
/* enter loader rockusb mode */
#define BOOT_LOADER		(REBOOT_FLAG + 1)
/* enter recovery */
#define BOOT_RECOVERY		(REBOOT_FLAG + 3)
/* reboot by panic */
#define BOOT_PANIC		(REBOOT_FLAG + 7)
/* reboot by watchdog */
#define BOOT_WATCHDOG		(REBOOT_FLAG + 8)
/* enter fastboot mode */
#define BOOT_FASTBOOT		(REBOOT_FLAG + 9)
/* enter charging mode */
#define BOOT_CHARGING		(REBOOT_FLAG + 11)
/* enter usb mass storage mode */
#define BOOT_UMS		(REBOOT_FLAG + 12)
/* enter dfu download mode */
#define BOOT_DFU                (REBOOT_FLAG + 13)
/* reboot system quiescent */
#define BOOT_QUIESCENT		(REBOOT_FLAG + 14)
/* enter bootrom download mode */
#define BOOT_BROM_DOWNLOAD	0xEF08A53C

#ifndef __ASSEMBLY__
int setup_boot_mode(void);
#endif

enum {
	BCB_MSG_RECOVERY_NONE,
	BCB_MSG_RECOVERY_RK_FWUPDATE,
	BCB_MSG_RECOVERY_PCBA,
};

#endif
