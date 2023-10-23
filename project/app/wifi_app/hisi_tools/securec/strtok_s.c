/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2014-2018. All rights reserved.
 * Description: strtok_s  function
 * Author: lishunda
 * Create: 2014-02-25
 */

#include "securecutil.h"


SECUREC_INLINE int SecIsInDelimit(char ch, const char *strDelimit)
{
    const char *ctl = strDelimit;
    while (*ctl != '\0' && *ctl != ch) {
        ++ctl;
    }
    return (int)(*ctl != '\0');
}

/*
 * Find beginning of token (skip over leading delimiters).
 * Note that there is no token if this loop sets string to point to the terminal null.
 */
SECUREC_INLINE char *SecFindBegin(char *strToken, const char *strDelimit)
{
    char *token = strToken;
    while (*token != '\0') {
        if (SecIsInDelimit(*token, strDelimit)) {
            ++token;
            continue;
        }
        /* Don't find any delimiter in string header, break the loop */
        break;
    }
    return token;
}

/*
 * Find rest of token
 */
SECUREC_INLINE char *SecFindRest(char *strToken, const char *strDelimit)
{
    /* Find the rest of the token. If it is not the end of the string, put a null there */
    char *token = strToken;
    while (*token != '\0') {
        if (SecIsInDelimit(*token, strDelimit)) {
            /* Find a delimiter, set string termintor */
            *token = '\0';
            ++token;
            break;
        }
        ++token;
    }
    return token;
}

/*
 * Find the final position pointer
 */
SECUREC_INLINE char *SecUpdateToken(char *strToken, const char *strDelimit, char **context)
{
    /* Point to updated position */
    char *token = SecFindRest(strToken, strDelimit);
    /* Record string position for next search in the context */
    *context = token;
    /* Determine if a token has been found. */
    if (token == strToken) {
        return NULL;
    }
    return strToken;
}

/*
 * <FUNCTION DESCRIPTION>
 *    The strtok_s function parses a string into a sequence of strToken,
 *    replace all characters in strToken string that match to strDelimit set with 0.
 *    On the first call to strtok_s the string to be parsed should be specified in strToken.
 *    In each subsequent call that should parse the same string, strToken should be NULL
 * <INPUT PARAMETERS>
 *    strToken            String containing token or tokens.
 *    strDelimit          Set of delimiter characters.
 *    context             Used to store position information between calls
 *                             to strtok_s
 * <OUTPUT PARAMETERS>
 *   context               is updated
 * <RETURN VALUE>
 *   On the first call returns the address of the first non \0 character, otherwise NULL is returned.
 *   In subsequent calls, the strtoken is set to NULL, and the context set is the same as the previous call,
 *   return NULL if the *context string length is equal 0, otherwise return *context.
 */
char *strtok_s(char *strToken, const char *strDelimit, char **context)
{
    char *orgToken = strToken;
    /* Validate delimiter and string context */
    if (context == NULL || strDelimit == NULL) {
        return NULL;
    }
    /* Valid input string and string pointer from where to search */
    if (orgToken == NULL && *context == NULL) {
        return NULL;
    }
    /* If string is null, continue searching from previous string position stored in context */
    if (orgToken == NULL) {
        orgToken = *context;
    }
    orgToken = SecFindBegin(orgToken, strDelimit);
    return SecUpdateToken(orgToken, strDelimit, context);
}
#if SECUREC_IN_KERNEL
EXPORT_SYMBOL(strtok_s);
#endif

