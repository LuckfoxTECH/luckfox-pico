# 更新日志
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
