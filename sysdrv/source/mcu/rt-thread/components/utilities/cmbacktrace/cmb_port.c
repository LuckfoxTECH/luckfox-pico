/*
 * This file is part of the CmBacktrace Library.
 *
 * Copyright (c) 2016-2018, zylx, <1346773219@qq.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: Initialize function and other general function.
 * Created on: 2016-12-15
 */
#include <rtthread.h>
#include <rthw.h>
#include <cm_backtrace.h>
#include <string.h>

#ifdef RT_USING_COREDUMP
#include "../../../bsp/rockchip/common/drivers/coredump/drv_coredump.h"
#endif

#if defined(__CC_ARM)
    #pragma O1
#elif defined(__ICCARM__)
    #pragma optimize=none
#elif defined(__GNUC__)
    #pragma GCC optimize ("O0")
#endif

#ifdef RT_USNING_FAULT_DUMP_HOOK
struct fault_dump_hook
{
    fault_println println;
    dump_end dump_end;
    unsigned int faulted;
};

static struct fault_dump_hook fault_hook = {NULL, NULL, 0};
#endif


#if defined(__CC_ARM)
    static __inline __asm void cmb_set_psp(uint32_t psp) {
        msr psp, r0
        bx lr
    }
#elif defined(__ICCARM__)
/* IAR iccarm specific functions */
/* Close Raw Asm Code Warning */
#pragma diag_suppress=Pe940
    static void cmb_set_psp(uint32_t psp)
    {
      __asm("msr psp, r0");
      __asm("bx lr");
    }
#pragma diag_default=Pe940
#elif defined(__GNUC__)
    __attribute__( ( always_inline ) ) static inline void cmb_set_psp(uint32_t psp) {
        __asm volatile ("MSR psp, %0\n\t" :: "r" (psp) );
    }
#else
    #error "not supported compiler"
#endif

RT_WEAK rt_err_t exception_hook(void *context) {
    volatile uint8_t _continue = 1;
    uint8_t lr_offset = 0;
    uint32_t lr, sp;

#define CMB_LR_WORD_OFFSET_START       6
#define CMB_LR_WORD_OFFSET_END         20
#define CMB_SP_WORD_OFFSET             (lr_offset + 1)

#if (CMB_CPU_PLATFORM_TYPE == CMB_CPU_ARM_CORTEX_M0) || (CMB_CPU_PLATFORM_TYPE == CMB_CPU_ARM_CORTEX_M3)
#define EXC_RETURN_MASK                0x0000000F // Bits[31:4]
#else
#define EXC_RETURN_MASK                0x0000000F // Bits[31:5]
#endif

    rt_enter_critical();

#ifdef RT_USING_COREDUMP
    uint32_t *regs = (uint32_t *)context;

#if (defined (__VFP_FP__) && !defined(__SOFTFP__)) || (defined (__ARMVFP__)) || (defined(__ARM_PCS_VFP) || defined(__TARGET_FPU_VFP))
    regs += 2;
#else
    regs += 1;
#endif
    cd_regs[0] = regs[8];
    cd_regs[1] = regs[9];
    cd_regs[2] = regs[10];
    cd_regs[3] = regs[11];
    cd_regs[4] = regs[0];
    cd_regs[5] = regs[1];
    cd_regs[6] = regs[2];
    cd_regs[7] = regs[3];
    cd_regs[8] = regs[4];
    cd_regs[9] = regs[5];
    cd_regs[10] = regs[6];
    cd_regs[11] = regs[7];
    cd_regs[12] = regs[12];     /* r12 */
    cd_regs[13] = (uint32_t)(regs + 16);    /* sp */
    cd_regs[14] = regs[13];     /* lr */
    cd_regs[15] = regs[14];     /* pc */
    cd_regs[16] = regs[15];     /* psr */

    rt_coredump_handler(CD_EXCEPTION);
#endif

#ifdef RT_USNING_FAULT_DUMP_HOOK
    fault_hook.faulted = 1;
#endif

#ifdef RT_USING_FINSH
    extern long list_thread(void);
    list_thread();
#endif

    /* the PSP is changed by RT-Thread HardFault_Handler, so restore it to HardFault context */
#if (defined (__VFP_FP__) && !defined(__SOFTFP__)) || (defined (__ARMVFP__)) || (defined(__ARM_PCS_VFP) || defined(__TARGET_FPU_VFP))
    cmb_set_psp(cmb_get_psp() + 4 * 10);
#else
    cmb_set_psp(cmb_get_psp() + 4 * 9);
#endif

    /* auto calculate the LR offset */
    for (lr_offset = CMB_LR_WORD_OFFSET_START; lr_offset <= CMB_LR_WORD_OFFSET_END; lr_offset ++)
    {
        lr = *((uint32_t *)(cmb_get_sp() + sizeof(uint32_t) * lr_offset));
        /*
         * Cortex-M0: http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0497a/Babefdjc.html
         * Cortex-M3: http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0552a/Babefdjc.html
         * Cortex-M4: http://infocenter.arm.com/help/topic/com.arm.doc.dui0553b/DUI0553.pdf P41
         * Cortex-M7: http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0646c/Babefdjc.html
         */
        if ((lr == 0xFFFFFFF1) || (lr == 0xFFFFFFF9) || (lr == 0xFFFFFFFD)
#if (CMB_CPU_PLATFORM_TYPE == CMB_CPU_ARM_CORTEX_M4) || (CMB_CPU_PLATFORM_TYPE == CMB_CPU_ARM_CORTEX_M3)
            || (lr == 0xFFFFFFE1) || (lr == 0xFFFFFFE9) || (lr == 0xFFFFFFED)
#endif
           )
        {
            break;
        }
    }

#if (defined (__VFP_FP__) && !defined(__SOFTFP__)) || (defined (__ARMVFP__)) || (defined(__ARM_PCS_VFP) || defined(__TARGET_FPU_VFP))
    sp = cmb_get_sp() + sizeof(uint32_t) * CMB_SP_WORD_OFFSET + 4 * 10;
#else
    sp = cmb_get_sp() + sizeof(uint32_t) * CMB_SP_WORD_OFFSET + 4 * 9;
#endif

    cm_backtrace_fault(lr, sp);

    cmb_println("Current system tick: %ld", rt_tick_get());
	
#ifdef RT_USNING_FAULT_DUMP_HOOK
    if (fault_hook.dump_end)
        fault_hook.dump_end();
#endif

#ifdef PKG_CMBACKTRACE_FAULT_REBOOT
    rt_hw_cpu_reset();
#endif

    while (_continue == 1);

    return RT_EOK;
}

RT_WEAK void assert_hook(const char* ex, const char* func, rt_size_t line) {
    volatile uint8_t _continue = 1;

#ifdef RT_USING_COREDUMP
    rt_save_context_to();
    rt_coredump_handler(CD_ASSERT);
#endif

    rt_enter_critical();

#ifdef RT_USING_FINSH
    extern long list_thread(void);
    list_thread();
#endif

    cmb_println("");
    cmb_println("(%s) has assert failed at %s:%ld.", ex, func, line);

    cm_backtrace_assert(cmb_get_sp());

    cmb_println("Current system tick: %ld", rt_tick_get());

    while (_continue == 1);
}

#ifdef RT_USNING_FAULT_DUMP_HOOK
void hook_println(const char *format, ...)
{
    va_list ap;
    static char fault_log_buf[RT_CONSOLEBUF_SIZE];

    if (fault_hook.faulted && fault_hook.println) {
        va_start(ap, format);
        rt_vsnprintf(fault_log_buf, sizeof(fault_log_buf) - 1, format, ap);
        fault_hook.println(fault_log_buf);
        va_end(ap);
        fault_hook.println("\r\n");
    }
}

void cmb_set_hook(fault_println println, dump_end end)
{
    fault_hook.println = println;
    fault_hook.dump_end = end;
}
#endif

int rt_cm_backtrace_init(void) {
    cm_backtrace_init("rtthread","1.0","1.0");
    
    rt_hw_exception_install(exception_hook);

    rt_assert_set_hook(assert_hook);
    
    return 0;
}
INIT_DEVICE_EXPORT(rt_cm_backtrace_init);

long cmb_test(int argc, char **argv) {
    volatile int * SCB_CCR = (volatile int *) 0xE000ED14; // SCB->CCR
    int x, y, z;
    
    if (argc < 2)
    {
        rt_kprintf("Please input 'cmb_test <DIVBYZERO|UNALIGNED>' \n");
        return 0;
    }

    if (!strcmp(argv[1], "DIVBYZERO"))
    {
        *SCB_CCR |= (1 << 4); /* bit4: DIV_0_TRP. */
        x = 10;
        y = rt_strlen("");
        z = x / y;
        rt_kprintf("z:%d\n", z);
        
        return 0;
    }
    else if (!strcmp(argv[1], "UNALIGNED"))
    {
        volatile int * p;
        volatile int value;
        *SCB_CCR |= (1 << 3); /* bit3: UNALIGN_TRP. */

        p = (int *) 0x00;
        value = *p;
        rt_kprintf("addr:0x%02X value:0x%08X\r\n", (int) p, value);

        p = (int *) 0x04;
        value = *p;
        rt_kprintf("addr:0x%02X value:0x%08X\r\n", (int) p, value);

        p = (int *) 0x03;
        value = *p;
        rt_kprintf("addr:0x%02X value:0x%08X\r\n", (int) p, value);
        
        return 0;
    }
    return 0;
}
MSH_CMD_EXPORT(cmb_test, cm_backtrace_test: cmb_test <DIVBYZERO|UNALIGNED> );
