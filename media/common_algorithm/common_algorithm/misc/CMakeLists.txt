#1.cmake version
cmake_minimum_required(VERSION 3.2)

#2.project name
project(misc_algorithm)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(SRC_LIBS)
install(DIRECTORY include/ DESTINATION "include")
if (RV1126_RV1109)
	file(GLOB SRC_LIBS ${CMAKE_CURRENT_SOURCE_DIR}/lib/arm-rockchip830-linux-gnueabihf/*.*)
endif()

if (RV1106_RV1103)
	file(GLOB SRC_LIBS ${CMAKE_CURRENT_SOURCE_DIR}/lib/arm-rockchip830-linux-uclibcgnueabihf/*.*)
endif()

if (RK3588)
	file(GLOB SRC_LIBS ${CMAKE_CURRENT_SOURCE_DIR}/lib/aarch64-rockchip1031-linux-gnu/*.*)
endif()

if (SRC_LIBS)
	foreach(lib_var ${SRC_LIBS})
		install(FILES ${lib_var} DESTINATION "lib")
	endforeach(lib_var)
else()
	message(WARNING  "Not install common algorithm misc libraries")
endif()
