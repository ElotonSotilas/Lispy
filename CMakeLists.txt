cmake_minimum_required(VERSION 3.24)
project(Lispy)

set(CMAKE_CXX_STANDARD 23)

add_executable(lispy main.cpp includes/lispy.h)

include_directories(includes)
