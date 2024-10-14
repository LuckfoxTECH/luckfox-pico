#include <stdint.h>

#include <rthw.h>
#include <rtthread.h>

void demo_test(void)
{
    rt_kprintf("this is demo_test\n");
}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(demo_test, demo test for driver);
#endif
