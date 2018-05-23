cmake_minimum_required(VERSION 3.9)

project(Examples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(ExampleSimple examples/example0_simple.cpp)
add_executable(ExampleFib examples/example1_fib.cpp)