/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#if defined(HAL_GPIO_MODULE_ENABLED) && defined(HAL_GPIO_IRQ_GROUP_MODULE_ENABLED)

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup GPIO_IRQ_GROUP
 *  @{
 */

/** @defgroup GPIO_IRQ_GROUP_How_To_Use How To Use
 *  @{

 The GPIO_IRQ_GROUP driver can be used as follows:
 - Invoke HAL_GPIO_IRQ_GROUP_Init() to initialize GPIO_IRQ_GROUP.
 - Invoke HAL_GPIO_IRQ_GROUP_GpioCtrlEnter() for gpio irq disable or enable control.
 - Invoke HAL_GPIO_IRQ_GROUP_GpioCtrlExit() for gpio irq disable or enable control.
 - Invoke HAL_GPIO_IRQ_GROUP_DIRQDispatchIrqs() to dispatch irqs of a irq group by a DIRQ.
 - Invoke HAL_GPIO_IRQ_GROUP_DispatchGIRQs() to dispatch GIRQs of every groups.

 @} */

/** @defgroup GPIO_IRQ_GROUP_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/
#define FILTER_GPIO_LEVEL_PULSE       1
#define SAVE_GPIO_LEVEL_PULSE         1
#define GPIO_IRQ_GROUP_CTRL_GPIO_MASK 1
#define GPIO_IRQ_GPIOAB_GROUP         0xffffU
#define GPIO_IRQ_GPIOCD_GROUP         0xffff0000U
#define ALL_PINS_NUM                  (GPIO_BANK_NUM * PIN_NUMBER_PER_BANK)

#define IS_GPIO_HWIRQ_ID_LINEEAR ((GPIO_IRQ_GROUP_GPION_HWIRQ - GPIO_IRQ_GROUP_GPIO0_HWIRQ) == \
                                  (GPIO_BANK_NUM - 1))

#define IS_DIRQ_GPIO_IRQ(irq) ((irq) >= GPIO_IRQ_GROUP_DIRQ_BASE && \
                               (irq) < (GPIO_IRQ_GROUP_DIRQ_BASE + GPIO_IRQ_GROUP_DIRQ_NUM))

#define IS_NOT_DIRQ_GPIO_IRQ(irq) (!IS_DIRQ_GPIO_IRQ(irq))

#define IS_NOT_GPIO_HWIRQ(irq) ((irq) < GPIO_IRQ_GROUP_GPIO0_HWIRQ || \
                                (irq) > GPIO_IRQ_GROUP_GPION_HWIRQ)

#define IRQ_DBG(fmt, arg...) do { if (0) { HAL_DBG(fmt, ##arg); } } while (0)

/********************* Private Structure Definition **************************/
struct GPIO_IRQ_GROUP_CTRL {
    uint32_t irqInitCpu;
    uint32_t hwIrqCpu;
    uint32_t hwIrqPrioLevel;
    uint32_t unuseGroup;
    uint32_t nextCpusGroupBits[GROUP_PRIO_LEVEL_MAX][PLATFORM_CORE_COUNT];
    uint32_t prioGroupBits[GROUP_PRIO_LEVEL_MAX];
    struct GPIO_IRQ_GROUP_DIRQ_CTRL DIRQInfo[GPIO_IRQ_GROUP_DIRQ_NUM];
};

/********************* Private Variable Definition ***************************/
static struct GPIO_IRQ_GROUP_CFG const *p_gpioIrqCfg;
static struct GPIO_IRQ_GROUP_OPS *g_irqOps;
static struct GPIO_IRQ_GROUP_CTRL s_groupCtrl;

/********************* Private Function Definition ***************************/
#if IS_GPIO_HWIRQ_ID_LINEEAR
struct GPIO_IRQ_GROUP_CFG const *GPIO_IRQ_GROUP_GetHwIrqCfg(uint32_t irq)
{
    struct GPIO_IRQ_GROUP_CFG const *gpioIrqCfg = NULL;
    uint32_t gpioBank;

    for (gpioBank = 0; gpioBank <= GPIO_BANK_NUM; gpioBank++) {
        if (p_gpioIrqCfg[gpioBank].hwIrq == irq) {
            return gpioIrqCfg;
        }
    }

    return NULL;
}
#endif

static bool GPIO_IRQ_GROUP_IsGpioHwIrq(uint32_t irq)
{
#if IS_GPIO_HWIRQ_ID_LINEEAR
    if (GPIO_IRQ_GROUP_GetHwIrqCfg(irq)) {
        return true;
    }
#else
    if (IS_NOT_GPIO_HWIRQ(irq)) {
        return false;
    } else {
        return true;
    }
#endif
}

static void GPIO_IRQ_GROUP_GIrqsDispatchCtrl(uint32_t group,
                                             uint32_t irqActBits,
                                             struct GPIO_REG *gpioReg)
{
    uint32_t irqActBitsHalf, groupLBits, groupHBits;

    groupLBits = group & GPIO_IRQ_GPIOAB_GROUP;
    gpioReg->INT_MASK_L = VAL_MASK_WE(groupLBits, groupLBits);

    groupHBits = (group & GPIO_IRQ_GPIOCD_GROUP) >> 16;
    gpioReg->INT_MASK_H = VAL_MASK_WE(groupHBits, groupHBits);

    irqActBitsHalf = irqActBits & GPIO_IRQ_GPIOAB_GROUP;
    if (irqActBitsHalf) {
        gpioReg->DBCLK_DIV_EN_L = VAL_MASK_WE(groupLBits, irqActBitsHalf);
        gpioReg->PORT_EOI_L = VAL_MASK_WE(irqActBitsHalf, irqActBitsHalf);
    }

    irqActBitsHalf = irqActBits & GPIO_IRQ_GPIOCD_GROUP;
    if (irqActBitsHalf) {
        irqActBitsHalf >>= 16;
        gpioReg->DBCLK_DIV_EN_H = VAL_MASK_WE(groupHBits, irqActBitsHalf);
        gpioReg->PORT_EOI_H = VAL_MASK_WE(irqActBitsHalf, irqActBitsHalf);
    }

    IRQ_DBG("  GIrqsDispatchCtrl act-%lx, mskReg:%lx_%lx, savest:%lx_%lx\n",
            irqActBits, gpioReg->INT_MASK_H, gpioReg->INT_MASK_L,
            gpioReg->DBCLK_DIV_EN_H, gpioReg->DBCLK_DIV_EN_L);
}

static uint32_t GPIO_IRQ_GROUP_GetGroupActIrqs(uint32_t group,
                                               struct GPIO_REG *gpioReg)
{
    uint32_t edgeActBits, irqActBits, intEnReg, grpEnBits;
    uint32_t intTypeEdge, intTypeLevel, saveActBits;
    uint32_t curLevelActBits;

#if SAVE_GPIO_LEVEL_PULSE
    uint32_t levelActBits;
#endif

    intEnReg = gpioReg->INT_EN_H;
    intEnReg <<= 16;
    intEnReg |= gpioReg->INT_EN_L;

    grpEnBits = intEnReg & group;

    saveActBits = (gpioReg->DBCLK_DIV_EN_H << 16) | gpioReg->DBCLK_DIV_EN_L;

    gpioReg->DBCLK_DIV_EN_L = MASK_TO_WE(group & GPIO_IRQ_GPIOAB_GROUP);
    gpioReg->DBCLK_DIV_EN_H = MASK_TO_WE((group & GPIO_IRQ_GPIOCD_GROUP) >> 16);

    __DSB();

    /* may be level type */
    if (!(saveActBits && group)) {
        HAL_DBG_WRN("GetGroupActIrqs invalid grp=%lx %lx && %lx\n",
                    group, saveActBits, group);
    }

    intTypeEdge = (gpioReg->INT_TYPE_H << 16) | gpioReg->INT_TYPE_L;
    edgeActBits = saveActBits & intTypeEdge;

    intTypeLevel = ~intTypeEdge;

    curLevelActBits = (gpioReg->INT_RAWSTATUS & intTypeLevel);

#if SAVE_GPIO_LEVEL_PULSE
    levelActBits = saveActBits & intTypeLevel;
    irqActBits = (levelActBits | curLevelActBits | edgeActBits) & grpEnBits;
    IRQ_DBG(" GetGroupIrqs grp: %lx-%lx-%lx edge:%lx-%lx lvl:%lx-%lx-%lx\n",
            group, irqActBits, saveActBits, edgeActBits, intTypeEdge,
            levelActBits, curLevelActBits, intTypeLevel);
#else
    irqActBits = (curLevelActBits | edgeActBits) & grpEnBits;
    IRQ_DBG(" GetGroupIrqs grp: %lx-%lx-%lx edge:%lx-%lx lvl:%lx-%lx\n",
            group, irqActBits, saveActBits, edgeActBits, intTypeEdge,
            levelActBits, intTypeLevel);
#endif

    return irqActBits;
}

static HAL_Status GPIO_IRQ_GROUP_EndGroupIrqs(struct GPIO_IRQ_GROUP_CFG const *gpioIrqCfg, uint32_t group,
                                              uint32_t irqActBits,
                                              struct GPIO_REG *gpioReg,
                                              uint32_t irq)
{
    uint32_t groupBitsL, groupBitsH, groupRawIrq, intTypeEdge;
    HAL_Status ret = HAL_OK;

    IRQ_DBG(" EndGrpIrqs %lx-%lx m:%lx_%lx e:%lx_%lx s:%lx_%lx\n",
            group, irqActBits,
            gpioReg->INT_MASK_H, gpioReg->INT_MASK_L,
            gpioReg->INT_EN_H, gpioReg->INT_EN_L,
            gpioReg->DBCLK_DIV_EN_H, gpioReg->DBCLK_DIV_EN_L);

    groupBitsL = group & GPIO_IRQ_GPIOAB_GROUP;
    groupBitsH = (group & GPIO_IRQ_GPIOCD_GROUP) >> 16;

    gpioReg->INT_MASK_L = VAL_MASK_WE(groupBitsL, 0);
    gpioReg->INT_MASK_H = VAL_MASK_WE(groupBitsH, 0);
    __DSB();
    intTypeEdge = (gpioReg->INT_TYPE_H << 16) | gpioReg->INT_TYPE_L;
    groupRawIrq = (gpioReg->INT_RAWSTATUS & intTypeEdge) & group;

    if (groupRawIrq) {
        g_irqOps->setIrqPending(gpioIrqCfg->hwIrq);
        ret = HAL_BUSY;
    }

    IRQ_DBG(" EndGrpIrqs %lx-%lx raw-%lx e:%lx_%lx s:%lx_%lx end\n",
            group, irqActBits, groupRawIrq,
            gpioReg->INT_EN_H, gpioReg->INT_EN_L,
            gpioReg->DBCLK_DIV_EN_H, gpioReg->DBCLK_DIV_EN_L);

    return ret;
}

#if !GPIO_IRQ_GROUP_CTRL_GPIO_MASK
static int DispatchGIRQsBusy(struct GPIO_IRQ_GROUP_PRIO_GROUP const *prioGroup,
                             struct GPIO_IRQ_GROUP_CFG const *gpioIrqCfg,
                             uint32_t prioLevel,
                             uint32_t cpu,
                             uint32_t irq,
                             uint32_t grp,
                             uint32_t intEnReg,
                             uint32_t unmskReg,
                             uint32_t mskReg)
{
    uint32_t grpEnBits, grpUnmskBits, grpMskBits;

    grpEnBits = intEnReg & grp;
    grpUnmskBits = unmskReg & grpEnBits;

    /* all irq must in unmask status in a group */
    if (grpUnmskBits != grpEnBits) {
        grpMskBits = mskReg & grpEnBits;
        IRQ_DBG(" DispatchGIrqs busy %ld-%ld st:%lx\n",
                prioLevel, cpu, g_irqOps->getIrqStatus(irq));
        IRQ_DBG(" DispatchGIrqs busy reg:%lx %lx %lx %lx %lx\n",
                intEnReg, mskReg, unmskReg, grpEnBits, grpMskBits);

        if (grpEnBits == grpMskBits) {
            return 1;
        }

        if (g_irqOps->getIrqStatus(prioGroup->GIRQId[cpu])) {
            return 1;
        }

        if (prioGroup->GIRQId[cpu] != gpioIrqCfg->hwIrq) {
            g_irqOps->setIrqPending(prioGroup->GIRQId[cpu]);
        }

        return 1;
    }

    return 0;
}
#endif

static void GPIO_IRQ_GROUP_DispatchPinsIrq(uint32_t bits,
                                           struct GPIO_IRQ_GROUP_CFG
                                           const *gpioIrqCfg)
{
    uint32_t pin;

    for (pin = 0; pin < PIN_NUMBER_PER_BANK; pin++) {
        if (bits & HAL_BIT(pin)) {
            HAL_GPIO_IRQDispatch(gpioIrqCfg->bank, pin);
        }
    }
}

static int GPIO_IRQ_Group_bankIrqsInit(uint32_t bank, uint32_t irqCfg)
{
    struct GPIO_IRQ_GROUP_CFG const *gpioIrqCfg;
    uint32_t aff;

    gpioIrqCfg = &p_gpioIrqCfg[bank];

    aff = __get_MPIDR() & MPIDR_AFFINITY_MASK;

    if (irqCfg) {
        IRQ_DBG(" **bankIrqsInit irqCfg %ld %d %lx\n", bank, gpioIrqCfg->hwIrq,
                gpioIrqCfg->bankTypeCfg.prio);
        g_irqOps->setIrqPriority(gpioIrqCfg->hwIrq,
                                 gpioIrqCfg->bankTypeCfg.prio);
    }

    if (aff == gpioIrqCfg->bankTypeCfg.hwIrqCpuAff) {
        IRQ_DBG(" **bankIrqsInit %ld-%d %lx=%lx\n", bank, gpioIrqCfg->hwIrq,
                aff, gpioIrqCfg->bankTypeCfg.hwIrqCpuAff);
        g_irqOps->setIrqHandler(gpioIrqCfg->hwIrq, GPIO_IRQ_GROUP_BANK_TYPE,
                                false, (void *)gpioIrqCfg);
        g_irqOps->enableIrq(gpioIrqCfg->hwIrq);
    }

    return HAL_OK;
}

static int GPIO_IRQ_GROUP_GirqCfgCheck(uint32_t bank)
{
    uint32_t prioLevel, cpu, irq, getHwirq, highprioLevel, prioGrpMsk, grp;
    struct GPIO_IRQ_GROUP_CFG const *gpioIrqCfg;
    struct GPIO_IRQ_GROUP_PRIO_GROUP const *prioGroup;

    gpioIrqCfg = &p_gpioIrqCfg[bank];

    getHwirq = 0;
    highprioLevel = GROUP_PRIO_LEVEL_MAX;

    for (prioLevel = 0; prioLevel < GROUP_PRIO_LEVEL_MAX; prioLevel++) {
        prioGroup = &gpioIrqCfg->prioGroup[prioLevel];
        prioGrpMsk = 0;

        for (cpu = 0; cpu < PLATFORM_CORE_COUNT; cpu++) {
            IRQ_DBG("  *GirqCfgCheck %ld-%ld: grp-%lx irq-%d\n", prioLevel, cpu,
                    prioGroup->cpuGroup[cpu], prioGroup->GIRQId[cpu]);

            grp = prioGroup->cpuGroup[cpu];
            if (!grp) {
                continue;
            }
            prioGrpMsk |= grp;

            irq = prioGroup->GIRQId[cpu];
            if (!GPIO_IRQ_GROUP_IsGpioHwIrq(irq) && IS_NOT_DIRQ_GPIO_IRQ(irq)) {
                return -1;
            }

            if (irq == gpioIrqCfg->hwIrq) {
                if (!getHwirq) {
                    getHwirq = 1;
                    s_groupCtrl.hwIrqCpu = cpu;
                    s_groupCtrl.hwIrqPrioLevel = prioLevel;
                } else {
                    return -1;
                }
            }

            s_groupCtrl.unuseGroup |= grp;

            if (highprioLevel > prioLevel) {
                highprioLevel = prioLevel;
            }
        }

        s_groupCtrl.prioGroupBits[prioLevel] = prioGrpMsk;

        for (cpu = 0; cpu < PLATFORM_CORE_COUNT; cpu++) {
            grp = prioGroup->cpuGroup[cpu];
            prioGrpMsk &= ~grp;
            s_groupCtrl.nextCpusGroupBits[prioLevel][cpu] = prioGrpMsk;
        }
    }

    if (!getHwirq) {
        return -1;
    }

    if (s_groupCtrl.hwIrqPrioLevel > highprioLevel) {
        return -1;
    }

    IRQ_DBG("  *GirqCfgCheck hwirq:%d, %ld-%ld\n", gpioIrqCfg->hwIrq,
            s_groupCtrl.hwIrqPrioLevel, s_groupCtrl.hwIrqCpu);

    return 0;
}

static int GPIO_IRQ_GROUP_GIrqCfg(uint32_t bank)
{
    uint32_t prioLevel, cpu, irq;
    struct GPIO_IRQ_GROUP_CFG const *gpioIrqCfg;
    struct GPIO_IRQ_GROUP_PRIO_GROUP const *prioGroup;

    for (prioLevel = 0; prioLevel < GROUP_PRIO_LEVEL_MAX;
         prioLevel++) {
        for (cpu = 0; cpu < PLATFORM_CORE_COUNT; cpu++) {
            gpioIrqCfg = &p_gpioIrqCfg[bank];
            prioGroup = &gpioIrqCfg->prioGroup[prioLevel];
            if (!prioGroup->cpuGroup[cpu]) {
                continue;
            }
            irq = prioGroup->GIRQId[cpu];
            g_irqOps->setIrqPriority(irq, prioGroup->prio);
            IRQ_DBG("  *GIrqCfg bank-%ld level:%ld-%ld-%ld %lx\n",
                    bank, prioLevel, cpu, irq, prioGroup->prio);
        }
    }

    return HAL_OK;
}

static void GPIO_IRQ_GROUP_GpioInit(uint32_t bank)
{
    struct GPIO_IRQ_GROUP_CFG const *gpioIrqCfg;
    struct GPIO_REG *gpioReg;

    gpioIrqCfg = &p_gpioIrqCfg[bank];
    gpioReg = gpioIrqCfg->gpioReg;

    printf("GROUP_GpioInit bank:%ld :%lx,%lx, %lx\n",
           bank, gpioReg->DBCLK_DIV_CON,
           gpioReg->DBCLK_DIV_EN_L, gpioReg->DBCLK_DIV_EN_H);

    gpioReg->DBCLK_DIV_CON = 0;
    gpioReg->DBCLK_DIV_EN_L = VAL_MASK_WE(0xffff, 0);
    gpioReg->DBCLK_DIV_EN_H = VAL_MASK_WE(0xffff, 0);

    printf("GROUP_GpioInit end-%lx,%lx, %lx\n", gpioReg->DBCLK_DIV_CON,
           gpioReg->DBCLK_DIV_EN_L, gpioReg->DBCLK_DIV_EN_H);
}
static int GPIO_IRQ_GROUP_GIrqCreate(uint32_t bank, uint32_t cpu,
                                     uint32_t prioLevel)
{
    uint32_t DIRQIdx, irq, group;
    struct GPIO_IRQ_GROUP_CFG const *gpioIrqCfg;

#if GPIO_IRQ_GROUP_CTRL_GPIO_MASK
    struct GPIO_REG *gpioReg;
#endif

    gpioIrqCfg = &p_gpioIrqCfg[bank];

    group = gpioIrqCfg->prioGroup[prioLevel].cpuGroup[cpu];

    if (!group) {
        return 0;
    }

    irq = gpioIrqCfg->prioGroup[prioLevel].GIRQId[cpu];

    IRQ_DBG("  *GIrqCreate bank-%ld cpu-%ld level-%ld irq-%ld\n",
            bank, cpu, prioLevel, irq);

    if (IS_DIRQ_GPIO_IRQ(irq)) {
        DIRQIdx = irq - GPIO_IRQ_GROUP_DIRQ_BASE;
        if (s_groupCtrl.DIRQInfo[DIRQIdx].enable) {
            printf("   IrqCreate invaild DIRQ:%ld-%ld!!\n",
                   DIRQIdx, irq);

            return -1;
        }
        s_groupCtrl.DIRQInfo[DIRQIdx].enable = 1;
        s_groupCtrl.DIRQInfo[DIRQIdx].gpioIrqCfg = gpioIrqCfg;
        s_groupCtrl.DIRQInfo[DIRQIdx].cpu = cpu;
        s_groupCtrl.DIRQInfo[DIRQIdx].prioLevel = prioLevel;
        s_groupCtrl.DIRQInfo[DIRQIdx].irq = irq;
        g_irqOps->setIrqHandler(irq, GPIO_IRQ_GROUP_GROUP_TYPE, false,
                                &s_groupCtrl.DIRQInfo[DIRQIdx]);
        IRQ_DBG("   IrqCreate DIRQ enable %ld-%ld\n", DIRQIdx, irq);
    } else if (GPIO_IRQ_GROUP_IsGpioHwIrq(irq)) {
        g_irqOps->setIrqHandler(irq, GPIO_IRQ_GROUP_GROUP_TYPE, true,
                                (void *)gpioIrqCfg);
        IRQ_DBG("   GIrqCreate hw irq enable %ld\n", irq);
    } else {
        return -1;
    }

#if GPIO_IRQ_GROUP_CTRL_GPIO_MASK
    gpioReg = gpioIrqCfg->gpioReg;
    gpioReg->INT_MASK_L = VAL_MASK_WE(group & GPIO_IRQ_GPIOAB_GROUP, 0);
    gpioReg->INT_MASK_H = VAL_MASK_WE((group & GPIO_IRQ_GPIOCD_GROUP) >> 16, 0);
#endif
    g_irqOps->enableIrq(irq);

    return 0;
}

static int GPIO_IRQ_GROUP_GIrqsInit(uint32_t bank, uint32_t irqCfg)
{
    uint32_t prioLevel, curCpu;
    int ret;

    curCpu = HAL_CPU_TOPOLOGY_GetCurrentCpuId();

    IRQ_DBG(" **GIrqsInit bank-%ld curCPU-%ld\n", bank, curCpu);

    ret = GPIO_IRQ_GROUP_GirqCfgCheck(bank);
    if (ret) {
        return ret;
    }

    if (irqCfg) {
        GPIO_IRQ_GROUP_GpioInit(bank);
        GPIO_IRQ_GROUP_GIrqCfg(bank);
    }

    for (prioLevel = 0; prioLevel < GROUP_PRIO_LEVEL_MAX;
         prioLevel++) {
        ret = GPIO_IRQ_GROUP_GIrqCreate(bank, curCpu, prioLevel);
        if (ret) {
            return ret;
        }
    }

    return HAL_OK;
}

static int GPIO_IRQ_Group_init(uint32_t irqCfg)
{
    uint32_t bank;
    int ret;
    struct GPIO_IRQ_GROUP_CFG const *gpioIrqCfg;

    for (bank = 0; bank < GPIO_BANK_NUM; bank++) {
        gpioIrqCfg = &p_gpioIrqCfg[bank];

        IRQ_DBG("***irq group int loop %d=%ld: %d en-%d\n", gpioIrqCfg->bank,
                bank, gpioIrqCfg->hwIrq, gpioIrqCfg->groupIrqEn);

        if (!gpioIrqCfg->gpioReg) {
            return -1;
        }

        if (!GPIO_IRQ_GROUP_IsGpioHwIrq(gpioIrqCfg->hwIrq)) {
            return -1;
        }

        if (gpioIrqCfg->bank != bank) {
            return -1;
        }

        if (gpioIrqCfg->groupIrqEn == GPIO_IRQ_GROUP_EN_GROUP_TYPE) {
            ret = GPIO_IRQ_GROUP_GIrqsInit(bank, irqCfg);
            if (ret) {
                return ret;
            }
        } else if (gpioIrqCfg->groupIrqEn == GPIO_IRQ_GROUP_EN_BANK_TYPE) {
            GPIO_IRQ_Group_bankIrqsInit(bank, irqCfg);
        }
    }

    return 0;
}
/** @} */

/********************* Public Function Definition ****************************/

/** @defgroup GPIO_IRQ_GROUP_Exported_Functions_Group5 Other Functions
 *  @{
 */

/**
 * @brief  dispatch irqs of every groups
 * @param  irq:  HWIRQ of a gpio bank.
 * @return HAL_Status.
 */
HAL_Status HAL_GPIO_IRQ_GROUP_DispatchGIRQs(uint32_t irq)
{
    struct GPIO_IRQ_GROUP_CFG const *gpioIrqCfg;
    struct GPIO_IRQ_GROUP_PRIO_GROUP const *prioGroup;
    struct GPIO_REG *gpioReg;
    uint32_t cpu, prioLevel, grp, gpioBank, irqActBits, rawStatus;
    uint32_t unmskReg, mskReg, grpsState;

#if GPIO_IRQ_GROUP_CTRL_GPIO_MASK
    uint32_t grpUnmskBits;
    HAL_UNUSED uint32_t grpMskBits;
#endif

#if IS_GPIO_HWIRQ_ID_LINEEAR
    gpioIrqCfg = GPIO_IRQ_GROUP_GetHwIrqCfg(irq);
    if (!gpioIrqCfg) {
        return HAL_INVAL;
    }
#else
    if (IS_NOT_GPIO_HWIRQ(irq)) {
        return HAL_INVAL;
    }
    gpioBank = irq - GPIO_IRQ_GROUP_GPIO0_HWIRQ;
    gpioIrqCfg = &p_gpioIrqCfg[gpioBank];
#endif

    if (gpioIrqCfg->groupIrqEn != GPIO_IRQ_GROUP_EN_GROUP_TYPE) {
        return HAL_OK;
    }

    gpioReg = gpioIrqCfg->gpioReg;
    rawStatus = gpioReg->INT_RAWSTATUS;

    IRQ_DBG("**DispatchGIrqs %ld-%ld st:%lx\n",
            irq, HAL_CPU_TOPOLOGY_GetCurrentCpuId(), rawStatus);
    /*
      * 1. read INT_MASK reg first, then INT_EN.
      * 2. for HAL_GPIO_DisableIRQ(), must set INT_EN first, then INT_MASK.
      * 3. for HAL_GPIO_EnableIRQ(), must set INT_MASK first, then INT_EN.
      */

    mskReg = gpioReg->INT_MASK_H;
    mskReg <<= 16;
    mskReg |= gpioReg->INT_MASK_L;
    unmskReg = ~mskReg;

    for (prioLevel = 0; prioLevel < GROUP_PRIO_LEVEL_MAX; prioLevel++) {
        grpsState = rawStatus & s_groupCtrl.prioGroupBits[prioLevel];
        if (!grpsState) {
            continue;
        }
        prioGroup = &gpioIrqCfg->prioGroup[prioLevel];

        for (cpu = 0; cpu < PLATFORM_CORE_COUNT; cpu++) {
            grp = prioGroup->cpuGroup[cpu];
            irqActBits = grp & rawStatus;
            if (!irqActBits) {
                continue;
            }

#if GPIO_IRQ_GROUP_CTRL_GPIO_MASK
            grpUnmskBits = unmskReg & grp;
            /* all irq must in unmask status in a group */
            if (grpUnmskBits != grp) {
                grpMskBits = mskReg & grp;
                IRQ_DBG(" DispatchGIrqs busy %ld-%ld st:%lx\n",
                        prioLevel, cpu, g_irqOps->getIrqStatus(irq));
                IRQ_DBG(" DispatchGIrqs busy reg:%lx %lx %lx\n",
                        grp, grpUnmskBits, grpMskBits);

                continue;
            }
#else
            if (DispatchGIRQsBusy(prioGroup, gpioIrqCfg, prioLevel, cpu, irq, grp,
                                  (gpioReg->INT_EN_H << 16) | gpioReg->INT_EN_L,
                                  unmskReg, mskReg)) {
                continue;
            }
#endif

            IRQ_DBG(" DispatchGIrqs:%ld-%ld-%d-%lx-%lx:%lx\n", prioLevel, cpu,
                    prioGroup->GIRQId[cpu], grp,
                    ((gpioReg->INT_EN_H << 16) | gpioReg->INT_EN_L) & grp, irqActBits);

            GPIO_IRQ_GROUP_GIrqsDispatchCtrl(grp, irqActBits, gpioReg);

            if (prioGroup->GIRQId[cpu] != gpioIrqCfg->hwIrq) {
                g_irqOps->setIrqPending(prioGroup->GIRQId[cpu]);
            }

            grpsState = s_groupCtrl.nextCpusGroupBits[prioLevel][cpu];

            IRQ_DBG("  DispatchGIrqs st:%ld-%ld:%lx %lx\n",
                    prioLevel, cpu, grpsState, grpsState & rawStatus);

            grpsState &= rawStatus;
            if (grpsState) {
                continue;
            } else {
                break;
            }
        }
    }

    return HAL_OK;
}

/**
 * @brief  dispatch pin irqs of a irq group by a HWIRQ.
 * @param  irq:  HWIRQ of a gpio bank.
 * @param  args:  private parameters
 * @return HAL_Status.
 */
HAL_Status HAL_GPIO_IRQ_GROUP_HWIRQDispatchIrqs(uint32_t irq, void *args)
{
    uint32_t cpu, level, group, irqActBits, mskReg, grpMskBits;
    struct GPIO_REG *gpioReg;
    struct GPIO_IRQ_GROUP_CFG const *gpioIrqCfg = args;

    gpioReg = gpioIrqCfg->gpioReg;
    cpu = s_groupCtrl.hwIrqCpu;
    level = s_groupCtrl.hwIrqPrioLevel;

    group = gpioIrqCfg->prioGroup[level].cpuGroup[cpu];

    if (!group) {
        return HAL_OK;
    }

    IRQ_DBG("**HwIrqDispatchIrqs %ld-%ld-%ld-%lx\n", level, cpu, irq, group);

    __disable_irq();

    mskReg = gpioReg->INT_MASK_H;
    mskReg <<= 16;
    mskReg |= gpioReg->INT_MASK_L;

    grpMskBits = mskReg & group;

    if (grpMskBits != group) {
        return HAL_OK;
    }

    irqActBits = GPIO_IRQ_GROUP_GetGroupActIrqs(group, gpioReg);

    if (irqActBits) {
        __enable_irq();
        GPIO_IRQ_GROUP_DispatchPinsIrq(irqActBits, gpioIrqCfg);
        __disable_irq();
    }

    GPIO_IRQ_GROUP_EndGroupIrqs(gpioIrqCfg, group, irqActBits, gpioReg, irq);

    return HAL_OK;
}

/**
 * @brief  dispatch pin irqs of a irq group by a DIRQ.
 * @param  irq:  the DIRQ to dispatch gpio pin irqs.
 * @param  args:  private parameters
 * @return HAL_Status.
 */
HAL_Status HAL_GPIO_IRQ_GROUP_DIRQDispatchIrqs(uint32_t irq, void *args)
{
    uint32_t group, irqActBits, mskReg, grpMskBits;
    struct GPIO_IRQ_GROUP_DIRQ_CTRL *DIRQCtrl = args;
    struct GPIO_IRQ_GROUP_CFG const *gpioIrqCfg;
    struct GPIO_REG *gpioReg;

    gpioIrqCfg = DIRQCtrl->gpioIrqCfg;
    gpioReg = gpioIrqCfg->gpioReg;

    group = gpioIrqCfg->prioGroup[DIRQCtrl->prioLevel].cpuGroup[DIRQCtrl->cpu];

    if (!group) {
        return HAL_OK;
    }

    IRQ_DBG("**DIRQDispatchIrqs %ld-%ld-%ld-%lx %lx\n", DIRQCtrl->prioLevel,
            DIRQCtrl->cpu, irq, group, g_irqOps->getIrqStatus(irq));

    __disable_irq();

    mskReg = gpioReg->INT_MASK_H;
    mskReg <<= 16;
    mskReg |= gpioReg->INT_MASK_L;

    grpMskBits = mskReg & group;

    if (grpMskBits != group) {
        return HAL_OK;
    }

    irqActBits = GPIO_IRQ_GROUP_GetGroupActIrqs(group, gpioReg);

    if (irqActBits) {
        __enable_irq();
        GPIO_IRQ_GROUP_DispatchPinsIrq(irqActBits, gpioIrqCfg);
        __disable_irq();
    }

    GPIO_IRQ_GROUP_EndGroupIrqs(gpioIrqCfg, group, irqActBits, gpioReg, irq);

    return HAL_OK;
}

/**
 * @brief  Some controls for a gpio pin must be operated by the cpu which is
 *         responsible for interrupt handling of the gpio pin. Use this
 *         function check the information of the cpu.
 * @param  pGPIO: base address.
 * @param  pin: The pin number defined in @ref ePINCTRL_GPIO_PINS.
 * @param  irqStatus: the irq mask status for arm core.
 * @return -1 if error; 0 if not need to check, 1 if need to check and information is ok.
 */
int HAL_GPIO_IRQ_GROUP_GpioCtrlEnter(struct GPIO_REG *pGPIO,
                                     uint32_t pin, uint32_t *irqStatus)
{
    uint32_t bank, cpu, getPin, prioLevel;
    struct GPIO_IRQ_GROUP_CFG const *gpioIrqCfg;

    if (!p_gpioIrqCfg) {
        return 0;
    }

    for (bank = 0; bank < GPIO_BANK_NUM; bank++) {
        gpioIrqCfg = &p_gpioIrqCfg[bank];
        if (gpioIrqCfg->gpioReg == pGPIO) {
            break;
        }
    }

    if (bank >= GPIO_BANK_NUM) {
        return -1;
    }

    if (gpioIrqCfg->groupIrqEn != GPIO_IRQ_GROUP_EN_GROUP_TYPE) {
        return 0;
    }

    cpu = HAL_CPU_TOPOLOGY_GetCurrentCpuId();
    getPin = 0;

    for (prioLevel = 0; prioLevel < GROUP_PRIO_LEVEL_MAX; prioLevel++) {
        getPin = gpioIrqCfg->prioGroup[prioLevel].cpuGroup[cpu] & pin;
        if (getPin) {
            break;
        }
    }

    if (!getPin) {
        return -1;
    }

    *irqStatus = __get_CPSR();
    __disable_irq();

    return 1;
}

/**
 * @brief  reference HAL_GPIO_IRQ_GpioIrqCtrlEnter().
 * @param  pGPIO: base address.
 * @param  pin: The pin number defined in @ref ePINCTRL_GPIO_PINS.
 * @param  irqStatus:  the irq mask status for arm core.
 * @return HAL_Status.
 */
HAL_Status HAL_GPIO_IRQ_GROUP_GpioCtrlExit(struct GPIO_REG *pGPIO,
                                           uint32_t pin, uint32_t irqStatus)
{
    /* resume irq status */
    __set_mode(irqStatus);

    return HAL_OK;
}

/** @} */

/** @defgroup GPIO_IRQ_GROUP_Exported_Functions_Group4 Init and DeInit Functions

 This section provides functions allowing to init and deinit the module:

 *  @{
 */

/**
 * @brief  Initialize GPIO_IRQ_GROUP.
 * @param  cfgCpuAff: the cpu affinity specified to config irq, from CPU_GET_AFFINITY().
 * @param  cfg: gpio irq group config data
 * @param  ops: operation callback for operating gic.
 * @return HAL_Status.
 */
HAL_Status HAL_GPIO_IRQ_GROUP_Init(uint32_t cfgCpuAff,
                                   struct GPIO_IRQ_GROUP_CFG const *cfg,
                                   struct GPIO_IRQ_GROUP_OPS *ops)
{
    uint32_t irqCfg, aff;

    if (!cfg || !ops) {
        return HAL_ERROR;
    }

    p_gpioIrqCfg = cfg;

    if (!ops->setIrqPriority || !ops->enableIrq || !ops->getIrqStatus ||
        !ops->setIrqPending || !ops->setIrqHandler) {
        ops = NULL;

        return HAL_ERROR;
    }

    g_irqOps = ops;

    aff = __get_MPIDR() & MPIDR_AFFINITY_MASK;

    if (aff == cfgCpuAff) {
        irqCfg = 1;
    } else {
        irqCfg = 0;
    }

    GPIO_IRQ_Group_init(irqCfg);

    return HAL_OK;
}

/** @} */

/** @} */

/** @} */
#endif
