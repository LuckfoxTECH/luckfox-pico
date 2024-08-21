#ifndef __ATBM_COMMON_H__
#define __ATBM_COMMON_H__


#define USER_DATE_LEN 255
#define MAC_FILTER_NUM 8

typedef struct _Wifi_Recv_Info
{
	unsigned char Ssid[32];
	unsigned char channel;
	unsigned int Rssi;		//信号强度
	unsigned int Quality;	 //信号质量
	unsigned char phy_noise;	//噪声
	unsigned char Bssid[6];	//热点 WiFi MAC地址
	unsigned char User_data[USER_DATE_LEN+1]; //存放收到广播包后要回复私有数据
}Wifi_Recv_Info_t;	  //接收结构体

typedef struct _Wifi_AP_Info
{
	unsigned int flag;
	Wifi_Recv_Info_t priv_recv_info;
}Wifi_Ap_Info_t;

typedef struct _Wifi_Send_Info
{
	unsigned char mac_filter;
	unsigned char Ssid[32];
	unsigned short channel;
	unsigned char Bssid[MAC_FILTER_NUM][ETH_ALEN];    //需要过滤的mac地址
	unsigned char User_data[USER_DATE_LEN+1]; //存放要发送的私有数据
}Wifi_Send_Info_t;	   //发送结构体

typedef struct atbm_private_data{
	int set_flag;
	int data_len;
	char user_data[USER_DATE_LEN+1];
}ATBM_PRIVATE_DATA;

/************** 6441 ***************************/

struct atbm_vendor_cfg_ie{
	u8 ie_id;
	u8 ie_len;
	u8 OUI[4];
	u8 ssid_len;
	u8 password_len;
	u8 ssid[32];
	u8 password[64];
};	
#define ATBM_6441_PRIVATE_OUI 0x4154424d





/*****************************************/


#define AP_SCAN_NUM_MAX 32 //ap number per channel
#ifdef CONFIG_ATBM_5G_PRETEND_2G
#define CHANNEL_NUM 18 //channel
#else
#define CHANNEL_NUM 14 //channel

#endif

typedef struct _best_channel_info_
{
	u8 ssid[32];
	u8 mac_addr[ETH_ALEN];
	u8 rssi;
	u8 flag;
	u8 enc_type;
	u8 enc_type_name;
}BEST_CHANNEL_INFO;
typedef struct _best_ch_busy_ratio_
{
	unsigned int best_ch_busy_ratio[CHANNEL_NUM];
	unsigned int total_cnt;
}BEST_CHANNEL_BUSY_RATIO;

typedef struct _best_ch_scan_result_
{
	unsigned int channel_ap_num[CHANNEL_NUM];
	unsigned int busy_ratio[CHANNEL_NUM];
	unsigned int weight[CHANNEL_NUM];
	unsigned char suggest_ch;
}Best_Channel_Scan_Result;

typedef struct _special_ch_freq_{
	//unsigned int special_ch;
	unsigned int special_freq;//2380MHz or 2504MHz
	unsigned int flag;
}SPECIAL_CH_FREQ;

#endif
