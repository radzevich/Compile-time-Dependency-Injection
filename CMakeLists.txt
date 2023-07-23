cmake_minimum_required(VERSION 3.24)
project(IOC)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib)

add_executable(IOC main.cpp src/service.cpp src/service.h examples/domain/employee.cpp examples/domain/employee.h examples/domain/company.cpp examples/domain/company.h examples/services/service.cpp examples/services/service.h examples/services/dependency_exposure.h examples/dao/unit_of_work.h examples/dao/repository.cpp examples/dao/repository.h examples/dao/dependency_exposure.h examples/dao/bootstrap.h)
