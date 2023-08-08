#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <sys/time.h>
#include <signal.h>
#include <linux/netlink.h>

#include "ssv_smartlink.h"
#include "ieee80211.h"

static uint8_t  gBuf[MAX_PAYLOAD]={0};
static uint32_t gBufLen=0;
//static usr_smartlink_result_t ssv_usr_smartlink_result;
static char *ssv_wpa_conf_file      = "%s/conf/wpa_supplicant.conf";
static char *ssv_enter_usr_smartlink_mode = "cd %s/scripts/ && ./startsmartlink.sh";
static char *ssv_start_sta_mode     = "cd %s/         && ./sta.sh";
static uint32_t gChan=SSV_MIN_CHANNEL;
static uint8_t ssv_debug = 0;
static uint8_t ssv_debug_parse = 0;

static void _ssv_usr_smartlink_finish(void)
{
    int ret=0;

    //ret = usr_smartlink_get_result(&akcontex, &ssv_usr_smartlink_result);
    if (ret == 0)
    {
        printf("usr_smartlink_get_result() ok!\n");
        //printf("ssid = %s, pwd = %s, ssid_length = %d, pwd_length = %d, random = 0x%02x\n",
        //    ssv_usr_smartlink_result.ssid, 
        //    ssv_usr_smartlink_result.pwd, 
        //    ssv_usr_smartlink_result.ssid_length,
        //    ssv_usr_smartlink_result.pwd_length, 
         //   ssv_usr_smartlink_result.random);
    }
    else
    {
        printf("usr_smartlink_get_result() failed!\n");
    }
}

static void _ssv_usr_smartlink_setchannel_callback(int signum)
{
    (void)signum;

    ssv_smartlink_set_channel(gChan);
    gChan++;
    if (gChan > SSV_MAX_CHANNEL)
    {
        gChan = SSV_MIN_CHANNEL;
    }
}

static int _ssv_usr_smartlink_enable_setchannel_timer(void)
{
    struct itimerval t;

    t.it_interval.tv_usec   = 200000; //100ms
    t.it_interval.tv_sec    = 0;
    t.it_value.tv_usec      = 200000; //100ms
    t.it_value.tv_sec       = 0;
    if (setitimer(ITIMER_REAL, &t, NULL) < 0)
    {
        printf("%s\n", strerror(errno));
        printf("setitimer error!\n");
        return -1;
    }
    signal(SIGALRM, _ssv_usr_smartlink_setchannel_callback);

    return 0;
}

static int _ssv_usr_smartlink_disable_setchannel_timer(void)
{
    struct itimerval t;

    getitimer(ITIMER_REAL, &t );

    t.it_interval.tv_usec   = 0;
    t.it_interval.tv_sec    = 0;
    t.it_value.tv_usec      = 0;
    t.it_value.tv_sec       = 0;
    if (setitimer(ITIMER_REAL, &t, NULL) < 0)
    {
        printf("%s\n", strerror(errno));
        printf("setitimer error!\n");
        return -1;
    }
    return 0;
}

static void _ssv_sig_int(int signum)
{
    (void)ssv_smartlink_stop();
 
    return;
}

/*
    Example
    -------------------------------------------
     ctrl_interface=/var/run/wpa_supplicant
     ap_scan=1
     
     network={
             ssid="SSV_AP1"
             psk="11111111"
     }
 */
static int _ssv_usr_smartlink_write_wpa_config_file(char *pFileName, char *pSSID, char *pPWD)
{
    FILE *fptr=NULL;

    if (!pFileName || !pSSID || !pPWD)
    {
        printf("Parameter error!\n");
        return -1;
    }
    
    fptr = fopen(pFileName, "w");
    if (fptr == NULL)
    {
        printf("Open %s failed: %s\n", pFileName, strerror(errno));
        return -2;
    }
    fprintf(fptr, "ctrl_interface=/var/run/wpa_supplicant\n");
    fprintf(fptr, "ap_scan=1\n");
    fprintf(fptr, "\n");
    fprintf(fptr, "network={\n");
    fprintf(fptr, "\tssid=\"%s\"\n", pSSID);
    fprintf(fptr, "\tpsk=\"%s\"\n",  pPWD);
    fprintf(fptr, "}\n");
    fclose(fptr);

    printf("Write %s success.\n", pFileName);
    
    return 0;

}

void print_raw_data(uint8_t *data, uint32_t len)
{
    int i  = 0;
    
    printf("data = %p, len = %d\r\n", data, len);
    for (i = 0; i < len; i++)
    {
        if (i % 16 == 0) printf("\r\n");
        printf("%02x ", *(data + i) & 0xff);
    }
    printf("\r\n");
}

#if 0
int parse_80211_data(uint8_t *data, uint32_t len)
{
    common_data_mac_header * hdr = (common_data_mac_header *)data;
    #if 1
    frame_control *fc = 0;
    if (data)
    {
        fc = (frame_control *)&(hdr->fc);
        printf("fc = %x\r\n", hdr->fc);
        printf("version = %x\r\n", fc->version);
        printf("type= %x\r\n", fc->type);
        printf("qos = %x\r\n", fc->qos);
        printf("sub_type = %x\r\n", fc->sub_type);
        printf("ds = %x\r\n", fc->ds);
        printf("more_frag = %x\r\n", fc->more_frag);
        printf("retry = %x\r\n", fc->retry);
        printf("pwr_mgt = %x\r\n", fc->pwr_mgt);
        printf("more_date = %x\r\n", fc->more_date);
        printf("protect = %x\r\n", fc->protect);
        
        printf("order = %x\r\n", fc->order);
    }
    #endif
    return 0;
}
#else

unsigned int ssv_ieee80211_hdrlen(__le16 fc)
{
	unsigned int hdrlen = 24;

	if (ieee80211_is_data(fc)) {
		if (ieee80211_has_a4(fc))
			hdrlen = 30;
		if (ieee80211_is_data_qos(fc)) {
			hdrlen += IEEE80211_QOS_CTL_LEN;
			if (ieee80211_has_order(fc))
				hdrlen += IEEE80211_HT_CTL_LEN;
		}
		goto out;
	}

	if (ieee80211_is_mgmt(fc)) {
		if (ieee80211_has_order(fc))
			hdrlen += IEEE80211_HT_CTL_LEN;
		goto out;
	}

	if (ieee80211_is_ctl(fc)) {
		/*
		 * ACK and CTS are 10 bytes, all others 16. To see how
		 * to get this condition consider
		 *   subtype mask:   0b0000000011110000 (0x00F0)
		 *   ACK subtype:    0b0000000011010000 (0x00D0)
		 *   CTS subtype:    0b0000000011000000 (0x00C0)
		 *   bits that matter:         ^^^      (0x00E0)
		 *   value of those: 0b0000000011000000 (0x00C0)
		 */
		if ((fc & cpu_to_le16(0x00E0)) == cpu_to_le16(0x00C0))
			hdrlen = 10;
		else
			hdrlen = 16;
	}
out:
	return hdrlen;
}

int parse_80211_data(uint8_t **data, uint32_t len)
{
	__le16 fc = 0;
    uint8_t *p_data = *data;
    uint32_t hdrlen = 0;

    if ((data == (void *)NULL) || (p_data == (void *)NULL) || (len < 10))
    {
        printf("parse_80211_data: parameter error\n");
        return -1;
    }
    
	fc = ((struct ieee80211_hdr *)p_data)->frame_control;
    
	if (ieee80211_is_mgmt(fc))
	{
        printf("ieee80211_is_mgmt: fc=0x%02x, len=%d\n", fc, len);
        return -2;
    }

    if (ieee80211_is_ctl(fc))
	{
        printf("ieee80211_is_ctl : fc=0x%02x, len=%d\n", fc, len);
        return -3;
    }

    if (ieee80211_is_data(fc))
	{
        printf("ieee80211_is_data: fc=0x%02x, len=%d, encryption=%d\n", fc, len, ieee80211_has_protected(fc));
        hdrlen = ssv_ieee80211_hdrlen(fc);
        *data += hdrlen;
        return len - hdrlen;
    }

    return -4;
}

#endif
int
parse_arguments(int argc, char **argv)
{
    int i;
    int blksize;
    int server_flag, client_flag, rate_flag, duration_flag;   
    char* slash;

    blksize = 0;
    server_flag = client_flag = rate_flag = duration_flag = 0;   
    for(i=1; i<argc; i++) {
        if (strcmp(argv[i], "-c") == 0)
        {
            if (++i >= argc) {
                return -1;
            }
            gChan = atoi(argv[i]);
            continue;
        }
        else if (strcmp(argv[i], "-d") == 0){
            ssv_debug = 1;
            continue;
        }
        else if (strcmp(argv[i], "-p") == 0){
           ssv_debug_parse = 1;
           continue;
       }
    }
    
    return 0;
}

int main(int argc, char *argv[])
{
    int ret=-1;
    //char *file_path_getcwd=NULL;
    const char *key="Wechatiothardwav";
    char cmdBuf[160]={0};
    int len = 0;
    uint8_t *data = 0;
    #if 0
    /************************************************
        usr_smartlink
     ************************************************/
    ret = usr_smartlink_init(&akcontex, &akconf);
    if (ret < 0)
    {
        printf("usr_smartlink init failed\n");
        goto out;
    }
    printf("%s\n", usr_smartlink_version());

    #if usr_smartlink_ENABLE_CRYPT
    usr_smartlink_set_key(&akcontex, key, strlen(key));
    #endif
    #endif
    /***********************************************
        SSV usr_smartlink
     ***********************************************/
    printf("%s\n", ssv_smartlink_version());
    ret = ssv_smartlink_start();
    if (ret < 0)
    {
        printf("ssv_smartlink_start error: %d\n", ret);
        goto out;
    }

    /*
        SSV: Enable WIFI Promisc Mode
     */
    ret = ssv_smartlink_set_promisc(1);
    if (ret < 0)
    {
        printf("ssv_smartlink_set_promisc error: %d\n", ret);
        goto out;
    }
    
    gChan = 0;
    parse_arguments(argc, argv);
    
    /*
        SSV: Enabled timer for updating channel with 100ms interval
     */
    if (gChan != 0)
    {
        ssv_smartlink_set_channel(gChan);
    }
    else
    {
        gChan = SSV_MIN_CHANNEL;
        ret = _ssv_usr_smartlink_enable_setchannel_timer();
        if (ret < 0)
        {
            goto out;
        }
    }

    signal(SIGINT, _ssv_sig_int); //Capture CTRL+C

    while (1)
    {
        gBufLen = 0;
        memset(gBuf, 0, sizeof(gBuf));
        ret = ssv_smartlink_recv_packet(gBuf, &gBufLen);
        if (ret < 0)
        {
            if ((errno == EINTR) || (errno == EAGAIN))
            {
                continue;
            }
            
            if ((errno == ENOMEM) || (errno == ENOBUFS))
            {
                continue;
            }
            
            printf("ssv_smartlink_recv_packet error: %d\n", ret);
            goto out;
        }
        
        //printf(" gBufLen: %d\n", gBufLen);
        //if (ssv_debug)
        //{
            //print_raw_data(gBuf, gBufLen);
        //}
        
        if (ssv_debug_parse)
        {
            data = gBuf;
            len = gBufLen;
            len = parse_80211_data(&data, len);
            if (len > 0)
            {
                // this is 802.3 raw packet
                printf("========== 80211_data=%p, len=%d 802.11_len=%d ==========\n", data, len, gBufLen);

                if (ssv_debug)
                {
                    print_raw_data(data, len);
                }
                
                printf("=========================================================\n\n");
            }
        }

        
        #if 0
        ret = usr_smartlink_recv(&akcontex, gBuf, gBufLen);
        if (ret == usr_smartlink_STATUS_CHANNEL_LOCKED)
        {
            ret = _ssv_usr_smartlink_disable_setchannel_timer();
            if (ret < 0)
            {
                goto out;
            }

            /*
                In timer callback, we have increased it.
                So now we decrease it!
             */
            gChan = gChan - 1;
            if (gChan < SSV_MIN_CHANNEL)
            {
                gChan = SSV_MIN_CHANNEL;
            }
            printf("Channel locked to %d!\n", gChan);
        }
        else if (ret == usr_smartlink_STATUS_COMPLETE)
        {
            _ssv_usr_smartlink_finish();
            ret = ssv_smartlink_set_promisc(0); //Disable WIFI Promisc Mode
            if (ret < 0)
            {
                goto out;
            }

            break;
        }
        else
        {
            /* continue; */
        }
        #else
         continue;
        #endif
    }

    ret = 0;

out:
    (void)ssv_smartlink_stop();
#if 0
    if (ret == 0)
    {
        sprintf(cmdBuf, ssv_wpa_conf_file, file_path_getcwd);
        if (0 == _ssv_usr_smartlink_write_wpa_config_file(cmdBuf, ssv_usr_smartlink_result.ssid, ssv_usr_smartlink_result.pwd))
        {
            char buf[64];
            
            printf("Config file: %s\n", cmdBuf);
            printf("----------------------------------------\n");
            sprintf(buf, "cat %s", cmdBuf);
            system(buf);
            printf("----------------------------------------\n");
            
            /*
                Call script to start STA
             */
            sprintf(cmdBuf, ssv_start_sta_mode, file_path_getcwd);
            system(cmdBuf);
        }
    }

    if (file_path_getcwd)
    {
        free(file_path_getcwd);
    }
#endif
    return ret;

}

