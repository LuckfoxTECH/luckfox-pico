/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup DEBUG
 *  @{
 */

/** @defgroup DEBUG_How_To_Use How To Use
 *  @{

 The DEBUG driver can be used as follows:

 Implement DBG hook:

 - printf func: define new HAL_SYSLOG in hal_conf.h or use HAL_DBG_Printf() in default;
 - assert func: redefine AssertFailed().

 Define debug level in hal_conf.h:

 - HAL_DBG_ON: print master switch;
 - HAL_DBG_INFO_ON: information printing switch;
 - HAL_DBG_WRN_ON: information printing switch;
 - HAL_DBG_ERR_ON: information printing switch;
 - HAL_ASSERT_ON: Support assert.

 APIS:

 - printf information by calling HAL_DBG();
 - printf warning by calling HAL_DBG_WRN();
 - printf error by calling HAL_DBG_ERR();
 - do assert by calling HAL_ASSERT().

 @} */

#include "hal_base.h"

/** @defgroup DEBUG_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/

/********************* Private Structure Definition **************************/

/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/

/** @} */
/********************* Public Function Definition ****************************/

/** @defgroup DEBUG_Exported_Functions_Group5 Other Functions

 This section provides functions allowing to init and deinit module as follows:

 *  @{
 */

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the HAL_ASSERT error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: HAL_ASSERT error line source number
  */
__WEAK void HAL_AssertFailed(const char *file, uint32_t line)
{
    HAL_DBG_ERR("assert failed at %s %lu\n", file, line);
    while (1) {
        ;
    }
}

/**
 * @brief  format hex print.
 * @param  s: head tag for every new line.
 * @param  buf: buffer for printing.
 * @param  width: every single printed object width.
 * @param  len: the number of printed objects.
 * @return HAL_Status: HAL_OK.
 * sum = width * len (BYTE).
 */
HAL_Status HAL_DBG_HEX(char *s, void *buf, uint32_t width, uint32_t len)
{
#ifdef HAL_DBG_ON
    uint32_t i, j;
    unsigned char *p8 = (unsigned char *)buf;
    unsigned short *p16 = (unsigned short *)buf;
    uint32_t *p32 = (uint32_t *)buf;

    j = 0;
    for (i = 0; i < len; i++) {
        if (j == 0) {
            HAL_SYSLOG("[HAL_DBG_HEX] %s %p + 0x%lx:", s, buf, i * width);
        }

        if (width == 4) {
            HAL_SYSLOG("0x%08lx,", p32[i]);
        } else if (width == 2) {
            HAL_SYSLOG("0x%04x,", p16[i]);
        } else {
            HAL_SYSLOG("0x%02x,", p8[i]);
        }

        if (++j >= 16) {
            j = 0;
            HAL_SYSLOG("\n");
        }
    }
    HAL_SYSLOG("\n");
#endif

    return HAL_OK;
}

#ifdef HAL_DBG_USING_HAL_PRINTF
static void reverse(char *start, char *end)
{
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

#ifdef __GNUC__
extern int _write(int fd, char *ptr, int len);
#endif

/**
 * @brief  format and print data
 * @param  format: format printf param. only support: \%d, \%s, \%ld, \%lld
 * @return int32_t.
 */
__WEAK int32_t HAL_DBG_Printf(const char *format, ...)
{
    static char g_printf_buf[HAL_PRINTF_BUF_SIZE];
    char *str = g_printf_buf;
    int32_t len = 0;
    va_list args;

    va_start(args, format);

    while (*format != '\0') {
        if (*format == '%') {
            format++;
            if (*format == 'd') {
                int i = va_arg(args, int);
                char *start = str;
                do {
                    *str++ = '0' + (i % 10);
                    i /= 10;
                } while (i > 0);
                reverse(start, str - 1);
            } else if (*format == 's') {
                char *s = va_arg(args, char *);
                while (*s) {
                    *str++ = *s++;
                }
            } else if (*format == 'l') {
                format++;
                if (*format == 'd') {
                    long i = va_arg(args, long);
                    char *start = str;
                    do {
                        *str++ = '0' + (i % 10);
                        i /= 10;
                    } while (i > 0);
                    reverse(start, str - 1);
                } else if (*format == 'l') {
                    format++;
                    if (*format == 'd') {
                        long long int i = va_arg(args, long long int);
                        char *start = str;
                        do {
                            *str++ = '0' + (i % 10);
                            i /= 10;
                        } while (i > 0);
                        reverse(start, str - 1);
                    }
                }
            }
        } else {
            *str++ = *format;
        }
        format++;
    }

    *str = '\0';

    va_end(args);
    len = str - g_printf_buf;

#ifdef __GNUC__

    return _write(2, g_printf_buf, len);
#else
    for (int i = 0; i < len; i++) {
        fputc(g_printf_buf[i], stdout);
    }

    return len;
#endif
}
#else
/**
 * @brief  format and print data
 * @param  format: format printf param.
 * @return int32_t.
 */
__WEAK int32_t HAL_DBG_Printf(const char *format, ...)
{
    return 0;
}
#endif /* HAL_DBG_USING_HAL_PRINTF */

/** @} */

/** @} */

/** @} */
