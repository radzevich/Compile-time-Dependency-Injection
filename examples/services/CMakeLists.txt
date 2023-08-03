cmake_minimum_required(VERSION 3.24.1)
project(IOC)

set(CMAKE_CXX_STANDARD 20)

add_library(IOC_example_services STATIC report_management.cpp department_management.cpp employee_management.cpp abstract/employee_management_descriptor.h abstract/department_management_descriptor.h abstract/report_management_descriptor.h)