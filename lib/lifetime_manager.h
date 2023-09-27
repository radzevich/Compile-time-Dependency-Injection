#pragma once

#include "lifetime.h"
#include "service_factory.h"
#include "util/evaluate_type.h"
#include <memory>
#include <optional>

namespace IOC {

    template <typename TDescriptor, typename TLifetime>
    struct LifetimeManager;

    template <typename TDescriptor>
    struct LifetimeManager<TDescriptor, Transient> {
        using TService = Binding<TDescriptor>::TService;

        constexpr auto GetOrCreate(const auto& container) -> decltype(auto) {
            return ServiceFactory<TService>::Create(container);
        }
    };

    template <typename TDescriptor>
    struct LifetimeManager<TDescriptor, Scoped> {
        using TService = Binding<TDescriptor>::TService;

        constexpr auto GetOrCreate(const auto& container) -> decltype(auto) {
            if (!Instance_.has_value()) [[unlikely]] {
                Instance_ = ServiceFactory<TService>::Create(container);
            }

            return std::addressof(Instance_.value());
        }

    private:
        std::optional<typename Util::EvaluateType<TService>::Type> Instance_;
    };

    template <typename TDescriptor>
    struct LifetimeManager<TDescriptor, Singleton> {
        using TService = Binding<TDescriptor>::TService;

        auto GetOrCreate(const auto& container) -> decltype(auto) {
            static auto instance = ServiceFactory<TService>::Create(container);
            return std::addressof(instance);
        }
    };

}
