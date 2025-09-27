#include "simple_test.h"
#include <ioc/ioc.hpp>

// Test services for service factory tests
struct ServiceFactoryTestDescriptor {};

class ServiceWithoutDependencies {
public:
    ServiceWithoutDependencies() : value_(42) {}
    int getValue() const { return value_; }

private:
    int value_;
};

// Bindings for service factory tests
template<>
struct IOC::Binding<ServiceFactoryTestDescriptor> {
    using TLifetime = IOC::Transient;
    using TService = ServiceWithoutDependencies;
};

void test_CanCreateSimpleService() {
    IOC::Container<ServiceFactoryTestDescriptor> container;
    auto factory = IOC::ServiceFactory<ServiceWithoutDependencies>();
    
    auto service = factory.Create(container);
    
    EXPECT_EQ(42, service.getValue());
}

// Test default constructible service behavior
class DefaultConstructibleService {
public:
    DefaultConstructibleService() = default;
    static bool WasDefaultConstructed() { return true; }
};

void test_DefaultConstructibleServiceUsesDefaultConstructor() {
    IOC::Container<> container;
    auto factory = IOC::ServiceFactory<DefaultConstructibleService>();
    
    auto service = factory.Create(container);
    
    EXPECT_TRUE(service.WasDefaultConstructed());
}

// Test custom service factory
struct CustomFactoryDescriptor {};

class CustomService {
public:
    CustomService(int value) : value_(value) {}
    int getValue() const { return value_; }

private:
    int value_;
};

template<>
struct IOC::Binding<CustomFactoryDescriptor> {
    using TLifetime = IOC::Transient;
    using TService = CustomService;
};

template<>
struct IOC::ServiceFactory<CustomService> {
    static constexpr CustomService Create(auto&) {
        return CustomService(999); // Custom creation logic
    }
};

void test_CustomServiceFactoryIsUsed() {
    IOC::Container<CustomFactoryDescriptor> container;
    
    auto service = container.Resolve<CustomFactoryDescriptor>();
    
    EXPECT_EQ(999, service.getValue());
}

void run_service_factory_tests() {
    std::cout << "=== Service Factory Tests ===" << std::endl;
    
    RUN_TEST(test_CanCreateSimpleService);
    RUN_TEST(test_DefaultConstructibleServiceUsesDefaultConstructor);
    RUN_TEST(test_CustomServiceFactoryIsUsed);
    
    std::cout << std::endl;
}