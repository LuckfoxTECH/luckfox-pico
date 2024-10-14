# rga_padding_demo
SET(DEMO_NAME rga_padding_demo)
add_executable(${DEMO_NAME}
    ${DEMO_NAME}.cpp
)
target_link_libraries(${DEMO_NAME}
    utils_obj
    ${RGA_LIB}
)
install(TARGETS ${DEMO_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})

