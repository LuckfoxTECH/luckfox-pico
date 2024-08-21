add_library(utils_obj OBJECT "")

SET(THIRD_PARTY_PATH ${CMAKE_CURRENT_LIST_DIR}/3rdparty)

include(${CMAKE_CURRENT_LIST_DIR}/allocator/CMakeLists.txt)

target_sources(utils_obj
    PRIVATE
  	    ${CMAKE_CURRENT_LIST_DIR}/utils.cpp
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/utils.h
)

target_include_directories(utils_obj
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

SET(RGA_SAMPLES_UTILS_COMPILED "true" CACHE STRING "")
