/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: A parameter is added to the security C APIs based on the standard C interface, that is, the upper
 *              limit of the write operation address space to prevent out-of-bounds write.
 * Author: Hisilicon
 * Create: 2018-08-04
 */

/**
 * @defgroup iot_secure_c Secure C library APIs
 * @ingroup iot_secure
 */

#ifndef __HI_STDLIB_H__
#define __HI_STDLIB_H__
#include <hi_types_base.h>

HI_START_HEADER

#ifndef EOK
#define EOK 0
#endif

#ifndef errno_t
typedef int errno_t;
#endif

#ifndef size_t
typedef unsigned int size_t;
#endif

/**
* @ingroup  iot_secure_c
* @brief  Copies the source string to the destination buffer.CNcomment:����Դ�ַ�����Ŀ�Ļ�������CNend
*
* @par ����: Copies the source string to the destination buffer.
CNcomment:����Դ�ַ�����Ŀ�Ļ�������CNend
*
* @attention None
*
* @param dest     [OUT] type #char *, Destination buffer.CNcomment:Ŀ�Ļ�������CNend
* @param dest_max [IN]  type #size_t, Size of the destination buffer.CNcomment:Ŀ�Ļ�������С��CNend
* @param src      [IN]  type #const #char *, Source buffer.CNcomment:Դ��������CNend
*
* @retval #EOK Success
* @retval #Other Failure
*
* @par ����:
* @li hi_stdlib.h: This file declares the APIs.CNcomment:�ýӿ��������ڵ�ͷ�ļ���CNend
* @see None
* @since Hi3861_V100R001C00
*/
extern errno_t strcpy_s(char *dest, size_t dest_max, const char *src);

/**
* @ingroup  iot_secure_c
* @brief  Copies the source string of a specified length to the destination buffer.
CNcomment:����ָ������Դ�ַ�����Ŀ�Ļ�������CNend
*
* @par ����:Copies the source string of a specified length to the destination buffer.
CNcomment:����ָ������Դ�ַ�����Ŀ�Ļ�������CNend
* @attention None
*
* @param dest     [IN] type #char *, Destination buffer.CNcomment:Ŀ�Ļ�������CNend
* @param dest_max [IN] type #size_t, Size of the destination buffer.CNcomment:Ŀ�Ļ�������С��CNend
* @param src      [IN] type #const #char *, Source buffer.CNcomment:Դ��������CNend
* @param count    [IN] type #size_t, Number of characters copied from the source buffer.
CNcomment:��Դ�������и��Ƶ��ַ�����CNend
*
* @retval #EOK Success
* @retval #Other Failure
*
* @par ����:
* @li hi_stdlib.h: This file declares the APIs.CNcomment:�ýӿ��������ڵ�ͷ�ļ���CNend
* @see None
* @since Hi3861_V100R001C00
*/
extern errno_t strncpy_s(char *dest, size_t dest_max, const char *src, size_t count);

/**
* @ingroup  iot_secure_c
* @brief  Concatenates the source string to the end of the destination string.
CNcomment:��Դ�ַ������ӵ�Ŀ���ַ�������CNend
*
* @par ����:Concatenates the source string to the end of the destination string.
CNcomment:��Դ�ַ������ӵ�Ŀ���ַ������档CNend
* @attention None
*
* @param dest     [IN] type #char *, Destination buffer.CNcomment:Ŀ�Ļ�������CNend
* @param dest_max [IN] type #size_t, Size of the destination buffer.CNcomment:Ŀ�Ļ�������С��CNend
* @param src      [IN] type #const #char *, Source buffer.CNcomment:Դ��������CNend
*
* @retval #EOK Success
* @retval #Other Failure
*
* @par ����:
* @li hi_stdlib.h: This file declares the APIs.CNcomment:�ýӿ��������ڵ�ͷ�ļ���CNend
* @see None
* @since Hi3861_V100R001C00
*/
extern errno_t strcat_s(char *dest, size_t dest_max, const char *src);

/**
* @ingroup  iot_secure_c
* @brief  Concatenates the source string of a specified length to the end of the destination string.
CNcomment:��ָ�����ȵ�Դ�ַ������ӵ�Ŀ���ַ������档CNend
*
* @par ����: Concatenates the source string of a specified length to the end of the destination string.
CNcomment:��ָ�����ȵ�Դ�ַ������ӵ�Ŀ���ַ������档CNend
* @attention None
*
* @param dest     [IN] type #char *, Destination buffer.CNcomment:Ŀ�Ļ�������CNend
* @param dest_max [IN] type #size_t, Size of the destination buffer.CNcomment:Ŀ�Ļ�������С��CNend
* @param src      [IN] type #const #char *, Source buffer.CNcomment:Դ��������CNend
* @param count    [IN] type #size_t, Number of characters copied from the source buffer.
CNcomment:��Դ���������ӵ��ַ�����CNend
*
* @retval #EOK Success
* @retval #Other Failure
*
* @par ����:
* @li hi_stdlib.h: This file declares the APIs.CNcomment:�ýӿ��������ڵ�ͷ�ļ���CNend
* @see None
* @since Hi3861_V100R001C00
*/
extern errno_t strncat_s(char *dest, size_t dest_max, const char *src, size_t count);

/**
* @ingroup  iot_secure_c
* @brief  Copies the data from the source buffer to the destination buffer.
CNcomment:����Դ�����������ݵ�Ŀ�Ļ�������CNend
*
* @par ����: Copies the data from the source buffer to the destination buffer.
CNcomment:����Դ�����������ݵ�Ŀ�Ļ�������CNend
* @attention None
*
* @param dest     [IN] type #char *, Destination buffer.CNcomment:Ŀ�Ļ�������CNend
* @param dest_max [IN] type #size_t, Size of the destination buffer.CNcomment:Ŀ�Ļ�������С��CNend
* @param src      [IN] type #const #char *, Source buffer.CNcomment:Դ��������CNend
* @param count    [IN] type #size_t, Number of characters copied from the source buffer.
CNcomment:��Դ�������и��Ƶ��ַ�����CNend
*
* @retval #EOK Success
* @retval #Other Failure
*
* @par ����:
* @li hi_stdlib.h: This file declares the APIs.CNcomment:�ýӿ��������ڵ�ͷ�ļ���CNend
* @see None
* @since Hi3861_V100R001C00
*/
extern errno_t memcpy_s(void *dest, size_t dest_max, const void *src, size_t count);

/**
* @ingroup  iot_secure_c
* @brief  Sets the size of the destination buffer to a specific value.
CNcomment:����Ŀ�Ļ�����Ϊ�ض�ֵ��CNend
*
* @par ����: Sets the size of the destination buffer to a specific value.
CNcomment:����Ŀ�Ļ�����Ϊ�ض�ֵ��CNend
* @attention None
*
* @param dest     [IN] type #char *, Destination buffer.CNcomment:Ŀ�Ļ�������CNend
* @param dest_max [IN] type #size_t, Size of the destination buffer.CNcomment:Ŀ�Ļ�������С��CNend
* @param c        [IN] type #const #char *, Source buffer.CNcomment:�ض�ֵ��CNend
* @param count    [IN] type #size_t, Number of characters copied from the source buffer.
CNcomment:����Ϊ�ض�ֵ���ַ�����CNend
*
* @retval #EOK Success
* @retval #Other Failure
*
* @par ����:
* @li hi_stdlib.h: This file declares the APIs.CNcomment:�ýӿ��������ڵ�ͷ�ļ���CNend
* @see None
* @since Hi3861_V100R001C00
*/
extern errno_t memset_s(void *dest, size_t dest_max, int c, size_t count);

/**
* @ingroup  iot_secure_c
* @brief  Moves the data from the source buffer to the destination buffer.
CNcomment:�ƶ�Դ�����������ݵ�Ŀ�Ļ�������CNend
*
* @par ����:Moves the data from the source buffer to the destination buffer.
CNcomment:�ƶ�Դ�����������ݵ�Ŀ�Ļ�������CNend
* @attention None
*
* @param dest     [IN] type #char *, Destination buffer.CNcomment:Ŀ�Ļ�������CNend
* @param dest_max [IN] type #size_t, Size of the destination buffer.CNcomment:Ŀ�Ļ�������С��CNend
* @param src      [IN] type #const #char *, Source buffer.CNcomment:Դ��������CNend
* @param count    [IN] type #size_t, Number of characters copied from the source buffer.
CNcomment:��Դ���������ƶ����ַ�����CNend
*
* @retval #EOK Success
* @retval #Other Failure
*
* @par ����:
* @li hi_stdlib.h: This file declares the APIs.CNcomment:�ýӿ��������ڵ�ͷ�ļ���CNend
* @see None
* @since Hi3861_V100R001C00
*/
extern errno_t memmove_s(void *dest, size_t dest_max, const void *src, size_t count);

/**
* @ingroup  iot_secure_c
* @brief  Splits a string into substrings according to the specified separators.
CNcomment:���ַ�������ָ���ķָ����ָ�����ַ�����CNend
*
* @par ����: Splits a string into substrings according to the specified separators.
CNcomment:���ַ�������ָ���ķָ����ָ�����ַ�����CNend
* @attention None
*
* @param token   [IN] type #char *�� String to be split.CNcomment:Ҫ�ָ���ַ�����CNend
* @param delimit [IN] type #const char *�� String separator.CNcomment:�ַ����ָ�����CNend
* @param context [IN] type #char* ��Position information after a call to HI_strtok_s is saved.
CNcomment:�������HI_strtok_s���λ����Ϣ��CNend
*
* @retval #char* Point to the next token. CNcomment:ָ����token�е���һ��token��CNend
* @retval #HI_NULL A specified substring is not found or an error occurs.
CNcomment:û���ҵ�ָ�������ַ������߷�������CNend
*
* @par ����:
* @li hi_stdlib.h: This file declares the APIs.CNcomment:�ýӿ��������ڵ�ͷ�ļ���CNend
* @see None
* @since Hi3861_V100R001C00
*/
extern char *strtok_s(char *token, const char *delimit, char **context);

/**
* @ingroup  iot_secure_c
* @brief  Formats the data and outputs it to the destination buffer.
CNcomment:�����ݸ�ʽ�������Ŀ�Ļ�������CNend
*
* @par ����: Formats the data and outputs it to the destination buffer.
CNcomment:�����ݸ�ʽ�������Ŀ�Ļ�������CNend
* @attention None
*
* @param dest     [OUT] type #char *�� Destination buffer.CNcomment:Ŀ�Ļ�������CNend
* @param dest_max [IN]  type #size_t�� Size of the destination buffer.CNcomment:Ŀ�Ļ�������С��CNend
* @param format   [IN]  type #const #char *�� Formatting control string.CNcomment:��ʽ�������ַ�����CNend
* @param ...      [IN]  Optional parameter CNcomment:��ѡ������CNend
*
* @retval #>=0 Return the number of bytes stored in dest, not counting the terminating null character.
CNcomment:���ش洢��dest���ֽ�����������������CNend
* @retval #-1 Failure
*
* @par ����:
* @li hi_stdlib.h: This file declares the APIs.CNcomment:�ýӿ��������ڵ�ͷ�ļ���CNend
* @see None
* @since Hi3861_V100R001C00
*/
extern int sprintf_s(char *dest, size_t dest_max, const char *format, ...);

/**
* @ingroup  iot_secure_c
* @brief Formats the data according to a specified length and outputs the data to the destination buffer.
CNcomment:�����ݰ���ָ�����ȸ�ʽ�������Ŀ�Ļ�������CNend
*
* @par ����: Formats the data according to a specified length and outputs the data to the destination buffer.
CNcomment:�����ݰ���ָ�����ȸ�ʽ�������Ŀ�Ļ�������CNend
* @attention None
*
* @param dest     [OUT] type #char *�� Destination buffer.CNcomment:Ŀ�Ļ�������CNend
* @param dest_max [IN]  type #size_t�� Size of the destination buffer.CNcomment:Ŀ�Ļ�������С��CNend
* @param count    [IN]  type #size_t�� Number of formatted characters to be output to the destination buffer.
CNcomment:Ҫ�����Ŀ�Ļ������ĸ�ʽ���ַ�������CNend
* @param format   [IN]  type #const #char *�� Formatting control string.CNcomment:��ʽ�������ַ�����CNend
* @param ...      [IN]  Optional parameter CNcomment:��ѡ������CNend
*
* @retval #>=0 Return the number of bytes stored in dest, not counting the terminating null character.
CNcomment:���ش洢��dest���ֽ�����������������CNend
* @retval #-1 Failure
*
* @par ����:
* @li hi_stdlib.h: This file declares the APIs.CNcomment:�ýӿ��������ڵ�ͷ�ļ���CNend
* @see None
* @since Hi3861_V100R001C00
*/
extern int snprintf_s(char *dest, size_t dest_max, size_t count, const char *format, ...);

/*
 * C��ӿ�
 */
extern int memcmp(const void *str1, const void *str2, size_t n);
extern size_t strlen(const char *src);
extern int strcmp(const char *str1, const char *str2);
extern int strncmp(const char *str1, const char *str2, size_t n);

#if defined(HAVE_PCLINT_CHECK)
#define UT_CONST const
#else
#define UT_CONST
#endif

extern unsigned long strtoul(const char *nptr, char **endptr, int base);
extern UT_CONST char *strstr(const char *str1, const char *str2);
extern UT_CONST char *strchr(const char *s, int c);

HI_END_HEADER
#endif /* __HI_STDLIB_H__ */

