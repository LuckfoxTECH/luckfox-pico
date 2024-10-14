/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_GIC_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup GIC
 *  @{
 */

/** @defgroup GIC_How_To_Use How To Use
 *  @{

 The GIC driver can be used as follows:
 - Invoke HAL_GIC_Init() to initialize GIC base address and config GIC.
 - Invoke HAL_GIC_Disable() to disable a IRQ.
 - Invoke HAL_GIC_Enable() to enable a IRQ.
 - Invoke HAL_GIC_GetEnableState() to get the enable state of a IRQ.
 - Invoke HAL_GIC_GetActiveIRQ() to get the IRQ ID of the active IRQ.
 - Invoke HAL_GIC_EndOfInterrupt() to end the active IRQ.
 - Invoke HAL_GIC_SetPending() to make a IRQ PENDING.
 - Invoke HAL_GIC_GetPending() to get the Pending state of a IRQ.
 - Invoke HAL_GIC_GetIRQStatus() to get the Pending and active status of a IRQ.
 - Invoke HAL_GIC_ClearPending() to clear the Pending state of a IRQ.
 - Invoke HAL_GIC_SetPriority() to set priority for a IRQ.
 - Invoke HAL_GIC_SetPriorityMask() to set the priority mask.
 - Invoke HAL_GIC_GetPriorityMask() to get the priority mask.
 - Invoke HAL_GIC_GetPriority() to get the priority value of a IRQ.
 - Invoke HAL_GIC_SetIRouter() to set the routting affinity value of a IRQ.
 - Invoke HAL_GIC_SendSGI() to send a sgi.
 - Invoke HAL_GIC_SetDir() to deactivate a IRQ.

 @} */

/** @defgroup GIC_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/
#define RESERVED(N, T)         T RESERVED##N;
#define GICR_CPU_BASE(cpu)     (GIC_REDISTRIBUTOR_BASE + (0x20000 * (cpu)))
#define GICR_SGI_CPU_BASE(cpu) (GICR_CPU_BASE(cpu) + 0x10000)

#define ICC_SGI1R_TARGET_LIST_SHIFT    0
#define ICC_SGI1R_TARGET_LIST_MASK     (0xffff << ICC_SGI1R_TARGET_LIST_SHIFT)
#define ICC_SGI1R_AFFINITY_1_SHIFT     16
#define ICC_SGI1R_AFFINITY_1_MASK      (0xff << ICC_SGI1R_AFFINITY_1_SHIFT)
#define ICC_SGI1R_SGI_ID_SHIFT         24
#define ICC_SGI1R_SGI_ID_MASK          (0xfULL << ICC_SGI1R_SGI_ID_SHIFT)
#define ICC_SGI1R_AFFINITY_2_SHIFT     32
#define ICC_SGI1R_AFFINITY_2_MASK      (0xffULL << ICC_SGI1R_AFFINITY_2_SHIFT)
#define ICC_SGI1R_IRQ_ROUTING_MODE_BIT 40
#define ICC_SGI1R_RS_SHIFT             44
#define ICC_SGI1R_RS_MASK              (0xfULL << ICC_SGI1R_RS_SHIFT)
#define ICC_SGI1R_AFFINITY_3_SHIFT     48
#define ICC_SGI1R_AFFINITY_3_MASK      (0xffULL << ICC_SGI1R_AFFINITY_3_SHIFT)

#define MPIDR_LEVEL_BITS 8
#define MPIDR_LEVEL_MASK ((1 << MPIDR_LEVEL_BITS) - 1)

#define MPIDR_AFFINITY_LEVEL(mpidr, level) \
    ((mpidr >> (MPIDR_LEVEL_BITS * level)) & MPIDR_LEVEL_MASK)

#define MPIDR_TO_SGI_AFFINITY(cluster_id, level) \
    (MPIDR_AFFINITY_LEVEL(cluster_id, level)     \
        << ICC_SGI1R_AFFINITY_## level ##_SHIFT)

#define MPIDR_RS(mpidr)                (((mpidr) & 0xF0UL) >> 4)
#define MPIDR_TO_SGI_RS(mpidr)         (MPIDR_RS(mpidr) << ICC_SGI1R_RS_SHIFT)
#define MPIDR_TO_SGI_CLUSTER_ID(mpidr) ((mpidr) & ~0xFUL)
#define GET_CUR_MPIDR_AFF()            (__get_MPIDR() & MPIDR_AFFINITY_MASK)
#define GIC_VLD_MSK_GRP_NUM            (NUM_INTERRUPTS/32 +1)
#define GIC_DBG(fmt, arg...)           do { if (0) { HAL_DBG(fmt, ##arg); } } while (0)

#define AMP_WAIT_INIT_TIME_US (5000*1000)
#define AMP_WAIT_INIT_ONCE_US (100)
#define AMP_WAIT_INIT_LOOPS   (AMP_WAIT_INIT_TIME_US / \
                                        AMP_WAIT_INIT_ONCE_US)
/********************* Private Structure Definition **************************/
struct  GIC_DISTRIBUTOR_REG {
    __IO uint32_t CTLR;               /* brief  Offset: 0x000 (R/W) Distributor Control Register */
    __I uint32_t TYPER;               /* brief  Offset: 0x004 (R/ ) Interrupt Controller Type Register */
    __I uint32_t IIDR;                /* brief  Offset: 0x008 (R/ ) Distributor Implementer Identification Register */
    RESERVED(0, uint32_t)
    __IO uint32_t STATUSR;            /* brief  Offset: 0x010 (R/W) Error Reporting Status Register, optional */
    RESERVED(1[11], uint32_t)
    __O uint32_t SETSPI_NSR;          /* brief  Offset: 0x040 ( /W) Set SPI Register */
    RESERVED(2, uint32_t)
    __O uint32_t CLRSPI_NSR;          /* brief  Offset: 0x048 ( /W) Clear SPI Register */
    RESERVED(3, uint32_t)
    __O uint32_t SETSPI_SR;           /* brief  Offset: 0x050 ( /W) Set SPI, Secure Register */
    RESERVED(4, uint32_t)
    __O uint32_t CLRSPI_SR;           /* brief  Offset: 0x058 ( /W) Clear SPI, Secure Register */
    RESERVED(5[9], uint32_t)
    __IO uint32_t IGROUPR[32];        /* brief  Offset: 0x080 (R/W) Interrupt Group Registers */
    __IO uint32_t ISENABLER[32];      /* brief  Offset: 0x100 (R/W) Interrupt Set-Enable Registers */
    __IO uint32_t ICENABLER[32];      /* brief  Offset: 0x180 (R/W) Interrupt Clear-Enable Registers */
    __IO uint32_t ISPENDR[32];        /* brief  Offset: 0x200 (R/W) Interrupt Set-Pending Registers */
    __IO uint32_t ICPENDR[32];        /* brief  Offset: 0x280 (R/W) Interrupt Clear-Pending Registers */
    __IO uint32_t ISACTIVER[32];      /* brief  Offset: 0x300 (R/W) Interrupt Set-Active Registers */
    __IO uint32_t ICACTIVER[32];      /* brief  Offset: 0x380 (R/W) Interrupt Clear-Active Registers */
    __IO uint32_t IPRIORITYR[255];    /* brief  Offset: 0x400 (R/W) Interrupt Priority Registers */
    RESERVED(6, uint32_t)
    __IO uint32_t ITARGETSR[255];     /* brief  Offset: 0x800 (R/W) Interrupt Targets Registers */
    RESERVED(7, uint32_t)
    __IO uint32_t ICFGR[64];          /* brief  Offset: 0xC00 (R/W) Interrupt Configuration Registers */
    __IO uint32_t IGRPMODR[32];       /* brief  Offset: 0xD00 (R/W) Interrupt Group Modifier Registers */
    RESERVED(8[32], uint32_t)
    __IO uint32_t NSACR[64];          /* brief  Offset: 0xE00 (R/W) Non-secure Access Control Registers */
    __O uint32_t SGIR;                /* brief  Offset: 0xF00 ( /W) Software Generated Interrupt Register */
    RESERVED(9[3], uint32_t)
    __IO uint32_t CPENDSGIR[4];       /* brief  Offset: 0xF10 (R/W) SGI Clear-Pending Registers */
    __IO uint32_t SPENDSGIR[4];       /* brief  Offset: 0xF20 (R/W) SGI Set-Pending Registers */
    RESERVED(10[5236], uint32_t)
    __IO uint64_t IROUTER[988];       /* brief  Offset: 0x6100(R/W) Interrupt Routing Registers */
};
#ifdef HAL_GIC_V2
struct GIC_CPU_INTERFACE_REG {
    __IO uint32_t CTLR;               /*!< \brief  Offset: 0x000 (R/W) CPU Interface Control Register */
    __IO uint32_t PMR;                /*!< \brief  Offset: 0x004 (R/W) Interrupt Priority Mask Register */
    __IO uint32_t BPR;                /*!< \brief  Offset: 0x008 (R/W) Binary Point Register */
    __I uint32_t IAR;                 /*!< \brief  Offset: 0x00C (R/ ) Interrupt Acknowledge Register */
    __O uint32_t EOIR;                /*!< \brief  Offset: 0x010 ( /W) End Of Interrupt Register */
    __I uint32_t RPR;                 /*!< \brief  Offset: 0x014 (R/ ) Running Priority Register */
    __I uint32_t HPPIR;               /*!< \brief  Offset: 0x018 (R/ ) Highest Priority Pending Interrupt Register */
    __IO uint32_t ABPR;               /*!< \brief  Offset: 0x01C (R/W) Aliased Binary Point Register */
    __I uint32_t AIAR;                /*!< \brief  Offset: 0x020 (R/ ) Aliased Interrupt Acknowledge Register */
    __O uint32_t AEOIR;               /*!< \brief  Offset: 0x024 ( /W) Aliased End Of Interrupt Register */
    __I uint32_t AHPPIR;              /*!< \brief  Offset: 0x028 (R/ ) Aliased Highest Priority Pending Interrupt Register */
    __IO uint32_t STATUSR;            /*!< \brief  Offset: 0x02C (R/W) Error Reporting Status Register, optional */
    RESERVED(1[40], uint32_t)
    __IO uint32_t APR[4];             /*!< \brief  Offset: 0x0D0 (R/W) Active Priority Register */
    __IO uint32_t NSAPR[4];           /*!< \brief  Offset: 0x0E0 (R/W) Non-secure Active Priority Register */
    RESERVED(2[3], uint32_t)
    __I uint32_t IIDR;                /*!< \brief  Offset: 0x0FC (R/ ) CPU Interface Identification Register */
    RESERVED(3[960], uint32_t)
    __O uint32_t DIR;                 /*!< \brief  Offset: 0x1000( /W) Deactivate Interrupt Register */
};
#else
struct GIC_REDISTRIBUTOR_REG {
    __IO uint32_t CTLR;            /* brief  Offset: 0x0 (R/W) Redistributor Control Register */
    __IO uint32_t IIDR;            /* brief  Offset: 0x4 (RO) Implementer Identification Register */
    __IO uint32_t TYPER;           /* brief  Offset: 0x8 (RO) Redistributor Type Register */
    __IO uint32_t STATUSR;         /* brief  Offset: 0x10 (R/W) Error Reporting Status Register */
    __IO uint32_t WAKER;           /* brief  Offset: 0x14 (R/W) Redistributor Wake Register */
};

struct GIC_REDISTRIBUTOR_SGI_REG {
    RESERVED(0[32], uint32_t)
    __IO uint32_t IGROUPR0;        /* brief  Offset: 0x080 (R/W) Interrupt Group Registers */
    RESERVED(1[31], uint32_t)
    __IO uint32_t ISENABLER0;      /* brief  Offset: 0x100 (R/W) Interrupt Set-Enable Registers */
    RESERVED(2[31], uint32_t)
    __IO uint32_t ICENABLER0;      /* brief  Offset: 0x180 (R/W) Interrupt Clear-Enable Registers */
    RESERVED(3[31], uint32_t)
    __IO uint32_t ISPENDR0;        /* brief  Offset: 0x200 (R/W) Interrupt Set-Pending Registers */
    RESERVED(4[31], uint32_t)
    __IO uint32_t ICPENDR0;        /* brief  Offset: 0x280 (R/W) Interrupt Clear-Pending Registers */
    RESERVED(5[31], uint32_t)
    __IO uint32_t ISACTIVER0;      /* brief  Offset: 0x300 (R/W) Interrupt Set-Active Registers */
    RESERVED(6[31], uint32_t)
    __IO uint32_t ICACTIVER0;      /* brief  Offset: 0x380 (R/W) Interrupt Clear-Active Registers */
    RESERVED(7[31], uint32_t)
    __IO uint32_t IPRIORITYR[256]; /* brief  Offset: 0x400 (R/W) Interrupt Priority Registers */
    RESERVED(8[256], uint32_t)
    __IO uint32_t ICFGR0;          /* brief  Offset: 0xC00 (R/W) Interrupt Configuration Registers */
    __IO uint32_t ICFGR1;          /* brief  Offset: 0xC04 (R/W) Interrupt Configuration Registers */
    RESERVED(9[62], uint32_t)
    __IO uint32_t IGRPMODR0;       /* brief  Offset: 0xD00 (R/W) Interrupt Group Modifier Registers */
    RESERVED(10[63], uint32_t)
    __IO uint32_t NSACR[64];       /* brief  Offset: 0xE00 (R/W) Non-secure Access Control Registers */
};
#endif

struct GIC_IRQ_VALID_INF {
    uint32_t prio;
    uint32_t aff;
    int flag;
};

struct GIC_IRQ_VALID_CTRL {
    uint32_t curCPUId;
    uint32_t curAff;
    struct GIC_IRQ_VALID_INF checkConfig[NUM_INTERRUPTS];
    uint32_t maskBits[PLATFORM_CORE_COUNT][GIC_VLD_MSK_GRP_NUM];
    uint32_t defRouteCpuBits[GIC_VLD_MSK_GRP_NUM];
};

/********************* Private Variable Definition ***************************/
static struct GIC_IRQ_AMP_CTRL *p_ampCtrl;
#ifdef HAL_GIC_V2
static struct GIC_IRQ_VALID_CTRL ampValid;
#endif
static struct GIC_DISTRIBUTOR_REG *pGICD = (struct GIC_DISTRIBUTOR_REG *)GIC_DISTRIBUTOR_BASE;
#ifdef HAL_GIC_V2
static struct GIC_CPU_INTERFACE_REG *pGICC = (struct GIC_CPU_INTERFACE_REG *)GIC_CPU_INTERFACE_BASE;
#else
static struct GIC_REDISTRIBUTOR_REG *pGICR;
static struct GIC_REDISTRIBUTOR_SGI_REG *pGICRSGI;
#endif
/********************* Private Function Definition ***************************/
#ifndef HAL_GIC_V2
static inline uint32_t GIC_GetIccCtlr(void)
{
    uint32_t val;

    __get_CP(15, 0, val, 12, 12, 4);

    return val;
}

static inline void GIC_SetIccCtlr(uint32_t val)
{
    __set_CP(15, 0, val, 12, 12, 4);
}

static inline void GIC_SetIccDir_EL1(uint32_t val)
{
    __set_CP(15, 0, val, 12, 11, 1);
}

static inline uint32_t GIC_GetIccIGrpen1_EL1(void)
{
    uint32_t val;

    __get_CP(15, 0, val, 12, 12, 7);

    return val;
}

static void GIC_SetIccIGrpen1_EL1(uint32_t val)
{
    __set_CP(15, 0, val, 12, 12, 7);
}

static uint32_t GIC_GetIccIar1_EL1(void)
{
    uint32_t val;

    __get_CP(15, 0, val, 12, 12, 0);

    return val;
}

static void GIC_SetIccEoir1_EL1(uint32_t val)
{
    __set_CP(15, 0, val, 12, 12, 1);
}

static inline uint32_t GIC_GetIccPmr_EL1(void)
{
    uint32_t val;

    __get_CP(15, 0, val, 4, 6, 0);

    return val;
}

static inline void GIC_SetIccPmr_EL1(uint32_t val)
{
    __set_CP(15, 0, val, 4, 6, 0);
}

static inline uint32_t GIC_GetIccbpr1(void)
{
    uint32_t val;

    __get_CP(15, 0, val, 12, 12, 3);

    return val;
}
static inline void GIC_SetIccbpr1(uint32_t val)
{
    __set_CP(15, 0, val, 12, 12, 3);
}

static inline uint32_t GIC_GetIccHppir1_EL1(void)
{
    uint32_t val;

    __get_CP(15, 0, val, 12, 12, 2);

    return val;
}

static inline void GIC_SetIccSgi1r(uint64_t val)
{
    __set_CP64(15, 0, val, 12);
}
#endif

static inline void GIC_EnableIRQ(uint32_t irq)
{
    HAL_ASSERT(irq < NUM_INTERRUPTS);
#ifdef HAL_GIC_V2
    pGICD->ISENABLER[irq / 32U] = 1U << (irq % 32U);
#else
    if (irq > 31) {
        pGICD->ISENABLER[irq / 32U] = 1U << (irq % 32U);
    } else {
        pGICRSGI->ISENABLER0 = 1U << (irq % 32U);
    }
#endif
}

static inline void GIC_DisableIRQ(uint32_t irq)
{
    HAL_ASSERT(irq < NUM_INTERRUPTS);
#ifdef HAL_GIC_V2
    pGICD->ICENABLER[irq / 32U] = 1U << (irq % 32U);
#else
    if (irq > 31) {
        pGICD->ICENABLER[irq / 32] = 1 << (irq % 32);
    } else {
        pGICRSGI->ICENABLER0 = 1 << (irq % 32);
    }
#endif
}

static inline uint32_t GIC_GetEnableState(uint32_t irq)
{
    HAL_ASSERT(irq < NUM_INTERRUPTS);
#ifdef HAL_GIC_V2

    return (pGICD->ISENABLER[irq / 32U] >> (irq % 32U)) & 1UL;
#else
    if (irq > 31) {
        return (pGICD->ISENABLER[irq / 32] >> (irq % 32)) & 1;
    } else {
        return (pGICRSGI->ISENABLER0 >> (irq % 32)) & 1;
    }
#endif
}

static inline uint32_t GIC_GetCtlr(void)
{
#ifdef HAL_GIC_V2

    return pGICC->CTLR;
#else

    return GIC_GetIccCtlr();
#endif
}

static inline void GIC_SetCtlr(uint32_t val)
{
#ifdef HAL_GIC_V2
    pGICC->CTLR = val;
#else
    GIC_SetIccCtlr(val);
#endif
}

static inline uint32_t GIC_GetActiveIRQ(void)
{
    uint32_t irq;

#ifdef HAL_GIC_V2
    irq = pGICC->IAR;
#else
    irq = GIC_GetIccIar1_EL1();
#endif
    __DSB();

    return irq;
}

static inline void GIC_EndOfInterrupt(uint32_t irq)
{
    irq &= 0x3FFU;
    HAL_ASSERT(irq < NUM_INTERRUPTS);
#ifdef HAL_GIC_V2
    pGICC->EOIR = irq;
#else
    GIC_SetIccEoir1_EL1(irq);
#endif
}

static inline void GIC_SetPending(uint32_t irq)
{
    HAL_ASSERT(irq < NUM_INTERRUPTS);
#ifdef HAL_GIC_V2
    if (irq >= 16U) {
        pGICD->ISPENDR[irq / 32U] = 1U << (irq % 32U);
    } else {
        pGICD->SGIR = (irq | 0x02000000U);
    }
#else
    if (irq > 31U) {
        pGICD->ISPENDR[irq / 32U] = 1u << (irq % 32U);
    } else {
        pGICRSGI->ISPENDR0 = 1U << (irq % 32U);
    }
#endif
}

static inline uint32_t GIC_GetPending(uint32_t irq)
{
    uint32_t pending;

    if (irq >= NUM_INTERRUPTS) {
        return HAL_INVAL;
    }
#ifdef HAL_GIC_V2
    if (irq >= 16U) {
        pending = (pGICD->ISPENDR[irq / 32U] >> (irq % 32U)) & 1UL;
    } else {
        pending = (pGICD->SPENDSGIR[irq / 4U] >> ((irq % 4U) * 8U)) & 0xFFUL;
        if (pending != 0U) {
            pending = 1U;
        } else {
            pending = 0U;
        }
    }
#else
    if (irq > 31) {
        pending = (pGICD->ISPENDR[irq / 32U] >> (irq % 32U)) & 1UL;
    } else {
        pending = (pGICRSGI->ISPENDR0 >> (irq % 32U)) & 1UL;
    }
#endif

    return pending;
}

static inline void GIC_ClearPending(uint32_t irq)
{
    HAL_ASSERT(irq < NUM_INTERRUPTS);
#ifdef HAL_GIC_V2
    if (irq >= 16U) {
        pGICD->ICPENDR[irq / 32U] = 1U << (irq % 32U);
    } else {
        pGICD->CPENDSGIR[irq / 4U] = 1U << ((irq % 4U) * 8U);
    }
#else
    if (irq > 31U) {
        pGICD->ICPENDR[irq / 32U] = 1U << (irq % 32U);
    } else {
        pGICRSGI->ICPENDR0 = 1U << (irq % 32U);
    }
#endif
}

#ifdef HAL_GIC_V2
static void GIC_SendSgi(IRQn_Type irq, uint32_t targetList, uint32_t routMode)
{
    HAL_ASSERT(irq < 16U);
    pGICD->SGIR = ((routMode & 3U) << 24U) |
                  ((targetList & 0xFFUL) << 16U) | (irq & 0x0FUL);
}
#else
static uint16_t GIC_GetTargetList(int *baseCpu, uint32_t mask, unsigned long clusterId)
{
    int nextCpu, cpu = *baseCpu;
    unsigned long mpidr = HAL_CPU_TOPOLOGY_GetCpuAffByCpuId(cpu);
    uint16_t tList = 0;

    tList |= 1 << (mpidr & 0xf);

    for (nextCpu = cpu + 1; nextCpu < PLATFORM_CORE_COUNT; nextCpu++) {
        mpidr = HAL_CPU_TOPOLOGY_GetCpuAffByCpuId(nextCpu);

        if (clusterId != MPIDR_TO_SGI_CLUSTER_ID(mpidr)) {
            break;
        }

        if (!(mask & HAL_BIT(nextCpu))) {
            continue;
        }

        tList |= 1 << (mpidr & 0xf);
    }

    *baseCpu = nextCpu;

    return tList;
}

static void GIC_SetSgi(uint64_t clusterId, uint16_t tList, unsigned int irq, uint32_t routMode)
{
    uint64_t val;

    if (routMode) {
        val = (0x1ULL << ICC_SGI1R_IRQ_ROUTING_MODE_BIT |
               irq << ICC_SGI1R_SGI_ID_SHIFT);
    } else {
        val = (MPIDR_TO_SGI_AFFINITY(clusterId, 3) |
               MPIDR_TO_SGI_AFFINITY(clusterId, 2) |
               irq << ICC_SGI1R_SGI_ID_SHIFT |
               MPIDR_TO_SGI_AFFINITY(clusterId, 1) |
               MPIDR_TO_SGI_RS(clusterId) |
               tList << ICC_SGI1R_TARGET_LIST_SHIFT);
    }

    GIC_SetIccSgi1r(val);
}

static void GIC_SendSgi(IRQn_Type irq, uint32_t targetList, uint32_t routMode)
{
    int i = 0;
    uint16_t tList;
    unsigned long mpidr;
    uint64_t clusterId;

    if (irq > 15U) {
        return;
    }

    if (routMode) {
        GIC_SetSgi(0, 0, irq, routMode);

        return;
    }

    while (i < PLATFORM_CORE_COUNT) {
        if (targetList & HAL_BIT(i)) {
            mpidr = HAL_CPU_TOPOLOGY_GetCpuAffByCpuId(i);
            clusterId = MPIDR_TO_SGI_CLUSTER_ID(mpidr);
            tList = GIC_GetTargetList(&i, targetList, clusterId);
            GIC_SetSgi(clusterId, tList, irq, routMode);
        } else {
            i++;
        }
    }
    __ISB();
}
#endif

static inline uint32_t GIC_GetIRQStatus(uint32_t irq)
{
    uint32_t pending, active;

    if (irq >= NUM_INTERRUPTS) {
        return HAL_INVAL;
    }
#ifdef HAL_GIC_V2
    pending = (pGICD->ISPENDR[irq / 32U] >> (irq % 32U)) & 1UL;
    active = (pGICD->ISACTIVER[irq / 32U] >> (irq % 32U)) & 1UL;
#else
    if (irq > 31U) {
        pending = (pGICD->ISPENDR[irq / 32U] >> (irq % 32U)) & 1UL;
        active = (pGICD->ISACTIVER[irq / 32U] >> (irq % 32U)) & 1UL;
    } else {
        pending = (pGICRSGI->ISPENDR0 >> (irq % 32U)) & 1UL;
        active = (pGICRSGI->ISACTIVER0 >> (irq % 32U)) & 1UL;
    }
#endif

    return ((active << 1U) | pending);
}

static inline void GIC_SetPriority(uint32_t irq, uint32_t priority)
{
    HAL_ASSERT(irq < NUM_INTERRUPTS);
#ifdef HAL_GIC_V2
    uint32_t mask = pGICD->IPRIORITYR[irq / 4U] & ~(0xFFUL << ((irq % 4U) * 8U));
    pGICD->IPRIORITYR[irq / 4U] = mask | ((priority & 0xFFUL) << ((irq % 4U) * 8U));
#else
    if (irq > 31) {
        uint32_t mask = pGICD->IPRIORITYR[irq / 4U] & ~(0xFFUL << ((irq % 4U) * 8U));
        pGICD->IPRIORITYR[irq / 4U] = mask | ((priority & 0xFFUL) << ((irq % 4U) * 8U));
    } else {
        uint32_t mask = pGICRSGI->IPRIORITYR[irq / 4U] & ~(0xFFUL << ((irq % 4U) * 8U));
        pGICRSGI->IPRIORITYR[irq / 4U] = mask | ((priority & 0xFFUL) << ((irq % 4U) * 8U));
    }
#endif
}

static inline uint32_t GIC_GetPriority(uint32_t irq)
{
    HAL_ASSERT(irq < NUM_INTERRUPTS);

#ifdef HAL_GIC_V2

    return (pGICD->IPRIORITYR[irq / 4U] >> ((irq % 4U) * 8U)) & 0xFFUL;
#else
    if (irq > 31) {
        return (pGICD->IPRIORITYR[irq / 4U] >> ((irq % 4U) * 8U)) & 0xFFUL;
    } else {
        return (pGICRSGI->IPRIORITYR[irq / 4U] >> ((irq % 4U) * 8U)) & 0xFFUL;
    }
#endif
}

static inline void GIC_SetPriorityMask(uint32_t priority)
{
    priority = priority & 0xFF;

#ifdef HAL_GIC_V2
    pGICC->PMR = priority;
#else
    GIC_SetIccPmr_EL1(priority);
#endif
}

static inline uint32_t GIC_GetPriorityMask(void)
{
#ifdef HAL_GIC_V2

    return pGICC->PMR;
#else

    return GIC_GetIccPmr_EL1();
#endif
}

static inline void GIC_SetDir(uint32_t irq)
{
#ifdef HAL_GIC_V2
    pGICC->DIR = irq;
#else
    GIC_SetIccDir_EL1(irq);
#endif
}

static inline void GIC_SetIRouter(uint32_t irq, uint32_t aff)
{
#ifdef HAL_GIC_V2
    uint32_t cpu_target, mask;
    uint32_t cpu = HAL_CPU_TOPOLOGY_GetCpuIdByMpidr(aff);

    HAL_ASSERT(irq < NUM_INTERRUPTS);
    HAL_ASSERT(cpu < PLATFORM_CORE_COUNT);

    if (irq > 31) {
        cpu_target = HAL_BIT(cpu);
        mask = pGICD->ITARGETSR[irq / 4U] & ~(0xFFUL << ((irq % 4U) * 8U));
        cpu_target = (cpu_target & 0xFFUL) << ((irq % 4U) * 8U);
        pGICD->ITARGETSR[irq / 4U] = mask | cpu_target;
    }
#else
    if (irq > 31) {
        pGICD->IROUTER[irq - 32U] = aff;
    }
#endif
}

static inline void GIC_SetConfiguration(uint32_t irq, uint32_t int_config)
{
    uint32_t icfgr = pGICD->ICFGR[irq / 16U];
    uint32_t shift = (irq % 16U) << 1U;

    icfgr &= (~(3U << shift));
    icfgr |= (int_config << shift);

#ifdef HAL_GIC_V2
    pGICD->ICFGR[irq / 16U] = icfgr;
#else
    if (irq > 31) {
        pGICD->ICFGR[irq / 16] = icfgr;
    } else if (irq > 15) {
        pGICRSGI->ICFGR1 = icfgr;
    } else {
        pGICRSGI->ICFGR0 = icfgr;
    }
#endif
}

static inline uint32_t GIC_GetConfiguration(uint32_t irq)
{
    uint32_t icfgr;

#ifdef HAL_GIC_V2
    icfgr = pGICD->ICFGR[irq / 16U];
#else
    if (irq > 31) {
        icfgr = pGICD->ICFGR[irq / 16U];
    } else if (irq > 15) {
        icfgr = pGICRSGI->ICFGR1;
    } else {
        icfgr = pGICRSGI->ICFGR0;
    }
#endif

    return icfgr >> ((irq % 16U) << 1U);
}

static inline void GIC_SetBinaryPoint(uint32_t binary_point)
{
#ifdef HAL_GIC_V2
    pGICC->ABPR = binary_point & 7U;
#else
    GIC_SetIccbpr1(binary_point);
#endif
}

static inline uint32_t GIC_GetBinaryPoint(void)
{
#ifdef HAL_GIC_V2

    return pGICC->BPR;
#else

    return GIC_GetIccbpr1();
#endif
}

static inline void GIC_EnableInterface(void)
{
#ifdef HAL_GIC_V2
    pGICC->CTLR |= 1U;
#else
    GIC_SetIccIGrpen1_EL1(1);
#endif
}

static inline void GIC_DisableInterface(void)
{
#ifdef HAL_GIC_V2
    pGICC->CTLR &= ~1U;
#else
    GIC_SetIccIGrpen1_EL1(0);
#endif
}

static inline void GIC_EnableDistributor(void)
{
    pGICD->CTLR |= 3;
}

static inline void GIC_DisableDistributor(void)
{
    pGICD->CTLR &= ~3;
}

static inline uint32_t GIC_DistributorInfo(void)
{
    return pGICD->TYPER;
}

static bool GIC_AmpCheckIrqValid(uint32_t irq)
{
#ifdef HAL_GIC_V2
    if (irq < 32) {
        return true;
    }

    if (ampValid.checkConfig[irq].aff == ampValid.curAff && ampValid.checkConfig[irq].flag) {
        return true;
    } else {
        return false;
    }
#else

    return true;
#endif
}

#ifdef HAL_GIC_V2
static bool GIC_AMPCheckIRoute(uint32_t irq, uint32_t aff)
{
    uint32_t cpu_target, mask;
    uint32_t cpu = HAL_CPU_TOPOLOGY_GetCpuIdByMpidr(aff);

    HAL_ASSERT(irq < NUM_INTERRUPTS);
    HAL_ASSERT(cpu < PLATFORM_CORE_COUNT);

    if (irq > 31) {
        cpu_target = HAL_BIT(cpu);
        mask = pGICD->ITARGETSR[irq / 4U] & (0xFFUL << ((irq % 4U) * 8U));
        cpu_target = (cpu_target & 0xFFUL) << ((irq % 4U) * 8U);

        GIC_DBG(" GIC_AMPCheckIRoute:irq-%ld(%lx) %lx-%lx\n",
                irq, (irq % 4U), mask, cpu_target);

        if (!(mask & cpu_target)) {
            GIC_DBG(" GIC_AMPCheckIRoute error,irq-%ld(%lx) %lx %lx\n",
                    irq, (irq % 4U), mask, cpu_target);

            return false;
        }

        mask = mask & ~cpu_target;
        if (mask) {
            GIC_DBG(" AMPCheckIRoute error: mult cpus = %lx\n", mask);

            return false;
        }
    }

    return true;
}

static void GIC_AmpCheckIrqInit(void)
{
    int loops, i;

    for (i = 32; i < NUM_INTERRUPTS; i++) {
        if (ampValid.checkConfig[i].flag && ampValid.checkConfig[i].aff == ampValid.curAff) {
            GIC_DBG("GIC_AmpCheckIrqInit: check irq-%d\n", i);
            loops = AMP_WAIT_INIT_LOOPS;
            while (GIC_AMPCheckIRoute(i, ampValid.curAff) == false &&
                   loops) {
                HAL_DelayUs(AMP_WAIT_INIT_ONCE_US);
                loops--;
            }

            if (loops == 0) {
                GIC_DBG(" GIC_AmpCheckIrqInit:irq-%d route error\n", i);
            }

            loops = AMP_WAIT_INIT_LOOPS;
            while (GIC_GetPriority(i) != ampValid.checkConfig[i].prio && loops) {
                HAL_DelayUs(AMP_WAIT_INIT_ONCE_US);
                loops--;
            }
            GIC_DBG(" AmpCheckIrqInit:irq-%d prio(%lx, %lx)\n",
                    i, GIC_GetPriority(i), ampValid.checkConfig[i].prio);

            if (loops == 0) {
                GIC_DBG(" AmpCheckIrqInit: irq-%d prio error\n", i);
            }
        }
    }
}

static bool GIC_AMPCheckPrio(uint32_t irq)
{
    if (irq < 32) {
        return true;
    }

    if (GIC_GetPriority(irq) == ampValid.checkConfig[irq].prio) {
        return true;
    } else {
        return false;
    }
}

static bool GIC_AMP_CheckCurIRoute(uint32_t irq)
{
    return GIC_AMPCheckIRoute(irq, ampValid.curAff);
}

static void GIC_AMPGetValidConfig(struct GIC_IRQ_AMP_CTRL *ampCtrl,
                                  struct GIC_IRQ_VALID_CTRL *valid, uint32_t curAff)
{
    struct GIC_AMP_IRQ_INIT_CFG *config;

#ifndef HAL_GIC_WAIT_LINUX_INIT_ENABLED
    int i;
#endif
    if (!ampCtrl) {
        return;
    }

    config = ampCtrl->irqsCfg;

    valid->curAff = curAff;

    while (config->prio && config->irq) {
        valid->checkConfig[config->irq].prio = config->prio;
        valid->checkConfig[config->irq].aff = config->routeAff;
        valid->checkConfig[config->irq].flag = 1;
        config++;
    }

#ifndef HAL_GIC_WAIT_LINUX_INIT_ENABLED
    for (i = 32; i < NUM_INTERRUPTS; i++) {
        if (!valid->checkConfig[i].flag) {
            valid->checkConfig[i].prio = ampCtrl->defPrio;
            valid->checkConfig[i].aff = ampCtrl->defRouteAff;
            valid->checkConfig[i].flag = 1;
        }
    }
#endif
}
#endif

static void GIC_DistInit(uint32_t initGicd, uint32_t amp, uint32_t priority, uint32_t aff)
{
    uint32_t i;
    uint32_t numIrq = 0U;
    uint32_t priorityField;

    numIrq = 32 * ((GIC_DistributorInfo() & 0x1F) + 1);

    if (!initGicd) {
        while (!(pGICD->CTLR & 0x3)) {
            ;
        }

#ifdef HAL_GIC_V2
        GIC_AmpCheckIrqInit();
#else
#ifdef HAL_GIC_WAIT_LINUX_INIT_ENABLED
        while (pGICD->IPRIORITYR[(numIrq - 1) / 4] != 0xa0a0a0a0) {
            ;
        }
#endif
#endif

        return;
    }

    GIC_DisableDistributor();

    if (!amp) {
        HAL_GIC_SetPriority(0, 0xFF);
        priorityField = GIC_GetPriority(0);
        priorityField /= 2;
        aff = 0;
    } else {
        priorityField = priority;
    }

    for (i = 32; i < numIrq; i++) {
        GIC_DisableIRQ(i);
        GIC_SetConfiguration(i, 0);
        GIC_SetPriority(i, priorityField);
        GIC_SetIRouter(i, aff);
    }

    GIC_EnableDistributor();
}

static void GIC_CPUInterfaceInit(uint32_t amp, uint32_t priority)
{
    uint32_t i, ctlr;
    uint32_t priorityField;

    GIC_DisableInterface();

    if (!amp) {
        HAL_GIC_SetPriority(0, 0xFF);
        priorityField = GIC_GetPriority(0);
        priorityField /= 2;
    } else {
        priorityField = priority;
    }

    for (i = 0U; i < 32; i++) {
        if (i > 15) {
            GIC_SetConfiguration(i, 0);
        }
        GIC_DisableIRQ(i);
        GIC_SetPriority(i, priorityField);
    }

    ctlr = GIC_GetCtlr();
    ctlr &= ~0x2;
    GIC_SetCtlr(ctlr);
    GIC_SetBinaryPoint(0);
    GIC_SetPriorityMask(0xFF);
    GIC_EnableInterface();
}

#ifndef HAL_GIC_V2
static inline void GIC_RedistInitBase(uint32_t cpu)
{
    pGICR = (struct GIC_REDISTRIBUTOR_REG *)GICR_CPU_BASE(cpu);
    pGICRSGI = (struct GIC_REDISTRIBUTOR_SGI_REG *)GICR_SGI_CPU_BASE(cpu);
}

static void GIC_EnableRedist(uint32_t enable)
{
    if (enable) {
        pGICR->WAKER &= ~GICR_WAKER_PROCESSOR_SLEEP;
        while ((pGICR->WAKER & GICR_WAKER_CHILDREN_ASLEEP) != 0) {
            ;
        }
    } else {
        pGICR->WAKER |= GICR_WAKER_PROCESSOR_SLEEP;
        while ((pGICR->WAKER & GICR_WAKER_CHILDREN_ASLEEP) == 0) {
            ;
        }
    }
}
#endif

static void GIC_Enable(uint32_t initGicd, uint32_t amp, uint32_t priority, uint32_t aff)
{
    GIC_DistInit(initGicd, amp, priority, aff);
#ifndef HAL_GIC_V2
    GIC_EnableRedist(1);
#endif
    GIC_CPUInterfaceInit(amp, priority);
}

#ifdef HAL_GIC_V2
static void GIC_AMPConfigIRQs(struct GIC_AMP_IRQ_INIT_CFG *irqsCfg)
{
    int i;

#ifdef HAL_GIC_WAIT_LINUX_INIT_ENABLED

    return;
#endif

    for (i = 32; i < NUM_INTERRUPTS; i++) {
        if (ampValid.checkConfig[i].flag) {
            GIC_SetPriority(i, ampValid.checkConfig[i].prio);
            GIC_SetIRouter(i, ampValid.checkConfig[i].aff);
            GIC_DBG("GIC_AMPConfigIRQs-%d: prio:%lx-%lx, aff:%lx-%d\n", i,
                    ampValid.checkConfig[i].prio, GIC_GetPriority(i),
                    ampValid.checkConfig[i].aff,
                    GIC_AMPCheckIRoute(i, ampValid.checkConfig[i].aff) ? 1 : 0
                    );
        }
    }
}

#else
static void GIC_AMPConfigIRQs(struct GIC_AMP_IRQ_INIT_CFG *irqsCfg)
{
    struct GIC_AMP_IRQ_INIT_CFG *config;

    if (!p_ampCtrl) {
        return;
    }

    config = irqsCfg;
    if (!config) {
        return;
    }

    while (config->prio && config->irq) {
        if ((uint32_t)config->irq < NUM_INTERRUPTS) {
            GIC_SetPriority(config->irq, config->prio);
        }
        config++;
    }
    ;
}
#endif

/** @} */

/********************* Public Function Definition ****************************/

/** @defgroup GIC_Exported_Functions_Group5 Other Functions
 *  @{
 */

/**
 * @brief  Enable a IRQ.
 * @param  irq: irq id.
 * @return HAL_Status.
 */
HAL_Status HAL_GIC_Enable(uint32_t irq)
{
    HAL_ASSERT(irq < NUM_INTERRUPTS);

    if (!GIC_AmpCheckIrqValid(irq)) {
        GIC_DBG("HAL_GIC_Enable: invalid irq-%ld\n", irq);

        return HAL_INVAL;
    }

#ifdef HAL_GIC_V2
    if (!GIC_AMP_CheckCurIRoute(irq)) {
        GIC_DBG("HAL_GIC_Enable: invalid route-%ld\n", irq);

        return HAL_ERROR;
    }
    if (!GIC_AMPCheckPrio(irq)) {
        GIC_DBG("HAL_GIC_Enable: invalid prio-%ld\n", irq);

        return HAL_ERROR;
    }
#else
    GIC_SetIRouter(irq, GET_CUR_MPIDR_AFF());
#endif

    GIC_EnableIRQ(irq);

    return HAL_OK;
}

/**
 * @brief  Disable a IRQ.
 * @param  irq: irq id.
 * @return HAL_Status.
 */
HAL_Status HAL_GIC_Disable(uint32_t irq)
{
    if (!GIC_AmpCheckIrqValid(irq)) {
        GIC_DBG("HAL_GIC_Disable: invalid prio-%ld\n", irq);

        return HAL_INVAL;
    }

    GIC_DisableIRQ(irq);

    return HAL_OK;
}

/**
 * @brief  Get the enable state of a IRQ.
 * @param  irq: irq id.
 * @return 1 if enable, 0 if disable.
 */

uint32_t HAL_GIC_GetEnableState(uint32_t irq)
{
    return GIC_GetEnableState(irq);
}

/**
 * @brief  Get the irq id of the active IRQ.
 * @return irq id.
 */
uint32_t HAL_GIC_GetActiveIRQ(void)
{
    return GIC_GetActiveIRQ();
}

/**
 * @brief  End the active IRQ.
 * @param  irq: irq id.
 * @return HAL_Status.
 */
HAL_Status HAL_GIC_EndOfInterrupt(uint32_t irq)
{
    GIC_EndOfInterrupt(irq);

    return HAL_OK;
}

/**
 * @brief  Make a IRQ PENDING.
 * @param  irq id.
 * @return HAL_Status.
 */
HAL_Status HAL_GIC_SetPending(uint32_t irq)
{
    GIC_SetPending(irq);

    return HAL_OK;
}

/**
 * @brief  Get the Pending state of a IRQ.
 * @param  irq: irq id.
 * @return 1 if pending, 0 if not pending.
 */
int32_t HAL_GIC_GetPending(uint32_t irq)
{
    return GIC_GetPending(irq);
}

/**
 * @brief  Clear the Pending state of a IRQ.
 * @param  irq: irq id.
 * @return HAL_Status.
 */
HAL_Status HAL_GIC_ClearPending(uint32_t irq)
{
    GIC_ClearPending(irq);

    return HAL_OK;
}

/**
 * @brief  Send a sgi.
 * @param  irq: irq id of a sgi.
 * @param  targetList: The set of CPUs for which SGI interrupts
                       will be generated. Each bit corresponds to a cpu.
                       for example,0x6 corresponds to cpu 1 and cpu2.
 * @param  routMode: 0 sgi routed to the Cpus specified by targetList.
                     1 sgi routed to all CPUs in the system, excluding "self".
 * @return HAL_Status.
 */
HAL_Status HAL_GIC_SendSGI(IRQn_Type irq,
                           uint32_t targetList,
                           uint32_t routMode)
{
    GIC_SendSgi(irq, targetList, routMode);

    return HAL_OK;
}

/**
 * @brief  Get the status for a given interrupt.
 * @param  irq: The interrupt to get status for.
 * @return 0 - not pending/active, 1 - pending, 2 - active, 3 - pending and active
 */
uint32_t HAL_GIC_GetIRQStatus(uint32_t irq)
{
    return GIC_GetIRQStatus(irq);
}

/**
 * @brief  Set priority for a IRQ.
 * @param  irq: irq id.
 * @param  priority: priority value.
 * @return HAL_Status.
 */
HAL_Status HAL_GIC_SetPriority(uint32_t irq, uint32_t priority)
{
    uint32_t aff;

    aff = GET_CUR_MPIDR_AFF();
    if (p_ampCtrl && p_ampCtrl->cpuAff != aff && irq > 31) {
        return HAL_INVAL;
    }

#ifdef HAL_GIC_V2
    if (GIC_GetPriority(irq) != priority) {
        GIC_DBG("HAL_GIC_SetPriority: invalid prio-%ld\n", irq);

        return HAL_INVAL;
    } else {
        return HAL_OK;
    }
#endif

    GIC_SetPriority(irq, priority);

    return HAL_OK;
}

/**
 * @brief  Set the priority mask.
 * @param  priority: priority value.
 * @return HAL_Status.
 */
HAL_Status HAL_GIC_SetPriorityMask(uint32_t priority)
{
    GIC_SetPriorityMask(priority);

    return HAL_OK;
}

/**
 * @brief  Get the priority mask.
 * @return priority value.
 */
uint32_t HAL_GIC_GetPriorityMask(void)
{
    return GIC_GetPriorityMask();
}

/**
 * @brief  Get the priority value of a IRQ.
 * @param  irq: irq id.
 * @return priority value.
 */
uint32_t HAL_GIC_GetPriority(uint32_t irq)
{
    return GIC_GetPriority(irq);
}

/**
 * @brief  Set the routting affinity value of a IRQ.
 * @param  irq: irq id.
 * @param  aff: the routting affinity value.
 * @return HAL_Status.
 */
HAL_Status HAL_GIC_SetIRouter(uint32_t irq, uint32_t aff)
{
#ifdef HAL_GIC_V2
    if (GIC_AMPCheckIRoute(irq, aff)) {
        return HAL_INVAL;
    } else {
        return HAL_OK;
    }
#endif

    GIC_SetIRouter(irq, aff);

    return HAL_OK;
}

/**
 * @brief  deactivate a IRQ.
 * @param  irq: irq id.
 * @return HAL_Status.
 */
HAL_Status HAL_GIC_SetDir(uint32_t irq)
{
    if (!GIC_AmpCheckIrqValid(irq)) {
        GIC_DBG("HAL_GIC_SetDir: invalid prio-%ld\n", irq);

        return HAL_INVAL;
    }

    GIC_SetDir(irq);

    return HAL_OK;
}

/** @} */

/** @defgroup GIC_Exported_Functions_Group4 Init and DeInit Functions

 This section provides functions allowing to init and deinit the module:

 *  @{
 */

/**
 * @brief Initialize GIC base address and config GIC.
 * @param ampCtrl: amp system config information.
 * @return HAL_Status.
 */
HAL_Status HAL_GIC_Init(struct GIC_IRQ_AMP_CTRL *ampCtrl)
{
    uint32_t aff, prio;

#ifndef HAL_GIC_V2
    GIC_RedistInitBase(HAL_CPU_TOPOLOGY_GetCurrentCpuId());
#endif

    p_ampCtrl = ampCtrl;

    if (!p_ampCtrl) {
        GIC_Enable(1, 0, 0, 0);

        return HAL_OK;
    }

    aff = GET_CUR_MPIDR_AFF();
#ifdef HAL_GIC_V2
    GIC_AMPGetValidConfig(p_ampCtrl, &ampValid, aff);
#endif
    prio = p_ampCtrl->defPrio;

    if (p_ampCtrl->cpuAff == aff) {
        aff = p_ampCtrl->defRouteAff;
        GIC_Enable(1, 1, prio, aff);
        GIC_AMPConfigIRQs(p_ampCtrl->irqsCfg);
    } else {
        GIC_Enable(0, 1, prio, 0);
    }

    return HAL_OK;
}

/** @} */

/** @} */

/** @} */
#endif
