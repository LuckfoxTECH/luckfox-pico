cmake_minimum_required( VERSION 2.8.8 )

set(RK_TEST_MOD_SRC
    test_mod_vpss.cpp
)

set(RK_TEST_COMMON_SRC
    test_comm_argparse.cpp
    test_comm_utils.cpp
    test_comm_bmp.cpp
    test_comm_imgproc.cpp
    test_comm_sys.cpp
    test_comm_rgn.cpp
    test_comm_venc.cpp
    test_comm_vpss.cpp
    test_comm_vdec.cpp
    test_comm_ao.cpp
    test_comm_tde.cpp
)

add_library(${RT_TEST_COMM_STATIC} STATIC ${RK_TEST_COMMON_SRC} ${RK_TEST_MOD_SRC})
set_target_properties(${RT_TEST_COMM_STATIC} PROPERTIES FOLDER "rt_test_comm")
target_link_libraries(${RT_TEST_COMM_STATIC})
