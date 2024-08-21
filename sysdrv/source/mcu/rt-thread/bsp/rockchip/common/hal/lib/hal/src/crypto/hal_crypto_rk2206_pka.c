/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#if defined(RKMCU_RK2206) && defined(HAL_CRYPTO_MODULE_ENABLED)

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup CRYPTO
 *  @{
 */

/** @addtogroup CRYPTO_Private_Definition Private Definition
 *  @{
 */

/********************* Private MACRO Definition ******************************/
/* RAM_CTL */
#define CRYPTO_RAM_PKA_RDY HAL_BIT(CRYPTO_RAM_CTL_RAM_PKA_RDY_SHIFT)

/* RAM_ST */
#define CRYPTO_CLK_RAM_RDY HAL_BIT(CRYPTO_RAM_ST_CLK_RAM_RDY_SHIFT)

/* DEBUG_CTL */
#define CRYPTO_DEBUG_MODE HAL_BIT(CRYPTO_DEBUG_CTL_PKA_DEBUG_MODE_SHIFT)

/* DEBUG_ST */
#define CRYPTO_PKA_DEBUG_CLK_EN HAL_BIT(CRYPTO_DEBUG_ST_PKA_DEBUG_CLK_EN_SHIFT)

/* PKA OPCODE register fields positions (low bit position) */
#define CRYPTO_PKA_OPCODE_R_DISCARD_SHIFT       11
#define CRYPTO_PKA_OPCODE_OPERAND_2_IMMED_SHIFT 17
#define CRYPTO_PKA_OPCODE_OPERAND_1_IMMED_SHIFT 23

#define PKA_MAX_REGS_COUNT          8
#define PKA_MAX_PHYS_MEM_REGS_COUNT 32
#define PKA_MAX_REGS_MEM_SIZE_BYTES 4096

#define RK_WORD_SIZE 32

/* PKA N_NP_T0_T1 register default (reset) value: N=0, NP=1, T0=30, T1=31 */
#define PKA_N  0UL
#define PKA_NP 1UL
#define PKA_T0 30UL
#define PKA_T1 31UL
#define RK_PKA_N_NP_T0_T1_REG_DEFAULT_VAL                        \
                (PKA_N  << CRYPTO_N_NP_TO_T1_ADDR_REG_N_SHIFT  | \
                 PKA_NP << CRYPTO_N_NP_TO_T1_ADDR_REG_NP_SHIFT | \
                 PKA_T0 << CRYPTO_N_NP_TO_T1_ADDR_REG_T0_SHIFT | \
                 PKA_T1 << CRYPTO_N_NP_TO_T1_ADDR_REG_T1_SHIFT)

#define RES_DISCARD 0x3F

/* Machine Opcodes definitions (according to HW CRS ) */

enum PKA_OPCODE {
    PKA_OPCODE_ADD = 0x04,
    PKA_OPCODE_SUB,
    PKA_OPCODE_MOD_ADD,
    PKA_OPCODE_MOD_SUB,
    PKA_OPCODE_AND,
    PKA_OPCODE_OR,
    PKA_OPCODE_XOR,
    PKA_OPCODE_SHR0 = 0x0C,
    PKA_OPCODE_SHR1,
    PKA_OPCODE_SHL0,
    PKA_OPCODE_SHL1,
    PKA_OPCODE_LMUL,
    PKA_OPCODE_MOD_MUL,
    PKA_OPCODE_MOD_MUL_NR,
    PKA_OPCODE_MOD_EXP,
    PKA_OPCODE_DIV,
    PKA_OPCODE_MOD_INV,
    PKA_OPCODE_MOD_DIV,
    PKA_OPCODE_HMUL,
    PKA_OPCODE_TERMINATE,
};

#ifndef min
#define min(x, y) ((x) > (y) ? (y) : (x))
#endif

#define PKA_CLK_ENABLE()
#define PKA_CLK_DISABLE()

/*************************************************************************
 * Macros for calling PKA operations (names according to operation issue *
 *************************************************************************/

/*--------------------------------------*/
/*   1.  ADD - SUBTRACT operations  */
/*--------------------------------------*/
/*  Add:   res =  opA + opB */
#define   RK_PKA_Add(lenId, opA, opB, res, tag)                 \
            PKA_ExecOpration(PKA_OPCODE_ADD, (lenId), 0, (opA), \
                             0, (opB), 0, (res), (tag))

/*  AddIm:  res =  opA + opB_im */
#define   RK_PKA_AddIm(lenId, opA, opB_im, res, tag)            \
            PKA_ExecOpration(PKA_OPCODE_ADD, (lenId), 0, (opA), \
                             1, (opB_im), 0, (res), (tag))

/*  Sub:  res =  opA - opB  */
#define   RK_PKA_Sub(lenId, opA, opB, res, tag)                 \
            PKA_ExecOpration(PKA_OPCODE_SUB, (lenId), 0, (opA), \
                             0, (opB), 0, (res), (tag))

/*  SubIm:  res =  opA - opB_im */
#define   RK_PKA_SubIm(lenId, opA, opB_im, res, tag)            \
            PKA_ExecOpration(PKA_OPCODE_SUB, (lenId), 0, (opA), \
                             1, (opB_im), 0, (res), (tag))

/*  Neg:  res =  0 - opB  */
#define   RK_PKA_Neg(lenId, opB, res, tag)                  \
            PKA_ExecOpration(PKA_OPCODE_SUB, (lenId), 1, 0, \
                             0, (opB), 0, (res), (tag))

/*  ModAdd:  res =  (opA + opB) mod N  */
#define   RK_PKA_ModAdd(lenId, opA, opB, res, tag)                  \
            PKA_ExecOpration(PKA_OPCODE_MOD_ADD, (lenId), 0, (opA), \
                             0, (opB), 0, (res), (tag))

/*  ModAddIm:  res =  (opA + opB_im) mod N  */
#define   RK_PKA_ModAddIm(lenId, opA, opB_im, res, tag)             \
            PKA_ExecOpration(PKA_OPCODE_MOD_ADD, (lenId), 0, (opA), \
                             1, (opB_im), 0, (res), (tag))

/*  ModSub:  res =  (opA - opB) mod N  */
#define   RK_PKA_ModSub(lenId, opA, opB, res, tag)                  \
            PKA_ExecOpration(PKA_OPCODE_MOD_SUB, (lenId), 0, (opA), \
                             0, (opB), 0, (res), (tag))

/*  ModSubIm:  res =  (opA - opB_im) mod N  */
#define   RK_PKA_ModSubIm(lenId, opA, opB_im, res, tag)             \
            PKA_ExecOpration(PKA_OPCODE_MOD_SUB, (lenId), 0, (opA), \
                             1, (opB_im), 0, (res), (tag))

/*  ModNeg:  res =  (0 - opB) mod N  */
#define   RK_PKA_ModNeg(lenId, opB, res, tag)                   \
            PKA_ExecOpration(PKA_OPCODE_MOD_SUB, (lenId), 1, 0, \
                             0, (opB), 0, (res), (tag))

/*--------------------------------------*/
/*   2.  Logical   operations   */
/*--------------------------------------*/

/*  AND:  res =  opA & opB  */
#define   RK_PKA_AND(lenId, opA, opB, res, tag)                 \
            PKA_ExecOpration(PKA_OPCODE_AND, (lenId), 0, (opA), \
                             0, (opB), 0, (res), (tag))

/*  AndIm:  res =  opA & opB  */
#define   RK_PKA_AndIm(lenId, opA, opB, res, tag)               \
            PKA_ExecOpration(PKA_OPCODE_AND, (lenId), 0, (opA), \
                             1, (opB), 0, (res), (tag))

/*  Tst0:  opA & 0x1 - tests the bit 0 of operand A. */
/*  If bit0 = 0, then ZeroOfStatus = 1, else 0  */
#define   RK_PKA_Tst0(lenId, opA, tag)                          \
            PKA_ExecOpration(PKA_OPCODE_AND, (lenId), 0, (opA), \
                             1, 0x01, 1, RES_DISCARD, (tag))

/*  Clr0:  res =  opA & (-2)  - clears the bit 0 of operand A. */
/*  Note:  -2 = 0x1E  for 5-bit size */
#define   RK_PKA_Clr0(lenId, opA, res, tag)                     \
            PKA_ExecOpration(PKA_OPCODE_AND, (lenId), 0, (opA), \
                             1, 0x1E, 0, (res), (tag))

/*  Clr:  res =  opA & 0  - clears the operand A.  */
#define   RK_PKA_Clr(lenId, opA, tag)                           \
            PKA_ExecOpration(PKA_OPCODE_AND, (lenId), 0, (opA), \
                             1, 0x00, 0, (opA), (tag))

/*  Clear:  for full clearing the actual register opA,
 *  this macro calls Clr operation twice.
 */
#define   RK_PKA_Clear(lenId, opA, tag) \
               RK_PKA_Clr(lenId, opA, tag)

/*  OR:  res =  opA || opB  */
#define   RK_PKA_OR(lenId, opA, opB, res, tag)                 \
            PKA_ExecOpration(PKA_OPCODE_OR, (lenId), 0, (opA), \
                             0, (opB), 0, (res), (tag))

/*  OrIm:  res =  opA || opB  */
#define   RK_PKA_OrIm(lenId, opA, opB, res, tag)               \
            PKA_ExecOpration(PKA_OPCODE_OR, (lenId), 0, (opA), \
                             1, (opB), 0, (res), (tag))

/*  Copy:  OpDest =  OpSrc || 0  */
#define   RK_PKA_Copy(lenId, op_dest, op_src, tag)                \
            PKA_ExecOpration(PKA_OPCODE_OR, (lenId), 0, (op_src), \
                             1, 0x00, 0, (op_dest), (tag))

/*  Set0:  res =  opA || 1  : set bit0 = 1, other bits are not changed */
#define   RK_PKA_Set0(lenId, opA, res, tag)                    \
            PKA_ExecOpration(PKA_OPCODE_OR, (lenId), 0, (opA), \
                             1, 0x01, 0, (res), (tag))

/*  Xor:  res =  opA ^ opB  */
#define   RK_PKA_Xor(lenId, opA, opB, res, tag)                 \
            PKA_ExecOpration(PKA_OPCODE_XOR, (lenId), 0, (opA), \
                             0, (opB), 0, (res), (tag))

/*  XorIm:  res =  opA ^ opB  */
#define   RK_PKA_XorIm(lenId, opA, opB, res, tag)               \
            PKA_ExecOpration(PKA_OPCODE_XOR, (lenId), 0, (opA), \
                             1, (opB), 0, (res), (tag))

/*  Flip0:  res =  opA || 1  - inverts the bit 0 of operand A  */
#define   RK_PKA_Flip0(lenId, opA, res, tag)                    \
            PKA_ExecOpration(PKA_OPCODE_XOR, (lenId), 0, (opA), \
                             1, 0x01, 0, (res), (tag))

/*  Invert:  res =  opA ^ 0xFFF.FF  :  inverts all bits of opA . */
/* Note: 0xFFFFF =  0x1F for 5 bits size of second operand */
#define   RK_PKA_Invert(lenId, opA, res, tag)                   \
            PKA_ExecOpration(PKA_OPCODE_XOR, (lenId), 0, (opA), \
                             1, 0x1F, 0, (res), (tag))

/*  Compare:  opA ^ opB . Rsult of compare in ZeroBitOfStatus: */
/*  If opA == opB then Z = 1 */
#define   RK_PKA_Compare(lenId, opA, opB, tag)                  \
            PKA_ExecOpration(PKA_OPCODE_XOR, (lenId), 0, (opA), \
                             0, (opB), 1, (0), (tag))

/*  CompareImmediate:  opA ^ opB . Rsult of compare in ZeroBitOfStatus: */
/*  If opA == opB then status Z = 1 */
#define   RK_PKA_CompareIm(lenId, opA, opB, tag)                \
            PKA_ExecOpration(PKA_OPCODE_XOR, (lenId), 0, (opA), \
                             1, (opB), 1, (0), (tag))

/*----------------------------------------------*/
/*   3.  SHIFT    operations        */
/*----------------------------------------------*/

/*  SHR0:  res =  opA >> (S+1) :
 *  shifts right operand A by S+1 bits, insert 0 to left most bits
 */
#define   RK_PKA_SHR0(lenId, opA, S, res, tag)                   \
            PKA_ExecOpration(PKA_OPCODE_SHR0, (lenId), 0, (opA), \
                             0, (S), 0, (res), (tag))

/*  SHR1:  res =  opA >> (S+1) :
 *  shifts right operand A by S+1 bits, insert 1 to left most bits
 */
#define   RK_PKA_SHR1(lenId, opA, S, res, tag)                   \
            PKA_ExecOpration(PKA_OPCODE_SHR1, (lenId), 0, (opA), \
                             0, (S), 0, (res), (tag))

/*  SHL0:  res =  opA << (S+1) :
 *  shifts left operand A by S+1 bits, insert 0 to right most bits
 */
#define   RK_PKA_SHL0(lenId, opA, S, res, tag)                   \
            PKA_ExecOpration(PKA_OPCODE_SHL0, (lenId), 0, (opA), \
                             0, (S), 0, (res), (tag))

/*  SHL1:  res =  opA << (S+1) :
 *  shifts left operand A by S+1 bits, insert 1 to right most bits
 */
#define   RK_PKA_SHL1(lenId, opA, S, res, tag)                   \
            PKA_ExecOpration(PKA_OPCODE_SHL1, (lenId), 0, (opA), \
                             0, (S), 0, (res), (tag))

/*--------------------------------------------------------------*/
/*   2.  Multiplication and other   operations      */
/*       Note:  See notes to RK_PKAExecOperation    */
/*--------------------------------------------------------------*/

/*  RMul:  res =  LowHalfOf(opA * opB), where size of operands and result
 *  is equaled to operation size, defined by lenId. Note: for receiving
 *  full result, the lenId must be set according to (sizeA + sizeB) and
 *  leading not significant bits of operands must be zeroed
 */
#define   RK_PKA_LMul(lenId, opA, opB, res, tag)                 \
            PKA_ExecOpration(PKA_OPCODE_LMUL, (lenId), 0, (opA), \
                             0, (opB), 0, (res), (tag))

/*  HMul:  res =  HighHalfOf(opA * opB) + one high word of low half of
 *  (opA * opB), where size of operands is equaled to operation size,
 *  defined by lenId. Note: Size of operation result is by one word large,
 *  than operation size
 */
#define   RK_PKA_HMul(lenId, opA, opB, res, tag)                 \
            PKA_ExecOpration(PKA_OPCODE_HMUL, (lenId), 0, (opA), \
                             0, (opB), 0, (res), (tag))

/*  ModMul:  res =  opA * opB  mod N - modular multiplication */
#define   RK_PKA_ModMul(lenId, opA, opB, res, tag)                  \
            PKA_ExecOpration(PKA_OPCODE_MOD_MUL, (lenId), 0, (opA), \
                             0, (opB), 0, (res), (tag))

/*  ModMulN:  res =  opA * opB  mod N
 *  - modular multiplication (final reduction is omitted)
 */
#define   RK_PKA_ModMulN(lenId, opA, opB, res, tag)             \
            PKA_ExecOpration(PKA_OPCODE_MOD_MUL_NR, (lenId), 0, \
                             (opA), 0, (opB), 0, (res), (tag))

/*  ModExp:  res =  opA ** opB  mod N - modular exponentiation */
#define   RK_PKA_ModExp(lenId, opA, opB, res, tag)                  \
            PKA_ExecOpration(PKA_OPCODE_MOD_EXP, (lenId), 0, (opA), \
                             0, (opB), 0, (res), (tag))

/*  Divide:  res =  opA / opB , opA = opA mod opB - division,  */
#define   RK_PKA_Div(lenId, opA, opB, res, tag)                 \
            PKA_ExecOpration(PKA_OPCODE_DIV, (lenId), 0, (opA), \
                             0, (opB), 0, (res), (tag))

/*  ModInv:  Modular inversion: calculates   res = 1/opB mod N  */
#define   RK_PKA_ModInv(lenId, opB, res, tag)                   \
            PKA_ExecOpration(PKA_OPCODE_MOD_INV, (lenId), 0, 1, \
                             0, (opB), 0, (res), (tag))
#define   RK_PKA_ModDiv(lenId, opA, opB, res, tag)                  \
            PKA_ExecOpration(PKA_OPCODE_MOD_DIV, (lenId), 0, (opA), \
                             0, (opB), 0, (res), (tag))

/*  Terminate  - special operation, which allows HOST access */
/*  to PKA data memory registers after end of PKA operations */
#define   RK_PKA_Terminate(tag)                                \
            PKA_ExecOpration(PKA_OPCODE_TERMINATE, 0, 0, 0, 0, \
                             0, 0, 0, (tag))

#define RK_PKA_DefaultInitPKA(max_size_bits, regsize_words) \
            PKA_Init(0, 0, 0, 0, (max_size_bits),           \
                    (regsize_words), 0, 1)

#define PKA_MEMSET_ZERO(buf, size)      PKA_WordMemset(buf, 0x00, size)
#define PKA_FAST_MEMCPY(dst, src, size) PKA_WordMemcpy(dst, src, size)

#define PKA_BIGNUM_IS_ZERO(x) ((x)->nWords == 0)
#define PKA_BIGNUM_SIZE(x)    ((x)->nWords)
#define PKA_BIGNUM_MSW(x)     ((x)->data[PKA_BIGNUM_SIZE(x) - 1])

/********************* Private Structure Definition **************************/
struct PKA_REGS_MAP {
    uint32_t regesNum[PKA_MAX_PHYS_MEM_REGS_COUNT];
    uint32_t regsAddr[PKA_MAX_PHYS_MEM_REGS_COUNT];
};

/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/

/* ------------------------------------------------------------
 **
 * @brief This function executes a word memory copy between 2 buffers.
 *
 * @param[in] pDst - The first data buffer.
 * @param[in] pSrc - The second data buffer.
 * @param[in] size - the first data size in words.
 *
 */
static void PKA_WordMemcpy(uint32_t *pDst, uint32_t *pSrc, uint32_t size)
{
    uint32_t i;

    /* execute the word memcopy */
    for (i = 0; i < size; i++) {
        pDst[i] = pSrc[i];
    }
} /* END OF PKA_WordMemcpy */

/* ------------------------------------------------------------
 **
 * @brief This function executes a memory set operation on a buffer.
 *
 * @param[in] pBuff - the buffer.
 * @param[in] val   - The value to set the buffer.
 * @param[in] size  - the buffers size in words.
 *
 */
static void PKA_WordMemset(uint32_t *pBuff, uint32_t val, uint32_t size)
{
    uint32_t i;

    /* execute the reverse memcopy */
    for (i = 0; i < size; i++) {
        pBuff[i] = val;
    }
} /* END OF PKA_WordMemset */

static void PKA_RamCtrlEnable(void)
{
    WRITE_REG_MASK_WE(CRYPTO->RAM_CTL, CRYPTO_RAM_CTL_RAM_PKA_RDY_MASK, CRYPTO_RAM_PKA_RDY);
}

static void PKA_RamCtrlDisable(void)
{
    WRITE_REG_MASK_WE(CRYPTO->RAM_CTL, CRYPTO_RAM_CTL_RAM_PKA_RDY_MASK, 0);
}

static void PKA_WaitRamRdy(void)
{
    uint32_t output_reg_val;

    do {
        output_reg_val = READ_REG(CRYPTO->RAM_ST);
    } while ((output_reg_val & CRYPTO_RAM_ST_CLK_RAM_RDY_MASK) != CRYPTO_CLK_RAM_RDY);
}

static void PKA_WaitPipeRdy(void)
{
    uint32_t output_reg_val;

    do {
        output_reg_val = READ_REG(CRYPTO->PKA_PIPE_RDY);
    } while ((output_reg_val & CRYPTO_PKA_PIPE_RDY_PKA_PIPE_RDY_MASK) != CRYPTO_PKA_PIPE_RDY);
}

static void PKA_WaitDone(void)
{
    uint32_t output_reg_val;

    do {
        output_reg_val = READ_REG(CRYPTO->PKA_DONE);
    } while ((output_reg_val & CRYPTO_PKA_DONE_PKA_DONE_MASK) != CRYPTO_PKA_DONE);
}

static uint32_t PKA_CheckStatus(uint32_t mask)
{
    uint32_t status;

    PKA_WaitDone();
    status = READ_REG(CRYPTO->PKA_STATUS);
    status = status & mask;

    return !!status;
}

static void PKA_ReadRegSize(uint32_t *pSizeBits, uint32_t entryNum)
{
    PKA_WaitDone();
    *pSizeBits = READ_REG(CRYPTO->PKA_L[entryNum]);
}

static void PKA_GetRegAddr(uint32_t virReg, uint32_t *pRamAddr)
{
    *pRamAddr = READ_REG(CRYPTO->PKA_MEM_MAP[virReg]);
}

static void PKA_ReadRegAddr(uint32_t virReg, uint32_t *pRamAddr)
{
    PKA_WaitDone();
    *pRamAddr = READ_REG(CRYPTO->PKA_MEM_MAP[virReg]);
}

static uint32_t PKA_MakeFullOpcode(uint32_t opcode, uint32_t lenId,
                                   uint32_t isAImmed, uint32_t opA,
                                   uint32_t isBImmed, uint32_t opB,
                                   uint32_t resDiscard, uint32_t res,
                                   uint32_t tag)
{
    uint32_t fullOpcode;

    fullOpcode =
        (((uint32_t)(opcode) & 31) << CRYPTO_PKA_OPCODE_OPCODE_SHIFT |
         ((uint32_t)(lenId) & 7) << CRYPTO_PKA_OPCODE_LEN_SHIFT |
         ((uint32_t)(isAImmed) & 1) << CRYPTO_PKA_OPCODE_OPERAND_1_IMMED_SHIFT |
         ((uint32_t)(opA) & 31) << CRYPTO_PKA_OPCODE_REG_A_SHIFT |
         ((uint32_t)(isBImmed) & 1) << CRYPTO_PKA_OPCODE_OPERAND_2_IMMED_SHIFT |
         ((uint32_t)(opB) & 31) << CRYPTO_PKA_OPCODE_REG_B_SHIFT |
         ((uint32_t)(resDiscard) & 1) << CRYPTO_PKA_OPCODE_R_DISCARD_SHIFT |
         ((uint32_t)(res) & 31) << CRYPTO_PKA_OPCODE_REG_R_SHIFT |
         ((uint32_t)(tag) & 31) << CRYPTO_PKA_OPCODE_TAG_SHIFT);

    return fullOpcode;
}

static void PKA_LoadData(uint32_t addr, uint32_t *pData,
                         uint32_t sizeWords)
{
    uint8_t *vaddr =
        (uint8_t *)((addr) + (uint8_t *)&CRYPTO->SRAM_ADDR);

    PKA_RamCtrlDisable();
    PKA_WaitRamRdy();
    PKA_FAST_MEMCPY((uint32_t *)vaddr, pData, sizeWords);
    PKA_RamCtrlEnable();
}

static void PKA_ClearMem(uint32_t addr, uint32_t sizeWords)
{
    uint8_t *vaddr =
        (uint8_t *)((addr) + (uint8_t *)&CRYPTO->SRAM_ADDR);

    PKA_RamCtrlDisable();
    PKA_WaitRamRdy();
    PKA_MEMSET_ZERO((uint32_t *)vaddr, sizeWords);
    PKA_RamCtrlEnable();
}

static void PKA_ReadData(uint32_t addr, uint32_t *pData,
                         uint32_t sizeWords)
{
    uint8_t *vaddr =
        (uint8_t *)((addr) + (uint8_t *)&CRYPTO->SRAM_ADDR);

    PKA_RamCtrlDisable();
    PKA_WaitRamRdy();
    PKA_FAST_MEMCPY(pData, (uint32_t *)vaddr, sizeWords);
    PKA_RamCtrlEnable();
}

static HAL_Status PKA_ExecOpration(enum PKA_OPCODE opcode, uint8_t lenId,
                                   uint8_t isAImmed, int8_t opA,
                                   uint8_t isBImmed, int8_t opB,
                                   uint8_t resDiscard, int8_t res, uint8_t tag)
{
    uint32_t fullOpcode;
    HAL_Status error = HAL_OK;

    if (res == RES_DISCARD) {
        resDiscard = 1;
        res = 0;
    }

    fullOpcode = PKA_MakeFullOpcode(opcode, lenId,
                                    isAImmed, opA,
                                    isBImmed, opB,
                                    resDiscard, res, tag);

    /* write full opcode into PKA CRYPTO_OPCODE register */
    WRITE_REG(CRYPTO->PKA_OPCODE, fullOpcode);

    /*************************************************/
    /* finishing operations for different cases      */
    /*************************************************/
    switch (opcode) {
    case PKA_OPCODE_DIV:
        /* for Div operation check, that opB != 0*/
        if (PKA_CheckStatus(CRYPTO_PKA_STATUS_DIV_BY_ZERO_MASK)) {
            error = HAL_ERROR;
            goto end;
        }
        break;
    case PKA_OPCODE_TERMINATE:
        /* wait for PKA done bit */
        PKA_WaitDone();
        break;
    default:
        /* wait for PKA pipe ready bit */
        PKA_WaitPipeRdy();
    }

end:

    return error;
}

static HAL_Status PKA_SetSizeTab(uint32_t *pRegsSizes, uint32_t countOfSizes,
                                 uint32_t maxSizeBits, uint32_t isDefaultMap)
{
    uint32_t i;
    HAL_Status error;
    uint32_t maxSize, minSize, maxSizeWords;

    error = HAL_OK;
    maxSize = 0;
    minSize = 0xFFFFFFFF;

    if (isDefaultMap > 1) {
        return HAL_ERROR;
    }

    /* 1. Case of user defined settings */
    if (isDefaultMap == 0) {
        /* find maximal and minimal sizes  */
        for (i = 0; i < countOfSizes; i++) {
            if (maxSize < pRegsSizes[i] &&
                pRegsSizes[i] != 0xFFFFFFFF) {
                maxSize = pRegsSizes[i];
            }

            if (minSize > pRegsSizes[i]) {
                minSize = pRegsSizes[i];
            }
        }

        /* set sizes into PKA registers sizes table */
        for (i = 0; i < countOfSizes; i++) {
            WRITE_REG(CRYPTO->PKA_L[i], pRegsSizes[i]);
        }
    } else {
        /* 2. Case of default settings */
        maxSizeWords = (maxSizeBits + 31) / 32;
        /* write exact size into first table entry */
        WRITE_REG(CRYPTO->PKA_L[0], maxSizeBits);

        /* write size with extra word into tab[1] = tab[0] + 32 */
        WRITE_REG(CRYPTO->PKA_L[1], 32 * maxSizeWords + 32);

        /* count of entries, which was set */
        countOfSizes = 2;
    }

    for (i = countOfSizes; i < 8; i++) {
        WRITE_REG(CRYPTO->PKA_L[i], 0xFFFFFFFF);
    }

    return error;
}

static HAL_Status PKA_SetMapTab(struct PKA_REGS_MAP *pRegsMap,
                                uint32_t *pCountOfRegs, uint32_t maxSizeWords,
                                uint32_t N_NP_T0_T1, uint32_t isDefaultMap)
{
    uint32_t i;
    HAL_Status error;
    uint32_t curAddr;
    uint32_t defaultMaxSize, defaultCountOfRegs;

    error = HAL_OK;
    curAddr = 0;

    if (isDefaultMap == 1) {
        defaultMaxSize = 32 * maxSizeWords;
        defaultCountOfRegs =
            min(32, (8 * PKA_MAX_REGS_MEM_SIZE_BYTES) /
                defaultMaxSize);

        for (i = 0; i < 32 - 2; i++) {
            if (i < defaultCountOfRegs - 2) {
                WRITE_REG(CRYPTO->PKA_MEM_MAP[i], curAddr);
                curAddr = curAddr + defaultMaxSize / 8;
            } else {
                WRITE_REG(CRYPTO->PKA_MEM_MAP[i], 0xFFC);
            }
        }

        WRITE_REG(CRYPTO->PKA_MEM_MAP[30], curAddr);
        curAddr = curAddr + defaultMaxSize / 8;
        WRITE_REG(CRYPTO->PKA_MEM_MAP[31], curAddr);
        *pCountOfRegs = defaultCountOfRegs;
        WRITE_REG(CRYPTO->N_NP_T0_T1_ADDR, (uint32_t)RK_PKA_N_NP_T0_T1_REG_DEFAULT_VAL);
    }

    if (isDefaultMap == 0) {
        for (i = 0; i < *pCountOfRegs; i++) {
            WRITE_REG(CRYPTO->PKA_MEM_MAP[pRegsMap->regesNum[i]], pRegsMap->regsAddr[i]);
        }
        WRITE_REG(CRYPTO->N_NP_T0_T1_ADDR, N_NP_T0_T1);
    }

    return error;
}

static HAL_Status PKA_ClearBlockOfRegs(uint8_t firstReg, uint8_t countOfRegs,
                                       uint8_t lenId)
{
    uint32_t i;
    uint32_t size, addr;
    int32_t countTemps;

    PKA_ReadRegSize(&size, lenId);

    countTemps = 0;

    if (firstReg + countOfRegs > 30) {
        countTemps = min((countOfRegs + firstReg - 30), 2);
        countOfRegs = 30;
    } else {
        countTemps = 2;
    }

    /* clear ordinary registers */
    for (i = 0; i < countOfRegs; i++) {
        RK_PKA_Clr(lenId, firstReg + i /*regNum*/, 0 /*tag*/);
    }

    /* clear PKA temp registers using macros (without PKA operations */
    if (countTemps > 0) {
        /* calculate size of register in words */
        size = (size + 31) / 32;
        PKA_WaitDone();
        PKA_GetRegAddr(30 /*vir_reg*/, &addr /*phys_addr*/);
        PKA_ClearMem(addr, size);

        if (countTemps > 1) {
            PKA_GetRegAddr(31 /*vir_reg*/, &addr /*phys_addr*/);
            PKA_ClearMem(addr, size);
        }
    }

    return HAL_OK;
}

static HAL_Status PKA_Init(uint32_t *pRegsSizes, uint32_t countOfSizes,
                           struct PKA_REGS_MAP *pRegsMap, uint32_t countOfRegs,
                           uint32_t opSizeBits, uint32_t regSizeWords,
                           uint32_t N_NP_T0_T1, uint32_t isDefaultMap)
{
    uint32_t addr;
    HAL_Status error;

    error = HAL_OK;

    PKA_CLK_ENABLE();
    PKA_RamCtrlEnable();

    error = PKA_SetSizeTab(pRegsSizes, countOfSizes,
                           opSizeBits, isDefaultMap);

    if (error != HAL_OK) {
        return error;
    }

    error = PKA_SetMapTab(pRegsMap, &countOfRegs, regSizeWords,
                          N_NP_T0_T1, isDefaultMap);

    if (error != HAL_OK) {
        return error;
    }

    /* set size of register into RegsSizesTable */
    WRITE_REG(CRYPTO->PKA_L[3], 32 * regSizeWords);

    /* clean PKA data memory */
    PKA_ClearBlockOfRegs(0, countOfRegs - 2, 3);

    /* clean temp PKA registers 30,31 */
    PKA_WaitDone();
    PKA_GetRegAddr(30 /*vir_reg*/, &addr /*phys_addr*/);
    PKA_ClearMem(addr, regSizeWords);
    PKA_GetRegAddr(31 /*vir_reg*/, &addr /*phys_addr*/);
    PKA_ClearMem(addr, regSizeWords);

    return error;
}

static void PKA_Finish(void)
{
    RK_PKA_Terminate(0);
    PKA_CLK_DISABLE();
}

static void PKA_CopyDataIntoReg(int8_t dstReg, uint8_t lenId,
                                uint32_t *pSrc, uint32_t sizeWords)
{
    uint32_t cur_addr;
    uint32_t reg_size;

    RK_PKA_Terminate(0);

    PKA_ReadRegAddr(dstReg, &cur_addr);

    PKA_ReadRegSize(&reg_size, lenId);
    reg_size = (reg_size + 31) / 32;

    PKA_LoadData(cur_addr, pSrc, sizeWords);
    cur_addr = cur_addr + sizeof(uint32_t) * sizeWords;

    PKA_ClearMem(cur_addr, reg_size - sizeWords);
}

static void PKA_CopyDataFromReg(uint32_t *pDst, uint32_t sizeWords,
                                int8_t srcReg)
{
    uint32_t curAddr;

    WRITE_REG(CRYPTO->PKA_OPCODE, 0);

    PKA_WaitDone();

    PKA_ReadRegAddr(srcReg, &curAddr);

    PKA_ReadData(curAddr, pDst, sizeWords);
}

/***********   PKA_DivLongNum function      **********************/
/**
 * @brief The function divides long number A*(2^S) by B:
 *            res =  A*(2^S) / B,  remainder A = A*(2^S) % B.
 *        where: A,B - are numbers of size, which is not grate than,
 *       maximal operands size,
 *       and B > 2^S;
 *               S  - exponent of binary factor of A.
 *               ^  - exponentiation operator.
 *
 *        The function algorithm:
 *
 *        1. Let nWords = S/32; nBits = S % 32;
 *        2. Set res = 0, r_t1 = opA;
 *        3. for(i=0; i<=nWords; i++) do:
 *            3.1. if(i < nWords )
 *                   s1 = 32;
 *                 else
 *                   s1 = nBits;
 *            3.2. r_t1 = r_t1 << s1;
 *            3.3. call PKA_div for calculating the quotient and remainder:
 *                      r_t2 = floor(r_t1/opB) //quotient;
 *                      r_t1 = r_t1 % opB   //remainder (is in r_t1 register);
 *            3.4. res = (res << s1) + r_t2;
 *           end do;
 *        4. Exit.
 *
 *        Assuming:
 *                  - 5 PKA registers are used: opA, opB, res, r_t1, r_t2.
 *                  - The registers sizes and mapping tables are set on
 *                    default mode according to operands size.
 *                  - The PKA clocks are initialized.
 *        NOTE !   Operand opA shall be overwritten by remainder.
 *
 * @param[in] lenId    - ID of operation size (modSize+32).
 * @param[in] opA      - Operand A: virtual register pointer of A.
 * @param[in] S        - exponent of binary factor of A.
 * @param[in] opB      - Operand B: virtual register pointer of B.
 * @param[in] res      - Virtual register pointer for result quotient.
 * @param[in] r_t1      - Virtual pointer to remainder.
 * @param[in] r_t2      - Virtual pointer of temp register.
 * @param[in] VirtualHwBaseAddr -  Virtual HW base address, passed by user.
 *
 * @return CRYSError_t - On success HAL_OK is returned:
 *
 */
static HAL_Status PKA_DivLongNum(uint8_t lenId, int8_t opA, uint32_t s,
                                 int8_t opB, int8_t res, int8_t rT1, int8_t rT2)
{
    int8_t s1;
    uint32_t i;
    uint32_t nBits, nWords;

    /* calculate shifting parameters (words and bits ) */
    nWords = ((uint32_t)s + 31) / 32;
    nBits = (uint32_t)s % 32;

    /* copy operand opA (including extra word) into temp reg rT1 */
    RK_PKA_Copy(lenId + 1, rT1 /*dst*/, opA /*src*/, 0 /*tag*/);

    /* set res = 0 (including extra word) */
    RK_PKA_Clear(lenId + 1, res /*dst*/, 0 /*tag*/);

    /* set s1 = 0 for first dividing in loop */
    s1 = 0;

    /*----------------------------------------------------*/
    /* Step 1.  Shifting and dividing loop                */
    /*----------------------------------------------------*/
    for (i = 0; i < nWords; i++) {
        /* 3.1 set shift value s1  */
        if (i > 0) {
            s1 = 32;
        } else {
            s1 = nBits;
        }

        /* 3.2. shift: rT1 = rT1 * 2**s1 (in code (s1-1),
         * because PKA performs s+1 shifts)
         */
        if (s1 > 0) {
            RK_PKA_SHL0(lenId + 1, rT1 /*opA*/, (s1 - 1) /*s*/,
                        rT1 /*res*/, 0 /*tag*/);
        }

        /* 3.3. perform PKA_OPCODE_MOD_DIV for calculating a quotient
         * rT2 = floor(rT1 / N)
        and remainder rT1 = rT1 % opB
         */
        RK_PKA_Div(lenId + 1, rT1 /*opA*/, opB /*B*/, rT2 /*res*/,
                   0 /*tag*/);

        /* 3.4. res = res * 2**s1 + res;   */
        if (s1 > 0) {
            RK_PKA_SHL0(lenId + 1, res /*opA*/, (s1 - 1) /*s*/,
                        res /*res*/, 0 /*tag*/);
        }

        RK_PKA_Add(lenId + 1, res /*opA*/, rT2 /*opB*/, res /*res*/,
                   0 /*tag*/);
    }

    PKA_WaitDone();

    return HAL_OK;
}  /* END OF PKA_DivLongNum */

static uint32_t PKA_CalcAndInitNP(uint32_t lenId, uint32_t modSizeBits,
                                  int8_t rT0, int8_t rT1, int8_t rT2)
{
    uint32_t i;
    uint32_t s;
    HAL_Status error;
    uint32_t numBits, numWords;

    /* Set s = 132 */
    s = 132;

    /*-------------------------------------------------------------------*/
    /* Step 1,2. Set registers: Set opA = 2^(sizeN+32)                  */
    /*           Registers using: 0 - N (is set in register 0,           */
    /*           1 - NP, temp regs: rT0 (A), rT1, r_t2.                */
    /*           lenId: 0 - exact size, 1 - exact+32 bit                */
    /*-------------------------------------------------------------------*/

    /* set register rT0 = 0 */
    RK_PKA_Clr(lenId + 1, rT0 /*opA*/, 0 /*tag*/); /* r2 = 0 */

    /* calculate bit position of said bit in the word */
    numBits = modSizeBits % 32;
    numWords = modSizeBits / 32;

    /* set 1 into register rT0 */
    RK_PKA_Set0(lenId + 1, rT0 /*opA*/, rT0 /*res*/, 0 /*tag*/);

    /* shift 1 to numBits+31 position */
    if (numBits > 0) {
        RK_PKA_SHL0(lenId + 1, rT0 /*opA*/, numBits - 1 /*s*/,
                    rT0 /*res*/, 0 /*tag*/);
    }

    /* shift to word position */
    for (i = 0; i < numWords; i++) {
        RK_PKA_SHL0(lenId + 1, rT0 /*opA*/, 31 /*s*/,
                    rT0 /*res*/, 0 /*tag*/);
    }

    /*-------------------------------------------------------------------*/
    /* Step 3.  Dividing:  (opA * 2**s) / N                             */
    /*-------------------------------------------------------------------*/
    error = PKA_DivLongNum(lenId,        /*lenId*/
                           rT0,   /*opA*/
                           s,     /*shift*/
                           0,     /*opB = N*/
                           1,     /*res NP*/
                           rT1,   /*temp reg*/
                           rT2 /*temp reg*/);

    return error;
}  /* END OF PKA_CalcAndInitNP */

/******PKA_InitModop function (physical pointers)***************/
/**
 * @brief The function initializes  modulus and Barret tag NP,
 *        used in modular PKA operations.
 *
 *        The function does the following:
 *          - calculates mod size in bits and sets it into PKA table sizes;
 *          - if parameter NpCreateFlag = PKA_CreateNP, then the function
 *            writes the modulus and the tag into registers
 *            r0 and r1 accordingly;
 *          - if NpCreateFlag= PKA_SetNP, the function calls the
 *            LLF_PKI_PKA_ExecCalcNpAndInitModOp, which calculates the Barret
 *            tag NP and initializes PKA registers; then the function outputs
 *            calcu1lated NP value.
 *
 *       Assumings: - The registers mapping table is set on default mode,
 *            according to modulus size:
 *         -- count of allowed registers is not less, than 7 (including 3
 *            registers r_t0,r_t2,rT3 for internal calculations and 4 default
 *            special registers N,NP,T0,T1);
 *         -- modulus exact and exact+32 bit sizes should be set into first
 *            two entries of sizes-table accordingly.
 *
 * @param[in]  N_ptr        - The pointer to the buffer, containing modulus N,
 * @param[in]  N_sizeBits   - The size of modulus in bytes, must be
 *              16 <= N_sizeBytes <= 264.
 * @param[out] NP_ptr       - The pointer to the buffer, containing
 *              result - modulus tag NP.
 * @param[in]  NpCreateFlag - Parameter, defining whether the NP shall be
 *              taken from NP buffer and set into
 *                            PKA register NP ( NpCreateFlag= PKA_CreateNP= 1 )
 *                            or it shall be calculated and send to
 *                            NP buffer ( NpCreateFlag= PKA_SetNP= 0 ).
 * @param[in]  r_t0,r_t1,r_t2  - Virtual pointers to temp registers
 *                        (sequence numbers).
 * @param[in]  VirtualHwBaseAddr -  Virtual HW base address, passed by user.
 *
 * @return CRYSError_t - On success HAL_OK is returned,
 *              on failure an error code:
 *              LLF_PKI_PKA_ILLEGAL_PTR_ERROR
 *              LLF_PKI_PKA_ILLEGAL_OPERAND_LEN_ERROR
 *
 */
static HAL_Status PKA_InitModop(uint32_t *pN, uint32_t nSizeBits,
                                int8_t rT0, int8_t rT1, int8_t rT2)
{
    uint32_t nSizeWords;
    HAL_Status error = HAL_OK;

    /* calculate size of modulus in bytes and in words */
    nSizeWords = (nSizeBits + 31) / 32;

    /* copy modulus N into r0 register */
    PKA_CopyDataIntoReg(0 /*dst_reg*/, 1 /*lenId*/, pN /*src_ptr*/,
                        nSizeWords);

    /*---------------------------------------------------------*/
    /*     execute calculation of NP and initialization of PKA */
    /*---------------------------------------------------------*/

    error = PKA_CalcAndInitNP(0 /*lenId*/, nSizeBits,
                              rT0, rT1, rT2);

    /* End of the function */
    return error;
} /* END OF PKA_InitModop */

/*get bignum data length*/
static uint32_t PKA_BN_CheckSize(uint32_t *data, uint32_t maxWordSize)
{
    for (int64_t i = (maxWordSize - 1); i >= 0; i--) {
        if (data[i] == 0) {
            continue;
        } else {
            return (uint32_t)(i + 1);
        }
    }

    return 0;
}

/*  --------------------------------------------------------------------
 *  Function:  PKA_BN_HighestBitIndex
 *  Returns the index of the highest 1 in |src|.
 *  The index starts at 0 for the least significant bit.
 *  If src == zero, it will return -1
 *
 */
static int PKA_BN_HighestBitIndex(const struct CRYPTO_BIGNUM *src)
{
    uint32_t w;
    uint32_t b;

    if (PKA_BIGNUM_IS_ZERO(src)) {
        return -1;
    }

    w = PKA_BIGNUM_MSW(src);

    for (b = 0; b < RK_WORD_SIZE; b++) {
        w >>= 1;
        if (w == 0) {
            break;
        }
    }

    return (int)(PKA_BIGNUM_SIZE(src) - 1) * RK_WORD_SIZE + b;
}

/*c = a % b*/
static HAL_Status PKA_Mod(struct CRYPTO_BIGNUM *m_a, struct CRYPTO_BIGNUM *m_b,
                          struct CRYPTO_BIGNUM *m_c)
{
    uint32_t max_word_size;
    uint32_t error = HAL_OK;

    if (!m_a || !m_b || !m_c || PKA_BIGNUM_SIZE(m_b) == 0) {
        error = HAL_ERROR;
        goto exit;
    }

    max_word_size = PKA_BIGNUM_SIZE(m_a);
    if (max_word_size < PKA_BIGNUM_SIZE(m_b)) {
        max_word_size = PKA_BIGNUM_SIZE(m_b);
    }

    error = RK_PKA_DefaultInitPKA(max_word_size * 32, max_word_size + 1);
    if (error != HAL_OK) {
        goto exit;
    }

    PKA_CopyDataIntoReg(2 /*dst_reg*/, 1 /*lenId*/,
                        m_a->data /*src_ptr*/,
                        PKA_BIGNUM_SIZE(m_a));
    PKA_CopyDataIntoReg(3 /*dst_reg*/, 1 /*lenId*/,
                        m_b->data /*src_ptr*/,
                        PKA_BIGNUM_SIZE(m_b));
    RK_PKA_Div(0 /*lenId*/, 2 /*opA*/, 3 /*opB*/, 4 /*res*/, 0 /*tag*/);
    PKA_CopyDataFromReg(m_c->data, max_word_size, 2 /*srcReg*/);
    m_c->nWords = PKA_BN_CheckSize(m_c->data, max_word_size);

    PKA_ClearBlockOfRegs(0 /*FirstReg*/, 5 /*Count*/, 1 /*lenId*/);
    PKA_ClearBlockOfRegs(30 /*FirstReg*/, 2 /*Count*/, 1 /*lenId*/);
    PKA_Finish();

exit:

    return error;
}

/** @} */

/********************* Public Function Definition ****************************/

/** @addtogroup CRYPTO_Exported_Functions_Group5 Other Functions
 *  @{
 */

/**
 * @brief  calculate exp mod. pOut = pIn ^ pE mod pN
 * @param  pCrypto: the handle of crypto.
 * @param  pIn: the point of input data bignum.
 * @param  pE: the point of exponent bignum.
 * @param  pN: the point of modulus bignum.
 * @param  pOut: the point of outputs bignum.
 * @param  pTmp: the point of tmpdata bignum.
 * @return HAL_Status
 */
HAL_Status HAL_CRYPTO_ExptMod(struct CRYPTO_DEV *pCrypto,
                              struct CRYPTO_BIGNUM *pIn,
                              struct CRYPTO_BIGNUM *pE,
                              struct CRYPTO_BIGNUM *pN,
                              struct CRYPTO_BIGNUM *pOut,
                              struct CRYPTO_BIGNUM *pTmp)
{
    HAL_Status error = HAL_OK;
    uint32_t maxWordSize, exactSize;

    PKA_Mod(pIn, pN, pTmp);

    if (!pCrypto || !pIn || !pE || !pN || !pOut || PKA_BIGNUM_SIZE(pN) == 0) {
        error = HAL_ERROR;
        goto exit;
    }

    maxWordSize = PKA_BIGNUM_SIZE(pTmp);
    if (maxWordSize < PKA_BIGNUM_SIZE(pE)) {
        maxWordSize = PKA_BIGNUM_SIZE(pE);
    }
    if (maxWordSize < PKA_BIGNUM_SIZE(pN)) {
        maxWordSize = PKA_BIGNUM_SIZE(pN);
    }

    error = RK_PKA_DefaultInitPKA(maxWordSize * 32, maxWordSize + 1);
    if (error != HAL_OK) {
        goto exit;
    }
    /* write exact size into first table entry */
    exactSize = PKA_BN_HighestBitIndex(pN) + 1;
    WRITE_REG(CRYPTO->PKA_L[0], exactSize);

    /* write size with extra word into tab[1] = tab[0] + 32 */
    WRITE_REG(CRYPTO->PKA_L[1], exactSize + 32);

    /* calculate NP by initialization PKA for modular operations */
    error = PKA_InitModop(
        (pN)->data,    /*in N*/
        exactSize,     /*in N size*/
        2,             /*in *r_t0*/
        3,             /*in r_t1*/
        4 /*in r_t2*/);
    if (error != HAL_OK) {
        goto exit;
    }
    PKA_ClearBlockOfRegs(2 /* FirstReg*/, 3, 1 /*lenId*/);
    PKA_CopyDataIntoReg(2 /*dst_reg*/, 1 /*lenId*/,
                        pTmp->data /*src_ptr*/,
                        PKA_BIGNUM_SIZE(pTmp));
    PKA_CopyDataIntoReg(3 /*dst_reg*/, 1 /*lenId*/,
                        (pE)->data /*src_ptr*/,
                        PKA_BIGNUM_SIZE(pE));
    PKA_CopyDataIntoReg(0 /*dst_reg*/, 1 /*lenId*/,
                        pN->data /*src_ptr*/,
                        PKA_BIGNUM_SIZE(pN));
    RK_PKA_ModExp(0, 2, 3, 4, 0);
    PKA_CopyDataFromReg(pOut->data, maxWordSize, 4 /*srcReg*/);
    pOut->nWords = PKA_BN_CheckSize(pOut->data, maxWordSize);

    PKA_ClearBlockOfRegs(0 /*FirstReg*/, 5 /*Count*/, 1 /*lenId*/);
    PKA_ClearBlockOfRegs(30 /*FirstReg*/, 2 /*Count*/, 1 /*lenId*/);
    PKA_Finish();

exit:

    return error;
}
/** @} */

/** @} */

/** @} */

#endif /* RKMCU_RK2206 && HAL_CRYPTO_MODULE_ENABLED */
