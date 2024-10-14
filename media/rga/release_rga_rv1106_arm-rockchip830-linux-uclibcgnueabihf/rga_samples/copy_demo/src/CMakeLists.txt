# rga_copy_demo
SET(DEMO_NAME rga_copy_demo)
add_executable(${DEMO_NAME}
${DEMO_NAME}.cpp
)
target_link_libraries(${DEMO_NAME}
    utils_obj
    ${RGA_LIB}
)
install(TARGETS ${DEMO_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})

# rga_copy_fbc_demo
SET(DEMO_NAME rga_copy_fbc_demo)
add_executable(${DEMO_NAME}
${DEMO_NAME}.cpp
)
target_link_libraries(${DEMO_NAME}
    utils_obj
    ${RGA_LIB}
)
install(TARGETS ${DEMO_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})

# rga_copy_tile_demo
SET(DEMO_NAME rga_copy_tile_demo)
add_executable(${DEMO_NAME}
${DEMO_NAME}.cpp
)
target_link_libraries(${DEMO_NAME}
    utils_obj
    ${RGA_LIB}
)
install(TARGETS ${DEMO_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})

# rga_copy_splice_demo
SET(DEMO_NAME rga_copy_splice_demo)
add_executable(${DEMO_NAME}
${DEMO_NAME}.cpp
)
target_link_libraries(${DEMO_NAME}
    utils_obj
    ${RGA_LIB}
)
install(TARGETS ${DEMO_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})

# rga_copy_splice_task_demo
SET(DEMO_NAME rga_copy_splice_task_demo)
add_executable(${DEMO_NAME}
${DEMO_NAME}.cpp
)
target_link_libraries(${DEMO_NAME}
    utils_obj
    ${RGA_LIB}
)
install(TARGETS ${DEMO_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
