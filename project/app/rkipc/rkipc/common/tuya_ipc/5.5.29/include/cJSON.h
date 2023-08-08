
#ifndef cJSON__h
#define cJSON__h

#ifdef __cplusplus
extern "C" {
#endif

#include "ty_cJSON.h"

/* ty_cJSON Types: */
#define cJSON_False ty_cJSON_False
#define cJSON_True ty_cJSON_True
#define cJSON_NULL ty_cJSON_NULL
#define cJSON_Number ty_cJSON_Number
#define cJSON_String ty_cJSON_String
#define cJSON_Array ty_cJSON_Array
#define cJSON_Object ty_cJSON_Object

#define cJSON_IsReference ty_cJSON_IsReference

/* The cJSON structure: */
typedef struct ty_cJSON cJSON;

typedef struct ty_cJSON_Hooks cJSON_Hooks;

//#define cJSON_InitHooks ty_cJSON_InitHooks
#define cJSON_Parse ty_cJSON_Parse
#define cJSON_Print ty_cJSON_Print
#define cJSON_PrintUnformatted ty_cJSON_PrintUnformatted
#define cJSON_FreeBuffer ty_cJSON_FreeBuffer
#define cJSON_Delete ty_cJSON_Delete
#define cJSON_GetArraySize ty_cJSON_GetArraySize
#define cJSON_GetArrayItem ty_cJSON_GetArrayItem

#define cJSON_GetObjectItem ty_cJSON_GetObjectItem
#define cJSON_GetErrorPtr ty_cJSON_GetErrorPtr

#define cJSON_CreateNull ty_cJSON_CreateNull
#define cJSON_CreateTrue ty_cJSON_CreateTrue
#define cJSON_CreateFalse ty_cJSON_CreateFalse
#define cJSON_CreateBool ty_cJSON_CreateBool
#define cJSON_CreateNumber ty_cJSON_CreateNumber
#define cJSON_CreateString ty_cJSON_CreateString
#define cJSON_CreateArray ty_cJSON_CreateArray
#define cJSON_CreateObject ty_cJSON_CreateObject

#define cJSON_CreateIntArray ty_cJSON_CreateIntArray
#define cJSON_CreateFloatArray ty_cJSON_CreateFloatArray
#define cJSON_CreateDoubleArray ty_cJSON_CreateDoubleArray
#define cJSON_CreateStringArray ty_cJSON_CreateStringArray

#define cJSON_AddItemToArray ty_cJSON_AddItemToArray
#define cJSON_AddItemToObject ty_cJSON_AddItemToObject
#define cJSON_AddItemReferenceToArray ty_cJSON_AddItemReferenceToArray
#define cJSON_AddItemReferenceToObject ty_cJSON_AddItemReferenceToObject

#define cJSON_DetachItemFromArray ty_cJSON_DetachItemFromArray
#define cJSON_DeleteItemFromArray ty_cJSON_DeleteItemFromArray
#define cJSON_DetachItemFromObject ty_cJSON_DetachItemFromObject
#define cJSON_DeleteItemFromObject ty_cJSON_DeleteItemFromObject

#define cJSON_ReplaceItemInArray ty_cJSON_ReplaceItemInArray
#define cJSON_ReplaceItemInObject ty_cJSON_ReplaceItemInObject

#define cJSON_Duplicate ty_cJSON_Duplicate
#define cJSON_ParseWithOpts ty_cJSON_ParseWithOpts

#define cJSON_Minify ty_cJSON_Minify

#define cJSON_IsArray ty_cJSON_IsArray
#define cJSON_IsObject ty_cJSON_IsObject
#define cJSON_GetObjectItemCaseSensitive ty_cJSON_GetObjectItemCaseSensitive
#define cJSON_IsString ty_cJSON_IsString
#define cJSON_IsNumber ty_cJSON_IsNumber
#define cJSON_IsBool ty_cJSON_IsBool

#define cJSON_AddNullToObject(object, name)                                                        \
	ty_cJSON_AddItemToObject(object, name, ty_cJSON_CreateNull())
#define cJSON_AddTrueToObject(object, name)                                                        \
	ty_cJSON_AddItemToObject(object, name, ty_cJSON_CreateTrue())
#define cJSON_AddFalseToObject(object, name)                                                       \
	ty_cJSON_AddItemToObject(object, name, ty_cJSON_CreateFalse())
#define cJSON_AddBoolToObject(object, name, b)                                                     \
	ty_cJSON_AddItemToObject(object, name, ty_cJSON_CreateBool(b))
#define cJSON_AddNumberToObject(object, name, n)                                                   \
	ty_cJSON_AddItemToObject(object, name, ty_cJSON_CreateNumber(n))
#define cJSON_AddStringToObject(object, name, s)                                                   \
	ty_cJSON_AddItemToObject(object, name, ty_cJSON_CreateString(s))

#define cJSON_SetIntValue(object, val)                                                             \
	((object) ? (object)->valueint = (object)->valuedouble = (val) : (val))

#define cJSON_ArrayForEach(pos, head) for (pos = (head)->child; pos != NULL; pos = pos->next)

#ifdef __cplusplus
}
#endif

#endif
