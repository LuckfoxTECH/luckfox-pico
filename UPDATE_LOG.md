# Updatelog
## V1.3 Updatelog
1. Added support for Luckfox-pico Ultra / Luckfox-pico Ultra W
2. Restructured the board support files, adding Wifi settings to the Luckfox-pico Ultra W board support files.
3. Added `kernelconfig` and `buildrootconfig` commands to `./build.sh`, enabling direct access to menuconfig. After configuration, modifications are automatically applied by replacing the corresponding defconfig file
4. Created symbolic links for device tree files, kernel configuration files, and buildroot configuration files, facilitating direct editing within the `config` folder
5. Implemented automatic switching of buildroot sources, enabling selection of repository sources based on network conditions before building the buildroot root file system
6. Partial bug fixes
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
