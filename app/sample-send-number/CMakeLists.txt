cmake_minimum_required(VERSION 3.10)
project(send_number_sample)

set(CMAKE_CXX_STANDARD 17)

include_directories($ENV{HOME}/armv7/libdatachannel/include)
link_directories($ENV{HOME}/armv7/libdatachannel/lib)
link_directories($ENV{HOME}/armv7/openssl/lib)

add_executable(send_number main.cpp)
target_link_libraries(send_number datachannel ssl crypto)
