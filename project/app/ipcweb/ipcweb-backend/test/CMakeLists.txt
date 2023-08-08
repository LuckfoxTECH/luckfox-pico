#
# Copyright 2019 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.
#
include(GoogleTest)

add_executable(ObjectsTest objects_unittest.cpp)
target_compile_features(ObjectsTest PRIVATE cxx_std_11)
target_link_libraries(ObjectsTest
    PRIVATE
    gtest
    gtest_main
    restapi)

add_executable(ResponseTest response_unittest.cpp)
target_compile_features(ResponseTest PRIVATE cxx_std_11)
target_link_libraries(ResponseTest
    PRIVATE
    gtest
    gtest_main
    restapi)

gtest_add_tests(TARGET ObjectsTest
    TEST_SUFFIX .noArgs
    TEST_LIST   noArgsTests
    )
gtest_add_tests(TARGET ResponseTest
    TEST_SUFFIX .noArgs
    TEST_LIST   noArgsTests
    )
