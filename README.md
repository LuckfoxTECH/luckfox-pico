# Embedded Linux based LoraWan Balloon design

## Introduction

This project is designed to build a linux based LoraWan HAB balloon. The idea of the project is to make an advancement of the current PicoBalloon communities. 

## Hardware design desisions

The hardware design is modular based, rather than a complete homegrown design to simplify development, and also ease of access to the project. (It's easy to reproduce at home!)

If significant advantages arise from a home developed board then it might be considered in the future. The weight difference, while lower is insignificant enough to not warrant the extra overhead and difficulty of access. Previous attempts of an in house designed circuit board using the F1C200S as a base design added complexity beyond what made the project enjoyable. (But easyeda.com I would recommend)

The hardware chosen has been based on multiple factors:
* Form size 
* Power consumption
* Ease of interface (The components should be able to work with each other without having to design an interface board in the middle to aid communication)

### Hardware chosen

#### Main CPU Board
The Luckyfox Pico Mini B board was chosen as the main CPU interface board. (https://www.aliexpress.com/item/1005006175964407.html?spm=a2g0o.productlist.main.5.23e65e7fajW5dU&algo_pvid=d7ac3b67-3060-4390-9949-0ae884c76ab1&algo_exp_id=d7ac3b67-3060-4390-9949-0ae884c76ab1-2&pdp_npi=4%40dis%21GBP%216.24%215.62%21%21%217.52%21%21%402103080617001519217352442e2997%2112000036136791338%21sea%21UK%212760040230%21&curPageLogUid=gNBLw8xFaTQN)

This decision was made for the following reasons:

* It's absolutely tiny. (28.14mm x 21.00mm)
* It has a CSI connector (Allowing close to plug and play with adding a camera to the device)
* It's low power.
* It has an excellent SDK, which is now buildroot based complete with python3.
* If I attempted to build the circuit design myself it would take 20 attempts, and the project would just never get finished.

#### LoraWan communication module
A random Semtec SX1276 based module has been chosen from AliExpress. (https://www.aliexpress.com/item/1005002603901624.html?spm=a2g0o.order_list.order_list_main.5.5e371802KeFoSQ)

This design decision has been made for the following reasons:
* The SX1276 module has extremely good support
* The fact that it is a "Random" module doesn't matter, it has a SPI interface, and one would hope the board is tested - because it's a SX1276 board, any other board with the same chip should be pretty much one-to-one replaceable.
* If I attempted to build the circuit design myself it would take 20 attempts, and the project would just never get finished.

#### GPS Module

Generally modern GPS modules are simply UART based, use similar chipsets and have essentially all become unified since they became integrated into smartphones. I didn't take much bother into researching this one. They should in theory neary all just be hotswappable.

#### Camera Module

This one needs research. I just picked the smallest, lightest camera I had on hand (It came from a ESP32 kit). Might not be the best solution power wise. This section might need an update.


## Notes on Source code.

The linux module used as the central processor is based on the LuckyFox Pico Mini B. Hence the basis of this git repository is a fork of their SDK for ease of updating any bug fixes for the board. Where there have been significant code changes, the README.md from LuckyFox have been renamed with a _luckyfox.md appendage. 