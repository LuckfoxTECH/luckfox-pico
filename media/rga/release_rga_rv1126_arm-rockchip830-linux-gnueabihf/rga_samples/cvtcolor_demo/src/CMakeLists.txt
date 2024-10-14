# rga_cvtcolor_demo
add_executable(rga_cvtcolor_demo
    rga_cvtcolor_demo.cpp
)
target_link_libraries(rga_cvtcolor_demo
    utils_obj
    ${RGA_LIB}
)
install(TARGETS rga_cvtcolor_demo DESTINATION ${CMAKE_INSTALL_BINDIR})

# rga_cvtcolor_csc_demo
add_executable(rga_cvtcolor_csc_demo
    rga_cvtcolor_csc_demo.cpp
)
target_link_libraries(rga_cvtcolor_csc_demo
    utils_obj
    ${RGA_LIB}
)
install(TARGETS rga_cvtcolor_csc_demo DESTINATION ${CMAKE_INSTALL_BINDIR})

# rga_cvtcolor_gray256_demo
add_executable(rga_cvtcolor_gray256_demo
    rga_cvtcolor_gray256_demo.cpp
)
target_link_libraries(rga_cvtcolor_gray256_demo
    utils_obj
    ${RGA_LIB}
)
install(TARGETS rga_cvtcolor_gray256_demo DESTINATION ${CMAKE_INSTALL_BINDIR})
