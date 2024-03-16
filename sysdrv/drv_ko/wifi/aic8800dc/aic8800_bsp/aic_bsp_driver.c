/**
 ******************************************************************************
 *
 * rwnx_cmds.c
 *
 * Handles queueing (push to IPC, ack/cfm from IPC) of commands issued to
 * LMAC FW
 *
 * Copyright (C) RivieraWaves 2014-2019
 *
 ******************************************************************************
 */

#include <linux/list.h>
#include <linux/version.h>
#include <linux/delay.h>
#include <linux/vmalloc.h>
#include <linux/firmware.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 12, 0)
#include <linux/hardirq.h>
#endif
#include <linux/fs.h>
#include "aicsdio_txrxif.h"
#include "aicsdio.h"
#include "aic_bsp_driver.h"
#include "md5.h"
#include "aic8800dc_compat.h"
#include "aic8800d80_compat.h"
#include "aicwf_firmware_array.h"
#define FW_PATH_MAX 200

extern int adap_test;
extern char aic_fw_path[FW_PATH_MAX];
extern struct aic_sdio_dev *aicbsp_sdiodev;

static void cmd_dump(const struct rwnx_cmd *cmd)
{
	printk(KERN_CRIT "tkn[%d]  flags:%04x  result:%3d  cmd:%4d - reqcfm(%4d)\n",
		   cmd->tkn, cmd->flags, cmd->result, cmd->id, cmd->reqid);
}

static void cmd_complete(struct rwnx_cmd_mgr *cmd_mgr, struct rwnx_cmd *cmd)
{
	//printk("cmdcmp\n");
	lockdep_assert_held(&cmd_mgr->lock);

	list_del(&cmd->list);
	cmd_mgr->queue_sz--;

	cmd->flags |= RWNX_CMD_FLAG_DONE;
	if (cmd->flags & RWNX_CMD_FLAG_NONBLOCK) {
		kfree(cmd);
	} else {
		if (RWNX_CMD_WAIT_COMPLETE(cmd->flags)) {
			cmd->result = 0;
			complete(&cmd->complete);
		}
	}
}

static int cmd_mgr_queue(struct rwnx_cmd_mgr *cmd_mgr, struct rwnx_cmd *cmd)
{
	bool defer_push = false;
	int err = 0;

	spin_lock_bh(&cmd_mgr->lock);

	if (cmd_mgr->state == RWNX_CMD_MGR_STATE_CRASHED) {
		printk(KERN_CRIT"cmd queue crashed\n");
		cmd->result = -EPIPE;
		spin_unlock_bh(&cmd_mgr->lock);
		return -EPIPE;
	}

	if (!list_empty(&cmd_mgr->cmds)) {
		struct rwnx_cmd *last;

		if (cmd_mgr->queue_sz == cmd_mgr->max_queue_sz) {
			printk(KERN_CRIT"Too many cmds (%d) already queued\n",
				   cmd_mgr->max_queue_sz);
			cmd->result = -ENOMEM;
			spin_unlock_bh(&cmd_mgr->lock);
			return -ENOMEM;
		}
		last = list_entry(cmd_mgr->cmds.prev, struct rwnx_cmd, list);
		if (last->flags & (RWNX_CMD_FLAG_WAIT_ACK | RWNX_CMD_FLAG_WAIT_PUSH)) {
			cmd->flags |= RWNX_CMD_FLAG_WAIT_PUSH;
			defer_push = true;
		}
	}

	if (cmd->flags & RWNX_CMD_FLAG_REQ_CFM)
		cmd->flags |= RWNX_CMD_FLAG_WAIT_CFM;

	cmd->tkn    = cmd_mgr->next_tkn++;
	cmd->result = -EINTR;

	if (!(cmd->flags & RWNX_CMD_FLAG_NONBLOCK))
		init_completion(&cmd->complete);

	list_add_tail(&cmd->list, &cmd_mgr->cmds);
	cmd_mgr->queue_sz++;
	spin_unlock_bh(&cmd_mgr->lock);

	if (!defer_push) {
		//printk("queue:id=%x, param_len=%u\n", cmd->a2e_msg->id, cmd->a2e_msg->param_len);
		rwnx_set_cmd_tx((void *)(cmd_mgr->sdiodev), cmd->a2e_msg, sizeof(struct lmac_msg) + cmd->a2e_msg->param_len);
		//rwnx_ipc_msg_push(rwnx_hw, cmd, RWNX_CMD_A2EMSG_LEN(cmd->a2e_msg));
		kfree(cmd->a2e_msg);
	} else {
		//WAKE_CMD_WORK(cmd_mgr);
		printk("ERR: never defer push!!!!");
		return 0;
	}

	if (!(cmd->flags & RWNX_CMD_FLAG_NONBLOCK)) {
		unsigned long tout = msecs_to_jiffies(RWNX_80211_CMD_TIMEOUT_MS * cmd_mgr->queue_sz);
		if (!wait_for_completion_killable_timeout(&cmd->complete, tout)) {
			printk(KERN_CRIT"cmd timed-out\n");
			cmd_dump(cmd);
			spin_lock_bh(&cmd_mgr->lock);
			cmd_mgr->state = RWNX_CMD_MGR_STATE_CRASHED;
			if (!(cmd->flags & RWNX_CMD_FLAG_DONE)) {
				cmd->result = -ETIMEDOUT;
				cmd_complete(cmd_mgr, cmd);
			}
			spin_unlock_bh(&cmd_mgr->lock);
			err = -ETIMEDOUT;
		} else {
			kfree(cmd);
		}
	} else {
		cmd->result = 0;
	}

	return err;
}

static int cmd_mgr_run_callback(struct rwnx_cmd_mgr *cmd_mgr, struct rwnx_cmd *cmd,
								struct rwnx_cmd_e2amsg *msg, msg_cb_fct cb)
{
	int res;

	if (!cb) {
		return 0;
	}
	spin_lock(&cmd_mgr->cb_lock);
	res = cb(cmd, msg);
	spin_unlock(&cmd_mgr->cb_lock);

	return res;
}

static int cmd_mgr_msgind(struct rwnx_cmd_mgr *cmd_mgr, struct rwnx_cmd_e2amsg *msg,
						  msg_cb_fct cb)
{
	struct rwnx_cmd *cmd;
	bool found = false;

	//printk("cmd->id=%x\n", msg->id);
	spin_lock(&cmd_mgr->lock);
	list_for_each_entry(cmd, &cmd_mgr->cmds, list) {
		if (cmd->reqid == msg->id &&
			(cmd->flags & RWNX_CMD_FLAG_WAIT_CFM)) {

			if (!cmd_mgr_run_callback(cmd_mgr, cmd, msg, cb)) {
				found = true;
				cmd->flags &= ~RWNX_CMD_FLAG_WAIT_CFM;

				if (WARN((msg->param_len > RWNX_CMD_E2AMSG_LEN_MAX),
						 "Unexpect E2A msg len %d > %d\n", msg->param_len,
						 RWNX_CMD_E2AMSG_LEN_MAX)) {
					msg->param_len = RWNX_CMD_E2AMSG_LEN_MAX;
				}

				if (cmd->e2a_msg && msg->param_len)
					memcpy(cmd->e2a_msg, &msg->param, msg->param_len);

				if (RWNX_CMD_WAIT_COMPLETE(cmd->flags))
					cmd_complete(cmd_mgr, cmd);

				break;
			}
		}
	}
	spin_unlock(&cmd_mgr->lock);

	if (!found)
		cmd_mgr_run_callback(cmd_mgr, NULL, msg, cb);

	return 0;
}

static void cmd_mgr_print(struct rwnx_cmd_mgr *cmd_mgr)
{
	struct rwnx_cmd *cur;

	spin_lock_bh(&cmd_mgr->lock);
	list_for_each_entry(cur, &cmd_mgr->cmds, list) {
		cmd_dump(cur);
	}
	spin_unlock_bh(&cmd_mgr->lock);
}

static void cmd_mgr_drain(struct rwnx_cmd_mgr *cmd_mgr)
{
	struct rwnx_cmd *cur, *nxt;

	spin_lock_bh(&cmd_mgr->lock);
	list_for_each_entry_safe(cur, nxt, &cmd_mgr->cmds, list) {
		list_del(&cur->list);
		cmd_mgr->queue_sz--;
		if (!(cur->flags & RWNX_CMD_FLAG_NONBLOCK))
			complete(&cur->complete);
	}
	spin_unlock_bh(&cmd_mgr->lock);
}

void rwnx_cmd_mgr_init(struct rwnx_cmd_mgr *cmd_mgr)
{
	cmd_mgr->max_queue_sz = RWNX_CMD_MAX_QUEUED;
	INIT_LIST_HEAD(&cmd_mgr->cmds);
	cmd_mgr->state = RWNX_CMD_MGR_STATE_INITED;
	spin_lock_init(&cmd_mgr->lock);
	spin_lock_init(&cmd_mgr->cb_lock);
	cmd_mgr->queue  = &cmd_mgr_queue;
	cmd_mgr->print  = &cmd_mgr_print;
	cmd_mgr->drain  = &cmd_mgr_drain;
	cmd_mgr->llind  = NULL;//&cmd_mgr_llind;
	cmd_mgr->msgind = &cmd_mgr_msgind;

#if 0
	INIT_WORK(&cmd_mgr->cmdWork, cmd_mgr_task_process);
	cmd_mgr->cmd_wq = create_singlethread_workqueue("cmd_wq");
	if (!cmd_mgr->cmd_wq) {
		txrx_err("insufficient memory to create cmd workqueue.\n");
		return;
	}
#endif
}

void rwnx_cmd_mgr_deinit(struct rwnx_cmd_mgr *cmd_mgr)
{
	cmd_mgr->print(cmd_mgr);
	cmd_mgr->drain(cmd_mgr);
	cmd_mgr->print(cmd_mgr);
	memset(cmd_mgr, 0, sizeof(*cmd_mgr));
}

void rwnx_set_cmd_tx(void *dev, struct lmac_msg *msg, uint len)
{
	struct aic_sdio_dev *sdiodev = (struct aic_sdio_dev *)dev;
	struct aicwf_bus *bus = sdiodev->bus_if;
	u8 *buffer = bus->cmd_buf;
	u16 index = 0;

	memset(buffer, 0, CMD_BUF_MAX);
	buffer[0] = (len+4) & 0x00ff;
	buffer[1] = ((len+4) >> 8) &0x0f;
	buffer[2] = 0x11;
    if (sdiodev->chipid == PRODUCT_ID_AIC8801 || sdiodev->chipid == PRODUCT_ID_AIC8800DC ||
        sdiodev->chipid == PRODUCT_ID_AIC8800DW)
        buffer[3] = 0x0;
    else if (sdiodev->chipid == PRODUCT_ID_AIC8800D80)
	    buffer[3] = crc8_ponl_107(&buffer[0], 3); // crc8
	index += 4;
	//there is a dummy word
	index += 4;

	//make sure little endian
	put_u16(&buffer[index], msg->id);
	index += 2;
	put_u16(&buffer[index], msg->dest_id);
	index += 2;
	put_u16(&buffer[index], msg->src_id);
	index += 2;
	put_u16(&buffer[index], msg->param_len);
	index += 2;
	memcpy(&buffer[index], (u8 *)msg->param, msg->param_len);

	aicwf_bus_txmsg(bus, buffer, len + 8);
}

static inline void *rwnx_msg_zalloc(lmac_msg_id_t const id,
									lmac_task_id_t const dest_id,
									lmac_task_id_t const src_id,
									uint16_t const param_len)
{
	struct lmac_msg *msg;
	gfp_t flags;

	if (in_softirq())
		flags = GFP_ATOMIC;
	else
		flags = GFP_KERNEL;

	msg = (struct lmac_msg *)kzalloc(sizeof(struct lmac_msg) + param_len,
									 flags);
	if (msg == NULL) {
		printk(KERN_CRIT "%s: msg allocation failed\n", __func__);
		return NULL;
	}
	msg->id = id;
	msg->dest_id = dest_id;
	msg->src_id = src_id;
	msg->param_len = param_len;

	return msg->param;
}

static void rwnx_msg_free(struct lmac_msg *msg, const void *msg_params)
{
	kfree(msg);
}


static int rwnx_send_msg(struct aic_sdio_dev *sdiodev, const void *msg_params,
						 int reqcfm, lmac_msg_id_t reqid, void *cfm)
{
	struct lmac_msg *msg;
	struct rwnx_cmd *cmd;
	bool nonblock;
	int ret = 0;

	msg = container_of((void *)msg_params, struct lmac_msg, param);
	if (sdiodev->bus_if->state == BUS_DOWN_ST) {
		rwnx_msg_free(msg, msg_params);
		printk("bus is down\n");
		return 0;
	}

	nonblock = 0;
	cmd = kzalloc(sizeof(struct rwnx_cmd), nonblock ? GFP_ATOMIC : GFP_KERNEL);
	cmd->result  = -EINTR;
	cmd->id      = msg->id;
	cmd->reqid   = reqid;
	cmd->a2e_msg = msg;
	cmd->e2a_msg = cfm;
	if (nonblock)
		cmd->flags = RWNX_CMD_FLAG_NONBLOCK;
	if (reqcfm)
		cmd->flags |= RWNX_CMD_FLAG_REQ_CFM;

	if (reqcfm) {
		cmd->flags &= ~RWNX_CMD_FLAG_WAIT_ACK; // we don't need ack any more
		ret = sdiodev->cmd_mgr.queue(&sdiodev->cmd_mgr, cmd);
	} else {
		rwnx_set_cmd_tx((void *)(sdiodev), cmd->a2e_msg, sizeof(struct lmac_msg) + cmd->a2e_msg->param_len);
	}

	if (!reqcfm)
		kfree(cmd);

	return ret;
}


int rwnx_send_dbg_mem_block_write_req(struct aic_sdio_dev *sdiodev, u32 mem_addr,
									  u32 mem_size, u32 *mem_data)
{
	struct dbg_mem_block_write_req *mem_blk_write_req;

	/* Build the DBG_MEM_BLOCK_WRITE_REQ message */
	mem_blk_write_req = rwnx_msg_zalloc(DBG_MEM_BLOCK_WRITE_REQ, TASK_DBG, DRV_TASK_ID,
										sizeof(struct dbg_mem_block_write_req));
	if (!mem_blk_write_req)
		return -ENOMEM;

	/* Set parameters for the DBG_MEM_BLOCK_WRITE_REQ message */
	mem_blk_write_req->memaddr = mem_addr;
	mem_blk_write_req->memsize = mem_size;
	memcpy(mem_blk_write_req->memdata, mem_data, mem_size);

	/* Send the DBG_MEM_BLOCK_WRITE_REQ message to LMAC FW */
	return rwnx_send_msg(sdiodev, mem_blk_write_req, 1, DBG_MEM_BLOCK_WRITE_CFM, NULL);
}

int rwnx_send_dbg_mem_read_req(struct aic_sdio_dev *sdiodev, u32 mem_addr,
							   struct dbg_mem_read_cfm *cfm)
{
	struct dbg_mem_read_req *mem_read_req;

	/* Build the DBG_MEM_READ_REQ message */
	mem_read_req = rwnx_msg_zalloc(DBG_MEM_READ_REQ, TASK_DBG, DRV_TASK_ID,
							   sizeof(struct dbg_mem_read_req));
	if (!mem_read_req)
		return -ENOMEM;

	/* Set parameters for the DBG_MEM_READ_REQ message */
	mem_read_req->memaddr = mem_addr;

	/* Send the DBG_MEM_READ_REQ message to LMAC FW */
	return rwnx_send_msg(sdiodev, mem_read_req, 1, DBG_MEM_READ_CFM, cfm);
}


int rwnx_send_dbg_mem_write_req(struct aic_sdio_dev *sdiodev, u32 mem_addr, u32 mem_data)
{
	struct dbg_mem_write_req *mem_write_req;

	/* Build the DBG_MEM_WRITE_REQ message */
	mem_write_req = rwnx_msg_zalloc(DBG_MEM_WRITE_REQ, TASK_DBG, DRV_TASK_ID,
									sizeof(struct dbg_mem_write_req));
	if (!mem_write_req)
		return -ENOMEM;

	/* Set parameters for the DBG_MEM_WRITE_REQ message */
	mem_write_req->memaddr = mem_addr;
	mem_write_req->memdata = mem_data;

	/* Send the DBG_MEM_WRITE_REQ message to LMAC FW */
	return rwnx_send_msg(sdiodev, mem_write_req, 1, DBG_MEM_WRITE_CFM, NULL);
}

int rwnx_send_dbg_mem_mask_write_req(struct aic_sdio_dev *sdiodev, u32 mem_addr,
									 u32 mem_mask, u32 mem_data)
{
	struct dbg_mem_mask_write_req *mem_mask_write_req;

	/* Build the DBG_MEM_MASK_WRITE_REQ message */
	mem_mask_write_req = rwnx_msg_zalloc(DBG_MEM_MASK_WRITE_REQ, TASK_DBG, DRV_TASK_ID,
										 sizeof(struct dbg_mem_mask_write_req));
	if (!mem_mask_write_req)
		return -ENOMEM;

	/* Set parameters for the DBG_MEM_MASK_WRITE_REQ message */
	mem_mask_write_req->memaddr = mem_addr;
	mem_mask_write_req->memmask = mem_mask;
	mem_mask_write_req->memdata = mem_data;

	/* Send the DBG_MEM_MASK_WRITE_REQ message to LMAC FW */
	return rwnx_send_msg(sdiodev, mem_mask_write_req, 1, DBG_MEM_MASK_WRITE_CFM, NULL);
}


int rwnx_send_dbg_start_app_req(struct aic_sdio_dev *sdiodev, u32 boot_addr, u32 boot_type, struct dbg_start_app_cfm *start_app_cfm)
{
	struct dbg_start_app_req *start_app_req;

	/* Build the DBG_START_APP_REQ message */
	start_app_req = rwnx_msg_zalloc(DBG_START_APP_REQ, TASK_DBG, DRV_TASK_ID,
									sizeof(struct dbg_start_app_req));
	if (!start_app_req) {
		printk("start app nomen\n");
		return -ENOMEM;
	}

	/* Set parameters for the DBG_START_APP_REQ message */
	start_app_req->bootaddr = boot_addr;
	start_app_req->boottype = boot_type;

	/* Send the DBG_START_APP_REQ message to LMAC FW */
	return rwnx_send_msg(sdiodev, start_app_req, 1, DBG_START_APP_CFM, start_app_cfm);
}

static msg_cb_fct dbg_hdlrs[MSG_I(DBG_MAX)] = {
};

static msg_cb_fct *msg_hdlrs[] = {
	[TASK_DBG] = dbg_hdlrs,
};

void rwnx_rx_handle_msg(struct aic_sdio_dev *sdiodev, struct ipc_e2a_msg *msg)
{
	sdiodev->cmd_mgr.msgind(&sdiodev->cmd_mgr, msg,
							msg_hdlrs[MSG_T(msg->id)][MSG_I(msg->id)]);
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 4, 0)
MODULE_IMPORT_NS(VFS_internal_I_am_really_a_filesystem_and_am_NOT_a_driver);
#endif

#define MD5(x) x[0],x[1],x[2],x[3],x[4],x[5],x[6],x[7],x[8],x[9],x[10],x[11],x[12],x[13],x[14],x[15]
#define MD5PINRT "file md5:%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\r\n"

int rwnx_load_firmware(u32 **fw_buf, const char *name, struct device *device)
{

#ifdef CONFIG_USE_FW_REQUEST
	const struct firmware *fw = NULL;
	u32 *dst = NULL;
	void *buffer=NULL;
	MD5_CTX md5;
	unsigned char decrypt[16];
	int size = 0;
	int ret = 0;

	printk("%s: request firmware = %s \n", __func__ ,name);


	ret = request_firmware(&fw, name, NULL);

	if (ret < 0) {
		printk("Load %s fail\n", name);
		release_firmware(fw);
		return -1;
	}

	size = fw->size;
	dst = (u32 *)fw->data;

	if (size <= 0) {
		printk("wrong size of firmware file\n");
		release_firmware(fw);
		return -1;
	}


	buffer = vmalloc(size);
	memset(buffer, 0, size);
	memcpy(buffer, dst, size);

	*fw_buf = buffer;

	MD5Init(&md5);
	MD5Update(&md5, (unsigned char *)buffer, size);
	MD5Final(&md5, decrypt);
	printk(MD5PINRT, MD5(decrypt));

	release_firmware(fw);

	return size;
#else
	void *buffer = NULL;
	char *path = NULL;
	struct file *fp = NULL;
	int size = 0, len = 0;// i = 0;
	ssize_t rdlen = 0;
	//u32 *src = NULL, *dst = NULL;
	MD5_CTX md5;
	unsigned char decrypt[16];

	#ifdef CONFIG_FIRMWARE_ARRAY
		size = aicwf_get_firmware_array((char*)name, fw_buf);
		printk("%s size:%d \r\n", __func__, size);
		MD5Init(&md5);
		MD5Update(&md5, (unsigned char *)*fw_buf, size);
		MD5Final(&md5, decrypt);
		printk(MD5PINRT, MD5(decrypt));

		return size;
	#endif

	/* get the firmware path */
	path = __getname();
	if (!path) {
		*fw_buf = NULL;
		return -1;
	}

    if(strlen(aic_fw_path) > 0){
        len = snprintf(path, AICBSP_FW_PATH_MAX, "%s/%s", aic_fw_path, name);
    }else{
	    len = snprintf(path, AICBSP_FW_PATH_MAX, "%s/%s", AICBSP_FW_PATH, name);
    }
	if (len >= AICBSP_FW_PATH_MAX) {
		printk("%s: %s file's path too long\n", __func__, name);
		*fw_buf = NULL;
		__putname(path);
		return -1;
	}

	printk("%s :firmware path = %s  \n", __func__, path);

	/* open the firmware file */
	fp = filp_open(path, O_RDONLY, 0);
	if (IS_ERR_OR_NULL(fp)) {
		printk("%s: %s file failed to open\n", __func__, name);
		*fw_buf = NULL;
		__putname(path);
		fp = NULL;
		return -1;
	}

	size = i_size_read(file_inode(fp));
	if (size <= 0) {
		printk("%s: %s file size invalid %d\n", __func__, name, size);
		*fw_buf = NULL;
		__putname(path);
		filp_close(fp, NULL);
		fp = NULL;
		return -1;
	}

	/* start to read from firmware file */
	buffer = vmalloc(size);

	if (!buffer) {
		*fw_buf = NULL;
		__putname(path);
		filp_close(fp, NULL);
		fp = NULL;
		return -1;
	}else{
		memset(buffer, 0, size);
	}

#if LINUX_VERSION_CODE > KERNEL_VERSION(4, 13, 16)
	rdlen = kernel_read(fp, buffer, size, &fp->f_pos);
#else
	rdlen = kernel_read(fp, fp->f_pos, buffer, size);
#endif

	if (size != rdlen) {
		printk("%s: %s file rdlen invalid %ld\n", __func__, name, (long int)rdlen);
		*fw_buf = NULL;
		__putname(path);
		filp_close(fp, NULL);
		fp = NULL;
		vfree(buffer);
		buffer = NULL;
		return -1;
	}
	if (rdlen > 0) {
		fp->f_pos += rdlen;
	}

#if 0
	/*start to transform the data format*/
	src = (u32 *)buffer;
	dst = (u32 *)vmalloc(size);

	if (!dst) {
		*fw_buf = NULL;
		__putname(path);
		filp_close(fp, NULL);
		fp = NULL;
		vfree(buffer);
		buffer = NULL;
		return -1;
	}else{
		memset(dst, 0, size);
	}

	for (i = 0; i < (size/4); i++) {
		dst[i] = src[i];
	}
#endif

	__putname(path);
	filp_close(fp, NULL);
	fp = NULL;
	//vfree(buffer);
	//buffer = NULL;
	*fw_buf = (u32*)buffer;

	MD5Init(&md5);
	MD5Update(&md5, (unsigned char *)buffer, size);
	MD5Final(&md5, decrypt);

	printk(MD5PINRT, MD5(decrypt));

	return size;
#endif
}

extern int testmode;

#ifdef CONFIG_M2D_OTA_AUTO_SUPPORT
extern char saved_sdk_ver[64];

int rwnx_plat_m2d_flash_ota_android(struct aic_sdio_dev *sdiodev, char *filename)
{
	struct device *dev = sdiodev->dev;
	unsigned int i=0;
	int size;
	u32 *dst=NULL;
	int err=0;
	int ret;
	u8 bond_id;
	const u32 mem_addr = 0x40500000;
	struct dbg_mem_read_cfm rd_mem_addr_cfm;

	ret = rwnx_send_dbg_mem_read_req(sdiodev, mem_addr, &rd_mem_addr_cfm);
	if (ret) {
		printk("m2d %x rd fail: %d\n", mem_addr, ret);
		return ret;
	}
	bond_id = (u8)(rd_mem_addr_cfm.memdata >> 24);
	printk("%x=%x\n", rd_mem_addr_cfm.memaddr, rd_mem_addr_cfm.memdata);
	if (bond_id & (1<<1)) {
		//flash is invalid
		printk("m2d flash is invalid\n");
		return -1;
	}

	/* load aic firmware */
	size = rwnx_load_firmware(&dst, filename, dev);
	if(size<=0){
			printk("wrong size of m2d file\n");
			vfree(dst);
			dst = NULL;
			return -1;
	}

	/* Copy the file on the Embedded side */
	printk("### Upload m2d %s flash, size=%d\n", filename, size);

	/*send info first*/
	err = rwnx_send_dbg_mem_block_write_req(sdiodev, AIC_M2D_OTA_INFO_ADDR, 4, (u32 *)&size);

	/*send data first*/
	if (size > 1024) {// > 1KB data
		for (i = 0; i < (size - 1024); i += 1024) {//each time write 1KB
			err = rwnx_send_dbg_mem_block_write_req(sdiodev, AIC_M2D_OTA_DATA_ADDR, 1024, dst + i / 4);
				if (err) {
				printk("m2d upload fail: %x, err:%d\r\n", AIC_M2D_OTA_DATA_ADDR, err);
				break;
			}
		}
	}

	if (!err && (i < size)) {// <1KB data
		err = rwnx_send_dbg_mem_block_write_req(sdiodev, AIC_M2D_OTA_DATA_ADDR, size - i, dst + i / 4);
		if (err) {
			printk("m2d upload fail: %x, err:%d\r\n", AIC_M2D_OTA_DATA_ADDR, err);
		}
	}

	if (dst) {
		vfree(dst);
		dst = NULL;
	}
	testmode = FW_NORMAL_MODE;
	aicbsp_info.cpmode = testmode;

	printk("m2d flash update complete\n\n");

	return err;
}

int rwnx_plat_m2d_flash_ota_check(struct aic_sdio_dev *sdiodev, char *filename)
{
	struct device *dev = sdiodev->dev;
	unsigned int i=0,j=0;
	int size;
	u32 *dst=NULL;
	int err=0;
	int ret=0;
	u8 bond_id;
	const u32 mem_addr = 0x40500000;
	const u32 mem_addr_code_start = AIC_M2D_OTA_CODE_START_ADDR;
	const u32 mem_addr_sdk_ver = AIC_M2D_OTA_VER_ADDR;
	const u32 driver_code_start_idx = (AIC_M2D_OTA_CODE_START_ADDR-AIC_M2D_OTA_FLASH_ADDR)/4;
	const u32 driver_sdk_ver_idx = (AIC_M2D_OTA_VER_ADDR-AIC_M2D_OTA_FLASH_ADDR)/4;
	u32 driver_sdk_ver_addr_idx = 0;
	u32 code_start_addr = 0xffffffff;
	u32 sdk_ver_addr = 0xffffffff;
	u32 drv_code_start_addr = 0xffffffff;
	u32 drv_sdk_ver_addr = 0xffffffff;
	struct dbg_mem_read_cfm rd_mem_addr_cfm;
	char m2d_sdk_ver[64];
	char flash_sdk_ver[64];
	u32 flash_ver[16];
	u32 ota_ver[16];

	ret = rwnx_send_dbg_mem_read_req(sdiodev, mem_addr, &rd_mem_addr_cfm);
	if (ret) {
		printk("m2d %x rd fail: %d\n", mem_addr, ret);
		return ret;
	}
	bond_id = (u8)(rd_mem_addr_cfm.memdata >> 24);
	printk("%x=%x\n", rd_mem_addr_cfm.memaddr, rd_mem_addr_cfm.memdata);
	if (bond_id & (1<<1)) {
		//flash is invalid
		printk("m2d flash is invalid\n");
		return -1;
	}
	ret = rwnx_send_dbg_mem_read_req(sdiodev, mem_addr_code_start, &rd_mem_addr_cfm);
	if (ret){
		printk("mem_addr_code_start %x rd fail: %d\n", mem_addr_code_start, ret);
		return ret;
	}
	code_start_addr = rd_mem_addr_cfm.memdata;

	#if !defined(CONFIG_M2D_OTA_LZMA_SUPPORT)
	ret = rwnx_send_dbg_mem_read_req(sdiodev, mem_addr_sdk_ver, &rd_mem_addr_cfm);
	if (ret){
		printk("mem_addr_sdk_ver %x rd fail: %d\n", mem_addr_code_start, ret);
		return ret;
	}
	sdk_ver_addr = rd_mem_addr_cfm.memdata;
	#else
	sdk_ver_addr = mem_addr_sdk_ver;
	#endif
	printk("code_start_addr: 0x%x,	sdk_ver_addr: 0x%x\n", code_start_addr,sdk_ver_addr);

	/* load aic firmware */
	size = rwnx_load_firmware(&dst, filename, dev);
	if(size<=0){
			printk("wrong size of m2d file\n");
			vfree(dst);
			dst = NULL;
			return -1;
	}
	if(code_start_addr == 0xffffffff && sdk_ver_addr == 0xffffffff) {
		printk("########m2d flash old version , must be upgrade\n");
		drv_code_start_addr = dst[driver_code_start_idx];
		drv_sdk_ver_addr = dst[driver_sdk_ver_idx];

		printk("drv_code_start_addr: 0x%x,	drv_sdk_ver_addr: 0x%x\n", drv_code_start_addr,drv_sdk_ver_addr);

		if(drv_sdk_ver_addr == 0xffffffff){
			printk("########driver m2d_ota.bin is old ,not need upgrade\n");
			return -1;
		}

	} else {
		for(i=0;i<16;i++){
			ret = rwnx_send_dbg_mem_read_req(sdiodev, (sdk_ver_addr+i*4), &rd_mem_addr_cfm);
			if (ret){
				printk("mem_addr_sdk_ver %x rd fail: %d\n", mem_addr_code_start, ret);
				return ret;
			}
			flash_ver[i] = rd_mem_addr_cfm.memdata;
		}
		memcpy((u8 *)flash_sdk_ver,(u8 *)flash_ver,64);
		memcpy((u8 *)saved_sdk_ver,(u8 *)flash_sdk_ver,64);
		printk("flash SDK Version: %s\r\n\r\n", flash_sdk_ver);

		drv_code_start_addr = dst[driver_code_start_idx];
		drv_sdk_ver_addr = dst[driver_sdk_ver_idx];

		printk("drv_code_start_addr: 0x%x,	drv_sdk_ver_addr: 0x%x\n", drv_code_start_addr,drv_sdk_ver_addr);

		if(drv_sdk_ver_addr == 0xffffffff){
			printk("########driver m2d_ota.bin is old ,not need upgrade\n");
			return -1;
		}

		#if !defined(CONFIG_M2D_OTA_LZMA_SUPPORT)
		driver_sdk_ver_addr_idx = (drv_sdk_ver_addr-drv_code_start_addr)/4;
		#else
		driver_sdk_ver_addr_idx = driver_sdk_ver_idx;
		#endif
		printk("driver_sdk_ver_addr_idx %d\n",driver_sdk_ver_addr_idx);

		if (driver_sdk_ver_addr_idx){
			for(j = 0; j < 16; j++){
				ota_ver[j] = dst[driver_sdk_ver_addr_idx+j];
			}
			memcpy((u8 *)m2d_sdk_ver,(u8 *)ota_ver,64);
			printk("m2d_ota SDK Version: %s\r\n\r\n", m2d_sdk_ver);
		} else {
			return -1;
		}

		if(!strcmp(m2d_sdk_ver,flash_sdk_ver)){
			printk("######## m2d %s flash is not need upgrade\r\n", filename);
			return -1;
		}
	}

	/* Copy the file on the Embedded side */
	printk("### Upload m2d %s flash, size=%d\n", filename, size);

	/*send info first*/
	err = rwnx_send_dbg_mem_block_write_req(sdiodev, AIC_M2D_OTA_INFO_ADDR, 4, (u32 *)&size);

	/*send data first*/
	if (size > 1024) {// > 1KB data
		for (i = 0; i < (size - 1024); i += 1024) {//each time write 1KB
			err = rwnx_send_dbg_mem_block_write_req(sdiodev, AIC_M2D_OTA_DATA_ADDR, 1024, dst + i / 4);
				if (err) {
				printk("m2d upload fail: %x, err:%d\r\n", AIC_M2D_OTA_DATA_ADDR, err);
				break;
			}
		}
	}

	if (!err && (i < size)) {// <1KB data
		err = rwnx_send_dbg_mem_block_write_req(sdiodev, AIC_M2D_OTA_DATA_ADDR, size - i, dst + i / 4);
		if (err) {
			printk("m2d upload fail: %x, err:%d\r\n", AIC_M2D_OTA_DATA_ADDR, err);
		}
	}

	if (dst) {
		vfree(dst);
		dst = NULL;
	}
	testmode = FW_NORMAL_MODE;

	printk("m2d flash update complete\n\n");

	return err;
}
#endif//CONFIG_M2D_OTA_AUTO_SUPPORT

int aicwf_patch_table_load(struct aic_sdio_dev *rwnx_hw, char *filename)
{
	struct device *dev = rwnx_hw->dev;
    int err = 0;
    unsigned int i = 0, size;
   	u32 *dst = NULL;
	u8 *describle;
	u32 fmacfw_patch_tbl_8800dc_u02_describe_size = 124;
	u32 fmacfw_patch_tbl_8800dc_u02_describe_base;//read from patch_tbl

    /* Copy the file on the Embedded side */
    printk("### Upload %s \n", filename);

    size = rwnx_load_firmware(&dst, filename,dev);
    if (!dst) {
       printk("No such file or directory\n");
       return -1;
    }
    if (size <= 0) {
            printk("wrong size of firmware file\n");
            dst = NULL;
            err = -1;
    }

	printk("tbl size = %d \n",size);

	fmacfw_patch_tbl_8800dc_u02_describe_base = dst[0];
	AICWFDBG(LOGINFO, "FMACFW_PATCH_TBL_8800DC_U02_DESCRIBE_BASE = %x \n",fmacfw_patch_tbl_8800dc_u02_describe_base);

	if (!err && (i < size)) {
		err=rwnx_send_dbg_mem_block_write_req(rwnx_hw, fmacfw_patch_tbl_8800dc_u02_describe_base, fmacfw_patch_tbl_8800dc_u02_describe_size + 4, dst);
		if(err){
			printk("write describe information fail \n");
		}

		describle=kzalloc(fmacfw_patch_tbl_8800dc_u02_describe_size,GFP_KERNEL);
		memcpy(describle,&dst[1],fmacfw_patch_tbl_8800dc_u02_describe_size);
		printk("%s",describle);
		kfree(describle);
		describle=NULL;
	}

    if (!err && (i < size)) {
        for (i =(128/4); i < (size/4); i +=2) {
            printk("patch_tbl:  %x  %x\n", dst[i], dst[i+1]);
            err = rwnx_send_dbg_mem_write_req(rwnx_hw, dst[i], dst[i+1]);
        }
        if (err) {
            printk("bin upload fail: %x, err:%d\r\n", dst[i], err);
        }
    }

	if (dst) {
#ifndef CONFIG_FIRMWARE_ARRAY
			vfree(dst);
#endif
			dst = NULL;
	}


   return err;

}

extern char aic_fw_path[200];
int aicwf_plat_patch_load_8800dc(struct aic_sdio_dev *sdiodev)
{
    int ret = 0;
    #if !defined(CONFIG_FPGA_VERIFICATION)
    if (chip_sub_id == 0) {
        printk("u01 is loaing ###############\n");
        ret = rwnx_plat_bin_fw_upload_android(sdiodev, ROM_FMAC_PATCH_ADDR, RWNX_MAC_PATCH_NAME2_8800DC);
    } else if (chip_sub_id == 1) {
        printk("u02 is loaing ###############\n");
        ret = rwnx_plat_bin_fw_upload_android(sdiodev, ROM_FMAC_PATCH_ADDR, RWNX_MAC_PATCH_NAME2_8800DC_U02);
    } else if (chip_sub_id == 2) {
        printk("h_u02 is loaing ###############\n");
        ret = rwnx_plat_bin_fw_upload_android(sdiodev, ROM_FMAC_PATCH_ADDR, RWNX_MAC_PATCH_NAME2_8800DC_H_U02);
    } else {
        printk("unsupported id: %d\n", chip_sub_id);
    }
    #endif
    return ret;
}

int aicwf_plat_rftest_load_8800dc(struct aic_sdio_dev *sdiodev)
{
    int ret = 0;
    ret = rwnx_plat_bin_fw_upload_android(sdiodev, RAM_LMAC_FW_ADDR, RWNX_MAC_FW_RF_BASE_NAME_8800DC);
    if (ret) {
        AICWFDBG(LOGINFO, "load rftest bin fail: %d\n", ret);
        return ret;
    }
    return ret;
}

#ifdef CONFIG_DPD
int aicwf_misc_ram_valid_check_8800dc(struct aic_sdio_dev *sdiodev, int *valid_out)
{
    int ret = 0;
    uint32_t cfg_base = 0x10164;
    struct dbg_mem_read_cfm cfm;
    uint32_t misc_ram_addr;
    uint32_t ram_base_addr, ram_word_cnt;
    uint32_t bit_mask[4];
    int i;
    if (valid_out) {
        *valid_out = 0;
    }
    if (testmode == FW_RFTEST_MODE) {
		
	    uint32_t vect1 = 0;
	    uint32_t vect2 = 0;
	    cfg_base = RAM_LMAC_FW_ADDR + 0x0004;
	    ret = rwnx_send_dbg_mem_read_req(sdiodev, cfg_base, &cfm);
	    if (ret) {
		    AICWFDBG(LOGERROR, "cfg_base:%x vcet1 rd fail: %d\n", cfg_base, ret);
		    return ret;
	    }
	    vect1 = cfm.memdata;
	    if ((vect1 & 0xFFFF0000) != (RAM_LMAC_FW_ADDR & 0xFFFF0000)) {
		    AICWFDBG(LOGERROR, "vect1 invalid: %x\n", vect1);
		    return ret;
	    }
	    cfg_base = RAM_LMAC_FW_ADDR + 0x0008;
	    ret = rwnx_send_dbg_mem_read_req(sdiodev, cfg_base, &cfm);
	    if (ret) {
		    AICWFDBG(LOGERROR, "cfg_base:%x vcet2 rd fail: %d\n", cfg_base, ret);
		    return ret;
	    }
	    vect2 = cfm.memdata;
	    if ((vect2 & 0xFFFF0000) != (RAM_LMAC_FW_ADDR & 0xFFFF0000)) {
		    AICWFDBG(LOGERROR, "vect2 invalid: %x\n", vect2);
		    return ret;
	    }

        cfg_base = RAM_LMAC_FW_ADDR + 0x0164;
    }
    // init misc ram
    ret = rwnx_send_dbg_mem_read_req(sdiodev, cfg_base + 0x14, &cfm);
    if (ret) {
        AICWFDBG(LOGERROR, "rf misc ram[0x%x] rd fail: %d\n", cfg_base + 0x14, ret);
        return ret;
    }
    misc_ram_addr = cfm.memdata;
    AICWFDBG(LOGERROR, "misc_ram_addr=%x\n", misc_ram_addr);
    // bit_mask
    ram_base_addr = misc_ram_addr + offsetof(rf_misc_ram_t, bit_mask);
    ram_word_cnt = (MEMBER_SIZE(rf_misc_ram_t, bit_mask) + MEMBER_SIZE(rf_misc_ram_t, reserved)) / 4;
    for (i = 0; i < ram_word_cnt; i++) {
        ret = rwnx_send_dbg_mem_read_req(sdiodev, ram_base_addr + i * 4, &cfm);
        if (ret) {
            AICWFDBG(LOGERROR, "bit_mask[0x%x] rd fail: %d\n",  ram_base_addr + i * 4, ret);
            return ret;
        }
        bit_mask[i] = cfm.memdata;
    }
    AICWFDBG(LOGTRACE, "bit_mask:%x,%x,%x,%x\n",bit_mask[0],bit_mask[1],bit_mask[2],bit_mask[3]);
    if ((bit_mask[0] == 0) && ((bit_mask[1] & 0xFFF00000) == 0x80000000) &&
        (bit_mask[2] == 0) && ((bit_mask[3] & 0xFFFFFF00) == 0x00000000)) {
        if (valid_out) {
            *valid_out = 1;
        }
    }
    return ret;
}

int aicwf_plat_calib_load_8800dc(struct aic_sdio_dev *sdiodev)
{
    int ret = 0;
    if (chip_sub_id == 1) {
        ret = rwnx_plat_bin_fw_upload_android(sdiodev, ROM_FMAC_CALIB_ADDR, RWNX_MAC_CALIB_NAME_8800DC_U02);
        if (ret) {
            AICWFDBG(LOGINFO, "load rftest bin fail: %d\n", ret);
            return ret;
        }
    } else if (chip_sub_id == 2) {
        ret = rwnx_plat_bin_fw_upload_android(sdiodev, ROM_FMAC_CALIB_ADDR, RWNX_MAC_CALIB_NAME_8800DC_H_U02);
        if (ret) {
            AICWFDBG(LOGINFO, "load rftest bin fail: %d\n", ret);
            return ret;
        }
    }
    return ret;
}

#ifndef CONFIG_FORCE_DPD_CALIB
int is_file_exist(char* name)
{
    char *path = NULL;
    struct file *fp = NULL;
    int len;

    path = __getname();
    if (!path) {
        AICWFDBG(LOGINFO, "%s getname fail\n", __func__);
        return -1;
    }

    len = snprintf(path, FW_PATH_MAX_LEN, "%s/%s", AICBSP_FW_PATH, name);

    fp = filp_open(path, O_RDONLY, 0);
    if (IS_ERR(fp)) {
        __putname(path);
        fp = NULL;
        return 0;
    } else {
        __putname(path);
        filp_close(fp, NULL);
		fp = NULL;
        return 1;
    }
}

EXPORT_SYMBOL(is_file_exist);
#endif
#endif

#ifdef CONFIG_DPD
rf_misc_ram_lite_t dpd_res = {{0},};
EXPORT_SYMBOL(dpd_res);
#endif

static int rwnx_plat_patch_load(struct aic_sdio_dev *sdiodev)
{
	int ret = 0;

	RWNX_DBG(RWNX_FN_ENTRY_STR);

    if (sdiodev->chipid == PRODUCT_ID_AIC8800DC || sdiodev->chipid == PRODUCT_ID_AIC8800DW) {
        AICWFDBG(LOGINFO, "testmode=%d\n", testmode);
        if (chip_sub_id == 0) {
            if (testmode == FW_NORMAL_MODE) {
                AICWFDBG(LOGINFO, "rwnx_plat_patch_loading\n");
                ret = aicwf_plat_patch_load_8800dc(sdiodev);
                if (ret) {
                    AICWFDBG(LOGINFO, "load patch bin fail: %d\n", ret);
                    return ret;
                }
            } else if (testmode == FW_RFTEST_MODE) {
                ret = aicwf_plat_rftest_load_8800dc(sdiodev);
                if (ret) {
                    AICWFDBG(LOGINFO, "load rftest bin fail: %d\n", ret);
                    return ret;
                }
            }
        } else if (chip_sub_id >= 1) {
            if (testmode == FW_NORMAL_MODE) {
                AICWFDBG(LOGINFO, "rwnx_plat_patch_loading\n");
                ret = aicwf_plat_patch_load_8800dc(sdiodev);
                if (ret) {
                    AICWFDBG(LOGINFO, "load patch bin fail: %d\n", ret);
                    return ret;
                }
                #ifdef CONFIG_DPD
                #ifdef CONFIG_FORCE_DPD_CALIB
                if (1) {
                    AICWFDBG(LOGINFO, "dpd calib & write\n");
                    ret = aicwf_dpd_calib_8800dc(sdiodev, &dpd_res);
                    if (ret) {
                        AICWFDBG(LOGINFO, "dpd calib fail: %d\n", ret);
                        return ret;
                    }
                }
                #else
                if (is_file_exist(FW_DPDRESULT_NAME_8800DC) == 1) {
                    AICWFDBG(LOGINFO, "dpd bin load\n");
                    ret = aicwf_dpd_result_load_8800dc(sdiodev, &dpd_res);
                    if (ret) {
                        AICWFDBG(LOGINFO, "load dpd bin fail: %d\n", ret);
                        return ret;
                    }
					ret = aicwf_dpd_result_apply_8800dc(sdiodev, &dpd_res);
					if (ret) {
						AICWFDBG(LOGINFO, "apply dpd bin fail: %d\n", ret);
						return ret;
					}
                }
                #endif
                else
                #endif
                {
                    ret = aicwf_misc_ram_init_8800dc(sdiodev);
                    if (ret) {
                        AICWFDBG(LOGINFO, "misc ram init fail: %d\n", ret);
                        return ret;
                    }
                }
            } else if (testmode == FW_RFTEST_MODE) {
                #ifdef CONFIG_DPD
                #ifdef CONFIG_FORCE_DPD_CALIB
                if (1) {
                    AICWFDBG(LOGINFO, "patch load\n");
                    ret = aicwf_plat_patch_load_8800dc(sdiodev);
                    if (ret) {
                        AICWFDBG(LOGINFO, "load patch bin fail: %d\n", ret);
                        return ret;
                    }
                    AICWFDBG(LOGINFO, "dpd calib & write\n");
                    ret = aicwf_dpd_calib_8800dc(sdiodev, &dpd_res);
                    if (ret) {
                        AICWFDBG(LOGINFO, "dpd calib fail: %d\n", ret);
                        return ret;
                    }
                }
                #endif
                #endif
                ret = aicwf_plat_rftest_load_8800dc(sdiodev);
                if (ret) {
                    AICWFDBG(LOGINFO, "load rftest bin fail: %d\n", ret);
                    return ret;
                }
            } else if (testmode == FW_DPDCALIB_MODE) {
                #if (defined(CONFIG_DPD) && !defined(CONFIG_FORCE_DPD_CALIB))
                if (is_file_exist(FW_DPDRESULT_NAME_8800DC) == 0) {
                    AICWFDBG(LOGINFO, "patch load\n");
                    ret = aicwf_plat_patch_load_8800dc(sdiodev);
                    if (ret) {
                        AICWFDBG(LOGINFO, "load patch bin fail: %d\n", ret);
                        return ret;
                    }
                    AICWFDBG(LOGINFO, "dpd calib & write\n");
                    ret = aicwf_dpd_calib_8800dc(sdiodev, &dpd_res);
                    if (ret) {
                        AICWFDBG(LOGINFO, "dpd calib fail: %d\n", ret);
                        return ret;
                    }
                    ret = aicwf_dpd_result_write_8800dc((void *)&dpd_res, DPD_RESULT_SIZE_8800DC);
                    if (ret) {
                        AICWFDBG(LOGINFO, "file write fail: %d\n", ret);
                        return ret;
                    }
                }
                #endif
                return 1; // exit calib mode
            }
        }
    }

    return ret;
}

int rwnx_plat_bin_fw_upload_android(struct aic_sdio_dev *sdiodev, u32 fw_addr,
							   const char *filename)
{
	struct device *dev = sdiodev->dev;
	unsigned int i = 0;
	int size;
	u32 *dst = NULL;
	int err = 0;

	printk("%s\n",__func__);

	/* load aic firmware */
	size = rwnx_load_firmware(&dst, filename, dev);
	if (size <= 0) {
		printk("wrong size of firmware file\n");
#ifndef CONFIG_FIRMWARE_ARRAY
		vfree(dst);
#endif
		dst = NULL;
		return -1;
	}

	/* Copy the file on the Embedded side */
	if (size > 1024) {// > 1KB data
		for (i = 0; i < (size - 1024); i += 1024) {//each time write 1KB
			err = rwnx_send_dbg_mem_block_write_req(sdiodev, fw_addr + i, 1024, dst + i / 4);
			if (err) {
				printk("bin upload fail: %x, err:%d\r\n", fw_addr + i, err);
				break;
			}
		}
	}

	if (!err && (i < size)) {// <1KB data
		err = rwnx_send_dbg_mem_block_write_req(sdiodev, fw_addr + i, size - i, dst + i / 4);
		if (err) {
			printk("bin upload fail: %x, err:%d\r\n", fw_addr + i, err);
		}
	}

	if (dst) {
#ifndef CONFIG_FIRMWARE_ARRAY
		vfree(dst);
#endif
		dst = NULL;
	}

	return err;
}

int aicbt_patch_table_free(struct aicbt_patch_table **head)
{
	struct aicbt_patch_table *p = *head, *n = NULL;
	while (p) {
		n = p->next;
		vfree(p->name);
		vfree(p->data);
		vfree(p);
		p = n;
	}
	*head = NULL;
	return 0;
}

struct aicbt_patch_table *aicbt_patch_table_alloc(const char *filename)
{
	uint8_t *rawdata = NULL, *p;
	int size;
	struct aicbt_patch_table *head = NULL, *new = NULL, *cur = NULL;

	/* load aic firmware */
	size = rwnx_load_firmware((u32 **)&rawdata, filename, NULL);
	if (size <= 0) {
		printk("wrong size of firmware file\n");
		goto err;
	}

	p = rawdata;
	if (memcmp(p, AICBT_PT_TAG, sizeof(AICBT_PT_TAG) < 16 ? sizeof(AICBT_PT_TAG) : 16)) {
		printk("TAG err\n");
		goto err;
	}
	p += 16;

	while (p - rawdata < size) {
		new = (struct aicbt_patch_table *)vmalloc(sizeof(struct aicbt_patch_table));
		memset(new, 0, sizeof(struct aicbt_patch_table));
		if (head == NULL) {
			head = new;
			cur  = new;
		} else {
			cur->next = new;
			cur = cur->next;
		}

		cur->name = (char *)vmalloc(sizeof(char) * 16);
		memset(cur->name, 0, sizeof(char) * 16);
		memcpy(cur->name, p, 16);
		p += 16;

		cur->type = *(uint32_t *)p;
		p += 4;

		cur->len = *(uint32_t *)p;
		p += 4;

		if((cur->type )  >= 1000 ) {//Temp Workaround
			cur->len = 0;
		}else{
			if(cur->len > 0){
				cur->data = (uint32_t *)vmalloc(sizeof(uint8_t) * cur->len * 8);
				memset(cur->data, 0, sizeof(uint8_t) * cur->len * 8);
				memcpy(cur->data, p, cur->len * 8);
				p += cur->len * 8;
			}
		}
	}
#ifndef CONFIG_FIRMWARE_ARRAY
	vfree(rawdata);
#endif
	return head;

err:
	aicbt_patch_table_free(&head);
	if (rawdata)
		vfree(rawdata);
	return NULL;
}
int aicbt_patch_info_unpack(struct aicbt_patch_info_t *patch_info, struct aicbt_patch_table *head_t)
{
    if (AICBT_PT_INF == head_t->type) {
        patch_info->info_len = head_t->len;
        if(patch_info->info_len == 0)
            return 0;
        memcpy(&patch_info->adid_addrinf, head_t->data, patch_info->info_len * sizeof(uint32_t) * 2);
    }
    return 0;
}
int aicbt_patch_trap_data_load(struct aic_sdio_dev *sdiodev, struct aicbt_patch_table *head)
{
	struct aicbt_patch_info_t patch_info = {
		.info_len          = 0,
		.adid_addrinf      = 0,
		.addr_adid         = 0,
		.patch_addrinf     = 0,
		.addr_patch        = 0,
		.reset_addr        = 0,
        .reset_val         = 0,
        .adid_flag_addr    = 0,
        .adid_flag         = 0,
	};
    if(head == NULL){
        return -1;
    }

	if(sdiodev->chipid == PRODUCT_ID_AIC8801){
		patch_info.addr_adid  = FW_RAM_ADID_BASE_ADDR;
		patch_info.addr_patch = FW_RAM_PATCH_BASE_ADDR;
	}
	else if(sdiodev->chipid == PRODUCT_ID_AIC8800DC){
		if(aicbsp_info.chip_rev == CHIP_REV_U01){
			patch_info.addr_adid = RAM_8800DC_U01_ADID_ADDR;
		}else if(aicbsp_info.chip_rev == CHIP_REV_U02){
			patch_info.addr_adid = RAM_8800DC_U02_ADID_ADDR;
		}
		patch_info.addr_patch = RAM_8800DC_FW_PATCH_ADDR;
        aicbt_patch_info_unpack(&patch_info, head);
        if(patch_info.reset_addr == 0) {
            patch_info.reset_addr        = FW_RESET_START_ADDR;
            patch_info.reset_val         = FW_RESET_START_VAL;
            patch_info.adid_flag_addr    = FW_ADID_FLAG_ADDR;
            patch_info.adid_flag         = FW_ADID_FLAG_VAL;
            if (rwnx_send_dbg_mem_write_req(sdiodev, patch_info.reset_addr, patch_info.reset_val))
                return -1;
            if (rwnx_send_dbg_mem_write_req(sdiodev, patch_info.adid_flag_addr, patch_info.adid_flag))
                return -1;
        }
	} else if(sdiodev->chipid == PRODUCT_ID_AIC8800D80){
        if (aicbsp_info.chip_rev == CHIP_REV_U01) {
		    patch_info.addr_adid = FW_RAM_ADID_BASE_ADDR_8800D80;
		    patch_info.addr_patch = FW_RAM_PATCH_BASE_ADDR_8800D80;
        } else if (aicbsp_info.chip_rev == CHIP_REV_U02 || aicbsp_info.chip_rev == CHIP_REV_U03) {
            patch_info.addr_adid = FW_RAM_ADID_BASE_ADDR_8800D80_U02;
		    patch_info.addr_patch = FW_RAM_PATCH_BASE_ADDR_8800D80_U02;
        }
        aicbt_patch_info_unpack(&patch_info, head);
        if(patch_info.info_len == 0) {
            printk("%s, aicbt_patch_info_unpack fail\n", __func__);
            return -1;
        }
	}

	if (rwnx_plat_bin_fw_upload_android(sdiodev, patch_info.addr_adid, aicbsp_firmware_list[aicbsp_info.cpmode].bt_adid))
		return -1;
	if (rwnx_plat_bin_fw_upload_android(sdiodev, patch_info.addr_patch, aicbsp_firmware_list[aicbsp_info.cpmode].bt_patch))
		return -1;
	return 0;

}

static struct aicbt_info_t aicbt_info[]={
    {
        .btmode        = AICBT_BTMODE_DEFAULT,
        .btport        = AICBT_BTPORT_DEFAULT,
        .uart_baud     = AICBT_UART_BAUD_DEFAULT,
        .uart_flowctrl = AICBT_UART_FC_DEFAULT,
        .lpm_enable    = AICBT_LPM_ENABLE_DEFAULT,
        .txpwr_lvl     = AICBT_TXPWR_LVL_DEFAULT,
    },//PRODUCT_ID_AIC8801
    {
        .btmode        = AICBT_BTMODE_BT_WIFI_COMBO,
        .btport        = AICBT_BTPORT_DEFAULT,
        .uart_baud     = AICBT_UART_BAUD_DEFAULT,
        .uart_flowctrl = AICBT_UART_FC_DEFAULT,
        .lpm_enable    = AICBT_LPM_ENABLE_DEFAULT,
        .txpwr_lvl     = AICBT_TXPWR_LVL_DEFAULT_8800dc,
    },//PRODUCT_ID_AIC8800DC
    {
        .btmode        = AICBT_BTMODE_BT_WIFI_COMBO,
        .btport        = AICBT_BTPORT_DEFAULT,
        .uart_baud     = AICBT_UART_BAUD_DEFAULT,
        .uart_flowctrl = AICBT_UART_FC_DEFAULT,
        .lpm_enable    = AICBT_LPM_ENABLE_DEFAULT,
        .txpwr_lvl     = AICBT_TXPWR_LVL_DEFAULT_8800dc,
    },//PRODUCT_ID_AIC8800DW
    {
        .btmode        = AICBT_BTMODE_DEFAULT_8800d80,
        .btport        = AICBT_BTPORT_DEFAULT,
        .uart_baud     = AICBT_UART_BAUD_DEFAULT,
        .uart_flowctrl = AICBT_UART_FC_DEFAULT,
        .lpm_enable    = AICBT_LPM_ENABLE_DEFAULT,
        .txpwr_lvl     = AICBT_TXPWR_LVL_DEFAULT_8800d80,
    }//PRODUCT_ID_AIC8800D80
};


int aicbt_patch_table_load(struct aic_sdio_dev *sdiodev, struct aicbt_patch_table *head)
{
	struct aicbt_patch_table *p;
	int ret = 0, i;
	uint32_t *data = NULL;
    if(head == NULL){
        return -1;
    }

    for (p = head; p != NULL; p = p->next) {
    	data = p->data;
    	if (AICBT_PT_BTMODE == p->type) {
    		*(data + 1)  = aicbsp_info.hwinfo < 0;
    		*(data + 3)  = aicbsp_info.hwinfo;
    		*(data + 5)  = (sdiodev->chipid == PRODUCT_ID_AIC8800DC?aicbsp_info.cpmode:0);//0;//aicbsp_info.cpmode;

    		*(data + 7)  = aicbt_info[sdiodev->chipid].btmode;
    		*(data + 9)  = aicbt_info[sdiodev->chipid].btport;
    		*(data + 11) = aicbt_info[sdiodev->chipid].uart_baud;
    		*(data + 13) = aicbt_info[sdiodev->chipid].uart_flowctrl;
    		*(data + 15) = aicbt_info[sdiodev->chipid].lpm_enable;
    		*(data + 17) = aicbt_info[sdiodev->chipid].txpwr_lvl;

            printk("%s bt btmode[%d]:%d \r\n", __func__, sdiodev->chipid, aicbt_info[sdiodev->chipid].btmode);
    		printk("%s bt uart_baud[%d]:%d \r\n", __func__, sdiodev->chipid, aicbt_info[sdiodev->chipid].uart_baud);
    		printk("%s bt uart_flowctrl[%d]:%d \r\n", __func__, sdiodev->chipid, aicbt_info[sdiodev->chipid].uart_flowctrl);
    		printk("%s bt lpm_enable[%d]:%d \r\n", __func__, sdiodev->chipid, aicbt_info[sdiodev->chipid].lpm_enable);
    		printk("%s bt tx_pwr[%d]:%d \r\n", __func__, sdiodev->chipid, aicbt_info[sdiodev->chipid].txpwr_lvl);
    	}

    	if (AICBT_PT_VER == p->type) {
    		printk("aicbsp: bt patch version: %s\n", (char *)p->data);
    		continue;
    	}

    	for (i = 0; i < p->len; i++) {
    		ret = rwnx_send_dbg_mem_write_req(sdiodev, *data, *(data + 1));
    		if (ret != 0)
    			return ret;
    		data += 2;
    	}
    	if (p->type == AICBT_PT_PWRON)
    		udelay(500);
    }


	///aicbt_patch_table_free(&head);
	return 0;
}


int aicbt_init(struct aic_sdio_dev *sdiodev)
{
    int ret = 0;
    struct aicbt_patch_table *head = aicbt_patch_table_alloc(aicbsp_firmware_list[aicbsp_info.cpmode].bt_table);
	if (head == NULL){
        printk("aicbt_patch_table_alloc fail\n");
        return -1;
    }

    if (aicbt_patch_trap_data_load(sdiodev, head)) {
		printk("aicbt_patch_trap_data_load fail\n");
        ret = -1;
		goto err;
	}

	if (aicbt_patch_table_load(sdiodev, head)) {
		 printk("aicbt_patch_table_load fail\n");
        ret = -1;
		goto err;
	}

err:
	aicbt_patch_table_free(&head);
	return ret;
}

static int aicwifi_start_from_bootrom(struct aic_sdio_dev *sdiodev)
{
	int ret = 0;

	/* memory access */
	const u32 fw_addr = RAM_FMAC_FW_ADDR;
	struct dbg_start_app_cfm start_app_cfm;

	/* fw start */
	ret = rwnx_send_dbg_start_app_req(sdiodev, fw_addr, HOST_START_APP_AUTO, &start_app_cfm);
	if (ret) {
		return -1;
	}
	aicbsp_info.hwinfo_r = start_app_cfm.bootstatus & 0xFF;

	return 0;
}

static int start_from_bootrom_8800DC(struct aic_sdio_dev *sdiodev)
{
    int ret = 0;
	u32 rd_addr;
	u32 fw_addr;
	u32 boot_type;
	struct dbg_mem_read_cfm rd_cfm;

	/* memory access */
	if(testmode == 1){
		rd_addr = RAM_LMAC_FW_ADDR;
		fw_addr = RAM_LMAC_FW_ADDR;
	}
	else{
		rd_addr = RAM_FMAC_FW_ADDR;
		fw_addr = RAM_FMAC_FW_ADDR;
	}

	AICWFDBG(LOGINFO, "Read FW mem: %08x\n", rd_addr);
	if ((ret = rwnx_send_dbg_mem_read_req(sdiodev, rd_addr, &rd_cfm))) {
		return -1;
	}
	AICWFDBG(LOGINFO, "cfm: [%08x] = %08x\n", rd_cfm.memaddr, rd_cfm.memdata);

    if (testmode == 0) {
        boot_type = HOST_START_APP_DUMMY;
    } else {
        boot_type = HOST_START_APP_AUTO;
    }
	/* fw start */
	AICWFDBG(LOGINFO, "Start app: %08x, %d\n", fw_addr, boot_type);
	if ((ret = rwnx_send_dbg_start_app_req(sdiodev, fw_addr, boot_type ,NULL))) {
		return -1;
	}
	return 0;
}

u32 adaptivity_patch_tbl[][2] = {
	{0x0004, 0x0000320A}, //linkloss_thd
    {0x0094, 0x00000000}, //ac_param_conf
	{0x00F8, 0x00010138}, //tx_adaptivity_en
};

u32 patch_tbl[][2] = {
#if !defined(CONFIG_LINK_DET_5G)
    {0x0104, 0x00000000}, //link_det_5g
#endif
#if defined(CONFIG_MCU_MESSAGE)
    {0x004c, 0x0000004B}, //pkt_cnt_1724=0x4B
    {0x0050, 0x0011FC00}, //ipc_base_addr
#endif
};

u32 syscfg_tbl_masked[][3] = {
	{0x40506024, 0x000000FF, 0x000000DF}, // for clk gate lp_level
};

u32 rf_tbl_masked[][3] = {
	{0x40344058, 0x00800000, 0x00000000},// pll trx
};

static int aicwifi_sys_config(struct aic_sdio_dev *sdiodev)
{
	int ret, cnt;
	int syscfg_num = sizeof(syscfg_tbl_masked) / sizeof(u32) / 3;
	for (cnt = 0; cnt < syscfg_num; cnt++) {
		ret = rwnx_send_dbg_mem_mask_write_req(sdiodev,
			syscfg_tbl_masked[cnt][0], syscfg_tbl_masked[cnt][1], syscfg_tbl_masked[cnt][2]);
		if (ret) {
			printk("%x mask write fail: %d\n", syscfg_tbl_masked[cnt][0], ret);
			return ret;
		}
	}

	ret = rwnx_send_dbg_mem_mask_write_req(sdiodev,
				rf_tbl_masked[0][0], rf_tbl_masked[0][1], rf_tbl_masked[0][2]);
	if (ret) {
		printk("rf config %x write fail: %d\n", rf_tbl_masked[0][0], ret);
		return ret;
	}

	return 0;
}

static int aicwifi_patch_config(struct aic_sdio_dev *sdiodev)
{
	const u32 rd_patch_addr = RAM_FMAC_FW_ADDR + 0x0180;
	u32 config_base;
	uint32_t start_addr = 0x1e6000;
	u32 patch_addr = start_addr;
	u32 patch_num = sizeof(patch_tbl)/4;
	struct dbg_mem_read_cfm rd_patch_addr_cfm;
	u32 patch_addr_reg = 0x1e5318;
	u32 patch_num_reg = 0x1e531c;
	int ret = 0;
	u16 cnt = 0;
	int tmp_cnt = 0;
	int adap_patch_num = 0;

	if (aicbsp_info.cpmode == AICBSP_CPMODE_TEST) {
		patch_addr_reg = 0x1e5304;
		patch_num_reg = 0x1e5308;
	}

	ret = rwnx_send_dbg_mem_read_req(sdiodev, rd_patch_addr, &rd_patch_addr_cfm);
	if (ret) {
		printk("patch rd fail\n");
		return ret;
	}

	config_base = rd_patch_addr_cfm.memdata;

	ret = rwnx_send_dbg_mem_write_req(sdiodev, patch_addr_reg, patch_addr);
	if (ret) {
		printk("0x%x write fail\n", patch_addr_reg);
		return ret;
	}

	if(adap_test){
		printk("%s for adaptivity test \r\n", __func__);
		adap_patch_num = sizeof(adaptivity_patch_tbl)/4;
		ret = rwnx_send_dbg_mem_write_req(sdiodev, patch_num_reg, patch_num + adap_patch_num);
	}else{
		ret = rwnx_send_dbg_mem_write_req(sdiodev, patch_num_reg, patch_num);
	}
	if (ret) {
		printk("0x%x write fail\n", patch_num_reg);
		return ret;
	}

	for (cnt = 0; cnt < patch_num/2; cnt += 1) {
		ret = rwnx_send_dbg_mem_write_req(sdiodev, start_addr+8*cnt, patch_tbl[cnt][0]+config_base);
		if (ret) {
			printk("%x write fail\n", start_addr+8*cnt);
			return ret;
		}

		ret = rwnx_send_dbg_mem_write_req(sdiodev, start_addr+8*cnt+4, patch_tbl[cnt][1]);
		if (ret) {
			printk("%x write fail\n", start_addr+8*cnt+4);
			return ret;
		}
	}

	tmp_cnt = cnt;

	if(adap_test){
		for(cnt = 0; cnt < adap_patch_num/2; cnt+=1)
		{
			if((ret = rwnx_send_dbg_mem_write_req(sdiodev, start_addr+8*(cnt+tmp_cnt), adaptivity_patch_tbl[cnt][0]+config_base))) {
				printk("%x write fail\n", start_addr+8*cnt);
			}

			if((ret = rwnx_send_dbg_mem_write_req(sdiodev, start_addr+8*(cnt+tmp_cnt)+4, adaptivity_patch_tbl[cnt][1]))) {
				printk("%x write fail\n", start_addr+8*cnt+4);
			}
		}
	}

	return 0;
}

int aicwifi_init(struct aic_sdio_dev *sdiodev)
{
	int ret = 0;
	if(sdiodev->chipid == PRODUCT_ID_AIC8801){
		#ifdef CONFIG_M2D_OTA_AUTO_SUPPORT
		if (testmode == FW_M2D_OTA_MODE) {
			rwnx_plat_m2d_flash_ota_android(sdiodev, FW_M2D_OTA_NAME);
		} else if (testmode == FW_NORMAL_MODE) {
			rwnx_plat_m2d_flash_ota_check(sdiodev, FW_M2D_OTA_NAME);
		}
		#endif
		if (rwnx_plat_bin_fw_upload_android(sdiodev, RAM_FMAC_FW_ADDR, aicbsp_firmware_list[aicbsp_info.cpmode].wl_fw)) {
			printk("download wifi fw fail\n");
			return -1;
		}

		if (aicwifi_patch_config(sdiodev)) {
			printk("aicwifi_patch_config fail\n");
			return -1;
		}

		if (aicwifi_sys_config(sdiodev)) {
			printk("aicwifi_sys_config fail\n");
			return -1;
		}

		if (aicwifi_start_from_bootrom(sdiodev)) {
			printk("wifi start fail\n");
			return -1;
		}
    }else if (sdiodev->chipid == PRODUCT_ID_AIC8800DC || sdiodev->chipid == PRODUCT_ID_AIC8800DW){
		printk("############ aicwifi_init begin \n");

		system_config_8800dc(sdiodev);
		printk("############ system_config_8800dc done\n");

		ret = rwnx_plat_patch_load(sdiodev);
		if (ret) {
			printk("patch load return %d\n", ret);
			return ret;
		}
		printk("############ rwnx_plat_patch_load done\n");

		//rwnx_plat_userconfig_load(sdiodev);

		aicwf_patch_config_8800dc(sdiodev);
		printk("############ aicwf_patch_config_8800dc done\n");

		start_from_bootrom_8800DC(sdiodev);
	}else if(sdiodev->chipid == PRODUCT_ID_AIC8800D80){
		if (rwnx_plat_bin_fw_upload_android(sdiodev, RAM_FMAC_FW_ADDR, aicbsp_firmware_list[aicbsp_info.cpmode].wl_fw)) {
			printk("8800d80 download wifi fw fail\n");
			return -1;
		}

		if (aicwifi_patch_config_8800d80(sdiodev)) {
			printk("aicwifi_patch_config_8800d80 fail\n");
			return -1;
		}

		if (aicwifi_sys_config_8800d80(sdiodev)) {
			printk("aicwifi_patch_config_8800d80 fail\n");
			return -1;
		}

		if (aicwifi_start_from_bootrom(sdiodev)) {
			printk("8800d80 wifi start fail\n");
			return -1;
		}
	}

#ifdef CONFIG_GPIO_WAKEUP
	if (aicwf_sdio_writeb(sdiodev, sdiodev->sdio_reg.wakeup_reg, 4)) {
		sdio_err("reg:%d write failed!\n", sdiodev->sdio_reg.wakeup_reg);
		return -1;
	}
#endif
	return 0;
}

u32 aicbsp_syscfg_tbl[][2] = {
	{0x40500014, 0x00000101}, // 1)
	{0x40500018, 0x00000109}, // 2)
	{0x40500004, 0x00000010}, // 3) the order should not be changed

	// def CONFIG_PMIC_SETTING
	// U02 bootrom only
	{0x40040000, 0x00001AC8}, // 1) fix panic
	{0x40040084, 0x00011580},
	{0x40040080, 0x00000001},
	{0x40100058, 0x00000000},

	{0x50000000, 0x03220204}, // 2) pmic interface init
	{0x50019150, 0x00000002}, // 3) for 26m xtal, set div1
	{0x50017008, 0x00000000}, // 4) stop wdg
};

static int aicbsp_system_config(struct aic_sdio_dev *sdiodev)
{
	int syscfg_num = sizeof(aicbsp_syscfg_tbl) / sizeof(u32) / 2;
	int ret, cnt;
	for (cnt = 0; cnt < syscfg_num; cnt++) {
		ret = rwnx_send_dbg_mem_write_req(sdiodev, aicbsp_syscfg_tbl[cnt][0], aicbsp_syscfg_tbl[cnt][1]);
		if (ret) {
			sdio_err("%x write fail: %d\n", aicbsp_syscfg_tbl[cnt][0], ret);
			return ret;
		}
	}
	return 0;
}



int aicbsp_platform_init(struct aic_sdio_dev *sdiodev)
{
	rwnx_cmd_mgr_init(&sdiodev->cmd_mgr);
	sdiodev->cmd_mgr.sdiodev = (void *)sdiodev;

	return 0;
}

void aicbsp_platform_deinit(struct aic_sdio_dev *sdiodev)
{
	(void)sdiodev;
}

int aicbsp_driver_fw_init(struct aic_sdio_dev *sdiodev)
{
	u32 mem_addr;
	struct dbg_mem_read_cfm rd_mem_addr_cfm;
	u32 btenable = 0;
	u8 is_chip_id_h = 0;
	int ret = 0;

	mem_addr = 0x40500000;

	testmode = aicbsp_info.cpmode;

	if(sdiodev->chipid == PRODUCT_ID_AIC8801){

		if (rwnx_send_dbg_mem_read_req(sdiodev, mem_addr, &rd_mem_addr_cfm))
			return -1;

		aicbsp_info.chip_rev = (u8)(rd_mem_addr_cfm.memdata >> 16);
		btenable = 1;

		if (aicbsp_info.chip_rev != CHIP_REV_U02 &&
			aicbsp_info.chip_rev != CHIP_REV_U03 &&
			aicbsp_info.chip_rev != CHIP_REV_U04) {
			pr_err("aicbsp: %s, unsupport chip rev: %d\n", __func__, aicbsp_info.chip_rev);
			return -1;
		}

		if (aicbsp_info.chip_rev != CHIP_REV_U02)
			aicbsp_firmware_list = fw_u03;

		if (aicbsp_system_config(sdiodev))
			return -1;
	}
	else if (sdiodev->chipid == PRODUCT_ID_AIC8800DC || sdiodev->chipid == PRODUCT_ID_AIC8800DW){
		if (rwnx_send_dbg_mem_read_req(sdiodev, mem_addr, &rd_mem_addr_cfm))
			return -1;

		aicbsp_info.chip_rev = (u8)((rd_mem_addr_cfm.memdata >> 16) & 0x3F);
		is_chip_id_h = (u8)(((rd_mem_addr_cfm.memdata >> 16) & 0xC0) == 0xC0);

		btenable = ((rd_mem_addr_cfm.memdata >> 26) & 0x1);
		AICWFDBG(LOGINFO, "btenable = %d \n",btenable);

		if(btenable == 0){
			sdiodev->chipid = PRODUCT_ID_AIC8800DW;
			AICWFDBG(LOGINFO, "AIC8800DC change to AIC8800DW \n");
		}

		if (aicbsp_info.chip_rev != CHIP_REV_U01 &&
			aicbsp_info.chip_rev != CHIP_REV_U02 &&
			aicbsp_info.chip_rev != CHIP_REV_U03 &&
			aicbsp_info.chip_rev != CHIP_REV_U04) {
			pr_err("aicbsp: %s, unsupport chip rev: %d\n", __func__, aicbsp_info.chip_rev);
			return -1;
		}
		if (is_chip_id_h) {
			AICWFDBG(LOGINFO, "IS_CHIP_ID_H \n");
			aicbsp_firmware_list = fw_8800dc_h_u02;
		} else {
			if(aicbsp_info.chip_rev == CHIP_REV_U01){
				aicbsp_firmware_list = fw_8800dc_u01;
			}else{
				aicbsp_firmware_list = fw_8800dc_u02;
			}
		}
	}
    else if(sdiodev->chipid == PRODUCT_ID_AIC8800D80){

		if (rwnx_send_dbg_mem_read_req(sdiodev, mem_addr, &rd_mem_addr_cfm))
			return -1;

		aicbsp_info.chip_rev = (u8)(rd_mem_addr_cfm.memdata >> 16);
		btenable = 1;

		if (aicbsp_info.chip_rev == CHIP_REV_U01)
            aicbsp_firmware_list = fw_8800d80_u01;
        if (aicbsp_info.chip_rev == CHIP_REV_U02 || aicbsp_info.chip_rev == CHIP_REV_U03)
            aicbsp_firmware_list = fw_8800d80_u02;
        if (aicbsp_system_config_8800d80(sdiodev))
            return -1;
	}

	AICWFDBG(LOGINFO, "aicbsp: %s, chip rev: %d\n", __func__, aicbsp_info.chip_rev);

	#ifndef CONFIG_MCU_MESSAGE
	if (testmode != 4) {
		if(btenable == 1){
			if (aicbt_init(sdiodev))
				return -1;
		}
	}
	#endif

	ret = aicwifi_init(sdiodev);
	if (ret)
		return ret;

	return 0;
}

int aicbsp_get_feature(struct aicbsp_feature_t *feature, char *fw_path)
{
	if (aicbsp_sdiodev->chipid == PRODUCT_ID_AIC8801 ||
        aicbsp_sdiodev->chipid == PRODUCT_ID_AIC8800DC ||
        aicbsp_sdiodev->chipid == PRODUCT_ID_AIC8800DW){
	    feature->sdio_clock = FEATURE_SDIO_CLOCK;
	}else if (aicbsp_sdiodev->chipid == PRODUCT_ID_AIC8800D80){
        feature->sdio_clock = FEATURE_SDIO_CLOCK_V3;
	}
	feature->sdio_phase = FEATURE_SDIO_PHASE;
	feature->hwinfo     = aicbsp_info.hwinfo;
	feature->fwlog_en   = aicbsp_info.fwlog_en;
	feature->irqf       = aicbsp_info.irqf;
	if(fw_path != NULL){
		sprintf(fw_path,"%s", AICBSP_FW_PATH);
	}
    sdio_dbg("%s, set FEATURE_SDIO_CLOCK %d MHz\n", __func__, feature->sdio_clock/1000000);
	return 0;
}
EXPORT_SYMBOL_GPL(aicbsp_get_feature);

#ifdef CONFIG_RESV_MEM_SUPPORT
static struct skb_buff_pool resv_skb[] = {
	{AIC_RESV_MEM_TXDATA, 1536*64, "resv_mem_txdata", 0, NULL},
};

int aicbsp_resv_mem_init(void)
{
    int i = 0;
	printk("%s \n",__func__);
    for (i = 0; i < sizeof(resv_skb) / sizeof(resv_skb[0]); i++) {
            resv_skb[i].skb = dev_alloc_skb(resv_skb[i].size);
    }
    return 0;
}

int aicbsp_resv_mem_deinit(void)
{
    int i = 0;
	printk("%s \n",__func__);
    for (i = 0; i < sizeof(resv_skb) / sizeof(resv_skb[0]); i++) {
            if (resv_skb[i].used == 0 && resv_skb[i].skb)
                    dev_kfree_skb(resv_skb[i].skb);
    }
    return 0;
}

struct sk_buff *aicbsp_resv_mem_alloc_skb(unsigned int length, uint32_t id)
{
    if (resv_skb[id].size < length) {
            pr_err("aicbsp: %s, no enough mem\n", __func__);
            goto fail;
    }

    if (resv_skb[id].used) {
            pr_err("aicbsp: %s, mem in use\n", __func__);
            goto fail;
    }

    if (resv_skb[id].skb == NULL) {
            pr_err("aicbsp: %s, mem not initialazed\n", __func__);
            resv_skb[id].skb = dev_alloc_skb(resv_skb[id].size);
            if (resv_skb[id].skb == NULL) {
                    pr_err("aicbsp: %s, mem reinitial still fail\n", __func__);
                    goto fail;
            }
    }

    printk("aicbsp: %s, alloc %s succuss, id: %d, size: %d\n", __func__,
                    resv_skb[id].name, resv_skb[id].id, resv_skb[id].size);

    resv_skb[id].used = 1;
	return resv_skb[id].skb;

fail:
    return NULL;
}
EXPORT_SYMBOL_GPL(aicbsp_resv_mem_alloc_skb);

void aicbsp_resv_mem_kfree_skb(struct sk_buff *skb, uint32_t id)
{
	resv_skb[id].used = 0;
	printk("aicbsp: %s, free %s succuss, id: %d, size: %d\n", __func__,
                    resv_skb[id].name, resv_skb[id].id, resv_skb[id].size);
}
EXPORT_SYMBOL_GPL(aicbsp_resv_mem_kfree_skb);

#else

int aicbsp_resv_mem_init(void)
{
	return 0;
}

int aicbsp_resv_mem_deinit(void)
{
	return 0;
}

#endif
