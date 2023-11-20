# Embedded Linux based LoraWan Balloon design

## Introduction

This project is designed to build a linux based LoraWan HAB balloon. The idea of the project is to make an advancement of the current PicoBalloon communities. 

## Network design decisions

There are 2 major providers of LoraWan coverage:
* TTN (The things network)
* Helium

Both have their own pros/cons. 

I would consider TTN to be the more reliable choice, however seems to have an extremely strict fair use policy for the type of application I'm trying to work with. 

Helium has what I would consider almost a "snake-oil"-esque blockchain based method of trying to encourage gateways - potentially creating unreliablity. However I feel this is offset by the generous fair use policy, and the fact that the coverage at 10KM in the air is pretty much to the horizon. So if one of the nodes is down, it really doesn't matter.

Therefore, Helium has been the chosen provider for this project.

## Hardware design desisions

The hardware design is modular based, rather than a complete homegrown design to simplify development, and also ease of access to the project. (It's easy to reproduce at home!)

If significant advantages arise from a home developed board then it might be considered in the future. The weight difference, while lower is insignificant enough to not warrant the extra overhead and difficulty of access. Previous attempts of an in house designed circuit board using the F1C200S as a base design added complexity beyond what made the project enjoyable. (But easyeda.com I would recommend)

The hardware chosen has been based on multiple factors:
* Form size 
* Power consumption
* Ease of hardware interface (The components should be able to work with each other without having to design an interface board in the middle to aid communication)

### Hardware chosen

#### Main CPU Board
The Luckyfox Pico Mini B board was chosen as the main CPU interface board. (https://www.aliexpress.com/item/1005006175964407.html?spm=a2g0o.productlist.main.5.23e65e7fajW5dU&algo_pvid=d7ac3b67-3060-4390-9949-0ae884c76ab1&algo_exp_id=d7ac3b67-3060-4390-9949-0ae884c76ab1-2&pdp_npi=4%40dis%21GBP%216.24%215.62%21%21%217.52%21%21%402103080617001519217352442e2997%2112000036136791338%21sea%21UK%212760040230%21&curPageLogUid=gNBLw8xFaTQN)

This decision was made for the following reasons:

* It's absolutely tiny. (28.14mm x 21.00mm)
* It has a CSI connector (Allowing close to plug and play with adding a camera to the device)
* It's low power.
* It has an excellent SDK, which is now buildroot based complete with python3.
* If I attempted to build the circuit design myself it would take 20 attempts involving a fresh board being built and shipped (This has to be done using multilayer PCB/SMT components), and the project would just never get finished.

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

## Notes on design documentation and Source code

The linux module used as the central processor is based on the LuckyFox Pico Mini B. Hence the basis of this git repository is a fork of their SDK for ease of updating any bug fixes for the board. Where there have been significant code changes, the README.md from LuckyFox have been renamed with a _luckyfox.md appendage. 

As the base design of this project is a fork of the Luckyfox Pico SDK (https://github.com/LuckfoxTECH/luckfox-pico) a lot of the project gets hidden within their design of how an embedded SDK design should look like (Which, actually, to be fair is pretty decent IMHO).

I have placed design thoughts within the ./doc directory within this repository. 
