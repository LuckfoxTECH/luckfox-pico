#ifndef _COMPONENT_DEF_H_
#define _COMPONENT_DEF_H_

#define COMPOONENT_WARN 1
#define COMPOONENT_TASK 1

#define COMPONENT_LOG(flags, fmt, arg...)   \
        do {                                \
                if (flags)                      \
                    rt_kprintf(fmt, ##arg);     \
            } while (0)

#define COMPONENT_TRACK(fmt, arg...)    \
                COMPONENT_LOG(COMPOONENT_TASK, "[COMPONENT TRACK] %s():%d "fmt, \
                                                                                    __func__, __LINE__, ##arg)
#define COMPONENT_WARN(fmt, arg...) \
                COMPONENT_LOG(COMPOONENT_WARN, "[COMPONENT WARN] %s():%d "fmt, \
                                                                                    __func__, __LINE__, ##arg)

typedef enum
{
    COMP_OK     = 0,
    COMP_ERROR   = -1,
    COMP_BUSY   = -2,
    COMP_TIMEOUT = -3,
    COMP_INVALID = -4
} Component_Status;

#endif

