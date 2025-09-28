#include "simple_test.h"
#include <ioc/ioc.hpp>

// Test services for lifetime manager tests
struct TransientLifetimeDescriptor {};
struct ScopedLifetimeDescriptor {};
struct SingletonLifetimeDescriptor {};

class TransientLifetimeService {
public:
    static int InstanceCount;
    static int NextId;
    TransientLifetimeService() { ++InstanceCount; }
    ~TransientLifetimeService() { --InstanceCount; }
    int getId() const { return id_; }
    
private:
    int id_ = ++NextId;
};

int TransientLifetimeService::InstanceCount = 0;
int TransientLifetimeService::NextId = 0;

class ScopedLifetimeService {
public:
    static int InstanceCount;
    static int NextId;
    ScopedLifetimeService() { ++InstanceCount; }
    ~ScopedLifetimeService() { --InstanceCount; }
    int getId() const { return id_; }
    
private:
    int id_ = ++NextId;
};

int ScopedLifetimeService::InstanceCount = 0;
int ScopedLifetimeService::NextId = 0;

class SingletonLifetimeService {
public:
    static int InstanceCount;
    static int NextId;
    SingletonLifetimeService() { ++InstanceCount; }
    ~SingletonLifetimeService() { --InstanceCount; }
    int getId() const { return id_; }
    
private:
    int id_ = ++NextId;
};

int SingletonLifetimeService::InstanceCount = 0;
int SingletonLifetimeService::NextId = 0;

// Binding configurations
template<>
struct IOC::Binding<TransientLifetimeDescriptor> {
    using TLifetime = IOC::Transient;
    using TService = TransientLifetimeService;
};

template<>
struct IOC::Binding<ScopedLifetimeDescriptor> {
    using TLifetime = IOC::Scoped;
    using TService = ScopedLifetimeService;
};

template<>
struct IOC::Binding<SingletonLifetimeDescriptor> {
    using TLifetime = IOC::Singleton;
    using TService = SingletonLifetimeService;
};

void setUpLifetimeManager() {
    TransientLifetimeService::InstanceCount = 0;
    TransientLifetimeService::NextId = 0;
    ScopedLifetimeService::InstanceCount = 0;
    ScopedLifetimeService::NextId = 0;
    SingletonLifetimeService::InstanceCount = 0;
    SingletonLifetimeService::NextId = 0;
}

void test_TransientLifetimeCreatesNewInstanceEachTime() {
    setUpLifetimeManager();
    IOC::Container<TransientLifetimeDescriptor> container;
    
    auto service1 = container.Resolve<TransientLifetimeDescriptor>();
    auto service2 = container.Resolve<TransientLifetimeDescriptor>();
    
    // Transient services return by value, so they have different IDs
    EXPECT_NE(service1.getId(), service2.getId());
    EXPECT_EQ(2, TransientLifetimeService::InstanceCount);
}

void test_ScopedLifetimeReturnsSameInstanceInScope() {
    setUpLifetimeManager();
    IOC::Container<ScopedLifetimeDescriptor> container;
    
    auto service1 = container.Resolve<ScopedLifetimeDescriptor>();
    auto service2 = container.Resolve<ScopedLifetimeDescriptor>();
    
    EXPECT_EQ_PTR(service1, service2);
    EXPECT_EQ(service1->getId(), service2->getId());
    EXPECT_EQ(1, ScopedLifetimeService::InstanceCount);
}

void test_ScopedLifetimeCreatesDifferentInstanceInDifferentScope() {
    setUpLifetimeManager();
    int firstId, secondId;
    
    {
        IOC::Container<ScopedLifetimeDescriptor> container1;
        auto service1 = container1.Resolve<ScopedLifetimeDescriptor>();
        firstId = service1->getId();
        EXPECT_EQ(1, ScopedLifetimeService::InstanceCount);
    }
    
    {
        IOC::Container<ScopedLifetimeDescriptor> container2;
        auto service2 = container2.Resolve<ScopedLifetimeDescriptor>();
        secondId = service2->getId();
        EXPECT_EQ(2, ScopedLifetimeService::InstanceCount);
    }
    
    EXPECT_NE(firstId, secondId);
}

void test_SingletonLifetimeReturnsSameInstanceAcrossContainers() {
    setUpLifetimeManager();
    int firstId, secondId;
    
    {
        IOC::Container<SingletonLifetimeDescriptor> container1;
        auto service1 = container1.Resolve<SingletonLifetimeDescriptor>();
        firstId = service1->getId();
        EXPECT_EQ(1, SingletonLifetimeService::InstanceCount);
    }
    
    {
        IOC::Container<SingletonLifetimeDescriptor> container2;
        auto service2 = container2.Resolve<SingletonLifetimeDescriptor>();
        secondId = service2->getId();
        EXPECT_EQ(1, SingletonLifetimeService::InstanceCount);
    }
    
    EXPECT_EQ(firstId, secondId);
}

void run_lifetime_manager_tests() {
    std::cout << "=== Lifetime Manager Tests ===" << std::endl;
    
    RUN_TEST(test_TransientLifetimeCreatesNewInstanceEachTime);
    RUN_TEST(test_ScopedLifetimeReturnsSameInstanceInScope);
    RUN_TEST(test_ScopedLifetimeCreatesDifferentInstanceInDifferentScope);
    RUN_TEST(test_SingletonLifetimeReturnsSameInstanceAcrossContainers);
    
    std::cout << std::endl;
}