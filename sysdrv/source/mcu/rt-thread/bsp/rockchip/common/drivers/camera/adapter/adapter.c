/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    adapter.c
  * @version V0.0.1
  * @brief   bridge system interfaces and variables of rkos and rt-thread for
  *          vicap driver
  * Change Logs:
  * Date           Author          Notes
  * 2019-03-29     ISP Team      first implementation
  *
  ******************************************************************************
  */

/** @addtogroup RKBSP_Driver_Reference
 *  @{
 */

/** @addtogroup Vicap
 *  @{
 */

/** @addtogroup Adapter
 *  @{
 */

/** @defgroup Adapter_How_To_Use How To Use
 *  @{
 The adapter layer bridges system interfaces and variables of rt-thread and rkos
 for vicap driver.Interfaces and variables can be called directly by vicap driver.
 @} */

#include "adapter.h"
#if defined(CONFIG_DRIVER_DSP) && defined(CONFIG_MCU_HAL_MBOX)
#include "driver/DspDevice.h"
#endif

/********************* Private Function Definition ***************************/
/** @defgroup ADAPTER_Private_Function Private Function
 *  @{
 */

#if defined(ADAPTER_MODULE_COMPILED)

/**
 * @brief  create mutex.
 * @param name : The name of mutex.
 * @param flag : The flag to create mutex.
 */
rk_mutex_t rk_mutex_create(const char *name, uint32_t flag)
{
#if defined(__RT_THREAD__)
    return rt_mutex_create(name, flag);
#elif defined(__RK_OS__)
    return rkos_mutex_create();
#endif
}

/**
 * @brief  take mutex.
 * @param mutex : The mutex to be taken.
 * @param time : The waiting time to take mutex.
 */
ret_err_t rk_mutex_take(rk_mutex_t mutex, uint32_t time)
{
#if defined(__RT_THREAD__)
    return rt_mutex_take(mutex, time);
#elif defined(__RK_OS__)
    return rkos_mutex_lock(mutex);
#endif
}

/**
 * @brief  release mutex.
 * @param mutex: The mutex to be released.
 */
ret_err_t rk_mutex_release(rk_mutex_t mutex)
{
#if defined(__RT_THREAD__)
    return rt_mutex_release(mutex);
#elif defined(__RK_OS__)
    return rkos_semaphore_give(mutex);
#endif
}

/**
 * @brief  delete mutex.
 * @param mutex: The mutex to be deleted.
 */
ret_err_t rk_mutex_delete(rk_mutex_t mutex)
{
#if defined(__RT_THREAD__)
    return rt_mutex_delete(mutex);
#elif defined(__RK_OS__)
    return rkos_semaphore_delete(mutex);
#endif
}

/**
 * @brief  create semaphore.
 * @param name : The semaphore name.
 * @param initcnt : The initial value of semaphore.
 * @param maxcnt : The max value of semaphore.
 */
rk_semaphore_t rk_semaphore_create(const char *name,
                                   uint32_t initcnt,
                                   uint32_t maxcnt)
{
#if defined(__RT_THREAD__)
    return rt_sem_create(name, initcnt, RT_IPC_FLAG_FIFO);
#elif defined(__RK_OS__)
    return rkos_semaphore_create(maxcnt, initcnt);
#endif
}

/**
 * @brief  take semaphore.
 * @param sem : The semaphore to be taken.
 * @param time : The time to be taken.
 */
ret_err_t rk_semaphore_take(rk_semaphore_t sem, uint32_t time)
{
#if defined(__RT_THREAD__)
    return rt_sem_take(sem, time);
#elif defined(__RK_OS__)
    return rkos_semaphore_take(sem, time);
#endif
}

/**
 * @brief  release semaphore.
 * @param sem : The semaphore to be released.
 */
ret_err_t rk_semaphore_release(rk_semaphore_t sem)
{
#if defined(__RT_THREAD__)
    return rt_sem_release(sem);
#elif defined(__RK_OS__)
    return rkos_semaphore_give(sem);
#endif
}

/**
 * @brief  delete semaphore.
 * @param sem : The semaphore to be deleted.
 */
ret_err_t rk_semaphore_delete(rk_semaphore_t sem)
{
#if defined(__RT_THREAD__)
    return rt_sem_delete(sem);
#elif defined(__RK_OS__)
    return rkos_semaphore_delete(sem);
#endif
}

/**
 * @brief  malloc buf for driver.
 * @param size : The size to be malloced.
 */
void *rk_malloc(uint32_t size)
{
#if defined(__RT_THREAD__)
    return rt_malloc(size);
#elif defined(__RK_OS__)
    return rkos_memory_malloc_align(size, 32);
#endif
}

/**
 * @brief  free malloced buf.
 * @param mem : The start addr of buf to freed.
 */
void rk_free(void *mem)
{
#if defined(__RT_THREAD__)
    rt_free(mem);
#elif defined(__RK_OS__)
    rkos_memory_free_align(mem);
#endif
}

/**
 * @brief  set value to memory.
 * @param s : The start addr of memory to be set.
 * @param val : The value to be set in memory.
 * @param count : The len of memory
 */
void *rk_memset(void *s, int val, dt_ubase_t count)
{
#if defined(__RT_THREAD__)
    return rt_memset(s, val, count);
#elif defined(__RK_OS__)
    return memset(s, val, count);
#endif
}

/**
 * @brief  copy value between two memory.
 * @param dst : The desitination address to copy to.
 * @param src : The source address to copy from.
 * @param count : The len of value to be copied.
 */
void *rk_memcpy(void *dst, const void *src, dt_ubase_t count)
{
#if defined(__RT_THREAD__)
    return rt_memcpy(dst, src, count);
#elif defined(__RK_OS__)
    rkos_memcpy(dst, (void *)src, count);
    return (void *)src;
#endif
}

/**
 * @brief  malloc buf by dma.
 * @param size : The size of buf to be malloced.
 */
void *rk_mem_malloc(uint32_t size)
{
#if defined(__RT_THREAD__)
#ifdef RT_USING_DMA
    return rt_dma_malloc(size);
#else
    return RT_NULL;
#endif
#elif defined(__RK_OS__)
    return rkos_memory_malloc_align(size, 32);
#endif
}

/**
 * @brief  free buf malloced by dma.
 * @param mem : The start address of buf malloced by dma.
 */
void rk_mem_free(void *mem)
{
#if defined(__RT_THREAD__)
#ifdef RT_USING_DMA
    rt_dma_free(mem);
#endif
#elif defined(__RK_OS__)
    rkos_memory_free_align(mem);
#endif
}

/**
 * @brief  free large buf malloced by dma.
 * @param mem : The start address of buf malloced by dma.
 */
void rk_dma_free_large(void *mem)
{
#if defined(__RT_THREAD__)
#ifdef RT_USING_LARGE_HEAP
    rt_dma_free_large(mem);
#endif
#elif defined(__RK_OS__)
    rkos_memory_free_align(mem);
#endif
}

/**
 * @brief  malloc large buf by dma.
 * @param size : The size of buf to be malloced.
 */
void *rk_dma_malloc_large(dt_ubase_t size)
{
#if defined(__RT_THREAD__)
#ifdef RT_USING_LARGE_HEAP
    return rt_dma_malloc_large(size);
#else
    return RT_NULL;
#endif
#elif defined(__RK_OS__)

    /*
     * vicap required 8 bytes align, but jpeg encoder
     * is a major function and it need to be 128 aligned.
     * Vicap buffers could be used directly by DSP encoder.
     */
#if RKDSP_CACHE_LINE_SIZE < 32
#error "DSP changes its buffer align, we at align with 8 bytes"
#endif
    return rkdsp_malloc(size);

#endif
}

/**
 * @brief  update dcache.
 * @param ops : The operation type.
 * @param addr : The start address of buf to be flushed.
 * @param size : The size of buf to be flushed.
 */
void rk_hw_cpu_dcache_ops(int ops, void *addr, int size)
{
#if defined(__RT_THREAD__)
#ifdef     ARCH_ARM_CORTEX_M
    rt_hw_cpu_dcache_ops(ops, addr, size);
#endif
#elif defined(__RK_OS__)
    rk_dcache_ops(ops, addr, size);
#endif
}

/**
 * @brief  disable interrupt.
 */
dt_base_t rk_hw_interrupt_disable(void)
{
#if defined(__RT_THREAD__)
    return rt_hw_interrupt_disable();
#elif defined(__RK_OS__)
    rk_interrupt_disable();
    return 0xffffffff;
#endif
}

/**
 * @brief  enable interrupt.
 * @param level : The last status of interrupt.
 */
void rk_hw_interrupt_enable(dt_base_t level)
{
#if defined(__RT_THREAD__)
    rt_hw_interrupt_enable(level);
#elif defined(__RK_OS__)
    rk_interrupt_enable();
#endif
}

#define RK_DEVICE_CRTL_DEVICE_INIT                  (0)
/**
 * @brief  find out registered device in kernel.
 * @param dev_name : The device name.
 */
rk_device *rk_find_device(const char *dev_name)
{
#if defined(__RT_THREAD__)
    return rt_device_find(dev_name);

#elif defined(__RK_OS__)
    char *cursor;
    uint8_t class_id, object_id;
    rk_device *dev;

    if (rk_strstr(dev_name, "vicap"))
    {
        class_id = DEV_CLASS_VICAP;
        rk_kprintf("find device class_id:%s\n", "DEV_CLASS_VICAP");
    }
    else if (rk_strstr(dev_name, "i2c"))
    {
        class_id = DEV_CLASS_I2C;
        rk_kprintf("find device class_id:%s\n", "DEV_CLASS_I2C");
    }
    else if (rk_strstr(dev_name, "dma"))
    {
        class_id = DEV_CLASS_DMA;
        rk_kprintf("find device class_id:%s\n", "DEV_CLASS_DMA");
    }
    else
    {
        class_id = DEV_CLASS_CAMERA;
        rk_kprintf("find device class_id:%s\n", "DEV_CLASS_CAMERA");
    }

    cursor = rk_strstr(dev_name, "_");
    cursor += 1;
    object_id = atoi(cursor);

    if (class_id == DEV_CLASS_I2C)
    {
        switch (object_id)
        {
        case 0:
            object_id = I2C_DEV0;
            break;

        case 1:
            object_id = I2C_DEV1;
            break;

        case 2:
            object_id = I2C_DEV2;
            break;

        default:
            object_id = I2C_DEV0;
            break;
        }
    }

    dev = (rk_device *)rkdev_find(class_id, object_id);
    if (class_id == DEV_CLASS_CAMERA)
    {
        if (!rk_strstr(dev->name, dev_name))
        {
            rk_kprintf("dev:%s is not found\n", dev_name);
            return RK_NULL;
        }
        rk_kprintf("find camera device:%s\n", dev->name);
    }

    return dev;
#endif
}

/**
 * @brief  close registered device in kernel.
 * @param dev :  The device to be closed.
 */
ret_err_t rk_device_close(rk_device *dev)
{
#if defined(__RT_THREAD__)
    return rt_device_close(dev);
#elif defined(__RK_OS__)
    return rkdev_close(dev);
#endif
}

/**
 * @brief  open registered device in kernel.
 * @param dev : The device to be opened.
 * @param flag : The flag to open device.
 */
ret_err_t rk_device_open(rk_device *dev, uint16_t flag)
{
#if defined(__RT_THREAD__)
    return rt_device_open(dev, flag);
#elif defined(__RK_OS__)
    ret_err_t ret = RET_SYS_EOK;
    rk_device *dev_instance;
    uint8_t class_id, object_id;

    class_id = dev->dev_class_id;
    object_id = dev->dev_object_id;


    dev_instance = rkdev_open(class_id, object_id, flag);
    if ((uint32_t)dev != (uint32_t) dev_instance)
    {
        rk_kprintf("Err:opened device is not matched the input!\n");
        ret = RET_SYS_ERROR;
    }

    if (class_id == DEV_CLASS_CAMERA ||
            class_id == DEV_CLASS_VICAP)
    {

        ret = rk_device_control(dev, RK_DEVICE_CRTL_DEVICE_INIT, RK_NULL);
        if (ret != RET_SYS_EOK)
        {
            rk_kprintf("Err:open device failed!\n");
            return RET_SYS_ERROR;
        }

    }

    return ret;
#endif
}

/**
 * @brief  init device.
 * @param dev : The device to be inited.
 */
ret_err_t rk_device_init(rk_device *dev)
{
#if defined(__RT_THREAD__)
    return rt_device_init(dev);
#elif defined(__RK_OS__)
#define RK_DEVICE_CTRL_DEVICE_INIT                  (0)
    return rk_device_control(dev, RK_DEVICE_CTRL_DEVICE_INIT, RK_NULL);
#endif
}

/**
 * @brief  control device registered in kernel.
 * @param dev : The device to be controlled.
 * @param cmd : The control command.
 * @param arg : The parameter to control device.
 */
ret_err_t rk_device_control(rk_device *dev, int cmd, void *arg)
{
#if defined(__RT_THREAD__)
    return rt_device_control(dev, cmd, arg);
#elif defined(__RK_OS__)
    return rkdev_control(dev, cmd, arg);
#endif
}

/**
 * @brief  calculate the len of string.
 * @param s : The string to be calculated.
 */
ret_size_t rk_strlen(const char *s)
{
#if defined(__RT_THREAD__)
    return rt_strlen(s);
#elif defined(__RK_OS__)
    return strlen(s);
#endif
}

/**
 * @brief  find sub string.
 * @param s1 : The source string.
 * @param s2 : The target string.
 */
char *rk_strstr(const char *s1, const char *s2)
{
#if defined(__RT_THREAD__)
    return rt_strstr(s1, s2);
#elif defined(__RK_OS__)
    return strstr(s1, s2);
#endif
}

/**
 * @brief  copy fixed len string.
 * @param dst : The targe string.
 * @param src : The source string.
 * @param len : The len of string to copy.
 */
char *rk_strncpy(char *dst, const char *src, dt_ubase_t len)
{
#if defined(__RT_THREAD__)
    return rt_strncpy(dst, src, len);
#elif defined(__RK_OS__)
    return strncpy(dst, src, len);
#endif
}

/**
 * @brief  compare two stringes.
 * @param ct : The targe string.
 * @param cs : The source string.
 */
int32_t rk_strcmp(const char *cs, const char *ct)
{
#if defined(__RT_THREAD__)
    return rt_strcmp(cs, ct);
#elif defined(__RK_OS__)
    return strcmp(cs, ct);
#endif
}

/**
 * @brief  compare two stringes.
 * @param ct : The targe string.
 * @param cs : The source string.
 */
int32_t rk_strncmp(const char *cs, const char *ct, dt_ubase_t len)
{
#if defined(__RT_THREAD__)
    return rt_strncmp(cs, ct, len);
#elif defined(__RK_OS__)
    return strncmp(cs, ct, len);
#endif
}

/**
 * @brief  get tick, 1 tick = 1ms .
 */
rk_tick_t rk_tick_get(void)
{
    return HAL_GetTick();
}

/**
 * @brief  enable clk.
 * @param gate : The clock gate to enable.
 */
ret_err_t rk_clk_enable(rk_clk_gate *gate)
{
#if defined(__RT_THREAD__)
#ifdef RT_USING_CRU
    return clk_enable(gate);
#else
    return -RT_EEMPTY;
#endif
#elif defined(__RK_OS__)
    return ClkEnable(gate);
#endif
}

/**
 * @brief  disable clk.
 * @param gate : The clock gate to disable.
 */
ret_err_t rk_clk_disable(rk_clk_gate *gate)
{
#if defined(__RT_THREAD__)
#ifdef RT_USING_CRU
    return clk_disable(gate);
#else
    return -RT_EEMPTY;
#endif
#elif defined(__RK_OS__)
    return ClkDisable(gate);
#endif
}

/**
 * @brief  get clk rate.
 * @param clk_id : The clock id.
 */
ret_err_t rk_clk_get_rate(eCLOCK_Name clk_id)
{
#if defined(__RT_THREAD__)
#ifdef RT_USING_CRU
    return clk_get_rate(clk_id);
#else
    return -RT_EEMPTY;
#endif
#elif defined(__RK_OS__)
    return ClkGetRate(clk_id);
#endif
}

/**
 * @brief  set clk rate.
 * @param clk_id : The clock id.
 * @param rate : The clock rate you want.
 */
ret_err_t rk_clk_set_rate(eCLOCK_Name clk_id, uint32_t rate)
{
#if defined(__RT_THREAD__)
#ifdef RT_USING_CRU
    return clk_set_rate(clk_id, rate);
#else
    return -RT_EEMPTY;
#endif
#elif defined(__RK_OS__)
    return ClkSetRate(clk_id, rate);
#endif
}

/**
 * @brief  get clk gate from clock id.
 * @param clk_id : The clock id.
 */
rk_clk_gate *rk_get_clk_gate_from_id(int clk_id)
{
#if defined(__RT_THREAD__)
#ifdef RT_USING_CRU
    return get_clk_gate_from_id(clk_id);
#else
    return RT_NULL;
#endif
#elif defined(__RK_OS__)
    return GetClkGateFromId(clk_id);
#endif
}

#endif
/** @} */  // ADAPTER_Private_Function

/** @} */  // Adapter

/** @} */  // VICAP

/** @} */  // RKBSP_Common_Driver
