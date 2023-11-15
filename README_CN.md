# Luckfox Pico SDK
[English Version](./README.md)
* 本SDK基于RK官方提供的SDK修改而来
* 专为Luckfox Pico系列开发板提供客制化的SDK
* 旨在为开发者提供更好的编程体验

## SDK 使用说明
* 推荐使用系统为Ubuntu 22.04
### 安装依赖
```shell
sudo apt-get install repo git ssh make gcc gcc-multilib g++-multilib module-assistant expect g++ gawk texinfo libssl-dev bison flex fakeroot cmake unzip gperf autoconf device-tree-compiler libncurses5-dev pkg-config
```
### 获取SDK
```
git clone https://github.com/LuckfoxTECH/luckfox-pico.git
```
### 环境变量
* 需要将交叉编译工具链设置
```
cd {SDK_PATH}/tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf/
source env_install_toolchain.sh
```
### build.sh使用说明
* SDK使用build.sh脚本实现自动编译，大部分编译操作均可以通过build.sh自动完成.
#### build.sh全部可用选项
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
#### 选择参考的板级配置
```shell
./build.sh lunch
```
将会输出对应的板级配置选项,输入对应板级配置的编号即可切换对应的板级配置
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

输入对应的序号选择对应的参考板级。

#### 一键自动编译
```shell
./build.sh lunch   # 选择参考板级
./build.sh         # 一键自动编译
```
#### 单独编译U-Boot
```shell
./build.sh clean uboot
./build.sh uboot
```
生成镜像文件：
output/image/MiniLoaderAll.bin
output/image/uboot.img

#### 单独编译kernel
```shell
./build.sh clean kernel
./build.sh kernel
```
生成镜像文件：
output/image/boot.img

#### 单独编译rootfs
```shell
./build.sh clean rootfs
./build.sh rootfs
```
* 注：编译后需使用`./build.sh firmware`命令重新打包

#### 单独编译media
```shell
./build.sh clean media
./build.sh media
```
生成文件的存放目录：
```
output/out/media_out
```
* 注：编译后需使用`./build.sh firmware`命令重新打包
#### 单独编译参考应用
```shell
./build.sh clean app
./build.sh app
```
* 注1：app依赖media
* 注2：编译后需使用`./build.sh firmware`命令重新打包
#### 固件打包
```shell
./build.sh firmware
```
生成文件的存放目录：
output/image



### 注意事项
    在windows下复制源码包时，linux下的可执行文件可能变为非可执行文件，或者软连接失效导致无法编译使用。
    因此使用时请注意不要在windows下复制源代码包。
