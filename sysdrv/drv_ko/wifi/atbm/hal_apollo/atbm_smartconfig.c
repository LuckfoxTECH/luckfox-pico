#ifdef ATBM_SUPPORT_SMARTCONFIG

#include "apollo.h"
#include "smartconfig.h"
//#define	 TOTAL_TIME	2000	//total time of send magic
//#define  INTERVAL	5	//send magic per 5ms

//extern struct hmac_configure hmac_cfg;


#define STC_STCTYPE_F_MASK     (7<<8)
#define STC_PALOAD_F_SSIDRANDOM  (1<<8)
#define STC_TOTALLEN_F_TOTAL_LEN    (0<<8)
#define STC_TOTALLEN_F_CSUM	     (4<<8)
#define STC_PALOAD_F_DATA	     (2<<8)
#define STC_PALOAD_F_CSUM	     (3<<8)

#define STC_TOTALLEN_F_MASK          (7<<8)
#define STC_TOTALCHECK_F_MASK		(0xff)

#define STC_PALOAD_F_MASK          (0xf<<7)
#define STC_SSIDRANDOM_F_MASK	   (0xf<<7)
#define STC_SSIDRANDOM_F_SSIDLEN    (0x2<<7)
#define STC_SSIDRANDOM_F_RANDOM     (0x3<<7)
#define STC_RANDOM_DATA_MASK	    (0x7f)
#define STC_SSIDLEN_DATA_MASK	    (0x7f)


#define SMARTCONFIG_MINI_LEN 5


#define STC_PALOAD_DATA_SN_MASK          (0xf<<4)
#define STC_PALOAD_DATA_MASK          (0xf)

#define SMARTCONFIG_MAGIC			(0x4)



//static short smartconfig_ori_payload[256] = {0};/*include ssid len /randomlen/random/payload data*/
extern short * st_payload_buf;/*include ssid len /randomlen/random/payload data*/

static short payloaddatalen =0;
static short payloaderror_cnt = 0;
static short payloaddata_id_pre =0;
//static short random =0;
static short totallen =0; /* include ssidlen[1] + randomlen[1] ++ random[randomlen] + ssid + password + checksum*/
//static short id_x =0;
//static short id_x_cnt =0;
static short magic_code[4]={0};
static short smartconfig_ori_data[2] = {0};
static short ssid_len=0;

extern int baselen;
static int smartconfg_st = 0;

#define  payloadHdrLen 1
#define smartconfig_ori_payload  st_payload_buf
struct hmac_configure hmac_cfg;

/*check sum ok return 1*/
int payloadchecksum(short *data,int len ){

	int j;

	int checksum_recv = 0;
	atbm_printk_smt("len = %d\n", len);
	for(j=0;j<len;j++)
	{	
		checksum_recv += data[j];
		//printk("checksum_recv = %x\n", checksum_recv);
	}
	checksum_recv &= STC_RANDOM_DATA_MASK;
	checksum_recv |= STC_PALOAD_F_CSUM;
	if((checksum_recv & 0xffff) == data[len]){
	//	printk("checksum_succ \n", (checksum_recv & 0xffff));
		return 1;
	}
	else{ 
		atbm_printk_smt("smartconfig checksum_fail = %d %d \n", checksum_recv,data[len]);
		return 0;
	}
}
int encrypt_payload(char *Enbuffer,char *Ori_buffer,int Ori_buflen,char * key, int keylen)
{
	int i=0;

	int encbufLen =0;


	for(;i<Ori_buflen;i++){
#if 0
		//low
		Enbuffer[encbufLen] = (Ori_buffer[i]&0xf)^key[keyid];
		encbufLen++;
		//next keyid
		keyid++;
		if(keyid>=keylen)
			keyid=0;

		//high
		Enbuffer[encbufLen] = ((Ori_buffer[i]>>4)&0xf)^key[keyid];		
		encbufLen++;
		//next keyid
		keyid++;
		if(keyid>=keylen)
			keyid=0;
#else
		
		//low
		Enbuffer[encbufLen] = (Ori_buffer[i]&0xf);
		encbufLen++;

		//high
		Enbuffer[encbufLen] = ((Ori_buffer[i]>>4)&0xf);		
		encbufLen++;
#endif 
	}
	return encbufLen;
}

int dencrypt_payload(char *Decbuffer,short *Ori_buffer,int Ori_buflen,char * key, int keylen)
{
	int i=0;
//	int keyid=0;
	int encbufLen =0;


	for(i=0;i<Ori_buflen;i++){
		
		//Decbuffer[encbufLen] = (Ori_buffer[i]&0xf)^key[keyid];
		Decbuffer[encbufLen] = (Ori_buffer[i]&0xf);
		encbufLen++;
		////next keyid
		//keyid++;
		//if(keyid>=keylen)
		//	keyid=0;
	}
	return encbufLen;
}


/*check sum ok return 1*/
int payload_build_data(short *data,int len){
	char * decBuffer = (char *)data;
	char ssid_password[96]={0};
	//char random[7];
//	int randomlen=0;
	int ssidlen;
	int i =0;

	ssidlen = *data & 0xff;
	data++;


	dencrypt_payload(decBuffer,(short *)data,len,0,0);
	for(i=0;i<len;i+=2){
		ssid_password[i/2] = decBuffer[i]|(decBuffer[i+1]<<4);
	}
	atbm_printk_smt("password&ssid %s\n",ssid_password);
	if(len/2 >= ssidlen){
		//password + ssid 
		hmac_cfg.password_len = len/2- ssidlen;
		memcpy(hmac_cfg.password,ssid_password,hmac_cfg.password_len);		  
		hmac_cfg.ssid_len = ssidlen;
		memcpy(hmac_cfg.ssid,&ssid_password[hmac_cfg.password_len],ssidlen);
		if(hmac_cfg.password_len > 8){
			//hmac_cfg.key_mgmt = KEY_WPA2;
			hmac_cfg.privacy =1;
		}
		else if(hmac_cfg.password_len==5){
			//hmac_cfg.key_mgmt = KEY_WEP;
			hmac_cfg.privacy =1;
		}
		else if(hmac_cfg.password_len==13){
			//hmac_cfg.key_mgmt = KEY_WEP;
			hmac_cfg.privacy =1;
		}
		else if(hmac_cfg.password_len==0) {
			//hmac_cfg.key_mgmt = KEY_NONE;
			hmac_cfg.privacy =0;
		}
		else {
			atbm_printk_smt("<ERROR> smartconfig key len error %d\n",hmac_cfg.password_len);

		}
			
	}
	return 1;
}


int fun_recv_magic(struct atbm_common *hw_priv,short rxdata)
{
	if((baselen > rxdata)||(baselen==0)){
		baselen = rxdata;
		atbm_printk_smt("baselen = %d\n", baselen);
	}
	atbm_printk_smt("%s %d\n", __FUNCTION__, __LINE__);
	magic_code[rxdata&3] = rxdata;
	//printk("rxdata&3 %d rxdata %d\n",rxdata&3,rxdata);
	if((magic_code[baselen & 3]+1 == magic_code[(baselen+1) & 3])&&
		(magic_code[baselen & 3]+2== magic_code[(baselen+2) & 3])&&
		(magic_code[baselen & 3]+3== magic_code[(baselen+3) & 3])){
		atbm_printk_smt("CONFIG_ST_PAYLOAD %d\n",baselen);
		smartconfg_st = CONFIG_ST_PAYLOAD ;
		return 1;
	}
	return 0;
}


/*not build in flash for Andes complie bug*/
int fun_recv_payload(short rxdata, int turn_id)
{

	   int payloaddata_id =0;
		//Console_SetPolling(1);
		atbm_printk_smt("turn_id = %d\n", turn_id);
		atbm_printk_smt("rxdata: 0x%x,st %d\n", rxdata,smartconfg_st);
		switch(smartconfg_st){
		case CONFIG_ST_PAYLOAD:		
			memset(smartconfig_ori_data, 0, sizeof(smartconfig_ori_data));
			memset(smartconfig_ori_payload, 0, sizeof(short)*ST_PAYLOAD_BUF_SZ);
			//payloadHdrLen = 0;
			payloaddatalen = 0;
			payloaderror_cnt = 0;
		case CONFIG_ST_RE_PAYLOAD:
			//id_x =0;
			payloaddata_id_pre =0;
			//id_x_cnt =0;
			if((rxdata > SMARTCONFIG_MAGIC) && ((rxdata & STC_STCTYPE_F_MASK) == STC_TOTALLEN_F_TOTAL_LEN)){
				smartconfig_ori_data[0] = rxdata;
				atbm_printk_smt("CONFIG_ST_RE_PAYLOAD:rxdata:0x%x\n", rxdata);
				//len=1;
				smartconfg_st = CONFIG_ST_GET_TOTALLEN;
			}
			break;
		case CONFIG_ST_GET_TOTALLEN:
			if((rxdata & STC_STCTYPE_F_MASK) == STC_TOTALLEN_F_CSUM){
				smartconfig_ori_data[1] = rxdata;
				if(rxdata != ((~smartconfig_ori_data[0] & 0xff)|STC_TOTALLEN_F_CSUM)){
					//if checksum error 
					smartconfg_st = CONFIG_ST_PAYLOAD;
					//printk("rxdata = 0x%x\n", rxdata);
					//printk("smartconfig_ori_data[0] = 0x%x\n", smartconfig_ori_data[0]);
					//printk("((~smartconfig_ori_data[0] & 0xff)|STC_TOTALLEN_F_CSUM) = 0x%x\n", ((~smartconfig_ori_data[0] & 0xff)|STC_TOTALLEN_F_CSUM));
					atbm_printk_smt("totallen_checksum error\n");
					break;
				}
				//save total len
				totallen= smartconfig_ori_data[0] & 0xff;
				atbm_printk_smt("st_totallen=%d\n",totallen);
				if(totallen < SMARTCONFIG_MINI_LEN){
					//if checksum error 
					smartconfg_st = CONFIG_ST_PAYLOAD;
					break;
				}				
				smartconfg_st = CONFIG_ST_GET_PAYLOAD;
			}
			else {			
				if((rxdata & STC_STCTYPE_F_MASK) == STC_TOTALLEN_F_TOTAL_LEN){
					smartconfig_ori_data[0] = rxdata;
					//printk("smartconfig_ori_data[0] = 0x%x\n", smartconfig_ori_data[0]);
				}
				else {
					smartconfg_st = CONFIG_ST_PAYLOAD;
				}
			}
			break;
		case CONFIG_ST_GET_PAYLOAD:
		case CONFIG_ST_GET_PAYLOAD_CSUM:

			//WARN_ON(smartconfig_ori_payload==NULL);
			if((rxdata & STC_PALOAD_F_MASK) == STC_SSIDRANDOM_F_SSIDLEN){
				smartconfig_ori_payload[0]=rxdata;
				ssid_len = rxdata & STC_SSIDLEN_DATA_MASK;
				//printk("payload[0]=%x\n",smartconfig_ori_payload[0]);
				//id_x = 0;
			}
		    else if((rxdata & STC_STCTYPE_F_MASK) == STC_PALOAD_F_DATA) /* if((payloadHdrLen >= random+2 )&&())*/{//data 
			
				payloaddata_id = (rxdata &STC_PALOAD_DATA_SN_MASK )>>4;
				if(smartconfig_ori_payload[payloadHdrLen+payloaddata_id+ turn_id*16] == 0){
					if((payloadHdrLen+payloaddata_id+ turn_id*16) >= totallen - payloadHdrLen){
						payloaderror_cnt++;
						break;
					}
					smartconfig_ori_payload[payloadHdrLen+payloaddata_id+ turn_id*16]=rxdata;
					atbm_printk_smt("st_payload[%d]=%x,len %d\n", payloadHdrLen+payloaddata_id+ turn_id*16,smartconfig_ori_payload[payloadHdrLen+payloaddata_id+ turn_id*16],payloaddatalen);
					payloaddatalen++;
				}
				else {						
				    //printk("222 id_x_cnt %d id_x %d payloaddata_id %d payloaddata_id_pre %d\n",id_x_cnt,id_x,payloaddata_id , payloaddata_id_pre);
					if(((payloaddata_id_pre+1)&0xf)==payloaddata_id){	
						atbm_printk_smt("before stpayload[%d]=%x\n", payloadHdrLen+payloaddata_id+ turn_id*16,smartconfig_ori_payload[payloadHdrLen+payloaddata_id+ turn_id*16]);
						smartconfig_ori_payload[payloadHdrLen+payloaddata_id+ turn_id*16]=rxdata;
						atbm_printk_smt("xxx stpayload[%d]=%x\n", payloadHdrLen+payloaddata_id+ turn_id*16,smartconfig_ori_payload[payloadHdrLen+payloaddata_id+ turn_id*16]);
					}
				}
	
				
				payloaddata_id_pre= payloaddata_id;	
				if((turn_id >= (totallen-1 - payloadHdrLen)/16 ) &&(payloaddata_id == (totallen-2 - payloadHdrLen )%16  )){
					atbm_printk_smt("id_x = %d\n", turn_id);
				  	smartconfg_st = CONFIG_ST_GET_PAYLOAD_CSUM;
				}
				
			}
			else if((rxdata & STC_STCTYPE_F_MASK) == STC_PALOAD_F_CSUM){					
				//printk("payloaddata_id %d (totallen-2)%16 %d\n",payloaddata_id, (totallen-2)%16);
				//id_x = 0;
				smartconfig_ori_payload[totallen-1]=rxdata;
				//payloaddatalen -= payloaderror_cnt;
				atbm_printk_smt("payloaddatalen=%d,totallen=%d\n",payloaddatalen,totallen);

				//if((payloaddatalen <= totallen/2) && (smartconfig_ori_payload[totallen/2] != 0)){
					//smartconfg_st = CONFIG_ST_PAYLOAD;
					//break;
				//}
				if(payloaddatalen == totallen  -payloadHdrLen -1){
					if(payloadchecksum(smartconfig_ori_payload,payloaddatalen+1)){	
						 payload_build_data(smartconfig_ori_payload,payloaddatalen);
						 smartconfg_st = CONFIG_ST_DONE_SUCCESS;
						 atbm_printk_smt("_SUCCESS \n");
						 atbm_printk_smt("smartconfig_ori_data[0] = %d]\n", smartconfig_ori_data[0]);
						 return CONFIG_ST_DONE_SUCCESS;
					}
					else {							
					 smartconfg_st = CONFIG_ST_PAYLOAD;
					 payloaddatalen = 0;
					}
				}
				else{
					//payloaddatalen += payloaderror_cnt;
					smartconfg_st = CONFIG_ST_RE_PAYLOAD;
				}	
				
			}
			break;	
		
		default:
			break;
		}
	//}	
	//printk("ret smartconfg_st %d\n", smartconfg_st);
	return smartconfg_st;
}


#if 0

#define SMARTCONFIG_MAGIC_CNT 20
#define SMARTCONFIG_PALOAD_CNT 5
short func_send_smartconfig_data(short data,int step)
{
	fun_recv_payload(data);
	//magic 
	return 0;
	//
}

char password[]="1234567890";
char ssid[]="atbm_wifi_ap2222";


int smartconfig_send_main(char *ssid,int len_ssid,char* password,int len_pwd)
{
	int payload_cnt = 0;
	int count = 0;
	int i= 0;
	unsigned char high = 0xf0, low = 0x0f;
	unsigned char seq = 0x00;
	char random[8] = {0};
	char buf[256] = {0};
	char checksumbuf[256] = {0};
	short short_data ;
	char password_ssid[128] = {0};
	int password_ssid_len = 0;
	short checksum_send = 0;
	//len_ssid = strlen(ssid);
	//len_pwd =  strlen(password);
	memcpy(password_ssid,password,len_pwd);
	memcpy(password_ssid+len_pwd,ssid,len_ssid);
	password_ssid_len = len_ssid + len_pwd;


	
	//create phtread
	//pthread_create(&pth_id, NULL, fun_recv_payload,(void *)fd );
	//pthread_detach(pth_id);
	
	//send magic package
	while(++count<SMARTCONFIG_MAGIC_CNT){
		//printf("write:0x%x 0x%x\n", magic[0], magic[1]);
		short_data = 1;
		func_send_smartconfig_data(short_data,0);
		short_data = 2;
		func_send_smartconfig_data(short_data,0);
		short_data = 3;
		func_send_smartconfig_data(short_data,0);
		short_data = 4;
		func_send_smartconfig_data(short_data,0);
	}

	count =0;
	while(++count<SMARTCONFIG_PALOAD_CNT){
		checksum_send = 0;

		printf("len_ssid=%d, len_pwd=%d\n", len_ssid, len_pwd);
		//send total len
		short_data = STC_TOTALLEN_F_TOTAL_LEN | (2*password_ssid_len + 2/*ssidlen + checksum*/);	
		func_send_smartconfig_data(short_data,1);

		//send checksum
		short_data = STC_TOTALLEN_F_CSUM | (~short_data & 0xff);	//checksum
		func_send_smartconfig_data(short_data,1);

		//send ssid len
		short_data = STC_PALOAD_F_SSIDRANDOM | (len_ssid & 0x7f);	
		printf("ssid_len0=%x\n",short_data);
		checksum_send = short_data;
		func_send_smartconfig_data(short_data,1);
	
		//get payload
		payload_cnt = encrypt_payload(buf,password_ssid,password_ssid_len,0,0);
		
		//send payload
		for(i=0;i<payload_cnt;i++)
		{
			//low
			short_data = STC_PALOAD_F_DATA | (buf[i]&STC_PALOAD_DATA_MASK) | (STC_PALOAD_DATA_SN_MASK &((i)<<4));
			printf("payload%d=%x\n",i +  1, short_data);
			//printf("write data:%x %x\n", data[1], buf[i]);
			checksum_send += short_data;
			if((rand() % 7)!=0){
				func_send_smartconfig_data(short_data,1);
			}
			else {
				printf("drop test \n");
			}

		}
		
		//checksum
		checksum_send &= 0x7f; 
		checksum_send |= STC_PALOAD_F_CSUM;
		printf("checksum_send = %d\n", checksum_send);
		func_send_smartconfig_data(checksum_send,2);
		//fun_recv_payload(STC_PALOAD_F_CSUM);
	}
	

	return 0;
}
int main(int argc, char *argv[])
{
	
	smartconfig_send_main(ssid,strlen(ssid)-2, password,strlen(password)-2);
}
#endif
#endif
