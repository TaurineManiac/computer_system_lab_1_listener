cmake_minimum_required(VERSION 4.0)
project(listener)

set(CMAKE_CXX_STANDARD 20)

add_executable(listener main.cpp)

target_link_libraries(listener ws2_32)