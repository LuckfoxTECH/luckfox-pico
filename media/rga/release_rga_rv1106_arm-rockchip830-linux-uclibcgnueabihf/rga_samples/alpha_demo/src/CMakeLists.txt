# rga_alpha_demo
add_executable(rga_alpha_demo
    rga_alpha_demo.cpp
)
target_link_libraries(rga_alpha_demo
    utils_obj
    ${RGA_LIB}
)
install(TARGETS rga_alpha_demo DESTINATION ${CMAKE_INSTALL_BINDIR})

# rga_alpha_global_alpha_demo
add_executable(rga_alpha_global_alpha_demo
    rga_alpha_global_alpha_demo.cpp
)
target_link_libraries(rga_alpha_global_alpha_demo
    utils_obj
    ${RGA_LIB}
)
install(TARGETS rga_alpha_global_alpha_demo DESTINATION ${CMAKE_INSTALL_BINDIR})

# rga_alpha_3channel_demo
add_executable(rga_alpha_3channel_demo
    rga_alpha_3channel_demo.cpp
)
target_link_libraries(rga_alpha_3channel_demo
    utils_obj
    ${RGA_LIB}
)
install(TARGETS rga_alpha_3channel_demo DESTINATION ${CMAKE_INSTALL_BINDIR})

# rga_alpha_3channel_demo
add_executable(rga_alpha_rgba5551_demo
    rga_alpha_rgba5551_demo.cpp
)
target_link_libraries(rga_alpha_rgba5551_demo
    utils_obj
    ${RGA_LIB}
)
install(TARGETS rga_alpha_rgba5551_demo DESTINATION ${CMAKE_INSTALL_BINDIR})

# rga_alpha_yuv_demo
add_executable(rga_alpha_yuv_demo
    rga_alpha_yuv_demo.cpp
)
target_link_libraries(rga_alpha_yuv_demo
    utils_obj
    ${RGA_LIB}
)
install(TARGETS rga_alpha_yuv_demo DESTINATION ${CMAKE_INSTALL_BINDIR})

# rga_alpha_colorkey_demo
add_executable(rga_alpha_colorkey_demo
    rga_alpha_colorkey_demo.cpp
)
target_link_libraries(rga_alpha_colorkey_demo
    utils_obj
    ${RGA_LIB}
)
install(TARGETS rga_alpha_colorkey_demo DESTINATION ${CMAKE_INSTALL_BINDIR})

# rga_alpha_osd_demo
add_executable(rga_alpha_osd_demo
    rga_alpha_osd_demo.cpp
)
target_link_libraries(rga_alpha_osd_demo
    utils_obj
    ${RGA_LIB}
)
install(TARGETS rga_alpha_osd_demo DESTINATION ${CMAKE_INSTALL_BINDIR})

