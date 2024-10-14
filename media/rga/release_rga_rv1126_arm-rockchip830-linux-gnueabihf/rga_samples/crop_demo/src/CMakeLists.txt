# rga_crop_demo
add_executable(rga_crop_demo
    rga_crop_demo.cpp
)
target_link_libraries(rga_crop_demo
    utils_obj
    ${RGA_LIB}
)
install(TARGETS rga_crop_demo DESTINATION ${CMAKE_INSTALL_BINDIR})

# rga_crop_rect_demo
add_executable(rga_crop_rect_demo
    rga_crop_rect_demo.cpp
)
target_link_libraries(rga_crop_rect_demo
    utils_obj
    ${RGA_LIB}
)
install(TARGETS rga_crop_rect_demo DESTINATION ${CMAKE_INSTALL_BINDIR})
