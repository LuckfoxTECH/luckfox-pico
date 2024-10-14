# rga_transform_rotate_demo
SET(DEMO_NAME rga_transform_rotate_demo)
add_executable(${DEMO_NAME}
    ${DEMO_NAME}.cpp
)
target_link_libraries(${DEMO_NAME}
    utils_obj
    ${RGA_LIB}
)
install(TARGETS ${DEMO_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})

# rga_transform_flip_demo
SET(DEMO_NAME rga_transform_flip_demo)
add_executable(${DEMO_NAME}
    ${DEMO_NAME}.cpp
)
target_link_libraries(${DEMO_NAME}
    utils_obj
    ${RGA_LIB}
)
install(TARGETS ${DEMO_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})

# rga_transform_rotate_flip_demo
SET(DEMO_NAME rga_transform_rotate_flip_demo)
add_executable(${DEMO_NAME}
    ${DEMO_NAME}.cpp
)
target_link_libraries(${DEMO_NAME}
    utils_obj
    ${RGA_LIB}
)
install(TARGETS ${DEMO_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})

