#pragma once

#include "lifetime.h"
#include "service_factory.h"
#include "util/evaluate_type.h"
#include <memory>

namespace IOC {

template <typename TDescriptor, typename TLifetime = typename Binding<TDescriptor>::TLifetime>
struct LifetimeManager;

template <typename TDescriptor>
struct LifetimeManager<TDescriptor, Transient> {
  using TService = typename Binding<TDescriptor>::TService;
  using TFactory = ResolveFactory_t<TService, TDescriptor>;

  constexpr auto GetOrCreate(auto& container) {
    return TFactory::Create(container);
  }
};

template <typename TDescriptor>
struct LifetimeManager<TDescriptor, Scoped> {
  using TService  = typename Binding<TDescriptor>::TService;
  using TRealType = typename Util::ReplaceDescriptors<TService>::TResult;
  using TFactory  = ResolveFactory_t<TService, TDescriptor>;

  // Storage is std::unique_ptr (not std::optional) so non-movable / non-copyable
  // services are supported: heap-construct in place via make_unique.
  auto GetOrCreate(auto& container) {
    if (!Instance_) [[unlikely]] {
      Instance_ = TFactory::CreateUnique(container);
    }
    return Instance_.get();
  }

private:
  std::unique_ptr<TRealType> Instance_;
};

template <typename TDescriptor>
struct LifetimeManager<TDescriptor, Singleton> {
  using TService = typename Binding<TDescriptor>::TService;
  using TFactory = ResolveFactory_t<TService, TDescriptor>;

  auto GetOrCreate(auto& container) {
    // NOTE: Singleton intentionally lives in a function-local static so
    // its lifetime spans the whole process and is shared across
    // containers. Per-container lifetime should use Scoped.
    static auto instance = TFactory::Create(container);
    return std::addressof(instance);
  }
};

// External: the container does not own the instance. The user must call
// Container::Bind<TDescriptor>(ptr) before the first Resolve. GetOrCreate
// returns the registered pointer (or nullptr if Bind was never called).
template <typename TDescriptor>
struct LifetimeManager<TDescriptor, External> {
  using TService  = typename Binding<TDescriptor>::TService;
  using TRealType = typename Util::ReplaceDescriptors<TService>::TResult;

  constexpr TRealType* GetOrCreate(auto&) const noexcept {
    return Instance_;
  }

  constexpr void SetInstance(TRealType* ptr) noexcept {
    Instance_ = ptr;
  }

private:
  TRealType* Instance_ = nullptr;
};

} // namespace IOC
