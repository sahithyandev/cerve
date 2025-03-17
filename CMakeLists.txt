cmake_minimum_required(VERSION 3.31.6)
project(Cerve VERSION 0.0.4)

find_program(CLANG_FORMAT "clang-format")
if (NOT CLANG_FORMAT)
    message(WARNING "clang-format not found. Code formatting target will not be available.")
else()
    file(GLOB_RECURSE ALL_SOURCE_FILES
            "src/*.c"
            "src/*.h"
    )
    add_custom_target(format
            COMMAND ${CLANG_FORMAT} -i ${ALL_SOURCE_FILES} --verbose
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Formatting all source files with clang-format..."
    )
endif()

include(FetchContent)
FetchContent_Declare(
        cwalk
        GIT_REPOSITORY https://github.com/likle/cwalk.git
        GIT_TAG v1.2.9
)
FetchContent_MakeAvailable(cwalk)

target_include_directories(cerve PRIVATE ${cwalk_SOURCE_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build")
configure_file(
  ${PROJECT_SOURCE_DIR}/src/config.h.in
  ${PROJECT_SOURCE_DIR}/src/config.h
  @ONLY
)

add_executable(cerve src/main.c)
