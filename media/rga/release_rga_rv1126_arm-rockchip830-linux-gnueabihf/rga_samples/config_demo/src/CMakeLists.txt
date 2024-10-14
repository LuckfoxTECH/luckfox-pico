# rga_config_thread_core_demo
add_executable(rga_config_thread_core_demo
    rga_config_thread_core_demo.cpp
)
target_link_libraries(rga_config_thread_core_demo
    utils_obj
    ${RGA_LIB}
)
install(TARGETS rga_config_thread_core_demo DESTINATION ${CMAKE_INSTALL_BINDIR})

# rga_config_single_core_demo
add_executable(rga_config_single_core_demo
    rga_config_single_core_demo.cpp
)
target_link_libraries(rga_config_single_core_demo
    utils_obj
    ${RGA_LIB}
)
install(TARGETS rga_config_single_core_demo DESTINATION ${CMAKE_INSTALL_BINDIR})
