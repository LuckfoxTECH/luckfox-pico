# rga_allocator_malloc_demo
add_executable(rga_allocator_malloc_demo
    rga_allocator_malloc_demo.cpp
)
target_link_libraries(rga_allocator_malloc_demo
    utils_obj
	${RGA_LIB}
)
install(TARGETS rga_allocator_malloc_demo DESTINATION ${CMAKE_INSTALL_BINDIR})

if (TARGET_SOC STREQUAL "RV1106")
    # rga_allocator_dma32_demo
    add_executable(rga_allocator_1106_cma_demo
        rga_allocator_1106_cma_demo.cpp
    )
    target_link_libraries(rga_allocator_1106_cma_demo
        utils_obj
        ${RGA_LIB}
    )
    install(TARGETS rga_allocator_1106_cma_demo DESTINATION ${CMAKE_INSTALL_BINDIR})
else ()
    # rga_allocator_drm_demo
    add_executable(rga_allocator_drm_demo
        rga_allocator_drm_demo.cpp
    )
    target_link_libraries(rga_allocator_drm_demo
        utils_obj
        ${RGA_LIB}
    )
    install(TARGETS rga_allocator_drm_demo DESTINATION ${CMAKE_INSTALL_BINDIR})

    # rga_allocator_drm_phy_demo
    add_executable(rga_allocator_drm_phy_demo
        rga_allocator_drm_phy_demo.cpp
    )
    target_link_libraries(rga_allocator_drm_phy_demo
        utils_obj
        ${RGA_LIB}
    )
    install(TARGETS rga_allocator_drm_phy_demo DESTINATION ${CMAKE_INSTALL_BINDIR})

    # rga_allocator_dma_demo
    add_executable(rga_allocator_dma_demo
        rga_allocator_dma_demo.cpp
    )
    target_link_libraries(rga_allocator_dma_demo
        utils_obj
        ${RGA_LIB}
    )
    install(TARGETS rga_allocator_dma_demo DESTINATION ${CMAKE_INSTALL_BINDIR})

    # rga_allocator_dma_cache_demo
    add_executable(rga_allocator_dma_cache_demo
        rga_allocator_dma_cache_demo.cpp
    )
    target_link_libraries(rga_allocator_dma_cache_demo
        utils_obj
        ${RGA_LIB}
    )
    install(TARGETS rga_allocator_dma_cache_demo DESTINATION ${CMAKE_INSTALL_BINDIR})

    # rga_allocator_dma32_demo
    add_executable(rga_allocator_dma32_demo
        rga_allocator_dma32_demo.cpp
    )
    target_link_libraries(rga_allocator_dma32_demo
        utils_obj
        ${RGA_LIB}
    )
    install(TARGETS rga_allocator_dma32_demo DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
