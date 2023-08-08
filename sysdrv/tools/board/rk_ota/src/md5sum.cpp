/*************************************************************************
    > File Name: md5sum.cpp
    > Author: jkand.huang
    > Mail: jkand.huang@rock-chips.com
    > Created Time: Thu 07 Mar 2019 03:06:21 PM CST
 ************************************************************************/

#include "openssl/md5.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "common.h"
#include "mtdutils.h"


#define	TMP_MD5SUM_NAME "/tmp/.tmp_md5sum"

bool checkdata_mtd(const char *dest_path, unsigned char* out_md5sum, long long offset, long long checkSize) {

    char nanddump_cmd[256] = {0};
    unsigned char buf[33] = {0};
    unsigned char md5sum[16];

    LOGI( "[%s:%d] offset [%lld] checksize [%lld] \n", __func__, __LINE__, offset, checkSize );

    memset(nanddump_cmd, 0, sizeof(nanddump_cmd)/sizeof(nanddump_cmd[0]));
    sprintf(nanddump_cmd, "nanddump --bb=skipbad -l %lld -s %lld %s | md5sum > %s",
            checkSize, offset, dest_path, TMP_MD5SUM_NAME);
    system(nanddump_cmd);

    int dest_fd = open(TMP_MD5SUM_NAME, O_RDONLY );
    if(dest_fd == 0){
        LOGE("open file failed %s", TMP_MD5SUM_NAME);
        return -1;
    }

    if (read(dest_fd, buf, 32) == -1) {
        close(dest_fd);
        LOGE("(%s:%d) read error: %s.\n", __func__, __LINE__, strerror(errno));
        return -2;
    }

    printf("\n");
    buf[32] = '\0';
    printf("read new md5: [%s]\n", buf);

    unsigned char md5sum_tmp[32];
    for ( int ii=0; ii<32; ii+=2) {
        sscanf((char *)(buf + ii), "%02hhx", &md5sum_tmp[ii]);
    }

    for ( int ii=0, j=0; ii<32; ii+=2 ) {
        // printf ( "%d = %02x\n", ii, md5sum_tmp[ii] );
        md5sum[j] = md5sum_tmp[ii];
        j++;
    }

    printf("new md5:");
    for(int i = 0; i < 16; i++){
        printf("%02x", md5sum[i]);
    }
    printf("\n");
    //change
    if(out_md5sum != NULL){
        memset(out_md5sum, 0, 16);
        memcpy(out_md5sum, md5sum, 16);
    }

    close(dest_fd);
    LOGI("MD5Check is ok of %s\n", dest_path);
    return 0;
}

bool checkdata(const char *dest_path, unsigned char *out_md5sum, long long offset, long long checkSize){
    MD5_CTX ctx;
    unsigned char md5sum[16];
    char buffer[512];
    int len = 0;
    int ret;

    FILE *fp = fopen(dest_path, "rb");
    if(fp == NULL){
        LOGE("open file failed %s", dest_path);
        return -1;
    }

    ret = fseeko64(fp, offset, SEEK_SET);
    if (ret < 0)
    {
        LOGE("%s:%d fseeko64 fail", __func__, __LINE__);
        return false;
    }

    MD5_Init(&ctx);

    long long readSize = 0;
    int step = 512;
    while(checkSize > 0){
        readSize = checkSize > step ? step: checkSize;
        if(fread(buffer, 1, readSize, fp) != readSize){
            LOGE("fread error.\n");
            return false;
        }
        checkSize = checkSize - readSize;
        MD5_Update(&ctx, buffer, readSize);
        memset(buffer, 0, sizeof(buffer));
    }
    MD5_Final(md5sum, &ctx);
    fclose(fp);

    printf("\n");
    printf("new md5:");
    for(int i = 0; i < 16; i++){
        printf("%02x", md5sum[i]);
    }
    printf("\n");
    //change
    if(out_md5sum != NULL){
        memset(out_md5sum, 0, 16);
        memcpy(out_md5sum, md5sum, 16);
    }
    LOGI("MD5Check is ok for %s\n", dest_path);
    return 0;
}

bool compareMd5sum(const char *dest_path, unsigned char *source_md5sum, long long offset, long long checkSize){
    unsigned char md5sum[16];

    checkdata(dest_path, md5sum, offset, checkSize);

    unsigned char tmp[16][2] = {
        0x30, 0x00,
        0x31, 0x01,
        0x32, 0x02,
        0x33, 0x03,
        0x34, 0x04,
        0x35, 0x05,
        0x36, 0x06,
        0x37, 0x07,
        0x38, 0x08,
        0x39, 0x09,
        0x61, 0x0a,
        0x62, 0x0b,
        0x63, 0x0c,
        0x64, 0x0d,
        0x65, 0x0e,
        0x66, 0x0f,
    };
    for(int i = 0; i < 32; i = i+2){
        for(int j = 0; j < 16; j++){
            if(tmp[j][1] == (md5sum[i/2] >> 4)){
                if(source_md5sum[i] != tmp[j][0]){
                    LOGE("MD5Check is error of %s", dest_path);
                    return false;
                }
            }
            if(tmp[j][1] == (md5sum[i/2] & 0x0f)){
                if(source_md5sum[i+1] != tmp[j][0]){
                    LOGE("MD5Check is error of %s", dest_path);
                    return false;
                }
            }
        }
    }
    return true;
}
