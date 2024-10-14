//////////////////////////////////////////////////////////////////////////////
//
//  Name:           btdisp_lock.h
//
//
//  Description:    Critical section definitions for btdisp functions.
//
//
//  Copyright (c) 1999-2003, WIDCOMM Inc., All Rights Reserved.
//  Widcomm Bluetooth Core. Proprietary and confidential.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BTDISP_LOCK
#define BTDISP_LOCK


#ifdef TESTER

#ifdef __cplusplus
extern "C" {
#endif

// External function declaration
extern void btdisp_lock();
extern void btdisp_unlock();
extern void btdisp_init();
extern void btdisp_uninit();

#ifdef __cplusplus
}
#endif

// Lock Macros
#define BTDISP_LOCK_LOG()           btdisp_lock();
#define BTDISP_UNLOCK_LOG()         btdisp_unlock();
#define BTDISP_INIT_LOCK()          btdisp_init();
#define BTDISP_UNINIT_LOCK()        btdisp_uninit();

#else

#define BTDISP_LOCK_LOG()
#define BTDISP_UNLOCK_LOG()
#define BTDISP_INIT_LOCK()
#define BTDISP_UNINIT_LOCK()

#endif



#endif // BTDISP_LOCK