#1.cmake version
cmake_minimum_required(VERSION 3.2)

#2.project name
project(RKAUDIOUCLIBC)

# ----------------------------------------------------------------------------
#  install resource
# ----------------------------------------------------------------------------
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
	DESTINATION "vqefiles"
	FILES_MATCHING PATTERN "*.json"
)

# ----------------------------------------------------------------------------
#  install headers
# ----------------------------------------------------------------------------
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
DESTINATION "include"
FILES_MATCHING PATTERN "*.h"
)

# ----------------------------------------------------------------------------
#  install libs
# ----------------------------------------------------------------------------
if (RKAPPLUS_ENABLE)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/libaec_bf_process.so
		${CMAKE_CURRENT_SOURCE_DIR}/libaec_bf_process.a
	DESTINATION "lib"
)
endif()

if (RKAPPLUS_AED_ENABLE)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/librkaudio_detect.so
		${CMAKE_CURRENT_SOURCE_DIR}/librkaudio_detect.a
	DESTINATION "lib"
)
endif()

if (RKAPPLUS_ENABLE OR RKAPPLUS_AED_ENABLE)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/librkaudio_common.so
		${CMAKE_CURRENT_SOURCE_DIR}/librkaudio_common.a
	DESTINATION "lib"
)
endif()

