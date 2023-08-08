cmake_minimum_required( VERSION 2.8.8 )
project (rockit)
include_directories(sdk/include)
if(ARCH64)
set(ROCKIT_DEP_COMMON_LIBS
    ${CMAKE_CURRENT_LIST_DIR}/../lib/lib64/librockit.so
)
set(ROCKIT_FILE_LIBS
    ${ROCKIT_DEP_COMMON_LIBS}
    ${CMAKE_CURRENT_LIST_DIR}/sdk/lib64/libgraphic_lsf.so
    ${CMAKE_CURRENT_LIST_DIR}/sdk/lib64/librkgfx_avs.so
	${CMAKE_CURRENT_LIST_DIR}/sdk/lib64/libpanoStitchApp.so
)
set(ROCKIT_DUMPSYS_FILE ${CMAKE_CURRENT_LIST_DIR}/example/bin/bin64/dumpsys)
else()
set(ROCKIT_DEP_COMMON_LIBS
    ${CMAKE_CURRENT_LIST_DIR}/../lib/lib32/librockit.so
)
set(ROCKIT_DEP_COMMON_LIBS_STATIC
    ${CMAKE_CURRENT_LIST_DIR}/../lib/lib32/librockit.a
)
set(ROCKIT_FILE_LIBS
    ${ROCKIT_DEP_COMMON_LIBS}
    ${CMAKE_CURRENT_LIST_DIR}/../lib/lib32/librockit_tiny.so
    ${CMAKE_CURRENT_LIST_DIR}/../lib/lib32/librockit_tiny.a
    ${CMAKE_CURRENT_LIST_DIR}/../lib/lib32/librockit_full.so
    ${CMAKE_CURRENT_LIST_DIR}/../lib/lib32/librockit_full.a
)
set(ROCKIT_DUMPSYS_FILE ${CMAKE_CURRENT_LIST_DIR}/example/bin/bin32/dumpsys)
endif()

add_subdirectory(sdk)
add_subdirectory(example)
