cmake_minimum_required(VERSION 2.8...3.22)

project(iio_test)

link_directories(iio_test lib)
include_directories(iio_test include)

add_executable(iio_test demo/rkiio_aiq_api_test.c)
target_link_libraries(iio_test PUBLIC easyiio iio serialport xml2)
