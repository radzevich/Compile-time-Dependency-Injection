#pragma once

#include "util/evaluate_type.h"
#include <memory>
#include <type_traits>
#include <utility>

namespace IOC {

    // Factory keyed by both the service type AND the requesting descriptor.
    // The descriptor parameter defaults to `void` so the common case ("one
    // factory per service type") keeps the simpler descriptor-less form.
    // Specialise with an explicit descriptor when several descriptors share
    // the same service type but need different construction recipes (e.g.
    // multiple `SpScBytesQueue` instances with different names/capacities).
    //
    // Lookup precedence (see ResolveFactory_t below):
    //   1. ServiceFactory<TService, TDescriptor>  (user-provided per-descriptor)
    //   2. ServiceFactory<TService, void>          (auto / default)
    template <typename TService, typename TDescriptor = void>
    struct ServiceFactory;

    // Default specialisation: descriptor-less, default-constructs the service.
    template <typename TService>
    struct ServiceFactory<TService, void> {
        static constexpr TService Create(auto&) {
            static_assert(std::is_default_constructible_v<TService>,
                          "Service has no descriptor parameters and is not default-constructible. "
                          "Specialise IOC::ServiceFactory<TService> (or "
                          "IOC::ServiceFactory<TService, TDescriptor>) with a custom Create().");
            return TService();
        }

        static auto CreateUnique(auto& container) {
            if constexpr (std::is_default_constructible_v<TService>) {
                return std::make_unique<TService>();
            } else {
                return std::unique_ptr<TService>(new TService(Create(container)));
            }
        }
    };

    // Templated-service specialisation: resolves each TDescriptor from the
    // container and forwards it to the actual constructor.
    template <template <typename...> class TService, typename... TDescriptors>
    struct ServiceFactory<TService<TDescriptors...>, void> {
        using TActualServiceType =
            typename Util::ReplaceDescriptors<TService<TDescriptors...>>::TResult;

        static constexpr auto Create(auto& container) {
            static_assert(
                std::is_constructible_v<TActualServiceType,
                                        decltype(container.template Resolve<TDescriptors>())...>,
                "Service is not constructible from its resolved dependencies. "
                "Either fix the constructor signature or specialise IOC::ServiceFactory<...>.");
            return TActualServiceType(container.template Resolve<TDescriptors>()...);
        }

        static auto CreateUnique(auto& container) {
            static_assert(
                std::is_constructible_v<TActualServiceType,
                                        decltype(container.template Resolve<TDescriptors>())...>,
                "Service is not constructible from its resolved dependencies (CreateUnique).");
            return std::make_unique<TActualServiceType>(
                container.template Resolve<TDescriptors>()...);
        }
    };

    // Selects the matching ServiceFactory specialisation for a (Service,
    // Descriptor) pair. If the user specialised
    // `ServiceFactory<TService, TDescriptor>` it is picked; otherwise the
    // descriptor-less `ServiceFactory<TService, void>` is used.
    //
    // Detection: the primary `ServiceFactory<TService, TDescriptor>` is only
    // forward-declared, so `sizeof(ServiceFactory<T, D>)` is valid *only* when
    // (a) D == void (the default partial spec is complete), or (b) the user
    // supplied a descriptor-keyed specialisation. Probe via SFINAE and use a
    // descriptor-keyed factory only when D != void.
    namespace detail {

        template <typename TService, typename TDescriptor, typename = void>
        struct HasDescriptorKeyedFactory : std::false_type {};

        template <typename TService, typename TDescriptor>
        struct HasDescriptorKeyedFactory<
            TService,
            TDescriptor,
            std::void_t<decltype(sizeof(ServiceFactory<TService, TDescriptor>))>>
            : std::bool_constant<!std::is_void_v<TDescriptor>> {};

    } // namespace detail

    template <typename TService, typename TDescriptor>
    using ResolveFactory_t =
        std::conditional_t<detail::HasDescriptorKeyedFactory<TService, TDescriptor>::value,
                           ServiceFactory<TService, TDescriptor>,
                           ServiceFactory<TService, void>>;

}
