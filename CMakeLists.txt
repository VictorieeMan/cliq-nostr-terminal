# CMake file for the project

cmake_minimum_required (VERSION 3.20)
project(cliq)

set(CMAKE_CXX_STANDARD 17)

#add_library()

add_executable(Cliq ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(unofficial-vcpkg::libwebsockets CONFIG REQUIRED)
target_link_libraries(Cliq PRIVATE unofficial-vcpkg::libwebsockets)

#[[
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/libwebsockets-4.3.2/include)
target_link_libraries(Cliq
	PRIVATE
		# Add libraries
	${CMAKE_CURRENT_SOURCE_DIR}/libs/libwebsockets-4.3.2/include/libwebsockets.h
)
]]