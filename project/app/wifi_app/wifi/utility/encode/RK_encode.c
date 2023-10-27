#include <stdio.h>
#include "gbk_to_utf8.h"
#include "utf8_to_gbk.h"
#include "RK_encode.h"

int RK_encode_is_utf8(char *buf, const int size)
{
	int i = 0;
	int bit1num = 0;
	unsigned char temp = 0;

	if ((size == 0) || (*buf == '\0')) {
		return 1;
	}

	for (i = 0; i < size; i++) {
		if ((!bit1num) && ((buf[i] & 0x80) == 0)) {
			continue;
		} else if ((bit1num) && ((buf[i] & 0xc0) == 0x80)) {
			bit1num--;
			continue;
		} else if ((bit1num) && ((buf[i] & 0xc0) != 0x80)) {
			return 0;
		} else {
			temp = buf[i] & 0xff;
			bit1num = 0;
			while ((temp << bit1num) & 0x80) {
				bit1num++;
				if(bit1num > 6){
					return 0;
				}
			}
			if(bit1num < 2) {
				return 0;
			}
			bit1num--;
		}
	}
	if(bit1num)
		return 0;

	return 1;
}

int RK_encode_gbk_to_utf8(unsigned char* src, int len, unsigned char* dst)
{
	return gbk_to_utf8(src, len, dst);
}


int RK_encode_utf8_to_gbk(unsigned char* src, int len, unsigned char* dst)
{
	return utf8_to_gbk(src, len, dst);
}
