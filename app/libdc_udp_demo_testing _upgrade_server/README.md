cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=../luckfox-armv7.cmake \
  -DCMAKE_INSTALL_PREFIX=$HOME/armv7/libdatachannel \
  -DBUILD_SHARED_LIBS=ON \
  -DOPENSSL_ROOT_DIR=$HOME/armv7/openssl
  
export PATH=~/luckfox-pico/tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf/bin:$PATH 
export CROSS_COMPILE=arm-rockchip830-linux-uclibcgnueabihf- 
export ARCH=arm 