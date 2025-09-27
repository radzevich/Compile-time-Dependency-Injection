# Basic Example

This directory contains a simple example demonstrating the usage of the IOC library.

## Files

- `main.cpp` - Basic example code showing how to use the IOC dependency injection system
- `CMakeLists.txt` - Build configuration (placeholder, the example is built from the root CMakeLists.txt)

## Running the Example

To build and run this example:

1. From the repository root, build with examples enabled:
   ```bash
   mkdir build && cd build
   cmake .. -DIOC_BUILD_EXAMPLES=ON
   make
   ```

2. Run the example:
   ```bash
   ./IOC_example
   ```

The example demonstrates the dependency injection container managing employee and department services with logging and database access.