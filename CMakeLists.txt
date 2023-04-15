cmake_minimum_required(VERSION 3.24)
project(Lispy)

set(CMAKE_CXX_STANDARD 23)

include_directories(includes)
include_directories(src)

add_executable(lispy main.cpp)
target_link_libraries(lispy LispyLib)