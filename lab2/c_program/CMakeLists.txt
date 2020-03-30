cmake_minimum_required(VERSION 3.10)
project(c_program C)

set(CMAKE_C_STANDARD 99)

add_executable(c_program main.c)

find_package(GSL REQUIRED)
target_link_libraries(c_program GSL::gsl GSL::gslcblas)