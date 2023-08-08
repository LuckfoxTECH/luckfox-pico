# ----------------------------------------------------------------------------
# Create git version information
# ----------------------------------------------------------------------------
set(VERSION_CNT         0)
set(VERSION_MAX_CNT     5)
set(VERSION_INFO        "\"unknown rkadk version for missing VCS info\"")
set(BUILD_INFO          "\"unknown rkadk build info\"")

if(EXISTS "${PROJECT_SOURCE_DIR}/.git")
    find_package(Git)
    if(GIT_FOUND)
        # get current version info
        set(GIT_LOG_FORMAT "git-%h %cd")

        execute_process(COMMAND ${GIT_EXECUTABLE} log -1 --oneline --pretty=format:${GIT_LOG_FORMAT}
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            OUTPUT_VARIABLE EXEC_OUT
            ERROR_VARIABLE EXEC_ERROR
            RESULT_VARIABLE EXEC_RET
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_STRIP_TRAILING_WHITESPACE)

        if (NOT EXEC_RET)
            set(VERSION_INFO ${EXEC_OUT})
            message(STATUS "git current version:")
            message(STATUS "${VERSION_INFO}")
            string(REPLACE "\"" "\\\"" VERSION_INFO ${VERSION_INFO})
            set(VERSION_INFO "\"${VERSION_INFO}\"")
        else()
            message(STATUS "git ret ${EXEC_RET}")
            message(STATUS "${EXEC_ERROR}")
        endif()

        # get version build info
        execute_process(COMMAND ${GIT_EXECUTABLE} config user.name
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            OUTPUT_VARIABLE EXEC_OUT
            ERROR_VARIABLE EXEC_ERROR
            RESULT_VARIABLE EXEC_RET
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_STRIP_TRAILING_WHITESPACE)

        if (NOT EXEC_RET)
            set(BUILD_AUTHOR ${EXEC_OUT})
            string(SUBSTRING ${BUILD_AUTHOR} 0 3 BUILD_AUTHOR)
            string(TIMESTAMP BUILD_TIME "%Y-%m-%d %H:%M:%S")
            set(BUILD_INFO "built ${BUILD_TIME}")
            set(BUILD_INFO "\"${BUILD_INFO}\"")
            message(STATUS "${BUILD_INFO}")
        else()
            message(STATUS "git ret ${EXEC_RET}")
            message(STATUS "${EXEC_ERROR}")
        endif()

        configure_file(
            "${PROJECT_SOURCE_DIR}/version/version.in"
            "${PROJECT_SOURCE_DIR}/include/version.h"
        )
    endif()
endif(EXISTS "${PROJECT_SOURCE_DIR}/.git")
