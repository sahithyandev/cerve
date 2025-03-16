cmake_minimum_required(VERSION 3.31.6)
project(Cerve VERSION 0.0.1)

find_program(CLANG_FORMAT "clang-format")
if (NOT CLANG_FORMAT)
    message(WARNING "clang-format not found. Code formatting target will not be available.")
else()
    file(GLOB_RECURSE ALL_SOURCE_FILES
            "*.c"
            "*.h"
    )
    add_custom_target(format
            COMMAND ${CLANG_FORMAT} -i ${ALL_SOURCE_FILES}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Formatting all source files with clang-format..."
    )
endif()

find_library(CWALK_LIBRARY cwalk)
if (NOT CWALK_LIBRARY)
    message(FATAL_ERROR "cwalk library not found!")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build")
configure_file(
  ${PROJECT_SOURCE_DIR}/src/config.h.in
  ${PROJECT_SOURCE_DIR}/src/config.h
  @ONLY
)

add_executable(cerve src/main.c)

target_link_libraries(cerve ${CWALK_LIBRARY})