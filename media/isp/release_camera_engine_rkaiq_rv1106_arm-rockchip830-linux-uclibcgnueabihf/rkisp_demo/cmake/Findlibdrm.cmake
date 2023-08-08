find_package(PkgConfig QUIET)
pkg_check_modules(libdrm QUIET "libdrm")
pkg_check_modules(libdrm_rockchip QUIET "libdrm_rockchip")

include(FindPackageHandleStandardArgs)
find_path(LIBDRM_INCLUDE_DIR
    NAMES
    drm.h
    PATH
    include
    PATH_SUFFIXES
    drm
)

find_path(LIBDRM_XF86DRM_INCLUDE_DIR
    NAMES
    xf86drm.h
    PATH
    include
)
find_library(LIBDRM_LIBRARY
    drm
)

find_package_handle_standard_args(libdrm
    DEFAULT_MSGS
    LIBDRM_INCLUDE_DIR
    LIBDRM_LIBRARY
)
mark_as_advanced(
    LIBDRM_INCLUDE_DIR
    LIBDRM_LIBRARY
)

if(LIBDRM_PLATFORM_ROCKCHIP)
find_path(LIBDRM_ROCKCHIP_INCLUDE_DIR
    NAMES
    rockchip_drm.h
    PATH
    include
    PATH_SUFFIXES
    libdrm
)
find_library(LIBDRM_ROCKCHIP_LIBRARY
    drm_rockchip
)
find_package_handle_standard_args(libdrm_rockchip
    DEFAULT_MSGS
    LIBDRM_ROCKCHIP_INCLUDE_DIR
    LIBDRM_ROCKCHIP_LIBRARY
)
mark_as_advanced(
    LIBDRM_ROCKCHIP_INCLUDE_DIR
    LIBDRM_ROCKCHIP_LIBRARY
)
endif()

if(libdrm_FOUND OR libdrm_rockchip_FOUND)
    if(libdrm_rockchip_FOUND)
        set(LIBDRM_LIBRARIES    ${LIBDRM_LIBRARY};${LIBDRM_ROCKCHIP_LIBRARY})
        set(LIBDRM_INCLUDE_DIRS ${LIBDRM_ROCKCHIP_INCLUDE_DIR};${LIBDRM_XF86DRM_INCLUDE_DIR};${LIBDRM_INCLUDE_DIR})
        set(LIBDRM_CFLAGS "-DHAS_LIBDRM -DHAS_LIBDRM_ROCKCHIP")
    else()
        set(LIBDRM_LIBRARIES    ${LIBDRM_LIBRARY})
        set(LIBDRM_INCLUDE_DIRS ${LIBDRM_INCLUDE_DIR})
        set(LIBDRM_CFLAGS "-DHAS_LIBDRM")
    endif()

    if(LIBDRM_LIBRARY AND NOT TARGET libdrm::libdrm)
        if(IS_ABSOLUTE "${LIBDRM_LIBRARY}")
            add_library(libdrm::libdrm UNKNOWN IMPORTED)
            set_target_properties(libdrm::libdrm PROPERTIES IMPORTED_LOCATION "${LIBDRM_LIBRARIES}")
        else()
            add_library(libdrm::libdrm INTERFACE IMPORTED)
            set_target_properties(libdrm::libdrm PROPERTIES IMPORTED_LIBNAME "${LIBDRM_LIBRARIES}")
        endif()
        set_target_properties(libdrm::libdrm PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${LIBDRM_INCLUDE_DIRS}")
    endif()
endif()
