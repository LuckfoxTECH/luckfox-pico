/*
 * (C) Copyright 2008 - 2009
 * Windriver, <www.windriver.com>
 * Tom Rix <Tom.Rix@windriver.com>
 *
 * Copyright 2011 Sebastian Andrzej Siewior <bigeasy@linutronix.de>
 *
 * Copyright 2014 Linaro, Ltd.
 * Rob Herring <robh@kernel.org>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#ifndef _FASTBOOT_H_
#define _FASTBOOT_H_

/* The 64 defined bytes plus \0 */
#define FASTBOOT_RESPONSE_LEN (64 + 1)

/* Fastboot getvar variables */
typedef enum {
	FB_VERSION,
	FB_BOOTLOADER_VERSION,
	FB_BASEBAND_VERSION,
	FB_PRODUCT,
	FB_SERIAL_NO,
	FB_SECURE,
	FB_DWNLD_SIZE,
	FB_BLK_SIZE,
	FB_ERASE_SIZE,
	FB_PART_TYPE,
	FB_PART_SIZE,
	FB_UNLOCKED,
	FB_VARIANT,
	FB_OFF_MODE_CHARGE,
	FB_BATT_VOLTAGE,
	FB_BATT_SOC_OK,
	FB_IS_USERSPACE,
#ifdef CONFIG_RK_AVB_LIBAVB_USER
	FB_HAS_COUNT,
	FB_HAS_SLOT,
	FB_CURR_SLOT,
	FB_SLOT_SUFFIXES,
	FB_SLOT_SUCCESSFUL,
	FB_SLOT_UNBOOTABLE,
	FB_SLOT_RETRY_COUNT,
	FB_AT_VBST,
	FB_SNAPSHOT_STATUS,
#endif
#ifdef CONFIG_OPTEE_CLIENT
	FB_AT_DH,
	FB_AT_UUID,
#endif
} fb_getvar_t;

void fastboot_fail(const char *reason, char *response);
void fastboot_okay(const char *reason, char *response);

/**
 * Send an INFO packet during long commands based on timer. If
 * CONFIG_UDP_FUNCTION_FASTBOOT is defined, an INFO packet is sent
 * if the time is 30 seconds after start. Else, noop.
 *
 * TODO: Handle the situation where both UDP and USB fastboot are
 *       enabled.
 *
 * @param start:  Time since last INFO packet was sent.
 * @param msg:    String describing the reason for waiting
 */
void timed_send_info(ulong *start, const char *msg);

#endif /* _FASTBOOT_H_ */
