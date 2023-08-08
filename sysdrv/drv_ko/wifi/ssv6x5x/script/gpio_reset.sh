#!/bin/bash
USB_PORT=
[[ $# -lt 1 || ( $1 != "on" && $1 != "off" ) ]] && echo "Usage: gpio_reset_on.sh on|off [FTDI_TTY_PORT]" && exit 1

reset_enable=$1

# If $2 is given, use it as the GPIO control port or use the one of the largest port ID.
if [ $# == 1 ]; then
    port_id=0
    if [ -d "/sys/module/ftdi_sio/drivers/usb-serial:ftdi_sio" ]; then
        for tty_port in /sys/module/ftdi_sio/drivers/usb-serial\:ftdi_sio/ttyUSB* ; do
            id=`echo $tty_port | sed -e s/.*ttyUSB//`
            [ $id -gt $port_id ] && port_id=$id
        done
    fi
    USB_PORT=ttyUSB${port_id}
    echo Using ${USB_PORT} as the control GPIO port
else
    USB_PORT=$2
fi

# Check if GPIO function is enabled in kernel FTDI serial driver.
if [ ! -d "/sys/module/ftdi_sio/drivers/usb-serial:ftdi_sio/${USB_PORT}" -o ! -d "/sys/class/tty/${USB_PORT}" ]; then
    echo "No GPIO support for ${USB_PORT}."
    exit 1 
fi

# Find out the last GPIO # of this port
gpio_id=0
for gpio_chip in /sys/class/tty/${USB_PORT}/device/gpio/gpiochip*; do
    gpio_id=`echo ${gpio_chip} | sed -e s/^.*gpiochip//`
    gpio_id=`expr $gpio_id + 7`
done

# Make this GPIO port available
gpio=/sys/class/gpio/gpio${gpio_id}
if [ ! -d ${gpio} ]; then
    echo Creating GPIO $gpio_id...
    echo $gpio_id > /sys/class/gpio/export
fi

if [ -d ${gpio} ]; then
    echo Using GPIO ${gpio} to control WiFi Reset.
    dir=`cat ${gpio}/direction`
    if [ $dir == in ]; then
        echo "out" > ${gpio}/direction
    fi
    if [ $reset_enable == "on" ]; then
        echo 0 > ${gpio}/value
    else
        echo 1 > ${gpio}/value
    fi
else
    echo "Failed to enable GPIO to control WiFi."
    exit 1
fi

exit 0
