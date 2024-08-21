
#include <rthw.h>
#include <rtthread.h>
#include "task.h"

/**
 * This function will init system tick and set it to zero.
 * @ingroup SystemInit
 *
 * @deprecated since 1.1.0, this function does not need to be invoked
 * in the system initialization.
 */
void rt_system_tick_init(void)
{
}

/**
 * @addtogroup Clock
 */

/**@{*/

/**
 * This function will return current tick from operating system startup
 *
 * @return current tick
 */
rt_tick_t rt_tick_get(void)
{
    if (xPortIsInsideInterrupt() == pdFALSE)
        return xTaskGetTickCount();
    else
        return xTaskGetTickCountFromISR();
}
RTM_EXPORT(rt_tick_get);

/**
 * This function will set current tick
 */
void rt_tick_set(rt_tick_t tick)
{
#if ( configUSE_TICKLESS_IDLE != 0 )
    rt_tick_t curr;

    curr = rt_tick_get();
    configASSERT(tick >= curr);
    vTaskStepTick(tick - curr);
#endif
}

/**
 * This function will notify kernel there is one tick passed. Normally,
 * this function is invoked by clock ISR.
 */
void rt_tick_increase(void)
{
    /* check timer */
    rt_timer_check();
}

/**
 * This function will calculate the tick from millisecond.
 *
 * @param ms the specified millisecond
 *           - Negative Number wait forever
 *           - Zero not wait
 *           - Max 0x7fffffff
 *
 * @return the calculated tick
 */
int rt_tick_from_millisecond(rt_int32_t ms)
{
    int tick;

    if (ms < 0)
        tick = RT_WAITING_FOREVER;
    else
        tick = (RT_TICK_PER_SECOND * ms + 999) / 1000;

    /* return the calculated tick */
    return tick;
}
RTM_EXPORT(rt_tick_from_millisecond);

/**@}*/

