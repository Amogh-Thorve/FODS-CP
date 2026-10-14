cmake_minimum_required(VERSION 3.10)
project(CONFIGCORE C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

file(GLOB SOURCES "src/*.c")
file(GLOB ENGINE_SOURCES "src/*.c")
list(REMOVE_ITEM ENGINE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.c")
list(REMOVE_ITEM ENGINE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/server.c")

add_executable(configcore ${SOURCES})

if (WIN32)
    target_link_libraries(configcore ws2_32)
endif()

# Unit tests
add_executable(test_runner tests/test_main.c ${ENGINE_SOURCES})
