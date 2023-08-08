#1.cmake version
cmake_minimum_required(VERSION 3.2)

#2.project name
project(occlusion_detect)

install(DIRECTORY include/ DESTINATION "include/occlusion_detect")
if (USE_32BIT)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/lib/libod_share.so
	DESTINATION "lib"
)
endif()
if (USE_64BIT)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/lib64/libod_share.so
        DESTINATION "lib"
)
endif()


if(COMPILES_EXAMPLES)
	add_subdirectory(test)
endif()
