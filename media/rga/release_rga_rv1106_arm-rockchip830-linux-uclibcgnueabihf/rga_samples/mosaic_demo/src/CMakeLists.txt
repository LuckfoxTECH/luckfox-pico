# rga_mosaic_demo
add_executable(rga_mosaic_demo
    rga_mosaic_demo.cpp
)
target_link_libraries(rga_mosaic_demo
    utils_obj
    ${RGA_LIB}
)
install(TARGETS rga_mosaic_demo DESTINATION ${CMAKE_INSTALL_BINDIR})
