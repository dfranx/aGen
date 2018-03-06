cmake_minimum_required(VERSION 3.9)

project(aGen CXX)

add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/variant)
include_directories(${PROJECT_SOURCE_DIR}/inc)

add_executable(aGen 
	main.cpp
	src/ByteCode.cpp
	src/Generator.cpp
	src/BitConverter.cpp
	src/FunctionManager.cpp
)

target_link_libraries(aGen mpark_variant)