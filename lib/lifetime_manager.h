#pragma once

#include "lifetime.h"
#include "service_factory.h"
#include <memory>
#include <optional>

namespace IOC {

    template <typename TService, typename TLifetime = void>
    struct LifetimeManager;

    template <typename TService>
    struct LifetimeManager<TService, Transient> {
        template<typename TContainer>
        constexpr auto GetOrCreate(const TContainer& container) -> decltype(auto) {
            return ServiceFactory<TService>::Create(container);
        }
    };

    template <typename TService>
    struct LifetimeManager<TService, Scoped> {
        template<typename TContainer>
        constexpr auto GetOrCreate(const TContainer& container) -> decltype(auto) {
            if (!Instance_.has_value()) [[unlikely]] {
                Instance_ = ServiceFactory<TService>::Create(container);
            }

            return std::addressof(Instance_.value());
        }

    private:
        std::optional<TService> Instance_;
    };

    template <typename TService>
    struct LifetimeManager<TService, Singleton> {
        template<typename TContainer>
        auto GetOrCreate(const TContainer& container) -> decltype(auto) {
            static TService instance = ServiceFactory<TService>::Create(container);
            return std::addressof(instance);
        }
    };

}
