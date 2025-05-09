#!/bin/bash

# Export Pins
echo 120 >/sys/class/gpio/export
echo 0 >/sys/class/gpio/export

# Set the initial level state
echo out >/sys/class/gpio/gpio0/direction
echo 1 >/sys/class/gpio/gpio0/value

echo out >/sys/class/gpio/gpio120/direction
echo 0 >/sys/class/gpio/gpio120/value

# Set GT911 address 0x14
sleep 0.5
echo 1 >/sys/class/gpio/gpio120/value
sleep 0.5

echo 0 >/sys/class/gpio/gpio0/value
echo in >/sys/class/gpio/gpio0/direction

echo 120 >/sys/class/gpio/unexport
echo 0 >/sys/class/gpio/unexport

# Insmod goodix driver
insmod /oem/usr/ko/goodix.ko
