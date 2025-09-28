#include "simple_test.h"
#include <ioc/ioc.hpp>
#include <type_traits>

// Test services for binding tests
struct BindingTestDescriptor {};

class BindingTestService {
public:
    int getValue() const { return 100; }
};

class AlternativeBindingTestService {
public:
    int getValue() const { return 200; }
};

// Default binding (should use descriptor as service)
template<>
struct IOC::Binding<BindingTestDescriptor> {
    using TLifetime = IOC::Transient;
    using TService = BindingTestService;
};

void test_DefaultBindingUsesTransientLifetime() {
    using binding = IOC::Binding<BindingTestDescriptor>;
    EXPECT_TRUE((std::is_same_v<binding::TLifetime, IOC::Transient>));
}

void test_BindingMapsToCorrectService() {
    using binding = IOC::Binding<BindingTestDescriptor>;
    EXPECT_TRUE((std::is_same_v<binding::TService, BindingTestService>));
}

void test_BindingWorksWithContainer() {
    IOC::Container<BindingTestDescriptor> container;
    
    auto service = container.Resolve<BindingTestDescriptor>();
    
    EXPECT_EQ(100, service.getValue());
}

// Test service with specific lifetime configurations
struct SingletonTestDescriptor {};
struct ScopedTestDescriptor {};

class SingletonTestService {
public:
    static int InstanceCount;
    SingletonTestService() { ++InstanceCount; }
    ~SingletonTestService() { --InstanceCount; }
    int getValue() const { return 300; }
};

int SingletonTestService::InstanceCount = 0;

class ScopedTestService {
public:
    static int InstanceCount;
    ScopedTestService() { ++InstanceCount; }
    ~ScopedTestService() { --InstanceCount; }
    int getValue() const { return 400; }
};

int ScopedTestService::InstanceCount = 0;

template<>
struct IOC::Binding<SingletonTestDescriptor> {
    using TLifetime = IOC::Singleton;
    using TService = SingletonTestService;
};

template<>
struct IOC::Binding<ScopedTestDescriptor> {
    using TLifetime = IOC::Scoped;
    using TService = ScopedTestService;
};

void setUpLifetime() {
    SingletonTestService::InstanceCount = 0;
    ScopedTestService::InstanceCount = 0;
}

void test_SingletonBindingCreatesOneInstance() {
    setUpLifetime();
    IOC::Container<SingletonTestDescriptor> container;
    
    auto service1 = container.Resolve<SingletonTestDescriptor>();
    auto service2 = container.Resolve<SingletonTestDescriptor>();
    
    EXPECT_EQ_PTR(service1, service2);
    EXPECT_EQ(1, SingletonTestService::InstanceCount);
}

void test_ScopedBindingCreatesOneInstancePerScope() {
    setUpLifetime();
    IOC::Container<ScopedTestDescriptor> container;
    
    auto service1 = container.Resolve<ScopedTestDescriptor>();
    auto service2 = container.Resolve<ScopedTestDescriptor>();
    
    EXPECT_EQ_PTR(service1, service2);
    EXPECT_EQ(1, ScopedTestService::InstanceCount);
}

void run_binding_tests() {
    std::cout << "=== Binding Tests ===" << std::endl;
    
    RUN_TEST(test_DefaultBindingUsesTransientLifetime);
    RUN_TEST(test_BindingMapsToCorrectService);
    RUN_TEST(test_BindingWorksWithContainer);
    RUN_TEST(test_SingletonBindingCreatesOneInstance);
    RUN_TEST(test_ScopedBindingCreatesOneInstancePerScope);
    
    std::cout << std::endl;
}