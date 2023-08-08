## The instructions of SDK build.sh

### Select the referenced board configuration

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

Enter the corresponding number to select the corresponding reference board level.

### One-click Automatic Compilation

```shell
./build.sh lunch    # Select reference board level
./build.sh          # One-click automatic compilation
````

### Build U-Boot

```shell
./build.sh clean uboot
./build.sh uboot
````

Generate image files: output/image/MiniLoaderAll.bin and output/image/uboot.img

### Build kernel

```shell
./build.sh clean kernel
./build.sh kernel
````

Generate image file: output/image/boot.img

### Build rootfs

```shell
./build.sh clean rootfs
./build.sh rootfs
````

Make rootfs.img firmware by the command of  `./build.sh firmware`
Generate image file: output/image/rootfs.img

### Build media

```shell
./build.sh clean media
./build.sh media
````

The storage directory of the generated files: output/out/media_out

### Build Reference Applications

```shell
./build.sh clean app
./build.sh app
````

The storage directory of the generated file: output/out/app_out
Note: app depends on media

### Firmware Packaging

```shell
./build.sh firmware
````

The path of the generated files: output/image

## SDK Directory Structure

```shell
  ├── build.sh -> project/build.sh --------- SDK compilation script
  ├── media -------------------------------- Multimedia codec, ISP, etc. Algorithm related (Can be compiled independently of the SDK)
  ├── sysdrv ------------------------------- U-Boot, kernel, rootfs directory (Can be compiled independently of the SDK)
  ├── project ------------------------------ Reference application, build configuration and script directory
  ├── output ------------------------------- The directory where the image files are stored after SDK compilation
  ├── docs --------------------------------- SDK Documents directory
  └── tools -------------------------------- Image packaging tools and Burning tools
````

## Image Storage Directory

The compiled files are stored in the output directory

```shell
output/
├── image
│ ├── download.bin ------------------------ Will Only be downloaded to the DDR of the board
│ ├── env.img ----------------------------- Partition table and boot parameter
│ ├── idblock.img ------------------------- loader image
│ ├── uboot.img --------------------------- uboot image
│ ├── boot.img ---------------------------- kernel image
│ ├── rootfs.img -------------------------- kernel image
│ └── userdata.img ------------------------ userdata image
└── out
  ├── app_out ----------------------------- Referenced applications compiled files
  ├── media_out --------------------------- media related compiled files
  ├── rootfs_xxx -------------------------- File system packaging directory
  ├── S20linkmount ------------------------ Partition mount script
  ├── sysdrv_out -------------------------- sysdrv compiled file
  └── userdata ---------------------------- userdata
````

## Notices
When copying the source code package under Windows, the executable file under Linux may become a non-executable file, or the soft link fails and cannot be compiled and used.
Therefore, please be careful not to copy the source code package under Windows.
