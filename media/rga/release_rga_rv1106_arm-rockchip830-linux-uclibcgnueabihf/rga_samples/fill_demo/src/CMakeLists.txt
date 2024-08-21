# rga_fill_demo
SET(DEMO_NAME rga_fill_demo)
add_executable(${DEMO_NAME}
    ${DEMO_NAME}.cpp
)
target_link_libraries(${DEMO_NAME}
    utils_obj
	${RGA_LIB}
)
install(TARGETS ${DEMO_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})

# rga_fill_rectangle_demo
SET(DEMO_NAME rga_fill_rectangle_demo)
add_executable(${DEMO_NAME}
    ${DEMO_NAME}.cpp
)
target_link_libraries(${DEMO_NAME}
    utils_obj
    ${RGA_LIB}
)
install(TARGETS ${DEMO_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})

# rga_fill_rectangle_array_demo
SET(DEMO_NAME rga_fill_rectangle_array_demo)
add_executable(${DEMO_NAME}
    ${DEMO_NAME}.cpp
)
target_link_libraries(${DEMO_NAME}
    utils_obj
    ${RGA_LIB}
)
install(TARGETS ${DEMO_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})

# rga_fill_rectangle_task_demo
SET(DEMO_NAME rga_fill_rectangle_task_demo)
add_executable(${DEMO_NAME}
    ${DEMO_NAME}.cpp
)
target_link_libraries(${DEMO_NAME}
    utils_obj
    ${RGA_LIB}
)
install(TARGETS ${DEMO_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})

# rga_fill_rectangle_task_array_demo
SET(DEMO_NAME rga_fill_rectangle_task_array_demo)
add_executable(${DEMO_NAME}
    ${DEMO_NAME}.cpp
)
target_link_libraries(${DEMO_NAME}
    utils_obj
    ${RGA_LIB}
)
install(TARGETS ${DEMO_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})

