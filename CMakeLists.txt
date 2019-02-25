cmake_minimum_required(VERSION 3.13)
project(DePoker)

set(CMAKE_CXX_STANDARD 14)

add_executable(DePoker test.cpp src/Menu.cpp src/windows.cpp src/network.h)
include_directories(DePoker src)
target_link_libraries(DePoker sfml-graphics sfml-window sfml-network sfml-system pthread)