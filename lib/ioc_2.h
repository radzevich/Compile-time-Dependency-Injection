#pragma once

#include "util.h"
#include "../src/service.h"

struct ADescriptor {};

struct BDescriptor {};

struct CDescriptor {};

namespace IOC2 {

    template <typename TDescriptor>
    struct Binding;

    template <>
    struct IOC2::Binding<ADescriptor> {
        using TService = A;
    };

    template <>
    struct IOC2::Binding<BDescriptor> {
        using TService = B;
    };

    template <>
    struct IOC2::Binding<CDescriptor> {
        using TService = C;

        template <typename TServiceContainer>
        auto operator()(const TServiceContainer& container) {
            auto a = container.template Resolve<ADescriptor>();
            auto b = container.template Resolve<BDescriptor>();

            return C(&a, &b);
        }
    };

    template <typename TDescriptor, typename ...TDescriptors>
    class ServiceCollection;

    template <typename TDescriptor>
    class ServiceCollection<TDescriptor> {
        using TService = typename Binding<TDescriptor>::TService;
    public:
        template <typename TThisContainer>
        auto Resolve(const TThisContainer& container) const -> decltype(auto) {
            if constexpr (std::is_invocable_v<Binding<TDescriptor>, TThisContainer>) {
                return Binding<TDescriptor>()(container);
            } else {
                return TService();
            }
        }
    };

    template <typename TDescriptor, typename ...TDescriptors>
    class ServiceCollection : protected ServiceCollection<TDescriptor>
                            , public ServiceCollection<TDescriptors...> {
    public:
        template <typename TRequestedDescriptor>
        auto Resolve() const -> decltype(auto) {
            const auto& desiredServiceCollection = static_cast<const ServiceCollection<TRequestedDescriptor>>(*this);
            return desiredServiceCollection.Resolve(*this);
        }
    };


}