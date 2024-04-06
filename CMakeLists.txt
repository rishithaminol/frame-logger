cmake_minimum_required(VERSION 3.27)

project (frame-logger)

SET(BUILD_SHARED_LIBS OFF)
SET(CMAKE_BUILD_TYPE Debug)

add_compile_options(-Wall -Wextra -Wpedantic)

add_executable(frame-logger frame-logger.c)
