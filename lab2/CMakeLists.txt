cmake_minimum_required(VERSION 3.3)
project(Router)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES routing.c node0.c node1.c node2.c node3.c)
add_executable(Router ${SOURCE_FILES})