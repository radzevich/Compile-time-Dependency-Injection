#pragma once

#include <type_traits>
#include "util/evaluate_type.h"

namespace IOC {

    template <typename TService>
    struct ServiceFactory;

    template <typename TService>
    struct ServiceFactory {
        static constexpr TService Create(auto&) {
            static_assert(std::is_default_constructible_v<TService>);
            return TService();
        }
    };

    template <template<typename ...> class TService, typename... TDescriptors>
    struct ServiceFactory<TService<TDescriptors...>> {
        static constexpr auto Create(auto& container) {
            using TActualServiceType = typename Util::ReplaceDescriptors<TService<TDescriptors...>>::TResult;

            if constexpr (std::is_constructible_v<TActualServiceType, decltype(container.template Resolve<TDescriptors>())...>) {
                return TActualServiceType(container.template Resolve<TDescriptors>()...);
            } else {
                return TActualServiceType();
            }
        }
    };

}
