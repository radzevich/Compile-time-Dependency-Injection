#pragma once

#include "lifetime.h"
#include "service_factory.h"
#include "util/evaluate_type.h"
#include <memory>
#include <optional>

namespace IOC {

    template <typename TDescriptor, typename TLifetime = typename Binding<TDescriptor>::TLifetime>
    struct LifetimeManager;

    template <typename TDescriptor>
    struct LifetimeManager<TDescriptor, Transient> {
        using TService = Binding<TDescriptor>::TService;

        constexpr auto GetOrCreate(auto& container) {
            return ServiceFactory<TService>::Create(container);
        }
    };

    template <typename TDescriptor>
    struct LifetimeManager<TDescriptor, Scoped> {
        using TService = Binding<TDescriptor>::TService;
        using TRealType = Util::ReplaceDescriptors<TService>::TResult;

        constexpr auto GetOrCreate(auto& container) {
            if (!Instance_.has_value()) [[unlikely]] {
                Instance_ = ServiceFactory<TService>::Create(container);
            }

            return std::addressof(Instance_.value());
        }

    private:
        std::optional<TRealType> Instance_;
    };

    template <typename TDescriptor>
    struct LifetimeManager<TDescriptor, Singleton> {
        using TService = Binding<TDescriptor>::TService;

        auto GetOrCreate(auto& container) {
            // TODO: Definition of a static variable in a constexpr function is a C++23 extension
            static auto instance = ServiceFactory<TService>::Create(container);
            return std::addressof(instance);
        }
    };

}
