
if(NOT CMAKE_BUILD_TYPE)
    # default to Release build for GCC builds
    set(CMAKE_BUILD_TYPE Release CACHE STRING
        "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel."
        FORCE)
endif()

cmake_minimum_required(VERSION 2.8)

#project name
PROJECT(rkisp_demo)

#set BUILROOT_BUILD_PROJECT
set(BUILROOT_BUILD_PROJECT true)
#set(ENABLE_ASAN true)

# ----------------------------------------------------------------------------
# SET THE COMPILER PATH
# ----------------------------------------------------------------------------
    add_compile_options(${LIBRGA_CFLAGS})
    add_compile_options(-DISPDEMO_ENABLE_RGA=1)
    add_compile_options(${LIBDRM_CFLAGS})
    add_compile_options(-DISPDEMO_ENABLE_DRM=1)
if(BUILROOT_BUILD_PROJECT)
    #SET(CMAKE_C_COMPILER "/home/camera/camera/rv1109_sdk/buildroot/output/rockchip_puma/host/bin/arm-linux-gnueabihf-gcc")
    #SET(CMAKE_CXX_COMPILER "/home/camera/camera/rv1109_sdk/buildroot/output/rockchip_puma/host/bin/arm-linux-gnueabihf-g++")
	#SET(CMAKE_C_COMPILER "/home/xuhf/isp2.0-hal/buildroot/output/host/bin/arm-buildroot-linux-uclibcgnueabihf-gcc")
	#SET(CMAKE_CXX_COMPILER "/home/xuhf/isp2.0-hal/buildroot/output/host/bin/arm-buildroot-linux-uclibcgnueabihf-g++")
else()
    #SET(CMAKE_C_COMPILER "/home/hkj/isp2-fpga/fpga-ramdisk/toolchains/host/bin/arm-buildroot-linux-gnueabi-gcc")
    #SET(CMAKE_CXX_COMPILER "/home/hkj/isp2-fpga/fpga-ramdisk/toolchains/host/bin/arm-buildroot-linux-gnueabi-g++")
endif()

# ----------------------------------------------------------------------------
# ADD SYMBOL TABLE FOR GDB
# ----------------------------------------------------------------------------
# SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -ggdb")
# SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -ggdb")

# ----------------------------------------------------------------------------
# set property to classify library kinds
# ----------------------------------------------------------------------------
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMakeTargets")

#set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

#set ADD_RK_AIQ
set(ADD_RK_AIQ true)
option(ENABLE_IIO_API "Enable IIO API for EIS algorithm" OFF)

#SET RK_AIQ SOURCE DIR
set(RK_AIQ_SOURCE_DIR ${PROJECT_SOURCE_DIR}/../)
message("rkaiq src path" ${RK_AIQ_SOURCE_DIR})

if(ENABLE_ASAN)
add_compile_options(-fsanitize=address -fno-omit-frame-pointer -g)
SET(LINK_ASAN asan)
endif()

if(ADD_RK_AIQ)
add_compile_options(-std=c++11)
add_compile_options(-DADD_RK_AIQ)
add_compile_options(-O0)
# add_compile_options(-v)
SET(RK_AIQ_LIB_DIR ${RK_AIQ_SOURCE_DIR}/build/linux/output/${ARCH}/all_lib/${CMAKE_BUILD_TYPE})
message("xuhf===============  rkaiq lib path" ${RK_AIQ_LIB_DIR})
SET(RK_AIQ_INCLUDE_DIR
${RK_AIQ_SOURCE_DIR}/include/
${RK_AIQ_SOURCE_DIR}/include/uAPI
${RK_AIQ_SOURCE_DIR}/include/xcore/
${RK_AIQ_SOURCE_DIR}/include/algos/
${RK_AIQ_SOURCE_DIR}/include/common/
${RK_AIQ_SOURCE_DIR}/include/iq_parser/
)
SET(RK_AIQ_LIB rkaiq)
endif()

##set extern libraries
if(NOT BUILROOT_BUILD_PROJECT)
    SET(RK_DRM_LIB_DIR ${PROJECT_SOURCE_DIR}/demo/libs/arm32/)
endif()
SET(RK_DRM_LIB drm)
SET(RK_RGA_LIB rga)

if (ENABLE_IIO_API)
add_compile_options(-DTEST_MEMS_SENSOR_INTF=1)
set(IIO_LIB_DIR ${PROJECT_SOURCE_DIR}/demo/iio/lib)
set(IIO_LIBS xml2 easyiio iio)
endif()

#set (ENABLE_SMARTIR_API true)
if (ENABLE_SMARTIR_API)
    set(SMARTIR_DIR ${PROJECT_SOURCE_DIR}/../smart_ir)
    set(SMARTIR_INCLUDE_DIR ${SMARTIR_DIR}/include)
    set(SMARTIR_INSTALLED_LIB_DIR ${SMARTIR_DIR}/build/linux/output/${ARCH}/installed/lib)
    set(SMARTIR_LIB smartIr)
    add_compile_options(-DSAMPLE_SMART_IR)
endif()

#set(ENABLE_ISPFEC_API true)
if (ENABLE_ISPFEC_API)
	set(ISPFEC_INSTALLED_DIR ${PROJECT_SOURCE_DIR}/../../IspFec/build/linux/output/${ARCH}/installed/)
	set(ISPFEC_INCLUDE_DIR ${ISPFEC_INSTALLED_DIR}/include/)
	set(ISPFEC_LIB_DIR ${ISPFEC_INSTALLED_DIR}/lib/)
	set(ISPFEC_LIB IspFec)
	add_compile_options(-DISPFEC_API)
endif()
message("ispfec include path" ===========${ISPFEC_INCLUDE_DIR})

include_directories(
demo
demo/include
demo/include/libdrm
demo/drmDsp
demo/iio/include
${RK_AIQ_INCLUDE_DIR}
demo/sample/
${SMARTIR_INCLUDE_DIR}
${ISPFEC_INCLUDE_DIR}
)
#source directory
AUX_SOURCE_DIRECTORY(demo DIR_SRCS)
AUX_SOURCE_DIRECTORY(demo/drmDsp DRMDSP_DIR_SRCS)
if (ENABLE_SMARTIR_API)
AUX_SOURCE_DIRECTORY(demo/sample SAMPLE_DIR_SRCS)
endif()
AUX_SOURCE_DIRECTORY(demo/ae_algo_demo AEDEMO_DIR_SRCS)
if (ENABLE_IIO_API)
AUX_SOURCE_DIRECTORY(demo/iio/demo IIO_DIR_SRCS)
endif()
list(APPEND DIR_SRCS ${DRMDSP_DIR_SRCS})
if (ENABLE_SMARTIR_API)
list(APPEND DIR_SRCS ${SAMPLE_DIR_SRCS})
endif()
list(APPEND DIR_SRCS ${AEDEMO_DIR_SRCS})
list(APPEND DIR_SRCS ${IIO_DIR_SRCS})

# set output binary path
if(WIN32)
    SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/exe)
    SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/all_lib/)
    SET(CMAKE_LIBRARY_PATH ${LIBRARY_OUTPUT_PATH}/${CMAKE_BUILD_TYPE})
else()
    SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/exe/${CMAKE_BUILD_TYPE})
    SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/all_lib/${CMAKE_BUILD_TYPE})
    SET(CMAKE_LIBRARY_PATH ${LIBRARY_OUTPUT_PATH})
endif()

#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
#set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
message(${CMAKE_LIBRARY_PATH})
message(${LIBRARY_OUTPUT_PATH})

add_definitions(-D_CRT_SECURE_NO_WARNINGS)


link_directories("${LIBRARY_OUTPUT_PATH}" "${RK_AIQ_LIB_DIR}" "${RK_DRM_LIB_DIR}" "${IIO_LIB_DIR}" "${SMARTIR_INSTALLED_LIB_DIR}" "${ISPFEC_LIB_DIR}")
#add executable file
ADD_EXECUTABLE(rkisp_demo ${DIR_SRCS})
target_link_libraries(rkisp_demo ${RK_AIQ_LIB} ${RK_DRM_LIB} ${RK_RGA_LIB} ${IIO_LIBS} ${LINK_ASAN} ${SMARTIR_LIB}  ${ISPFEC_LIB} pthread)
