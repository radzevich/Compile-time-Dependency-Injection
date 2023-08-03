cmake_minimum_required(VERSION 3.24.1)
project(IOC)

set(CMAKE_CXX_STANDARD 20)

add_library(IOC_lib STATIC service_collection.cpp service_collection.h lifetime_manager.cpp lifetime_manager.h lifetime.cpp lifetime.h binding.cpp binding.h service_factory.cpp service_factory.h util/evaluate_type.cpp util/evaluate_type.h)