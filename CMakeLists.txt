# CMake file for the project

cmake_minimum_required (VERSION 3.20)
project(cliq)

set(CMAKE_CXX_STANDARD 17)

# Add Executable
add_executable(Cliq code/main.cpp)