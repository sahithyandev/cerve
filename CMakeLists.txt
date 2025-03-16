cmake_minimum_required(VERSION 3.31.6)
project(Cerve VERSION 0.0.1)

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