cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(sample)

include(cmake/function.cmake)

add_subdirectory(lib)
add_subdirectory(main)
