cmake_minimum_required(VERSION 3.24)
project(IOC)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib)
add_subdirectory(examples/dao)
add_subdirectory(examples/services)

add_executable(IOC main.cpp examples/domain/employee.cpp examples/domain/employee.h examples/domain/department.cpp examples/domain/department.h examples/utils/enumerable.cpp examples/utils/enumerable.h examples/utils/input_range.cpp examples/utils/input_range.h examples/bootstrap.h examples/exposure.h examples/infra/logger.cpp examples/infra/logger.h examples/infra/printer.cpp examples/infra/printer.h examples/service.h examples/infra/abstract/logger_descriptor.h examples/infra/abstract/printer_descriptor.h)
