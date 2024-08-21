cmake_minimum_required(VERSION 3.0)

project(RockAA_Utils)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

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

# rockaa_capt
add_executable(rockaa_capt
  rockaa_capt.c
)
target_link_libraries(rockaa_capt
  rockaa
)
install(TARGETS rockaa_capt
  RUNTIME DESTINATION "bin"
)

# rockaa_play
add_executable(rockaa_play
  rockaa_play.c
)
target_link_libraries(rockaa_play
  rockaa
)
install(TARGETS rockaa_play
  RUNTIME DESTINATION "bin"
)

