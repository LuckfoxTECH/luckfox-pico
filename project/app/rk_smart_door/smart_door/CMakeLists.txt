cmake_minimum_required(VERSION 3.5)

project(smart_door)

option(COMPILE_FOR_RV1106_RMSL "compile for rv1106 RMSL" OFF)
option(COMPILE_FOR_RV1106_NORMAL "compile for rv1106 normal IR" OFF)
option(COMPILE_FOR_RV1106_IR_FACE "compile for rv1106 IR face" OFF)
option(COMPILE_FOR_RV1103_RMSL "compile for rv1103 RMSL" OFF)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fdata-sections -ffunction-sections -Wl,--gc-sections")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -fdata-sections -ffunction-sections -Wl,--gc-sections")

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "MinSizeRel" CACHE STRING
	"Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
	FORCE)
endif(NOT CMAKE_BUILD_TYPE)

if(COMPILE_FOR_RV1106_RMSL)
  add_subdirectory(rv1106_rmsl)
endif()

if(COMPILE_FOR_RV1106_NORMAL)
  add_subdirectory(rv1106_normal)
endif()

if(COMPILE_FOR_RV1106_IR_FACE)
  add_subdirectory(rv1106_ir_face)
endif()

if(COMPILE_FOR_RV1103_RMSL)
  add_subdirectory(rv1103_rmsl311)
endif()
