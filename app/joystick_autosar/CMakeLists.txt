cmake_minimum_required(VERSION 3.10)
project(libdc_udp_demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)
find_package(PkgConfig QUIET)

# Try pkg-config first
set(LIBDATACHANNEL_LIBS "")
set(LIBDATACHANNEL_INCLUDE_DIRS "")
set(LIBDATACHANNEL_LIBRARY_DIRS "")

if (PKG_CONFIG_FOUND)
  pkg_check_modules(LIBDATACHANNEL libdatachannel)
endif()

if (LIBDATACHANNEL_FOUND)
  message(STATUS "Found libdatachannel via pkg-config")
  list(APPEND LIBDATACHANNEL_INCLUDE_DIRS ${LIBDATACHANNEL_INCLUDE_DIRS})
  list(APPEND LIBDATACHANNEL_LIBRARY_DIRS ${LIBDATACHANNEL_LIBRARY_DIRS})
  set(LIBDATACHANNEL_LIBS ${LIBDATACHANNEL_LIBRARIES})
else()
  message(STATUS "pkg-config couldn't find libdatachannel.")
  message(STATUS "You can export PKG_CONFIG_PATH or set environment variables LIBDATACHANNEL_INCLUDE_DIR and LIBDATACHANNEL_LIB before running cmake.")
  # Allow manual override via environment variables:
  if (DEFINED ENV{LIBDATACHANNEL_INCLUDE_DIR})
    set(LIBDATACHANNEL_INCLUDE_DIRS $ENV{LIBDATACHANNEL_INCLUDE_DIR})
    message(STATUS "Using LIBDATACHANNEL_INCLUDE_DIR = ${LIBDATACHANNEL_INCLUDE_DIRS}")
  else()
    # common default
    set(LIBDATACHANNEL_INCLUDE_DIRS /usr/local/include)
    message(STATUS "Defaulting LIBDATACHANNEL_INCLUDE_DIRS = ${LIBDATACHANNEL_INCLUDE_DIRS}")
  endif()

  if (DEFINED ENV{LIBDATACHANNEL_LIB})
    # user can provide full path or linker name(s)
    set(LIBDATACHANNEL_LIBS $ENV{LIBDATACHANNEL_LIB})
    message(STATUS "Using LIBDATACHANNEL_LIB = ${LIBDATACHANNEL_LIBS}")
  else()
    # sensible default linker names (adjust if your lib files differ)
    set(LIBDATACHANNEL_LIBS datachannel sctp ssl crypto pthread dl)
    message(STATUS "Defaulting LIBDATACHANNEL_LIBS = ${LIBDATACHANNEL_LIBS}")
  endif()

  # optional library directories (if user set PKG_CONFIG_PATH not used)
  if (DEFINED ENV{LIBDATACHANNEL_LIBRARY_DIR})
    set(LIBDATACHANNEL_LIBRARY_DIRS $ENV{LIBDATACHANNEL_LIBRARY_DIR})
    link_directories(${LIBDATACHANNEL_LIBRARY_DIRS})
  else()
    # we assume /usr/local/lib is in link path - you can add more if needed
    link_directories(/usr/local/lib)
  endif()
endif()

# include dirs for our headers
include_directories(${CMAKE_SOURCE_DIR}/src)
# also include libdatachannel headers
include_directories(${LIBDATACHANNEL_INCLUDE_DIRS})

# add signaling static lib
add_library(signaling STATIC src/udp_signaling.cpp src/udp_signaling.hpp)

# targets
add_executable(peer_server src/peer_server.cpp)
add_executable(peer_client src/peer_client.cpp)

# make sure signaling target exposes include dir (for consumers)
target_include_directories(signaling PUBLIC ${CMAKE_SOURCE_DIR}/src)

# Link libraries: correct ordering and visibility
# Note: target_link_libraries(<target> <PRIVATE|PUBLIC|INTERFACE> <libs...>)
target_link_libraries(peer_server
    PRIVATE
      signaling
      ${LIBDATACHANNEL_LIBS}
      Threads::Threads
)

target_link_libraries(peer_client
    PRIVATE
      signaling
      ${LIBDATACHANNEL_LIBS}
      Threads::Threads
)

# install rules (optional)
install(TARGETS peer_server peer_client DESTINATION bin)
