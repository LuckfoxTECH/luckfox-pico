# Updatelog
## V1.4 Updatelog
1. Updated U-Boot to support fast boot for RV1106 using SPI NAND and eMMC.
2. Optimized U-Boot compatibility with SD cards, reducing the likelihood of SD card recognition failures.
3. Updated the kernel version to 5.10.160, increasing the NPU frequency for RV1106G3.
4. Updated the Buildroot mirror source for more stable package downloads.
5. Added support for custom file systems.
6. Partial bug fixes
## V1.3 Updatelog
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
## V1.2 Updatelog
1. Added compatibility for Ubuntu system
2. Restored support for busybox, allowing users to choose between busybox, buildroot, and Ubuntu according to their needs
3. Added Ubuntu root file submodule and support for switching between GitHub and Gitee sources
4. Added support for exFAT file system
5. Partial bug fixes
## V1.1 Updatelog
1. Rootfs built using buildroot, making it easier to add or remove applications
2. Previously, the rootfs was directly built using busybox, which has been moved to the busybox branch
3. Buildroot now defaults to enabling the following libraries:
   1. Python3
      * Default libraries enabled:
      1. PERIPHERY
      2. PILLOW (font loading temporarily unavailable)
      3. SERIAL
      4. SMBUS
      5. SPIDEV
   2. SSH
      1. Automatically starts on boot
      2. Account: root
      3. Password: luckfox
   3. Samba
      1. Automatically starts on boot
      2. Account: root
      3. Password: luckfox
   4. ADB
      1. Automatically starts on boot
## V1.0 Updatelog
* Initial upload, default system is busybox
