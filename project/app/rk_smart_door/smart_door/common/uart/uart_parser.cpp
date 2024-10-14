/*
 *  Copyright (c) 2021 Rockchip Electronics Co. Ltd.
 *
 */

#include "uart_parser.h"
#include "message_mcu.h"
#include "mcu_inf.h"
#include "md5.h"
#include "aes.h"

static unsigned char g_cmd_enc_mode;
static unsigned char g_cmd_aes_key_sel[16];
static unsigned char g_cmd_aes_key[16];

static short cmd_uart_msg_size;
static short cmd_uart_data_len_cnt;
static unsigned short sync_code;
static unsigned short head_size_cnt;
struct cmd_uart_msg_buf_t g_cmd_uart_msg_buf;
enum cmd_uart_state g_cmd_uart_state = CMD_UART_STATE_WAIT;

typedef int (*uart_send_callback)(unsigned char str);
static uart_send_callback rk_uart_sendbyte_cb = NULL;

#define CMD_SYNC_CODE 0xEFAA

#define CMD_UART_BUF_LEN 256

struct cmd_uart_sender{
	unsigned char send_buf[CMD_UART_BUF_LEN];
	unsigned short data_size;
	unsigned short send_len;
};
static struct cmd_uart_sender g_cmd_uart_sender;

void rk_uart_sendbyte(uint8_t str)
{
	if (rk_uart_sendbyte_cb)
		rk_uart_sendbyte_cb(str);
}

void uart_sender_reset(void)
{
	struct cmd_uart_sender *s = &g_cmd_uart_sender;
	s->data_size = 0;
	s->send_len = 0;
	memset(s->send_buf,0,CMD_UART_BUF_LEN);
}

void uart_sender_set_sync(void)
{
	struct cmd_uart_sender *s = &g_cmd_uart_sender;
	s->send_buf[0] = 0xEF;
	s->send_buf[1] = 0xAA;
}

void uart_sender_set_id(unsigned char id)
{
	struct cmd_uart_sender *s = &g_cmd_uart_sender;
	s->send_buf[2] = id;
}

int uart_sender_set_data(unsigned char *data, unsigned short size)
{
	struct cmd_uart_sender *s = &g_cmd_uart_sender;
	if (size > CMD_UART_BUF_LEN - 8){
		return -1;
	}

	s->data_size = size;

	s->send_buf[3] = (size >> 8) & 0xff;
	s->send_buf[4] = size & 0xff;

	if(size > 0){
		memcpy(&s->send_buf[5], data, size);
	}
	return 0;
}

int uart_sender_set_reply_data(unsigned char result, unsigned char mid,unsigned char *data, unsigned short size)
{
	struct cmd_uart_sender *s = &g_cmd_uart_sender;
	if (size > CMD_UART_BUF_LEN - 8){
		return -1;
	}
	s->data_size = size + 2;
	s->send_buf[3] = (s->data_size >> 8) & 0xff;
	s->send_buf[4] = s->data_size & 0xff;

	s->send_buf[5] = mid;
	s->send_buf[6] = result;
	if(size > 0){
		memcpy(&s->send_buf[7], data, size);
	}
	return 0;
}

int uart_sender_set_note_data(unsigned char nid,unsigned char *data, unsigned short size)
{
	struct cmd_uart_sender *s = &g_cmd_uart_sender;
	if (size > CMD_UART_BUF_LEN - 8){
		return -1;
	}
	s->data_size = size + 1;
	s->send_buf[3] = (s->data_size >> 8) & 0xff;
	s->send_buf[4] = s->data_size & 0xff;

	s->send_buf[5] = nid;
	if(size > 0){
		memcpy(&s->send_buf[6], data, size);
	}
	return 0;
}

void uart_sender_set_checksum(void)
{
	int i;
	unsigned char checksum = 0;
	struct cmd_uart_sender *s = &g_cmd_uart_sender;

	for(i=2;i<s->data_size+5;i++){
		checksum ^= s->send_buf[i];
	}

	s->send_buf[i] = checksum;
}

int uart_sender_set_message(unsigned char id,unsigned char *data, unsigned short size)
{
	struct cmd_uart_sender *s = &g_cmd_uart_sender;
	uart_sender_set_sync();
	uart_sender_set_id(id);
	uart_sender_set_data(data, size);
	uart_sender_set_checksum();

	s->send_len = s->data_size + 6;
	return 0;
}

int uart_sender_set_reply(unsigned char result, unsigned char mid,unsigned char *data, unsigned short size)
{
	struct cmd_uart_sender *s = &g_cmd_uart_sender;
	uart_sender_set_sync();
	uart_sender_set_id(MID_REPLY);
	uart_sender_set_reply_data(result, mid, data, size);
	uart_sender_set_checksum();

	s->send_len = s->data_size + 6;
	return 0;
}

int uart_sender_set_note(unsigned char nid,unsigned char *data, unsigned short size)
{
	struct cmd_uart_sender *s = &g_cmd_uart_sender;
	uart_sender_set_sync();
	uart_sender_set_id(MID_NOTE);
	uart_sender_set_note_data(nid, data, size);
	uart_sender_set_checksum();

	s->send_len = s->data_size + 6;
	return 0;
}

static int aes_encrypt(void *key, void *in, int size, void *out)
{
	aes_context ctx;
	uint8_t *src = (uint8_t *)in;

	int len = (size + 15)/16 * 16;
	int blkcnt = len / 16;

	for(int i=size; i< blkcnt * 16; i++)
		src[i] = 0;

	if (aes_set_key(&ctx, (uint8_t*)key, 128) != RETSUCCESS)
		printf("aes_set_key error\n");

	for(int n=0; n < blkcnt; n++) {
		if(aes_encrypt_block(&ctx, (uint8_t*)out + 16 * n, src + 16 * n) != RETSUCCESS)
			printf("aes_encrypt_block error\n");
	}
	return len;
}

static int aes_decrypt(void *key, void *in, int size, void *out)
{
	aes_context ctx;
	uint8_t *src = (uint8_t *)in;

	int len = (size + 15)/16 * 16;
	int blkcnt = len / 16;

	for(int i=size; i< blkcnt * 16; i++)
	   src[i] = 0;

	if (aes_set_key(&ctx, (uint8_t*)key, 128) != RETSUCCESS)
		printf("aes_set_key error.");

	for(int n=0; n < blkcnt; n++) {
	   if(aes_decrypt_block(&ctx, (uint8_t*)out+16 * n, src+16 * n) != RETSUCCESS) {
			printf("aes_decrypt_block error.");
	   }
	}

	return len;
}

void uart_sender_send_message(void)
{
	int i;
	uint8_t aes_buf[CMD_UART_BUF_LEN];
	uint8_t tmp_buf[CMD_UART_BUF_LEN];
	uint8_t aes_len = 0;
	uint8_t aes_checksum = 0;
	struct cmd_uart_sender *s = &g_cmd_uart_sender;

	if(g_cmd_enc_mode == 1){
		aes_len = aes_encrypt(g_cmd_aes_key, &s->send_buf[2], s->send_len-3, aes_buf);

		if(aes_len > CMD_UART_BUF_LEN){
			printf("aes_encrypt too long.");
		}

		s->send_buf[0] = 0xEF;
		s->send_buf[1] = 0xAA;
		s->send_buf[2] = (aes_len>> 8) & 0xff;
		s->send_buf[3] = aes_len & 0xff;

		memcpy(&s->send_buf[4], aes_buf, aes_len);
		for(i=0;i<aes_len;i++){
			aes_checksum ^= aes_buf[i];
		}

		s->send_buf[aes_len + 4] = aes_checksum;
		s->send_len = aes_len + 5;
	}

	for(i=0;i<s->send_len;i++){
		if (rk_uart_sendbyte)
			rk_uart_sendbyte(s->send_buf[i]);
	}
}

void cmd_uart_init(void* param)
{
	rk_uart_sendbyte_cb = (uart_send_callback)param;
	unsigned char keysel[16] = {31, 28, 25, 22, 19, 16, 13, 10, 21, 18, 15, 12, 9, 6, 3, 0};
	memcpy(g_cmd_aes_key_sel, keysel, 16);
	memset(&g_cmd_uart_msg_buf, 0x0, sizeof(struct cmd_uart_msg_buf_t));
	g_cmd_uart_msg_buf.front_num = 0;
	g_cmd_uart_msg_buf.back_num = CMD_UART_MAX;
	pthread_mutex_init(&g_cmd_uart_msg_buf.mutex_lock, NULL);

	unsigned char init_log[] = "Uart parser Init!\n";
	for(char i = 0; i < sizeof(init_log); i++){
		if (rk_uart_sendbyte)
			rk_uart_sendbyte(init_log[i]);
	}
}

void cmd_uart_deinit(void)
{
	pthread_mutex_destroy(&g_cmd_uart_msg_buf.mutex_lock);
	memset(&g_cmd_uart_msg_buf, 0x0, sizeof(struct cmd_uart_msg_buf_t));
}

int cmd_uart_crc_check(struct cmd_uart_msg_head_t *msg)
{
	int32_t ret = 0;
	uint8_t crc;
	uint8_t size[2];
	size[0] = (msg->msg_size & 0xff);
	size[1] = ((msg->msg_size >> 8) & 0xff);
	crc = msg->msg_id ^ size[0] ^ size[1];

	for(int i=0; i<msg->msg_size; i++)
		crc ^= msg->msg_data[i];

	if (crc != msg->crc) {
		printf("warning cmd_uart crc check failed %x %x\n", g_cmd_uart_msg_buf.front_num, msg->msg_id);
		ret = -1;
	}

	return ret;
}

void cmd_uart_parser(unsigned char str)
{
	//printf("recv:0x%x\n", str);
	switch(g_cmd_uart_state)
	{
		case CMD_UART_STATE_WAIT:
			sync_code = ((sync_code & 0xff) << 8) | str;
			if (sync_code == CMD_SYNC_CODE){
				if(g_cmd_enc_mode == 1)
					g_cmd_uart_state = CMD_UART_STATE_HEAD_ENC_SIZE;
				else
					g_cmd_uart_state = CMD_UART_STATE_HEAD_MSG_ID;
			}
			break;
		case CMD_UART_STATE_HEAD_MSG_ID:
			if(str == 0){
				g_cmd_uart_state = CMD_UART_STATE_WAIT;
			}else{
				g_cmd_uart_msg_buf.cmd[g_cmd_uart_msg_buf.front_num].msg_id = str;
				g_cmd_uart_state = CMD_UART_STATE_HEAD_SIZE;
				head_size_cnt = 0;
			}
			break;
		case CMD_UART_STATE_HEAD_ENC_SIZE:
			cmd_uart_msg_size = ((cmd_uart_msg_size << 8) & 0xff00) | str;
			g_cmd_uart_msg_buf.cmd[g_cmd_uart_msg_buf.front_num].msg_size = cmd_uart_msg_size;
			head_size_cnt++;
			if (head_size_cnt == 2)
			{
				g_cmd_uart_state = CMD_UART_STATE_HEAD_ENC_DATA;
				cmd_uart_data_len_cnt = 0;
				head_size_cnt = 0;
			}
			break;
		case CMD_UART_STATE_HEAD_SIZE:
			cmd_uart_msg_size = ((cmd_uart_msg_size << 8) & 0xff00) | str;
			g_cmd_uart_msg_buf.cmd[g_cmd_uart_msg_buf.front_num].msg_size = cmd_uart_msg_size;
			head_size_cnt++;
			if (head_size_cnt == 2)
			{
				if (cmd_uart_msg_size == 0)
					g_cmd_uart_state = CMD_UART_STATE_HEAD_RCV_CRC;
				else
					g_cmd_uart_state = CMD_UART_STATE_HEAD_RCV_DATA;
				cmd_uart_data_len_cnt = 0;
				head_size_cnt = 0;
			}
			break;
		case CMD_UART_STATE_HEAD_ENC_DATA:{
			struct cmd_uart_msg_head_t *cmd_p = &g_cmd_uart_msg_buf.cmd[g_cmd_uart_msg_buf.front_num];
			if (cmd_uart_data_len_cnt == 0) {
				cmd_p->msg_data = (uint8_t *)malloc(cmd_p->msg_size);
			}
			cmd_p->msg_data[cmd_uart_data_len_cnt] = str;
			cmd_uart_data_len_cnt++;
			if (cmd_uart_data_len_cnt == cmd_p->msg_size)
			{
				g_cmd_uart_state = CMD_UART_STATE_HEAD_ENC_CRC;
				cmd_uart_data_len_cnt = 0;
			}
			break;
		}
		case CMD_UART_STATE_HEAD_RCV_DATA:
			{
				struct cmd_uart_msg_head_t *cmd_p = &g_cmd_uart_msg_buf.cmd[g_cmd_uart_msg_buf.front_num];
				if (cmd_uart_data_len_cnt == 0) {
					cmd_p->msg_data = (uint8_t *)malloc(cmd_p->msg_size);
			}

			cmd_p->msg_data[cmd_uart_data_len_cnt] = str;
			cmd_uart_data_len_cnt++;
			if (cmd_uart_data_len_cnt == cmd_p->msg_size)
			{
				g_cmd_uart_state = CMD_UART_STATE_HEAD_RCV_CRC;
				cmd_uart_data_len_cnt = 0;
			}
				break;
			}
		case CMD_UART_STATE_HEAD_RCV_CRC:
		{
			struct cmd_uart_msg_head_t *cmd_p = &g_cmd_uart_msg_buf.cmd[g_cmd_uart_msg_buf.front_num];
			cmd_p->crc = str;
			g_cmd_uart_state = CMD_UART_STATE_WAIT;
			if (!cmd_uart_crc_check(cmd_p))
			{
				if (((g_cmd_uart_msg_buf.front_num + 8) < g_cmd_uart_msg_buf.back_num) &&
					((g_cmd_uart_msg_buf.front_num + 1 + 8) == g_cmd_uart_msg_buf.back_num))
				{
					printf("WARNING cmd_uart_buf is full\n");
					g_cmd_uart_msg_buf.full_flag = 1;
				}

				g_cmd_uart_msg_buf.front_num++;
				g_cmd_uart_msg_buf.front_num &= 0x7;

				msg_mcu_receive();

			}
			break;
		}

		case CMD_UART_STATE_HEAD_ENC_CRC:{
			struct cmd_uart_msg_head_t *cmd_p = &g_cmd_uart_msg_buf.cmd[g_cmd_uart_msg_buf.front_num];
			unsigned char checksum = 0;
			unsigned char aes_buf[CMD_UART_BUF_LEN];
			int i;

			cmd_p->crc = str;
			g_cmd_uart_state = CMD_UART_STATE_WAIT;

			for(i=0;i<cmd_p->msg_size;i++){
				checksum ^= cmd_p->msg_data[i];
			}
			if(checksum == cmd_p->crc){
				if (((g_cmd_uart_msg_buf.front_num + 8) < g_cmd_uart_msg_buf.back_num) &&
					((g_cmd_uart_msg_buf.front_num + 1 + 8) == g_cmd_uart_msg_buf.back_num))
				{
					printf("WARNING cmd_uart_buf is full\n");
					g_cmd_uart_msg_buf.full_flag = 1;
				}
				g_cmd_uart_msg_buf.front_num++;
				g_cmd_uart_msg_buf.front_num &= 0x7;

				if(cmd_p->msg_size > CMD_UART_BUF_LEN){
					printf("aes msg too long! %d\n",cmd_p->msg_size);
					break;
				}
				aes_decrypt(g_cmd_aes_key,cmd_p->msg_data,cmd_p->msg_size,aes_buf);

				cmd_p->msg_id = aes_buf[0];
				cmd_p->msg_size = aes_buf[1] <<8 | aes_buf[2];
				memcpy(cmd_p->msg_data, &aes_buf[3], cmd_p->msg_size);

				msg_mcu_receive();
			}else{
				printf("receive encrypted cmd, but checksum error %x %x\n",checksum,cmd_p->crc);
			}

			break;
		}
		default:
			break;
	}
	//printf("recv:%c\n", str);
}

void cmd_uart_send_sync_head()
{
	unsigned char data[2];

	data[0] = 0xEF;
	data[1] = 0xAA;

	rk_uart_sendbyte(data[0]);
	rk_uart_sendbyte(data[1]);
}

void cmd_uart_send_nid_ready()
{
	s_msg_note_ready_data data;
	data.nid = NID_READY;

	uart_sender_set_message( MID_NOTE, (unsigned char *)&data, sizeof(data));
	uart_sender_send_message();
	/*
	unsigned char data[2];
	unsigned char checksum;

	cmd_uart_send_sync_head();

	data[0] = MID_NOTE;
	data[1] = NID_READY;

	rk_uart_sendbyte(data[0]);
	rk_uart_sendbyte(0x1);
	rk_uart_sendbyte(0x0);
	rk_uart_sendbyte(data[1]);
	checksum = data[0] ^ 0x0 ^ 0x1 ^ data[1];
	rk_uart_sendbyte(checksum);
	*/
}

void cmd_uart_send_nid_unknowerror()
{
	unsigned char data[2];
	unsigned char checksum;

	cmd_uart_send_sync_head();

	data[0] = MID_NOTE;
	data[1] = NID_UNKNOWNERROR;

	rk_uart_sendbyte(data[0]);
	rk_uart_sendbyte(0x1);
	rk_uart_sendbyte(0x0);
	rk_uart_sendbyte(data[1]);
	checksum = data[0] ^ 0x0 ^ 0x1 ^ data[1];
	rk_uart_sendbyte(checksum);
}

void cmd_uart_send_nid_ota_done()
{
	unsigned char data[2];
	unsigned char checksum;

	cmd_uart_send_sync_head();

	data[0] = MID_NOTE;
	data[1] = NID_OTA_DONE;

	rk_uart_sendbyte(data[0]);
	rk_uart_sendbyte(0x1);
	rk_uart_sendbyte(0x0);
	rk_uart_sendbyte(data[1]);
	checksum = data[0] ^ 0x0 ^ 0x1 ^ data[1];
	rk_uart_sendbyte(checksum);
}

void cmd_uart_send_mid_image(uint8_t *image_data, uint16_t len)
{
	unsigned char data[2];
	unsigned char checksum;

	cmd_uart_send_sync_head();

	data[0] = MID_IMAGE;

	rk_uart_sendbyte(data[0]);
	rk_uart_sendbyte((len >> 8) & 0xff);
	rk_uart_sendbyte(len & 0xff);

	checksum = image_data[0];
	for(int i=0; i<len; i++) {
		rk_uart_sendbyte(image_data[i]);
		checksum ^= image_data[i];
	}

	rk_uart_sendbyte(checksum);
}

void cmd_uart_send_mid_powerdown()
{
	unsigned char data[2];
	cmd_uart_send_sync_head();

	data[0] = MID_POWERDOWN;
	rk_uart_sendbyte(data[0]);
}

void cmd_uart_send_mid_replay(uint8_t *data, uint32_t data_len)
{
	unsigned char flag[2];
	unsigned char checksum = 0;

	cmd_uart_send_sync_head();

	flag[0] = MID_REPLY;
	rk_uart_sendbyte(flag[0]);
	rk_uart_sendbyte((data_len >> 8) & 0xff);
	rk_uart_sendbyte((data_len) & 0xff);
	checksum = flag[0] ^ ((data_len >> 8) & 0xff) ^ (data_len & 0xff);
	for(uint32_t i=0; i<data_len; i++)
	{
		rk_uart_sendbyte(data[i]);
		checksum ^= data[i];
	}
	rk_uart_sendbyte(checksum);
}

void cmd_uart_send_mid_note(uint8_t *data, uint32_t data_len)
{
	unsigned char flag[2];
	unsigned char checksum = 0;

	cmd_uart_send_sync_head();

	flag[0] = MID_NOTE;
	rk_uart_sendbyte(flag[0]);
	rk_uart_sendbyte((data_len >> 8) & 0xff);
	rk_uart_sendbyte((data_len) & 0xff);
	checksum = flag[0] ^ ((data_len >> 8) & 0xff) ^ (data_len & 0xff);
	rk_uart_sendbyte(data[0]);
	for(uint32_t i=0; i<data_len; i++)
	{
		rk_uart_sendbyte(data[i]);
		checksum ^= data[i];
	}
	rk_uart_sendbyte(checksum);
}

void cmd_uart_send_mid_replay_gesture_info(uint8_t *data, uint32_t data_len, uint8_t result)
{
	unsigned char flag[2];
	unsigned char checksum = 0;
	uint32_t origin_data_len = data_len;

	cmd_uart_send_sync_head();

	data_len += 2;

	flag[0] = MID_REPLY;
	rk_uart_sendbyte(flag[0]);
	rk_uart_sendbyte((data_len >> 8) & 0xff);
	rk_uart_sendbyte((data_len) & 0xff);
	checksum = flag[0] ^ ((data_len >> 8) & 0xff) ^ (data_len & 0xff);
	flag[0] = MID_GETUSERINFO;
	rk_uart_sendbyte(flag[0]);
	checksum ^= flag[0];
	rk_uart_sendbyte(result);
	checksum ^= result;
	for(uint32_t i=0; i<origin_data_len; i++)
	{
		rk_uart_sendbyte(data[i]);
		checksum ^= data[i];
	}
	rk_uart_sendbyte(checksum);
}

void cmd_uart_send_mid_replay_alluser_id(uint8_t *data, uint32_t data_len, uint8_t result)
{
	unsigned char flag[2];
	unsigned char checksum = 0;
	uint32_t origin_data_len = data_len;

	cmd_uart_send_sync_head();

	data_len += 2;

	flag[0] = MID_REPLY;
	rk_uart_sendbyte(flag[0]);
	rk_uart_sendbyte((data_len >> 8) & 0xff);
	rk_uart_sendbyte((data_len) & 0xff);
	checksum = flag[0] ^ ((data_len >> 8) & 0xff) ^ (data_len & 0xff);
	flag[0] = MID_GET_ALL_USERID;
	rk_uart_sendbyte(flag[0]);
	checksum ^= flag[0];
	rk_uart_sendbyte(result);
	checksum ^= result;
	for(uint32_t i=0; i<origin_data_len; i++)
	{
		rk_uart_sendbyte(data[i]);
		checksum ^= data[i];
	}
	rk_uart_sendbyte(checksum);
}

void cmd_uart_send_mid_replay_data(
		uint8_t *data, uint32_t data_len,
		uint8_t mid_type, uint8_t result)
{

	uart_sender_set_reply(result, mid_type, data, data_len);
	uart_sender_send_message();
	/*
	unsigned char flag[2];
	unsigned char checksum = 0;
	uint32_t origin_data_len = data_len;

	cmd_uart_send_sync_head();

	data_len += 2;

	flag[0] = MID_REPLY;
	rk_uart_sendbyte(flag[0]);
	rk_uart_sendbyte((data_len >> 8) & 0xff);
	rk_uart_sendbyte((data_len) & 0xff);
	checksum = flag[0] ^ ((data_len >> 8) & 0xff) ^ (data_len & 0xff);
	rk_uart_sendbyte(mid_type);
	checksum ^= mid_type;
	rk_uart_sendbyte(result);
	checksum ^= result;
	for(uint32_t i=0; i<origin_data_len; i++)
	{
		rk_uart_sendbyte(data[i]);
		checksum ^= data[i];
	}
	rk_uart_sendbyte(checksum);
	*/
}

void cmd_uart_send_data_direct(
		uint8_t *data, uint32_t data_len,
		uint8_t mid_type, uint8_t result)
{
	unsigned char flag[2];
	unsigned char checksum = 0;
	uint32_t origin_data_len = data_len;

	cmd_uart_send_sync_head();

	data_len += 2;

	flag[0] = MID_REPLY;
	rk_uart_sendbyte(flag[0]);
	rk_uart_sendbyte((data_len >> 8) & 0xff);
	rk_uart_sendbyte((data_len) & 0xff);
	checksum = flag[0] ^ ((data_len >> 8) & 0xff) ^ (data_len & 0xff);
	rk_uart_sendbyte(mid_type);
	checksum ^= mid_type;
	rk_uart_sendbyte(result);
	checksum ^= result;
	for(uint32_t i=0; i<origin_data_len; i++)
	{
		rk_uart_sendbyte(data[i]);
		checksum ^= data[i];
	}
	rk_uart_sendbyte(checksum);
}

void cmd_uart_send_mid_note_replay(
		uint8_t nid_type, uint8_t *data, uint32_t data_len)
{
	uart_sender_set_note(nid_type, data, data_len);
	uart_sender_send_message();
	/*
	unsigned char flag[2];
	unsigned char checksum = 0;
	uint32_t origin_data_len = data_len;

	cmd_uart_send_sync_head();

	data_len += 1;

	flag[0] = MID_NOTE;
	rk_uart_sendbyte(flag[0]);
	rk_uart_sendbyte((data_len >> 8) & 0xff);
	rk_uart_sendbyte((data_len) & 0xff);
	checksum = flag[0] ^ ((data_len >> 8) & 0xff) ^ (data_len & 0xff);
	rk_uart_sendbyte(nid_type);
	checksum ^= nid_type;
	for(uint32_t i=0; i<origin_data_len; i++)
	{
		rk_uart_sendbyte(data[i]);
		checksum ^= data[i];
	}
	rk_uart_sendbyte(checksum);
	*/
}


int md5_calc(uint8_t *src, int srclen, char *obuf)
{
	MD5_CTX ctx;
	unsigned char md[16];
	char tmp[3] = {'0'};

	MD5Init(&ctx);
	MD5Update(&ctx, src, srclen);
	MD5Final(&ctx,md);

	for(int i = 0; i < 16; i++)
	{
		sprintf(tmp, "%02x", md[i]);
		strcat(obuf, tmp);
	}
	//printf("MD5 str= %s\n", obuf);
	return 0;
}

void sys_proc_set_aes_keysel(uint8_t *keysel, unsigned char debug)
{
	memcpy(g_cmd_aes_key_sel, keysel, 16);
	if(debug)
		cmd_uart_send_mid_replay_data(NULL, 0,MID_SET_DEBUG_ENC_KEY, 0);
	else
		cmd_uart_send_mid_replay_data(NULL, 0,MID_SET_RELEASE_ENC_KEY, 0);
}

void sys_proc_init_encryption(void *param)
{
	int i;
	char md5sum[32] = {0};
	s_msg_init_encryption_data *data = (s_msg_init_encryption_data *)param;
	//printf("mode %d\n",data->mode);
	//printf("seed 0x%02x 0x%02x 0x%02x 0x%02x\n",data->seed[0],data->seed[1],data->seed[2],data->seed[3]);

	g_cmd_enc_mode = data->mode;

	md5_calc(data->seed, 4, md5sum);
	for(i=0; i<16; i++){
		g_cmd_aes_key[i] = md5sum[g_cmd_aes_key_sel[i]];
	}

	s_msg_reply_init_encryption_data reply = {0};
	memcpy(reply.device_id,"ydb_test_hahahah",16);
	cmd_uart_send_mid_replay_data((uint8_t *)&reply, sizeof(reply), MID_INIT_ENCRYPTION, 0);
}

void sys_send_system_ready_msg(void)
{
	cmd_uart_send_mid_note_replay(NID_READY,
			(uint8_t*)NULL, 0);
}

void sys_send_facestate_result_msg(void* param)
{
	s_note_data_face *result = (s_note_data_face*)param;

	cmd_uart_send_mid_note_replay(NID_FACE_STATE,
			(uint8_t*)result, sizeof(s_note_data_face));
}

void sys_send_enroll_result_msg(int16_t enroll_mode, int16_t result, int16_t usr_id, unsigned char dir)
{
	printf("enroll result:%d, usr_id=%d dir:0x%x\n",
			result, usr_id, dir);

	mcu_inf_clear_cmd();
	s_msg_reply_enroll_data enroll_replay;
	memset(&enroll_replay, 0, sizeof(enroll_replay));
	enroll_replay.user_id_heb = usr_id >> 8;
	enroll_replay.user_id_leb = usr_id & 0xff;
	enroll_replay.face_direction = dir;

	cmd_uart_send_mid_replay_data((uint8_t *)&enroll_replay,
			sizeof(enroll_replay), enroll_mode, result);
	mcu_ms_status_set(MS_STANDBY);
}

void sys_send_verify_result_msg(int16_t result, int16_t usr_id)
{
	printf("verify result:%d, usr_id=%d\n", result, usr_id);
	mcu_inf_clear_cmd();
	mcu_ms_status_set(MS_STANDBY);

	if (result != MR_SUCCESS) {
		cmd_uart_send_mid_replay_data(NULL, 0,
				MID_VERIFY,
				result);
		return;
	}

	if (usr_id < 0) {
		cmd_uart_send_mid_replay_data(NULL, 0,
				MID_VERIFY,
				MR_FAILED4_UNKNOWNUSER);
	} else {
		s_msg_reply_verify_data verify_reply;
		memset(&verify_reply, 0, sizeof(verify_reply));
		verify_reply.user_id_heb = (usr_id >> 8)&0xff;
		verify_reply.user_id_leb = usr_id&0xff;
		verify_reply.unlockStatus = ST_FACE_MODULE_STATUS_UNLOCK_OK;
		cmd_uart_send_mid_replay_data((uint8_t *)&verify_reply,
				sizeof(verify_reply), MID_VERIFY, MR_SUCCESS);
	}
	return;
}

