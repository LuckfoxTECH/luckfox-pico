/**
 * @file hashmap.h
 * @brief tuya hasmap module
 * @version 1.0
 * @date 2019-10-30
 *
 * @copyright Copyright (c) tuya.inc 2019
 *
 */

#ifndef __HASHMAP_H__
#define __HASHMAP_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief define some special error code for hashmap module
 *
 */
#define MAP_MISSING -2 /* No such element */
#define MAP_OMEM -1    /* Out of Memory */
#define MAP_OK 0       /* OK */

/**
 * @brief any_t is a pointer.  This allows you to put arbitrary structures in
 * the hashmap.
 */
typedef void *any_t;

/**
 * @brief map_t is a pointer to an internally maintained data structure.
 * Clients of this package do not need to know how hashmaps are
 * represented.  They see and manipulate only map_t's.
 */
typedef any_t map_t;

/**
 * @brief any_t_iter is a iterator which used to traverse the hashmap
 *
 */
typedef any_t *any_t_iter;

/**
 * @brief create a new empty hashmap
 *
 * @param[in] table_size the hash table size
 * @return a new empty hashmap
 */
map_t hashmap_new(unsigned int table_size);

/**
 * @brief Add an element to the hashmap
 *
 * @param[in] in the hashmap
 * @param[in] key the key of hash element
 * @param[in] data the data of hash element
 * @return MAP_OK on success, others on failed, please refer to the define of hashmap error code
 *
 * @note For same key, it does not replace it. it is inserted in the head of the list
 */
int hashmap_put(map_t in, const char *key, const any_t data);

/**
 * @brief get an element from the hashmap
 *
 * @param[in] in the hashmap
 * @param[in] key the key of the element
 * @param[out] arg the first value that the key matches
 * @return MAP_OK on success, others on failed, please refer to the define of hashmap error code
 */
int hashmap_get(map_t in, const char *key, any_t *arg);

/**
 * @brief traverse all data with same key
 *
 * @param[in] in the hashmap
 * @param[in] key the key of element
 * @param[inout] arg_iterator the traverse iterator
 * @return MAP_OK on success, others on failed, please refer to the define of hashmap error code
 *
 * @note if arg_iterator is NULL, fetch the first element, otherwise, fetch the next element
 */
int hashmap_data_traversal(map_t in, const char *key, any_t_iter *arg_iterator);

/**
 * @brief traverse all data with same key
 *
 */
#define HASHMAP_FOR_EACH_DATA(in, key, data_iter)                                                  \
	for (data_iter = NULL; hashmap_data_traversal(in, key, (any_t_iter *)&data_iter) == MAP_OK;    \
	     /*empty*/)

/**
 * @brief remove an element from the hashmap
 *
 * @param[in] in the hashmap
 * @param[in] key the key of element
 * @param[in] data the data of the element
 * @return MAP_OK on success, others on failed, please refer to the define of hashmap error code
 *
 * @note if data is NULL,then delete the first note match key.if data is not null, then delete the
 * node match key and data.
 */
int hashmap_remove(map_t in, char *key, any_t data);

/**
 * @brief free the hashmap
 *
 * @param[in] in the hashmap need to free
 *
 * @warning must remove all element first, otherwise, it will cause element leak
 */
void hashmap_free(map_t in);

/**
 * @brief get current size of the hashmap
 *
 * @param[in] in the hashmap
 * @return the current size
 */
int hashmap_length(map_t in);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __HASHMAP_H__
