#pragma once

#include <iostream>
#include <string>
#include <exception>

// Simple test framework - minimal implementation
class TestRunner {
public:
    static int tests_run;
    static int tests_passed;
    static int tests_failed;
    
    static void run_test(const std::string& test_name, void (*test_func)()) {
        tests_run++;
        std::cout << "Running " << test_name << "... ";
        
        try {
            test_func();
            tests_passed++;
            std::cout << "PASSED" << std::endl;
        } catch (const std::exception& e) {
            tests_failed++;
            std::cout << "FAILED: " << e.what() << std::endl;
        } catch (...) {
            tests_failed++;
            std::cout << "FAILED: Unknown exception" << std::endl;
        }
    }
    
    static int print_summary() {
        std::cout << "\n========== Test Summary ==========" << std::endl;
        std::cout << "Tests run: " << tests_run << std::endl;
        std::cout << "Tests passed: " << tests_passed << std::endl;
        std::cout << "Tests failed: " << tests_failed << std::endl;
        std::cout << "==================================" << std::endl;
        
        return tests_failed;
    }
};

#define EXPECT_EQ(expected, actual) \
    do { \
        if ((expected) != (actual)) { \
            throw std::runtime_error(std::string("Expected ") + std::to_string(expected) + \
                                   " but got " + std::to_string(actual)); \
        } \
    } while(0)

#define EXPECT_NE(not_expected, actual) \
    do { \
        if ((not_expected) == (actual)) { \
            throw std::runtime_error("Values should not be equal"); \
        } \
    } while(0)

#define EXPECT_TRUE(condition) \
    do { \
        if (!(condition)) { \
            throw std::runtime_error("Expected condition to be true"); \
        } \
    } while(0)

#define EXPECT_FALSE(condition) \
    do { \
        if (condition) { \
            throw std::runtime_error("Expected condition to be false"); \
        } \
    } while(0)

#define RUN_TEST(test_name) \
    TestRunner::run_test(#test_name, test_name)

template<typename T>
void EXPECT_NE_PTR(T* not_expected, T* actual) {
    if (not_expected == actual) {
        throw std::runtime_error("Pointers should not be equal");
    }
}

template<typename T>
void EXPECT_EQ_PTR(T* expected, T* actual) {
    if (expected != actual) {
        throw std::runtime_error("Pointers should be equal");
    }
}

// Helper for null pointer checks
template<typename T>
void EXPECT_NOT_NULL(T* ptr) {
    if (ptr == nullptr) {
        throw std::runtime_error("Pointer should not be null");
    }
}