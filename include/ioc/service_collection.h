#pragma once

#include "binding.h"
#include "lifetime_manager.h"

#include <concepts>
#include <optional>
#include <tuple>
#include <type_traits>

namespace IOC {

template <typename TDescriptor, typename... TDescriptors>
concept OneOf = (std::same_as<TDescriptor, TDescriptors> || ...);

template <typename... TDescriptors>
class Container;

template <typename... TDescriptors>
class Container {
private:
  std::tuple<LifetimeManager<TDescriptors>...> LifetimeManagers_;

public:
  template <OneOf<TDescriptors...> TDescriptor>
  constexpr auto Resolve() {
    return std::get<LifetimeManager<TDescriptor>>(LifetimeManagers_).GetOrCreate(*this);
  }

  template <typename TDescriptor>
  constexpr auto Resolve() {
    // Unregistered descriptor. Returning std::nullopt keeps SFINAE-style
    // probes working but resolving a missing descriptor in production code
    // is almost always a wiring bug.
    return std::nullopt;
  }

  // Register an externally-owned instance for an `External`-lifetime
  // descriptor. Must be called before the first Resolve<TDescriptor>().
  // Compile error if TDescriptor is not registered or is not External.
  template <OneOf<TDescriptors...> TDescriptor, typename T>
  constexpr void Bind(T* ptr) {
    using Manager = LifetimeManager<TDescriptor>;
    static_assert(std::is_same_v<typename Binding<TDescriptor>::TLifetime, External>,
                  "Container::Bind<D>(ptr) only applies to descriptors with "
                  "TLifetime = IOC::External.");
    std::get<Manager>(LifetimeManagers_).SetInstance(ptr);
  }
};

template <typename... TContainerArs, typename... Rest>
class Container<Container<TContainerArs...>, Rest...> :
    public Container<Rest..., TContainerArs...> {};

} // namespace IOC
