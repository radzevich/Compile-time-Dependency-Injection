cmake_minimum_required(VERSION 3.24)
project(IOC)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(lib)

add_executable(IOC main.cpp src/service.cpp src/service.h)
