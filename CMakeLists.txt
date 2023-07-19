# CMake file for the project

cmake_minimum_required (VERSION 3.20)
project(cliq)

set(CMAKE_CXX_STANDARD 17)

add_executable(Cliq ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp )
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Including external libraries
find_package(libwebsockets CONFIG REQUIRED)
    target_link_libraries(Cliq PRIVATE websockets_shared)

find_package(simdjson CONFIG REQUIRED)
    target_link_libraries(Cliq PRIVATE simdjson::simdjson)