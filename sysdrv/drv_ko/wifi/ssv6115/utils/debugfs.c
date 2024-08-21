/*
 * Copyright (c) 2015 iComm-semi Ltd.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/proc_fs.h> /* for proc_mkdir, create */
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <asm/uaccess.h>	/* for copy_from_user */
#include <linux/errno.h>
#include <linux/fs.h>
//#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/buffer_head.h>
#include <linux/ctype.h> /* for isalpha & isdigit */
#include <linux/netdevice.h>

#include "debugfs.h"
#include "ssv_debug.h"

MODULE_AUTHOR("iComm-semi, Ltd");
MODULE_DESCRIPTION("Shared library for SSV wireless LAN cards.");
MODULE_LICENSE("Dual BSD/GPL");

/* for debug */
static struct proc_dir_entry *__ssv_procfs;

#define	READ_CHUNK	32 
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0)
#define PDE_DATA(inode) ({ \
    struct proc_dir_entry *dp = PDE(inode); \
    data = dp->data; })
#endif

extern struct ssv_cmd_table cmd_table[];

void ssv_snprintf_res(struct ssv_cmd_data *cmd_data, const char *fmt, ... )
{
    char *buf_head;
    int buf_left;
    va_list args;
    char *ssv6xxx_result_buf = cmd_data->ssv6xxx_result_buf;


    ssv6xxx_result_buf = cmd_data->ssv6xxx_result_buf;  
    
    // buf end reached
    if (cmd_data->rsbuf_len >= (cmd_data->rsbuf_size -1))
        return;
    buf_head = ssv6xxx_result_buf + cmd_data->rsbuf_len;
    buf_left = cmd_data->rsbuf_size - cmd_data->rsbuf_len;
   
    va_start(args, fmt); 
    cmd_data->rsbuf_len += vsnprintf(buf_head, buf_left, fmt, args);
    va_end(args);
    // No need for protection here. If vsnprint use more 
    // than buf_left. It will put 0x0 at last byte automatically
    // when overflow and stop copy,

}
static int ssv_cmd_submit(struct ssv_cmd_data *cmd_data, char *cmd)
{
    struct ssv_cmd_table *sc_tbl;
    char                 *pch, ch;
    int                   ret, bf_size;
    char                 *sg_cmd_buffer;
    char                 *sg_argv[CLI_ARG_SIZE];
    u32                   sg_argc;
    
    if (cmd_data->cmd_in_proc)
        return -1;

    sg_cmd_buffer = cmd;
    
    for (sg_argc = 0, ch = 0, pch = sg_cmd_buffer;
         (*pch!=0x00) && (sg_argc<CLI_ARG_SIZE); pch++ )
    {
//        if (*pch == 0x0a || *pch == 0x0d)
//            *pch = ' ';
        if ( (ch==0) && (*pch!=' ') )
        {
            ch = 1;
            sg_argv[sg_argc] = pch;
        }
    
        if ( (ch==1) && (*pch==' ') )
        {
            *pch = 0x00;
            ch = 0;
            sg_argc ++;
        }                               
    }
    if ( ch == 1) 
    {
        sg_argc ++;
    }
    else if ( sg_argc > 0 )
    {
        *(pch-1) = ' ';
    }               
    
    if ( sg_argc > 0 )
    {
        /* Dispatch command */
        for( sc_tbl=cmd_table; sc_tbl->cmd; sc_tbl ++ )
        {
            if ( !strcmp(sg_argv[0], sc_tbl->cmd) )
            {
                //struct ssv_softc         *sc;

                //sc = container_of(cmd_data, struct ssv_softc, cmd_data);
        #if 0
                if (sc_tbl->cmd_func_ptr != ssv_cmd_cfg) {
                    cmd_data->ssv6xxx_result_buf = (char *)kzalloc(128, GFP_KERNEL);
                    if (!cmd_data->ssv6xxx_result_buf)
                         return -EFAULT;
                    
                    cmd_data->ssv6xxx_result_buf[0] = 0x00; 
					ssv_snprintf_res(cmd_data, "The command is not found\n");
					return -1;
				}
        #endif
                cmd_data->ssv6xxx_result_buf = (char *)kzalloc(sc_tbl->result_buffer_size, GFP_KERNEL);
                if (!cmd_data->ssv6xxx_result_buf)
                    return -EFAULT;
		
                cmd_data->ssv6xxx_result_buf[0] = 0x00;
                cmd_data->rsbuf_len = 0;
                cmd_data->rsbuf_size = sc_tbl->result_buffer_size;
                cmd_data->cmd_in_proc = true;


                ret = sc_tbl->cmd_func_ptr(/* sc */cmd_data, sg_argc, sg_argv);
                if (ret < 0) {
                    strcpy(cmd_data->ssv6xxx_result_buf, "Invalid command !\n");
                }
                
                bf_size = strlen(cmd_data->ssv6xxx_result_buf);
                
                if ((sc_tbl->result_buffer_size -1) <= bf_size){
                    cmd_data->rsbuf_len = 0;
                    ssv_snprintf_res(cmd_data,
                       "\nALLOCATED BUFFER %d <= BUFFER USED +1 %d, OVERFLOW!!\n\n",
                       sc_tbl->result_buffer_size, bf_size+1);                   
                } else {
                    ssv_snprintf_res(cmd_data, "\nALLOCATED BUFFER %d , BUFFER USED %d\n\n",
                       sc_tbl->result_buffer_size, bf_size); 
                }

                return 0;
            }       

        }
        
        cmd_data->ssv6xxx_result_buf = (char *)kzalloc(64, GFP_KERNEL);
        if (!cmd_data->ssv6xxx_result_buf)
            return -EFAULT;
        
        cmd_data->ssv6xxx_result_buf[0] = 0x00;
        cmd_data->rsbuf_len = 0;
        cmd_data->rsbuf_size = 64;
        cmd_data->cmd_in_proc = true;                                       
	    ssv_snprintf_res(cmd_data, "Command not found !\n");
	    return -EFAULT;
	    
    } else {
        cmd_data->ssv6xxx_result_buf = (char *)kzalloc(64, GFP_KERNEL);
        if (!cmd_data->ssv6xxx_result_buf)
            return -EFAULT;
        
        
        cmd_data->ssv6xxx_result_buf[0] = 0x00;
        cmd_data->rsbuf_len = 0;
        cmd_data->rsbuf_size = 64;
        cmd_data->cmd_in_proc = true;
        ssv_snprintf_res(cmd_data, "./cli -h\n");
    }
    return 0;
}

static int ssv6xxx_cmd_file_open(struct inode *inode, struct file *filp)
{
    void *data = PDE_DATA(inode);

    filp->private_data = data;
    return 0;
}

static ssize_t ssv6xxx_cmd_file_read(struct file *filp, char __user *buffer,
                                size_t count, loff_t *ppos)
{
    char   *ssv6xxx_result_buf;
    struct ssv_cmd_data *cmd_data = filp->private_data;
    int len;
    int ret = 0;
    
    if (!(cmd_data->cmd_in_proc)){
        goto out;
    }    
    ssv6xxx_result_buf = cmd_data->ssv6xxx_result_buf; 
    cmd_data->cmd_in_proc = false;

    if (!ssv6xxx_result_buf){
        goto out;
    }
    cmd_data->ssv6xxx_result_buf = NULL;    
    if (*ppos != 0){
        goto free;
    }

    if (cmd_data->rsbuf_size < cmd_data->rsbuf_len)
        cmd_data->rsbuf_len = cmd_data->rsbuf_size-1;

    len = cmd_data->rsbuf_len + 1;

    if (len == 1){
        goto free;
    }

    if (copy_to_user(buffer, ssv6xxx_result_buf, len)){
        ret = -EFAULT;
        goto free;
    }    

    ret = len;
free:   
    kfree(ssv6xxx_result_buf);
out:
    return ret;

}


static ssize_t ssv6xxx_cmd_file_write(struct file *filp, const char __user *buffer,
                size_t count, loff_t *ppos)
{
    char *ssv6xxx_cmd_buf = NULL;
    //struct ssv_cmd_data *cmd_data = filp->private_data;

    if (*ppos != 0 || count > 255)
        return 0;

    ssv6xxx_cmd_buf = (char *)kzalloc(count, GFP_KERNEL);
    if (!ssv6xxx_cmd_buf)
        return 0;    

    ssv6xxx_cmd_buf[0] = 0x00;
    if (copy_from_user(ssv6xxx_cmd_buf, buffer, count))
        return -EFAULT;
    
    ssv6xxx_cmd_buf[count-1] = 0x00;
    ssv_cmd_submit((struct ssv_cmd_data *)filp->private_data, ssv6xxx_cmd_buf);
    kfree(ssv6xxx_cmd_buf);
    return count;
}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 5, 0))
static struct file_operations ssv6xxx_cmd_fops = {
    .owner = THIS_MODULE,
    .open  = ssv6xxx_cmd_file_open,
    .read  = ssv6xxx_cmd_file_read,
    .write = ssv6xxx_cmd_file_write,
};
#else
static struct proc_ops ssv6xxx_cmd_fops = {
	.proc_open  = ssv6xxx_cmd_file_open,
	.proc_read  = ssv6xxx_cmd_file_read,
	.proc_write = ssv6xxx_cmd_file_write,
};
#endif

int ssv_init_cli(const char *dev_name, struct ssv_cmd_data *cmd_data)
{
    struct proc_dir_entry *proc_file_entry;
    
    __ssv_procfs = proc_mkdir(PROC_DIR_ENTRY, NULL);
    if (!__ssv_procfs)
        return -ENOMEM;

    // Create directory for each device. Then create following proc entries under it.
    cmd_data->proc_dev_entry = proc_mkdir(dev_name, __ssv_procfs);
    if (!cmd_data->proc_dev_entry) 
        SSV_LOG_DBG("KERN_ERR" "Failed to create %s dev directory for CLI. \n", dev_name);

    proc_file_entry = proc_create_data(PROC_SSV_CMD_ENTRY, S_IRUGO|S_IWUGO, 
            cmd_data->proc_dev_entry, &ssv6xxx_cmd_fops, cmd_data);
    
    if (proc_file_entry == NULL)
        SSV_LOG_DBG("Failed to create %s for CLI.\n", PROC_SSV_CMD_ENTRY);

    return 0;
}
EXPORT_SYMBOL(ssv_init_cli);


void ssv_deinit_cli(const char *dev_name, struct ssv_cmd_data *cmd_data)
{
    remove_proc_entry(PROC_SSV_CMD_ENTRY, cmd_data->proc_dev_entry);
    remove_proc_entry(dev_name, __ssv_procfs);
	/* Remove dev directory must be last */
    remove_proc_entry(PROC_DIR_ENTRY, NULL);
}
EXPORT_SYMBOL(ssv_deinit_cli);

