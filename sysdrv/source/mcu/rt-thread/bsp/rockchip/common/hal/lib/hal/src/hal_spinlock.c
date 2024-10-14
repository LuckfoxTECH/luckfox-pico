/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_SPINLOCK_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup SPINLOCK
 *  @{
 */

/** @defgroup SPINLOCK_How_To_Use How To Use
 *  @{

 The SPINLOCK driver can be used as follows:

```
    // The ownerid is a unique ID, which is used to identify your running instance.
    // The ownerid has only 4bits and exclude zero, so it = [1...15].
    uint32_t ownerID = HAL_CPU_TOPOLOGY_GetCurrentCpuId() << 1 | 1;
    // The SPINLOCK driver only needs to be initialized once.
    HAL_SPINLOCK_Init(ownerID);

    HAL_SPINLOCK_Lock(lock);
    // do something in critical zone;
    HAL_SPINLOCK_Unlock(lock);
```

 @} */

#ifndef HAL_HWSPINLOCK_MODULE_ENABLED

typedef int SPINLOCK_t;

extern uint32_t __spinlock_mem_start__[];
extern uint32_t __spinlock_mem_end__[];

static int g_spinlock_max = 0;
static SPINLOCK_t *g_spinlock = NULL;
static uint32_t g_ownerID;

/********************* Public Function Definition ****************************/

static HAL_Status HAL_SW_SPINLOCK_Init(uint32_t ownerID)
{
    g_spinlock = (SPINLOCK_t *)__spinlock_mem_start__;
    g_spinlock_max = ((uint32_t)__spinlock_mem_end__ - (uint32_t)__spinlock_mem_start__) / sizeof(SPINLOCK_t);
    if (g_spinlock_max <= 0 || ownerID == 0) {
        HAL_DBG("there is no memory for spinlock, or ownerID is zero, spinlock init failed\n");
        g_spinlock = NULL;

        return HAL_INVAL;
    }

    g_ownerID = ownerID;

    return HAL_OK;
}

static SPINLOCK_t *HAL_SW_SPINLOCK(uint32_t index)
{
    if (g_spinlock && index < (uint32_t)g_spinlock_max) {
        return &g_spinlock[index];
    } else {
        return NULL;
    }
}

static void HAL_SW_SPINLOCK_Lock(SPINLOCK_t *lock)
{
#if defined(__ARM_ARCH_8A__)           /* for armv8a */
#if defined(__AARCH64__)               /* for armv8a 64bit*/
    int tmp;

    asm volatile (
        "SEVL\n"
        "PRFM PSTL1KEEP, [%1]\n"
        "Loop:\n"
        "WFE\n"
        "LDAXR %0, [%1]\n"
        "CBNZ %0, Loop\n"
        "STXR %0, %2, [%1]\n"
        "CBNZ %0, Loop\n"
        : "=&r" (tmp)
        : "r" (lock), "r" (g_ownerID)
        : "cc"
        );
#else                                  /* for armv8a 32bit */
    int tmp;

    asm volatile (
        "PLDW [%1]\n"
        "Loop:\n"
        "LDAEX %0, [%1]\n"
        "CMP %0, #0\n"
        "WFENE\n"
        "STREXEQ %0, %2, [%1]\n"
        "CMPEQ %0, #0\n"
        "BNE Loop\n"
        : "=&r" (tmp)
        : "r" (lock), "r" (g_ownerID)
        : "cc"
        );
#endif
#elif defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7A__)   /* for armv7m & armv7a */
    int tmp;

    asm volatile (
        "PLDW [%1]\n"
        "Loop:\n"
        "LDREX %0, [%1]\n"
        "CMP %0, #0\n"
        "WFENE\n"
        "STREXEQ %0, %2, [%1]\n"
        "CMPEQ %0, #0\n"
        "BNE Loop\n"
        : "=&r" (tmp)
        : "r" (lock), "r" (g_ownerID)
        : "cc"
        );
#elif defined(__ARM_ARCH_8M__)        /* for armv8m */
    int tmp;

    asm volatile (
        "PLDW [%1]\n"
        "Loop:\n"
        "LDAEX %0, [%1]\n"
        "CMP %0, #0\n"
        "WFENE\n"
        "STREXEQ %0, %2, [%1]\n"
        "CMPEQ %0, #0\n"
        "BNE Loop\n"
        : "=&r" (tmp)
        : "r" (lock), "r" (g_ownerID)
        : "cc"
        );
#endif
}

static HAL_Check HAL_SW_SPINLOCK_TryLock(SPINLOCK_t *lock)
{
#if defined(__ARM_ARCH_8A__)           /* for armv8a */
#if defined(__AARCH64__)               /* for armv8a 64bit*/
    int tmp;

    asm volatile (
        "SEVL\n"
        "PRFM PSTL1KEEP, [%1]\n"
        "LDAXR %0, [%1]\n"
        "CBNZ %0, Out\n"
        "STXR %0, %2, [%1]\n"
        "CBNZ %0, Out\n"
        "Out:         \n"
        : "=&r" (tmp)
        : "r" (lock), "r" (g_ownerID)
        : "cc"
        );

    return tmp ? HAL_FALSE : HAL_TRUE;
#else                                  /* for armv8a 32bit */
    int tmp;

    asm volatile (
        "PLDW [%1]\n"
        "LDAEX %0, [%1]\n"
        "CMP %0, #0\n"
        "BNE Out\n"
        "STREXEQ %0, %2, [%1]\n"
        "CMPEQ %0, #0\n"
        "BNE Out\n"
        "Out:   \n"
        : "=&r" (tmp)
        : "r" (lock), "r" (g_ownerID)
        : "cc"
        );

    return tmp ? HAL_FALSE : HAL_TRUE;
#endif
#elif defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7A__)   /* for armv7m & armv7a */
    int tmp;

    asm volatile (
        "PLDW [%1]\n"
        "LDREX %0, [%1]\n"
        "CMP %0, #0\n"
        "BNE Out\n"
        "STREXEQ %0, %2, [%1]\n"
        "CMPEQ %0, #0\n"
        "BNE Out\n"
        "Out:   \n"
        : "=&r" (tmp)
        : "r" (lock), "r" (g_ownerID)
        : "cc"
        );

    return tmp ? HAL_FALSE : HAL_TRUE;
#elif defined(__ARM_ARCH_8M__)        /* for armv8m */
    int tmp;

    asm volatile (
        "PLDW [%1]\n"
        "LDAEX %0, [%1]\n"
        "CMP %0, #0\n"
        "BNE Out\n"
        "STREXEQ %0, %2, [%1]\n"
        "CMPEQ %0, #0\n"
        "BNE Out\n"
        "Out:   \n"
        : "=&r" (tmp)
        : "r" (lock), "r" (g_ownerID)
        : "cc"
        );

    return tmp ? HAL_FALSE : HAL_TRUE;
#endif
}

static void HAL_SW_SPINLOCK_Unlock(SPINLOCK_t *lock)
{
#if defined(__ARM_ARCH_8A__)           /* for armv8a */
#if defined(__AARCH64__)               /* for armv8a 64bit*/
    asm (
        "STLR WZR, [%0]"
        :
        : "r" (lock)
        : "cc"
        );
#else                                  /* for armv8a 32bit */
    asm (
        "STL %1, [%0]\n"
        :
        : "r" (lock), "r" (0)
        : "cc"
        );
#endif
#elif defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7A__)   /* for armv7m & armv7a */
    asm (
        "STR %1, [%0]\n"
        "DSB\n"
        "SEV\n"
        :
        : "r" (lock), "r" (0)
        : "cc"
        );
#elif defined(__ARM_ARCH_8M__)        /* for armv8m */
    asm (
        "STL %1, [%0]\n"
        :
        : "r" (lock), "r" (0)
        : "cc"
        );
#endif
}

static uint32_t HAL_SW_SPINLOCK_GetOwner(SPINLOCK_t *lock)
{
    return *lock;
}

#endif    /* end of ifndef HAL_HWSPINLOCK_MODULE_ENABLED */

/** @defgroup SPINLOCK_Exported_Functions_Group5 Other Functions
 *  @{
 */

/**
 * @brief  Init SpinLock driver.
 * @param  ownerID: The Owner ID wanna set.
 * @return
 *         - HAL_OK: success.
 *         - Other: failed.
 */
HAL_Status HAL_SPINLOCK_Init(uint32_t ownerID)
{
#ifdef HAL_HWSPINLOCK_MODULE_ENABLED

    return HAL_HWSPINLOCK_Init(ownerID) ? HAL_OK : HAL_ERROR;
#else

    return HAL_SW_SPINLOCK_Init(ownerID);
#endif
}

/**
 * @brief  try acquiring a spinlock.
 * @param  lockID: the spinlock ID you want to acquire.
 * @return
 *         - HAL_FALSE : lock failed.
 *         - HAL_TRUE : lock success.
 */
HAL_Check HAL_SPINLOCK_TryLock(uint32_t lockID)
{
#ifdef HAL_HWSPINLOCK_MODULE_ENABLED

    return HAL_HWSPINLOCK_TryLock(lockID);
#else
    SPINLOCK_t *lock = HAL_SW_SPINLOCK(lockID);

    if (lock) {
        return HAL_SW_SPINLOCK_TryLock(lock);
    }

    return HAL_TRUE;
#endif
}

/**
 * @brief  acquiring a spinlock.
 * @param  lockID: the spinlock ID you want to acquire.
 *
 */
void HAL_SPINLOCK_Lock(uint32_t lockID)
{
#ifdef HAL_HWSPINLOCK_MODULE_ENABLED
    HAL_Check ret;

    do {
        ret = HAL_HWSPINLOCK_TryLock(lockID);
    } while (!ret);
#else
    SPINLOCK_t *lock = HAL_SW_SPINLOCK(lockID);

    if (lock) {
        HAL_SW_SPINLOCK_Lock(lock);
    }
#endif
}

/**
 * @brief  releasing a spinlock.
 * @param  lockID: the spinlock ID you want to release.
 *
 */
void HAL_SPINLOCK_Unlock(uint32_t lockID)
{
#ifdef HAL_HWSPINLOCK_MODULE_ENABLED
    HAL_HWSPINLOCK_Unlock(lockID);
#else
    SPINLOCK_t *lock = HAL_SW_SPINLOCK(lockID);

    if (lock) {
        HAL_SW_SPINLOCK_Unlock(lock);
    }
#endif
}

/**
 * @brief  get the owner of spinlock.
 * @param  lockID: the spinlock ID you want to known it's owner.
 * @return
 *         - 0 : the spinlock has no owner.
 *         - other : the owner of spinlock.
 */
uint32_t HAL_SPINLOCK_GetOwner(uint32_t lockID)
{
#ifdef HAL_HWSPINLOCK_MODULE_ENABLED

    return HAL_HWSPINLOCK_GetOwner(lockID);
#else
    SPINLOCK_t *lock = HAL_SW_SPINLOCK(lockID);

    if (lock) {
        return HAL_SW_SPINLOCK_GetOwner(lock);
    }

    return 0;
#endif
}

/** @} */

/** @} */

/** @} */

#endif
