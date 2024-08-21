#include <rtthread.h>

#ifdef RT_USING_TINYMEMBENCH
extern int tinymembench_main(void);
#endif

#ifdef RT_USING_LINPACK
extern void linpack_main(void);
#endif

#ifdef RT_USING_DHRYSTONE
extern void dhrystone_main(void);
#endif

#ifdef RT_USING_FHOURSTONE
extern int fhourstones_main();
#endif

#ifdef RT_USING_PI_CSS5
extern int pi_css5_main(int argc, char *argv[]);
#endif

#ifdef RT_USING_WHETSTONE
extern int whetstone_main(int argc, char *argv[]);
#endif

#ifdef RT_USING_COREMARK
extern void coremark_main(void);
#endif

void benchmark(void)
{
#ifdef RT_USING_TINYMEMBENCH
    tinymembench_main();
#endif

#ifdef RT_USING_LINPACK
    linpack_main();
#endif

#ifdef RT_USING_DHRYSTONE
    dhrystone_main();
#endif

#ifdef RT_USING_FHOURSTONE
    fhourstones_main();
#endif

#ifdef RT_USING_PI_CSS5
    pi_css5_main(1, NULL);
#endif

#ifdef RT_USING_WHETSTONE
    whetstone_main(1, NULL);
#endif

#ifdef RT_USING_COREMARK
    coremark_main();
#endif
}

MSH_CMD_EXPORT(benchmark, benchmark);
