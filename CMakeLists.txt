cmake_minimum_required(VERSION 3.24)
project(Lispy VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(includes)
add_subdirectory(src)

add_executable(lispy main.cpp src)

target_link_libraries(lispy PRIVATE LispyLib)