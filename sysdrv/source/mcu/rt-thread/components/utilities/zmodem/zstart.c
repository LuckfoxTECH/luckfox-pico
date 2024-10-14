/*
 * File      : zstart.c
 * the implemention of zmodem protocol.
 * Change Logs:
 * Date           Author       Notes
 * 2011-03-29     itspy       
 */

#include <rtthread.h>
#include <finsh.h>
#include <shell.h>
#include <dfs.h>
#include <dfs_file.h>
#include "zdef.h"



struct zmodemf zmodem;

rt_err_t zmodem_rx_ind(rt_device_t dev, rt_size_t size)
{
    /* release semaphore */
    rt_sem_release(&zmodem.zsem);

    return RT_EOK;
}

void finsh_rz(void *parameter)
{
	char *path;
    rt_err_t (*rx_indicate)(rt_device_t dev, rt_size_t size);
    rt_uint8_t flag;	

	flag = RT_DEVICE_FLAG_STREAM;
    zmodem.device->flag &=(~flag);
    rt_sem_init(&(zmodem.zsem), "zsem", 0, 0);
	path = rt_thread_self()->parameter;
    /* save old rx_indicate	*/
    rx_indicate = zmodem.device->rx_indicate;
    /* set new rx_indicate */
    rt_device_set_rx_indicate(zmodem.device, RT_NULL);
	/* start receive remote files */
    zr_start(path);
	zmodem.device->flag |=flag;
    /* recovery old rx_indicate	*/
    rt_device_set_rx_indicate(zmodem.device, rx_indicate);
    /* finsh>> */
    rt_kprintf(FINSH_PROMPT);
    if (path)
        rt_free(path);
}
void finsh_sz(void *parameter)
{
	char *path;
    rt_err_t (*rx_indicate)(rt_device_t dev, rt_size_t size);
    rt_uint8_t flag;	

	flag = RT_DEVICE_FLAG_STREAM;
    zmodem.device->flag &=(~flag);
    rt_sem_init(&(zmodem.zsem), "zsem", 0, 0);
	path = rt_thread_self()->parameter;
	rt_kprintf("finsh_sz, path=%s, ptr=%p\n", path, rt_thread_self()->parameter);
	/* save old rx_indicate	*/
    rx_indicate = zmodem.device->rx_indicate;
	/* set new rx_indicate */
    rt_device_set_rx_indicate(zmodem.device, zmodem_rx_ind);
	zs_start(path);
	zmodem.device->flag |=flag;
    /* recovery old rx_indicate	*/
    rt_device_set_rx_indicate(zmodem.device, rx_indicate);
    /* finsh>> */
    rt_kprintf(FINSH_PROMPT);
    rt_free(path);
}

#ifdef RT_USING_FINSH
#include <finsh.h>
#include <shell.h>

static void rz(int argc, char **argv)
{
    rt_thread_t init_thread;
    rt_device_t device;
    const char* device_name = finsh_get_device();
    char *para;

    RT_ASSERT(argc == 1 || argc == 2);

    device = rt_device_find(device_name);
    if( device == RT_NULL )
    {
        rt_kprintf("%s not find\r\n",device_name);
    }
    zmodem.device = device;
    if (argv[1])
        para = rt_strdup(argv[1]);
    else
        para = NULL;
    init_thread = rt_thread_create("rz",
                                   finsh_rz,
                                   (void*)para,
                                   2048,
                                   rt_thread_self()->current_priority+1,
                                   20);

    if (init_thread != RT_NULL) rt_thread_startup(init_thread);
}
//FINSH_FUNCTION_EXPORT(rz, receive files by zmodem protocol);
FINSH_FUNCTION_EXPORT_ALIAS(rz, __cmd_rz, receive files by zmodem protocol);
static void sz(int argc, char **argv)
{
    rt_thread_t init_thread;
    rt_device_t device;
    const char* device_name = finsh_get_device();
    char *para;

    RT_ASSERT(argc == 2 && argv[1] != NULL)

    device = rt_device_find(device_name);
    if( device == RT_NULL )
    {
        rt_kprintf("%s not find\r\n",device_name);
    }
    zmodem.device = device;
    para = rt_strdup(argv[1]);
    init_thread = rt_thread_create("sz",
                                   finsh_sz,
                                   (void*)para,
                                   2048,
                                   rt_thread_self()->current_priority+1,
                                   20);

    if (init_thread != RT_NULL) rt_thread_startup(init_thread);
}
//FINSH_FUNCTION_EXPORT(sz, send files by zmodem protocol);
FINSH_FUNCTION_EXPORT_ALIAS(sz, __cmd_sz, send files by zmodem protocol);
#endif
