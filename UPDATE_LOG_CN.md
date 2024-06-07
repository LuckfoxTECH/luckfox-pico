# 更新日志
## V1.3 更新日志
1. 添加Luckfox-pico-Ultra和Luckfox-pico-Ultra-W支持
2. 优化了板级支持文件的选择操作
3. 优化了buildroot的软件包下载速度，会根据下载环境选择速度较快的源服务器
4. 优化了buildroot的包管理操作，`build.sh` 命令添加了 `buildrootconfig` 选项可以直接进入buildroot的menuconfig
5. 优化了rootfs的clean操作，可以保留buildroot已经下载的软件包
6. 优化了kernel的配置操作，`build.sh` 命令添加了 `kernelconfig` 选项可以进入kernel的menuconfig
7. 添加了 `config` 配置文件夹，可以快速配置设备树、内核和Buildroot
8. 优化了系统的根文件系统打包流程，可以在 `<Luckfox-pico SDK PATH>/output/out/rootfs_uclibc_rv1106`文件夹下对根文件进行客制化修改
9. 修改了默认设备树的配置，可以在板端系统使用`luckfox-config`命令配置引脚和接口功能
10. 部分bug修复
## V1.2 更新日志
1. 添加ubuntu系统兼容
2. 恢复对busybox的支持，使用户可以根据自身需求busybox、buildroot和ubuntu
3. 添加ubuntu根文件子库，并且支持github与gitee源切换
4. 添加exfat文件系统的支持
5. 部分bug修复
## V1.1 更新日志
1. rootfs使用buildroot构建，可以更加轻松的添加或者删除应用程序
2. 原先使用busybox直接构建rootfs，被移动到busybox分支
3. buildroot默认开启以下库
   1. python3
      * 默认开启以下库
      1. PERIPHERY
      2. PILLOW（暂时无法加载字体）
      3. SERIAL
      4. SMBUS
      5. SPIDEV
   2. ssh
      1. 默认开机自启
      2. 账号为root
      3. 密码为luckfox
   3. samba
      1. 默认开机自启
      2. 账号为root
      3. 密码为luckfox
   4. adb
      1. 默认开机自启
## V1.0 更新日志
* 初次上传，默认系统为busybox
