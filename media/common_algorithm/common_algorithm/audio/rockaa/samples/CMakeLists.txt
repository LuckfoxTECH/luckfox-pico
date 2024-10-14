cmake_minimum_required(VERSION 3.0)

project(RockAA_Samples)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

message("USE_UCLIBC: ${USE_UCLIBC}")
message("USE_32BIT: ${USE_32BIT}")
message("USE_64BIT: ${USE_64BIT}")

if (USE_UCLIBC)
link_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/../lib/uclibc
)
elseif (USE_32BIT)
link_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/../lib/lib32
)
elseif (USE_64BIT)
link_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/../lib/lib64
)
else()
  message("Unsupported platform")
endif()

# rockaa_sample_capt
add_executable(rockaa_sample_capt
  rockaa_sample_capt.c
)
target_link_libraries(rockaa_sample_capt
  rockaa
)
install(TARGETS rockaa_sample_capt
  RUNTIME DESTINATION "bin"
)

# rockaa_sample_play
add_executable(rockaa_sample_play
  rockaa_sample_play.c
)
target_link_libraries(rockaa_sample_play
  rockaa
)
install(TARGETS rockaa_sample_play
  RUNTIME DESTINATION "bin"
)

