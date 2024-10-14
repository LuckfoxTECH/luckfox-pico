cmake_minimum_required(VERSION 3.5)

project(rkipc)

# generate version info
include(cmake/Version.cmake)

option(COMPILE_FOR_RV1103_IPC "compile for rv1103 ipc" OFF)
option(COMPILE_FOR_RV1106_IPC "compile for rv1106 ipc" OFF)
option(COMPILE_FOR_RV1106_BATTERY_IPC_TUYA "compile for rv1106 battery ipc tuya" OFF)
option(COMPILE_FOR_RV1106_BATTERY_IPC_CLIENT "compile for rv1106 battery ipc client" OFF)
option(COMPILE_FOR_RV1106_DUAL_IPC "compile for rv1106 dual-ipc" OFF)
option(COMPILE_FOR_RV1106_WAKEUP_IPC "compile for rv1106 wakeup-ipc" OFF)
option(COMPILE_FOR_RV1106_AIISP "compile for rv1106 aiisp" OFF)

option(COMPILE_FOR_RV1126_RKMEDIA "compile for rv1126 ipc rkmedia" OFF)
option(COMPILE_FOR_RV1126_ROCKIT "compile for rv1126 ipc rockit" OFF)
option(COMPILE_FOR_RV1126_BATTERY_IPC "compile for rv1126 battery ipc" OFF)
option(COMPILE_FOR_RV1126_SNAPSHOT "compile for rv1126 snapshot" OFF)
option(COMPILE_FOR_RV1126_DUAL_IPC "compile for rv1126 dual-ipc" OFF)
option(COMPILE_FOR_RV1126_AIISP "compile for rv1126 aiisp" OFF)

option(COMPILE_FOR_RK3588 "compile for rk3588 ipc" OFF)
option(COMPILE_FOR_RK3588_MULTI_IPC "compile for rk3588 multi-ipc" OFF)

if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
	message(STATUS "CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")
	add_definitions(-g -ggdb -gdwarf -funwind-tables -rdynamic -O0)
	add_definitions(-fno-stack-protector -fno-omit-frame-pointer)
elseif(${CMAKE_BUILD_TYPE} STREQUAL Release)
	message(STATUS "CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")
elseif(${CMAKE_BUILD_TYPE} STREQUAL RelWithDebInfo)
	message(STATUS "CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")
elseif(${CMAKE_BUILD_TYPE} STREQUAL MinSizeRel)
	message(STATUS "CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")
	add_definitions(-Os)
endif()

if(RKIPC_CROSS_COMPILE)
message(STATUS "CROSS COMPILE TOOL: ${RKIPC_CROSS_COMPILE}")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fdata-sections -ffunction-sections -Wl,-rpath-link,${PROJECT_SOURCE_DIR}/lib/${RKIPC_CROSS_COMPILE}")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdata-sections -ffunction-sections -Wl,-rpath-link,${PROJECT_SOURCE_DIR}/lib/${RKIPC_CROSS_COMPILE}")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections -Wl,--as-needed")
endif()

if(COMPILE_FOR_RV1103_IPC)
  add_subdirectory(src/rv1103_ipc)
endif()

if(COMPILE_FOR_RV1106_IPC)
  add_subdirectory(src/rv1106_ipc)
endif()

if(COMPILE_FOR_RV1106_BATTERY_IPC_TUYA)
  add_subdirectory(src/rv1106_battery_ipc_tuya)
endif()

if(COMPILE_FOR_RV1106_BATTERY_IPC_CLIENT)
  add_subdirectory(src/rv1106_battery_ipc_client)
endif()

if(COMPILE_FOR_RV1106_DUAL_IPC)
  add_subdirectory(src/rv1106_dual_ipc)
endif()

if(COMPILE_FOR_RV1106_WAKEUP_IPC)
  add_subdirectory(src/rv1106_wakeup_ipc)
endif()

if(COMPILE_FOR_RV1106_AIISP)
  add_subdirectory(src/rv1106_aiisp_ipc)
endif()

if(COMPILE_FOR_RV1126_RKMEDIA)
  add_subdirectory(src/rv1126_ipc_rkmedia)
endif()

if(COMPILE_FOR_RV1126_ROCKIT)
  add_subdirectory(src/rv1126_ipc_rockit)
endif()

if(COMPILE_FOR_RV1126_BATTERY_IPC)
  add_subdirectory(src/rv1126_battery_ipc)
endif()

if(COMPILE_FOR_RV1126_SNAPSHOT)
  add_subdirectory(src/rv1126_snapshot)
endif()

if(COMPILE_FOR_RV1126_DUAL_IPC)
  add_subdirectory(src/rv1126_dual_ipc)
endif()

if(COMPILE_FOR_RV1126_AIISP)
  add_subdirectory(src/rv1126_aiisp_ipc)
endif()

if(COMPILE_FOR_RK3588)
  add_subdirectory(src/rk3588_ipc)
endif()

if(COMPILE_FOR_RK3588_MULTI_IPC)
  add_subdirectory(src/rk3588_multi_ipc)
endif()
