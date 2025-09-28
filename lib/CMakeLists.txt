# IOC Library CMakeLists.txt
cmake_minimum_required(VERSION 3.20)

# Create the main IOC interface library
add_library(IOC INTERFACE)
add_library(IOC::IOC ALIAS IOC)

# Set C++ standard requirement
target_compile_features(IOC INTERFACE cxx_std_20)

# Set include directories
target_include_directories(IOC
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
        $<INSTALL_INTERFACE:include>
)

# Installation configuration
include(GNUInstallDirs)

# Install the target
install(TARGETS IOC
    EXPORT IOCTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install header files
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../include/ioc
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

# Install the export set
install(EXPORT IOCTargets
    FILE IOCTargets.cmake
    NAMESPACE IOC::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/IOC
)

# Create config files
include(CMakePackageConfigHelpers)

# Generate the config file
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/IOCConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/IOCConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/IOC
)

# Generate the version file
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/IOCConfigVersion.cmake
    VERSION 1.0.0
    COMPATIBILITY SameMajorVersion
)

# Install the config files  
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/IOCConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/IOCConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/IOC
)