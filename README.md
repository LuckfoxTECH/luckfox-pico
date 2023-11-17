# Luckfox Pico SDK
[中文版](./README_CN.md)
* This SDK is modified based on the SDK provided by Rockchip
* It provides a customized SDK specifically for Luckfox Pico series development boards 
* Aimed at providing developers with a better programming experience
## SDK Introduction
* The current main branch uses buildroot to build the rootfs, which makes it easier to add or remove applications.
* The original rootfs built directly with busybox has been moved to the busybox branch.
### Default Applications
The default SDK includes or installs the following applications (but not limited to):
1. python3
    * The following libraries are enabled by default:
    1. PERIPHERY
    2. PILLOW (font loading is temporarily unavailable)
    3. SERIAL
    4. SMBUS
    5. SPIDEV
2. ssh
   1. Automatically starts at boot
   2. Username: root
   3. Password: luckfox
3. samba
   1. Automatically starts at boot
   2. Username: root
   3. Password: luckfox
4. adb
   1. Automatically starts at boot
## SDK Usage Instructions
* recommended operating system : Ubuntu 22.04 
### Installing Dependencies
```shell
sudo apt-get install -y git ssh make gcc gcc-multilib g++-multilib module-assistant expect g++ gawk texinfo libssl-dev bison flex fakeroot cmake unzip gperf autoconf device-tree-compiler libncurses5-dev pkg-config bc python-is-python3 passwd openssl openssh-server openssh-client vim file cpio rsync
```
### Get SDK
```
git clone https://github.com/LuckfoxTECH/luckfox-pico.git
```
### Environment Variables
* The cross-compilation toolchain needs to be set Environment Variables
```
cd {SDK_PATH}/tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf/
source env_install_toolchain.sh
```
### Instructions for build.sh
* The build.sh script is used to automate the compilation process. 
* Most of the compilation operations can be completed automatically through build.sh.
#### Options for build.sh
```shell
Usage: build.sh [OPTIONS]
Available options:
lunch              -Select Board Configure
env                -build env
meta               -build meta (optional)
uboot              -build uboot
kernel             -build kernel
rootfs             -build rootfs
driver             -build kernel's drivers
sysdrv             -build uboot, kernel, rootfs
media              -build rockchip media libraries
app                -build app
recovery           -build recovery
tool               -build tool
updateimg          -build update image
unpackimg          -unpack update image
factory            -build factory image
all                -build uboot, kernel, rootfs, recovery image
allsave            -build all & firmware & save

clean              -clean all
clean uboot        -clean uboot
clean kernel       -clean kernel
clean driver       -clean driver
clean rootfs       -clean rootfs
clean sysdrv       -clean uboot/kernel/rootfs
clean media        -clean rockchip media libraries
clean app          -clean app
clean recovery     -clean recovery

firmware           -pack all the image we need to boot up system
ota                -pack update_ota.tar
save               -save images, patches, commands used to debug
check              -check the environment of building
info               -see the current board building information
```
#### Select the referenced board configuration
```shell
./build.sh lunch
```
* It will display the corresponding board configuration options. Enter the number corresponding to the board configuration to switch to it.
```shell
You're building on Linux
Lunch menu...pick a combo:

BoardConfig-*.mk naming rules:
BoardConfig-"启动介质"-"电源方案"-"硬件版本"-"应用场景".mk
BoardConfig-"boot medium"-"power solution"-"hardware version"-"applicaton".mk

----------------------------------------------------------------
1. BoardConfig_IPC/BoardConfig-EMMC-NONE-RV1103_Luckfox_Pico-IPC.mk
                            boot medium(启动介质): EMMC
                        power solution(电源方案): NONE
                        hardware version(硬件版本): RV1103_Luckfox_Pico
                            applicaton(应用场景): IPC
----------------------------------------------------------------

----------------------------------------------------------------
2. BoardConfig_IPC/BoardConfig-EMMC-NONE-RV1103_Luckfox_Pico_Mini_A-IPC.mk
                            boot medium(启动介质): EMMC
                        power solution(电源方案): NONE
                        hardware version(硬件版本): RV1103_Luckfox_Pico_Mini_A
                            applicaton(应用场景): IPC
----------------------------------------------------------------

----------------------------------------------------------------
3. BoardConfig_IPC/BoardConfig-SPI_NAND-NONE-RV1103_Luckfox_Pico_Mini_B-IPC.mk
                            boot medium(启动介质): SPI_NAND
                        power solution(电源方案): NONE
                        hardware version(硬件版本): RV1103_Luckfox_Pico_Mini_B
                            applicaton(应用场景): IPC
----------------------------------------------------------------

----------------------------------------------------------------
4. BoardConfig_IPC/BoardConfig-SPI_NAND-NONE-RV1103_Luckfox_Pico_Plus-IPC.mk
                            boot medium(启动介质): SPI_NAND
                        power solution(电源方案): NONE
                        hardware version(硬件版本): RV1103_Luckfox_Pico_Plus
                            applicaton(应用场景): IPC
----------------------------------------------------------------

----------------------------------------------------------------
5. BoardConfig_IPC/BoardConfig-SPI_NAND-NONE-RV1106_Luckfox_Pico_Pro_Max-IPC.mk
                            boot medium(启动介质): SPI_NAND
                        power solution(电源方案): NONE
                        hardware version(硬件版本): RV1106_Luckfox_Pico_Pro_Max
                            applicaton(应用场景): IPC
----------------------------------------------------------------

Which would you like? [0]:
```


#### One-click Automatic Compilation
```shell
./build.sh lunch   # Select the reference board configuration
./build.sh         # One-click automatic compilation
```
#### Build U-Boot
```shell
./build.sh clean uboot
./build.sh uboot
```
The path of the generated files:
```
output/image/MiniLoaderAll.bin
output/image/uboot.img
```
#### Build kernel
```shell
./build.sh clean kernel
./build.sh kernel
```
The path of the generated files:
```
output/image/boot.img
```
#### Build rootfs
```shell
./build.sh clean rootfs
./build.sh rootfs
```
* Note : After compilation, use the command ./build.sh firmware to repackage.

#### Build media
```shell
./build.sh clean media
./build.sh media
```
The path of the generated files:
```
output/out/media_out
```
* Note : After compilation, use the command ./build.sh firmware to repackage.
#### Build Reference Applications
```shell
./build.sh clean app
./build.sh app
```
* Note 1: The app depends on media.
* Note 2: After compilation, use the command ./build.sh firmware to repackage.
#### Firmware Packaging
```shell
./build.sh firmware
```
The path of the generated files:
```
output/image
```


## Notices
When copying the source code package under Windows, the executable file under Linux may become a non-executable file, or the soft link fails and cannot be compiled and used.
Therefore, please be careful not to copy the source code package under Windows.
