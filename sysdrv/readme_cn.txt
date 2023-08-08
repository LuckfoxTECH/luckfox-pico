1.  sysdrv 顶层 Makefile 使用说明

具体命令如下：

(1)编译整个sysdrv目录：
   make  clean
   make

(2)查看编译配置
    make  info

(3)清除各个sysdrv模块的编译文件：
    make  clean

(4)清除整个sysdrv生成的编译文件：
    make  distclean

(5)单独编译各个模块：

    (5.1)编译uboot
            make uboot BOOT_MEDIUM=emmc

    (5.2)编译kernel
            make kernel BOOT_MEDIUM=emmc

    (5.3)编译rootfs
            make rootfs BOOT_MEDIUM=emmc

BOOT_MEDIUM=[emmc,spi_nand,spi_nor,slc_nand]
注：
    - 不同的芯片平台BOOT_MEDIUM支持不一样
    - uboot/kernel/rootfs模块独单编译的命令，具体可以用make info查看

2. 制作文件系统镜像
    生成PC制作根文件系统镜像工具的out/bin/pc/
    make  pctools

    2.1 制作用于emmc启动介质的文件系统镜像
        emmc启动介质可以用ext4和squashfs，（其中squashfs是只读文件系统，不可写）
        ./out/bin/pc/mkfs_ext4.sh <source> <dest image> <partition size>
        例如，制作用于emmc,镜像大小64MB的ext4文件系统镜像命令：
            ./out/bin/pc/mkfs_ext4.sh rootfs_source_dir rootfs.ext4 64*0x100000

        例如，制作用于emmc的squashfs文件系统镜像命令：
            ./out/bin/pc/mkfs_squashfs.sh <source> <dest image> <compression>
            ./out/bin/pc/mkfs_squashfs.sh rootfs_source_dir rootfs.squashfs

    2.2 制作用于spi nor启动介质的文件系统镜像
        spi nor启动介质可以用jffs2和squashfs，（其中squashfs是只读文件系统，不可写）
        ./out/bin/pc/mkfs_jffs2.sh <source> <dest image> <partition size>
        例如，制作用于spi nor,镜像大小8MB的jffs2文件系统镜像命令：
            ./out/bin/pc/mkfs_jffs2.sh rootfs_source_dir rootfs.jffs2 8*0x100000

        例如，制作用于spi nor的squashfs文件系统镜像命令：
            ./out/bin/pc/mkfs_squashfs.sh <source> <dest image> <compression>
            ./out/bin/pc/mkfs_squashfs.sh rootfs_source_dir rootfs.squashfs

    2.3 制作用于spi nand或slc nand启动介质的文件系统镜像
        spi nand或slc nand启动介质可以用ubifs和squashfs，（其中squashfs是只读文件系统，不可写）
        ./out/bin/pc/mkfs_ubi.sh <source dir> <output dir> <partition size> <partition name> <fs type> <fs comp>

        <fs type>: filesystem type on ubi (default ubifs)
                   support ubifs or squashfs(readonly)

        <fs comp>: filesystem compression
                   <fs type> is ubifs, support lzo|zlib
                   <fs type> is squashfs, support lz4|lzo|lzma|xz|gzip

        例如，制作用于spi nand或slc nand,镜像大小64MB的ubifs文件系统镜像命令：
            mkdir image_out
            ./out/bin/pc/mkfs_ubi.sh rootfs_source_dir image_out 64*0x100000 rootfs ubifs
            ls -l image_out
                rootfs.img -> rootfs_2KB_128KB_64MB.ubi
                rootfs_2KB_128KB_64MB.ubi --------------- 镜像用于page size 2KB,block size 128KB的spi nand或slc nand
                rootfs_2KB_256KB_64MB.ubi --------------- 镜像用于page size 2KB,block size 256KB的spi nand或slc nand
                rootfs_4KB_256KB_64MB.ubi --------------- 镜像用于page size 4KB,block size 256KB的spi nand或slc nand

        例如，制作用于spi nand或slc nand,镜像大小64MB的squashfs文件系统镜像命令：
            mkdir image_out
            ./out/bin/pc/mkfs_ubi.sh rootfs_source_dir image_out 64*0x100000 rootfs squashfs
            ls -l image_out
                rootfs.img -> rootfs_2KB_128KB_64MB.ubi
                rootfs_2KB_128KB_64MB.ubi --------------- 镜像用于page size 2KB,block size 128KB的spi nand或slc nand
                rootfs_2KB_256KB_64MB.ubi --------------- 镜像用于page size 2KB,block size 256KB的spi nand或slc nand
                rootfs_4KB_256KB_64MB.ubi --------------- 镜像用于page size 4KB,block size 256KB的spi nand或slc nand


3. 镜像存放目录说明
编译完的image，rootfs等存放在out/image_xxx目录下
out
├── bin
│   ├── board_xxx ---------------- 运行在单板端的文件
│   └── pc ----------------------- 运行在PC服务器端的工具
├── image_xxx -------------------- 生成的烧录固件
│   ├── download.bin ------------- 烧录工具升级通讯的设备端程序，只会下载到板子内存
│   ├── env.img ------------------ 包含分区表和启动参数
│   ├── idblock.img -------------- loader镜像
│   ├── uboot.img ---------------- uboot镜像
│   ├── boot.img ----------------- kernel镜像
│   └── rootfs_base.img ---------- rootfs镜像
│
├── rootfs_xxx ------------------- 生成的根文件系统目录
└── rootfs_xxx.tar --------------- 生成的根文件系统目录的tar文件包

4.  sysdrv目录结构说明：
├── cfg
│   ├── cfg.mk ------------------- 平台相关配置
│   └── package.mk --------------- 部分工具包配置
│
├── source
│   ├── busybox ------------------ busybox编译目录，代码在tools/board/busybox
│   ├── kernel ------------------- 内核源码目录
│   └── uboot
│       ├── rkbin ---------------- 启动相关的预编译文件
│       └── u-boot --------------- U-Boot源码目录
└── tools
    ├── board -------------------- 运行在单板端的工具源码目录
    │   ├── android-tools -------- adb工具
    │   ├── busybox -------------- busybox工具
    │   ├── e2fsprogs ------------ ext4文件系统工具
    │   ├── eudev ---------------- 设备管理工具
    │   ├── gdb ------------------ gdb调试工具
    │   ├── mtd-utils ------------ jffs2/ubifs文件系统工具
    │   ├── rootfs_script.tar ---- 根文件系统脚本目录
    │   └── toolkits
    │
    └── pc ----------------------- 运行在PC服务器端的工具源码目录

5. 注意事项
    在windows下复制源码包时，linux下的可执行文件可能变为非可执行文件，或者软连接失效导致无法编译使用。
    因此使用时请注意不要在windows下复制源代码包。
