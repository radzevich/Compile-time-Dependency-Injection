cmake_minimum_required(VERSION 3.24)
project(IOC VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include standard modules
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Add the main IOC library
add_subdirectory(lib)

# Build examples and main executable only when building standalone (not via FetchContent)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    option(IOC_BUILD_EXAMPLES "Build IOC examples" ON)
    
    if(IOC_BUILD_EXAMPLES)
        add_subdirectory(examples/dao)
        add_subdirectory(examples/services)
        add_subdirectory(examples/basic)
        
        add_executable(IOC_example examples/basic/main.cpp 
            examples/domain/employee.cpp examples/domain/employee.h 
            examples/domain/department.cpp examples/domain/department.h 
            examples/utils/enumerable.cpp examples/utils/enumerable.h 
            examples/utils/input_range.cpp examples/utils/input_range.h 
            examples/bootstrap.h examples/exposure.h 
            examples/infra/logger.cpp examples/infra/logger.h 
            examples/infra/printer.cpp examples/infra/printer.h 
            examples/service.h 
            examples/infra/abstract/logger_descriptor.h 
            examples/infra/abstract/printer_descriptor.h)
        
        target_link_libraries(IOC_example PRIVATE IOC::IOC)
    endif()
endif()
