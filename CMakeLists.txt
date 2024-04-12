cmake_minimum_required(VERSION 3.27)

project (frame-logger)
add_compile_options(-Wall -Wextra -Wpedantic)

set(BUILD_SHARED_LIBS OFF)
set(CMAKE_BUILD_TYPE Debug)

set(SOURCES
    frame-logger.c
    logging.c
    processor.c
    ethertype.c
)

# -------------------------- json-c build process ---------------------------
set(JSONC_SOURCE_DIR ${CMAKE_SOURCE_DIR}/json-c)
set(JSONC_BIN_DIR ${JSONC_SOURCE_DIR}/build)

add_custom_command(
    OUTPUT ${JSONC_BIN_DIR}/libjson-c.a
    COMMAND ${CMAKE_COMMAND} -E make_directory ${JSONC_BIN_DIR}
    COMMAND ${CMAKE_COMMAND} -E chdir ${JSONC_BIN_DIR} ${CMAKE_COMMAND} -DBUILD_SHARED_LIBS=OFF ..
    COMMAND ${CMAKE_COMMAND} -E chdir ${JSONC_BIN_DIR} make
    WORKING_DIRECTORY ${JSONC_SOURCE_DIR}
    COMMENT "Building json-c library"
)

add_custom_target(json-c-build DEPENDS ${JSONC_BIN_DIR}/libjson-c.a)
include_directories(${JSONC_SOURCE_DIR} ${JSONC_BIN_DIR})
link_directories(${JSONC_BIN_DIR})
# -------------------------- json-c build process ---------------------------

add_executable(${PROJECT_NAME} ${SOURCES})
add_dependencies(${PROJECT_NAME} json-c-build)

target_link_libraries(${PROJECT_NAME} pcap)
target_link_libraries(${PROJECT_NAME} json-c)
