#include <ioc/ioc.hpp>
#include <iostream>

// Simple test to verify the IOC library headers work
int main() {
    // This is just a basic compilation test
    // Create a simple container to verify the API works
    IOC::Container<> empty_container;
    
    std::cout << "IOC library headers compiled successfully!" << std::endl;
    return 0;
}