#1.cmake version
cmake_minimum_required(VERSION 3.2)

#2.project name
project(RKAUDIOALGORITHMS)

# ----------------------------------------------------------------------------
#  install resource
# ----------------------------------------------------------------------------
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/conf/
	DESTINATION "vqefiles"
	FILES_MATCHING PATTERN "*.json"
)

# ----------------------------------------------------------------------------
#  install headers
# ----------------------------------------------------------------------------
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/include/
	DESTINATION "include"
	FILES_MATCHING PATTERN "*.h"
)

# ----------------------------------------------------------------------------
#  install libs
# ----------------------------------------------------------------------------
if (USE_UCLIBC)
	install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/lib/uclibc/librockaa.so
			DESTINATION "lib")
endif()

if (USE_32BIT)
	install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/lib/lib32/librockaa.so
			DESTINATION "lib")
endif()

if (USE_64BIT)
	install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/lib/lib64/librockaa.so
			DESTINATION "lib")
endif()

add_subdirectory(samples)
add_subdirectory(utils)
