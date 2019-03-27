cmake_minimum_required(VERSION 2.8)
project(SokobanSolver)

set(CMAKE_CSS_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -pedantic -Wextra -Wconversion -fexceptions")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src/deadlocks)
add_subdirectory(src)
add_subdirectory(deadlock_generator_src)

enable_testing()
add_subdirectory(test)
