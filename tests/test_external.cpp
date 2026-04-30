#include "simple_test.h"
#include <atomic>
#include <ioc/ioc.hpp>
#include <string>

// ── External lifetime ───────────────────────────────────────────────────────

struct ExternalConfig {
  std::string name;
  int capacity;
};

struct ExternalConfigDescriptor {};

template <>
struct IOC::Binding<ExternalConfigDescriptor> {
  using TLifetime = IOC::External;
  using TService  = ExternalConfig;
};

void test_ExternalLifetimeReturnsBoundInstance() {
  IOC::Container<ExternalConfigDescriptor> container;
  ExternalConfig cfg { "queue_a", 4096 };

  container.Bind<ExternalConfigDescriptor>(&cfg);
  auto* resolved = container.Resolve<ExternalConfigDescriptor>();

  EXPECT_TRUE(resolved == &cfg);
  EXPECT_EQ(4096, resolved->capacity);
}

void test_ExternalLifetimeIsNullBeforeBind() {
  IOC::Container<ExternalConfigDescriptor> container;
  auto* resolved = container.Resolve<ExternalConfigDescriptor>();
  EXPECT_TRUE(resolved == nullptr);
}

// ── Scoped + non-movable service ────────────────────────────────────────────

struct NonMovableDescriptor {};

class NonMovableService {
public:
  NonMovableService()                                    = default;
  NonMovableService(NonMovableService const&)            = delete;
  NonMovableService(NonMovableService&&)                 = delete;
  NonMovableService& operator=(NonMovableService const&) = delete;
  NonMovableService& operator=(NonMovableService&&)      = delete;

  int value() const {
    return counter_.load();
  }
  void bump() {
    counter_.fetch_add(1);
  }

private:
  std::atomic<int> counter_ { 7 };
};

template <>
struct IOC::Binding<NonMovableDescriptor> {
  using TLifetime = IOC::Scoped;
  using TService  = NonMovableService;
};

void test_ScopedSupportsNonMovableService() {
  IOC::Container<NonMovableDescriptor> container;
  auto* a = container.Resolve<NonMovableDescriptor>();
  a->bump();
  auto* b = container.Resolve<NonMovableDescriptor>();
  EXPECT_TRUE(a == b);
  EXPECT_EQ(8, b->value());
}

// ── External instance injected into a Scoped service ────────────────────────

struct UsesExternalDescriptor {};

template <typename TConfig = ExternalConfigDescriptor>
class UsesExternal {
public:
  explicit UsesExternal(TConfig* cfg) : cfg_(cfg) {}
  int capacity() const {
    return cfg_->capacity;
  }

private:
  TConfig* cfg_;
};

template <>
struct IOC::Binding<UsesExternalDescriptor> {
  using TLifetime = IOC::Scoped;
  using TService  = UsesExternal<>;
};

void test_ScopedServiceCanInjectExternalInstance() {
  IOC::Container<ExternalConfigDescriptor, UsesExternalDescriptor> container;
  ExternalConfig cfg { "queue_b", 1234 };
  container.Bind<ExternalConfigDescriptor>(&cfg);

  auto* svc = container.Resolve<UsesExternalDescriptor>();
  EXPECT_EQ(1234, svc->capacity());
}

void run_external_tests() {
  std::cout << "=== External Lifetime Tests ===" << std::endl;
  RUN_TEST(test_ExternalLifetimeReturnsBoundInstance);
  RUN_TEST(test_ExternalLifetimeIsNullBeforeBind);
  RUN_TEST(test_ScopedSupportsNonMovableService);
  RUN_TEST(test_ScopedServiceCanInjectExternalInstance);
  std::cout << std::endl;
}
