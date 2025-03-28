cmake_minimum_required(VERSION 3.31.6)
project(Cerve VERSION 0.1.0 LANGUAGES C)

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

set(CWALK_SOURCE_DIR "${PROJECT_SOURCE_DIR}/lib/cwalk") # Adjust path if necessary
set(CWALK_SOURCES "${CWALK_SOURCE_DIR}/src/cwalk.c")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build")

configure_file(
  ${PROJECT_SOURCE_DIR}/src/config.h.in
  ${PROJECT_SOURCE_DIR}/src/config.h
  @ONLY
)

# Add cwalk as a separate library
add_library(cwalk STATIC lib/cwalk/src/cwalk.c)
target_include_directories(cwalk PUBLIC lib/cwalk/include)

# Add executable and link cwalk
add_executable(cerve src/main.c ${CWALK_SOURCES})
target_link_libraries(cerve PRIVATE cwalk)
target_include_directories(cerve PRIVATE "${PROJECT_SOURCE_DIR}/lib/cwalk/include")
