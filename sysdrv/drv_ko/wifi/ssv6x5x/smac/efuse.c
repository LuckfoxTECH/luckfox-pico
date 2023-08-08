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

/*
    4bits as efuse access unit.
*/
#include <linux/version.h>
#include <linux/etherdevice.h>
#include <ssv6200.h>
#include "efuse.h"
#include <hal.h>

mm_segment_t oldfs;
struct file *openFile(char *path,int flag,int mode)
{
    struct file *fp=NULL;

    fp=filp_open(path, flag, 0);
    if(IS_ERR(fp))
        return NULL;
    else
        return fp;
}
#if 0
int readFile(struct file *fp,char *buf,int readlen)
{
    if (fp->f_op && fp->f_op->read)
        return fp->f_op->read(fp,buf,readlen, &fp->f_pos);
    else
    return -1;
}
#else
static int readFile(struct file *fp, char *buf, int len)
{
        int rlen = 0, sum = 0;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0))
        if (!(fp->f_mode & FMODE_CAN_READ))
#else
        if (!fp->f_op || !fp->f_op->read)
#endif
                return -EPERM;

        while (sum < len) {
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0))
                rlen = kernel_read(fp, buf + sum, len - sum, &fp->f_pos);
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0))
                rlen = __vfs_read(fp, buf + sum, len - sum, &fp->f_pos);
#else
                rlen = fp->f_op->read(fp, buf + sum, len - sum, &fp->f_pos);
#endif
                if (rlen > 0)
                        sum += rlen;
                else if (0 != rlen)
                        return rlen;
                else
                        break;
        }

        return  sum;

}
#endif


int closeFile(struct file *fp)
{
    filp_close(fp,NULL);
    return 0;
}

void initKernelEnv(void)
{
    oldfs = get_fs();
    set_fs(KERNEL_DS);
}

void parseMac(char* mac, u_int8_t addr[])
{
    long b;
    int i;
    for (i = 0; i < 6; i++)
    {
        b = simple_strtol(mac+(3*i), (char **) NULL, 16);
        addr[i] = (char)b;
    }
}

static int readfile_mac(u8 *path,u8 *mac_addr) 
{
    char buf[128];
    struct file *fp=NULL;
    int ret=0;

    fp=openFile(path,O_RDONLY,0);
    if (fp!=NULL)
    {
        initKernelEnv();
        memset(buf,0,128); 
        if ((ret=readFile(fp,buf,128))>0)
        {
            parseMac(buf,(uint8_t *)mac_addr);
        }
        else
            printk("read file error %d=[%s]\n",ret,path);
        set_fs(oldfs);
        closeFile(fp);
    }
    else
        printk("Read open File fail[%s]!!!! \n",path);
    return ret;
}

static int write_mac_to_file(u8 *mac_path,u8 *mac_addr) 
{
    char buf[128];
    struct file *fp=NULL;
    int ret=0,len;
    mm_segment_t old_fs;

    fp=openFile(mac_path,O_WRONLY|O_CREAT,0640);
    if (fp!=NULL)
    {
        initKernelEnv();
        memset(buf,0,128); 
        sprintf(buf,"%x:%x:%x:%x:%x:%x",mac_addr[0],mac_addr[1],mac_addr[2],mac_addr[3],mac_addr[4],mac_addr[5]);
        len = strlen(buf)+1;

        old_fs = get_fs();
        set_fs(KERNEL_DS);
        fp->f_op->write(fp, (char *)buf, len, &fp->f_pos);
        set_fs(old_fs);

        closeFile(fp);
    }
    else
        printk("Write open File fail!!!![%s] \n",mac_path);
    return ret;
}

u16 parser_efuse(struct ssv_hw *sh, u8 *pbuf, u8 *mac_addr, u8 *new_mac_addr, struct efuse_map *efuse_tbl)
{
    u8 *rtemp8,idx=0;
	u16 shift=0,i;
    //bit as unit
    u16 efuse_real_content_len = 0;

	rtemp8 = pbuf;

    if (*rtemp8 == 0x00) {
		return efuse_real_content_len;
    }

	do
	{
		idx = (*(rtemp8) >> shift)&0xf;
		switch(idx)
		{
            //1 byte type
			case EFUSE_R_CALIBRATION_RESULT:
			case EFUSE_CRYSTAL_FREQUENCY_OFFSET:
			case EFUSE_TX_POWER_INDEX_1:
			case EFUSE_TX_POWER_INDEX_2:
			case EFUSE_SAR_RESULT:
			case EFUSE_CHIP_ID:
            case NO_USE:
            case EFUSE_VID:
            case EFUSE_PID:
            case EFUSE_RATE_TABLE_1:
            case EFUSE_RATE_TABLE_2:
				if(shift)
				{
					rtemp8 ++;
					efuse_tbl[idx].value = (u16)((u8)(*((u16*)rtemp8)) & ((1<< efuse_tbl[idx].byte_cnts) - 1));
				}
				else
				{
					efuse_tbl[idx].value = (u16)((u8)(*((u16*)rtemp8) >> 4) & ((1<< efuse_tbl[idx].byte_cnts) - 1));
				}
				efuse_real_content_len += (efuse_tbl[idx].offset + efuse_tbl[idx].byte_cnts);
                sh->efuse_bitmap |= BIT(idx);
				break;
            //MAC address
			case EFUSE_MAC:
                if(shift)
				{
					rtemp8 ++;
					memcpy(mac_addr,rtemp8,6);
				}
				else
				{
					for(i=0;i<6;i++)
					{
						mac_addr[i] = (u16)(*((u16*)rtemp8) >> 4) & 0xff;
						rtemp8++;
					}
				}
				efuse_real_content_len += (efuse_tbl[idx].offset + efuse_tbl[idx].byte_cnts);
                sh->efuse_bitmap |= BIT(idx);
				break;
			case EFUSE_MAC_NEW:
                if(shift)
				{
					rtemp8 ++;
					memcpy(new_mac_addr, rtemp8, 6);
				}
				else
				{
					for(i = 0; i < 6; i ++)
					{
						new_mac_addr[i] = (u16)(*((u16*)rtemp8) >> 4) & 0xff;
						rtemp8++;
					}
				}
				efuse_real_content_len += (efuse_tbl[idx].offset + efuse_tbl[idx].byte_cnts);
                sh->efuse_bitmap |= BIT(idx);
				break;				
			default:
                idx = 0;
				break;
		}
		shift = efuse_real_content_len % 8;
		rtemp8 = &pbuf[efuse_real_content_len / 8];
	}while(idx != 0);
    return efuse_real_content_len;
}

void addr_increase_copy(u8 *dst, u8 *src)
{
#if 0
	u16 *a = (u16 *)dst;
	const u16 *b = (const u16 *)src;

	//xx:xx:xx:xx:ff:00 -> xx:xx:xx:xx:00:01
	//xx:xx:xx:xx:ff:ff -> xx:xx:xx:xx:fe:ff
	a[0] = b[0];
	a[1] = b[1];
	if (b[2] == 0xffff)
		a[2] = b[2] - 1;
	else
		a[2] = b[2] + 1;
#endif
    u8 *a = (u8 *)dst;
    const u8 *b = (const u8 *)src;

    a[0] = b[0];
    a[1] = b[1];
    a[2] = b[2];
    a[3] = b[3];
    a[4] = b[4];
    if (b[5] & 0x1)
        a[5] = b[5] - 1;
    else
        a[5] = b[5] + 1;

}

static u8 key_char2num(u8 ch)
{
    if((ch>='0')&&(ch<='9'))
        return ch - '0';
    else if ((ch>='a')&&(ch<='f'))
        return ch - 'a' + 10;
    else if ((ch>='A')&&(ch<='F'))
        return ch - 'A' + 10;
    else
        return 0xff;
}

u8 key_2char2num(u8 hch, u8 lch)
{
    return ((key_char2num(hch) << 4) | key_char2num(lch));
}

//extern struct ssv6xxx_cfg ssv_cfg;
extern char* ssv_initmac;
#ifdef ROCKCHIP_3126_SUPPORT
extern int rockchip_wifi_mac_addr(unsigned char *buf);
#endif
void efuse_read_all_map(struct ssv_hw *sh)
{

    u8 mac[ETH_ALEN] = {0};
    int jj,kk;
    u8 efuse_mapping_table[EFUSE_HWSET_MAX_SIZE/8];
#ifndef CONFIG_SSV_RANDOM_MAC
    u8 pseudo_mac0[ETH_ALEN] = { 0x00, 0x33, 0x33, 0x33, 0x33, 0x33 };
#endif
    u8 efuse_mac[ETH_ALEN];
    u8 efuse_mac_new[ETH_ALEN];
#ifdef EFUSE_DEBUG
    int i;
#endif
    struct efuse_map ssv_efuse_item_table[] = {
        {4, 0, 0},
        {4, 8, 0},
        {4, 8, 0},
        {4, 48, 0},//Mac address old
        {4, 8, 0},
        {4, 8, 0},
        {4, 8, 0},
        {4, 4, 0},
        {4, 0, 0},
        {4, 16, 0},
        {4, 16, 0},
        {4, 48, 0}, // mac address new
        {4,  8, 0},
        {4,  8, 0},
    };

    memset(efuse_mac, 0x00, ETH_ALEN);
    memset(efuse_mac_new, 0x00, ETH_ALEN);
	memset(efuse_mapping_table,0x00,EFUSE_HWSET_MAX_SIZE/8);
    
    SSV_READ_EFUSE(sh, efuse_mapping_table);

#ifdef EFUSE_DEBUG
    for(i=0;i<(EFUSE_HWSET_MAX_SIZE/8);i++)
    {
        if(i%4 == 0)
            printk("\n");
        printk("%02x-",efuse_mapping_table[i]);
    }
    printk("\n");
#endif

    //Parser data. 
    parser_efuse(sh, efuse_mapping_table, efuse_mac, efuse_mac_new, ssv_efuse_item_table);

    // 1 byte type
    if (sh->efuse_bitmap & BIT(EFUSE_R_CALIBRATION_RESULT)) 
        sh->cfg.r_calbration_result = (u8)ssv_efuse_item_table[EFUSE_R_CALIBRATION_RESULT].value;
    
    if (sh->efuse_bitmap & BIT(EFUSE_SAR_RESULT))
        sh->cfg.sar_result = (u8)ssv_efuse_item_table[EFUSE_SAR_RESULT].value;
    
    if (sh->efuse_bitmap & BIT(EFUSE_CRYSTAL_FREQUENCY_OFFSET))
        sh->cfg.crystal_frequency_offset = (u8)ssv_efuse_item_table[EFUSE_CRYSTAL_FREQUENCY_OFFSET].value;
    
    if (sh->efuse_bitmap & BIT(EFUSE_TX_POWER_INDEX_1))  
        sh->cfg.tx_power_index_1 = (u8)ssv_efuse_item_table[EFUSE_TX_POWER_INDEX_1].value;
    
    if (sh->efuse_bitmap & BIT(EFUSE_TX_POWER_INDEX_2))  
        sh->cfg.tx_power_index_2 = (u8)ssv_efuse_item_table[EFUSE_TX_POWER_INDEX_2].value;
        
    if (sh->efuse_bitmap & BIT(EFUSE_RATE_TABLE_1))
        sh->cfg.rate_table_1 = (u8)ssv_efuse_item_table[EFUSE_RATE_TABLE_1].value;
        
    if (sh->efuse_bitmap & BIT(EFUSE_RATE_TABLE_2))  
        sh->cfg.rate_table_2 = (u8)ssv_efuse_item_table[EFUSE_RATE_TABLE_2].value;
    
    // 2 bytes type
    //sh->cfg.iq_calbration_result = (u16)ssv_efuse_item_table[EFUSE_IQ_CALIBRAION_RESULT].value;

    //Priority 1. From wif.cfg [ hw_mac & hw_mac_2 ]
    if (!is_valid_ether_addr(&sh->cfg.maddr[0][0]))
    {
#ifdef ROCKCHIP_3126_SUPPORT
            //rockchip: get mac address from flash  
        if (!rockchip_wifi_mac_addr(mac)) {
            printk("=========> get mac address from flash [%02x:%02x:%02x:%02x:%02x:%02x]\n", mac[0], mac[1],
                    mac[2], mac[3], mac[4], mac[5]);
            if(is_valid_ether_addr(mac)) {
                memcpy(&sh->cfg.maddr[0][0],mac,ETH_ALEN);
                addr_increase_copy(&sh->cfg.maddr[1][0],mac);
                goto Done;
            }
        }
#endif
        //Priority 2. From e-fuse
        if(!sh->cfg.ignore_efuse_mac)
        {
            if (is_valid_ether_addr(efuse_mac_new)) {
                printk("MAC address from e-fuse\n");
                memcpy(&sh->cfg.maddr[0][0], efuse_mac_new, ETH_ALEN);
                addr_increase_copy(&sh->cfg.maddr[1][0], efuse_mac_new);
                goto Done;
            }
        }
        //Priority 3. From insert module
        if (ssv_initmac != NULL)
        {
            for( jj = 0, kk = 0; jj < ETH_ALEN; jj++, kk += 3 ) {
                mac[jj] = key_2char2num(ssv_initmac[kk], ssv_initmac[kk+ 1]);
            }
            if(is_valid_ether_addr(mac)) {
                printk("MAC address from insert module\n");
                memcpy(&sh->cfg.maddr[0][0],mac,ETH_ALEN);
                addr_increase_copy(&sh->cfg.maddr[1][0],mac);
                goto Done;
            }
        }

        //Priority 4. From externel file path
        if (sh->cfg.mac_address_path[0] !=  0x00)
        {
            if((readfile_mac(sh->cfg.mac_address_path,&sh->cfg.maddr[0][0])) && (is_valid_ether_addr(&sh->cfg.maddr[0][0])))
            {
                printk("MAC address from sh->cfg.mac_address_path[wifi.cfg]\n");
                addr_increase_copy(&sh->cfg.maddr[1][0], &sh->cfg.maddr[0][0]);
                goto Done;
            }
        }
        //Priority 5. Software MAC mode
        //0. or others 00:33:33:33:33:33 
        //1. Always random
        //2. First random and write to file[Default path /data/wifimac]
        switch (sh->cfg.mac_address_mode) {
        case 1:
            get_random_bytes(&sh->cfg.maddr[0][0],ETH_ALEN);
            //Avoid Broadcast address & Multicast address
            sh->cfg.maddr[0][0] = sh->cfg.maddr[0][0] & 0xF0;
            addr_increase_copy(&sh->cfg.maddr[1][0], &sh->cfg.maddr[0][0]);
            break;
        case 2:
            //MAC address from /data/wifimac ...
            if((readfile_mac(sh->cfg.mac_output_path,&sh->cfg.maddr[0][0])) && (is_valid_ether_addr(&sh->cfg.maddr[0][0])))
            {
                addr_increase_copy(&sh->cfg.maddr[1][0], &sh->cfg.maddr[0][0]);
            }
            else//MAC address from E-fuse ...
            {
                {
                    get_random_bytes(&sh->cfg.maddr[0][0],ETH_ALEN);
                    //Avoid Broadcast address & Multicast address
                    sh->cfg.maddr[0][0] = sh->cfg.maddr[0][0] & 0xF0;
                    addr_increase_copy(&sh->cfg.maddr[1][0], &sh->cfg.maddr[0][0]);
                    if (sh->cfg.mac_output_path[0] != 0x00)
                        write_mac_to_file(sh->cfg.mac_output_path,&sh->cfg.maddr[0][0]);
                }
            }
            break;
        default:
            memcpy(&sh->cfg.maddr[0][0], pseudo_mac0, ETH_ALEN);
            addr_increase_copy(&sh->cfg.maddr[1][0], pseudo_mac0);
            break;
        }
        printk("MAC address from Software MAC mode[%d]\n",sh->cfg.mac_address_mode);
    }
		
Done:
    printk("EFUSE configuration\n");
    printk("Read efuse chip identity[%08x]\n", sh->cfg.chip_identity);
    printk("r_calbration_result- %x\n", sh->cfg.r_calbration_result);
    printk("sar_result- %x\n", sh->cfg.sar_result);
    printk("crystal_frequency_offset- %x\n", sh->cfg.crystal_frequency_offset);
    printk("tx_power_index_1- %x\n", sh->cfg.tx_power_index_1);
    printk("tx_power_index_2- %x\n", sh->cfg.tx_power_index_2);
	printk("MAC address - %pM\n", efuse_mac_new);
    printk("rate_table_1- %x\n", sh->cfg.rate_table_1);
    printk("rate_table_2- %x\n", sh->cfg.rate_table_2);
}
 
