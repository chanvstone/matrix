cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED 11)

add_library(math math.cpp)
add_executable(test test.cpp)
target_link_libraries(test math)