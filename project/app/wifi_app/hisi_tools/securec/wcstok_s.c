/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2014-2018. All rights reserved.
 * Description: wcstok_s  function
 * Author: lishunda
 * Create: 2014-02-25
 */

#include "securecutil.h"


SECUREC_INLINE int SecIsInDelimitW(wchar_t ch, const wchar_t *strDelimit)
{
    const wchar_t *ctl = strDelimit;
    while (*ctl != L'\0' && *ctl != ch) {
        ++ctl;
    }
    return (int)(*ctl != L'\0');
}

/*
 * Find beginning of token (skip over leading delimiters).
 * Note that there is no token if this loop sets string to point to the terminal null.
 */
SECUREC_INLINE wchar_t *SecFindBeginW(wchar_t *strToken, const wchar_t *strDelimit)
{
    wchar_t *token = strToken;
    while (*token != L'\0') {
        if (SecIsInDelimitW(*token, strDelimit)) {
            ++token;
            continue;
        }
        /* Don't find any delimiter in string header, break the loop */
        break;
    }
    return token;
}

/*
 * Find the end of the token. If it is not the end of the string, put a null there.
 */
SECUREC_INLINE wchar_t *SecFindRestW(wchar_t *strToken, const wchar_t *strDelimit)
{
    wchar_t *token = strToken;
    while (*token != L'\0') {
        if (SecIsInDelimitW(*token, strDelimit)) {
            /* Find a delimiter, set string termintor */
            *token = L'\0';
            ++token;
            break;
        }
        ++token;
    }
    return token;
}

/*
 * Update Token wide character  function
 */
SECUREC_INLINE wchar_t *SecUpdateTokenW(wchar_t *strToken, const wchar_t *strDelimit, wchar_t **context)
{
    /* Point to updated position */
    wchar_t *token = SecFindRestW(strToken, strDelimit);
    /* Update the context */
    *context = token;
    /* Determine if a token has been found. */
    if (token == strToken) {
        return NULL;
    }
    return strToken;
}

/*
 * <NAME>
 *    wcstok_s
 *
 *
 * <FUNCTION DESCRIPTION>
 *   The  wcstok_s  function  is  the  wide-character  equivalent  of the strtok_s function
 *
 * <INPUT PARAMETERS>
 *    strToken               String containing token or tokens.
 *    strDelimit             Set of delimiter characters.
 *    context                Used to store position information between calls to
 *                               wcstok_s.
 *
 * <OUTPUT PARAMETERS>
 *    context               is updated
 * <RETURN VALUE>
 *   The  wcstok_s  function  is  the  wide-character  equivalent  of the strtok_s function
 */
wchar_t *wcstok_s(wchar_t *strToken, const wchar_t *strDelimit, wchar_t **context)
{
    wchar_t *orgToken = strToken;
    /* Validation section */
    if (context == NULL || strDelimit == NULL) {
        return NULL;
    }
    if (orgToken == NULL && *context == NULL) {
        return NULL;
    }
    /* If string==NULL, continue with previous string */
    if (orgToken == NULL) {
        orgToken = *context;
    }
    orgToken = SecFindBeginW(orgToken, strDelimit);
    return SecUpdateTokenW(orgToken, strDelimit, context);
}

