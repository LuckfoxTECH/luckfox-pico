message(STATUS "cmake version ${CMAKE_VERSION}")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project(rkadk)
include(${PROJECT_SOURCE_DIR}/version/Version.cmake)

install(DIRECTORY include/ DESTINATION include/rkadk FILES_MATCHING PATTERN "*.h")
include_directories(include)

if(${RKADK_CHIP} STREQUAL "rk3506")
	add_definitions(-DRK3506)
endif()

if(${RKADK_CHIP} STREQUAL "rv1106")
	add_definitions(-DRV1106_1103 -DTHUMB_NORMAL)
else()
        add_definitions(-DTHUMB_NORMAL)
endif()

if(USE_RKAIQ)
    find_package(RkAiq REQUIRED)
    include_directories(${RKAIQ_INCLUDE_DIRS})
    add_definitions(-DRKAIQ)
    if(${RKADK_CHIP} STREQUAL "rv1106")
	add_definitions(-DRKAIQ_UAPI2)
    endif()
endif()

if(ENABLE_FILE_CACHE)
	add_definitions(-DFILE_CACHE)
endif()

if(ENABLE_AOV)
	add_definitions(-DENABLE_AOV)
endif()

if(ENABLE_EIS)
	add_definitions(-DENABLE_EIS)
endif()

#-Wextra
add_definitions(-Wall
		-Wshadow
		-ffast-math
		-Wignored-qualifiers
		-Werror-implicit-function-declaration
		-fPIC
		-Wl,-Bsymbolic)

add_subdirectory(src)
add_subdirectory(examples)

#Install rkadk default configuration file
if(${RKADK_CHIP} STREQUAL "rv1106")
	set(RKADK_INICFG_PATH inicfg/rv1106_1103)
elseif(${RKADK_CHIP} STREQUAL "rv1126")
	set(RKADK_INICFG_PATH inicfg/rv1126_1109)
else()
	message("Warn: no valid chip is selected")
endif()

file(GLOB RKADK_INICFG_LIST ${RKADK_INICFG_PATH}/rkadk_defsetting*.ini)
install(FILES ${RKADK_INICFG_LIST} DESTINATION etc)
