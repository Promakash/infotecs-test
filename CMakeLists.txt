cmake_minimum_required(VERSION 3.13)

project (infotecs_task)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(client)
add_subdirectory(server)