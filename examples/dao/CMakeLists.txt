cmake_minimum_required(VERSION 3.24.1)
project(IOC)

set(CMAKE_CXX_STANDARD 20)

add_library(IOC_example_dao STATIC repository.cpp repository.h unit_of_work.h abstract/repository_descriptor.h abstract/unit_of_work_descriptor.h)