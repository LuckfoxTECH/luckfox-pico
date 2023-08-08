find_package(PkgConfig QUIET)
pkg_check_modules(librga QUIET "librga")

include(FindPackageHandleStandardArgs)
find_path(LIBRGA_INCLUDE_DIR
    NAMES
    im2d.h
    PATH
    include
    PATH_SUFFIXES
    rga
)
find_library(LIBRGA_LIBRARY
    rga
)
find_package_handle_standard_args(librga
    DEFAULT_MSGS
    LIBRGA_INCLUDE_DIR
    LIBRGA_LIBRARY
)
mark_as_advanced(
    LIBRGA_INCLUDE_DIR
    LIBRGA_LIBRARY
)

if(librga_FOUND)
	get_filename_component(LIBRGA_LIBRARY_DIR "${LIBRGA_LIBRARY}" PATH)
	set(LIBRGA_LIBRARIES    ${LIBRGA_LIBRARY})
	set(LIBRGA_INCLUDE_DIRS ${LIBRGA_INCLUDE_DIR})
    set(LIBRGA_CFLAGS "-DHAS_LIBRGA")

	if(LIBRGA_LIBRARY AND NOT TARGET librga::librga)
		if(IS_ABSOLUTE "${LIBRGA_LIBRARY}")
			add_library(librga::librga UNKNOWN IMPORTED)
			set_target_properties(librga::librga PROPERTIES IMPORTED_LOCATION
				"${LIBRGA_LIBRARY}")
		else()
			add_library(librga::librga INTERFACE IMPORTED)
			set_target_properties(librga::librga PROPERTIES IMPORTED_LIBNAME
				"${LIBRGA_LIBRARY}")
		endif()
		set_target_properties(librga::librga PROPERTIES INTERFACE_INCLUDE_DIRECTORIES
			"${LIBRGA_INCLUDE_DIR}")
	endif()
endif()
