cmake_minimum_required(VERSION 3.26)

project (frame-logger VERSION 0.1.0)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)

configure_file(prog_version.h.in ${CMAKE_BINARY_DIR}/prog_version.h)
include_directories(${CMAKE_BINARY_DIR})

add_compile_options(-Wall -Wextra -Wpedantic)

enable_testing()

set(BUILD_SHARED_LIBS OFF)
set(CMAKE_BUILD_TYPE Debug)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  add_compile_options(-DLINUX_OS)
elseif(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
  add_compile_options(-DFREEBSD_OS)
endif()

set(SOURCES
    frame-logger.c
    logging.c
    processor.c
    ethertype.c
    timeutils.c
    packet_chain.c
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
target_link_libraries(${PROJECT_NAME} pthread)

add_subdirectory(tests)
include(CTest)
