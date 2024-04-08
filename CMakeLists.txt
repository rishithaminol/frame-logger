cmake_minimum_required(VERSION 3.27)

project (frame-logger)
add_compile_options(-Wall -Wextra -Wpedantic)

SET(BUILD_SHARED_LIBS OFF)
SET(CMAKE_BUILD_TYPE Debug)

set(SOURCES
    frame-logger.c
)

add_executable(frame-logger ${SOURCES})

target_link_libraries(frame-logger pcap)
