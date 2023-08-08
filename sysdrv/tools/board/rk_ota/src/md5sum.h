/*************************************************************************
    > File Name: md5sum.h
    > Author: jkand.huang
    > Mail: jkand.huang@rock-chips.com
    > Created Time: Thu 07 Mar 2019 03:14:08 PM CST
 ************************************************************************/

#ifndef _MD5SUM_H
#define _MD5SUM_H
bool checkdata(const char *dest_path, unsigned char *out_md5sum, long long offset, long long checkSize);
bool checkdata_mtd(const char *dest_path, unsigned char *out_md5sum, long long offset, long long checkSize);
bool compareMd5sum(const char *dest_path, unsigned char *source_md5sum, long long offset, long long checkSize);
#endif
