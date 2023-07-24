#pragma once

#include <optional>

namespace IOC {

    struct Singleton {};

    struct Scoped {};

    struct Transient {};

    template <typename TDescriptor>
    struct Binding;

    template <typename TService>
    struct ServiceFactory;

    template <typename TService>
    struct ServiceFactory {
        static constexpr TService Create(const auto&) {
            return TService();
        }
    };

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

    template <typename TDescriptor, typename ...TDescriptors>
    class ServiceCollection;

    template <typename TDescriptor>
    class ServiceCollection<TDescriptor> {
        using TService = typename Binding<TDescriptor>::TService;
        using TLifetime = typename Binding<TDescriptor>::TLifetime;
        using TLifetimeManager = LifetimeManager<TService, TLifetime>;

    private:
        mutable TLifetimeManager LifetimeManager_;

    public:
        template <typename TThisContainer>
        auto ResolveService(const TThisContainer& container) const -> decltype(auto) {
            return LifetimeManager_.template GetOrCreate(container);
        }
    };

    template <typename TDescriptor, typename ...TDescriptors>
    class ServiceCollection : protected ServiceCollection<TDescriptor>
                            , public ServiceCollection<TDescriptors...> {
    public:
        template <typename TRequestedDescriptor>
        auto Resolve() const -> decltype(auto) {
            return static_cast<const ServiceCollection<TRequestedDescriptor>&>(*this).ResolveService(*this);
        }
    };

    template <typename ...TInnerDescriptors, typename ...TDescriptors>
    class ServiceCollection<ServiceCollection<TInnerDescriptors...>, TDescriptors...>
        : public ServiceCollection<TInnerDescriptors...>
        , public ServiceCollection<TDescriptors...> {
    public:
        template <typename TRequestedDescriptor>
        auto Resolve() const -> decltype(auto) {
            return static_cast<const ServiceCollection<TRequestedDescriptor>&>(*this).ResolveService(*this);
        }
    };

    template <typename ...TInnerDescriptors>
    class ServiceCollection<ServiceCollection<TInnerDescriptors...>> : public ServiceCollection<TInnerDescriptors...> {
    public:
        template <typename TRequestedDescriptor>
        auto Resolve() const -> decltype(auto) {
            return static_cast<const ServiceCollection<TRequestedDescriptor>&>(*this).ResolveService(*this);
        }
    };

}