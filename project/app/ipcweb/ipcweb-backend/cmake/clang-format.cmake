# additional target to perform clang-format run, requires clang-format

# get all project files
file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp *.h)
foreach (SOURCE_FILE ${ALL_SOURCE_FILES})
    string(FIND "${SOURCE_FILE}" "${PROJECT_THIRDPARTY_DIR}" PROJECT_THIRDPARTY_DIR_FOUND)
    if (NOT ${PROJECT_THIRDPARTY_DIR_FOUND} EQUAL -1)
        list(REMOVE_ITEM ALL_SOURCE_FILES ${SOURCE_FILE})
    endif ()
endforeach ()

add_custom_target(
    clangformat
    COMMAND /usr/bin/clang-format
    -style=LLVM
    -i
    ${ALL_SOURCE_FILES}
)
