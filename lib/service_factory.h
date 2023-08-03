#pragma once

#include "util/evaluate_type.h"

namespace IOC {

    template <typename TService>
    struct ServiceFactory;

    template <typename TService>
    struct ServiceFactory {
        static constexpr TService Create(const auto&) {
            static_assert(std::is_default_constructible_v<TService>);
            return TService();
        }
    };

    template <template<typename ...> class TService, typename... TDescriptors>
    struct ServiceFactory<TService<TDescriptors...>> {
        static constexpr auto Create(const auto& container) -> decltype(auto) {
            using TActualServiceType = typename Util::EvaluateType<TService<TDescriptors...>>::Type;

            if constexpr (std::is_constructible_v<TActualServiceType, typename Util::EvaluateType<decltype(container.template Resolve<TDescriptors>())>::Type...>) {
                return TActualServiceType(container.template Resolve<TDescriptors>()...);
            } else {
                return TActualServiceType();
            }
        }
    };

}
