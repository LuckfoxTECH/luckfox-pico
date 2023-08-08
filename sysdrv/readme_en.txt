1. How to use Makefile of directory sysdrv:

Detail command as follow:

(1) compile all of sysdrv:
   make  clean
   make

(2) see the detail of make comamnd
    make  info

(3) clear all compiled files under sysdrv directory
    make  clean

(4) completely remove all compiled files under sysdrv directory, and the generated images
    make  distclean

(5) separately compile every modules

    (5.1) compile uboot
            make uboot BOOT_MEDIUM=emmc

    (5.2) compile kernel
            make kernel BOOT_MEDIUM=emmc

    (5.3) compile rootfs
            make rootfs BOOT_MEDIUM=emmc

BOOT_MEDIUM=[emmc,spi_nand,spi_nor,slc_nand]
Notic:
    - Different chip platforms BOOT_MEDIUM support different
    - Uboot/kernel/rootfs modules are compiled separately, which can be viewed with make info

2. Build file system image
    Generate a PC to make a root file system image tool out/bin/pc/
        make  pctools

    2.1 Make a file system image of the emmc boot media
        emmc boot media can be used with ext4 and squashfs, (where squashfs is a read-only file system and is not writable)
        ./out/bin/pc/mkfs_ext4.sh <source> <dest image> <partition size>

        For example, make an ext4 file system image for emmc, image size 64MB command:
            ./out/bin/pc/mkfs_ext4.sh rootfs_source_dir rootfs.ext4 64*0x100000

        For example, make an squashfs file system image for emmc command:
            ./out/bin/pc/mkfs_squashfs.sh <source> <dest image> <compression>
            ./out/bin/pc/mkfs_squashfs.sh rootfs_source_dir rootfs.squashfs

    2.2 Make a file system image of the spi nor boot media
        spi nor boot media can be used with jffs2 and squashfs, (where squashfs is a read-only file system and is not writable)
        ./out/bin/pc/mkfs_jffs2.sh <source> <dest image> <partition size>
        For example, make an jffs2 file system image for spi nor, image size 8MB command:
            ./out/bin/pc/mkfs_jffs2.sh rootfs_source_dir rootfs.jffs2 8*0x100000

        For example, make an squashfs file system image for spi nor command:
            ./out/bin/pc/mkfs_squashfs.sh <source> <dest image> <compression>
            ./out/bin/pc/mkfs_squashfs.sh rootfs_source_dir rootfs.squashfs

    2.3 Make a file system image of the spi nand (or slc nand) boot media
        spi nand (or slc nand) boot media can be used with ubifs and squashfs
            (where squashfs is a read-only file system and is not writable)

        ./out/bin/pc/mkfs_ubi.sh <source dir> <output dir> <partition size> <partition name> <fs type> <fs comp>
        <fs type>: filesystem type on ubi (default ubifs)
                   support ubifs or squashfs(readonly)
        <fs comp>: filesystem compression
                   <fs type> is ubifs, support lzo|zlib
                   <fs type> is squashfs, support lz4|lzo|lzma|xz|gzip

        For example, make an ubifs file system image for spi nand (or slc nand), image size 64MB command:
            mkdir image_out
            ./out/bin/pc/mkfs_ubi.sh rootfs_source_dir image_out 64*0x100000 rootfs ubifs
            ls -l image_out
                rootfs.img -> rootfs_2KB_128KB_64MB.ubi
                rootfs_2KB_128KB_64MB.ubi --------------- used for page size 2KB,block size 128KB spi nand (or slc nand)
                rootfs_2KB_256KB_64MB.ubi --------------- used for page size 2KB,block size 256KB spi nand (or slc nand)
                rootfs_4KB_256KB_64MB.ubi --------------- used for page size 4KB,block size 256KB spi nand (or slc nand)

        For example, make an squashfs file system image for spi nand (or slc nand), image size 64MB command:
            mkdir image_out
            ./out/bin/pc/mkfs_ubi.sh rootfs_source_dir image_out 64*0x100000 rootfs squashfs
            ls -l image_out
                rootfs.img -> rootfs_2KB_128KB_64MB.ubi
                rootfs_2KB_128KB_64MB.ubi --------------- used for page size 2KB,block size 128KB spi nand (or slc nand)
                rootfs_2KB_256KB_64MB.ubi --------------- used for page size 2KB,block size 256KB spi nand (or slc nand)
                rootfs_4KB_256KB_64MB.ubi --------------- used for page size 4KB,block size 256KB spi nand (or slc nand)


3. Output directory description
    All compiled images, rootfs are located in directory out/image_xxx

out
├── bin
│   ├── board_xxx ------------------- bin run on board
│   └── pc -------------------------- bin run on PC
├── image_xxx ----------------------- Image output dirctory
│   ├── download.bin ---------------- Will Only be downloaded to the DDR of the board
│   ├── env.img --------------------- include partiton table and boot parameter
│   ├── idblock.img ----------------- loader image
│   ├── uboot.img ------------------- uboot image
│   ├── boot.img -------------------- kernel image
│   └── rootfs_base.img ------------- rootfs image
│
├── rootfs_xxx ---------------------- the directory of rootfs files
└── rootfs_xxx.tar ------------------ the tarball of rootfs files

4.  Sysdrv directory structure

├── cfg
│   ├── cfg.mk ---------------------- configure of platform
│   └── package.mk ------------------ config some package compile
│
├── source
│   ├── busybox --------------------- the dir of compile busybox
│   ├── kernel ---------------------- Linux kernel source dir
│   └── uboot
│       ├── rkbin ------------------- pre-built binary about boot
│       └── u-boot ------------------ U-Boot source dir
└── tools
    ├── board ----------------------- the source dir which run on board
    │   ├── android-tools ----------- adb tool
    │   ├── busybox ----------------- busybox tool
    │   ├── e2fsprogs --------------- ext4 filesystem tool
    │   ├── eudev ------------------- device manage tool
    │   ├── gdb --------------------- gdb debug tool
    │   ├── mtd-utils --------------- jffs2/ubifs filesystem tool
    │   ├── rootfs_script.tar ------- the script of root filesystem
    │   └── toolkits
    │
    └── pc -------------------------- run on PC

5. Note
    Executable files in Linux may become non-executable after copying them to somewhere else under Windows,
    Or the soft-link file connection failure and result in souce  code cannot be compiled.
    So, DO NOT copy source package in windows.
