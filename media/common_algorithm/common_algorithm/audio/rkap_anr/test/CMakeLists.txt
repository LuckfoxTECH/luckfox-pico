cmake_minimum_required(VERSION 3.2)

project(test_anr)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/../include
	${CMAKE_CURRENT_SOURCE_DIR}/../../rkap_common/include)

link_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/../lib
	${CMAKE_CURRENT_SOURCE_DIR}/../../rkap_common/lib)

# add executable file
add_executable(${PROJECT_NAME}
	${CMAKE_CURRENT_SOURCE_DIR}/test_anr.c)

# add link library
target_link_libraries(${PROJECT_NAME}
	RKAP_Common	
	RKAP_ANR
	m)

install(TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION bin)
