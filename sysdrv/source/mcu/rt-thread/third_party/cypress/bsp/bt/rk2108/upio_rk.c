/*****************************************************************************
 **
 **  Name    upio_bby.c
 **
 **  Description
 **  This file contains the universal driver wrapper for the BTE-QC pio
 **  drivers
 **
 **  Copyright (c) 2001-2004, WIDCOMM Inc., All Rights Reserved.
 **  WIDCOMM Bluetooth Core. Proprietary and confidential.
 *****************************************************************************/
#include "bt_target.h"
#include "gki.h"
#include "upio.h"
#include "platform_bluetooth.h"
#include <rtthread.h>
#include <rtdevice.h>
#include <stdbool.h>
#include "hal_base.h"
#include "hal_pinctrl.h"


/******************************************************
 *               Variables Definitions
 ******************************************************/

//static volatile wiced_bool_t bus_initialised = WICED_FALSE;
//static volatile wiced_bool_t device_powered = WICED_FALSE;

/*******************************************************************************
 **  UPIO Driver functions
 *******************************************************************************/

/*****************************************************************************
 **
 ** Function         UPIO_Init
 **
 ** Description
 **      Initialize the GPIO service.
 **      This function is typically called once upon system startup.
 **
 ** Returns          nothing
 **
 *****************************************************************************/
UDRV_API void UPIO_Init(void *p_cfg)
{
    DRV_TRACE_DEBUG0("UPIO_Init");
}

/*****************************************************************************
 **
 ** Function         UPIO_Set
 **
 ** Description
 **      This function sets one or more GPIO devices to the given state.
 **      Multiple GPIOs of the same type can be masked together to set more
 **      than one GPIO. This function can only be used on types UPIO_LED and
 **      UPIO_GENERAL.
 **
 ** Input Parameters:
 **      type    The type of device.
 **      pio     Indicates the particular GPIOs.
 **      state   The desired state.
 **
 ** Output Parameter:
 **      None.
 **
 ** Returns:
 **      None.
 **
 *****************************************************************************/
UDRV_API void UPIO_Set(tUPIO_TYPE type, tUPIO pio, tUPIO_STATE state)
{
#if (defined(HCILP_INCLUDED) && HCILP_INCLUDED == TRUE)
    DRV_TRACE_DEBUG2("GPIO set (%d)->%d\n", pio, state);

    if (UPIO_GENERAL == type)
    {
        if (pio == WICED_BT_PIN_DEVICE_WAKE)
        {
            HAL_PINCTRL_SetIOMUX(GPIO_BANK1, GPIO_PIN_B7, PIN_CONFIG_MUX_FUNC0);
            HAL_GPIO_SetPinDirection(GPIO1, GPIO_PIN_B7, GPIO_OUT);
            if (state == UPIO_OFF)
            {
                HAL_GPIO_SetPinLevel(GPIO1, GPIO_PIN_B7, GPIO_LOW);
            }
            else
            {
                HAL_GPIO_SetPinLevel(GPIO1, GPIO_PIN_B7, GPIO_HIGH);
            }
        }
    }
#endif
}

/*****************************************************************************
 **
 ** Function         UPIO_Read
 **
 ** Description
 **      Read the state of a GPIO. This function can be used for any type of
 **      device. Parameter pio can only indicate a single GPIO; multiple GPIOs
 **      cannot be masked together.
 **
 ** Input Parameters:
 **      Type:  The type of device.
 **      pio:    Indicates the particular GUPIO.
 **
 ** Output Parameter:
 **      None.
 **
 ** Returns:
 **      State of GPIO (UPIO_ON or UPIO_OFF).
 **
 *****************************************************************************/
UDRV_API tUPIO_STATE UPIO_Read(tUPIO_TYPE type, tUPIO pio)
{
    tUPIO_STATE state = UPIO_OFF;
#if (defined(HCILP_INCLUDED) && HCILP_INCLUDED == TRUE)
    if (UPIO_GENERAL == type)
    {
        if (pio == WICED_BT_PIN_HOST_WAKE)
        {
            //rt_pin_read(PIN_BT_HOST);
        }
    }
#endif
    DRV_TRACE_DEBUG2("GPIO read (%d)->%d\n", pio, state);
    return state;
}

/*****************************************************************************
 **
 ** Function         UPIO_Config
 **
 ** Description      - Configure GPIOs of type UPIO_GENERAL as inputs or outputs
 **                  - Configure GPIOs to be polled or interrupt driven
 **
 **
 ** Output Parameter:
 **      None.
 **
 ** Returns:
 **      None.
 **
 *****************************************************************************/
UDRV_API void UPIO_Config(tUPIO_TYPE type, tUPIO pio, tUPIO_CONFIG config, tUPIO_CBACK *cback)
{
}

#if 0
wiced_bool_t bt_bus_is_ready(void)
{
    return (bus_initialised == WICED_FALSE) ? WICED_FALSE : ((wiced_gpio_input_get(BLUETOOTH_GPIO_CTS_PIN) == WICED_TRUE) ? WICED_FALSE : WICED_TRUE);
}
#endif
