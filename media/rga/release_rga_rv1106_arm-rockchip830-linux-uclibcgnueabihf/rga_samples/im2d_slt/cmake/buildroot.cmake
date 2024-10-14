SET(TOOLCHAIN_HOME "/home1/yqw/linux/1106_ipc/tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf")

# this is required
#SET(CMAKE_SYSTEM_NAME Linux)

# specify the cross compiler
SET(CMAKE_C_COMPILER ${TOOLCHAIN_HOME}/bin/arm-rockchip830-linux-uclibcgnueabihf-gcc)
SET(CMAKE_CXX_COMPILER ${TOOLCHAIN_HOME}/bin/arm-rockchip830-linux-uclibcgnueabihf-g++)

# where is the target environment 
SET(CMAKE_FIND_ROOT_PATH  ${TOOLCHAIN_HOME})
 
# search for programs in the build host directories (not necessary)
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
