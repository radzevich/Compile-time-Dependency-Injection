cmake_minimum_required(VERSION 3.24.1)
project(IOC)

set(CMAKE_CXX_STANDARD 20)

add_library(IOC_lib STATIC ioc.cpp ioc_2.cpp ioc_2.h util.cpp util.h)