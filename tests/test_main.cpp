#include "simple_test.h"

// Define the static members here to avoid multiple definition
int TestRunner::tests_run = 0;
int TestRunner::tests_passed = 0;
int TestRunner::tests_failed = 0;

int main() {
    // All test functions will be called from respective test files
    extern void run_container_tests();
    extern void run_binding_tests();
    extern void run_lifetime_manager_tests();
    extern void run_service_factory_tests();
    
    std::cout << "Starting IOC Library Tests..." << std::endl << std::endl;
    
    run_container_tests();
    run_binding_tests();
    run_lifetime_manager_tests();
    run_service_factory_tests();
    
    return TestRunner::print_summary();
}