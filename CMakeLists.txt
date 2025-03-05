cmake_minimum_required(VERSION 3.31.6)
project(Cerve VERSION 0.0.1)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build")

add_executable(cerve src/main.c)
