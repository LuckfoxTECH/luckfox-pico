/**
 ******************************************************************************
 *
 * @file ipc_host.c
 *
 * @brief IPC module.
 *
 * Copyright (C) RivieraWaves 2011-2019
 *
 ******************************************************************************
 */

/*
 * INCLUDE FILES
 ******************************************************************************
 */
#ifndef __KERNEL__
#include <stdio.h>
#else
#include <linux/spinlock.h>
#include "rwnx_defs.h"
#include "rwnx_prof.h"
#endif

#include "ipc_host.h"

/*
 * TYPES DEFINITION
 ******************************************************************************
 */

const int nx_txdesc_cnt[] = {
	NX_TXDESC_CNT0,
	NX_TXDESC_CNT1,
	NX_TXDESC_CNT2,
	NX_TXDESC_CNT3,
	#if NX_TXQ_CNT == 5
	NX_TXDESC_CNT4,
	#endif
};

const int nx_txuser_cnt[] = {
	CONFIG_USER_MAX,
	CONFIG_USER_MAX,
	CONFIG_USER_MAX,
	CONFIG_USER_MAX,
	#if NX_TXQ_CNT == 5
	1,
	#endif
};


