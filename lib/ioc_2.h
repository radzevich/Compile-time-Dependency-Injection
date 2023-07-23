#pragma once

#include "util.h"
#include "../src/service.h"

#include <optional>

struct ADescriptor {};

struct BDescriptor {};

struct CDescriptor {};

namespace IOC2 {

    struct Singleton {};

    struct Scoped {};

    struct Transient {};

    template <typename TDescriptor>
    struct Binding;

    // Forward declaration for the primary template
    template <typename TService, typename ...TArgs>
    struct ServiceFactory;

    // Specialization for the case where TService is a template type
    template <template<typename...> class TService, typename ...TArgs>
    struct ServiceFactory<TService<TArgs...>, TArgs...> {
        template <typename TContainer>
        static constexpr TService<TArgs...> Create(const TContainer&) {
            return TService<TArgs...>();
        }
    };

    // Specialization for the case where TService is a non-template type
    template <typename TService>
    struct ServiceFactory<TService, void> {
        template <typename TContainer>
        static constexpr TService Create(const TContainer&) {
            return TService();
        }
    };

    template <typename TService, typename TLifetime = void>
    struct LifetimeManager;

    template <typename TDescriptor>
    struct LifetimeManager<TDescriptor, Transient> {
        using TService = typename Binding<TDescriptor>::TService;

        template<typename TContainer>
        constexpr TService GetOrCreate(const TContainer& container) {
            return ServiceFactory<TService>::Create(container);
        }
    };

    template <typename TDescriptor>
    struct LifetimeManager<TDescriptor, Scoped> {
        using TService = typename Binding<TDescriptor>::TService;

        template<typename TContainer>
        constexpr TService* GetOrCreate(const TContainer& container) {
            if (!Instance_.has_value()) [[unlikely]] {
                Instance_ = ServiceFactory<TService>::Create(container);
            }

            return std::addressof(Instance_.value());
        }

    private:
        std::optional<TService> Instance_;
    };

    template <typename TDescriptor>
    struct LifetimeManager<TDescriptor, Singleton> {
        using TService = typename Binding<TDescriptor>::TService;

        template<typename TContainer>
        TService* GetOrCreate(const TContainer& container) {
            static TService instance = ServiceFactory<TService>::Create(container);
            return std::addressof(instance);
        }
    };


    template <>
    struct IOC2::Binding<ADescriptor> {
        using TService = A;
        using TLifetime = Singleton;
    };

    template <>
    struct IOC2::Binding<BDescriptor> {
        using TService = B;
        using TLifetime = Scoped;
    };

    template <>
    struct IOC2::Binding<CDescriptor> {
        using TService = C;
        using TLifetime = Transient;
    };

    template <>
    struct ServiceFactory<C> {
        template <typename TContainer>
        static C Create(const TContainer& container) {
            auto* a = container.template Resolve<ADescriptor>();
            auto* b = container.template Resolve<BDescriptor>();

            return C(a, b);
        }
    };

    template <typename TDescriptor, typename ...TDescriptors>
    class ServiceCollection;

    template <typename TDescriptor>
    class ServiceCollection<TDescriptor> {
        using TLifetime = typename Binding<TDescriptor>::TLifetime;
        using TLifetimeManager = LifetimeManager<TDescriptor, TLifetime>;

    private:
        mutable TLifetimeManager LifetimeManager_;

    public:
        template <typename TThisContainer>
        auto Resolve(const TThisContainer& container) const -> decltype(auto) {
            return LifetimeManager_.template GetOrCreate(container);
        }
    };

    template <typename TDescriptor, typename ...TDescriptors>
    class ServiceCollection : protected ServiceCollection<TDescriptor>
                            , public ServiceCollection<TDescriptors...> {
    public:
        template <typename TRequestedDescriptor>
        auto Resolve() const -> decltype(auto) {
            const auto& desiredServiceCollection = static_cast<const ServiceCollection<TRequestedDescriptor>&>(*this);
            return desiredServiceCollection.Resolve(*this);
        }
    };

    template <typename ...TInnerDescriptors, typename ...TDescriptors>
    class ServiceCollection<ServiceCollection<TInnerDescriptors...>, TDescriptors...>
            : public ServiceCollection<TInnerDescriptors...>
            , public ServiceCollection<TDescriptors...>
    {};

    template <typename ...TInnerDescriptors>
    class ServiceCollection<ServiceCollection<TInnerDescriptors...>>
            : public ServiceCollection<TInnerDescriptors...>
    {};

}