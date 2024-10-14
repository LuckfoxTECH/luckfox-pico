/*
 * $ Copyright Cypress Semiconductor $
 */

/** @file
 *
 */

#include "string.h"
#include "linked_list.h"

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *               Static Function Declarations
 ******************************************************/

/******************************************************
 *               Variable Definitions
 ******************************************************/

/******************************************************
 *               Function Definitions
 ******************************************************/

wiced_result_t linked_list_init(linked_list_t *list)
{
    if (list == NULL)
    {
        return WICED_BADARG;
    }

    memset(list, 0, sizeof(*list));
    return WICED_SUCCESS;
}

wiced_result_t linked_list_deinit(linked_list_t *list)
{
    linked_list_node_t *current;

    if (list == NULL)
    {
        return WICED_BADARG;
    }

    /* Traverse through all nodes and detach them */
    current = list->front;

    while (current != NULL)
    {
        linked_list_node_t *next = current->next;

        /* Detach node from the list */
        current->prev = NULL;
        current->next = NULL;

        /* Move to the next node */
        current = next;
    }

    memset(list, 0, sizeof(*list));
    return WICED_SUCCESS;
}

wiced_result_t linked_list_get_count(linked_list_t *list, uint32_t *count)
{
    if (list == NULL)
    {
        return WICED_BADARG;
    }

    *count = list->count;

    return WICED_SUCCESS;
}

wiced_result_t linked_list_set_node_data(linked_list_node_t *node, const void *data)
{
    if (node == NULL)
    {
        return WICED_BADARG;
    }

    node->data = (void *)data;
    return WICED_SUCCESS;
}

wiced_result_t linked_list_get_front_node(linked_list_t *list, linked_list_node_t **front_node)
{
    if (list == NULL)
    {
        return WICED_BADARG;
    }

    if (list->count == 0)
    {
        *front_node = NULL;
        return WICED_NOT_FOUND;
    }

    *front_node = list->front;

    return WICED_SUCCESS;
}

wiced_result_t linked_list_get_rear_node(linked_list_t *list, linked_list_node_t **rear_node)
{
    if (list == NULL)
    {
        return WICED_BADARG;
    }

    if (list->count == 0)
    {
        *rear_node = NULL;
        return WICED_NOT_FOUND;
    }

    *rear_node = list->rear;

    return WICED_SUCCESS;
}

wiced_result_t linked_list_find_node(linked_list_t *list, linked_list_compare_callback_t callback, void *user_data, linked_list_node_t **node_found)
{
    linked_list_node_t *current;

    if (list == NULL || callback == NULL || node_found == NULL || list->count == 0)
    {
        return WICED_BADARG;
    }

    current = list->front;

    while (current != NULL)
    {
        if (callback(current, user_data) == WICED_TRUE)
        {
            *node_found = current;
            return WICED_SUCCESS;
        }

        current = current->next;
    }

    return WICED_NOT_FOUND;
}

wiced_result_t linked_list_insert_node_at_front(linked_list_t *list, linked_list_node_t *node)
{
    if (list == NULL || node == NULL)
    {
        return WICED_BADARG;
    }

    if (list->count == 0)
    {
        list->front = node;
        list->rear  = node;
        node->prev  = NULL;
        node->next  = NULL;
    }
    else
    {
        node->prev        = NULL;
        node->next        = list->front;
        list->front->prev = node;
        list->front       = node;
    }

    list->count++;

    return WICED_SUCCESS;
}

wiced_result_t linked_list_insert_node_at_rear(linked_list_t *list, linked_list_node_t *node)
{
    if (list == NULL || node == NULL)
    {
        return WICED_BADARG;
    }

    if (list->count == 0)
    {
        list->front = node;
        list->rear  = node;
        node->prev  = NULL;
        node->next  = NULL;
    }
    else
    {
        node->next       = NULL;
        node->prev       = list->rear;
        list->rear->next = node;
        list->rear       = node;
    }

    list->count++;

    return WICED_SUCCESS;
}

wiced_result_t linked_list_insert_node_before(linked_list_t *list, linked_list_node_t *reference_node, linked_list_node_t *node_to_insert)
{
    /* WARNING: User must make sure that reference_node is in the list */
    if (list == NULL || reference_node == NULL || node_to_insert == NULL || list->count == 0)
    {
        return WICED_BADARG;
    }

    if (list->count == 0)
    {
        return WICED_NOT_FOUND;
    }

    if (reference_node == list->front)
    {
        return linked_list_insert_node_at_front(list, node_to_insert);
    }
    else
    {
        node_to_insert->prev       = reference_node->prev;
        node_to_insert->prev->next = node_to_insert;
        node_to_insert->next       = reference_node;
        reference_node->prev       = node_to_insert;

        list->count++;

        return WICED_SUCCESS;
    }
}

wiced_result_t linked_list_insert_node_after(linked_list_t *list, linked_list_node_t *reference_node, linked_list_node_t *node_to_insert)
{
    /* WARNING: User must make sure that reference_node is in the list */
    if (list == NULL || reference_node == NULL || node_to_insert == NULL || list->count == 0)
    {
        return WICED_BADARG;
    }

    if (reference_node == list->rear)
    {
        return linked_list_insert_node_at_rear(list, node_to_insert);
    }
    else
    {
        node_to_insert->prev       = reference_node;
        node_to_insert->next       = reference_node->next;
        reference_node->next->prev = node_to_insert;
        reference_node->next       = node_to_insert;

        list->count++;

        return WICED_SUCCESS;
    }
}

wiced_result_t linked_list_remove_node(linked_list_t *list, linked_list_node_t *node)
{
    /* WARNING: User must make sure that node to remove is in the list */
    if (list == NULL || node == NULL)
    {
        return WICED_BADARG;
    }

    if (list->count == 0)
    {
        return WICED_NOT_FOUND;
    }

    if (list->count == 1)
    {
        list->front = NULL;
        list->rear  = NULL;
    }
    else if (node == list->front)
    {
        linked_list_node_t *removed_node;

        return linked_list_remove_node_from_front(list, &removed_node);
    }
    else if (node == list->rear)
    {
        linked_list_node_t *removed_node;

        return linked_list_remove_node_from_rear(list, &removed_node);
    }
    else
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    /* Make sure that detach node does not point to some arbitrary memory location */
    node->prev = NULL;
    node->next = NULL;

    list->count--;

    return WICED_SUCCESS;
}

wiced_result_t linked_list_remove_node_from_front(linked_list_t *list, linked_list_node_t **removed_node)
{
    if (list == NULL || removed_node == NULL || list->count == 0)
    {
        return WICED_BADARG;
    }

    *removed_node = list->front;

    if (list->count == 1)
    {
        list->front = NULL;
        list->rear  = NULL;
    }
    else
    {
        list->front       = list->front->next;
        list->front->prev = NULL;
    }

    /* Make sure node does not point to some arbitrary memory location */
    (*removed_node)->prev = NULL;
    (*removed_node)->next = NULL;

    list->count--;

    return WICED_SUCCESS;
}

wiced_result_t linked_list_remove_node_from_rear(linked_list_t *list, linked_list_node_t **removed_node)
{
    if (list == NULL || removed_node == NULL)
    {
        return WICED_BADARG;
    }

    if (list->count == 0)
    {
        return WICED_NOT_FOUND;
    }

    *removed_node = list->rear;

    if (list->count == 1)
    {
        list->front = NULL;
        list->rear  = NULL;
    }
    else
    {
        list->rear       = list->rear->prev;
        list->rear->next = NULL;
    }

    /* Make sure node does not point to some arbitrary memory location */
    (*removed_node)->prev = NULL;
    (*removed_node)->next = NULL;

    list->count--;

    return WICED_SUCCESS;
}
