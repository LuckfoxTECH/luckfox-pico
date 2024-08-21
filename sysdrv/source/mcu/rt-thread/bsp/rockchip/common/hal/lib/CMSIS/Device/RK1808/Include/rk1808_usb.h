/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2021 Rockchip Electronics Co., Ltd.
 */

#ifndef __RK1808_USB_H
#define __RK1808_USB_H
#ifdef __cplusplus
  extern "C" {
#endif
/****************************************************************************************/
/*                                                                                      */
/*                               Module Structure Section                               */
/*                                                                                      */
/****************************************************************************************/
#ifndef __ASSEMBLY__
/* EHCI Register Structure Define */
struct EHCI_REG {
    __I  uint32_t HCCAPBASE;           /* Address Offset: 0x0000 EHCI Version Number Register                   */
    __I  uint32_t HCSPARAMS;           /* Address Offset: 0x0004 EHCI Structural Parameters Register            */
    __I  uint32_t HCCPARAMS;           /* Address Offset: 0x0008 EHCI Capability Parameters Register            */
    __I  uint32_t RESERVED0[1];        /* Address Offset: 0x000C                                                */
    __IO uint32_t USBCMD;              /* Address Offset: 0x0010 USB Command Register                           */
    __IO uint32_t USBSTS;              /* Address Offset: 0x0014 USB Status Register                            */
    __IO uint32_t USBINTR;             /* Address Offset: 0x0018 USB Interrupt Enable Register                  */
    __IO uint32_t FRINDEX;             /* Address Offset: 0x001C USB Frame Index Register                       */
    __I  uint32_t RESERVED1[1];        /* Address Offset: 0x0020                                                */
    __IO uint32_t PERIODICLISTBASE;    /* Address Offset: 0x0024 USB Periodic Frame List Base Address Register  */
    __IO uint32_t ASYNCLISTADDR;       /* Address Offset: 0x0028 USB Current Asynchronous List Address Register */
    __I  uint32_t RESERVED2[9];        /* Address Offset: 0x002C                                                */
    __IO uint32_t CONFIGFLAG;          /* Address Offset: 0x0050 USB Configure Flag Register                    */
    __IO uint32_t PORTSC[1];           /* Address Offset: 0x0054 USB Port Status and Control Register           */
};

/* OHCI Register Structure Define */
struct OHCI_REG {
    __I  uint32_t REVISION;            /* Address Offset: 0x0000 Host Controller Revision Register              */
    __IO uint32_t CONTROL;             /* Address Offset: 0x0004 Host Controller Control Register               */
    __IO uint32_t CMDSTATUS;           /* Address Offset: 0x0008 Host Controller Command Status Register        */
    __IO uint32_t INTRSTATUS;          /* Address Offset: 0x000C Host Controller Interrupt Status Register      */
    __IO uint32_t INTRENABLE;          /* Address Offset: 0x0010 Host Controller Interrupt Enable Register      */
    __IO uint32_t INTRDISABLE;         /* Address Offset: 0x0014 Host Controller Interrupt Disable Register     */
    __IO uint32_t HCCA;                /* Address Offset: 0x0018 Host Controller Communication Area Register    */
    __IO uint32_t ED_PERIODCURRENT;    /* Address Offset: 0x001C Host Controller Period Current ED Register     */
    __IO uint32_t ED_CONTROLHEAD;      /* Address Offset: 0x0020 Host Controller Control Head ED Register       */
    __IO uint32_t ED_CONTROLCURRENT;   /* Address Offset: 0x0024 Host Controller Control Current ED Register    */
    __IO uint32_t ED_BULKHEAD;         /* Address Offset: 0x0028 Host Controller Bulk Head ED Register          */
    __IO uint32_t ED_BULKCURRENT;      /* Address Offset: 0x002C Host Controller Bulk Current ED Register       */
    __IO uint32_t DONEHEAD;            /* Address Offset: 0x0030 Host Controller Done Head Register             */
    __IO uint32_t FMINTERVAL;          /* Address Offset: 0x0034 Host Controller Frame Interval Register        */
    __I  uint32_t FMREMAINING;         /* Address Offset: 0x0038 Host Controller Frame Remaining Register       */
    __I  uint32_t FMNUMBER;            /* Address Offset: 0x003C Host Controller Frame Number Register          */
    __IO uint32_t PERIODICSTART;       /* Address Offset: 0x0040 Host Controller Periodic Start Register        */
    __IO uint32_t LSTHRESH;            /* Address Offset: 0x0044 Host Controller Low-speed Threshold Register   */
    __IO uint32_t RH_DESCRIPTORA;      /* Address Offset: 0x0048 Host Controller Root Hub Descriptor A Register */
    __IO uint32_t RH_DESCRIPTORB;      /* Address Offset: 0x004C Host Controller Root Hub Descriptor B Register */
    __IO uint32_t RH_STATUS;           /* Address Offset: 0x0050 Host Controller Root Hub Status Register       */
    __IO uint32_t RH_PORTSTATUS[1];    /* Address Offset: 0x0054 Host Controller Root Hub Port Status [1]       */
};
#endif /* __ASSEMBLY__ */
/****************************************************************************************/
/*                                                                                      */
/*                               Module Variable Section                                */
/*                                                                                      */
/****************************************************************************************/
/* Module Variable Define */
#define EHCI                 ((struct EHCI_REG *) EHCI_BASE)
#define OHCI                 ((struct OHCI_REG *) OHCI_BASE)

#define IS_EHCI_INSTANCE(instance) ((instance) == EHCI)
#define IS_OHCI_INSTANCE(instance) ((instance) == OHCI)
/****************************************************************************************/
/*                                                                                      */
/*                               Register Bitmap Section                                */
/*                                                                                      */
/****************************************************************************************/
/**************************  Bit definition for EHCI register  **************************/
#define EHCI_HCCAPBASE_CRLEN_SHIFT             (0U)
#define EHCI_HCCAPBASE_CRLEN_MASK              (0xffU << EHCI_HCCAPBASE_CRLEN_SHIFT)
#define EHCI_HCCAPBASE_VERSION_SHIFT           (16U)
#define EHCI_HCCAPBASE_VERSION_MASK            (0xffffU << EHCI_HCCAPBASE_VERSION_SHIFT)

#define EHCI_HCSPARAMS_N_PORTS_SHIFT           (0U)
#define EHCI_HCSPARAMS_N_PORTS_MASK            (0xfU << EHCI_HCSPARAMS_N_PORTS_SHIFT)
#define EHCI_HCSPARAMS_PPC_SHIFT               (4U)
#define EHCI_HCSPARAMS_PPC_MASK                (0x1U << EHCI_HCSPARAMS_PPC_SHIFT)
#define EHCI_HCSPARAMS_N_PCC_SHIFT             (8U)
#define EHCI_HCSPARAMS_N_PCC_MASK              (0xfU << EHCI_HCSPARAMS_N_PCC_SHIFT)
#define EHCI_HCSPARAMS_N_CC_SHIFT              (12)
#define EHCI_HCSPARAMS_N_CC_MASK               (0xfU << EHCI_HCSPARAMS_N_CC_SHIFT)

#define EHCI_HCCPARAMS_AC64_SHIFT              (0U)
#define EHCI_HCCPARAMS_AC64_MASK               (0x1U << EHCI_HCCPARAMS_AC64_SHIFT)
#define EHCI_HCCPARAMS_PFLF_SHIFT              (1U)
#define EHCI_HCCPARAMS_PFLF_MASK               (0x1U << EHCI_HCCPARAMS_PFLF_SHIFT)
#define EHCI_HCCPARAMS_ASPC_SHIFT              (2U)
#define EHCI_HCCPARAMS_ASPC_MASK               (0x1U << EHCI_HCCPARAMS_ASPC_SHIFT)
#define EHCI_HCCPARAMS_IST_SHIFT               (4U)
#define EHCI_HCCPARAMS_IST_MASK                (0xfU << EHCI_HCCPARAMS_IST_SHIFT)
#define EHCI_HCCPARAMS_EECP_SHIFT              (8U)
#define EHCI_HCCPARAMS_EECP_MASK               (0xffU << EHCI_HCCPARAMS_EECP_SHIFT)

#define EHCI_USBCMD_RUN_SHIFT                  (0U)
#define EHCI_USBCMD_RUN_MASK                   (0x1U << EHCI_USBCMD_RUN_SHIFT)
#define EHCI_USBCMD_HCRST_SHIFT                (1U)
#define EHCI_USBCMD_HCRST_MASK                 (0x1U << EHCI_USBCMD_HCRST_SHIFT)
#define EHCI_USBCMD_FLSZ_SHIFT                 (2U)
#define EHCI_USBCMD_FLSZ_MASK                  (0x3U << EHCI_USBCMD_FLSZ_SHIFT)
#define EHCI_USBCMD_PSEN_SHIFT                 (4U)
#define EHCI_USBCMD_PSEN_MASK                  (0x1U << EHCI_USBCMD_PSEN_SHIFT)
#define EHCI_USBCMD_ASEN_SHIFT                 (5U)
#define EHCI_USBCMD_ASEN_MASK                  (0x1U << EHCI_USBCMD_ASEN_SHIFT)
#define EHCI_USBCMD_IAAD_SHIFT                 (6U)
#define EHCI_USBCMD_IAAD_MASK                  (0x1U << EHCI_USBCMD_IAAD_SHIFT)
#define EHCI_USBCMD_LRESET_SHIFT               (7U)
#define EHCI_USBCMD_LRESET_MASK                (0x1U << EHCI_USBCMD_LRESET_SHIFT)
#define EHCI_USBCMD_ITC_SHIFT                  (16U)
#define EHCI_USBCMD_ITC_MASK                   (0xffU << EHCI_USBCMD_ITC_SHIFT)

#define EHCI_USBSTS_USBINT_SHIFT               (0U)
#define EHCI_USBSTS_USBINT_MASK                (0x1U << EHCI_USBSTS_USBINT_SHIFT)
#define EHCI_USBSTS_UERRINT_SHIFT              (1U)
#define EHCI_USBSTS_UERRINT_MASK               (0x1U << EHCI_USBSTS_UERRINT_SHIFT)
#define EHCI_USBSTS_PCD_SHIFT                  (2U)
#define EHCI_USBSTS_PCD_MASK                   (0x1U << EHCI_USBSTS_PCD_SHIFT)
#define EHCI_USBSTS_FLR_SHIFT                  (3U)
#define EHCI_USBSTS_FLR_MASK                   (0x1U << EHCI_USBSTS_FLR_SHIFT)
#define EHCI_USBSTS_HSERR_SHIFT                (4U)
#define EHCI_USBSTS_HSERR_MASK                 (0x1U << EHCI_USBSTS_HSERR_SHIFT)
#define EHCI_USBSTS_IAA_SHIFT                  (5U)
#define EHCI_USBSTS_IAA_MASK                   (0x1U << EHCI_USBSTS_IAA_SHIFT)
#define EHCI_USBSTS_HCHalted_SHIFT             (12U)
#define EHCI_USBSTS_HCHalted_MASK              (0x1U << EHCI_USBSTS_HCHalted_SHIFT)
#define EHCI_USBSTS_RECLA_SHIFT                (13U)
#define EHCI_USBSTS_RECLA_MASK                 (0x1U << EHCI_USBSTS_RECLA_SHIFT)
#define EHCI_USBSTS_PSS_SHIFT                  (14U)
#define EHCI_USBSTS_PSS_MASK                   (0x1U << EHCI_USBSTS_PSS_SHIFT)
#define EHCI_USBSTS_ASS_SHIFT                  (15U)
#define EHCI_USBSTS_ASS_MASK                   (0x1U << EHCI_USBSTS_ASS_SHIFT)

#define EHCI_USBINTR_USBIEN_SHIFT              (0U)
#define EHCI_USBINTR_USBIEN_MASK               (0x1U << EHCI_USBINTR_USBIEN_SHIFT)
#define EHCI_USBINTR_UERRIEN_SHIFT             (1U)
#define EHCI_USBINTR_UERRIEN_MASK              (0x1U << EHCI_USBINTR_UERRIEN_SHIFT)
#define EHCI_USBINTR_PCIEN_SHIFT               (2U)
#define EHCI_USBINTR_PCIEN_MASK                (0x1U << EHCI_USBINTR_PCIEN_SHIFT)
#define EHCI_USBINTR_FLREN_SHIFT               (3U)
#define EHCI_USBINTR_FLREN_MASK                (0x1U << EHCI_USBINTR_FLREN_SHIFT)
#define EHCI_USBINTR_HSERREN_SHIFT             (4U)
#define EHCI_USBINTR_HSERREN_MASK              (0x1U << EHCI_USBINTR_HSERREN_SHIFT)
#define EHCI_USBINTR_IAAEN_SHIFT               (5U)
#define EHCI_USBINTR_IAAEN_MASK                (0x1U << EHCI_USBINTR_IAAEN_SHIFT)

#define EHCI_FRINDEX_FI_SHIFT                  (0U)
#define EHCI_FRINDEX_FI_MASK                   (0x3fffU << EHCI_FRINDEX_FI_SHIFT)

#define EHCI_PERIODICLISTBASE_BADDR_SHIFT      (12U)
#define EHCI_PERIODICLISTBASE_BADDR_MASK       (0xfffffU << EHCI_PERIODICLISTBASE_BADDR_SHIFT)

#define EHCI_ASYNCLISTADDR_LPL_SHIFT           (5U)
#define EHCI_ASYNCLISTADDR_LPL_MASK            (0x7ffffffU << EHCI_ASYNCLISTADDR_LPL_SHIFT)

#define EHCI_CONFIGFLAG_CF_SHIFT               (0U)
#define EHCI_CONFIGFLAG_CF_MASK                (0x1U << EHCI_CONFIGFLAG_CF_SHIFT)

#define EHCI_PORTSC_CCS_SHIFT                  (0U)
#define EHCI_PORTSC_CCS_MASK                   (0x1U << EHCI_PORTSC_CCS_SHIFT)
#define EHCI_PORTSC_CSC_SHIFT                  (1U)
#define EHCI_PORTSC_CSC_MASK                   (0x1U << EHCI_PORTSC_CSC_SHIFT)
#define EHCI_PORTSC_PE_SHIFT                   (2U)
#define EHCI_PORTSC_PE_MASK                    (0x1U << EHCI_PORTSC_PE_SHIFT)
#define EHCI_PORTSC_PEC_SHIFT                  (3U)
#define EHCI_PORTSC_PEC_MASK                   (0x1U << EHCI_PORTSC_PEC_SHIFT)
#define EHCI_PORTSC_OCA_SHIFT                  (4U)
#define EHCI_PORTSC_OCA_MASK                   (0x1U << EHCI_PORTSC_OCA_SHIFT)
#define EHCI_PORTSC_OCC_SHIFT                  (5U)
#define EHCI_PORTSC_OCC_MASK                   (0x1U << EHCI_PORTSC_OCC_SHIFT)
#define EHCI_PORTSC_FPR_SHIFT                  (6U)
#define EHCI_PORTSC_FPR_MASK                   (0x1U << EHCI_PORTSC_FPR_SHIFT)
#define EHCI_PORTSC_SUSPEND_SHIFT              (7U)
#define EHCI_PORTSC_SUSPEND_MASK               (0x1U << EHCI_PORTSC_SUSPEND_SHIFT)
#define EHCI_PORTSC_PRST_SHIFT                 (8U)
#define EHCI_PORTSC_PRST_MASK                  (0x1U << EHCI_PORTSC_PRST_SHIFT)
#define EHCI_PORTSC_LSTS_SHIFT                 (10)
#define EHCI_PORTSC_LSTS_MASK                  (0x3U << EHCI_PORTSC_LSTS_SHIFT)
#define EHCI_PORTSC_PP_SHIFT                   (12U)
#define EHCI_PORTSC_PP_MASK                    (0x1U << EHCI_PORTSC_PP_SHIFT)
#define EHCI_PORTSC_PO_SHIFT                   (13U)
#define EHCI_PORTSC_PO_MASK                    (0x1U << EHCI_PORTSC_PO_SHIFT)
#define EHCI_PORTSC_PTC_SHIFT                  (16U)
#define EHCI_PORTSC_PTC_MASK                   (0xfU << EHCI_PORTSC_PTC_SHIFT)

#define EHCI_USBPCR0_SUSPEND_SHIFT             (8U)
#define EHCI_USBPCR0_SUSPEND_MASK              (0x1U << EHCI_USBPCR0_SUSPEND_SHIFT)
#define EHCI_USBPCR0_CLKVALID_SHIFT            (11U)
#define EHCI_USBPCR0_CLKVALID_MASK             (0x1U << EHCI_USBPCR0_CLKVALID_SHIFT)

#define EHCI_USBPCR1_SUSPEND_SHIFT             (8U)
#define EHCI_USBPCR1_SUSPEND_MASK              (0x1U << EHCI_USBPCR1_SUSPEND_SHIFT)

/**************************  Bit definition for OHCI register  **************************/
#define OHCI_REVISION_REV_SHIFT                (0U)
#define OHCI_REVISION_REV_MASK                 (0xffU << OHCI_REVISION_REV_SHIFT)

#define OHCI_CONTROL_CBSR_SHIFT                (0U)
#define OHCI_CONTROL_CBSR_MASK                 (0x3U << OHCI_CONTROL_CBSR_SHIFT)
#define OHCI_CONTROL_PLE_SHIFT                 (2U)
#define OHCI_CONTROL_PLE_MASK                  (0x1U << OHCI_CONTROL_PLE_SHIFT)
#define OHCI_CONTROL_IE_SHIFT                  (3U)
#define OHCI_CONTROL_IE_MASK                   (0x1U << OHCI_CONTROL_IE_SHIFT)
#define OHCI_CONTROL_CLE_SHIFT                 (4U)
#define OHCI_CONTROL_CLE_MASK                  (0x1U << OHCI_CONTROL_CLE_SHIFT)
#define OHCI_CONTROL_BLE_SHIFT                 (5U)
#define OHCI_CONTROL_BLE_MASK                  (0x1U << OHCI_CONTROL_BLE_SHIFT)
#define OHCI_CONTROL_HCFS_SHIFT                (6U)
#define OHCI_CONTROL_HCFS_MASK                 (0x3U << OHCI_CONTROL_HCFS_SHIFT)

#define OHCI_CMDSTATUS_HCR_SHIFT               (0U)
#define OHCI_CMDSTATUS_HCR_MASK                (0x1U << OHCI_CMDSTATUS_HCR_SHIFT)
#define OHCI_CMDSTATUS_CLF_SHIFT               (1U)
#define OHCI_CMDSTATUS_CLF_MASK                (0x1U << OHCI_CMDSTATUS_CLF_SHIFT)
#define OHCI_CMDSTATUS_BLF_SHIFT               (2U)
#define OHCI_CMDSTATUS_BLF_MASK                (0x1U << OHCI_CMDSTATUS_BLF_SHIFT)
#define OHCI_CMDSTATUS_SOC_SHIFT               (16U)
#define OHCI_CMDSTATUS_SOC_MASK                (0x3U << OHCI_CMDSTATUS_SOC_SHIFT)

#define OHCI_INTRSTATUS_SO_SHIFT               (0U)
#define OHCI_INTRSTATUS_SO_MASK                (0x1U << OHCI_INTRSTATUS_SO_SHIFT)
#define OHCI_INTRSTATUS_WDH_SHIFT              (1U)
#define OHCI_INTRSTATUS_WDH_MASK               (0x1U << OHCI_INTRSTATUS_WDH_SHIFT)
#define OHCI_INTRSTATUS_SF_SHIFT               (2U)
#define OHCI_INTRSTATUS_SF_MASK                (0x1U << OHCI_INTRSTATUS_SF_SHIFT)
#define OHCI_INTRSTATUS_RD_SHIFT               (3U)
#define OHCI_INTRSTATUS_RD_MASK                (0x1U << OHCI_INTRSTATUS_RD_SHIFT)
#define OHCI_INTRSTATUS_FNO_SHIFT              (5U)
#define OHCI_INTRSTATUS_FNO_MASK               (0x1U << OHCI_INTRSTATUS_FNO_SHIFT)
#define OHCI_INTRSTATUS_RHSC_SHIFT             (6U)
#define OHCI_INTRSTATUS_RHSC_MASK              (0x1U << OHCI_INTRSTATUS_RHSC_SHIFT)

#define OHCI_INTRENABLE_SO_SHIFT               (0U)
#define OHCI_INTRENABLE_SO_MASK                (0x1U << OHCI_INTRENABLE_SO_SHIFT)
#define OHCI_INTRENABLE_WDH_SHIFT              (1U)
#define OHCI_INTRENABLE_WDH_MASK               (0x1U << OHCI_INTRENABLE_WDH_SHIFT)
#define OHCI_INTRENABLE_SF_SHIFT               (2U)
#define OHCI_INTRENABLE_SF_MASK                (0x1U << OHCI_INTRENABLE_SF_SHIFT)
#define OHCI_INTRENABLE_RD_SHIFT               (3U)
#define OHCI_INTRENABLE_RD_MASK                (0x1U << OHCI_INTRENABLE_RD_SHIFT)
#define OHCI_INTRENABLE_FNO_SHIFT              (5U)
#define OHCI_INTRENABLE_FNO_MASK               (0x1U << OHCI_INTRENABLE_FNO_SHIFT)
#define OHCI_INTRENABLE_RHSC_SHIFT             (6U)
#define OHCI_INTRENABLE_RHSC_MASK              (0x1U << OHCI_INTRENABLE_RHSC_SHIFT)
#define OHCI_INTRENABLE_MIE_SHIFT              (31U)
#define OHCI_INTRENABLE_MIE_MASK               (0x1U << OHCI_INTRENABLE_MIE_SHIFT)

#define OHCI_INTRDISABLE_SO_SHIFT              (0U)
#define OHCI_INTRDISABLE_SO_MASK               (0x1U << OHCI_INTRDISABLE_SO_SHIFT)
#define OHCI_INTRDISABLE_WDH_SHIFT             (1U)
#define OHCI_INTRDISABLE_WDH_MASK              (0x1U << OHCI_INTRDISABLE_WDH_SHIFT)
#define OHCI_INTRDISABLE_SF_SHIFT              (2U)
#define OHCI_INTRDISABLE_SF_MASK               (0x1U << OHCI_INTRDISABLE_SF_SHIFT)
#define OHCI_INTRDISABLE_RD_SHIFT              (3U)
#define OHCI_INTRDISABLE_RD_MASK               (0x1U << OHCI_INTRDISABLE_RD_SHIFT)
#define OHCI_INTRDISABLE_FNO_SHIFT             (5U)
#define OHCI_INTRDISABLE_FNO_MASK              (0x1U << OHCI_INTRDISABLE_FNO_SHIFT)
#define OHCI_INTRDISABLE_RHSC_SHIFT            (6U)
#define OHCI_INTRDISABLE_RHSC_MASK             (0x1U << OHCI_INTRDISABLE_RHSC_SHIFT)
#define OHCI_INTRDISABLE_MIE_SHIFT             (31U)
#define OHCI_INTRDISABLE_MIE_MASK              (0x1U << OHCI_INTRDISABLE_MIE_SHIFT)

#define OHCI_HCCA_HCCA_SHIFT                   (8U)
#define OHCI_HCCA_HCCA_MASK                    (0xffffffU << OHCI_HCCA_HCCA_SHIFT)

#define OHCI_ED_PERIODCURRENT_PCED_SHIFT       (4U)
#define OHCI_ED_PERIODCURRENT_PCED_MASK        (0xfffffffU << OHCI_ED_PERIODCURRENT_PCED_SHIFT)

#define OHCI_ED_CONTROLHEAD_CHED_SHIFT         (4U)
#define OHCI_ED_CONTROLHEAD_CHED_MASK          (0xfffffffU << OHCI_ED_CONTROLHEAD_CHED_SHIFT)

#define OHCI_ED_CONTROLCURRENT_CCED_SHIFT      (4U)
#define OHCI_ED_CONTROLCURRENT_CCED_MASK       (0xfffffffU << OHCI_ED_CONTROLCURRENT_CCED_SHIFT)

#define OHCI_ED_BULKHEAD_BHED_SHIFT            (4U)
#define OHCI_ED_BULKHEAD_BHED_MASK             (0xfffffffU << OHCI_ED_BULKHEAD_BHED_SHIFT)

#define OHCI_ED_BULKCURRENT_BCED_SHIFT         (4U)
#define OHCI_ED_BULKCURRENT_BCED_MASK          (0xfffffffU << OHCI_ED_BULKCURRENT_BCED_SHIFT)

#define OHCI_DONEHEAD_DH_SHIFT                 (4U)
#define OHCI_DONEHEAD_DH_MASK                  (0xfffffffU << OHCI_DONEHEAD_DH_SHIFT)

#define OHCI_FMINTERVAL_FI_SHIFT               (0U)
#define OHCI_FMINTERVAL_FI_MASK                (0x3fffU << OHCI_FMINTERVAL_FI_SHIFT)
#define OHCI_FMINTERVAL_FSMPS_SHIFT            (16U)
#define OHCI_FMINTERVAL_FSMPS_MASK             (0x7fffU << OHCI_FMINTERVAL_FSMPS_SHIFT)
#define OHCI_FMINTERVAL_FIT_SHIFT              (31U)
#define OHCI_FMINTERVAL_FIT_MASK               (0x1U << OHCI_FMINTERVAL_FIT_SHIFT)

#define OHCI_FMREMAINING_FR_SHIFT              (0U)
#define OHCI_FMREMAINING_FR_MASK               (0x3fffU << OHCI_FMREMAINING_FR_SHIFT)
#define OHCI_FMREMAINING_FRT_SHIFT             (31U)
#define OHCI_FMREMAINING_FRT_MASK              (0x1U << OHCI_FMREMAINING_FRT_SHIFT)

#define OHCI_FMNUMBER_FN_SHIFT                 (0U)
#define OHCI_FMNUMBER_FN_MASK                  (0xffffU << OHCI_FMNUMBER_FN_SHIFT)

#define OHCI_PERIODICSTART_PS_SHIFT            (0U)
#define OHCI_PERIODICSTART_PS_MASK             (0x3fffU << OHCI_PERIODICSTART_PS_SHIFT)

#define OHCI_LSTHRESH_LST_SHIFT                (0U)
#define OHCI_LSTHRESH_LST_MASK                 (0xfffU << OHCI_LSTHRESH_LST_SHIFT)

#define OHCI_RH_DESCRIPTORA_NDP_SHIFT          (0U)
#define OHCI_RH_DESCRIPTORA_NDP_MASK           (0xffU << OHCI_RH_DESCRIPTORA_NDP_SHIFT)
#define OHCI_RH_DESCRIPTORA_PSM_SHIFT          (8U)
#define OHCI_RH_DESCRIPTORA_PSM_MASK           (0x1U << OHCI_RH_DESCRIPTORA_PSM_SHIFT)
#define OHCI_RH_DESCRIPTORA_OCPM_SHIFT         (11U)
#define OHCI_RH_DESCRIPTORA_OCPM_MASK          (0x1U << OHCI_RH_DESCRIPTORA_OCPM_SHIFT)
#define OHCI_RH_DESCRIPTORA_NOCP_SHIFT         (12)
#define OHCI_RH_DESCRIPTORA_NOCP_MASK          (0x1U << OHCI_RH_DESCRIPTORA_NOCP_SHIFT)
#define OHCI_RH_DESCRIPTORB_PPCM_SHIFT         (16U)
#define OHCI_RH_DESCRIPTORB_PPCM_MASK          (0xffffU << OHCI_RH_DESCRIPTORB_PPCM_SHIFT)

#define OHCI_RH_STATUS_LPS_SHIFT               (0U)
#define OHCI_RH_STATUS_LPS_MASK                (0x1U << OHCI_RH_STATUS_LPS_SHIFT)
#define OHCI_RH_STATUS_OCI_SHIFT               (1U)
#define OHCI_RH_STATUS_OCI_MASK                (0x1U << OHCI_RH_STATUS_OCI_SHIFT)
#define OHCI_RH_STATUS_DRWE_SHIFT              (15U)
#define OHCI_RH_STATUS_DRWE_MASK               (0x1U << OHCI_RH_STATUS_DRWE_SHIFT)
#define OHCI_RH_STATUS_LPSC_SHIFT              (16U)
#define OHCI_RH_STATUS_LPSC_MASK               (0x1U << OHCI_RH_STATUS_LPSC_SHIFT)
#define OHCI_RH_STATUS_OCIC_SHIFT              (17U)
#define OHCI_RH_STATUS_OCIC_MASK               (0x1U << OHCI_RH_STATUS_OCIC_SHIFT)
#define OHCI_RH_STATUS_CRWE_SHIFT              (31U)
#define OHCI_RH_STATUS_CRWE_MASK               (0x1U << OHCI_RH_STATUS_CRWE_SHIFT)

#define OHCI_RH_PORTSTATUS_CCS_SHIFT           (0U)
#define OHCI_RH_PORTSTATUS_CCS_MASK            (0x1U << OHCI_RH_PORTSTATUS_CCS_SHIFT)
#define OHCI_RH_PORTSTATUS_PES_SHIFT           (1U)
#define OHCI_RH_PORTSTATUS_PES_MASK            (0x1U << OHCI_RH_PORTSTATUS_PES_SHIFT)
#define OHCI_RH_PORTSTATUS_PSS_SHIFT           (2U)
#define OHCI_RH_PORTSTATUS_PSS_MASK            (0x1U << OHCI_RH_PORTSTATUS_PSS_SHIFT)
#define OHCI_RH_PORTSTATUS_POCI_SHIFT          (3U)
#define OHCI_RH_PORTSTATUS_POCI_MASK           (0x1U << OHCI_RH_PORTSTATUS_POCI_SHIFT)
#define OHCI_RH_PORTSTATUS_PRS_SHIFT           (4U)
#define OHCI_RH_PORTSTATUS_PRS_MASK            (0x1U << OHCI_RH_PORTSTATUS_PRS_SHIFT)
#define OHCI_RH_PORTSTATUS_PPS_SHIFT           (8U)
#define OHCI_RH_PORTSTATUS_PPS_MASK            (0x1U << OHCI_RH_PORTSTATUS_PPS_SHIFT)
#define OHCI_RH_PORTSTATUS_LSDA_SHIFT          (9U)
#define OHCI_RH_PORTSTATUS_LSDA_MASK           (0x1U << OHCI_RH_PORTSTATUS_LSDA_SHIFT)
#define OHCI_RH_PORTSTATUS_CSC_SHIFT           (16U)
#define OHCI_RH_PORTSTATUS_CSC_MASK            (0x1U << OHCI_RH_PORTSTATUS_CSC_SHIFT)
#define OHCI_RH_PORTSTATUS_PESC_SHIFT          (17U)
#define OHCI_RH_PORTSTATUS_PESC_MASK           (0x1U << OHCI_RH_PORTSTATUS_PESC_SHIFT)
#define OHCI_RH_PORTSTATUS_PSSC_SHIFT          (18U)
#define OHCI_RH_PORTSTATUS_PSSC_MASK           (0x1U << OHCI_RH_PORTSTATUS_PSSC_SHIFT)
#define OHCI_RH_PORTSTATUS_OCIC_SHIFT          (19U)
#define OHCI_RH_PORTSTATUS_OCIC_MASK           (0x1U << OHCI_RH_PORTSTATUS_OCIC_SHIFT)
#define OHCI_RH_PORTSTATUS_PRSC_SHIFT          (20U)
#define OHCI_RH_PORTSTATUS_PRSC_MASK           (0x1U << OHCI_RH_PORTSTATUS_PRSC_SHIFT)

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __RK1808_USB_H */
