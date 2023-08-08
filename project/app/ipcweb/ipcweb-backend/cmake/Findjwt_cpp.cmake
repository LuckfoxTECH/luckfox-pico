find_package(PkgConfig QUIET)

if(NOT EXISTS "${JWTCPP_INCLUDE_DIR}")
    find_path(JWTCPP_INCLUDE_DIR
        NAMES jwt.h
        DOC "json web token library header files"
        )
endif()

if(EXISTS "${JWTCPP_INCLUDE_DIR}")
    include(FindPackageHandleStandardArgs)
    mark_as_advanced(JWTCPP_INCLUDE_DIR)
else()
    include(ExternalProject)
    find_program(MAKE_EXE NAMES make gmake nmake)
    ExternalProject_Add(
        jwt-cpp
        SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/jwt-cpp"
        PREFIX "${CMAKE_CURRENT_BINARY_DIR}/thirdparty/jwt-cpp"
        BUILD_IN_SOURCE true
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
        UPDATE_COMMAND ""
        )
    ExternalProject_Get_Property(jwt-cpp source_dir)
    set(JWTCPP_INCLUDE_DIR ${source_dir}/include)
endif()

if(EXISTS "${JWTCPP_INCLUDE_DIR}")
    set(JWTCPP_FOUND 1)
else()
    set(JWTCPP_FOUND 0)
endif()
