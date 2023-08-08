## SDK build.sh使用说明

### 选择参考的板级配置

    ./build.sh lunch

    You're building on Linux
    Lunch menu...pick a combo:

    BoardConfig-*.mk naming rules:
    BoardConfig-"启动介质"-"电源方案"-"硬件版本"-"应用场景".mk
    BoardConfig-"boot medium"-"power solution"-"hardware version"-"applicaton".mk

    ----------------------------------------------------------------
    0. BoardConfig-EMMC-ALL-2xRK806-HW_V10-IPC_MULTI_SENSOR.mk
                                 boot medium(启动介质): EMMC
                              power solution(电源方案): 2xRK806
                            hardware version(硬件版本): HW_V10
                                  applicaton(应用场景): IPC_MULTI_SENSOR
    ----------------------------------------------------------------

    ----------------------------------------------------------------
    1. BoardConfig-SPI_NAND-ALL-RK806-HW_V10-IPC_SINGLE_SENSOR.mk
                                 boot medium(启动介质): SPI_NAND
                              power solution(电源方案): RK806
                            hardware version(硬件版本): HW_V10
                                  applicaton(应用场景): IPC_SINGLE_SENSOR
    ----------------------------------------------------------------

    Which would you like? [0]:

输入对应的序号选择对应的参考板级。

### 一键自动编译

./build.sh lunch   # 选择参考板级
./build.sh         # 一键自动编译

### 编译U-Boot

./build.sh clean uboot
./build.sh uboot

生成镜像文件：
output/image/MiniLoaderAll.bin
output/image/uboot.img

### 编译kernel

./build.sh clean kernel
./build.sh kernel

生成镜像文件：
output/image/boot.img

### 编译rootfs

./build.sh clean rootfs
./build.sh rootfs

编译后使用`./build.sh firmware`命令打包成rootfs.img
生成镜像文件：output/image/rootfs.img

### 编译media

./build.sh clean media
./build.sh media

生成文件的存放目录：
output/out/media_out

### 编译参考应用

./build.sh clean app
./build.sh app

生成文件的存放目录：
output/out/app_out
注：app依赖media

### 固件打包

./build.sh firmware

生成文件的存放目录：
output/image

## SDK目录结构说明：

```shell
├── build.sh -> project/build.sh ---- SDK编译脚本
├── media --------------------------- 多媒体编解码、ISP等算法相关（可独立SDK编译）
├── sysdrv -------------------------- U-Boot、kernel、rootfs目录（可独立SDK编译）
├── project ------------------------- 参考应用、编译配置以及脚本目录
├── output -------------------------- SDK编译后镜像文件存放目录
├── docs ---------------------------- SDK文档目录
└── tools --------------------------- 烧录镜像打包工具以及烧录工具
```

## 镜像存放目录说明

编译完的文件存放在output目录下

```shell
output/
├── image
│   ├── download.bin ---------------- 烧录工具升级通讯的设备端程序，只会下载到板子内存
│   ├── env.img --------------------- 包含分区表和启动参数
│   ├── uboot.img ------------------- uboot镜像
│   ├── idblock.img ----------------- loader镜像
│   ├── boot.img -------------------- kernel镜像
│   ├── rootfs.img ------------------ kernel镜像
│   └── userdata.img ---------------- userdata镜像
└── out
    ├── app_out --------------------- 参考应用编译后的文件
    ├── media_out ------------------- media相关编译后的文件
    ├── rootfs_xxx ------------------ 文件系统打包目录
    ├── S20linkmount ---------------- 分区挂载脚本
    ├── sysdrv_out ------------------ sysdrv编译后的文件
    └── userdata -------------------- userdata
```

## 注意事项
    在windows下复制源码包时，linux下的可执行文件可能变为非可执行文件，或者软连接失效导致无法编译使用。
    因此使用时请注意不要在windows下复制源代码包。
