file(GLOB TEST_SOURCES "./*.c")
file(GLOB C_MODE_SOURCES "./c_mode/*.c")
include_directories(include)
include_directories(include/c_mode)

set(RKCRYPTO_TEST_NAME librkcrypto_test)

add_executable(${RKCRYPTO_TEST_NAME} ${TEST_SOURCES} ${C_MODE_SOURCES})

target_link_libraries(librkcrypto_test rkcrypto)

install(TARGETS ${RKCRYPTO_TEST_NAME} RUNTIME DESTINATION "bin")
