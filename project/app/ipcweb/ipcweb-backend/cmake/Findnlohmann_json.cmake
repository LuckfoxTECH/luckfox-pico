if(NOT EXISTS ${NLOHMANN_JSON_INCLUDE_DIR})
    find_path(NLOHMANN_JSON_INCLUDE_DIR
        NAMES nlohmann/json.hpp
        DOC "json library header files"
        )
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(nlohmann_json DEFAULT_MSG
    NLOHMANN_JSON_INCLUDE_DIR)
mark_as_advanced(nlohmann_json NLOHMANN_JSON_INCLUDE_DIR)

if(NOT nlohmann_json_FOUND)
    set(JSON_BuildTests OFF CACHE STRING "")
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/nlohmann_json)
    set(nlohmann_json_FOUND 1)
endif()

if(nlohmann_json_FOUND AND NOT TARGET nlohmann_json::nlohmann_json)
    add_library(nlohmann_json INTERFACE)
    add_library(nlohmann_json::nlohmann_json ALIAS nlohmann_json)
    set_property(TARGET nlohmann_json PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${NLOHMANN_JSON_INCLUDE_DIR})
endif()
