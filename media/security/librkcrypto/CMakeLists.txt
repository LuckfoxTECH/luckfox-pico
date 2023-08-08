cmake_minimum_required (VERSION 2.8)
project (rkcrypto C)

################################################################################
# toolchain config
################################################################################
set (CMAKE_SYSTEM_NAME Linux)

if (NOT DEFINED CMAKE_C_COMPILER)
	message(FATAL_ERROR "librkcrypto: CMAKE_C_COMPILER not define")
endif()

if (NOT DEFINED CMAKE_CXX_COMPILER)
	message(FATAL_ERROR "librkcrypto: CMAKE_CXX_COMPILER not define")
endif()

################################################################################
# compile flags
################################################################################
add_compile_options(-Wall -Werror)
add_compile_options(-Wno-unused-function)
add_compile_options(-Wno-unused-parameter)
add_compile_options(-Wno-format-truncation)
add_compile_options(-Wno-maybe-uninitialized)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release" CACHE STRING
	"Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
	FORCE)
endif(NOT CMAKE_BUILD_TYPE)

################################################################################
# librkcrypto files
################################################################################
set(RKCRYPTO_PUBLIC_HEADER
	include/rkcrypto_otp_key.h
	include/rkcrypto_common.h
	include/rkcrypto_core.h
	include/rkcrypto_mem.h
)

include_directories(include)
file(GLOB SOURCES "src/*.c")

################################################################################
# libdrm dependencies
################################################################################
include_directories(third_party/libdrm/include)
include_directories(third_party/libdrm/include/drm)
file(GLOB SOURCES_DRM "third_party/libdrm/src/*.c")
add_definitions(-DMAJOR_IN_SYSMACROS=1 -D_GNU_SOURCE)

################################################################################
# libteec dependencies
################################################################################
add_definitions(-DBINARY_PREFIX=\"TEEC\")
set(TEEC_PATH third_party/optee_client/libteec)
include_directories(${TEEC_PATH}/../public ${TEEC_PATH}/include)
file(GLOB SOURCES_TEEC
	${TEEC_PATH}/src/tee_client_api.c
	${TEEC_PATH}/src/teec_trace.c)

################################################################################
# build librkcrypto shared library
################################################################################
set(SHARED_LIB_NAME rkcrypto)

add_library(${SHARED_LIB_NAME} SHARED ${SOURCES} ${SOURCES_DRM} ${SOURCES_TEEC})
set_target_properties(${SHARED_LIB_NAME} PROPERTIES PUBLIC_HEADER "${RKCRYPTO_PUBLIC_HEADER}")

################################################################################
# build librkcrypto static library
################################################################################
set(STATIC_LIB_NAME ${SHARED_LIB_NAME}_static)
add_library(${STATIC_LIB_NAME} STATIC ${SOURCES} ${SOURCES_DRM} ${SOURCES_TEEC})
set_target_properties(${STATIC_LIB_NAME} PROPERTIES OUTPUT_NAME ${SHARED_LIB_NAME})

################################################################################
# build other components
################################################################################
add_subdirectory(test)

################################################################################
# install public files
################################################################################
install(TARGETS ${SHARED_LIB_NAME}
        LIBRARY DESTINATION "lib"
        PUBLIC_HEADER DESTINATION "include")
install(TARGETS ${STATIC_LIB_NAME}
        ARCHIVE DESTINATION "lib")
