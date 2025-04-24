cmake_minimum_required(VERSION 3.26)

project (frame-logger VERSION 0.1.1)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 1)

set(BUILD_SHARED_LIBS OFF)
set(CMAKE_BUILD_TYPE Debug)

add_compile_options(-Wall -Wextra -Wpedantic)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  add_compile_options(-DLINUX_OS)
elseif(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
  add_compile_options(-DFREEBSD_OS)
endif()

# Version header generation
configure_file(prog_version.h.in ${CMAKE_BINARY_DIR}/prog_version.h)
include_directories(${CMAKE_BINARY_DIR})

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


set(SOURCES
    src/frame-logger.c
    src/logging.c
    src/processor.c
    src/ethertype.c
    src/timeutils.c
    src/packet_chain.c
)

add_executable(${PROJECT_NAME} ${SOURCES})
add_dependencies(${PROJECT_NAME} json-c-build)

target_link_libraries(${PROJECT_NAME} pcap json-c pthread)

enable_testing()
add_subdirectory(tests)
include(CTest)
