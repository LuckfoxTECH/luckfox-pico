/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#if defined(HAL_EHCI_MODULE_ENABLED) || defined(HAL_OHCI_MODULE_ENABLED)

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup USB_HOST
 *  @{
 */

/** @defgroup USB_HOST_How_To_Use How To Use
 *  @{

 The USB_HOST HAL can be used as follows:

 - The USB_HOST HAL is the glue layer between driver and EHCI/OHCI HAL lib.
 - The USB_HCD_HANDLE data structure includes EHCI/OHCI HCD structure and
     their operations structure. It is the bridge between driver and HAL.
 - The Host driver must implement weak functions that the lib required, and
     invoke the HAL_USBH_xxxXfer() APIs to transfer the USB data.
 - The Host driver must alloc one block coherent memory for EHCI/OHCI HAL,
     the memory use for QH, qTD, ED, TD and other HW structure mapping.

 @} */

/** @defgroup USB_HOST_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/

/********************* Private Structure Definition **************************/

/********************* Private Variable Definition ***************************/

static struct USB_COHERENT_MEM g_usbMem;

/********************* Private Function Definition ***************************/

static HAL_Status USBH_HcdInit(struct USB_HCD_HANDLE *hcdHdl)
{
    HAL_Status ret;

    HAL_ASSERT(hcdHdl);

#ifdef HAL_OHCI_MODULE_ENABLED
    HAL_OHCI_OpsInit(&hcdHdl->opsOHCI);
    ret = hcdHdl->opsOHCI.Init(&hcdHdl->ohci);
    if (ret) {
        return ret;
    }
#endif

#ifdef HAL_EHCI_MODULE_ENABLED
    HAL_EHCI_OpsInit(&hcdHdl->opsEHCI);
    ret = hcdHdl->opsEHCI.Init(&hcdHdl->ehci);
    if (ret) {
        return ret;
    }
#endif

    return HAL_OK;
}

static struct USB_DEV *USBH_FindDeviceByPort(struct USB_DEV *udevList,
                                             int portNum, uint8_t isHS)
{
    struct USB_DEV *udev = udevList;

    while (udev != NULL) {
        if (udev->portNum == portNum) {
            if ((isHS && udev->speed == USB_SPEED_HIGH) ||
                (!isHS && udev->speed != USB_SPEED_HIGH)) {
                return udev;
            }
        }
        udev = udev->next;
    }

    return NULL;
}

static int USBH_PollingEhciRh(struct USB_HCD_HANDLE *hcdHdl,
                              struct USB_DEV *udevList)
{
#ifdef HAL_EHCI_MODULE_ENABLED
    struct USB_DEV *udev;
    int status, ret;

    do {
        status = hcdHdl->opsEHCI.RthubPortStatus(&hcdHdl->ehci, 0);
        if (status) {
            /* Disconnect the devices attached to this port */
            udev = USBH_FindDeviceByPort(udevList, 0, 1);
            if (udev) {
                HAL_USBH_DisconnectCallback(udev);
            }

            /* EHCI RT Hub polling */
            status = hcdHdl->opsEHCI.RthubPolling(&hcdHdl->ehci);
            if (status > 0) {
                ret = HAL_USBH_ConnectCallback(USB_SPEED_HIGH);
                if (ret < 0) {
                    HAL_DBG_ERR("connect device error, ret=%d\n", ret);

                    return ret;
                }
            }
        }
    } while (status == 1);

    return status;
#else

    return USBH_OK;
#endif
}

static int USBH_PollingOhciRh(struct USB_HCD_HANDLE *hcdHdl,
                              struct USB_DEV *udevList)
{
#ifdef HAL_OHCI_MODULE_ENABLED
    struct USB_DEV *udev;
    int speed;
    int status, ret;

    do {
        status = hcdHdl->opsOHCI.RthubPortStatus(&hcdHdl->ohci, 0);
        if (status) {
            /* Disconnect the devices attached to this port */
            udev = USBH_FindDeviceByPort(udevList, 0, 0);
            if (udev) {
                HAL_USBH_DisconnectCallback(udev);
            }

            /* OHCI RT Hub polling */
            status = hcdHdl->opsOHCI.RthubPolling(&hcdHdl->ohci);
            if (status) {
                speed = HAL_OHCI_GetRhPortSpeed(&hcdHdl->ohci, 0);
                ret = HAL_USBH_ConnectCallback(speed);
                if (ret < 0) {
                    HAL_DBG_ERR("connect device error, ret=%d\n", ret);

                    return ret;
                }
            }
        }
    } while (status == 1);

    return status;
#else

    return USBH_OK;
#endif
}

/* Memory alloc/free recording */
static void *USBH_MemAlign(void *ptr, uint32_t align)
{
    void *alignPtr;

    /* the allocated memory block is aligned */
    if (((uint32_t)ptr & (align - 1)) == 0) {
        alignPtr = (void *)((uint32_t)ptr + align);
    } else {
        alignPtr = (void *)(((uint32_t)ptr + (align - 1)) & ~(align - 1));
    }

    return alignPtr;
}

static HAL_Status USBH_MemInit(uint8_t *memStart)
{
    int i;
    uint8_t *base;

    if (!memStart) {
        return HAL_INVAL;
    }

    /*
     * EHCI/OHCI Coherent Memory layout (size/aligned):
     * ------------------------------------------------------
     * | Pool (16K/64B) | Hcca (256B/256B) | PFList (2K/4K) |
     * ------------------------------------------------------
     */
    g_usbMem.startAddr = memStart;

    for (i = 0; i < MEM_POOL_UNIT_NUM; i++) {
        g_usbMem.poolUsed[i] = 0;
        g_usbMem.pools[i] = (uint8_t *)((uint32_t)&memStart[i * MEM_POOL_UNIT_SIZE]);
    }

    /* Assign OHCI Hcca address */
    base = (uint8_t *)((uint32_t)&memStart[MEM_POOL_UNIT_NUM * MEM_POOL_UNIT_SIZE]);
    g_usbMem.hccaAddr = USBH_MemAlign(base, 256);

    /* Assign EHCI pfList address */
    base = g_usbMem.hccaAddr + 256;
    g_usbMem.pfListAddr = USBH_MemAlign(base, 4096);

    return HAL_OK;
}

/** @} */

/********************* Public Function Definition ****************************/
/** @defgroup USB_HOST_Exported_Functions_Group1 Suspend and Resume Functions

 This section provides functions allowing to suspend and resume the module:

 *  @{
 */

/**
 * @brief  Execute USB HCD supsend.
 * @param  hcdHdl: the USB HCD phandle
 * @param  isEhci: the EHCI port or not
 * @return HAL_Status
 */
HAL_Status HAL_USBH_Supsend(struct USB_HCD_HANDLE *hcdHdl, uint8_t isEhci)
{
    struct HCD_OPS *ops;

    if (!hcdHdl) {
        return HAL_INVAL;
    }

    ops = HAL_USBH_GetHcdOps(hcdHdl, isEhci);

    return ops->Suspend(HAL_USBH_GetHcd(hcdHdl, isEhci));
}

/**
 * @brief  Execute USB HCD resume.
 * @param  hcdHdl: the USB HCD phandle
 * @param  isEhci: the EHCI port or not
 * @return HAL_Status
 */
HAL_Status HAL_USBH_Resume(struct USB_HCD_HANDLE *hcdHdl, uint8_t isEhci)
{
    struct HCD_OPS *ops;

    if (!hcdHdl) {
        return HAL_INVAL;
    }

    ops = HAL_USBH_GetHcdOps(hcdHdl, isEhci);

    return ops->Resume(HAL_USBH_GetHcd(hcdHdl, isEhci));
}

/** @} */

/** @defgroup USB_HOST_Exported_Functions_Group2 State and Errors Functions

 This section provides functions allowing to get the status of the module:

 *  @{
 */

/**
 * @brief  Get EHCI/OHCI HCD phandle.
 * @param  hcdHdl: the USB HCD phandle
 * @param  ehciPort: the EHCI port flag
 * @return EHCI or OHCI HCD phandle
 */
void *HAL_USBH_GetHcd(struct USB_HCD_HANDLE *hcdHdl, uint8_t ehciPort)
{
    HAL_ASSERT(hcdHdl);

    return ehciPort ? (void *)(&hcdHdl->ehci) : (void *)(&hcdHdl->ohci);
}

/**
 * @brief  Get EHCI/OHCI HCD OPS phandle.
 * @param  hcdHdl: the USB HCD phandle
 * @param  ehciPort: the EHCI port flag
 * @return EHCI or OHCI HCD OPS phandle
 */
struct HCD_OPS *HAL_USBH_GetHcdOps(struct USB_HCD_HANDLE *hcdHdl,
                                   uint8_t ehciPort)
{
    HAL_ASSERT(hcdHdl);

    return ehciPort ? &hcdHdl->opsEHCI : &hcdHdl->opsOHCI;
}

/** @} */

/** @defgroup USB_HOST_Exported_Functions_Group3 IO Functions

 This section provides functions allowing to IO controlling:

 *  @{
 */

/**
 * @brief  Mask EHCI/OHCI HW interrupt, should be implemented in driver layer
 * @param  isEhci: the EHCI irq or not
 * @return HAL_Status
 */
__WEAK HAL_Status HAL_USBH_MaskIrq(uint8_t isEhci)
{
    return HAL_OK;
}

/**
 * @brief  Unmask EHCI/OHCI HW interrupt, should be implemented in driver layer
 * @param  isEhci: the EHCI irq or not
 * @return HAL_Status
 */
__WEAK HAL_Status HAL_USBH_UnmaskIrq(uint8_t isEhci)
{
    return HAL_OK;
}

/**
 * @brief  USB device connection event callback.
 * @param  speed: the connected device speed
 * @return HAL_Status
 */
__WEAK HAL_Status HAL_USBH_ConnectCallback(int speed)
{
    return HAL_OK;
}

/**
 * @brief USB device disconnection event callback.
 * @param  udev: the disconnected device phandle
 * @return HAL_Status
 */
__WEAK HAL_Status HAL_USBH_DisconnectCallback(struct USB_DEV *udev)
{
    return HAL_OK;
}

/**
 * @brief  Reset root hub port.
 * @param  hcdHdl: the USB HCD phandle
 * @param  udev: the USB device
 * @return HAL_Status
 */
HAL_Status HAL_USBH_ResetPort(struct USB_HCD_HANDLE *hcdHdl,
                              struct USB_DEV *udev)
{
    struct HCD_OPS *ops;

    if (!hcdHdl || !udev) {
        return HAL_INVAL;
    }

    ops = HAL_USBH_GetHcdOps(hcdHdl, udev->ehciPort);

    return ops->RthubPortReset(HAL_USBH_GetHcd(hcdHdl, udev->ehciPort), udev->portNum - 1);
}

/**
 * @brief  Polling HCD root hubs.
 * @param  hcdHdl: the USB HCD phandle
 * @param  udevList: the USB device list
 * @return HAL_Status
 */
HAL_Status HAL_USBH_PollingRtHubs(struct USB_HCD_HANDLE *hcdHdl,
                                  struct USB_DEV *udevList)
{
    int status;

    if (!hcdHdl) {
        return HAL_INVAL;
    }

    status = USBH_PollingEhciRh(hcdHdl, udevList);
    if (status) {
        return HAL_ERROR;
    }

    status = USBH_PollingOhciRh(hcdHdl, udevList);
    if (status) {
        return HAL_ERROR;
    }

    return HAL_OK;
}

/**
 * @brief  Force to quit an endpoint transfer.
 * @param  hcdHdl: the USB HCD phandle
 * @param  udev: the USB device
 * @param  ep: the endpoint to be quit
 * @return HAL_Status
 */
HAL_Status HAL_USBH_QuitXfer(struct USB_HCD_HANDLE *hcdHdl,
                             struct USB_DEV *udev, struct USB_EP_INFO *ep)
{
    struct HCD_OPS *ops;

    if (!hcdHdl || !udev || !ep) {
        return HAL_INVAL;
    }

    ops = HAL_USBH_GetHcdOps(hcdHdl, udev->ehciPort);

    return ops->QuitXfer(HAL_USBH_GetHcd(hcdHdl, udev->ehciPort), NULL, ep);
}

/**
 * @brief  Force to quit an UTR transfer.
 * @param  hcdHdl: the USB HCD phandle
 * @param  utr: the UTR transfer to be quit
 * @return HAL_Status
 */
HAL_Status HAL_USBH_QuitUtr(struct USB_HCD_HANDLE *hcdHdl, struct UTR *utr)
{
    struct HCD_OPS *ops;

    if (!hcdHdl || !utr || !utr->udev) {
        return HAL_INVAL;
    }

    ops = HAL_USBH_GetHcdOps(hcdHdl, utr->udev->ehciPort);

    return ops->QuitXfer(HAL_USBH_GetHcd(hcdHdl, utr->udev->ehciPort), utr, NULL);
}

/**
 * @brief  Execute an USB request in control transfer.
 * @param  hcdHdl: the USB HCD phandle
 * @param  udev: the USB device
 * @param  xferLen: transmitted/received length of data [out]
 * @param  utr: the UTR transfer to be quit
 * @param  timeout: time-out limit of this transfer
 * @return HAL_Status
 */
HAL_Status HAL_USBH_CtrlXfer(struct USB_HCD_HANDLE *hcdHdl,
                             struct USB_DEV *udev, uint32_t *xferLen,
                             struct UTR *utr, uint32_t timeout)
{
    struct HCD_OPS *ops;
    int timeDelay;
    int status;

    if (!hcdHdl || !utr || !udev) {
        return HAL_INVAL;
    }

    *xferLen = 0;
    ops = HAL_USBH_GetHcdOps(hcdHdl, udev->ehciPort);

    status = ops->CtrlXfer(utr);
    if (status < 0) {
        udev->ep0.hwPipe = NULL;

        return status;
    }

    timeDelay = timeout * 1000;
    while (utr->bIsTransferDone == 0) {
        if (timeDelay < 0) {
            HAL_USBH_QuitUtr(hcdHdl, utr);
            udev->ep0.hwPipe = NULL;

            return HAL_TIMEOUT;
        }

        HAL_DelayUs(1);
        timeDelay--;
    }

    status = utr->status;
    if (!status) {
        status = HAL_OK;
        *xferLen = utr->xferLen;
    } else {
        status = HAL_ERROR;
    }

    return status;
}

/**
 * @brief  Execute a bulk transfer request.
 * @param  hcdHdl: the USB HCD phandle
 * @param  utr: the UTR transfer to be quit
 * @return HAL_Status
 */
HAL_Status HAL_USBH_BulkXfer(struct USB_HCD_HANDLE *hcdHdl, struct UTR *utr)
{
    struct HCD_OPS *ops;

    if (!hcdHdl || !utr || !utr->udev) {
        return HAL_INVAL;
    }

    ops = HAL_USBH_GetHcdOps(hcdHdl, utr->udev->ehciPort);

    return ops->BulkXfer(utr);
}

/**
 * @brief  Execute an interrupt transfer request.
 * @param  hcdHdl: the USB HCD phandle
 * @param  utr: the UTR transfer to be quit
 * @return HAL_Status
 */
HAL_Status HAL_USBH_IntrXfer(struct USB_HCD_HANDLE *hcdHdl, struct UTR *utr)
{
    struct HCD_OPS *ops;

    if (!hcdHdl || !utr || !utr->udev) {
        return HAL_INVAL;
    }

    ops = HAL_USBH_GetHcdOps(hcdHdl, utr->udev->ehciPort);

    return ops->IntrXfer(utr);
}

/** @} */

/** @defgroup USB_HOST_Exported_Functions_Group4 Init and DeInit Functions

 This section provides functions allowing to init and deinit the module:

 *  @{
 */

/**
 * @brief  Initialize USB Host controller and USB HAL.
 * @param  hcdHdl: the USB HCD phandle
 * @param  coherentMem: coherent memory which allocated in driver layer
 * @return HAL_Status
 */
HAL_Status HAL_USBH_CoreInit(struct USB_HCD_HANDLE *hcdHdl, void * *coherentMem)
{
    HAL_Status ret;

    if (!hcdHdl || !(*coherentMem)) {
        return HAL_INVAL;
    }

    hcdHdl->coherentMem = *coherentMem;
    ret = USBH_MemInit(hcdHdl->coherentMem);
    if (ret) {
        return ret;
    }

    /* HCD class init */
    return USBH_HcdInit(hcdHdl);
}

/** @} */

/** @defgroup DEMO_Exported_Functions_Group5 Other Functions
 *  @{
 */

/**
 * @brief  Pool Memory alloc
 * @return pool memory pointer
 */
void *HAL_USBH_AllocPool()
{
    int i;
    void *pool;

    for (i = 0; i < MEM_POOL_UNIT_NUM; i++) {
        if (!g_usbMem.poolUsed[i]) {
            g_usbMem.poolUsed[i] = 1; /* mark */
            pool = (void *)g_usbMem.pools[i];
            memset(pool, 0, MEM_POOL_UNIT_SIZE);

            HAL_DBG("%s: 0x%08lx\n", __func__, (uint32_t)pool);

            return pool;
        }
    }
    HAL_DBG_ERR("%s failed!\n", __func__);

    return NULL;
}

/**
 * @brief  Pool Memory free
 * @param  pool: phandle of the pool memory
 * @return HAL_Status
 */
HAL_Status HAL_USBH_FreePool(void *pool)
{
    int i;

    for (i = 0; i < MEM_POOL_UNIT_NUM; i++) {
        if ((uint32_t)g_usbMem.pools[i] == (uint32_t)pool) {
            g_usbMem.poolUsed[i] = 0; /* unmark */

            HAL_DBG("%s: 0x%08lx\n", __func__, (uint32_t)pool);

            return HAL_OK;
        }
    }

    HAL_DBG_ERR("%s pool cell 0x%08lx not found!\n", __func__, (uint32_t)pool);

    return HAL_INVAL;
}

/**
 * @brief  Assign EHCI PF List Memory address
 * @return pf list memory address
 */
uint32_t *HAL_USBH_AssignEhciPfList()
{
    return (uint32_t *)g_usbMem.pfListAddr;
}

/**
 * @brief  Assign OHCI HCCA Memory address
 * @return HCCA memory address
 */
uint8_t *HAL_USBH_AssignOhciHcca()
{
    return (uint8_t *)g_usbMem.hccaAddr;
}

/** @} */

/** @} */

/** @} */

#endif /* HAL_EHCI_MODULE_ENABLED || HAL_OHCI_MODULE_ENABLED */
