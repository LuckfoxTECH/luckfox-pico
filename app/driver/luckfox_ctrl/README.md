* New Firmware
scp libdatachannel.so.0.24 root@luckfox:/usr/lib/
chmod 755 /usr/lib/libdatachannel.so.0.24
ln -s libdatachannel.so.0.24 /usr/lib/libdatachannel.so
export LD_LIBRARY_PATH=/usr/lib:$LD_LIBRARY_PATH


export PATH=~/luckfox-pico/tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf/bin:$PATH 
export CROSS_COMPILE=arm-rockchip830-linux-uclibcgnueabihf- 
export ARCH=arm 
