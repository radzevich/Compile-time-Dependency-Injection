cmake_minimum_required(VERSION 3.24)
project(IOC)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib)
add_subdirectory(examples/dao)

add_executable(IOC main.cpp examples/domain/employee.cpp examples/domain/employee.h examples/domain/department.cpp examples/domain/department.h examples/services/service.cpp examples/services/service.h examples/services/exposure.h examples/dao/bootstrap.h examples/utils/enumerable.cpp examples/utils/enumerable.h)
