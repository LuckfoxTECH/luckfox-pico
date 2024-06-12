![luckfox](https://github.com/LuckfoxTECH/luckfox-pico/assets/144299491/cec5c4a5-22b9-4a9a-abb1-704b11651e88)
# Luckfox Pico SDK
[中文版](./README_CN.md)
* This SDK is modified based on the SDK provided by Rockchip
* It provides a customized SDK specifically for Luckfox Pico series development boards 
* Aimed at providing developers with a better programming experience
## SDK Updatelog
* Current version V1.3
1. Added support for Luckfox-pico-Ultra and Luckfox-pico-Ultra-W
2. Optimized the selection process for board support files
3. Improved the download speed of buildroot by selecting the fastest mirror based on the download environment
4. Enhanced buildroot package management operations; added the `buildrootconfig` option to the `build.sh` command to directly enter buildroot's menuconfig
5. Improved the rootfs clean operation to retain Buildroot already downloaded packages
6. Enhanced kernel configuration operations; added the `kernelconfig` option to the `build.sh` command to enter the kernel's menuconfig
7. Added a `config` folder for quick configuration of device trees, kernel, and buildroot
8. Optimized the system's root filesystem packaging process, allowing customization of root files in the `<Luckfox-pico SDK PATH>/output/out/rootfs_uclibc_rv1106` folder
9. Modified the default device tree configuration, enabling pin and interface function configuration on the board system using the `luckfox-config` command
10. Partial bug fixes
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
### Get the SDK
* GitHub
    ```
    git clone <https://github.com/LuckfoxTECH/luckfox-pico.git>
    ```
* Gitee
    ```
    git clone <https://gitee.com/LuckfoxTECH/luckfox-pico.git>
    ```
   * If you need to compile the Ubuntu system and use the Gitee source
   * Please modify the corresponding board mk file LF_SUBMODULES_BY to gitee, for example
        ```
        LF_SUBMODULES_BY=gitee
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

buildrootconfig    -config buildroot and save defconfig"
kernelconfig       -config kernel and save defconfig"
```
#### Select the referenced board configuration
```shell
./build.sh lunch
```
+ Output the corresponding Luckfox-pico hardware model. Enter the corresponding number to proceed to the storage media options (press Enter to select option [0] directly).
  ```shell
  You're building on Linux
    Lunch menu...pick the Luckfox Pico hardware version:
    选择 Luckfox Pico 硬件版本:
                  [0] RV1103_Luckfox_Pico
                  [1] RV1103_Luckfox_Pico_Mini_A
                  [2] RV1103_Luckfox_Pico_Mini_B
                  [3] RV1103_Luckfox_Pico_Plus
                  [4] RV1106_Luckfox_Pico_Pro_Max
                  [5] RV1106_Luckfox_Pico_Ultra
                  [6] RV1106_Luckfox_Pico_Ultra_W
                  [7] custom
  Which would you like? [0~7][default:0]:
  ```
+ Output the supported storage media for the corresponding Luckfox-pico hardware model. Enter the corresponding number to proceed to the root filesystem options (press Enter to select option [0] directly).For example, Luckfox Pico Plus.
  ```shell
    Lunch menu...pick the boot medium:
    选择启动媒介:
                  [0] SD_CARD
                  [1] SPI_NAND

  Which would you like? [0~1][default:0]:
  ```
+ Output the supported root filesystem types for the corresponding Luckfox-pico hardware model. Enter the corresponding number to complete the configuration (press Enter to select option [0] directly).
  ```shell
    Lunch menu...pick the system version:
    选择系统版本:
                  [0] Buildroot(Support Rockchip official features)
                  [1] Ubuntu(Support for the apt package management tool)

  Which would you like? [0~1][default:0]:
  ```
+ If you need to use the old configuration method or a custom board support file, select the "[7]custom" option when configuring the Luckfox-pico hardware model.
  ```shell
  You're building on Linux
    Lunch menu...pick the Luckfox Pico hardware version:
    选择 Luckfox Pico 硬件版本:
                  [0] RV1103_Luckfox_Pico
                  [1] RV1103_Luckfox_Pico_Mini_A
                  [2] RV1103_Luckfox_Pico_Mini_B
                  [3] RV1103_Luckfox_Pico_Plus
                  [4] RV1106_Luckfox_Pico_Pro_Max
                  [5] RV1106_Luckfox_Pico_Ultra
                  [6] RV1106_Luckfox_Pico_Ultra_W
                  [7] custom
  Which would you like? [0~7][default:0]: 7
  ----------------------------------------------------------------
  0. BoardConfig_IPC/BoardConfig-EMMC-Buildroot-RV1106_Luckfox_Pico_Ultra-IPC.mk
                              boot medium(启动介质): EMMC
                            system version(系统版本): Buildroot
                          hardware version(硬件版本): RV1106_Luckfox_Pico_Ultra
                                applicaton(应用场景): IPC
  ----------------------------------------------------------------

  ----------------------------------------------------------------
  1. BoardConfig_IPC/BoardConfig-EMMC-Buildroot-RV1106_Luckfox_Pico_Ultra_W-IPC.mk
                              boot medium(启动介质): EMMC
                            system version(系统版本): Buildroot
                          hardware version(硬件版本): RV1106_Luckfox_Pico_Ultra_W
                                applicaton(应用场景): IPC
  ----------------------------------------------------------------

  ----------------------------------------------------------------
  2. BoardConfig_IPC/BoardConfig-EMMC-Ubuntu-RV1106_Luckfox_Pico_Ultra-IPC.mk
                              boot medium(启动介质): EMMC
                            system version(系统版本): Ubuntu
                          hardware version(硬件版本): RV1106_Luckfox_Pico_Ultra
                                applicaton(应用场景): IPC
  ----------------------------------------------------------------

  ----------------------------------------------------------------
  3. BoardConfig_IPC/BoardConfig-EMMC-Ubuntu-RV1106_Luckfox_Pico_Ultra_W-IPC.mk
                              boot medium(启动介质): EMMC
                            system version(系统版本): Ubuntu
                          hardware version(硬件版本): RV1106_Luckfox_Pico_Ultra_W
                                applicaton(应用场景): IPC
  ----------------------------------------------------------------

  ----------------------------------------------------------------
  4. BoardConfig_IPC/BoardConfig-SD_CARD-Buildroot-RV1103_Luckfox_Pico-IPC.mk
                              boot medium(启动介质): SD_CARD
                            system version(系统版本): Buildroot
                          hardware version(硬件版本): RV1103_Luckfox_Pico
                                applicaton(应用场景): IPC
  ----------------------------------------------------------------

  ----------------------------------------------------------------
  5. BoardConfig_IPC/BoardConfig-SD_CARD-Buildroot-RV1103_Luckfox_Pico_Mini_A-IPC.mk
                              boot medium(启动介质): SD_CARD
                            system version(系统版本): Buildroot
                          hardware version(硬件版本): RV1103_Luckfox_Pico_Mini_A
                                applicaton(应用场景): IPC
  ----------------------------------------------------------------

  ----------------------------------------------------------------
  6. BoardConfig_IPC/BoardConfig-SD_CARD-Buildroot-RV1103_Luckfox_Pico_Mini_B-IPC.mk
                              boot medium(启动介质): SD_CARD
                            system version(系统版本): Buildroot
                          hardware version(硬件版本): RV1103_Luckfox_Pico_Mini_B
                                applicaton(应用场景): IPC
  ----------------------------------------------------------------

  ----------------------------------------------------------------
  7. BoardConfig_IPC/BoardConfig-SD_CARD-Buildroot-RV1103_Luckfox_Pico_Plus-IPC.mk
                              boot medium(启动介质): SD_CARD
                            system version(系统版本): Buildroot
                          hardware version(硬件版本): RV1103_Luckfox_Pico_Plus
                                applicaton(应用场景): IPC
  ----------------------------------------------------------------

  ----------------------------------------------------------------
  8. BoardConfig_IPC/BoardConfig-SD_CARD-Buildroot-RV1106_Luckfox_Pico_Pro_Max-IPC.mk
                              boot medium(启动介质): SD_CARD
                            system version(系统版本): Buildroot
                          hardware version(硬件版本): RV1106_Luckfox_Pico_Pro_Max
                                applicaton(应用场景): IPC
  ----------------------------------------------------------------

  ----------------------------------------------------------------
  9. BoardConfig_IPC/BoardConfig-SD_CARD-Ubuntu-RV1103_Luckfox_Pico-IPC.mk
                              boot medium(启动介质): SD_CARD
                            system version(系统版本): Ubuntu
                          hardware version(硬件版本): RV1103_Luckfox_Pico
                                applicaton(应用场景): IPC
  ----------------------------------------------------------------

  ----------------------------------------------------------------
  10. BoardConfig_IPC/BoardConfig-SD_CARD-Ubuntu-RV1103_Luckfox_Pico_Mini_A-IPC.mk
                              boot medium(启动介质): SD_CARD
                            system version(系统版本): Ubuntu
                          hardware version(硬件版本): RV1103_Luckfox_Pico_Mini_A
                                applicaton(应用场景): IPC
  ----------------------------------------------------------------

  ----------------------------------------------------------------
  11. BoardConfig_IPC/BoardConfig-SD_CARD-Ubuntu-RV1103_Luckfox_Pico_Mini_B-IPC.mk
                              boot medium(启动介质): SD_CARD
                            system version(系统版本): Ubuntu
                          hardware version(硬件版本): RV1103_Luckfox_Pico_Mini_B
                                applicaton(应用场景): IPC
  ----------------------------------------------------------------

  ----------------------------------------------------------------
  12. BoardConfig_IPC/BoardConfig-SD_CARD-Ubuntu-RV1103_Luckfox_Pico_Plus-IPC.mk
                              boot medium(启动介质): SD_CARD
                            system version(系统版本): Ubuntu
                          hardware version(硬件版本): RV1103_Luckfox_Pico_Plus
                                applicaton(应用场景): IPC
  ----------------------------------------------------------------

  ----------------------------------------------------------------
  13. BoardConfig_IPC/BoardConfig-SD_CARD-Ubuntu-RV1106_Luckfox_Pico_Pro_Max-IPC.mk
                              boot medium(启动介质): SD_CARD
                            system version(系统版本): Ubuntu
                          hardware version(硬件版本): RV1106_Luckfox_Pico_Pro_Max
                                applicaton(应用场景): IPC
  ----------------------------------------------------------------

  ----------------------------------------------------------------
  14. BoardConfig_IPC/BoardConfig-SPI_NAND-Buildroot-RV1103_Luckfox_Pico_Mini_B-IPC.mk
                              boot medium(启动介质): SPI_NAND
                            system version(系统版本): Buildroot
                          hardware version(硬件版本): RV1103_Luckfox_Pico_Mini_B
                                applicaton(应用场景): IPC
  ----------------------------------------------------------------

  ----------------------------------------------------------------
  15. BoardConfig_IPC/BoardConfig-SPI_NAND-Buildroot-RV1103_Luckfox_Pico_Plus-IPC.mk
                              boot medium(启动介质): SPI_NAND
                            system version(系统版本): Buildroot
                          hardware version(硬件版本): RV1103_Luckfox_Pico_Plus
                                applicaton(应用场景): IPC
  ----------------------------------------------------------------

  ----------------------------------------------------------------
  16. BoardConfig_IPC/BoardConfig-SPI_NAND-Buildroot-RV1106_Luckfox_Pico_Pro_Max-IPC.mk
                              boot medium(启动介质): SPI_NAND
                            system version(系统版本): Buildroot
                          hardware version(硬件版本): RV1106_Luckfox_Pico_Pro_Max
                                applicaton(应用场景): IPC
  ----------------------------------------------------------------

  Which would you like? [default:0]:
  ```
  Enter the corresponding board support file number to complete the configuration.
#### Set Buildroot System Default WIFI Configuration
* Navigate to the board-level configuration directory
    ```shell
    cd {SDK_PATH}/project/cfg/BoardConfig_IPC/
    ```
* Open the corresponding board-level configuration file
* Modify the parameters LF_WIFI_PASSWD and LF_WIFI_SSID
    ```shell
    export LF_WIFI_SSID="Your wifi ssid"
    export LF_WIFI_PSK="Your wifi password"
    ```
#### One-click Automatic Compilation
```shell
./build.sh lunch   # Select the reference board configuration
./build.sh         # One-click automatic compilation
```
* Compile busybox/buildroot    
    ```
    ./build.sh lunch   # Select the reference board
    ./build.sh         # One-click automatic compilation  
    ``` 
* Compile Ubuntu
    ```
    sudo ./build.sh lunch   # Select the reference board
    sudo ./build.sh         # One-click automatic compilation  
    ```
    * Note that when compiling Ubuntu, make sure to use sudo, otherwise it may cause file system errors
    * The following text will not distinguish between the two sets of instructions, please choose accordingly based on the situation
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
#### Kernel Config
```shell
./build.sh kernelconfig
```
Open the menuconfig interface for the kernel.
#### Buildroot Config
```shell
./build.sh buildrootconfig
```
Open the menuconfig interface for buildroot.
* Note: This is only applicable when selecting buildroot as the root file system.

## Notices
When copying the source code package under Windows, the executable file under Linux may become a non-executable file, or the soft link fails and cannot be compiled and used.
Therefore, please be careful not to copy the source code package under Windows.
