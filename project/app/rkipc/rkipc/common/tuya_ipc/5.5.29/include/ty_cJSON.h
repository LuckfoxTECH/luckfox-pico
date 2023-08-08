/*
  Copyright (c) 2009 Dave Gamble

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#ifndef ty_cJSON__h
#define ty_cJSON__h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

/* ty_cJSON Types: */
#define ty_cJSON_False 0
#define ty_cJSON_True 1
#define ty_cJSON_NULL 2
#define ty_cJSON_Number 3
#define ty_cJSON_String 4
#define ty_cJSON_Array 5
#define ty_cJSON_Object 6

#define ty_cJSON_IsReference 256

/* The ty_cJSON structure: */
typedef struct ty_cJSON {
	struct ty_cJSON *next, *prev; /* next/prev allow you to walk array/object chains. Alternatively,
	                                 use GetArraySize/GetArrayItem/GetObjectItem */
	struct ty_cJSON *child; /* An array or object item will have a child pointer pointing to a chain
	                           of the items in the array/object. */

	int type; /* The type of the item, as above. */

	char *valuestring;  /* The item's string, if type==ty_cJSON_String */
	int valueint;       /* The item's number, if type==ty_cJSON_Number */
	double valuedouble; /* The item's number, if type==ty_cJSON_Number */

	char *string; /* The item's name string, if this item is the child of, or is in the list of
	                 subitems of an object. */
} ty_cJSON;

/* Supply a block of JSON, and this returns a ty_cJSON object you can interrogate. Call
 * ty_cJSON_Delete when finished. */
extern ty_cJSON *ty_cJSON_Parse(const char *value);
/* Render a ty_cJSON entity to text for transfer/storage. Free the char* when finished. */
extern char *ty_cJSON_Print(ty_cJSON *item);
/* Render a ty_cJSON entity to text for transfer/storage without any formatting. Free the char* when
 * finished. */
extern char *ty_cJSON_PrintUnformatted(ty_cJSON *item);
extern void ty_cJSON_FreeBuffer(char *buffer);
/* Delete a ty_cJSON entity and all subentities. */
extern void ty_cJSON_Delete(ty_cJSON *c);

/* Returns the number of items in an array (or object). */
extern int ty_cJSON_GetArraySize(ty_cJSON *array);
/* Retrieve item number "item" from array "array". Returns NULL if unsuccessful. */
extern ty_cJSON *ty_cJSON_GetArrayItem(ty_cJSON *array, int item);
/* Get item "string" from object. Case insensitive. */
extern ty_cJSON *ty_cJSON_GetObjectItem(ty_cJSON *object, const char *string);

/* For analysing failed parses. This returns a pointer to the parse error. You'll probably need to
 * look a few chars back to make sense of it. Defined when ty_cJSON_Parse() returns 0. 0 when
 * ty_cJSON_Parse() succeeds. */
extern const char *ty_cJSON_GetErrorPtr(void);

/* These calls create a ty_cJSON item of the appropriate type. */
extern ty_cJSON *ty_cJSON_CreateNull(void);
extern ty_cJSON *ty_cJSON_CreateTrue(void);
extern ty_cJSON *ty_cJSON_CreateFalse(void);
extern ty_cJSON *ty_cJSON_CreateBool(int b);
extern ty_cJSON *ty_cJSON_CreateNumber(double num);
extern ty_cJSON *ty_cJSON_CreateString(const char *string);
extern ty_cJSON *ty_cJSON_CreateArray(void);
extern ty_cJSON *ty_cJSON_CreateObject(void);

/* These utilities create an Array of count items. */
extern ty_cJSON *ty_cJSON_CreateIntArray(const int *numbers, int count);
extern ty_cJSON *ty_cJSON_CreateFloatArray(const float *numbers, int count);
extern ty_cJSON *ty_cJSON_CreateDoubleArray(const double *numbers, int count);
extern ty_cJSON *ty_cJSON_CreateStringArray(const char **strings, int count);

/* Append item to the specified array/object. */
extern void ty_cJSON_AddItemToArray(ty_cJSON *array, ty_cJSON *item);
extern void ty_cJSON_AddItemToObject(ty_cJSON *object, const char *string, ty_cJSON *item);
/* Append reference to item to the specified array/object. Use this when you want to add an existing
 * ty_cJSON to a new ty_cJSON, but don't want to corrupt your existing ty_cJSON. */
extern void ty_cJSON_AddItemReferenceToArray(ty_cJSON *array, ty_cJSON *item);
extern void ty_cJSON_AddItemReferenceToObject(ty_cJSON *object, const char *string, ty_cJSON *item);

/* Remove/Detatch items from Arrays/Objects. */
extern ty_cJSON *ty_cJSON_DetachItemFromArray(ty_cJSON *array, int which);
extern void ty_cJSON_DeleteItemFromArray(ty_cJSON *array, int which);
extern ty_cJSON *ty_cJSON_DetachItemFromObject(ty_cJSON *object, const char *string);
extern void ty_cJSON_DeleteItemFromObject(ty_cJSON *object, const char *string);

/* Update array items. */
extern void ty_cJSON_ReplaceItemInArray(ty_cJSON *array, int which, ty_cJSON *newitem);
extern void ty_cJSON_ReplaceItemInObject(ty_cJSON *object, const char *string, ty_cJSON *newitem);

/* Duplicate a ty_cJSON item */
extern ty_cJSON *ty_cJSON_Duplicate(ty_cJSON *item, int recurse);
/* Duplicate will create a new, identical ty_cJSON item to the one you pass, in new memory that will
need to be released. With recurse!=0, it will duplicate any children connected to the item.
The item->next and ->prev pointers are always zero on return from Duplicate. */

/* ParseWithOpts allows you to require (and check) that the JSON is null terminated, and to retrieve
 * the pointer to the final byte parsed. */
extern ty_cJSON *ty_cJSON_ParseWithOpts(const char *value, const char **return_parse_end,
                                        int require_null_terminated);

extern void ty_cJSON_Minify(char *json);

/*extern for p2p libuv */
extern ty_cJSON *ty_cJSON_GetObjectItemCaseSensitive(ty_cJSON *object, const char *string);
extern int ty_cJSON_IsObject(const ty_cJSON *object);
extern int ty_cJSON_IsArray(const ty_cJSON *object);
extern int ty_cJSON_IsString(const ty_cJSON *object);
extern int ty_cJSON_IsNumber(const ty_cJSON *object);
extern int ty_cJSON_IsBool(const ty_cJSON *object);

/* Macros for creating things quickly. */
#define ty_cJSON_AddNullToObject(object, name)                                                     \
	ty_cJSON_AddItemToObject(object, name, ty_cJSON_CreateNull())
#define ty_cJSON_AddTrueToObject(object, name)                                                     \
	ty_cJSON_AddItemToObject(object, name, ty_cJSON_CreateTrue())
#define ty_cJSON_AddFalseToObject(object, name)                                                    \
	ty_cJSON_AddItemToObject(object, name, ty_cJSON_CreateFalse())
#define ty_cJSON_AddBoolToObject(object, name, b)                                                  \
	ty_cJSON_AddItemToObject(object, name, ty_cJSON_CreateBool(b))
#define ty_cJSON_AddNumberToObject(object, name, n)                                                \
	ty_cJSON_AddItemToObject(object, name, ty_cJSON_CreateNumber(n))
#define ty_cJSON_AddStringToObject(object, name, s)                                                \
	ty_cJSON_AddItemToObject(object, name, ty_cJSON_CreateString(s))

/* When assigning an integer value, it needs to be propagated to valuedouble too. */
#define ty_cJSON_SetIntValue(object, val)                                                          \
	((object) ? (object)->valueint = (object)->valuedouble = (val) : (val))

/* Macro for iterating over an array */
#define ty_cJSON_ArrayForEach(pos, head) for (pos = (head)->child; pos != NULL; pos = pos->next)

#ifdef __cplusplus
}
#endif

#endif
