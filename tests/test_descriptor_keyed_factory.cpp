#include "simple_test.h"
#include <ioc/ioc.hpp>
#include <string>

// Service shared by multiple descriptors but with per-descriptor construction
// recipes — analogous to having several SpScBytesQueue instances with
// different names/capacities.
class NamedQueue {
public:
  NamedQueue(std::string name, int capacity) : name_(std::move(name)), capacity_(capacity) {}
  std::string const& name() const {
    return name_;
  }
  int capacity() const {
    return capacity_;
  }

private:
  std::string name_;
  int capacity_;
};

struct FsQueueD {};
struct MdQueueD {};
struct LinesQueueD {};

template <>
struct IOC::Binding<FsQueueD> {
  using TLifetime = IOC::Scoped;
  using TService  = NamedQueue;
};
template <>
struct IOC::Binding<MdQueueD> {
  using TLifetime = IOC::Scoped;
  using TService  = NamedQueue;
};
template <>
struct IOC::Binding<LinesQueueD> {
  using TLifetime = IOC::Scoped;
  using TService  = NamedQueue;
};

// Descriptor-keyed factories — same TService, different recipes.
template <>
struct IOC::ServiceFactory<NamedQueue, FsQueueD> {
  static auto CreateUnique(auto&) {
    return std::make_unique<NamedQueue>("dmd_fs", 1024);
  }
};
template <>
struct IOC::ServiceFactory<NamedQueue, MdQueueD> {
  static auto CreateUnique(auto&) {
    return std::make_unique<NamedQueue>("dmd_md", 2048);
  }
};
template <>
struct IOC::ServiceFactory<NamedQueue, LinesQueueD> {
  static auto CreateUnique(auto&) {
    return std::make_unique<NamedQueue>("dmd_lines", 4096);
  }
};

void test_DescriptorKeyedFactoryReturnsCorrectInstance() {
  IOC::Container<FsQueueD, MdQueueD, LinesQueueD> container;

  auto* fs    = container.Resolve<FsQueueD>();
  auto* md    = container.Resolve<MdQueueD>();
  auto* lines = container.Resolve<LinesQueueD>();

  EXPECT_TRUE(fs != md);
  EXPECT_TRUE(md != lines);
  EXPECT_TRUE(fs->name() == "dmd_fs");
  EXPECT_TRUE(md->name() == "dmd_md");
  EXPECT_TRUE(lines->name() == "dmd_lines");
  EXPECT_EQ(1024, fs->capacity());
  EXPECT_EQ(2048, md->capacity());
  EXPECT_EQ(4096, lines->capacity());
}

void test_DescriptorKeyedFactoryIsScopedSingletonPerContainer() {
  IOC::Container<FsQueueD> container;
  auto* a = container.Resolve<FsQueueD>();
  auto* b = container.Resolve<FsQueueD>();
  EXPECT_TRUE(a == b);
}

// Service that uses a descriptor-keyed dependency: an Edge-like wrapper around
// a NamedQueue resolved from the container.
struct FsEdgeD {};

class QueueRef {
public:
  explicit QueueRef(NamedQueue* q) : q_(q) {}
  NamedQueue* queue() const {
    return q_;
  }

private:
  NamedQueue* q_;
};

template <>
struct IOC::Binding<FsEdgeD> {
  using TLifetime = IOC::Scoped;
  using TService  = QueueRef;
};

template <>
struct IOC::ServiceFactory<QueueRef, FsEdgeD> {
  static auto CreateUnique(auto& c) {
    // Resolve a sibling descriptor from the container — the whole point of
    // Option B: factories can compose descriptors freely.
    return std::make_unique<QueueRef>(c.template Resolve<FsQueueD>());
  }
};

void test_DescriptorKeyedFactoryCanResolveOtherDescriptors() {
  IOC::Container<FsQueueD, FsEdgeD> container;
  auto* edge = container.Resolve<FsEdgeD>();
  EXPECT_TRUE(edge->queue() != nullptr);
  EXPECT_TRUE(edge->queue()->name() == "dmd_fs");
  // Sibling resolution returns the same Scoped instance.
  EXPECT_TRUE(edge->queue() == container.Resolve<FsQueueD>());
}

void run_descriptor_keyed_factory_tests() {
  std::cout << "=== Descriptor-keyed Factory Tests ===" << std::endl;
  RUN_TEST(test_DescriptorKeyedFactoryReturnsCorrectInstance);
  RUN_TEST(test_DescriptorKeyedFactoryIsScopedSingletonPerContainer);
  RUN_TEST(test_DescriptorKeyedFactoryCanResolveOtherDescriptors);
  std::cout << std::endl;
}
