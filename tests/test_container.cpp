#include "simple_test.h"
#include <ioc/ioc.hpp>
#include <memory>
#include <type_traits>

// Test descriptors for unit tests
struct TestServiceDescriptor {};

// Simple test service without dependencies
class TestService {
public:
    static int InstanceCount;
    TestService() { ++InstanceCount; }
    ~TestService() { --InstanceCount; }
    
    int getValue() const { return 42; }
};

int TestService::InstanceCount = 0;

// Bindings for test services
template<>
struct IOC::Binding<TestServiceDescriptor> {
    using TLifetime = IOC::Transient;
    using TService = TestService;
};

void setUp() {
    TestService::InstanceCount = 0;
}

void test_CanResolveSimpleService() {
    setUp();
    IOC::Container<TestServiceDescriptor> container;
    
    auto service = container.Resolve<TestServiceDescriptor>();
    
    EXPECT_EQ(42, service.getValue());
    EXPECT_EQ(1, TestService::InstanceCount);
}

void test_TransientServiceCreatesNewInstanceEachTime() {
    setUp();
    IOC::Container<TestServiceDescriptor> container;
    
    auto service1 = container.Resolve<TestServiceDescriptor>();
    auto service2 = container.Resolve<TestServiceDescriptor>();
    
    // Transient services return by value, so they will have different instance counts
    EXPECT_EQ(2, TestService::InstanceCount);
}

void test_EmptyContainerReturnsNulloptForUnregisteredService() {
    setUp();
    IOC::Container<> container;
    
    auto result = container.Resolve<TestServiceDescriptor>();
    
    // The result should be std::nullopt_t type
    EXPECT_TRUE((std::is_same_v<decltype(result), std::nullopt_t>));
}

void run_container_tests() {
    std::cout << "=== Container Tests ===" << std::endl;
    
    RUN_TEST(test_CanResolveSimpleService);
    RUN_TEST(test_TransientServiceCreatesNewInstanceEachTime);
    RUN_TEST(test_EmptyContainerReturnsNulloptForUnregisteredService);
    
    std::cout << std::endl;
}