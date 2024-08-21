/**
  * Copyright (c) 2022 Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_dw_pcie.c
  * @version V1.0
  * @brief   dw_pcie interface
  *
  * Change Logs:
  * Date           Author          Notes
  * 2022-12-30     Dingqiang Lin   the first version
  *
  ******************************************************************************
  */

/** @addtogroup RKBSP_Driver_Reference
 *  @{
 */

/** @addtogroup DW_PCIE
 *  @{
 */

/** @defgroup PCIE_How_To_Use How To Use
 *  @{

 @} */

#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

#include "hal_bsp.h"
#include "drv_pcie_dma.h"

#ifdef RT_USING_DW_PCIE

/********************* Private MACRO Definition ******************************/
/********************* Private Structure Definition **************************/

struct rk_pcie
{
    struct rt_device parent;

    struct HAL_PCIE_HANDLE *pcie;

    struct rk_pcie_dma_device *obj;
    struct rt_mutex lock;
};

/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ****************************/
static void rk_pcie_start_dma_dwc(struct rk_pcie_dma_device *obj, struct DMA_TABLE *table)
{
    struct rk_pcie *rk_pcie = (struct rk_pcie *)(obj->parent.user_data);

    HAL_PCIE_StartDma(rk_pcie->pcie, table);
}

static void rk_pcie_config_dma_dwc(struct rk_pcie_dma_device *obj, struct DMA_TABLE *table)
{
    struct rk_pcie *rk_pcie = (struct rk_pcie *)(obj->parent.user_data);

    HAL_PCIE_ConfigDma(rk_pcie->pcie, table);
}

static int rk_pcie_dma_wait_for_finished(struct rk_pcie_dma_device *obj, struct DMA_TABLE *table, u32 timeout_ms)
{
    struct rk_pcie *rk_pcie = (struct rk_pcie *)(obj->parent.user_data);
    int ret;

    do
    {
        ret = HAL_PCIE_GetDmaStatus(rk_pcie->pcie, table->chn, table->dir);
        HAL_CPUDelayUs(1);
    }
    while (!ret && timeout_ms--);

    return ret;
}

struct rk_pcie_dma_ops dw_pcie_dma_ops =
{
    .start_dma_func = rk_pcie_start_dma_dwc,
    .config_dma_func = rk_pcie_config_dma_dwc,
    .wait_for_finished = rk_pcie_dma_wait_for_finished,
};

static int rk_pcie_really_probe(struct rk_pcie *rk_pcie)
{
    int retries;

    for (retries = 0; retries < 100000; retries++)
    {
        if (HAL_PCIE_LinkUp(rk_pcie->pcie))
        {
            /*
             * We may be here in case of L0 in Gen1. But if EP is capable
             * of Gen2 or Gen3, Gen switch may happen just in this time, but
             * we keep on accessing devices in unstable link status. Given
             * that LTSSM max timeout is 24ms per period, we can wait a bit
             * more for Gen switch.
             */
            rt_thread_mdelay(50);
            rt_kprintf("PCIe Link up, LTSSM is 0x%x\n", HAL_PCIE_GetLTSSM(rk_pcie->pcie));
            return 0;
        }

        rt_kprintf("PCIe Linking... LTSSM is 0x%x\n", HAL_PCIE_GetLTSSM(rk_pcie->pcie));
        rt_thread_mdelay(20);
    }

    return 0;
}

/********************* Public Function Definition ****************************/

/**
 * @brief  Init SNOR framwork and apply to use.
 * @attention The SNOR will be enabled when board initialization, do not dynamically switch SNOR
 *      unless specifically required.
 */
void rt_dw_pcie_init(void *para)
{
    rt_err_t ret = -RT_ERROR;
    struct HAL_PCIE_DEV *pcie_dev;
    struct rk_pcie *dev;

    if (!para)
    {
        rt_kprintf("%s para err!\n", __func__);
        return;
    }
    pcie_dev = (struct HAL_PCIE_DEV *)para;

    dev = rt_malloc(sizeof(struct rk_pcie));
    if (!dev)
    {
        rt_kprintf("%s dev malloc err!\n", __func__);
        return;
    }
    dev->pcie = rt_malloc(sizeof(struct HAL_PCIE_HANDLE));
    if (!dev->pcie)
    {
        rt_kprintf("%s pcie malloc err!\n", __func__);
        return;
    }
    HAL_PCIE_Init(dev->pcie, pcie_dev);

    if (rt_mutex_init(&dev->lock, "pcie_lock", RT_IPC_FLAG_FIFO) != RT_EOK)
    {
        rt_kprintf("Init pcie dma mutex error\n");
        RT_ASSERT(0);
    }

    rk_pcie_really_probe(dev);

    dev->parent.type = RT_Device_Class_Char;
    dev->parent.user_data = dev;

    ret = rt_device_register(&dev->parent, "pcie", RT_DEVICE_FLAG_STANDALONE);
    if (ret != RT_EOK)
    {
        goto out;
    }

    dev->obj = rk_pcie_dma_register(dev, &dw_pcie_dma_ops);
    if (!dev->obj)
    {
        rt_kprintf("rk_pcie_dma_register failed %d\n", ret);
        goto out;
    }

    return;

out:
    rt_free(dev);
}

int rt_hw_dw_pcie_init(void)
{
    static rt_thread_t reboot_thread = RT_NULL;
    struct HAL_PCIE_DEV *pcie;

#ifdef HAL_PCIE_MODULE_ENABLED
    pcie = &g_pcieDev;
#endif

    if (!pcie)
    {
        rt_kprintf("%s failed, no host!", __func__);

        return RT_ERROR;
    }

    reboot_thread = rt_thread_create("rt_dw_pcie_init", rt_dw_pcie_init, &g_pcieDev, 1024, 10, 20);
    RT_ASSERT(reboot_thread);
    rt_thread_startup(reboot_thread);

    return RT_EOK;
}
INIT_APP_EXPORT(rt_hw_dw_pcie_init);

/** @} */  // PCIE_DMA_Public_Function

#endif

/** @} */  // PCIE_DMA

/** @} */  // RKBSP_Common_Driver
