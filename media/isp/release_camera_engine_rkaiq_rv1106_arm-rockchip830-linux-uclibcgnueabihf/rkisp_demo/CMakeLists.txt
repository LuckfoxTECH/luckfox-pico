if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING
        "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel."
        FORCE)
endif()

cmake_minimum_required(VERSION 2.8...3.22)

PROJECT(rkisp_demo)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(BuildOptions)
include(CompileOptions)
include(GNUInstallDirs)

find_package(librga QUIET)
if (librga_FOUND)
    add_compile_options(${LIBRGA_CFLAGS})
    add_compile_options(-DISPDEMO_ENABLE_RGA=1)
endif()

if (RKAIQ_ENABLE_LIBDRM)
find_package(libdrm QUIET)
endif()
if (libdrm_FOUND)
    add_compile_options(${LIBDRM_CFLAGS})
    add_compile_options(-DISPDEMO_ENABLE_DRM=1)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMakeTargets")

set(ADD_RK_AIQ true)
option(ENABLE_IIO_API "Enable IIO API for EIS algorithm" OFF)

set(RK_AIQ_SOURCE_DIR ${PROJECT_SOURCE_DIR}/../rk_aiq)
message("rkaiq src path" ${RK_AIQ_SOURCE_DIR})

if(ADD_RK_AIQ)
    add_compile_options(-Os)
    add_compile_options(-DADD_RK_AIQ)
    add_compile_options(-fstack-protector-strong -D_FORTIFY_SOURCE=2)
    set(RK_AIQ_INSTALLED_DIR "${RK_AIQ_SOURCE_DIR}/build/linux/output/${ARCH}/installed" CACHE STRING "AIQ library installed dir")
    set(RK_AIQ_LIB_DIR ${RK_AIQ_INSTALLED_DIR}/lib)
    message("rkaiq lib path" ${RK_AIQ_LIB_DIR})
    set(RK_AIQ_INCLUDE_DIR
        ${RK_AIQ_INSTALLED_DIR}/include/
        ${RK_AIQ_INSTALLED_DIR}/include/rkaiq/
        ${RK_AIQ_INSTALLED_DIR}/include/rkaiq/uAPI2
        ${RK_AIQ_INSTALLED_DIR}/include/rkaiq/xcore/
        ${RK_AIQ_INSTALLED_DIR}/include/rkaiq/algos/
        ${RK_AIQ_INSTALLED_DIR}/include/rkaiq/common/
        ${RK_AIQ_INSTALLED_DIR}/include/rkaiq/iq_parser/
        ${RK_AIQ_INSTALLED_DIR}/include/rkaiq/iq_parser_v2/
        )
    set(RK_AIQ_LIB rkaiq)
endif()

if (ENABLE_IIO_API)
    set(IIO_LIB_DIR ${PROJECT_SOURCE_DIR}/demo/iio/lib)
    set(IIO_LIBS easyiio iio serialport xml2 )
endif()

#set (ENABLE_SMARTIR_API true)
if (ENABLE_SMARTIR_API)
	set(SMARTIR_DIR ${PROJECT_SOURCE_DIR}/../smart_ir)
	set(SMARTIR_INCLUDE_DIR ${SMARTIR_DIR}/include)
	set(SMARTIR_INSTALLED_LIB_DIR ${SMARTIR_DIR}/build/linux/output/${ARCH}/installed/lib)
	set(SMARTIR_LIB smartIr)
    add_compile_options(-DSAMPLE_SMART_IR)
endif()

include_directories(
    deps/include
    demo
    demo/include
    demo/include/rga
    demo/include/libdrm
    demo/drmDsp
    demo/iio/include
    ${RK_AIQ_INCLUDE_DIR}
    demo/sample/
	${SMARTIR_INCLUDE_DIR}
    )

if (libdrm_FOUND)
    aux_source_directory(demo DIR_SRCS)
    aux_source_directory(demo/drmDsp DRMDSP_DIR_SRCS)
    aux_source_directory(demo/sample SAMPLE_DIR_SRCS)
else()
    set (DIR_SRCS
        demo/rkisp_demo.cpp
        )
    aux_source_directory(demo/sample SAMPLE_DIR_SRCS)
endif()

if (ENABLE_IIO_API)
    AUX_SOURCE_DIRECTORY(demo/iio/demo IIO_DIR_SRCS)
endif()
list(APPEND DIR_SRCS ${DRMDSP_DIR_SRCS} ${IIO_DIR_SRCS} ${SAMPLE_DIR_SRCS})

AUX_SOURCE_DIRECTORY(demo/ae_algo_demo AEDEMO_DIR_SRCS)
list(APPEND DIR_SRCS ${AEDEMO_DIR_SRCS})

#AUX_SOURCE_DIRECTORY(demo/awb_algo_demo AWBDEMO_DIR_SRCS)
#list(APPEND DIR_SRCS ${AWBDEMO_DIR_SRCS})

AUX_SOURCE_DIRECTORY(demo/af_algo_demo AFDEMO_DIR_SRCS)
list(APPEND DIR_SRCS ${AFDEMO_DIR_SRCS})

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

if (libdrm_FOUND)
link_directories("${LIBRARY_OUTPUT_PATH}" "${RK_AIQ_LIB_DIR}" "${IIO_LIB_DIR}"
	"${PROJECT_SOURCE_DIR}/deps/lib" "${SMARTIR_INSTALLED_LIB_DIR}")
else()
link_directories("${LIBRARY_OUTPUT_PATH}" "${RK_AIQ_LIB_DIR}" "${IIO_LIB_DIR}"
	"${SMARTIR_INSTALLED_LIB_DIR}")
endif()
add_executable(rkisp_demo ${DIR_SRCS})
if (libdrm_FOUND)
target_link_libraries(rkisp_demo rga drm ${RK_AIQ_LIB} ${IIO_LIBS}
	${SMARTIR_LIB} pthread)
else()
target_link_libraries(rkisp_demo ${RK_AIQ_LIB} ${IIO_LIBS} ${SMARTIR_LIB} pthread)
endif()

if (libdrm_FOUND)
    target_link_libraries(rkisp_demo
        libdrm::libdrm
        )
endif()

if (librga_FOUND)
    target_link_libraries(rkisp_demo
        librga::librga
        )
endif()
