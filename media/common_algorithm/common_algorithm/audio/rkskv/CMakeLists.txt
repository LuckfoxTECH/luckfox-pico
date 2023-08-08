#1.cmake version
cmake_minimum_required(VERSION 3.2)

#2.project name
project(RKSKV)

# ----------------------------------------------------------------------------
#  install headers
# ----------------------------------------------------------------------------
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
	DESTINATION "include"
	FILES_MATCHING PATTERN "*.h"
)

# ----------------------------------------------------------------------------
#  install resource
# ----------------------------------------------------------------------------
if (NOT USE_UCLIBC)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/conf/
	DESTINATION "vqefiles"
	FILES_MATCHING PATTERN "*.json"
)
endif()

# ----------------------------------------------------------------------------
#  install libs
# ----------------------------------------------------------------------------
if (USE_32BIT)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/lib32/libaec_bf_process.so
	DESTINATION "lib"
)
endif()

if (USE_64BIT)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/lib64/libaec_bf_process.so
	DESTINATION "lib"
)
endif()
