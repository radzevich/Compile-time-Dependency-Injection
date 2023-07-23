#pragma once

#include <optional>

namespace IOC {

    struct Singleton {};

    struct Scoped {};

    struct Transient {};

    template <typename TDescriptor>
    struct Binding;

    template <typename TService>
    struct ServiceFactory {
        static constexpr TService Create(const auto&) {
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