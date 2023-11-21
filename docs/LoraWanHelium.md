# LoraWan, Integration with a LoraWan Network, etc.

## Introduction

LoraWan is based on two parts, described how it should be, but isn't really in the linux kernel (Not my Job!(?)!):
* PHY - A FSK based protocol called Lora. Essentially using Chirp Spectrum Spreading.
* MAC - The LoraWan protocol. Essentially an extremely basic IP stack which supports "UDP" and "TCP" packets. (They are in quotes because, actually it's closer to IPv4 but the devices have absolutely no idea what subnet they are on) 

## Getting the actual device to integrate with Linux.

This is suprisingly a tough one. 
* Easy part - Rockchip provided SPI device driver, which has a filesystem interface.
* Bad Part - No one seems to be able to have designed something which is suitable for this project (But starnight has done some amazing work!) [https://patchwork.kernel.org/project/linux-wpan/patch/20170910093026.4386-1-starnight@g.ncu.edu.tw/]

_Overall_ - there are two choices. 
* Adapt starnights work on a branch which he didn't try 802.15.4 as an implementation [https://github.com/starnight/LoRa/tree/file-ops]
* Hack the thing with python, and replace the RPi.GPIO library [https://github.com/raspberrypi-tw/lora-sx1276]

The former, while a PITA initially, actually provides what initially appears to be an easy to use interface to Lora, with functions such as spreading factor, band etc all controllable using ioctl calls.

The solution here will likely be to create the MAC "Driver" in userland. Should it be there? Absolutely not. (But, once again future improvement, otherwise the project will never get finished.)

