/******************************************************************************
*
* $ Copyright Broadcom Corporation $
*
*****************************************************************************/

#ifndef WICED_BT_LOGMSG
#define WICED_BT_LOGMSG

#include "wiced_result.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
 *                      Macros
 ******************************************************/
//#define ScrLog( trace_set_mask, fmt_str, ... ) LogMsg (trace_set_mask, fmt_str, ##__VA_ARGS__ )

#ifdef  RTT
#include <rtthread.h>
#define ScrLog( trace_set_mask, fmt, args... )   do{rt_kprintf("\n"); rt_kprintf(fmt, ##args);}while(0)

#endif
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
 *                 Global Variables
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/

/** \brief Initializes logging data structures
 *
 *   This function initializes the Bluetooth Audio logging infrastructure. Must be invoked
 *   prior to using any of the other logging APIs
 *
 *   \return WICED_SUCCESS : on success;
 *              WICED_ERROR    : if an error occurred
 *
 **/
wiced_result_t LogMsg_init(void);


/** \brief Shuts down the logging infrastructure
 *
 *      This function tears down the Bluetooth audio logging mechanism
 *
 *   \return WICED_SUCCESS : on success;
 *              WICED_ERROR    : if an error occurred
 **/
wiced_result_t LogMsg_cleanup(void);


/** \brief Turn on/off Bluetooth tracing
 *
 *    This function allows the caller to enable or disable logging of BT library traces.
 *
 *   \param enable    tracing is enabled when this is set to TRUE, else disabled.
 *
 **/
void LogMsg_enable_log(uint8_t enable);


/** \brief Log a trace buffer.
 *
 *    This function is called to output the trace data typically to a console
 *
 * \param trace_set_mask    Indicates the trace level one of TRACE_TYPE_WARNING,
 *                                    TRACE_TYPE_API, TRACE_TYPE_ERROR or TRACE_TYPE_DEBUG
 *
 * \param format   Specifies how subsequent arguments are converted.
 *
 */
//void LogMsg(uint32_t trace_set_mask, const char *format, ...);
#ifndef     LogMsg
#include <rtthread.h>

#define  LogMsg (trace_set_mask, fmt, args...)  do{ rt_kprintf(fmt, ##args);} while(0)

#endif

/** \brief Build a trace buffer.
 *
 *    This function is called to build a trace buffer based on the format string
 *
 *   \param buffer   Trace buffer to be built
 *
 *   \param format   Specifies how subsequent arguments are converted.
 *
 *   \return WICED_SUCCESS : on success;
 *              WICED_ERROR    : if an error occurred
 */
wiced_result_t LogMsg_build_trace_buffer(char *buffer, char *format, ...);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif //#ifndef WICED_BT_LOGMSG
