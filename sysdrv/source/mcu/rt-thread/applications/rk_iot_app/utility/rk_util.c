/*
 *  Copyright (C) 2019, Fuzhou Rockchip Electronics Co., Ltd.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of Fuzhou Rockchip Electronics Co., Ltd. nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <rtthread.h>
#include <rtdef.h>
#include <dfs_posix.h>

#include "rkdef.h"
#define DBG_ENABLE
#define DBG_SECTION_NAME    "RKUTIL"
#define DBG_LEVEL           DBG_INFO
#include "rkdebug.h"
#include "rk_util.h"

uint32 Num2Str(uint8 *pstr, uint32 num)
{
    uint32 i = 0;
    uint8 pstr1[16] = {0};

    do
    {
        pstr1[i++] = num % 10 + 0x30;
        num = num / 10;
    }
    while (num != 0);

    num = i;
    for (i = 0; i < num; i++)
        pstr[num - i - 1] = pstr1[i];

    pstr[num] = 0;

    return num;
}

uint32 Str2Num(uint8 *pstr)
{
    uint32 num = 0;

    /*ptr save the fisrt char of pstr*/
    const uint8 *ptr = pstr;

    /*if pstr start with '-' or '+' sign,move to the next charactor.*/
    if (*pstr == '-' || *pstr == '+')
        pstr++;

    while (*pstr != 0)
    {
        /*if pstr is none digal number ,break and return*/
        if (*pstr < '0' || *pstr > '9')
            break;
        /*current is digal ,calculate the value*/
        num = num * 10 + (*pstr - '0');
        pstr++;
    }

    if (*ptr == '-')
        num = -num;

    return num;
}

uint32 Str2Hex(uint8 *pstr)
{
    uint32 num = 0;
    uint8 temp = 0;

    while (*pstr != 0)
    {
        if (*pstr < '0' || *pstr > 'z')
            break;

        if (*pstr >= 'a')
        {
            temp = *pstr - 'a' + 'A';
            num = num * 16 + (temp - 'A' + 10);
        }
        else
        {
            num = num * 16 + (*pstr - '0');
        }
        pstr++;
    }

    return num;
}

//scan the path files count
uint32 ScanFiles(const char *path)
{
    DIR *pth = NULL;
    struct dirent *dr = NULL;
    uint32 cnt = 0;

    pth = opendir(path);
    if (NULL == pth)
    {
        printf("open dir %s fail \n", path);
        return cnt;
    }

    printf("open dir %s success :%p\n\n", path, pth);
    while (1)
    {
        dr = readdir(pth);
        if (dr != NULL)
        {
            if (!strcmp(dr->d_name, ".") || !strcmp(dr->d_name, ".."))
                continue;

            LOGD(" \t d_name = %s dtype = %d\n", dr->d_name, dr->d_type);

            if (dr->d_type == 1)
            {
                LOGD("dr->d_name=%s \n", dr->d_name);
                cnt++;
            }
        }
        if (dr == NULL)
        {
            closedir(pth);
            break;
        }
    }

    LOGD("PATH [%s]: have %ld files \n", path, cnt);
    return cnt;
}

/**
 * @description: create a range numer between 1 ~ range
 * @param : int range
 * @return: rand number
 */
uint32_t GetRandom(uint32_t range)
{
    uint32_t seed;
    uint32_t val;
    seed = rt_tick_get();

    srand(seed);
    val = rand() % range;
    if (val == 0)
        val = 1;

    return val;
}
