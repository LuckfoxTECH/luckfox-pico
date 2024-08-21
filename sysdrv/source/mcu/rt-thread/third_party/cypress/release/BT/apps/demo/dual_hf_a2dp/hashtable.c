/*
 * $ Copyright Cypress Semiconductor $
 */

/** @hashtable.c
 *
 * File Description
 *
 */

#include "hashtable.h"

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

wiced_app_service_cell_t *wiced_service_array[WICED_MAXIMUM_PRIORITY_LEVELS];

/******************************************************
 *               Function Definitions
 ******************************************************/

/*******************************************************************************
**
** Function         wiced_init_hashtable
**
** Description      This function initializes the service table with service
**                  entries.
**
** Returns          void
*******************************************************************************/
void wiced_init_hashtable()
{
    int i = 0;

    for (i = 0; i < WICED_MAXIMUM_PRIORITY_LEVELS; i++)
    {
        wiced_service_array[i] = NULL;
    }

}

/*******************************************************************************
**
** Function         wiced_add_entry
**
** Description      This function adds an entry to the service table.
**
** Returns          void
*******************************************************************************/
wiced_result_t wiced_add_entry(wiced_app_service_t *service)
{
    wiced_result_t result = WICED_SUCCESS;
    wiced_app_service_cell_t *cell_ptr = NULL;
    int i;

    if (service == NULL)
    {
        wiced_assert("[wiced_add_entry] Error: Service Entry is NULL", service != NULL);
        return WICED_ERROR;
    }

    cell_ptr = wiced_service_array[service->priority];

    if (cell_ptr == NULL)
    {
        if ((cell_ptr = (wiced_app_service_cell_t *)rt_malloc(sizeof(wiced_app_service_cell_t))) == NULL)
        {
            wiced_assert("[wiced_add_entry] Error: Service Entry is NULL", cell_ptr != NULL);
            return WICED_FALSE;
        }

        memcpy(&cell_ptr->service, service, sizeof(wiced_app_service_t));
        cell_ptr->next = NULL;
        wiced_service_array[service->priority] = cell_ptr;
    }
    else
    {
        for (i = 0; cell_ptr->next != NULL; i++) // check
        {
            cell_ptr = cell_ptr->next;
        }

        if (cell_ptr == NULL)
        {
            wiced_assert("[%s:%s] Error: Service Entry is NULL", cell_ptr != NULL);
            return WICED_FALSE;
        }
        cell_ptr->next = (wiced_app_service_cell_t *)rt_malloc(sizeof(wiced_app_service_cell_t));

        if (cell_ptr->next == NULL)
            return WICED_FALSE;

        cell_ptr = cell_ptr->next;
        memcpy(&cell_ptr->service, service, sizeof(wiced_app_service_t));
        cell_ptr->next = NULL;
    }

    return result;
}

/*******************************************************************************
**
** Function         wiced_get_entry
**
** Description      This function gets an entry from the service table.
**
** Returns          pointer to a service structure.
*******************************************************************************/
wiced_app_service_t *wiced_get_entry(int service_id)
{
    int i = 0;
    wiced_app_service_cell_t *cell_ptr = NULL;

    for (i = 0; i < WICED_MAXIMUM_PRIORITY_LEVELS ; i++)
    {
        cell_ptr = wiced_service_array[i];

        if (cell_ptr == NULL) continue;

        if (cell_ptr->service.type == service_id)
        {
            return &cell_ptr->service;
        }
        else
        {
            for (; cell_ptr->next != NULL;)
            {
                cell_ptr = cell_ptr->next;
                if (cell_ptr->service.type == service_id)
                {
                    return &cell_ptr->service;
                }
            }
        }

    }

    return NULL;
}

wiced_result_t wiced_remove_entry(void)
{
    int i = 0;
    wiced_app_service_cell_t *cell_ptr = NULL;

    for (i = 0; i < WICED_MAXIMUM_PRIORITY_LEVELS ; i++)
    {
        cell_ptr = wiced_service_array[i];

        if (cell_ptr == NULL) continue;

        wiced_assert("cell_ptr->next is not NULL", cell_ptr->next == NULL);

        rt_free(cell_ptr);
        cell_ptr = NULL;
    }

    memset(wiced_service_array, 0, sizeof(wiced_service_array));

    return WICED_SUCCESS;
}

