find_package(PkgConfig QUIET)
pkg_check_modules(PKG_CGICC QUIET "libcgicc")
set(CGICC_DEFINITIONS ${PKG_CGICC_CFLAGS_OTHER})

find_path(CGICC_INCLUDE_DIR
    NAMES cgicc/Cgicc.h
    HINTS ${PKG_CGICC_INCLUDEDIR} ${PKG_CGICC_INCLUDE_DIRS})

find_library(CGICC_LIBRARY
    NAMES cgicc
    HINTS /usr/lib/x86_64-linux-gnu ${PKG_CGICC_LIBDIR} ${PKG_CGICC_LIBRARY_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Cgicc DEFAULT_MSG
    CGICC_LIBRARY CGICC_INCLUDE_DIR)
mark_as_advanced(Cgicc_FOUND CGICC_INCLUDE_DIR CGICC_LIBRARY)

if(NOT Cgicc_FOUND)
    include(ExternalProject)
    find_program(MAKE_EXE NAMES make gmake nmake)
    ExternalProject_Add(
        Cgicc
        SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/cgicc"
        PREFIX "${CMAKE_CURRENT_BINARY_DIR}/thirdparty/cgicc"
        BUILD_BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/thirdparty/cgicc/lib/${CMAKE_SHARED_LIBRARY_PREFIX}cgicc${CMAKE_SHARED_LIBRARY_SUFFIX}"
        BUILD_IN_SOURCE true
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ./configure --host=${TARGET_HOST} CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER} --prefix=${CMAKE_CURRENT_BINARY_DIR}/thirdparty/cgicc && ${MAKE_EXE}
        INSTALL_COMMAND ${MAKE_EXE} install
        )
    ExternalProject_Get_Property(Cgicc install_dir)
    ExternalProject_Get_Property(Cgicc source_dir)
    set(CGICC_INCLUDE_DIR ${source_dir})
    set(CGICC_LIBRARY ${install_dir}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}cgicc${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(Cgicc_FOUND 1)
endif()

if(Cgicc_FOUND AND NOT TARGET Cgicc::Cgicc)
    add_library(Cgicc::Cgicc UNKNOWN IMPORTED)
    add_dependencies(Cgicc::Cgicc Cgicc)
    set_property(TARGET Cgicc::Cgicc PROPERTY IMPORTED_LOCATION ${CGICC_LIBRARY})
    set_property(TARGET Cgicc::Cgicc PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${CGICC_INCLUDE_DIR}")
endif()
