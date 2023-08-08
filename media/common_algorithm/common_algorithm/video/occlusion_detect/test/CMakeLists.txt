#1.cmake version
cmake_minimum_required(VERSION 3.2)

#2.project name
project(test_od)

set(PROJECT_SOURCE_DIR ./)
#3. header file path
include_directories(
	${PROJECT_SOURCE_DIR}/../include)

#4. add lib path
link_directories(
	${PROJECT_SOURCE_DIR}/../lib)

#5. add executable file
add_executable(${PROJECT_NAME}
	${PROJECT_SOURCE_DIR}/test.c)

#6. add link library
target_link_libraries(${PROJECT_NAME}
	od_share)

install(TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION bin)
