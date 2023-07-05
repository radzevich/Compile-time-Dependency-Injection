cmake_minimum_required(VERSION 3.24.1)
project(IOC)

set(CMAKE_CXX_STANDARD 17)

add_library(IOC_lib STATIC service.cpp)