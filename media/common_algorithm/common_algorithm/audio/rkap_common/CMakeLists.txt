#1.cmake version
cmake_minimum_required(VERSION 3.2)

#2.project name
project(RKAP_COMMON)

# ----------------------------------------------------------------------------
#  install headers
# ----------------------------------------------------------------------------
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
	DESTINATION "include"
	FILES_MATCHING PATTERN "*.h"
)

# ----------------------------------------------------------------------------
#  install libs
# ----------------------------------------------------------------------------
if (USE_32BIT)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/lib/libRKAP_Common.so
	DESTINATION "lib"
)
endif()

if (USE_64BIT)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/lib64/libRKAP_Common.so
	DESTINATION "lib"
)
endif()