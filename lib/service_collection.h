#pragma once

#include "binding.h"
#include "lifetime_manager.h"

namespace IOC {

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
        template <typename TThisServiceCollection>
        auto ResolveService(const TThisServiceCollection& ServiceCollection) const -> decltype(auto) {
            return LifetimeManager_.template GetOrCreate(ServiceCollection);
        }
    };

    template <typename TDescriptor, typename ...TDescriptors>
    class ServiceCollection
        : protected ServiceCollection<TDescriptor>
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
